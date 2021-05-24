#!/bin/bash


target_release() {
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
    echo "Built target in bin/release/"
    cd ../..
}

target_debug() {
    cd debug 
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
    echo "Built target in bin/debug/"
    cd ../..
}

# Create folder for distribution
if [ "$1" = "release" ]
then
    if [ -d "$sfml_imgui_opengl_ex " ]
    then
        rm -rf -d sfml_imgui_opengl_ex 
    fi

    mkdir -p sfml_imgui_opengl_ex 
fi

# Creates the folder for the binaries
mkdir -p sfml_imgui_opengl_ex 
mkdir -p sfml_imgui_opengl_ex/Data
mkdir -p bin
mkdir -p bin/release
mkdir -p bin/debug
cd bin

conan install .. -s compiler.libcxx=libstdc++11 --build=missing


# Builds target
if [ "$1" = "release" ]
then
    target_release
    cp bin/release/sfml_imgui_opengl_ex  sfml_imgui_opengl_ex/sfml_imgui_opengl_ex 
else
    target_debug
fi

cp -R Data sfml_imgui_opengl_ex/Data
