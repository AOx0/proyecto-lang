# Generate a CMakeLists.txt file that includes c files at ./lib and ./src
# and header files at ./include

cmake_minimum_required(VERSION 3.10)
project(lng)

# Get all the c files in ./src
file(GLOB SRC_SOURCES src/*.c)

# Get all the header files in ./include
file(GLOB HEADERS src/include/*.h)

# Add the executable
add_executable(${PROJECT_NAME} ${LIB_SOURCES} ${SRC_SOURCES} ${HEADERS})

# Add the include directory to the include paths
target_include_directories(${PROJECT_NAME} PUBLIC include)
