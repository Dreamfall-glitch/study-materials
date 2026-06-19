# 04-非周期信号的傅里叶变换与DTFT

## 知识地图

```
周期信号的傅里叶级数（已学）—— 离散频谱、谐波分解
    ↓
【非周期信号的傅里叶变换】—— 周期 T→∞，离散谱变为连续谱
    ↓
拉普拉斯变换 → Z变换（对变换域的进一步推广）
```

**本讲回答的核心问题**：非周期信号的频谱是连续的——如何用傅里叶变换来描述？连续和离散的非周期信号在频域中如何对应？傅里叶变换有哪些重要性质和应用？

---

## 1. 从傅里叶级数到傅里叶变换

### 1.1. 傅里叶变换的引出

周期信号 $f_T(t)$ 可以展开为傅里叶级数 $f_T(t) = \sum_{n=-\infty}^{\infty} F_n e^{jn\omega_0 t}$，其中 $F_n = \frac{1}{T}\int_{-T/2}^{T/2} f_T(t) e^{-jn\omega_0 t}dt$。

当周期 $T \to \infty$ 时，信号变为非周期 $f(t)$。此时 $\omega_0 = 2\pi/T \to 0$，离散谱 $F_n$ 变为连续谱。记 $\omega = n\omega_0$，并考虑谱密度函数：

$$F(j\omega) = \lim_{T\to\infty} T\cdot F_n = \int_{-\infty}^{\infty} f(t) e^{-j\omega t}dt$$

### 1.2. 傅里叶变换的定义

**傅里叶正变换**（从时域到频域）：

$$
\boxed{F(j\omega) = \mathcal{F}[f(t)] = \int_{-\infty}^{\infty} f(t) e^{-j\omega t}\,dt}
$$

**傅里叶逆变换**（从频域恢复时域）：

$$
\boxed{f(t) = \mathcal{F}^{-1}[F(j\omega)] = \frac{1}{2\pi}\int_{-\infty}^{\infty} F(j\omega) e^{j\omega t}\,d\omega}
$$

**傅里叶变换存在的充分条件**（Dirichlet 条件在非周期下的推广）：
- $f(t)$ 在任意有限区间内满足 Dirichlet 条件
- $f(t)$ **绝对可积**：$\int_{-\infty}^{\infty} |f(t)| dt < \infty$
- 对某些不满足绝对可积但能量有限的信号（如 $\sin$、$u(t)$），其傅里叶变换可通过引入 $\delta$ 函数或取极限来定义（广义傅里叶变换）

> **理解**：$F(j\omega)$ 是信号在各频率上的**密度分布**——类似于概率密度，它在某个频率 $\omega$ 处的值表示该频率附近单位频带内的"频谱含量"。而傅里叶级数的 $F_n$ 是离散频率点上的"实际值"。

### 1.3. 傅里叶变换的物理意义

$F(j\omega)$ 一般为复函数，可写为：

$$F(j\omega) = |F(j\omega)| e^{j\varphi(\omega)}$$

- $|F(j\omega)|$：**幅度谱密度**（简称幅度谱）
- $\varphi(\omega)$：**相位谱**

---

## 2. 常用傅里叶变换对

以下变换对必须熟练掌握（建议熟记前 10 个）：

| 序号 | 时域 $f(t)$ | 频域 $F(j\omega)$ | 说明 |
|:---:|:-----------|:-----------------|:----|
| 1 | $\delta(t)$（单位冲激） | $1$ | 冲激的频谱是均匀的——包含所有频率分量 |
| 2 | $1$（常数） | $2\pi\delta(\omega)$ | 直流信号的频谱是零频处的冲激 |
| 3 | $\delta(t - t_0)$ | $e^{-j\omega t_0}$ | 时延 $\to$ 相位线性变化 |
| 4 | $e^{j\omega_0 t}$（复指数） | $2\pi\delta(\omega - \omega_0)$ | 单频信号在频域为冲激 |
| 5 | $\cos(\omega_0 t)$ | $\pi[\delta(\omega-\omega_0) + \delta(\omega+\omega_0)]$ | 实正弦在 $\pm\omega_0$ 处各有一冲激 |
| 6 | $\sin(\omega_0 t)$ | $j\pi[\delta(\omega+\omega_0) - \delta(\omega-\omega_0)]$ | 与 $\cos$ 相差 $90^\circ$ 相位 |
| 7 | $u(t)$（单位阶跃） | $\pi\delta(\omega) + \frac{1}{j\omega}$ | 包含直流分量 + 非周期部分 |
| 8 | $\text{sgn}(t)$（符号函数） | $\frac{2}{j\omega}$ | 奇函数的傅里叶变换为纯虚函数 |
| 9 | $e^{-at}u(t),\ a>0$（单边指数） | $\frac{1}{a + j\omega}$ | 一阶系统的冲激响应 |
| 10 | $e^{-a\|t\|},\ a>0$（双边指数） | $\frac{2a}{a^2 + \omega^2}$ | 偶函数的傅里叶变换为实偶函数 |
| 11 | $g_\tau(t)$（矩形脉冲，宽度 $\tau$，高度 $1$） | $\tau\,\text{Sa}\left(\frac{\omega\tau}{2}\right)$ | Sa 函数包络，第一零点 $\omega = 2\pi/\tau$ |
| 12 | $\text{Sa}\left(\frac{Wt}{2}\right)$ | $\frac{2\pi}{W}\,\text{g}_W(\omega)$ | 时频对偶性——Sa 函数 $\to$ 矩形谱 |
| 13 | $\delta_T(t) = \sum \delta(t - nT)$（冲激串） | $\omega_0\sum \delta(\omega - n\omega_0)$ | 时域冲激串 $\iff$ 频域冲激串 |
| 14 | $e^{-at}\sin(\omega_0 t)u(t)$ | $\frac{\omega_0}{(a+j\omega)^2 + \omega_0^2}$ | 衰减振荡 |
| 15 | $e^{-at}\cos(\omega_0 t)u(t)$ | $\frac{a + j\omega}{(a+j\omega)^2 + \omega_0^2}$ | 衰减振荡 |

### 2.1. 矩形脉冲的频谱分析

矩形脉冲 $g_\tau(t)$（幅度 $1$，宽度 $\tau$）：

$$F(j\omega) = \int_{-\tau/2}^{\tau/2} 1 \cdot e^{-j\omega t}dt = \frac{2\sin(\omega\tau/2)}{\omega} = \tau\,\text{Sa}\left(\frac{\omega\tau}{2}\right)$$

其中 $\text{Sa}(x) = \sin x / x$ 为**抽样函数**。

**频谱特点**：
- 主瓣宽度（第一零点带宽）：$\Delta\omega = 2\pi/\tau$，$\Delta f = 1/\tau$
- 脉冲宽度 $\tau$ 越小 $\to$ 带宽越大 $\to$ **时宽-带宽积为常数**（$\tau \cdot \Delta f \approx 1$）
- 旁瓣幅度随频率增大而衰减，能量主要集中在主瓣内

> **时宽-带宽积**：信号的时域宽度和频域宽度的乘积是一个常数（约等于 $1$）。这意味着时间上越窄的信号（如极窄脉冲），频率上占据的带宽越宽——这是通信系统中"高速率需要大带宽"的根本原因。

---

## 3. 傅里叶变换的性质

以下性质是最常用的，构成傅里叶变换应用的基础：

| 序号 | 性质 | 时域 $f(t)$ | 频域 $F(j\omega)$ | 条件/说明 |
|:---:|:----:|:-----------|:-----------------|:---------|
| 1 | **线性** | $af_1(t) + bf_2(t)$ | $aF_1(j\omega) + bF_2(j\omega)$ | 叠加性 |
| 2 | **时移** | $f(t - t_0)$ | $F(j\omega) e^{-j\omega t_0}$ | 时延 $\to$ 相位线性变化 |
| 3 | **频移/调制** | $f(t) e^{j\omega_0 t}$ | $F(j(\omega - \omega_0))$ | 频谱搬移 |
| 4 | **尺度变换** | $f(at)$ | $\frac{1}{|a|}F\!\left(j\frac{\omega}{a}\right)$ | $a>1$ 压缩时域 $\to$ 扩展频域 |
| 5 | **反转** | $f(-t)$ | $F(-j\omega)$ | 时域反转 $\to$ 频域反转 |
| 6 | **共轭对称** | $f^*(t)$ | $F^*(-j\omega)$ | 实信号：$F(-j\omega) = F^*(j\omega)$ |
| 7 | **时域微分** | $\frac{d^n}{dt^n}f(t)$ | $(j\omega)^n F(j\omega)$ | 微分 $\to$ 高频增强 |
| 8 | **时域积分** | $\int_{-\infty}^{t} f(\tau)d\tau$ | $\pi F(0)\delta(\omega) + \frac{1}{j\omega}F(j\omega)$ | 注意直流项 |
| 9 | **频域微分** | $(-jt)^n f(t)$ | $\frac{d^n}{d\omega^n}F(j\omega)$ | 时域乘 $t$ $\to$ 频域微分 |
| 10 | **时域卷积** | $f_1(t) * f_2(t)$ | $F_1(j\omega) \cdot F_2(j\omega)$ | **最重要的性质！** |
| 11 | **频域卷积** | $f_1(t) \cdot f_2(t)$ | $\frac{1}{2\pi}F_1(j\omega) * F_2(j\omega)$ | 时域相乘 $\to$ 频域卷积 |
| 12 | **帕塞瓦尔定理** | $\int_{-\infty}^{\infty}\|f(t)\|^2 dt = \frac{1}{2\pi}\int_{-\infty}^{\infty}\|F(j\omega)\|^2 d\omega$ | 能量守恒 |
| 13 | **对偶性** | 若 $f(t) \leftrightarrow F(j\omega)$，则 $F(t) \leftrightarrow 2\pi f(-\omega)$ | 时频对称 |

### 3.1. 时域卷积定理（最重要的性质）

$$\boxed{f_1(t) * f_2(t) \longleftrightarrow F_1(j\omega) \cdot F_2(j\omega)}$$

**意义**：时域中复杂的卷积运算对应频域中简单的乘法运算。这就是为什么在频域分析系统比在时域更方便——时域的卷积积分变为频域的代数乘积。

### 3.2. 频域卷积定理（调制性质）

$$\boxed{f_1(t) \cdot f_2(t) \longleftrightarrow \frac{1}{2\pi}F_1(j\omega) * F_2(j\omega)}$$

**应用**：调制——将信号乘以载波 $\cos(\omega_0 t)$ 相当于将信号的频谱搬移到 $\pm\omega_0$ 处。这正是**幅度调制（AM）**的理论基础。

### 3.3. 对偶性

若 $f(t)$ 的傅里叶变换为 $F(j\omega)$，则 $F(jt)$ 的傅里叶变换为 $2\pi f(-\omega)$。

**示例**：矩形脉冲的傅里叶变换是 Sa 函数——由对偶性，Sa 函数的傅里叶变换是矩形脉冲（的 $2\pi$ 倍）。这种时频对称性可以用来推导新的傅里叶变换对。

### 3.4. 尺度变换性质

$$f(at) \longleftrightarrow \frac{1}{|a|}F\!\left(j\frac{\omega}{a}\right)$$

**理解**：时域压缩（$a > 1$）$\to$ 频域展宽且幅度降低。信号的**时宽-带宽积**守恒——时间分辨率与频率分辨率不能同时提高，这是**不确定原理**在信号分析中的体现。

---

## 4. 周期信号的傅里叶变换（广义傅里叶变换）

对于周期信号（不满足绝对可积条件），可以引入 $\delta$ 函数求其傅里叶变换：

$$f(t) = \sum_{n=-\infty}^{\infty} F_n e^{jn\omega_0 t}$$

取傅里叶变换，利用 $e^{jn\omega_0 t} \longleftrightarrow 2\pi\delta(\omega - n\omega_0)$：

$$\boxed{F(j\omega) = 2\pi \sum_{n=-\infty}^{\infty} F_n \delta(\omega - n\omega_0)}$$

**物理意义**：周期信号的傅里叶变换是位于各谐波频率处的冲激串——冲激的强度正比于该谐波的傅里叶系数 $F_n$。

**示例**：$\cos(\omega_0 t) \longleftrightarrow \pi[\delta(\omega-\omega_0) + \delta(\omega+\omega_0)]$

---

## 5. 离散时间傅里叶变换（DTFT）

### 5.1. DTFT 的定义

对于离散时间序列 $x[n]$，其**离散时间傅里叶变换**（DTFT）定义为：

$$
\boxed{X(e^{j\Omega}) = \sum_{n=-\infty}^{\infty} x[n] e^{-j\Omega n}}
$$

**逆变换**（IDTFT）：

$$
\boxed{x[n] = \frac{1}{2\pi}\int_{-\pi}^{\pi} X(e^{j\Omega}) e^{j\Omega n}\,d\Omega}
$$

其中 $\Omega = \omega T_s$ 为**归一化数字角频率**（单位：弧度/样本），$T_s$ 为采样间隔。

### 5.2. DTFT 的关键性质

1. **周期性**：$X(e^{j(\Omega + 2\pi)}) = X(e^{j\Omega})$——DTFT 是 $\Omega$ 的周期函数，周期为 $2\pi$
2. **与 CTFT 的关系**：若 $x[n] = x_a(nT_s)$（对模拟信号 $x_a(t)$ 采样），则：

   $$X(e^{j\Omega}) = \frac{1}{T_s}\sum_{k=-\infty}^{\infty} X_a\!\left(j\frac{\Omega - 2\pi k}{T_s}\right)$$

   即：DTFT 是 CTFT 的**周期延拓**（以 $2\pi/T_s$ 为周期）——这就是采样定理的频域表达。

3. **收敛条件**：$\sum_{n=-\infty}^{\infty} |x[n]| < \infty$（绝对可和）

### 5.3. 常用 DTFT 变换对

| 序号  | 时域 $x[n]$                | 频域 $X(e^{j\Omega})$                                                          | 条件     |
| :-: | :----------------------- | :--------------------------------------------------------------------------- | :----- |
|  1  | $\delta[n]$（单位脉冲）        | $1$                                                                          |        |
|  2  | $1$（常数）                  | $2\pi\sum_{k=-\infty}^{\infty}\delta(\Omega - 2\pi k)$                       | 周期延拓   |
|  3  | $a^n u[n]$（$ \|a\| < 1$） | $\frac{1}{1 - a e^{-j\Omega}}$                                               | 指数衰减序列 |
|  4  | $u[n]$（单位阶跃）             | $\frac{1}{1 - e^{-j\Omega}} + \pi\sum_{k}\delta(\Omega - 2\pi k)$            |        |
|  5  | $\cos(\Omega_0 n)$       | $\pi\sum_k[\delta(\Omega-\Omega_0-2\pi k) + \delta(\Omega+\Omega_0-2\pi k)]$ |        |

### 5.4. CTFT 与 DTFT 的对比

| 项目 | CTFT（连续傅里叶变换） | DTFT（离散时间傅里叶变换） |
|:----:|:--------------------:|:------------------------:|
| **时域** | 连续 $t$ | 离散 $n$（整数） |
| **频域** | 连续 $\omega$（非周期） | 连续 $\Omega$（周期 $2\pi$） |
| **正变换** | $F(j\omega) = \int_{-\infty}^{\infty} f(t) e^{-j\omega t}dt$ | $X(e^{j\Omega}) = \sum_{-\infty}^{\infty} x[n] e^{-j\Omega n}$ |
| **逆变换** | $f(t) = \frac{1}{2\pi}\int_{-\infty}^{\infty} F(j\omega) e^{j\omega t}d\omega$ | $x[n] = \frac{1}{2\pi}\int_{-\pi}^{\pi} X(e^{j\Omega}) e^{j\Omega n}d\Omega$ |
| **频域周期性** | 非周期 | 周期 $2\pi$ |
| **时频对偶性** | 时域连续 $\to$ 频域非周期 | 时域离散 $\to$ 频域周期 |
| **适用** | 模拟信号分析 | 数字信号分析 |

---

## 6. DTFT 与 DFT 的关系

**DTFT** 是 $\Omega$ 的连续函数（周期 $2\pi$），**DFT** 是对 DTFT 在一个周期内的等间隔采样。

对长度为 $N$ 的序列 $x[n]$，其 DFT $X[k]$ 与 DTFT $X(e^{j\Omega})$ 的关系为：

$$X[k] = X(e^{j\Omega})\big|_{\Omega = 2\pi k/N},\quad k = 0, 1, \dots, N-1$$

**物理意义**：DFT 是在 DTFT 的频率轴上以 $\Delta\Omega = 2\pi/N$ 为间隔的**频域采样**。DFT 是计算机可以实际计算的傅里叶变换形式，通过 FFT（快速傅里叶变换）算法高效实现。

---

## 7. 傅里叶变换的应用

### 7.1. 系统的频率响应与滤波

系统冲激响应 $h(t)$ 的傅里叶变换 $H(j\omega)$ 即为系统的**频率响应**：

$$H(j\omega) = \int_{-\infty}^{\infty} h(t) e^{-j\omega t}dt$$

由卷积定理，系统对输入 $e(t)$ 的零状态响应为：

$$R(j\omega) = E(j\omega) \cdot H(j\omega)$$

**理想滤波器**的频率响应：
- **理想低通**：$H(j\omega) = \begin{cases}1, & |\omega| < \omega_c \\ 0, & |\omega| > \omega_c\end{cases}$
- **理想高通**、**带通**、**带阻**类似定义

> **注意**：理想低通滤波器在时域对应的冲激响应为 Sa 函数，是非因果的——因此理想滤波器物理不可实现，只能逼近。

### 7.2. 无失真传输条件

信号通过系统无失真时，输出 $\propto$ 输入延时 $t_0$：

$$r(t) = K e(t - t_0) \iff R(j\omega) = K E(j\omega) e^{-j\omega t_0}$$

因此无失真传输的条件为：

$$
|H(j\omega)| = K\ (\text{常数}),\quad \varphi(\omega) = -\omega t_0\ (\text{线性相位})
$$

即系统的幅度响应为常数，相位响应与频率成线性关系。

### 7.3. 采样定理

对模拟信号 $x_a(t)$ 以采样间隔 $T_s$ 进行采样，得到 $x[n] = x_a(nT_s)$。采样信号的傅里叶变换为：

$$X_s(j\omega) = \frac{1}{T_s}\sum_{k=-\infty}^{\infty} X_a(j(\omega - k\omega_s)),\quad \omega_s = 2\pi/T_s$$

**奈奎斯特采样定理**：若 $x_a(t)$ 是带限信号（最高频率 $\omega_m$），则当采样频率 $\omega_s > 2\omega_m$ 时，可以从采样信号中无失真地恢复原模拟信号。$\omega_s/2$ 称为**奈奎斯特频率**。

> **混叠**：若 $\omega_s < 2\omega_m$，频谱会发生混叠（aliasing），无法恢复原信号。实际系统中，采样前需加**抗混叠滤波器**（低通滤波器截止频率为 $\omega_s/2$）。

---

## 本讲核心要点

1. **傅里叶变换** $F(j\omega) = \int_{-\infty}^{\infty} f(t) e^{-j\omega t}dt$ 是傅里叶级数在 $T\to\infty$ 时的推广——从离散谱变为连续谱密度函数
2. **时域卷积定理** $f_1 * f_2 \longleftrightarrow F_1 \cdot F_2$ 是傅里叶变换最重要的性质——时域卷积简化为频域乘法
3. **频域卷积定理**（调制性质）是通信系统的基础——乘载波 $\to$ 频谱搬移
4. **尺度变换性质**揭示了时宽-带宽积守恒——时间分辨率与频率分辨率不可兼得
5. **DTFT** 是离散时间序列的傅里叶分析工具——频域周期 $2\pi$，是CTFT的周期延拓
6. **DTFT 与 DFT 的关系**：DFT 是 DTFT 在频域的等间隔采样——FFT 使 DFT 可计算
7. **周期信号的傅里叶变换**是位于各谐波处的冲激串——统一了周期与非周期信号的频域描述
8. **系统频率响应** $H(j\omega)$ 完全描述了 LTI 系统的频域特性——$R = E \cdot H$
9. **采样定理** $\omega_s > 2\omega_m$ 是模拟与数字信号的桥梁——采样前必须使用抗混叠滤波器
10. **15个常用变换对**需要熟练记忆，它们是频域分析的基础工具
