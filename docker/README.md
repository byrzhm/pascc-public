## 构建 Docker 镜像

> 👇 注意你要将自己的私钥 id_rsa 放在当前目录下

``` sh
docker build . -t pascc
```

## 启动 Docker 容器

``` sh
docker run -it pascc
```

## 删除 Docker 容器

``` sh
docker rm 容器ID
```

## 删除 Docker 镜像

``` sh
docker image rm pascc
```
