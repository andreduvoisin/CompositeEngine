#!/bin/bash

clang-format -i $1
clang-tidy -fix $1
