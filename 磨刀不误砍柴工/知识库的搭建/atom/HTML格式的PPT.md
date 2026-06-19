> 文本是Agent的语言，用 AI 处理文本比处理文件准确、方便，做PPT亦是如此。

# 前期准备

## 生成故事框架

提示词：

```
我想做一个xxx的Markdown文档，用于生成HTML网页ppt，我的内容有

请你参照一份xxx应该有的元素，帮我生成这份Markdown文件，不要超过x页，每页内容不要太多，要简洁
```

反复修改Markdown格式文案，直到满意为止

## 安装skill与导入模板

选择性参考 [CC-skills-MCP配置](../AI与知识库配置/CC-skills-MCP配置.md) 的内容

| skill名称                  | 功能                       | GitHub 仓库                                                                                  |
| ------------------------ | ------------------------ | ------------------------------------------------------------------------------------------ |
| Frontend-Design          | 创建具有独特风格、生产级品质且设计精良的前端界面 | [anthropics/skills](https://github.com/anthropics/skills/tree/main/skills/frontend-design) |
| beautiful-html-templates | 精美HTML模版                 | [beautiful-html-templates.](https://github.com/zarazhangrui/beautiful-html-templates)      |

模版skill提示词：

```
帮我修改Frontend-Design skill，生成PPT时调用xxx/beautiful-html-templates/AGENT.md里面的模版
```

## 准备素材库

图表、图片、视频等

# AGENT制作

## 制作PPT

1. Claude-Code调用 `/frontend-design` 这个skill

2. 回答问题，做PPT

3. （选做）加入JavaScript交互操作

4. （选做）使用图片（视频）占位符或填入图片（视频）位置（直接拖取图片需要多模态的模型）

## 编辑文字、图片或视频

1. 使用 [栗氪聊AI-HTML PPT编辑器](https://html-ppt-edit.liketalkai.top/) 进行在线编辑

2. 通过提示词加入编辑功能（需反复修改）

```
给这个xxx的HTML加入可编辑工具。

1. 我希望里面的字可以增删、调节字号和颜色，也可以在图片位置点击替换图片，也可以移动文本框让文字上下左右移动，也可以在任意位置插入文本或图片。

2. 请你注意核对功能完成度，创建一个新的html，不要覆盖原html，并且保证版式和内容都和之前一模一样，仅仅是加了个工具箱。

3. 我还希望这个工具箱是一个小妖怪的图标，位于html页面右下角，我点击的时候就进入编辑模式，再次点击退出编辑模式。

4. 请你反复检查我的功能都已经帮我实现，且页面流畅切换丝滑，排版没有问题。
```

## 打包分享 & 部署上线

- 打包就是文件夹压缩

- 部署上线可以使用 [Netlify](https://www.netlify.com/) 进行免费上传