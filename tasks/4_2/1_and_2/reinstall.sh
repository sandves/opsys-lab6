#!/bin/bash

rmmod simp_malloc.ko
rm /dev/simp_malloc
make
insmod simp_malloc.ko
mknod /dev/simp_malloc c 245 0
chgrp wheel /dev/simp_malloc
chmod 664 /dev/simp_malloc
