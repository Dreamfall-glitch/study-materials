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
<!-- docs/_sidebar.md -->

- Section Header 1

  - [Home](/)
  - [Page 1](page-1.md)
```

#### .nojekyll

GitHub Pages 默认会用 **Jekyll** 来生成网站. 而Jekyll 有一个规则：忽略所有以下划线 `_` 开头的文件或文件夹.

所以建立此文件(留空即可), 保证侧边栏正常.

#### 查找

Docsify 加载侧边栏时，会按这个顺序找：

1. **先看当前页面所在的文件夹里有没有 `_sidebar.md`**。  
    有的话，就用这个文件夹里的侧边栏。
    
2. 如果当前文件夹没有，它会向上一级目录找；再没有就继续往上，直到根目录。
    
3. 如果一直找到根目录都没有，或者你配置了 `loadSidebar` 指定了某个文件，那就用你在 `index.html` 里配置的那个侧边栏文件。  
    通常 `loadSidebar: true` 时，默认就是根目录的 `docs/_sidebar.md`

### 启用

```HTML
<!-- index.html -->

<script>
  window.$docsify = {
    loadSidebar: true,
  };
</script>
```

### 嵌套

在每个子文件夹里放一个 `_sidebar.md`

Docsify 会优先使用当前目录的 `_sidebar.md`，没有就向父级目录找

如果不想找:

```HTML
<script>
  window.$docsify = {
    loadSidebar: true,
    alias: {
      '/.*/_sidebar.md': '/_sidebar.md',
    },
  };
</script>
```

在子目录中创建一个 README.md 文件，并将其用作该路由的着陆页

### 添加页标题

```Markdown
<!-- docs/_sidebar.md -->

- [Home](/)
- [Guide](guide.md 'The greatest guide in the world')
```

### 自动生成目录(Markdown 里的标题会自动变成侧边栏目录)

```HTML
<script>
  window.$docsify = {
    loadSidebar: true,
    subMaxLevel: 2,
    <!-- 表示最多把二级标题加入目录 -->
  };
</script>
```

#### 忽略某个

add `<!-- {docsify-ignore} -->` to it, 如

`## Header <!-- {docsify-ignore} -->`

---

# 封面Cover

## 基本设置

Set `coverpage` to **true**, and create a `_coverpage.md`:

```js
window.$docsify = {
  coverpage: true,
};
```

```markdown
<!-- _coverpage.md -->

![logo](_media/icon.svg)

# docsify

> A magical documentation site generator

- Simple and lightweight
- No statically built HTML files
- Multiple themes

[GitHub](https://github.com/docsifyjs/docsify/)
[Get Started](#docsify)
```

## 背景

### `_coverpage.md`添加

```markdown
<!-- background color -->

![color](#f0f0f0)
```

```markdown
<!-- background image -->

![](_media/bg.png)
```

## 独立封面页

把封面页当作整个网站的入口，用户必须点击封面上的链接（比如某个外部链接或 `#/xxx` 路由）才能去其他页面

```js
window.$docsify = { 
	onlyCover: false,
};
```

## 多语言不同封面

If your docs site is in more than one language, it may be useful to set multiple covers.

For example, your docs structure is like this

```text
.
└── docs
    ├── README.md
    ├── guide.md
    ├── _coverpage.md
    └── zh-cn
        ├── README.md
        └── guide.md
        └── _coverpage.md
```

Now, you can set

```js
window.$docsify = {
  coverpage: ['/', '/zh-cn/'],
};
```

Or a special file name

```js
window.$docsify = {
  coverpage: {
    '/': 'cover.md',
    '/zh-cn/': 'cover.md',
  },
};
```

---

# 导航栏

## HTML

通过 `#/` 链接导航

```html
<!-- index.html -->

<body>
  <nav>
    <a href="#/">EN</a>
    <a href="#/zh-cn/">简体中文</a>
  </nav>
  <div id="app"></div>
</body>
```

## Markdown

通过开启`loadNavbar`, 在`_navbar.md`编写

```html
<!-- index.html -->

<script>
  window.$docsify = {
    loadNavbar: true,
  };
</script>
<script src="//cdn.jsdelivr.net/npm/docsify@5/dist/docsify.min.js"></script>
```

```markdown
<!-- _navbar.md -->

- [En](/)
- [chinese](/zh-cn/)
```

下拉菜单:

```markdown
<!-- _navbar.md -->

- Translations
  
	- [En](/)
	- [chinese](/zh-cn/)
```
