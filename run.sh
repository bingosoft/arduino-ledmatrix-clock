#!/usr/bin/env bash

export PATH="$PATH:$HOME/.platformio/penv/bin/"

platformio run -t upload;

if [ $? -eq 0 ]; then
    platformio device monitor --filter time
fi
