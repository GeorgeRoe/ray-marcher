meson setup --wipe build --buildtype=release -Dcpp_args='-O3'
meson compile -C build
date
time ./build/ray-marcher
