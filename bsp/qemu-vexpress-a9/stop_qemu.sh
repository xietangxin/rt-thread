#!/bin/bash
pid=`ps -aux | grep "sudo ./qemu-nographic.sh"  | head -1 | awk '{printf $2}'`
#echo $pid
kill -9 $pid
