#!/bin/bash

scp -r Makefile src root@192.168.0.12:/root/
ssh root@192.168.0.12 "make clean; make server; chmod +x server;"