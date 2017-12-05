cmake_minimum_required(VERSION 3.8)
project(MNIST_CLINK_Clustering)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp utils/mnist_file_utils.cpp utils/mnist_file_utils.h mnist/mnist.cpp mnist/mnist.h cluster.cpp cluster.h)
add_executable(MNIST_CLINK_Clustering ${SOURCE_FILES})