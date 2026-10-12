# Manuale d'uso di ChooChooTracker

> Traduzione italiana di [`docs/USER_MANUAL.md`](../USER_MANUAL.md), realizzata a partire dalla versione inglese al commit `7c3ed6f`. In caso di discrepanze, fa fede la versione inglese.
>
> **Convenzioni.** L'interfaccia dell'applicazione è in inglese. Per questo i nomi di schermate, menu, campi e pulsanti (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**, **OPT**, **SELECT**...), i codici FX (`HOP`, `TBX`, `M1A`...), i nomi di moduli e motori (ADSR, LFO, Braids, Plaits...) e le estensioni dei file restano non tradotti, così da riconoscerli sullo schermo.

ChooChooTracker è un tracker musicale a 8 tracce per console portatili.

Segue il flusso di lavoro e il sistema di navigazione classici di LSDj, con diverse opzioni di sound design:

- emulazione del chip AY
- oscillatori Mutable Instruments Braids, Plaits e Plaits-Alt, con un filtro multimodale
- campioni PCM, forme d'onda a ciclo singolo e oscillatori a tabella d'onda, con un filtro multimodale
- aChChid, un versatile sintetizzatore acid basato su Open303
- Bogie, un sintetizzatore di batteria nativo a 12 modelli, con un filtro multimodale
- MME, un sintetizzatore a due oscillatori e modulazione multipla, con un filtro multimodale
- Sintered, un sintetizzatore di batteria digitale sperimentale nativo a sei modelli

## 1. Installazione e file

### ArkOS / PortMaster

Installa `choochootracker.zip` tramite PortMaster, oppure copia il pacchetto estratto nella cartella `ports` della console. Avvia **ChooChooTracker** dal menu Ports.

### Miyoo Mini / Mini Plus (Onion OS)

Estrai l'archivio MiyooPorts nella radice della scheda SD, in modo che le sue cartelle `Roms/PORTS/` si uniscano a quelle già presenti in Onion. Avvia **ChooChooTracker** dal menu Ports dopo aver aggiornato la collezione. Fai una copia di sicurezza della cartella `projects/` dell'applicazione prima di sostituire un'installazione esistente.

### Windows

Esegui `choochootracker.exe` con `SDL2.dll` e `libwinpthread-1.dll` nella stessa cartella.
Premi **ALT + ENTER** per attivare o disattivare lo schermo intero. La finestra è ridimensionabile; l'immagine
si ridimensiona mantenendo il rapporto d'aspetto originale.

### Web

La pagina del browser segue il tema attivo del tracker. Le impostazioni salvate, comprese
le assegnazioni dei tasti personalizzate confermate con **Done**, vengono scritte subito nella
memoria di questo browser. Dopo il blocco dello schermo o un cambio di app, tocca un controllo per ripristinare
l'audio, se necessario. **Restart audio** riapre l'uscita senza scartare il
progetto. **Reload app** salva il lavoro corrente in **Continue**, salva le impostazioni
e attende la memoria del browser prima di ricaricare; un salvataggio fallito o scaduto
annulla il ricaricamento. Usa **Download .cct** per un backup portabile. La memoria del browser
può essere svuotata in modo indipendente, e il ripristino dell'instradamento di controller e audio su iOS
non è garantito.

La versione per browser aggiorna lo schermo a circa 30 FPS e usa un buffer audio fisso di
1024 frame per dare all'audio più tempo tra una chiamata e l'altra. Le vecchie impostazioni del buffer
salvate vengono regolate automaticamente; i progetti e le altre impostazioni
vengono conservati. Il polling degli input e i timer dell'applicazione mantengono il ritmo abituale.

### Configurazione delle cartelle

| Cartella | Contenuto |
|---|---|
| `AY_Wavetables` | tabelle d'onda `.aywave` del chip AY |
| `fonts` | file di font ChipNomad `.cnfont` |
| `instruments` | preset di strumento `.cni`, inclusi i file ChipNomad |
| `pitch-tables` | accordature alternative `.csv` |
| `projects` | file di canzoni e progetti `.cct` |
| `samples` | file `.wav` per il motore Sampler |
| `SR_wavetables` | tabelle d'onda in formato Serum per il motore BYOWTBL |
| `themes` | temi ChipNomad `.cth` |
| `title` | risorse della schermata del titolo |
| `waveforms` | forme d'onda `.wav` a ciclo singolo per il motore 2xSCWF |

## 2. Controlli

ChooChooTracker funziona meglio con un gamepad dotato di croce direzionale, 2 stick analogici e 7 pulsanti.

| Controllo logico | Predefinito su console portatile | Predefinito su Windows |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | croce direzionale | tasti freccia |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Spazio |
| **SELECT** | Select | Shift |
| Assi di modulazione | stick sinistro e destro | stick del controller |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Le assegnazioni si possono cambiare in **Settings > Key mapping**. Su PortMaster, rilascia
il pulsante usato per selezionare un'assegnazione prima di premere il nuovo pulsante. Salva una
riassegnazione con **Done**, poi rilascia il pulsante prima di proseguire in Settings.
I layout desktop predefiniti usano QWERTZ solo per le regioni QWERTZ; le tastiere polacche
mantengono i valori predefiniti QWERTY standard.

Anche gli utenti di Windows, web e Android possono usare un controller. Su Android,
i controlli touch compaiono automaticamente quando non è collegato alcun gamepad: in verticale
si trovano sotto il tracker, con una croce direzionale quadrata, i controlli A/EDIT, B/OPT, START/PLAY e
SELECT/SHIFT, poi una sezione Live/Motion separata con due stick di modulazione
e controlli REC/DEL momentanei. In orizzontale la croce resta a sinistra
e i pulsanti d'azione a destra. I progetti e i campioni su Android restano privati per
l'applicazione; usa il selettore di file di Android per importare progetti `.cct` e campioni `.wav`,
e per scegliere dove salvare le esportazioni WAV.

### Navigazione

- Usa **[UP/DOWN/LEFT/RIGHT]** per spostare il cursore.
- Tieni premuto **SELECT + [DIRECTION]** per spostarti tra le schermate.
- **SELECT + [LEFT/RIGHT]** fa anche uscire dalle pagine secondarie verso la schermata MSCPIT
  vicina, come Instrument Pool: Project/Settings (compresi i sottomenu di Settings)
  portano a Mixer/Chain; Groove a Chain/Instrument; Modulation/Insert FX a
  Phrase/Table; AY Wavetable a Instrument/Table. Al bordo sinistro, Reverb/Delay
  tornano al Mixer con SELECT + LEFT. La navigazione verticale esistente non cambia.
- Tieni premuto **OPT + [DIRECTION]** per la navigazione specifica di ogni schermata.
- Su un valore, usa **EDIT + [LEFT/RIGHT]** per modifiche fini o **EDIT + [UP/DOWN]** per modifiche grossolane.
- Su una **Destination** di modulazione, **LEFT/RIGHT** scorre direttamente le destinazioni disponibili; tocca **EDIT** per aprire il suo menu raggruppato.
- Tocca **EDIT** per inserire un valore o attivare un comando.
- Tocca **EDIT** due volte per creare una nuova catena, frase o strumento dove è supportato.
- Tenere premuta una direzione la ripete dopo il ritardo configurato in Settings.

### Selezione e appunti

- **SELECT + OPT** attiva la modalità di selezione e scorre intervalli di selezione utili.
- Usa **[DIRECTION]** per estendere l'intervallo.
- **OPT** copia la selezione.
- **OPT + EDIT** la taglia.
- **SELECT + EDIT** la incolla.
- **EDIT + [DIRECTION]** modifica insieme le celle selezionate.

Nella schermata Phrase, con una selezione attiva, la colonna delle note può essere riempita
con pattern ritmici:

- **OPT + LEFT** riempie le celle delle note della selezione con un pattern fisso di 16 passi
  e passa al pattern successivo a ogni pressione. Dopo l'ultimo pattern le
  note vengono cancellate e il ciclo ricomincia. I pattern sono allineati alle righe
  assolute della frase, quindi una selezione parziale riceve solo i passi che cadono al suo interno.
- **OPT + RIGHT** riempie le celle delle note della selezione con un pattern casuale di
  3-8 note (7 e 8 sono meno probabili), ripetendosi all'infinito finché **OPT** è
  tenuto premuto.
- **OPT + UP** muta le note già inserite nella selezione: a ogni pressione
  ne sceglie 1-3 e sposta ciascuna di una quantità casuale entro un'ottava
  (gli spostamenti piccoli sono più probabili di quelli estremi). Con **Note Lock** attivo,
  le note spostate vengono riportate nella scala; gli strumenti Sampler con
  **Slice** attivo restano cromatici. Non viene cancellato nulla: l'8ª pressione ripristina
  le note che la selezione conteneva all'inizio del ciclo, poi il ciclo ricomincia.
- **OPT + UP** su una selezione che copre solo la colonna del volume (nessuna cella di nota,
  strumento o FX) rende invece casuali le velocity: la
  prima pressione applica il pattern Minimal (ogni volume si sposta fino a
  +-20), la seconda Medium (fino a +-50), la terza Random (valori
  completamente casuali), e la quarta ripristina i volumi che la selezione conteneva
  all'inizio del ciclo. Rilasciare **OPT** riporta il ciclo a Minimal.
  Le righe senza volume non vengono toccate.
- **OPT + DOWN** distribuisce le note inserite degli strumenti Sampler tagliati (sliced) nella
  selezione in una serie cromatica: la prima nota di ciascuno strumento (la più alta
  nel sequencer) mantiene il suo valore e ogni nota successiva di quello
  strumento sale di un semitono rispetto alla precedente, trasformando una pila di
  note identiche in attivazioni consecutive di slice. Più strumenti
  tagliati si distribuiscono in modo indipendente a partire dalle proprie prime note, le note
  di strumenti senza taglio non vengono toccate e il risultato non viene mai
  agganciato alla scala del progetto. L'ultima nota scritta dalla distribuzione diventa
  l'ultima nota usata nell'inserimento, così continuare la serie a mano riprende dallo
  slice successivo. Una selezione che contiene solo strumenti tagliati
  si distribuisce a ogni pressione; quando sono presenti anche note non tagliate,
  la distribuzione parte alla prima pressione e dalla seconda l'arpeggio casuale si occupa di
  quelle note.
- **OPT + DOWN** su una selezione senza note di strumenti tagliati è un arpeggiatore
  casuale per le note già presenti. Un accordo viene scelto
  dalla tavolozza CRD - accordi di 3 note per selezioni di 1-2 note, accordi di 3 o più note
  per 3, qualsiasi accordo per 4 o più - con la sua fondamentale su una nota casuale
  della scala del progetto nel registro della selezione (qualsiasi nota
  cromatica quando la scala è disattivata o la traccia ne è esclusa), e le sue note
  vengono quantizzate nella scala come fa la riproduzione per CRD. Ogni
  pressione ridistribuisce l'accordo sulle note inserite: in modo casuale,
  dalla più grave alla più acuta, dalla più acuta alla più grave, poi avanti e indietro; la 5ª pressione
  ripristina le note originali e la successiva sceglie un nuovo accordo.
  Il nome dell'accordo compare nella barra di stato finché l'arpeggio è attivo, non
  vengono aggiunte né rimosse note, e rilasciare **OPT** azzera l'intero
  processo.
- Ogni pressione di riempimento cancella le note della selezione prima di scrivere, e le note
  inserite portano l'ultimo strumento usato; rilasciare **OPT** riporta i cicli
  al loro primo passo.
- Finché è attiva una selezione e non viene mostrato altro messaggio, la barra
  di stato mostra a rotazione ogni 2 secondi un promemoria di queste combinazioni: `EDIT + DIR =
  batch note edit`, `Double-tap EDIT: resample selection`, `OPT + LEFT =
  pattern fill`, `OPT + RIGHT = random fill`, `OPT + UP =
  mutate/randomize velocity` e `OPT + DOWN = random arp/slice spread`.
  Entrare nella modalità di selezione o uscirne riavvia la rotazione; qualsiasi messaggio
  di azione sostituisce il suggerimento finché non scade.

### Riproduzione

- **PLAY** parte dal cursore. Premilo di nuovo per fermare.
- Puoi modificare note, strumenti e catene durante la riproduzione; le modifiche si sentono
  al tick successivo del sequencer.
- **SELECT + PLAY** avvia tutte le tracce quando usato fuori dalla schermata Song.
- **EDIT + PLAY** ascolta uno strumento dall'Instrument Pool.

## 3. Struttura della canzone e mappa delle schermate

Tieni premuto **SELECT + [DIRECTION]** per spostarti tra le schermate.

La riga di navigazione principale è:

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Riga centrale:

- **M**: Mixer (porta a Reverb, Delay)
- **S**: Song (porta a Project, Settings)
- **C**: Chain
- **P**: Phrase (porta a Groove)
- **I**: Instrument (porta a Modulation e a Instrument Pool)
- **T**: Table (porta all'editor AY Wavetable)

Riga superiore:

- **R**: Reverb
- **P**: Project
- **G**: Groove
- **M**: Modulation

Riga inferiore:

- **D**: Delay
- **S**: Settings
- **P**: Instrument Pool
- **W**: editor AY Wavetable

Una canzone di ChooChooTracker segue questa gerarchia:

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

Una canzone ha 8 tracce. Ogni traccia contiene una sequenza di catene, e ogni catena contiene una sequenza di frasi. Le frasi sono sequenze di 16 passi di note. Una nota contiene un'altezza, uno strumento, una velocity e fino a 3 FX di traccia. Uno strumento è ciò che produce il suono.

### Font opzionali 16x24 e NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 contiene TechMonoAudit (convertito da Share Tech Mono), Departure Mono, Spleen e Cozette. Ognuno fornisce l'intero set ASCII stampabile in celle 16×24 per uno schermo 640×480. I font costruiti con pixel mantengono i loro pixel nativi o una scalatura intera con margine; Cozette resta più piccolo perché raddoppiare la sua altezza d'inchiostro di 13 pixel supererebbe la cella. Il tema NostromoAmberDa2 è disponibile tramite Settings → Edit color theme → Load. Mantiene lo sfondo ambra e il testo informativo di Nostromo, con testo principale rosso, valori arancioni più scuri e un cursore verde acqua. Caricare queste risorse non cambia il font né il tema scelti. Consulta il file README.txt allegato per le attribuzioni e le licenze.

### Spaziatura della pagina Project

La pagina Project mette la versione dell'applicazione su una riga a sé e separa Load, Save, New, Export e Manage dai metadati del progetto con righe vuote. Le posizioni del cursore e le aree touch seguono i campi mostrati; le impostazioni del progetto e le operazioni sui file non cambiano.

### Telemetria della visualizzazione audio

Gli indicatori audio usano un'istantanea, di proprietà dell'interfaccia, del mix finale e dei contributi delle singole tracce. I campioni di traccia e i picchi stereo vengono acquisiti dopo il livello di traccia e il tilt, prima dei ritorni degli effetti condivisi e del guadagno master. Il monitoraggio non modifica il segnale audio. Il callback audio pubblica istantanee di dimensione fissa senza lock né allocazioni; la cronologia mostrata copre circa 21 ms.

### Forma d'onda persistente

Graphics → Persistent waveform è su OFF per impostazione predefinita. Tocca EDIT per attivarla o disattivarla, oppure usa EDIT + direzione; la scelta viene salvata come preferenza dell'applicazione. OFF ripristina le pagine a tutta altezza e le anteprime delle forme d'onda degli strumenti esistenti. Quando è attiva, una forma d'onda su due righe sopra le pagine di modifica mostra il mix finale sommato in mono. Le pagine Instrument e Modulation mostrano l'uscita di una traccia che usa lo strumento selezionato; se più tracce lo usano, prevale la traccia di Song selezionata. Project, Settings, le tabelle d'onda e i popup mantengono il contenuto a tutta altezza. Le liste scorrono nelle righe rimanenti. I piè di pagina di Instrument mantengono le anteprime ADSR/campione, e i campi di Modulation vengono compattati per mantenere accessibili tutti i controlli. Le sue aree touch seguono i campi mostrati in entrambe le modalità di forma d'onda.

### Pianoforte di riproduzione

Un pianoforte di pixel disegnato dal codice, sotto le otto righe di traccia sul lato destro, illumina le classi di altezza che suonano, ripiegate in un'ottava. Include le note degli accordi e rispetta le tracce silenziate o fermate. Il contorno usa il colore dei valori del tema e tutti i tasti attivi usano il colore della forma d'onda; i tasti inattivi hanno riempimenti scuri e ombre sottili. La sua griglia usa scalatura intera e bordi di larghezza fissa. Il pianoforte è un indicatore di riproduzione, non un controllo per l'inserimento di note. I popup e l'editor di tabelle d'onda mantengono il proprio contenuto in quest'area.

### Indicatori del Mixer

Il Mixer mostra barre di picco **L/R** adiacenti per ogni traccia, dopo il suo PAN e il tilt, prima degli effetti condivisi e del guadagno master. L'indicatore copre da -48 a 0 dBFS e decade tra un aggiornamento e l'altro. Le sue bande bassa e media usano i colori del tema, mentre la banda alta è di un rosso brillante vicino al fondo scala.

## 4. Song, Chain e Phrase

### Song

La schermata Song contiene 8 colonne, 1 per traccia audio. Ogni cella rimanda a una catena. Una canzone può contenere fino a 256 righe (`00-FF`).

Le tracce scorrono da sinistra a destra, mentre le loro catene scorrono in verticale. Ogni traccia è indipendente, e le catene possono contenere da 1 a 16 frasi, quindi le tracce possono andare fuori sincrono. Progetta le tue catene in modo che restino insieme, oppure lasciale vagare se è ciò di cui ha bisogno la traccia.

È comune riservare 1 catena per le frasi vuote, di solito `00` o `FE`.

Puoi tenere più sotto-canzoni in un progetto creando «isole»: sezioni separate da righe vuote.

#### Live Mode

Tocca **OPT** due volte per entrare nella Live Mode o uscirne; `LIVE` sostituisce il titolo Song. In Live Mode, **PLAY** avvia la catena selezionata sulla sua traccia quando questa è ferma. Su una traccia in riproduzione, mette in coda la catena selezionata per il prossimo confine di catena; premi **PLAY** due volte per un cambio urgente al prossimo confine di frase. Una cella Song vuota selezionata mette invece in coda uno stop quantizzato. `+` indica una catena in coda, `!` una catena urgente e `-` uno stop in coda.

Seleziona una riga di Song su più colonne per mettere in coda o fermare quelle tracce insieme. Le selezioni su più righe vengono rifiutate di proposito. Le code Live vengono svuotate mentre è attivo un loop di intervallo di Song. Live è una modalità di performance e non viene salvata nel progetto.

#### Controlli

- **OPT + [UP/DOWN]**: salta di 16 posizioni in su o in giù
- Seleziona un intervallo, poi **SHIFT + EDIT**: clona superficialmente le catene mantenendo le loro frasi originali
- Seleziona un intervallo, poi tocca due volte **SHIFT + EDIT**: clona in profondità sia le catene sia le loro frasi
- Seleziona un intervallo, poi **EDIT + [UP/DOWN]**: sposta la selezione in su o in giù
- Tocca **OPT** 3 volte: aggiunge o rimuove un'evidenziazione per l'organizzazione visiva
- **OPT + SHIFT**: silenzia la traccia corrente o le colonne selezionate (rilascia prima **OPT** per mantenere il mute attivo)
- **OPT + PLAY**: mette in solo la traccia corrente o le colonne selezionate (rilascia prima **OPT** per mantenere il solo attivo)
- **OPT + [LEFT/RIGHT]**: mette in solo ogni traccia a sinistra o a destra della traccia corrente
- Seleziona un intervallo, poi tocca due volte **EDIT**: apre la schermata BOUNCE TO SAMPLE (vedi Bounce più sotto)

#### Bounce

Seleziona un intervallo di righe e colonne di Song, poi tocca due volte **EDIT** per aprire la schermata BOUNCE TO SAMPLE. Il bounce include ogni traccia selezionata: le tracce la cui prima catena inizia più avanti nella selezione attendono in silenzio ed entrano all'inizio della loro prima catena, così il file contiene le catene di tutte le tracce selezionate.

La schermata offre:

- **File name**: si modifica con la tastiera di caratteri. All'apertura della schermata, qui viene proposto il prossimo numero di sequenza libero (`001`, `002`, ...): il primo numero il cui file `.wav` non esiste ancora nella cartella di esportazione. Modificalo liberamente: il nome viene usato così com'è. Se esiste già un file con quel nome, viene aggiunto un suffisso `_001`, `_002`, ... così i file esistenti non vengono mai sovrascritti. Il contatore si azzera quando carichi o crei un progetto.
- **Sample rate** e **Bit depth**: le stesse opzioni della schermata di esportazione
- **Include in a sample name**: tre caselle che antepongono al nome del file etichette di contesto quando si preme **Start** (tutte disattivate per impostazione predefinita):
  - **[BPM]** - il tempo corrente del progetto, ad esempio `[120]`
  - **[Key]** - la nota fondamentale e la scala del progetto, ad esempio `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** - la durata renderizzata del bounce: una frase equivale a una battuta (16 sedicesimi), un movimento sono 4 sedicesimi. Le frasi intere mostrano solo le battute, ad esempio `[2]`; le frasi parziali mostrano `[0:2]` (due movimenti) o `[0:1:3]` (un movimento più tre sedicesimi). Nei bounce di più tracce decide la traccia più lunga. Una selezione che termina a metà di un movimento conta un sedicesimo in meno: l'ultima riga selezionata è il punto di taglio, quindi il suo note-off cade alla fine del sedicesimo precedente.
  - Le etichette vengono applicate nell'ordine `[BPM][Key][Length]` prima del nome del file.
- **Start**: renderizza la selezione in un file WAV nella cartella di esportazione (vedi *Export location* nella schermata Project)
- **Cancel**: torna indietro senza fare il bounce

Mentre il bounce viene renderizzato, **OPT** lo annulla. Quando il bounce termina o viene annullato, torna la schermata da cui lo hai avviato.

Su desktop, il key jazz porta qui anche l'inserimento diretto di indici esadecimali, la selezione
con Shift+frecce come in Phrase, Delete/Backspace/Insert e Ctrl+C/X/V - vedi
[Key jazz](#key-jazz-solo-desktop).

### Chain

Una catena è un elenco ordinato di frasi da 16 passi, con trasposizione opzionale. Può contenere fino a 16 frasi, e la stessa frase può comparire più volte. La 2ª colonna imposta la trasposizione in semitoni.

Un asterisco (`*`) compare accanto a una catena riutilizzata nel progetto. Puoi clonare le catene dalla schermata Song.

#### Controlli

- **OPT + [LEFT/RIGHT]**: spostarsi tra le tracce
- **OPT + [UP/DOWN]**: spostarsi tra le catene della traccia corrente
- Seleziona un intervallo, poi **SHIFT + EDIT**: clona le frasi
- Seleziona un intervallo, poi tocca due volte **EDIT**: apre la schermata BOUNCE TO SAMPLE per le righe di catena selezionate della traccia corrente (vedi Bounce in Song)

Su desktop, il key jazz porta qui anche l'inserimento diretto di indici esadecimali - vedi
[Key jazz](#key-jazz-solo-desktop).

### Phrase

Una frase è il pattern di una traccia in un sequencer a passi tradizionale.

Una frase è lunga 16 passi, con 1 riga per passo. Ogni riga contiene una nota, uno strumento, un volume da `00` a `7F` e colonne di FX. Le note usano la notazione da tracker, come `C-4` (nota e ottava).

Per fermare una nota, inserisci `NOTE OFF`, usa l'FX di interruzione della nota, oppure suona un'altra nota senza strumento impostato.

Quando una catena contiene più frasi, la schermata Phrase funziona come un editor di pattern continuo. Scendere sotto la riga `F` o salire sopra la riga `0` apre la frase successiva o precedente della catena.

Un asterisco (`*`) compare accanto a una frase riutilizzata nel progetto. Puoi clonare le frasi dalla schermata Chain.

Il selettore FX mostra i comandi comuni più quelli supportati dallo strumento di quella riga. Così si evita un lungo elenco globale di comandi specifici dei motori. ChooChooTracker supporta più FX di ChipNomad.

### Controlli

- **OPT + EDIT** su una nota vuota: inserisce `NOTE OFF`
- **EDIT + [UP/DOWN]** su una colonna di nome FX: apre la schermata di selezione FX
- **OPT + [LEFT/RIGHT]**: spostarsi tra le tracce
- **OPT + [UP/DOWN]**: spostarsi tra le frasi della catena corrente
- Seleziona un intervallo nella colonna dello strumento, poi **SHIFT + EDIT**: clona gli strumenti
- Seleziona un intervallo, poi **EDIT + [UP/DOWN]**: ruota le righe della frase
- Seleziona un intervallo, poi tocca due volte **EDIT**: apre la schermata BOUNCE TO SAMPLE per le righe di frase selezionate della traccia corrente (vedi Bounce in Song)

### Key jazz (solo desktop)

Premi **Esc** nella schermata Phrase per attivare o disattivare il key jazz, una modalità di inserimento
note in stile pianoforte QWERTY (come [m8c](https://github.com/laamaa/m8c)):

- `Z S X D C V G B H N J M` e `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` inseriscono note
  in modo cromatico su più di due ottave, usando la posizione fisica di ciascun tasto,
  così il layout è lo stesso sulle tastiere AZERTY, QWERTY e QWERTZ.
- Ogni pressione scrive la nota sulla riga corrente e passa alla successiva.
- `[` / `]` spostano l'ottava.
- **Shift + frecce** selezionano un intervallo di righe e colonne; una freccia semplice
  dopo cancella la selezione.
- **Ctrl+C** / **Ctrl+X** copiano o tagliano le colonne selezionate, oppure
  nota/strumento/volume della riga corrente se non è selezionato nulla; **Ctrl+V** incolla
  al cursore.
- **Delete** rimuove l'intera riga o righe (tutte le colonne) e sposta in su il resto della
  frase per chiudere il vuoto; il cursore resta sulla stessa riga.
- **Backspace** rimuove l'elemento sotto il cursore solo nella colonna corrente
  (o nelle colonne della selezione, se ce n'è una attiva), sposta in su ciò che c'è
  sotto per chiudere il vuoto e fa retrocedere il cursore di una riga, come in
  un editor di testo.
- **Insert** inserisce una riga vuota, spingendo in giù il resto della frase.
- **Ctrl+S** salva il progetto. Fuori dal key jazz, Ctrl+S non fa nulla.
- Esc di nuovo disattiva il key jazz.

Finché il key jazz è attivo, prende interamente il controllo dei tasti delle note nella schermata
Phrase, comprese le scorciatoie Edit/Opt/Motion che condividono quei tasti - disattivalo
con Esc per usarli di nuovo.

Il key jazz funziona anche, attivabile in modo indipendente con Esc, su:

- **Song** e **Chain**: digita direttamente l'indice esadecimale di una catena o di una frase
  (`0-9`, `A-F`) invece di incrementare con Up/Down. La prima cifra digitata
  in una cella ne sostituisce il valore; le cifre successive digitate senza spostarsi scorrono
  al suo interno (digitando «3» e poi «F» si ottiene 3F). In Song, **Shift+frecce** selezionano un
  intervallo, **Delete** rimuove l'intera riga o righe e sposta in su il resto, **Backspace**
  rimuove la colonna o colonne correnti/selezionate e sposta in su ciò che c'è sotto,
  **Insert** inserisce una riga vuota, e **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  copiano/tagliano/incollano le righe selezionate (o la riga corrente se non è selezionato
  nulla) - sono le stesse scorciatoie di modifica della struttura della schermata Phrase.
  Questo prende Shift, quindi Shift+Right/Up non saltano più a Chain/Project
  finché il key jazz è attivo qui - premi Esc per riavere quelle scorciatoie.
- **Ogni schermata con un campo di testo**: digita direttamente sulla tastiera
  invece di usare la tastiera virtuale a schermo. Questo riguarda il nome del file, il titolo
  e l'autore di Project, il nome dello strumento, il nome del tema colori,
  il nome della tabella di accordatura, il nome del file del bounce e le finestre di dialogo per i nomi (inserimento
  nome, creazione cartella). Backspace funziona come in un normale campo di testo; Shift
  digita maiuscole. Esc lo attiva su tutte queste schermate insieme,
  e la tastiera virtuale continua ad aprirsi normalmente quando il key jazz è disattivato.

## 5. Instruments

Tutti gli strumenti hanno alcuni parametri comuni:

- Tipo di strumento
- Nome, fino a 15 caratteri
- Velocità predefinita della tabella, in tick per riga di tabella
- Trasposizione attiva o disattiva

Ogni strumento ha una tabella predefinita con lo stesso numero nell'intervallo `00-7F`. Puoi usarle come tabelle ausiliarie, ma tenere le tabelle ausiliarie nell'intervallo `80-FE` evita conflitti e confusione. Le tabelle sono tra i principali strumenti di sound design di ChooChooTracker.

### Controlli

- **OPT + [LEFT/RIGHT]**: spostarsi tra gli strumenti
- **EDIT + PLAY**: ascolta lo strumento
- **SHIFT + OPT**: copia lo strumento
- **SHIFT + EDIT**: incolla lo strumento

### AY Classic, AY Plus e AY Sample

Questi sono i motori AY/YM originali di ChipNomad. AY Classic espone controlli di tono, rumore e inviluppo in stile hardware. AY Plus aggiunge oscillatori software e una modulazione più ricca. AY Sample riproduce un campione attraverso i livelli di volume in stile AY ed è distinto dal motore Sampler.

#### AY Classic

È un tipo di strumento ereditato da ChipNomad `v1.0.0`. Il più recente strumento AY Plus offre più funzioni.

Gli strumenti AY Classic hanno questi parametri:

- Mixer: tono attivo o no, rumore attivo o no, forma dell'inviluppo (`0-F`)
- Volume: inviluppo ADSR generato via software
- Periodo di inviluppo automatico: attivo o no, con un rapporto da `1:1` a `F:F`

**AY Quality** in Settings riguarda solo il rendering AY/YM. Non cambia la qualità di Braids, Plaits o Sampler. **Sample dithering** si applica alla quantizzazione di AY Sample.

Consulta la [documentazione di ChipNomad sull'AY-3-8910](https://chipnomad.org/chips/ay-3-8910/) per i dettagli del chip.

#### AY Plus

AY Plus applica concetti classici di sintesi al sound design dell'AY-3-8910/YM2149F. Puoi pensare all'AY come a un sintetizzatore a 5 oscillatori:

- 3 oscillatori di tono a onda quadra
- 1 oscillatore di rumore
- 1 generatore di inviluppo d'ampiezza che può essere usato come oscillatore

Secondo questo modello, ogni strumento ha 3 oscillatori hardware: tono, rumore e inviluppo. AY Plus ne aggiunge un 4º, software. Aggiornare i registri dell'AY più velocemente della frequenza dei tick del progetto apre più possibilità di sound design. Per come funziona l'AY, gli oscillatori vengono mixati con la modulazione ad anello. Prova diversi offset di altezza, scordature e combinazioni di oscillatori per scoprire che cosa ne esce.

Ogni oscillatore tonale può essere accordato in modo indipendente con offset di altezza e di accordatura fine.

Tipi di oscillatori software e loro parametri:

- Pulse - onda quadra con larghezza d'impulso controllabile
  - FM Depth - profondità di FM semplice
  - Pulse width - l'intervallo completo è `00-FF`, mentre il numero di passi si imposta nella schermata Project
  - Pulse low - controlla il valore basso di una coppia ripetuta il cui valore alto è sempre `15`
- Sync Tone - effetto di sincronizzazione forzata (hard sync) per l'oscillatore di tono
  - FM Depth - profondità di FM semplice
- Sync Env - effetto di sincronizzazione forzata per l'oscillatore di inviluppo
  - FM Depth - profondità di FM semplice
  - Pulse width - rapporto di durata tra le 2 forme della coppia di inviluppi
  - Envelope Pair - 2 forme di inviluppo tra cui alternare
- Wavetable - tabelle d'onda a 32 passi, modificate nella schermata Wavetable
  - FM Depth - profondità di FM semplice
  - Wavetable index
- Tone FM - semplice effetto di tipo FM per l'oscillatore di tono
  - FM Depth - profondità di FM del tono
- Env FM - semplice effetto di tipo FM per l'oscillatore di inviluppo
  - FM Depth - profondità di FM dell'inviluppo

Gli oscillatori software Pulse e Wavetable non possono essere usati con l'oscillatore Envelope. L'oscillatore Envelope si disattiva automaticamente.

La FM semplice alterna rapidamente tra 2 valori di periodo, 1 più basso e 1 più alto. È simile a usare un'onda quadra come modulatore FM. Pulse, Sync Tone, Sync Env e Wavetable usano un rapporto `1:1` tra portante e modulatore FM. Tone FM e Env FM possono imitare altri rapporti tramite gli offset di altezza dell'oscillatore software.

#### AY Sample

Puoi caricare file WAV mono o stereo a `8 bit`, `16 bit`, `24 bit` o `32 bit`. Vengono convertiti a `8 bit` e troncati a `16 KB`. Questo limite corrisponde alla dimensione di una pagina di RAM dello ZX Spectrum.

L'AY riproduce i campioni tramite scritture rapide nel registro del volume. La sua uscita è unipolare, quindi i campioni devono essere «portati a zero» per una riproduzione più pulita. Questo elimina i clic a fine campione e migliora le code silenziose.

L'AY ha un DAC a `4 bit` con una scala di volume non lineare, quindi i campioni a `8 bit` vengono convertiti a `4 bit` durante la riproduzione. L'opzione Sample dithering in Settings può migliorare la profondità di bit percepita. Il dithering non è praticabile su piattaforme retro come lo ZX Spectrum e l'Atari ST, quindi lascialo disattivato quando scrivi per hardware reale o cerchi un suono lo-fi croccante.

I campioni si comportano come un altro oscillatore software di AY Plus, ma i loro parametri aggiuntivi li rendono più facili da gestire come tipo di strumento separato.

Parametri del campione:

- Sample rate - riprodurre un campione alla sua frequenza nativa produce un `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

Le sezioni degli oscillatori Tone e Noise corrispondono a quelle di AY Plus. Per come funziona l'AY, tutti gli oscillatori vengono mixati con la modulazione ad anello.

### Braids

Braids offre 47 modelli di sintesi. I suoi controlli principali sono:

- **Model**: oscillatore o algoritmo di sintesi
- **Timbre** e **Color**: parametri macro dipendenti dal modello
- **Filter**: filtro aggiuntivo LP/BP/HP, pendenza di `12` o `24 dB`, frequenza di taglio e risonanza
- **ADSR**: attack, decay, sustain e release

Tocca **Model** per scegliere tra elenchi di modelli per categoria.

| Categoria | Modelli |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Parametri

Timbre e Color dipendono dal modello selezionato. Consulta il manuale originale di Braids di Mutable Instruments per l'elenco completo dei modelli e i dettagli, oppure suona semplicemente a orecchio.

#### VCF e VCA

Braids alimenta il filtro multimodale e l'inviluppo ADSR standard di ChooChooTracker.

#### Impostazioni

Le impostazioni globali di Braids si trovano nelle impostazioni dell'applicazione:

- **BITS**: riduce la risoluzione di uscita
- **DRFT**: aggiunge instabilità di altezza agli oscillatori
- **SIGN**: riproduce l'algoritmo di imperfezione della forma d'onda del Braids originale

### aChChid

**aChChid** è un motore di basso acid monofonico basato su Open303. `Square` e `Saw` usano l'oscillatore, il filtro, l'inviluppo e il comportamento d'accento nativi del TB-303. `Braids` sostituisce solo l'oscillatore, poi prosegue nello stesso percorso di filtro e amplificatore del 303. Espone Model, Timbre, Color e Shaper al posto di Fine tune. Shaper passa progressivamente da una lieve saturazione al wavefolding. aChChid non usa il post-filtro unificato né l'ADSR di ChooChooTracker.

Cutoff e Decay si modificano in modo logaritmico: **EDIT + LEFT/RIGHT** si sposta di un
passo di controllo musicale, mentre **EDIT + UP/DOWN** si sposta di sedici passi. Cutoff
copre da 200 Hz a 20 kHz; Decay copre da 200 ms a 2 s.

Una `F` nella colonna del volume della nota attiva un accento. Le note normali usano un gate di mezzo passo (3 tick nel groove predefinito da 6 tick). `ASL` scivola verso quella nota dall'altezza precedente senza riattivare l'inviluppo del 303; mantiene automaticamente il gate precedente, e `ASL 00` dà un glissando di `60 ms`. `ATY` mantiene aperta la sua nota per l'intero passo corrente. Le note senza `ASL` si riattivano sempre. Le destinazioni di modulazione includono Decay e Accent, oltre a Timbre e Color in modalità onda Braids.

### Bogie

**Bogie** è un sintetizzatore di batteria nativo a colpo singolo (one-shot). Le note del tracker fissano l'altezza e il volume del tracker/strumento fissa il livello; ogni attivazione ha il proprio decadimento interno, quindi non ha pagina ADSR. Ha 12 modelli: `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` e `Clave`.

Le sue sei macro sono **Decay**, **Tone**, **Sweep**, **Noise**, **FM** e **Drive**. Le loro etichette si adattano dove utile (per esempio il Noise di Kick è `Click`, e l'FM di Cowbell è una modulazione incrociata metallica). Ogni macro è attiva su ogni modello, ma il suo ruolo musicale cambia con il modello: Sweep può alterare l'altezza, la spaziatura delle raffiche o la dispersione metallica; FM può aggiungere modulazione incrociata, uno strato metallico o una sovratonalità di tipo ad anello. La metà inferiore di ogni intervallo è pensata per suoni di batteria convenzionali; la metà superiore apre di proposito metamorfosi timbriche più ampie e risultati più sintetici.

Ogni modello può usare il filtro multimodale LP/BP/HP condiviso, con carattere Clean, Classic, Aggro o Acid, pendenza di `12`/`24 dB`, frequenza di taglio e risonanza. Bogie supporta tutte le sue macro attive più Volume, Pitch, Cutoff e Resonance come destinazioni di modulazione e di registrazione del movimento. Usa un Decay breve con Cowbell per attacchi phonk taglienti; aumenta FM per una campana più dura e metallica.

Nella schermata Modulation, premi **EDIT** su una sorgente per scegliere una famiglia (Envelopes, LFO o Sticks), poi la sorgente. **Left** e **Right** continuano a scorrere direttamente le sorgenti.

### Sintered

**Sintered** è un sintetizzatore di percussioni digitale nativo a colpo singolo. Le note fissano la sua altezza fondamentale; i suoi sei modelli sono `Knot`, `Shard`, `Burst`, `Comb`, `Logic` e `Melt`. Ogni modello ha **Decay**, **Mod** e **Motion**, più tre controlli specifici del modello: Knot usa Ratio/Spread/Fold, Shard Ratio/Feedback/Bite, Burst Noise/Color/Feedback, Comb Time/Damping/Regen, Logic Rate/Pattern/Crush e Melt Ratio/Chaos/Drive. **Mod** è sempre la profondità di accoppiamento interna.

Motion è un inviluppo macro interno, non un glissando d'altezza: i valori sotto `80` usano una forma AD, `80` la disattiva, e i valori sopra `80` usano una forma di solo decadimento. Allontanarsi dal centro rende l'inviluppo più veloce. Le sue destinazioni sono fisse per modello, quindi Sintered resta prevedibile nei pattern del tracker: Knot anima Mod/Fold; Shard Feedback/Bite; Burst Noise/Color/Feedback; Comb Damping/Regen; Logic Rate/Crush; Melt Chaos/Drive. Tutti i modelli azzerano il proprio stato all'attivazione di una nota e limitano il feedback interno per colpi ripetibili.

### MME

**MME** (Multi Modulation Engine) è un sintetizzatore a due oscillatori dedicato alle interazioni tra oscillatori. I suoi modelli sono `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` e `Vocode`. Entrambi gli oscillatori sono interni e seguono le note del tracker; `Interval` ne fissa il rapporto.

Le sei macro sono **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** e **Shaper**. Waves seleziona coppie musicali di forme d'onda A/B. Amount controlla l'interazione centrale del modello, Flow ne cambia la direzione o la variante dell'algoritmo, e Feedback è limitato ma diventa volutamente selvaggio a fine corsa. Shaper è uno stadio finale condiviso che passa da pulito a saturazione e poi a wavefolding.

MME usa il filtro multimodale LP/BP/HP condiviso e l'ADSR. Tutte e sei le macro, Cutoff e Resonance sono disponibili per la modulazione e la registrazione del movimento. Tieni premuto **EDIT** su Model per aprire il popup dei modelli.

### MIDI Out

**MIDI Out** (solo desktop) pilota un dispositivo MIDI esterno invece di sintetizzare audio: attivare una nota invia un vero MIDI Note On sul **Channel** dello strumento (`1-16`) e, quando la nota finisce, il Note Off corrispondente. Il volume diventa velocity, e ogni voce di una traccia di accordi riceve i propri Note On/Off. Da ChooChooTracker stesso non si sente alcun suono: seleziona prima il dispositivo di uscita in [Settings](#12-settings). Per il quadro completo, compreso l'ascolto dei suoni da una tastiera MIDI, vedi [MIDI](#14-midi).

**Program** e **Bank high/low** (CC0/CC32) sono facoltativi - mostrati come `--` quando disattivati, `Clear` disattiva un campo ricordandone il valore. Quando sono impostati, vengono inviati una volta, subito prima della nota successiva, ogni volta che differiscono da ciò che è stato comunicato per ultimo a quel canale (non prima di ogni nota, il che riattiverebbe gli inviluppi o la patch del dispositivo ricevente).

**MC1 number**-**MC4 number** scelgono a quale numero di CC (`00-7F`) invia ciascuno dei quattro FX di riga MC1-MC4 - vedi [MIDI Out FX](#midi-out-fx). Anche `--`/facoltativo; uno slot lasciato non impostato rende inerte il suo FX.

### Motori sottrattivi

I motori di questa categoria condividono un'architettura VCO verso VCF verso VCA.

Sono disponibili diversi tipi di VCO:

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Ogni strumento invia il proprio VCO a un filtro multimodale.

Il filtro ha una pendenza commutabile di `12 dB` o `24 dB` e può funzionare come passa-basso (LP), passa-banda (BP) o passa-alto (HP).

Sono disponibili diversi caratteri di filtro:

- **Clean**: sterile e digitale
- **Classic**: ispirato ai classici sintetizzatori analogici americani
- **Aggro**: ispirato ai ruggenti sintetizzatori analogici giapponesi
- **Acid**: ispirato agli psichedelici

Classic, Aggro e Acid possono risuonare moltissimo in questa versione alpha. Parti da valori bassi e alzali con delicatezza.

Il filtro alimenta un VCA controllato da un inviluppo ADSR. La forma dell'inviluppo può trasformarsi tra curve esponenziale, lineare e logaritmica.

#### Plaits e Plaits-Alt

**Plaits** offre 24 motori, raggruppati nel popup di selezione come segue:

| Categoria | Motori |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** è una collezione di modelli alternativi per il modulo Plaits originale. La maggior parte è più strana o più sperimentale del VCO medio.

| Categoria | Motori |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

I parametri seguono il progetto di Mutable:

- **Harmonic** controlla le relazioni armoniche, l'equilibrio o la scelta del modello all'interno di un motore.
- **Timbre** va in genere da spettri scuri/rarefatti a spettri brillanti/densi.
- **Morph** esplora un'altra dimensione timbrica.
- **Main/Aux** miscela l'uscita principale con l'uscita alternativa del motore.

Il loro significato preciso dipende dal motore. Per esempio, i motori di accordi li usano per tipo di accordo, rivolto e forma d'onda. I modelli fisici li usano per materiale, eccitazione e decadimento, mentre i motori di batteria li usano per tono, carattere e decadimento. Consulta il manuale utente di Plaits per i dettagli.

Per `CHORD`, `DIATONIC CHORD` e `WT DIATONIC CHORD`, Harmonic mostra il nome dell'accordo selezionato (`MIN`, `MAJ`, `MIN7`, `MAJ7` e così via) anziché il suo valore di controllo grezzo. Anche il `WAVE PARA` di Braids dà un nome ai suoi voicing di Color.

Tocca **Engine** per scegliere tra elenchi di motori per categoria.

**Env Mode** ha 2 instradamenti:

- `TRIG` riproduce il modulo con TRIG collegato e LEVEL non collegato
- `VCA` tiene LEVEL aperto e applica l'ADSR del tracker dopo la voce

#### Sampler

Questo motore di campioni pulito riproduce campioni PCM mono o stereo caricati in RAM.

- Tocca **Sample** per caricare un WAV PCM non compresso a `8 bit`, `16 bit` o `24 bit`. Premi **PLAY** nel browser per ascoltare il file evidenziato.
- Quando è caricato un campione, accanto al suo nome compare **EDIT**. Toccalo per aprire la schermata Sample Edit. **OPT** o **SELECT + [LEFT]** torna allo strumento.
- La schermata Sample Edit mostra il nome del file (con un indicatore `*` quando il campione in RAM differisce dal file su disco), un'indicazione del formato (frequenza di campionamento, canali), una forma d'onda alta e i campi **Region**, **Select**, **Slice**, **Process** e **File**.
- Le modifiche vivono solo in RAM finché un flusso di salvataggio non le scrive: il progetto memorizza il percorso del file del campione e ricarica il WAV da disco al successivo caricamento del progetto, quindi uscire dal tracker senza **Save**/**Save As** scarta le operazioni di elaborazione (l'indicatore `*` è l'avviso). Anche caricare un altro campione nello strumento scarta le modifiche non salvate e riporta la Region all'intero campione (Start `00`, End `FF`).
- **Region** imposta i marcatori di riproduzione Start/End, disponibili anche nella schermata dello strumento Sampler. Fissano limiti di riproduzione normalizzati (`00-FF`); se Start è dopo End, il campione viene riprodotto al contrario. **EDIT + [LEFT/RIGHT]** regola finemente il marcatore a passi di uno (`01`) e ingrandisce la forma d'onda attorno a esso finché **EDIT** è tenuto premuto; **EDIT + [UP/DOWN]** regola in modo grossolano (passo 16) e riporta la vista all'intero campione. **EDIT + OPT** riporta il marcatore al valore predefinito (Start `00`, End `FF`).
- La vista della forma d'onda ingrandisce una finestra fissa di un secondo di audio attorno al marcatore modificato e scorre per mantenerlo visibile; i campioni che stanno nella finestra restano nella vista completa 1:1. L'ingrandimento è transitorio: dura finché **EDIT** è tenuto premuto, e rilasciare **EDIT** riporta alla vista completa 1:1, come i passi grossolani. Entrare nella schermata riporta sempre la vista all'intero campione.
- **Select** imposta una selezione di elaborazione in frame assoluti, indipendente dai marcatori di riproduzione Start/End. **EDIT + [LEFT/RIGHT]** sposta una maniglia di venti frame e ingrandisce su di essa finché **EDIT** è tenuto premuto; **EDIT + [UP/DOWN]** salta di `frameCount/64` (minimo 16) e torna alla vista dell'intero campione. **EDIT (tocco)** su una maniglia le copia la posizione corrente del marcatore Start o End. **EDIT + OPT** su una delle due maniglie svuota l'intera selezione. Quando la selezione è vuota entrambe le maniglie mostrano `-`; quando le maniglie sono invertite si scambiano automaticamente. La selezione è uno stato dell'editor valido solo per la sessione: non viene salvata con il progetto e, entrando nella schermata, viene inizializzata con l'estensione della Region di riproduzione (l'intero campione con i marcatori predefiniti).
- **Slice** è una riga unica con quattro celle di valore - **Mode**, **Count**,
  **Slice** e **Frame** - e viene salvata con lo strumento. Off riproduce la
  finestra Start/End. Le note della frase selezionano gli slice in modo cromatico a partire da **C-0**, e
  le note oltre l'ultimo slice ricominciano dall'inizio (la nota N riproduce lo slice
  `N % count`, quindi una serie oltre l'ultimo slice torna al primo).
  Le note tagliate non traspongono l'altezza né usano la quantizzazione Scale, ma **CRD**
  mantiene lo slice selezionato e traspone le sue voci come un accordo. Gli inizi degli slice
  sono indicati sulla forma d'onda da brevi linee larghe due pixel (arancione scuro; la linea
  dello slice corrente è più luminosa), e lo slice sotto il cursore riceve una
  banda di sfondo nera sulla forma d'onda (la tinta della selezione non viene disegnata sopra
  la banda, che resta completamente nera e nettamente distinta dal resto
  dell'anteprima).
  - **Mode** scorre `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + sinistra/destra o
    tocco; EDIT + OPT disattiva il taglio). Cambiare modalità inizializza i punti
    di slice: **EQUAL** divide la finestra Start/End in parti uguali in base a **Count**;
    **AUTO** esegue un rilevamento dei transienti sulla finestra e mette uno
    slice su ogni attacco trovato (fino a **Count**); **LAZY** parte con uno
    slice che copre l'intera finestra. Passare a LAZY mette da parte l'impostazione
    EQUAL/AUTO corrente (numero e confini), e tornare a EQUAL o
    AUTO la ripristina tale e quale - non viene chiesta alcuna conferma e non si
    perde nulla. Slice e Stretch si escludono a vicenda: attivarne uno disattiva
    l'altro, e le celle Slice sono attenuate mentre Stretch determina la
    durata.
  - **Count** mostra il numero totale di slice (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** lo cambia di uno alla volta (minimo 1, massimo 64), **EDIT +
    [UP/DOWN]** scorre i numeri in potenza di due `2 / 4 / 8 / 16 / 32 /
    64` con ritorno all'inizio. Ogni modifica ricalcola la divisione: **EQUAL**
    ridivide la finestra in parti uguali, **AUTO** riesegue il rilevamento con il nuovo
    obiettivo (la sensibilità del rilevamento deriva dal numero - più slice
    significano una soglia più sensibile). La casella mostra `-` quando la modalità è
    OFF. In **LAZY** resta luminosa (conta gli slice posizionati a mano) ma
    serve solo per la visualizzazione: il cursore la salta - spostarsi a destra da **Mode** o
    a sinistra da **Slice** porta su **Slice**, scendere dal marcatore **END**
    della riga **Select** porta lì ugualmente, e toccare la casella non fa nulla
    (gli slice posizionati a mano non hanno una divisione da ricalcolare). **EDIT + OPT** e
    un semplice tocco di **EDIT** qui non fanno nulla: disattivare il taglio si fa
    su **Mode**, eliminare uno slice si fa su **Slice**.
  - **Slice** è il browser: mostra lo slice corrente, numerato da 1
    (`01`..count). **EDIT + [LEFT/RIGHT]** passa da uno slice all'altro uno alla
    volta, **EDIT + [UP/DOWN]** salta di quattro slice (entrambi limitati, senza
    ritorno all'inizio); lo slice esplorato è indicato sulla forma d'onda dal suo marcatore
    più luminoso e dalla banda nera, e la vista ingrandita si ricentra sull'inizio dello
    slice. **EDIT + OPT** elimina lo slice corrente (si fonde con
    il precedente; il primo slice non può essere eliminato, ed eliminare
    l'ultimo slice rimasto disattiva il taglio - i confini sopravvivono a un'andata e ritorno
    attraverso OFF e tornano quando si sceglie di nuovo una modalità). **EDIT (tocco)**
    ascolta lo slice attualmente selezionato come riproduzione singola (funziona in
    ogni modalità di slice; l'ascolto si ferma quando si rilascia il tasto o si
    esce dalla schermata). La casella mostra `-` quando la modalità è OFF.
  - **Frame** mostra in esadecimale il frame iniziale dello slice corrente. **EDIT +
    [LEFT/RIGHT]** lo sposta di dieci frame (ingrandendo sul marcatore finché
    **EDIT** è tenuto premuto), **EDIT + [UP/DOWN]** lo sposta di cento e
    riporta alla vista completa. Un marcatore resta dentro il proprio slice: non
    può superare l'inizio dello slice precedente o successivo. **EDIT + OPT** elimina
    lo slice.
  - **B** torna alla schermata dello strumento dalla riga Slice, come ovunque
    in questa schermata; **SHIFT + LEFT** esce da qualsiasi punto.
  - **Flusso di lavoro LAZY**: con LAZY selezionato, tocca **PLAY** per avviare una
    riproduzione singola dell'intero campione - suona una volta dall'inizio e si ferma
    da solo, all'altezza e alla velocità originali del campione (un marcatore verde di due pixel
    segue la posizione sulla forma d'onda); toccare **PLAY** di nuovo
    lo ferma prima. Mentre suona,
    ogni clic su **EDIT** lascia uno slice alla posizione di riproduzione, ovunque sia
    il cursore (gli slice a meno di 50 ms da uno esistente vengono
    rifiutati con `Too close to slice`). Riprodurre di nuovo il campione mantiene
    i tagli esistenti e ne aggiunge di nuovi a ogni posizione toccata, in
    ordine - la casella **Count** si aggiorna a ogni slice lasciato e il
    browser **Slice** segue il nuovo. Agli slice posizionati a mano si applicano le stesse regole di
    esplorazione e modifica delle altre modalità: **EDIT +
    direzione** su **Slice** li esplora, **Frame** regola l'inizio
    selezionato, **EDIT + OPT** elimina lo slice corrente. La cella Frame è attenuata
    mentre il rilascio durante la riproduzione è armato. **SHIFT + PLAY** non viene intercettato
    in LAZY: avvia la riproduzione della frase come su ogni altra schermata. Gli slice
    LAZY suonano nel sequencer esattamente come EQUAL e AUTO: le note della frase
    vengono associate in modo cromatico a partire da **C-0** agli slice posizionati a mano,
    con ritorno all'inizio dopo l'ultimo.
  - **Suggerimenti sulle combinazioni**: mentre il cursore riposa su una cella della riga Slice e non viene mostrato altro
    messaggio, la barra di stato mostra il suggerimento di quella cella e lo mantiene
    finché il cursore resta lì. Su **Mode** il suggerimento è legato alla
    modalità attiva e mostra solo la descrizione (il nome della modalità è già nella
    cella): `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - cambiare modalità
    sostituisce subito il testo, e OFF svuota la barra. Su **Count** mostra
    `Adjust the number of slices`, su **Slice**
    `Browse slices`, su **Frame** `Adjust slice start`. Qualsiasi messaggio di azione
    sostituisce il suggerimento finché non scade.
- **Process** seleziona un'operazione di modifica distruttiva: **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** o **Reverse** (EDIT + sinistra/destra scorre, il tocco avanza, EDIT + OPT non ne sceglie nessuna). **GO** (nella stessa riga) esegue l'operazione selezionata sulla selezione corrente, oppure sull'intero campione quando la selezione è vuota. **Crop** mantiene solo la selezione; **Normalize** scala la selezione in modo che il suo picco raggiunga il fondo scala (i due canali condividono un guadagno per preservare l'immagine stereo); **Delete** rimuove la selezione e unisce le code (eliminare l'intero campione viene rifiutato); **Silence** la azzera; **Fade In**/**Fade Out** fanno salire o scendere linearmente la selezione dal/verso il silenzio; **Reverse** riproduce la selezione al contrario (i frame vengono scambiati sul posto, i due canali di un frame si muovono insieme, lunghezza e marcatori non cambiano). Crop e Delete richiedono una selezione - con una selezione vuota mostrano `Select region first`. Ogni operazione mette brevemente in pausa l'audio, conserva un livello di annullamento e segna il campione come modificato in RAM: il file su disco non viene toccato fino ai flussi di Save (vedi sotto), e uscire dalla schermata scarta lo slot di annullamento.
- **UNDO** (accanto a GO) riporta il campione allo stato precedente all'ultima operazione. È attenuato finché non viene eseguita un'operazione, alterna tra gli stati prima e dopo l'operazione a pressioni ripetute, e viene svuotato quando si rientra nella schermata.
- **File** contiene i flussi di salvataggio. Lo strumento memorizza il percorso completo del WAV da cui è stato caricato; questi flussi scrivono quel file o fanno puntare lo strumento a uno nuovo. Non toccano mai il nome dello strumento.
  - Il campo File alterna tra **Save** e **Save As** (EDIT + sinistra/destra o tocco); **GO** (nella stessa riga) esegue l'azione mostrata.
  - **Save** sovrascrive il WAV da cui è stato caricato il campione. Per un campione
    senza slice chiede prima `Overwrite <name>?`. Un campione tagliato apre prima
    la finestra **SAVE SLICES**, che chiede a quale destinazione appartengano i punti di slice:
    **SAVE TO SAMPLE** li scrive nel WAV come chunk cue (portabile -
    le DAW li mostrano come marcatori) e li mantiene anche nel progetto,
    **SAVE TO PROJECT** scrive un WAV semplice e mantiene i punti
    solo nel progetto, **CANCEL** interrompe. Una casella `Don't ask again in this project`
    memorizza la destinazione scelta con il progetto e salta
    la finestra da quel momento (la scelta viene salvata come `- Sample save choice:` nel
    .cct e può essere cambiata solo modificando il file del progetto). La
    finestra viene saltata per i campioni senza slice. **Save** è attenuato (e
    saltato nella navigazione) finché non è caricato un campione con un percorso di file. Una
    scrittura fallita mantiene l'indicatore di modifica e mostra l'errore.
  - **Save As** scrive il campione corrente in un nuovo file: inserisci un nome di file (precompilato con il nome corrente senza estensione), poi scegli una cartella nel browser. Il campione viene scritto come `<folder>/<name>.wav`, lo strumento punta al nuovo file e la cartella viene ricordata come cartella dei campioni predefinita. È attenuato (e saltato nella navigazione) finché non sono caricati dati del campione, così a un campione appena caricato si può assegnare un percorso di file. I percorsi di più di 255 caratteri vengono rifiutati.
  - L'indicatore `*` prima del nome del file significa che il campione in RAM differisce dal file su disco (qualsiasi operazione di elaborazione lo imposta, Save e Save As lo cancellano). L'indicatore vale solo per la sessione: non viene salvato con il progetto e si azzera quando si rientra nella schermata. Se più strumenti fanno riferimento allo stesso file WAV, salvare uno sovrascrive il file per tutti.
- Nella schermata dello strumento Sample, usa **EDIT + [LEFT/RIGHT]** per caricare il WAV precedente o successivo nella stessa cartella.
- **Pitch** traspone per semitoni (da `-48` a `+48`).
- **Loop** sceglie Off, Loop o Ping-Pong.
- **Speed** controlla il time-stretching granulare da `0%` a `500%` (`100%` è normale).

I formati WAV non supportati mostrano un errore. Converti i file insoliti in WAV `PCM8`, `PCM16` o `PCM24` prima di importarli.

#### 2xSCWF

2xSCWF è un oscillatore doppio a forma d'onda a ciclo singolo. Carica in ciascun oscillatore 1 WAV mono contenente esattamente 1 periodo. Entrambi gli oscillatori leggono in avanti e ricominciano alla fine del loro ciclo.

Mix fa una dissolvenza incrociata tra A e B.

Detune è fine ed esponenziale dall'unisono fino a `+200 ct`, poi procede a semitoni da `+3 st` a `+24 st`.

2xSCWF tratta ogni file come una tabella a 1 ciclo e lo legge esattamente una volta per ciclo di oscillatore. Questo permette di miscelare e scordare 2 onde a ciclo singolo.

La cartella di fabbrica `waveforms/AKWF/` contiene forme d'onda della libreria Adventure Kid. Molte altre sono disponibili online se vuoi ampliare la collezione.

#### BYOWTBL

BYOWTBL è un oscillatore doppio a tabella d'onda compatibile con le tabelle di Serum. Ogni oscillatore carica un WAV mono contenente frame a ciclo singolo consecutivi. `Pos A` e `Pos B` scorrono le loro tabelle in modo indipendente da `00` a `FF`. Il motore interpola linearmente all'interno di ogni frame e tra frame adiacenti. `Mix` fa una dissolvenza incrociata tra gli oscillatori, e `Detune` sposta l'oscillatore B.

## 6. Modulazione e registrazione del movimento

Ogni strumento può avere fino a 4 slot di modulazione. Gli strumenti audio possono avere come destinazione **Instrument Pan** o **Track Pan**; entrambi usano `00` per sinistra, `80` per il centro e `FF` per destra.

I tipi di modulazione disponibili sono:

- ADSR - classico inviluppo attack-decay-sustain-release
- AHD - inviluppo attack-hold-decay
- LFO - oscillatore a bassa frequenza con diverse forme
- SLFO - LFO con un moltiplicatore di periodo per i cicli lenti
- FLFO - LFO a frequenza audio che può raggiungere la gamma dei kHz; il FLFO può essere pesante per la CPU
- SLIN - controllo lineare tramite joystick
- SRAT - controllo tramite joystick basato sulla velocità di variazione

L'elenco delle destinazioni di modulazione dipende dal tipo di strumento.

Per una destinazione `Cutoff`, Amount è esponenziale con una risposta quadratica: il 25% raggiunge 4,5 semitoni, il 50% raggiunge 18 semitoni, il 75% raggiunge 40,5 semitoni e il 100% raggiunge sei ottave. Questo mantiene un controllo fine ai valori bassi permettendo comunque spazzate profonde.

Quando un inviluppo ADSR o AHD ha come destinazione Volume, diventa l'inviluppo di volume dello strumento. Un LFO che ha come destinazione Volume sposta invece il volume di uscita corrente.

La quantità di modulazione può essere positiva o negativa. Il suo intervallo dipende dalla destinazione. Se l'intervallo completo della destinazione è `127` o meno, come per la maggior parte dei parametri, la quantità definisce un intervallo assoluto. Destinazioni più ampie come l'altezza usano una quantità scalata.

Tutti i parametri di durata, inclusi attack, hold, decay e periodo, si misurano in tick.

Forme dell'LFO:

- Tri - onda triangolare bipolare
- Sin - onda sinusoidale bipolare
- UniTri - onda triangolare unipolare
- UniSin - onda sinusoidale unipolare
- RampDn - rampa lineare discendente (onda a dente di sega)
- RampUp - rampa lineare ascendente (onda a dente di sega)
- ExpDn - rampa esponenziale discendente (dente di sega esponenziale)
- ExpUp - rampa esponenziale ascendente (dente di sega esponenziale)
- Square - onda quadra
- Random - onda casuale sample-and-hold
- Wavetable - legge una delle tabelle d'onda AY a 32 passi del progetto. I valori da `00` a `0F` corrispondono da `-100%` a `+100%`, senza interpolazione.

Tipi di attivazione dell'LFO:

- Free - riavvia l'LFO solo quando cambia lo strumento
- Retrig - riavvia l'LFO a ogni nota
- Phrase - riavvia LFO e SLFO quando la riproduzione entra in una nuova frase
- Chain - riavvia LFO e SLFO quando la riproduzione entra in una nuova catena
- Hold - si ferma dopo 1 ciclo e mantiene l'ultimo valore
- Once - si ferma dopo 1 ciclo e torna a zero

### Registrazione del movimento (Motion recording)

- **STICK LIVE** (`L1` per impostazione predefinita) applica la modulazione degli stick senza modificare la frase. In **Settings > Stick live mode**, `HOLD` (il valore predefinito) la attiva finché è tenuto premuto; `TOGGLE` la attiva con una pressione e la disattiva con la successiva; `FREE` la lascia attiva in modo permanente. Rilasciare il pulsante in `TOGGLE` la lascia attiva.
- Durante la riproduzione, tieni premuto **MOTION RECORD** (`L2`) per applicare la modulazione degli stick e registrare le destinazioni modificate come valori FX assoluti.
- Tieni premuto **MOTION ERASE** (`R2`) per rimuovere dalla riga corrente gli FX di destinazione corrispondenti.

La `~` in basso a destra indica che Stick live è attivo. Record (`*`, o `!` in caso di overflow) e Erase (`x`) hanno priorità di indicatore e restano momentanei in entrambe le modalità. Entrambi attivano l'elaborazione degli stick in tempo reale finché sono tenuti premuti; rilasciarli lascia intatto un interruttore live attivato separatamente.

La modalità viene salvata con le impostazioni dell'applicazione, ma l'interruttore live parte sempre disattivato all'avvio. Cambiare modalità azzera l'interruttore: `HOLD` segue qualsiasi pulsante Stick live attualmente tenuto premuto, `TOGGLE` attende una nuova pressione e `FREE` è attivo subito.

La registrazione del movimento scrive FX di traccia nella frase in riproduzione. Aggiorna prima gli FX corrispondenti, poi usa gli slot vuoti da destra a sinistra. Non sovrascrive mai un FX diverso. Se tutti e 3 gli slot sono pieni, quel movimento non viene registrato sul passo. Un `!` nell'angolo in basso a destra significa che sono cambiate più destinazioni di quante ne potessero contenere le 3 colonne FX.

La registrazione del movimento supporta le destinazioni di Braids, Plaits, Sampler, 2xSCWF, BYOWTBL e Bogie, comprese le macro Bogie attive e i controlli del filtro dove applicabile.

## 7. Tables

Le tabelle sono uno strumento centrale di sound design nei tracker. Nei termini di Vortex Tracker, combinano strumenti e ornamenti, ma possono fare molto di più. Se conosci LSDj o NerdSEQ, l'idea dovrebbe esserti già familiare.

La colonna Pitch accetta valori di altezza relativi (`~`) o assoluti (`=`) in semitoni. La colonna Volume va da `00` a `0F`, si applica a ogni motore sopra il suo inviluppo e può servire come gate: alterna `0F` e `00`, poi crea un loop con `HOP 00`. Le 4 corsie FX corrispondono per lo più alle corsie di una frase, anche se alcuni FX si comportano diversamente nelle tabelle.

Mettere un FX `TIC` sull'ultima riga della tabella imposta la velocità di quella colonna e sostituisce la velocità di tabella predefinita dello strumento. Ogni colonna FX può funzionare a una diversa velocità di tick. Le colonne Pitch e Volume seguono la velocità della 1ª colonna FX.

Una tabella di 16 righe può sembrare corta accanto a un lungo strumento di Vortex Tracker, ma l'FX `HOP` può creare loop condizionali come «ripeti queste righe 5 volte», oltre a loop annidati. Con i loop e le velocità indipendenti delle colonne FX, quelle 16 righe possono arrivare sorprendentemente lontano.

### Modalità Retrigger

Il campo `Retrig` in cima alla schermata Table sceglie quando una tabella già attiva riparte. `Inst` è la modalità predefinita e compatibile: una riga di frase che contiene uno strumento seleziona la tabella predefinita di quello strumento e la avvia alla riga `00`. `Phrase` riavvia una tabella attiva quando la riproduzione entra in una frase, `Chain` quando entra in una catena, e `Free` la lascia in esecuzione finché la traccia non si ferma o un comando esplicito la sostituisce.

Una modalità strutturale non torna mai indietro nel tempo. Se una traccia non ha una tabella di strumento attiva, la prima riga di frase che introduce uno strumento ne attiva la tabella esattamente su quella riga, anche se è la riga `4`, `8` o `F` di una frase. I numeri di strumento successivi non sostituiscono una tabella `Phrase`, `Chain` o `Free` attiva. Questo rende le tabelle utili come automazione di tipo LFO senza simulare in silenzio passi di frase precedenti.

`TBL`, `TBX` e `RET` sono sempre azioni esplicite: selezionano o riavviano immediatamente, qualunque sia la modalità. A un confine di Phrase o di Chain, il suo riavvio avviene prima che venga letta la nuova riga di frase, quindi un `TBL` o `TBX` su quella riga ha l'ultima parola. Gli FX della riga `00` della tabella vengono quindi eseguiti di nuovo normalmente; gli FX cumulativi mantengono la loro consueta accumulazione.

Le tabelle `00-7F` sono riservate alle tabelle di strumento predefinite. Le tabelle `80-FE` sono pensate per le tabelle ausiliarie avviate dall'effetto `TBX`. L'intervallo basso funziona anche per le tabelle ausiliarie, ma usarlo così può causare conflitti e confusione inattesi.

### Copiare FX tra Phrase e Table

Seleziona solo colonne di nome/valore FX, poi copiale o tagliale da una Phrase e incollale in una qualsiasi corsia FX di una Table, o viceversa. La copia mantiene esattamente le celle selezionate, quindi una selezione di soli comandi o soli valori resta parziale. L'incollaggio inizia dalla cella FX corrente e viene troncato alla riga `F` e all'ultima corsia FX della destinazione. Note, numeri di strumento, altezza e volume non passano mai tra queste due schermate.

### Controlli

- **EDIT + [UP/DOWN]** su una colonna di nome FX: apre la schermata di selezione FX
- **OPT + [DIRECTION]**: spostarsi tra le tabelle

## 8. Altre schermate

### Instrument Pool

L'Instrument Pool mostra ogni strumento del progetto e permette di riordinarli. Durante la riproduzione mostra anche quali strumenti sono attualmente attivi.

#### Controlli

- **EDIT**: modifica lo strumento e passa alla schermata Instrument
- **SHIFT + OPT**: copia lo strumento
- **SHIFT + EDIT**: incolla lo strumento
- **EDIT + [UP/DOWN]**: riordina gli strumenti
- **EDIT + PLAY**: ascolta lo strumento

### AY Wavetable

Wavetable è un tipo di oscillatore software di AY Plus. Un progetto può contenere fino a 256 tabelle d'onda, ciascuna di 32 passi. Tutti gli strumenti a tabella d'onda condividono lo stesso insieme di onde. È in questa schermata che le modifichi.

I livelli di volume dell'AY non sono lineari, quindi la forma d'onda della vista Standard viene disegnata in modo da corrispondere ai livelli di uscita reali. Tocca **OPT** due volte per alternare tra la vista Standard e la **vista LFO** persistente, con la stessa scorciatoia di Song/Live Mode: la vista LFO disegna linearmente i passi grezzi da `0` a `15`, con la linea centrale tra `7` e `8` come punto zero della modulazione.

La schermata ha 2 righe logiche. La riga superiore contiene i pulsanti **Load** e **Save**, mentre la 2ª contiene l'editor della tabella d'onda.

#### Controlli

- **OPT + [LEFT/RIGHT]**: scorrere l'elenco delle tabelle d'onda
- **OPT + [UP/DOWN]**: scorrere l'elenco di 16 onde alla volta
- **SHIFT + OPT**: copia la tabella d'onda
- **SHIFT + EDIT**: incolla la tabella d'onda
- **OPT + PLAY**: alterna tra le viste Standard e LFO

## 9. Tracker FX

Gli FX di traccia forzano valori del sequencer o del motore sonoro su singoli passi. Altre groovebox li chiamano spesso «parameter lock». Ogni riga di Phrase ha 3 slot FX.

Ogni FX ha un comando di 3 lettere e un valore esadecimale. Il pannello di aiuto integrato nell'app fornisce una breve descrizione del comando selezionato.

### Automazione della scala (Scale)

`SCL XY` cambia la scala globale di riproduzione e la fondamentale senza cambiare le note memorizzate nelle frasi. `X` seleziona una scala (`0` Chromatic, `1` Major, `2` Minor, fino a `C` Custom); `Y` seleziona la fondamentale (`0` C fino a `B` B). Il comando si applica solo agli FX di frase, non alle tabelle. Se più tracce emettono `SCL` nello stesso tick, vince la traccia con il numero più basso.

`SCL` appartiene alla modalità Quantizer. Con **Note Lock** attivo il comando non viene proposto nell'elenco FX, e qualsiasi `SCL` già scritto nelle frasi viene ignorato finché la modalità è attiva.

### Accordi (Chords)

`CRD XY` trasforma la nota della sua stessa riga di Phrase in un accordo. `Y` seleziona: `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9 o `F` Min9. `X` seleziona uno dei 16 voicing: `0-3` sono un ciclo ascendente di rivolti stretti; `4-7` aggiungono il drop-2; `8-B` aggiungono il drop-3; e `C-F` combinano entrambi i drop. Il ciclo prosegue attraverso un'ottava aggiuntiva per le triadi e i power chord, quindi ogni valore è distinto; i valori più grandi diventano progressivamente più ampi e meno convenzionali. Per esempio, `CRD 4Y` è posizione fondamentale in drop-2 e `CRD DY` è il primo rivolto con entrambi i drop. In fondo o in cima alla tabella delle altezze, le voci vengono limitate al suo intervallo. Il comando non è persistente: scrivilo su ogni riga con accordo. Ogni nota generata viene quantizzata in modo indipendente quando Scale è attivo, sia in modalità Quantizer sia in Note Lock. È disponibile solo per i motori software; gli strumenti AY lo ignorano e restano monofonici.

### FX del sequencer

| FX | Valore | Comportamento dettagliato |
|---|---|---|
| `ARP` | `XY` | Arpeggia la nota base, i passi `+X` e i passi `+Y`. `37` produce un pattern di accordo minore. |
| `ARC` | `XY` | `X` seleziona la direzione dell'arpeggio o la modalità di intervallo; `Y` è la sua velocità in tick. |
| `PVB` | `XY` | Vibrato d'altezza: `X` è la velocità e `Y` la profondità. In modalità Linear, la profondità usa passi da `10 cent`. |
| `PBN` | `XX` con segno | Aggiunge `XX` unità d'altezza a ogni riga di frase/tabella; `FF` significa `-1`. Usa `00` per fermare. |
| `PSL` | `XX` tick | Scivola dall'altezza precedente alla nuova nota in `XX` tick. |
| `PIT` | `XX` con segno | Offset relativo accumulato, in passi della tabella delle altezze. |
| `FIN` | `XX` con segno | Offset fine accumulato, in cent con altezza Linear, in unità di periodo altrimenti. |
| `PRD` | `XX` con segno | Offset relativo accumulato del periodo dell'oscillatore. |
| `VOL` | `XX` con segno | Offset relativo accumulato del volume (`FF` è -1, non il volume massimo). |
| `VSL` | `XX` con segno | Aggiunge `XX` al volume a ogni riga di frase/tabella. Usa `00` per fermare. |
| `PAN` | `00-FF` | Panorama assoluto dello strumento: sinistra, centro (`80`), destra. |
| `TPN` | `00-FF` | Panorama assoluto della traccia del mixer: sinistra, centro (`80`), destra. |
| `RET` | `XY` | Riattiva ogni `Y` tick; `X` applica una variazione di volume. `Y=0` ferma la riattivazione. |
| `DEL` | `XX` tick | Ritarda il note-on. Un ritardo più lungo del passo di groove corrente fa saltare la nota. |
| `OFF` | `XX` tick | Invia il note-off dopo `XX` tick ed entra in una fase di release dell'ADSR. |
| `KIL` | `XX` tick | Interrompe bruscamente la voce dopo `XX` tick senza eseguire il release dell'ADSR. |
| `TIC` | `XX` tick | Imposta i tick di tabella per riga. In una tabella cambia la velocità di quella colonna FX. |
| `TBL` | `00-FE`, `FF` spento | Sostituisce e riavvia subito la tabella dello strumento; `FF` la ferma. |
| `TBX` | `00-FE`, `FF` spento | Avvia o sostituisce subito una tabella ausiliaria; `FF` la ferma. |
| `THO` | riga `XX` | Porta tutte le colonne della tabella dello strumento alla riga `XX`. |
| `TXH` | riga `XX` | Porta tutte le colonne della tabella ausiliaria alla riga `XX`; non si usa dall'interno di una tabella. |
| `GRV` | groove `XX` | Seleziona un groove per la traccia corrente. |
| `GGR` | groove `XX` | Seleziona un groove per ogni traccia. |
| `HOP` | `XY` | Salta alla riga `Y`, `X` volte; `X=0` ripete all'infinito. L'HOP di tabella agisce sulla propria colonna. |
| `SNG` | `XX` con segno | Sposta la riproduzione di `XX` righe di Song. |
| `PRO` | `00-64` | Valuta una probabilità di attivazione assoluta da `0%` a `100%`. |
| `MOD` | `AB` | Si attiva al passaggio `A` di un ciclo di `B` passaggi. |
| `SPD` | `XX` con segno | Seleziona un rapporto di clock persistente per traccia; vedi la tabella delle velocità più sotto. |
| `SLE` | `00-FF` tick | Imposta il tempo di glissando persistente per traccia per gli FX continui del motore. `00` è immediato. L'impostazione torna a `00` quando la riproduzione si ferma. |

### Mandate di effetti audio (Audio FX sends)

| FX | Valore | Comportamento dettagliato |
|---|---|---|
| `RSN` | `00-FF` | Imposta la mandata di reverb di questa traccia fino alla prossima attivazione di nota. |
| `DSN` | `00-FF` | Imposta la mandata di delay di questa traccia fino alla prossima attivazione di nota. |

### FX ADSR / di attivazione

| FX | Valore | Comportamento dettagliato |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Sostituiscono attack, decay, sustain, release o la forma dell'inviluppo. |
| `TDC` / `TCL` | `00-FF` | Sostituiscono Decay o Color della modalità Trigger su Plaits e Plaits-Alt. |

#### Condizioni

Ispirato alle «trig conditions» svedesi, il tracker supporta probabilità di attivazione e condizioni di modulo.

| FX | Valore | Significato |
|---|---|---|
| `PRO` | `00-64` | Probabilità di attivazione assoluta da `0%` a `100%` |
| `MOD` | `AB` | Si attiva all'iterazione `A` di `B`; per esempio `12`, `22`, `14`, `34` |

I contatori `MOD` sono locali alla traccia e alla frase. Le combinazioni non valide, come `A` maggiore di `B`, non si attivano.

`MOD34` si attiva al 3º passaggio di ogni ciclo di 4 passaggi. `MOD1F` si attiva al 1º passaggio di ogni ciclo di 16 passaggi.

#### Velocità di riproduzione

Come NerdSEQ, ChooChooTracker supporta una velocità di riproduzione indipendente per ogni traccia.

`SPD` legge il suo valore come un byte con segno. `00` è la velocità normale, i valori positivi velocizzano la traccia e quelli negativi la rallentano. L'impostazione resta attiva finché un altro comando `SPD` non la cambia.

| Valore | Velocità |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

I vecchi progetti senza supporto per `SPD` con segno mantengono la loro corrispondenza originale `00-10`, quindi suonano come erano stati salvati.

Moltiplicatori molto alti possono superare la risoluzione dello scheduler dei tick del tracker e non sono stati testati su hardware.

### FX di modulazione (Modulation FX)

Gli FX di frase e di tabella possono cambiare uno slot di modulazione senza modificare lo strumento:

| Schema FX | Significato |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Offset relativo di Amount per gli slot di modulazione 1-4 |
| `M11`, `M12`, `M13`, `M14` | Offset relativi P1-P4 per lo slot di modulazione 1 |
| `M21`, `M22`, `M23`, `M24` | Offset relativi P1-P4 per lo slot di modulazione 2 |
| `M31`, `M32`, `M33`, `M34` | Offset relativi P1-P4 per lo slot di modulazione 3 |
| `M41`, `M42`, `M43`, `M44` | Offset relativi P1-P4 per lo slot di modulazione 4 |

Il valore viene interpretato come una variazione relativa con segno a `8 bit` (`01` aggiunge `1`, `FF` sottrae `1`). I comandi ripetuti si accumulano. I valori effettivi vengono limitati al loro intervallo valido.

`P5` non è esposto come FX di frase o di tabella. È disponibile come destinazione di modulazione `M1–M4 Wavetable`, così un modulatore può scorrere l'indice AYWavetable di un altro.

| Tipo di modulazione | P1 | P2 | P3 | P4 | P5 quando Shape è Wavetable |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Non usato |
| AHD | Attack | Hold | Decay | Non usato | Non usato |
| LFO | Shape | Trigger mode | Period | Non usato | Indice della tabella d'onda AY |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | Indice della tabella d'onda AY |
| FLFO | Shape | Trigger mode | Frequency (da `1 Hz` a `20 kHz`) | Non usato | Indice della tabella d'onda AY |

### Braids FX

| FX | Valore | Significato |
|---|---|---|
| `BMD` | `00-2E` | Modello Braids |
| `BTM` | `00-FF` | Timbre normalizzato assoluto |
| `BCL` | `00-FF` | Color normalizzato assoluto |
| `BCF` | `00-FF` | Frequenza di taglio esponenziale, da `20 Hz` a `20 kHz` |
| `BRS` | `00-FF` | Risonanza del filtro esponenziale |

### aChChid FX

| FX | Valore | Significato |
|---|---|---|
| `ASL` | `00-FF` | Scivola verso la nota senza riattivare. `00` è `60 ms`; i valori più alti allungano il glissando. |
| `ADC` | `00-FF` | Sostituzione del decay: da `200 ms` a `2 s`. |
| `AAC` | `00-FF` | Quantità di accento: da nessuna al massimo. |
| `ATM` | `00-FF` | Sostituzione del Timbre di Braids; attiva solo in modalità onda Braids. |
| `ACL` | `00-FF` | Sostituzione del Color di Braids; attiva solo in modalità onda Braids. |
| `ACF` | `00-FF` | Frequenza di taglio esponenziale del filtro del 303, da `20 Hz` a `20 kHz`. |
| `ARS` | `00-FF` | Risonanza del filtro del 303, da nessuna al massimo. |
| `AEM` | `00-FF` | Modulazione dell'inviluppo del filtro del 303, da nessuna al massimo. |

### Bogie FX

| FX | Valore | Significato |
|---|---|---|
| `DMD` | `00-0B` | Seleziona il modello Bogie fino alla prossima attivazione. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Sostituiscono Decay, Tone, Sweep, Noise, FM o Drive fino alla prossima attivazione. Tutte e sei le macro sono disponibili su ogni modello. |
| `DCF` | `00-FF` | Frequenza di taglio del filtro, con mappatura logaritmica. |
| `DRS` | `00-FF` | Risonanza del filtro. |

### Sintered FX

| FX | Valore | Significato |
|---|---|---|
| `SMD` | `00-05` | Seleziona il modello Sintered fino alla prossima attivazione. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Sostituiscono Decay, Mod, le prime due macro del modello, Motion o l'ultima macro del modello fino alla prossima attivazione. |
| `SCF`, `SRS` | `00-FF` | Frequenza di taglio e risonanza del filtro. |

### MIDI Out FX

| FX | Valore | Significato |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Invia un CC MIDI, riscalato a `0-127`. Il numero di CC a cui invia ciascuno slot si imposta per strumento (`MC1 number`-`MC4 number` nella schermata dello strumento), non dall'FX stesso - uno slot senza numero è inerte. |

### Plaits FX

| FX | Valore | Significato |
|---|---|---|
| `PMD` | `00-17` | Motore Plaits |
| `PHA` | `00-FF` | Harmonics normalizzato assoluto |
| `PTM` | `00-FF` | Timbre normalizzato assoluto |
| `PMO` | `00-FF` | Morph normalizzato assoluto |
| `PAX` | `00-FF` | Miscela Main/Aux: `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Frequenza di taglio esponenziale, da `20 Hz` a `20 kHz` |
| `PRS` | `00-FF` | Risonanza del filtro esponenziale |

### Sample FX

| FX | Valore | Significato |
|---|---|---|
| `SPL` | `00-03` | Modalità di riproduzione: `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Riproduce lo slice numerato (`01` = primo slice) indipendentemente dalla nota; `00` mantiene la normale associazione delle note. Richiede la modalità di taglio attiva sullo strumento |
| `SPT` | `XX` con segno | Trasposizione del campione in semitoni |
| `SST` | `00-FF` | Inizio di riproduzione normalizzato |
| `STA` | `00-FF` | Alias di `SST` (inizio di riproduzione normalizzato) |
| `SEN` | `00-FF` | Fine di riproduzione normalizzata |
| `SVL` | `00-FF` | Volume assoluto del campione |
| `SCF` | `00-FF` | Frequenza di taglio esponenziale, da `20 Hz` a `20 kHz` |
| `SRS` | `00-FF` | Risonanza del filtro esponenziale |
| `SSP` | `00-FF` | Velocità del campione, mappata da `0%` a `500%` |

### FX AY condivisi dagli strumenti AY

| FX | Valore | Comportamento dettagliato |
|---|---|---|
| `AYM` | `XY` | `X` è la forma dell'inviluppo; `Y` seleziona off/tone/noise/tone+noise (`0/1/2/3`). |
| `NOI` | `XX` con segno | Offset relativo accumulato del periodo del rumore. |
| `NOA` | `00-1F`, `FF` | Periodo del rumore assoluto; `FF` cede la priorità del periodo del rumore alle tracce precedenti. |
| `ERT` | qualsiasi | Riattiva la forma d'inviluppo hardware corrente. |
| `EAU` | `XY` | Rapporto di inviluppo automatico `X:Y`; `X=0` lo disattiva. |

### FX solo per AY Classic

| FX | Valore | Comportamento dettagliato |
|---|---|---|
| `EVB` | `XY` | Vibrato del periodo dell'inviluppo: `X` velocità, `Y` profondità. |
| `EBN` | `XX` con segno | Aggiunge `XX` al periodo dell'inviluppo a ogni riga di frase/tabella. |
| `ESL` | `XX` tick | Scivola dal periodo d'inviluppo precedente al nuovo valore. |
| `ENT` | nota `XX` | Imposta il periodo dell'inviluppo dalla nota della tabella delle altezze mostrata dall'interfaccia. |
| `EPT` | `XX` con segno | Offset relativo accumulato del periodo dell'inviluppo. |
| `EPL` | byte `XX` | Imposta il byte basso del periodo dell'inviluppo. |
| `EPH` | byte `XX` | Imposta il byte alto del periodo dell'inviluppo. |

### AY Plus FX

| FX | Valore | Comportamento dettagliato |
|---|---|---|
| `TNN` | nota `XX` | Imposta l'oscillatore di tono su una nota della tabella delle altezze, ignorando la nota della riga. |
| `TNP` / `TNF` | `XX` con segno | Offset accumulato del tono in passi d'altezza / fine. |
| `TRT` | qualsiasi | Riattiva la fase dell'oscillatore di tono dove supportato. |
| `ENN` | nota `XX` | Imposta l'oscillatore di inviluppo su una nota della tabella delle altezze. |
| `ENP` / `ENF` | `XX` con segno | Offset accumulato dell'inviluppo in passi d'altezza / fine. |
| `SFT` | `00-07` | Tipo software: None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | nota `XX` | Imposta l'oscillatore software su una nota della tabella delle altezze. |
| `SFP` / `SFF` | `XX` con segno | Offset accumulato dell'oscillatore software in passi d'altezza / fine. |
| `SRT` | qualsiasi | Azzera la fase dell'oscillatore software. |
| `SFM` | `XX` con segno | Offset accumulato della profondità FM. |
| `PWM` | `XX` con segno | Offset accumulato della larghezza d'impulso. |
| `SPL` | `XX` con segno | Offset accumulato del livello basso dell'impulso. |
| `SWT` | `XX` con segno | Offset accumulato dell'indice della tabella d'onda. |

### AY Sample FX

AY Sample accetta i comandi AY condivisi più `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` e `SFF` come descritto sopra.

| FX | Valore | Comportamento dettagliato |
|---|---|---|
| `SMS` | `00-FF` | Imposta l'inizio di riproduzione legacy di AY Sample a `XX x 64` campioni sorgente. |

Gli FX dei parametri del motore si applicano al loro tipo di strumento corrispondente e vengono azzerati alla prossima attivazione di nota, salvo diversa indicazione. `SPD` è persistente di proposito.

Il selettore FX filtra questo riferimento sui comandi comuni più il gruppo supportato dallo strumento attivo. Il pannello di aiuto contestuale fornisce i dettagli dei valori durante la modifica.

## 10. Mixer, Reverb e Delay

Il Mixer è la schermata principale più a sinistra. Mostra il carico della CPU e gli avvisi di clipping. Un `!` rosso nella colonna **CLIP** segna la traccia il cui segnale secco ha spinto il mix oltre l'intervallo sicuro.

Usalo per bilanciare le 8 tracce, dare forma a ciascuna traccia con il Tilt EQ e inviare l'audio al reverb o al delay:

| Campo | Significato |
|---|---|
| LVL | Livello della traccia dopo il motore, `000-100` |
| REV | Mandata al reverb Clouds condiviso, `000-100` |
| DLY | Mandata al delay ping-pong condiviso, `000-100` |
| TLT | Tilt EQ per traccia, `00-FF`; `80` è neutro, i valori bassi favoriscono i bassi e quelli alti gli acuti. I valori bassi introducono un lieve overdrive. |
| M | `*` silenzia questa traccia |
| S | `*` mette questa traccia in solo |

Il mixer è basato sulle tracce, non sugli strumenti. Se una traccia cambia strumento, il suo livello, PAN, Tilt e le mandate restano legati alla traccia. Anche gli strumenti hanno i propri **Vol** e **Pan** da `00-FF` nella riga superiore della schermata Instrument. Il PAN dello strumento viene applicato per primo, poi quello della traccia. Entrambi preservano un'immagine stereo esistente al centro; attenuano progressivamente il lato opposto quando vengono spostati a sinistra o a destra.

### Master FX

La voce **MASTER FX** apre un sottomenu Insert FX a due slot per l'intero
mix. I suoi effetti girano dopo i ritorni di reverb e delay, prima del volume
master. Seleziona un modulo, modifica i suoi parametri o mettilo in bypass esattamente come per un
insert di traccia; premi **OPT** per tornare al Mixer. Le impostazioni di Master FX vengono salvate
con il progetto e non sono automatizzate da frasi, tabelle o modulazione.

### Clouds Reverb

Usa **SELECT + [UP]** dal Mixer per aprire la schermata delle impostazioni del reverb.

- **Return**: livello del reverb umido nel mix master.
- **Time**: controllo di decadimento/tempo, `00-FF`.
- **Damping**: assorbimento delle alte frequenze, `00-FF`.
- **Filter**: filtro passa-basso applicato prima del reverb.

Questa è la sezione di reverb di Clouds, non il processore granulare Clouds completo.

### Ping-pong Delay

Usa **SELECT + [DOWN]** dal Mixer per aprire Delay, poi **SELECT + [UP]** per tornare. Reverb funziona nel verso opposto: **SELECT + [UP]** lo apre e **SELECT + [DOWN]** torna al Mixer.

- **Return**: livello del delay umido.
- **To Reverb**: quantità del segnale di delay umido inviata al reverb condiviso, `0-100%`.
- **Ticks**: tempo di delay in tick del tracker (`9 ticks` = `1 movimento` per impostazione predefinita).
- **Feedback**: quantità di retroazione incrociata, limitata al `95%`.
- **Filter**: filtro passa-basso nel percorso del segnale ritardato.

La 1ª ripetizione segue l'ingresso stereo. La retroazione successiva si incrocia tra i canali sinistro e destro.

## 11. Schermata Project

La schermata Project offre i comandi **Load**, **Save**, **New**, **Export**, **Manage** e **Scale**, insieme ai metadati di nome del file, titolo e autore.

### Posizione delle esportazioni (Export location)

Esportazioni, stem e bounce vengono scritti in una cartella specifica del progetto all'interno della cartella dei campioni già esistente dell'applicazione:

- **Predefinito**: `<app folder>/samples/Exports/<project name>/`. Finché il progetto non ha un nome, la cartella è `current-project`.
- Nella versione web il valore predefinito è `/user/samples/Exports/<project name>/`.
- Su Android la cartella dei campioni si trova nello spazio di lavoro privato dell'applicazione.

La riga **Folder** della schermata EXPORT mostra la destinazione attiva:

- Tocca **EDIT** per scegliere una cartella personalizzata con il browser delle cartelle. La cartella scelta viene usata esattamente come selezionata (non viene aggiunta alcuna sottocartella di progetto) e viene ricordata nelle impostazioni.
- **EDIT + OPT** (clear) ripristina la posizione predefinita.

Quando un progetto con nome viene salvato con un nuovo nome, la cartella di esportazione predefinita viene rinominata di conseguenza, così i file già esportati restano con il progetto. La rinomina non si applica a una cartella personalizzata, e la cartella non viene rinominata dal salvataggio automatico. Se esiste già una cartella con il nome di destinazione, vengono mantenute entrambe. Se il tracker ha perso traccia della cartella (per esempio dopo un riavvio), salvare un progetto con nome rinomina invece la cartella `current-project` esistente. Gli strumenti continuano a funzionare dopo una rinomina: i percorsi dei campioni caricati che puntano nella cartella rinominata vengono aggiornati automaticamente.

**Load** accetta anche file `.mid`/`.midi`, importati come nuovo progetto: le note vengono raggruppate per canale MIDI (un canale per traccia, fino al numero di tracce), quantizzate a 4 righe per movimento e collocate su un unico strumento AY predefinito - i numeri di programma MIDI non hanno equivalente chiptune, quindi scegli strumenti veri in seguito. Viene usato solo il primo tempo del file (una frequenza di tick globale, nessun cambio di tempo per sezione). La riga **MIDI** della schermata Export fa l'inverso: scrive le note, il volume e il tempo/groove dell'arrangiamento corrente come file MIDI standard (una traccia MIDI per traccia del tracker); gli strumenti [MIDI Out](#midi-out) e gli altri FX per riga oltre al volume e al groove globale non hanno equivalente MIDI e non vengono tradotti.

**Load** accetta anche le canzoni Dirtywave M8 (`.m8s`, firmware 2.x-4.x), importate come nuovo progetto. Passano solo la struttura e le note (righe della canzone, catene con la loro trasposizione, frasi con nota, velocity e numero di strumento, note-off, tempo e nome della canzone); le note mantengono la loro altezza reale (l'M8 nomina le ottave due più in alto: il suo `C-6` è `C-4` qui). Ogni strumento usato da una frase viene importato con il nome dello strumento M8. Gli strumenti M8 **MACROSYN** diventano strumenti Braids (le 47 forme sono i modelli Braids, con gli stessi numeri; timbre, color, tipo di filtro, frequenza di taglio, risonanza e pan vengono riportati, e la curva di taglio è approssimativa). Gli strumenti **SAMPLER** diventano strumenti Sample: il `.m8s` memorizza solo il percorso del WAV sulla scheda dell'M8, quindi il file viene cercato sotto la cartella del `.m8s` e le sue superiori (va bene una copia della struttura della scheda) e nella tua cartella dei campioni corrente (`Samples` o `samples` funzionano entrambe); se non viene trovato, lo strumento mantiene il percorso dell'M8 e resta muto. I campioni importati ricevono +24 semitoni nell'impostazione Pitch, poiché l'M8 riproduce un campione alla sua altezza originale due ottave più in basso rispetto a questa applicazione. **FMSYNTH** diventa uno strumento Genesis FM (approssimazione grossolana), **WAVSYNTH** e **HYPERSYNTH** diventano aChChid. **MIDI OUT** diventa uno strumento MIDI Out (canale, bank, programma e i primi quattro numeri di CC). Gli altri tipi (esterni) diventano uno strumento AY predefinito - scegli suoni veri in seguito. Gli FX e le tabelle vengono ignorati.

Su desktop, la riga **M8S** della schermata Export fa l'inverso, in modo limitato: toccala e scegli un `.m8s` esistente come **modello** (ricordato fino all'uscita, **EDIT + OPT** lo dimentica). La sua canzone, le catene, le frasi, il tempo e il titolo vengono sostituiti e tutto il resto resta com'è, tranne che gli strumenti Braids vengono riscritti come MacroSynth e gli strumenti Sample come Sampler (percorso `/Samples/<file name>`: copia tu stesso il WAV lì sull'M8), purché il modello abbia uno strumento di quel tipo da cui copiare gli altri parametri. Gli FX non vengono esportati, e una canzone che usa la frase `FF` o superiore non può essere esportata (l'M8 ha 255 frasi). Il file esportato non è ancora stato provato su un vero M8. Vedi [M8 songs](../m8s-format.md) per i dettagli.

Su desktop, il key jazz permette di digitare nome del file, titolo e autore direttamente sulla tastiera invece di usare la tastiera virtuale a schermo - vedi [Key jazz](#key-jazz-solo-desktop).

### Scale / Quantize

La schermata Scale controlla il motore di scala globale a 12-TET. La riga **Mode** sceglie tra due modalità:

- **Quantizer** (predefinita): l'inserimento nelle frasi resta cromatico; quando è attivo, le note vengono arrotondate per difetto subito prima di raggiungere il motore sonoro, così cambiare fondamentale o scala riarmonizza una canzone senza modificarne le frasi.
- **Note Lock**: le note digitate nelle frasi vengono agganciate alla nota più vicina della scala al momento dell'inserimento, quindi si possono scrivere solo note della scala. Gli inserimenti nuovi e le diminuzioni si agganciano verso il basso; `EDIT + RIGHT` / `EDIT + UP` portano la nota alla nota successiva della scala sopra. L'inserimento segue sempre la scala del progetto in questa schermata: cambiare fondamentale o scala influisce subito sulle note appena inserite sulle tracce abilitate, mentre le note già memorizzate restano intatte. L'FX `SCL` non è disponibile in questa modalità e qualsiasi `SCL` già scritto viene ignorato. La quantizzazione in riproduzione delle note semplici viene aggirata (l'inserimento è già bloccato); la quantizzazione degli accordi tramite `CRD` continua ad applicarsi.

Con il cursore su **Mode**, la barra di stato mantiene visibile una descrizione fissa della modalità attiva: `Quantizes notes on playback` per **Quantizer**, `Snaps sequencer to scale` per **Note Lock**. Cambiare modalità sostituisce subito il testo; qualsiasi messaggio di azione lo sostituisce finché non scade.

Entrambe le modalità condividono la stessa fondamentale, scala, modifica delle note **Custom** e caselle delle tracce. Ciascuna delle otto caselle delle tracce decide a quali tracce si applica la modalità. Il motore di scala viene aggirato per le tabelle delle altezze che non hanno 12 note e per gli strumenti Sampler la cui modalità **Slice** è EQUAL, AUTO o LAZY (le loro note selezionano gli slice in modo cromatico a partire da C-0); l'ingresso MIDI non fa parte di questa versione.

- **Linear pitch** seleziona la modalità della tabella delle altezze. **Off** è il valore predefinito e l'impostazione validata sull'hardware per un corretto tracciamento delle ottave di AY, Braids e Plaits.
- **Tick rate** imposta il timing del tracker e mostra i BPM corrispondenti (`tick rate x 60 / 24`).
- ChooChooTracker salva i progetti come `.cct`. Questo formato non è compatibile con ChipNomad.

Usa **Save** prima di cambiare i tipi di strumento o caricare un altro progetto.

## 12. Settings

- **Repeat delay / speed** regolano la ripetizione dei pulsanti tenuti premuti.
- **Stick live mode** seleziona `HOLD`, `TOGGLE` o `FREE` per il pulsante Stick live già assegnato.
- **MIDI** apre il sottomenu [MIDI](#14-midi): scelta del dispositivo e associazione dei canali MIDI In agli strumenti.
- **Synths** contiene **AY Quality**, **Sample dithering** e le impostazioni globali **Braids BITS / DRFT / SIGN**.
- **Mixer** contiene **Mix volume** e il **Tilt pivot** del progetto da `250-4000 Hz` (predefinito `1 kHz`) usato da tutti i controlli Tilt del Mixer.
- **Graphics** contiene **Edit color theme**, **Load font**, **Persistent waveform**, **Track visuals**, **Waveform FPS** e il renderer attivo (`GPU` o `Software`). **Waveform FPS** imposta la frequenza di aggiornamento dell'oscilloscopio dello strumento e dei visualizzatori del mixer da `01` a `60`. Track visuals seleziona **Detailed** (la forma d'onda del sintetizzatore e le sue sovrapposizioni) o **Audio waveform** (l'uscita reale sommata di quella traccia) in modo indipendente per ogni traccia; la forma d'onda audio cancella subito qualsiasi sovrapposizione ADSR precedente all'avvio della riproduzione. I font e i temi ChipNomad dovrebbero funzionare.
- **Key mapping** personalizza i controlli.
- **Support report** scrive un piccolo file di testo diagnostico con la versione dell'applicazione, la piattaforma, le impostazioni audio e il carico della CPU, lo stato di riproduzione/overflow e un riepilogo compatto del progetto. Non include dati di campioni né testo di titolo/autore del progetto. Su Android e Web apre il normale flusso di esportazione/download; le build desktop e per console portatili salvano `support-report.txt` nella cartella scrivibile dell'applicazione.
- **Quit ChooChooTracker** esce correttamente.

**Graphics → Boot screen** seleziona **Original** (predefinito) o **Night**. Tocca EDIT
o usa EDIT + direzione per cambiare; la scelta viene salvata per la prossima
schermata del titolo. Night usa la scena ferroviaria originale con toni grigio-azzurri più morbidi,
una Via Lattea procedurale con sottili scintillii di stelle e qualche stella cadente occasionale.
Il paesaggio e il treno originali mantengono i loro cicli di scorrimento.

## 13. Prestazioni e risoluzione dei problemi

### CPU

Il costo in CPU dipende dai motori e dagli effetti attivi. I modelli fisici di Plaits e Clouds Reverb sono più pesanti delle voci AY di base. Controlla le prestazioni sulla console di destinazione, soprattutto con 8 voci Plaits e entrambe le mandate attive. Una lettura della CPU vicina al `100%` può causare crepitii o scadenze audio mancate.

L'indicatore della CPU è una misura smussata del tempo di rendering audio. Non misura i ritardi prima dell'esecuzione del callback audio né nell'uscita audio del dispositivo, quindi i crepitii possono verificarsi anche con una lettura bassa.

Su Android, l'uscita audio ora preferisce AAudio per evitare i crepitii intermittenti osservati con OpenSL ES sul Pixel 7a. Questa uscita con buffer dà priorità a una riproduzione stabile e può aggiungere un po' di latenza di risposta. La tela del tracker mantiene il suo layout 4:3 completo su schermi quadrati e altri rapporti insoliti; lo spazio inutilizzato viene mostrato come bordi anziché ritagliare colonne. Se i crepitii persistono, segnala la canzone, il modello di telefono, la versione di Android e se usi l'altoparlante, le cuffie o il Bluetooth.

Se riscontri schiocchi, crash o rallentamenti, usa **Settings > Support report** e allega il `support-report.txt` generato alla segnalazione del bug. Se un brano specifico provoca il problema, allega anche quel `.cct`.

### Nessun suono

Controlla il numero dello strumento, lo stato di mute o solo della traccia, il LVL della traccia, il Mix volume dell'applicazione e l'inviluppo dello strumento. Per i campioni, controlla che il WAV originale esista ancora al percorso salvato. Su ArkOS, premi **MENU + L3** per attivare o disattivare il silenziamento del sistema operativo. Se questo non aiuta, riavvia l'hardware e ricollega la tua interfaccia audio.

### I controlli sembrano sbagliati

Regola Repeat delay e Repeat speed in Settings. Se una singola pressione si sposta due volte, controlla che solo 1 controllo fisico sia assegnato a quella direzione e segnala la schermata e la scorciatoia esatte.

## 14. MIDI

Desktop e PortMaster. Settings > **MIDI** apre questo sottomenu: **MIDI In** / **MIDI Out** scelgono un dispositivo collegato, scorrendo le porte rilevate con `OFF` a entrambe le estremità; entrambe le direzioni richiedono un dispositivo selezionato. La scelta del dispositivo vale per la sessione: non viene salvata in settings.txt, perché la posizione di una porta nell'elenco può cambiare tra un riavvio e l'altro o quando i dispositivi vengono collegati in un ordine diverso. Il controllo **PLAY** dell'applicazione resta disponibile da questa schermata e avvia la canzone. Su PortMaster, collega un dispositivo MIDI USB prima dell'avvio; deve essere esposto dal sistema operativo della console portatile tramite ALSA.

- **Ascolto dei suoni da una tastiera MIDI**: con un dispositivo MIDI In selezionato, suonare note sulla tastiera collegata fa ascoltare uno strumento su qualsiasi schermata, come la scorciatoia a schermo **EDIT + PLAY** nella schermata [Instrument](#5-instruments). È solo ascolto - non inserisce note nella canzone.
- **Channel mapping**: apre un elenco dei 16 canali MIDI; assegna a ciascuno uno strumento (`OFF` per impostazione predefinita) in modo che le note ricevute su quel canale facciano ascoltare quello strumento indipendentemente da quale sia selezionato nella schermata Instrument - per esempio canale `01` -> strumento `05`. Un canale lasciato su `OFF` ripiega sullo strumento attualmente selezionato, il comportamento originale. Questa associazione viene salvata in settings.txt.
- **MIDI CC mapping**: associa un controller in ingresso a un parametro di strumento, incluso il suo **Volume**, o a controlli globali. Le righe globali mostrano `TRK 1`-`TRK 8` invece di uno strumento e offrono, per traccia, **Mute**, **Solo**, **Volume**, **Reverb send** e **Delay send**; **Song play/stop** non ha destinazione. Una nuova associazione ignora il CC che l'ha appresa/configurata e ha effetto al successivo movimento fisico, quindi configurare associazioni durante la riproduzione non può azzerare una patch. Mute e solo usano `0-63` = off, `64-127` = on.
- **Pilotare un dispositivo MIDI esterno**: assegna a una traccia il tipo di strumento [MIDI Out](#midi-out) e imposta il suo Channel; attivare note su quella traccia invia MIDI reale al dispositivo MIDI Out selezionato invece di produrre suono in ChooChooTracker.

## 15. Crediti e licenze

ChooChooTracker è un fork di ChipNomad e mantiene il suo approccio di licenza MIT. Il codice di Braids, Plaits, Plaits-Alt, Clouds DSP, del DSP di MME derivato da Warps e di stmlib deriva dalle pubblicazioni open source di Mutable Instruments, con le relative note MIT applicabili. Plaits-Alt proviene dal fork Plaits Lab di lylepmills/eurorack; si applicano le sue note di sorgente conservate. Il motore aChChid usa Open303 di Robin Schmidt, copyright 2009, con licenza MIT. Bogie e Sintered sono implementazioni native originali. L'I/O MIDI usa RtMidi di Gary P. Scavone, con la sua licenza di tipo MIT. Consulta i file di licenza inclusi per le attribuzioni esatte.

## Effetti insert di traccia (Track insert effects)

Ognuna delle otto tracce ha due slot in serie, **TF1 → TF2**. Da Instrument,
usa il consueto gesto Shift+Su per aprire MOD, poi di nuovo Shift+Su per **F: Insert
FX**. Shift+Giù torna a MOD. OPT+Sinistra/Destra seleziona la traccia (1-8), senza
cambiare il selettore dello strumento. L'intestazione del modulo apre il selettore esistente;
il bypass mantiene il modulo, i valori configurati e l'automazione. I parametri sono
valori a byte: colonna sinistra 1-4, colonna destra 5-8. Il testo contestuale mostra i nomi completi e
i valori decodificati. I vecchi progetti si caricano con entrambi gli slot su OFF.

La visualizzazione di navigazione mantiene visibile l'intera colonna F-M-I-P in tutte le
schermate dello strumento, comprese Insert FX e l'instrument pool, ed evidenzia
la schermata corrente.

| Modulo | Parametri in ordine |
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

I controlli continui usano 00-FF, compreso uno zero valido e il massimo FF. I controlli
bipolari hanno un neutro esatto a 80. Le modalità di Distortion sono 00 Density, 01 Drive,
02 Spiral, 03 Mojo, 04 Dyno. I rapporti del Compressor 00-07 sono 1,5, 2, 3, 4, 6, 8, 16,
20:1; le sorgenti del rilevatore 00-02 sono ingresso stereo, sinistra, destra. Detector filter 80 è
neutro, sotto è un passa-basso, sopra un passa-alto. Non è previsto alcun sidechain esterno o
tra tracce. Ciascuno slot può contenere qualsiasi modulo; i moduli ripetuti sono indipendenti.
Chorus, Flanger, Phaser e Rotary usano valori predefiniti di modulazione delicati. Rotary Speed
usa l'intero intervallo **00-FF**: **00 = 0,01 Hz** (una rotazione ogni 100 secondi),
**FF = 25 Hz** (l'ex massimo di Chaos). La spaziatura esponenziale dà un controllo più fine
sul movimento lento dell'LFO. I nuovi insert Rotary partono per impostazione predefinita da **8F**, circa
0,8 Hz. Questo sostituisce i quattro valori di velocità discreti; le impostazioni Rotary esistenti
e l'automazione della velocità F11/F21 vanno ritarate sulla nuova scala (i vecchi Slow/Fast/Hyper/
Chaos corrispondono approssimativamente a **8F/CB/E7/FF**). Gli altri parametri non cambiano.
La profondità di bit del Bitcrusher va da 4 a 16 bit e
la riduzione di frequenza da 1x a 32x. Le modalità di Destruction sono Fold, Clip e Crush.
Saturation e distortion includono controlli di livello di uscita o di mix per bilanciarli
rispetto al segnale non elaborato.
Il Parametric EQ ha shelf di boost/cut da 24 dB a 60-500 Hz e 2-14 kHz, più una
campana a Q fisso 0,9 da 250 Hz a 5 kHz. I suoi controlli di guadagno sono neutri a 80; con tutti e
tre neutri è un bypass esatto. L'EQ attivo usa un soft knee trasparente sopra il
fondo scala e raggiunge un massimo di 1,5 per contenere gli aumenti estremi.

Il livello di traccia e il tilt EQ precedono gli insert, quindi entrambi influenzano il segnale che entra
in compressione e distorsione. Tutte le voci/note di accordo della traccia alimentano un'unica
catena di insert. Le mandate di reverb e di delay ricevono entrambe la sua uscita. Gli effetti condivisi, i ritorni,
i controlli master e mute/solo mantengono il comportamento esistente. Gli insert non
elaborano l'audio di dispositivi MIDI esterni.

I comandi degli insert usano **F11-F18** per TF1 e **F21-F28** per TF2. La selezione FX
del roll e il popup Phrase/Table offrono solo i parametri usati dagli insert configurati
della traccia selezionata. Il popup li raggruppa come **TF1: nome dell'effetto** e
**TF2: nome dell'effetto**; gli slot OFF sono nascosti. Gli insert in bypass restano modificabili.
Nel popup FX di Phrase e Table, la descrizione segue lo slot di insert della
traccia selezionata: mostra il modulo, il nome del parametro, gli estremi decodificati dell'intervallo e
il valore di base salvato. I comandi di canzone esistenti vengono preservati se un cambio di modulo li rende
inutilizzati; aprire il loro selettore parte da un controllo disponibile.
Durante la modifica di un valore, il suggerimento in basso mostra il suo valore decodificato e le unità.
Qualsiasi colonna FX di Phrase o Table può indirizzare entrambi gli slot; la griglia Phrase mantiene le sue
tre colonne. Questi valori assoluti di runtime persistono attraverso note, strumenti,
frasi, catene e loop ordinari. Una riproduzione nuova o uno stop brusco li azzera. Modificare
un valore di base nella pagina F cancella solo l'override di quel parametro. MOD agisce sul
valore di base/di automazione effettivo senza salvare i suoi offset. Scegliere un nuovo
modulo ripristina i valori predefiniti e cancella gli override di quello slot. Il bypass mantiene
l'automazione esistente sospendendo l'elaborazione dopo una breve dissolvenza.

**Gli indirizzi si riferiscono a slot di traccia e posizioni di parametro, non a effetti con nome.** Una
Phrase riutilizzata su un'altra traccia usa i moduli di quella traccia. Cambiare i moduli può
cambiare il significato di un comando; i comandi non vengono riscritti. Gli indirizzi OFF/inutilizzati
sono no-op sicuri e restano intatti nei dati di Phrase/Table salvati.

Il selettore MOD dello strumento aggiunge una categoria Insert FX. Le sorgenti di ciascuno strumento
controllano gli insert della traccia che lo suona, compreso il segnale combinato dell'accordo.
Lo stesso strumento su due tracce influenza catene indipendenti. Si applicano il timing esistente delle sorgenti,
le quantità con segno, le regole di combinazione delle sorgenti, le modalità degli stick e la registrazione del
movimento. I controlli discreti vengono quantizzati e limitati a scelte valide. Non c'è
nessun nuovo banco di modulazione di proprietà della traccia.

Il makeup del Compressor parte da 0 dB. OTT parte con circa il 25% di depth, il 25%
di upward, il 50% di downward, 0 dB in ingresso e -3 dB in uscita. Le sue soglie di banda, i guadagni e
le frequenze di crossover restano ai valori nativi di Rui-727; Depth è il suo controllo wet/dry.
TAPESCAM mantiene i valori ausiliari nativi: nastro nuovo/ad alta velocità, rumore e
compressione disattivati, allargamento attivo. Tieni d'occhio gli indicatori di clipping esistenti quando aggiungi
drive/gain. Le transizioni degli slot usano dissolvenze limitate di 5 ms attraverso l'audio secco.

I salvataggi memorizzano gli ID dei moduli, il bypass e i byte configurati nelle
sezioni opzionali versionate `Track inserts: 1,8,2` e `Master inserts: 1,2`. Gli override di runtime
e le storie DSP non vengono salvati.
Fai dei backup prima di aprire salvataggi con insert in build più vecchie. L'attribuzione del DSP,
le revisioni dei sorgenti e gli adattamenti sono in `chipnomad_lib/external/insert_fx/SOURCES.txt`;
le note MIT complete sono incluse in `licenses/INSERT_FX.txt`.

Su un R36H a 48 kHz, una coppia di insert misurata su una traccia ha richiesto circa il 4-5%
del tempo reale per Compressor, il 6% per Distortion, il 18% per Doubler, il 16% per TAPESCAM
e il 24% per OTT, prima del costo dei sintetizzatori e degli effetti condivisi. Sedici istanze
attive di Doubler, TAPESCAM o OTT hanno superato il tempo reale. La modulazione a frequenza audio aggiunge
ulteriore costo. Inizia con pochi insert, tieni d'occhio il sovraccarico audio e metti in bypass
gli slot inutilizzati; le sedici posizioni disponibili non sono un budget di CPU garantito.

La pagina Insert mantiene visibile il suggerimento del campo selezionato dopo il rilascio del pulsante;
gli avvisi temporanei possono comunque prenderne il posto. Il selettore dei moduli mostra per ogni
modulo il tipo di effetto e il progetto di origine accanto al nome.

### Strumenti nativi OPLL e VRC7 (sviluppo)

Il selettore Instrument Type ha un gruppo FM con **OPLL / MSX** (YM2413)
e **VRC7** (DS1001). Ciascuno offre ora **73 programmi** in elenchi Bank / Preset
scorrevoli: i 15 timbri originali, 40 timbri distinti aggiuntivi dalle tabelle
YM2413/VRC7/YMF281B di emu2413 e 18 programmi a due operatori scritti per ChooChoo.
I duplicati esatti dei byte di timbro vengono omessi all'interno di ciascun motore. Le palette
aggiuntive usano lo slot di timbro programmabile del chip; non ampliano la
ROM fisica. Il programma zero identifica un timbro personalizzato. EDIT + sinistra/destra seleziona il
preset precedente o successivo all'interno del filtro di banco. Fine ct regola l'accordatura
da -100 a +100 cent.

Nell'elenco dei programmi, EDIT + PLAY ascolta il suono evidenziato; rilascia i
pulsanti per fermare. EDIT conferma e OPT annulla. Esplorare/ascoltare non cambia
lo strumento, la tabella né la canzone. Il normale gesto di ascolto della pagina Instrument
continua a funzionare dopo la selezione. Gli inviluppi nativi del chip forniscono attack/release;
l'Amp env opzionale aggiunge un ADSR software (vedi Controlli dei chip nativi più sotto).
Non c'è un editor di operatori completo.

I nuovi strumenti memorizzano gli otto byte di timbro nativi, banco/nome, programma e accordatura fine nel
file di canzone/strumento. I file che contengono questi tipi usano il formato 7.0 e richiedono
questa build o una successiva. I formati precedenti restano leggibili; le canzoni senza questi tipi
continuano a essere salvate come 5.0. I programmi ritmici sono rimandati. La libreria ampliata è
validata sull'host; l'ascolto umano e un nuovo controllo su console portatile restano in sospeso.

### AdLib / OPL2 e OPL3 (sviluppo)

L'FM comprende anche AdLib / OPL2 (YM3812) e OPL3 (YMF262). Bank filtra la
lista di fabbrica; Preset apre gruppi per categoria e una vista All. EDIT + PLAY ascolta
prima della selezione, EDIT seleziona, OPT annulla, e EDIT + sinistra/destra su Preset
carica la voce corrispondente precedente/successiva. Fine ct regola l'accordatura locale. Mode mostra
2 operatori, 4 operatori o Dual voice. OPL2 nasconde le patch OPL3 incompatibili;
OPL3 può suonare la collezione condivisa a due operatori.

I titoli dei popup di banco e preset identificano il motore corrente. Cambiare motore o
slot di strumento riporta il filtro di banco su **All banks**, anche passando
tra i motori compatibili AdLib e OPL3. Tornare da un popup all'interno dello stesso
slot mantiene il filtro scelto. Annullare l'importazione di un banco DX7 ripristina il
filtro precedente.

La conferma del preset pubblica anche le modifiche fatte al rilascio del pulsante al successivo
tick di interfaccia/audio. Le canzoni in riproduzione non hanno più bisogno di un'altra pressione di pulsante né di un
riavvio del transport per ricevere quella patch. L'uscita segue sempre il buffer audio configurato
e il comportamento di inviluppo/riattivazione del motore.

La collezione di fabbrica contiene 697 voci sorgente da The Fat Man 2-op,
The Fat Man 4-op e DMXOPL3, con 589 identità di patch uniche normalizzate.
Gli alias mantengono i nomi sorgente e l'attribuzione. Vengono mantenuti i livelli degli operatori nativi della sorgente, i
flag di profondità dell'LFO, gli offset di nota, le altezze delle percussioni fisse e l'accordatura delle voci doppie.
Il volume del tracker usa un'ampiezza software; non riproduce
le curve di volume né le politiche di offset di velocity dei lettori MIDI originali. Le
stime di durata del release della sorgente non troncano le note sostenute. Alcuni effetti hanno
attacchi lenti: tieni premuto Seashore invece di aspettarti che un clic breve lo riveli.

Le risorse di fabbrica si trovano in `instruments/chips` accanto alla libreria di strumenti
esistente, con le note sotto `licenses/chip-banks`. La selezione di un preset lascia invariati la
tabella dello slot e gli insert di proprietà della traccia. I file salvati di nuovo usano 7.0 e
incorporano il timbro completo. L'audio di fabbrica è testato automaticamente sull'host e su ARM64; l'ascolto
umano resta in sospeso.

### Strumenti nativi Sega PSG e Game Boy (sviluppo)

CHIP ora include Sega PSG, GB Pulse e GB Noise. Sega usa il clock master NTSC
e la sua retroazione di rumore a 16 bit, compreso il rumore derivato dal canale di tono.
Il suo **Bass range: Extended** predefinito abbassa il clock virtuale quando serve per
suonare sotto il limite del divisore del chip, di circa 109 Hz. Questo mantiene A-2, G-2
e F-2 distinti. Scegli **Chip** per l'intervallo originale, in cui le note più gravi
convergono sul limite del divisore. Il rumore a frequenza fissa mantiene il clock originale;
il rumore derivato dal tono segue l'altezza estesa. I vecchi strumenti Sega si caricano con
Extended attivo; l'impostazione viene salvata con lo strumento.
Gli strumenti GB usano i registri di impulso/rumore del DMG; non è esposto alcun canale d'onda del Game Boy.
Le loro liste di preset composti contengono ora rispettivamente **24, 24 e 28** preset.
Le aggiunte coprono il rumore Sega collegato/periodico, gli sweep d'altezza GB,
percussioni brevi, rumore metallico, drone e salite. Sono programmi originali,
non suoni estratti da giochi.
La pagina offre controlli nativi di modalità/duty/larghezza, inviluppo/sweep/rumore
dove applicabile, oltre a un ADSR d'ampiezza software. Vengono salvati in file nativi di versione 6.
I controlli automatici di prestazioni/audio su ARM64 passano entro i limiti di
carico indicati; l'ascolto umano resta in sospeso.

### DX7 FM (sviluppo)

Seleziona **Type → FM → DX7 FM**; DX7 è la settima voce, dopo Arcade / YM2151.
DX7 FM usa un nucleo MSFA a sei operatori. La pagina FM esistente offre
Bank, Preset e Fine ct accanto alle impostazioni comuni dello strumento. L'esplorazione dei preset
usa elenchi per banco/categoria. Tieni premuto EDIT+PLAY per sentire il suono evidenziato; il rilascio
interrompe l'ascolto. EDIT da solo conferma al rilascio; OPT annulla. Caricare un suono
copia la sua patch completa senza cambiare le tabelle del tracker né gli FX insert di traccia.

Il catalogo di fabbrica contiene attualmente **67 patch di parametri DX7 distinte**:
31 suoni musicali originali di OpenDX7, quattro suoni unici YSE CC0 (i suoi 32 slot di banco
ripetono quei quattro con nomi diversi) e 32 patch scritte per ChooChoo.
Qui «originale» significa programmi di parametri creati per questo progetto; non
significa contenuto di fabbrica originale Yamaha. YSE viene fornito come quattro suoni, non come 32
nomi artificialmente diversi.
Le categorie descrivono l'intento di sound design rivisto. I nomi ambigui restano
Unsorted. I controlli numerici di suonabilità sono stati superati; l'accettazione all'ascolto è
in sospeso. L'obiettivo separato di 1.000 suoni liberi da vincoli di ridistribuzione non è raggiunto.

Per una libreria personale persistente, metti file `.syx` in
**`instruments/banks/dx7/`** accanto alla libreria di strumenti esistente. Le sottocartelle
sono supportate. Apri **DX7 → Bank**: ogni file compare come un banco con nome, pronto per
l'esplorazione in Preset. Riapri Bank dopo aver aggiunto o rimosso file; non servono conversione né
il passaggio Load Instrument. Un banco DX7/TX7 originale standard contiene
**32 voci**. Un file con quattro messaggi di banco compare come quattro banchi numerati
(128 voci in totale). Sono accettati anche i file a voce singola. L'applicazione nativa
legge questa cartella; la versione browser legge il suo file system virtuale, non cartelle
arbitrarie del computer.

I file difettosi o non supportati vengono saltati con un conteggio a schermo; i banchi validi restano
disponibili. La scansione è limitata a 2.048 file, 1 MiB per file, 64 MiB in totale,
60.000 voci e otto livelli di cartelle annidate. I collegamenti simbolici vengono ignorati. La
patch selezionata appartiene allo strumento: rimuovere il suo banco sorgente non può
cambiare la canzone salvata. L'intera libreria di esplorazione resta in questa cartella e
non viene copiata in blocco in ogni progetto.

LOAD INSTRUMENT accetta anche dump `.syx` originali DX7/TX7 a voce singola e banchi da 32 voci,
comprese sequenze limitate di quei messaggi supportati. Controlla
inquadratura, numero di byte, dati a sette bit, checksum, intervalli dei parametri e dimensione del file
prima di aprire il banco importato nello stesso browser FM. Selezionare una patch
conferma; annullare mantiene lo strumento corrente della canzone. Questa scorciatoia di importazione diretta
mantiene la sua lista di esplorazione per la sessione; usa la cartella dei banchi qui sopra per un'esplorazione
persistente. Salva la canzone o un `.cni` per conservare le patch selezionate/modificate. I dump senza intestazione,
con checksum errati, le performance/estensioni DX7II e le altre famiglie Yamaha vengono
rifiutati. L'importazione non invia mai messaggi MIDI ad apparecchiature esterne.

CNI e la versione 6 dei progetti memorizzano tutti i 155 byte di voce originali, il nome completo visualizzato,
la velocity di attacco nativa, l'accordatura fine e l'identità della sorgente. Le canzoni non hanno bisogno di
alcun banco esterno per riaprirsi. Le versioni più vecchie di ChooChoo non possono leggere questi file
di versione 7; i progetti che non li contengono continuano a essere salvati come versione 5. La velocity nativa è per impostazione predefinita
100; il volume del tracker è un guadagno post-sintesi e non riattiva l'inviluppo.
Gli inviluppi degli operatori, la modalità a frequenza fissa, il keyboard scaling, l'inviluppo d'altezza e
l'LFO restano comportamento nativo della patch. L'ADSR d'ampiezza FM comune opzionale modella
la loro uscita combinata senza sostituire gli inviluppi degli operatori.

Ogni parte di traccia ha un LFO e quattro slot di voci di accordo di proprietà distinta, in linea con
la politica esistente di accordi/sostituzione del tracker. Le tracce non condividono lo stato delle note.
Il note-off rilascia gli inviluppi nativi; cut/panic li cancella. Il nucleo MSFA scalare
usa quanti da 64 campioni a 44,1 kHz interni fissi, con un FIR in streaming verso
la frequenza di uscita. Le tabelle di consultazione vengono inizializzate una sola volta fuori dal rendering audio, in modo che
i renderer offline e live simultanei non possano cambiarsi a vicenda la frequenza. Gli eventi
hanno effetto al successivo quanto interno (fino a 1,45 ms), seguito dal ritardo di gruppo del FIR
di circa 0,25 ms. I campioni in buffer vengono mantenuti tra un callback e l'altro.
Questa è la linea MSFA Modern, non una dichiarazione di identità bit per bit con l'hardware DX7 né di un'
emulazione di Dexed Mark I. DX7 è limitato a **16 note attive in tutta la canzone**,
comprese le code di release, mantenendo quattro slot di accordo propri per traccia.
Quando si supera il budget, scarta prima le code di release, poi le note sostenute più silenziose,
poi gli attacchi freschi. A parità di attacchi, le note fondamentali vengono mantenute tra le tracce prima delle
estensioni dell'accordo, con spareggio stabile per slot/traccia. Questa politica si applica su tutte le
piattaforme, così la stessa canzone ha la stessa allocazione di note limitata. Non
cambia la polifonia degli altri strumenti. L'ascolto dei preset è disattivato durante la riproduzione.
Il limite è stato scelto da misure su R36H e validato in riproduzione mista.

#### Editor di operatori FM

Gli strumenti Genesis FM, Arcade FM, OPL2, OPL3, OPLL e VRC7 hanno un pulsante **EDIT**
a destra di Fine ct. Apre una tabella dei parametri della patch:

- Genesis FM / Arcade FM: algorithm, feedback, pan, AMS, PMS, maschera degli operatori e
  impostazioni dell'LFO, poi MUL, DT, TL, KS, AR, D1R, D2R, RR e D1L per i quattro
  operatori, più SSG (Genesis) o DT2 e AM (Arcade).
- OPL2 / OPL3: feedback, connection e pan per voce, deep vibrato e tremolo,
  poi MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS e KSR per
  operatore. Su OPL3 qui si può cambiare la modalità (2op, 4op, dual).
- OPLL / VRC7: i registri del timbro personalizzato (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Modificare un preset ROM lo trasforma in
  «Custom».

Sinistra/Destra e i consueti tasti di modifica cambiano i valori; lo strumento continua a suonare
come al solito, così senti le modifiche. La barra di stato descrive il parametro sotto
il cursore. OPT o Left+Shift torna alla schermata dello strumento. DX7 e SID
non hanno un editor di operatori.

Genesis FM (YM2612) e Arcade FM (YM2151) usano ora lo stesso browser FM Bank/Preset,
lo stesso ascolto EDIT+PLAY, la stessa conferma/annullamento e gli stessi controlli di accordatura fine. Genesis ha ora
**73 preset**: i 24 originali e 49 programmi melodici supportati dal banco CC0
16-Bit FM Music Station di NeoSoundFonts. Arcade ha **81 preset**: i
24 originali e 57 programmi supportati e sonori dalla collezione GPLv3 di YMulator-Synth.
La coppia originale di ChooChoo condivide le sue ricette di base; le nuove
collezioni sorgente forniscono palette separate. Sono collezioni di sound design con nome,
non ricostruzioni dichiarate di colonne sonore di giochi specifici. Gli offset di nota non supportati,
i tasti di percussione fissi, le impostazioni di rumore arcade e i programmi sorgente silenziosi
sono registrati come esclusioni in `expansion-manifest.json`. Le loro impostazioni complete di patch a quattro operatori, inviluppo,
LFO, stereo e accordatura viaggiano dentro strumenti e canzoni di versione 6.
L'inviluppo nativo del chip controlla il release; il guadagno e l'altezza ordinari del tracker
non lo riattivano. L'uscita DAC di Genesis usa un blocco della componente continua a 20 Hz. La modalità rumore e
la modalità a frequenza speciale del canale 3 sono fuori da questa implementazione dello strumento.

Per un banco esterno locale, usa `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`. Accetta TFI rigoroso da
42 byte, testo VOPM OPM, il formato WOPLX verificato e SysEx DX7 originale.
Carica i file `.cni` risultanti tramite Load Instrument. Le importazioni utente vengono tenute
separate dalla libreria di fabbrica distribuibile. I file OPM con rumore attivo,
dati di frequenza del rumore diversi da zero o panning parziale vengono rifiutati con una spiegazione;
i valori di pan VOPM 0/64/127 diventano sinistra/entrambi/destra. Il WOPL binario non è supportato.
Il catalogo FM condiviso contiene **1.064 voci**: 697 OPL, 67 DX7, 146
OPLL/VRC7, 73 Genesis e 81 Arcade. Altri 76 file espongono i preset Sega/Game Boy
tramite il normale browser di file, per **1.140 preset nativi inclusi**.
Revisioni dei sorgenti, hash, note complete e dati sorgente originali accompagnano le
nuove collezioni in `licenses/chip-banks/expansion`.

### Collezioni di fabbrica portatili e preset USER

Gli strumenti dei chip nativi usano **Bank:** per scegliere ALL, una collezione inclusa o
USER. **Preset:** sceglie un suono all'interno di quella selezione. Tieni premuto EDIT e premi
Sinistra o Destra su una delle due righe per scorrere; un breve tocco di EDIT apre il suo selettore.

Le collezioni incluse sono pacchetti ZIP in `instruments/FACTORY/`. I file USER vanno
in `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` o `gb-noise`.
Esplora le cartelle direttamente o mettici uno ZIP di preset compatibili. Gli ZIP restano
intatti su disco. Tutte queste cartelle accettano file `.cni` compatibili; CNI è
il formato di strumento portatile di ChooChoo/ChipNomad, non uno standard di un produttore di chip.
Il browser USER legge anche i seguenti formati sorgente, anche all'interno degli ZIP:

| Cartella USER | Formati aggiuntivi |
| --- | --- |
| `dx7` | Yamaha DX7/TX7 `.syx`, voci singole e banchi da 32 voci |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | banchi VOPM `.opm`, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | patch personalizzate Furnace `.fui` e selezioni ROM fisse |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

Un banco si apre in voci compatibili. I file o le funzioni non supportati segnalano
un motivo; le voci incompatibili in un banco misto vengono escluse mentre i loro numeri
sorgente restano stabili. Il caricamento è transazionale: un preset rifiutato lascia
lo strumento corrente invariato. I preset sorgente sostituiscono la tabella del tracker dello strumento
con una vuota, tranne il SysEx DX7, che conserva la sua tabella esistente.
CNI ripristina la sua tabella salvata. Le patch caricate appartengono alla canzone e continuano
a funzionare dopo aver spostato o eliminato il pacchetto sorgente.

I banchi WOPL possono contenere slot inutilizzati senza un flag esplicito di vuoto. Il browser
omette gli slot senza nome i cui operatori attivi sono completamente attenuati e non hanno
attack. Le voci suonabili senza nome restano disponibili come `Program N [Bbank]` o
`Drum N [Bbank]`, usando i numeri di programma e di banco della sorgente, contati da zero.
Un singolo strumento OPLI senza nome usa il proprio nome di file. Le patch silenziose con nome
vengono mantenute.

Il supporto a Furnace copre le versioni legacy a strumento singolo 29-112 e le versioni
`FINS` basate su funzionalità 127-251. Il supporto a DefleMask copre gli strumenti della versione 11 di OPN, OPM, Sega PSG
e Game Boy. Sono importazioni di strumenti, non lettori di tracker
completi: macro FM/di operatori animate, impostazioni LFO FM della canzone sorgente, campioni,
sequenze di comandi hardware del Game Boy, macro relative di pulse/filter SID e dipendenze
ring/sync del SID vengono attualmente rifiutate. La selezione ROM costante di OPLL è
supportata; VRC7 usa la propria ROM per quella selezione. I `.swi` di SID-Wizard e i file di
musica SID (`.sid`) non sono importazioni di strumenti in questa build.

Le sequenze PSG/GB/SID supportate vengono memorizzate nella patch e riprodotte a 60 Hz per
Furnace/DefleMask e a 50 Hz per GoatTracker. Quei file non portano la configurazione di riproduzione completa
di una canzone: usa il tracker originale quando servono il suo tick rate specifico della canzone, l'instradamento dei canali,
il comportamento di hard-restart o le impostazioni di compatibilità. Per Sega PSG, una macro di duty seleziona la voce di rumore; senza una, l'importazione
usa una voce di tono. I file Game Boy vanno collocati nella cartella pulse o
noise corretta. Le sequenze importate restano per ora dati di riproduzione incorporati; la pagina
dello strumento modifica i parametri di base, non le tabelle del tracker sorgente.

Lo scorrimento dei preset USER continua attraverso file, cartelle, banchi e pacchetti ZIP in
entrambe le direzioni, con ritorno all'inizio alle estremità. Riaprire il browser USER segue la
selezione corrente. ALL include i suoni USER sotto Unsorted per i motori
con categorie; USER resta una scelta distinta di primo livello. Sega PSG e Game Boy usano
elenchi ALL/Factory piatti e un browser USER gerarchico, senza un pannello
Unsorted. I vecchi file DX7 in `instruments/banks/dx7/` restano accessibili in USER.

Un motore mostra solo le sue collezioni incluse compatibili. Factory Presets
combina i set di timbri OPLL/VRC7 inclusi per il motore selezionato; DX7 combina
gli originali ChooChoo e YSE. Le collezioni con nome scaricate restano separate.
Fat Man 2-op è elencato sotto OPL2 e Fat Man 4-op sotto OPL3. I file CNI OPL2
compatibili possono comunque essere usati nelle cartelle USER di OPL3.

L'uscita dei motori nativi usa una compensazione di guadagno fissa misurata anziché una
normalizzazione per preset. Le canzoni esistenti con strumenti nativi possono quindi suonare
a un livello diverso; controlla il bilanciamento dei tuoi mix salvati. Il metodo di calibrazione
e i limiti sono documentati in `scripts/README_MEASURE.md`.

`native-chip-audition.cct` offre un breve ascolto sequenziale dei tredici
banchi di fabbrica originali. Ogni sezione usa uno strumento proprio diverso, quindi funziona anche
con la cartella dei preset rimossa. I WAV di banco più lunghi, ignorati da Git, e i livelli misurati sono
elencati in `docs/chip-preset-auditions.tsv`; l'ascolto soggettivo resta in sospeso.

### Controlli dei chip nativi (Native chip controls)

Le anteprime delle forme d'onda degli strumenti nativi si aggiornano quando i loro controlli vengono ridisegnati.
Gli strumenti FM possono sovrapporre la loro curva Amp env opzionale; SID mantiene la sua anteprima della forma d'onda
senza quella sovrapposizione propria dell'FM, anche durante l'esplorazione di banchi e preset.

Tutti e sette i motori FM offrono **Bright**, **Feedback** e **Amp env**. Bright
va da -63 a +63, e zero preserva la patch; cambia i livelli degli operatori
modulanti mantenendo quelli dei carrier. Il suo effetto udibile dipende dall'
algoritmo. Feedback è per impostazione predefinita **Preset**, oppure sostituisce la retroazione nativa
con 0-7. Questi controlli non riscrivono i byte degli operatori nativi salvati.

Amp env è per impostazione predefinita **Bypass**. Seleziona **ADSR** per aggiungere attack, decay, sustain,
release e forma attorno al suono nativo. A/D/R usano l'intervallo quadratico comune di 0-5 secondi;
il sustain va dal silenzio al livello pieno. Gli inviluppi nativi continuano a
funzionare, quindi questo inviluppo non può prolungare un suono oltre il suo release nativo. L'esplorazione
dei preset mantiene i controlli di ampiezza FM e di timbro dello slot. Tutte le voci FM usano anche
una transizione di 3 ms all'attacco/riattivazione e un livellamento del guadagno del tracker di 1 ms, anche
quando Amp env è in bypass. Hard cut/panic resta immediato.

I seguenti FX di frase compaiono anche come destinazioni supportate di Modulation e di registrazione del movimento.
I valori nella colonna FX sono esadecimali. Le impostazioni si applicano alla riproduzione
senza cambiare lo strumento salvato. I controlli nativi di sweep e inviluppo del GB
si agganciano alla successiva attivazione di nota; duty/larghezza e frequenza del rumore possono cambiare in tempo reale.

| Motori | FX | Controllo |
|---|---|---|
| Native FM | `OL1`-`OL6` | Livelli di uscita assoluti degli operatori: modulatore OPLL/VRC7 e OPL `00-3F`; carrier OPLL/VRC7 `00-0F`; Genesis/Arcade `00-7F`; DX7 `00-63` (0-99). Più alto significa più uscita. Compaiono solo gli operatori supportati. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Attack, decay, release e attenuazione del sustain dell'operatore 1, `00-0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | Moltiplicatore di frequenza dell'operatore 1. |
| Genesis/Arcade | `LFR` | Frequenza dell'LFO nativo: Genesis `00-07`, Arcade `00-FF`. |
| Arcade | `LAD`, `LPD 00-7F` | Profondità separate dell'LFO per ampiezza e altezza. |
| Genesis/Arcade | `LAS`, `LEN` | Sensibilità di ampiezza nativa `00-03` e attivazione dell'LFO `00-01`. |
| Genesis/Arcade | `LPS 00-07` | Sensibilità d'altezza nativa. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | Attack, decay, sustain e release nativi. I valori di tempo aumentano verso `0F`; il sustain aumenta verso il livello pieno. |
| SID | `SPR 01-10` | Frequenza del partner silenzioso da 1× a 16×; influisce sulla modulazione ad anello e sull'hard sync. |
| Tutti gli FM nativi | `FBK 00-07` | Retroazione nativa assoluta, inizializzata dallo strumento. |
| Tutti gli FM nativi con Amp env attivo; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack, decay, sustain, release, forma. Non attivano un ampli FM in bypass. |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | Tono / rumore bianco / rumore periodico; tre frequenze di rumore fisse o frequenza derivata dal tono. |
| GB Pulse | `CMD 00-03` | Duty d'impulso nativo. |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | Larghezza del rumore, divisore di clock e shift. |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | Periodo dello sweep, shift, direzione discendente. |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | Livello iniziale dell'inviluppo nativo, periodo, direzione ascendente. |

La selezione degli FX nativi usa lo strumento nella colonna `I` della riga di frase, oppure
lo strumento attivo trovato guardando indietro quando `I` è vuota. Il popup FX
mostra una breve descrizione del controllo, il suo valore corrente nel preset e l'intervallo di comando valido
in un blocco compatto. Selezionare un diverso
effetto nativo parte da quel valore; riaprire lo stesso effetto mantiene il suo
valore modificato. Una selezione su più righe risolve separatamente lo strumento di ogni riga.
Le tabelle usano il contesto del loro strumento. Le modifiche ai valori nativi si fermano ai loro
estremi legali, compresi duty, rumore, sweep, ADSR, feedback e livelli degli operatori.
I valori sono esadecimali: per esempio, il massimo `63` del DX7 significa 99 in decimale.

Questi comandi FM usano valori nativi assoluti. I comandi degli operatori puntano sempre all'
operatore 1; `OMU 03` seleziona il suo moltiplicatore 3. I controlli specifici del tracker per DX7
sono i livelli degli operatori e il feedback. Preset/Range mantiene il suo colore informativo;
titoli e descrizioni seguono gli stessi colori di FM Feedback.

La modulazione dal vivo mantiene il suo insieme completo di destinazioni native a operatori fissi.
La registrazione del movimento scrive solo i controlli rappresentati nell'elenco compatto del tracker;
i parametri degli operatori vengono registrati solo per l'operatore 1. Le altre destinazioni di modulazione
continuano a funzionare dal vivo ma non generano comandi di frase.

`SCP` e `SCT` mantengono mappature scalate a byte verso i registri SID più larghi di 8 bit.
`SMR 01-C8`, `SWV 01-08` e `SPR 01-10` corrispondono ora alla numerazione nativa dei preset.
I valori dei preset mostrati sono impostazioni di base, indipendenti dall'inviluppo
o dall'LFO in riproduzione. `SLE` agisce sui livelli degli operatori e sui parametri FM diretti senza
riattivare la nota né cambiare la patch salvata.

Tutti gli FX nativi hanno titoli, intervalli e comportamento descrittivi nel selettore FX
di frase, più suggerimenti sui valori. `CMD` descrive tono/rumore, duty o larghezza del rumore
a seconda dello strumento selezionato. Questi controlli integrano i gruppi condivisi Track,
Envelope e Modulation; il solo gruppo del motore non è l'insieme completo
degli effetti di frase disponibili.

I filtri LP/BP/HP condivisi esistenti su Braids, Plaits, PCM e altri motori supportati
sono elaborazione software dopo la sintesi. aChChid usa invece il suo percorso di filtro
303 nativo. Questo giro non aggiunge alcun filtro ad AY né all'FM nativo; la luminosità cambia
la sintesi FM stessa. Gli insert di traccia restano disponibili per un'elaborazione aggiuntiva.

Gli strumenti nativi vengono salvati in CNI versione 7 (8 quando sono presenti comandi assoluti),
e le canzoni native in CCT versione 9. Gli strumenti che contengono sequenze importate
usano CNI versione 9 e le canzoni che le contengono usano CCT versione 10. Le build più vecchie
non possono aprire quei file più recenti con sequenze. I byte dei preset FM salvati e
le impostazioni di timbro della pagina dello strumento restano invariati.

### Indicazioni sul carico di lavoro su console portatile per i chip nativi

Ci sono otto tracce di canzone e **due slot di effetti insert per traccia**. I sedici
slot disponibili non garantiscono abbastanza CPU per eseguire sedici effetti.
Il limite di sedici note attive del DX7 è un budget di sintesi separato per tutta la canzone;
accordi e code di release vi contribuiscono.

Nelle misure su R36H, quattro tracce WAV in loop, Sega PSG, GB Pulse, DX7 e
OPL3 con mandate condivise e quattro insert (due Compressor, Doubler, TAPESCAM)
sono andati meglio degli arrangiamenti FM densi a otto tracce. La versione a nota singola
non ha avuto scadenze di rendering mancate nella sua misura di trenta secondi a 48 kHz/512 frame;
aggiungere un accordo DX7 a quattro note ha prodotto un picco di timing. Le canzoni FM dense
con molti insert costosi hanno superato il budget di CPU. Mantieni un po' di margine,
tieni d'occhio l'indicatore di sovraccarico esistente e aggiungi effetti dove aiutano la canzone.
L'applicazione mantiene ogni traccia e ogni slot; non c'è un nuovo limite rigido al numero di insert.
Vedi `chip-instruments-report.md` per le misure complete e lo stato dei test prolungati.

Il corrispondente test audio fisico di 70 secondi di quell'arrangiamento di accordi bilanciato
è passato senza scadenze di rendering mancate né underrun ALSA registrati all'impostazione
esistente di 48 kHz / 4906 frame. Il suo callback peggiore è stato di 81,819 ms contro una
scadenza di 102,208 ms. I test hanno usato un guadagno master separato di 0,4 per avere margine.
Mantieni il percorso diretto alla scheda del launcher normale, `AUDIODEV=plughw:0,0`: il
mixer condiviso predefinito del sistema ha prodotto underrun nelle sonde diagnostiche. Nessuna
impostazione audio dell'utente è stata modificata. Il test di stress di dieci minuti con buffer più piccolo ha comunque registrato
28 picchi di timing; i risultati completi sono nel report.

### Strumenti SID e FX di frase

Scegli **CHIP → SID**. Bank offre **ChooChoo SID Originals** (32 programmi
composti) e **SIDkit Effects** (24 effetti con licenza MIT). Entrambi usano lo stesso
browser di preset e salvano il loro programma completo selezionato nello strumento
e nella canzone. Non serve alcun banco esterno per condividere la canzone. Sono programmi
di parametri SID nativi con inviluppi e ricette di movimento, non audio campionato né
canzoni C64 complete. I dodici candidati di ricerca GoatTracker, a parte, non
vengono distribuiti; le loro tabelle wave/pulse/filter richiedono un importatore/player dedicato.

La pagina dello strumento espone forma d'onda/larghezza d'impulso, modalità del filtro/frequenza di taglio/risonanza
e controlli ADSR nativi. SID usa l'oscillatore digitale, l'inviluppo e il filtro
per ciclo in stile 6581 di floooh/chips, bloccati a una versione precisa. Non ci sono modelli di chip alternativi,
selettori di revisione né profili di carattere aggiunti. È un'approssimazione generica,
non un modello analogico calibrato di R2/R3/R4/8580.

Ogni nota ha il proprio filtro. Ring/sync usano un oscillatore partner silenzioso, quindi
la condivisione del filtro e le interazioni a tre voci differiscono da un SID fisico.
La console portatile ha un budget per tutta la canzone di quattro note SID ordinarie.
Ring o sync costa due unità di budget per nota, il che permette due di queste note, o una più due
note ordinarie. Accordi e code di release contano. Le note rilasciate vengono ritirate
per prime; le nuove note hanno priorità sulle note tenute più vecchie. Gli insert costosi e
gli altri sintetizzatori condividono comunque il budget di CPU audio.

| FX | Intervallo | Azione |
| --- | --- | --- |
| `SCP` | `00-FF` | Base della larghezza d'impulso, scalata alla larghezza nativa a 12 bit; il movimento di pulse della ricetta resta attivo. |
| `SCT` | `00-FF` | Base della frequenza di taglio, scalata al taglio nativo a 11 bit; il movimento del filtro della ricetta resta attivo. |
| `SRN` | `00-0F` | Risonanza nativa. |
| `SWV` | `00-07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00-07` | Bit della modalità del filtro: 1 passa-basso, 2 passa-banda, 4 passa-alto; 0 bypass. |
| `SMR` | `00-FF` | Clock della macro della ricetta, scalato da 1 a 200 Hz. |
| `SRG` | `00-01` | Modulazione ad anello triangolare con partner silenzioso. |
| `SSY` | `00-01` | Sincronizzazione dell'oscillatore con partner silenzioso. |

Larghezza d'impulso e frequenza di taglio supportano `SLE`. Le forme d'onda discrete, gli interruttori, la risonanza
e la retroazione FM mantengono i loro utili passi nativi; valori di byte più ampi non
creerebbero più stati hardware. `FBK` seleziona direttamente il feedback `00-07`.
I livelli degli operatori usano gli intervalli specifici di ciascun motore elencati sopra. Le regolazioni di luminosità e
di master mantengono l'intero intervallo di un byte.

### Collezioni di fabbrica portatili e preset USER

Gli strumenti dei chip nativi usano **Bank:** per scegliere ALL, una collezione inclusa o
USER. **Preset:** sceglie un suono all'interno di quella selezione. Tieni premuto EDIT e premi
Sinistra o Destra su una delle due righe per scorrere; un breve tocco di EDIT apre il suo selettore.

Le collezioni incluse sono pacchetti ZIP in `instruments/FACTORY/`. I file USER vanno
in `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` o `gb-noise`.
Esplora direttamente le tue cartelle, oppure mettici uno ZIP di cartelle e preset `.cni`
compatibili. DX7 accetta anche i `.syx` supportati con voci singole e banchi da 32 voci,
anche all'interno di ZIP. Gli ZIP restano intatti su disco. Il caricamento diretto di WOPL/WOPLX non è supportato;
usa preset CNI compatibili oppure converti prima offline i formati sorgente supportati.

Lo scorrimento dei preset USER continua attraverso file, cartelle, banchi e pacchetti ZIP in
entrambe le direzioni, con ritorno all'inizio alle estremità. Riaprire il browser USER segue la
selezione corrente. ALL include i suoni USER sotto Unsorted per i motori
con categorie; USER resta una scelta distinta di primo livello. Sega PSG e Game Boy usano
elenchi ALL/Factory piatti e un browser USER gerarchico, senza un pannello
Unsorted. I vecchi file DX7 in `instruments/banks/dx7/` restano accessibili in USER.

Un motore mostra solo le sue collezioni incluse compatibili. Factory Presets
combina i set di timbri OPLL/VRC7 inclusi per il motore selezionato; DX7 combina
gli originali ChooChoo e YSE. Le collezioni con nome scaricate restano separate.
Fat Man 2-op è elencato sotto OPL2 e Fat Man 4-op sotto OPL3. I file CNI OPL2
compatibili possono comunque essere usati nelle cartelle USER di OPL3.

L'uscita dei motori nativi usa una compensazione di guadagno fissa misurata anziché una
normalizzazione per preset. Le canzoni esistenti con strumenti nativi possono quindi suonare
a un livello diverso; controlla il bilanciamento dei tuoi mix salvati. Il metodo di calibrazione
e i limiti sono documentati in `scripts/README_MEASURE.md`.
