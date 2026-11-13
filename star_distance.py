import numpy as np
from itertools import combinations

# Star gulor simplified 2D position
stars = {"Alkaid": np.array([0, 0]), "Mizar": np.array([1.2, 0.4]),
         "Dubhe": np.array([4.9, 0.9]), "Merak": np.array([4.8, -0.4])}

# NumPy diye proti jora star er moddhe distance ber kora
print(f"{'Star 1':<10}{'Star 2':<10}Distance")
for a, b in combinations(stars, 2):
    dist = np.linalg.norm(stars[a] - stars[b])
    print(f"{a:<10}{b:<10}{dist:.2f}")

# Shobcheye kachhe ar shobcheye dure er jora
pairs = {(a, b): np.linalg.norm(stars[a] - stars[b]) for a, b in combinations(stars, 2)}
print("Closest :", min(pairs, key=pairs.get))
print("Farthest:", max(pairs, key=pairs.get))
