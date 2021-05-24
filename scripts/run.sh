#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/bin/sfml_imgui_opengl_ex 
else
    ./bin/debug/bin/sfml_imgui_opengl_ex 
fi