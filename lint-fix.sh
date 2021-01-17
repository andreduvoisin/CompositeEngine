#!/bin/bash

/Users/andreduvoisin/Downloads/clang+llvm-8.0.0-x86_64-apple-darwin/bin/clang-format -i $1
/Users/andreduvoisin/Downloads/clang+llvm-8.0.0-x86_64-apple-darwin/bin/clang-tidy -fix $1