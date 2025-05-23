import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D

mode = "2D"  # 2D/3D

if mode.upper() == "2D":
    points = 300
else:
    points = 20

# link lengths (mm)
L0 = 43
L1 = 80
L2 = 134

# joint angle ranges (degrees converted to radians)
theta1_range = np.radians(np.linspace(-45, 45, 20))  # joint 1
theta2_range = np.radians(np.linspace(0, 90, points))    # joint 2
theta3_range = np.radians(np.linspace(-90, 0, points))   # joint 3

def compute_workspace():
    X, Y, Z = [], [], []

    for t1 in theta1_range:
        for t2 in theta2_range:
            for t3 in theta3_range:
                x_local = L1 * np.cos(t2) + L2 * np.cos(t2 + t3)
                z = L1 * np.sin(t2) + L2 * np.sin(t2 + t3)
                x = (L0 + x_local) * np.cos(t1)
                y = (L0 + x_local) * np.sin(t1)

                X.append(x)
                Y.append(y)
                Z.append(z)

    return np.array(X), np.array(Y), np.array(Z)

def plot_2d_workspace(X, Z):
    plt.figure(figsize=(8, 6))
    plt.scatter(X, Z, c='orange', s=1)
    plt.scatter(0, 0, color='red', s=50, label='Origin')
    plt.title("Hexapod Leg Workspace in X-Z Plane")
    plt.xlabel("X (mm)")
    plt.ylabel("Z (mm)")
    plt.grid(True)
    plt.legend()
    plt.axis('equal')
    plt.show()

def plot_3d_workspace(X, Y, Z):
    norm = plt.Normalize(Z.min(), Z.max())
    colors = cm.viridis(norm(Z))

    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(X, Y, Z, c=colors, s=1)
    ax.scatter(0, 0, 0, color='red', s=50, label='Origin')
    ax.set_title("Hexapod Leg 3D Workspace")
    ax.set_xlabel("X (mm)")
    ax.set_ylabel("Y (mm)")
    ax.set_zlabel("Z (mm)")
    ax.set_xlim([-300, 300])
    ax.set_ylim([-300, 300])
    ax.set_zlim([0, Z.max() + 20])
    ax.view_init(elev=30, azim=135)
    ax.legend()
    plt.show()


X, Y, Z = compute_workspace()

if mode.upper() == "2D":
    plot_2d_workspace(X, Z)
else:
    plot_3d_workspace(X, Y, Z)
