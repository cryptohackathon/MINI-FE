#!/bin/bash

read -p "Do you want to additionally install the version of our library and demo for the Stanford's pbc library? You need to have pbc library already installed in your system. [0/1] " W

if [ $W == 1 ]; then
read -p "Insert the path of the header files of your Stanford pbc's installation " P
make PBC_INCLUDE_PATH=$P all
else
make onlycifer
fi
echo "Installation terminated. Copy the file libminife.so in one of your system paths and run the command: sudo ldconfig"
