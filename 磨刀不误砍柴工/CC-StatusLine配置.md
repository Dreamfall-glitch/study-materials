# 如何配置 Claude Code StatusLine

## 目标

Claude Code 底部状态栏显示：**当前目录 | 模型名 | 上下文使用百分比**

```
x:/xx/xx │ xxx-model │ 使用 ████████ xx%
```

---

## 方案一：cc-statusline-cli（推荐，Node.js 跨平台）

### 安装

```bash

npm install -g cc-statusline-cli

```

### 初始化配置

```bash

cc-statusline install --force

```

这会自动写入 `C:\Users\<用户名>\.claude\settings.json`，statusLine 字段指向 Node.js 命令。

### 自定义显示内容

编辑配置文件 `C:\Users\<用户名>\AppData\Roaming\cc-statusline\config.json`：

```json
{
  "separator": " │ ",

  "lines": [
    ["directory", "model", "context_bar"]
  ],
  
  "modules": {
    "model": { "with_version": false, "with_effort": true },
    "directory": { "tilde": true, "truncate": 30 },
    "context_bar": {
      "label": "使用",
      "width": 10,
      "thresholds_pct": [70, 90],
      "default_size": "200k"
    }
  },
  
  "colors": {
    "model": "white",
    "effort": "gray",
    "directory": "blue",
    "context_ok": "bright_green",
    "context_warn": "yellow",
    "context_crit": "red"
  }
}
```

### 可用 modules

| module          | 说明     | 常用选项                                                  |
| --------------- | ------ | ----------------------------------------------------- |
| `directory`     | 当前目录   | `tilde: true`（~缩写）, `truncate: 30`（截断长度）              |
| `model`         | 模型名    | `with_version: true/false`, `with_effort: true/false` |
| `context_bar`   | 上下文进度条 | `label`, `width`, `thresholds_pct`, `default_size`    |
| `git`           | Git 状态 | `show_status`, `show_ahead_behind`                    |
| `session_timer` | 会话时长   | —                                                     |
| `rate_limit`    | 速率限制   | `bar_width`, `thresholds_pct`                         |
| `cost`          | API 费用 | `format: "$%.2f"`, `hide_below`                       |

### 内置 preset

安装后自动应用 `default` preset。其他内置 preset：

```bash
cc-statusline install --force   # 用 default preset
```

更换 preset：直接编辑 config.json，设置 `"preset": "developer"` 或 `"preset": "minimal"`，或者直接覆盖 lines/modules（如上自定义配置）。

### 测试命令

```bash
# 模拟 Claude Code 的 JSON 输入，查看渲染效果
echo '{"workspace":{"current_dir":"e:/thesis/mypaper"},"model":{"display_name":"deepseek-v4-flash","effort":"high"},"context_window":{"used_percentage":15,"context_window_size":200000}}' | node "C:\Users\<用户名>\AppData\Roaming\npm\node_modules\cc-statusline-cli\bin\cc-statusline.js" render
```

---

## 方案二：settings.json 内联命令（不装 npm 包）

### 依赖

- 需要 `jq` 在 PATH 中（Windows 下可用 `winget install jqlang.jq`）

- 需要 bash 环境（Git Bash）

### 配置

编辑 `C:\Users\<用户名>\.claude\settings.json`：

```json
{
  "statusLine": {
    "type": "command",
    "command": "input=$(cat); dir=$(echo \"$input\" | jq -r '.workspace.current_dir'); model=$(echo \"$input\" | jq -r '.model.display_name'); used=$(echo \"$input\" | jq -r '.context_window.used_percentage // empty'); if [ -n \"$used\" ]; then printf '%s | %s | 使用: %.0f%%' \"$dir\" \"$model\" \"$used\"; else printf '%s | %s' \"$dir\" \"$model\"; fi"
  }
}
```

### 测试

```bash
# 模拟输入测试
echo '{"workspace":{"current_dir":"/e/thesis/mypaper"},"model":{"display_name":"deepseek-v4-flash"},"context_window":{"used_percentage":15}}' | bash -c 'input=$(cat); dir=$(echo "$input" | jq -r ".workspace.current_dir"); model=$(echo "$input" | jq -r ".model.display_name"); used=$(echo "$input" | jq -r ".context_window.used_percentage // empty"); if [ -n "$used" ]; then printf "%s | %s | 使用: %.0f%%\n" "$dir" "$model" "$used"; else printf "%s | %s\n" "$dir" "$model"; fi'
```

---

## 已知问题

### Windows Terminal 不渲染

**现象**：statusLine 配置正确，命令执行成功，但底部不显示。

**原因**：Claude Code 在 Windows Terminal 的 PTY 层下无法捕获子进程 stdout（GitHub issues [#14125](https://github.com/anthropics/claude-code/issues/14125)、[#6526](https://github.com/anthropics/claude-code/issues/6526)）。

**解决方案**（任一）：

1. 用 **VS Code 集成终端**（选 Git Bash profile）运行 `claude`

2. 用 **Git Bash 独立窗口**（右键 → Git Bash Here）运行 `claude`

3. 等待 Anthropic 修复此 bug

> 注：方案一（cc-statusline-cli）用 Node.js 直接渲染，部分用户报告在 Windows Terminal 下可用，建议优先尝试。

### 其他常见问题

| 问题                         | 解决                                                                                   |
| -------------------------- | ------------------------------------------------------------------------------------ |
| 显示 `jq: command not found` | 安装 jq：`winget install jqlang.jq` 或 `scoop install jq`                                |
| PowerShell 5.1 语法错误        | 必须用 Git Bash，不要用 PowerShell 运行内联命令                                                   |
| `/clear` 后 token 未重置       | 已知 bug [#16189](https://github.com/anthropics/claude-code/issues/16189)，需完全重启 Claude |
| 目录路径含空格/括号报错               | 确保命令中用双引号包裹路径变量                                                                      |