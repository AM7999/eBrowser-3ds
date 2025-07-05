cmake_minimum_required(VERSION 3.13)

project(eBrowser LANGUAGES CXX VERSION 1.0.0)

set(DCMAKE_EXPORT_COMPILE_COMMANDS 1)

# -------------------------------------
# eBrowser.3dsx
# -------------------------------------
add_executable(${PROJECT_NAME} source/main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -g -O2 -Wall )
target_include_directories(${PROJECT_NAME} PRIVATE include 3rdparty/include /opt/devkitpro/portlibs/3ds/include )
find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} -L/opt/devkitpro/portlibs/3ds/lib -lcurl -lmbedtls -lmbedx509 -lmbedcrypto -lz -lctru -lmbedtls -lmbedx509 -lmbedcrypto -lz -lctru -ljansson)

ctr_generate_smdh(${PROJECT_NAME}.smdh
  NAME "${PROJECT_NAME}"
  DESCRIPTION "v${PROJECT_VERSION}, A Browser for e621 on the 3ds"
  AUTHOR "AM7999"
)

ctr_create_3dsx(${PROJECT_NAME}
  SMDH ${PROJECT_NAME}.smdh
)
# -------------------------------------

# -------------------------------------
# test.elf
# -------------------------------------

add_executable(test source/test.cpp)
target_compile_options(test PRIVATE -g -O2 -Wall )
target_include_directories(test PRIVATE include 3rdparty/include /opt/devkitpro/portlibs/3ds/include )
find_package(CURL REQUIRED)
target_link_libraries(test -L/opt/devkitpro/portlibs/3ds/lib -lcurl -lmbedtls -lmbedx509 -lmbedcrypto -lz -lctru -lmbedtls -lmbedx509 -lmbedcrypto -lz -lctru -ljansson)
# -------------------------------------
