make Debug -j8 -s
cd bin
gdb -ex=run --args example_debug reset
cd ..
