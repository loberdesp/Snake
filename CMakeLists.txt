cmake_minimum_required(VERSION 3.22)
# Tu wstawcie nazwę swojego projektu
project (snake)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -pedantic -Werror=return-type -Werror=parentheses -Werror=vla -Werror=array-bounds -Wcast-qual -Wmaybe-uninitialized )

# Windows specific config
# To możecie zmienić jeśli zainstalujecie SFML-a w innym miejscu
IF (WIN32)
    # Include local sfml cmake config
    set(SFML_DIR "C:/lib/SFML-2.5.1/lib/cmake/SFML")
    # Link sfml statically (Optional)
    set(SFML_STATIC_LIBRARIES TRUE)
ENDIF()

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

set(SOURCE_FILES main.cpp snake.cpp)

# tu ustawiacie nazwę pliku wykonywalnego - możecie zmienić choinka na coś innego
add_executable(snake ${SOURCE_FILES})
target_link_libraries(snake sfml-graphics)