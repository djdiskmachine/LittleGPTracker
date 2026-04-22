# Contributing

Thank you for considering contributing to this project!

## Branch & Merge Strategy

- **master** is the integration branch. It may contain unstable/experimental features.
- **Pull Requests** should target `master`
- **Releases** follow a release candidate workflow:
  - Release candidates: `1.2.0-bacon0`, `1.2.0-bacon2`, etc.
  - Stable releases: `v1.2.0` (after testing period)
- **Users should only use tagged releases**, build from master at your own peril, may contain half-done pork

## Pull Request Requirements

Before submitting a PR, ensure:

- [ ] **Version bumped** in `sources/Application/Model/Project.h`:
  - Bump BUILD_COUNT (`#define BUILD_COUNT "3-bacon3"`) for the most part
  
- [ ] **Documentation updated**:
  - Update `docs/wiki/What-is-LittlePiggyTracker.md` reflecting your changes
  - Update CHANGELOG
  
- [ ] **Clean commit history**:
  - As changes are merged via squash commit, please provide a clear and descriptive commit message

- [ ] **Testing**:
  - Describe how you tested your changes in the PR description
  - Include relevant test results

## Merge Process

Maintainer will review and merge approved PRs using **squash and merge**, combining your commits with a detailed merge message.

## Questions?

Open an issue or reach out to maintainers.
