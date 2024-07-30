add_library(snps-module MODULE "")
target_compile_options(snps-module PRIVATE -fno-rtti)
target_include_directories(snps-module
    PRIVATE
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(snps-module PRIVATE ${CMAKE_CURRENT_LIST_DIR}/SnpsModule.cpp)

add_subdirectory(AvoidCurrentTime)