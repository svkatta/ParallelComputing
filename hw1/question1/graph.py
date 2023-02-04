import matplotlib.pyplot as plt
import pandas
import numpy as np
df = pandas.read_csv('output.txt', index_col=False, header=None)

xpoints = np.arange(1000,10000001,1000)
data = df.iloc[:,0].to_numpy()

from sklearn.linear_model import LinearRegression
model = LinearRegression().fit(xpoints.reshape(-1,1), data.reshape(-1,1))
y_pred = model.predict(xpoints.reshape(-1,1))


# The coefficients
print("Coefficients: \n", model.coef_)
plt.figure()
plt.plot(xpoints,data)
plt.plot(xpoints,y_pred)


plt.title('')
plt.xlabel('array length', fontsize=14)
plt.ylabel('Time in microsecs', fontsize=14)

plt.show()