#!/usr/bin/env bash

flags='-Llib -Wl,-rpath=./lib -lsfml-graphics -lsfml-window -lsfml-system'
name='fractal_renderer'

compile() {
        mkdir -p objects

        local file
        for file in src/*.cpp; do
                echo -e "\e[1;32m * Compiling: $file\e[0m"
                bname=${file##*/}
                bname=${bname%.cpp}
                # -Wpedantic
                g++ -c "$file" -o "objects/$bname.o" -Iinclude -Wall -Wextra -O3
        done
}

link() {
        echo -e '\e[1;32m * Linking\e[0m'
        g++ objects/*.o -o $name -std=c++23 $flags
}

clean() {
        rm -r objects
}

build() {
        compile
        link
}

main() {
        local time=$(date +%s)

        local arg
        for arg in "$@"; do
                $arg
        done

        local dtime=$(($(date +%s)-$time))
        echo -e "Done in $dtime seconds!"
}

main "$@"
