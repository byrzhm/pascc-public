FROM ubuntu:latest

WORKDIR /root

# 使用清华源
RUN sed -i "s@http://.*.ubuntu.com@http://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list

# 安装依赖
RUN apt-get update && apt-get install -y \
    python3 \
    build-essential \
    clang \
    git \
    flex \
    bison \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/byrzhm/pascc-private.git --recursive

CMD ["/bin/bash"]