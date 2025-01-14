#!/bin/bash

set -e

# 如果没有build目录，创建该目录
if [ ! -d "build" ]; then
    mkdir build
fi    

rm -rf build/*

cd  build  &&
    cmake .. &&
    make
# 回到项目根目录
cd ..
#把头文件拷贝到 /usr/include/mymuduo  so库拷贝到 /usr/lib PATH
if [ ! -d /usr/include/mymuduo ]; then
    mkdir /usr/include/mymuduo
fi

for header in *.h
do 
    cp "$header" /usr/include/muduo
done 

cp $(pwd)/lib/libmymuduo.so /usr/lib

ldconfig