#!/usr/bin/env bash

platformio run -t upload;
platformio device monitor --filter time
