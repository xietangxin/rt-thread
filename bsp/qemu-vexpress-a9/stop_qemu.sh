#!/bin/bash
for pid in `ps -aux | grep qemu  | awk '{printf $2"\n"}'`
do
    echo  " will kill process $pid"
    sudo kill -9 $pid
done
