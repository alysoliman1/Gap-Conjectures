import json
import matplotlib.pyplot as plt

with open('lowerbounds.json', 'r') as lowerboundsjson:
    lowerbounds = lowerboundsjson.read()
lowerbounds = json.loads(lowerbounds)

plt.plot(lowerbounds)
plt.show()