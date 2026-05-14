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
        # partial J(w)/ partial w = 2x(wx-y)
        grad += 2 * x * (x * w - y)
        return grad / len(xs)

#模型训练，epoch（周期/代次）是指在训练神经网络时，整个训练数据集被完整地向前传播和反向传播一次。
for epoch in range(100):
    cost_val = cost(x_data, y_data)
    grad_val = gradient(x_data, y_data)
    alpha = 0.01
    w = w - alpha * grad_val
    print('训练轮次：',epoch+1, 'w(权重值)：',w,'损失函数：',cost_val)
