# 16-Linux系统编程概览

## 知识地图

```
文件I/O：系统调用（open/read/write/close）
    ↓      C标准库缓冲（fopen/fread/fwrite/fclose）
文件系统：inode（元数据）→ 目录项（文件名↔inode）→ VFS（虚拟文件系统）
    ↓
进程：fork（复制）→ exec（替换）→ wait（等待）
    ↓
正则表达式基础：字符匹配 → 量词 → 锚定 → 分组
    ↓    实际工具：grep（搜索）→ sed（替换）
```

**本讲回答的核心问题**：系统调用和C标准库I/O有什么本质区别？进程是怎么创建的？正则表达式的基本语法怎么用？

---

## 1. 文件与I/O

### 1.1. 系统调用 vs C标准库

Linux 中的 I/O 操作分两个层次：

| 层次 | 函数 | 缓冲 | 可移植性 | 数据单元 |
|------|------|:--:|:--:|:--:|
| C标准库 | `fopen/fread/fwrite/fclose` | 有（FILE* 缓冲区） | 跨平台 | 字符/行/块 |
| 系统调用 | `open/read/write/close` | 无 | Linux 专用 | 字节 |

```c
/* 系统调用方式 */
#include <fcntl.h>
#include <unistd.h>

int fd = open("data.txt", O_RDONLY);
if (fd < 0) { perror("open"); return 1; }

char buf[256];
ssize_t n = read(fd, buf, sizeof(buf));  /* 返回实际读到的字节数 */
close(fd);

/* C标准库方式——有缓冲，效率更高 */
FILE *fp = fopen("data.txt", "r");
if (fp == NULL) { perror("fopen"); return 1; }

fgets(buf, sizeof(buf), fp);   /* 安全读一行 */
fprintf(fp, "%s", buf);        /* 格式化写入 */
fclose(fp);
```

**为什么 C 标准库更快？** 系统调用每次都要陷入内核（用户态→内核态→用户态），开销大。标准库先在用户空间缓冲一批数据，再集中系统调用。

**文件描述符（fd）**：系统调用通过一个整数（文件描述符）来标识打开的文件，0=stdin，1=stdout，2=stderr。

### 1.2. mmap 内存映射

mmap 可以把文件映射到进程的地址空间，**像操作数组一样操作文件**：

```c
#include <sys/mman.h>

int fd = open("large.bin", O_RDWR);
int *data = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
data[0] = 42;               /* 直接修改内存 = 修改文件 */
msync(data, size, MS_SYNC); /* 同步回磁盘 */
munmap(data, size);          /* 解除映射 */
```

---

## 2. 文件系统

### 2.1. 索引节点（inode）

文件系统把文件分为两部分：
- **元数据（inode）**：大小、权限、时间戳、数据块指针（不包含文件名！）
- **数据**：文件的实际内容

### 2.2. 目录项（dentry）

目录是文件名到 inode 的映射表。**硬链接**是一个目录项，多个硬链接指向同一个 inode；**软链接**（符号链接）是一个特殊的文件，内容指向另一个路径。

### 2.3. VFS 虚拟文件系统

Linux 通过 VFS 层统一处理各种不同的文件系统（ext4、xfs、NTFS 等）。VFS 定义了一组通用接口（`open/read/write`），每种文件系统自己实现。这就是"一切皆文件"的基础——设备、管道、socket 都可以通过文件接口操作。

---

## 3. 进程

### 3.1. fork——复制

`fork` 创建子进程，子进程是父进程的**完整副本**（代码段、数据段、堆栈、文件描述符都复制一份，写时复制优化）：

```c
#include <unistd.h>
#include <sys/wait.h>

pid_t pid = fork();

if (pid == -1) {
    perror("fork");
    exit(1);
} else if (pid == 0) {
    printf("Child: PID=%d, Parent PID=%d\n", getpid(), getppid());
    _exit(0);              /* 子进程退出 */
} else {
    printf("Parent: child PID=%d\n", pid);
    wait(NULL);            /* 等待子进程结束（避免僵尸进程） */
}
```

**关键**：fork 之后的父子进程**独立运行**，各有各的地址空间。子进程继承父进程的文件描述符、环境变量等。

**为什么要 wait？** 子进程退出时如果父进程不 `wait`，子进程变成**僵尸进程**（占用进程表条目）。父进程退出后，孤儿进程由 `init`（PID=1）收养。

### 3.2. exec——替换

fork 创建了子进程后，子进程通常用 exec 执行另一个程序（替换当前的映像）：

```c
if (fork() == 0) {
    execl("/bin/ls", "ls", "-l", NULL);     /* 替换为 ls 命令 */
    perror("execl");                         /* 只有 exec 失败才会执行 */
    _exit(1);
}
wait(NULL);
```

exec 系列函数有哪些：

| 函数 | 路径 | 参数 | 环境 |
|------|:--:|:--:|:--:|
| `execl` | 完整路径 | 可变参数 | 继承 |
| `execv` | 完整路径 | 数组 | 继承 |
| `execlp` | **搜索 PATH** | 可变参数 | 继承 |
| `execvp` | **搜索 PATH** | 数组 | 继承 |
| `execle` | 完整路径 | 可变参数 | **指定** |

### 3.3. 简易 Shell

Shell 的本质就是 `fork → exec → wait` 的无限循环：

```c
while (1) {
    printf("> ");
    char cmd[256];
    fgets(cmd, sizeof(cmd), stdin);
    cmd[strcspn(cmd, "\n")] = '\0';     /* 去掉末尾换行 */

    if (strcmp(cmd, "exit") == 0) break;
    if (strcmp(cmd, "") == 0) continue;

    if (fork() == 0) {
        execlp(cmd, cmd, NULL);         /* 到 PATH 中查找并执行 */
        fprintf(stderr, "%s: not found\n", cmd);
        _exit(1);
    }
    wait(NULL);
}
```

---

## 4. 正则表达式

### 4.1. 基本语法

正则表达式描述字符串的模式，在文本搜索、数据验证中无处不在。

**字符匹配**：

| 模式 | 含义 | 匹配示例 |
|------|------|---------|
| `.` | 任意单个字符（u） | `c.t` → cat, cut |
| `[abc]` | a,b,c 之一 | `[aeiou]` → 元音字母 |
| `[^abc]` | 非 a,b,c 的字符 | `[^0-9]` → 非数字 |
| `[a-z]` | a 到 z 的范围 | `[a-zA-Z]` → 所有字母 |
| `\d` | 数字（扩展） | `\d+` → 123, 4567 |
| `\w` | 字母数字下划线 | `\w+` → identifier |
| `\s` | 空白字符 | `\s+` → 空格/Tab/换行 |
| `\` | 转义元字符 | `\.` 匹配字面点号 |

**量词**：

| 模式 | 含义 |
|:--:|------|
| `*` | 前面元素出现 0 次或多次 |
| `+` | 前面元素出现 1 次或多次 |
| `?` | 前面元素出现 0 或 1 次 |
| `{n}` | 恰好 n 次 |
| `{n,}` | 至少 n 次 |
| `{n,m}` | n 到 m 次 |

**贪婪 vs 懒惰**：
- `.*`：贪婪——匹配尽可能多（如 `<h1>title</h1>` 中 .* 匹配整行）
- `.*?`：懒惰——匹配尽可能少（如 `<h1>title</h1>` 中 .*? 只匹配到 `>`）

**位置锚定**：

| 模式 | 含义 |
|:--:|------|
| `^` | 行首 |
| `$` | 行尾 |
| `\<` | 单词开头 |
| `\>` | 单词结尾 |

**分组与捕获**：

```regex
(abc)+         # 匹配 abc 出现一次或多次
(\d{3})-(\d{4})   # 捕获组1=区号，组2=号码
(?:abc)        # 非捕获分组（只分组不捕获）
\1             # 反向引用：引用第一个捕获组的内容
```

### 4.2. 常用工具

**grep**（搜索）：

```bash
$ grep 'int' *.c                 # 匹配包含 int 的行
$ grep -E '\d{3}-\d{4}' data     # 扩展正则，匹配电话号码
$ grep -v '^#' config            # -v 取反：显示非注释行
$ grep -r 'TODO' .               # 递归搜索
$ grep -n 'main' *.c             # 显示行号
$ grep -c 'error' log.txt        # 只统计匹配行数
```

**管道组合**：

```bash
$ ps aux | grep firefox           # 查找 firefox 进程
$ cat log.txt | grep 'ERROR' | head -20  # 前20条错误
```

**sed**（流编辑）：

```bash
$ sed 's/foo/bar/' file           # 替换每行第一个 foo 为 bar
$ sed 's/foo/bar/g' file          # g 标志：全局替换
$ sed '/^$/d' file                # 删除空行
$ sed -n '10,20p' file            # 打印 10-20 行
$ sed -i.bak 's/old/new/g' file   # 直接修改文件（备份 .bak）
```

### 4.3. C 中使用正则

```c
#include <regex.h>

int match(const char *str, const char *pattern) {
    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
        return -1;                              /* 编译正则失败 */
    int ret = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return ret == 0 ? 1 : 0;                    /* 1=匹配, 0=不匹配 */
}
```

### 4.4. 正则实战表

| 需求 | 正则模式 |
|------|----------|
| 匹配整数 | `^-?\d+$` |
| 匹配浮点数 | `^-?\d+\.?\d*$` |
| 匹配邮箱 | `^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$` |
| 匹配IP地址 | `\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}` |
| 匹配URL | `https?://[^\s]+` |
| 匹配中文 | `[\u4e00-\u9fa5]+` |
| 匹配空白行 | `^\s*$` |
| 匹配18位身份证 | `^\d{17}[\dXx]$` |

---

## 本讲核心要点

1. **系统调用层**（`open/read`）无缓冲、不可移植；**C标准库**（`fopen/fread`）有缓冲、可移植
2. **文件描述符** 0=stdin, 1=stdout, 2=stderr；mmap 将文件映射为内存
3. **VFS**统一了不同文件系统的操作接口，是"一切皆文件"的基础
4. **fork 创建子进程**（是父进程的副本），**exec 替换进程映像**
5. fork 后父进程必须 `wait`，否则子进程变成僵尸进程
6. Shell 的本质是 `fork → exec → wait` 无限循环
7. **正则表达式**用模式匹配描述字符串规则：`.` `[]` `*` `+` `?` `{}` `^$`
8. **grep** 搜索匹配行，**sed** 流式编辑替换，C 中用 `<regex.h>`
9. 交叉表：正则的 `.` 元字符需 `\.` 才能匹配字面点号

---

## 习题

**写一个 fork + exec 的示例，子进程执行 `/bin/echo` 打印 "Hello"。**

```c
if (fork() == 0) {
    execl("/bin/echo", "echo", "Hello", NULL);
    _exit(1);
}
wait(NULL);
```
