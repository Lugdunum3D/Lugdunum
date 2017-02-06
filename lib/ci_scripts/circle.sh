#!/bin/bash

case $CIRCLE_NODE_INDEX in
  0)  export CXX=clang++
      mkdir build && cd build
      cmake ../ -DENABLE_TEST=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS
      make all test
      ;;
  1)  export CXX=g++
      mkdir build && cd build
      cmake ../ -DENABLE_TEST=true -DBUILD_DOCUMENTATION=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS
      make all test
      ;;

esac

