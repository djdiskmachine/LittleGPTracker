# Supported WAV formats

Little Piggy Tracker loads samples as **16-bit PCM WAV** (`WAVE_FORMAT_PCM`,
compression code `1`). That is the format used by the classic tracker
sample pools and the one you should export to.

## What works

| Format                          | Compression code | Result                                  |
|---------------------------------|------------------|-----------------------------------------|
| PCM 16-bit, mono                | `1`              | Full support                            |
| PCM 16-bit, stereo              | `1`              | Full support                            |
| PCM 16-bit with a `JUNK` chunk  | `1`              | Supported (the `JUNK` chunk is skipped) |
| PCM 8-bit, mono                 | `1`              | Loads (expanded to 16-bit on load)      |
| PCM 8-bit, stereo               | `1`              | Loads but the right channel is garbage  |

## What is rejected

| Format                  | Compression code | Why                                             |
|-------------------------|------------------|-------------------------------------------------|
| IEEE float (32-bit)     | `3`              | `Unsupported compression` — not PCM             |
| ADPCM                   | `2`              | `Unsupported compression`                       |
| µ-law / A-law           | `6` / `7`        | `Unsupported compression`                       |
| WAVE_FORMAT_EXTENSIBLE  | `0xFFFE`         | `Unsupported compression` — even if the sub-format is PCM |
| PCM 24-bit / 32-bit int | `1`              | `Only 8/16 bit supported`                       |

## Common gotchas

- **WAVE_FORMAT_EXTENSIBLE** is what Windows exports when the "16-bit PCM"
  option is picked from a save dialog. It looks like a normal 16-bit WAV
  but carries compression code `0xFFFE` in the header, so the tracker
  refuses it. Re-encode to plain PCM (`compression code 1`) to fix.
- **32-bit float** is the default export format of several DAWs and of
  Audacity when "WAV (Float)" is selected. Export as WAV (PCM) 16-bit
  instead.
- **24-bit** is common from audio interfaces. Not supported — convert.
- The `fmt ` chunk must declare exactly 8 or 16 bits per sample.

## Converting a WAV

With [sox](http://sox.sourceforge.net):

```sh
sox in.wav out.wav           # re-encodes to the input format
sox in.wav -b 16 out.wav     # force 16-bit (default PCM)
```

With Audacity: *File → Export Audio → File type: WAV (Microsoft) → Encoding:
Signed 16-bit PCM*.

## Where the parsing lives

The checks live in `WavFile::Open()`
(`sources/Application/Instruments/WavFile.cpp`):

- `comp != 1` → `"Unsupported compression"`
- `bitPerSample` not in `{8, 16}` → `"Only 8/16 bit supported"`
