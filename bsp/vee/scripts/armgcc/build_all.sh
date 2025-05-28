#!/bin/sh

cmake --preset debug "${@:2}" .
cmake --build --preset debug

cmake --preset release "${@:2}" .
cmake --build --preset release
