# 02-Transformer架构

## 知识地图

```
Attention 机制 —— 从 CV 借来的"关注重点"思想
    │
    ├── 基本形式：Q·K^T → Softmax → 加权 V
    │       │
    │       ├── 自注意力 (Self-Attention)：Q=K=V，建模序列内部依赖
    │       ├── 掩码自注意力 (Masked)：遮蔽未来 token，实现并行语言模型
    │       └── 多头注意力 (Multi-Head)：多组并行，捕获不同语义关系
    │
    ├── Encoder：自注意力 + FNN → 编码输入序列为上下文表示
    │       └── 辅助组件：LayerNorm、残差连接、位置编码
    │
    ├── Decoder：掩码自注意力 + 交叉注意力 + FNN → 自回归生成
    │
    └── 完整 Transformer：Embedding → Positional Encoding → N×(Enc + Dec) → Linear → Softmax
```

**本讲回答的核心问题**：为什么 RNN/LSTM 统治 NLP 多年后被 Transformer 彻底取代？注意力机制如何用三个矩阵（Q、K、V）实现对序列中任意两个位置的直接建模？从零搭建一个 Transformer 需要哪些组件、每个组件解决什么问题？

---

## 1. 注意力机制

### 1.1. 从 RNN 的困境到 Attention 的诞生

在 Transformer 出现之前，NLP 领域的主流架构是 RNN 及其变体 LSTM。第一章介绍的 ELMo 就使用了双向 LSTM。RNN 有两个难以弥补的缺陷：

1. **无法并行计算**：序列必须逐 token 输入、依序计算，GPU 的并行能力被严重浪费。即便参数量不大，训练时间成本却极高。
2. **长距离依赖弱**：距离越远的 token 之间的关系越难被捕捉。LSTM 的门机制有所缓解但未根本解决。

> **通俗理解**：RNN 像一个人逐字阅读——读到第 100 个字时，第 1 个字已经模糊了。Transformer 则像同时看整段文字，任何两个字之间的关系都可以直接计算。

Vaswani 等学者将 CV 领域提出的注意力机制独立出来，搭建了完全由注意力构成的 Transformer——《Attention Is All You Need》，由此开启了 LLM 时代。

### 1.2. 注意力机制的核心概念

注意力机制的三个核心变量：

| 变量 | 全称 | 角色 |
|:----|:-----|:-----|
| **Q**（Query） | 查询值 | 表示"要找什么"，代表当前关注的焦点 |
| **K**（Key） | 键值 | 表示"有什么可匹配"，用于和 Q 计算相关性 |
| **V**（Value） | 真值 | 表示"实际内容"，被加权求和后作为输出 |

> **通俗理解**：把注意力机制类比为"词典查找"——Q 是你想查的词，K 是词典的索引，V 是词典的释义。但和精确的词典查找不同，注意力机制不要求 Q 与某个 K 完全匹配，而是计算 Q 与所有 K 的相似度，按相似度比例混合所有 V。查"fruit"时既拿到 apple 的释义也拿到 banana 的释义，各取一部分。

### 1.3. 注意力公式的推导

从词典类比出发，逐步推导标准注意力公式：

**第一步—点积度量相似度**：第一章讲过，语义相似的词在向量空间中点积较大。用 Q 向量与每个 K 向量的点积衡量相似程度：

$$x = qK^T$$

**第二步—Softmax 转化为权重**：将相似度分数转化为和为 1 的概率分布，即注意力权重：

$$\text{softmax}(x)_i = \frac{e^{x_i}}{\sum_{j}e^{x_j}}$$

**第三步—加权求和**：用注意力权重对 V 加权求和，得到最终输出：

$$\text{attention}(Q,K,V) = \text{softmax}(QK^T)V$$

**第四步—缩放因子**：当 K 的维度 $d_k$ 较大时，点积值的方差也随之增大，导致 Softmax 梯度趋近于极端值（0 或 1），梯度消失。除以 $\sqrt{d_k}$ 稳定训练：

$$\boxed{\text{Attention}(Q,K,V) = \text{softmax}\left(\frac{QK^T}{\sqrt{d_k}}\right)V}$$

这就是注意力机制的最终标准形式。`torch.matmul` + 除法 + `softmax` + `torch.matmul`，四行代码。

（可以补充注意力计算流程图）

### 1.4. 自注意力（Self-Attention）

在经典注意力中，Q 来自一个序列，K、V 来自另一个序列。例如 Transformer Decoder 中的交叉注意力：Q 来自 Decoder 输入，K、V 来自 Encoder 输出。

**自注意力**则是 Q、K、V 都来自同一个输入，通过三个不同的参数矩阵 $W_q$、$W_k$、$W_v$ 投影得到：

```
x (输入序列) → 分别乘 W_q, W_k, W_v → Q, K, V → Attention(Q, K, V)
```

这使每个 token 都能直接关注序列中的所有其他 token，建模全局依赖关系。

```python
# 自注意力：Q、K、V 来自同一输入
attention(x, x, x)
```

### 1.5. 掩码自注意力（Masked Self-Attention）

**核心动机**：语言模型的任务是"根据前文预测下一个词"。如果不加限制，自注意力会让每个 token 看到后面的词（未来信息），等于作弊。

**掩码机制**：生成一个上三角矩阵作为掩码，上三角位置设为 $-\infty$，下三角设为 0。将掩码加到注意力分数上再 Softmax——$-\infty$ 经过 Softmax 后变为 0，从而遮蔽未来 token。

输入序列：【BOS】I like you【EOS】，掩码矩阵 $M$ 为上三角 $-\infty$ 矩阵：

$$
\begin{array}{c|ccccc}
M & \text{【BOS】} & \text{I} & \text{like} & \text{you} & \text{【EOS】} \\
\hline
\text{【BOS】} & 0 & -\infty & -\infty & -\infty & -\infty \\
\text{I} & 0 & 0 & -\infty & -\infty & -\infty \\
\text{like} & 0 & 0 & 0 & -\infty & -\infty \\
\text{you} & 0 & 0 & 0 & 0 & -\infty \\
\text{【EOS】} & 0 & 0 & 0 & 0 & 0
\end{array}
$$

计算时执行 $\text{scores} = \text{scores} + M$，Softmax 将 $-\infty$ 位置归零。

**关键洞察**：掩码矩阵使得所有时间步可以并行输入，模型仍只使用历史信息预测——这正是 Transformer 比 RNN 高效的根本原因。RNN 必须串行 step by step，Transformer 一次性并行处理整个序列。

```python
# 生成上三角掩码矩阵
mask = torch.full((1, 1, seq_len, seq_len), float("-inf"))
mask = torch.triu(mask, diagonal=1)
# 在注意力计算中：scores = scores + mask
```

（可以补充掩码自注意力示意图）

### 1.6. 多头注意力（Multi-Head Attention）

**核心动机**：单次注意力计算只能捕获一种关系模式，但语言中同时存在语法关系、语义关系、指代关系等多种依赖。多头注意力用多组 $W_q$、$W_k$、$W_v$ 并行计算，每组（一个"头"）关注不同的关系维度。

$$\boxed{\text{MultiHead}(Q, K, V) = \text{Concat}(\text{head}_1, \dots, \text{head}_h)W^O}$$

$$\text{head}_i = \text{Attention}(QW_i^Q, KW_i^K, VW_i^V)$$

| 概念 | 说明 |
|:-----|:-----|
| **头的数量 h** | 通常取 8 或 12，每个头维度 = $d_{\text{model}} / h$ |
| **组合矩阵技巧** | 不是创建 h 组独立参数矩阵，而是用一个大的线性层同时计算所有头，再 reshape + transpose 拆分 |
| **拼接后投影** | 所有头的输出拼接后经 $W^O$ 线性变换，恢复原始维度 |

> **通俗理解**：单头注意力像一个人读文章，只能从一个角度理解。多头注意力像 8 个人同时读同一篇文章——有人关注语法结构，有人关注实体关系，有人关注情感色彩——最后把 8 个人的理解汇总。论文实验也证实，不同注意力头确实学到了不同层次的语言特征。

**习题 1**：某 Transformer 模型的隐藏维度 $d_{\text{model}} = 512$，使用 $h = 8$ 个注意力头。请问每个注意力头的维度是多少？如果输入序列长度 $L = 128$，batch size $B = 32$，计算 Q、K、V 各自经过线性投影后的张量形状。

**解**：
- 每个头维度：$d_k = d_{\text{model}} / h = 512 / 8 = 64$
- 线性投影后 Q、K、V 形状均为 $(B, L, d_{\text{model}}) = (32, 128, 512)$
- 拆分为多头后：$(B, h, L, d_k) = (32, 8, 128, 64)$
- 注意力分数矩阵：$(32, 8, 128, 128)$，即每个 batch 的每个头都有一个 $128 \times 128$ 的注意力方阵

---

## 2. Encoder-Decoder 架构

### 2.1. Seq2Seq：Transformer 的任务背景

**Seq2Seq（序列到序列）**：输入一个序列，输出另一个（可能不等长的）序列。几乎所有 NLP 任务都可以归约为 Seq2Seq：

| 任务 | 输入序列 | 输出序列 |
|:-----|:---------|:---------|
| 机器翻译 | 源语言句子 | 目标语言句子 |
| 文本分类 | 文档 | 类别标签（长度 1） |
| 词性标注 | 句子 | 等长标签序列 |

Transformer 最初正是为机器翻译设计的 Seq2Seq 模型，沿用经典的"编码—解码"范式：
- **Encoder**：将输入序列编码为富含上下文信息的向量表示
- **Decoder**：根据编码结果自回归地生成目标序列

（可以补充Transformer整体架构图）

### 2.2. 前馈神经网络（FNN）

每个 Encoder/Decoder Layer 在注意力层之后都有一个**位置独立的前馈网络**：

```python
class MLP(nn.Module):
    def forward(self, x):
        # dim → hidden_dim (如 4×dim) → dim
        return self.dropout(self.w2(F.relu(self.w1(x))))
```

特点：**两个线性层 + ReLU 激活**，中间隐藏维度通常膨胀到输入的 4 倍。

> **通俗理解**：注意力层负责"交流"——让每个 token 和其他 token 交换信息。FNN 层负责"思考"——每个 token 独立地对收集到的信息进行非线性变换和加工。两者交替，形成 Transformer Layer 的核心节奏。

### 2.3. 层归一化（LayerNorm）

深层网络中，每一层的输入分布会随参数更新而漂移（内部协变量偏移），导致训练不稳定。归一化操作将每层输入拉回标准正态分布。

| 归一化类型 | 统计维度 | 问题 |
|:----------|:--------|:-----|
| **Batch Norm** | 跨 batch 内样本 | 小 batch 时不稳定，不适用于 RNN 变长序列 |
| **Layer Norm** | 跨单个样本的所有特征 | 每个样本独立归一化，适合 NLP |

$$\widetilde{Z_j} = \frac{Z_j - \mu_j}{\sqrt{\sigma^2 + \epsilon}}$$

LayerNorm 在每个样本上计算所有特征维度的均值和方差，对 batch size 不敏感，天然适合处理变长序列。

**注意**：Transformer 原论文图示是 Post-Norm（LayerNorm 在子层之后），但实际发布的代码和现代 LLM 普遍采用 Pre-Norm（LayerNorm 在子层之前），后者训练更稳定。

### 2.4. 残差连接（Residual Connection）

深层网络容易退化——层数增加反而效果变差。残差连接让每层的输出 = 原输入 + 该层的变换结果：

$$x = x + \text{SubLayer}(\text{LayerNorm}(x))$$

> **通俗理解**：残差连接相当于给信息一条"高速通道"——即使某个子层的变换效果不佳，原始信息仍能无损地传递到下一层。模型只需学习"残差"（即当前层需要修正的部分），大大降低了深层网络的训练难度。

### 2.5. Encoder 结构

每个 Encoder Layer 包含两个子层：

```
输入 x
  │
  ├→ LayerNorm → 多头自注意力 → + x  ─┐
  │                                     ├→ 输出
  └→ LayerNorm → FNN ──────────── + ───┘
```

- **自注意力**：Q=K=V=x，建模输入序列内部依赖
- **FNN**：位置独立非线性变换
- 原论文中 $N = 6$，即 6 个 Encoder Layer 堆叠

### 2.6. Decoder 结构

每个 Decoder Layer 包含三个子层：

```
输入 x（目标序列）        Encoder 输出 enc_out
  │                           │
  ├→ LayerNorm → 掩码自注意力 → + x ─┐
  │                                    │
  ├→ LayerNorm → 交叉注意力(Q=x, K=enc_out, V=enc_out) → + ─┐
  │                                                          │
  └→ LayerNorm → FNN → + ────────────────────────────────────┘ → 输出
```

三个子层的关键区别：

| 子层 | Q 来源 | K 来源 | V 来源 | 掩码 | 作用 |
|:----|:------|:------|:------|:----|:-----|
| **掩码自注意力** | Decoder 输入 | 同 Q | 同 Q | 是 | 利用已生成的历史 token |
| **交叉注意力** | Decoder 输出 | Encoder 输出 | Encoder 输出 | 否 | 融合编码器信息 |
| **FNN** | — | — | — | — | 非线性变换 |

> **通俗理解**：Decoder 的工作分三步——第一步"回忆"（掩码自注意力，看看自己已经说了什么），第二步"查阅"（交叉注意力，回头看看原文说了什么），第三步"消化"（FNN，综合信息决定下一个词）。

**习题 2**：在 Transformer 推理阶段，输入 "I love" 到 Decoder 生成下一个 token。已知 Decoder 已经生成了 "I"，现在要生成第二个 token。请说明掩码自注意力层和交叉注意力层各自做了什么。

**解**：
- **掩码自注意力层**：输入序列为 `["I"]`（仅一个 token），自注意力的 Q=K=V 都来自这个单 token 序列。由于只有一步历史，掩码无需遮蔽任何内容。该层让模型基于已生成的 "I" 理解当前的语言状态。
- **交叉注意力层**：Q 来自掩码自注意力的输出（即当前 Decoder 对 "I" 的表示），K 和 V 来自 Encoder 对源语言句子的编码。该层计算当前生成状态与源语言每个 token 的注意力权重，决定翻译/生成时应该关注源句的哪些部分。

---

## 3. 从零搭建 Transformer

### 3.1. Embedding 层

将 tokenizer 输出的整数索引映射为稠密向量：

```
输入：(batch_size, seq_len) 的整数张量
       │
       └→ nn.Embedding(vocab_size, dim)
       │
输出：(batch_size, seq_len, dim) 的浮点张量
```

本质是一个可训练的查找表：词表中每个 token 对应一行 dim 维的向量。

### 3.2. 位置编码（Positional Encoding）

注意力机制对位置不敏感——"我喜欢你"和"你喜欢我"在注意力计算中没有区别。位置编码将 token 在序列中的位置信息注入模型。

Transformer 使用**正弦位置编码**：

$$\boxed{PE_{(pos, 2i)} = \sin\left(\frac{pos}{10000^{2i/d_{\text{model}}}}\right)}$$

$$\boxed{PE_{(pos, 2i+1)} = \cos\left(\frac{pos}{10000^{2i/d_{\text{model}}}}\right)}$$

| 特性 | 说明 |
|:----|:-----|
| **外推能力** | 训练中没见过的更长序列也能计算位置编码（基于函数而非查表） |
| **相对位置可计算** | $\text{PE}(pos+k)$ 可由 $\text{PE}(pos)$ 线性表示，因为 $\sin(A+B) = \sin A\cos B + \cos A\sin B$ |
| **频率多样性** | 不同维度使用不同频率（$10000^{-2i/d}$），低维捕获短距离位置关系，高维捕获长距离位置关系 |

（可以补充位置编码热力图）

位置编码与词向量**相加**后进入 Transformer：

$$\text{input} = \text{Embedding}(x) + \text{PE}(x)$$

**注意**：位置编码的数学推导证明了它的合理性——通过将位置向量视为复数 $e^{im\theta}$，内积 $\langle p_m, p_n\rangle$ 只依赖于相对位置 $m-n$，这正是位置编码需要满足的核心性质。

### 3.3. 完整 Transformer 结构

将所有组件串联：

```
原始文本
  │
  └→ Tokenizer → 整数索引序列
                    │
                    ├→ Embedding 层 → 词向量
                    ├→ 位置编码 → + → 含位置信息的表示
                    │
                    ├→ Encoder × N（默认 N=6）
                    │     └→ 每层：Self-Attn → FNN（均含残差 + LayerNorm）
                    │
                    └→ Decoder × N（默认 N=6）
                          └→ 每层：Masked-Self-Attn → Cross-Attn → FNN
                    │
                    └→ Linear（dim → vocab_size）
                    │
                    └→ Softmax → 概率分布 → 取最大概率 token
```

| 超参数 | 原论文值 | 说明 |
|:------|:--------|:-----|
| $N$（层数） | 6 | Encoder 和 Decoder 各 6 层 |
| $d_{\text{model}}$ | 512 | 隐藏维度 |
| $h$（头数） | 8 | 多头注意力头数 |
| $d_k = d_v$ | 64 | 每个头的维度 |
| $d_{\text{ff}}$ | 2048 | FNN 中间层维度（4× 膨胀） |

**Pre-Norm vs Post-Norm**：原论文图示是 Post-Norm，但实际代码和现代 LLM 普遍采用 Pre-Norm。Pre-Norm 的梯度流更稳定，是当前大模型训练的默认选择。

---

## 本讲核心要点

1. **Attention 公式** $\text{softmax}\left(\frac{QK^T}{\sqrt{d_k}}\right)V$ 是整个 Transformer 的数学核心——Q 与 K 计算相关性，Softmax 归一化为权重，加权 V 得到输出
2. Transformer 取代 RNN 的根本优势在于**并行计算**和**直接长距离依赖建模**——每个 token 都能在一次前向传播中关注任意位置
3. **自注意力**中 Q=K=V 来自同一输入，建模序列内部依赖；**交叉注意力**中 Q 来自 Decoder，K、V 来自 Encoder，融合编码信息
4. **掩码自注意力**通过上三角 $-\infty$ 掩码遮蔽未来 token，实现并行化的语言模型训练——这是 Transformer 高效性的关键设计
5. **多头注意力**用多组 $W_q$、$W_k$、$W_v$ 并行计算，捕获语法、语义、指代等不同层次的语言关系
6. **LayerNorm** 在每个样本的特征维度上归一化，比 Batch Norm 更适合 NLP 的变长序列和小 batch 场景
7. **残差连接**（$x = x + \text{SubLayer}(x)$）让信息有"高速通道"，解决深层网络退化问题
8. **位置编码**用正弦函数将序列位置信息注入模型，具备外推能力和相对位置可计算性
9. Encoder 的核心节奏是"注意力（交流）+ FNN（思考）"交替；Decoder 在此之上增加了掩码和交叉注意力
10. 一个完整的 Transformer = Tokenizer + Embedding + Positional Encoding + N×Encoder + N×Decoder + Linear + Softmax