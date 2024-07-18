#!/bin/bash
./schedule help
./schedule alice 1234 signup
./schedule alice 1234 addtask -n task1 -b 23:59:24 
./schedule alice 1234 deltask 0
./schedule alice 1234 showtask
./schedule alice 1234 addtask -n task2 -b 23:59:24 -r 23:59:00 -e 23:59:59 -p high -c sleep -t relax
./schedule alice 1234 addtask -n task3 -b 2024/9/10-23:23:23 -p low
./schedule alice 1234 addtask -n task4 -b 2024/8/7-22:23:23
./schedule alice 1234 showtask -b
./schedule alice 1234 showtask -e
./schedule alice 1234 showtask -r
./schedule alice 1234 showtask -d
./schedule alice 1234 showtask -d 200
./schedule alice 1234 showtask -p high
./schedule alice 1234 showtask -t relax
./schedule alice 1234 edittask 1 -s fin
./schedule alice 1234 edittask 1 -n TASK2 -b 2024/8/17-23:59:24 -r 2024/8/17-23:59:00 -e 2024/8/17-23:59:59 -p mid -s unfin
./schedule alice 1234 addtask -b 2024/0/7-12:81:21
./schedule alice 1234 cancel