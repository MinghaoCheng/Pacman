echo "Building car programme......................."
echo "............................................."
cd ./car/build
cmake .
make clean
make all
echo "Cleaning temp files.........................."
rm -r CMakeFiles
rm CMakeCache.txt
rm cmake_install.cmake
rm Makefile
mv car ./../../car_exe
cd ./../..

echo "............................................."
echo "Finished....................................."
echo "............................................."
