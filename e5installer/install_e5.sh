#!/bin/sh
##****************************************************************************
##
## Copyright (C) EDAN Instruments, INC.  2013, 1014, 2015
## All rights reserved.
##
## Project: E5
## Date:    2014-5-10
## Coder:   Xia Aijun
## Description: E5 system install script
##
## Modify History:
##    2014-5-10   Xiaaijun   Create.
##
##****************************************************************************

echo "----->partition the disk"
node=$1
sys_part=${node}$2 
echo sfdisk --force ${node} < e5-partition-info.sfdisk
sfdisk --force ${node} < e5-partition-info.sfdisk

echo "----->Format filesystem partions"
umount ${sys_part}
mkfs.ext3 ${sys_part}

echo "----->Install bootloader(grub2)"
mount ${sys_part} /mnt
grub-install --no-floppy --force --root-directory=/mnt ${node}
cp -rf boot /mnt
