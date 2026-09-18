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