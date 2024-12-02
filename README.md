# arch-keylogger
This project implements a Linux kernel module that logs keystrokes by intercepting keyboard events at the kernel level. It captures keypress events and logs them into a buffer, which can be read from user space via the Debugfs filesystem. The module supports multiple logging formats: US keyboard characters, hexadecimal keycodes, and decimal keycodes.
The objective of this project is to gain a working knowledge of Linux kernel module hacking/programming. The module was built in an Arch Linux virtual machine using VMWare Workstation and is based on [spy](https://github.com/jarun/spy), another kernel-level keylogger.

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
- Add logic for the core keyboard event handler
- Add module initialization and cleanup functions
- Test basic functionality
- Write build/usage instructions
- Implement a stealth mode to hide the keylogger from process-viewers like top/ps?

