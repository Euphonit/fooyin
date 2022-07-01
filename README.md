# Fooyin
Fooyin is a customisable music player for linux. It has not yet reached a stable release so bugs and breaking changes should be expected.

## Features
* Fully customisable layout
* Play local music library
* Filter and search library

## Planned features
* Full playlist support
* Tag editing
* FFmpeg backend
* Visualisations
* Lyrics support - embedded & lrc + enhanced lrc
* Networking (Last.fm, Discogs, lyric querying)

## Dependencies
Fooyin is built with and requires:
* Qt6
* Taglib (1.12)
* Libmpv (mpv)

## Setup
After you have installed the required dependencies, use your terminal to issue the following commands:

```
git clone git@github.com:ludouzi/fooyin.git
cd fooyin
mkdir build
cd build
cmake ..
make
sudo make install
```
