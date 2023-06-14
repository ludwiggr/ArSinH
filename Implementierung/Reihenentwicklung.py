import numpy as np
import math
import matplotlib.pyplot as plt

print(math.asinh(0.5))
# n = 1024
# b = [1.0]
# a = 0.0
# for i in range(1, n):
#     b.append(math.pow(2,i))
    
# for i in b:
#     try:
#         a = math.asinh(i)
#     except Exception as e:
#         print("Overflow bei i = ", i)
#         break




#genauigkeit n (anzahl an stützstellen)

# y_arr = np.array([math.asinh(x) for x in x_arr])

# f_matrix = np.zeros((len(x_arr), len(x_arr)))
# for i, j in enumerate(range(0, len(x_arr))):
#     try:
#         f_matrix[i][j] = math.pow(x_arr[i], j)
#     except:
#         print("Overflow bei i = ", i, " und j = ", j)
#         break

# a = np.linalg.solve(f_matrix, y_arr)
# print(a)

x_arr = np.linspace(0, 10000, 100)
y_arr = np.array([math.asinh(x) for x in x_arr])

a = range(0, 100)
b = range(0, 100)
c = 1
coeffs1 = [1.0]
for i in range(0,7):
    c = c * (a[2*i+1]/a[2*i+2]) * (-1.0)
    coeffs1.append(c/a[2*i+3])


y_approx = np.array([sum([coeffs1[i] * math.pow(x, i) for i in range(0, len(coeffs1))]) for x in x_arr])
plt.plot(x_arr, y_arr, label="asinh(x)", )
plt.plot(x_arr, y_approx, label="Approximation")
plt.legend()
plt.grid()
plt.show()
print(coeffs1)