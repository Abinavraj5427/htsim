import matplotlib.pyplot as plt

# Sample data for three lines
x = [0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
y1 = [273, 300, 300, 301, 286, 312, 311]
y2 = [263, 273, 300, 290, 315, 331, 365]
y3 = [163, 171, 183, 177, 193, 190, 185]
y4 = [49, 49, 49, 49, 49, 49, 49]

z1 = [223]*7
z2 = [203]*7
z3 = [113]*7
z4 = [42]*7

# Plotting the lines
plt.plot(x, y1, label='50 us MTPA')
plt.plot(x, z1, label='50 us MTPS')

plt.legend()
plt.show()

plt.plot(x, y2, label='500 us MTPA')
plt.plot(x, z2, label='500 us MTPS')

plt.legend()
plt.show()


plt.plot(x, y3, label='5000 us MTPA')
plt.plot(x, z3, label='5000 us MTPS')

plt.legend()
plt.show()


plt.plot(x, y4, label='20000 us MTPA')
plt.plot(x, z4, label='20000 us MTPS')

plt.legend()
plt.show()

# Adding labels and title
plt.xlabel('Queue Threshold Percentage')
plt.ylabel('Throughput Msg/Sec')
plt.title('1 Sender 1 Receiver MTP Austin Threshold Performance')

