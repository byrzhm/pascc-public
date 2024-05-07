# pascc

## 快速配置

### 虚拟机配置

``` shell
sudo apt-get -y update && sudo apt-get -y install openssh-server net-tools # 下载ssh服务器和ifconfig工具(在net-tools中)
ifconfig # 查看虚拟机服务器ip
```

> 在本机`cmd/terminal`中使用`ssh-keygen`生成ssh密钥，在服务器的`~/.ssh/`下添加`authorized_keys`文件，将公钥(`id_rsa.pub`)的内容复制粘贴于其中。（完成了免密登录）

### 拉取代码

在虚拟机服务器中使用`ssh-keygen`生成密钥，将公钥放在github上。
使用下面的指令拉取代码：

```shell
git clone git@github.com:byrzhm/pascc-private.git
```

### 搭建环境

``` shell
sudo apt-get -y update && sudo apt-get -y install build-essential clang flex bison cmake git fp-compiler graphviz doxygen
git submodule update --init --recursive
```

### 配置git

``` shell
# 配置 git user.name 和 user.email
git config user.name "youername" # --global
git config user.email "youremail" # --global
git config pull.rebase false
```

### 构建

``` sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -S . -B build
cmake --build build --config Release --target all -j 4 --
```

## 小组成员

<!--

[![Contributors](https://contributors-img.web.app/image?repo=byrzhm/pascc-public)](https://github.com/byrzhm/pascc-private/graphs/contributors)

-->

<table>
  <tr>
    <td>
        <a href="https://github.com/byrzhm">
        <figure>
        <img src="https://github.com/byrzhm.png" alt="朱宏明" width="100" height="100">
        <figcaption style="text-align: center;">朱宏明</figcaption>
        </figure>
        </a>
    </td>
    <td>
        <a href="https://github.com/TALON416">
        <figure>
        <img src="https://github.com/TALON416.png" alt="张新博" width="100" height="100">
        <figcaption style="text-align: center;">张新博</figcaption>
        </figure>
        </a>
    </td>
    <td>
        <a href="https://github.com/micropuma">
        <figure>
        <img src="https://github.com/micropuma.png" alt="窦力扬" width="100" height="100">
        <figcaption style="text-align: center;">窦力扬</figcaption>
        </figure>
        </a>
    </td>
  </tr>
  <tr>
    <td>
        <a href="https://github.com/2645012179">
        <figure>
        <img src="https://github.com/2645012179.png" alt="曾辉" width="100" height="100">
        <figcaption style="text-align: center;">曾辉</figcaption>
        </figure>
        </a>
    </td>
    <td>
        <a href="https://github.com/KanameHomu">
        <figure>
        <img src="https://github.com/KanameHomu.png" alt="马方舟" width="100" height="100">
        <figcaption style="text-align: center;">马方舟</figcaption>
        </figure>
        </a>
    </td>
    <td>
        <a href="https://github.com/duoaidaoc">
        <figure>
        <img src="https://github.com/duoaidaoc.png" alt="夏博焕" width="100" height="100">
        <figcaption style="text-align: center;">夏博焕</figcaption>
        </figure>
        </a>
    </td>
  </tr>
    <tr>
    <td>
        <a href="https://github.com/LeaveryF">
        <figure>
        <img src="https://github.com/LeaveryF.png" alt="伏培燚" width="100" height="100">
        <figcaption style="text-align: center;">伏培燚</figcaption>
        </figure>
        </a>
    </td>
  </tr>
</table>

## References

- [postgres](https://github.com/postgres/postgres/blob/master/src/backend/parser/scan.l#L409)
- [graphviz dotguide](https://graphviz.org/pdf/dotguide.pdf)