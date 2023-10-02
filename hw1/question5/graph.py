import matplotlib.pyplot as plt
import pandas
import numpy as np



fig = plt.figure()
plt.plot([100,300,500,700,1000],[340,281,245,175,186],label="Normal")
plt.plot([100,300,500,700,1000],[113,179,178,178,176],label="Block")
plt.legend()
plt.xlabel('matrix size ', fontsize=14)
plt.ylabel('Computation rate', fontsize=14)
fig.savefig('temp.png')
