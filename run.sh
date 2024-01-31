#!/usr/bin/env bash

export PATH="$PATH:/System/Volumes/Data/Users/bingo/.platformio/penv/bin/"

platformio run -t upload;
platformio device monitor --filter time
