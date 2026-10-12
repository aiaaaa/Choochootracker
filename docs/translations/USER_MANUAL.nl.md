# ChooChooTracker-handleiding

> Nederlandse vertaling van [`docs/USER_MANUAL.md`](../USER_MANUAL.md), gemaakt op basis van de Engelse versie bij commit `7c3ed6f`. Bij verschillen is de Engelse versie leidend.
>
> **Afspraken.** De interface van de applicatie is Engels. Daarom blijven de namen van schermen, menu's, velden en knoppen (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**, **OPT**, **SELECT**...), FX-codes (`HOP`, `TBX`, `M1A`...), module- en enginenamen (ADSR, LFO, Braids, Plaits...) en bestandsextensies onvertaald, zodat je ze op het scherm herkent.

ChooChooTracker is een muziektracker met 8 sporen voor handheldconsoles.

Het volgt de klassieke werkwijze en navigatie van LSDj, met verschillende mogelijkheden voor geluidsontwerp:

- AY-chipemulatie
- Mutable Instruments Braids-, Plaits- en Plaits-Alt-oscillatoren, met een multimodefilter
- PCM-samples, single-cycle-golfvormen en wavetable-oscillatoren, met een multimodefilter
- aChChid, een veelzijdige acid-synthesizer op basis van Open303
- Bogie, een native drumsynthesizer met 12 modellen en een multimodefilter
- MME, een synthesizer met twee oscillatoren en meervoudige modulatie, met een multimodefilter
- Sintered, een native experimentele digitale drumsynthesizer met zes modellen

## 1. Installatie en bestanden

### ArkOS / PortMaster

Installeer `choochootracker.zip` via PortMaster, of kopieer het uitgepakte pakket naar de map `ports` van de console. Start **ChooChooTracker** via het Ports-menu.

### Miyoo Mini / Mini Plus (Onion OS)

Pak het MiyooPorts-archief uit in de hoofdmap van de SD-kaart, zodat de mappen `Roms/PORTS/` samengaan met de bestaande mappen van Onion. Start **ChooChooTracker** via het Ports-menu nadat je de collectie hebt vernieuwd. Maak een back-up van de map `projects/` van de app voordat je een bestaande installatie vervangt.

### Windows

Start `choochootracker.exe` met `SDL2.dll` en `libwinpthread-1.dll` in dezelfde map.
Druk op **ALT + ENTER** om volledig scherm in of uit te schakelen. Het venster is van grootte te veranderen; het beeld
schaalt mee met behoud van de oorspronkelijke beeldverhouding.

### Web

De browserpagina volgt het actieve trackerthema. Opgeslagen instellingen, inclusief
aangepaste toetstoewijzingen die met **Done** zijn bevestigd, worden direct naar de
opslag van deze browser geschreven. Tik na schermvergrendeling of het wisselen van app op een bedieningselement om
zo nodig het geluid te herstellen. **Restart audio** opent de uitvoer opnieuw zonder het
project weg te gooien. **Reload app** slaat het huidige werk op in **Continue**, slaat de instellingen op
en wacht op de browseropslag voordat de pagina opnieuw wordt geladen; een mislukte of verlopen opslag
annuleert het herladen. Gebruik **Download .cct** voor een draagbare back-up. De browseropslag
kan los daarvan worden gewist, en herstel van controller- en audiorouting op iOS
is niet gegarandeerd.

De browserversie ververst haar weergave met ongeveer 30 FPS en gebruikt een vaste
audiobuffer van 1024 frames om de audio tussen de aanroepen meer tijd te geven. Oudere opgeslagen
bufferinstellingen worden automatisch aangepast; projecten en andere instellingen blijven
behouden. Het uitlezen van invoer en de timers van de app houden hun gebruikelijke ritme.

### Maprichting

| Map | Inhoud |
|---|---|
| `AY_Wavetables` | `.aywave`-wavetables van de AY-chip |
| `fonts` | ChipNomad-lettertypebestanden `.cnfont` |
| `instruments` | `.cni`-instrumentpresets, inclusief ChipNomad-bestanden |
| `pitch-tables` | `.csv`-alternatieve stemmingen |
| `projects` | `.cct`-song- en projectbestanden |
| `samples` | `.wav`-bestanden voor de Sampler-engine |
| `SR_wavetables` | wavetables in Serum-formaat voor de BYOWTBL-engine |
| `themes` | ChipNomad-thema's `.cth` |
| `title` | Materiaal voor het titelscherm |
| `waveforms` | `.wav` single-cycle-golfvormen voor de 2xSCWF-engine |

## 2. Bediening

ChooChooTracker werkt het best met een gamepad met een d-pad, 2 analoge sticks en 7 knoppen.

| Logisch bedieningselement | Standaard op handheld | Standaard op Windows |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | d-pad | Pijltoetsen |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Spatiebalk |
| **SELECT** | Select | Shift |
| Modulatieassen | Linker- en rechterstick | Sticks van de gamecontroller |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Toewijzingen kunnen worden gewijzigd in **Settings > Key mapping**. Laat op PortMaster de knop los die je gebruikte om een toewijzing te selecteren voordat je de nieuwe knop indrukt. Sla een
herindeling op met **Done** en laat de knop dan los voordat je in Settings verdergaat.
Desktopindelingen gebruiken QWERTZ als standaard alleen voor QWERTZ-regio's; Poolse toetsenborden
behouden de standaard QWERTY-instellingen.

Ook gebruikers van Windows, web en Android kunnen een gamecontroller gebruiken. Op Android
verschijnen de aanraakbedieningen automatisch als er geen gamepad is aangesloten: in staand formaat
staan ze onder de tracker, met een vierkante d-pad, de bedieningen A/EDIT, B/OPT, START/PLAY en
SELECT/SHIFT, gevolgd door een apart Live/Motion-gedeelte met twee modulatiesticks
en tijdelijke REC/DEL-bedieningen. In liggend formaat blijft de d-pad links
en staan de actieknoppen rechts. Android-projecten en -samples blijven privé voor
de app; gebruik de Android-bestandskiezer om `.cct`-projecten en `.wav`-samples te importeren,
en om te kiezen waar WAV-exports worden opgeslagen.

### Navigatie

- Gebruik **[UP/DOWN/LEFT/RIGHT]** om de cursor te verplaatsen.
- Houd **SELECT + [DIRECTION]** ingedrukt om tussen schermen te wisselen.
- **SELECT + [LEFT/RIGHT]** leidt ook vanaf zijpagina's naar het naburige MSCPIT-scherm,
  zoals bij Instrument Pool: Project/Settings (inclusief de submenu's van Settings)
  leiden naar Mixer/Chain; Groove naar Chain/Instrument; Modulation/Insert FX naar
  Phrase/Table; AY Wavetable naar Instrument/Table. Aan de linkerrand
  keren Reverb/Delay met SELECT + LEFT terug naar de Mixer. De bestaande verticale navigatie blijft ongewijzigd.
- Houd **OPT + [DIRECTION]** ingedrukt voor schermspecifieke navigatie.
- Gebruik op een waarde **EDIT + [LEFT/RIGHT]** voor fijne wijzigingen of **EDIT + [UP/DOWN]** voor grove wijzigingen.
- Op een Modulation-**Destination** doorloopt **LEFT/RIGHT** de beschikbare bestemmingen rechtstreeks; tik op **EDIT** om het gegroepeerde menu te openen.
- Tik op **EDIT** om een waarde in te voeren of een opdracht uit te voeren.
- Tik twee keer op **EDIT** om, waar ondersteund, een nieuwe chain, phrase of nieuw instrument te maken.
- Een ingedrukt gehouden richting herhaalt zich na de in Settings ingestelde vertraging.

### Selectie en klembord

- **SELECT + OPT** schakelt de selectiemodus in en doorloopt nuttige selectiebereiken.
- Gebruik **[DIRECTION]** om het bereik uit te breiden.
- **OPT** kopieert de selectie.
- **OPT + EDIT** knipt haar uit.
- **SELECT + EDIT** plakt.
- **EDIT + [DIRECTION]** bewerkt de geselecteerde cellen gezamenlijk.

Op het Phrase-scherm kan bij een actieve selectie de notenkolom worden gevuld
met ritmepatronen:

- **OPT + LEFT** vult de notencellen van de selectie met een vast patroon van 16 stappen
  en gaat bij elke druk naar het volgende patroon. Na het laatste patroon worden de
  noten gewist en begint de cyclus opnieuw. De patronen zijn uitgelijnd op de absolute
  phrase-rijen, zodat een gedeeltelijke selectie alleen de stappen krijgt die erbinnen vallen.
- **OPT + RIGHT** vult de notencellen van de selectie met een willekeurig patroon van
  3-8 noten (7 en 8 zijn minder waarschijnlijk), eindeloos herhalend zolang **OPT**
  ingedrukt wordt gehouden.
- **OPT + UP** muteert de noten die al in de selectie staan: bij elke druk
  worden er 1-3 gekozen en wordt elk met een willekeurige hoeveelheid binnen een octaaf verschoven
  (kleine verschuivingen zijn waarschijnlijker dan extreme). Bij actieve **Note Lock**
  worden de verschoven noten terug in de toonladder gezet; Sampler-instrumenten met
  **Slice** aan blijven chromatisch. Er wordt niets gewist: de 8e druk herstelt de
  noten die de selectie aan het begin van de cyclus bevatte, waarna de cyclus opnieuw begint.
- **OPT + UP** op een selectie die alleen de volumekolom beslaat (geen noot-,
  instrument- of FX-cellen) maakt in plaats daarvan velocities willekeurig: de
  eerste druk past het patroon Minimal toe (elk volume verschuift met maximaal
  +-20), de tweede Medium (maximaal +-50), de derde Random (volledig
  willekeurige waarden), en de vierde herstelt de volumes die de selectie aan het begin
  van de cyclus bevatte. Het loslaten van **OPT** zet de cyclus terug op Minimal.
  Rijen zonder volume blijven onaangeroerd.
- **OPT + DOWN** spreidt de geplaatste noten van geslicete (sliced) Sampler-instrumenten in
  de selectie uit tot een chromatische reeks: de eerste noot van elk instrument (de hoogste
  in de sequencer) behoudt haar waarde en elke volgende noot van dat
  instrument stijgt een halve toon boven de vorige, waardoor een stapel
  identieke noten wordt omgezet in opeenvolgende slice-triggers. Meerdere geslicete
  instrumenten spreiden onafhankelijk uit vanaf hun eigen eerste noten, noten
  van instrumenten zonder slicing blijven onaangeroerd, en het resultaat wordt nooit
  op de toonladder van het project vastgezet. De laatste noot die de spreiding schrijft, wordt
  de laatst gebruikte noot bij het invoeren, zodat handmatig doorgaan met de reeks
  bij de volgende slice verdergaat. Een selectie met alleen geslicete
  instrumenten spreidt bij elke druk uit; als er ook niet-geslicete noten zijn,
  wordt bij de eerste druk uitgespreid en neemt vanaf de tweede druk de willekeurige arpeggio
  die noten over.
- **OPT + DOWN** op een selectie zonder noten van geslicete instrumenten is een willekeurige
  arpeggiator voor de daar al geplaatste noten. Een akkoord wordt gekozen
  uit het CRD-palet - akkoorden van 3 noten voor selecties van 1-2 noten, akkoorden van 3 of meer noten
  voor 3, elk akkoord voor 4 of meer - met zijn grondtoon op een willekeurige noot
  van de projecttoonladder binnen het register van de selectie (elke chromatische
  noot als de toonladder uit staat of het spoor ervan is uitgesloten), en zijn tonen
  worden in de toonladder gekwantiseerd zoals de weergave dat voor CRD doet. Elke
  druk verdeelt het akkoord opnieuw over de geplaatste noten: willekeurig,
  van laag naar hoog, van hoog naar laag, daarna op en neer; de 5e druk
  herstelt de oorspronkelijke noten en de volgende kiest een nieuw akkoord.
  De akkoordnaam verschijnt in de statusbalk zolang de arpeggio actief is, er worden geen
  noten toegevoegd of verwijderd, en het loslaten van **OPT** zet het hele
  proces terug.
- Elke vuldruk wist de noten van de selectie voordat er wordt geschreven, en geplaatste
  noten krijgen het laatst gebruikte instrument; het loslaten van **OPT** zet de cycli
  terug op hun eerste stap.
- Zolang een selectie actief is en er geen andere melding wordt getoond, laat de
  statusbalk elke 2 seconden bij toerbeurt een geheugensteun voor deze combinaties zien: `EDIT + DIR =
  batch note edit`, `Double-tap EDIT: resample selection`, `OPT + LEFT =
  pattern fill`, `OPT + RIGHT = random fill`, `OPT + UP =
  mutate/randomize velocity` en `OPT + DOWN = random arp/slice spread`.
  Het betreden of verlaten van de selectiemodus start de rotatie opnieuw; elke actiemelding
  vervangt de hint totdat ze verloopt.

### Weergave

- **PLAY** start vanaf de cursor. Druk er nogmaals op om te stoppen.
- Je kunt noten, instrumenten en chains bewerken tijdens het afspelen; wijzigingen zijn te horen
  bij de volgende sequencer-tick.
- **SELECT + PLAY** start alle sporen wanneer het buiten het Song-scherm wordt gebruikt.
- **EDIT + PLAY** beluistert een instrument vanuit de Instrument Pool.

## 3. Songstructuur en schermenoverzicht

Houd **SELECT + [DIRECTION]** ingedrukt om tussen schermen te wisselen.

De hoofdnavigatierij is:

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Middelste rij:

- **M**: Mixer (vertakt naar Reverb, Delay)
- **S**: Song (vertakt naar Project, Settings)
- **C**: Chain
- **P**: Phrase (vertakt naar Groove)
- **I**: Instrument (vertakt naar Modulation en Instrument Pool)
- **T**: Table (vertakt naar de AY Wavetable-editor)

Bovenste rij:

- **R**: Reverb
- **P**: Project
- **G**: Groove
- **M**: Modulation

Onderste rij:

- **D**: Delay
- **S**: Settings
- **P**: Instrument Pool
- **W**: AY Wavetable-editor

Een ChooChooTracker-song volgt deze hiërarchie:

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

Een song heeft 8 sporen. Elk spoor bevat een reeks chains, en elke chain bevat een reeks phrases. Phrases zijn reeksen van 16 stappen met noten. Een noot bevat een toonhoogte, een instrument, een velocity en maximaal 3 spoor-FX. Een instrument is datgene wat het geluid maakt.

### Optionele 16x24-lettertypen en NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 bevat TechMonoAudit (omgezet uit Share Tech Mono), Departure Mono, Spleen en Cozette. Elk levert de volledige set afdrukbare ASCII-tekens in cellen van 16×24 voor een beeldscherm van 640×480. Lettertypen die uit pixels zijn opgebouwd, behouden hun oorspronkelijke pixels of een geheeltallige schaling met opvulling; Cozette blijft kleiner omdat het verdubbelen van haar inktdoorsnede van 13 pixels de cel zou overschrijden. Het thema NostromoAmberDa2 is beschikbaar via Settings → Edit color theme → Load. Het behoudt de amberkleurige achtergrond en informatietekst van Nostromo, met rode hoofdtekst, donkerdere oranje waarden en een blauwgroene cursor. Het laden van deze bestanden verandert je gekozen lettertype of thema niet. Zie de bijgeleverde README.txt voor naamsvermelding en licenties.

### Tussenruimte op de Project-pagina

De Project-pagina zet de appversie op een eigen regel en scheidt Load, Save, New, Export en Manage met lege rijen van de projectmetadata. Cursorposities en aanraakgebieden volgen de getoonde velden; projectinstellingen en bestandsbewerkingen zijn ongewijzigd.

### Telemetrie van de audioweergave

De audio-uitlezingen gebruiken een momentopname, die eigendom is van de interface, van de eindmix en van de bijdragen van de afzonderlijke sporen. Spoorsamples en stereopieken worden vastgelegd na spoorniveau en tilt, vóór de gedeelde effectreturns en de mastervermogenswinst. Het monitoren verandert het audiosignaal niet. De audiocallback publiceert momentopnamen van vaste grootte zonder locks of geheugentoewijzing; de getoonde geschiedenis beslaat ongeveer 21 ms.

### Permanente golfvorm

Graphics → Persistent waveform staat standaard op OFF. Tik op EDIT om te schakelen, of gebruik EDIT + richting; de keuze wordt opgeslagen als app-voorkeur. OFF herstelt pagina's op volledige hoogte en de bestaande golfvormvoorbeelden van instrumenten. Wanneer ingeschakeld, toont een golfvorm van twee rijen boven de bewerkingspagina's de eindmix, opgeteld tot mono. De pagina's Instrument en Modulation tonen de uitvoer van een spoor dat het geselecteerde instrument gebruikt; als meerdere sporen het gebruiken, wint het geselecteerde Song-spoor. Project, Settings, wavetables en pop-ups behouden hun inhoud op volledige hoogte. Lijsten scrollen binnen de resterende rijen. De voetteksten van Instrument behouden de ADSR-/samplevoorbeelden, en de velden van Modulation worden compacter gemaakt zodat alle bedieningselementen bereikbaar blijven. De aanraakgebieden volgen de getoonde velden in beide golfvormmodi.

### Pianoweergave tijdens het afspelen

Een met code getekende pixelpiano onder de acht spoorrijen aan de rechterkant laat de klinkende toonhoogteklassen oplichten, opgevouwen tot één octaaf. Hij omvat akkoordnoten en respecteert gedempte of gestopte sporen. De omtrek gebruikt de waardekleur van het thema en alle actieve toetsen gebruiken de golfvormkleur; rustende toetsen hebben donkere vulling en subtiele schaduwen. Het raster gebruikt geheeltallige schaling en randen van vaste breedte. De piano is een afspeelindicator, geen bedieningselement voor noteninvoer. Pop-ups en de wavetable-editor behouden hun eigen inhoud in dit gebied.

### Mixer-meters

De Mixer toont voor elk spoor naast elkaar liggende **L/R**-pieken, na zijn PAN en tilt, vóór de gedeelde effecten en de mastervermogenswinst. De meter beslaat -48 tot 0 dBFS en zakt terug tussen de updates. De lage en middelste banden gebruiken themakleuren, terwijl de bovenste band helderrood is nabij volle schaal.

## 4. Song, Chain en Phrase

### Song

Het Song-scherm bevat 8 kolommen, 1 per audiospoor. Elke cel verwijst naar een chain. Een song kan tot 256 rijen bevatten (`00-FF`).

Sporen lopen van links naar rechts, terwijl hun chains verticaal lopen. Elk spoor is onafhankelijk, en chains kunnen 1 tot 16 phrases bevatten, dus sporen kunnen uit de pas lopen. Ontwerp je chains zo dat ze bij elkaar blijven, of laat ze afdwalen als het spoor dat nodig heeft.

Het is gebruikelijk om 1 chain te reserveren voor lege phrases, meestal `00` of `FE`.

Je kunt meerdere subsongs in één project houden door "eilanden" te maken: secties die door lege rijen worden gescheiden.

#### Live Mode

Tik twee keer op **OPT** om de Live Mode te betreden of te verlaten; `LIVE` vervangt de Song-titel. In Live Mode start **PLAY** de geselecteerde chain op haar eigen spoor wanneer dat spoor gestopt is. Op een spelend spoor zet het de geselecteerde chain in de wachtrij voor de volgende chaingrens; druk twee keer op **PLAY** voor een dringende wissel op de volgende phrasegrens. Een geselecteerde lege Song-cel zet in plaats daarvan een gekwantiseerde stop in de wachtrij. `+` markeert een chain in de wachtrij, `!` een dringende chain en `-` een stop in de wachtrij.

Selecteer één Song-rij over meerdere kolommen om die sporen samen in de wachtrij te zetten of te stoppen. Selecties over meerdere rijen worden bewust geweigerd. Live-wachtrijen worden gewist zolang een Song-bereikslus actief is. Live is een uitvoeringsmodus en wordt niet in het project opgeslagen.

#### Bediening

- **OPT + [UP/DOWN]**: 16 posities omhoog of omlaag springen
- Selecteer een bereik en gebruik dan **SHIFT + EDIT**: de chains oppervlakkig klonen en hun oorspronkelijke phrases behouden
- Selecteer een bereik en tik dan twee keer op **SHIFT + EDIT**: zowel de chains als hun phrases diep klonen
- Selecteer een bereik en gebruik dan **EDIT + [UP/DOWN]**: de selectie omhoog of omlaag verplaatsen
- Tik 3 keer op **OPT**: een markering toevoegen of verwijderen, voor visuele ordening
- **OPT + SHIFT**: het huidige spoor of de geselecteerde kolommen dempen (laat eerst **OPT** los om de demping actief te houden)
- **OPT + PLAY**: het huidige spoor of de geselecteerde kolommen solo zetten (laat eerst **OPT** los om de solo actief te houden)
- **OPT + [LEFT/RIGHT]**: elk spoor links of rechts van het huidige spoor solo zetten
- Selecteer een bereik en tik dan twee keer op **EDIT**: het scherm BOUNCE TO SAMPLE openen (zie Bounce hieronder)

#### Bounce

Selecteer een bereik van Song-rijen en -kolommen en tik dan twee keer op **EDIT** om het scherm BOUNCE TO SAMPLE te openen. De bounce omvat elk geselecteerd spoor: sporen waarvan de eerste chain later in de selectie begint, wachten stil en vallen in aan het begin van hun eerste chain, zodat het bestand de chains van alle geselecteerde sporen bevat.

Het scherm biedt:

- **File name**: bewerk met het tekentoetsenbord. Wanneer het scherm opent, wordt hier het eerstvolgende vrije volgnummer (`001`, `002`, ...) voorgesteld: het eerste nummer waarvan het `.wav`-bestand nog niet in de exportmap bestaat. Bewerk het vrij: de naam wordt ongewijzigd gebruikt. Als er al een bestand met die naam bestaat, wordt er een achtervoegsel `_001`, `_002`, ... toegevoegd, zodat bestaande bestanden nooit worden overschreven. De teller wordt gereset wanneer je een project laadt of maakt.
- **Sample rate** en **Bit depth**: dezelfde opties als op het exportscherm
- **Include in a sample name**: drie selectievakjes die contexttags voor de bestandsnaam zetten wanneer **Start** wordt ingedrukt (standaard allemaal uit):
  - **[BPM]** - het huidige tempo van het project, bijvoorbeeld `[120]`
  - **[Key]** - de grondtoon en toonladder van het project, bijvoorbeeld `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** - de gerenderde lengte van de bounce: één phrase is gelijk aan één maat (16 zestiende noten), een tel is 4 zestiende noten. Hele phrases tonen alleen maten, bijvoorbeeld `[2]`; gedeeltelijke phrases tonen `[0:2]` (twee tellen) of `[0:1:3]` (één tel plus drie zestiende noten). Bij bounces over meerdere sporen bepaalt het langste spoor de lengte. Een selectie die halverwege een tel eindigt, telt één zestiende minder: de laatste geselecteerde rij is het knippunt, dus haar note-off valt aan het einde van de voorgaande zestiende.
  - Tags worden in de volgorde `[BPM][Key][Length]` vóór de bestandsnaam toegepast.
- **Start**: de selectie renderen naar een WAV-bestand in de exportmap (zie *Export location* onder het Project-scherm)
- **Cancel**: terugkeren zonder te bouncen

Terwijl de bounce rendert, annuleert **OPT** hem. Wanneer de bounce klaar of geannuleerd is, keert het scherm terug waarvandaan je hebt gebounced.

Op desktop brengt key jazz hier ook directe invoer van hex-indexen, selectie met
Shift+pijltjes zoals in Phrase, Delete/Backspace/Insert en Ctrl+C/X/V - zie
[Key jazz](#key-jazz-alleen-desktop).

### Chain

Een chain is een geordende lijst van phrases van 16 stappen, met optionele transpositie. Hij kan tot 16 phrases bevatten, en dezelfde phrase kan meer dan eens voorkomen. De 2e kolom stelt de transpositie in halve tonen in.

Een sterretje (`*`) verschijnt naast een chain die in het project hergebruikt wordt. Je kunt chains vanaf het Song-scherm klonen.

#### Bediening

- **OPT + [LEFT/RIGHT]**: tussen sporen bewegen
- **OPT + [UP/DOWN]**: tussen chains van het huidige spoor bewegen
- Selecteer een bereik en gebruik dan **SHIFT + EDIT**: phrases klonen
- Selecteer een bereik en tik dan twee keer op **EDIT**: het scherm BOUNCE TO SAMPLE openen voor de geselecteerde chainrijen van het huidige spoor (zie Bounce onder Song)

Op desktop brengt key jazz hier ook directe invoer van hex-indexen - zie
[Key jazz](#key-jazz-alleen-desktop).

### Phrase

Een phrase is het patroon van een spoor in een traditionele stapsequencer.

Een phrase is 16 stappen lang, met 1 rij per stap. Elke rij bevat een noot, een instrument, een volume van `00` tot `7F` en FX-kolommen. Noten gebruiken trackernotatie zoals `C-4` (noot en octaaf).

Om een noot te stoppen voeg je `NOTE OFF` in, gebruik je de kill-note-FX, of speel je een andere noot zonder ingesteld instrument.

Wanneer een chain meerdere phrases bevat, werkt het Phrase-scherm als een doorlopende patroonbewerker. Onder rij `F` of boven rij `0` bewegen opent de volgende of vorige phrase in de chain.

Een sterretje (`*`) verschijnt naast een phrase die in het project hergebruikt wordt. Je kunt phrases vanaf het Chain-scherm klonen.

De FX-kiezer toont gangbare opdrachten plus die welke het instrument op die rij ondersteunt. Zo wordt een lange globale lijst met enginespecifieke opdrachten vermeden. ChooChooTracker ondersteunt meer FX dan ChipNomad.

### Bediening

- **OPT + EDIT** op een lege noot: `NOTE OFF` invoegen
- **EDIT + [UP/DOWN]** op een FX-naamkolom: het FX-selectiescherm openen
- **OPT + [LEFT/RIGHT]**: tussen sporen bewegen
- **OPT + [UP/DOWN]**: tussen phrases van de huidige chain bewegen
- Selecteer een bereik in de instrumentkolom en gebruik dan **SHIFT + EDIT**: instrumenten klonen
- Selecteer een bereik en gebruik dan **EDIT + [UP/DOWN]**: de phrase-rijen roteren
- Selecteer een bereik en tik dan twee keer op **EDIT**: het scherm BOUNCE TO SAMPLE openen voor de geselecteerde phrase-rijen van het huidige spoor (zie Bounce onder Song)

### Key jazz (alleen desktop)

Druk op **Esc** op het Phrase-scherm om key jazz in of uit te schakelen, een noteninvoermodus
in de stijl van een QWERTY-piano (zoals [m8c](https://github.com/laamaa/m8c)):

- `Z S X D C V G B H N J M` en `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` voeren noten
  chromatisch in over meer dan twee octaven, met de fysieke positie van elke toets,
  zodat de indeling hetzelfde is op AZERTY-, QWERTY- en QWERTZ-toetsenborden.
- Elke toetsaanslag schrijft de noot op de huidige rij en gaat naar de volgende rij.
- `[` / `]` verschuiven het octaaf.
- **Shift + pijltjes** selecteren een bereik van rijen en kolommen; een gewoon pijltje
  daarna wist de selectie.
- **Ctrl+C** / **Ctrl+X** kopiëren of knippen de geselecteerde kolommen, of de
  noot/het instrument/het volume van de huidige rij als er niets is geselecteerd; **Ctrl+V** plakt op
  de cursor.
- **Delete** verwijdert de hele rij of rijen (alle kolommen) en schuift de rest van
  de phrase omhoog om de opening te vullen; de cursor blijft op dezelfde rij.
- **Backspace** verwijdert het element onder de cursor alleen in de huidige kolom
  (of de kolommen van de selectie, als er een actief is), schuift wat eronder staat
  omhoog om de opening te vullen en zet de cursor één rij terug, zoals in
  een teksteditor.
- **Insert** voegt een lege rij in en duwt de rest van de phrase omlaag.
- **Ctrl+S** slaat het project op. Buiten key jazz doet Ctrl+S niets.
- Nogmaals Esc schakelt key jazz uit.

Zolang key jazz aan staat, neemt het de notentoetsen op het Phrase-scherm volledig over,
inclusief de Edit/Opt/Motion-sneltoetsen die dezelfde toetsen delen - schakel het
met Esc uit om ze weer te gebruiken.

Key jazz werkt ook, onafhankelijk met Esc schakelbaar, op:

- **Song** en **Chain**: typ de hex-index van een chain of phrase rechtstreeks
  (`0-9`, `A-F`) in plaats van op te hogen met Up/Down. Het eerste cijfer dat op
  een cel wordt getypt, vervangt haar waarde; verdere cijfers die zonder te bewegen worden getypt, schuiven
  erin door (typ "3" en dan "F" om 3F te krijgen). Op Song selecteert **Shift+pijltjes** een
  bereik, **Delete** verwijdert de hele rij of rijen en schuift de rest omhoog, **Backspace**
  verwijdert de huidige/geselecteerde kolom(men) en schuift wat eronder staat omhoog,
  **Insert** voegt een lege rij in, en **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  kopiëren/knippen/plakken de geselecteerde rijen (of de huidige rij als er niets is
  geselecteerd) - dezelfde sneltoetsen voor structuurbewerking als op het Phrase-scherm.
  Dit neemt Shift over, dus Shift+Right/Up springen niet meer naar Chain/Project
  zolang key jazz hier actief is - druk op Esc om die sneltoetsen terug te krijgen.
- **Elk scherm met een tekstveld**: typ rechtstreeks op het toetsenbord
  in plaats van het schermtoetsenbord te gebruiken. Dit omvat de bestandsnaam, titel
  en auteur van Project, de instrumentnaam, de naam van het kleurenthema,
  de naam van de stemmingstabel, de bestandsnaam van de bounce en de naamdialogen (naam
  invoeren, map maken). Backspace werkt als in een gewoon tekstveld; Shift
  typt hoofdletters. Esc schakelt het op al deze schermen tegelijk om,
  en het schermtoetsenbord opent nog steeds normaal wanneer key jazz uit staat.

## 5. Instruments

Alle instrumenten hebben een paar gemeenschappelijke parameters:

- Instrumenttype
- Naam, maximaal 15 tekens
- Standaard tabelsnelheid, in ticks per tabelrij
- Transpositie aan of uit

Elk instrument heeft een standaardtabel met hetzelfde nummer in het bereik `00-7F`. Je kunt deze als hulptabellen gebruiken, maar hulptabellen in het bereik `80-FE` houden voorkomt conflicten en verwarring. Tabellen behoren tot de belangrijkste hulpmiddelen voor geluidsontwerp in ChooChooTracker.

### Bediening

- **OPT + [LEFT/RIGHT]**: tussen instrumenten bewegen
- **EDIT + PLAY**: het instrument beluisteren
- **SHIFT + OPT**: het instrument kopiëren
- **SHIFT + EDIT**: het instrument plakken

### AY Classic, AY Plus en AY Sample

Dit zijn de oorspronkelijke AY/YM-engines van ChipNomad. AY Classic biedt bedieningselementen voor toon, ruis en envelope in hardwarestijl. AY Plus voegt software-oscillatoren en rijkere modulatie toe. AY Sample speelt een sample af via AY-achtige volumeniveaus en is iets anders dan de Sampler-engine.

#### AY Classic

Dit is een verouderd instrumenttype uit ChipNomad `v1.0.0`. Het nieuwere AY Plus-instrument biedt meer functies.

AY Classic-instrumenten hebben deze parameters:

- Mixer: toon aan of uit, ruis aan of uit, envelopevorm (`0-F`)
- Volume: door software gegenereerde ADSR-envelope
- Automatische envelopeperiode: aan of uit, met een verhouding van `1:1` tot `F:F`

**AY Quality** in Settings beïnvloedt alleen de AY/YM-weergave. Het verandert de kwaliteit van Braids, Plaits of Sampler niet. **Sample dithering** is van toepassing op de kwantisatie van AY Sample.

Zie de [ChipNomad-documentatie over de AY-3-8910](https://chipnomad.org/chips/ay-3-8910/) voor details over de chip.

#### AY Plus

AY Plus past klassieke synthesizerconcepten toe op geluidsontwerp met de AY-3-8910/YM2149F. Je kunt de AY zien als een synthesizer met 5 oscillatoren:

- 3 toonoscillatoren met blokgolf
- 1 ruisoscillator
- 1 amplitude-envelopegenerator die als oscillator kan worden gebruikt

Volgens dit model heeft elk instrument 3 hardware-oscillatoren: toon, ruis en envelope. AY Plus voegt een 4e, software-oscillator toe. De AY-registers sneller bijwerken dan de tickfrequentie van het project opent meer mogelijkheden voor geluidsontwerp. Door de manier waarop de AY werkt, worden de oscillatoren gemengd met ringmodulatie. Probeer verschillende toonhoogteverschuivingen, ontstemmingen en oscillatorcombinaties om te horen wat eruit komt.

Elke tonale oscillator kan onafhankelijk worden gestemd met toonhoogte- en fijnafstemmingsverschuivingen.

Typen software-oscillatoren en hun parameters:

- Pulse - blokgolf met instelbare pulsbreedte
  - FM Depth - diepte van eenvoudige FM
  - Pulse width - het volledige bereik is `00-FF`, terwijl het aantal stappen op het Project-scherm wordt ingesteld
  - Pulse low - bepaalt de lage waarde in een herhalend paar waarvan de hoge waarde altijd `15` is
- Sync Tone - harde-synchronisatie-effect voor de toonoscillator
  - FM Depth - diepte van eenvoudige FM
- Sync Env - harde-synchronisatie-effect voor de envelope-oscillator
  - FM Depth - diepte van eenvoudige FM
  - Pulse width - tijdsverhouding tussen de 2 vormen in het envelopepaar
  - Envelope Pair - 2 envelopevormen waartussen wordt geschakeld
- Wavetable - wavetables van 32 stappen, bewerkt op het Wavetable-scherm
  - FM Depth - diepte van eenvoudige FM
  - Wavetable index
- Tone FM - eenvoudig FM-achtig effect voor de toonoscillator
  - FM Depth - FM-diepte van de toon
- Env FM - eenvoudig FM-achtig effect voor de envelope-oscillator
  - FM Depth - FM-diepte van de envelope

Software-oscillatoren Pulse en Wavetable kunnen niet met de Envelope-oscillator worden gebruikt. De Envelope-oscillator schakelt automatisch uit.

Eenvoudige FM schakelt snel tussen 2 periodewaarden, 1 lagere en 1 hogere. Dit lijkt op het gebruik van een blokgolf als FM-modulator. Pulse, Sync Tone, Sync Env en Wavetable gebruiken een verhouding `1:1` tussen FM-draaggolf en modulator. Tone FM en Env FM kunnen andere verhoudingen nabootsen via de toonhoogteverschuivingen van de software-oscillator.

#### AY Sample

Je kunt mono of stereo WAV-bestanden laden van `8 bit`, `16 bit`, `24 bit` of `32 bit`. Ze worden omgezet naar `8 bit` en afgekapt tot `16 KB`. Deze limiet komt overeen met de RAM-paginagrootte van de ZX Spectrum.

De AY speelt samples af via snelle schrijfacties naar het volumeregister. De uitvoer is unipolair, dus samples moeten "tot nul worden opgetild" voor een schonere weergave. Dit haalt kliks aan het einde van het sample weg en verbetert zachte uitloop.

De AY heeft een DAC-resolutie van `4 bit` met een niet-lineaire volumeschaal, dus `8 bit` samples worden tijdens het afspelen omgezet naar `4 bit`. De optie Sample dithering in Settings kan de waargenomen bitdiepte verbeteren. Dithering is niet praktisch op retroplatforms zoals de ZX Spectrum en de Atari ST, dus laat het uit wanneer je voor echte hardware schrijft of een knisperende lo-fi-klank zoekt.

Samples gedragen zich als een extra software-oscillator van AY Plus, maar hun extra parameters maken ze makkelijker te hanteren als een apart instrumenttype.

Sampleparameters:

- Sample rate - een sample op zijn oorspronkelijke snelheid afspelen levert een `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

De secties van de Tone- en Noise-oscillatoren komen overeen met die van AY Plus. Door de manier waarop de AY werkt, worden alle oscillatoren gemengd met ringmodulatie.

### Braids

Braids biedt 47 synthesemodellen. De belangrijkste bedieningselementen zijn:

- **Model**: oscillator of synthesealgoritme
- **Timbre** en **Color**: van het model afhankelijke macroparameters
- **Filter**: extra LP/BP/HP-filter, helling `12` of `24 dB`, cutoff en resonantie
- **ADSR**: attack, decay, sustain en release

Tik op **Model** om te kiezen uit modellijsten per categorie.

| Categorie | Modellen |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Parameters

Timbre en Color hangen af van het geselecteerde model. Raadpleeg de oorspronkelijke Braids-handleiding van Mutable Instruments voor de volledige lijst met modellen en details, of speel gewoon op het gehoor.

#### VCF en VCA

Braids voedt het standaard multimodefilter en de ADSR-envelope van ChooChooTracker.

#### Instellingen

Globale Braids-instellingen staan in de app-instellingen:

- **BITS**: verlaagt de uitvoerresolutie
- **DRFT**: voegt instabiliteit in de toonhoogte van de oscillatoren toe
- **SIGN**: bootst het algoritme voor golfvormonvolkomenheden van de oorspronkelijke Braids na

### aChChid

**aChChid** is een monofone acid-bassengine op basis van Open303. `Square` en `Saw` gebruiken de eigen TB-303-oscillator, het filter, de envelope en het accentgedrag. `Braids` vervangt alleen de oscillator en loopt daarna door hetzelfde 303-filter- en versterkerpad. Het biedt Model, Timbre, Color en Shaper in plaats van Fine tune. Shaper gaat geleidelijk van zachte verzadiging naar wavefolding. aChChid gebruikt niet het uniforme postfilter of de ADSR van ChooChooTracker.

Cutoff en Decay worden logaritmisch bewerkt: **EDIT + LEFT/RIGHT** beweegt met één
muzikale besturingsstap, terwijl **EDIT + UP/DOWN** met zestien stappen beweegt. Cutoff
beslaat 200 Hz tot 20 kHz; Decay beslaat 200 ms tot 2 s.

Een `F` in de volumekolom van de noot activeert een accent. Normale noten gebruiken een gate van een halve stap (3 ticks in de standaard groove van 6 ticks). `ASL` glijdt vanaf de vorige toonhoogte naar die noot zonder de 303-envelope opnieuw te triggeren; het houdt automatisch de voorgaande gate vast, en `ASL 00` geeft een glide van `60 ms`. `ATY` houdt zijn noot open voor de hele huidige stap. Noten zonder `ASL` triggeren altijd opnieuw. Modulatiebestemmingen omvatten Decay en Accent, plus Timbre en Color in de Braids-golfmodus.

### Bogie

**Bogie** is een native one-shot-drumsynthesizer. Trackernoten bepalen de toonhoogte en het volume van de tracker/het instrument bepaalt het niveau; elke trigger heeft zijn eigen interne decay, dus er is geen ADSR-pagina. Hij heeft 12 modellen: `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` en `Clave`.

De zes macro's zijn **Decay**, **Tone**, **Sweep**, **Noise**, **FM** en **Drive**. Hun labels passen zich aan waar dat nuttig is (bij Kick is Noise bijvoorbeeld `Click`, en de FM van Cowbell is metaalachtige kruismodulatie). Elke macro is actief op elk model, maar zijn muzikale rol verandert met het model: Sweep kan toonhoogte, burst-afstand of metaalachtige spreiding veranderen; FM kan kruismodulatie, een metaalachtige laag of een ringachtige boventoon toevoegen. De onderste helft van elk bereik is bedoeld voor conventionele drumgeluiden; de bovenste helft opent bewust bredere klankkleurmetamorfosen en meer synthetische resultaten.

Elk model kan het gedeelde LP/BP/HP-multimodefilter gebruiken, met het karakter Clean, Classic, Aggro of Acid, helling `12`/`24 dB`, cutoff en resonantie. Bogie ondersteunt al zijn actieve macro's plus Volume, Pitch, Cutoff en Resonance als bestemmingen voor modulatie en bewegingsopname. Gebruik een korte Decay met Cowbell voor scherpe phonk-attacks; verhoog FM voor een hardere, metaalachtigere bel.

Druk op het Modulation-scherm op **EDIT** op een bron om een familie te kiezen (Envelopes, LFO of Sticks) en vervolgens de bron. **Left** en **Right** doorlopen de bronnen nog steeds rechtstreeks.

### Sintered

**Sintered** is een native digitale one-shot-percussiesynthesizer. Noten bepalen zijn grondtoonhoogte; zijn zes modellen zijn `Knot`, `Shard`, `Burst`, `Comb`, `Logic` en `Melt`. Elk model heeft **Decay**, **Mod** en **Motion**, plus drie modelspecifieke bedieningselementen: Knot gebruikt Ratio/Spread/Fold, Shard Ratio/Feedback/Bite, Burst Noise/Color/Feedback, Comb Time/Damping/Regen, Logic Rate/Pattern/Crush en Melt Ratio/Chaos/Drive. **Mod** is altijd de interne koppelingsdiepte.

Motion is een interne macro-envelope, geen toonhoogteglide: waarden onder `80` gebruiken een AD-vorm, `80` schakelt hem uit en waarden boven `80` gebruiken een vorm met alleen decay. Verder van het midden af wordt de envelope sneller. De bestemmingen liggen per model vast, zodat Sintered voorspelbaar blijft in trackerpatronen: Knot animeert Mod/Fold; Shard Feedback/Bite; Burst Noise/Color/Feedback; Comb Damping/Regen; Logic Rate/Crush; Melt Chaos/Drive. Alle modellen resetten hun toestand bij een noot-trigger en begrenzen interne feedback voor herhaalbare klappen.

### MME

**MME** (Multi Modulation Engine) is een synthesizer met twee oscillatoren, gewijd aan de wisselwerking tussen oscillatoren. De modellen zijn `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` en `Vocode`. Beide oscillatoren zijn intern en volgen de trackernoten; `Interval` bepaalt hun onderlinge verhouding.

De zes macro's zijn **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** en **Shaper**. Waves kiest muzikale A/B-golfvormparen. Amount regelt de kernwisselwerking van het model, Flow verandert de richting of algoritmevariant, en Feedback is begrensd maar wordt aan het eind van het bereik bewust wild. Shaper is een gedeelde eindtrap die van schoon via verzadiging naar wavefolding gaat.

MME gebruikt het gedeelde LP/BP/HP-multimodefilter en de ADSR. Alle zes macro's, Cutoff en Resonance zijn beschikbaar voor modulatie en bewegingsopname. Houd **EDIT** ingedrukt op Model om de model-pop-up te openen.

### MIDI Out

**MIDI Out** (alleen desktop) stuurt een extern MIDI-apparaat aan in plaats van audio te synthetiseren: het triggeren van een noot stuurt een echte MIDI Note On op het **Channel** van het instrument (`1-16`) en, wanneer de noot eindigt, de bijbehorende Note Off. Het volume wordt velocity, en elke stem van een akkoordspoor krijgt zijn eigen Note On/Off. ChooChooTracker zelf maakt geen geluid - kies eerst het uitvoerapparaat onder [Settings](#12-settings). Zie [MIDI](#14-midi) voor het volledige plaatje, inclusief het beluisteren van klanken vanaf een MIDI-keyboard.

**Program** en **Bank high/low** (CC0/CC32) zijn optioneel - getoond als `--` wanneer uit, `Clear` schakelt een veld uit en onthoudt zijn waarde. Wanneer ingesteld, worden ze eenmaal verzonden, vlak vóór de volgende noot, telkens wanneer ze verschillen van wat dat kanaal het laatst is verteld (niet vóór elke noot, wat de eigen envelopes/patch van het ontvangende apparaat opnieuw zou triggeren).

**MC1 number**-**MC4 number** kiezen naar welk CC-nummer (`00-7F`) elk van de vier rij-FX MC1-MC4 verzendt - zie [MIDI Out FX](#midi-out-fx). Ook `--`/optioneel; een niet ingestelde slot maakt zijn FX werkloos.

### Subtractieve engines

De engines in deze categorie delen een architectuur van VCO naar VCF naar VCA.

Er zijn verschillende VCO-typen beschikbaar:

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Elk instrument voedt zijn VCO in een multimodefilter.

Het filter heeft een schakelbare helling van `12 dB` of `24 dB` en kan werken als laagdoorlaat (LP), banddoorlaat (BP) of hoogdoorlaat (HP).

Er zijn verschillende filterkarakters beschikbaar:

- **Clean**: steriel en digitaal
- **Classic**: geïnspireerd door klassieke Amerikaanse analoge synthesizers
- **Aggro**: geïnspireerd door brullende Japanse analoge synthesizers
- **Acid**: geïnspireerd door psychedelica

Classic, Aggro en Acid kunnen in deze alfaversie serieus gaan resoneren. Begin laag en draai ze voorzichtig open.

Het filter voedt een VCA die door een ADSR-envelope wordt aangestuurd. De vorm van de envelope kan overgaan tussen exponentiële, lineaire en logaritmische curven.

#### Plaits en Plaits-Alt

**Plaits** biedt 24 engines, in de selectie-pop-up als volgt gegroepeerd:

| Categorie | Engines |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** is een verzameling alternatieve modellen voor de oorspronkelijke Plaits-module. De meeste zijn vreemder of experimenteler dan de gemiddelde VCO.

| Categorie | Engines |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

De parameters volgen het ontwerp van Mutable:

- **Harmonic** regelt harmonische verhoudingen, balans of de modelkeuze binnen een engine.
- **Timbre** gaat doorgaans van donkere/ijle naar heldere/dichte spectra.
- **Morph** verkent een andere dimensie van de klankkleur.
- **Main/Aux** mengt de hoofduitgang met de alternatieve uitgang van de engine.

Hun precieze betekenis hangt van de engine af. Akkoordengines gebruiken ze bijvoorbeeld voor akkoordtype, omkering en golfvorm. Fysische modellen gebruiken ze voor materiaal, excitatie en decay, terwijl drumengines ze gebruiken voor toon, karakter en decay. Raadpleeg de Plaits-gebruikershandleiding voor details.

Voor `CHORD`, `DIATONIC CHORD` en `WT DIATONIC CHORD` toont Harmonic de naam van het geselecteerde akkoord (`MIN`, `MAJ`, `MIN7`, `MAJ7`, enzovoort) in plaats van zijn ruwe regelwaarde. Braids `WAVE PARA` benoemt zijn Color-voicings op dezelfde manier.

Tik op **Engine** om te kiezen uit enginelijsten per categorie.

**Env Mode** heeft 2 routings:

- `TRIG` bootst de module na met TRIG aangesloten en LEVEL niet aangesloten
- `VCA` houdt LEVEL open en past de ADSR van de tracker toe na de stem

#### Sampler

Deze schone sample-engine speelt mono of stereo PCM-samples af die in het RAM zijn geladen.

- Tik op **Sample** om een ongecomprimeerde PCM-WAV van `8 bit`, `16 bit` of `24 bit` te laden. Druk in de browser op **PLAY** om het gemarkeerde bestand te beluisteren.
- Wanneer een sample is geladen, verschijnt **EDIT** naast de naam. Tik erop om het Sample Edit-scherm te openen. **OPT** of **SELECT + [LEFT]** keert terug naar het instrument.
- Het Sample Edit-scherm toont de bestandsnaam (met een `*`-markering wanneer het sample in het RAM verschilt van het bestand op schijf), een formaataanduiding (samplesnelheid, kanalen), een hoge golfvorm, en de velden **Region**, **Select**, **Slice**, **Process** en **File**.
- Bewerkingen leven alleen in het RAM totdat een opslagstroom ze wegschrijft: het project slaat het bestandspad van het sample op en laadt de WAV bij het volgende laden van het project opnieuw van schijf, dus de tracker verlaten zonder **Save**/**Save As** gooit bewerkingen weg (de `*`-markering is de waarschuwing). Een ander sample in het instrument laden gooit ook niet-opgeslagen bewerkingen weg en zet de Region terug op het volledige sample (Start `00`, End `FF`).
- **Region** stelt de afspeelmarkeringen Start/End in, ook beschikbaar op het Sampler-instrumentscherm. Ze stellen genormaliseerde afspeelgrenzen in (`00-FF`); als Start na End ligt, wordt het sample achterstevoren afgespeeld. **EDIT + [LEFT/RIGHT]** stelt de markering fijn af in stappen van één (`01`) en zoomt de golfvorm eromheen in zolang **EDIT** wordt ingedrukt gehouden; **EDIT + [UP/DOWN]** stelt grof af (stap 16) en brengt de weergave terug naar het volledige sample. **EDIT + OPT** zet de markering terug op de standaard (Start `00`, End `FF`).
- De golfvormweergave zoomt in op een vast venster van één seconde audio rond de bewerkte markering en schuift mee om hem zichtbaar te houden; samples die in het venster passen, blijven in de volledige 1:1-weergave. De zoom is tijdelijk: hij duurt zolang **EDIT** wordt ingedrukt gehouden, en het loslaten van **EDIT** keert terug naar de volledige 1:1-weergave, net als grove stappen. Het betreden van het scherm zet de weergave altijd terug op het volledige sample.
- **Select** stelt een bewerkingsselectie in absolute frames in, onafhankelijk van de afspeelmarkeringen Start/End. **EDIT + [LEFT/RIGHT]** verplaatst een handvat twintig frames en zoomt erop in zolang **EDIT** wordt ingedrukt gehouden; **EDIT + [UP/DOWN]** springt met `frameCount/64` (minimaal 16) en keert terug naar de weergave van het volledige sample. **EDIT (tik)** op een handvat kopieert de huidige positie van de markering Start of End ernaartoe. **EDIT + OPT** op een van beide handvatten leegt de hele selectie. Wanneer de selectie leeg is, tonen beide handvatten `-`; wanneer de handvatten zijn omgedraaid, wisselen ze automatisch. De selectie is een editortoestand die alleen voor de sessie geldt: ze wordt niet met het project opgeslagen, en bij het betreden van het scherm wordt ze voorgevuld met de omvang van de afspeel-Region (bij de standaardmarkeringen het hele sample).
- **Slice** is één rij met vier waardecellen - **Mode**, **Count**,
  **Slice** en **Frame** - en wordt met het instrument opgeslagen. Off speelt het
  venster Start/End af. Phrase-noten kiezen slices chromatisch vanaf **C-0**, en
  noten voorbij de laatste slice slaan terug naar het begin (noot N speelt slice
  `N % count`, zodat een reeks voorbij de laatste slice terugkeert naar de eerste).
  Geslicete noten transponeren de toonhoogte niet en gebruiken geen Scale-kwantisatie, maar **CRD**
  behoudt de geselecteerde slice en transponeert zijn stemmen als akkoord. Slicebeginpunten
  worden op de golfvorm aangegeven met korte lijnen van twee pixels breed (donkeroranje; de lijn
  van de huidige slice is helderder), en de slice onder de cursor krijgt een
  zwarte achtergrondband op de golfvorm (de selectiekleur wordt niet over
  de band getekend, dus hij blijft volledig zwart en duidelijk onderscheiden van de rest van
  het voorbeeld).
  - **Mode** doorloopt `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + links/rechts of
    tik; EDIT + OPT schakelt slicing uit). Het wisselen van modus initialiseert de slicepunten:
    **EQUAL** verdeelt het venster Start/End gelijkmatig in **Count**
    delen; **AUTO** voert transiëntdetectie over het venster uit en plaatst een
    slice op elke gevonden inzet (tot **Count**); **LAZY** begint met één
    slice die het hele venster beslaat. Overschakelen naar LAZY bergt de huidige
    EQUAL/AUTO-instelling (aantal en grenzen) op, en terugschakelen naar EQUAL of
    AUTO herstelt haar ongewijzigd - er wordt geen bevestiging gevraagd en er gaat niets
    verloren. Slice en Stretch sluiten elkaar uit: het inschakelen van het ene schakelt
    het andere uit, en de Slice-cellen zijn gedimd terwijl Stretch de
    duur bepaalt.
  - **Count** toont het totale aantal slices (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** verandert het met één (minimaal 1, maximaal 64), **EDIT +
    [UP/DOWN]** doorloopt de aantallen als macht van twee `2 / 4 / 8 / 16 / 32 /
    64` met omslag. Elke wijziging berekent de verdeling opnieuw: **EQUAL**
    verdeelt het venster opnieuw gelijkmatig, **AUTO** voert de detectie opnieuw uit met het nieuwe
    doel (de detectiegevoeligheid wordt afgeleid van het aantal - meer slices
    betekenen een gevoeligere drempel). Het vak toont `-` wanneer de modus
    OFF is. In **LAZY** blijft het helder (het telt de met de hand geplaatste slices), maar
    het dient alleen ter weergave: de cursor slaat het over - van **Mode** naar rechts of
    van **Slice** naar links belandt op **Slice**, van de **END**-markering
    van de **Select**-rij naar beneden belandt daar ook, en het aantikken van het vak doet niets
    (met de hand geplaatste slices hebben geen verdeling om opnieuw te berekenen). **EDIT + OPT** en
    een gewone **EDIT**-tik doen hier niets: slicing uitschakelen gebeurt
    op **Mode**, een slice verwijderen op **Slice**.
  - **Slice** is de browser: hij toont de huidige slice, vanaf 1 genummerd
    (`01`..count). **EDIT + [LEFT/RIGHT]** gaat één voor één tussen slices,
    **EDIT + [UP/DOWN]** springt vier slices (beide begrensd, zonder
    omslag); de doorzochte slice wordt op de golfvorm aangeduid door zijn helderdere
    markering en de zwarte band, en de ingezoomde weergave centreert zich opnieuw op het
    begin van de slice. **EDIT + OPT** verwijdert de huidige slice (hij gaat op in
    de vorige; de eerste slice kan niet worden verwijderd, en het verwijderen van de
    laatste resterende slice schakelt slicing uit - de grenzen overleven een heen-en-terugreis
    via OFF en komen terug wanneer opnieuw een modus wordt gekozen). **EDIT (tik)**
    beluistert de momenteel geselecteerde slice als eenmalige weergave (werkt in
    elke slicemodus; het beluisteren stopt wanneer de toets wordt losgelaten of wanneer je
    het scherm verlaat). Het vak toont `-` wanneer de modus OFF is.
  - **Frame** toont het beginframe van de huidige slice in hex. **EDIT +
    [LEFT/RIGHT]** verschuift het met tien frames (zoomt in op de markering zolang
    **EDIT** wordt ingedrukt gehouden), **EDIT + [UP/DOWN]** verschuift met honderd en
    keert terug naar de volledige weergave. Een markering blijft binnen zijn eigen slice: hij
    kan het begin van de vorige of volgende slice niet overschrijden. **EDIT + OPT** verwijdert
    de slice.
  - **B** keert vanaf de Slice-rij terug naar het instrumentscherm, zoals overal
    elders op dit scherm; **SHIFT + LEFT** gaat vanaf elke plek terug.
  - **LAZY-werkwijze**: tik met LAZY geselecteerd op **PLAY** om een eenmalige
    weergave van het hele sample te starten - het speelt eenmaal vanaf het begin en stopt
    vanzelf, op de oorspronkelijke toonhoogte en snelheid van het sample (een groene markering van twee pixels
    volgt de positie op de golfvorm); nogmaals tikken op **PLAY**
    stopt het eerder. Terwijl het speelt,
    zet elke **EDIT**-klik een slice op de afspeelpositie, waar de cursor ook
    staat (slices die minder dan 50 ms van een bestaande liggen, worden
    geweigerd met `Too close to slice`). Het sample opnieuw afspelen behoudt
    de bestaande knippen en voegt nieuwe toe op elke aangetikte positie, op
    volgorde - het vak **Count** ververst bij elke geplaatste slice en de
    **Slice**-browser volgt de nieuwe. Dezelfde regels voor doorzoeken en bewerken als in de
    andere modi gelden voor met de hand geplaatste slices: **EDIT +
    richting** op **Slice** doorzoekt ze, **Frame** stelt het geselecteerde
    begin bij, **EDIT + OPT** verwijdert de huidige slice. De Frame-cel is gedimd
    zolang het plaatsen tijdens het afspelen is ingeschakeld. **SHIFT + PLAY** wordt in
    LAZY niet onderschept: het start de phrase-weergave zoals op elk ander scherm. LAZY-
    slices spelen in de sequencer precies zoals EQUAL en AUTO: phrase-noten
    worden chromatisch vanaf **C-0** op de met de hand geplaatste slices afgebeeld,
    met omslag voorbij de laatste.
  - **Combinatiehints**: zolang de cursor op een cel van de Slice-rij rust en er geen andere
    melding wordt getoond, toont de statusbalk de hint van die cel en houdt hem vast
    zolang de cursor daar rust. Op **Mode** is de hint gekoppeld aan de
    actieve modus en toont alleen de beschrijving (de modusnaam staat al in
    de cel): `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - het wisselen van modus
    vervangt de tekst direct, en OFF maakt de balk leeg. Op **Count** toont hij
    `Adjust the number of slices`, op **Slice**
    `Browse slices`, op **Frame** `Adjust slice start`. Elke actiemelding
    vervangt de hint totdat ze verloopt.
- **Process** selecteert een destructieve bewerking: **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** of **Reverse** (EDIT + links/rechts doorloopt, tik gaat vooruit, EDIT + OPT stelt geen in). **GO** (in dezelfde rij) voert de geselecteerde bewerking uit op de huidige selectie, of op het hele sample wanneer de selectie leeg is. **Crop** behoudt alleen de selectie; **Normalize** schaalt de selectie zodat haar piek volle schaal bereikt (beide kanalen delen één versterking zodat het stereobeeld behouden blijft); **Delete** verwijdert de selectie en voegt de uiteinden samen (het verwijderen van het hele sample wordt geweigerd); **Silence** zet haar op nul; **Fade In**/**Fade Out** laten de selectie lineair aanzwellen vanuit of wegsterven naar stilte; **Reverse** speelt de selectie achterstevoren af (frames worden ter plekke verwisseld, beide kanalen van een frame bewegen samen, lengte en markeringen blijven ongewijzigd). Crop en Delete vereisen een selectie - bij een lege selectie tonen ze `Select region first`. Elke bewerking pauzeert de audio kort, houdt één niveau ongedaan maken bij en markeert het sample in het RAM als gewijzigd: het bestand op schijf wordt pas aangeraakt bij de opslagstromen (zie hieronder), en het verlaten van het scherm gooit het undo-slot weg.
- **UNDO** (naast GO) zet het sample terug naar de toestand vóór de laatste bewerking. Het is gedimd totdat een bewerking is uitgevoerd, wisselt bij herhaaldelijk drukken tussen de toestanden vóór en na de bewerking, en wordt gewist wanneer het scherm opnieuw wordt betreden.
- **File** bevat de opslagstromen. Het instrument slaat het volledige pad op van de WAV waaruit het is geladen; deze stromen schrijven dat bestand of laten het instrument naar een nieuw bestand wijzen. Ze raken de instrumentnaam nooit aan.
  - Het veld File wisselt tussen **Save** en **Save As** (EDIT + links/rechts of tik); **GO** (in dezelfde rij) voert de getoonde actie uit.
  - **Save** overschrijft de WAV waaruit het sample is geladen. Voor een sample
    zonder slices vraagt het eerst `Overwrite <name>?`. Een geslicet sample opent eerst
    het dialoogvenster **SAVE SLICES** met de vraag waar de slicepunten thuishoren:
    **SAVE TO SAMPLE** schrijft ze als cue-chunks in de WAV (draagbaar -
    DAW's tonen ze als markeringen) en bewaart ze ook in het project,
    **SAVE TO PROJECT** schrijft een gewone WAV en bewaart de punten
    alleen in het project, **CANCEL** breekt af. Een selectievakje `Don't ask again in this project`
    slaat de gekozen bestemming met het project op en slaat
    het dialoogvenster daarna over (de keuze wordt als `- Sample save choice:` in
    de .cct opgeslagen en kan alleen worden gewijzigd door het projectbestand te bewerken). Het
    dialoogvenster wordt overgeslagen voor samples zonder slices. **Save** is gedimd (en
    wordt bij navigatie overgeslagen) totdat een sample met een bestandspad is geladen. Een
    mislukte schrijfactie behoudt de wijzigingsmarkering en toont de fout.
  - **Save As** schrijft het huidige sample naar een nieuw bestand: voer een bestandsnaam in (vooraf ingevuld met de huidige naam zonder extensie) en kies dan een map in de browser. Het sample wordt geschreven als `<folder>/<name>.wav`, het instrument wijst naar het nieuwe bestand en de map wordt onthouden als standaardsamplemap. Het is gedimd (en wordt bij navigatie overgeslagen) totdat sampledata is geladen, zodat een zojuist geladen sample een bestandspad kan krijgen. Paden langer dan 255 tekens worden geweigerd.
  - De `*`-markering vóór de bestandsnaam betekent dat het sample in het RAM verschilt van het bestand op schijf (elke bewerking zet haar, Save en Save As wissen haar). De markering geldt alleen voor de sessie: ze wordt niet met het project opgeslagen en wordt gereset wanneer het scherm opnieuw wordt betreden. Als meerdere instrumenten naar hetzelfde WAV-bestand verwijzen, overschrijft het opslaan van één het bestand voor allemaal.
- Gebruik op het Sample-instrumentscherm **EDIT + [LEFT/RIGHT]** om de vorige of volgende WAV in dezelfde map te laden.
- **Pitch** transponeert in halve tonen (`-48` tot `+48`).
- **Loop** kiest Off, Loop of Ping-Pong.
- **Speed** regelt granulaire tijdrek van `0%` tot `500%` (`100%` is normaal).

Niet-ondersteunde WAV-formaten tonen een fout. Zet ongebruikelijke bestanden om naar `PCM8`-, `PCM16`- of `PCM24`-WAV voordat je ze importeert.

#### 2xSCWF

2xSCWF is een dubbele single-cycle-golfvormoscillator. Laad in elke oscillator 1 mono-WAV met precies 1 periode. Beide oscillatoren lezen vooruit en slaan aan het einde van hun cyclus om.

Mix faadt tussen A en B.

Detune is fijn en exponentieel van unisono tot `+200 ct`, en gaat daarna in halvetoonstappen van `+3 st` tot `+24 st`.

2xSCWF behandelt elk bestand als een tabel van 1 cyclus en leest het precies één keer per oscillatorcyclus. Zo kun je 2 single-cycle-golven mengen en ontstemmen.

De fabrieksmap `waveforms/AKWF/` bevat golfvormen uit de Adventure Kid-bibliotheek. Online zijn er nog veel meer beschikbaar als je de collectie wilt uitbreiden.

#### BYOWTBL

BYOWTBL is een dubbele wavetable-oscillator die compatibel is met Serum-tabellen. Elke oscillator laadt een mono-WAV met opeenvolgende single-cycle-frames. `Pos A` en `Pos B` doorlopen hun tabellen onafhankelijk van `00` tot `FF`. De engine interpoleert lineair binnen elk frame en tussen aangrenzende frames. `Mix` faadt tussen de oscillatoren, en `Detune` verschuift oscillator B.

## 6. Modulatie en bewegingsopname

Elk instrument kan tot 4 modulatieslots hebben. Audio-instrumenten kunnen **Instrument Pan** of **Track Pan** als doel hebben; beide gebruiken `00` voor links, `80` voor het midden en `FF` voor rechts.

De beschikbare modulatietypen zijn:

- ADSR - klassieke attack-decay-sustain-release-envelope
- AHD - attack-hold-decay-envelope
- LFO - laagfrequente oscillator met verschillende golfvormen
- SLFO - LFO met een periodevermenigvuldiger voor langzame cycli
- FLFO - LFO op audiosnelheid die het kHz-bereik kan bereiken; FLFO kan zwaar zijn voor de CPU
- SLIN - lineaire joystickbesturing
- SRAT - op snelheid gebaseerde joystickbesturing

De lijst met modulatiebestemmingen hangt van het instrumenttype af.

Voor een `Cutoff`-bestemming is Amount exponentieel met een kwadratische respons: 25% bereikt 4,5 halve tonen, 50% bereikt 18 halve tonen, 75% bereikt 40,5 halve tonen en 100% bereikt zes octaven. Zo blijft er bij lage hoeveelheden fijne controle, terwijl diepe sweeps toch mogelijk blijven.

Wanneer een ADSR- of AHD-envelope Volume als doel heeft, wordt het de volume-envelope van het instrument. Een LFO met Volume als doel verschuift in plaats daarvan het huidige uitvoervolume.

De modulatiehoeveelheid kan positief of negatief zijn. Het bereik hangt van de bestemming af. Als het volledige bereik van de bestemming `127` of minder is, zoals bij de meeste parameters, bepaalt de hoeveelheid een absoluut bereik. Bredere bestemmingen zoals toonhoogte gebruiken een geschaalde hoeveelheid.

Alle duurparameters, waaronder attack, hold, decay en periode, worden in ticks gemeten.

LFO-golfvormen:

- Tri - bipolaire driehoeksgolf
- Sin - bipolaire sinusgolf
- UniTri - unipolaire driehoeksgolf
- UniSin - unipolaire sinusgolf
- RampDn - lineaire neerwaartse helling (zaagtandgolf)
- RampUp - lineaire opwaartse helling (zaagtandgolf)
- ExpDn - exponentiële neerwaartse helling (exponentiële zaagtand)
- ExpUp - exponentiële opwaartse helling (exponentiële zaagtand)
- Square - blokgolf
- Random - willekeurige sample-and-hold-golf
- Wavetable - leest een van de AY-wavetables van 32 stappen van het project. Waarden `00` tot `0F` worden zonder interpolatie afgebeeld van `-100%` tot `+100%`.

LFO-triggertypen:

- Free - start de LFO alleen opnieuw wanneer het instrument verandert
- Retrig - start de LFO bij elke noot opnieuw
- Phrase - start LFO en SLFO opnieuw wanneer de weergave een nieuwe phrase binnengaat
- Chain - start LFO en SLFO opnieuw wanneer de weergave een nieuwe chain binnengaat
- Hold - stopt na 1 cyclus en houdt de laatste waarde vast
- Once - stopt na 1 cyclus en keert terug naar nul

### Bewegingsopname (Motion recording)

- **STICK LIVE** (standaard `L1`) past stickmodulatie toe zonder de phrase te wijzigen. In **Settings > Stick live mode** schakelt `HOLD` (de standaard) het in zolang de knop wordt ingedrukt; `TOGGLE` schakelt het in bij één druk en uit bij de volgende; `FREE` laat het permanent ingeschakeld. Het loslaten van de knop bij `TOGGLE` laat het ingeschakeld.
- Houd tijdens het afspelen **MOTION RECORD** (`L2`) ingedrukt om stickmodulatie toe te passen en gewijzigde bestemmingen als absolute FX-waarden op te nemen.
- Houd **MOTION ERASE** (`R2`) ingedrukt om de overeenkomende bestemmings-FX uit de huidige rij te verwijderen.

De `~` rechtsonder geeft aan dat Stick live actief is. Record (`*`, of `!` bij overloop) en Erase (`x`) hebben voorrang bij de indicator en blijven in beide modi kortstondig. Beide schakelen de live-stickverwerking in zolang ze ingedrukt worden gehouden; het loslaten ervan laat een apart ingeschakelde live-schakelaar intact.

De modus wordt met de app-instellingen opgeslagen, maar de live-schakelaar staat bij het opstarten altijd uit. Het wijzigen van de modus wist de schakelaar: `HOLD` volgt elke momenteel ingedrukte Stick live-knop, `TOGGLE` wacht op een nieuwe druk en `FREE` is direct actief.

Bewegingsopname schrijft spoor-FX in de phrase die op dat moment speelt. Ze werkt eerst overeenkomende FX bij en gebruikt daarna lege slots van rechts naar links. Ze overschrijft nooit een andere FX. Als alle 3 slots vol zijn, wordt die beweging niet op de stap opgenomen. Een `!` rechtsonder betekent dat er meer bestemmingen zijn gewijzigd dan de 3 FX-kolommen konden bevatten.

Bewegingsopname ondersteunt bestemmingen van Braids, Plaits, Sampler, 2xSCWF, BYOWTBL en Bogie, inclusief actieve Bogie-macro's en filterbediening waar van toepassing.

## 7. Tables

Tabellen zijn een centraal hulpmiddel voor geluidsontwerp in trackers. In termen van Vortex Tracker combineren ze instrumenten en ornamenten, maar ze kunnen veel meer. Als je LSDj of NerdSEQ kent, zou het idee al bekend moeten voelen.

De kolom Pitch accepteert relatieve (`~`) of absolute (`=`) toonhoogtewaarden in halve tonen. De kolom Volume is `00`-`0F`, geldt voor elke engine bovenop zijn envelope en kan als gate worden gebruikt: wissel `0F` en `00` af en loop dan met `HOP 00`. De 4 FX-banen komen grotendeels overeen met de banen in een phrase, al gedragen een paar FX zich in tabellen anders.

Een `TIC`-FX op de laatste tabelrij stelt de snelheid van die kolom in en overschrijft de standaardtabelsnelheid van het instrument. Elke FX-kolom kan met een andere ticksnelheid lopen. De kolommen Pitch en Volume volgen de snelheid van de 1e FX-kolom.

Een tabel van 16 rijen lijkt misschien kort naast een lang Vortex Tracker-instrument, maar de `HOP`-FX kan voorwaardelijke lussen maken zoals "herhaal deze rijen 5 keer", evenals geneste lussen. Met lussen en onafhankelijke snelheden van de FX-kolommen komen die 16 rijen verrassend ver.

### Retrigger-modus

Het veld `Retrig` bovenaan het Table-scherm kiest wanneer een al actieve tabel opnieuw start. `Inst` is de standaard en compatibele modus: een phrase-rij met een instrument selecteert de standaardtabel van dat instrument en start haar op rij `00`. `Phrase` start een actieve tabel opnieuw wanneer de weergave een phrase binnengaat, `Chain` wanneer ze een chain binnengaat, en `Free` laat haar doorlopen totdat het spoor stopt of een expliciete opdracht haar vervangt.

Een structurele modus gaat nooit terug in de tijd. Als een spoor geen actieve instrumenttabel heeft, activeert de eerste phrase-rij die een instrument introduceert zijn tabel op precies die rij, zelfs als dat rij `4`, `8` of `F` van een phrase is. Latere instrumentnummers vervangen een actieve `Phrase`-, `Chain`- of `Free`-tabel niet. Daardoor zijn tabellen bruikbaar als LFO-achtige automatisering zonder stilzwijgend eerdere phrase-stappen te simuleren.

`TBL`, `TBX` en `RET` zijn altijd expliciete acties: ze selecteren of starten onmiddellijk opnieuw, ongeacht de modus. Op een Phrase- of Chain-grens vindt de herstart plaats voordat de nieuwe phrase-rij wordt gelezen, dus een `TBL` of `TBX` op die rij heeft het laatste woord. FX op tabelrij `00` worden daarom weer normaal uitgevoerd; cumulatieve FX behouden hun gebruikelijke opbouw.

Tabellen `00-7F` zijn gereserveerd voor standaard-instrumenttabellen. Tabellen `80-FE` zijn bedoeld voor hulptabellen die met het effect `TBX` worden gestart. Het lagere bereik werkt ook voor hulptabellen, maar op die manier gebruiken kan onverwachte conflicten en verwarring veroorzaken.

### FX kopiëren tussen Phrase en Table

Selecteer alleen FX-naam-/waardekolommen, kopieer of knip ze dan uit een Phrase en plak ze in een willekeurige FX-baan van een Table, of omgekeerd. De kopie behoudt precies de geselecteerde cellen, dus een selectie van alleen opdrachten of alleen waarden blijft gedeeltelijk. Plakken begint bij de huidige FX-cel en wordt afgekapt bij rij `F` en de laatste FX-baan van de bestemming. Noten, instrumentnummers, toonhoogte en volume gaan nooit tussen deze twee schermen over.

### Bediening

- **EDIT + [UP/DOWN]** op een FX-naamkolom: het FX-selectiescherm openen
- **OPT + [DIRECTION]**: tussen tabellen bewegen

## 8. Overige schermen

### Instrument Pool

De Instrument Pool toont elk instrument in het project en laat je ze herordenen. Tijdens het afspelen toont hij ook welke instrumenten op dat moment actief zijn.

#### Bediening

- **EDIT**: het instrument bewerken en naar het Instrument-scherm springen
- **SHIFT + OPT**: het instrument kopiëren
- **SHIFT + EDIT**: het instrument plakken
- **EDIT + [UP/DOWN]**: instrumenten herordenen
- **EDIT + PLAY**: het instrument beluisteren

### AY Wavetable

Wavetable is een type software-oscillator van AY Plus. Een project kan tot 256 wavetables bevatten, elk met 32 stappen. Alle wavetable-instrumenten delen dezelfde set golven. Op dit scherm bewerk je ze.

De AY-volumeniveaus zijn niet-lineair, dus de golfvorm van de Standard-weergave wordt getekend om bij de werkelijke uitvoerniveaus te passen. Tik twee keer op **OPT** om te wisselen tussen de Standard-weergave en de bewaarde **LFO-weergave**, met dezelfde sneltoets als Song/Live Mode: de LFO-weergave tekent de ruwe stappen `0`-`15` lineair, met de middellijn tussen `7` en `8` als nulpunt van de modulatie.

Het scherm heeft 2 logische rijen. De bovenste rij bevat de knoppen **Load** en **Save**, terwijl de 2e de wavetable-editor bevat.

#### Bediening

- **OPT + [LEFT/RIGHT]**: door de wavetablelijst bewegen
- **OPT + [UP/DOWN]**: de lijst doorlopen in stappen van 16 golven
- **SHIFT + OPT**: de wavetable kopiëren
- **SHIFT + EDIT**: de wavetable plakken
- **OPT + PLAY**: wisselen tussen Standard- en LFO-weergave

## 9. Tracker FX

Spoor-FX forceren waarden van de sequencer of de geluidsengine op afzonderlijke stappen. Andere groove-boxen noemen deze vaak "parameter locks". Elke Phrase-rij heeft 3 FX-slots.

Elke FX heeft een opdracht van 3 letters en een hexadecimale waarde. Het ingebouwde hulppaneel van de app geeft een korte beschrijving van de geselecteerde opdracht.

### Toonladderautomatisering (Scale)

`SCL XY` verandert de globale afspeeltoonladder en grondtoon zonder de in phrases opgeslagen noten te wijzigen. `X` selecteert een toonladder (`0` Chromatic, `1` Major, `2` Minor, tot en met `C` Custom); `Y` selecteert de grondtoon (`0` C tot en met `B` B). De opdracht geldt alleen voor phrase-FX, niet voor tabellen. Als meerdere sporen in dezelfde tick `SCL` geven, wint het spoor met het laagste nummer.

`SCL` hoort bij de Quantizer-modus. Bij actieve **Note Lock** wordt de opdracht niet aangeboden in de FX-lijst, en elke al in phrases geschreven `SCL` wordt genegeerd zolang die modus aan staat.

### Akkoorden (Chords)

`CRD XY` verandert de noot op zijn eigen Phrase-rij in een akkoord. `Y` selecteert: `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9 of `F` Min9. `X` selecteert een van 16 voicings: `0-3` zijn een oplopende cyclus van gesloten omkeringen; `4-7` voegen drop-2 toe; `8-B` voegen drop-3 toe; en `C-F` combineren beide drops. De cyclus loopt bij drieklanken en powerakkoorden door tot een extra octaaf, zodat elke waarde verschillend is; grotere waarden worden geleidelijk wijder en minder conventioneel. `CRD 4Y` is bijvoorbeeld grondligging met drop-2 en `CRD DY` is eerste omkering met beide drops. Aan de onder- of bovenkant van de toonhoogtetabel worden stemmen tot haar bereik begrensd. De opdracht is niet blijvend: schrijf haar op elke rij met akkoord. Elke gegenereerde noot wordt onafhankelijk gekwantiseerd wanneer Scale actief is, zowel in de modi Quantizer als Note Lock. Ze is alleen beschikbaar voor software-engines; AY-instrumenten negeren haar en blijven monofoon.

### Sequencer-FX

| FX | Waarde | Gedetailleerd gedrag |
|---|---|---|
| `ARP` | `XY` | Arpeggieert de grondnoot, `+X`-stappen en `+Y`-stappen. `37` geeft een mineurakkoordpatroon. |
| `ARC` | `XY` | `X` kiest de arpeggiorichting of het bereikmodus; `Y` is de snelheid in ticks. |
| `PVB` | `XY` | Toonhoogtevibrato: `X` is snelheid en `Y` is diepte. In Linear-modus gebruikt de diepte stappen van `10 cent`. |
| `PBN` | `XX` met teken | Telt `XX` toonhoogte-eenheden op bij elke phrase-/tabelrij; `FF` betekent `-1`. Gebruik `00` om te stoppen. |
| `PSL` | `XX` ticks | Glijdt van de voorgaande toonhoogte naar de nieuwe noot in `XX` ticks. |
| `PIT` | `XX` met teken | Opgebouwde relatieve verschuiving in stappen van de toonhoogtetabel. |
| `FIN` | `XX` met teken | Opgebouwde fijne verschuiving in cents bij Linear-toonhoogte, anders in periode-eenheden. |
| `PRD` | `XX` met teken | Opgebouwde relatieve verschuiving van de oscillatorperiode. |
| `VOL` | `XX` met teken | Opgebouwde relatieve volumeverschuiving (`FF` is -1, niet vol volume). |
| `VSL` | `XX` met teken | Telt `XX` bij het volume op bij elke phrase-/tabelrij. Gebruik `00` om te stoppen. |
| `PAN` | `00-FF` | Absoluut instrumentpanorama: links, midden (`80`), rechts. |
| `TPN` | `00-FF` | Absoluut panorama van het mixerspoor: links, midden (`80`), rechts. |
| `RET` | `XY` | Triggert elke `Y` ticks opnieuw; `X` past een volumeverandering toe. `Y=0` stopt het opnieuw triggeren. |
| `DEL` | `XX` ticks | Vertraagt note-on. Een vertraging die langer is dan de huidige groovestap slaat de noot over. |
| `OFF` | `XX` ticks | Stuurt note-off na `XX` ticks en gaat een ADSR-releasefase in. |
| `KIL` | `XX` ticks | Kapt de stem hard af na `XX` ticks zonder de ADSR-release uit te voeren. |
| `TIC` | `XX` ticks | Stelt de tabelticks per rij in. In een tabel verandert het de snelheid van die FX-kolom. |
| `TBL` | `00-FE`, `FF` uit | Vervangt en herstart de instrumenttabel onmiddellijk; `FF` stopt haar. |
| `TBX` | `00-FE`, `FF` uit | Start of vervangt onmiddellijk een hulptabel; `FF` stopt haar. |
| `THO` | rij `XX` | Laat alle kolommen van de instrumenttabel naar rij `XX` springen. |
| `TXH` | rij `XX` | Laat alle kolommen van de hulptabel naar rij `XX` springen; wordt niet vanuit een tabel gebruikt. |
| `GRV` | groove `XX` | Selecteert een groove voor het huidige spoor. |
| `GGR` | groove `XX` | Selecteert een groove voor elk spoor. |
| `HOP` | `XY` | Springt `X` keer naar rij `Y`; `X=0` herhaalt eindeloos. De HOP van een tabel werkt op haar eigen kolom. |
| `SNG` | `XX` met teken | Verplaatst de weergave met `XX` Song-rijen. |
| `PRO` | `00-64` | Evalueert een absolute triggerkans van `0%` tot `100%`. |
| `MOD` | `AB` | Triggert bij doorloop `A` van een cyclus van `B` doorlopen. |
| `SPD` | `XX` met teken | Selecteert een blijvende klokverhouding per spoor; zie de snelheidstabel hieronder. |
| `SLE` | `00-FF` ticks | Stelt de blijvende glidetijd per spoor in voor continue engine-FX. `00` is onmiddellijk. De instelling keert terug naar `00` wanneer het afspelen stopt. |

### Audio-FX-sends (Audio FX sends)

| FX | Waarde | Gedetailleerd gedrag |
|---|---|---|
| `RSN` | `00-FF` | Stelt de reverbsend van dit spoor in tot de volgende noot-trigger. |
| `DSN` | `00-FF` | Stelt de delaysend van dit spoor in tot de volgende noot-trigger. |

### ADSR-/trigger-FX

| FX | Waarde | Gedetailleerd gedrag |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Overschrijven attack, decay, sustain, release of envelopevorm. |
| `TDC` / `TCL` | `00-FF` | Overschrijven Decay of Color van de Trigger-modus bij Plaits en Plaits-Alt. |

#### Voorwaarden

Geïnspireerd op Zweedse "trig conditions" ondersteunt de tracker triggerkansen en modulovoorwaarden.

| FX | Waarde | Betekenis |
|---|---|---|
| `PRO` | `00-64` | Absolute triggerkans van `0%` tot `100%` |
| `MOD` | `AB` | Triggert bij iteratie `A` van `B`; bijvoorbeeld `12`, `22`, `14`, `34` |

`MOD`-tellers zijn lokaal voor het spoor en de phrase. Ongeldige combinaties, zoals `A` groter dan `B`, triggeren niet.

`MOD34` triggert bij de 3e doorloop van elke cyclus van 4 doorlopen. `MOD1F` triggert bij de 1e doorloop van elke cyclus van 16 doorlopen.

#### Afspeelsnelheid

Zoals NerdSEQ ondersteunt ChooChooTracker voor elk spoor een onafhankelijke afspeelsnelheid.

`SPD` leest zijn waarde als een byte met teken. `00` is normale snelheid, positieve waarden maken het spoor sneller en negatieve waarden trager. De instelling blijft actief totdat een andere `SPD`-opdracht haar wijzigt.

| Waarde | Snelheid |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

Oudere projecten zonder ondersteuning voor `SPD` met teken behouden hun oorspronkelijke toewijzing `00-10`, zodat ze blijven klinken zoals opgeslagen.

Zeer hoge vermenigvuldigers kunnen de resolutie van de tick-scheduler van de tracker overschrijden en zijn niet op hardware getest.

### Modulatie-FX (Modulation FX)

Phrase- en tabel-FX kunnen een modulatieslot wijzigen zonder het instrument te bewerken:

| FX-patroon | Betekenis |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Relatieve Amount-verschuiving voor modulatieslots 1-4 |
| `M11`, `M12`, `M13`, `M14` | Relatieve P1-P4-verschuivingen voor modulatieslot 1 |
| `M21`, `M22`, `M23`, `M24` | Relatieve P1-P4-verschuivingen voor modulatieslot 2 |
| `M31`, `M32`, `M33`, `M34` | Relatieve P1-P4-verschuivingen voor modulatieslot 3 |
| `M41`, `M42`, `M43`, `M44` | Relatieve P1-P4-verschuivingen voor modulatieslot 4 |

De waarde wordt geïnterpreteerd als een relatieve wijziging van `8 bit` met teken (`01` telt `1` op, `FF` trekt `1` af). Herhaalde opdrachten stapelen op. Effectieve waarden worden tot hun geldige bereik begrensd.

`P5` wordt niet als phrase- of tabel-FX aangeboden. Het is beschikbaar als modulatiebestemming `M1–M4 Wavetable`, zodat een modulator de AYWavetable-index van een andere kan doorlopen.

| Modulatietype | P1 | P2 | P3 | P4 | P5 wanneer Shape Wavetable is |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Ongebruikt |
| AHD | Attack | Hold | Decay | Ongebruikt | Ongebruikt |
| LFO | Shape | Trigger mode | Period | Ongebruikt | AY-wavetable-index |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | AY-wavetable-index |
| FLFO | Shape | Trigger mode | Frequency (`1 Hz` tot `20 kHz`) | Ongebruikt | AY-wavetable-index |

### Braids FX

| FX | Waarde | Betekenis |
|---|---|---|
| `BMD` | `00-2E` | Braids-model |
| `BTM` | `00-FF` | Absoluut genormaliseerd Timbre |
| `BCL` | `00-FF` | Absoluut genormaliseerde Color |
| `BCF` | `00-FF` | Exponentiële cutoff, `20 Hz` tot `20 kHz` |
| `BRS` | `00-FF` | Exponentiële filterresonantie |

### aChChid FX

| FX | Waarde | Betekenis |
|---|---|---|
| `ASL` | `00-FF` | Glijdt naar de noot zonder opnieuw te triggeren. `00` is `60 ms`; hogere waarden verlengen de glide. |
| `ADC` | `00-FF` | Overschrijving van de decay: `200 ms` tot `2 s`. |
| `AAC` | `00-FF` | Accenthoeveelheid: van geen tot maximum. |
| `ATM` | `00-FF` | Overschrijving van het Braids-Timbre; alleen actief in de Braids-golfmodus. |
| `ACL` | `00-FF` | Overschrijving van de Braids-Color; alleen actief in de Braids-golfmodus. |
| `ACF` | `00-FF` | Exponentiële cutoff van het 303-filter, `20 Hz` tot `20 kHz`. |
| `ARS` | `00-FF` | Resonantie van het 303-filter, van geen tot maximum. |
| `AEM` | `00-FF` | Envelopemodulatie van het 303-filter, van geen tot maximum. |

### Bogie FX

| FX | Waarde | Betekenis |
|---|---|---|
| `DMD` | `00-0B` | Selecteert het Bogie-model tot de volgende trigger. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Overschrijven Decay, Tone, Sweep, Noise, FM of Drive tot de volgende trigger. Alle zes macro's zijn op elk model beschikbaar. |
| `DCF` | `00-FF` | Filtercutoff, logaritmisch afgebeeld. |
| `DRS` | `00-FF` | Filterresonantie. |

### Sintered FX

| FX | Waarde | Betekenis |
|---|---|---|
| `SMD` | `00-05` | Selecteert het Sintered-model tot de volgende trigger. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Overschrijven Decay, Mod, de eerste twee modelmacro's, Motion of de laatste modelmacro tot de volgende trigger. |
| `SCF`, `SRS` | `00-FF` | Filtercutoff en -resonantie. |

### MIDI Out FX

| FX | Waarde | Betekenis |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Stuurt een MIDI-CC, herschaald naar `0-127`. Naar welk CC-nummer elk slot verzendt, wordt per instrument ingesteld (`MC1 number`-`MC4 number` op het instrumentscherm), niet door de FX zelf - een slot zonder nummer is werkloos. |

### Plaits FX

| FX | Waarde | Betekenis |
|---|---|---|
| `PMD` | `00-17` | Plaits-engine |
| `PHA` | `00-FF` | Absoluut genormaliseerde Harmonics |
| `PTM` | `00-FF` | Absoluut genormaliseerd Timbre |
| `PMO` | `00-FF` | Absoluut genormaliseerde Morph |
| `PAX` | `00-FF` | Main/Aux-mix: `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Exponentiële cutoff, `20 Hz` tot `20 kHz` |
| `PRS` | `00-FF` | Exponentiële filterresonantie |

### Sample FX

| FX | Waarde | Betekenis |
|---|---|---|
| `SPL` | `00-03` | Afspeelmodus: `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Speelt de genummerde slice af (`01` = eerste slice) ongeacht de noot; `00` behoudt de normale notentoewijzing. Vereist dat de slicemodus op het instrument is ingeschakeld |
| `SPT` | `XX` met teken | Transpositie van het sample in halve tonen |
| `SST` | `00-FF` | Genormaliseerd afspeelbegin |
| `STA` | `00-FF` | Alias van `SST` (genormaliseerd afspeelbegin) |
| `SEN` | `00-FF` | Genormaliseerd afspeeleinde |
| `SVL` | `00-FF` | Absoluut samplevolume |
| `SCF` | `00-FF` | Exponentiële cutoff, `20 Hz` tot `20 kHz` |
| `SRS` | `00-FF` | Exponentiële filterresonantie |
| `SSP` | `00-FF` | Samplesnelheid, afgebeeld van `0%` tot `500%` |

### Door AY-instrumenten gedeelde AY-FX

| FX | Waarde | Gedetailleerd gedrag |
|---|---|---|
| `AYM` | `XY` | `X` is de envelopevorm; `Y` kiest off/tone/noise/tone+noise (`0/1/2/3`). |
| `NOI` | `XX` met teken | Opgebouwde relatieve verschuiving van de ruisperiode. |
| `NOA` | `00-1F`, `FF` | Absolute ruisperiode; `FF` laat de voorrang voor de ruisperiode aan eerdere sporen. |
| `ERT` | willekeurig | Triggert de huidige hardware-envelopevorm opnieuw. |
| `EAU` | `XY` | Automatische enveloperverhouding `X:Y`; `X=0` schakelt haar uit. |

### Alleen voor AY Classic geldende FX

| FX | Waarde | Gedetailleerd gedrag |
|---|---|---|
| `EVB` | `XY` | Vibrato van de envelopeperiode: `X` snelheid, `Y` diepte. |
| `EBN` | `XX` met teken | Telt `XX` op bij de envelopeperiode bij elke phrase-/tabelrij. |
| `ESL` | `XX` ticks | Glijdt van de voorgaande envelopeperiode naar de nieuwe waarde. |
| `ENT` | noot `XX` | Stelt de envelopeperiode in aan de hand van de door de interface getoonde noot uit de toonhoogtetabel. |
| `EPT` | `XX` met teken | Opgebouwde relatieve verschuiving van de envelopeperiode. |
| `EPL` | byte `XX` | Stelt de lage byte van de envelopeperiode in. |
| `EPH` | byte `XX` | Stelt de hoge byte van de envelopeperiode in. |

### AY Plus FX

| FX | Waarde | Gedetailleerd gedrag |
|---|---|---|
| `TNN` | noot `XX` | Zet de toonoscillator op een noot uit de toonhoogtetabel en negeert de rijnoot. |
| `TNP` / `TNF` | `XX` met teken | Opgebouwde verschuiving van de toon in toonhoogtestappen / fijn. |
| `TRT` | willekeurig | Triggert de fase van de toonoscillator opnieuw waar ondersteund. |
| `ENN` | noot `XX` | Zet de envelope-oscillator op een noot uit de toonhoogtetabel. |
| `ENP` / `ENF` | `XX` met teken | Opgebouwde verschuiving van de envelope in toonhoogtestappen / fijn. |
| `SFT` | `00-07` | Softwaretype: None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | noot `XX` | Zet de software-oscillator op een noot uit de toonhoogtetabel. |
| `SFP` / `SFF` | `XX` met teken | Opgebouwde verschuiving van de software-oscillator in toonhoogtestappen / fijn. |
| `SRT` | willekeurig | Reset de fase van de software-oscillator. |
| `SFM` | `XX` met teken | Opgebouwde verschuiving van de FM-diepte. |
| `PWM` | `XX` met teken | Opgebouwde verschuiving van de pulsbreedte. |
| `SPL` | `XX` met teken | Opgebouwde verschuiving van het lage pulsniveau. |
| `SWT` | `XX` met teken | Opgebouwde verschuiving van de wavetable-index. |

### AY Sample FX

AY Sample accepteert de gedeelde AY-opdrachten plus `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` en `SFF` zoals hierboven beschreven.

| FX | Waarde | Gedetailleerd gedrag |
|---|---|---|
| `SMS` | `00-FF` | Stelt het verouderde afspeelbegin van AY Sample in op `XX x 64` bronsamples. |

Engine-parameter-FX gelden voor hun overeenkomstige instrumenttype en worden bij de volgende noot-trigger gereset, tenzij anders vermeld. `SPD` is bewust blijvend.

De FX-kiezer filtert dit naslagwerk tot gangbare opdrachten plus de groep die het actieve instrument ondersteunt. Het contextuele hulppaneel geeft tijdens het bewerken details over de waarden.

## 10. Mixer, Reverb en Delay

De Mixer is het hoofdscherm helemaal links. Hij toont de CPU-belasting en overstuurwaarschuwingen. Een rode `!` in de kolom **CLIP** markeert het spoor waarvan het droge signaal de mix boven het veilige bereik heeft geduwd.

Gebruik hem om de 8 sporen in balans te brengen, elk spoor te vormen met Tilt EQ, en audio naar de reverb of delay te sturen:

| Veld | Betekenis |
|---|---|
| LVL | Spoorniveau na de engine, `000-100` |
| REV | Send naar de gedeelde Clouds-reverb, `000-100` |
| DLY | Send naar de gedeelde ping-pong-delay, `000-100` |
| TLT | Tilt EQ per spoor, `00-FF`; `80` is neutraal, lage waarden begunstigen de bas en hoge waarden de hoge tonen. Lage waarden brengen een zachte overdrive met zich mee. |
| M | `*` dempt dit spoor |
| S | `*` zet dit spoor solo |

De mixer is spoorgebaseerd, niet instrumentgebaseerd. Als een spoor van instrument wisselt, blijven zijn niveau, PAN, Tilt en sends aan het spoor gekoppeld. Instrumenten hebben ook hun eigen **Vol** en **Pan** van `00-FF` in de bovenste rij van het Instrument-scherm. De PAN van het instrument wordt eerst toegepast, daarna de PAN van het spoor. Beide behouden een bestaand stereobeeld in het midden; ze dempen geleidelijk de tegenovergestelde kant wanneer ze naar links of rechts worden bewogen.

### Master FX

Het item **MASTER FX** opent een Insert FX-submenu met twee slots voor de volledige
mix. De effecten ervan lopen na de reverb- en delayreturns, vóór het master-
volume. Selecteer een module, bewerk de parameters of zet hem in bypass, precies zoals bij een
spoorinsert; druk op **OPT** om terug te keren naar de Mixer. De instellingen van Master FX worden
met het project opgeslagen en worden niet geautomatiseerd door phrase, tabel of modulatie.

### Clouds Reverb

Gebruik **SELECT + [UP]** vanuit de Mixer om het reverbinstellingenscherm te openen.

- **Return**: niveau van de natte reverb in de mastermix.
- **Time**: regeling van uitsterven/tijd, `00-FF`.
- **Damping**: absorptie van hoge frequenties, `00-FF`.
- **Filter**: laagdoorlaatfilter dat vóór de reverb wordt toegepast.

Dit is het reverbgedeelte van Clouds, niet de volledige granulaire Clouds-processor.

### Ping-pong Delay

Gebruik **SELECT + [DOWN]** vanuit de Mixer om Delay te openen, en daarna **SELECT + [UP]** om terug te keren. Reverb werkt in tegengestelde richting: **SELECT + [UP]** opent hem en **SELECT + [DOWN]** keert terug naar de Mixer.

- **Return**: niveau van de natte delay.
- **To Reverb**: hoeveelheid van het natte delaysignaal dat naar de gedeelde reverb wordt gestuurd, `0-100%`.
- **Ticks**: delaytijd in tracker-ticks (`9 ticks` = `1 tel` standaard).
- **Feedback**: hoeveelheid kruisfeedback, begrensd tot `95%`.
- **Filter**: laagdoorlaatfilter in het pad van het vertraagde signaal.

De 1e herhaling volgt de stereo-ingang. Latere feedback kruist tussen het linker- en rechterkanaal.

## 11. Project-scherm

Het Project-scherm biedt de opdrachten **Load**, **Save**, **New**, **Export**, **Manage** en **Scale**, samen met metadata voor bestandsnaam, titel en auteur.

### Exportlocatie (Export location)

Exports, stems en bounces worden geschreven naar een projectspecifieke map binnen de reeds bestaande samplesmap van de app:

- **Standaard**: `<app folder>/samples/Exports/<project name>/`. Zolang het project geen naam heeft, is de map `current-project`.
- In de webversie is de standaard `/user/samples/Exports/<project name>/`.
- Op Android staat de samplesmap in de privéwerkruimte van de app.

De rij **Folder** op het EXPORT-scherm toont de actieve bestemming:

- Tik op **EDIT** om met de maplader een eigen map te kiezen. De gekozen map wordt precies zo gebruikt als geselecteerd (er wordt geen projectsubmap toegevoegd) en wordt in de instellingen onthouden.
- **EDIT + OPT** (clear) herstelt de standaardlocatie.

Wanneer een project met naam onder een nieuwe naam wordt opgeslagen, wordt de standaard exportmap overeenkomstig hernoemd, zodat eerder geëxporteerde bestanden bij het project blijven. Hernoemen geldt niet voor een eigen map, en de map wordt niet door automatisch opslaan hernoemd. Als er al een map met de doelnaam bestaat, blijven beide mappen behouden. Als de tracker de map uit het oog heeft verloren (bijvoorbeeld na een herstart), hernoemt het opslaan van een project met naam in plaats daarvan de bestaande map `current-project`. Instrumenten blijven werken na een hernoeming: geladen samplepaden die naar de hernoemde map wijzen, worden automatisch bijgewerkt.

**Load** accepteert ook `.mid`/`.midi`-bestanden, die als nieuw project worden geïmporteerd: noten worden gegroepeerd per MIDI-kanaal (één kanaal per spoor, tot het aantal sporen), gekwantiseerd op 4 rijen per tel en op één standaard AY-instrument geplaatst - MIDI-programmanummers hebben geen chiptune-equivalent, dus kies daarna echte instrumenten. Alleen het eerste tempo van het bestand wordt gebruikt (één globale tickfrequentie, geen tempowijzigingen per sectie). De rij **MIDI** van het Export-scherm doet het omgekeerde: ze schrijft de noten, het volume en het tempo/de groove van het huidige arrangement als Standard MIDI File (één MIDI-spoor per trackerspoor); [MIDI Out](#midi-out)-instrumenten en andere FX per rij buiten volume en de globale groove hebben geen MIDI-equivalent en worden niet vertaald.

**Load** accepteert ook Dirtywave M8-songs (`.m8s`, firmware 2.x-4.x), die als nieuw project worden geïmporteerd. Alleen de structuur en de noten komen mee (songrijen, chains met hun transpositie, phrases met noot, velocity en instrumentnummer, note-offs, tempo en songnaam); noten behouden hun werkelijke toonhoogte (de M8 benoemt octaven twee hoger: zijn `C-6` is hier `C-4`). Elk instrument dat door een phrase wordt gebruikt, wordt geïmporteerd onder de naam van het M8-instrument. M8-**MACROSYN**-instrumenten worden Braids-instrumenten (de 47 vormen zijn de Braids-modellen, met dezelfde nummers; timbre, color, filtertype, cutoff, resonantie en pan worden overgenomen, waarbij de cutoffcurve benaderd is). **SAMPLER**-instrumenten worden Sample-instrumenten: de `.m8s` slaat alleen het pad van de WAV op de M8-kaart op, dus het bestand wordt gezocht onder de map van de `.m8s` en haar bovenliggende mappen (een kopie van de kaartindeling werkt) en in je huidige samplesmap (`Samples` of `samples` werken allebei); als het niet wordt gevonden, behoudt het instrument het M8-pad en blijft het stil. Geïmporteerde samples krijgen +24 halve tonen in hun Pitch-instelling, omdat de M8 een sample op zijn oorspronkelijke toonhoogte twee octaven lager afspeelt dan deze app. **FMSYNTH** wordt een Genesis FM-instrument (grove benadering), **WAVSYNTH** en **HYPERSYNTH** worden aChChid. **MIDI OUT** wordt een MIDI Out-instrument (kanaal, bank, programma en de eerste vier CC-nummers). Andere (externe) typen worden een standaard AY-instrument - kies daarna echte klanken. FX en tabellen worden genegeerd.

Op desktop doet de rij **M8S** van het Export-scherm het omgekeerde, in beperkte vorm: tik erop en kies een bestaande `.m8s` als **sjabloon** (onthouden tot je afsluit, **EDIT + OPT** vergeet hem). De song, chains, phrases, het tempo en de titel worden vervangen en al het andere blijft zoals het is, behalve dat Braids-instrumenten worden teruggeschreven als MacroSynth en Sample-instrumenten als Sampler (pad `/Samples/<file name>`: kopieer de WAV daar zelf naartoe op de M8), mits het sjabloon een instrument van dat type heeft waarvan de overige parameters kunnen worden gekopieerd. FX worden niet geëxporteerd, en een song die phrase `FF` of hoger gebruikt, kan niet worden geëxporteerd (de M8 heeft 255 phrases). Het geëxporteerde bestand is nog niet op een echte M8 getest. Zie [M8 songs](../m8s-format.md) voor de details.

Op desktop kun je met key jazz de bestandsnaam, titel en auteur rechtstreeks op het toetsenbord typen in plaats van het schermtoetsenbord te gebruiken - zie [Key jazz](#key-jazz-alleen-desktop).

### Scale / Quantize

Het Scale-scherm regelt de globale 12-TET-toonladderengine. De rij **Mode** kiest tussen twee modi:

- **Quantizer** (standaard): phrase-invoer blijft chromatisch; wanneer ingeschakeld, worden noten vlak voordat ze de geluidsengine bereiken naar beneden afgerond, zodat het wijzigen van grondtoon of toonladder een song herharmoniseert zonder zijn phrases te bewerken.
- **Note Lock**: in phrases getypte noten worden bij het invoeren op de dichtstbijzijnde noot van de toonladder vastgezet, zodat alleen toonladdernoten kunnen worden geschreven. Nieuwe invoer en verlagingen worden naar beneden vastgezet; `EDIT + RIGHT` / `EDIT + UP` verhogen de noot naar de volgende toonladdernoot erboven. Invoer volgt op dit scherm altijd de projecttoonladder: het wijzigen van grondtoon of toonladder beïnvloedt direct nieuw ingevoerde noten op de ingeschakelde sporen, terwijl reeds opgeslagen noten onaangeroerd blijven. De `SCL`-FX is in deze modus niet beschikbaar en elke al geschreven `SCL` wordt genegeerd. Kwantisatie bij het afspelen van gewone noten wordt omzeild (de invoer is al vergrendeld); akkoordkwantisatie via `CRD` blijft van toepassing.

Terwijl de cursor op **Mode** rust, houdt de statusbalk een vaste beschrijving van de actieve modus zichtbaar: `Quantizes notes on playback` voor **Quantizer**, `Snaps sequencer to scale` voor **Note Lock**. Het omschakelen van de modus vervangt de tekst direct; elke actiemelding vervangt haar totdat ze verloopt.

Beide modi delen dezelfde grondtoon, toonladder, **Custom**-notenbewerking en spoorvakjes. Elk van de acht spoorvakjes bepaalt op welke sporen de modus van toepassing is. De toonladderengine wordt omzeild voor toonhoogtetabellen zonder 12 noten en voor Sampler-instrumenten waarvan de **Slice**-modus EQUAL, AUTO of LAZY is (hun noten kiezen slices chromatisch vanaf C-0); MIDI-invoer maakt geen deel uit van deze versie.

- **Linear pitch** selecteert de modus van de toonhoogtetabel. **Off** is de standaard en de op hardware gevalideerde instelling voor correcte octaafvolging van AY, Braids en Plaits.
- **Tick rate** stelt de tracker-timing in en toont de bijbehorende BPM (`tick rate x 60 / 24`).
- ChooChooTracker slaat projecten op als `.cct`. Dit formaat is niet compatibel met ChipNomad.

Gebruik **Save** voordat je instrumenttypen wijzigt of een ander project laadt.

## 12. Settings

- **Repeat delay / speed** stellen het herhalen van ingedrukt gehouden knoppen in.
- **Stick live mode** kiest `HOLD`, `TOGGLE` of `FREE` voor de reeds toegewezen Stick live-knop.
- **MIDI** opent het submenu [MIDI](#14-midi): apparaatkeuze en de koppeling van MIDI In-kanalen aan instrumenten.
- **Synths** bevat **AY Quality**, **Sample dithering** en de globale instellingen **Braids BITS / DRFT / SIGN**.
- **Mixer** bevat **Mix volume** en de projectspecifieke **Tilt pivot** van `250-4000 Hz` (standaard `1 kHz`) die alle Tilt-regelaars van de Mixer gebruiken.
- **Graphics** bevat **Edit color theme**, **Load font**, **Persistent waveform**, **Track visuals**, **Waveform FPS** en de actieve renderer (`GPU` of `Software`). **Waveform FPS** stelt de verversingssnelheid van de instrumentscope en de mixervisualisaties in van `01` tot `60`. Track visuals kiest **Detailed** (de synthgolfvorm en zijn overlays) of **Audio waveform** (de werkelijke opgetelde uitvoer van dat spoor) onafhankelijk voor elk spoor; de audiogolfvorm wist bij het starten van het afspelen onmiddellijk elke eerdere ADSR-overlay. ChipNomad-lettertypen en -thema's zouden moeten werken.
- **Key mapping** past de bediening aan.
- **Support report** schrijft een klein diagnostisch tekstbestand met de appversie, het platform, audio-instellingen en CPU-belasting, de afspeel-/overloopstatus en een compacte projectsamenvatting. Het bevat geen sampledata of titel-/auteurtekst van het project. Op Android en Web opent het de normale export-/downloadstroom; desktop- en handheldversies slaan `support-report.txt` op in de beschrijfbare map van de app.
- **Quit ChooChooTracker** sluit netjes af.

**Graphics → Boot screen** kiest **Original** (standaard) of **Night**. Tik op EDIT
of gebruik EDIT + richting om te wijzigen; de keuze wordt bewaard voor het volgende
titelscherm. Night gebruikt het oorspronkelijke spoorwegtafereel met zachtere blauwgrijze tinten,
een procedurele Melkweg met subtiel sterrengefonkel en af en toe een vallende ster.
Het oorspronkelijke landschap en de trein behouden hun scrolllussen.

## 13. Prestaties en probleemoplossing

### CPU

De CPU-kosten hangen af van de actieve engines en effecten. Fysische modellen van Plaits en Clouds Reverb zijn zwaarder dan eenvoudige AY-stemmen. Controleer de prestaties op de doelconsole, vooral met 8 Plaits-stemmen en beide sends actief. Een CPU-uitlezing dicht bij `100%` kan knetteren of gemiste audiodeadlines veroorzaken.

De CPU-weergave is een afgevlakte meting van de audiorendertijd. Ze meet geen vertragingen voordat de audiocallback draait of in de audio-uitvoer van het apparaat, dus knetteren kan optreden ook bij een lage uitlezing.

Op Android geeft de audio-uitvoer nu de voorkeur aan AAudio om het wisselvallige knetteren te voorkomen dat met OpenSL ES op de Pixel 7a werd waargenomen. Deze gebufferde uitvoer geeft voorrang aan stabiele weergave en kan enige reactielatentie toevoegen. Het trackerdoek behoudt zijn volledige 4:3-indeling op vierkante en andere ongebruikelijke beeldverhoudingen; ongebruikte ruimte wordt als randen getoond in plaats van kolommen af te snijden. Als het knetteren aanhoudt, meld dan de song, het telefoonmodel, de Android-versie en of je de luidspreker, een koptelefoon of Bluetooth gebruikt.

Als je last hebt van pops, crashes of vertragingen, gebruik dan **Settings > Support report** en voeg het gegenereerde `support-report.txt` bij het foutrapport. Als een specifieke song het probleem veroorzaakt, voeg dan ook die `.cct` bij.

### Geen geluid

Controleer het instrumentnummer, de demp- of solostatus van het spoor, de LVL van het spoor, het Mix volume van de app en de envelope van het instrument. Controleer bij samples of de oorspronkelijke WAV nog op zijn opgeslagen pad bestaat. Druk op ArkOS op **MENU + L3** om het dempen van het besturingssysteem om te schakelen. Als dat niet helpt, herstart dan de hardware en sluit je audio-interface opnieuw aan.

### De bediening voelt verkeerd

Pas Repeat delay en Repeat speed in Settings aan. Als één druk twee keer beweegt, controleer dan dat er maar 1 fysiek bedieningselement aan die richting is toegewezen en meld het exacte scherm en de exacte sneltoets.

## 14. MIDI

Desktop en PortMaster. Settings > **MIDI** opent dit submenu: **MIDI In** / **MIDI Out** kiezen een aangesloten apparaat en doorlopen de gedetecteerde poorten met `OFF` aan beide uiteinden; beide richtingen hebben een gekozen apparaat nodig. De apparaatkeuze geldt per sessie: ze wordt niet in settings.txt opgeslagen, omdat de positie van een poort in de lijst kan veranderen tussen herstarts of wanneer apparaten in een andere volgorde worden aangesloten. De besturing **PLAY** van de app blijft vanaf dit scherm beschikbaar en start de song. Sluit op PortMaster vóór het starten een USB-MIDI-apparaat aan; het moet door het besturingssysteem van de handheld via ALSA beschikbaar worden gesteld.

- **Klanken beluisteren vanaf een MIDI-keyboard**: met een MIDI In-apparaat geselecteerd beluistert het spelen van noten op het aangesloten keyboard op elk scherm een instrument, net als de sneltoets **EDIT + PLAY** op het scherm [Instrument](#5-instruments). Dit is alleen beluisteren - het voert geen noten in de song in.
- **Channel mapping**: opent een lijst van de 16 MIDI-kanalen; wijs elk een instrument toe (standaard `OFF`) zodat op dat kanaal ontvangen noten dat instrument beluisteren, ongeacht welk instrument op het Instrument-scherm is geselecteerd - bijvoorbeeld kanaal `01` -> instrument `05`. Een kanaal dat op `OFF` blijft staan, valt terug op het huidige geselecteerde instrument, het oorspronkelijke gedrag. Deze koppeling wordt in settings.txt opgeslagen.
- **MIDI CC mapping**: koppelt een inkomende controller aan een instrumentparameter, inclusief zijn **Volume**, of aan globale bedieningselementen. Globale rijen tonen `TRK 1`-`TRK 8` in plaats van een instrument en bieden per spoor **Mute**, **Solo**, **Volume**, **Reverb send** en **Delay send**; **Song play/stop** heeft geen doel. Een nieuwe koppeling negeert de CC die haar heeft geleerd/ingesteld en werkt bij de volgende fysieke beweging, dus het instellen van koppelingen tijdens het afspelen kan een patch niet op nul terugzetten. Mute en solo gebruiken `0-63` = off, `64-127` = on.
- **Een extern MIDI-apparaat aansturen**: geef een spoor het instrumenttype [MIDI Out](#midi-out) en stel zijn Channel in; het triggeren van noten op dat spoor stuurt echte MIDI naar het geselecteerde MIDI Out-apparaat in plaats van geluid te maken in ChooChooTracker.

## 15. Dankbetuigingen en licenties

ChooChooTracker is een fork van ChipNomad en behoudt zijn MIT-licentieaanpak. De code van Braids, Plaits, Plaits-Alt, Clouds DSP, de van Warps afgeleide MME-DSP en stmlib is afgeleid van de open-sourcereleases van Mutable Instruments onder de daarvoor geldende MIT-vermeldingen. Plaits-Alt is afkomstig uit de Plaits Lab-fork van lylepmills/eurorack; de daarin bewaarde bronvermeldingen zijn van toepassing. De aChChid-engine gebruikt Open303 van Robin Schmidt, copyright 2009, onder de MIT-licentie. Bogie en Sintered zijn eigen native implementaties. MIDI-I/O gebruikt RtMidi van Gary P. Scavone, onder zijn MIT-achtige licentie. Zie de meegeleverde licentiebestanden voor de exacte naamsvermeldingen.

## Insert-effecten per spoor

Elk van de acht sporen heeft twee seriële slots, **TF1 → TF2**. Open vanuit Instrument met het gebruikelijke gebaar Shift+Up de MOD, en daarna nogmaals Shift+Up voor **F: Insert
FX**. Shift+Down keert terug naar MOD. OPT+Left/Right selecteert het spoor (1-8), zonder
de instrumentkeuze te wijzigen. De modulekop opent de bestaande keuzelijst;
bypass behoudt de module, de ingestelde waarden en de automatisering. Parameters zijn
bytewaarden: linkerkolom 1-4, rechterkolom 5-8. De contexttekst toont de volledige namen en
de gedecodeerde waarden. Oude projecten laden met beide slots OFF.

De navigatieweergave houdt de volledige kolom F-M-I-P zichtbaar op alle
instrumentschermen, ook Insert FX en de instrumentpool, en markeert
het huidige scherm.

| Module | Parameters in volgorde |
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

Continue regelaars gebruiken 00-FF, inclusief de geldige nul en het maximum FF. Bipolaire
regelaars hebben een exacte neutrale stand bij 80. De distortionmodi zijn 00 Density, 01 Drive,
02 Spiral, 03 Mojo, 04 Dyno. Compressorverhoudingen 00-07 zijn 1,5, 2, 3, 4, 6, 8, 16,
20:1; detectorbronnen 00-02 zijn stereo-ingang, links, rechts. Detector filter 80 is
neutraal, eronder is laagdoorlaat, erboven is hoogdoorlaat. Er is geen externe sidechain
of sidechain tussen sporen. Elk slot kan elke module bevatten; herhaalde modules zijn onafhankelijk.
Chorus, Flanger, Phaser en Rotary gebruiken milde modulatie-standaardwaarden. Rotary Speed
gebruikt het volledige bereik **00-FF**: **00 = 0,01 Hz** (één rotatie per 100 seconden),
**FF = 25 Hz** (het voormalige Chaos-maximum). Exponentiële spreiding geeft nauwkeurigere
controle over langzame LFO-bewegingen. Nieuwe Rotary-inserts staan standaard op **8F**, ongeveer
0,8 Hz. Dit vervangt de vier discrete snelheidswaarden; bestaande Rotary-instellingen
en F11/F21-snelheidsautomatisering moeten opnieuw op de nieuwe schaal worden afgesteld (oude Slow/Fast/Hyper/
Chaos komen bij benadering overeen met **8F/CB/E7/FF**). Andere parameters zijn ongewijzigd.
De bitdiepte van Bitcrusher loopt van 4 tot 16 bits en
de ratereductie van 1x tot 32x. De Destruction-modi zijn Fold, Clip en Crush.
Saturation en distortion bevatten uitgangsniveau- of mixregelaars om ze in balans te brengen
met het onbewerkte signaal.
Parametric EQ heeft 24 dB boost/cut-shelves bij 60-500 Hz en 2-14 kHz, plus een
bel met vaste Q van 0,9 bij 250 Hz-5 kHz. De gainregelaars zijn neutraal bij 80; als alle
drie neutraal zijn, is het een exacte bypass. De actieve EQ gebruikt een transparante soft knee boven
full scale en bereikt een maximum van 1,5 om extreme boosts te beperken.

Spoorniveau en tilt-EQ gaan vóór de inserts, dus beide beïnvloeden het signaal dat de
compressie en distortion binnenkomt. Alle stemmen/akkoordnoten van het spoor voeden één insertketen.
Zowel de reverb- als de delaysend ontvangen de uitvoer ervan. Gedeelde effecten, returns,
mastercontroles en mute/solo behouden hun bestaande gedrag. Inserts verwerken geen
audio van externe MIDI-apparaten.

Insertopdrachten gebruiken **F11-F18** voor TF1 en **F21-F28** voor TF2. De FX-selectie van de roll
en het Phrase/Table-popup bieden alleen parameters aan die door de geconfigureerde inserts van het
geselecteerde spoor worden gebruikt. Het popup groepeert ze als **TF1: effectnaam** en
**TF2: effectnaam**; OFF-slots zijn verborgen. Gebypasste inserts blijven bewerkbaar.
In het FX-popup van Phrase en Table volgt de beschrijving het insertslot van het geselecteerde spoor: ze toont de module, de parameternaam, de gedecodeerde eindpunten van het bereik en de
opgeslagen basiswaarde. Bestaande songopdrachten blijven behouden als een modulewijziging
ze ongebruikt maakt; bij het openen van hun kiezer wordt begonnen bij een beschikbare regelaar.
Tijdens het bewerken van een waarde toont de hint onderaan de gedecodeerde waarde en eenheden.
Elke FX-kolom van Phrase of Table kan beide slots aansturen; het Phrase-raster houdt zijn
drie kolommen. Deze absolute runtimewaarden blijven bestaan over noten, instrumenten,
phrases, chains en gewone lussen heen. Een nieuwe afspeelbeurt/harde stop zet ze terug. Het bewerken
van een basiswaarde op de F-pagina wist alleen de override van die parameter. MOD werkt op de
effectieve basis-/automatiseringswaarde zonder zijn offsets op te slaan. Het selecteren van een nieuwe
module herstelt de standaardwaarden en wist de overrides van dat slot. Bypass behoudt
bestaande automatisering terwijl de verwerking na een korte fade wordt opgeschort.

**Adressen verwijzen naar spoorslots/parameterposities, niet naar benoemde effecten.** Een
Phrase die op een ander spoor wordt hergebruikt, gebruikt de modules van dat spoor. Het wisselen van modules kan
de betekenis van een opdracht veranderen; opdrachten worden niet herschreven. OFF-/ongebruikte adressen
zijn veilige no-ops en blijven intact in opgeslagen Phrase-/Table-data.

De MOD-keuzelijst van het instrument krijgt een categorie Insert FX. De bronnen van elk instrument
sturen de inserts aan van het spoor dat het afspeelt, inclusief het gecombineerde akkoordsignaal.
Hetzelfde instrument op twee sporen beïnvloedt onafhankelijke ketens. De bestaande timing van bronnen,
getekende hoeveelheden, regels voor bronnencombinaties, stickmodi en bewegingsopname
zijn van toepassing. Discrete regelaars worden gekwantiseerd en begrensd tot geldige keuzes. Er is geen
nieuwe modulatiebank die eigendom is van het spoor.

De makeup van de compressor begint bij 0 dB. OTT begint bij ongeveer 25% depth, 25%
upward, 50% downward, 0 dB input en -3 dB output. De bandthresholds, gains en
crossovers blijven op de oorspronkelijke standaardwaarden van Rui-727; Depth is de wet/dry-regelaar.
TAPESCAM behoudt de oorspronkelijke hulpstandaardwaarden: nieuwe/hogesnelheidstape, ruis en
compressie uit, verbreding aan. Let op bestaande clippingindicatoren wanneer je
drive/gain toevoegt. Slotovergangen gebruiken begrensde fades van 5 ms via droog audiosignaal.

Opslagbestanden bevatten module-ID's, bypass en geconfigureerde bytes in de optionele, van versies voorziene
secties `Track inserts: 1,8,2` en `Master inserts: 1,2`. Runtime-overrides
en DSP-geschiedenissen worden niet opgeslagen.
Bewaar back-ups voordat je opslagbestanden met inserts in oudere builds opent. DSP-bronvermelding,
bronrevisies en aanpassingen staan in `chipnomad_lib/external/insert_fx/SOURCES.txt`;
de volledige MIT-vermeldingen worden meegeleverd in `licenses/INSERT_FX.txt`.

Op een R36H bij 48 kHz kostte een gemeten paar inserts op één spoor ongeveer 4-5%
van de realtime voor Compressor, 6% voor Distortion, 18% voor Doubler, 16% voor TAPESCAM
en 24% voor OTT, vóór de kosten van de synths en gedeelde effecten. Zestien actieve
exemplaren van Doubler, TAPESCAM of OTT overschreden de realtime. Modulatie op audiosnelheid voegt
nog meer kosten toe. Begin met een paar inserts, let op audio-overbelasting en zet
ongebruikte slots in bypass; de zestien beschikbare posities zijn geen gegarandeerd CPU-budget.

De Insert-pagina houdt de tip van het geselecteerde veld zichtbaar nadat een knop is losgelaten;
tijdelijke meldingen kunnen die nog steeds vervangen. De modulekeuzelijst toont van elke
module het effecttype en het bronproject naast de naam.

### Native OPLL- en VRC7-instrumenten (ontwikkeling)

De instrumenttypekiezer heeft een FM-groep met **OPLL / MSX** (YM2413)
en **VRC7** (DS1001). Elk biedt nu **73 programma's** in scrollende Bank-/Preset-
lijsten: de oorspronkelijke 15 klanken, 40 extra afzonderlijke klanken uit de
YM2413/VRC7/YMF281B-tabellen van emu2413, en 18 door ChooChoo gemaakte tweeoperatorprogramma's.
Exact dubbele toonbytes worden binnen elke engine weggelaten. De extra
paletten gebruiken het programmeerbare klankslot van de chip; ze vergroten de fysieke
ROM niet. Programma nul geeft een eigen klank aan. EDIT + links/rechts selecteert de
vorige of volgende preset binnen het bankfilter. Fine ct past de stemming aan van
-100 tot +100 cent.

In de programmalijst beluistert EDIT + PLAY de gemarkeerde klank; laat de
knoppen los om te stoppen. EDIT bevestigt en OPT annuleert. Bladeren/beluisteren wijzigt niet
het instrument, de tabel of de song. Het normale beluistergebaar van de Instrument-pagina
blijft werken na de selectie. Native chip-envelopes verzorgen attack/release;
de optionele Amp env voegt een software-ADSR toe (zie Native chip controls hieronder).
Er is geen volledige operatoreditor.

Nieuwe instrumenten slaan alle acht native toonbytes, bank/naam, programma en fijnstemming op in
het song-/instrumentbestand. Bestanden met deze typen gebruiken formaat 7.0 en vereisen
deze build of nieuwer. Eerdere formaten blijven leesbaar; songs zonder deze typen
worden nog steeds als 5.0 opgeslagen. Ritmeprogramma's zijn uitgesteld. De uitgebreide bibliotheek is
op de host gevalideerd; luisteren door een mens en een nieuwe controle op een handheld staan nog open.

### AdLib / OPL2 en OPL3 (ontwikkeling)

FM omvat ook AdLib / OPL2 (YM3812) en OPL3 (YMF262). Bank filtert de
fabriekslijst; Preset opent categoriegroepen en een All-weergave. EDIT + PLAY beluistert
vóór het selecteren, EDIT selecteert, OPT annuleert, en EDIT + links/rechts op Preset
laadt de vorige/volgende overeenkomende vermelding. Fine ct past de lokale stemming aan. Mode toont
2 operator, 4 operator of Dual voice. OPL2 verbergt incompatibele OPL3-patches;
OPL3 kan de gedeelde collectie met twee operators afspelen.

De titels van de bank- en presetpopups geven de huidige engine aan. Het wisselen van engine of
instrumentslot zet het bankfilter terug op **All banks**, ook bij het wisselen
tussen de compatibele AdLib- en OPL3-engines. Terugkeren uit een popup binnen hetzelfde
slot behoudt het gekozen filter. Het annuleren van een DX7-bankimport herstelt het
vorige filter.

Het bevestigen van een preset publiceert ook bewerkingen die bij het loslaten van de knop zijn gemaakt naar de volgende
UI-/audiotick. Songs die spelen hebben geen extra knopdruk of herstart
van de transportfunctie meer nodig om die patch te ontvangen. De uitvoer volgt nog steeds de ingestelde audiobuffer
en het envelope-/retriggergedrag van de engine.

De fabriekscollectie bevat 697 bronvermeldingen uit The Fat Man 2-op,
The Fat Man 4-op en DMXOPL3, met 589 genormaliseerde unieke patchidentiteiten.
Aliassen behouden bronnamen en naamsvermelding. De oorspronkelijke operatorniveaus, LFO-diepte-vlaggen,
nootoffsets, vaste percussietoonhoogtes en dual-voice-stemming
blijven behouden. Trackervolume gebruikt softwareamplitude; het reproduceert niet
de volumecurves of velocity-offsetregels van de oorspronkelijke MIDI-spelers. Schattingen van de
releaseduur uit de bron knippen aangehouden noten niet af. Sommige effecten hebben
trage attacks: houd Seashore ingedrukt in plaats van te verwachten dat een korte klik hem laat horen.

De fabrieksbestanden staan in `instruments/chips` naast de bestaande instrumentbibliotheek,
met vermeldingen onder `licenses/chip-banks`. Het selecteren van een preset laat de tabel en de aan
het spoor gekoppelde inserts van het slot ongewijzigd. Nieuw opgeslagen bestanden gebruiken 7.0 en
bevatten de volledige klank. Fabrieksaudio is machinaal getest op host en ARM64; beluistering door een mens
staat nog open.

### Native instrumenten voor Sega PSG en Game Boy (ontwikkeling)

CHIP omvat nu Sega PSG, GB Pulse en GB Noise. Sega gebruikt de NTSC-master-
klok en zijn 16-bits ruisfeedback, inclusief ruis afgeleid van het tonenkanaal.
De standaard **Bass range: Extended** verlaagt de virtuele klok waar nodig om
onder de deelgrens van de chip van ongeveer 109 Hz te kunnen spelen. Zo blijven A-2, G-2
en F-2 onderscheiden. Kies **Chip** voor het oorspronkelijke bereik, waarbij lagere noten
samenvallen op de deelgrens. Ruis met vaste snelheid behoudt de oorspronkelijke klok;
van de toon afgeleide ruis volgt de uitgebreide toonhoogte. Oude Sega-instrumenten laden met
Extended ingeschakeld; de instelling wordt met het instrument opgeslagen.
De GB-instrumenten gebruiken DMG-pulse-/ruisregisters; er wordt geen Game Boy-golfkanaal
getoond. Hun zelfgemaakte presetlijsten bevatten nu respectievelijk **24, 24 en 28** presets.
De toevoegingen omvatten gekoppelde/periodieke Sega-ruis, GB-pitchsweeps,
korte percussie, metaalachtige ruis, drones en stijgers. Dit zijn oorspronkelijke programma's,
geen uit games gehaalde geluiden.
De pagina biedt native regelaars voor mode/duty/width, envelope/sweep/ruis voor zover
van toepassing, plus een software-amplitude-ADSR. Deze worden opgeslagen in native bestanden van versie 6.
Geautomatiseerde ARM64-prestatie-/audiocontroles slagen binnen de gemelde
belastingsgrenzen; luisteren door een mens staat nog open.

### DX7 FM (ontwikkeling)

Selecteer **Type → FM → DX7 FM**; DX7 is het zevende item, na Arcade / YM2151.
DX7 FM gebruikt een MSFA-kern met zes operators. De bestaande FM-pagina biedt
Bank, Preset en Fine ct naast de gewone instrumentinstellingen. Het bladeren door presets
gebruikt bank-/categorielijsten. Houd EDIT+PLAY ingedrukt om de gemarkeerde klank te horen; loslaten
stopt het beluisteren. EDIT alleen bevestigt bij het loslaten; OPT annuleert. Het laden van een klank
kopieert de volledige patch zonder trackertabellen of insert-FX van het spoor te wijzigen.

De fabriekscatalogus bevat momenteel **67 afzonderlijke DX7-parameterpatches**:
31 originele muzikale klanken van OpenDX7, vier unieke CC0-klanken van YSE (zijn 32 bankslots
herhalen die vier met andere namen), en 32 door ChooChoo gemaakte patches.
"Origineel" betekent hier: parameterprogramma's die voor dit project zijn gemaakt; het betekent niet
originele fabrieksinhoud van Yamaha. YSE wordt als vier klanken meegeleverd, niet als 32
kunstmatig verschillende namen.
Categorieën beschrijven de beoordeelde geluidsontwerpintentie. Dubbelzinnige namen blijven
Unsorted. Numerieke speelbaarheidscontroles zijn geslaagd; acceptatie door luisteren
staat nog open. Het afzonderlijke doel van 1.000 voor herdistributie vrijgegeven klanken is niet gehaald.

Voor een blijvende persoonlijke bibliotheek zet je `.syx`-bestanden in
**`instruments/banks/dx7/`** naast de bestaande instrumentbibliotheek. Submappen
worden ondersteund. Open **DX7 → Bank**: elk bestand verschijnt als een benoemde bank, klaar voor
het bladeren door presets. Open Bank opnieuw na het toevoegen of verwijderen van bestanden; er is geen conversie of
Load Instrument-stap nodig. Een standaard originele DX7/TX7-bank bevat
**32 voices**. Een bestand met vier bankberichten verschijnt als vier genummerde banken
(128 voices in totaal). Bestanden met één voice worden ook geaccepteerd. De native applicatie
leest deze map; de browserbuild leest zijn virtuele bestandssysteem, niet willekeurige
mappen op de computer.

Slechte of niet-ondersteunde bestanden worden overgeslagen met een telling op het scherm; geldige banken blijven
beschikbaar. Het scannen is begrensd tot 2.048 bestanden, 1 MiB per bestand, 64 MiB in totaal,
60.000 voices en acht niveaus van geneste mappen. Symlinks worden genegeerd. De
geselecteerde patch is eigendom van het instrument: het verwijderen van de bronbank kan de
opgeslagen song niet wijzigen. De volledige bladerbibliotheek blijft in deze map en
wordt niet in zijn geheel naar elk project gekopieerd.

LOAD INSTRUMENT accepteert ook `.syx` originele DX7/TX7-dumps van één voice en van banken met 32 voices,
inclusief begrensde reeksen van die ondersteunde berichten. Het controleert
framing, bytetellingen, zevenbitsdata, checksums, parameterbereiken en bestandsgrootte
voordat de geïmporteerde bank in dezelfde FM-browser wordt geopend. Het selecteren van een patch
bevestigt; annuleren behoudt het huidige song-instrument. Deze directe importsnelkoppeling
behoudt zijn bladerlijst voor de sessie; gebruik de bankmap hierboven voor blijvend
bladeren. Sla de song of een `.cni` op om geselecteerde/bewerkte patches te behouden. Dumps zonder header,
foute checksums, DX7II-performances/-extensies en andere Yamaha-families worden
geweigerd. Import verstuurt nooit MIDI-berichten naar externe apparatuur.

CNI en projectversie 6 slaan alle 155 originele voice-bytes, de volledige weergavenaam,
de native aanslagvelocity, fijnstemming en bronidentiteit op. Songs hebben geen
externe bank nodig om weer te openen. Oudere ChooChoo-releases kunnen deze bestanden van versie 7
niet lezen; projecten met alleen bestaande typen worden nog als versie 5 opgeslagen. De native velocity staat standaard
op 100; trackervolume is gain na de synthese en laat de envelope niet opnieuw aanslaan.
Operator-envelopes, de modus met vaste frequentie, keyboard scaling, pitch-envelope en
LFO blijven native patchgedrag. De optionele gemeenschappelijke FM-amplitude-ADSR vormt
hun gecombineerde uitvoer zonder de operator-envelopes te vervangen.

Elk sporendeel heeft één LFO en vier afzonderlijk beheerde akkoordstemslots, in overeenstemming met
het bestaande akkoord-/vervangingsbeleid van de tracker. Sporen delen geen nootstatus.
Note-off laat de native envelopes los; cut/panic wist ze. De scalaire MSFA-
kern gebruikt kwanta van 64 samples bij een vaste interne 44,1 kHz, met een streaming FIR naar
de uitvoersnelheid. Opzoektabellen worden eenmalig buiten het audiorenderen geïnitialiseerd, zodat
gelijktijdige offline/live renderers elkaars snelheid niet kunnen wijzigen. Events
werken vanaf het volgende interne kwantum (tot 1,45 ms), gevolgd door de FIR-groepsvertraging
van ongeveer 0,25 ms. Gebufferde samples blijven over callbacks heen behouden.
Dit is de MSFA Modern-lijn, geen bewering van bitidentieke DX7-hardware of van een
Dexed Mark I-emulatie. DX7 is beperkt tot **16 actieve noten in de hele song**,
inclusief release-staarten, met behoud van vier eigen akkoordslots per spoor.
Bij overschrijding van het budget neemt hij eerst release-staarten, dan de zachtste aangehouden noten,
dan nieuwe aanslagen. Gelijke aanslagen behouden grondtonen over sporen heen vóór akkoord-
uitbreidingen, met stabiele slot-/spoorvoorrang bij gelijke stand. Dit beleid geldt op elk platform, zodat
dezelfde song dezelfde begrensde nootverdeling heeft. Het verandert de polyfonie van andere instrumenten niet.
Preset-beluistering is uitgeschakeld tijdens het afspelen.
De limiet is gekozen op basis van R36H-metingen en gevalideerd in gemengd afspelen.

#### FM-operatoreditor

Genesis FM-, Arcade FM-, OPL2-, OPL3-, OPLL- en VRC7-instrumenten hebben een **EDIT**-
knop rechts van Fine ct. Die opent een tabel met de patchparameters:

- Genesis FM / Arcade FM: algoritme, feedback, pan, AMS, PMS, operatormasker en
  LFO-instellingen, daarna MUL, DT, TL, KS, AR, D1R, D2R, RR en D1L voor de vier
  operators, plus SSG (Genesis) of DT2 en AM (Arcade).
- OPL2 / OPL3: feedback, connection en pan per voice, deep vibrato en tremolo,
  daarna MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS en KSR per
  operator. Op OPL3 kan hier de modus (2op, 4op, dual) worden gewijzigd.
- OPLL / VRC7: de eigen toonregisters (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Het bewerken van een ROM-preset maakt er
  "Custom" van.

Left/Right en de gebruikelijke bewerkingstoetsen wijzigen waarden; het instrument blijft spelen
zoals gewoonlijk, zodat je de wijzigingen kunt horen. De statusbalk beschrijft de parameter onder
de cursor. OPT of Left+Shift keert terug naar het instrumentscherm. DX7 en SID
hebben geen operatoreditor.

Genesis FM (YM2612) en Arcade FM (YM2151) gebruiken nu dezelfde FM Bank/Preset-
browser, EDIT+PLAY-beluistering, bevestig-/annuleerbediening en fijnstemregelaars. Genesis heeft nu
**73 presets**: de oorspronkelijke 24 en 49 ondersteunde melodische programma's uit de CC0-bank
16-Bit FM Music Station van NeoSoundFonts. Arcade heeft **81 presets**: de
oorspronkelijke 24 en 57 ondersteunde, klinkende programma's uit de GPLv3-collectie van
YMulator-Synth. Het oorspronkelijke ChooChoo-paar deelt zijn onderliggende recepten; de nieuwe
broncollecties bieden afzonderlijke paletten. Dit zijn benoemde geluidsontwerpcollecties,
geen beweerde nabootsingen van bepaalde game-soundtracks. Niet-ondersteunde
nootoffsets, vaste percussietoetsen, arcade-ruisinstellingen en stille bronprogramma's
worden als uitsluitingen vastgelegd in `expansion-manifest.json`. Hun volledige
patch met vier operators, envelope-, LFO-, stereo- en stemmingsinstellingen reizen mee binnen instrumenten en songs van versie 6.
De native chip-envelope regelt de release; gewone trackergain en toonhoogte
laten hem niet opnieuw starten. De Genesis DAC-uitvoer gebruikt een DC-blokkeerfilter van 20 Hz. Ruismodus en
de speciale frequentiemodus van kanaal 3 vallen buiten deze instrumentimplementatie.

Voor een lokale externe bank gebruik je `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`. Het accepteert strikte
TFI van 42 bytes, VOPM OPM-tekst, het geverifieerde WOPLX-formaat en originele DX7-SysEx.
Laad de resulterende `.cni`-bestanden via Load Instrument. Gebruikersimporten worden gescheiden
gehouden van de verspreidbare fabrieksbibliotheek. OPM-bestanden met ruis ingeschakeld,
niet-nul ruisfrequentiedata of gedeeltelijke panning worden geweigerd met een uitleg;
VOPM-panwaarden 0/64/127 worden links/beide/rechts. Binair WOPL wordt niet ondersteund.
De gedeelde FM-catalogus bevat **1.064 vermeldingen**: 697 OPL, 67 DX7, 146
OPLL/VRC7, 73 Genesis en 81 Arcade. Nog eens 76 bestanden maken de Sega-/Game Boy-
presets beschikbaar via de normale bestandsbrowser, voor **1.140 meegeleverde native presets**.
Bronrevisies, hashes, volledige vermeldingen en oorspronkelijke brongegevens begeleiden de
nieuwe collecties onder `licenses/chip-banks/expansion`.

### Draagbare fabriekscollecties en USER-presets

Native chipinstrumenten gebruiken **Bank:** om ALL, een meegeleverde collectie of
USER te kiezen. **Preset:** kiest een klank binnen die selectie. Houd EDIT ingedrukt en druk
op Left of Right op een van beide rijen om te doorlopen; een korte tik op EDIT opent de keuzelijst.

Meegeleverde collecties zijn ZIP-pakketten onder `instruments/FACTORY/`. USER-bestanden gaan
onder `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` of `gb-noise`.
Blader direct door mappen of zet er een ZIP met compatibele presets neer.
ZIP-bestanden blijven intact op schijf. Al deze mappen accepteren compatibele `.cni`-bestanden; CNI is
het draagbare instrumentformaat van ChooChoo/ChipNomad, geen standaard van een chipfabrikant.
De USER-browser leest ook de volgende bronformaten, ook binnen ZIP-bestanden:

| USER-map | Aanvullende formaten |
| --- | --- |
| `dx7` | Yamaha DX7/TX7 `.syx` enkele voices en banken met 32 voices |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | VOPM `.opm`-banken, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | Furnace `.fui` eigen patches en vaste ROM-selecties |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

Een bank opent als compatibele voices. Niet-ondersteunde bestanden of functies melden een
reden; incompatibele voices in een gemengde bank worden uitgesloten terwijl hun bronnummers
stabiel blijven. Laden is transactioneel: een geweigerde preset laat het
huidige instrument ongewijzigd. Bronpresets vervangen de trackertabel van het instrument
door een lege, behalve DX7 SysEx, dat zijn bestaande tabel behoudt.
CNI herstelt zijn opgeslagen tabel. Geladen patches zijn eigendom van de song en blijven
werken nadat het bronpakket is verplaatst of verwijderd.

WOPL-banken kunnen ongebruikte slots bevatten zonder expliciete lege-vlag. De browser
laat naamloze slots weg waarvan de actieve operators volledig gedempt zijn en geen
attack hebben. Naamloze speelbare voices blijven beschikbaar als `Program N [Bbank]` of
`Drum N [Bbank]`, met de nulgebaseerde programma- en banknummers van de bron.
Een naamloos enkel OPLI-instrument gebruikt zijn bestandsnaam. Benoemde stille patches blijven
behouden.

Furnace-ondersteuning omvat oude versies met één instrument 29-112 en op functies gebaseerde
`FINS`-versies 127-251. DefleMask-ondersteuning omvat versie 11 OPN-, OPM-, Sega PSG-
en Game Boy-instrumenten. Dit zijn instrumentimports, geen volledige trackerspelers: geanimeerde
FM-/operatormacro's, FM-LFO-instellingen van de bronsong, samples,
hardware-opdrachtreeksen van de Game Boy, relatieve SID-pulse-/filtermacro's en SID-
ring-/sync-afhankelijkheden worden momenteel geweigerd. Constante OPLL-ROM-selectie wordt
ondersteund; VRC7 gebruikt voor die selectie zijn eigen ROM. SID-Wizard `.swi` en SID-
muziekbestanden (`.sid`) zijn in deze build geen instrumentimports.

Ondersteunde PSG-/GB-/SID-reeksen worden in de patch opgeslagen en afgespeeld op 60 Hz voor
Furnace/DefleMask en 50 Hz voor GoatTracker. Die bestanden bevatten niet de volledige
afspeelconfiguratie van een song: gebruik de oorspronkelijke tracker wanneer de songspecifieke
tickfrequentie, kanaalroutering, hard-restartgedrag of compatibiliteitsinstellingen
nodig zijn. Voor Sega PSG kiest een duty-macro de ruisstem; zonder macro gebruikt de
import een toonstem. Game Boy-bestanden moeten in de bedoelde pulse- of
noise-map worden geplaatst. Geïmporteerde reeksen blijven vooralsnog ingebedde afspeeldata; de
instrumentpagina bewerkt basisparameters, niet de brontrackertabellen.

Het doorlopen van USER-presets gaat verder over bestanden, mappen, banken en ZIP-pakketten heen in
beide richtingen, en begint aan de uiteinden weer opnieuw. Het heropenen van de USER-browser volgt de
huidige selectie. ALL bevat USER-klanken onder Unsorted voor engines met categorieën;
USER blijft een aparte keuze op het hoogste niveau. Sega PSG en Game Boy gebruiken
platte ALL-/Factory-lijsten en een hiërarchische USER-browser, zonder Unsorted-
paneel. Oudere DX7-bestanden in `instruments/banks/dx7/` blijven toegankelijk in USER.

Een engine toont alleen zijn compatibele meegeleverde collecties. Factory Presets
combineert de meegeleverde OPLL-/VRC7-klankensets voor de geselecteerde engine; DX7 combineert
de originelen van ChooChoo en YSE. Gedownloade benoemde collecties blijven gescheiden.
Fat Man 2-op staat onder OPL2 en Fat Man 4-op onder OPL3. Compatibele OPL2-
CNI-bestanden kunnen nog steeds in OPL3 USER-mappen worden gebruikt.

De uitvoer van native engines gebruikt vaste, gemeten gaincompensatie in plaats van
normalisatie per preset. Bestaande songs met native instrumenten kunnen daardoor op
een ander niveau klinken; controleer de balans van opgeslagen mixen. De kalibratiemethode
en de grenzen zijn gedocumenteerd in `scripts/README_MEASURE.md`.

`native-chip-audition.cct` biedt een korte opeenvolgende beluistering van de oorspronkelijke
dertien fabrieksbanken. Elke sectie gebruikt een ander eigen instrument, dus het werkt ook met
de presetmap verwijderd. Langere, genegeerde WAV-bestanden per bank en gemeten niveaus staan
in `docs/chip-preset-auditions.tsv`; subjectief luisteren staat nog open.

### Native chipregelaars

De golfvormvoorbeelden van native instrumenten worden ververst wanneer hun regelaars opnieuw worden getekend.
FM-instrumenten kunnen hun optionele Amp env-curve eroverheen leggen; SID behoudt zijn golfvorm-
voorbeeld zonder die FM-only overlay, ook bij het bladeren door banken en presets.

Alle zeven FM-engines bieden **Bright**, **Feedback** en **Amp env**. Bright
loopt van -63 tot +63, waarbij nul de patch behoudt; het wijzigt de niveaus van de
modulatie-operators terwijl de niveaus van de carriers behouden blijven. Het hoorbare effect hangt af van
het algoritme. Feedback staat standaard op **Preset**, of overschrijft de native feedback
met 0-7. Deze regelaars herschrijven de opgeslagen native operatorbytes niet.

Amp env staat standaard op **Bypass**. Selecteer **ADSR** om attack, decay, sustain,
release en shape rond het native geluid toe te voegen. A/D/R gebruiken het gedeelde kwadratische bereik van 0-5 seconden;
sustain loopt van stilte tot vol niveau. Native envelopes blijven
draaien, dus deze envelope kan een geluid niet verlengen voorbij zijn native release. Het bladeren
door presets behoudt de FM-amp- en toonregelaars van het slot. Alle FM-voices gebruiken ook
een onset-/retriggerovergang van 3 ms en 1 ms gladstrijken van trackergain, ook
wanneer Amp env in bypass staat. Harde cut/panic blijft onmiddellijk.

De volgende phrase-FX verschijnen ook als ondersteunde modulatie- en bewegingsopname-
bestemmingen. Waarden in de FX-kolom zijn hexadecimaal. Instellingen werken op het afspelen
zonder het opgeslagen instrument te wijzigen. Native GB-sweep- en envelope-regelaars
worden vastgelegd bij de volgende nootactivering; duty/width en ruisfrequentie kunnen live veranderen.

| Engines | FX | Regelaar |
|---|---|---|
| Native FM | `OL1`-`OL6` | Absolute operatoruitgangsniveaus: OPLL/VRC7-modulator en OPL `00-3F`; OPLL/VRC7-carrier `00-0F`; Genesis/Arcade `00-7F`; DX7 `00-63` (0-99). Hoger betekent meer uitvoer. Alleen ondersteunde operators verschijnen. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Attack, decay, release en sustaindemping van operator 1, `00-0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | Frequentievermenigvuldiger van operator 1. |
| Genesis/Arcade | `LFR` | Native LFO-snelheid: Genesis `00-07`, Arcade `00-FF`. |
| Arcade | `LAD`, `LPD 00-7F` | Afzonderlijke LFO-amplitude- en toonhoogtediepte. |
| Genesis/Arcade | `LAS`, `LEN` | Native amplitudegevoeligheid `00-03` en LFO-activering `00-01`. |
| Genesis/Arcade | `LPS 00-07` | Native toonhoogtegevoeligheid. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | Native attack, decay, sustain en release. Tijdwaarden nemen toe richting `0F`; sustain neemt toe richting vol niveau. |
| SID | `SPR 01-10` | Frequentie van de stille partner van 1× tot 16×; beïnvloedt ringmodulatie en hard sync. |
| Alle native FM | `FBK 00-07` | Absolute native feedback, geïnitialiseerd vanuit het instrument. |
| Alle native FM met Amp env ingeschakeld; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack, decay, sustain, release, shape. Deze schakelen een gebypasste FM-amp niet in. |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | Toon / witte ruis / periodieke ruis; drie vaste ruissnelheden of van de toon afgeleide snelheid. |
| GB Pulse | `CMD 00-03` | Native pulse duty. |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | Ruisbreedte, klokdeler en shift. |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | Sweepperiode, shift, neerwaartse richting. |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | Native envelope: beginniveau, periode, stijgende richting. |

De selectie van native FX gebruikt het instrument in de kolom `I` van de phrase-rij, of het
actieve instrument dat achterwaarts wordt gevonden wanneer `I` leeg is. Het FX-popup
toont in een compact blok een korte beschrijving van de regelaar, zijn huidige presetwaarde en het geldige opdrachtbereik. Het selecteren van een ander
native effect begint bij die waarde; het heropenen van hetzelfde effect behoudt zijn
bewerkte waarde. Een selectie van meerdere rijen bepaalt het instrument van elke rij afzonderlijk.
Tabellen gebruiken hun instrumentcontext. Native waardebewerkingen stoppen bij hun wettelijke
eindpunten, inclusief duty, ruis, sweep, ADSR, feedback en operatorniveaus.
De waarden zijn hexadecimaal: het maximum `63` van DX7 betekent bijvoorbeeld decimaal 99.

Deze FM-opdrachten gebruiken absolute native waarden. Operatoropdrachten richten zich altijd op
operator 1; `OMU 03` selecteert zijn vermenigvuldiger 3. De trackerspecifieke regelaars van DX7
zijn operatorniveaus en feedback. Preset/Range behoudt zijn informatiekleur;
titels en beschrijvingen volgen dezelfde kleuren als FM Feedback.

Live Modulation behoudt zijn volledige set native bestemmingen met vaste operators.
Bewegingsopname schrijft alleen regelaars die in de compacte trackerlijst staan;
operatorparameters worden alleen voor operator 1 opgenomen. Andere modulatiebestemmingen
blijven live werken maar genereren geen phrase-opdrachten.

`SCP` en `SCT` behouden bytegeschaalde koppelingen naar SID-registers die breder zijn dan 8 bits.
`SMR 01-C8`, `SWV 01-08` en `SPR 01-10` komen nu overeen met de native presetnummering.
Weergegeven presetwaarden zijn basisinstellingen, onafhankelijk van de afspelende envelope
of LFO. `SLE` werkt op operatorniveaus en de directe FM-parameters zonder
de noot opnieuw te activeren of de opgeslagen patch te wijzigen.

Alle native FX hebben beschrijvende titels, bereiken en gedrag in de phrase-
FX-keuzelijst, plus waardehints. `CMD` beschrijft toon/ruis, duty of ruisbreedte
afhankelijk van het geselecteerde instrument. Deze regelaars vullen de gedeelde
groepen Track, Envelope en Modulation aan; de engine-groep alleen is niet de volledige
set beschikbare phrase-effecten.

De bestaande gedeelde LP/BP/HP-filters op Braids, Plaits, PCM en andere ondersteunde
engines zijn softwareverwerking na de synthese. aChChid gebruikt in plaats daarvan zijn native
303-filterpad. Deze ronde voegt geen filter toe aan AY of native FM; helderheid verandert
de FM-synthese zelf. Track-inserts blijven beschikbaar voor extra verwerking.

Native instrumenten worden opgeslagen in CNI-versie 7 (8 wanneer absolute opdrachten aanwezig zijn),
en native songs in CCT-versie 9. Instrumenten met geïmporteerde reeksen
gebruiken CNI-versie 9 en songs die ze bevatten gebruiken CCT-versie 10. Oudere builds
kunnen die nieuwere bestanden met reeksen niet openen. Opgeslagen FM-presetbytes en
toon-instellingen van de instrumentpagina blijven ongewijzigd.

### Richtlijnen voor de werklast van native chips op handhelds

Er zijn acht songsporen en **twee insert-effectslots per spoor**. De
zestien beschikbare slots garanderen niet genoeg CPU om zestien effecten te draaien.
De limiet van zestien actieve noten van DX7 is een afzonderlijk synthesebudget voor de hele song;
akkoorden en release-staarten tellen mee.

In R36H-metingen presteerden vier loopende WAV-sporen, Sega PSG, GB Pulse, DX7 en
OPL3 met gedeelde sends en vier inserts (twee Compressors, Doubler, TAPESCAM)
beter dan dichte FM-arrangementen met acht sporen. De versie met enkele noten
had geen gemiste render-deadlines in zijn meting van dertig seconden bij 48 kHz/512 frames;
het toevoegen van een DX7-akkoord van vier noten gaf één timingpiek. Dichte FM-songs
met veel dure inserts overschreden het CPU-budget. Houd wat speelruimte over,
let op de bestaande overbelastingsindicator en voeg effecten toe waar ze de song helpen.
De app behoudt elk spoor en slot; er is geen nieuwe harde limiet op het aantal inserts.
Zie `chip-instruments-report.md` voor alle metingen en de status van de langdurige tests.

De bijbehorende fysieke audiotest van 70 seconden van dat evenwichtige akkoordarrangement
slaagde zonder gemiste render-deadlines of gelogde ALSA-underruns bij de bestaande
instelling van 48 kHz / 4906 frames. De slechtste callback was 81,819 ms tegen een
deadline van 102,208 ms. De tests gebruikten een afzonderlijke master-gain van 0,4 voor speelruimte.
Behoud de directe-kaartroute `AUDIODEV=plughw:0,0` van de gewone launcher: de
gedeelde standaardmixer van het systeem gaf underruns in de diagnostische tests. Er is geen
audio-instelling van de gebruiker gewijzigd. De stresstest van tien minuten met kleinere buffer
registreerde nog steeds 28 timingpieken; de volledige resultaten staan in het rapport.

### SID-instrumenten en Phrase-FX

Kies **CHIP → SID**. Bank biedt **ChooChoo SID Originals** (32 zelfgemaakte
programma's) en **SIDkit Effects** (24 effecten onder MIT-licentie). Beide gebruiken dezelfde
presetbrowser en slaan hun volledige geselecteerde programma op in het instrument
en de song. Er is geen externe bank nodig wanneer je de song deelt. Dit zijn native
SID-parameterprogramma's met envelopes en bewegingsrecepten, geen gesamplede audio en
geen volledige C64-songs. De afzonderlijke twaalf onderzoekskandidaten van GoatTracker
worden niet meegeleverd; hun wave-/pulse-/filtertabellen hebben een eigen importer/speler nodig.

De instrumentpagina toont golfvorm/pulsbreedte, filtermodus/cutoff/resonantie
en native ADSR-regelaars. SID gebruikt de vastgepinde digitale oscillator van floooh/chips, de envelope
en het filter in 6581-stijl per cyclus. Er zijn geen alternatieve chipmodellen,
revisiekeuzes of toegevoegde karakterprofielen. Dit is een generieke benadering,
geen gekalibreerd analoog model van R2/R3/R4/8580.

Elke noot heeft zijn eigen filter. Ring/sync gebruiken een stille partneroscillator, dus
het delen van filters en interacties tussen drie stemmen verschillen van een fysieke SID.
De handheld heeft een budget voor de hele song van vier gewone SID-noten.
Ring of sync kost twee budgeteenheden per noot, zodat twee van zulke noten mogelijk zijn, of één plus twee
gewone noten. Akkoorden en release-staarten tellen mee. Losgelaten noten worden eerst
afgevoerd; nieuwe noten gaan vóór oudere aangehouden noten. Dure inserts en
andere synths delen nog steeds het CPU-budget voor audio.

| FX | Bereik | Actie |
| --- | --- | --- |
| `SCP` | `00-FF` | Basis van de pulsbreedte, geschaald naar native 12-bits breedte; pulsbeweging van het recept blijft actief. |
| `SCT` | `00-FF` | Basis van de cutoff, geschaald naar native 11-bits cutoff; filterbeweging van het recept blijft actief. |
| `SRN` | `00-0F` | Native resonantie. |
| `SWV` | `00-07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00-07` | Bits van de filtermodus: 1 laagdoorlaat, 2 banddoorlaat, 4 hoogdoorlaat; 0 bypass. |
| `SMR` | `00-FF` | Macroklok van het recept, geschaald van 1 tot 200 Hz. |
| `SRG` | `00-01` | Driehoek-ringmodulatie met stille partner. |
| `SSY` | `00-01` | Oscillatorsync met stille partner. |

Pulsbreedte en cutoff ondersteunen `SLE`. Discrete golfvormen, schakelaars, resonantie
en FM-feedback behouden hun bruikbare native stappen; bredere bytewaarden zouden geen
extra hardwaretoestanden opleveren. `FBK` selecteert feedback `00-07` direct.
Operatorniveaus gebruiken de engine-specifieke bereiken die hierboven staan. Aanpassingen van helderheid en
master behouden het volledige bytebereik.

### Draagbare fabriekscollecties en USER-presets

Native chipinstrumenten gebruiken **Bank:** om ALL, een meegeleverde collectie of
USER te kiezen. **Preset:** kiest een klank binnen die selectie. Houd EDIT ingedrukt en druk
op Left of Right op een van beide rijen om te doorlopen; een korte tik op EDIT opent de keuzelijst.

Meegeleverde collecties zijn ZIP-pakketten onder `instruments/FACTORY/`. USER-bestanden gaan
onder `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` of `gb-noise`.
Blader direct door je mappen, of zet er een ZIP met mappen en compatibele `.cni`-
presets neer. DX7 accepteert ook ondersteunde `.syx` enkele voices en banken met
32 voices, ook binnen ZIP-bestanden. ZIP-bestanden blijven intact op schijf. Direct laden van WOPL/WOPLX wordt niet ondersteund;
gebruik compatibele CNI-presets of converteer ondersteunde bronformaten eerst offline.

Het doorlopen van USER-presets gaat verder over bestanden, mappen, banken en ZIP-pakketten heen in
beide richtingen, en begint aan de uiteinden weer opnieuw. Het heropenen van de USER-browser volgt de
huidige selectie. ALL bevat USER-klanken onder Unsorted voor engines met categorieën;
USER blijft een aparte keuze op het hoogste niveau. Sega PSG en Game Boy gebruiken
platte ALL-/Factory-lijsten en een hiërarchische USER-browser, zonder Unsorted-
paneel. Oudere DX7-bestanden in `instruments/banks/dx7/` blijven toegankelijk in USER.

Een engine toont alleen zijn compatibele meegeleverde collecties. Factory Presets
combineert de meegeleverde OPLL-/VRC7-klankensets voor de geselecteerde engine; DX7 combineert
de originelen van ChooChoo en YSE. Gedownloade benoemde collecties blijven gescheiden.
Fat Man 2-op staat onder OPL2 en Fat Man 4-op onder OPL3. Compatibele OPL2-
CNI-bestanden kunnen nog steeds in OPL3 USER-mappen worden gebruikt.

De uitvoer van native engines gebruikt vaste, gemeten gaincompensatie in plaats van
normalisatie per preset. Bestaande songs met native instrumenten kunnen daardoor op
een ander niveau klinken; controleer de balans van opgeslagen mixen. De kalibratiemethode
en de grenzen zijn gedocumenteerd in `scripts/README_MEASURE.md`.
