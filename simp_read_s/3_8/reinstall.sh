#!/bin/bash

rmmod simp_rw.ko
rm /dev/simp_rw
make
insmod simp_rw.ko
mknod /dev/simp_rw c 245 0
sudo chgrp wheel /dev/simp_rw
sudo chmod 664 /dev/simp_rw
