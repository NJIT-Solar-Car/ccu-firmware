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

## Docker stuff

Optionally, you can use docker to do quick build testing, and also doxygen docs regeneration. To build the image (for the first time only), install Docker Desktop (best on windows) or the Docker Engine, restart ur PC, and run the command below in a terminal. The ccu-build name can be replaced with whatever you want.

*Note!* The built image will take up ~800MB to 1GB on your PC, and building will likely take a few minutes too.

```bash
docker build -t ccu-build ./
```

The image runs alpine linux to be lightweight, and comes with Doxygen 1.9.8 and the GNU ARM build tools. To run a container, use:

```bash
docker run -it --rm -v %CD%:/project ccu-build # For windows
docker run -it --rm -v $PWD:/project ccu-build # For linux/mac
```

This will:

- `-it` opens an interactive terminal. Exit any time by typing the command `exit`.
- `--rm` deletes the container when you're done with it.
- `-v` links your current directory (CD or PWD) to the `/project` directory in the container.

Then run `make -j$(nproc)` to build the code into binaries, or `doxygen Doxyfile` to regenerate the docs.

<!-- Links -->
[01]: https://git-scm.com/install/
[02]: ./CHANGELOG.md
[03]: https://www.doxygen.nl/manual/docblocks.html
[04]: ./docs/html/index.html
[05]: https://NJIT-Solar-Car.github.io/ccu-firmware
