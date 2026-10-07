# Mesure de niveau des moteurs

Depuis `tracker` dans MSYS2 UCRT64, lancez `make -f Makefile.test measurements -j4`. Cela crée, sans les ajouter à Git, les WAV de mesure pour AY/YM (une puce, canal A, par piste), Bogie, Braids, MME, Plaits/Plaits-Alt et Sintered. Plaits et Plaits-Alt sont rendus dans leur routage natif `TRIG/LPG`; les moteurs percussifs utilisent 250 ms. `pcm/` contient la référence : un échantillon PCM sinusoïdal à 90 %. Les WAV 32 bits sont atténués de 18,06 dB pour conserver les crêtes hors pleine échelle; l'analyse restaure automatiquement ce facteur.

Pour diagnostiquer séparément le routage VCA, utilisez `build/tests/render_engine_measurements.exe plaits-vca` ou `plaits-alt-vca`. Ne mélangez jamais ces WAV à la calibration native.

Puis, à la racine :

```powershell
python scripts/measure_amplitude.py
```

Les résultats sont `results/amplitude_metrics.csv`, `results/model_amplitude_summary.csv` et `results/compensation_gains.json`. Le gain proposé aligne la médiane de loudness K-pondérée BS.1770 sur PCM, puis le limite à `[-12 dB, +6 dB]` et préserve 1 dB de marge sur le 95e percentile des crêtes. Le JSON contient aussi les neuf points de compensation du Shaper pour chaque modèle MME.


## Native chip and FM engines

`build/tests/render_engine_measurements native-chips` renders the public factory
ZIP presets for SID, OPLL/VRC7, OPL2, OPL3, Genesis, Arcade, DX7, Sega PSG,
GB Pulse and GB Noise, plus the same PCM reference. Personal USER libraries
are not read. Each patch is measured at MIDI 48 and 60, at 90% input gain and
96 kHz; percussion-category patches use 250 ms. OPLL and VRC7 share the same
programmable voice adapter and one pooled calibration. The established
pre-mixer measurement convention and runtime routing gains are retained.

Run `python scripts/measure_amplitude.py` on these WAVs to obtain the bounded
per-engine correction. Calibration is a fixed engine gain, not automatic
normalization or a per-preset rewrite. Repeated measurements include the
currently compiled calibration; treat their gains as residual corrections,
not as replacement absolute gains. Do not apply calibration twice. The
native gain header records the measurement result and baseline.
