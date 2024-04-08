import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

import sys
if __name__ == "__main__":
    param = sys.argv[1]
    name_hash = sys.argv[2]

#Plot

path1 = "./Processing/data.txt"
file1 = "./Processing/pictures/output" + param + ".png"
xs = []
ys = []
counter = 0
n = 0
with open(path1, "r") as f:
    for line in f.readlines():
        if (counter):
            x, y = map(int, line.strip().split())
            xs.append(x)
            ys.append(y)
        else: 
            n = int(line)
        counter += 1
xs = np.array(xs)
ys = np.array(ys)
ht_data = pd.DataFrame(data= {'x':xs, 'y': ys})
ht_data.head()

_, ax = plt.subplots(figsize=(18, 10))
ax.set_xlabel("Hash", fontweight="bold")
ax.set_ylabel("Amount of collusions", fontweight="bold")
ax.set_title(name_hash, fontweight="bold")

#sns.set(context="notebook",font="Sans", font_scale=1.3, style = "whitegrid", color_codes=True, palette="husl")
#sns_plot = sns.barplot(data = ht_data, x='x', y='y', ax = ax)
sns_plot = plt.bar(ht_data['x'], ht_data['y'], 3)
ax.xaxis.set_major_locator(ticker.MaxNLocator(90))

ax.set_xticklabels(ax.get_xticklabels(), rotation=90)
#plt.grid()  #just add this
#fig = sns_plot.get_figure()
#fig.savefig(file1)
plt.savefig(file1)

#Chi-squared test
print()
print()
print("Max Collusion:", max(ys))
m = len(xs)
#n defined
summ = 0
for amount in ys:
    summ += amount*(amount+1)/2
x2 = m*summ/n - n
w = (x2 + n + m)/(n+2*m-1)
print("Chi-squared test: chi = ", x2, ", w = ", w)
print()
print()