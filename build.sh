#!/usr/bin/env bash
cd $(dirname $0)

set -x
: ${CMAKE_PROJECT_ROOT_PATH:=$(pwd)}
: ${CMAKE_BUILD_PATH:=build}

rm -fr ${CMAKE_BUILD_PATH}

: ${ANDROID_SDK_PATH:=~/Library/Android/sdk}
ANDROID_NDK_PATH=${ANDROID_SDK_PATH}/ndk-bundle
ANDROID_CMAKE_ROOT_PATH=$(find ${ANDROID_SDK_PATH}/cmake -type d -maxdepth 1 -mindepth 1 | tail -n 1)

for abi in arm64-v8a armeabi-v7a
do
    ${ANDROID_CMAKE_ROOT_PATH}/bin/cmake -H${CMAKE_PROJECT_ROOT_PATH} -B${CMAKE_BUILD_PATH}/android/${abi} \
        -DANDROID_ABI=${abi} \
        -DANDROID_PLATFORM=android-16 \
        -DANDROID_NDK=${ANDROID_NDK_PATH} \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_PATH}/build/cmake/android.toolchain.cmake \
        -DCMAKE_MAKE_PROGRAM=${ANDROID_CMAKE_ROOT_PATH}/bin/ninja \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_SYSTEM_NAME=Android \
        -G Ninja

    ${ANDROID_CMAKE_ROOT_PATH}/bin/cmake --build ${CMAKE_BUILD_PATH}/android/${abi}
done

# iOS
cmake -H${CMAKE_PROJECT_ROOT_PATH} -B${CMAKE_BUILD_PATH}/ios -DCMAKE_SYSTEM_NAME=iOS
cmake --build ${CMAKE_BUILD_PATH}/ios

# macOS
cmake -H${CMAKE_PROJECT_ROOT_PATH} -B${CMAKE_BUILD_PATH}/macos
cmake --build ${CMAKE_BUILD_PATH}/macos
