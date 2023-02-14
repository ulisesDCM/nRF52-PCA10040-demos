cmake_minimum_required(VERSION 3.16.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(minimal)
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/myMath.c)

# addition of the custom files to be conditional
target_sources_ifdef(CONFIG_MYMATH app PRIVATE src/myMath.c)

