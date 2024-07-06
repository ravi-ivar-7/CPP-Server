# Stage 1: Build Stage
FROM alpine:latest

# Set working directory inside the container
WORKDIR /cpp

# Copy all files from the host to the container's working directory
COPY . .

# Update package index and install necessary dependencies
RUN apk update && \
    apk add --no-cache \
    libboost-all-dev \
    nlohmann-json3-dev \
    libssl-dev \
    make \
    g++

# Compile the C++ server application using make
RUN make

# Expose port 8080 for the server to listen on
EXPOSE 8080

# Set the command to run the server when the container starts
CMD ["./server"]
