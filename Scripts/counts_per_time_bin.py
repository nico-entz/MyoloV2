import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import timedelta, datetime

# =========================
# Parameter
# =========================
CSV_FILE = "detector_data.csv"

CHANNEL_COL = 0       # Kanal (0 oder 1)
TIME_COL = 1          # Zeitstempel in µs seit Start

COINC_WINDOW_US = 100     # Koinzidenzfenster
BIN_WIDTH_S = 60          # 60-Sekunden-Bins

# =========================
# Daten laden
# =========================
df = pd.read_csv(CSV_FILE, header=None)

channel = df.iloc[:, CHANNEL_COL].to_numpy()
time_us = df.iloc[:, TIME_COL].to_numpy()

# Zeit seit Start in Sekunden
time_s = (time_us - time_us.min()) / 1e6

# =========================
# Zeit-Bins
# =========================
t_max = time_s.max()
bins = np.arange(0, t_max + BIN_WIDTH_S, BIN_WIDTH_S)

rate0 = np.zeros(len(bins) - 1)
rate1 = np.zeros(len(bins) - 1)
rate_coinc = np.zeros(len(bins) - 1)

# =========================
# Koinzidenzvorbereitung
# =========================
t0 = time_us[channel == 0]
t1 = time_us[channel == 1]
t1.sort()

# =========================
# Auswertung
# =========================
for i in range(len(bins) - 1):
    t_start = bins[i] * 1e6
    t_end   = bins[i + 1] * 1e6

    mask0 = (t0 >= t_start) & (t0 < t_end)
    mask1 = (t1 >= t_start) & (t1 < t_end)

    n0 = np.sum(mask0)
    n1 = np.sum(mask1)

    coinc = 0
    for t in t0[mask0]:
        idx = np.searchsorted(t1, t)
        if idx < len(t1) and abs(t1[idx] - t) <= COINC_WINDOW_US:
            coinc += 1

    rate0[i] = n0 / BIN_WIDTH_S
    rate1[i] = n1 / BIN_WIDTH_S
    rate_coinc[i] = coinc / BIN_WIDTH_S

# =========================
# Zeitachse als HH:MM:SS
# =========================
start_time = datetime(2000, 1, 1)  # künstlicher Startzeitpunkt
time_axis = [
    start_time + timedelta(seconds=0.5 * (bins[i] + bins[i + 1]))
    for i in range(len(bins) - 1)
]

# =========================
# Plot
# =========================
plt.figure()
plt.plot(time_axis, rate0, label="Rate Kanal 0")
plt.plot(time_axis, rate1, label="Rate Kanal 1")
plt.plot(time_axis, rate_coinc, label="Koinzidenz (100 µs)")

plt.xlabel("Zeit seit Start (HH:MM:SS)")
plt.ylabel("Zählrate [Hz]")
plt.legend()
plt.grid(True)

ax = plt.gca()
ax.xaxis.set_major_formatter(mdates.DateFormatter("%H:%M:%S"))
ax.xaxis.set_major_locator(mdates.AutoDateLocator())

plt.tight_layout()
plt.show()
