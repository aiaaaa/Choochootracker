# ChooChooTracker Benutzerhandbuch

> Deutsche Übersetzung von [`docs/USER_MANUAL.md`](../USER_MANUAL.md), erstellt nach der englischen Fassung im Commit `0bffff9`. Bei Abweichungen gilt die englische Fassung.
>
> **Konventionen.** Die Oberfläche der Anwendung ist englisch. Deshalb bleiben Namen von Bildschirmen, Menüs, Feldern und Tasten (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**, **OPT**, **SELECT**...), FX-Codes (`HOP`, `TBX`, `M1A`...), Modul- und Engine-Namen (ADSR, LFO, Braids, Plaits...) und Dateiendungen unübersetzt, damit du sie auf dem Bildschirm wiedererkennst.

ChooChooTracker ist ein Musik-Tracker mit 8 Spuren für Handheld-Konsolen.

Er folgt dem klassischen Arbeitsablauf und der Navigation von LSDj, mit mehreren Möglichkeiten zur Klanggestaltung:

- Emulation des AY-Chips
- Oszillatoren Mutable Instruments Braids, Plaits und Plaits-Alt, mit einem Multimode-Filter
- PCM-Samples, Single-Cycle-Wellenformen und Wavetable-Oszillatoren, mit einem Multimode-Filter
- aChChid, ein vielseitiger Acid-Synthesizer auf Basis von Open303
- Bogie, ein nativer Drum-Synthesizer mit 12 Modellen und einem Multimode-Filter
- MME, ein Synthesizer mit zwei Oszillatoren und mehrfacher Modulation, mit einem Multimode-Filter
- Sintered, ein nativer experimenteller digitaler Drum-Synthesizer mit sechs Modellen

## 1. Installation und Dateien

### ArkOS / PortMaster

Installiere `choochootracker.zip` über PortMaster oder kopiere das entpackte Paket in das Verzeichnis `ports` der Konsole. Starte **ChooChooTracker** über das Ports-Menü.

### Miyoo Mini / Mini Plus (Onion OS)

Entpacke das MiyooPorts-Archiv im Hauptverzeichnis der SD-Karte, sodass seine Ordner `Roms/PORTS/` mit den bereits vorhandenen Ordnern von Onion zusammengeführt werden. Starte **ChooChooTracker** nach dem Aktualisieren der Sammlung über das Ports-Menü. Sichere den Ordner `projects/` der Anwendung, bevor du eine bestehende Installation ersetzt.

### Windows

Starte `choochootracker.exe`, wobei `SDL2.dll` und `libwinpthread-1.dll` im selben Verzeichnis liegen.
Drücke **ALT + ENTER**, um den Vollbildmodus umzuschalten. Die Fenstergröße ist veränderbar; das Bild
wird unter Beibehaltung des ursprünglichen Seitenverhältnisses skaliert.

### Web

Die Browserseite folgt dem aktiven Tracker-Theme. Gespeicherte Einstellungen, einschließlich
benutzerdefinierter Tastenbelegungen, die mit **Done** bestätigt wurden, werden umgehend in den
Speicher dieses Browsers geschrieben. Tippe nach einer Bildschirmsperre oder einem App-Wechsel auf ein Bedienelement, um bei Bedarf
den Ton wiederherzustellen. **Restart audio** öffnet den Ausgang neu, ohne das
Projekt zu verwerfen. **Reload app** speichert die aktuelle Arbeit in **Continue**, speichert die Einstellungen
und wartet auf den Browserspeicher, bevor neu geladen wird; ein fehlgeschlagenes oder abgelaufenes Speichern
bricht das Neuladen ab. Verwende **Download .cct** für eine portable Sicherung. Der Browserspeicher
kann unabhängig davon gelöscht werden, und die Wiederherstellung von Controller- und Audio-Routing unter iOS
ist nicht garantiert.

Die Browserversion aktualisiert ihre Anzeige mit etwa 30 FPS und nutzt einen festen
Audiopuffer von 1024 Frames, um dem Ton zwischen den Aufrufen mehr Zeit zu geben. Ältere gespeicherte
Puffereinstellungen werden automatisch angepasst; Projekte und andere Einstellungen bleiben
erhalten. Eingabeabfrage und App-Timer behalten ihren üblichen Takt.

### Verzeichniskonfiguration

| Ordner | Inhalt |
|---|---|
| `AY_Wavetables` | `.aywave`-Wavetables des AY-Chips |
| `fonts` | ChipNomad-Schriftdateien `.cnfont` |
| `instruments` | Instrument-Presets `.cni`, einschließlich ChipNomad-Dateien |
| `pitch-tables` | alternative Stimmungen `.csv` |
| `projects` | Song- und Projektdateien `.cct` |
| `samples` | `.wav`-Dateien für die Sampler-Engine |
| `SR_wavetables` | Wavetables im Serum-Format für die BYOWTBL-Engine |
| `themes` | ChipNomad-Themes `.cth` |
| `title` | Ressourcen des Titelbildschirms |
| `waveforms` | `.wav`-Single-Cycle-Wellenformen für die 2xSCWF-Engine |

## 2. Steuerung

ChooChooTracker funktioniert am besten mit einem Gamepad mit Steuerkreuz, 2 Analogsticks und 7 Tasten.

| Logisches Bedienelement | Standard auf dem Handheld | Standard unter Windows |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | Steuerkreuz | Pfeiltasten |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Leertaste |
| **SELECT** | Select | Shift |
| Modulationsachsen | linker und rechter Stick | Sticks des Game-Controllers |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Die Belegungen lassen sich unter **Settings > Key mapping** ändern. Speichere unter PortMaster eine
Neubelegung mit **Done** und lass die Taste los, bevor du in Settings weitermachst.
Desktop-Layouts verwenden QWERTZ als Standard nur in QWERTZ-Regionen; polnische Tastaturen
behalten die QWERTY-Standardwerte.

Auch Windows-, Web- und Android-Nutzer können einen Game-Controller verwenden. Unter Android
erscheinen die Touch-Bedienelemente automatisch, wenn kein Gamepad angeschlossen ist: im Hochformat
liegen sie unter dem Tracker, mit einem quadratischen Steuerkreuz, den Elementen A/EDIT, B/OPT, START/PLAY und
SELECT/SHIFT sowie einem separaten Live/Motion-Bereich mit zwei Modulationssticks
und kurzzeitigen REC/DEL-Bedienelementen. Im Querformat bleibt das Steuerkreuz links
und die Aktionstasten rechts. Android-Projekte und -Samples bleiben für
die Anwendung privat; verwende den Android-Dateiauswähler, um `.cct`-Projekte und `.wav`-Samples zu importieren
und um festzulegen, wo WAV-Exporte gespeichert werden.

### Navigation

- Verwende **[UP/DOWN/LEFT/RIGHT]**, um den Cursor zu bewegen.
- Halte **SELECT + [DIRECTION]** gedrückt, um zwischen Bildschirmen zu wechseln.
- **SELECT + [LEFT/RIGHT]** führt auch von Nebenseiten zum benachbarten MSCPIT-Bildschirm,
  wie bei Instrument Pool: Project/Settings (einschließlich der Settings-Untermenüs)
  führen zu Mixer/Chain; Groove zu Chain/Instrument; Modulation/Insert FX zu
  Phrase/Table; AY Wavetable zu Instrument/Table. Am linken Rand
  kehren Reverb/Delay mit SELECT + LEFT zum Mixer zurück. Die bestehende vertikale Navigation bleibt unverändert.
- Halte **OPT + [DIRECTION]** gedrückt für die bildschirmspezifische Navigation.
- Verwende auf einem Wert **EDIT + [LEFT/RIGHT]** für feine Änderungen oder **EDIT + [UP/DOWN]** für grobe Änderungen.
- Bei einer Modulations-**Destination** schaltet **LEFT/RIGHT** direkt durch die verfügbaren Ziele; tippe auf **EDIT**, um das gruppierte Menü zu öffnen.
- Tippe auf **EDIT**, um einen Wert einzugeben oder einen Befehl auszuführen.
- Tippe zweimal auf **EDIT**, um, wo unterstützt, eine neue Chain, Phrase oder ein neues Instrument anzulegen.
- Gedrückt gehaltene Richtungen wiederholen sich nach der in Settings eingestellten Verzögerung.

### Auswahl und Zwischenablage

- **SELECT + OPT** aktiviert den Auswahlmodus und schaltet durch sinnvolle Auswahlbereiche.
- Verwende **[DIRECTION]**, um den Bereich zu erweitern.
- **OPT** kopiert die Auswahl.
- **OPT + EDIT** schneidet sie aus.
- **SELECT + EDIT** fügt sie ein.
- **EDIT + [DIRECTION]** bearbeitet die ausgewählten Zellen gemeinsam.

Auf dem Phrase-Bildschirm kann bei aktiver Auswahl die Notenspalte
mit rhythmischen Mustern gefüllt werden:

- **OPT + LEFT** füllt die Notenzellen der Auswahl mit einem festen 16-Schritte-Muster
  und springt bei jedem Druck zum nächsten Muster. Nach dem letzten Muster werden die
  Noten gelöscht und der Zyklus beginnt von vorn. Die Muster sind an den absoluten
  Phrase-Zeilen ausgerichtet, sodass eine teilweise Auswahl nur die Schritte erhält, die in sie fallen.
- **OPT + RIGHT** füllt die Notenzellen der Auswahl mit einem zufälligen Muster aus
  3-8 Noten (7 und 8 sind weniger wahrscheinlich), das sich endlos wiederholt, solange **OPT**
  gehalten wird.
- **OPT + UP** verändert die bereits in der Auswahl platzierten Noten: Bei jedem Druck
  werden 1-3 davon ausgewählt und jede um einen zufälligen Betrag innerhalb einer Oktave verschoben
  (kleine Verschiebungen sind wahrscheinlicher als extreme). Bei aktivem **Note Lock**
  werden die verschobenen Noten wieder in die Tonleiter eingerastet; Sampler-Instrumente mit
  eingeschaltetem **Slice** bleiben chromatisch. Nichts wird gelöscht: Der 8. Druck stellt die
  Noten wieder her, die die Auswahl zu Beginn des Zyklus enthielt, danach beginnt der Zyklus von vorn.
- **OPT + UP** bei einer Auswahl, die nur die Lautstärkespalte umfasst (keine Noten-,
  Instrument- oder FX-Zellen), randomisiert stattdessen die Velocity: Der
  erste Druck wendet das Muster Minimal an (jede Lautstärke verschiebt sich um bis zu
  +-20), der zweite Medium (bis zu +-50), der dritte Random (völlig
  zufällige Werte), und der vierte stellt die Lautstärken wieder her, die die Auswahl
  zu Beginn des Zyklus enthielt. Das Loslassen von **OPT** setzt den Zyklus auf Minimal zurück.
  Zeilen ohne Lautstärke bleiben unberührt.
- **OPT + DOWN** verteilt die platzierten Noten geschnittener (sliced) Sampler-Instrumente in
  der Auswahl zu einer chromatischen Folge: Die erste Note jedes Instruments (die höchste
  im Sequenzer) behält ihren Wert, und jede folgende Note dieses
  Instruments steigt einen Halbton über die vorherige, sodass aus einem Stapel
  gleicher Noten aufeinanderfolgende Slice-Auslösungen werden. Mehrere geschnittene
  Instrumente breiten sich unabhängig von ihren eigenen ersten Noten aus, Noten
  von Instrumenten ohne Slicing bleiben unberührt, und das Ergebnis wird nie
  in die Tonleiter des Projekts eingerastet. Die letzte von der Verteilung geschriebene Note wird zur
  letzten bei der Eingabe verwendeten Note, sodass das manuelle Fortsetzen der Folge beim
  nächsten Slice weitermacht. Eine Auswahl, die nur geschnittene
  Instrumente enthält, verteilt bei jedem Druck; sind auch ungeschnittene Noten vorhanden,
  läuft die Verteilung beim ersten Druck, und ab dem zweiten übernimmt das zufällige Arpeggio
  diese Noten.
- **OPT + DOWN** bei einer Auswahl ohne Noten geschnittener Instrumente ist ein zufälliger
  Arpeggiator für die dort bereits platzierten Noten. Ein Akkord wird aus
  der CRD-Palette gewählt - Akkorde mit 3 Noten bei Auswahlen von 1-2 Noten, Akkorde mit 3 oder mehr Noten
  bei 3, beliebige Akkorde bei 4 oder mehr - mit seinem Grundton auf einer zufälligen Note
  der Projekt-Tonleiter im Register der Auswahl (jede chromatische
  Note, wenn die Tonleiter ausgeschaltet oder die Spur davon ausgenommen ist), und seine Töne
  werden wie bei der Wiedergabe für CRD in die Tonleiter quantisiert. Jeder
  Druck verteilt den Akkord neu auf die platzierten Noten: zufällig,
  von der tiefsten zur höchsten, von der höchsten zur tiefsten, dann auf und ab; der 5. Druck
  stellt die ursprünglichen Noten wieder her, und der nächste wählt einen neuen Akkord.
  Der Akkordname erscheint in der Statusleiste, solange das Arpeggio aktiv ist, es werden
  keine Noten hinzugefügt oder entfernt, und das Loslassen von **OPT** setzt den ganzen
  Vorgang zurück.
- Jeder Füll-Druck löscht die Noten der Auswahl vor dem Schreiben, und platzierte
  Noten tragen das zuletzt verwendete Instrument; das Loslassen von **OPT** setzt die Zyklen
  auf ihren ersten Schritt zurück.
- Solange eine Auswahl aktiv ist und keine andere Meldung angezeigt wird, zeigt die
  Statusleiste alle 2 Sekunden abwechselnd eine Erinnerung an diese Kombinationen: `EDIT + DIR =
  batch note edit`, `Double-tap EDIT: resample selection`, `OPT + LEFT =
  pattern fill`, `OPT + RIGHT = random fill`, `OPT + UP =
  mutate/randomize velocity` und `OPT + DOWN = random arp/slice spread`.
  Das Betreten oder Verlassen des Auswahlmodus startet den Wechsel neu; jede Aktionsmeldung
  ersetzt den Hinweis, bis sie abläuft.

### Wiedergabe

- **PLAY** startet ab dem Cursor. Drücke erneut, um zu stoppen.
- Du kannst Noten, Instrumente und Chains während der Wiedergabe bearbeiten; Änderungen sind
  beim nächsten Sequencer-Tick zu hören.
- **SELECT + PLAY** startet alle Spuren, wenn es außerhalb des Song-Bildschirms verwendet wird.
- **EDIT + PLAY** spielt ein Instrument aus dem Instrument Pool zur Probe ab.

## 3. Songstruktur und Bildschirmübersicht

Halte **SELECT + [DIRECTION]** gedrückt, um zwischen Bildschirmen zu wechseln.

Die Hauptnavigationszeile lautet:

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Mittlere Zeile:

- **M**: Mixer (verzweigt zu Reverb, Delay)
- **S**: Song (verzweigt zu Project, Settings)
- **C**: Chain
- **P**: Phrase (verzweigt zu Groove)
- **I**: Instrument (verzweigt zu Modulation und Instrument Pool)
- **T**: Table (verzweigt zum AY-Wavetable-Editor)

Obere Zeile:

- **R**: Reverb
- **P**: Project
- **G**: Groove
- **M**: Modulation

Untere Zeile:

- **D**: Delay
- **S**: Settings
- **P**: Instrument Pool
- **W**: AY-Wavetable-Editor

Ein ChooChooTracker-Song folgt dieser Hierarchie:

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

Ein Song hat 8 Spuren. Jede Spur enthält eine Folge von Chains, und jede Chain enthält eine Folge von Phrases. Phrases sind 16-Schritte-Folgen von Noten. Eine Note enthält eine Tonhöhe, ein Instrument, eine Velocity und bis zu 3 Spur-FX. Ein Instrument ist das, was den Klang erzeugt.

### Optionale 16x24-Schriften und NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 enthält TechMonoAudit (umgewandelt aus Share Tech Mono), Departure Mono, Spleen und Cozette. Jede liefert den vollständigen druckbaren ASCII-Zeichensatz in 16×24-Zellen für ein 640×480-Display. Aus Pixeln aufgebaute Schriften behalten ihre nativen Pixel oder eine ganzzahlige Skalierung mit Rand; Cozette bleibt kleiner, weil eine Verdopplung ihrer 13 Pixel hohen Tintenhöhe die Zelle überschreiten würde. Das Theme NostromoAmberDa2 ist über Settings → Edit color theme → Load verfügbar. Es behält den bernsteinfarbenen Hintergrund und den Infotext von Nostromo bei, mit rotem Haupttext, dunkleren orangefarbenen Werten und einem petrolfarbenen Cursor. Das Laden dieser Ressourcen ändert weder die gewählte Schrift noch das gewählte Theme. Quellenangaben und Lizenzen findest du in der beiliegenden README.txt.

### Abstände auf der Project-Seite

Die Project-Seite stellt die Anwendungsversion in eine eigene Zeile und trennt Load, Save, New, Export und Manage durch Leerzeilen von den Projekt-Metadaten. Cursorpositionen und Touch-Bereiche folgen den angezeigten Feldern; Projekteinstellungen und Dateioperationen bleiben unverändert.

### Telemetrie der Audioanzeige

Die Audioanzeigen verwenden einen der Oberfläche gehörenden Schnappschuss des Gesamtmixes und der Beiträge der einzelnen Spuren. Spur-Samples und Stereospitzen werden nach Spurpegel und Tilt, vor den gemeinsamen Effekt-Returns und der Master-Verstärkung erfasst. Das Monitoring verändert das Audiosignal nicht. Der Audio-Callback veröffentlicht Schnappschüsse fester Größe ohne Sperren oder Speicherzuweisung; der angezeigte Verlauf umfasst etwa 21 ms.

### Dauerhafte Wellenform

Graphics → Persistent waveform ist standardmäßig OFF. Tippe auf EDIT, um sie umzuschalten, oder verwende EDIT + Richtung; die Wahl wird als Anwendungseinstellung gespeichert. OFF stellt Seiten in voller Höhe und die bestehenden Instrument-Wellenformvorschauen wieder her. Wenn aktiviert, zeigt eine zweizeilige Wellenform über den Bearbeitungsseiten den Gesamtmix, zu Mono summiert. Die Seiten Instrument und Modulation zeigen den Ausgang einer Spur, die das gewählte Instrument verwendet; verwenden mehrere Spuren es, gewinnt die gewählte Song-Spur. Project, Settings, Wavetables und Popups behalten ihren Inhalt in voller Höhe. Listen scrollen innerhalb der verbleibenden Zeilen. Die Fußbereiche der Instrument-Seiten behalten die ADSR-/Sample-Vorschauen, und die Felder der Modulation werden verdichtet, damit alle Bedienelemente erreichbar bleiben. Ihre Touch-Bereiche folgen den angezeigten Feldern in beiden Wellenformmodi.

### Wiedergabe-Klavier

Ein per Code gezeichnetes Pixel-Klavier unter den acht Spurzeilen auf der rechten Seite leuchtet bei klingenden Tonhöhenklassen, auf eine Oktave zusammengefaltet. Es enthält Akkordnoten und beachtet stummgeschaltete oder gestoppte Spuren. Der Umriss verwendet die Wertefarbe des Themes, und alle aktiven Tasten verwenden die Wellenformfarbe; Tasten im Ruhezustand haben dunkle Füllungen und dezente Schatten. Sein Raster verwendet ganzzahlige Skalierung und Rahmen fester Breite. Das Klavier ist eine Wiedergabeanzeige, kein Bedienelement zur Noteneingabe. Popups und der Wavetable-Editor behalten in diesem Bereich ihren eigenen Inhalt.

### Mixer-Pegelanzeigen

Der Mixer zeigt für jede Spur nebeneinanderliegende **L/R**-Spitzenbalken, nach deren PAN und Tilt, vor den gemeinsamen Effekten und der Master-Verstärkung. Die Anzeige reicht von -48 bis 0 dBFS und fällt zwischen den Aktualisierungen ab. Das untere und mittlere Band verwenden Theme-Farben, während das obere Band nahe der Vollaussteuerung leuchtend rot ist.

## 4. Song, Chain und Phrase

### Song

Der Song-Bildschirm enthält 8 Spalten, 1 pro Audiospur. Jede Zelle verweist auf eine Chain. Ein Song kann bis zu 256 Zeilen enthalten (`00-FF`).

Spuren laufen von links nach rechts, ihre Chains dagegen vertikal. Jede Spur ist unabhängig, und Chains können 1 bis 16 Phrases enthalten, sodass Spuren auseinanderlaufen können. Gestalte deine Chains so, dass sie zusammenbleiben, oder lass sie wandern, wenn die Spur das braucht.

Es ist üblich, 1 Chain für leere Phrases zu reservieren, meist `00` oder `FE`.

Du kannst mehrere Untersongs in einem Projekt halten, indem du „Inseln“ anlegst: Abschnitte, die durch leere Zeilen getrennt sind.

#### Live Mode

Tippe zweimal auf **OPT**, um den Live Mode zu betreten oder zu verlassen; `LIVE` ersetzt den Song-Titel. Im Live Mode startet **PLAY** die gewählte Chain auf ihrer eigenen Spur, wenn diese gestoppt ist. Auf einer spielenden Spur stellt es die gewählte Chain für die nächste Chain-Grenze in die Warteschlange; drücke **PLAY** zweimal für einen dringenden Wechsel an der nächsten Phrase-Grenze. Eine gewählte leere Song-Zelle stellt stattdessen einen quantisierten Stopp in die Warteschlange. `+` kennzeichnet eine Chain in der Warteschlange, `!` eine dringende Chain und `-` einen Stopp in der Warteschlange.

Wähle eine Song-Zeile über mehrere Spalten, um diese Spuren gemeinsam in die Warteschlange zu stellen oder zu stoppen. Auswahlen über mehrere Zeilen werden bewusst abgelehnt. Live-Warteschlangen werden geleert, solange eine Song-Bereichsschleife aktiv ist. Live ist ein Performance-Modus und wird nicht im Projekt gespeichert.

#### Steuerung

- **OPT + [UP/DOWN]**: 16 Positionen nach oben oder unten springen
- Wähle einen Bereich, dann **SHIFT + EDIT**: Chains flach klonen und dabei ihre ursprünglichen Phrases behalten
- Wähle einen Bereich, dann zweimal auf **SHIFT + EDIT** tippen: sowohl die Chains als auch ihre Phrases tief klonen
- Wähle einen Bereich, dann **EDIT + [UP/DOWN]**: die Auswahl nach oben oder unten verschieben
- Tippe 3-mal auf **OPT**: eine Hervorhebung für die visuelle Organisation hinzufügen oder entfernen
- **OPT + SHIFT**: die aktuelle Spur oder die gewählten Spalten stummschalten (lass zuerst **OPT** los, damit die Stummschaltung aktiv bleibt)
- **OPT + PLAY**: die aktuelle Spur oder die gewählten Spalten auf Solo schalten (lass zuerst **OPT** los, damit das Solo aktiv bleibt)
- **OPT + [LEFT/RIGHT]**: alle Spuren links oder rechts der aktuellen Spur auf Solo schalten
- Wähle einen Bereich, dann zweimal auf **EDIT** tippen: den Bildschirm BOUNCE TO SAMPLE öffnen (siehe Bounce unten)

#### Bounce

Wähle einen Bereich aus Song-Zeilen und -Spalten und tippe zweimal auf **EDIT**, um den Bildschirm BOUNCE TO SAMPLE zu öffnen. Der Bounce enthält jede gewählte Spur: Spuren, deren erste Chain später in der Auswahl beginnt, warten stumm und steigen am Beginn ihrer ersten Chain ein, sodass die Datei die Chains aller gewählten Spuren enthält.

Der Bildschirm bietet:

- **File name**: Bearbeitung mit der Zeichentastatur. Beim Öffnen des Bildschirms wird hier die nächste freie Sequenznummer (`001`, `002`, ...) vorgeschlagen: die erste Nummer, deren `.wav`-Datei im Exportordner noch nicht existiert. Bearbeite sie frei: Der Name wird unverändert verwendet. Existiert bereits eine Datei mit diesem Namen, wird ein Suffix `_001`, `_002`, ... angehängt, sodass vorhandene Dateien nie überschrieben werden. Der Zähler wird zurückgesetzt, wenn du ein Projekt lädst oder anlegst.
- **Sample rate** und **Bit depth**: dieselben Optionen wie auf dem Exportbildschirm
- **Include in a sample name**: drei Kontrollkästchen, die beim Drücken von **Start** dem Dateinamen Kontext-Tags voranstellen (standardmäßig alle aus):
  - **[BPM]** - das aktuelle Tempo des Projekts, z. B. `[120]`
  - **[Key]** - Grundton und Tonleiter des Projekts, z. B. `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** - die gerenderte Länge des Bounce: Eine Phrase entspricht einem Takt (16 Sechzehntel), ein Schlag sind 4 Sechzehntel. Ganze Phrases zeigen nur Takte, z. B. `[2]`; teilweise Phrases zeigen `[0:2]` (zwei Schläge) oder `[0:1:3]` (ein Schlag plus drei Sechzehntel). Bei Bounces über mehrere Spuren entscheidet die längste Spur. Eine Auswahl, die mitten in einem Schlag endet, zählt ein Sechzehntel weniger: Die letzte gewählte Zeile ist der Schnittpunkt, sodass ihr Note-off am Ende des vorherigen Sechzehntels landet.
  - Die Tags werden in der Reihenfolge `[BPM][Key][Length]` vor dem Dateinamen angewendet.
- **Start**: die Auswahl in eine WAV-Datei im Exportordner rendern (siehe *Export location* unter dem Project-Bildschirm)
- **Cancel**: ohne Bounce zurückkehren

Während der Bounce rendert, bricht **OPT** ihn ab. Wenn der Bounce abgeschlossen oder abgebrochen ist, kehrt der Bildschirm zurück, von dem aus du gebounct hast.

Auf dem Desktop bringt Key Jazz hier auch die direkte Eingabe von Hex-Indizes, die
Auswahl per Shift+Pfeil wie in Phrase, Delete/Backspace/Insert und Ctrl+C/X/V - siehe
[Key jazz](#key-jazz-nur-desktop).

### Chain

Eine Chain ist eine geordnete Liste von 16-Schritte-Phrases mit optionaler Transposition. Sie kann bis zu 16 Phrases enthalten, und dieselbe Phrase kann mehrfach vorkommen. Die 2. Spalte legt die Transposition in Halbtönen fest.

Ein Sternchen (`*`) erscheint neben einer Chain, die im Projekt wiederverwendet wird. Du kannst Chains vom Song-Bildschirm aus klonen.

#### Steuerung

- **OPT + [LEFT/RIGHT]**: zwischen Spuren wechseln
- **OPT + [UP/DOWN]**: zwischen Chains der aktuellen Spur wechseln
- Wähle einen Bereich, dann **SHIFT + EDIT**: Phrases klonen
- Wähle einen Bereich, dann zweimal auf **EDIT** tippen: den Bildschirm BOUNCE TO SAMPLE für die gewählten Chain-Zeilen der aktuellen Spur öffnen (siehe Bounce unter Song)

Auf dem Desktop bringt Key Jazz hier auch die direkte Eingabe von Hex-Indizes - siehe
[Key jazz](#key-jazz-nur-desktop).

### Phrase

Eine Phrase ist das Spurmuster in einem traditionellen Step-Sequencer.

Eine Phrase ist 16 Schritte lang, mit 1 Zeile pro Schritt. Jede Zeile enthält eine Note, ein Instrument, eine Lautstärke von `00` bis `7F` und FX-Spalten. Noten verwenden die Tracker-Notation, etwa `C-4` (Note und Oktave).

Um eine Note zu beenden, füge `NOTE OFF` ein, verwende den Kill-Note-FX oder spiele eine andere Note ohne gesetztes Instrument.

Enthält eine Chain mehrere Phrases, funktioniert der Phrase-Bildschirm wie ein fortlaufender Pattern-Editor. Wer unter Zeile `F` oder über Zeile `0` hinausgeht, öffnet die nächste oder vorherige Phrase der Chain.

Ein Sternchen (`*`) erscheint neben einer Phrase, die im Projekt wiederverwendet wird. Du kannst Phrases vom Chain-Bildschirm aus klonen.

Die FX-Auswahl zeigt gängige Befehle plus die, die das Instrument in dieser Zeile unterstützt. So bleibt eine lange globale Liste engine-spezifischer Befehle erspart. ChooChooTracker unterstützt mehr FX als ChipNomad.

### Steuerung

- **OPT + EDIT** auf einer leeren Note: `NOTE OFF` einfügen
- **EDIT + [UP/DOWN]** auf einer FX-Namensspalte: den FX-Auswahlbildschirm öffnen
- **OPT + [LEFT/RIGHT]**: zwischen Spuren wechseln
- **OPT + [UP/DOWN]**: zwischen Phrases der aktuellen Chain wechseln
- Wähle einen Bereich in der Instrumentspalte, dann **SHIFT + EDIT**: Instrumente klonen
- Wähle einen Bereich, dann **EDIT + [UP/DOWN]**: die Phrase-Zeilen rotieren
- Wähle einen Bereich, dann zweimal auf **EDIT** tippen: den Bildschirm BOUNCE TO SAMPLE für die gewählten Phrase-Zeilen der aktuellen Spur öffnen (siehe Bounce unter Song)

### Key jazz (nur Desktop)

Drücke **Esc** auf dem Phrase-Bildschirm, um Key Jazz ein- oder auszuschalten, einen Modus zur
Noteneingabe im Stil eines QWERTY-Klaviers (wie [m8c](https://github.com/laamaa/m8c)):

- `Z S X D C V G B H N J M` und `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` geben Noten
  chromatisch über zwei und mehr Oktaven ein und verwenden die physische Position jeder Taste,
  sodass das Layout auf AZERTY-, QWERTY- und QWERTZ-Tastaturen gleich ist.
- Jeder Tastendruck schreibt die Note in die aktuelle Zeile und rückt zur nächsten vor.
- `[` / `]` verschieben die Oktave.
- **Shift + Pfeile** wählen einen Bereich aus Zeilen und Spalten; ein einfacher Pfeil
  danach hebt die Auswahl auf.
- **Ctrl+C** / **Ctrl+X** kopieren oder schneiden die gewählten Spalten aus, oder
  Note/Instrument/Lautstärke der aktuellen Zeile, wenn nichts gewählt ist; **Ctrl+V** fügt am
  Cursor ein.
- **Delete** entfernt die ganze(n) Zeile(n) (alle Spalten) und rückt den Rest der
  Phrase nach oben, um die Lücke zu schließen; der Cursor bleibt in derselben Zeile.
- **Backspace** entfernt das Element unter dem Cursor nur in der aktuellen Spalte
  (oder in den Spalten der Auswahl, falls eine aktiv ist), rückt alles darunter
  nach oben, um die Lücke zu schließen, und setzt den Cursor eine Zeile zurück, wie in
  einem Texteditor.
- **Insert** fügt eine leere Zeile ein und schiebt den Rest der Phrase nach unten.
- **Ctrl+S** speichert das Projekt. Außerhalb von Key Jazz tut Ctrl+S nichts.
- Esc erneut schaltet Key Jazz aus.

Solange Key Jazz an ist, übernimmt es auf dem Phrase-Bildschirm vollständig die Notentasten,
einschließlich der Edit/Opt/Motion-Kürzel, die sich diese Tasten teilen - schalte es
mit Esc aus, um sie wieder zu verwenden.

Key Jazz funktioniert auch, unabhängig per Esc umschaltbar, auf:

- **Song** und **Chain**: gib den Hex-Index einer Chain oder Phrase direkt ein
  (`0-9`, `A-F`), statt mit Up/Down zu erhöhen. Die erste in eine Zelle getippte
  Ziffer ersetzt deren Wert; weitere ohne Bewegung getippte Ziffern rücken
  hinein (tippst du „3“ und dann „F“, ergibt das 3F). Auf Song wählt **Shift+Pfeile** einen
  Bereich, **Delete** entfernt die ganze(n) Zeile(n) und rückt den Rest nach oben, **Backspace**
  entfernt die aktuelle(n)/gewählte(n) Spalte(n) und rückt, was darunter liegt, nach oben,
  **Insert** fügt eine leere Zeile ein, und **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  kopieren/schneiden/fügen die gewählten Zeilen ein (oder die aktuelle Zeile, wenn nichts
  gewählt ist) - dieselben Kürzel zur Strukturbearbeitung wie auf dem Phrase-Bildschirm.
  Das übernimmt Shift, sodass Shift+Right/Up nicht mehr zu Chain/Project springen,
  solange Key Jazz hier aktiv ist - drücke Esc, um diese Kürzel zurückzubekommen.
- **Jeder Bildschirm mit einem Textfeld**: tippe direkt auf der Tastatur
  statt über die virtuelle Bildschirmtastatur. Das betrifft Dateiname, Titel
  und Autor in Project, den Instrumentnamen, den Namen des Farb-Themes,
  den Namen der Stimmungstabelle, den Bounce-Dateinamen und die Namensdialoge (Namenseingabe,
  Ordner erstellen). Backspace funktioniert wie in einem normalen Textfeld; Shift
  tippt Großbuchstaben. Esc schaltet es auf all diesen Bildschirmen gleichzeitig um,
  und die virtuelle Tastatur öffnet sich weiterhin normal, wenn Key Jazz aus ist.

## 5. Instruments

Alle Instrumente haben einige gemeinsame Parameter:

- Instrumenttyp
- Name, bis zu 15 Zeichen
- Standard-Tabellengeschwindigkeit, in Ticks pro Tabellenzeile
- Transposition ein oder aus

Jedes Instrument hat eine Standardtabelle mit derselben Nummer im Bereich `00-7F`. Du kannst diese als Hilfstabellen verwenden, aber wenn du Hilfstabellen im Bereich `80-FE` hältst, vermeidest du Konflikte und Verwirrung. Tabellen gehören zu den wichtigsten Werkzeugen der Klanggestaltung in ChooChooTracker.

### Steuerung

- **OPT + [LEFT/RIGHT]**: zwischen Instrumenten wechseln
- **EDIT + PLAY**: das Instrument vorhören
- **SHIFT + OPT**: das Instrument kopieren
- **SHIFT + EDIT**: das Instrument einfügen

### AY Classic, AY Plus und AY Sample

Dies sind die ursprünglichen AY/YM-Engines von ChipNomad. AY Classic bietet Hardware-nahe Steuerung von Ton, Rauschen und Hüllkurve. AY Plus fügt Software-Oszillatoren und eine reichhaltigere Modulation hinzu. AY Sample gibt ein Sample über AY-artige Lautstärkestufen wieder und unterscheidet sich von der Sampler-Engine.

#### AY Classic

Dies ist ein Legacy-Instrumenttyp aus ChipNomad `v1.0.0`. Das neuere Instrument AY Plus bietet mehr Funktionen.

AY-Classic-Instrumente haben diese Parameter:

- Mixer: Ton ein oder aus, Rauschen ein oder aus, Hüllkurvenform (`0-F`)
- Volume: softwaregenerierte ADSR-Hüllkurve
- Automatische Hüllkurvenperiode: ein oder aus, mit einem Verhältnis von `1:1` bis `F:F`

**AY Quality** in Settings betrifft nur das AY/YM-Rendering. Es ändert nicht die Qualität von Braids, Plaits oder Sampler. **Sample dithering** gilt für die Quantisierung von AY Sample.

Einzelheiten zum Chip findest du in der [ChipNomad-Dokumentation zum AY-3-8910](https://chipnomad.org/chips/ay-3-8910/).

#### AY Plus

AY Plus wendet klassische Synthesizer-Konzepte auf die Klanggestaltung mit dem AY-3-8910/YM2149F an. Du kannst dir den AY als Synthesizer mit 5 Oszillatoren vorstellen:

- 3 Ton-Oszillatoren mit Rechteckwelle
- 1 Rausch-Oszillator
- 1 Amplituden-Hüllkurvengenerator, der als Oszillator verwendet werden kann

Nach diesem Modell hat jedes Instrument 3 Hardware-Oszillatoren: Ton, Rauschen und Hüllkurve. AY Plus fügt einen 4., Software-Oszillator hinzu. Die AY-Register schneller zu aktualisieren als die Tickrate des Projekts eröffnet mehr Möglichkeiten der Klanggestaltung. Aufgrund der Funktionsweise des AY werden die Oszillatoren per Ringmodulation gemischt. Probiere verschiedene Tonhöhen-Offsets, Verstimmungen und Oszillator-Kombinationen aus, um zu hören, was dabei herauskommt.

Jeder tonale Oszillator kann unabhängig mit Tonhöhen- und Feinstimmungs-Offsets gestimmt werden.

Typen von Software-Oszillatoren und ihre Parameter:

- Pulse - Rechteckwelle mit steuerbarer Pulsbreite
  - FM Depth - einfache FM-Tiefe
  - Pulse width - der volle Bereich ist `00-FF`, während die Anzahl der Schritte auf dem Project-Bildschirm festgelegt wird
  - Pulse low - steuert den niedrigen Wert eines sich wiederholenden Paares, dessen hoher Wert immer `15` ist
- Sync Tone - Hard-Sync-Effekt für den Ton-Oszillator
  - FM Depth - einfache FM-Tiefe
- Sync Env - Hard-Sync-Effekt für den Hüllkurven-Oszillator
  - FM Depth - einfache FM-Tiefe
  - Pulse width - Dauerverhältnis zwischen den 2 Formen des Hüllkurvenpaares
  - Envelope Pair - 2 Hüllkurvenformen, zwischen denen umgeschaltet wird
- Wavetable - Wavetables mit 32 Schritten, bearbeitet auf dem Wavetable-Bildschirm
  - FM Depth - einfache FM-Tiefe
  - Wavetable index
- Tone FM - einfacher FM-ähnlicher Effekt für den Ton-Oszillator
  - FM Depth - FM-Tiefe des Tons
- Env FM - einfacher FM-ähnlicher Effekt für den Hüllkurven-Oszillator
  - FM Depth - FM-Tiefe der Hüllkurve

Die Software-Oszillatoren Pulse und Wavetable können nicht mit dem Envelope-Oszillator verwendet werden. Der Envelope-Oszillator schaltet sich automatisch ab.

Einfache FM schaltet schnell zwischen 2 Periodenwerten um, einem niedrigeren und einem höheren. Das ähnelt der Verwendung einer Rechteckwelle als FM-Modulator. Pulse, Sync Tone, Sync Env und Wavetable verwenden ein Verhältnis von `1:1` zwischen FM-Träger und -Modulator. Tone FM und Env FM können andere Verhältnisse über die Tonhöhen-Offsets des Software-Oszillators nachahmen.

#### AY Sample

Du kannst Mono- oder Stereo-WAV-Dateien mit `8 Bit`, `16 Bit`, `24 Bit` oder `32 Bit` laden. Sie werden in `8 Bit` umgewandelt und auf `16 KB` gekürzt. Diese Grenze entspricht der RAM-Seitengröße des ZX Spectrum.

Der AY spielt Samples durch schnelle Schreibzugriffe auf das Lautstärkeregister ab. Sein Ausgang ist unipolar, daher müssen Samples für eine sauberere Wiedergabe „auf null angehoben“ werden. Das beseitigt Klicks am Sample-Ende und verbessert leise Ausklänge.

Der AY hat einen `4-Bit`-DAC mit nichtlinearer Lautstärkeskala, daher werden `8-Bit`-Samples bei der Wiedergabe in `4 Bit` umgewandelt. Die Option Sample dithering in Settings kann die wahrgenommene Bittiefe verbessern. Dithering ist auf Retro-Plattformen wie dem ZX Spectrum und dem Atari ST nicht praktikabel, lass es daher aus, wenn du für echte Hardware schreibst oder einen knackigen Lo-Fi-Sound suchst.

Samples verhalten sich wie ein weiterer Software-Oszillator von AY Plus, aber ihre zusätzlichen Parameter machen sie als eigenen Instrumenttyp leichter handhabbar.

Sample-Parameter:

- Sample rate - ein Sample mit seiner nativen Rate abzuspielen ergibt ein `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

Die Abschnitte der Oszillatoren Tone und Noise entsprechen denen von AY Plus. Aufgrund der Funktionsweise des AY werden alle Oszillatoren per Ringmodulation gemischt.

### Braids

Braids bietet 47 Synthesemodelle. Seine wichtigsten Bedienelemente sind:

- **Model**: Oszillator oder Syntheseverfahren
- **Timbre** und **Color**: modellabhängige Makroparameter
- **Filter**: zusätzlicher LP/BP/HP-Filter, Flankensteilheit `12` oder `24 dB`, Grenzfrequenz und Resonanz
- **ADSR**: Attack, Decay, Sustain und Release

Tippe auf **Model**, um aus nach Kategorien geordneten Modelllisten zu wählen.

| Kategorie | Modelle |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Parameter

Timbre und Color hängen vom gewählten Modell ab. Die vollständige Modellliste und Details findest du im ursprünglichen Braids-Handbuch von Mutable Instruments, oder spiele einfach nach Gehör.

#### VCF und VCA

Braids speist den Standard-Multimode-Filter und die ADSR-Hüllkurve von ChooChooTracker.

#### Einstellungen

Globale Braids-Einstellungen finden sich in den Anwendungseinstellungen:

- **BITS**: verringert die Ausgangsauflösung
- **DRFT**: fügt den Oszillatoren eine Tonhöheninstabilität hinzu
- **SIGN**: reproduziert den Algorithmus für Wellenform-Unvollkommenheiten des ursprünglichen Braids

### aChChid

**aChChid** ist eine monophone Acid-Bass-Engine auf Basis von Open303. `Square` und `Saw` verwenden den nativen TB-303-Oszillator, -Filter, die Hüllkurve und das Akzentverhalten. `Braids` ersetzt nur den Oszillator und läuft dann durch denselben 303-Filter- und Verstärkerpfad weiter. Es bietet Model, Timbre, Color und Shaper anstelle von Fine tune. Shaper geht schrittweise von sanfter Sättigung zum Wavefolding über. aChChid verwendet weder den einheitlichen Post-Filter noch die ADSR von ChooChooTracker.

Cutoff und Decay werden logarithmisch bearbeitet: **EDIT + LEFT/RIGHT** bewegt um einen
musikalischen Steuerungsschritt, während **EDIT + UP/DOWN** um sechzehn Schritte bewegt. Cutoff
deckt 200 Hz bis 20 kHz ab; Decay deckt 200 ms bis 2 s ab.

Ein `F` in der Lautstärkespalte der Note löst einen Akzent aus. Normale Noten verwenden ein Gate von einem halben Schritt (3 Ticks im Standard-Groove mit 6 Ticks). `ASL` gleitet von der vorherigen Tonhöhe zu dieser Note, ohne die 303-Hüllkurve neu auszulösen; es hält automatisch das vorangehende Gate, und `ASL 00` ergibt ein Glissando von `60 ms`. `ATY` hält seine Note für den gesamten aktuellen Schritt offen. Noten ohne `ASL` lösen immer neu aus. Zu den Modulationszielen gehören Decay und Accent sowie Timbre und Color im Braids-Wellenmodus.

### Bogie

**Bogie** ist ein nativer One-Shot-Drum-Synthesizer. Tracker-Noten legen die Tonhöhe fest, und die Lautstärke von Tracker/Instrument legt den Pegel fest; jeder Trigger hat sein eigenes internes Decay, daher gibt es keine ADSR-Seite. Er hat 12 Modelle: `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` und `Clave`.

Seine sechs Makros sind **Decay**, **Tone**, **Sweep**, **Noise**, **FM** und **Drive**. Ihre Bezeichnungen passen sich an, wo es sinnvoll ist (zum Beispiel ist Noise bei Kick `Click`, und FM bei Cowbell ist eine metallische Kreuzmodulation). Jedes Makro ist in jedem Modell aktiv, aber seine musikalische Rolle ändert sich mit dem Modell: Sweep kann Tonhöhe, Burst-Abstand oder metallische Streuung verändern; FM kann Kreuzmodulation, eine metallische Schicht oder einen ringartigen Oberton hinzufügen. Die untere Hälfte jedes Bereichs ist für konventionelle Drum-Sounds gedacht; die obere Hälfte öffnet bewusst weitere Klangfarben-Verwandlungen und synthetischere Ergebnisse.

Jedes Modell kann den gemeinsamen LP/BP/HP-Multimode-Filter verwenden, mit dem Charakter Clean, Classic, Aggro oder Acid, Flankensteilheit `12`/`24 dB`, Grenzfrequenz und Resonanz. Bogie unterstützt alle seine aktiven Makros sowie Volume, Pitch, Cutoff und Resonance als Ziele für Modulation und Bewegungsaufzeichnung. Verwende ein kurzes Decay mit Cowbell für scharfe Phonk-Attacks; erhöhe FM für eine härtere, metallischere Glocke.

Drücke auf dem Modulation-Bildschirm **EDIT** auf einer Quelle, um eine Familie (Envelopes, LFO oder Sticks) und dann die Quelle zu wählen. **Left** und **Right** schalten weiterhin direkt durch die Quellen.

### Sintered

**Sintered** ist ein nativer digitaler One-Shot-Percussion-Synthesizer. Noten legen seine Grundtonhöhe fest; seine sechs Modelle sind `Knot`, `Shard`, `Burst`, `Comb`, `Logic` und `Melt`. Jedes Modell hat **Decay**, **Mod** und **Motion** sowie drei modellspezifische Regler: Knot verwendet Ratio/Spread/Fold, Shard Ratio/Feedback/Bite, Burst Noise/Color/Feedback, Comb Time/Damping/Regen, Logic Rate/Pattern/Crush und Melt Ratio/Chaos/Drive. **Mod** ist immer die interne Kopplungstiefe.

Motion ist eine interne Makro-Hüllkurve, kein Tonhöhen-Glide: Werte unter `80` verwenden eine AD-Form, `80` schaltet sie aus, und Werte über `80` verwenden eine Form nur mit Decay. Je weiter man sich von der Mitte entfernt, desto schneller wird die Hüllkurve. Ihre Ziele sind pro Modell festgelegt, sodass Sintered in Tracker-Patterns vorhersehbar bleibt: Knot animiert Mod/Fold; Shard Feedback/Bite; Burst Noise/Color/Feedback; Comb Damping/Regen; Logic Rate/Crush; Melt Chaos/Drive. Alle Modelle setzen ihren Zustand bei einem Note-Trigger zurück und begrenzen das interne Feedback für wiederholbare Anschläge.

### MME

**MME** (Multi Modulation Engine) ist ein Synthesizer mit zwei Oszillatoren, der den Wechselwirkungen zwischen Oszillatoren gewidmet ist. Seine Modelle sind `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` und `Vocode`. Beide Oszillatoren sind intern und folgen den Tracker-Noten; `Interval` legt ihr Verhältnis fest.

Die sechs Makros sind **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** und **Shaper**. Waves wählt musikalische A/B-Wellenformpaare. Amount steuert die Kernwechselwirkung des Modells, Flow ändert deren Richtung oder Algorithmusvariante, und Feedback ist begrenzt, wird aber am Ende seines Bereichs bewusst wild. Shaper ist eine gemeinsame Endstufe, die von sauber über Sättigung bis zum Wavefolding geht.

MME verwendet den gemeinsamen LP/BP/HP-Multimode-Filter und die ADSR. Alle sechs Makros, Cutoff und Resonance sind für Modulation und Bewegungsaufzeichnung verfügbar. Halte **EDIT** auf Model gedrückt, um das Modell-Popup zu öffnen.

### MIDI Out

**MIDI Out** (nur Desktop) steuert ein externes MIDI-Gerät, statt Audio zu synthetisieren: Das Auslösen einer Note sendet ein echtes MIDI Note On auf dem **Channel** des Instruments (`1-16`) und, wenn die Note endet, das passende Note Off. Die Lautstärke wird zur Velocity, und jede Stimme einer Akkordspur erhält ihr eigenes Note On/Off. Von ChooChooTracker selbst ist kein Ton zu hören - wähle zuerst das Ausgabegerät unter [Settings](#12-settings). Das Gesamtbild, einschließlich des Vorhörens von Klängen über ein MIDI-Keyboard, siehe [MIDI](#14-midi).

**Program** und **Bank high/low** (CC0/CC32) sind optional - bei Deaktivierung als `--` angezeigt, `Clear` schaltet ein Feld aus und merkt sich seinen Wert. Wenn gesetzt, werden sie einmal gesendet, direkt vor der nächsten Note, sobald sie sich von dem unterscheiden, was diesem Kanal zuletzt mitgeteilt wurde (nicht vor jeder Note, was die eigenen Hüllkurven bzw. den Patch des empfangenden Geräts neu auslösen würde).

**MC1 number**-**MC4 number** wählen, an welche CC-Nummer (`00-7F`) jeder der vier Zeilen-FX MC1-MC4 sendet - siehe [MIDI Out FX](#midi-out-fx). Ebenfalls `--`/optional; ein nicht gesetzter Slot macht seinen FX wirkungslos.

### Subtraktive Engines

Die Engines dieser Kategorie teilen sich eine VCO-zu-VCF-zu-VCA-Architektur.

Mehrere VCO-Typen sind verfügbar:

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Jedes Instrument speist seinen VCO in einen Multimode-Filter.

Der Filter hat eine umschaltbare Flankensteilheit von `12 dB` oder `24 dB` und kann als Tiefpass (LP), Bandpass (BP) oder Hochpass (HP) arbeiten.

Mehrere Filtercharaktere sind verfügbar:

- **Clean**: steril und digital
- **Classic**: inspiriert von klassischen amerikanischen Analogsynthesizern
- **Aggro**: inspiriert von röhrenden japanischen Analogsynthesizern
- **Acid**: inspiriert von Psychedelika

Classic, Aggro und Acid können in dieser Alpha-Version sehr stark resonieren. Fang niedrig an und dreh sie vorsichtig auf.

Der Filter speist einen VCA, der von einer ADSR-Hüllkurve gesteuert wird. Die Hüllkurvenform kann zwischen exponentiellen, linearen und logarithmischen Kurven übergehen.

#### Plaits und Plaits-Alt

**Plaits** bietet 24 Engines, im Auswahl-Popup wie folgt gruppiert:

| Kategorie | Engines |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** ist eine Sammlung alternativer Modelle für das ursprüngliche Plaits-Modul. Die meisten sind seltsamer oder experimenteller als der durchschnittliche VCO.

| Kategorie | Engines |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

Die Parameter folgen dem Design von Mutable:

- **Harmonic** steuert harmonische Beziehungen, Balance oder die Modellwahl innerhalb einer Engine.
- **Timbre** geht im Allgemeinen von dunklen/dünnen zu hellen/dichten Spektren.
- **Morph** erkundet eine weitere Klangfarbendimension.
- **Main/Aux** mischt den Hauptausgang mit dem alternativen Ausgang der Engine.

Ihre genaue Bedeutung hängt von der Engine ab. Akkord-Engines verwenden sie zum Beispiel für Akkordtyp, Umkehrung und Wellenform. Physikalische Modelle verwenden sie für Material, Anregung und Ausklang, während Drum-Engines sie für Ton, Charakter und Ausklang verwenden. Einzelheiten findest du im Plaits-Benutzerhandbuch.

Bei `CHORD`, `DIATONIC CHORD` und `WT DIATONIC CHORD` zeigt Harmonic den Namen des gewählten Akkords (`MIN`, `MAJ`, `MIN7`, `MAJ7` usw.) statt seines rohen Steuerwerts. Braids `WAVE PARA` benennt seine Color-Voicings ähnlich.

Tippe auf **Engine**, um aus nach Kategorien geordneten Engine-Listen zu wählen.

**Env Mode** hat 2 Routings:

- `TRIG` gibt das Modul mit angeschlossenem TRIG und nicht gepatchtem LEVEL wieder
- `VCA` hält LEVEL offen und wendet die Tracker-ADSR nach der Stimme an

#### Sampler

Diese saubere Sample-Engine spielt Mono- oder Stereo-PCM-Samples ab, die in den RAM geladen sind.

- Tippe auf **Sample**, um eine unkomprimierte PCM-WAV mit `8 Bit`, `16 Bit` oder `24 Bit` zu laden. Drücke im Browser **PLAY**, um die markierte Datei vorzuhören.
- Ist ein Sample geladen, erscheint neben seinem Namen **EDIT**. Tippe darauf, um den Sample-Edit-Bildschirm zu öffnen. **OPT** oder **SELECT + [LEFT]** kehrt zum Instrument zurück.
- Der Sample-Edit-Bildschirm zeigt den Dateinamen (mit einer Markierung `*`, wenn das Sample im RAM von der Datei auf dem Datenträger abweicht), eine Formatanzeige (Samplerate, Kanäle), eine hohe Wellenform und die Felder **Region**, **Select**, **Slice**, **Process** und **File**.
- Bearbeitungen leben nur im RAM, bis ein Speichervorgang sie schreibt: Das Projekt speichert den Dateipfad des Samples und lädt die WAV beim nächsten Laden des Projekts vom Datenträger neu, sodass das Verlassen des Trackers ohne **Save**/**Save As** Bearbeitungsvorgänge verwirft (die Markierung `*` ist die Warnung). Auch das Laden eines anderen Samples in das Instrument verwirft nicht gespeicherte Bearbeitungen und setzt die Region auf das ganze Sample zurück (Start `00`, End `FF`).
- **Region** legt die Wiedergabe-Marker Start/End fest, die auch auf dem Sampler-Instrumentbildschirm verfügbar sind. Sie legen normalisierte Wiedergabegrenzen fest (`00-FF`); liegt Start hinter End, wird das Sample rückwärts abgespielt. **EDIT + [LEFT/RIGHT]** stellt den Marker in Einerschritten (`01`) fein ein und zoomt die Wellenform um ihn herum, solange **EDIT** gehalten wird; **EDIT + [UP/DOWN]** stellt grob ein (Schritt 16) und setzt die Ansicht auf das ganze Sample zurück. **EDIT + OPT** setzt den Marker auf seinen Standard zurück (Start `00`, End `FF`).
- Die Wellenformansicht zoomt auf ein festes Fenster von einer Sekunde Audio um den bearbeiteten Marker und verschiebt sich, um ihn sichtbar zu halten; Samples, die ins Fenster passen, bleiben in der vollen 1:1-Ansicht. Der Zoom ist vorübergehend: Er dauert, solange **EDIT** gehalten wird, und das Loslassen von **EDIT** kehrt zur vollen 1:1-Ansicht zurück, ebenso wie grobe Schritte. Das Betreten des Bildschirms setzt die Ansicht immer auf das ganze Sample zurück.
- **Select** legt eine Bearbeitungsauswahl in absoluten Frames fest, unabhängig von den Wiedergabe-Markern Start/End. **EDIT + [LEFT/RIGHT]** bewegt einen Griff um zwanzig Frames und zoomt darauf, solange **EDIT** gehalten wird; **EDIT + [UP/DOWN]** springt um `frameCount/64` (mindestens 16) und kehrt zur Ansicht des ganzen Samples zurück. **EDIT (Tippen)** auf einem Griff kopiert die aktuelle Position des Markers Start oder End dorthin. **EDIT + OPT** auf einem der beiden Griffe leert die gesamte Auswahl. Ist die Auswahl leer, zeigen beide Griffe `-`; sind die Griffe vertauscht, werden sie automatisch getauscht. Die Auswahl ist ein reiner Sitzungszustand des Editors: Sie wird nicht mit dem Projekt gespeichert, und beim Betreten des Bildschirms wird sie mit der Spanne der Wiedergabe-Region vorbelegt (das ganze Sample bei den Standard-Markern).
- **Slice** ist eine einzelne Zeile mit vier Wertezellen - **Mode**, **Count**,
  **Slice** und **Frame** - und wird mit dem Instrument gespeichert. Off spielt das
  Fenster Start/End. Phrase-Noten wählen Slices chromatisch ab **C-0**, und
  Noten hinter dem letzten Slice springen an den Anfang zurück (Note N spielt Slice
  `N % count`, sodass ein Lauf über den letzten Slice hinaus wieder beim ersten landet).
  Geschnittene Noten transponieren die Tonhöhe nicht und verwenden keine Scale-Quantisierung, aber **CRD**
  behält den gewählten Slice bei und transponiert seine Stimmen als Akkord. Slice-Anfänge
  sind auf der Wellenform durch kurze, zwei Pixel breite Linien markiert (dunkelorange; die Linie
  des aktuellen Slice ist heller), und der Slice unter dem Cursor erhält ein
  schwarzes Hintergrundband auf der Wellenform (die Auswahlfärbung wird nicht über das
  Band gezeichnet, sodass es vollständig schwarz bleibt und sich deutlich vom Rest der
  Vorschau abhebt).
  - **Mode** schaltet durch `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + links/rechts oder
    Tippen; EDIT + OPT schaltet das Slicing aus). Ein Moduswechsel initialisiert die Slice-
    Punkte: **EQUAL** teilt das Fenster Start/End gleichmäßig in **Count**
    Teile; **AUTO** führt eine Transientenerkennung über das Fenster aus und setzt einen
    Slice auf jeden gefundenen Einsatz (bis zu **Count**); **LAZY** beginnt mit einem
    Slice, der das ganze Fenster abdeckt. Der Wechsel zu LAZY legt die aktuelle
    EQUAL/AUTO-Einstellung (Anzahl und Grenzen) beiseite, und der Wechsel zurück zu EQUAL oder
    AUTO stellt sie unverändert wieder her - es wird keine Bestätigung verlangt und nichts
    geht verloren. Slice und Stretch schließen sich gegenseitig aus: Das Aktivieren des einen schaltet
    das andere ab, und die Slice-Zellen sind abgedunkelt, solange Stretch die
    Dauer bestimmt.
  - **Count** zeigt die Gesamtzahl der Slices (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** ändert sie in Einerschritten (Minimum 1, Maximum 64), **EDIT +
    [UP/DOWN]** schaltet durch die Zweierpotenzen `2 / 4 / 8 / 16 / 32 /
    64` mit Überlauf. Jede Änderung berechnet die Aufteilung neu: **EQUAL**
    teilt das Fenster erneut gleichmäßig, **AUTO** führt die Erkennung mit dem neuen
    Ziel erneut aus (die Erkennungsempfindlichkeit wird aus der Anzahl abgeleitet - mehr Slices
    bedeuten eine empfindlichere Schwelle). Das Feld zeigt `-`, wenn der Modus
    OFF ist. In **LAZY** bleibt es hell (es zählt die von Hand gesetzten Slices), dient aber
    nur der Anzeige: Der Cursor überspringt es - nach rechts von **Mode** oder
    nach links von **Slice** landet man auf **Slice**, von der **END**-Markierung
    der **Select**-Zeile nach unten landet man ebenfalls dort, und das Antippen des Feldes bewirkt nichts
    (von Hand gesetzte Slices haben keine Aufteilung, die neu berechnet werden könnte). **EDIT + OPT** und
    ein einfaches Tippen auf **EDIT** bewirken hier nichts: Das Ausschalten des Slicings erfolgt
    auf **Mode**, das Löschen eines Slice auf **Slice**.
  - **Slice** ist der Browser: Er zeigt den aktuellen Slice, ab 1 nummeriert
    (`01`..count). **EDIT + [LEFT/RIGHT]** wechselt einzeln zwischen den Slices,
    **EDIT + [UP/DOWN]** springt um vier Slices (beides begrenzt, ohne
    Überlauf); der durchsuchte Slice wird auf der Wellenform durch seinen helleren
    Marker und das schwarze Band angezeigt, und die gezoomte Ansicht zentriert sich neu auf den
    Slice-Anfang. **EDIT + OPT** löscht den aktuellen Slice (er verschmilzt mit
    dem vorherigen; der erste Slice kann nicht gelöscht werden, und das Löschen des
    letzten verbleibenden Slice schaltet das Slicing aus - die Grenzen überstehen eine Hin-und-zurück-Fahrt
    über OFF und kehren zurück, wenn wieder ein Modus gewählt wird). **EDIT (Tippen)**
    spielt den aktuell gewählten Slice als einmalige Wiedergabe vor (funktioniert in
    jedem Slice-Modus; das Vorhören stoppt, wenn die Taste losgelassen wird oder wenn du
    den Bildschirm verlässt). Das Feld zeigt `-`, wenn der Modus OFF ist.
  - **Frame** zeigt den Start-Frame des aktuellen Slice in Hex. **EDIT +
    [LEFT/RIGHT]** verschiebt ihn um zehn Frames (mit Zoom auf den Marker, solange
    **EDIT** gehalten wird), **EDIT + [UP/DOWN]** verschiebt um hundert und
    kehrt zur vollen Ansicht zurück. Ein Marker bleibt innerhalb seines eigenen Slice: Er
    kann den Anfang des vorherigen oder nächsten Slice nicht überschreiten. **EDIT + OPT** löscht
    den Slice.
  - **B** führt wie überall sonst auf diesem Bildschirm von der Slice-Zeile zurück zum
    Instrumentbildschirm; **SHIFT + LEFT** verlässt von überall.
  - **LAZY-Arbeitsablauf**: Tippe bei gewähltem LAZY auf **PLAY**, um eine einmalige
    Wiedergabe des ganzen Samples zu starten - es spielt einmal von Anfang an und stoppt
    von selbst, mit der ursprünglichen Tonhöhe und Geschwindigkeit des Samples (ein grüner Marker von zwei Pixeln
    folgt der Position auf der Wellenform); erneutes Tippen auf **PLAY**
    stoppt es früher. Solange es spielt,
    setzt jeder **EDIT**-Klick einen Slice an der Wiedergabeposition, unabhängig davon,
    wo der Cursor steht (Slices, die näher als 50 ms an einem vorhandenen liegen, werden
    mit `Too close to slice` abgelehnt). Das erneute Abspielen des Samples behält
    die vorhandenen Schnitte und fügt an jeder angetippten Position neue hinzu, in
    der Reihenfolge - das Feld **Count** aktualisiert sich mit jedem gesetzten Slice und der
    **Slice**-Browser folgt dem neuen. Für von Hand gesetzte Slices gelten dieselben Regeln zum
    Durchsuchen und Bearbeiten wie in den anderen Modi: **EDIT +
    Richtung** auf **Slice** durchsucht sie, **Frame** stellt den gewählten
    Anfang ein, **EDIT + OPT** löscht den aktuellen Slice. Die Frame-Zelle ist abgedunkelt,
    solange das Setzen während der Wiedergabe scharfgestellt ist. **SHIFT + PLAY** wird in
    LAZY nicht abgefangen: Es startet die Phrase-Wiedergabe wie auf jedem anderen Bildschirm. LAZY-
    Slices spielen im Sequencer genau wie EQUAL und AUTO: Phrase-
    Noten werden chromatisch ab **C-0** auf die von Hand gesetzten Slices abgebildet,
    mit Überlauf hinter dem letzten.
  - **Kombinations-Hinweise**: Solange der Cursor auf einer Zelle der Slice-Zeile ruht und keine andere
    Meldung angezeigt wird, zeigt die Statusleiste den Hinweis dieser Zelle und behält ihn,
    solange der Cursor dort ruht. Auf **Mode** ist der Hinweis an den
    aktiven Modus gebunden und zeigt nur die Beschreibung (der Modusname steht bereits in
    der Zelle): `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - ein Moduswechsel
    tauscht den Text sofort, und OFF leert die Leiste. Auf **Count** zeigt sie
    `Adjust the number of slices`, auf **Slice**
    `Browse slices`, auf **Frame** `Adjust slice start`. Jede Aktionsmeldung
    ersetzt den Hinweis, bis sie abläuft.
- **Process** wählt eine destruktive Bearbeitungsoperation: **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** oder **Reverse** (EDIT + links/rechts schaltet durch, Tippen schaltet vorwärts, EDIT + OPT wählt keine). **GO** (in derselben Zeile) führt die gewählte Operation an der aktuellen Auswahl aus, oder am ganzen Sample, wenn die Auswahl leer ist. **Crop** behält nur die Auswahl; **Normalize** skaliert die Auswahl so, dass ihre Spitze die Vollaussteuerung erreicht (beide Kanäle teilen sich eine Verstärkung, damit das Stereobild erhalten bleibt); **Delete** entfernt die Auswahl und fügt die Enden zusammen (das Löschen des ganzen Samples wird abgelehnt); **Silence** setzt sie auf null; **Fade In**/**Fade Out** lassen die Auswahl linear aus der Stille anschwellen bzw. in die Stille abklingen; **Reverse** spielt die Auswahl rückwärts (Frames werden an Ort und Stelle getauscht, beide Kanäle eines Frames bewegen sich gemeinsam, Länge und Marker bleiben unverändert). Crop und Delete benötigen eine Auswahl - bei leerer Auswahl zeigen sie `Select region first`. Jede Operation pausiert das Audio kurz, behält eine Rückgängig-Ebene und markiert das Sample im RAM als geändert: Die Datei auf dem Datenträger wird erst bei den Save-Abläufen angetastet (siehe unten), und das Verlassen des Bildschirms verwirft den Rückgängig-Speicher.
- **UNDO** (neben GO) setzt das Sample auf den Zustand vor der letzten Operation zurück. Es ist abgedunkelt, bis eine Operation ausgeführt wurde, wechselt bei wiederholtem Drücken zwischen den Zuständen vor und nach der Operation und wird beim erneuten Betreten des Bildschirms geleert.
- **File** enthält die Speicherabläufe. Das Instrument speichert den vollständigen Pfad der WAV, aus der es geladen wurde; diese Abläufe schreiben diese Datei oder lassen das Instrument auf eine neue zeigen. Sie rühren den Instrumentnamen nie an.
  - Das Feld File schaltet zwischen **Save** und **Save As** um (EDIT + links/rechts oder Tippen); **GO** (in derselben Zeile) führt die angezeigte Aktion aus.
  - **Save** überschreibt die WAV, aus der das Sample geladen wurde. Bei einem Sample
    ohne Slices wird zuerst `Overwrite <name>?` gefragt. Ein geschnittenes Sample öffnet zuerst
    den Dialog **SAVE SLICES**, der fragt, wohin die Slice-Punkte gehören:
    **SAVE TO SAMPLE** schreibt sie als Cue-Chunks in die WAV (portabel -
    DAWs zeigen sie als Marker) und behält sie auch im Projekt,
    **SAVE TO PROJECT** schreibt eine einfache WAV und behält die Punkte
    nur im Projekt, **CANCEL** bricht ab. Ein Kontrollkästchen `Don't ask again in this project`
    speichert das gewählte Ziel mit dem Projekt und überspringt
    den Dialog künftig (die Wahl wird als `- Sample save choice:` in
    der .cct gespeichert und lässt sich nur durch Bearbeiten der Projektdatei ändern). Der
    Dialog entfällt bei Samples ohne Slices. **Save** ist abgedunkelt (und
    wird bei der Navigation übersprungen), bis ein Sample mit Dateipfad geladen ist. Ein
    fehlgeschlagener Schreibvorgang behält die Änderungsmarkierung und zeigt den Fehler.
  - **Save As** schreibt das aktuelle Sample in eine neue Datei: Gib einen Dateinamen ein (vorbelegt mit dem aktuellen Namen ohne Endung) und wähle dann im Browser einen Ordner. Das Sample wird als `<folder>/<name>.wav` geschrieben, das Instrument zeigt auf die neue Datei, und der Ordner wird als Standard-Sample-Ordner gemerkt. Es ist abgedunkelt (und wird bei der Navigation übersprungen), bis Sample-Daten geladen sind, sodass einem frisch geladenen Sample ein Dateipfad gegeben werden kann. Pfade mit mehr als 255 Zeichen werden abgelehnt.
  - Die Markierung `*` vor dem Dateinamen bedeutet, dass das Sample im RAM von der Datei auf dem Datenträger abweicht (jede Bearbeitungsoperation setzt sie, Save und Save As löschen sie). Die Markierung gilt nur für die Sitzung: Sie wird nicht mit dem Projekt gespeichert und beim erneuten Betreten des Bildschirms zurückgesetzt. Verweisen mehrere Instrumente auf dieselbe WAV-Datei, überschreibt das Speichern eines davon die Datei für alle.
- Verwende auf dem Sample-Instrumentbildschirm **EDIT + [LEFT/RIGHT]**, um die vorherige oder nächste WAV im selben Ordner zu laden.
- **Pitch** transponiert in Halbtönen (`-48` bis `+48`).
- **Loop** wählt Off, Loop oder Ping-Pong.
- **Speed** steuert granulares Time-Stretching von `0%` bis `500%` (`100%` ist normal).

Nicht unterstützte WAV-Formate zeigen einen Fehler. Konvertiere ungewöhnliche Dateien vor dem Import in `PCM8`-, `PCM16`- oder `PCM24`-WAV.

#### 2xSCWF

2xSCWF ist ein dualer Single-Cycle-Wellenform-Oszillator. Lade in jeden Oszillator 1 Mono-WAV, die genau 1 Periode enthält. Beide Oszillatoren lesen vorwärts und springen am Ende ihres Zyklus zurück.

Mix blendet zwischen A und B über.

Detune ist fein und exponentiell vom Einklang bis `+200 ct` und geht dann in Halbtonschritten von `+3 st` bis `+24 st`.

2xSCWF behandelt jede Datei als Tabelle mit 1 Zyklus und liest sie genau einmal pro Oszillatorzyklus. So lassen sich 2 Single-Cycle-Wellen mischen und verstimmen.

Der Werksordner `waveforms/AKWF/` enthält Wellenformen aus der Adventure-Kid-Bibliothek. Viele weitere sind online verfügbar, wenn du die Sammlung erweitern willst.

#### BYOWTBL

BYOWTBL ist ein dualer Wavetable-Oszillator, kompatibel mit Serum-Tabellen. Jeder Oszillator lädt eine Mono-WAV mit aufeinanderfolgenden Single-Cycle-Frames. `Pos A` und `Pos B` durchlaufen ihre Tabellen unabhängig voneinander von `00` bis `FF`. Die Engine interpoliert linear innerhalb jedes Frames und zwischen benachbarten Frames. `Mix` blendet zwischen den Oszillatoren über, und `Detune` verstimmt Oszillator B.

## 6. Modulation und Bewegungsaufzeichnung

Jedes Instrument kann bis zu 4 Modulations-Slots haben. Audio-Instrumente können **Instrument Pan** oder **Track Pan** ansteuern; beide verwenden `00` für links, `80` für die Mitte und `FF` für rechts.

Die verfügbaren Modulationstypen sind:

- ADSR - klassische Attack-Decay-Sustain-Release-Hüllkurve
- AHD - Attack-Hold-Decay-Hüllkurve
- LFO - Niederfrequenzoszillator mit mehreren Formen
- SLFO - LFO mit einem Periodenmultiplikator für langsame Zyklen
- FLFO - LFO im Audiobereich, der bis in den kHz-Bereich reichen kann; FLFO kann die CPU stark belasten
- SLIN - lineare Joystick-Steuerung
- SRAT - ratenbasierte Joystick-Steuerung

Die Liste der Modulationsziele hängt vom Instrumenttyp ab.

Bei einem Ziel `Cutoff` ist Amount exponentiell mit quadratischer Kennlinie: 25 % erreichen 4,5 Halbtöne, 50 % erreichen 18 Halbtöne, 75 % erreichen 40,5 Halbtöne und 100 % erreichen sechs Oktaven. So bleibt bei niedrigen Beträgen eine feine Kontrolle erhalten, während dennoch tiefe Sweeps möglich sind.

Wenn eine ADSR- oder AHD-Hüllkurve Volume ansteuert, wird sie zur Lautstärkehüllkurve des Instruments. Ein LFO, der Volume ansteuert, verschiebt stattdessen die aktuelle Ausgangslautstärke.

Der Modulationsbetrag kann positiv oder negativ sein. Sein Bereich hängt vom Ziel ab. Beträgt der volle Bereich des Ziels `127` oder weniger, wie bei den meisten Parametern, legt der Betrag einen absoluten Bereich fest. Weitere Ziele wie die Tonhöhe verwenden einen skalierten Betrag.

Alle Dauerparameter, einschließlich Attack, Hold, Decay und Periode, werden in Ticks gemessen.

LFO-Formen:

- Tri - bipolare Dreieckwelle
- Sin - bipolare Sinuswelle
- UniTri - unipolare Dreieckwelle
- UniSin - unipolare Sinuswelle
- RampDn - lineare abfallende Rampe (Sägezahnwelle)
- RampUp - lineare ansteigende Rampe (Sägezahnwelle)
- ExpDn - exponentiell abfallende Rampe (exponentieller Sägezahn)
- ExpUp - exponentiell ansteigende Rampe (exponentieller Sägezahn)
- Square - Rechteckwelle
- Random - zufällige Sample-and-Hold-Welle
- Wavetable - liest eine der 32-Schritte-AY-Wavetables des Projekts. Die Werte `00` bis `0F` bilden `-100%` bis `+100%` ohne Interpolation ab.

LFO-Triggertypen:

- Free - startet den LFO nur neu, wenn sich das Instrument ändert
- Retrig - startet den LFO bei jeder Note neu
- Phrase - startet LFO und SLFO neu, wenn die Wiedergabe in eine neue Phrase eintritt
- Chain - startet LFO und SLFO neu, wenn die Wiedergabe in eine neue Chain eintritt
- Hold - stoppt nach 1 Zyklus und hält den letzten Wert
- Once - stoppt nach 1 Zyklus und kehrt auf null zurück

### Bewegungsaufzeichnung (Motion recording)

- **STICK LIVE** (standardmäßig `L1`) wendet die Stick-Modulation an, ohne die Phrase zu ändern. Unter **Settings > Stick live mode** aktiviert `HOLD` (Standard) sie, solange die Taste gehalten wird; `TOGGLE` aktiviert sie mit einem Druck und deaktiviert sie mit dem nächsten; `FREE` lässt sie dauerhaft aktiviert. Das Loslassen der Taste bei `TOGGLE` lässt sie aktiviert.
- Halte während der Wiedergabe **MOTION RECORD** (`L2`), um die Stick-Modulation anzuwenden und geänderte Ziele als absolute FX-Werte aufzuzeichnen.
- Halte **MOTION ERASE** (`R2`), um passende Ziel-FX aus der aktuellen Zeile zu entfernen.

Das `~` unten rechts zeigt aktives Stick live an. Record (`*`, oder `!` bei Überlauf) und Erase (`x`) haben Vorrang bei der Anzeige und bleiben in beiden Modi kurzzeitig. Beide aktivieren die Live-Stick-Verarbeitung, solange sie gehalten werden; ihr Loslassen lässt einen separat aktivierten Live-Schalter unangetastet.

Der Modus wird mit den Anwendungseinstellungen gespeichert, aber der Live-Schalter startet beim Start immer ausgeschaltet. Ein Moduswechsel löscht den Schalter: `HOLD` folgt jeder gerade gehaltenen Stick-live-Taste, `TOGGLE` wartet auf einen neuen Druck, und `FREE` ist sofort aktiv.

Die Bewegungsaufzeichnung schreibt Spur-FX in die gerade spielende Phrase. Sie aktualisiert zuerst passende FX und nutzt dann leere Slots von rechts nach links. Sie überschreibt nie einen anderen FX. Sind alle 3 Slots belegt, wird diese Bewegung im Schritt nicht aufgezeichnet. Ein `!` in der unteren rechten Ecke bedeutet, dass sich mehr Ziele geändert haben, als die 3 FX-Spalten aufnehmen konnten.

Die Bewegungsaufzeichnung unterstützt Ziele von Braids, Plaits, Sampler, 2xSCWF, BYOWTBL und Bogie, einschließlich aktiver Bogie-Makros und Filtersteuerung, wo anwendbar.

## 7. Tables

Tabellen sind in Trackern ein zentrales Werkzeug der Klanggestaltung. In den Begriffen von Vortex Tracker kombinieren sie Instrumente und Ornamente, können aber viel mehr. Wenn du LSDj oder NerdSEQ kennst, sollte dir die Idee schon vertraut vorkommen.

Die Spalte Pitch akzeptiert relative (`~`) oder absolute (`=`) Tonhöhenwerte in Halbtönen. Die Spalte Volume reicht von `00` bis `0F`, wirkt bei jeder Engine zusätzlich zu ihrer Hüllkurve und kann als Gate dienen: Wechsle `0F` und `00` ab und schleife dann mit `HOP 00`. Die 4 FX-Spuren entsprechen größtenteils den Spuren einer Phrase, auch wenn sich einige FX in Tabellen anders verhalten.

Ein `TIC`-FX in der letzten Tabellenzeile legt die Geschwindigkeit dieser Spalte fest und überschreibt die Standard-Tabellengeschwindigkeit des Instruments. Jede FX-Spalte kann mit einer anderen Tick-Geschwindigkeit laufen. Die Spalten Pitch und Volume folgen der Geschwindigkeit der 1. FX-Spalte.

Eine Tabelle mit 16 Zeilen mag neben einem langen Vortex-Tracker-Instrument kurz wirken, aber der FX `HOP` kann bedingte Schleifen wie „diese Zeilen 5-mal wiederholen“ sowie verschachtelte Schleifen erzeugen. Mit Schleifen und unabhängigen Geschwindigkeiten der FX-Spalten reichen diese 16 Zeilen überraschend weit.

### Retrigger-Modus

Das Feld `Retrig` oben im Table-Bildschirm legt fest, wann eine bereits aktive Tabelle neu startet. `Inst` ist der Standard- und Kompatibilitätsmodus: Eine Phrase-Zeile mit einem Instrument wählt die Standardtabelle dieses Instruments und startet sie in Zeile `00`. `Phrase` startet eine aktive Tabelle neu, wenn die Wiedergabe in eine Phrase eintritt, `Chain`, wenn sie in eine Chain eintritt, und `Free` lässt sie laufen, bis die Spur stoppt oder ein expliziter Befehl sie ersetzt.

Ein struktureller Modus greift nie zeitlich rückwärts. Hat eine Spur keine aktive Instrumenttabelle, aktiviert die erste Phrase-Zeile, die ein Instrument einführt, dessen Tabelle genau in dieser Zeile, selbst wenn es Zeile `4`, `8` oder `F` einer Phrase ist. Spätere Instrumentnummern ersetzen keine aktive Tabelle `Phrase`, `Chain` oder `Free`. So sind Tabellen als LFO-ähnliche Automation nützlich, ohne frühere Phrase-Schritte stillschweigend zu simulieren.

`TBL`, `TBX` und `RET` sind immer explizite Aktionen: Sie wählen aus oder starten sofort neu, unabhängig vom Modus. An einer Phrase- oder Chain-Grenze erfolgt der Neustart, bevor die neue Phrase-Zeile gelesen wird, sodass ein `TBL` oder `TBX` in dieser Zeile das letzte Wort hat. Die FX der Tabellenzeile `00` laufen daher wieder normal; kumulative FX behalten ihre übliche Akkumulation.

Die Tabellen `00-7F` sind für Standard-Instrumenttabellen reserviert. Die Tabellen `80-FE` sind für Hilfstabellen gedacht, die vom Effekt `TBX` gestartet werden. Der untere Bereich funktioniert ebenfalls für Hilfstabellen, aber diese Verwendung kann unerwartete Konflikte und Verwirrung verursachen.

### FX zwischen Phrase und Table kopieren

Wähle nur FX-Namens-/Wertspalten, kopiere oder schneide sie dann aus einer Phrase aus und füge sie in eine beliebige FX-Spur einer Table ein, oder umgekehrt. Die Kopie behält genau die gewählten Zellen, sodass eine Auswahl nur der Befehle oder nur der Werte teilweise bleibt. Das Einfügen beginnt an der aktuellen FX-Zelle und wird an Zeile `F` und der letzten FX-Spur des Ziels abgeschnitten. Noten, Instrumentnummern, Tonhöhe und Lautstärke wandern nie zwischen diesen beiden Bildschirmen.

### Steuerung

- **EDIT + [UP/DOWN]** auf einer FX-Namensspalte: den FX-Auswahlbildschirm öffnen
- **OPT + [DIRECTION]**: zwischen Tabellen wechseln

## 8. Weitere Bildschirme

### Instrument Pool

Der Instrument Pool zeigt jedes Instrument des Projekts und ermöglicht, sie neu zu ordnen. Während der Wiedergabe zeigt er außerdem, welche Instrumente gerade aktiv sind.

#### Steuerung

- **EDIT**: das Instrument bearbeiten und zum Instrument-Bildschirm springen
- **SHIFT + OPT**: das Instrument kopieren
- **SHIFT + EDIT**: das Instrument einfügen
- **EDIT + [UP/DOWN]**: Instrumente neu ordnen
- **EDIT + PLAY**: das Instrument vorhören

### AY Wavetable

Wavetable ist ein Software-Oszillatortyp von AY Plus. Ein Projekt kann bis zu 256 Wavetables mit je 32 Schritten enthalten. Alle Wavetable-Instrumente teilen sich denselben Satz von Wellen. Auf diesem Bildschirm bearbeitest du sie.

Die AY-Lautstärkestufen sind nichtlinear, daher wird die Wellenform der Standard-Ansicht passend zu den tatsächlichen Ausgangspegeln gezeichnet. Tippe zweimal auf **OPT**, um zwischen der Standard-Ansicht und der gespeicherten **LFO-Ansicht** umzuschalten, mit demselben Kürzel wie bei Song/Live Mode: Die LFO-Ansicht zeichnet die rohen Schritte `0`-`15` linear, mit der Mittellinie zwischen `7` und `8` als Modulations-Nullpunkt.

Der Bildschirm hat 2 logische Zeilen. Die obere Zeile enthält die Schaltflächen **Load** und **Save**, die 2. den Wavetable-Editor.

#### Steuerung

- **OPT + [LEFT/RIGHT]**: durch die Wavetable-Liste gehen
- **OPT + [UP/DOWN]**: in 16er-Schritten durch die Liste gehen
- **SHIFT + OPT**: die Wavetable kopieren
- **SHIFT + EDIT**: die Wavetable einfügen
- **OPT + PLAY**: zwischen Standard- und LFO-Ansicht umschalten

## 9. Tracker FX

Spur-FX erzwingen Werte des Sequenzers oder der Klang-Engine in einzelnen Schritten. Andere Grooveboxen nennen diese oft „Parameter Locks“. Jede Phrase-Zeile hat 3 FX-Slots.

Jeder FX hat einen Befehl aus 3 Buchstaben und einen hexadezimalen Wert. Das in der App eingebaute Hilfefenster gibt eine kurze Beschreibung des gewählten Befehls.

### Tonleiter-Automation (Scale)

`SCL XY` ändert die globale Wiedergabe-Tonleiter und den Grundton, ohne die in Phrases gespeicherten Noten zu ändern. `X` wählt eine Tonleiter (`0` Chromatic, `1` Major, `2` Minor, bis `C` Custom); `Y` wählt den Grundton (`0` C bis `B` B). Der Befehl gilt nur für Phrase-FX, nicht für Tabellen. Geben mehrere Spuren im selben Tick `SCL` aus, gewinnt die Spur mit der niedrigsten Nummer.

`SCL` gehört zum Quantizer-Modus. Bei aktivem **Note Lock** wird der Befehl in der FX-Liste nicht angeboten, und jedes bereits in Phrases geschriebene `SCL` wird ignoriert, solange dieser Modus aktiv ist.

### Akkorde (Chords)

`CRD XY` verwandelt die Note in seiner eigenen Phrase-Zeile in einen Akkord. `Y` wählt: `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9 oder `F` Min9. `X` wählt eines von 16 Voicings: `0-3` sind ein aufsteigender Zyklus enger Umkehrungen; `4-7` fügen Drop-2 hinzu; `8-B` fügen Drop-3 hinzu; und `C-F` kombinieren beide Drops. Der Zyklus setzt sich bei Dreiklängen und Power-Akkorden durch eine zusätzliche Oktave fort, sodass jeder Wert verschieden ist; größere Werte werden zunehmend weiter und unkonventioneller. Zum Beispiel ist `CRD 4Y` Grundstellung in Drop-2 und `CRD DY` die erste Umkehrung mit beiden Drops. Am unteren oder oberen Ende der Tonhöhentabelle werden die Stimmen auf deren Bereich begrenzt. Der Befehl ist nicht dauerhaft: Schreibe ihn in jede Zeile mit Akkord. Jede erzeugte Note wird unabhängig quantisiert, wenn Scale aktiv ist, sowohl im Quantizer- als auch im Note-Lock-Modus. Er ist nur für Software-Engines verfügbar; AY-Instrumente ignorieren ihn und bleiben monophon.

### Sequencer-FX

| FX | Wert | Detailliertes Verhalten |
|---|---|---|
| `ARP` | `XY` | Arpeggiert die Grundnote, `+X`-Schritte und `+Y`-Schritte. `37` erzeugt ein Moll-Akkordmuster. |
| `ARC` | `XY` | `X` wählt die Arpeggio-Richtung oder den Bereichsmodus; `Y` ist seine Geschwindigkeit in Ticks. |
| `PVB` | `XY` | Tonhöhen-Vibrato: `X` ist die Geschwindigkeit und `Y` die Tiefe. Im Linear-Modus verwendet die Tiefe Schritte von `10 Cent`. |
| `PBN` | `XX` mit Vorzeichen | Addiert `XX` Tonhöheneinheiten in jeder Phrase-/Tabellenzeile; `FF` bedeutet `-1`. Mit `00` stoppen. |
| `PSL` | `XX` Ticks | Gleitet von der vorherigen Tonhöhe zur neuen Note über `XX` Ticks. |
| `PIT` | `XX` mit Vorzeichen | Akkumulierter relativer Offset in Schritten der Tonhöhentabelle. |
| `FIN` | `XX` mit Vorzeichen | Akkumulierter Fein-Offset in Cent bei Linear-Pitch, sonst in Periodeneinheiten. |
| `PRD` | `XX` mit Vorzeichen | Akkumulierter relativer Offset der Oszillatorperiode. |
| `VOL` | `XX` mit Vorzeichen | Akkumulierter relativer Lautstärke-Offset (`FF` ist -1, nicht volle Lautstärke). |
| `VSL` | `XX` mit Vorzeichen | Addiert `XX` zur Lautstärke in jeder Phrase-/Tabellenzeile. Mit `00` stoppen. |
| `PAN` | `00-FF` | Absolutes Instrument-Panorama: links, Mitte (`80`), rechts. |
| `TPN` | `00-FF` | Absolutes Panorama der Mixer-Spur: links, Mitte (`80`), rechts. |
| `RET` | `XY` | Löst alle `Y` Ticks neu aus; `X` wendet eine Lautstärkeänderung an. `Y=0` stoppt das Neuauslösen. |
| `DEL` | `XX` Ticks | Verzögert Note-on. Eine Verzögerung, die länger als der aktuelle Groove-Schritt ist, überspringt die Note. |
| `OFF` | `XX` Ticks | Sendet Note-off nach `XX` Ticks und tritt in eine ADSR-Release-Phase ein. |
| `KIL` | `XX` Ticks | Beendet die Stimme nach `XX` Ticks hart, ohne das ADSR-Release auszuführen. |
| `TIC` | `XX` Ticks | Legt die Tabellen-Ticks pro Zeile fest. In einer Tabelle ändert es die Geschwindigkeit dieser FX-Spalte. |
| `TBL` | `00-FE`, `FF` aus | Ersetzt und startet die Instrumenttabelle sofort neu; `FF` stoppt sie. |
| `TBX` | `00-FE`, `FF` aus | Startet oder ersetzt sofort eine Hilfstabelle; `FF` stoppt sie. |
| `THO` | Zeile `XX` | Springt mit allen Spalten der Instrumenttabelle zu Zeile `XX`. |
| `TXH` | Zeile `XX` | Springt mit allen Spalten der Hilfstabelle zu Zeile `XX`; wird nicht innerhalb einer Tabelle verwendet. |
| `GRV` | Groove `XX` | Wählt einen Groove für die aktuelle Spur. |
| `GGR` | Groove `XX` | Wählt einen Groove für jede Spur. |
| `HOP` | `XY` | Springt `X`-mal zu Zeile `Y`; `X=0` schleift endlos. Das HOP einer Tabelle wirkt auf die eigene Spalte. |
| `SNG` | `XX` mit Vorzeichen | Verschiebt die Wiedergabe um `XX` Song-Zeilen. |
| `PRO` | `00-64` | Wertet eine absolute Auslösewahrscheinlichkeit von `0%` bis `100%` aus. |
| `MOD` | `AB` | Löst im Durchlauf `A` eines Zyklus mit `B` Durchläufen aus. |
| `SPD` | `XX` mit Vorzeichen | Wählt ein dauerhaftes Taktverhältnis pro Spur; siehe die Geschwindigkeitstabelle unten. |
| `SLE` | `00-FF` Ticks | Legt die dauerhafte Glide-Zeit pro Spur für kontinuierliche Engine-FX fest. `00` ist sofort. Die Einstellung kehrt beim Stoppen der Wiedergabe auf `00` zurück. |

### Audio-FX-Sends (Audio FX sends)

| FX | Wert | Detailliertes Verhalten |
|---|---|---|
| `RSN` | `00-FF` | Legt den Reverb-Send dieser Spur bis zum nächsten Note-Trigger fest. |
| `DSN` | `00-FF` | Legt den Delay-Send dieser Spur bis zum nächsten Note-Trigger fest. |

### ADSR- / Trigger-FX

| FX | Wert | Detailliertes Verhalten |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Überschreiben Attack, Decay, Sustain, Release oder Hüllkurvenform. |
| `TDC` / `TCL` | `00-FF` | Überschreiben Decay oder Color im Trigger-Modus bei Plaits und Plaits-Alt. |

#### Bedingungen

Inspiriert von schwedischen „Trig Conditions“ unterstützt der Tracker Auslösewahrscheinlichkeiten und Modulo-Bedingungen.

| FX | Wert | Bedeutung |
|---|---|---|
| `PRO` | `00-64` | Absolute Auslösewahrscheinlichkeit von `0%` bis `100%` |
| `MOD` | `AB` | Löst bei Iteration `A` von `B` aus; zum Beispiel `12`, `22`, `14`, `34` |

`MOD`-Zähler sind lokal für Spur und Phrase. Ungültige Kombinationen, etwa `A` größer als `B`, lösen nicht aus.

`MOD34` löst im 3. Durchlauf jedes 4-Durchlauf-Zyklus aus. `MOD1F` löst im 1. Durchlauf jedes 16-Durchlauf-Zyklus aus.

#### Wiedergabegeschwindigkeit

Wie NerdSEQ unterstützt ChooChooTracker eine unabhängige Wiedergabegeschwindigkeit für jede Spur.

`SPD` liest seinen Wert als Byte mit Vorzeichen. `00` ist die normale Geschwindigkeit, positive Werte machen die Spur schneller, negative langsamer. Die Einstellung bleibt aktiv, bis ein anderer `SPD`-Befehl sie ändert.

| Wert | Geschwindigkeit |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

Ältere Projekte ohne Unterstützung für `SPD` mit Vorzeichen behalten ihre ursprüngliche Zuordnung `00-10`, sodass sie weiterhin so klingen, wie sie gespeichert wurden.

Sehr hohe Multiplikatoren können die Auflösung des Tick-Schedulers des Trackers überschreiten und wurden nicht auf Hardware getestet.

### Modulations-FX (Modulation FX)

Phrase- und Tabellen-FX können einen Modulations-Slot ändern, ohne das Instrument zu bearbeiten:

| FX-Muster | Bedeutung |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Relativer Amount-Offset für die Modulations-Slots 1-4 |
| `M11`, `M12`, `M13`, `M14` | Relative P1-P4-Offsets für Modulations-Slot 1 |
| `M21`, `M22`, `M23`, `M24` | Relative P1-P4-Offsets für Modulations-Slot 2 |
| `M31`, `M32`, `M33`, `M34` | Relative P1-P4-Offsets für Modulations-Slot 3 |
| `M41`, `M42`, `M43`, `M44` | Relative P1-P4-Offsets für Modulations-Slot 4 |

Der Wert wird als relative `8-Bit`-Änderung mit Vorzeichen interpretiert (`01` addiert `1`, `FF` subtrahiert `1`). Wiederholte Befehle akkumulieren. Effektive Werte werden auf ihren gültigen Bereich begrenzt.

`P5` ist nicht als Phrase- oder Tabellen-FX verfügbar. Es steht als Modulationsziel `M1–M4 Wavetable` zur Verfügung, sodass ein Modulator den AYWavetable-Index eines anderen abtasten kann.

| Modulationstyp | P1 | P2 | P3 | P4 | P5, wenn Shape Wavetable ist |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Ungenutzt |
| AHD | Attack | Hold | Decay | Ungenutzt | Ungenutzt |
| LFO | Shape | Trigger mode | Period | Ungenutzt | AY-Wavetable-Index |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | AY-Wavetable-Index |
| FLFO | Shape | Trigger mode | Frequency (`1 Hz` bis `20 kHz`) | Ungenutzt | AY-Wavetable-Index |

### Braids FX

| FX | Wert | Bedeutung |
|---|---|---|
| `BMD` | `00-2E` | Braids-Modell |
| `BTM` | `00-FF` | Absolutes normalisiertes Timbre |
| `BCL` | `00-FF` | Absolute normalisierte Color |
| `BCF` | `00-FF` | Exponentielle Grenzfrequenz, `20 Hz` bis `20 kHz` |
| `BRS` | `00-FF` | Exponentielle Filterresonanz |

### aChChid FX

| FX | Wert | Bedeutung |
|---|---|---|
| `ASL` | `00-FF` | Gleitet zur Note, ohne neu auszulösen. `00` sind `60 ms`; höhere Werte verlängern den Glide. |
| `ADC` | `00-FF` | Decay-Überschreibung: `200 ms` bis `2 s`. |
| `AAC` | `00-FF` | Akzentstärke: von keiner bis zum Maximum. |
| `ATM` | `00-FF` | Überschreibung des Braids-Timbres; nur im Braids-Wellenmodus aktiv. |
| `ACL` | `00-FF` | Überschreibung der Braids-Color; nur im Braids-Wellenmodus aktiv. |
| `ACF` | `00-FF` | Exponentielle Grenzfrequenz des 303-Filters, `20 Hz` bis `20 kHz`. |
| `ARS` | `00-FF` | Resonanz des 303-Filters, von keiner bis zum Maximum. |
| `AEM` | `00-FF` | Hüllkurvenmodulation des 303-Filters, von keiner bis zum Maximum. |

### Bogie FX

| FX | Wert | Bedeutung |
|---|---|---|
| `DMD` | `00-0B` | Wählt das Bogie-Modell bis zum nächsten Trigger. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Überschreiben Decay, Tone, Sweep, Noise, FM oder Drive bis zum nächsten Trigger. Alle sechs Makros sind bei jedem Modell verfügbar. |
| `DCF` | `00-FF` | Filter-Grenzfrequenz, logarithmisch abgebildet. |
| `DRS` | `00-FF` | Filterresonanz. |

### Sintered FX

| FX | Wert | Bedeutung |
|---|---|---|
| `SMD` | `00-05` | Wählt das Sintered-Modell bis zum nächsten Trigger. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Überschreiben Decay, Mod, die ersten beiden Modell-Makros, Motion oder das letzte Modell-Makro bis zum nächsten Trigger. |
| `SCF`, `SRS` | `00-FF` | Filter-Grenzfrequenz und -Resonanz. |

### MIDI Out FX

| FX | Wert | Bedeutung |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Sendet ein MIDI-CC, auf `0-127` umskaliert. An welche CC-Nummer jeder Slot sendet, wird pro Instrument festgelegt (`MC1 number`-`MC4 number` auf dem Instrumentbildschirm), nicht durch den FX selbst - ein Slot ohne gesetzte Nummer ist wirkungslos. |

### Plaits FX

| FX | Wert | Bedeutung |
|---|---|---|
| `PMD` | `00-17` | Plaits-Engine |
| `PHA` | `00-FF` | Absolute normalisierte Harmonics |
| `PTM` | `00-FF` | Absolutes normalisiertes Timbre |
| `PMO` | `00-FF` | Absoluter normalisierter Morph |
| `PAX` | `00-FF` | Main/Aux-Mischung: `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Exponentielle Grenzfrequenz, `20 Hz` bis `20 kHz` |
| `PRS` | `00-FF` | Exponentielle Filterresonanz |

### Sample FX

| FX | Wert | Bedeutung |
|---|---|---|
| `SPL` | `00-03` | Wiedergabemodus: `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Spielt den nummerierten Slice (`01` = erster Slice) unabhängig von der Note; `00` behält die normale Notenzuordnung bei. Erfordert einen aktivierten Slice-Modus am Instrument |
| `SPT` | `XX` mit Vorzeichen | Sample-Transposition in Halbtönen |
| `SST` | `00-FF` | Normalisierter Wiedergabestart |
| `STA` | `00-FF` | Alias von `SST` (normalisierter Wiedergabestart) |
| `SEN` | `00-FF` | Normalisiertes Wiedergabeende |
| `SVL` | `00-FF` | Absolute Sample-Lautstärke |
| `SCF` | `00-FF` | Exponentielle Grenzfrequenz, `20 Hz` bis `20 kHz` |
| `SRS` | `00-FF` | Exponentielle Filterresonanz |
| `SSP` | `00-FF` | Sample-Geschwindigkeit, abgebildet von `0%` bis `500%` |

### Von AY-Instrumenten gemeinsam genutzte AY-FX

| FX | Wert | Detailliertes Verhalten |
|---|---|---|
| `AYM` | `XY` | `X` ist die Hüllkurvenform; `Y` wählt off/tone/noise/tone+noise (`0/1/2/3`). |
| `NOI` | `XX` mit Vorzeichen | Akkumulierter relativer Offset der Rauschperiode. |
| `NOA` | `00-1F`, `FF` | Absolute Rauschperiode; `FF` überlässt die Priorität der Rauschperiode früheren Spuren. |
| `ERT` | beliebig | Löst die aktuelle Hardware-Hüllkurvenform neu aus. |
| `EAU` | `XY` | Automatisches Hüllkurvenverhältnis `X:Y`; `X=0` schaltet es aus. |

### Nur für AY Classic geltende FX

| FX | Wert | Detailliertes Verhalten |
|---|---|---|
| `EVB` | `XY` | Vibrato der Hüllkurvenperiode: `X` Geschwindigkeit, `Y` Tiefe. |
| `EBN` | `XX` mit Vorzeichen | Addiert `XX` zur Hüllkurvenperiode in jeder Phrase-/Tabellenzeile. |
| `ESL` | `XX` Ticks | Gleitet von der vorherigen Hüllkurvenperiode zum neuen Wert. |
| `ENT` | Note `XX` | Setzt die Hüllkurvenperiode anhand der in der Oberfläche angezeigten Note der Tonhöhentabelle. |
| `EPT` | `XX` mit Vorzeichen | Akkumulierter relativer Offset der Hüllkurvenperiode. |
| `EPL` | Byte `XX` | Setzt das niederwertige Byte der Hüllkurvenperiode. |
| `EPH` | Byte `XX` | Setzt das höherwertige Byte der Hüllkurvenperiode. |

### AY Plus FX

| FX | Wert | Detailliertes Verhalten |
|---|---|---|
| `TNN` | Note `XX` | Setzt den Ton-Oszillator auf eine Note der Tonhöhentabelle und ignoriert die Zeilennote. |
| `TNP` / `TNF` | `XX` mit Vorzeichen | Akkumulierter Ton-Offset in Tonhöhenschritten / fein. |
| `TRT` | beliebig | Löst die Phase des Ton-Oszillators neu aus, wo unterstützt. |
| `ENN` | Note `XX` | Setzt den Hüllkurven-Oszillator auf eine Note der Tonhöhentabelle. |
| `ENP` / `ENF` | `XX` mit Vorzeichen | Akkumulierter Hüllkurven-Offset in Tonhöhenschritten / fein. |
| `SFT` | `00-07` | Software-Typ: None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | Note `XX` | Setzt den Software-Oszillator auf eine Note der Tonhöhentabelle. |
| `SFP` / `SFF` | `XX` mit Vorzeichen | Akkumulierter Offset des Software-Oszillators in Tonhöhenschritten / fein. |
| `SRT` | beliebig | Setzt die Phase des Software-Oszillators zurück. |
| `SFM` | `XX` mit Vorzeichen | Akkumulierter Offset der FM-Tiefe. |
| `PWM` | `XX` mit Vorzeichen | Akkumulierter Offset der Pulsbreite. |
| `SPL` | `XX` mit Vorzeichen | Akkumulierter Offset des unteren Pulspegels. |
| `SWT` | `XX` mit Vorzeichen | Akkumulierter Offset des Wavetable-Index. |

### AY Sample FX

AY Sample akzeptiert die gemeinsamen AY-Befehle sowie `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` und `SFF`, wie oben beschrieben.

| FX | Wert | Detailliertes Verhalten |
|---|---|---|
| `SMS` | `00-FF` | Setzt den Legacy-Wiedergabestart von AY Sample auf `XX x 64` Quell-Samples. |

Engine-Parameter-FX gelten für den passenden Instrumenttyp und werden beim nächsten Note-Trigger zurückgesetzt, sofern nicht anders angegeben. `SPD` ist absichtlich dauerhaft.

Die FX-Auswahl filtert diese Referenz auf gängige Befehle plus die vom aktiven Instrument unterstützte Gruppe. Das kontextbezogene Hilfefenster liefert beim Bearbeiten Details zu den Werten.

## 10. Mixer, Reverb und Delay

Der Mixer ist der am weitesten links liegende Hauptbildschirm. Er zeigt die CPU-Last und Übersteuerungswarnungen. Ein rotes `!` in der Spalte **CLIP** kennzeichnet die Spur, deren Direktsignal den Mix über den sicheren Bereich hinausgetrieben hat.

Damit gleichst du die 8 Spuren aus, formst jede Spur mit dem Tilt EQ und schickst Audio an den Reverb oder das Delay:

| Feld | Bedeutung |
|---|---|
| LVL | Spurpegel nach der Engine, `000-100` |
| REV | Send an den gemeinsamen Clouds-Reverb, `000-100` |
| DLY | Send an das gemeinsame Ping-Pong-Delay, `000-100` |
| TLT | Tilt EQ pro Spur, `00-FF`; `80` ist neutral, niedrige Werte betonen die Bässe, hohe Werte die Höhen. Niedrige Werte bringen eine sanfte Übersteuerung mit. |
| M | `*` schaltet diese Spur stumm |
| S | `*` schaltet diese Spur auf Solo |

Der Mixer ist spurbasiert, nicht instrumentbasiert. Wechselt eine Spur das Instrument, bleiben ihr Pegel, PAN, Tilt und ihre Sends an der Spur. Instrumente haben außerdem ihre eigenen **Vol** und **Pan** von `00-FF` in der oberen Zeile des Instrument-Bildschirms. Das Instrument-PAN wird zuerst angewendet, dann das Spur-PAN. Beide erhalten ein bestehendes Stereobild in der Mitte; sie dämpfen die gegenüberliegende Seite zunehmend, wenn sie nach links oder rechts bewegt werden.

### Master FX

Der Eintrag **MASTER FX** öffnet ein Insert-FX-Untermenü mit zwei Slots für den gesamten
Mix. Seine Effekte laufen nach den Reverb- und Delay-Returns, vor der Master-
Lautstärke. Wähle ein Modul, bearbeite seine Parameter oder schalte es in Bypass, genau wie bei einem
Spur-Insert; drücke **OPT**, um zum Mixer zurückzukehren. Master-FX-Einstellungen werden
mit dem Projekt gespeichert und nicht durch Phrase, Tabelle oder Modulation automatisiert.

### Clouds Reverb

Verwende **SELECT + [UP]** im Mixer, um den Reverb-Einstellungsbildschirm zu öffnen.

- **Return**: Pegel des Reverb-Effektanteils im Master-Mix.
- **Time**: Steuerung von Ausklang/Zeit, `00-FF`.
- **Damping**: Absorption hoher Frequenzen, `00-FF`.
- **Filter**: Tiefpassfilter vor dem Reverb.

Dies ist der Reverb-Teil von Clouds, nicht der vollständige granulare Clouds-Prozessor.

### Ping-pong Delay

Verwende **SELECT + [DOWN]** im Mixer, um Delay zu öffnen, und dann **SELECT + [UP]**, um zurückzukehren. Reverb arbeitet in umgekehrter Richtung: **SELECT + [UP]** öffnet ihn und **SELECT + [DOWN]** kehrt zum Mixer zurück.

- **Return**: Pegel des Delay-Effektanteils.
- **To Reverb**: Anteil des Delay-Effektsignals, der in den gemeinsamen Reverb geschickt wird, `0-100%`.
- **Ticks**: Delay-Zeit in Tracker-Ticks (`9 ticks` = `1 Schlag` standardmäßig).
- **Feedback**: Betrag der Kreuzrückkopplung, begrenzt auf `95%`.
- **Filter**: Tiefpassfilter im Pfad des verzögerten Signals.

Die 1. Wiederholung folgt dem Stereoeingang. Spätere Rückkopplung wechselt zwischen dem linken und dem rechten Kanal.

## 11. Project-Bildschirm

Der Project-Bildschirm bietet die Befehle **Load**, **Save**, **New**, **Export**, **Manage** und **Scale** sowie Metadaten zu Dateiname, Titel und Autor.

### Exportort (Export location)

Exporte, Stems und Bounces werden in einen projektspezifischen Ordner innerhalb des bereits vorhandenen Sample-Verzeichnisses der Anwendung geschrieben:

- **Standard**: `<app folder>/samples/Exports/<project name>/`. Solange das Projekt keinen Namen hat, heißt der Ordner `current-project`.
- In der Webversion lautet der Standard `/user/samples/Exports/<project name>/`.
- Unter Android liegt der Sample-Ordner im privaten Arbeitsbereich der Anwendung.

Die Zeile **Folder** auf dem EXPORT-Bildschirm zeigt das aktive Ziel:

- Tippe auf **EDIT**, um mit dem Ordner-Browser einen eigenen Ordner zu wählen. Der gewählte Ordner wird genau wie ausgewählt verwendet (es wird kein Projekt-Unterordner angelegt) und in den Einstellungen gemerkt.
- **EDIT + OPT** (clear) setzt den Standardort zurück.

Wird ein benanntes Projekt unter neuem Namen gespeichert, wird der Standard-Exportordner entsprechend umbenannt, sodass bereits exportierte Dateien beim Projekt bleiben. Das Umbenennen gilt nicht für einen eigenen Ordner, und der Ordner wird nicht durch die automatische Speicherung umbenannt. Existiert bereits ein Ordner mit dem Zielnamen, bleiben beide Ordner erhalten. Hat der Tracker den Ordner aus den Augen verloren (zum Beispiel nach einem Neustart), benennt das Speichern eines benannten Projekts stattdessen den vorhandenen Ordner `current-project` um. Instrumente funktionieren nach einer Umbenennung weiter: Geladene Sample-Pfade, die in den umbenannten Ordner zeigen, werden automatisch aktualisiert.

**Load** akzeptiert auch `.mid`/`.midi`-Dateien, die als neues Projekt importiert werden: Noten werden nach MIDI-Kanal gruppiert (ein Kanal pro Spur, bis zur Spurzahl), auf 4 Zeilen pro Schlag quantisiert und auf ein einziges Standard-AY-Instrument gelegt - MIDI-Programmnummern haben kein Chiptune-Gegenstück, wähle also danach echte Instrumente. Es wird nur das erste Tempo der Datei verwendet (eine globale Tickrate, keine Tempowechsel pro Abschnitt). Die Zeile **MIDI** des Export-Bildschirms macht das Gegenteil: Sie schreibt die Noten, die Lautstärke und das Tempo/den Groove des aktuellen Arrangements als Standard-MIDI-Datei (eine MIDI-Spur pro Tracker-Spur); [MIDI Out](#midi-out)-Instrumente und andere FX pro Zeile über Lautstärke und den globalen Groove hinaus haben kein MIDI-Gegenstück und werden nicht übersetzt.

**Load** akzeptiert auch Dirtywave-M8-Songs (`.m8s`, Firmware 2.x-4.x), die als neues Projekt importiert werden. Es kommen nur die Struktur und die Noten mit (Song-Zeilen, Chains mit ihrer Transposition, Phrases mit Note, Velocity und Instrumentnummer, Note-offs, Tempo und Songname); Noten behalten ihre tatsächliche Tonhöhe (der M8 benennt Oktaven zwei höher: sein `C-6` ist hier `C-4`). Jedes in einer Phrase verwendete Instrument wird unter dem Namen des M8-Instruments importiert. M8-**MACROSYN**-Instrumente werden zu Braids-Instrumenten (die 47 Formen sind die Braids-Modelle, mit denselben Nummern; Timbre, Color, Filtertyp, Grenzfrequenz, Resonanz und Pan werden übernommen, wobei die Grenzfrequenzkurve ungefähr ist). **SAMPLER**-Instrumente werden zu Sample-Instrumenten: Die `.m8s` speichert nur den Pfad der WAV auf der M8-Karte, sodass die Datei unter dem Ordner der `.m8s` und deren übergeordneten Ordnern gesucht wird (eine Kopie der Kartenstruktur funktioniert) sowie in deinem aktuellen Sample-Ordner (`Samples` oder `samples` funktionieren beide); wird sie nicht gefunden, behält das Instrument den M8-Pfad und bleibt stumm. Importierte Samples erhalten +24 Halbtöne in ihrer Pitch-Einstellung, da der M8 ein Sample mit seiner Originaltonhöhe zwei Oktaven tiefer abspielt als diese Anwendung. **FMSYNTH** wird zu einem Genesis-FM-Instrument (grobe Näherung), **WAVSYNTH** und **HYPERSYNTH** werden zu aChChid. **MIDI OUT** wird zu einem MIDI-Out-Instrument (Kanal, Bank, Programm und die ersten vier CC-Nummern). Andere (externe) Typen werden zu einem Standard-AY-Instrument - wähle danach echte Klänge. FX und Tabellen werden ignoriert.

Auf dem Desktop macht die Zeile **M8S** des Export-Bildschirms das Gegenteil, eingeschränkt: Tippe darauf und wähle eine vorhandene `.m8s` als **Vorlage** (bleibt bis zum Beenden gemerkt, **EDIT + OPT** vergisst sie). Ihr Song, ihre Chains, Phrases, ihr Tempo und ihr Titel werden ersetzt, und alles andere bleibt unverändert, außer dass Braids-Instrumente als MacroSynth und Sample-Instrumente als Sampler zurückgeschrieben werden (Pfad `/Samples/<file name>`: kopiere die WAV selbst dorthin auf dem M8), sofern die Vorlage ein Instrument dieses Typs hat, von dem die übrigen Parameter kopiert werden können. FX werden nicht exportiert, und ein Song mit Phrase `FF` oder höher kann nicht exportiert werden (der M8 hat 255 Phrases). Die exportierte Datei wurde noch nicht auf einem echten M8 getestet. Einzelheiten siehe [M8 songs](../m8s-format.md).

Auf dem Desktop ermöglicht Key Jazz, Dateiname, Titel und Autor direkt auf der Tastatur statt über die virtuelle Bildschirmtastatur einzugeben - siehe [Key jazz](#key-jazz-nur-desktop).

### Scale / Quantize

Der Scale-Bildschirm steuert die globale 12-TET-Tonleiter-Engine. Die Zeile **Mode** wählt zwischen zwei Modi:

- **Quantizer** (Standard): Die Phrase-Eingabe bleibt chromatisch; wenn aktiviert, werden Noten unmittelbar vor dem Erreichen der Klang-Engine abgerundet, sodass das Ändern von Grundton oder Tonleiter einen Song neu harmonisiert, ohne seine Phrases zu bearbeiten.
- **Note Lock**: In Phrases eingegebene Noten werden bei der Eingabe auf die nächste Note der Tonleiter eingerastet, sodass nur Tonleiternoten geschrieben werden können. Neue Eingaben und Verringerungen rasten nach unten ein; `EDIT + RIGHT` / `EDIT + UP` heben die Note auf die nächste Tonleiternote darüber. Die Eingabe folgt auf diesem Bildschirm immer der Projekt-Tonleiter: Das Ändern von Grundton oder Tonleiter wirkt sich sofort auf neu eingegebene Noten auf den aktivierten Spuren aus, während bereits gespeicherte Noten unberührt bleiben. Der FX `SCL` ist in diesem Modus nicht verfügbar, und jedes bereits geschriebene `SCL` wird ignoriert. Die Wiedergabe-Quantisierung einfacher Noten entfällt (die Eingabe ist bereits gesperrt); die Akkord-Quantisierung über `CRD` wird weiterhin angewendet.

Solange der Cursor auf **Mode** ruht, hält die Statusleiste eine feste Beschreibung des aktiven Modus sichtbar: `Quantizes notes on playback` für **Quantizer**, `Snaps sequencer to scale` für **Note Lock**. Das Umschalten des Modus tauscht den Text sofort; jede Aktionsmeldung ersetzt ihn, bis sie abläuft.

Beide Modi teilen sich denselben Grundton, dieselbe Tonleiter, die **Custom**-Notenbearbeitung und die Spur-Kontrollkästchen. Jedes der acht Spur-Kontrollkästchen legt fest, für welche Spuren der Modus gilt. Die Tonleiter-Engine entfällt bei Tonhöhentabellen ohne 12 Noten und bei Sampler-Instrumenten, deren **Slice**-Modus EQUAL, AUTO oder LAZY ist (ihre Noten wählen Slices chromatisch ab C-0); MIDI-Eingabe ist nicht Teil dieser Version.

- **Linear pitch** wählt den Modus der Tonhöhentabelle. **Off** ist der Standard und die auf Hardware validierte Einstellung für korrektes Oktav-Tracking von AY, Braids und Plaits.
- **Tick rate** legt das Tracker-Timing fest und zeigt die entsprechenden BPM (`tick rate x 60 / 24`).
- ChooChooTracker speichert Projekte als `.cct`. Dieses Format ist nicht mit ChipNomad kompatibel.

Verwende **Save**, bevor du Instrumenttypen änderst oder ein anderes Projekt lädst.

## 12. Settings

- **Repeat delay / speed** stellen die Wiederholung gehaltener Tasten ein.
- **Stick live mode** wählt `HOLD`, `TOGGLE` oder `FREE` für die bereits zugewiesene Stick-live-Taste.
- **MIDI** öffnet das Untermenü [MIDI](#14-midi): Geräteauswahl und Zuordnung der MIDI-In-Kanäle zu Instrumenten.
- **Synths** enthält **AY Quality**, **Sample dithering** und die globalen Einstellungen **Braids BITS / DRFT / SIGN**.
- **Mixer** enthält **Mix volume** und den projektspezifischen **Tilt pivot** von `250-4000 Hz` (Standard `1 kHz`), den alle Tilt-Regler des Mixers verwenden.
- **Graphics** enthält **Edit color theme**, **Load font**, **Persistent waveform**, **Track visuals**, **Waveform FPS** und den aktiven Renderer (`GPU` oder `Software`). **Waveform FPS** legt die Aktualisierungsrate des Instrument-Scopes und der Mixer-Visualisierungen von `01` bis `60` fest. Track visuals wählt **Detailed** (die Synthesizer-Wellenform und ihre Überlagerungen) oder **Audio waveform** (das tatsächliche summierte Ausgangssignal dieser Spur) unabhängig für jede Spur; die Audio-Wellenform löscht beim Start der Wiedergabe sofort jede frühere ADSR-Überlagerung. ChipNomad-Schriften und -Themes sollten funktionieren.
- **Key mapping** passt die Steuerung an.
- **Support report** schreibt eine kleine diagnostische Textdatei mit Anwendungsversion, Plattform, Audioeinstellungen und CPU-Last, Wiedergabe-/Überlaufzustand und einer kompakten Projektzusammenfassung. Sie enthält keine Sample-Daten und keinen Titel-/Autorentext des Projekts. Unter Android und Web öffnet sie den normalen Export-/Download-Ablauf; Desktop- und Handheld-Builds speichern `support-report.txt` im beschreibbaren Ordner der Anwendung.
- **Quit ChooChooTracker** beendet sauber.

**Graphics → Boot screen** wählt **Original** (Standard) oder **Night**. Tippe auf EDIT
oder verwende EDIT + Richtung, um zu wechseln; die Wahl wird für den nächsten
Titelbildschirm gespeichert. Night verwendet die originale Eisenbahnszene mit weicheren blaugrauen Tönen,
eine prozedurale Milchstraße mit dezentem Sternflimmern und gelegentlichen Sternschnuppen.
Landschaft und Zug des Originals behalten ihre Scroll-Schleifen.

## 13. Leistung und Fehlerbehebung

### CPU

Die CPU-Kosten hängen von den aktiven Engines und Effekten ab. Physikalische Plaits-Modelle und der Clouds Reverb sind rechenintensiver als einfache AY-Stimmen. Prüfe die Leistung auf der Zielkonsole, besonders mit 8 Plaits-Stimmen und beiden aktiven Sends. Eine CPU-Anzeige nahe `100%` kann Knistern oder verpasste Audio-Deadlines verursachen.

Die CPU-Anzeige ist ein geglättetes Maß der Audio-Renderzeit. Sie misst keine Verzögerungen vor dem Ausführen des Audio-Callbacks oder im Audioausgang des Geräts, sodass Knistern auch bei niedriger Anzeige auftreten kann.

Unter Android bevorzugt die Audioausgabe jetzt AAudio, um das zeitweilige Knistern zu vermeiden, das mit OpenSL ES auf dem Pixel 7a beobachtet wurde. Diese gepufferte Ausgabe stellt eine stabile Wiedergabe in den Vordergrund und kann eine gewisse Reaktionslatenz hinzufügen. Die Tracker-Fläche behält ihr volles 4:3-Layout auf quadratischen und anderen ungewöhnlichen Seitenverhältnissen; ungenutzter Platz wird als Rand angezeigt, statt Spalten abzuschneiden. Wenn das Knistern anhält, melde den Song, das Telefonmodell, die Android-Version und ob du Lautsprecher, Kopfhörer oder Bluetooth verwendest.

Wenn Knackser, Abstürze oder Verlangsamungen auftreten, verwende **Settings > Support report** und hänge die erzeugte `support-report.txt` an den Fehlerbericht an. Löst ein bestimmter Song das Problem aus, hänge auch diese `.cct` an.

### Kein Ton

Prüfe die Instrumentnummer, den Stummschalt- oder Solo-Zustand der Spur, das LVL der Spur, das Mix volume der Anwendung und die Hüllkurve des Instruments. Prüfe bei Samples, ob die ursprüngliche WAV noch unter ihrem gespeicherten Pfad existiert. Drücke unter ArkOS **MENU + L3**, um die Stummschaltung des Betriebssystems umzuschalten. Hilft das nicht, starte die Hardware neu und verbinde dein Audio-Interface erneut.

### Die Steuerung fühlt sich falsch an

Passe Repeat delay und Repeat speed in Settings an. Wenn ein einzelner Druck zweimal bewegt, prüfe, dass dieser Richtung nur 1 physisches Bedienelement zugeordnet ist, und melde den genauen Bildschirm und das genaue Kürzel.

## 14. MIDI

Desktop und PortMaster. Settings > **MIDI** öffnet dieses Untermenü: **MIDI In** / **MIDI Out** wählen ein angeschlossenes Gerät, wobei durch die erkannten Ports geschaltet wird, mit `OFF` an beiden Enden; beide Richtungen benötigen ein gewähltes Gerät. Die Geräteauswahl gilt pro Sitzung: Sie wird nicht in settings.txt gespeichert, da sich die Position eines Ports in der Liste zwischen Neustarts ändern kann oder wenn Geräte in anderer Reihenfolge angeschlossen werden. Das Bedienelement **PLAY** der Anwendung bleibt von diesem Bildschirm aus verfügbar und startet den Song. Verbinde unter PortMaster vor dem Start ein USB-MIDI-Gerät; es muss vom Betriebssystem des Handhelds über ALSA bereitgestellt werden.

- **Klänge über ein MIDI-Keyboard vorhören**: Ist ein MIDI-In-Gerät gewählt, hört man durch Spielen von Noten auf dem angeschlossenen Keyboard auf jedem Bildschirm ein Instrument vor, genau wie mit dem Bildschirmkürzel **EDIT + PLAY** auf dem [Instrument](#5-instruments)-Bildschirm. Das ist nur Vorhören - es gibt keine Noten in den Song ein.
- **Channel mapping**: öffnet eine Liste der 16 MIDI-Kanäle; ordne jedem ein Instrument zu (standardmäßig `OFF`), sodass auf diesem Kanal empfangene Noten dieses Instrument vorhören, unabhängig davon, welches auf dem Instrument-Bildschirm gewählt ist - z. B. Kanal `01` -> Instrument `05`. Ein auf `OFF` belassener Kanal fällt auf das aktuell gewählte Instrument zurück, das ursprüngliche Verhalten. Diese Zuordnung wird in settings.txt gespeichert.
- **MIDI CC mapping**: ordnet einen eingehenden Controller einem Instrumentparameter zu, einschließlich seines **Volume**, oder globalen Bedienelementen. Globale Zeilen zeigen `TRK 1`-`TRK 8` statt eines Instruments und bieten pro Spur **Mute**, **Solo**, **Volume**, **Reverb send** und **Delay send**; **Song play/stop** hat kein Ziel. Eine neue Zuordnung ignoriert das CC, das sie gelernt/konfiguriert hat, und wird bei der nächsten physischen Bewegung wirksam, sodass das Einrichten von Zuordnungen während der Wiedergabe einen Patch nicht auf null zurücksetzen kann. Mute und Solo verwenden `0-63` = off, `64-127` = on.
- **Ein externes MIDI-Gerät ansteuern**: Gib einer Spur den Instrumenttyp [MIDI Out](#midi-out) und stelle ihren Channel ein; das Auslösen von Noten auf dieser Spur sendet echtes MIDI an das gewählte MIDI-Out-Gerät, statt in ChooChooTracker Klang zu erzeugen.

## 15. Danksagungen und Lizenzen

ChooChooTracker ist ein Fork von ChipNomad und behält dessen MIT-Lizenzierungsansatz bei. Der Code von Braids, Plaits, Plaits-Alt, Clouds DSP, des von Warps abgeleiteten MME-DSP und von stmlib ist von den Open-Source-Veröffentlichungen von Mutable Instruments unter ihren jeweils geltenden MIT-Hinweisen abgeleitet. Plaits-Alt stammt aus dem Plaits-Lab-Fork von lylepmills/eurorack; die dort beibehaltenen Quellcode-Hinweise gelten. Die aChChid-Engine verwendet Open303 von Robin Schmidt, Copyright 2009, unter der MIT-Lizenz. Bogie und Sintered sind eigene native Implementierungen. Die MIDI-E/A verwendet RtMidi von Gary P. Scavone unter ihrer MIT-ähnlichen Lizenz. Die genauen Zuschreibungen findest du in den mitgelieferten Lizenzdateien.

## Spur-Insert-Effekte (Track insert effects)

Jede der acht Spuren hat zwei serielle Slots, **TF1 → TF2**. Öffne von Instrument aus
mit dem üblichen Shift+Hoch MOD und dann erneut mit Shift+Hoch **F: Insert
FX**. Shift+Runter kehrt zu MOD zurück. OPT+Links/Rechts wählt die Spur (1-8), ohne
die Instrumentauswahl zu ändern. Die Modulkopfzeile öffnet die bestehende Auswahl;
Bypass behält das Modul, die konfigurierten Werte und die Automation bei. Die Parameter sind
Bytewerte: linke Spalte 1-4, rechte Spalte 5-8. Der Kontexttext zeigt vollständige Namen und
dekodierte Werte. Alte Projekte laden mit beiden Slots auf OFF.

Die Navigationsanzeige hält die vollständige Spalte F-M-I-P auf allen
Instrument-Bildschirmen sichtbar, einschließlich Insert FX und Instrument Pool, und hebt
den aktuellen Bildschirm hervor.

| Modul | Parameter der Reihe nach |
| --- | --- |
| Work Compressor | Threshold, Attack, Release, Makeup, Ratio, Detector source, Detector filter, Mix |
| Airwindows Distortion | Input, Mode, Output, Mix |
| Airwindows StereoDoubler | Detune, Mix |
| TAPESCAM | Input, Drive, Color, Wobble, Tone, Output |
| OTT (Rui-727) | Depth, Time, Upward, Downward, Input, Output |
| Chorus | Rate, Depth, Tone, Mix |
| Flanger | Rate, Depth, Feedback, Mix |
| Phaser | Rate, Depth, Feedback, Mix |
| Rotary | Speed, Depth, Drive, Mix |
| Saturation | Drive, Tone, Level, Mix |
| Bitcrusher | Bit depth, Rate reduction, Tone, Mix |
| Destruction | Mode, Amount, Tone, Mix |
| Parametric EQ | Low gain, Low frequency, Mid gain, Mid frequency, High gain, High frequency |

Kontinuierliche Regler verwenden 00-FF, einschließlich einer gültigen Null und des Maximums FF. Bipolare
Regler haben ein exaktes Neutral bei 80. Die Distortion-Modi sind 00 Density, 01 Drive,
02 Spiral, 03 Mojo, 04 Dyno. Die Compressor-Verhältnisse 00-07 sind 1,5, 2, 3, 4, 6, 8, 16,
20:1; die Detektorquellen 00-02 sind Stereoeingang, links, rechts. Detector filter 80 ist
neutral, darunter ist es ein Tiefpass, darüber ein Hochpass. Ein externer Sidechain oder
Sidechain zwischen Spuren wird nicht bereitgestellt. Jeder Slot kann jedes Modul enthalten; wiederholte Module sind unabhängig.
Chorus, Flanger, Phaser und Rotary verwenden sanfte Modulationsstandardwerte. Rotary Speed
verwendet den vollen Bereich **00-FF**: **00 = 0,01 Hz** (eine Umdrehung pro 100 Sekunden),
**FF = 25 Hz** (das frühere Chaos-Maximum). Exponentielle Abstufung ermöglicht eine feinere
Kontrolle langsamer LFO-Bewegung. Neue Rotary-Inserts stehen standardmäßig auf **8F**, etwa
0,8 Hz. Das ersetzt die vier diskreten Geschwindigkeitswerte; bestehende Rotary-Einstellungen
und die F11/F21-Geschwindigkeitsautomation müssen auf die neue Skala angepasst werden (die alten Slow/Fast/Hyper/
Chaos entsprechen ungefähr **8F/CB/E7/FF**). Die übrigen Parameter bleiben unverändert.
Die Bittiefe des Bitcrusher reicht von 4 bis 16 Bit und
die Ratenreduktion von 1x bis 32x. Die Destruction-Modi sind Fold, Clip und Crush.
Saturation und Distortion enthalten Regler für Ausgangspegel oder Mix, um sie gegen das
unbearbeitete Signal abzugleichen.
Der Parametric EQ hat 24-dB-Boost/Cut-Shelves bei 60-500 Hz und 2-14 kHz sowie eine
Glocke mit fester Güte 0,9 bei 250 Hz-5 kHz. Seine Gain-Regler sind bei 80 neutral; sind alle
drei neutral, ist er ein exakter Bypass. Der aktive EQ verwendet ein transparentes Soft Knee oberhalb der
Vollaussteuerung und erreicht ein Maximum von 1,5, um extreme Anhebungen einzudämmen.

Spurpegel und Tilt EQ liegen vor den Inserts, sodass beide das Signal beeinflussen, das in
Kompression und Verzerrung eingeht. Alle Stimmen/Akkordnoten der Spur speisen eine einzige
Insert-Kette. Reverb- und Delay-Sends erhalten beide ihren Ausgang. Gemeinsame Effekte, Returns,
Master-Regler und Mute/Solo behalten ihr bestehendes Verhalten. Inserts
verarbeiten kein Audio externer MIDI-Geräte.

Insert-Befehle verwenden **F11-F18** für TF1 und **F21-F28** für TF2. Die FX-Auswahl
des Rolls und das Phrase-/Table-Popup bieten nur Parameter an, die von den konfigurierten Inserts
der gewählten Spur verwendet werden. Das Popup gruppiert sie als **TF1: Effektname** und
**TF2: Effektname**; OFF-Slots sind ausgeblendet. Inserts im Bypass bleiben bearbeitbar.
Im FX-Popup von Phrase und Table folgt die Beschreibung dem Insert-Slot der
gewählten Spur: Sie zeigt das Modul, den Parameternamen, die dekodierten Bereichsgrenzen und
den gespeicherten Basiswert. Bestehende Song-Befehle bleiben erhalten, wenn ein Modulwechsel sie
unbenutzt macht; beim Öffnen ihrer Auswahl wird bei einem verfügbaren Regler begonnen.
Beim Bearbeiten eines Werts zeigt der Hinweis unten seinen dekodierten Wert und die Einheiten.
Jede FX-Spalte von Phrase oder Table kann beide Slots ansprechen; das Phrase-Raster behält seine
drei Spalten. Diese absoluten Laufzeitwerte bleiben über Noten, Instrumente,
Phrases, Chains und gewöhnliche Schleifen hinweg bestehen. Neue Wiedergabe/harter Stopp setzt sie zurück. Das Bearbeiten
eines Basiswerts auf der Seite F löscht nur die Überschreibung dieses Parameters. MOD wirkt auf den
effektiven Basis-/Automationswert, ohne seine Offsets zu speichern. Die Wahl eines neuen
Moduls stellt die Standardwerte wieder her und löscht die Überschreibungen dieses Slots. Bypass behält
die bestehende Automation bei und setzt die Verarbeitung nach einer kurzen Ausblendung aus.

**Adressen beziehen sich auf Spur-Slots und Parameterpositionen, nicht auf benannte Effekte.** Eine
auf einer anderen Spur wiederverwendete Phrase verwendet die Module dieser Spur. Ein Modulwechsel kann
die Bedeutung eines Befehls ändern; Befehle werden nicht umgeschrieben. OFF-/ungenutzte Adressen
sind unbedenkliche No-ops und bleiben in gespeicherten Phrase-/Tabellendaten erhalten.

Die MOD-Auswahl des Instruments fügt eine Kategorie Insert FX hinzu. Die Quellen jedes Instruments
steuern die Inserts der Spur, die es spielt, einschließlich des kombinierten Akkordsignals.
Dasselbe Instrument auf zwei Spuren beeinflusst unabhängige Ketten. Es gelten das bestehende Quellen-Timing,
vorzeichenbehaftete Beträge, Regeln zur Kombination von Quellen, Stick-Modi und die Bewegungs-
aufzeichnung. Diskrete Regler werden quantisiert und auf gültige Auswahlmöglichkeiten begrenzt. Es gibt keine
neue, einer Spur gehörende Modulationsbank.

Das Compressor-Makeup beginnt bei 0 dB. OTT beginnt mit etwa 25 % Depth, 25 %
Upward, 50 % Downward, 0 dB Eingang und -3 dB Ausgang. Seine Bandschwellen, Verstärkungen und
Übergangsfrequenzen bleiben bei den nativen Standardwerten von Rui-727; Depth ist sein Wet/Dry-Regler.
TAPESCAM behält native Hilfsstandardwerte bei: neues/schnelles Band, Rauschen und
Kompression aus, Verbreiterung an. Beobachte die vorhandenen Übersteuerungsanzeigen, wenn du
Drive/Gain hinzufügst. Slot-Übergänge verwenden begrenzte 5-ms-Überblendungen über das Direktsignal.

Speicherstände enthalten Modul-IDs, Bypass und konfigurierte Bytes in den optionalen
versionierten Abschnitten `Track inserts: 1,8,2` und `Master inserts: 1,2`. Laufzeit-Überschreibungen
und DSP-Verläufe werden nicht gespeichert.
Lege Sicherungen an, bevor du Speicherstände mit Inserts in älteren Builds öffnest. Zuschreibung des DSP,
Quellrevisionen und Anpassungen stehen in `chipnomad_lib/external/insert_fx/SOURCES.txt`;
die vollständigen MIT-Hinweise werden in `licenses/INSERT_FX.txt` mitgeliefert.

Auf einem R36H bei 48 kHz benötigte ein gemessenes Paar Inserts auf einer Spur etwa 4-5 %
der Echtzeit für Compressor, 6 % für Distortion, 18 % für Doubler, 16 % für TAPESCAM
und 24 % für OTT, vor den Kosten der Synthesizer und gemeinsamen Effekte. Sechzehn aktive
Instanzen von Doubler, TAPESCAM oder OTT überschritten die Echtzeit. Modulation im Audiobereich verursacht
weitere Kosten. Beginne mit wenigen Inserts, beobachte die Audio-Überlastung und schalte
ungenutzte Slots in Bypass; die sechzehn verfügbaren Positionen sind kein garantiertes CPU-Budget.

Die Insert-Seite hält den Hinweis des gewählten Felds nach dem Loslassen der Taste sichtbar;
vorübergehende Hinweise können ihn aber dennoch ersetzen. Die Modulauswahl zeigt für jedes
Modul den Effekttyp und das Quellprojekt neben seinem Namen.

### Native OPLL- und VRC7-Instrumente (Entwicklung)

Die Instrument-Type-Auswahl hat eine FM-Gruppe mit **OPLL / MSX** (YM2413)
und **VRC7** (DS1001). Jede bietet jetzt **73 Programme** in scrollbaren Bank-/Preset-
Listen: die ursprünglichen 15 Klänge, 40 zusätzliche unterschiedliche Klänge aus den
YM2413/VRC7/YMF281B-Tabellen von emu2413 und 18 für ChooChoo geschriebene Zwei-Operator-Programme.
Exakte Duplikate von Klang-Bytes werden innerhalb jeder Engine ausgelassen. Die zusätzlichen
Paletten verwenden den programmierbaren Klang-Slot des Chips; sie erweitern nicht das physische
ROM. Programm null bezeichnet einen eigenen Klang. EDIT + links/rechts wählt das
vorherige oder nächste Preset innerhalb des Bank-Filters. Fine ct stellt die Stimmung von
-100 bis +100 Cent ein.

In der Programmliste hört EDIT + PLAY den markierten Klang vor; lass die
Tasten los, um zu stoppen. EDIT bestätigt, OPT bricht ab. Durchsuchen/Vorhören ändert weder
das Instrument noch die Tabelle noch den Song. Die normale Vorhör-Geste der Instrument-Seite
funktioniert nach der Auswahl weiter. Native Chip-Hüllkurven liefern Attack/Release;
die optionale Amp env fügt eine Software-ADSR hinzu (siehe Native Chip-Regler weiter unten).
Es gibt keinen vollständigen Operator-Editor.

Neue Instrumente speichern alle acht nativen Klang-Bytes, Bank/Name, Programm und Feinstimmung in
der Song-/Instrumentdatei. Dateien mit diesen Typen verwenden das Format 7.0 und benötigen
diesen Build oder einen neueren. Frühere Formate bleiben lesbar; Songs ohne diese Typen
werden weiterhin als 5.0 gespeichert. Rhythmus-Programme sind zurückgestellt. Die erweiterte Bibliothek ist
auf dem Host validiert; Hörprüfung durch Menschen und eine neue Handheld-Prüfung stehen noch aus.

### AdLib / OPL2 und OPL3 (Entwicklung)

FM umfasst auch AdLib / OPL2 (YM3812) und OPL3 (YMF262). Bank filtert die
Werksliste; Preset öffnet Kategoriegruppen und eine Ansicht All. EDIT + PLAY hört vor der
Auswahl vor, EDIT wählt aus, OPT bricht ab, und EDIT + links/rechts auf Preset
lädt den vorherigen/nächsten passenden Eintrag. Fine ct stellt die lokale Stimmung ein. Mode zeigt
2 Operatoren, 4 Operatoren oder Dual voice. OPL2 blendet inkompatible OPL3-Patches aus;
OPL3 kann die gemeinsame Zwei-Operator-Sammlung spielen.

Die Titel der Bank- und Preset-Popups nennen die aktuelle Engine. Das Wechseln der Engine oder des
Instrument-Slots setzt den Bank-Filter auf **All banks** zurück, auch beim Wechsel
zwischen den kompatiblen Engines AdLib und OPL3. Die Rückkehr aus einem Popup innerhalb desselben
Slots behält den gewählten Filter bei. Das Abbrechen eines DX7-Bank-Imports stellt den
vorherigen Filter wieder her.

Die Preset-Bestätigung veröffentlicht auch beim Loslassen der Taste vorgenommene Änderungen zum nächsten
UI-/Audio-Tick. Spielende Songs brauchen keinen weiteren Tastendruck und keinen Transport-
Neustart mehr, um diesen Patch zu erhalten. Die Ausgabe folgt weiterhin dem konfigurierten Audiopuffer
und dem Hüllkurven-/Retrigger-Verhalten der Engine.

Die Werkssammlung enthält 697 Quelleinträge aus The Fat Man 2-op,
The Fat Man 4-op und DMXOPL3, mit 589 normalisierten eindeutigen Patch-Identitäten.
Aliase behalten Quellnamen und Zuschreibung. Native Operatorpegel der Quelle, Flags für
die LFO-Tiefe, Notenversätze, feste Percussion-Tonhöhen und die Stimmung von Dual-Voice
bleiben erhalten. Die Tracker-Lautstärke verwendet Software-Amplitude; sie bildet nicht
die Lautstärkekurven oder Velocity-Offset-Regeln der ursprünglichen MIDI-Player nach. Schätzungen
der Release-Dauer aus der Quelle schneiden gehaltene Noten nicht ab. Einige Effekte haben
langsame Attacks: Halte Seashore, statt zu erwarten, dass ein kurzer Klick ihn offenbart.

Werksressourcen liegen in `instruments/chips` neben der bestehenden Instrumentenbibliothek,
mit Hinweisen unter `licenses/chip-banks`. Die Preset-Auswahl lässt die Tabelle des
Slots und die spureigenen Inserts unverändert. Neu gespeicherte Dateien verwenden 7.0 und
betten den vollständigen Klang ein. Werksaudio wird auf dem Host und auf ARM64 maschinell getestet; die Hörprüfung
durch Menschen steht noch aus.

### Native Sega-PSG- und Game-Boy-Instrumente (Entwicklung)

CHIP enthält jetzt Sega PSG, GB Pulse und GB Noise. Sega verwendet den NTSC-Mastertakt
und seine 16-Bit-Rauschrückkopplung, einschließlich des vom Tonkanal abgeleiteten Rauschens.
Der Standard **Bass range: Extended** senkt bei Bedarf den virtuellen Takt, um
unterhalb der Teilergrenze des Chips von etwa 109 Hz zu spielen. So bleiben A-2, G-2
und F-2 unterscheidbar. Wähle **Chip** für den ursprünglichen Bereich, in dem tiefere Noten
an der Teilergrenze zusammenfallen. Rauschen mit fester Rate behält den ursprünglichen Takt;
vom Ton abgeleitetes Rauschen folgt der erweiterten Tonhöhe. Alte Sega-Instrumente laden mit
aktiviertem Extended; die Einstellung wird mit dem Instrument gespeichert.
Die GB-Instrumente verwenden DMG-Puls-/Rauschregister; ein Game-Boy-Wave-Kanal wird nicht
bereitgestellt. Ihre selbst komponierten Preset-Listen enthalten jetzt **24, 24 und 28** Presets
jeweils. Die Ergänzungen decken verbundenes/periodisches Sega-Rauschen, GB-Tonhöhen-Sweeps,
kurze Percussion, metallisches Rauschen, Drones und Anstiege ab. Es sind eigene Programme,
keine aus Spielen extrahierten Klänge.
Die Seite bietet native Regler für Modus/Duty/Breite, Hüllkurve/Sweep/Rauschen,
soweit anwendbar, sowie eine Software-Amplituden-ADSR. Diese werden in nativen Dateien der Version 6 gespeichert.
Automatisierte ARM64-Leistungs-/Audioprüfungen bestehen innerhalb der angegebenen Lastgrenzen;
die Hörprüfung durch Menschen steht noch aus.

### DX7 FM (Entwicklung)

Wähle **Type → FM → DX7 FM**; DX7 ist der siebte Eintrag, nach Arcade / YM2151.
DX7 FM verwendet einen MSFA-Kern mit sechs Operatoren. Die bestehende FM-Seite bietet
Bank, Preset und Fine ct neben den gemeinsamen Instrumenteinstellungen. Das Durchsuchen von Presets
verwendet Bank-/Kategorielisten. Halte EDIT+PLAY, um den markierten Klang zu hören; Loslassen
beendet das Vorhören. EDIT allein bestätigt beim Loslassen; OPT bricht ab. Das Laden eines Klangs
kopiert seinen vollständigen Patch, ohne Tracker-Tabellen oder Spur-Insert-FX zu ändern.

Der Werkskatalog enthält derzeit **67 verschiedene DX7-Parameter-Patches**:
31 originale musikalische Klänge von OpenDX7, vier einzigartige YSE-CC0-Klänge (seine 32 Bank-Slots
wiederholen diese vier unter anderen Namen) und 32 für ChooChoo geschriebene Patches.
„Original“ bedeutet hier für dieses Projekt erstellte Parameterprogramme; es bedeutet nicht
originale Werksinhalte von Yamaha. YSE wird als vier Klänge ausgeliefert, nicht als 32
künstlich verschiedene Namen.
Kategorien beschreiben die geprüfte klanggestalterische Absicht. Mehrdeutige Namen bleiben
Unsorted. Numerische Spielbarkeitsprüfungen sind bestanden; die Abnahme per Gehör steht
noch aus. Das davon getrennte Ziel von 1.000 weitergabefreien Klängen ist nicht erreicht.

Für eine dauerhafte persönliche Bibliothek lege `.syx`-Dateien in
**`instruments/banks/dx7/`** neben der bestehenden Instrumentenbibliothek ab. Unterordner
werden unterstützt. Öffne **DX7 → Bank**: Jede Datei erscheint als benannte Bank, bereit zum
Durchsuchen in Preset. Öffne Bank erneut, nachdem du Dateien hinzugefügt oder entfernt hast; eine Konvertierung oder der
Schritt Load Instrument ist nicht nötig. Eine standardmäßige originale DX7/TX7-Bank enthält
**32 Stimmen**. Eine Datei mit vier Bank-Nachrichten erscheint als vier nummerierte Banken
(insgesamt 128 Stimmen). Einzelstimmen-Dateien werden ebenfalls akzeptiert. Die native Anwendung
liest diesen Ordner; die Browserversion liest ihr virtuelles Dateisystem, nicht beliebige
Ordner auf dem Computer.

Fehlerhafte oder nicht unterstützte Dateien werden mit einer Anzeige ihrer Anzahl auf dem Bildschirm übersprungen; gültige Banken bleiben
verfügbar. Das Scannen ist begrenzt auf 2.048 Dateien, 1 MiB pro Datei, 64 MiB insgesamt,
60.000 Stimmen und acht Ebenen verschachtelter Ordner. Symbolische Links werden ignoriert. Der
gewählte Patch gehört dem Instrument: Das Entfernen seiner Quellbank kann den gespeicherten Song
nicht ändern. Die vollständige Durchsuchungsbibliothek bleibt in diesem Ordner und
wird nicht pauschal in jedes Projekt kopiert.

LOAD INSTRUMENT akzeptiert auch `.syx`-Dumps originaler DX7/TX7-Einzelstimmen und 32-Stimmen-Banken,
einschließlich begrenzter Folgen dieser unterstützten Nachrichten. Es prüft
Rahmung, Byteanzahl, Sieben-Bit-Daten, Prüfsummen, Parameterbereiche und Dateigröße,
bevor die importierte Bank im selben FM-Browser geöffnet wird. Die Auswahl eines Patches
bestätigt; Abbrechen behält das aktuelle Song-Instrument. Diese Direktimport-Abkürzung
behält ihre Durchsuchungsliste für die Sitzung; verwende den obigen Bank-Ordner für dauerhaftes
Durchsuchen. Speichere den Song oder eine `.cni`, um gewählte/bearbeitete Patches zu behalten. Dumps ohne Header,
falsche Prüfsummen, DX7II-Performances/-Erweiterungen und andere Yamaha-Familien werden
abgelehnt. Der Import sendet nie MIDI-Nachrichten an externe Geräte.

CNI und Projektversion 6 speichern alle 155 originalen Stimmen-Bytes, den vollständigen angezeigten
Namen, die native Anschlagstärke, die Feinstimmung und die Quellidentität. Songs brauchen keine
externe Bank zum erneuten Öffnen. Ältere ChooChoo-Versionen können diese Dateien
der Version 7 nicht lesen; Projekte ohne sie werden weiterhin als Version 5 gespeichert. Die native Velocity beträgt standardmäßig
100; die Tracker-Lautstärke ist eine Verstärkung nach der Synthese und löst die Hüllkurve nicht neu aus.
Operator-Hüllkurven, Festfrequenzmodus, Keyboard Scaling, Tonhöhenhüllkurve und
LFO bleiben natives Patch-Verhalten. Die optionale gemeinsame FM-Amplituden-ADSR formt
ihren kombinierten Ausgang, ohne die Operator-Hüllkurven zu ersetzen.

Jeder Spurteil hat einen LFO und vier separat gehörende Akkordstimmen-Slots, passend zur
bestehenden Akkord-/Ersetzungsrichtlinie des Trackers. Spuren teilen sich keinen Notenzustand.
Note-off lässt die nativen Hüllkurven los; cut/panic löscht sie. Der skalare MSFA-Kern
verwendet 64-Sample-Quanten bei einer festen internen Rate von 44,1 kHz, mit einem Streaming-FIR zur
Ausgaberate. Nachschlagetabellen werden einmalig außerhalb des Audio-Renderings initialisiert, sodass
gleichzeitige Offline- und Live-Renderer sich nicht gegenseitig die Rate ändern können. Ereignisse
werden im nächsten internen Quantum wirksam (bis zu 1,45 ms), gefolgt von der Gruppenlaufzeit des FIR
von etwa 0,25 ms. Gepufferte Samples bleiben über Callbacks hinweg erhalten.
Dies ist die MSFA-Modern-Linie, keine Behauptung bitgenauer Übereinstimmung mit DX7-Hardware oder einer
Dexed-Mark-I-Emulation. DX7 ist auf **16 aktive Noten im gesamten Song** begrenzt,
einschließlich Release-Ausklängen, wobei vier eigene Akkord-Slots pro Spur erhalten bleiben.
Bei Überschreitung des Budgets werden zuerst Release-Ausklänge entfernt, dann die leisesten gehaltenen Noten,
dann frische Attacks. Bei gleichen Attacks bleiben Grundnoten über Spuren hinweg vor Akkord-
erweiterungen erhalten, mit stabiler Entscheidung nach Slot/Spur bei Gleichstand. Diese Regel gilt auf allen
Plattformen, sodass derselbe Song dieselbe begrenzte Notenzuteilung hat. Sie
ändert nicht die Polyphonie anderer Instrumente. Das Vorhören von Presets ist während der Wiedergabe deaktiviert.
Die Grenze wurde aus Messungen auf dem R36H gewählt und bei gemischter Wiedergabe validiert.

#### FM-Operator-Editor

Genesis FM, Arcade FM, OPL2, OPL3, OPLL und VRC7 Instrumente haben eine Schaltfläche **EDIT**
rechts von Fine ct. Sie öffnet eine Tabelle der Patch-Parameter:

- Genesis FM / Arcade FM: algorithm, feedback, pan, AMS, PMS, Operatormaske und
  LFO-Einstellungen, dann MUL, DT, TL, KS, AR, D1R, D2R, RR und D1L für die vier
  Operatoren, plus SSG (Genesis) oder DT2 und AM (Arcade).
- OPL2 / OPL3: feedback, connection und pan pro Stimme, deep vibrato und tremolo,
  dann MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS und KSR pro
  Operator. Bei OPL3 kann hier der Modus (2op, 4op, dual) geändert werden.
- OPLL / VRC7: die Register des eigenen Klangs (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Das Bearbeiten eines ROM-Presets macht es zu
  „Custom“.

Links/Rechts und die üblichen Bearbeitungstasten ändern Werte; das Instrument spielt weiter
wie gewohnt, sodass du Änderungen hörst. Die Statusleiste beschreibt den Parameter unter
dem Cursor. OPT oder Left+Shift kehrt zum Instrument-Bildschirm zurück. DX7 und SID
haben keinen Operator-Editor.

Genesis FM (YM2612) und Arcade FM (YM2151) verwenden jetzt denselben FM-Bank-/Preset-Browser,
dasselbe EDIT+PLAY-Vorhören, dieselbe Bestätigung/Abbruch und dieselben Feinstimmungsregler. Genesis hat jetzt
**73 Presets**: die ursprünglichen 24 und 49 unterstützte melodische Programme aus der CC0-
Bank 16-Bit FM Music Station von NeoSoundFonts. Arcade hat **81 Presets**: die
ursprünglichen 24 und 57 unterstützte, klingende Programme aus der GPLv3-Sammlung von YMulator-Synth.
Das ursprüngliche ChooChoo-Paar teilt sich seine zugrunde liegenden Rezepte; die neuen
Quellsammlungen liefern getrennte Paletten. Dies sind benannte Klanggestaltungs-Sammlungen,
keine behaupteten Nachbildungen bestimmter Spiel-Soundtracks. Nicht unterstützte
Notenversätze, feste Percussion-Tasten, Arcade-Rauscheinstellungen und stumme Quellprogramme
sind als Ausschlüsse in `expansion-manifest.json` festgehalten. Ihre vollständigen Vier-Operator-Patch-, Hüllkurven-,
LFO-, Stereo- und Stimmungseinstellungen werden in Instrumenten und Songs der Version 6 mitgeführt.
Die native Chip-Hüllkurve steuert das Release; normale Tracker-Verstärkung und -Tonhöhe
starten sie nicht neu. Der Genesis-DAC-Ausgang verwendet einen 20-Hz-Gleichspannungsfilter. Der Rauschmodus und
der Sonderfrequenzmodus von Kanal 3 liegen außerhalb dieser Instrumentimplementierung.

Für eine lokale externe Bank verwende `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`. Es akzeptiert striktes
42-Byte-TFI, VOPM-OPM-Text, das verifizierte WOPLX-Format und originales DX7-SysEx.
Lade die entstandenen `.cni`-Dateien über Load Instrument. Benutzerimporte werden
getrennt von der verteilbaren Werksbibliothek gehalten. OPM-Dateien mit aktiviertem Rauschen,
Rauschfrequenzdaten ungleich null oder teilweisem Panning werden mit einer Erklärung abgelehnt;
VOPM-Pan-Werte 0/64/127 werden zu links/beide/rechts. Binäres WOPL wird nicht unterstützt.
Der gemeinsame FM-Katalog enthält **1.064 Einträge**: 697 OPL, 67 DX7, 146
OPLL/VRC7, 73 Genesis und 81 Arcade. Weitere 76 Dateien stellen die Sega-/Game-Boy-
Presets über den normalen Datei-Browser bereit, insgesamt **1.140 mitgelieferte native Presets**.
Quellrevisionen, Hashes, vollständige Hinweise und ursprüngliche Quelldaten begleiten die
neuen Sammlungen unter `licenses/chip-banks/expansion`.

### Portable Werkssammlungen und USER-Presets

Native Chip-Instrumente verwenden **Bank:**, um ALL, eine mitgelieferte Sammlung oder
USER zu wählen. **Preset:** wählt einen Klang innerhalb dieser Auswahl. Halte EDIT und drücke
Links oder Rechts in einer der beiden Zeilen, um durchzuschalten; ein kurzes Tippen auf EDIT öffnet seine Auswahl.

Mitgelieferte Sammlungen sind ZIP-Pakete unter `instruments/FACTORY/`. USER-Dateien kommen
unter `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` oder `gb-noise`.
Durchsuche die Ordner direkt oder lege dort ein ZIP kompatibler Presets ab. ZIPs bleiben
auf dem Datenträger unverändert. Alle diese Ordner akzeptieren kompatible `.cni`-Dateien; CNI ist
das portable Instrumentformat von ChooChoo/ChipNomad, kein Standard eines Chip-Herstellers.
Der USER-Browser liest außerdem die folgenden Quellformate, auch innerhalb von ZIPs:

| USER-Ordner | Zusätzliche Formate |
| --- | --- |
| `dx7` | Yamaha DX7/TX7 `.syx`, Einzelstimmen und 32-Stimmen-Banken |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | VOPM-`.opm`-Banken, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | eigene Furnace-`.fui`-Patches und feste ROM-Auswahlen |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

Eine Bank öffnet sich in kompatible Stimmen. Nicht unterstützte Dateien oder Funktionen melden
einen Grund; inkompatible Stimmen in einer gemischten Bank werden ausgeschlossen, während ihre Quell-
nummern stabil bleiben. Das Laden ist transaktional: Ein abgelehntes Preset lässt das
aktuelle Instrument unverändert. Quell-Presets ersetzen die Tracker-Tabelle des Instruments
durch eine leere, außer DX7-SysEx, das seine bestehende Tabelle beibehält.
CNI stellt seine gespeicherte Tabelle wieder her. Geladene Patches gehören dem Song und funktionieren
weiter, nachdem das Quellpaket verschoben oder gelöscht wurde.

WOPL-Banken können ungenutzte Slots ohne ausdrückliches Leer-Flag enthalten. Der Browser
lässt unbenannte Slots aus, deren aktive Operatoren vollständig gedämpft sind und die kein
Attack haben. Unbenannte spielbare Stimmen bleiben als `Program N [Bbank]` oder
`Drum N [Bbank]` verfügbar, anhand der bei null beginnenden Programm- und Banknummern der Quelle.
Ein unbenanntes einzelnes OPLI-Instrument verwendet seinen Dateinamen. Benannte stumme Patches
bleiben erhalten.

Die Furnace-Unterstützung umfasst die älteren Einzelinstrument-Versionen 29-112 und die funktionsbasierten
`FINS`-Versionen 127-251. Die DefleMask-Unterstützung umfasst Instrumente der Version 11 für OPN, OPM, Sega PSG
und Game Boy. Dies sind Instrumentimporte, keine vollständigen Tracker-
Player: animierte FM-/Operator-Makros, FM-LFO-Einstellungen des Quellsongs, Samples,
Game-Boy-Hardware-Befehlsfolgen, relative SID-Pulse-/Filter-Makros und SID-
Ring-/Sync-Abhängigkeiten werden derzeit abgelehnt. Die konstante OPLL-ROM-Auswahl wird
unterstützt; VRC7 verwendet für diese Auswahl sein eigenes ROM. SID-Wizard-`.swi`- und SID-
Musikdateien (`.sid`) sind in diesem Build keine Instrumentimporte.

Unterstützte PSG-/GB-/SID-Folgen werden im Patch gespeichert und mit 60 Hz für
Furnace/DefleMask und 50 Hz für GoatTracker abgespielt. Diese Dateien enthalten nicht die vollständige
Wiedergabekonfiguration eines Songs: Verwende den ursprünglichen Tracker, wenn seine songspezifische
Tickrate, Kanalzuordnung, sein Hard-Restart-Verhalten oder seine Kompatibilitätseinstellungen
erforderlich sind. Bei Sega PSG wählt ein Duty-Makro die Rauschstimme; ohne eines verwendet der Import
eine Tonstimme. Game-Boy-Dateien müssen im vorgesehenen Pulse- oder
Noise-Ordner liegen. Importierte Folgen bleiben vorerst eingebettete Wiedergabedaten; die Instrument-
Seite bearbeitet Basisparameter, nicht die Tabellen des Quell-Trackers.

Das Durchschalten der USER-Presets setzt sich über Dateien, Ordner, Banken und ZIP-Pakete in
beide Richtungen fort und springt an den Enden um. Das erneute Öffnen des USER-Browsers folgt der
aktuellen Auswahl. ALL enthält USER-Klänge unter Unsorted für Engines
mit Kategorien; USER bleibt eine eigene Auswahl auf oberster Ebene. Sega PSG und Game Boy verwenden
flache ALL-/Factory-Listen und einen hierarchischen USER-Browser, ohne ein Unsorted-
Fenster. Ältere DX7-Dateien in `instruments/banks/dx7/` bleiben in USER zugänglich.

Eine Engine zeigt nur ihre kompatiblen mitgelieferten Sammlungen. Factory Presets
kombiniert die mitgelieferten OPLL-/VRC7-Klangsätze für die gewählte Engine; DX7 kombiniert
die Originale von ChooChoo und YSE. Heruntergeladene benannte Sammlungen bleiben getrennt.
Fat Man 2-op ist unter OPL2 aufgeführt und Fat Man 4-op unter OPL3. Kompatible OPL2-
CNI-Dateien können weiterhin in OPL3-USER-Ordnern verwendet werden.

Die Ausgabe nativer Engines verwendet eine feste gemessene Verstärkungskompensation statt einer
Normalisierung pro Preset. Bestehende Songs mit nativen Instrumenten können daher
mit anderem Pegel spielen; prüfe die Balance gespeicherter Mixe. Die Kalibriermethode
und die Grenzen sind in `scripts/README_MEASURE.md` dokumentiert.

`native-chip-audition.cct` bietet ein kurzes sequenzielles Vorhören über die ursprünglichen
dreizehn Werksbanken. Jeder Abschnitt verwendet ein anderes eigenes Instrument, sodass es auch bei
entferntem Preset-Ordner funktioniert. Längere, von Git ignorierte Bank-WAVs und gemessene Pegel sind
in `docs/chip-preset-auditions.tsv` aufgeführt; das subjektive Hören steht noch aus.

### Native Chip-Regler (Native chip controls)

Wellenformvorschauen nativer Instrumente werden aktualisiert, wenn ihre Regler neu gezeichnet werden.
FM-Instrumente können ihre optionale Amp-env-Kurve überlagern; SID behält seine Wellenformvorschau
ohne diese nur für FM geltende Überlagerung, auch beim Durchsuchen von Banken und Presets.

Alle sieben FM-Engines bieten **Bright**, **Feedback** und **Amp env**. Bright
reicht von -63 bis +63, wobei null den Patch erhält; es ändert die Pegel der modulierenden
Operatoren und behält die Pegel der Carrier bei. Seine hörbare Wirkung hängt vom
Algorithmus ab. Feedback steht standardmäßig auf **Preset** oder überschreibt das native Feedback
mit 0-7. Diese Regler schreiben die gespeicherten nativen Operator-Bytes nicht um.

Amp env steht standardmäßig auf **Bypass**. Wähle **ADSR**, um Attack, Decay, Sustain,
Release und Form um den nativen Klang zu legen. A/D/R verwenden den gemeinsamen quadratischen Bereich von 0-5 Sekunden;
Sustain reicht von Stille bis zum vollen Pegel. Native Hüllkurven laufen weiterhin,
sodass diese Hüllkurve einen Klang nicht über sein natives Release hinaus verlängern kann. Das Durchsuchen von
Presets behält die FM-Amp- und Klangregler des Slots bei. Alle FM-Stimmen verwenden außerdem
einen 3-ms-Übergang beim Einsatz/Neuauslösen und 1 ms Glättung der Tracker-Verstärkung, auch
wenn Amp env im Bypass ist. Hard cut/panic bleibt unmittelbar.

Die folgenden Phrase-FX erscheinen auch als unterstützte Ziele für Modulation und Bewegungsaufzeichnung.
Werte in der FX-Spalte sind hexadezimal. Die Einstellungen gelten für die Wiedergabe,
ohne das gespeicherte Instrument zu ändern. Native GB-Sweep- und Hüllkurvenregler
rasten beim nächsten Note-Trigger ein; Duty/Breite und Rauschfrequenz können live geändert werden.

| Engines | FX | Regler |
|---|---|---|
| Native FM | `OL1`-`OL6` | Absolute Operator-Ausgangspegel: OPLL/VRC7-Modulator und OPL `00-3F`; OPLL/VRC7-Carrier `00-0F`; Genesis/Arcade `00-7F`; DX7 `00-63` (0-99). Höher bedeutet mehr Ausgang. Nur unterstützte Operatoren erscheinen. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Attack, Decay, Release und Sustain-Dämpfung von Operator 1, `00-0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | Frequenzmultiplikator von Operator 1. |
| Genesis/Arcade | `LFR` | Native LFO-Rate: Genesis `00-07`, Arcade `00-FF`. |
| Arcade | `LAD`, `LPD 00-7F` | Getrennte LFO-Tiefen für Amplitude und Tonhöhe. |
| Genesis/Arcade | `LAS`, `LEN` | Native Amplitudenempfindlichkeit `00-03` und LFO-Aktivierung `00-01`. |
| Genesis/Arcade | `LPS 00-07` | Native Tonhöhenempfindlichkeit. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | Natives Attack, Decay, Sustain und Release. Zeitwerte steigen zu `0F` hin; Sustain steigt zum vollen Pegel hin. |
| SID | `SPR 01-10` | Frequenz des stummen Partners von 1× bis 16×; wirkt auf Ringmodulation und Hard Sync. |
| Alle nativen FM | `FBK 00-07` | Absolutes natives Feedback, vom Instrument initialisiert. |
| Alle nativen FM mit aktivierter Amp env; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack, Decay, Sustain, Release, Form. Sie aktivieren keine im Bypass befindliche FM-Amp. |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | Ton / weißes Rauschen / periodisches Rauschen; drei feste Rauschraten oder tonabgeleitete Rate. |
| GB Pulse | `CMD 00-03` | Nativer Pulse-Duty. |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | Rauschbreite, Taktteiler und Shift. |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | Sweep-Periode, Shift, Abwärtsrichtung. |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | Anfangspegel der nativen Hüllkurve, Periode, Aufwärtsrichtung. |

Die Auswahl nativer FX verwendet das Instrument in der Spalte `I` der Phrase-Zeile oder
das beim Zurückschauen gefundene aktive Instrument, wenn `I` leer ist. Das FX-Popup
zeigt eine kurze Reglerbeschreibung, seinen aktuellen Preset-Wert und den gültigen Befehls-
bereich in einem kompakten Block. Die Wahl eines anderen
nativen Effekts beginnt bei diesem Wert; das erneute Öffnen desselben Effekts behält seinen
bearbeiteten Wert. Eine Auswahl über mehrere Zeilen löst das Instrument jeder Zeile einzeln auf.
Tabellen verwenden den Kontext ihres Instruments. Native Wertänderungen stoppen an ihren
gültigen Endpunkten, einschließlich Duty, Rauschen, Sweep, ADSR, Feedback und Operatorpegeln.
Die Werte sind hexadezimal: Zum Beispiel bedeutet das Maximum `63` von DX7 dezimal 99.

Diese FM-Befehle verwenden absolute native Werte. Operator-Befehle zielen immer auf
Operator 1; `OMU 03` wählt dessen Multiplikator 3. Die Tracker-spezifischen Regler von DX7
sind Operatorpegel und Feedback. Preset/Range behält seine Info-Farbe;
Titel und Beschreibungen folgen denselben Farben wie FM Feedback.

Die Live-Modulation behält ihren vollen Satz nativer Ziele mit festen Operatoren.
Die Bewegungsaufzeichnung schreibt nur Regler, die in der kompakten Tracker-Liste vertreten sind;
Operator-Parameter werden nur für Operator 1 aufgezeichnet. Andere Modulationsziele
funktionieren weiterhin live, erzeugen aber keine Phrase-Befehle.

`SCP` und `SCT` behalten bytes-skalierte Zuordnungen zu SID-Registern, die breiter als 8 Bit sind.
`SMR 01-C8`, `SWV 01-08` und `SPR 01-10` entsprechen jetzt der nativen Preset-Nummerierung.
Angezeigte Preset-Werte sind Grundeinstellungen, unabhängig von der laufenden Hüllkurve
oder dem LFO. `SLE` wirkt auf Operatorpegel und die direkten FM-Parameter, ohne
die Note neu auszulösen oder den gespeicherten Patch zu ändern.

Alle nativen FX haben beschreibende Titel, Bereiche und Verhalten in der Phrase-
FX-Auswahl sowie Werthinweise. `CMD` beschreibt Ton/Rauschen, Duty oder Rauschbreite
je nach gewähltem Instrument. Diese Regler ergänzen die gemeinsamen Gruppen Track,
Envelope und Modulation; die Engine-Gruppe allein ist nicht der vollständige
Satz verfügbarer Phrase-Effekte.

Die bestehenden gemeinsamen LP/BP/HP-Filter von Braids, Plaits, PCM und anderen unterstützten
Engines sind Software-Verarbeitung nach der Synthese. aChChid verwendet stattdessen seinen nativen
303-Filterpfad. Diese Runde fügt weder AY noch nativem FM einen Filter hinzu; Brightness ändert
die FM-Synthese selbst. Spur-Inserts bleiben für zusätzliche Verarbeitung verfügbar.

Native Instrumente werden in CNI Version 7 gespeichert (8, wenn absolute Befehle vorhanden sind),
und native Songs in CCT Version 9. Instrumente mit importierten Folgen
verwenden CNI Version 9 und Songs, die sie enthalten, verwenden CCT Version 10. Ältere Builds
können diese neueren Dateien mit Folgen nicht öffnen. Gespeicherte FM-Preset-Bytes und
Klangeinstellungen der Instrument-Seite bleiben unverändert.

### Handheld-Lastrichtlinien für native Chips

Es gibt acht Song-Spuren und **zwei Insert-Effekt-Slots pro Spur**. Die sechzehn
verfügbaren Slots garantieren nicht genug CPU, um sechzehn Effekte auszuführen.
Das Limit von sechzehn aktiven Noten bei DX7 ist ein separates songweites Synthesebudget;
Akkorde und Release-Ausklänge zählen dazu.

In Messungen auf dem R36H schnitten vier loopende WAV-Spuren, Sega PSG, GB Pulse, DX7 und
OPL3 mit gemeinsamen Sends und vier Inserts (zwei Compressors, Doubler, TAPESCAM)
besser ab als dichte achtspurige FM-Arrangements. Die Version mit einer einzelnen Note
hatte in ihrer dreißigsekündigen Messung bei 48 kHz/512 Frames keine verpassten Render-Deadlines;
das Hinzufügen eines vierstimmigen DX7-Akkords brachte eine Timing-Spitze. Dichte FM-Songs
mit vielen aufwendigen Inserts überschritten das CPU-Budget. Lass etwas Reserve,
beobachte die vorhandene Überlastanzeige und füge Effekte dort hinzu, wo sie dem Song helfen.
Die Anwendung behält jede Spur und jeden Slot; es gibt kein neues hartes Limit für die Zahl der Inserts.
Vollständige Messungen und den Stand der Dauertests siehe `chip-instruments-report.md`.

Der entsprechende 70-Sekunden-Test mit physischem Audio dieses ausgewogenen Akkord-Arrangements
bestand ohne verpasste Render-Deadlines oder protokollierte ALSA-Underruns bei der bestehenden
Einstellung 48 kHz / 4906 Frames. Sein schlechtester Callback lag bei 81,819 ms gegenüber einer
Deadline von 102,208 ms. Die Tests verwendeten eine separate Master-Verstärkung von 0,4 als Headroom.
Behalte die direkte Kartenroute des regulären Launchers bei, `AUDIODEV=plughw:0,0`: Der
gemeinsame Standard-Mixer des Systems erzeugte in den Diagnosetests Underruns. Keine
Audioeinstellung des Nutzers wurde geändert. Der zehnminütige Stresstest mit kleinerem Puffer zeichnete dennoch
28 Timing-Spitzen auf; die vollständigen Ergebnisse stehen im Bericht.

### SID-Instrumente und Phrase-FX

Wähle **CHIP → SID**. Bank bietet **ChooChoo SID Originals** (32 selbst komponierte
Programme) und **SIDkit Effects** (24 MIT-lizenzierte Effekte). Beide verwenden denselben
Preset-Browser und speichern ihr vollständiges gewähltes Programm im Instrument
und im Song. Zum Weitergeben des Songs ist keine externe Bank nötig. Dies sind native
SID-Parameterprogramme mit Hüllkurven und Bewegungsrezepten, kein gesampeltes Audio und keine
vollständigen C64-Songs. Die separaten zwölf GoatTracker-Forschungskandidaten werden
nicht ausgeliefert; ihre Wave-/Pulse-/Filter-Tabellen brauchen einen eigenen Importer/Player.

Die Instrument-Seite bietet Wellenform/Pulsbreite, Filtermodus/Grenzfrequenz/Resonanz
und native ADSR-Regler. SID verwendet den festgelegten digitalen Oszillator, die Hüllkurve und den
zyklusweisen Filter im Stil des 6581 von floooh/chips. Es gibt keine alternativen Chipmodelle,
Revisionsauswahlen oder zusätzlichen Charakterprofile. Dies ist eine generische Näherung,
kein kalibriertes analoges Modell des R2/R3/R4/8580.

Jede Note hat ihren eigenen Filter. Ring/Sync verwenden einen stummen Partner-Oszillator, sodass
die Filteraufteilung und Dreistimmen-Wechselwirkungen von einem physischen SID abweichen.
Das Handheld hat ein songweites Budget von vier gewöhnlichen SID-Noten.
Ring oder Sync kostet zwei Budgeteinheiten pro Note, was zwei solche Noten oder eine plus zwei
gewöhnliche zulässt. Akkorde und Release-Ausklänge zählen. Losgelassene Noten werden zuerst
entfernt; neue Noten haben Vorrang vor älteren gehaltenen Noten. Aufwendige Inserts und
andere Synthesizer teilen sich weiterhin das Audio-CPU-Budget.

| FX | Bereich | Aktion |
| --- | --- | --- |
| `SCP` | `00-FF` | Basis der Pulsbreite, auf die native 12-Bit-Breite skaliert; die Pulsbewegung des Rezepts bleibt aktiv. |
| `SCT` | `00-FF` | Basis der Grenzfrequenz, auf die native 11-Bit-Grenzfrequenz skaliert; die Filterbewegung des Rezepts bleibt aktiv. |
| `SRN` | `00-0F` | Native Resonanz. |
| `SWV` | `00-07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00-07` | Filtermodus-Bits: 1 Tiefpass, 2 Bandpass, 4 Hochpass; 0 Bypass. |
| `SMR` | `00-FF` | Makrotakt des Rezepts, skaliert von 1 bis 200 Hz. |
| `SRG` | `00-01` | Dreieck-Ringmodulation mit stummem Partner. |
| `SSY` | `00-01` | Oszillator-Sync mit stummem Partner. |

Pulsbreite und Grenzfrequenz unterstützen `SLE`. Diskrete Wellenformen, Schalter, Resonanz
und FM-Feedback behalten ihre nützlichen nativen Schritte; breitere Bytewerte würden
nicht mehr Hardware-Zustände erzeugen. `FBK` wählt direkt das Feedback `00-07`.
Operatorpegel verwenden die oben aufgeführten engine-spezifischen Bereiche. Brightness- und
Master-Anpassungen behalten den vollen Bytebereich.

### Portable Werkssammlungen und USER-Presets

Native Chip-Instrumente verwenden **Bank:**, um ALL, eine mitgelieferte Sammlung oder
USER zu wählen. **Preset:** wählt einen Klang innerhalb dieser Auswahl. Halte EDIT und drücke
Links oder Rechts in einer der beiden Zeilen, um durchzuschalten; ein kurzes Tippen auf EDIT öffnet seine Auswahl.

Mitgelieferte Sammlungen sind ZIP-Pakete unter `instruments/FACTORY/`. USER-Dateien kommen
unter `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` oder `gb-noise`.
Durchsuche deine Ordner direkt oder lege dort ein ZIP aus Ordnern und kompatiblen `.cni`-
Presets ab. DX7 akzeptiert außerdem unterstützte `.syx`-Einzelstimmen und 32-Stimmen-
Banken, auch innerhalb von ZIPs. ZIPs bleiben auf dem Datenträger unverändert. Das direkte Laden von WOPL/WOPLX wird nicht unterstützt;
verwende kompatible CNI-Presets oder konvertiere unterstützte Quellformate zuvor offline.

Das Durchschalten der USER-Presets setzt sich über Dateien, Ordner, Banken und ZIP-Pakete in
beide Richtungen fort und springt an den Enden um. Das erneute Öffnen des USER-Browsers folgt der
aktuellen Auswahl. ALL enthält USER-Klänge unter Unsorted für Engines
mit Kategorien; USER bleibt eine eigene Auswahl auf oberster Ebene. Sega PSG und Game Boy verwenden
flache ALL-/Factory-Listen und einen hierarchischen USER-Browser, ohne ein Unsorted-
Fenster. Ältere DX7-Dateien in `instruments/banks/dx7/` bleiben in USER zugänglich.

Eine Engine zeigt nur ihre kompatiblen mitgelieferten Sammlungen. Factory Presets
kombiniert die mitgelieferten OPLL-/VRC7-Klangsätze für die gewählte Engine; DX7 kombiniert
die Originale von ChooChoo und YSE. Heruntergeladene benannte Sammlungen bleiben getrennt.
Fat Man 2-op ist unter OPL2 aufgeführt und Fat Man 4-op unter OPL3. Kompatible OPL2-
CNI-Dateien können weiterhin in OPL3-USER-Ordnern verwendet werden.

Die Ausgabe nativer Engines verwendet eine feste gemessene Verstärkungskompensation statt einer
Normalisierung pro Preset. Bestehende Songs mit nativen Instrumenten können daher
mit anderem Pegel spielen; prüfe die Balance gespeicherter Mixe. Die Kalibriermethode
und die Grenzen sind in `scripts/README_MEASURE.md` dokumentiert.
