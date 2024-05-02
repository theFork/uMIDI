#!/bin/sh -e

cd "/src/$1" || exit 1
make "$2"
