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

## 3a5c5f0

### Added

- Added the skeleton of a function that runs A2D\_BATCH\_READ1. It, and all other functions in this family, will have the following characteristics:
    - In main.c, there are two Task Handles called `xLeftMotorWaiting` and `xRightMotorWaiting`. When a task calls the function `A2D_BATCH_READ1_Func()`, the function will organize and send a message over the CAN bus to the corresponding controller. Then, it will call `xTaskNotifyWait`, which will *block* the parent task's execution entirely. The task will restart after a CAN message matching the description pops into the FIFO and the ISR callback discovers it. Waking the task from ISR uses the vTaskNotifyGiveFromISR function. At this point, the re-woken function will process the buffer as it needs to, and return `HAL_OK`.
    - In the case that the function is blocked for longer than `CAN_MOTOR_TIMEOUT` milliseconds, it will return `HAL_TIMEOUT`.
- So far, the ISR portion and much of the `A2D_BATCH_READ1_Func` body is yet to be done. In the body there's a whole if block for the left motor alone; much of that common code can be extracted from the block, and then the extra code for the right motor controller can be added.
- I believe much of this is just boilerplate; the real code is just making transmitting the CAN message, and parsing the controller's response. Once this function is complete it should be easy to port over to the other functions.

## 98df7c9

### Added

- Kelly KBL controller datasheet
- Added MotorControllerInfo and MotorPacketInfo structs, which will hold static information about the motor controllers, and live status info about the motors/motor controllers respectively.
- Yet to add functions which will communicate with the controllers over CAN bus
- Once functions are written, they can most likely be called periodically using a timer, and have the outputs fill up a buffer which the LoRA code will pop from.

## 28696b2

### Added

- As of commit 54235db, JP added the CAN\_Dictionary, which holds all the motor controller and MPPT charge controller addresses and IDs.

### Changed

- Moved Doxygen output to docs folder. This will allow a gh pages deployment. Possibly implement the doxygen running thing with gh actions in the future.
- Moved the CAN\_Dictionary.h file to `Core/Src/CAN/Inc`. Other CAN .h files can go in the same place, and .c files can go in `Core/Src/CAN/`.

## 80da62c

### Added

- Added this changelog, and an initial Doxygen doxyfile
- Also added the initial Doxygen generated docs
    - Haven't yet looked at all the generated stuff, but HAL functions for CAN, GPIO, I2C, SPI, and DMA all look fine.

## 2e22da2

### Added

- Created the initial CubeMX configuration. Not complete! It's a temporary configuration that will change once the CCU is finalized.
- Added an initial `can_dictionary.h` file, which will house all the CAN commands, mainly going to/from the CCU and motor controller.
