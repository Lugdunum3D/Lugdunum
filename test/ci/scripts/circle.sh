#!/bin/bash

function build_lugdunum() {
    cd ~/Lugdunum

    mkdir build && cd build
    (cmake .. -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DLUG_THIRDPARTY_DIR=$HOME/.local/thirdparty) || return 1
    (make all test && sudo make install) || return 1

    return 0
}

function build_samples() {
    cd ~/Lugdunum/samples

    mkdir build && cd build
    (cmake .. -DBUILD_SHADERS=false) || return 1
    (make all) || return 1

    return 0
}

case $CIRCLE_NODE_INDEX in
    0)
        export CXX=clang++
        build_lugdunum # && build_samples
    ;;

    1)
        export CXX=g++
        build_lugdunum # && build_samples
    ;;
esac

