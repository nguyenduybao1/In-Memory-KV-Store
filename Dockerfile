# ===== Build stage =====
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make

WORKDIR /app
COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make -j

# ===== Runtime stage =====
FROM ubuntu:22.04

WORKDIR /app

COPY --from=builder /app/build/kvstore_app .

EXPOSE 6379

CMD ["./kvstore_app"]
