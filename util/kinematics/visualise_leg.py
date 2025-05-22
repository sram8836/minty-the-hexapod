import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation

# load data
def load_trajectory(filename):
    frames = []
    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile)
        next(reader)  # skip header
        for row in reader:
            frame = [list(map(float, row[i:i+3])) for i in range(0, len(row), 3)]
            frames.append(frame)
    return frames

# animation
def animate_leg(frames):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_title("3-DOF Leg Trajectory")
    ax.set_xlim([-1, 1])
    ax.set_ylim([-1, 1])
    ax.set_zlim([0, 1])
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")

    # leg links
    line, = ax.plot([], [], [], 'o-', lw=2, color='blue')

    def update(frame):
        xs = [p[0] for p in frame]
        ys = [p[1] for p in frame]
        zs = [p[2] for p in frame]
        line.set_data(xs, ys)
        line.set_3d_properties(zs)
        return line,

    ani = animation.FuncAnimation(fig, update, frames=frames, interval=50, blit=True)
    plt.show()

if __name__ == "__main__":
    frames = load_trajectory("leg_trajectory0.csv")
    animate_leg(frames)
