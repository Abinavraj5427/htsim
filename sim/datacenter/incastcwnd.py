import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('mtpsrc1.csv')
df2 = pd.read_csv('mtpsrc2.csv')
print(df.shape)
print(df.columns)
plt.plot(df['ts'], df['cwnd'], color = 'orange', label = 'cwnd1')
plt.plot(df['ts'], df2['cwnd'], color='purple', label='cwnd2')
plt.xlabel('Time (ms)')
plt.ylabel('CWND (messages)')
plt.title('Sender CWND for MTP Austin 2 Sender Incast')
plt.legend()
plt.show()

