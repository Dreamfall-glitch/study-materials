# 08-调试工具gdb

## 知识地图

```
编译加 -g 保留调试信息，调试阶段不加优化
    ↓
基本命令：run / next（不进入函数） / step（进入函数）
    ↓
断点：break（行号/函数名） / 条件断点（break N if cond）
观察点：watch（数据改变时暂停）
    ↓
print 查看变量 / backtrace 查看调用栈 / frame 切换栈帧
    ↓
段错误调试：run → 崩溃行 → print 可疑变量
core dump：ulimit -c unlimited 保留崩溃快照
```

**本讲回答的核心问题**：程序崩溃了怎么办？如何在运行时查看变量的值？如何一步步跟踪程序的执行逻辑？

---

## 1. 准备工作

### 1.1. 编译时保留调试信息

```c
/* test.c —— 待调试的示例程序 */
#include <stdio.h>

int factorial(int n)
{
    int result = 1;
    while (n > 1)
        result *= n--;
    return result;
}

int main(void)
{
    int a = 5;
    int r = factorial(a);
    printf("%d! = %d\n", a, r);
    return 0;
}
```

```
$ gcc -g test.c -o test        # -g: 生成调试信息
$ gdb ./test                   # 启动 gdb
```

**关键**：不加 `-g` 编译的程序，gdb 无法显示源代码行号和变量名，只能看到汇编指令和内存地址。

### 1.2. 编译优化与调试的冲突

如果使用 `-O2` 或 `-O3` 优化编译，编译器的重排、内联等优化会使源代码和机器指令的对应关系很松散——单步执行时会"跳来跳去"。调试阶段应使用：

```
$ gcc -g -O0 test.c -o test     # -O0: 不优化（调试友好）
```

---

## 2. 基本调试命令

### 2.1. 运行与退出

```
(gdb) run              # 启动程序
(gdb) run arg1 arg2    # 带命令行参数运行
(gdb) quit             # 退出 gdb
```

### 2.2. 单步执行

| 命令 | 简写 | 作用 |
|------|------|------|
| `next` | n | 执行下一行，遇函数调用**不进入** |
| `step` | s | 执行下一行，遇函数调用**进入**内部 |
| `finish` | fin | 执行完当前函数并返回 |
| `continue` | c | 继续运行直到下一个断点或程序结束 |

以 factorial 为例：
```
(gdb) break factorial
(gdb) run
Breakpoint 1, factorial (n=5) at test.c:6
6           int result = 1;

(gdb) next
7           while (n > 1)

(gdb) print n
$1 = 5

(gdb) next
8               result *= n--;

(gdb) print n
$2 = 5
(gdb) print result
$3 = 1

(gdb) next           # 执行 result *= n--; 后 n 变为 4，result 变为 5
(gdb) print n
$4 = 4
(gdb) print result
$5 = 5
```

### 2.3. 查看变量

```c
(gdb) print n           # 查看 n 的值
(gdb) print result      # 查看 result 的值
(gdb) print n + 5       # 打印任意表达式的结果
(gdb) print factorial(3)  # 甚至调用函数！
```

`print` 的结果会保存在 `$1`、`$2`、`$3`... 中，后续可以引用：`print $1 * 2`。

**自动显示**——每次暂停都打印：

```
(gdb) display n         # 每次 single-step 后自动打印 n
(gdb) display result
(gdb) info display      # 查看所有自动显示项
(gdb) undisplay 1       # 取消1号自动显示
```

---

## 3. 断点与观察点

### 3.1. 断点（Breakpoint）

断点使程序在指定位置暂停：

```
(gdb) break 15              # 在第15行设断点
(gdb) break factorial       # 在函数入口设断点
(gdb) break test.c:20       # 在指定文件的第20行
(gdb) info break            # 查看所有断点
(gdb) delete 1              # 删除1号断点
(gdb) disable 1             # 暂时禁用1号断点
(gdb) enable 1              # 重新启用
```

### 3.2. 条件断点

**这是 gdb 最强大的功能之一**——只在满足条件时才停下：

```
(gdb) break 15 if i == 99      # 循环第100次才停下
(gdb) break factorial if n < 0  # 只在参数为负数时停下
(gdb) condition 1 n == 0        # 给已有断点(1号)加条件
```

条件断点极大减少了不需要的暂停，使调试循环和大规模代码变得可行。

### 3.3. 观察点（Watchpoint）

断点停在地点（代码位置），观察点停在**数据被修改时**：

```
(gdb) watch n               # 当 n 的值被改变时自动暂停
(gdb) watch n > 100         # 当 n 超过 100 时暂停
(gdb) info watchpoints      # 查看所有观察点
```

观察点用于追踪"不知道是谁改了某个变量"的问题——程序会在变量值改变的瞬间停在该行代码上。

---

## 4. 查看调用栈

### 4.1. backtrace

`backtrace`（简写 `bt`）显示当前执行位置的函数调用链：

```
(gdb) backtrace
#0  factorial (n=1) at test.c:6
#1  0x401136 in factorial (n=2) at test.c:8
#2  0x401136 in factorial (n=3) at test.c:8
#3  0x401155 in main () at test.c:15
```

- `#0` 是当前正在执行的函数（最深的一层）
- `#1` 是调用 `#0` 的函数，以此类推
- 最外层是 `main`

### 4.2. 在调用栈中切换

```
(gdb) frame 2              # 切换到 #2 的上下文
(gdb) print n               # 现在打印的是 #2 中的 n
(gdb) frame 0               # 切换回当前
```

可以在不同调用层之间跳转，查看各层的局部变量——这在调试递归和深层调用时非常有用。

### 4.3. 查看变量和源码

```
(gdb) list                   # 显示当前行附近的源码
(gdb) info locals            # 显示当前函数的全部局部变量
(gdb) info args              # 显示当前函数的参数
```

---

## 5. 调试段错误

### 5.1. 段错误的现象

```c
/* crash.c */
int main(void)
{
    int *p = NULL;
    *p = 10;                /* 向空指针写入——段错误！ */
    return 0;
}
```

```
$ gcc -g crash.c -o crash
$ ./crash
Segmentation fault (core dumped)
```

### 5.2. 用 gdb 定位

```
$ gdb ./crash
(gdb) run
Program received signal SIGSEGV, Segmentation fault.
0x0000000000401142 in main () at crash.c:5
5           *p = 10;

(gdb) print p
$1 = (int *) 0x0              # 空指针！问题找到

(gdb) backtrace
#0  0x0000000000401142 in main () at crash.c:5
```

两步就定位了：`run` 自动停在崩溃行，`print p` 发现是空指针。

### 5.3. 核心转储（Core Dump）

如果程序已经崩溃退出了，还可以用 coredump 文件分析：

```
$ ulimit -c unlimited          # 允许生成 core 文件
$ ./crash
Segmentation fault (core dumped)   # 生成了 core 文件
$ ls -lh core
-rw------- 1 user user 280K core

$ gdb ./crash core             # 用 core 文件做"事后分析"
(gdb) backtrace                # 崩溃时的调用栈一览无余
```

**core dump 就像一个时间胶囊**——保存了程序崩溃瞬间的完整内存快照。

---

## 本讲核心要点

1. **编译加 `-g`**保留调试信息，调试阶段用 `-O0` 禁用优化
2. **next**（不过函数边界）和 **step**（进入函数）是最常用的单步命令
3. **print** 可查看任意表达式的值，**display** 每次暂停自动显示
4. **断点**停在代码位置（行号/函数名），**观察点**停在数据变化时
5. **条件断点**（`break N if cond`）是调试循环和大规模代码的利器
6. **backtrace** 查看完整函数调用链，**frame** 可在不同层之间跳转
7. **段错误**用 `gdb ./a.out` → `run` → `bt` 三步直接定位崩溃行和原因
8. **Core dump** 用 `ulimit -c unlimited` 开启，保存崩溃快照供事后分析

---

## 调试会话速查

```
$ gcc -g -O0 program.c -o program
$ gdb ./program

常用命令（短写）：
r          run                   运行程序
n          next                  下一步（不进入函数）
s          step                  下一步（进入函数）
c          continue              继续到下一断点
b 20       break 20              在第20行设断点
b func     break func            在函数入口设断点
b 20 if x>5                      条件断点
p var      print var             查看变量
p expr     print expr            计算表达式
bt         backtrace             查看调用栈
f 2        frame 2               切换到2号栈帧
l          list                  显示源码
info b     info breakpoints      查看断点列表
d 1        delete 1              删除1号断点
q          quit                  退出
```
