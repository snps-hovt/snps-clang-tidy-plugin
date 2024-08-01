# snps-clang-tidy-plugin

clang-tidy plugins used in Synopsys

## How to build

First, setup to use llvm (version 15.0.6 or higher), and cmake (version 3.20 or higher).

```bash
export CC=/depot/tools/clang/clang1506_gcc1220/linux64/bin/clang
export CXX=/depot/tools/clang/clang1506_gcc1220/linux64/bin/clang++
export PATH=/depot/cmake/cmake-3.26.1/bin/:$PATH
export CMAKE_PREFIX_PATH=/depot/tools/clang/clang1506_gcc1220/linux64/lib/cmake/llvm/
```

Then use a standard cmake build flow.

```bash
mkdir build
cd build
cmake ../
cmake --build ./ -j 16
```

## How to run tests

The correct way of testing clang-tidy checks is to use the check-clang-tools executable. We don't have the check-clang-tools in the clang directory on our machines, so we will test it with a custom script.

You may need to change the permission for the test script to executable.

```bash
chmod a+x src/snps/test/run_clang_tidy_tests.sh
```

To run all tests simply use ctest command.

```bash
ctest
```

The logs for tests run should be located at < PathToBuildDirectory >/Testing/Temporary/LastTest.log file.
