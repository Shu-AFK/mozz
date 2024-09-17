# MOZZ

**MOZZ** is a DLL designed for shellcode injection and downloading using **Process Hallowing**.
It provides a stealthy way to inject and execute shellcode within a target process without storing the shellcode on disk
while *hopefully* evading detection from conventional security solutions when finished.

## Features
- **Process Hollowing**: Creates a new process in a suspended state, replaces its memory with shellcode, and resumes execution to avoid detection.
- **Flexible Payload Delivery**: Allows dynamic loading of shellcode from a remote source.
- **Minimal Footprint**: Operates without leaving traces on the filesystem, enhancing stealth.

## Disclaimer
Mozz is intended for educational and research purposes only. It should not be used for 
unauthorized access or malicious activities. Use responsibly and in compliance with all 
applicable laws and regulations.