@REM cd src/ThirdPart/vcpkg
@REM ./bootstrap-vcpkg.bat
@REM cd ../../../
@REM "./tool/vcpkg/scripts/buildsystems/vcpkg integrate install

@REM rd build /s/q
@REM mkdir build 
cd build


cmake .. -DCMAKE_TOOLCHAIN_FILE="./tool/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_SYSTEM_VERSION=10.0.22621.0
@REM set Root="./src/ThirdPart/vcpkg"
@REM $env:VCPKG_ROOT ="./src/ThirdPart/vcpkg"
@REM $env:PATH = "$env:VCPKG_ROOT;$env:PATH"
@REM cmake --preset=default 
@REM cmake .. -DCMAKE_SYSTEM_VERSION=10.0.22621.0