#!/bin/bash
sudo route add 192.168.17.240 eth2
sudo chmod 666 /dev/video0
export ROS_MASTER_URI=http://192.168.1.2:11311
