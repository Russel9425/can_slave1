cd mex_cmake
cd build
make
make install
cd ..
cd bin
rm stop.mexa64
mv can_master.mexa64 stop.mexa64
cp stop.mexa64 /home/russel25/Desktop/can_slave1/z_matlab_file
