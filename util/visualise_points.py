import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def load_points_from_csv(filename):
    points = []
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header
        for row in reader:
            try:
                x, y, z = map(float, row)
                points.append((x, y, z))
            except ValueError:
                print(f"Skipping invalid row: {row}")
    return points

def set_equal_axes(ax, xs, ys, zs):
    max_range = max(
        max(xs) - min(xs),
        max(ys) - min(ys),
        max(zs) - min(zs)
    ) / 2.0

    mid_x = (max(xs) + min(xs)) / 2.0
    mid_y = (max(ys) + min(ys)) / 2.0
    mid_z = min(zs)

    ax.set_xlim(mid_x - max_range, mid_x + max_range)
    ax.set_ylim(mid_y - max_range, mid_y + max_range)
    ax.set_zlim(mid_z, mid_z + 2*max_range)

def plot_points_with_lines(points):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    xs, ys, zs = zip(*points)

    # Plot first halfslide
    ax.plot(xs[0:26], ys[0:26], zs[0:26], color='red')

    # Plot swing
    ax.plot(xs[26:-26], ys[26:-26], zs[26:-26], color='blue')

    # Plot second halfslide
    ax.plot(xs[-26:], ys[-26:], zs[-26:], color='green')

    # Plot key points
    ax.plot(xs[0], ys[0], zs[0], color='red', marker='o')
    ax.plot(xs[26], ys[26], zs[26], color='blue', marker='o')
    ax.plot(xs[-26], ys[-26], zs[-26], color='green', marker='o')
    ax.plot(xs[-1], ys[-1], zs[-1], color='black', marker='x')

    # Set equal aspect ratio for all axes
    set_equal_axes(ax, xs, ys, zs)

    # Labels
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('3D Line Visualization from CSV Points')

    plt.show()

if __name__ == '__main__':
    filename = 'build/step_xyz.csv'
    points = load_points_from_csv(filename)
    if points:
        plot_points_with_lines(points)
    else:
        print("No valid points found.")
