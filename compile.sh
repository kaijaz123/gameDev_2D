#! /bin/bash

make clean
rm -rf bin*
rm -rf Makefile gui.make
./premake5.exe gmake2
make
./bin/x86_64/gui
