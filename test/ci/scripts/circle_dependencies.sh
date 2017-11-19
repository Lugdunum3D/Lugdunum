#!/bin/bash

# Exit if a command fails
set -e

invalidate=false
cache_dir="$HOME/.local/debs"
date_cache=0

if [[ -d "$cache_dir" ]]; then
    echo "$cache_dir is present"

    echo -ne "$cache_dir modification time: "
    date -r "$cache_dir"
    echo -ne "$0 modification time: "
    date --date="$(git log -1 --format="%ai" -- $0)"
    date_cache=$(date -r "$cache_dir" +%s)
else
    echo "$cache_dir is missing, creating cache"
fi

date_script=$(date --date="$(git log -1 --format="%ai" -- $0)" +%s)

echo "date_script: $date_script, date_cache: $date_cache"
if [[ $date_script -gt $date_cache ]]; then
    echo "$0 is newer than $cache_dir, invalidating cache"
    invalidate=true
else
    echo "$0 is older than $cache_dir, cache is valid"
fi

if [[ ! -d "$cache_dir" || "$invalidate" = true ]]; then
    sudo add-apt-repository ppa:george-edison55/cmake-3.x -y
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo apt-get update

    rm -rf "$cache_dir"
    mkdir "$cache_dir"
    cd "$cache_dir"

    packages=(
        clang-3.8 cmake gcc-6 g++-6     # compilation
        ninja-build                     # ninja
        doxygen graphviz                # doxigen
    )

    for package in ${packages[*]}; do
        sudo aptitude --download-only install $package -y
    done

    sudo cp /var/cache/apt/archives/*.deb .
fi

sudo dpkg -i $cache_dir/*.deb

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 60 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-3.8
sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100
