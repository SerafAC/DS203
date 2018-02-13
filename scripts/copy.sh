#!/bin/bash
fail(){
	echo $@ >&2
	exit 1
} 
discname="usb-Vertual_DFU_Disk-0:0"
disc="/dev/disk/by-id/$discname"

grep $discname  ~/.mtoolsrc | {
 read dummy drive rest
 n=$(basename ${1%.*})
 which mdir >/dev/null || fail "mtools are missing"
 [ -z "$drive" ] && fail "missing entry for DSO disc in ~/.mtoolsrc."
 echo "copy $1 to $drive"
 sudo mcopy $1 $drive
 sudo mdir $drive
 while true
 do
  sleep 1 
  [ -b "$disc" ] && sudo mdir $drive | grep $n | egrep " rdy| err" && break
 done
}
