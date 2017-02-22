#!/bin/bash

# Install dependencies

if [[ ! -d "$HOME/.local/debs" ]]; then
  sudo add-apt-repository ppa:george-edison55/cmake-3.x -y
  sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
  sudo apt-get update

  mkdir -p "$HOME/.local/debs"
  cd "$HOME/.local/debs"

  for package in clang-3.8 cmake gcc-6 g++-6; do
    sudo aptitude --download-only install $package -y
  done

  sudo cp /var/cache/apt/archives/*.deb .
fi

sudo dpkg -i $HOME/.local/debs/*.deb

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 60 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-3.8
sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100

# Build gmock

cd "$HOME/Lugdunum/lib/googletest/googlemock"
mkdir build
cd build
cmake ..
cmake --build .
