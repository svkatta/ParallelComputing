import matplotlib.pyplot as plt
import pandas
import numpy as np
df = pandas.read_csv('output.txt', index_col=False, header=None)

xpoints = np.arange(1,251,1)
data = df.iloc[:,0].to_numpy()


fig = plt.figure()
plt.plot(xpoints,data)
plt.title('Q2 Cache Size Estimation')
plt.xlabel('skip', fontsize=14)
plt.ylabel('Time in microsecs', fontsize=14)

plt.show()
fig.savefig('temp.png')
