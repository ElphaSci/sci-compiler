cmake_minimum_required(VERSION 3.12)
project(SCI_COMPILER C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-write-strings -Wno-unused-result -fpermissive")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_definitions(-D_MAX_PATH=260)

#add_library(sc_lib OBJECT
add_executable(sc
        sc.cpp
        sc.hpp
        sol.hpp
        jeff.hpp
        string.cpp
        string.hpp
        asm.cpp
        asm.hpp
        banner.cpp
        banner.hpp
        builtins.cpp
        builtins.hpp
        error.cpp
        error.hpp
        input.cpp
        input.hpp
        listing.cpp
        listing.hpp
        object.cpp
        object.hpp
        opcodes.hpp
        output.cpp
        output.hpp
        parse.hpp
        parse.cpp
        resource.cpp
        resource.hpp
        share.cpp
        share.hpp
        token.cpp
        token.hpp
        update.cpp
        update.hpp

        alist.cpp
        alist.hpp
        anode.cpp
        anode.hpp
        chartype.cpp
        class.cpp
        compile.cpp
        compile.hpp
        compiler.hpp
        debug.cpp
        debug.hpp
        define.cpp
        define.hpp
        expr.cpp
        fileio.cpp
        fileio.hpp
        getargs.cpp
        getargs.hpp
        list.cpp
        list.hpp
        loop.cpp
        memtype.hpp
        new.cpp
        new.hpp
        optimize.cpp
        optimize.hpp
        pnode.cpp
        proc.cpp
        selector.cpp
        symbol.cpp
        symbol.hpp
        symtbl.cpp
        symtbl.hpp
        text.cpp
        text.hpp
        toktypes.cpp
        vocab.hpp


)