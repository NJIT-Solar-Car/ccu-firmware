# Solar Car CCU Firmware

This repo will hold firmware for the STM32F413-based central control unit for NJIT's Solar Car. Will add details as they come.

## Getting Started

[Install Git][01] for your operating system. Then clone the repo in a convenient location, using the following:

```bash
git clone https://github.com/NJIT-Solar-Car/ccu-firmware.git
```

You will also need to have STM32CubeIDE installed, and maybe even CubeMX if you intend to use any of the extra GPIOs. From here you can make changes and commit as you wish.

## Best Practices

Strong projects must always have good documentation, for ease of maintenance and development. As of right now, the two best practices are:

1. Whenever you make and commit a change (large enough change; one line edits may be omitted), be sure to add a commit that *documents that change in the [changelog!][02]* This makes reverting/resetting/cherry picking commits easier later on.
2. Whenever declaring a new file, public function, constant, or struct, try to use [Doxygen's comment syntax][03] along with them. Thus, when the generated docs are updated, it'll be easy to view not just the HAL functions, constants, and structs, but also that of our own.

On that last point, you can view the generated docs by opening [./docs/html/index.html][04] in a browser (especially if you regenerate on-the-fly), and the committed version is also available at [njit-solar-car.github.io/ccu-firmware][05]. This documents the entirety of the HAL (and eventually FreeRTOS, still have to figure that out) as it exists *within this repo*, as well as our own code, in the CCU\_Firmware group; best to look to there first when stuck.

<!-- Links -->
[01]: https://git-scm.com/install/
[02]: ./CHANGELOG.md
[03]: https://www.doxygen.nl/manual/docblocks.html
[04]: ./docs/html/index.html
[05]: https://NJIT-Solar-Car.github.io/ccu-firmware
