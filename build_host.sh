cd ./host/build
cmake .
make clean
make all
mv host ../../host_exe

cd ./../

cd src/kernelmod
make
mv Pacman_dev.ko ../../../Pacman_dev.ko
