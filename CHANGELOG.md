# Changelog for CCU Firmware

Put all software notes/changelog stuff here. Try to format something like below to minimize git conflicts:

```
## <date, or commit hash>

<notes>

### Added

### Changed

### Removed

## <older date/hash>

...
```

This format is adopted from [keepachangelog.com](https://keepachangelog.com).

## 80da62c

### Added

- Added this changelog, and an initial Doxygen doxyfile
- Also added the initial Doxygen generated docs
    - Haven't yet looked at all the generated stuff, but HAL functions for CAN, GPIO, I2C, SPI, and DMA all look fine.

## 2e22da2

### Added

- Created the initial CubeMX configuration. Not complete! It's a temporary configuration that will change once the CCU is finalized.
- Added an initial `can_dictionary.h` file, which will house all the CAN commands, mainly going to/from the CCU and motor controller.
