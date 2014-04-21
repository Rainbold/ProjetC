#!/bin/bash

kill -9 $(ps aux | grep $1 | tr -s ' ' | cut -f2 -d' ' | cut -f1 -d$'\n')
