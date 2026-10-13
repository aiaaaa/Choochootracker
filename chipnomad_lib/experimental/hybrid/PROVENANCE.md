# Development provenance

This package contains newly authored C++ source developed through technical
documentation, behavioral reverse engineering and comparison testing. Development
also included review of existing reference code and headers; it is not described
as a formally separated clean-room process.

The engine uses independently expressed DSP and compact models calibrated from
observations. Reference implementation source files, translated routines,
extracted calibration tables and original noise sequences are not included.
Behavioral compatibility does not imply recovery of original source code or
complete equivalence to a particular instrument or plugin.

Waveform ROMs, commercial binaries, external preset collections, comparison
audio and research tooling are outside this source package. Tests generate their
own synthetic waveforms. Runtime wave banks are supplied separately; their
provenance and any applicable notices belong with those assets.

No third-party implementation is vendored in this package. Existing notices in
the separate research dependencies remain with those dependencies. Instrument
names identify compatibility targets and do not imply affiliation or endorsement.
No distribution license has been selected for this source package.
