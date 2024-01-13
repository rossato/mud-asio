FROM quay.io/fedora/fedora:38
RUN dnf -y update && dnf -y install cmake gtest gtest-devel boost-devel boost-static lua-devel lua-static glibc-static gcc-c++ libstdc++-static && dnf clean all