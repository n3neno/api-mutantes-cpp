
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive


RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsqlite3-dev \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /app

COPY . .

RUN mkdir -p build && cd build && cmake .. && make


EXPOSE 18081


CMD ["./build/api"]