FROM ubuntu:latest

RUN apt update && apt install -y \
    git \
    python3-pip \
    sudo \
    gcc-aarch64-linux-gnu \
    qemu-system-aarch64 \
    gdb-multiarch \
    && rm -rf /var/lib/apt/lists/*

# Allow sudo without a password
RUN echo "ALL ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN pip3 install --no-cache-dir --break-system-packages \
    pyserial \
    pwn \
    argparse

WORKDIR /usr/os_lab

CMD ["/bin/bash"]