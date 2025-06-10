#! /bin/bash

meson setup --wipe build --buildtype=release -Dcpp_args='-O3'
meson compile -C build
time ./build/ray-marcher
