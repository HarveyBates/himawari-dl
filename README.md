# Himawari-dl <img align="right" width=250 border=0;" src="imgs/outfile.png">

CLI for downloading high-resolution images from the Japanese geostationary
satellite, Himawari.

This program multi-threading to download image segments and
combines them to produce a final high-resolution image.

It is written in C89 and was a weekend project.

## Instructions for use

### Requirements

The only requirement is [libpng](http://www.libpng.org/pub/png/libpng.html).
Which you probably already have installed. If not you can install it using:

```bash
# Linux
sudo apt-get install libpng-dev
# Mac (Homebrew)
brew install libpng
```

### Install

```bash
git clone https://github.com/HarveyBates/himawari-dl
cd himawari-dl
mkdir build
cd build
cmake ..
# Executable is in bin/
```

