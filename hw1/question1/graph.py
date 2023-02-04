import matplotlib.pyplot as plt
import pandas
import numpy as np
df = pandas.read_csv('output.txt', index_col=False, header=None)

lens = np.arange(1000,10000001,1000) # len
time = df.iloc[:,0].to_numpy()          # time

from sklearn.linear_model import LinearRegression
model = LinearRegression().fit(time.reshape(-1,1), lens.reshape(-1,1))
y_pred = model.predict(time.reshape(-1,1))


# The coefficients
print("Coefficients: \n", model.coef_)
plt.figure()
plt.plot(time,lens)
plt.plot(time,y_pred)


plt.title('Q1 Bandwidth Estimation')
plt.ylabel('No of Words(array length)', fontsize=14)
plt.xlabel('Time in microsecs', fontsize=14)

plt.show()