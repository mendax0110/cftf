cmake_minimum_required(VERSION 3.10)

project(cftf)

if(APPLE)
    set(CLANG_DIR /opt/homebrew/opt/llvm)
    set(CLANG_BIN_DIR ${CLANG_DIR}/bin)
    set(CLANG_INCLUDE_DIR ${CLANG_DIR}/include)
    set(LLVM_INCLUDE_DIR ${CLANG_DIR}/include)
    set(LLVM_LIBRARY_DIR ${CLANG_DIR}/lib)

    find_package(Clang REQUIRED CONFIG HINTS ${CLANG_DIR}/lib/cmake/clang)
    find_package(LLVM REQUIRED CONFIG HINTS ${CLANG_DIR}/lib/cmake/llvm)

    include_directories(${CLANG_INCLUDE_DIR} ${LLVM_INCLUDE_DIR})
    link_directories(${LLVM_LIBRARY_DIR})
elseif(WIN32)
    set(CLANG_DIR c:/clang_llvm)
    set(CLANG_INCLUDE_DIR ${CLANG_DIR}/include)
    set(LLVM_INCLUDE_DIR ${CLANG_DIR}/include)

    find_package(Clang REQUIRED CONFIG HINTS ${CLANG_DIR}/lib/cmake/clang)
    find_package(LLVM REQUIRED CONFIG HINTS ${CLANG_DIR}/lib/cmake/llvm)

    include_directories(${CLANG_INCLUDE_DIR} ${LLVM_INCLUDE_DIR})
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

# Define Target
add_executable(cftf ${SOURCE_FILES} ${HEADER_FILES})

set_property(TARGET cftf PROPERTY CXX_STANDARD 17)
set_property(TARGET cftf PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(cftf PRIVATE
        ${CLANG_INCLUDE_DIR}
        ${LLVM_INCLUDE_DIR}
)

if(APPLE)
    target_link_libraries(cftf
            LLVM
            clangAST
            clangBasic
            clangFrontend
            clangLex
            clangTooling
            clangRewrite
            clangRewriteFrontend
            clangDriver
            clangSerialization
            clangAnalysis
            clangEdit
            clangSema
            clangParse
            clangASTMatchers
            clangToolingCore
            clangFormat
            clangCodeGen
            clangDynamicASTMatchers
            clangSupport
    )
    set(CMAKE_CXX_COMPILER ${CLANG_BIN_DIR}/clang++)
    set(CMAKE_C_COMPILER ${CLANG_BIN_DIR}/clang)
elseif(WIN32)
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
    set(CMAKE_CXX_COMPILER ${CLANG_DIR}/bin/clang++.exe)
    set(CMAKE_C_COMPILER ${CLANG_DIR}/bin/clang.exe)
endif()

message(STATUS "Clang include directory: ${CLANG_INCLUDE_DIR}")
message(STATUS "LLVM include directory: ${LLVM_INCLUDE_DIR}")
message(STATUS "Clang binary directory: ${CLANG_BIN_DIR}")
message(STATUS "LLVM library directory: ${LLVM_LIBRARY_DIR}")

install(TARGETS cftf RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
