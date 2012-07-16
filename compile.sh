#!/bin/bash

COPTS=-Wall
OUTFILE=game-of-life

gcc src/*.c $COPTS -o $OUTFILE
