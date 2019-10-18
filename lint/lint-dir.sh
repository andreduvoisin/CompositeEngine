#!/bin/bash

for filename in "$1"/*.h; do
	echo "$filename"
	clang-format -i "$filename"
	clang-tidy "$filename"
done

for filename in "$1"/*.cpp; do
	echo "$filename"
	clang-format -i "$filename"
	clang-tidy "$filename"
done
