import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("src/sync_log.csv")

for col in df.columns[1:]:
    plt.plot(df["Tick"], df[col], label=col)

plt.xlabel("Tick")
plt.ylabel("Step Progress")
plt.title("Hexapod Leg Synchronisation Over Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("sync_plot.png")
plt.show()
