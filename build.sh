#!/bin/bash
mkdir -p build
cd build # kybr/br/build/
cmake ..
cmake --build .
cmake --build . --target install
cd .. # kybr/br/
cd test # kybr/br/test/
mkdir -p build
cd build # kybr/br/test/build/
cmake ..
cmake --build .
cmake --build . --target install
cd .. # kybr/br/test/
ls -la
./000-create-destroy
./001-broadcaster
./002-receiver
cd .. # kybr/br/
ls -la 
ls -la test/
