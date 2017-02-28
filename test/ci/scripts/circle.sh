#!/bin/bash

case $CIRCLE_NODE_INDEX in
    0)
        export CXX=clang++
        mkdir install
        mkdir build && cd build
        cmake ../ -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DCMAKE_INSTALL_PREFIX=../install
        make all install test
    ;;

    1)
        export CXX=g++
        mkdir build && cd build
        cmake ../ -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DCMAKE_INSTALL_PREFIX=../install
        make all install test
    ;;
esac

