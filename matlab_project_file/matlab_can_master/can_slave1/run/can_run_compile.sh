cd mex_cmake
cd build
make
make install
cd ..
cd bin
rm run.mexa64
mv can_master.mexa64 run.mexa64
cp run.mexa64 /home/russel25/Desktop/can_slave1/z_matlab_file
