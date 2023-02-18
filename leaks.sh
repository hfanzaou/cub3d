#!/bin/bash

while [ 1 ]
do
    leaks cub3d | grep "leaked bytes"
    sleep 1
done