# Multi-stage build for C++ HTTP Server
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    nlohmann-json3-dev \
    libssl-dev \
    libsqlite3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cd bcrypt && mkdir -p build && cd build && cmake .. && make && cd ../..

RUN make clean && make

FROM ubuntu:22.04

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libboost-system1.74.0 \
    libboost-filesystem1.74.0 \
    libssl3 \
    libsqlite3-0 \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -r -s /bin/false cppserver

WORKDIR /app

COPY --from=builder /app/server .
COPY --from=builder /app/templates ./templates
COPY --from=builder /app/config ./config
COPY --from=builder /app/bcrypt/build/libbcrypt.a ./bcrypt/build/

RUN chown -R cppserver:cppserver /app

USER cppserver

EXPOSE 8080

HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:8080/health || exit 1

CMD ["./server"]
