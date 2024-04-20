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
    cmake

RUN mkdir /root/.ssh/
ADD id_rsa /root/.ssh/id_rsa
RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan -T 60 github.com >> /root/.ssh/known_hosts

RUN git clone git@github.com:byrzhm/pascc-public.git --recursive

CMD ["/bin/bash"]