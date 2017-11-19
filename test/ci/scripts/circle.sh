#!/bin/bash

function build_lugdunum() {
    cd ~/Lugdunum

    mkdir build && cd build
    # $1 is true if is android build
    if [[ "$1" != true ]]; then
        (cmake .. -DBUILD_TESTS=true -DTEST_OUTPUT=$CIRCLE_TEST_REPORTS -DLUG_THIRDPARTY_DIR=$HOME/.local/thirdparty_linux) || return 1
        (make all test && sudo make install) || return 1
    else
        (${ANDROID_SDK_ROOT}/cmake/*/bin/cmake .. \
            -G "Android Gradle - Unix Makefiles" \
            -DLUG_THIRDPARTY_DIR=$HOME/.local/thirdparty_android \
            -DANDROID=ON \
            -DANDROID_PLATFORM=android-24 \
            -DANDROID_STL=c++_shared \
            -DCMAKE_BUILD_TYPE=Debug \
            -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake \
        ) || return 1
        (make all) || return 1
    fi


    return 0
}

function build_samples() {
    cd ~/Lugdunum/samples

    mkdir build && cd build
    (cmake .. -DBUILD_SHADERS=false -DLUG_THIRDPARTY_DIR=$HOME/.local/thirdparty) || return 1
    (make all) || return 1

    return 0
}

case $BUILD_ANDROID in
    0)
        # Linux
        export CXX=g++
        build_lugdunum false && build_samples
    ;;

    1)
        # Android
        build_lugdunum true
    ;;
esac

