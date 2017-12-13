#!/bin/bash


invalidate=false
cache_dir="$HOME/.local/debs"
dependencies_sums_path="$HOME/.local/dependencies_sums.md5"

cat $dependencies_sums_path
md5sum -c $dependencies_sums_path
if [[ ! $? -eq 0 ]]; then
    echo "Our script has changed"
    invalidate=true
    md5sum $0 > $dependencies_sums_path
fi

# Exit if a command fails
set -e

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
        doxygen graphviz                # doxygen
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
