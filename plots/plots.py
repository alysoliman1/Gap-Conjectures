import json
import matplotlib.pyplot as plt

with open('lowerbounds.json', 'r') as lowerboundsjson:
    lowerbounds = lowerboundsjson.read()
lowerbounds = json.loads(lowerbounds)

N = len(lowerbounds)

sim = [(n ** 2) / 2  for n in range(1, N + 1)]

plt.plot(lowerbounds)
plt.plot(sim)
plt.show()