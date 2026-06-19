# Claude Code 多模态工具链配置手册

## 概述

本配置为 Claude Code（通过 CC Switch 使用 DeepSeek Flash）增加了**网页搜索、网页抓取、PDF 读取、图片识别（视觉桥接）和图片生成**等五大多模态能力。核心思路是：**用 MCP 服务器扩展工具能力，用 Skills 扩展工作流指导**。

---

## 一、准备工作

### 1.1 环境要求

- Node.js >= 18
- Claude Code >= 2.1.140
- 操作系统：Windows 11（本配置基于 Windows 编写，macOS/Linux 需调整 `command` 格式）

### 1.2 注册账号（需人工操作）

| 服务 | 用途 | 注册地址 |
|------|------|----------|
| 硅基流动 SiliconFlow | Luma MCP 图片识别（免费 DeepSeek-OCR） | https://siliconflow.cn |
| 火山引擎 | jimenggen-mcp 即梦 AI 图片生成 | https://console.volcengine.com |

注册后获取 API Key 备用。硅基流动的 DeepSeek-OCR 有免费额度，火山引擎即梦 AI 新用户有免费体验额度。

---

## 二、MCP 服务器配置

MCP 服务器为 Claude Code 提供**外部工具调用能力**。配置方式有两种，选其一即可。

### 2.1 方式一：通过 claude mcp add 命令安装

```bash
# 网页搜索 - bing-cn-mcp-enhanced（使用 Playwright 绕过必应反爬）
claude mcp add bing-search -- cmd /c npx -y bing-cn-mcp-enhanced

# 网页抓取 - markfetch（URL 转 Markdown）
claude mcp add fetch -- npx -y markfetch

# PDF 读取 - pdf-reader-mcp
claude mcp add pdf-reader -- npx -y @sylphlab/pdf-reader-mcp

# 图片识别 - Luma MCP（视觉桥接，将图片发给外部视觉模型处理）
claude mcp add luma-vision -e MODEL_PROVIDER=siliconflow -e SILICONFLOW_API_KEY=你的硅基流动Key -- npx -y luma-mcp

# 图片生成 - jimenggen-mcp（字节跳动即梦 AI）
claude mcp add jimenggen -e JIMENG_ACCESS_KEY=你的AccessKey -e JIMENG_SECRET_KEY=你的SecretKey -- npx -y jimenggen-mcp
```

**Windows 注意事项**：
- `claude mcp add` 命令有时会将 `/c` 参数错误转换为 `C:/`，导致连接失败。需要手动编辑配置文件修复。
- 若出现此问题，在 `C:\Users\用户名\.claude.json` 中找到对应条目，将 `"C:/"` 改回 `"/c"` 即可。

### 2.2 方式二：手动编辑配置文件

将以下内容加入 `C:\Users\用户名\.claude.json` 的项目配置节点中：

```json
{
  "mcpServers": {
    "bing-search": {
      "type": "stdio",
      "command": "cmd",
      "args": ["/c", "npx", "-y", "bing-cn-mcp-enhanced"],
      "env": {}
    },
    "fetch": {
      "type": "stdio",
      "command": "npx",
      "args": ["-y", "markfetch"],
      "env": {}
    },
    "pdf-reader": {
      "type": "stdio",
      "command": "npx",
      "args": ["-y", "@sylphlab/pdf-reader-mcp"],
      "env": {}
    },
    "luma-vision": {
      "type": "stdio",
      "command": "npx",
      "args": ["-y", "luma-mcp"],
      "env": {
        "MODEL_PROVIDER": "siliconflow",
        "SILICONFLOW_API_KEY": "你的硅基流动API Key"
      }
    },
    "jimenggen": {
      "type": "stdio",
      "command": "npx",
      "args": ["-y", "jimenggen-mcp"],
      "env": {
        "JIMENG_ACCESS_KEY": "你的火山引擎AccessKey",
        "JIMENG_SECRET_KEY": "你的火山引擎SecretKey"
      }
    }
  }
}
```

**Windows 特有规则**：
- 使用 `cmd /c npx -y package-name` 格式（对应 `"command": "cmd"`, `"args": ["/c", "npx", "-y", "package-name"]`）
- 部分 npm 包（如 markfetch、luma-mcp、jimenggen-mcp）可直接用 `npx` 启动，不需要 `cmd /c` 包装

---

## 三、各 MCP 功能说明

### 3.1 bing-search（网页搜索）
- **包名**：`bing-cn-mcp-enhanced`
- **功能**：使用必应中文搜索引擎实时搜索网络信息
- **工具**：`bing_search`（搜索）、`crawl_webpage`（网页抓取）
- **首次启动**：自动下载 Chromium 浏览器（约 150-180MB）
- **费用**：完全免费

### 3.2 fetch（网页抓取）
- **包名**：`markfetch`
- **功能**：抓取单个 URL 的网页内容，HTML 转 Markdown
- **工具**：`fetch`
- **费用**：完全免费

### 3.3 pdf-reader（PDF 读取）
- **包名**：`@sylphlab/pdf-reader-mcp`
- **功能**：读取 PDF 文件文本内容、元数据、页数
- **工具**：`read_pdf`
- **费用**：完全免费

### 3.4 luma-vision（图片识别/视觉理解）
- **包名**：`luma-mcp`
- **功能**：将图片发送给外部视觉模型，返回文字描述（解决 DeepSeek Flash 无原生视觉能力）
- **后端模型**：硅基流动 SiliconFlow 的 DeepSeek-OCR（免费方案）
- **工具**：`image_understand` 等视觉分析工具
- **费用**：DeepSeek-OCR 免费额度充足

### 3.5 jimenggen（图片/视频生成）
- **包名**：`jimenggen-mcp`
- **功能**：调用字节跳动即梦 AI 生成图片和视频
- **模型**：jimeng_t2i_v40、jimeng_t2i_v31（文生图）等
- **工具**：`text_to_image`、`image_to_image`、`text_to_video`
- **费用**：火山引擎按量付费，新用户有免费额度

---

## 四、Skills 配置

Skills 是 Markdown 格式的**工作流程指导文件**，告诉 Claude Code 怎么做某件事。

### 4.1 安装方法

```bash
# 安装科学技能集（含 markitdown 等 138 个 skill）
npx skills add K-Dense-AI/scientific-agent-skills --agent claude-code
```

### 4.2 核心 Skill —— markitdown

| Skill      | 来源                                 | 用途                                    |
| ---------- | ---------------------------------- | ------------------------------------- |
| markitdown | K-Dense-AI/scientific-agent-skills | PDF/DOCX/PPTX/XLSX/HTML/图片等转 Markdown |

注：`npx skills add` 安装的是完整技能集（含 138 个技能），markitdown 是核心工具。

### 4.3 优质 Skill 安装

根据 [Claude-Code配置](Claude-Code配置.md#1.%20Skill%20技能扩展) 中推荐的优质 skill，可下载并安装到全局 skill 目录（可以选择手动也可以交给agent自动安装）

| skill名称         | 功能                                     | GitHub 仓库                                                                                  |
| --------------- | -------------------------------------- | ------------------------------------------------------------------------------------------ |
| Find-Skill      | 根据用户需求，查找和安装来自 agent skill 开放生态的技能     | [vercel-labs/skills](https://github.com/vercel-labs/skills/tree/main/skills/find-skills)   |
| Frontend-Design | 创建具有独特风格、生产级品质且设计精良的前端界面               | [anthropics/skills](https://github.com/anthropics/skills/tree/main/skills/frontend-design) |
| Skill-Creator   | 创建新 skill、修改和改进现有 skill，并衡量 skill 表现   | [anthropics/skills](https://github.com/anthropics/skills/tree/main/skills/skill-creator)   |
| Karpathy-Skill  | 依据卡帕西经验总结，用于提升 Claude Code 编码表现的 skill | [multica-ai/andrej-karpathy-skills](https://github.com/multica-ai/andrej-karpathy-skills)  |

#### **安装步骤**

```bash
# 1. 创建全局 skill 目录
mkdir -p ~/.claude/skills

# 2. 下载 Find-Skill（从 vercel-labs 仓库）
cd ~/.claude/skills && git clone --depth 1 https://github.com/vercel-labs/skills.git vercel-temp
cp -r vercel-temp/skills/find-skills ~/.claude/skills/
rm -rf vercel-temp

# 3. 下载 Frontend-Design 和 Skill-Creator（从 anthropics 仓库）
cd ~/.claude/skills && git clone --depth 1 https://github.com/anthropics/skills.git anthropic-temp
cp -r anthropic-temp/skills/frontend-design ~/.claude/skills/
cp -r anthropic-temp/skills/skill-creator ~/.claude/skills/
rm -rf anthropic-temp

# 4. 下载卡帕西 Skill
cd ~/.claude/skills && git clone --depth 1 https://github.com/forrestchang/andrej-karpathy-skills.git karpathy-skill
```

#### **安装后目录结构**
```
~/.claude/skills/
├── find-skills/
│   └── SKILL.md
├── frontend-design/
│   └── SKILL.md
├── skill-creator/
│   └── SKILL.md
└── karpathy-skill/
    ├── CLAUDE.md
    ├── README.md
    └── skills/
```

#### **核心 Skill 提炼**

1. Find-Skill

用于发现和安装 agent skills 的工具：

```bash
npx skills find [query]     # 搜索 skills
npx skills add <package>    # 安装 skill
npx skills check            # 检查更新
npx skills update           # 更新所有 skills
```

- **使用场景**：用户问"如何做 X"、"找一个能做 X 的 skill"、"你能做 X 吗"
- **推荐来源**：vercel-labs/agent-skills、anthropics/skills、ComposioHQ/awesome-claude-skills
- **搜索优先级**：先查 skills.sh 排行榜，再用 CLI 搜索；推荐前验证质量（安装量 1K+、来源可信度）

2. Karpathy-skill

具体见 [Karpathy-Code指南](Karpathy-Code指南.md)

核心准则：**倾向于谨慎而非速度**。简单任务可灵活处理。

|原则|解决什么问题|
|---|---|
|**编码前思考**|错误假设、隐藏困惑、缺少权衡|
|**简洁优先**|过度复杂、臃肿抽象|
|**精准修改**|无关编辑、触碰不应碰的代码|
|**目标驱动执行**|通过测试优先、可验证的成功标准|

### 4.4 查找更多 Skill

```bash
npx skills find [关键词]
# 或访问 https://skills.sh/
```

---

## 五、配置验证

```bash
# 检查所有 MCP 是否连接成功
claude mcp list
```

预期输出：全部 5 个 MCP 均为 `✓ Connected`

### 使用示例

```
# 搜索信息
使用 bing_search 搜索'关键词'

# 抓取网页
抓取 https://example.com 的网页内容

# 图片识别（重要：需显式调用 MCP 工具）
用 luma-vision 识别 E:/图片路径/图片.png

# 读取 PDF
读取 E:/文档/文档.pdf 的第 1 页内容

# 生成图片
生成一张图片：提示词描述
```

**注意**：DeepSeek Flash 是纯文本模型，图片识别需在提示词中**显式指定调用 luma-vision MCP**，模型不会自动识别图片。

---

## 六、费用汇总

| 项目 | 费用 |
|------|------|
| bing-cn-mcp-enhanced | 完全免费 |
| markfetch | 完全免费 |
| pdf-reader-mcp | 完全免费 |
| Luma MCP + SiliconFlow | DeepSeek-OCR 免费额度 |
| jimenggen-mcp（即梦 AI） | 按量付费（新用户有免费额度） |
| Skills | 完全免费 |

**预估月费**：除即梦 AI 生图外全部免费。图片生成轻度使用（每月数十张）约 5-20 元/月。

---

## 七、常见问题

### 7.1 MCP 连接失败
```bash
claude mcp list          # 查看状态
claude mcp remove 服务器名  # 删除后重加
```

### 7.2 /c 被转为 C:/
手动编辑 `C:\Users\用户名\.claude.json` 修复。

### 7.3 bing-cn-mcp-enhanced 首次启动慢
正在下载 Chromium（~150MB），此后正常。

### 7.4 更换视觉模型提供商
Luma MCP 支持切换后端：
- `MODEL_PROVIDER=zhipu` + `ZHIPU_API_KEY=xxx`（智谱 GLM-4.6V）
- `MODEL_PROVIDER=aliyun` + `DASHSCOPE_API_KEY=xxx`（通义千问 Qwen-VL）

### 7.5 加密/密码保护 PDF 无法读取
**问题**：部分 PDF（如知网 CNKI 下载的学术论文）带有密码保护或 DRM 加密，`@sylphlab/pdf-reader-mcp`（以及绝大多数 MCP 工具）不支持密码参数，直接读取会失败。

**解决方案**：使用 Node.js 的 `pdf-parse@1.1.1` 库（旧版，API 简洁）回退提取文本。

```bash
# 临时安装 pdf-parse（安装到任意临时目录即可）
cd /tmp && npm install pdf-parse@1.1.1 --silent

# 提取 PDF 文本内容
node -e "
const fs = require('fs');
const pdf = require('pdf-parse');
const data = fs.readFileSync('目标PDF文件路径.pdf');
pdf(data).then(d => {
  console.log('页数:', d.numpages);
  console.log(d.text);
}).catch(e => console.error('失败:', e.message));
"
```

**原理**：`pdf-parse@1.1.1` 底层使用 `pdf.js` 解析引擎，能绕过部分简单的 PDF 加密限制提取文本内容。注意：
- 此方法只能提取**文本层**内容，无法提取图片、图表等非文本元素
- 对于需要密码才能打开的强加密 PDF 仍然无效
- 提取后需手动清理作者信息、参考文献、图片引用标记等无关内容

**推荐流程**：
1. 优先使用 `@sylphlab/pdf-reader-mcp` 读取无保护的 PDF（MCP 方式更方便）
2. 若 MCP 无法读取（加密/保护），退而使用 `pdf-parse@1.1.1` Node.js 方案
3. 若仍无法读取，尝试 OCR 方案：先用工具将 PDF 转图片，再用 Luma MCP 的视觉识别能力逐一识别

### 7.6 npx 缓存问题
```bash
npx clear-npx-cache
```

---

## 八、移植到新环境

将此手册投喂给 Claude Code，准备好以下信息即可自动完成全部配置：

1. 硅基流动 API Key（Luma MCP 图片识别）
2. 火山引擎 AccessKey + SecretKey（即梦 AI 图片生成）
3. Node.js 已安装

Claude Code 将依次执行：
1. 运行 `claude mcp add` 安装 5 个 MCP 服务器
2. 检查并修复 Windows 下 `/c` 参数转换 bug
3. 运行 `npx skills add K-Dense-AI/scientific-agent-skills` 安装技能
4. 运行 `claude mcp list` 验证全部连接成功
5. 填入 API Key 到环境变量中