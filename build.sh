WORK_DIR=`pwd`/

cd ${WORK_DIR}
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug >> cmake.log 2>&1
make -j >> make.log 2>&1