> 可以先用Obsidian ([Obsidian配置](Obsidian配置.md)) 编辑内容, 再使用docsify作为前端展现, 作为个人Wiki.

[官网手册](https://docsify.js.org/#/)

# 配环境

## 安装`docsify-cli`

```bash
npm i docsify-cli -g
```

## 初始化

```bash
docsify init ./docs
```

然后打开

```bash
docsify serve docs
```

地址为 `http://localhost:3000`

## index.html-template

```HTML
<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">

    <!-- Core Theme -->
    <link rel="stylesheet" href="//cdn.jsdelivr.net/npm/docsify@5/dist/themes/core.min.css">
  </head>
  <body class="loading">
    <div id="app"></div>

    <!-- Configuration -->
    <script>
      window.$docsify = {
        //...
      };
    </script>

    <!-- Docsify.js -->
    <script src="//cdn.jsdelivr.net/npm/docsify@5"></script>

    <!-- Plugins (optional) -->
    <!-- <script src="//cdn.jsdelivr.net/npm/docsify@5/dist/plugins/search.min.js"></script> -->
  </body>
</html>
```

## 使用python启动

```python
# Python 2
cd docs && python -m SimpleHTTPServer 3000
```

```python
# Python 3
cd docs && python -m http.server 3000
```

---

# 加页

## 添加md文件

在 `docs` 目录下直接新建 `.md` 文件就行. 比如新建 `guide.md`，访问地址就是：
`http://localhost:3000/#/guide`

## 侧边栏

### \_sidebar.md

```Markdown
<!-- markdownlint-disable first-line-h1 -->

- Getting started

  - [Quick start](quickstart.md)
  - [Adding pages](adding-pages.md)
  - [Cover page](cover.md)
  - [Custom navbar](custom-navbar.md)

...

* [Awesome docsify](awesome.md)
* [Changelog](changelog.md)
```

