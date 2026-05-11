# Description

Thank you for contributing to Little Piggy Tracker!
For fixes, please include a summary of the changes and any related issue.
For features, please include a detailed description of the new feature and how it works.

All PR:s are run through line-by-line clang-format and builds for all supported platforms.
clang-format can be applied locally on commit by installing pre-commit from repo root:
    cp pre-commit .git/hooks/

In section below, delete any option that isn't relevant.
## Type of change
- Fixes #(issue number)
- Feature

- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)

# How Has This Been Tested?

Please describe the tests procedure by which you verified your changes.
Provide instructions on how to reproduce.

**Test Configuration**:
* Hardware:
* Test steps:

# Checklist:

- [ ] I have performed a self-review of my code
- [ ] I have commented particularly in hard-to-understand areas
- [ ] I have updated CHANGELOG
- [ ] I have updated docs/wiki/What-is-LittlePiggyTracker.md reflecting my changes
- [ ] I have version bumped in `sources/Application/Model/Project.h`
- [ ] My changes generate no new warnings (build without your change then apply your change to check this)
