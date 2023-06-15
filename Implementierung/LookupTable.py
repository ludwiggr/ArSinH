import numpy as np
import math
import matplotlib.pyplot as plt

n = 1023
x_arr = np.geomspace(1, math.pow(2, n), n)
y_arr = np.array([math.asinh(x) for x in x_arr])
f = open("ylookupTable.txt", "a")
for i in range(0, len(x_arr)):
    f.write(str(y_arr[i]) + ",")
f.close()
f = open("xlookupTable.txt", "a")
for i in range(0, len(x_arr)):
    f.write(str(x_arr[i]) + ",")
f.close()
print(y_arr)


# exit()

def approx(x):
    if (x < 0):
        return -approx(-x)
    if (x == 0):
        return 0
    app = math.floor(math.log2(x))
    m1 = (y_arr[app + 1] - y_arr[app])
    m2 = (x_arr[app + 1] - x_arr[app])
    m = m1 / m2
    return m * (x - x_arr[app]) + y_arr[app]


x = np.linspace(0, 100000000000, 100)
y = np.array([math.asinh(x) for x in x])
y_approx = np.array([approx(x) for x in x])
y_err = np.abs(y - y_approx) / y
plt.plot(x, y_err, label="Fehler")
plt.legend()
plt.grid()
plt.show()
