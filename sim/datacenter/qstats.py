import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('q1.csv')
print(df.shape)
print(df.columns)
plt.plot(df['ts'], df['rx'], color = 'blue', label="rx queue")
plt.plot(df['ts'], df['tx'], color = 'green', label="tx queue")
plt.xlabel('Time (ms)')
plt.ylabel('Packets (9000B)')
plt.legend()
plt.title("INC Queue Size 1 Sender 1 Receiver MTP Austin")
plt.show()