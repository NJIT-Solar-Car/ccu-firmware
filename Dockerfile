FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential \
    bison \
    flex \
    curl \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# clone the doxygen repo
RUN git clone --depth 1 --branch Release_1_9_8 \
    https://github.com/doxygen/doxygen.git \
    && cd doxygen

RUN mkdir /doxygen/build

WORKDIR /doxygen/build

RUN cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_EXE_LINKER_FLAGS="-static -static-libgcc -static-libstdc++" \
          -DForceStaticInlining=ON ..

RUN make -j$(nproc)

FROM alpine:3.19

RUN apk update && apk add --no-cache \
    graphviz \
    git \
    python3 \
    make cmake \
    bzip2 \
    ttf-freefont \
    gcc-arm-none-eabi \
    newlib-arm-none-eabi

COPY --from=builder /doxygen/build/bin/doxygen /usr/local/bin/doxygen

WORKDIR /project

# CMD ["doxygen", "--version"]
