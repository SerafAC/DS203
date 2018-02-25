#!/bin/sh
cd ../Source
find HwLayer/SDL2 HwLayer/Common Core/ Framework/ Gui/ Library/ Main/ User/ -name *.cpp -o -name *.h -o -name *.c | xargs clang-format-5.0 -i --style=Google
