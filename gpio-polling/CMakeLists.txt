cmake_minimum_required(VERSION 3.16.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gpio_polling)
target_sources(app PRIVATE src/main.c)