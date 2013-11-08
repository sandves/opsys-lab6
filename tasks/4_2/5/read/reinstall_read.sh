#!/bin/bash

rmmod ptr_kernel_read.ko
rm /dev/ptr_kernel_read
make
insmod ptr_kernel_read.ko
mknod /dev/ptr_kernel_read c 244 0
chgrp wheel /dev/ptr_kernel_read
chmod 664 /dev/ptr_kernel_read
