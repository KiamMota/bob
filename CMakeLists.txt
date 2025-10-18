cmake_minimum_required(VERSION 3.15)
project(bob)
message("bob -> http server")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adiciona libuv como subdiretório
add_subdirectory(services/libuv)

set(BOB_SRC 
  Bob/Server/BobServer.cpp
  Bob/Http/Request.cpp
  Bob/Http/Response.cpp
)

add_library(bob ${BOB_SRC})
target_include_directories(bob PRIVATE Bob)
target_link_libraries(bob PRIVATE uv_a)



set(BOB_TST test/bob/main.cpp)
add_executable(bob_test ${BOB_TST})

target_include_directories(bob_test PRIVATE services/libuv/include Bob)

# Linkar com a libuv compilada local
target_link_libraries(bob_test PRIVATE uv_a bob)

# Garantir que em tempo de execução o sistema use a lib local
set_target_properties(bob_test PROPERTIES
    BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/services/libuv"
)


