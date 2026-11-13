import numpy as np
import matplotlib.pyplot as plt

# Big Dipper er 7 ta star (simplified 2D position)
stars = {"Alkaid": [0, 0], "Mizar": [1.2, 0.4], "Alioth": [2.2, 0.7],
         "Megrez": [3.2, 0.8], "Phecda": [3.4, -0.4],
         "Merak": [4.8, -0.4], "Dubhe": [4.9, 0.9]}
order = ["Alkaid", "Mizar", "Alioth", "Megrez", "Phecda", "Merak", "Dubhe", "Megrez"]
points = np.array([stars[n] for n in order])

fig, ax = plt.subplots()
fig.patch.set_facecolor("black")
ax.set_facecolor("black")
ax.plot(points[:, 0], points[:, 1], "-", color="skyblue")
for name, (x, y) in stars.items():
    ax.scatter(x, y, color="gold", s=80)
    ax.text(x, y + 0.12, name, color="white", ha="center")
ax.set_title("Big Dipper Map", color="white")
plt.show()
