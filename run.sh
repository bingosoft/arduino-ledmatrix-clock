#!/usr/bin/env bash

export PATH="$PATH:$HOME/.platformio/penv/bin/"

platformio run -t upload;
platformio device monitor --filter time
