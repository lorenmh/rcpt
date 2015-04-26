#!/bin/sh
g++ -o a rcpt_bounds.cpp `pkg-config --cflags --libs opencv`
