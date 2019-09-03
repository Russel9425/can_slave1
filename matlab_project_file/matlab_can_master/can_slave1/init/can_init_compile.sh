cd mex_cmake
cd build
make
make install
cd ..
cd bin
rm init.mexa64
mv can_master.mexa64 init.mexa64
cp init.mexa64 /home/russel25/Desktop/can_slave1/z_matlab_file
