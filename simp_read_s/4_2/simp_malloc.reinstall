#!/bin/bash

rmmod simp_malloc.ko
rm /dev/simp_malloc
make
insmod simp_malloc.ko
mknod /dev/simp_malloc c 245 0
sudo chgrp wheel /dev/simp_malloc
sudo chmod 664 /dev/simp_malloc