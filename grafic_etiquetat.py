import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from matplotlib.patches import Patch

# === 1. Carregar i normalitzar la senyal ===
fs, dades = wavfile.read('pav_2181.wav')
dades = dades / np.max(np.abs(dades))
temps_total = len(dades) / fs
eix_temps = np.linspace(0, temps_total, len(dades))

# === 2. Funció per llegir etiquetes des de fitxer ===
def carrega_etiquetes(path, separador):
    resultat = []
    with open(path, 'r') as f:
        for línia in f:
            trossos = línia.strip().split(separador)
            if len(trossos) == 3:
                inici, fi, etiqueta = trossos
                resultat.append((float(inici), float(fi), etiqueta))
    return resultat

et_man = carrega_etiquetes('pav_2181.lab', ' ')
et_auto = carrega_etiquetes('pav_2181.vad', '\t')

# === 3. Fusionar segments consecutius amb la mateixa etiqueta ===
def fusiona_segments(etiquetes):
    if not etiquetes:
        return []
    fusionades = [etiquetes[0]]
    for ini, fi, lbl in etiquetes[1:]:
        ult_ini, ult_fi, ult_lbl = fusionades[-1]
        # Juntem si són consecutius i mateix tipus
        if lbl == ult_lbl and abs(ini - ult_fi) < 1e-9:
            fusionades[-1] = (ult_ini, fi, lbl)
        else:
            fusionades.append((ini, fi, lbl))
    return fusionades

et_auto_fus = fusiona_segments(et_auto)

# === 4. Plot de la senyal i les etiquetes ===
plt.figure(figsize=(14, 6))
plt.plot(eix_temps, dades, color='gray', alpha=0.6, label='Senyal de veu "pav_2181.wav"')

# Etiquetes manuals (v = veu, S = silenci) a y = -1.1
for ini, fi, lbl in et_man:
    col = 'blue' if lbl == 'V' else 'red'
    plt.fill_betweenx([-1.10, -1.00], ini, fi, color=col, alpha=0.8)

# Etiquetes automàtiques a y = -1.25
for ini, fi, lbl in et_auto_fus:
    col = 'blue' if lbl == 'V' else 'red'
    plt.fill_betweenx([-1.25, -1.15], ini, fi, color=col, alpha=0.8)

# Etiquetes de text a la dreta
plt.text(temps_total, -1.02, 'Manual', va='center', fontsize=10)
plt.text(temps_total, -1.18, 'Automàtic', va='center', fontsize=10)

# Llegenda personalitzada
llegenda = [
    Patch(facecolor='blue', edgecolor='black', label='Veu (V)'),
    Patch(facecolor='red', edgecolor='black', label='Silenci (S)')
]
plt.legend(handles=llegenda, title='Tipus', loc='upper left')

# Ajustos finals
plt.ylim(-1.4, 1.2)
plt.xlabel('Temps (segons)')
plt.ylabel('Amplitud')
plt.title('Comparació etiquetes manuals (.lab) vs automàtiques (.vad)')
plt.grid(which='both', linestyle='--', alpha=0.5)
plt.tight_layout()
plt.show()