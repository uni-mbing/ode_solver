import json 
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(description='Plot 3D simulation results')
parser.add_argument("file", type=str, help="Filename")
args = parser.parse_args()

simulation_dir = "/home/max/Documents/Code/bin/"

with open(simulation_dir+args.file+".json") as f:
    data = json.load(f)

title = data["DESCRIPTION"]

if data["DIM"] == 3:
    times = data["TIME"]
    x = data["X0"]
    y = data["X1"]
    z = data["X2"]

    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')

    scatter = ax.plot(x, y, z)

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title(title)

elif data["DIM"] == 2:
    times = data["TIME"]
    x = data["X0"]
    y = data["X1"]
    plt.plot(x, y, color="b")

    plt.xlabel("X")
    plt.ylabel("Y")

    plt.title(title)

    plt.legend()

    # plt.grid(True)

plt.show()
