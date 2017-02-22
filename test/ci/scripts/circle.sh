#!/bin/bash

GTEST_ROOT="$HOME/.local/gmock"
GMOCK_ROOT="$HOME/.local/gmock"

GTEST_INCLUDE_DIR=../thirdparty/googletest/googletest/include
GMOCK_INCLUDE_DIR=../thirdparty/googletest/googlemock/include

case $CIRCLE_NODE_INDEX in
  0)  export CXX=clang++
      mkdir build && cd build
      cmake ../ -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DCI_BUILD=true -DGTEST_ROOT=$GTEST_ROOT -DGMOCK_ROOT=$GMOCK_ROOT -DGTEST_INCLUDE_DIR=$GTEST_INCLUDE_DIR -DGMOCK_INCLUDE_DIR=$GMOCK_INCLUDE_DIR
      make all test
      ;;
  1)  export CXX=g++
      mkdir build && cd build
      cmake ../ -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DCI_BUILD=true -DGTEST_ROOT=$GTEST_ROOT -DGMOCK_ROOT=$GMOCK_ROOT -DGTEST_INCLUDE_DIR=$GTEST_INCLUDE_DIR -DGMOCK_INCLUDE_DIR=$GMOCK_INCLUDE_DIR
      make all test
      ;;
esac

