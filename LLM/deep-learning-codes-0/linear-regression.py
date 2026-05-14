# 定义数据集

# 定义数据特征
x_data = [1, 2, 3]
# 定义数据标签
y_data = [2, 4, 6]

# 初始化参数槽
w = 4

# 定义线性回归模型
def forward(x):
    return w * x

# 定义损失函数
def cost(xs, ys):
    cost_value = 0
    # zip(xs, ys) 的作用是将两个列表中的对应元素打包成元组
    for x, y in zip(xs, ys):
        f_x = forward(x)    # 预测f(x)
        cost_value += (f_x - y) ** 2    # 均方根
        return cost_value / len(xs) # 求平均

# 定义梯度函数
def gradient(xs, ys):
    grad = 0
    for x, y in zip(xs, ys):
        grad += 2 * x * (x * w - y)