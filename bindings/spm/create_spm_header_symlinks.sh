#!/bin/bash
cd "$(dirname "$0")/include"
rm *.h
rm *.hpp
find ../../../include \( -iname '*.hpp' -o -iname '*.h' \) -exec ln -s '{}' . \;
find ../../../libmei \( -iname '*.hpp' -o -iname '*.h' \) -exec ln -s '{}' . \;
rm win_*.h
rm zip_*.hpp
