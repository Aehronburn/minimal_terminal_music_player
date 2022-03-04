# Minimal terminal mp3 player

Terminal music player written in C++ using SFML Audio and Ncurses

![alt text](assets/example.png "UI of the program")

## Description
This utility was intended to be a lightweight music player that plays in a terminal, as an alternative to resourse heavy GUI apps found on Windows, like Groove for example.
It is written in C++ using only the standard library aside from the dependencies mentioned above.
Features supported are: 
* play/pause
* jump to
* autoplay
* loop
* seek

Extensions supported are: .ogg, .flac and .wav.

## Dependencies
* [SFML/Audio](https://www.sfml-dev.org/) for playing music files
* [Ncurses](https://invisible-island.net/ncurses/) for creating the TUI

## Usage
Run the program from within a terminal, passing the directory where the music files are stored as the argument.
Press:
* <kbd>↑</kbd> and <kbd>↓</kbd> to navigate through the playlist
* <kbd>↲</kbd> to play the selected song
* <kbd>←</kbd> and <kbd>→</kbd> to seek -5 and +5 seconds respectively
* <kbd>p</kbd> to toggle play/pause
* <kbd>r</kbd> to toggle loop on/off
* <kbd>q</kbd> to close(also <kbd>ctrl+c</kbd> or just closing the window works)

You can resize the terminal as you wish, the UI will adapt

## Architecture
* The *main* process is tasked with displaying the UI and handling the input.
* The *player* class manages to load, play, pause, seek, loop and get che playing offset of a track.
* The *trackList* class is used to retrieve the list of music files of a given directory.

The main process launches a two threads:
1. For printing the current updated playing offset of the song every second, without interrupting the main program loop which is waiting for input
2. For checking if the current song has finished playing every two seconds, and skipping to the next track in the list in that case.

## How to build
1. Download Ncurses(PDcurses for Windows) and build it **statically**.
2. Copy curses.h and panel.h from the Ncurses folder and paste them into **/include**
3. Copy the just built libncurses.a(pdcurses.lib on Windows) into **/external**. 
4. Download SFML 2.5.1 32bit
4. Copy the extracted folder into **/external**

After the steps above are completed, configure cmake to generate for a 32bit architecture(-A flag), then build.
For example, on a Windows build:
```shell
mkdir build && cd build
cmake -A Win32 ..
cmake --build . --config Release
```

If you wish to hardcode the path to the music directory, just modify the variable ```path``` in src/main.cpp main function.

## Issues
Listening with headphones and then unplugging them will require a restart of the program.

