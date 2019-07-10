#!/bin/sh

if [ -z "$1" ]; then
    echo "Specify version!"
    exit 1
fi

for f in $(find . -maxdepth 2 -type f -name platformio.ini); do
    echo "$f"
    sed -i "s=\(RB3201-RBControl-Roboruka-library/archive/v\)[0-9.]\+\.zip=\1$1.zip=" "$f"
done
