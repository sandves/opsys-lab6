#!/bin/bash

rmmod ptr_kernel_write.ko
rm /dev/ptr_kernel_write
make
insmod ptr_kernel_write.ko
mknod /dev/ptr_kernel_write c 245 0
sudo chgrp wheel /dev/ptr_kernel_write
sudo chmod 664 /dev/ptr_kernel_write