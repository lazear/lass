#!/bin/bash
make
./lass new.s
ndisasm -b 32 output
