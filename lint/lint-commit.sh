#!/bin/bash

git add *.h *.cpp
staged_files=$(git diff --name-only --cached)
git commit -m "lint: $staged_files"
