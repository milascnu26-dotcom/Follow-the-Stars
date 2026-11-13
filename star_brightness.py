import numpy as np
import matplotlib.pyplot as plt

# Apparent magnitude: number joto chhoto, tara toto ujjol
magnitudes = {"Alioth": 1.77, "Dubhe": 1.79, "Alkaid": 1.85, "Polaris": 1.98,
              "Mizar": 2.23, "Merak": 2.34, "Phecda": 2.44, "Megrez": 3.31}

names = list(magnitudes)
values = np.array(list(magnitudes.values()))
brightness = 10 ** (-0.4 * values)  # magnitude ke brightness e convert kora
brightness = brightness / brightness.max() * 100

fig, ax = plt.subplots(figsize=(8, 4))
fig.patch.set_facecolor("black")
ax.set_facecolor("black")
ax.bar(names, brightness, color="gold")
ax.set_ylabel("Relative brightness (%)", color="white")
ax.set_title("Star Brightness Comparison", color="white")
ax.tick_params(colors="white")
plt.show()
