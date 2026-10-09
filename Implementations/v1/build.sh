#!/bin/bash

mkdir -p build

gcc src/main.c -o build/compiler --std=c23
