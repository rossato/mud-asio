cmake_minimum_required( VERSION 3.0 )
project(mud)

find_package( Boost COMPONENTS system REQUIRED )
find_package( Threads )
find_package( Doxygen )

include_directories( ${Boost_INCLUDE_DIR} . )

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wreturn-type -Wreorder)

include_directories(src)
set(SRCS
  src/Dictionary/Dictionary.cpp
  src/Dictionary/Tokenizer.cpp
  src/Grammar/BasicMatchers.cpp
  src/Grammar/Grammar.cpp
  src/Grammar/GrammarBuilder.cpp
  src/Http/HttpConnectionFactory.cpp
  src/Http/HttpInterface.cpp
  src/Logic/AdminActions.cpp
  src/Logic/BasicActions.cpp
  src/Logic/HelpActions.cpp
  src/Logic/ItemHandling.cpp
  src/Logic/Logic.cpp
  src/Logic/MudConnectionFactory.cpp
  src/Logic/MudInterface.cpp
  src/Logic/NounMatchEvaluator.cpp
  src/Server/ConnectionBase.cpp
  src/Server/Server.cpp
  src/World/Direction.cpp
  src/World/Location.cpp
  src/World/User.cpp
  src/World/World.cpp
  src/World/WorldBuilder.cpp
  src/World/WorldReader.cpp
  )

add_library(mudlib ${SRCS})
target_link_libraries(mudlib ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_executable(mud src/main.cpp)
target_link_libraries(mud mudlib)

set(TEST_SRCS
  test/HasWorld.cpp
  test/MudInterfaceTest.cpp
  test/TokenizerTest.cpp
  test/HelpTest.cpp
  test/LookTest.cpp
  test/GoTest.cpp
  test/SayTest.cpp)

add_executable(mudtest ${TEST_SRCS})
target_link_libraries(mudtest mudlib -lgtest -lgtest_main)
add_custom_target(check mudtest)
add_custom_target(dump mudtest --gtest_filter="*Dump*" > dump.txt)
doxygen_add_docs(doc)
