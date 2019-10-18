#!/bin/bash

clang-format -i $1
clang-tidy $1
