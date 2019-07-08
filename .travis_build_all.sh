#!/bin/sh
set -eu

for f in $(ls); do
    if [ -d "$f" ] && [ -f "$f/platformio.ini" ]; then
        cd "$f"
        echo "Build $f"
        platformio run -t buildprog
        cd ..
    fi
done
