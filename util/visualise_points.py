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

def plot_points_with_lines(points):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    xs, ys, zs = zip(*points)

    # Plot the line connecting all points
    ax.plot(xs[1:-1], ys[1:-1], zs[1:-1], marker='o')

    # Plot starting point
    ax.plot(xs[0], ys[0], zs[0], color='red', marker='o')

    # Plot change point 1
    ax.plot(xs[25], ys[25], zs[25], color='yellow', marker='o')

    # Plot ending point
    ax.plot(xs[-1], ys[-1], zs[-1], color='green', marker='o')

    # Labels and axes limits (optional)
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
