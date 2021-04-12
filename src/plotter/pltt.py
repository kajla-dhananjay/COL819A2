import matplotlib.pyplot as plt
import math
import numpy as np
x = [20,50,100,150,200,250,300,350,400]
ysparse = [198, 1016, 3381, 6646, 9954, 16049, 22054, 30942, 37643]
def func (x): 
    return ((x * (x-1) * 0.2) + (5 * x * math.log(x)))
#yexp = x * (x-1) * 0.2 + 5 * x * ln(x)
plt.xlabel("Nodes")
plt.ylabel("Messages")
q = np.linspace(1, 400, 10000) 
yq = [func(xz) for xz in q]
plt.plot(q, yq)
plt.plot(x,ysparse)
plt.legend(["2E + N.log(N)", "Nodes vs Messages for GHS"], loc ="lower right")
plt.title("Experiment for sparse graphs")
plt.show()
plt.savefig("sparse.png")

