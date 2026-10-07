# Engine level measurement

From `tracker` in MSYS2 UCRT64, run `make -f Makefile.test measurements -j4`. This generates measurement WAVs for AY/YM (one chip, channel A, per track), Bogie, Braids, MME, Plaits/Plaits-Alt, and Sintered without adding them to Git. Plaits and Plaits-Alt are rendered through their native `TRIG/LPG` routing; percussive engines use 250 ms. `pcm/` contains the reference: a 90% sine PCM sample. The 32-bit WAVs are attenuated by 18.06 dB to keep peaks below full scale; the analysis restores this factor automatically.

To diagnose VCA routing separately, use `build/tests/render_engine_measurements.exe plaits-vca` or `plaits-alt-vca`. Never mix these WAVs into the native calibration.

Then, from the repository root:

```powershell
python scripts/measure_amplitude.py
```

The outputs are `results/amplitude_metrics.csv`, `results/model_amplitude_summary.csv`, and `results/compensation_gains.json`. The proposed gain aligns the median BS.1770 K-weighted loudness with PCM, then caps it to `[-12 dB, +6 dB]` while retaining 1 dB of headroom at the 95th percentile of peaks. The JSON also contains the nine Shaper compensation points for each MME model.
