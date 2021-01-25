#!/bin/bash
ldconfig -p |grep libcifer 1>/dev/null
if [ $? == 1 ]; then
	echo "libcifer is required to install MINI-FE package and was not found."
	exit 1
fi
ldconfig -p |grep libgmp 1>/dev/null
if [ $? == 1 ]; then
	echo "libgmp is required to install MINI-FE package and was not found."
	exit 1
fi

read -p "Do you want to additionally install the version of our library and demo for the Stanford's pbc library? You need to have pbc library already installed in your system. [y/n] " W

if [ $W == "y" ]; then
ldconfig -p |grep libpbc 1>/dev/null
if [ $? == 1 ]; then
	echo "libpibc was not found."
	exit 1
fi
read -p "Insert the path of the header files of your Stanford pbc's installation " P
make PBC_INCLUDE_PATH=$P all
else
make onlycifer
fi
echo "Installation terminated. Copy the file libminife.so in one of your system paths and run the command: sudo ldconfig"
