#!/usr/bin/env python
# coding: utf-8

# In[17]:


import matplotlib.pyplot as plt
import math
import numpy as np
from matplotlib import rcParams
rcParams.update({'figure.autolayout': True})


# In[18]:


x = [20,50,100,150,200,250,300,350,400]
ysparse = [198, 1016, 3381, 6646, 9954, 16049, 22054, 30942, 37643]
def func (x): 
    return ((x * (x-1) * 0.2) + (5 * x * math.log(x)))
#yexp = x * (x-1) * 0.2 + 5 * x * ln(x)
plt.xlabel("Nodes")
plt.ylabel("Messages")
q = np.linspace(1, 400, 10000) 
yq = [func(xz) for xz in q]
fig, ax = plt.subplots()
#fig.patch.set_alpha(1)  # solution
plt.plot(q, yq)
plt.xlabel("Nodes")
plt.ylabel("Messages")
plt.plot(x,ysparse)
plt.legend(["2.E + 5.N.log(N)", "GHS(p = 0.2)"], loc ="lower right")
plt.title("Messages Experiment for sparse graphs")
plt.savefig("sparse.png",  transparent=False)
plt.show()
plt.close()



def funct (x): 
    return ((x * (x-1) * 0.8) + (5 * x * math.log(x)))

ydense = [505, 2436, 9049, 19525, 34471, 53218, 76051, 102848, 133375]
plt.xlabel("Nodes")
plt.ylabel("Messages")
q = np.linspace(1, 400, 10000) 
yq = [funct(xz) for xz in q]
fig, ax = plt.subplots()
#fig.patch.set_alpha(1)  # solution
plt.plot(q, yq)
plt.xlabel("Nodes")
plt.ylabel("Messages")
plt.plot(x,ydense)
plt.legend(["2.E + 5.N.log(N)", "GHS(p = 0.8)"], loc ="lower right")
plt.title("Messages Experiment for dense graphs")
plt.savefig("dense.png",  transparent=False)
plt.show()
plt.close()

def funct (x): 
    return (0.05 * x * math.log(x))

xx = [5,10,20,50,100,150,200,250,300,350,400]
ytime = [0.000503, 0.242961, 0.770877, 3.93151, 11.9101, 23.478, 38.7509, 54.9079, 78.804, 95.1269, 128.236]

plt.xlabel("Nodes")
plt.ylabel("Time")
q = np.linspace(1, 400, 10000) 
yq = [funct(xz) for xz in q]
fig, ax = plt.subplots()
#fig.patch.set_alpha(1)  # solution
plt.plot(xx,ytime)
plt.legend(["GHS time(p = 1)"], loc ="lower right")
plt.title("Time experiment for Complete Graphs")
plt.savefig("time.png",  transparent=False)
plt.show()
plt.close()


# In[ ]:





# In[ ]:




