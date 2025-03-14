import json 
import matplotlib.pyplot as plt

with open("/home/max/Documents/Code/bin/Lorenz_RK4.json") as f:
    data = json.load(f)

# Extrahiere auch die Zeit
times = data["TIME"]
x = data["X0"]
y = data["X1"]
z = data["X2"]
title = data["DESCRIPTION"]

# 3D-Plot mit Zeit als Farbwert
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

# Plotte mit Farbverlauf basierend auf der Zeit
scatter = ax.plot(x, y, z)

# Achsenbeschriftungen und Titel
ax.set_xlabel('X-Achse')
ax.set_ylabel('Y-Achse')
ax.set_zlabel('Z-Achse')
ax.set_title(title)

# Zeige den Plot
plt.show()
