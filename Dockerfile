# Dockerfile for native (x86_64/amd64) build
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN rm -rf build && mkdir build && cd build && cmake .. && make

CMD ["/bin/bash", "-c", "cd build && ./unit_tests && echo '\n=== Running benchmark ===' && ./benchmark"]
