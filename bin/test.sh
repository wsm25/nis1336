#!/bin/bash
./schedule help
./schedule alice 1234 signup
./schedule alice 1234 addtask -n task1 -b 23:59:24 
./schedule alice 1234 deltask 0
./schedule alice 1234 showtask