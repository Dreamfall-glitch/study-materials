# 一、初步上手

## 1. 部署前准备

首先需要下载和安装一个 **Agent IDE**（智能体集成开发环境）

这里就用 **VS Code** ，新建/打开个人项目文件夹


## 2. 安装Claude Code

### **方式 1：终端命令行安装**（需魔法上网）

根据自身系统，选择对应代码，在IDE终端输入并回车：

#### **Windows：**

**步骤 1：**在ide终端用WinGet先安装Git，遇到选项直接输入`y`（yes）

```cmd
winget install Git.Git
```

**步骤 2**：在ide终端输入代码，下载Claude code

- **Windows - PowerShell（ide终端默认用这个）：**
  

```powershell
irm https://claude.ai/install.ps1 | iex
```

- **Windows - CMD：**

```cmd
curl -fsSL https://claude.ai/install.cmd -o install.cmd && install.cmd && del install.cmd
```

- 下载好以后进行版本验证，**版本号正常显示，代表安装完成！**
  

```cmd
claude --version
```

#### **macOS、Linux：**

- 直接在ide终端输入以下代码
  

```bash
curl -fsSL https://claude.ai/install.sh | bash
```

- 下载好以后进行版本验证
  

```bash
claude --version
```

### **方式 2：无魔法安装Claude code**

#### **Windows：**

**步骤 1：**用WinGet先安装Git，遇到选项直接y（yes）

```cmd
winget install Git.Git
```

**步骤 2**：在ide agent输入以下提示词，让agent一条龙搞定Claude code安装（使用VS Code 的 Github Copilot）

```
执行这条代码安装Claude code：winget install Anthropic.ClaudeCode
安装完后，把Claude Code的可执行文件路径配置到系统环境变量的Path里。
```

具体安装过程：

```bash
User: 执行这条代码安装Claude code：winget install Anthropic.ClaudeCode
安装完后，把Claude Code的可执行文件路径配置到系统环境变量的Path里。

GitHub Copilot: 我将先用 `winget` 安装 `Anthropic.ClaudeCode`，然后用 `Get-Command/where.exe` 找到实际的可执行文件位置，最后把该目录追加到系统级 `Path`（如果当前终端未提升权限，会提示并改为写入用户级 `Path` 作为替代）。

Ran terminal command: winget install -e --id Anthropic.ClaudeCode

安装命令看起来没回显信息；我将加上 `--accept-*` 参数重跑一次，并确保拿到安装结果输出。

Ran terminal command: winget install -e --id Anthropic.ClaudeCode --accept-package-agreements --accept-source-agreements

当前环境里 `winget` 命令不存在；我先确认系统是否装了“应用安装程序（App Installer）/ Microsoft.DesktopAppInstaller”，这是 `winget` 的来源，然后再决定下一步安装路径。

Ran terminal command: Get-Command winget -ErrorAction SilentlyContinue | Format-List *; Get-AppxPackage -Name Microsoft.DesktopAppInstaller | Select-Object Name, Version, PackageFullName

已确认 `winget.exe` 实际存在于 `WindowsApps` 目录；为避免“执行别名/Path”导致的识别问题，我将用完整路径调用 `winget.exe` 来安装 Claude Code。

Ran terminal command: $winget = "$env:LOCALAPPDATA\Microsoft\WindowsApps\winget.exe"; & $winget install -e --id Anthropic.ClaudeCode --accept-package-agreements --accept-source-agreements

Claude Code 看起来已安装完成；下一步我会定位它的 CLI 可执行文件（如 `claude.exe`）所在目录，然后把该目录追加进系统级 `Path`。

Ran terminal command: Get-Command claude -ErrorAction SilentlyContinue | Format-List Name,CommandType,Source,Path; where.exe claude 2>$null; where.exe claude.exe 2>$null

`claude` 命令当前没解析到；我将检查（1）`WindowsApps` 里的“执行别名”文件是否存在，以及（2）系统里的 MSIX 包安装信息（含 InstallLocation），从而确定需要加入 Path 的真实目录。

Ran terminal command: $wa = "$env:LOCALAPPDATA\Microsoft\WindowsApps"; Get-ChildItem $wa -ErrorAction SilentlyContinue | Where-Object { $_.Name -match 'claude|anthropic' } | Select-Object Name, FullName | Format-Table -AutoSize; 
Get-AppxPackage | Where-Object { $_.Name -match 'Claude|Anthropic' -or $_.PublisherDisplayName -match 'Anthropic' } | Select-Object Name, PublisherDisplayName, Version, InstallLocation | Format-List

没有搜到明显的 AppX/MSIX 包信息；我接下来用 `winget show` 查看该包的安装器类型与可能的安装目录线索，然后再做精准的 Path 配置。

Ran terminal command: $winget = "$env:LOCALAPPDATA\Microsoft\WindowsApps\winget.exe"; & $winget show -e --id Anthropic.ClaudeCode

从 `winget show` 看这是 `portable` 安装器（直接下载 `claude.exe`）；接下来我会在 WinGet 的默认 `Packages/Links` 目录里定位 `claude.exe`，然后把它所在目录写入系统级 `Path`。

Ran terminal command: $base = Join-Path $env:LOCALAPPDATA 'Microsoft\WinGet'; $candidates = @(
  (Join-Path $base 'Links'),
  (Join-Path $base 'Packages'),
  (Join-Path $base 'PortableLinks'),
  (Join-Path $base 'PortablePackages')
);
$candidates | ForEach-Object { if (Test-Path $_) { "-- $_"; Get-ChildItem -Path $_ -Recurse -File -ErrorAction SilentlyContinue | Where-Object { $_.Name -match '^claude(\.exe)?$' -or $_.Name -match 'claude' } | Select-Object FullName, Length, LastWriteTime | Sort-Object LastWriteTime -Descending | Select-Object -First 20 | Format-Table -AutoSize } }


Ran terminal command: $targetDir = Join-Path $env:LOCALAPPDATA 'Microsoft\WinGet\Links';
if (!(Test-Path $targetDir)) { throw "目标目录不存在: $targetDir" }

$inMachine = ($([Environment]::GetEnvironmentVariable('Path','Machine') ?? '') -split ';') -contains $targetDir
$inUser    = ($([Environment]::GetEnvironmentVariable('Path','User') ?? '') -split ';') -contains $targetDir

$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
"TargetDir: $targetDir"; "IsAdmin: $isAdmin"; "InMachinePath: $inMachine"; "InUserPath: $inUser";

if (-not $inMachine) {
  try {
    $machinePath = [Environment]::GetEnvironmentVariable('Path','Machine')
    if ([string]::IsNullOrWhiteSpace($machinePath)) { $machinePath = '' }
    $newMachinePath = if ($machinePath.TrimEnd(';') -eq '') { $targetDir } else { $machinePath.TrimEnd(';') + ';' + $targetDir }
    [Environment]::SetEnvironmentVariable('Path', $newMachinePath, 'Machine')
    "✅ 已写入系统级 Path (Machine)."
  } catch {
    "⚠️ 写入系统级 Path 失败：$($_.Exception.Message)"
    "   将改为写入用户级 Path (User) 作为替代。"
    if (-not $inUser) {
      $userPath = [Environment]::GetEnvironmentVariable('Path','User')
      if ([string]::IsNullOrWhiteSpace($userPath)) { $userPath = '' }
      $newUserPath = if ($userPath.TrimEnd(';') -eq '') { $targetDir } else { $userPath.TrimEnd(';') + ';' + $targetDir }
      [Environment]::SetEnvironmentVariable('Path', $newUserPath, 'User')
      "✅ 已写入用户级 Path (User)."
    } else {
      "ℹ️ 用户级 Path 已包含该目录，无需重复写入。"
    }
  }
} else {
  "ℹ️ 系统级 Path 已包含该目录，无需重复写入。"
}

# 让当前会话立即可用
if (($env:Path -split ';') -notcontains $targetDir) { $env:Path = $env:Path.TrimEnd(';') + ';' + $targetDir }

"--- 验证 ---";
Get-Command claude -ErrorAction SilentlyContinue | Select-Object Name, Source, Version | Format-List;
& claude --version 2>$null


Path 写入已成功并且 `claude --version` 正常返回；我再做一次系统级 Path 复核，确认该目录确实已持久化写入且没有被写到错误的范围。

Ran terminal command: $targetDir = Join-Path $env:LOCALAPPDATA 'Microsoft\WinGet\Links';
$machinePath = [Environment]::GetEnvironmentVariable('Path','Machine');
$hits = ($machinePath -split ';') | Where-Object { $_ -eq $targetDir };
"MachinePathContains: $($hits.Count -ge 1) (count=$($hits.Count))";
"UserPathContains: $((([Environment]::GetEnvironmentVariable('Path','User')) -split ';') -contains $targetDir)";
"Resolved: $((Get-Command claude).Source)";

已完成安装并把 Claude Code 配到系统环境变量 `Path` 里了。

- 安装：通过 `winget` 确认 `Anthropic.ClaudeCode` 已安装（当前版本 `2.1.140`）。
- 可执行文件位置：claude.exe
- `Path` 配置：已将目录 Links 写入系统级 `Path (Machine)`（复核命中次数 `count=1`）。
- 验证：当前终端可直接运行 `claude --version`，输出 `2.1.140 (Claude Code)`。

如需在其它新开的终端/程序里生效，建议新开一个终端窗口（或重启 VS Code/PowerShell 会话）后再用 `claude --version` 验证一次。
```

**步骤 3**：装完后，重启ide，验证Claude code版本号，确认安装成功

```cmd
claude --version
```

#### **macOS、Linux：**

**步骤 1**：在ide终端先装Homebrew

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**步骤 2**：向ide agent输入提示词：

```bash
帮我把homebrew加到PATH路径变量里面去
```

**步骤 3**：在ide终端输入代码，安装Claude Code

```bash
brew install --cask claude-code@latest
```

**步骤 4：**验证Claude code版本号，确认安装成功

```bash
claude --version
```

## 3. 配置大模型

- 下载CC Switch做多模型切换和管理
  **CC Switch的GitHub仓库下载页：**https://github.com/farion1231/cc-switch/releases/tag/v3.14.1
  **最新版本号：**v3.14.1
  （建议设置`开机自启`）

**步骤1：**在下载页，根据系统选择对应下载包![image-20260513140343410](C:\Users\Master\AppData\Roaming\Typora\typora-user-images\image-20260513140343410.png)

**步骤2：**安装完成后，**务必在打开Claude Code之前**，优先设置CC Switch。在CC Switch的Claude Code页面添加API Key供应商：![](assets/Claude%20Code配置/file-20260513141130342.png)

**步骤3：**以 **DeepSeek** 为例，选择对应厂商（中国版），然后填写API Key并配置模型：
![](assets/Claude%20Code配置/file-20260513145427547.png)
可进行`模型测试`，会提示**运行正常**
![697](assets/Claude%20Code配置/file-20260513145513665.png)
可进行`配置用量查询`,勾选后即可查看余额
![](assets/Claude%20Code配置/file-20260513145751509.png)

**步骤4：**选择「启用」设置好的API，CC的大模型配置完成！下一步可以准备打开Claude Code了：

## 4. 基础实操

**步骤1：**在IDE终端打开CC，输入：

```powershell
claude
```

**步骤2：**根据个人喜好设置皮肤与主题，然后一路 `yes` 后，进入CC主界面：
![](assets/Claude%20Code配置/file-20260513151843295.png)

**步骤3：**默认情况下，CC接到任务后会进入 **计划模式（Plan Mode）**：

**步骤4：**使用shift+tab，可以让CC在计划模式、默认模式和Accept Edits模式之间来回切换

|模式|功能|
|---|---|
|默认模式|启动 Claude Code 后的初始交互状态，类似于一个增强版的对话终端，用于意图确认、信息查询和简单任务。|
|计划模式|Claude 进行复杂的代码更改时，它会进入这一阶段。分析需求并制定执行步骤。不直接动代码。|
|Accept Edits模式|这是 Claude Code 的“落地”阶段，会执行代码变更的最终确认与写入|

额外提醒：

1. 如果希望CC能一路绿灯执行所有操作，需要输入指令`/exit` 退出重启后，在启动CC时输入以下命令

```powershell
claude --dangerously-skip-permissions
```

2. CC在Accept Edits模式下，执行终端命令时，下图三个选项的含义依次分别是：
   
    **仅同意这一次**的命令执行
    
    **同意**，且该项目之后执行项目依赖安装时，不再询问
    
    **不同意**，再商量
    
    ![](assets/Claude%20Code配置/file-20260513155555667.png)
    
3. 对话过程中要想执行终端操作，可以通过*新建终端*或者*对话开头加入`!`*for bash mode

## 5. 如何提供文件给CC

### **方式 1：本地文件**

使用 `@` 指令让CC进行本地文件信息查找：

### **方式 2：图片**

直接拖拽图片至对话框，或复制/粘贴：

- **Windows：**`Alt + V`
  
- **macOS：**`Command + V`

### **方式 3：多行文本输入**

在CC文本框内换行的快捷键（**不是** Shift + Enter）：

- **Windows：**`Ctrl + Enter`
  
- **macOS：**`Option + Enter`

> 可新建本地文档，写入要求后 `@` 提供长提示词

## 6. 指令大全

|Claude指令|功能说明|
|---|---|
|`/help`|提供所有指令，以及指令背后遵循的意思|
|`/model`|**切换高中低档模型**|
|`/btw`|**By the way缩写，可以暂时切出正在执行的项目，隔离上下文，方便使用者与CC进行临时对话。会话完毕后，可按Esc消除临时会话**|
|`/simplify`|**输入后会派生出3个agent，从代码质量、运行效率和复用性三个角度做一次代码审核，然后自动优化修改**|
|`/rewind`|**进入回滚界面**|
|`/compact`|**主动压缩精简上下文**|
|`/clear`|彻底清空上下文，相当于重开一个会话|
|`/context`|**详细展示agent当前的上下文信息，诸如：上下文占比，上下文类别等等**|
|`/resume`|**在全新的上下文窗口，选择恢复到之前的对话**|
|`/init`|**初始化创建项目级Claude.md**|
|`/memory`|**针对Claude的全局、项目记忆，以及auto memory进行操作和管理**|
|`/agents`|创建、调用、管理子agent|
|`/plugin`|发现新插件，管理已下载插件，新增插件生态|

# 二、掌控与管理

## 1. Git下载与设置

## 2.上下文管理（清除大模型空间）

**步骤1:**确认上下文进度

- 输入`/context`命令，它会详细的展示我们的上下文占比信息：
  
    ```powershell
    /context
    ```
    

**步骤2:**主动压缩上下文

- cc会在上下文即将满的时候，会帮我们自动压缩
  
- 我的习惯是看到高于60%了，就大概率需要 `/compact`
  

```powershell
/compact
```

**步骤3:**彻底清空上下文

```powershell
/clear
```

**步骤4:**context占用条常驻

- 对CC输入提示词，并根据CC引导完成后重启终端即可：
  
    ```
    帮我配一个 statusLine,显示当前目录+模型+上下文剩余百分比 
    ```
    

**步骤5:**对话恢复

- 输入以下指令后，选择恢复到你想要的会话即可：
  
    ```powershell
    /resume
    ```
    
    - 恢复上次对话
    
      ```powershell
      claude -c
      ```
    
      -c 即 -continue

# 三、个性化

## 1. Claude.md 配置

*Claude.md分为两种*

**项目级 Claude.md** 创建方式：

> 等项目有一定雏形了再创建

```powershell
/init
```

**全局级 Claude.md** 有两种创建方式：

- **方式 1：提示词交互：**xxx，写进全局claude.md

  例如，`始终使用中文回复。所有对话、解释和代码注释都应使用中文。`

- **方式 2：使用指令：**输入 `/memory` 后，选择「User Memory」进入

## 2. Auto Memory

- 打开Auto Memory：输入`/memory` ，选择「**Auto-memory**」并输入回车开启
  
    > 打开以后，我们与CC的工作交互过程中，那些没有显式的主动写进claude.md的一些习惯、错误、经验，都会以自动记忆形式被记录，但仅限于当前项目，不会跨项目产生影响。
    

## 3. 写提示词

参考 [如何写提示词](如何写提示词.md) 的内容

# 四、能力扩展

## 1. Skill 技能扩展

1. **Claude Code优质skill推荐**
   
    1. 可以将skill链接给cc，让cc帮你直接下好
       
    
    |skill名称|功能|下载链接|
    |---|---|---|
    |Find-Skill|根据用户需求，查找和安装来自agent skill开放生态的技能|[GitHub](https://github.com/vercel-labs/skills/tree/main/skills/find-skills)|
    |Frontend- Design|创建具有独特风格、生产级品质且设计精良的前端界面|[GitHub](https://github.com/anthropics/skills/tree/main/skills/frontend-design)|
    |Skill- Creator|创建新skill、修改和改进现有skill，并衡量skill表现|[GitHub](https://github.com/anthropics/skills/tree/main/skills/skill-creator)|
    |卡帕西skill|一个依据卡帕西经验总结，用于提升Claude Code编码表现的skill|[GitHub](https://github.com/forrestchang/andrej-karpathy-skills)|
    
2. **skill合集网站：****[lobehub](https://lobehub.com/zh/skills)**
   
    大家既可以根据分类去寻找自己需要的skill，也可以直接在精选合集查看推荐的优质skill
    
3. **想创建自己的skill？**
   
    我们还有一份网页版教学文档，非常详细：[Agent Skills指南（Claude Code版）](https://ccnk05wgo092.aiforce.cloud/spark/faas/app_4jdqh6vm3jpdp)
    
4. **下载了skill怎么装？**
   
    可根据需求，将skill文件放入**项目级skill**与**全局级skill**的存放**文件夹**
    
    skill文件结构
    
    ```
	.claude
	└──skills
		└──xxx-skill/
		    ├── SKILL.md 
		    ├── scripts/
		    ├── references/
		    └── assets/
    ```

## 2. MCP 扩展

关于MCP的部分，可结合以下教程进行学习与实践：

- MCP（模型上下文协议）是Anthropic推出的开源标准，相当于AI界的“万能插头”，解决大模型连接外部数据源和工具时需定制代码的问题。它与Skill的区别在于：MCP是“基础设施层”，负责连接外部系统（解决“能不能连”）；Skill是“流程层”，封装专业知识（解决“怎么做得好”），两者互补。

- 在Claude Code中接入MCP：启动`claude .`后，输入`/mcp`命令，通过菜单授权即可添加本地或远程MCP服务器。CLI是“执行层”，MCP是“能力层”，两者可协同。

- 常见MCP工具：`mcp-image-gen`（生成/编辑图片）、`youtube-uploader-mcp`（上传视频）、`@magun/watch`（终端播放YouTube）。

## 3. CLI 命令行工具

**推荐几个优质CLI，**直接将下载链接给CC，让CC全权负责下载和引导操作：

| **CLI名称**  | **功能**                                                                         | **下载**                                                            |
| ---------- | ------------------------------------------------------------------------------ | ----------------------------------------------------------------- |
| 飞书CLI      | 飞书官方CLI工具，覆盖消息、文档、多维表格、电子表格、幻灯片、日历、邮箱、任务、会议等核心业务域，提供200+命令及24个AI Agent Skills  | [GitHub](https://github.com/larksuite/cli/blob/main/README.zh.md) |
| OpenCLI    | 万能命令行工具箱，通用命令行中心与AI原生运行平台，能将任何网站、桌面应用或本地程序变成统一命令行操作界面                          | [GitHub](https://github.com/jackwener/opencli)                    |
| CLI        | GitHub 的官方命令行工具。它将拉取请求、问题和其他 GitHub 概念带到终端中，与你已经在使用 `git` 和代码的地方并排显示。          | [GitHub](https://github.com/cli/cli)                              |
| gemini-CLI | Gemini CLI 可将 Gemini 的功能直接引入终端。它提供轻量级的 Gemini 访问方式，能够以最直接的方式从终端命令访问 Gemini 模型。 | [GitHub](https://github.com/google-gemini/gemini-cli)             |

> 给大家推荐一个GitHub上的CLI主题推荐网页，大家可按需查找自己想要的CLI：[Command-line interface](https://github.com/topics/cli)

## 4. 子Agent（SubAgent）

**创建子Agent的两种方式：**

1. **自动触发：**任务复杂且存在并行可能时，CC会自动派生子Agent并行推进
   
2. **手动创建：**通过指令 `/agents` ，在Library界面进行创建
   

**手动创建步骤：**

**步骤1：**选择创建项目级或全局级子Agent

**步骤2：**选择「AI辅助创建」，让AI根据意图辅助创建

**步骤3：**描述想要的子Agent功能

**步骤4：**决定子Agent工具权限（✓为选中）

**步骤5：**选择Claude的模型

- 一般选Sonnet就可以（如果使用CC Switch配置了模型，选哪个都一样）
  

**步骤6：**为子Agent挑选区别于主Agent的颜色

- 选个好看的颜色
  

**步骤7:**调用与管理子Agent

- 输入 `/agents`，在Library下选择已创建的项目级子Agent
  
- 根据需求对子Agent进行相应操作
  

## 5. Hook 钩子

推荐以下Hook设置，直接告诉CC帮忙设置：

```Plain
设置一个hook，每次完成任务之后，都自动执行一个声音脚本，发出一个提示音"叮"进行提醒
```

```Plain
设置一个hook，每次提交代码之前，都会自动触发代码格式的检查
```

## 6. 插件（Plugin）

**插件说明**

插件是打包了 **Skill**、**SubAgent**、**Hook**、**MCP** 的整合性概念

**步骤1：**通过指令 `/plugin` 进入插件管理界面

**步骤2：**在插件管理界面，可以收录下载钟意的插件，或管理已下载插件

推荐给大家一些Claude Code官方精选的一些插件：

|插件名称|插件功能|链接|
|---|---|---|
|commit-commands|使用简单的命令简化 Git 工作流程，包括提交、推送和创建拉取请求|[GitHub](https://github.com/anthropics/claude-code/tree/main/plugins/commit-commands?utm_source=claudecodeplugins.dev)|
|content-creator|这位内容创作者擅长跨平台内容创作，从长篇博客文章到引人入胜的视频脚本和社交媒体内容，无所不包。|[GitHub](https://github.com/ccplugins/awesome-claude-code-plugins/tree/main/plugins/content-creator?utm_source=claudecodeplugins.dev)|
|security-guidance|安全提醒钩子，在编辑文件时提示潜在的安全问题，包含命令注入、XSS 及不安全的代码模式|[GitHub](https://github.com/anthropics/claude-code/tree/main/plugins/security-guidance?utm_source=claudecodeplugins.dev)|

如果仍有需求，可以去官方/三方插件网站继续查看自己需要的插件：[Plugins](https://claude.com/plugins#plugins) / [Claude Code Plugins](https://claudecodeplugins.dev/zh)