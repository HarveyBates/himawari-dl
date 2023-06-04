# Himawari-dl
<html>
<img align="right" width=300 border=0;" src="imgs/outfile.png">
</html>

CLI for downloading high-resolution images from the Japanese geostationary
satellite, Himawari.

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

