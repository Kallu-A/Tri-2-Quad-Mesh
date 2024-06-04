#!/bin/bash

# This script is used to compile the project.
cmake -B build -DCMAKE_BUILD_TYPE=Release
cd build
make -j
cd ..