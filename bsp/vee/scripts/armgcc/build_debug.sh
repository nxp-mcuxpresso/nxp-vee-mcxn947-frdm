#!/bin/bash

cmake --preset debug "${@:2}" .
cmake --build --preset debug 2>&1 | tee build_log.txt
