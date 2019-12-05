#!/bin/bash

config-pin 9.12 in+

g++ led.cpp -o led.cgi -lcgicc

echo "Program has been built"
