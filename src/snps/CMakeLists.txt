add_library(snps-module MODULE "")
target_compile_options(snps-module PRIVATE -fno-rtti)
target_include_directories(snps-module
    PRIVATE
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(snps-module PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SnpsModule.cpp)

add_subdirectory(AvoidCurrentTime)

# Locate the clang_tidy executable
find_program(CLANG_TIDY_EXE NAMES clang-tidy PATHS ${LLVM_TOOLS_BINARY_DIR} NO_DEFAULT_PATH)

if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "clang-tidy not found")
endif()

set(CTEST_OUTPUT_ON_FAILURE TRUE)

set(CLANG_TIDY_SNPS_PLUGIN ${CMAKE_BINARY_DIR}/lib64/libsnps-module.so)

add_subdirectory(test)
