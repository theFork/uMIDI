#!/bin/sh -e

cd "/github/workspace/$1" || exit 1
make "$2"
