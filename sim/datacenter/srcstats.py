import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('mtpogsrc.csv')
print(df.shape)
print(df.columns)
plt.plot(df['ts'], df['cwnd'], color = 'green', label = 'cwnd')
plt.plot(df['ts'], df['if'], color='blue', label='in flight')
plt.xlabel('Time (ms)')
plt.ylabel('CWND (packets)')
plt.title('Sender CWND and In flight messages MTP Swift')
plt.legend()
plt.show()

plt.plot(df['ts'], df['txq'], color = 'orange')
plt.show()


plt.plot(df['ts'], df['rtx'], color = 'blue')
plt.show()
