# parse

## bison

### useful resources

- [Declaration Summary](https://www.gnu.org/software/bison/manual/html_node/Decl-Summary.html)
- [%define summary](https://www.gnu.org/software/bison/manual/html_node/_0025define-Summary.html)
- [%code summary](https://www.gnu.org/software/bison/manual/html_node/_0025code-summary)

### Declarations

首先确定使用 LALR(1) 框架， 并且指定版本, `%header` 将语法文件中定义的语法记号导出到头文件中(默认生成同名`.hpp`文件)

``` plaintext
%skeleton "lalr1.cc"
%require "3.8"
%header
```

使得scanner返回的*token kind*与parser使用的*symbol kind*保持一致

``` plaintext
%define api.token.raw
```

在scanner中将语法符号视作一个整体。使用C++时应确保开启variant

``` plaintext
%define api.token.constructor
```

在生成定义到目标语言时，添加在token名前添加前缀

``` plaintext
%define api.token.prefix {TOK_}
```

大多数c++对象无法使用union进行存储，所以最好使用variant

``` plaintext
%define api.value.type variant
```

指定生成的parser类名

``` plaintext
%define api.parser.class {Parser}
```

指定生成的parser所在的命名空间

``` plaintext
%define api.namespace {pascc::parse}
```

运行时检查

``` plaintext
%define parse.assert
```


``` plaintext
%define parse.trace
```

控制生成语法错误信息

``` plaintext
%define parse.error detailed
```

启用[lac(lookahead correction)](https://www.gnu.org/software/bison/manual/html_node/LAC.html)，优化语法错误处理

``` plaintext
%define parse.lac full
```

生成代码处理位置

``` plaintext
%locations
```

下面的代码会直接添加到bison生成的.cpp文件中

``` plaintext
%code{
  #include "defs.h"
}
```

下面的代码会直接添加到bison生成的.hpp文件中

```plaintext
%code requires {
  #include "ast.h"
}
```

下面的代码会在调用 yylex 和 yyparse 时，传入 ParserDriver

```plaintext
%param { ParserDriver &drv }
```
