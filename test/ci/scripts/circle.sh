#!/bin/bash

function build_samples() {
    cd ~/Lugdunum

    for sample in `find samples -mindepth 1 -maxdepth 1 ! -name "compiler" -type d`
    do
        echo "Building $sample"
        cd ~/Lugdunum/$sample
        mkdir cibuild && cd cibuild
        (cmake .. -DBUILD_SHADERS=false && make all) || return 1
    done

    return 0
}

case $CIRCLE_NODE_INDEX in
    0)
        export CXX=clang++
        mkdir build && cd build
        cmake .. -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DLUG_THIRDPARTY_DIR=$HOME/.local/thirdparty
        make all test && sudo make install # && build_samples
    ;;

    1)
        export CXX=g++
        mkdir build && cd build
        cmake .. -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DLUG_THIRDPARTY_DIR=$HOME/.local/thirdparty
        make all test && sudo make install # && build_samples
    ;;
esac

