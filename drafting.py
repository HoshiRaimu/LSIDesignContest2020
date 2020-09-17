import matplotlib.pyplot as plt

f = open('result.txt','r')
numbers = f.readlines()

i = 1
x = []
y = []
for number in numbers:
    x.append(i)
    y.append(float(number))
    i+=1

# figureを生成する
fig = plt.figure()

# axをfigureに設定する
ax = fig.add_subplot(1, 1, 1)

# axesにplot
ax.plot(x, y)
# 表示する
plt.show()

f.close()