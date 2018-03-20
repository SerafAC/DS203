FROM ubuntu:16.04

RUN apt update \
    && apt install -y \
        apt-transport-https \
        binutils-arm-none-eabi \
        build-essential \
        g++-mingw-w64-x86-64 \
        gcc-arm-none-eabi \
        gcc-mingw-w64-x86-64 \
        libnewlib-arm-none-eabi \
        mtools \
        software-properties-common \
        wget

RUN dpkg --add-architecture i386 \
    && wget -nc https://dl.winehq.org/wine-builds/Release.key \
    && apt-key add Release.key \
    && apt-add-repository https://dl.winehq.org/wine-builds/ubuntu/ \
    && apt update \
    && apt install -y --install-recommends winehq-stable \
    && rm Release.key