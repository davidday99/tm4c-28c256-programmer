# Universal Memory Programmer
This project is dedicated to creating a framework for turning any MCU device
into a memory programmer and viewer. The primary output of this framework is 
a firmware that will achieve this.

## Dependencies

This project uses the GNU ARM toolchain for compilation and debugging. [OpenOCD](https://github.com/openocd-org/openocd.git) is used for debugging the remote target, and [lm4flash](https://github.com/utzig/lm4tools.git) is used to flash the board.
