# arch-keylogger
This project implements a Linux kernel module that logs keystrokes by intercepting keyboard events at the kernel level. It captures keypress events and logs them into a buffer, which can be read from user space via the Debugfs filesystem. The module supports multiple logging formats: US keyboard characters, hexadecimal keycodes, and decimal keycodes.
The objective of this project is to gain a working knowledge of Linux kernel module hacking/programming. The module was built in an Arch Linux virtual machine using VMWare Workstation and is based on [spy](https://github.com/jarun/spy), another kernel-level keylogger.



## TODO
- Add buffer management logic
- Define a file operations structure
- Implement a function to read logged keys from a debugfs file
- Implement a keycode to string conversion function
- Add logic for the core keyboard event handler
- More todo items TBD...
- Test basic functionality
- Write build/usage instructions
