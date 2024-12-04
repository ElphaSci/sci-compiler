cmake_minimum_required(VERSION 3.12)
project(SCI_COMPILER C CXX)

message("Project Source Directory: ${PROJECT_SOURCE_DIR}")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-write-strings -Wno-unused-result -fpermissive")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_definitions(-D_MAX_PATH=260)

add_executable(sc
        sc.cpp
        alist.cpp
        alist.hpp
        anode.cpp
        anode.hpp
        asm.cpp
        asm.hpp
        banner.cpp
        banner.hpp
        builtins.cpp
        builtins.hpp
        chartype.cpp
        class.cpp
        compile.cpp
        compile.hpp
        compiler.hpp
        debug.cpp
        debug.hpp
        define.cpp
        define.hpp
        error.cpp
        error.hpp
        expr.cpp
        fileio.cpp
        fileio.hpp
        getargs.cpp
        getargs.hpp
        input.cpp
        input.hpp
        jeff.hpp
        list.cpp
        list.hpp
        listing.cpp
        listing.hpp
        loop.cpp
        mem.cpp
        mem.hpp
        memtype.hpp
        new.cpp
        new.hpp
        object.cpp
        object.hpp
        opcodes.hpp
        optimize.cpp
        optimize.hpp
        output.cpp
        output.hpp
        parse.cpp
        parse.hpp
        pnode.cpp
        proc.cpp
        resource.cpp
        resource.hpp
        sc.hpp
        selector.cpp
        share.cpp
        share.hpp
        sol.hpp
        string.cpp
        string.hpp
        symbol.cpp
        symbol.hpp
        symtbl.cpp
        symtbl.hpp
        text.cpp
        text.hpp
        token.cpp
        token.hpp
        toktypes.cpp
        update.cpp
        update.hpp
        vocab.hpp
)