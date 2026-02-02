FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    netcat-openbsd

WORKDIR /app

COPY . .

RUN mkdir build && cd build && \
    cmake -DBUILD_TESTS=OFF .. && \
    make -j
    

EXPOSE 6379

CMD ["./build/kvstore_app"]
