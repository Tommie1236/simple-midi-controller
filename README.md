# Simple MIDI Controller

### A simple 32-button MIDI controller with bank support, built around the RP2040 microcontroller (e.g., RP2040-Zero, but other RP2040 boards should work with appropriate pin changes). Designed for compatibility with any software that accepts USB MIDI input.
## Features

  32 configurable buttons
  Multiple banks for expanded control
  USB MIDI device class (works out-of-the-box with most DAWs and lighting control software)
  Example use: MyDMX, generic macropad for other MIDI-compatible software

## Hardware

  Primary target: RP2040-Zero
  Any RP2040-based board can be used (adjust pin mappings as needed)
  Wiring and assembly instructions: TODO!!

## Software Dependencies

  Raspberry Pi Pico SDK
  TinyUSB

## Building and Flashing

  Clone this repository and its dependencies.
  Build the firmware using the included build script in the root directory. 
  Optionally use the -l flag to automaticly use picotool to load it to the device.
  Or reboot into bootsel mode and drag the compiled binary(.uf2) onto the usb mass storage device. 

## Usage

Once flashed, the controller will appear as a standard USB MIDI device. It should work with any MIDI-compatible software. Currently tested with MyDMX, but suitable for general MIDI macro control.
Pin Configuration

  Default pin mappings are defined in the code for RP2040-Zero.
  If you use a different RP2040 board, update the pin definitions as needed in the source files.

## Roadmap

  - [ ] implement segment display
  - [ ] add banking support
  - [ ] add a menu for configuration?
  - [ ] Add full hardware assembly and wiring instructions
  
## Ai useage (hackclub Summer of Making)
I've used ai in this project. for example to create a base for this readme but i have also used some chatbots with high level debugging if i got stuck.
I've also had copilot enable in vscode when i started the project. but i've since switched to neovim and didn't add any ai or lsp yet. so mostly just high level debugging and writing text. (I'm very bad at this as you can probably already see.)
