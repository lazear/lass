#!/bin/bash
make
./asm new.s
ndisasm output
