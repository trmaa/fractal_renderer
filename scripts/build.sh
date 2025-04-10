#!/bin/bash

g++ src/**.cpp -o ray_marcher.app -Iinclude/ -Llib/ -Wl,-rpath=./lib -lsfml-graphics -lsfml-window -lsfml-system

./ray_marcher.app
