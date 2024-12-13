# 3.10 is the version I'm using, but if you're on a lower version feel free to comment out this line and report back results...
cmake_minimum_required(VERSION 3.10)

project(cftf)

if (__LINUX__)
    find_package(Clang REQUIRED CONFIG HINTS /usr/lib64/cmake/clang)
    find_package(LLVM REQUIRED CONFIG HINTS /usr/lib64/cmake/llvm)
endif()

if (__WINDOWS__)
    set(CLANG_DIR c:/clang_llvm)
    find_package(Clang REQUIRED CONFIG HINTS ${CLANG_DIR}/lib/cmake/clang)
    find_package(LLVM REQUIRED CONFIG HINTS ${CLANG_DIR}/lib/cmake/llvm)

    set(CLANG_INCLUDE_DIR ${CLANG_DIR}/include)
    include_directories(${CLANG_INCLUDE_DIR})
    include_directories(${CLANG_DIR}/include)
endif()

set(SOURCE_FILES
    constexpr_if.cpp
    main.cpp
    rewriter.cpp
    structured_bindings.cpp
    template_specializer.cpp
    modernizer.cpp
    compiler_wrapper.cpp
)

set(HEADER_FILES
    ast_visitor.h
    rewriter.h
    modernizer.h
    compiler_wrapper.h
)

add_executable(cftf ${SOURCE_FILES} ${HEADER_FILES})

if (__LINUX__)
    target_link_libraries(cftf 
        LLVM 
        clangAST 
        clangBasic 
        clangFrontend 
        clangLex 
        clangTooling 
        clangRewrite 
        stdc++fs
)
elseif (__WINDOWS__)
    target_link_libraries(cftf 
        LLVMCore
        LLVMSupport
        LLVMIRReader
        LLVMBitReader
        LLVMBitWriter
        LLVMAnalysis
        LLVMMC
        LLVMMCParser
        LLVMLinker
        clangBasic
        clangFrontend
        clangLex
        clangTooling
        clangRewrite
        kernel32
        user32
        gdi32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
)
endif()

set_property(TARGET cftf PROPERTY CXX_STANDARD 17)
set_property(TARGET cftf PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER ${CLANG_BIN_DIR}/clang++.exe)
set(CMAKE_C_COMPILER ${CLANG_BIN_DIR}/clang.exe)

install(TARGETS cftf RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
