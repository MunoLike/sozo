#!/bin/bash


wait_sec=$1
nohup bash -c "sleep ${wait_sec};echo ${wait_sec}"