# 快速配置

## 虚拟机配置

``` shell
sudo apt-get -y update && sudo apt-get -y install openssh-server net-tools # 下载ssh服务器和ifconfig工具(在net-tools中)
ifconfig # 查看虚拟机服务器ip
```

> 在本机`cmd/terminal`中使用`ssh-keygen`生成ssh密钥，在服务器的`~/.ssh/`下添加`authorized_keys`文件，将公钥(`id_rsa.pub`)的内容复制粘贴于其中。（完成了免密登录）

## 拉取代码

在虚拟机服务器中使用`ssh-keygen`生成密钥，将公钥放在github上。
使用下面的指令拉取代码：

```shell
git clone git@github.com:byrzhm/pascc-private.git
```

## 搭建环境

``` shell
sudo apt-get -y update && sudo apt-get -y install build-essential clang flex bison cmake git fp-compiler graphviz doxygen
git submodule update --init --recursive
```

## 配置git

``` shell
# 配置 git user.name 和 user.email
git config user.name "youername" # --global
git config user.email "youremail" # --global
git config pull.rebase false
```
