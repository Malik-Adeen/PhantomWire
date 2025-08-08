cmake_minimum_required(VERSION 3.15)
project(cpp_rat)

set(CMAKE_CXX_STANDARD 17)

# Add include directories
include_directories(include)
include_directories(common)

# Add encryption source file (used by both client and server)
set(SHARED_SRC
    src/encryption.cpp
)

# === Client ===
add_executable(client
    client/main.cpp
    ${SHARED_SRC}            # ✅ Add encryption.cpp
)

target_include_directories(client PRIVATE include)

# === Server ===
add_executable(server
    server/main.cpp
    ${SHARED_SRC}            # ✅ Add encryption.cpp
)

target_include_directories(server PRIVATE include)
