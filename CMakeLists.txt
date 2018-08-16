# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 2.8)
project(ej)

enable_language(ASM)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(USE_ASAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif(USE_ASAN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17 -fvisibility=hidden -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wsuggest-override -Wtrampolines -Winline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fuse-ld=gold -Wl,--gc-sections -Wl,-z,relro,-z,now")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -march=native -fno-asynchronous-unwind-tables -fomit-frame-pointer -fno-math-errno -DNDEBUG -Wsuggest-attribute=malloc -Wsuggest-attribute=noreturn")

set(CMAKE_AR "gcc-ar")
set(CMAKE_RANLIB "gcc-ranlib")

include_directories("h")
file(GLOB source_files "src/*.cxx" "src/x86_64/*.S")
add_library(ej STATIC ${source_files})
