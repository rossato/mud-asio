cmake_minimum_required( VERSION 3.0 )
project(mud)

if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

find_package( Boost COMPONENTS system REQUIRED )
find_package( Threads )
find_package( Doxygen )
find_package( GTest REQUIRED )

find_package( PkgConfig )
pkg_check_modules( LUA REQUIRED lua )

include_directories( ${Boost_INCLUDE_DIR} . )

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wreturn-type -Wreorder)

include_directories(src)
set(SRCS
  src/Actions/Actions.cpp
  src/Actions/AdminActions.cpp
  src/Actions/BasicActions.cpp
  src/Actions/HelpActions.cpp
  src/Actions/ItemHandling.cpp
  src/Actions/NounMatchEvaluator.cpp
  src/Dictionary/Dictionary.cpp
  src/Dictionary/Tokenizer.cpp
  src/Grammar/BasicMatchers.cpp
  src/Grammar/Grammar.cpp
  src/Grammar/GrammarBuilder.cpp
  src/Http/HttpConnectionFactory.cpp
  src/Http/HttpInterface.cpp
  src/Interface/HandleLoginState.cpp
  src/Interface/HandlePasswordState.cpp
  src/Interface/LuaState.cpp
  src/Interface/MudInterface.cpp
  src/Interface/ParsingGrammarState.cpp
  src/Program/MudConnectionFactory.cpp
  src/Server/ConnectionBase.cpp
  src/Server/Server.cpp
  src/World/Direction.cpp
  src/World/Location.cpp
  src/World/Lua.cpp
  src/World/Noun.cpp
  src/World/User.cpp
  src/World/UserReader.cpp
  src/World/World.cpp
  src/World/WorldBuilder.cpp
  )

add_library(mudlib ${SRCS})
add_executable(mud src/main.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  target_link_libraries(mud mudlib -static ${Boost_LIBRARIES} ${LUA_STATIC_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT})
  add_custom_target(image DEPENDS mud COMMAND podman build -t mud-asio:static .)
else()
  target_link_libraries(mud mudlib ${Boost_LIBRARIES} ${LUA_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT})
endif()

set(TEST_SRCS
  test/HasWorld.cpp
  test/MudInterfaceTest.cpp
  test/TokenizerTest.cpp
  test/HelpTest.cpp
  test/LookTest.cpp
  test/GoTest.cpp
  test/SayTest.cpp
  test/TestConnection.cpp)

add_executable(mudtest ${TEST_SRCS})
target_link_libraries(mudtest mudlib ${Boost_LIBRARIES} ${LUA_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})
add_custom_target(check DEPENDS mudtest COMMAND mudtest)
add_custom_target(dump DEPENDS mudtest COMMAND mudtest --gtest_filter="*Dump*" > dump.txt)
doxygen_add_docs(doc)
