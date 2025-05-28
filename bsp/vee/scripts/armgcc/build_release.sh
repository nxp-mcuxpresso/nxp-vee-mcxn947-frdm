#!/bin/bash

cmake --preset release "${@:2}" .
cmake --build --preset release 2>&1 | tee build_log.txt
