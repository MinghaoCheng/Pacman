echo "Building main programme......................"
echo "............................................."
cd ./host/build
cmake .
make clean
make all
echo "Cleaning temp files.........................."
rm -r CMakeFiles
rm CMakeCache.txt
rm cmake_install.cmake
mv host ./../../host_exe
cd ./../..

if [ ! -f Pacman_dev.ko ]; then
    echo "Building kernel module......................."
    echo "............................................."
    cd host/src/kernelmod
    make
    echo "Cleaning temp files.........................."
    rm -r .tmp_versions
    rm .*.cmd
    rm *.symvers
    rm *.order
    rm *.mod.c
    rm *.o
    mv Pacman_dev.ko ./../../../Pacman_dev.ko
    cd ./../../
fi

echo "............................................."
echo "Finished....................................."
echo "............................................."
