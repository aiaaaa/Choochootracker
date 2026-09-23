# Mesure de niveau des moteurs

Depuis `tracker` dans MSYS2 UCRT64, lancez `make -f Makefile.test measurements -j4`. Cela crée, sans les ajouter à Git, les WAV de mesure pour AY/YM (une puce, canal A, par piste), Bogie, Braids, MME, Plaits/Plaits-Alt et Sintered. Plaits et Plaits-Alt sont rendus dans leur routage natif `TRIG/LPG`; les moteurs percussifs utilisent 250 ms. `pcm/` contient la référence : un échantillon PCM sinusoïdal à 90 %. Les WAV 32 bits sont atténués de 18,06 dB pour conserver les crêtes hors pleine échelle; l'analyse restaure automatiquement ce facteur.

Pour diagnostiquer séparément le routage VCA, utilisez `build/tests/render_engine_measurements.exe plaits-vca` ou `plaits-alt-vca`. Ne mélangez jamais ces WAV à la calibration native.

Puis, à la racine :

```powershell
python scripts/measure_amplitude.py
```

Les résultats sont `results/amplitude_metrics.csv`, `results/model_amplitude_summary.csv` et `results/compensation_gains.json`. Le gain proposé aligne la médiane de loudness K-pondérée BS.1770 sur PCM, puis le limite à `[-12 dB, +6 dB]` et préserve 1 dB de marge sur le 95e percentile des crêtes. Le JSON contient aussi les neuf points de compensation du Shaper pour chaque modèle MME.
