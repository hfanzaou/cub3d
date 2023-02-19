#!/bin/bash

while [ 1 ]
do
    leaks cub3D | grep "leaked bytes"
    sleep 1
done