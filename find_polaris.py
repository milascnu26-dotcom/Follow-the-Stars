import numpy as np
import matplotlib.pyplot as plt

# Big Dipper er duita pointer star (simplified 2D position)
stars = {"Merak": np.array([3.0, 1.0]), "Dubhe": np.array([3.4, 2.4])}

# Pointer line: Merak theke Dubhe, tarpor 5 gun distance
direction = stars["Dubhe"] - stars["Merak"]
polaris = stars["Dubhe"] + 5 * direction

fig, ax = plt.subplots()
fig.patch.set_facecolor("black")
ax.set_facecolor("black")
for name, pos in {**stars, "Polaris": polaris}.items():
    ax.scatter(*pos, color="gold", s=80)
    ax.text(pos[0] + 0.2, pos[1], name, color="white")
ax.plot(*zip(stars["Merak"], polaris), "--", color="skyblue")
ax.set_title("Find Polaris from the Big Dipper", color="white")
plt.show()
