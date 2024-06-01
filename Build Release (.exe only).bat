@echo off
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_JAR=OFF
cmake --build build
pause