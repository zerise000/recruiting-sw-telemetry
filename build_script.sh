#! /bin/bash
LOGS_DIR="./logs"
CSV_DIR="./csv"
BUILD_DIR="build"

if [[ $1 == "--clean" ]];then
	rm -rf $LOGS_DIR/* 
	rm -rf $CSV_DIR/*
	exit 0
fi

mkdir -p $LOGS_DIR
mkdir -p $CSV_DIR 

mkdir -p $BUILD_DIR 
cd $BUILD_DIR
cmake ..

make -j$(nproc)
