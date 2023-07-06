import json
import matplotlib.pyplot as plt

with open('lowerbounds.json', 'r') as lowerboundsjson:
    lowerbounds = lowerboundsjson.read()
lowerbounds = json.loads(lowerbounds)

with open('upperbounds.json', 'r') as upperboundsjson:
    upperbounds = upperboundsjson.read()
upperbounds = json.loads(upperbounds)

plt.plot(lowerbounds)
plt.plot(upperbounds)
plt.show()