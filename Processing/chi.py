import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

path2 = "./Processing/chi.txt"
file2 = "./pictures/chi.png"

#Plot
xs = []
ys = []
with open(path2, "r") as f:
    for line in f.readlines():
        x, y = str(line.strip().split()[0]), float(line.strip().split()[1])
        xs.append(x)
        ys.append(y)
xs = np.array(xs)
ys = np.array(ys)
chi_data = pd.DataFrame(data= {'x':xs, 'y': ys})
chi_data.head()
_, ax1 = plt.subplots(figsize=(14, 9))
ax1.set_xlabel("Number of hash function", fontweight="bold")
ax1.set_ylabel("Chi-squared test value", fontweight="bold")
ax1.set_title("Chi-Squared Test", fontweight="bold")
sns.set(context="notebook",font="Sans", font_scale=2, style = "whitegrid", color_codes=True, palette="husl")
sns1_plot = sns.barplot(data = chi_data, x='x', y='y', ax = ax1)
ax1.axhline(0.84, xmin=-2, xmax = len(xs) + 2 , color="red")
ax1.set_xticklabels(ax1.get_xticklabels(), rotation=0)
plt.grid()  #just add this
plt.savefig(file2)