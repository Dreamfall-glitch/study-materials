# 08-BGA封装技术

## 知识地图

从QFP的四边引脚到BGA的底部面阵列——这是封装I/O密度的一次飞跃。

| 封装类型 | 引脚形式 | I/O密度 | 典型节距 |
|---------|---------|---------|---------|
| DIP | 双列直插 | 低 | 2.54mm |
| QFP | 四边鸥翼引脚 | 中 | 0.3~1.0mm |
| PGA | 底部插针阵列 | 较高 | 1.27~2.54mm |
| **BGA** | **底部焊球阵列** | **高** | **0.4~1.27mm** |

**本讲核心**：BGA（Ball Grid Array）用底部焊球面阵列替代周边引脚，在封装底部实现高密度、高性能互连。本讲涵盖PBGA、CBGA、CCGA、TBGA、FCBGA五大类型。

---

## 1. BGA的分类

按基板材料划分为六大类型：

```
BGA分类（按基板）：
├── PBGA (Plastic BGA)      — BT树脂/玻璃纤维基板
├── CBGA (Ceramic BGA)      — 多层陶瓷基板
├── CCGA (Ceramic Column Grid Array) — 陶瓷基板+焊料柱
├── TBGA (TAB BGA)          — PI载带基板
├── FCBGA (Flip Chip BGA)   — 倒装芯片+有机/陶瓷基板
└── MBGA / EBGA             — 金属基板/带散热器BGA
```

---

## 2. PBGA（塑料BGA）

PBGA是应用最广泛的BGA类型，基板材料为**BT树脂/玻璃纤维层压板**。

### 2.1. PBGA基板制备（四步工艺）

**① 层压覆铜板**
在BT树脂/玻璃芯板两面层压极薄铜箔（12~18μm）。

**② 钻通孔和通孔金属化**
通孔一般位于基板四周，钻完后进行镀通孔（PTH）金属化。

**③ 线路制造（PWB工艺）**
```
前处理 → 压掩膜 → 上底片 → 曝光(UV)
    → 显影 → 蚀刻 → 剥膜 → 水洗
```
在基板两面制作导带、电极和安装焊球的焊区阵列。

**④ Pad制造**
覆盖介质阻焊膜（绿漆），露出电极和焊区。
Pad镀层功能：
1. **绿漆** → 保护铜层，防止氧化
2. **Ni层** → 作为金和铜的介质，防止金-铜互扩散
3. **Au层** → 利于打金线（Gold wire bonding）
工艺方式：**化学镀**（无需外加电源，选择性沉积）

### 2.2. 芯片-基板内互连

| 工序 | 工艺 | 材料 |
|------|------|------|
| **芯片粘结** | 导电胶粘结 | 含银环氧树脂 |
| **引线键合** | 金丝球焊 | 芯片Al pad → 基板Ni-Au焊区 |
| **模塑封装** | Transfer molding | 填有石英粉的环氧树脂模塑料 |

### 2.3. 外互连——植球方法

BGA的外互连是将焊球安装到基板焊区上的工艺，共有四种方法：

**方法一：球在上（Ball Up）**
```
基板上丝网印焊膏 → 带筛孔顶板对准 → 放球（焊球通过筛孔落入焊膏）
    → 取下顶板 → 再流焊 → 清洗
```
焊球规格：φ0.76mm(30mil) 或 φ0.89mm(35mil)，成分为低熔点305SnAgCu焊料。

**方法二：球在下（Ball Down）**
```
焊球舟振动定位（孔中） → 基板焊盘印焊膏 → 基板对准放置在焊球舟上
    → 再流焊 → 清洗
```

**方法三：真空吸球（BGA植球自动化流程）**
```
真空吸球 → 滴助焊剂 → 放球 → N₂气氛中回流
```
设备配置：助焊剂滴涂和置球机 → 氮气再流焊炉 → 助焊剂清洗/分离/打标机

**方法四：丝网印刷焊膏**
分两种类型：接触式印刷（On-Contact Printing）和非接触式印刷（Off-Contact Printing）。

### 2.4. PBGA的优缺点

| 优点                  | 缺点                     |
| ------------------- | ---------------------- |
| 与环氧树脂电路板（FR-4）热匹配性好 | **对湿气敏感**（吸潮导致"爆米花"效应） |
| 焊球共面要求宽松（再流焊自对准补偿）  | 封装密度有限（难以实现细间距）        |
| 成本较低                | 散热性能较差（依赖PCB散热）        |
| 电性能良好（互连短）          | 焊点可靠性受PCB翘曲影响          |
| 与PWB焊接时可自对准         | 不适合高频/高速应用（寄生电感较大）     |
| 可用于MCM（多芯片模块）封装     | 返修困难，需要专业设备            |

---

## 3. CBGA（陶瓷BGA）

### 3.1. CBGA的特点

| 项目 | 内容 |
|------|------|
| 基板技术 | **多层陶瓷布线基板**（AlN等），采用HTCC或LTCC工艺 |
| 内互连 | 高熔点共晶焊料（FCB倒装芯片，共晶键合） |
| 外互连 | 低温共晶焊料；**封盖+陶瓷金属化密封**，属于**气密封装**范畴 |

### 3.2. CBGA的优缺点

| 优点 | 缺点 |
|------|------|
| 封装组件可靠性高 | 热匹配性差（陶瓷CTE与PCB差异大） |
| 共面性好，焊点成形容易 | 对准困难 |
| **对湿气不敏感**（气密封装） | 封装成本高 |
| 封装密度高 | |

### 3.3. 陶瓷基板的应用

5G阵列天线（毫米波）→ 低损耗、高介电常数稳定性
LED封装 → 高导热AlN/Al₂O₃陶瓷
IGBT功率模块 → 高可靠性封装

---

## 4. CCGA（陶瓷焊柱阵列）

**定义**：CBGA的扩展技术，用**焊料柱**代替焊球作为互连基材。

**适用条件**：当器件面积大于32mm²时，CBGA面临热应力问题，CCGA是替代方案。

**焊料柱 vs 焊料球**：
- 降低封装部件和焊料在接触点之间的压力
- 清洗容易
- 热性能好

---

## 5. TBGA（载带BGA）

**定义**：TAB BGA，阵列载带自动键合。

| 项目 | 内容 |
|------|------|
| 基板 | PI（聚酰亚胺）多层布线基板 |
| 焊球 | 高熔点焊料合金 |
| 与PCB互连 | 低熔点焊料合金 |
| 内互连 | 采用**TAB**内引线键合技术 |

**优点**：
1. 与环氧树脂PCB热匹配性较好
2. **最薄型的BGA封装**
3. **最经济的BGA封装**

**缺点**：
1. 对湿气敏感
2. 对热敏感

---

## 6. FCBGA（倒装芯片BGA）

### 6.1. 为什么用Flip Chip？

**面阵列I/O**：可利用芯片整个底部面积排布I/O，芯片可缩小，每片晶圆产出更多芯片

**电性能优势**：
- **电感 < WB的10%**
- 电容、电阻、传播延迟显著降低

**可靠性**：据Delco和IBM报告，FC焊点从未发生现场失效

**成本**：大批量组装时成本更低

### 6.2. FCBGA的Bumping工艺

**Print Bumping Process（印刷凸点成型）**：

```
Step 1: Wafer Clean（晶圆清洗）
    → 去除氧化物/光刻胶残留；粗化钝化层和焊盘表面以增强UBM附着力

Step 2: UBM Sputter（UBM溅射）
    → 三层金属：Al（粘附层）+ Ni（扩散阻挡层）+ Cu（润湿层）

Step 3: Apply Photoresist → Pattern → Develop（涂光刻胶→曝光→显影）

Step 4: Etch UBM（刻蚀UBM）
    → 仅保留焊盘上的UBM，其余部分刻蚀去除

Step 5: Solder Disposition（焊料置放）
    → 三种方式：Evaporative(蒸镀/IBM) / Stencil Printing(网板印刷/FCT) / Electroplating(电镀/Aptos)

Step 6: Reflow Solder into a sphere（回流成球）
```

**三种Bumping流程对比**：

| 步骤 | 蒸发法(Evaporation) | 电镀法(Electroplated) | 印刷法(Printed) |
|------|-------------------|---------------------|----------------|
| 1 | Wafer Clean | Wafer Clean | Wafer Clean |
| 2 | Metal Mask | UBM Deposition | UBM Deposition |
| 3 | — | Apply PR, pattern & develop | Apply PR, pattern & develop |
| 4 | — | Electroplate mini-bump / UBM | Etch UBM |
| 5 | UBM Evaporation | Apply 2nd PR, pattern & develop | Solder Printing |
| 6 | Solder Evaporation | Electroplate solder | — |
| 7 | Strip PR & etch UBM | Strip PR & etch UBM | — |
| 8 | Solder Reflow | Solder Reflow | Solder Reflow |

**三种方法UBM体系对比**：

| UBM层 | 蒸发法(C4) | 电镀法 | 印刷法(FCT) |
|-------|-----------|--------|------------|
| 粘附层 | Cr / Cr-Cu | TiW / Al | Al |
| 扩散阻挡层 | Cr-Cu复合相 | Cu stud/mini-bump / Ni | Ni |
| 润湿层 | Cu | Cu | Cu |
| 防氧化层 | Au | Au | Cu |
| 63Sn/Pb匹配性 | 差(1)/差(2) | 无/否 | 是 |
| 探针晶圆适用 | 否 | 否 | **是** |

### 6.3. 焊球规格

| 类型 | 成分 | 熔点 |
|------|------|------|
| 有铅 | Sn63/Pb37 | **183°C** |
| 无铅 | Sn95.5/Ag4.0/Cu0.5 (SAC405) | **217°C** |

| 应用 | 焊球直径 |
|------|----------|
| PD1 flash | φ0.30 / 0.35 / 0.40 mm |
| Chipset / PBGA | φ0.60 / 0.76 mm |

### 6.4. FCBGA组装流程

```
工序              设备示例               材料示例
Die Saw           Disco DFD640/641
Die Pick & Place  ESEC Micron-2         Flux: Alphametal
Reflow            Vitronics 730N
Flux Clean
Underfill         Aysmtek Millennium    Underfill: Dexto, Hysol
Curing            C-Sun
H/S Attach        (散热片安装)
Marking           E&R/RS CSP106D
Solder Ball Place Motorola              Solder Ball: Senju
Final Vision      RVSI GS-5700DB
```

### 6.5. Flux（助焊剂）

**Flux的类型**：

| 类型 | 特点 |
|------|------|
| **Rosin Based（松香型）** | 残留需用溶剂去除 |
| **No Clean（免洗型）** | 无需清洗 |
| **Water Soluble（水溶性）** | 残留可用水去除 |

**Flux涂布方法**：Screen Print / Stencil Print（丝网印刷）、Pin-transfer（针转移）、Dip Fluxing（浸蘸）

**目的**：去除焊料和焊盘表面的氧化物，促进润湿

**Deflux（去flux）**：使用Zestron、Kyzen、IPA、Water等材料去除flux残留，以利于后续underfill工序。

### 6.6. Reflow（回流焊接）

**加热方式**：Infra-Red（红外热辐射） / Forced-Air Convection（热风对流——SPIL采用）

**回流曲线**：Preheat（预热）→ Soak（保温）→ Strip（快速升温）→ Cool Down（冷却）

**润湿 vs 脱湿**：良好的润湿（Wetting）形成均匀IMC；脱湿（Dewetting）表明表面污染或氧化。

### 6.7. Underfill（底部填充）

**目的**：
- 保护器件底部免受环境影响
- 降低焊点应力

**流程**：Dispense（点胶）→ Capillary Flow（毛细流动）→ Cure（固化）

---

## 7. 质量检测与可靠性

### 7.1. 入料检验（IQC）

检验项目：Bump Height（凸点高度）、Bump Diameter（直径）、Bump Shear Strength（剪切力）

**剪切力失效模式**：

| Mode | 描述 |
|------|------|
| 1 | Bump残留>25%（韧性断裂） |
| 2 | Bump从UBM剥离 |
| 3 | UBM剥离 |
| 4 | Al Pad剥离 |
| 5 | Bump剥离+硅裸露（最严重） |

### 7.2. 目检

缺陷类型：**Ball Bridge（桥接）**、**Ball Size（球径偏差）**、**Miss Ball（缺球）**

### 7.3. Underfill Void

UF空洞会导致回流后焊球桥接。检测手段：**SAT（Scanning Acoustic Tomography，超声扫描）**

### 7.4. Ball Shear Test

| 焊球直径 | 剪切力标准 |
|----------|-----------|
| 0.4mm / 0.35mm | ≥ 400g |
| 0.3mm | ≥ 200g |

### 7.5. 可靠性测试

| 测试项 | 条件 |
|--------|------|
| **PCT（压力锅测试）** | 121°C, 2atm, 100%RH |
| **MRT（吸湿敏感度）** | 30°C, 60%RH, 192hrs |
| **Thermal Cycling（热循环）** | -55°C / +125°C, 1000次 |
| **HTS（高温储存）** | 150°C, 1000hrs |
| **TH（温湿偏置）** | 85°C / 85%RH, 1000hrs |

### 7.6. 失效分析方法

| 方法 | 可检测的失效模式 |
|------|-----------------|
| **C-SAM（超声扫描）** | 分层、空洞、裂纹 |
| **X-Ray** | 焊桥 |
| **Decapsulation（开盖）** | 腐蚀、潮气侵蚀 |
| **Cross-section（金相截面）** | 分层、空洞、金属间化合物（IMC）、裂纹 |

---

## 8. Build-Up Substrate（增层基板）

**Build-Up（增层法）**：可有效减少通孔占用的面积，轻易达到细线、小孔的高密度要求。

典型结构（如1+2+1 Build-Up Substrate）：在芯板上下两侧逐层增建布线层，通过微盲孔（Microvia）实现层间互连。

---

## 9. BGA封装的特点总结

| 特点 | 说明 |
|------|------|
| (1) **Solder Ball互连** | 降低失效率 |
| (2) **SMT适应性强** | 焊点节距大，对设备要求低 |
| (3) **封装密度高** | 器件引出数和本体尺寸比率大（节距<0.8mm，边长31mm可容纳大量IO） |
| (4) **改善共面问题** | 焊球熔化时自对准，减少共面损坏 |
| (5) **引脚牢固** | BGA焊球比QFP引脚结实 |
| (6) **电性能好** | 互连短，寄生电容/电感小 |
| (7) **利于散热** | 底部焊球导热路径短 |
| (8) **利于MCM** | 有利于实现高密度、高性能多芯片模块 |

### 焊球分布类型

**周边型**（Perimeter）：焊球只在封装边缘排布
**交错型**（Staggered）：焊球交错排列，可缩小节距
**全阵列型**（Full Array）：焊球布满整个底部，密度最高

---

## 本讲核心要点

1. **BGA用底部焊球面阵列替代QFP的周边引脚**，大幅提升了I/O密度
2. **PBGA成本最低、应用最广**，但吸潮是核心弱点
3. **CBGA/CCGA**用于高可靠性场景（气密封装），CTE失配是主要挑战
4. **TBGA最薄最经济**，采用TAB内引线键合
5. **FCBGA代表最高端**——结合倒装芯片，电感仅为WB的10%
6. **焊球小型化趋势**：BGA球(φ300μm)→Bump(φ50μm)→Pillar(φ10μm)→Hybrid Bonding(φ1μm)
7. **BGA可靠性检测**包括球剪切力测试、PCT、温循、C-SAM和X-Ray分析
