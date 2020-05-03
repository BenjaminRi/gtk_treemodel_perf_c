#!/bin/bash

gcc `pkg-config --cflags gtk+-3.0` -o treemodel_perf treemodel_perf.c `pkg-config --libs gtk+-3.0`

