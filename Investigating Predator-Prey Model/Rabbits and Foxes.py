import numpy as np
import matplotlib.pyplot as plt

k_r = 2
u_r = 1
k_f = 5
u_f = 1
n = 100
tn = 10
c = 10

def dR(t, r, f):
    return (k_r * r * (1 - r/c)) - (u_r * r * f)


def dF(t, r, f):
    return (-k_f * f) + (u_f * r * f)

t0 = 0
r0 = 10
f0 = 1


h = (tn - t0)/n


t_values = np.zeros(n+1)
r_values = np.zeros(n+1)
f_values = np.zeros(n+1)

t_values[0] = t0
r_values[0] = r0
f_values[0] = f0

for i in range(n):
    t = t_values[i]
    r = r_values[i]
    f = f_values[i]
    
    r_prime = dR(t, r, f)
    f_prime = dF(t, r, f)
    
    t_values[i+1] = t + h
    r_values[i+1] = r + h * r_prime
    f_values[i+1] = f + h * f_prime

fig, ax = plt.subplots(1, 2, figsize = (12, 6))


ax[0].plot(t_values, r_values, marker = 'o', c = 'b')
ax[0].grid(True)

ax[0].set_title("rabbit population")
ax[0].set_xlabel("time")
ax[0].set_ylabel("rabbit")

ax[1].plot(t_values, f_values, marker = 'o', c = 'r')
ax[1].grid(True)

ax[1].set_title("fox population")
ax[1].set_xlabel("time")
ax[1].set_ylabel("fox")

fig.show()
