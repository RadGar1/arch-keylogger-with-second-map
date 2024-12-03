# arch-keylogger
This project implements a Linux kernel module that logs keystrokes by intercepting keyboard events at the kernel level. It captures keypress events and logs them into a buffer, which can be read from user space via the Debugfs filesystem. The module supports multiple logging formats: US keyboard characters, hexadecimal keycodes, and decimal keycodes.
The objective of this project is to gain a working knowledge of Linux kernel module hacking/programming. The module was built in an Arch Linux virtual machine using VMWare Workstation and is based on [spy](https://github.com/jarun/spy), another kernel-level keylogger.

## Build/Usage Instructions

### 1. Clone the Repository and Build
```
git clone https://github.com/egrassia11/arch-keylogger.git
cd arch-keylogger
make
```

### 1.5. Linux Kernel Headers are Required to Compile; Install if Necessary
#### Arch Linux
`sudo pacman -Syu linux-headers`

#### Ubuntu/Debian
`sudo apt install linux-headers`

### 2. Hook the Module into the Kernel (Activate the Keylogger)
#### U.S. Keys:

`sudo insmod innocent_module.ko`

#### Hexadecimal:

`sudo insmod innocent_module.ko mode=1`

#### Decimal:

`sudo insmod innocent_module.ko mode=2`

### 3. View the Keylog
`sudo cat /sys/kernel/debug/secret/keys`

### 4. Unhook the Module and Clear the Logs
`sudo rmmod innocent_module`

### 5. Clean Up (Remove Kernel Module Object Files)
`make clean`

## Project Timeline

| Task                         | Description                                                                  | Deadline       |
|------------------------------|-----------------------------------------------------------------------------|----------------|
| Week 1: Research & Planning  | Set up environment, review kernel module basics, understand keylogging concepts | End of Week 1 |
| Week 2: Core Implementation  | Develop keylogging functionality and basic logging to a file or buffer          | End of Week 2 |
| Week 3: Stealth & Error Handling (Possibly) | Implement stealth features and error handling mechanisms         | End of Week 3 |
| Week 4: Testing & Documentation | Conduct testing, debugging, optimize code, write documentation, finalize project | End of Week 4 |

## TODO
- Add buffer management logic ✅
- Define a file operations structure ✅
- Implement a function to read logged keys from a debugfs file ✅
- Implement a keycode to string conversion function ✅
- Add logic for the core keyboard event handler ✅
- Add module initialization and cleanup functions ✅
- Test basic functionality ✅
- Optimize code and documentation ✅
- Write build/usage instructions ✅
- Add a feature to write logged keys to a second debugfs file with only alphanumeric characters using a simplified keymap
- Implement a stealth mode to hide the keylogger from process viewers like top/ps?

