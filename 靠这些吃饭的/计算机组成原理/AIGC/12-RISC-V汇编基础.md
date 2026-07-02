# 12-RISC-V汇编基础

```知识地图
RISC-V是什么？→ 开源RISC指令集架构
    ↓
RV32I 基础指令集
    ├── 寄存器结构（32个通用寄存器 + PC）
    ├── 指令格式（R/I/S/B/U/J六种）
    └── 寻址方式
    ↓
常用指令分类
    ├── 算术运算（ADD/SUB/ADDI）
    ├── 逻辑运算（AND/OR/XOR）
    ├── 访存指令（LW/SW/LB/SB）
    └── 分支跳转（BEQ/BNE/JAL/JALR）
    ↓
伪指令与程序结构
```

**本讲回答的核心问题**：RISC-V的指令格式是怎样的？如何用RISC-V汇编编写简单的程序？RV32I的六种指令格式分别对应哪些指令？

## 1. RISC-V概述

### 1.1. 什么是RISC-V

**RISC-V**（读作"risk-five"）是一个基于RISC原则设计的**开源指令集架构**（ISA），由加州大学伯克利分校的Krste Asanović教授团队于2010年发起。

**核心特点**：

| 特点 | 说明 |
|:----|:------|
| **开源免费** | 不像ARM（需授权费）或x86（Intel/AMD独占），任何人都可以免费使用 |
| **模块化设计** | 基础指令集（RV32I）+ 可选扩展（M/F/D/C等） |
| **简洁规整** | 指令格式统一、寄存器数量固定、易于实现 |
| **生态完善** | 有GCC、LLVM、Linux、FreeRTOS等完整软件栈支持 |

### 1.2. RISC-V的模块化扩展

RISC-V采用"基础集 + 扩展"的设计：

| 缩写 | 名称 | 说明 |
|:----|:-----|:------|
| **RV32I** | 32位基础整数指令集 | **必须实现**——包含算术、逻辑、访存、分支等40多条指令 |
| **RV64I** | 64位基础整数指令集 | 地址空间扩大到64位 |
| **M** | 整数乘除扩展 | MUL、DIV、REM |
| **F** | 单精度浮点扩展 | FLW、FSW、FADD.S |
| **D** | 双精度浮点扩展 | 64位浮点运算 |
| **C** | 压缩指令扩展 | 16位短指令，减小代码体积 |

本章以**RV32I**为基础展开。

---

## 2. RV32I的寄存器结构

### 2.1. 通用寄存器

RV32I提供了**32个通用寄存器**（x0~x31），每个32位宽。这是RISC架构的典型特征——远比CISC（如x86的8个）多。

| 寄存器 | ABI名称 | 用途 | 是否被调用者保存 |
|:------|:-------|:-----|:--------------|
| x0 | **zero** | 硬件零寄存器——读取始终为0，写入被忽略 | — |
| x1 | **ra** | 返回地址（return address） | 否 |
| x2 | **sp** | 栈指针（stack pointer） | 是 |
| x3 | **gp** | 全局指针（global pointer） | — |
| x4 | **tp** | 线程指针（thread pointer） | — |
| x5~x7 | t0~t2 | 临时寄存器（temporary） | 否 |
| x8 | **s0/fp** | 保存寄存器 / 帧指针 | 是 |
| x9 | s1 | 保存寄存器 | 是 |
| x10~x11 | a0~a1 | 函数参数 / 返回值 | 否 |
| x12~x17 | a2~a7 | 函数参数 | 否 |
| x18~x27 | s2~s11 | 保存寄存器 | 是 |
| x28~x31 | t3~t6 | 临时寄存器 | 否 |

**关键规则**：
- **x0（zero）**：始终为0——常用于清零操作（`ADD x5, x0, x0` 相当于 `x5=0`）
- **返回地址**：`JAL`指令自动将返回地址写入 **x1（ra）**
- **栈指针**：x2（sp）指向栈顶，由惯例管理
- **参数传递**：a0~a7传递函数参数，a0~a1传递返回值

### 2.2. PC寄存器

**PC**（Program Counter，程序计数器）指向当前正在执行的指令地址。RISC-V中PC是一个独立的寄存器，不属于通用寄存器组。分支和跳转指令通过修改PC来控制程序流。

> **自己的话**：32个通用寄存器是RISC-V的"工作台"——大部分操作在寄存器之间完成，只有LOAD/STORE指令才访问主存。这显著减少了访存次数，简化了流水线设计。

---

## 3. RV32I的六种指令格式

RISC-V将指令分为六种格式，每种格式的指令字段位置**固定**——这极大简化了译码逻辑（相比x86的可变长度译码）。

### 3.1. 格式总览

```
R型（寄存器-寄存器运算）：
31:25   24:20   19:15   14:12   11:7    6:0
funct7  rs2     rs1     funct3  rd      opcode

I型（立即数运算/访存加载）：
31:20           19:15   14:12   11:7    6:0
imm[11:0]       rs1     funct3  rd      opcode

S型（访存存储）：
31:25   24:20   19:15   14:12   11:7    6:0
imm[11:5]  rs2  rs1     funct3  imm[4:0]  opcode

B型（条件分支）：
31   30:25  24:20  19:15  14:12  11:8  7   6:0
imm[12] imm[10:5] rs2   rs1   funct3 imm[4:1] imm[11] opcode

U型（高位立即数）：
31:12                   11:7    6:0
imm[31:12]              rd      opcode

J型（无条件跳转）：
31   30:21  20    19:12       11:7    6:0
imm[20] imm[10:1] imm[11] imm[19:12] rd  opcode
```

### 3.2. 各格式说明

| 格式 | 用途 | 典型指令 | 特点 |
|:----|:-----|:--------|:------|
| **R型** | 寄存器-寄存器运算 | ADD、SUB、AND、OR、XOR、SLL | 两个源寄存器(rs1, rs2) → 目标寄存器(rd) |
| **I型** | 立即数运算/加载 | ADDI、LW、LB、ADDI、SLTI | 一个源寄存器+12位立即数 → 目标寄存器 |
| **S型** | 存储到主存 | SW、SB、SH | 源寄存器(rs2) → 基址寄存器(rs1)+偏移量(imm) |
| **B型** | 条件分支 | BEQ、BNE、BLT、BGE | 比较两个寄存器，条件满足则跳转 |
| **U型** | 取高位立即数 | LUI、AUIPC | 20位立即数送寄存器高位 |
| **J型** | 无条件跳转 | JAL | 跳转并链接（保存返回地址到rd） |

**字段含义**：
- **opcode**（操作码）：7位，标识指令类型
- **rd**（destination register）：目标寄存器（5位，指向x0~x31）
- **rs1**（source register 1）：源寄存器1（5位）
- **rs2**（source register 2）：源寄存器2（5位）
- **funct3**：3位功能码，进一步区分opcode下的操作
- **funct7**：7位功能码，用于R型指令的进一步区分
- **imm**：立即数，按不同格式分布

> **自己的话**：x86的译码器需要处理1~15字节的可变长度指令，而RISC-V的所有指令都是固定的32位——译码器只需要看opcode和funct3/funct7就知道了要做什么。这是RISC-V设计简洁性的核心体现。

---

## 4. RV32I常用指令

### 4.1. 算术运算指令

| 指令 | 格式 | 功能 | C语言等价 | 示例 |
|:----|:---:|:-----|:---------|:-----|
| **ADD** | R | rd = rs1 + rs2 | `rd = rs1 + rs2` | `ADD x5, x6, x7` |
| **SUB** | R | rd = rs1 - rs2 | `rd = rs1 - rs2` | `SUB x5, x6, x7` |
| **ADDI** | I | rd = rs1 + imm | `rd = rs1 + 常数` | `ADDI x5, x6, 100` |
| **SLT** | R | rd = (rs1 < rs2) ? 1 : 0 | `rd = rs1 < rs2` | `SLT x5, x6, x7` |
| **SLTI** | I | rd = (rs1 < imm) ? 1 : 0 | `rd = rs1 < 常数` | `SLTI x5, x6, 100` |
| **LUI** | U | rd = imm << 12 | 加载20位高立即数 | `LUI x5, 0x12345` |
| **AUIPC** | U | rd = PC + (imm << 12) | 获取PC+偏移的地址 | `AUIPC x5, 0` |

**LUI使用示例**：
```
LUI x5, 0x12345       ; x5 = 0x12345000（加载高20位）
ADDI x5, x5, 0x678   ; x5 = 0x12345678（加上低12位）
```

### 4.2. 逻辑运算指令

| 指令 | 格式 | 功能 | 示例 |
|:----|:---:|:-----|:-----|
| **AND** | R | rd = rs1 & rs2 | `AND x5, x6, x7` |
| **OR** | R | rd = rs1 \| rs2 | `OR x5, x6, x7` |
| **XOR** | R | rd = rs1 ^ rs2 | `XOR x5, x6, x7` |
| **ANDI** | I | rd = rs1 & imm | `ANDI x5, x6, 0xFF` |
| **ORI** | I | rd = rs1 \| imm | `ORI x5, x6, 0xFF` |
| **XORI** | I | rd = rs1 ^ imm | `XORI x5, x6, 0xFF` |
| **SLL** | R | rd = rs1 << rs2 | 逻辑左移 |
| **SRL** | R | rd = rs1 >> rs2 | 逻辑右移 |
| **SRA** | R | rd = rs1 >> rs2（算术）| 算术右移（符号位扩展） |
| **SLLI** | I | rd = rs1 << imm | 立即数逻辑左移 |

**掩码操作示例**：
```
ADDI x5, x0, 0x3F    ; x5 = 0x0000003F（低6位掩码）
AND  x6, x6, x5       ; 取出x6的低6位
```

### 4.3. 数据传送指令

RISC-V作为RISC架构，只有**LOAD和STORE**两类指令访问主存，其余指令只在寄存器之间操作（这是与x86最大的区别之一）。

| 指令 | 格式 | 功能 | 示例 |
|:----|:---:|:-----|:-----|
| **LW** | I | rd = MEM[rs1 + imm]（32位加载） | `LW x5, 0(x6)` |
| **LH** | I | rd = MEM[rs1 + imm]（16位加载，符号扩展） | `LH x5, 0(x6)` |
| **LHU** | I | rd = MEM[rs1 + imm]（16位加载，零扩展） | `LHU x5, 0(x6)` |
| **LB** | I | rd = MEM[rs1 + imm]（8位加载，符号扩展） | `LB x5, 0(x6)` |
| **LBU** | I | rd = MEM[rs1 + imm]（8位加载，零扩展） | `LBU x5, 0(x6)` |
| **SW** | S | MEM[rs1 + imm] = rs2（32位存储） | `SW x5, 0(x6)` |
| **SH** | S | MEM[rs1 + imm] = rs2（16位存储） | `SH x5, 0(x6)` |
| **SB** | S | MEM[rs1 + imm] = rs2（8位存储） | `SB x5, 0(x6)` |

**访存示例**：
```
LW   x5, 0(x6)        ; x5 ← MEM[x6 + 0]    加载一个字
ADDI x5, x5, 1        ; x5 = x5 + 1         数值加1
SW   x5, 0(x6)        ; MEM[x6 + 0] = x5    存回主存
```

### 4.4. 分支与跳转指令

| 指令 | 格式 | 功能 | 条件 | 示例 |
|:----|:---:|:-----|:----|:-----|
| **BEQ** | B | if(rs1 == rs2) branch | 相等则跳转 | `BEQ x5, x6, label` |
| **BNE** | B | if(rs1 != rs2) branch | 不等则跳转 | `BNE x5, x6, label` |
| **BLT** | B | if(rs1 < rs2) branch | 有符号小于则跳转 | `BLT x5, x6, label` |
| **BGE** | B | if(rs1 >= rs2) branch | 有符号大于等于则跳转 | `BGE x5, x6, label` |
| **BLTU** | B | 无符号小于 | 无符号不等式 | `BLTU x5, x6, label` |
| **BGEU** | B | 无符号大于等于 | 无符号不等式 | `BGEU x5, x6, label` |
| **JAL** | J | rd = PC+4; PC += imm | 无条件跳转并链接 | `JAL x1, label` |
| **JALR** | I | rd = PC+4; PC = rs1+imm | 寄存器间接跳转并链接 | `JALR x1, x6, 0` |

**条件分支示例**：
```
LOOP:
    LW   x5, 0(x6)        ; 加载一个数据
    ADDI x5, x5, 1        ; 加1
    SW   x5, 0(x6)        ; 存回
    ADDI x6, x6, 4        ; 指针前移4字节
    BNE  x6, x7, LOOP     ; 若未结束则继续
```

**函数调用示例**：
```
    ADDI sp, sp, -16      ; 分配栈空间
    SW   ra, 12(sp)       ; 保存返回地址
    JAL  x1, my_func      ; 调用函数
    LW   ra, 12(sp)       ; 恢复返回地址
    ADDI sp, sp, 16       ; 释放栈空间
    JALR x0, x1, 0        ; 返回（x0忽略写入）
```

> **JALR x0, x1, 0 的含义**：将PC设为x1+0（即跳转到ra中保存的返回地址），x0忽略写入。这就是RISC-V的"返回指令"——虽然它不是专用的RET指令，但通过通用指令组合出RET的效果。

### 4.5. 同步指令

| 指令 | 格式 | 功能 |
|:----|:---:|:-----|
| **FENCE** | I | 内存屏障——确保之前的访存操作全部完成后再执行后续指令 |
| **ECALL** | I | 系统调用（environment call）——触发陷入，切换到操作系统 |
| **EBREAK** | I | 调试断点——触发调试器中断 |

---

## 5. RISC-V伪指令

伪指令（pseudo-instruction）是汇编器提供的语法糖——它们不是真正的机器指令，而是被汇编器翻译成一条或多条真实指令。RISC-V的伪指令设计得非常丰富：

| 伪指令 | 等价真实指令 | 功能 |
|:------|:-----------|:-----|
| **NOP** | `ADDI x0, x0, 0` | 空操作 |
| **MV rd, rs** | `ADDI rd, rs, 0` | 寄存器复制 |
| **LI rd, imm** | `LUI` + `ADDI`（大立即数） | 加载立即数（汇编器自动拆分） |
| **LA rd, label** | `AUIPC` + `ADDI` | 加载地址 |
| **RET** | `JALR x0, x1, 0` | 从函数返回 |
| **CALL label** | `JAL x1, label` | 函数调用 |
| **J label** | `JAL x0, label` | 无条件跳转 |
| **NEG rd, rs** | `SUB rd, x0, rs` | 取负 |
| **SEQZ rd, rs** | `SLTIU rd, rs, 1` | 判断是否等于0 |
| **SNEZ rd, rs** | `SLTU rd, x0, rs` | 判断是否不等于0 |
| **BEQZ rs, label** | `BEQ rs, x0, label` | 等于0则跳转 |
| **BNEZ rs, label** | `BNE rs, x0, label` | 不等于0则跳转 |

**示例对比**：
```
; 不用伪指令的写法
ADDI x5, x0, 0          ; x5 = 0（相当于MV）

; 用伪指令的写法
MV    x5, x0             ; 更清晰——"把x0移动到x5"
RET                      ; 返回——而不是JALR x0, x1, 0
LI    x5, 0x12345678     ; 汇编器自动生成LUI+ADDI
```

---

## 6. 汇编程序结构

### 6.1. 基本框架

一个典型的RISC-V汇编程序：

```assembly
.section .data          ; 数据段
    var1: .word 0x1234  ; 定义一个32位变量
    arr:  .word 1,2,3,4 ; 定义一个数组
    str:  .asciz "Hello"; 定义一个字符串

.section .text          ; 代码段
.globl main             ; 声明main为全局符号
main:                   ; 程序入口
    addi sp, sp, -16    ; 分配栈空间
    sw   ra, 12(sp)     ; 保存返回地址
    
    ; 主程序代码
    li   a0, 42         ; 参数a0=42
    jal  x1, my_func    ; 调用my_func
    
    lw   ra, 12(sp)     ; 恢复返回地址
    addi sp, sp, 16     ; 释放栈空间
    ret                 ; 返回

my_func:
    addi sp, sp, -8     ; 分配栈空间
    sw   s0, 4(sp)      ; 保存s0
    addi s0, sp, 8      ; 设置帧指针
    
    ; 函数体
    addi a0, a0, 1      ; a0 = a0 + 1
    
    lw   s0, 4(sp)      ; 恢复s0
    addi sp, sp, 8      ; 释放栈空间
    ret                 ; 返回
```

### 6.2. 常用汇编指令

| 伪指令 | 功能 |
|:------|:-----|
| `.section .data` | 声明数据段 |
| `.section .text` | 声明代码段 |
| `.globl sym` | 声明sym为全局符号 |
| `.word val1, val2, ...` | 定义32位数据 |
| `.half val1, val2, ...` | 定义16位数据 |
| `.byte val1, val2, ...` | 定义8位数据 |
| `.asciz "str"` | 定义以'\0'结尾的字符串 |
| `.space n` | 预留n字节空间 |

---

### 编程题

1. **编写RISC-V汇编程序，计算1到100的累加和。**

```assembly
.section .text
.globl main
main:
    addi sp, sp, -4
    sw   ra, 0(sp)
    
    li   a0, 0          ; sum = 0
    li   a1, 1          ; i = 1
    li   a2, 100        ; n = 100
loop:
    add  a0, a0, a1     ; sum += i
    addi a1, a1, 1      ; i++
    ble  a1, a2, loop   ; if (i <= 100) goto loop
    
    lw   ra, 0(sp)
    addi sp, sp, 4
    ret
```

2. **实现数组求和。假设数组首地址在a0中，数组长度在a1中。**

```assembly
array_sum:
    li   t0, 0          ; sum = 0
    li   t1, 0          ; i = 0
loop:
    bge  t1, a1, done   ; if (i >= n) done
    slli t2, t1, 2      ; t2 = i * 4（字节偏移）
    add  t2, a0, t2     ; t2 = &arr[i]
    lw   t3, 0(t2)      ; t3 = arr[i]
    add  t0, t0, t3     ; sum += arr[i]
    addi t1, t1, 1      ; i++
    j    loop
done:
    mv   a0, t0         ; 返回值 = sum
    ret
```

---

## 本讲核心要点

1. **RISC-V是开源RISC架构**——任何人都可以免费使用，模块化设计（基础集+扩展），生态成熟
2. **32个通用寄存器**（x0~x31）——x0恒为0，ABI命名规范（ra/sp/a0~a7/t0~t6/s0~s11）
3. **六种指令格式**（R/I/S/B/U/J）——所有指令32位固定长度，译码器极其简单
4. **Load-Store架构**——只有LW/SW等访存指令操作主存，算术逻辑指令只操作寄存器
5. **R型格式**：`funct7 | rs2 | rs1 | funct3 | rd | opcode`——用于ADD/SUB/AND等
6. **I型格式**：`imm[11:0] | rs1 | funct3 | rd | opcode`——用于ADDI/LW/ADDI等
7. **B型格式**：`imm[12]|imm[10:5]|rs2|rs1|funct3|imm[4:1]|imm[11]|opcode`——用于BEQ/BNE/BLT等
8. **伪指令**使代码更可读——NOP（空操作）、MV（寄存器移动）、RET（返回）、LI（加载立即数）
9. **函数调用惯例**：用JAL保存返回地址到ra，函数返回用RET（即JALR x0,x1,0）
10. **与x86的对比**：寄存器数量更多（32 vs 8）、指令格式统一（vs可变长度）、访存更精简（vs复杂寻址）
