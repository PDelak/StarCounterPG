setlocal

mkdir build-msvc
pushd build-msvc
cmake -G "Visual Studio 14 2015 Win64" ..
popd
