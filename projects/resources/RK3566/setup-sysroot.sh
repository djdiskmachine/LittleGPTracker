#!/bin/bash
#
# Assemble the cross-compilation sysroot used to build the RK3566 port.
#
# The port links against the *target's own* SDL2 rather than a bundled copy, so
# the sysroot only needs two things:
#
#   1. SDL2 headers, at the version the device actually ships, and
#   2. the device's libSDL2.so for the linker to resolve symbols against.
#
# Nothing else is copied: the C library comes from the Arm GNU Toolchain, whose
# glibc must match (or be older than) the glibc on the device.
#
# Usage:
#   ./setup-sysroot.sh [DEST] [DEVICE]
#
#   DEST    where to build the sysroot   (default: ../sysroot)
#   DEVICE  ssh target of the device     (default: root@RK3566.lan)
#
# Extra ssh/scp options can be passed through SSH_OPTS, which is useful when the
# build host carries a stale host key or a host specific ssh_config:
#
#   SSH_OPTS="-F /dev/null -o StrictHostKeyChecking=no" ./setup-sysroot.sh
#
# Example:
#   ./setup-sysroot.sh /opt/rocknix-sysroot root@192.168.1.136
#
# Then build with:
#   make PLATFORM=RK3566 RK3566_SYSROOT=/opt/rocknix-sysroot \
#        RK3566_TOOLCHAIN=/opt/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu
#
set -euo pipefail

DEST="${1:-$(cd "$(dirname "$0")/../.." && pwd)/sysroot}"
DEVICE="${2:-root@RK3566.lan}"
SSH_OPTS="${SSH_OPTS:-}"

# SDL2 version to fetch headers for.  ROCKNIX 20240815 ships 2.30.5; using the
# matching headers avoids compiling against API that the device does not have.
SDL_VERSION="${SDL_VERSION:-2.30.5}"

mkdir -p "$DEST/usr/include" "$DEST/usr/lib" "$DEST/dl"

echo "==> sysroot:  $DEST"
echo "==> device:   $DEVICE"

# ---------------------------------------------------------------- SDL2 headers
if [ ! -f "$DEST/usr/include/SDL2/SDL.h" ]; then
    echo "==> fetching SDL2 $SDL_VERSION headers"
    curl -fsSL -o "$DEST/dl/SDL2-$SDL_VERSION.tar.gz" \
        "https://libsdl.org/release/SDL2-$SDL_VERSION.tar.gz"
    tar xzf "$DEST/dl/SDL2-$SDL_VERSION.tar.gz" -C "$DEST/dl"
    cp -r "$DEST/dl/SDL2-$SDL_VERSION/include" "$DEST/usr/include/SDL2"

    # The tracker mixes <SDL2/SDL.h> and legacy <SDL/SDL.h> includes, so the
    # parent directory has to be on the include path and the two names must
    # both resolve.
    ln -sfn SDL2 "$DEST/usr/include/SDL"
else
    echo "==> SDL2 headers already present, skipping"
fi

# --------------------------------------------------- target's libSDL2 (linker)
if [ ! -f "$DEST/usr/lib/libSDL2-2.0.so.0" ]; then
    echo "==> copying libSDL2 from $DEVICE"
    # Resolve the real file behind the symlink, wherever the distro put it.
    REMOTE_LIB=$(ssh $SSH_OPTS "$DEVICE" 'readlink -f /usr/lib/libSDL2-2.0.so.0 || readlink -f /usr/lib/libSDL2.so.0')
    scp $SSH_OPTS "$DEVICE:$REMOTE_LIB" "$DEST/usr/lib/"
    ln -sfn "$(basename "$REMOTE_LIB")" "$DEST/usr/lib/libSDL2.so"
    ln -sfn "$(basename "$REMOTE_LIB")" "$DEST/usr/lib/libSDL2-2.0.so.0"
else
    echo "==> libSDL2 already present, skipping"
fi

echo
echo "sysroot ready:"
ls -l "$DEST/usr/lib/libSDL2"*
echo
echo "Check that the device glibc is not newer than the toolchain's:"
echo "  device:    ssh $DEVICE 'ldd --version | head -1'"
echo "  toolchain: \$TOOLCHAIN/bin/aarch64-none-linux-gnu-gcc -print-sysroot"
