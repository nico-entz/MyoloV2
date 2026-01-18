import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# =========================
# Parameter
# =========================
CSV_FILE = "detector_data.csv"

CHANNEL_COL = 0
TIME_COL = 1
ENERGY_COL = 2      # ADC / Pulsintegral

COINC_WINDOW_US = 100
ENERGY_BINS = 250

# =========================
# Daten laden
# =========================
df = pd.read_csv(CSV_FILE, header=None)

channel = df.iloc[:, CHANNEL_COL].to_numpy()
time_us = df.iloc[:, TIME_COL].to_numpy()
energy = df.iloc[:, ENERGY_COL].to_numpy()

# =========================
# Nach Kanälen trennen
# =========================
t0 = time_us[channel == 0]
e0 = energy[channel == 0]

t1 = time_us[channel == 1]
e1 = energy[channel == 1]

# sortieren (wichtig für searchsorted)
idx = np.argsort(t1)
t1 = t1[idx]
e1 = e1[idx]

# =========================
# Koinzidenzen finden
# =========================
coinc_energy = []

for t, e in zip(t0, e0):
    i = np.searchsorted(t1, t)
    if i < len(t1) and abs(t1[i] - t) <= COINC_WINDOW_US:
        # SiPM + Plastik → Summe der Lichtmenge
        coinc_energy.append(e + e1[i])

coinc_energy = np.array(coinc_energy)

# =========================
# Histogram plotten
# =========================
plt.figure()
plt.hist(coinc_energy, bins=ENERGY_BINS, color='skyblue', edgecolor='black')
plt.xlabel("ADC Summe (SiPM)")
plt.ylabel("Koinzidenz-Counts")
plt.title("Koinzidenz-Energie-Histogramm")
plt.tight_layout()
plt.show()
