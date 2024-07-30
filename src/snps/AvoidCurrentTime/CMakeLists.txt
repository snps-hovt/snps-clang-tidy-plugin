target_include_directories(snps-module
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(snps-module PRIVATE ${CMAKE_CURRENT_LIST_DIR}/AvoidCurrentTimeCheck.cpp)