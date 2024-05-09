# pascc

- 预备知识
  - [pascal](docs/pascal.md)
  - [C++](docs/c++.md)

- pascc 三大模块
  - [parse 模块](docs/parse.md)
  - [semant 模块](docs/semant.md)
  - [codegen 模块](docs/codegen.md)
  

- 项目搭建
  - [环境依赖](#环境依赖)
  - [配置项目](docs/quick-configure.md)
  - [构建项目](#构建项目)

- [参考资料](#参考资料)

- [小组成员](#小组成员)


## 环境依赖

- OS: ubuntu 22.04 / MacOS
- Compiler: gcc 11.4.0 / clang 14
- bison: 3.8.2
- flex: 2.6.4
- cmake 3.10+
- [graphviz 10.0.1](https://graphviz.org/)
- [doxygen 1.10.0](https://www.doxygen.nl/)
- 3rdparty libraries:
  - [argparse](https://github.com/p-ranav/argparse)
  - [fmt](https://github.com/fmtlib/fmt)
  - [googletest](https://github.com/google/googletest)

## 构建项目

``` sh
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build --config Release --target all -j 4 --
```

## 参考资料

- [postgres](https://github.com/postgres/postgres/blob/master/src/backend/parser/scan.l#L409)
- [graphviz dotguide](https://graphviz.org/pdf/dotguide.pdf)

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
