import numpy as np
import matplotlib.pyplot as plt

# Cassiopeia er W shape (simplified 2D position)
w = np.array([[0, 1], [1, 0], [2, 1], [3, 0], [4, 1]])
names = ["Caph", "Schedar", "Gamma Cas", "Ruchbah", "Segin"]

# Gamma Cas theke W er majhkhan dhore 4 gun upore Polaris
middle = (w[1] + w[3]) / 2
polaris = w[2] + 4 * (w[2] - middle)

fig, ax = plt.subplots()
fig.patch.set_facecolor("black")
ax.set_facecolor("black")
ax.plot(w[:, 0], w[:, 1], "-o", color="gold")
for n, p in zip(names, w):
    ax.text(p[0], p[1] - 0.3, n, color="white", ha="center")
ax.scatter(*polaris, color="red", s=100)
ax.text(polaris[0] + 0.15, polaris[1], "Polaris", color="white")
ax.plot(*zip(w[2], polaris), "--", color="skyblue")
ax.set_title("Find Polaris from Cassiopeia", color="white")
plt.show()
