import math

# coefficients for ln approx series
c_ln = []
j = -1

for i in range(1, 52):
    r = (-j) / i
    j *= -1
    c_ln.append(r)

# coefficients for rest series

c_rest = []
j = -1

for i in range(1, 52):
    r = (-j) * (math.factorial(2 * i) / (2 * i * math.pow(math.pow(2, i) * math.factorial(i), 2)))
    j *= -1
    c_rest.append(r)

# coefficients for series below 1

c_below = []
j = -1

for i in range(0, 52):
    r = (-j) * (math.factorial(2 * i) / ((2 * i + 1) * math.pow(math.pow(2, i) * math.factorial(i), 2)))
    j *= -1
    c_below.append(r)

f = open("coeffs_ln.txt", "a")
for i in range(0, len(c_ln)):
    f.write(str(c_ln[i]) + ",")
f.close()

f = open("coeffs_rest.txt", "a")
for i in range(0, len(c_rest)):
    f.write(str(c_rest[i]) + ",")
f.close()

f = open("coeffs_below.txt", "a")
for i in range(0, len(c_below)):
    f.write(str(c_below[i]) + ", ")
f.close()
print("a")
