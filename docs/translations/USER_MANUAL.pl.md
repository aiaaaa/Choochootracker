# Podręcznik użytkownika ChooChooTracker

> Polskie tłumaczenie [`docs/USER_MANUAL.md`](../USER_MANUAL.md), wykonane na podstawie wersji angielskiej z commita `7c3ed6f`. W razie rozbieżności obowiązuje wersja angielska.
>
> **Konwencje.** Interfejs aplikacji jest po angielsku. Dlatego nazwy ekranów, menu, pól i przycisków (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**, **OPT**, **SELECT**...), kody FX (`HOP`, `TBX`, `M1A`...), nazwy modułów i silników (ADSR, LFO, Braids, Plaits...) oraz rozszerzenia plików pozostają nieprzetłumaczone, aby można je było rozpoznać na ekranie.

ChooChooTracker to 8-ścieżkowy tracker muzyczny na konsole przenośne.

Opiera się na klasycznym sposobie pracy i nawigacji LSDj, z kilkoma opcjami projektowania dźwięku:

- emulacja układu AY
- oscylatory Mutable Instruments Braids, Plaits i Plaits-Alt, z filtrem wielotrybowym
- sample PCM, przebiegi single-cycle i oscylatory wavetable, z filtrem wielotrybowym
- aChChid, wszechstronny syntezator acid oparty na Open303
- Bogie, natywny syntezator perkusji z 12 modelami i filtrem wielotrybowym
- MME, syntezator z dwoma oscylatorami i wielokrotną modulacją, z filtrem wielotrybowym
- Sintered, natywny eksperymentalny cyfrowy syntezator perkusji z sześcioma modelami

## 1. Instalacja i pliki

### ArkOS / PortMaster

Zainstaluj `choochootracker.zip` przez PortMaster albo skopiuj rozpakowany pakiet do katalogu `ports` konsoli. Uruchom **ChooChooTracker** z menu Ports.

### Miyoo Mini / Mini Plus (Onion OS)

Rozpakuj archiwum MiyooPorts w katalogu głównym karty SD, tak aby jego foldery `Roms/PORTS/` połączyły się z istniejącymi folderami Onion. Po odświeżeniu kolekcji uruchom **ChooChooTracker** z menu Ports. Przed zastąpieniem istniejącej instalacji zrób kopię zapasową folderu `projects/` aplikacji.

### Windows

Uruchom `choochootracker.exe`, mając `SDL2.dll` i `libwinpthread-1.dll` w tym samym katalogu.
Naciśnij **ALT + ENTER**, aby przełączyć tryb pełnoekranowy. Rozmiar okna można zmieniać; obraz
skaluje się z zachowaniem pierwotnych proporcji.

### Web

Strona w przeglądarce podąża za aktywnym motywem trackera. Zapisane ustawienia, w tym
własne mapowania klawiszy potwierdzone przyciskiem **Done**, są szybko zapisywane w pamięci
tej przeglądarki. Po zablokowaniu ekranu lub przełączeniu aplikacji dotknij dowolnej kontrolki, aby w razie potrzeby
przywrócić dźwięk. **Restart audio** ponownie otwiera wyjście bez utraty
projektu. **Reload app** zapisuje bieżącą pracę w **Continue**, zapisuje ustawienia
i czeka na pamięć przeglądarki przed ponownym załadowaniem; nieudany lub przekroczony czasowo zapis
anuluje ponowne załadowanie. Użyj **Download .cct** do przenośnej kopii zapasowej. Pamięć przeglądarki
można wyczyścić niezależnie, a odzyskiwanie routingu kontrolera/audio w iOS nie jest
gwarantowane.

Wersja przeglądarkowa odświeża obraz z częstotliwością około 30 FPS i używa stałego
bufora audio 1024 ramek, aby dać dźwiękowi więcej czasu między wywołaniami zwrotnymi. Starsze zapisane
ustawienia bufora są dostosowywane automatycznie; projekty i pozostałe ustawienia
zostają zachowane. Odpytywanie wejścia i timery aplikacji zachowują zwykłe tempo.

### Konfiguracja katalogów

| Folder | Co zawiera |
|---|---|
| `AY_Wavetables` | tablice falowe układu AY w formacie `.aywave` |
| `fonts` | pliki czcionek ChipNomad `.cnfont` |
| `instruments` | presety instrumentów `.cni`, w tym pliki ChipNomad |
| `pitch-tables` | alternatywne strojenia `.csv` |
| `projects` | pliki utworów i projektów `.cct` |
| `samples` | pliki `.wav` dla silnika Sampler |
| `SR_wavetables` | tablice falowe w formacie Serum dla silnika BYOWTBL |
| `themes` | motywy ChipNomad `.cth` |
| `title` | zasoby ekranu tytułowego |
| `waveforms` | przebiegi single-cycle `.wav` dla silnika 2xSCWF |

## 2. Sterowanie

ChooChooTracker działa najlepiej z gamepadem z krzyżakiem, 2 analogowymi drążkami i 7 przyciskami.

| Kontrolka logiczna | Domyślnie na konsoli | Domyślnie w Windows |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | krzyżak | Klawisze strzałek |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Spacja |
| **SELECT** | Select | Shift |
| Osie modulacji | Lewy i prawy drążek | Drążki kontrolera |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Mapowania można zmienić w **Settings > Key mapping**. W PortMaster zwolnij
przycisk użyty do wyboru mapowania, zanim naciśniesz nowy przycisk. Zapisz
nowe mapowanie przyciskiem **Done**, a potem zwolnij przycisk przed kontynuowaniem w Settings.
Domyślne układy desktopowe używają QWERTZ tylko w regionach QWERTZ; klawiatury polskie
zachowują standardowe ustawienia domyślne QWERTY.

Użytkownicy Windows, web i Androida mogą też używać kontrolera do gier. Na Androidzie
sterowanie dotykowe pojawia się automatycznie, gdy nie jest podłączony gamepad: w orientacji pionowej
znajduje się pod trackerem i obejmuje kwadratowy krzyżak, przyciski A/EDIT, B/OPT, START/PLAY oraz
SELECT/SHIFT, a następnie osobną sekcję Live/Motion z dwoma drążkami modulacji
i chwilowymi przyciskami REC/DEL. W orientacji poziomej krzyżak jest po lewej,
a przyciski akcji po prawej. Projekty i sample na Androidzie pozostają prywatne dla
aplikacji; użyj selektora plików Androida, aby importować projekty `.cct` i sample `.wav`
oraz wybrać, gdzie zapisywane są eksporty WAV.

### Nawigacja

- Użyj **[UP/DOWN/LEFT/RIGHT]**, aby przesuwać kursor.
- Przytrzymaj **SELECT + [DIRECTION]**, aby przechodzić między ekranami.
- SELECT + [LEFT/RIGHT] wychodzi też ze stron bocznych do sąsiedniego ekranu MSCPIT,
  jak Instrument Pool: Project/Settings (wraz z podmenu Settings)
  prowadzą do Mixer/Chain; Groove do Chain/Instrument; Modulation/Insert FX do
  Phrase/Table; AY Wavetable do Instrument/Table. Na lewej krawędzi Reverb/Delay
  wracają do Mixer przez SELECT + LEFT. Dotychczasowa nawigacja pionowa pozostaje bez zmian.
- Przytrzymaj **OPT + [DIRECTION]**, aby korzystać z nawigacji właściwej dla ekranu.
- Na wartości użyj **EDIT + [LEFT/RIGHT]** do drobnych zmian lub **EDIT + [UP/DOWN]** do grubych.
- Na **Destination** modulacji **LEFT/RIGHT** przełącza dostępne cele bezpośrednio; stuknij **EDIT**, aby otworzyć zgrupowane menu.
- Stuknij **EDIT**, aby wpisać wartość lub uruchomić polecenie.
- Stuknij **EDIT** dwukrotnie, aby utworzyć nowy chain, phrase lub instrument tam, gdzie jest to obsługiwane.
- Przytrzymanie kierunku powtarza go po opóźnieniu ustawionym w Settings.

### Zaznaczanie i schowek

- **SELECT + OPT** włącza tryb zaznaczania i przełącza użyteczne zakresy zaznaczenia.
- Użyj **[DIRECTION]**, aby rozszerzyć zakres.
- **OPT** kopiuje zaznaczenie.
- **OPT + EDIT** wycina je.
- **SELECT + EDIT** wkleja.
- **EDIT + [DIRECTION]** edytuje zaznaczone komórki jednocześnie.

Na ekranie Phrase, przy aktywnym zaznaczeniu, kolumnę nut można wypełnić
wzorami rytmicznymi:

- **OPT + LEFT** wypełnia komórki nut zaznaczenia stałym 16-krokowym wzorem
  i przy każdym naciśnięciu przechodzi do następnego wzoru. Po ostatnim wzorze
  nuty są kasowane i cykl zaczyna się od nowa. Wzory są wyrównane do bezwzględnych
  wierszy phrase, więc częściowe zaznaczenie otrzymuje tylko kroki, które w nim
  się mieszczą.
- **OPT + RIGHT** wypełnia komórki nut zaznaczenia losowym wzorem z
  3-8 nut (7 i 8 są mniej prawdopodobne), cyklicznie bez końca, dopóki **OPT** jest
  przytrzymany.
- **OPT + UP** mutuje nuty już umieszczone w zaznaczeniu: każde naciśnięcie
  wybiera 1-3 z nich i przesuwa każdą o losową wartość w obrębie oktawy
  (małe przesunięcia są bardziej prawdopodobne niż skrajne). Przy aktywnym **Note Lock**
  przesunięte nuty są przyciągane z powrotem do skali; instrumenty Sampler z włączonym
  **Slice** pozostają chromatyczne. Nic nie jest kasowane: 8. naciśnięcie przywraca
  nuty, które zaznaczenie zawierało na początku cyklu, po czym cykl zaczyna się od nowa.
- **OPT + UP** na zaznaczeniu obejmującym tylko kolumnę głośności (bez komórek nut,
  instrumentu ani FX) zamiast mutować losuje velocity: pierwsze
  naciśnięcie stosuje wzór Minimal (każda głośność przesuwa się maksymalnie o
  +-20), drugie Medium (maksymalnie +-50), trzecie Random (całkowicie
  losowe wartości), a czwarte przywraca głośności, które zaznaczenie zawierało
  na początku cyklu. Zwolnienie **OPT** resetuje cykl do Minimal.
  Wiersze bez głośności pozostają nietknięte.
- **OPT + DOWN** rozkłada umieszczone nuty instrumentów Sampler ze slice'ami
  w zaznaczeniu w chromatyczny ciąg: pierwsza nuta każdego instrumentu (najwyższa
  w sekwencerze) zachowuje swoją wartość, a każda kolejna nuta tego
  instrumentu wznosi się o półton ponad poprzednią, zamieniając stos
  identycznych nut w kolejne wyzwalania slice'ów. Kilka instrumentów ze slice'ami
  rozkłada się niezależnie od własnych pierwszych nut, nuty na
  instrumentach bez slice'ów pozostają nietknięte, a wynik nigdy nie jest
  przyciągany do skali projektu. Ostatnia nuta zapisana przez rozkładanie staje się
  ostatnią nutą używaną przy wprowadzaniu, więc ręczne kontynuowanie ciągu
  przejmuje następny slice. Zaznaczenie zawierające wyłącznie instrumenty ze
  slice'ami rozkłada się przy każdym naciśnięciu; gdy obecne są też nuty bez slice'ów,
  rozkładanie działa przy pierwszym naciśnięciu, a od drugiego naciśnięcia
  te nuty przejmuje losowe arpeggio.
- **OPT + DOWN** na zaznaczeniu bez nut instrumentów ze slice'ami to losowy
  arpeggiator dla nut już tam umieszczonych. Akord jest wybierany z
  palety CRD - akordy 3-nutowe dla zaznaczeń 1-2 nut, akordy z 3+ nutami
  dla 3, dowolny akord dla 4 lub więcej - z podstawą na losowej nucie
  skali projektu w rejestrze zaznaczenia (dowolna chromatyczna
  nuta, gdy skala jest wyłączona lub ścieżka jest z niej wykluczona), a jego
  dźwięki są kwantyzowane do skali tak, jak robi to odtwarzanie dla CRD. Każde
  naciśnięcie ponownie rozkłada akord na umieszczone nuty: losowo,
  od najniższej do najwyższej, od najwyższej do najniższej, a potem w górę i w dół; 5.
  naciśnięcie przywraca oryginalne nuty, a następne wybiera nowy akord.
  Nazwa akordu pokazuje się na pasku stanu, gdy arpeggio jest aktywne, żadne
  nuty nie są dodawane ani usuwane, a zwolnienie **OPT** resetuje cały
  proces.
- Każde naciśnięcie wypełnienia kasuje nuty zaznaczenia przed zapisem, a umieszczone
  nuty otrzymują ostatnio użyty instrument; zwolnienie **OPT** resetuje cykle
  do pierwszego kroku.
- Gdy zaznaczenie jest aktywne i nie jest wyświetlany żaden inny komunikat, pasek
  stanu co 2 sekundy zmienia przypomnienie o tych kombinacjach: `EDIT + DIR =
  batch note edit`, `Double-tap EDIT: resample selection`, `OPT + LEFT =
  pattern fill`, `OPT + RIGHT = random fill`, `OPT + UP =
  mutate/randomize velocity` i `OPT + DOWN = random arp/slice spread`.
  Wejście w tryb zaznaczania lub wyjście z niego restartuje rotację; każdy komunikat
  o akcji zastępuje wskazówkę, dopóki nie wygaśnie.

### Odtwarzanie

- **PLAY** startuje od kursora. Naciśnij ponownie, aby zatrzymać.
- Podczas odtwarzania możesz edytować nuty, instrumenty i chainy; zmiany słychać
  przy następnym ticku sekwencera.
- **SELECT + PLAY** uruchamia wszystkie ścieżki, gdy użyty poza ekranem Song.
- **EDIT + PLAY** podsłuchuje instrument z Instrument Pool.

## 3. Struktura utworu i mapa ekranów

Przytrzymaj **SELECT + [DIRECTION]**, aby przechodzić między ekranami.

Główny rząd nawigacji wygląda tak:

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Rząd środkowy:

- **M**: Mixer (odgałęzia się do Reverb, Delay)
- **S**: Song (odgałęzia się do Project, Settings)
- **C**: Chain
- **P**: Phrase (odgałęzia się do Groove)
- **I**: Instrument (odgałęzia się do Modulation i Instrument Pool)
- **T**: Table (odgałęzia się do edytora AY Wavetable)

Rząd górny:

- **R**: Reverb
- **P**: Project
- **G**: Groove
- **M**: Modulation

Rząd dolny:

- **D**: Delay
- **S**: Settings
- **P**: Instrument Pool
- **W**: edytor AY Wavetable

Utwór ChooChooTracker ma następującą hierarchię:

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

Utwór ma 8 ścieżek. Każda ścieżka zawiera sekwencję chainów, a każdy chain zawiera sekwencję phrase'ów. Phrase to 16-krokowe sekwencje nut. Nuta zawiera wysokość dźwięku, instrument, velocity i do 3 efektów FX ścieżki. Instrument to to, co wytwarza dźwięk.

### Opcjonalne czcionki 16x24 i NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 zawiera TechMonoAudit (przekonwertowany z Share Tech Mono), Departure Mono, Spleen i Cozette. Każda z nich zawiera pełny zestaw drukowalnych znaków ASCII w komórkach 16×24 dla wyświetlacza 640×480. Czcionki zbudowane z pikseli zachowują swoje natywne piksele lub skalowanie całkowite z dopełnieniem; Cozette pozostaje mniejsza, ponieważ podwojenie jej 13-pikselowej wysokości tuszu przekroczyłoby komórkę. Motyw NostromoAmberDa2 jest dostępny przez Settings → Edit color theme → Load. Zachowuje bursztynowe tło Nostromo i tekst informacyjny, z czerwonym tekstem podstawowym, ciemniejszymi pomarańczowymi wartościami i morskim kursorem. Załadowanie zasobów nie zmienia wybranej czcionki ani motywu. Informacje o autorach i licencjach znajdziesz w dołączonym pliku README.txt.

### Odstępy na stronie Project

Strona Project umieszcza wersję aplikacji w osobnym wierszu i oddziela Load, Save, New, Export i Manage od metadanych projektu pustymi wierszami. Pozycje kursora i cele dotykowe odpowiadają wyświetlanym polom; ustawienia projektu i operacje na plikach pozostają bez zmian.

### Telemetria wyświetlania audio

Odczyty audio korzystają z należącej do interfejsu migawki końcowego miksu i wkładów poszczególnych ścieżek. Próbki ścieżek i szczyty stereo są pobierane po poziomie ścieżki i tilt, przed powrotami efektów współdzielonych i wzmocnieniem master. Monitorowanie nie zmienia sygnału audio. Callback audio publikuje migawki o stałym rozmiarze bez blokad i alokacji; wyświetlana historia obejmuje około 21 ms.

### Persistent waveform

Graphics → Persistent waveform jest domyślnie wyłączone (OFF). Stuknij EDIT, aby je przełączyć, lub użyj EDIT + kierunek; wybór jest zapisywany jako preferencja aplikacji. OFF przywraca strony o pełnej wysokości i dotychczasowe podglądy przebiegu instrumentu. Po włączeniu dwuwierszowy przebieg nad stronami edycji pokazuje końcowy miks zsumowany do mono. Strony Instrument i Modulation pokazują wyjście ścieżki używającej wybranego instrumentu; jeśli używa go kilka ścieżek, wygrywa wybrana ścieżka Song. Project, Settings, wavetable i okna podręczne zachowują zawartość o pełnej wysokości. Listy przewijają się w pozostałych wierszach. Stopki instrumentów zachowują podglądy ADSR/sampla, a pola Modulation są zagęszczone, aby wszystkie kontrolki pozostały dostępne. Jego cele dotykowe odpowiadają wyświetlanym polom w obu trybach przebiegu.

### Playback piano

Narysowane kodem pikselowe pianino pod ośmioma prawostronnymi wierszami ścieżek podświetla brzmiące klasy wysokości dźwięku, złożone do jednej oktawy. Uwzględnia nuty akordów oraz respektuje ścieżki wyciszone/zatrzymane. Kontur używa koloru wartości motywu, a wszystkie aktywne klawisze używają koloru przebiegu; nieużywane klawisze mają ciemne wypełnienia i subtelne cienie. Siatka używa skalowania całkowitego i obramowań o stałej szerokości. Pianino jest wskaźnikiem odtwarzania, a nie kontrolką wprowadzania nut. Okna podręczne i edytor wavetable zachowują w tym obszarze własną zawartość.

### Mixer meters

Mixer pokazuje sąsiadujące paski szczytów **L/R** dla każdej ścieżki po jej PAN i tilt, przed efektami współdzielonymi i wzmocnieniem master. Miernik obejmuje zakres od -48 do 0 dBFS i opada między aktualizacjami. Dolne i środkowe pasma używają kolorów motywu, a górne jest jaskrawoczerwone w pobliżu pełnej skali.

## 4. Song, Chain i Phrase

### Song

Ekran Song zawiera 8 kolumn, po 1 na każdą ścieżkę audio. Każda komórka odnosi się do chaina. Utwór może zawierać do 256 wierszy (`00-FF`).

Ścieżki biegną od lewej do prawej, a ich chainy pionowo. Każda ścieżka jest niezależna, a chainy mogą zawierać od 1 do 16 phrase'ów, więc ścieżki mogą się rozjeżdżać. Zaprojektuj chainy tak, by trzymały się razem, albo pozwól im błądzić, jeśli tego potrzebuje ścieżka.

Często rezerwuje się 1 chain na puste phrase'y, zwykle `00` lub `FE`.

W jednym projekcie można trzymać wiele pod-utworów, tworząc "wyspy": sekcje oddzielone pustymi wierszami.

#### Live Mode

Dwukrotnie stuknij **OPT**, aby wejść w Live Mode lub z niego wyjść; `LIVE` zastępuje tytuł Song. W Live Mode **PLAY** uruchamia wybrany chain na jego własnej ścieżce, gdy ta ścieżka jest zatrzymana. Na odtwarzanej ścieżce ustawia wybrany chain w kolejce na następną granicę chaina; naciśnij **PLAY** dwa razy, aby pilnie zmienić na następnej granicy phrase'a. Zaznaczona pusta komórka Song ustawia w kolejce kwantyzowane zatrzymanie. `+` oznacza chain w kolejce, `!` chain pilny, a `-` zatrzymanie w kolejce.

Zaznacz jeden wiersz Song w kilku kolumnach, aby wspólnie ustawić w kolejce lub zatrzymać te ścieżki. Zaznaczenia obejmujące wiele wierszy są celowo odrzucane. Kolejki Live są czyszczone, gdy aktywna jest pętla zakresu Song. Live to tryb występu i nie jest zapisywany w projekcie.

#### Sterowanie

- **OPT + [UP/DOWN]**: skok o 16 pozycji w górę lub w dół
- Zaznacz zakres, a następnie użyj **SHIFT + EDIT**: płytkie klonowanie chainów z zachowaniem ich oryginalnych phrase'ów
- Zaznacz zakres, a następnie dwukrotnie stuknij **SHIFT + EDIT**: głębokie klonowanie zarówno chainów, jak i ich phrase'ów
- Zaznacz zakres, a następnie użyj **EDIT + [UP/DOWN]**: przesunięcie zaznaczenia w górę lub w dół
- Stuknij **OPT** 3 razy: dodaje lub usuwa wyróżnienie dla porządku wizualnego
- **OPT + SHIFT**: wycisza bieżącą ścieżkę lub zaznaczone kolumny (najpierw zwolnij **OPT**, aby wyciszenie pozostało aktywne)
- **OPT + PLAY**: solo bieżącej ścieżki lub zaznaczonych kolumn (najpierw zwolnij **OPT**, aby solo pozostało aktywne)
- **OPT + [LEFT/RIGHT]**: solo każdej ścieżki na lewo lub na prawo od bieżącej
- Zaznacz zakres, a następnie dwukrotnie stuknij **EDIT**: otwiera ekran BOUNCE TO SAMPLE (zob. Bounce poniżej)

#### Bounce

Zaznacz zakres wierszy i kolumn Song, a następnie dwukrotnie stuknij **EDIT**, aby otworzyć ekran BOUNCE TO SAMPLE. Bounce obejmuje każdą zaznaczoną ścieżkę: ścieżki, których pierwszy chain zaczyna się później w zaznaczeniu, czekają w ciszy i dołączają przy swoim pierwszym chainie, więc plik zawiera chainy wszystkich zaznaczonych ścieżek.

Ekran oferuje:

- **File name**: edytuj za pomocą klawiatury znakowej. Po otwarciu ekranu proponowany jest tu następny wolny numer kolejny (`001`, `002`, ...): pierwszy numer, którego plik `.wav` nie istnieje jeszcze w folderze eksportu. Edytuj go swobodnie - nazwa jest używana bez zmian. Jeśli plik o tej nazwie już istnieje, dodawany jest przyrostek `_001`, `_002`, ..., aby istniejące pliki nigdy nie były nadpisywane. Licznik resetuje się po wczytaniu lub utworzeniu projektu.
- **Sample rate** i **Bit depth**: te same opcje co na ekranie eksportu
- **Include in a sample name**: trzy pola wyboru, które po naciśnięciu **Start** dopisują na początku nazwy pliku znaczniki kontekstu (domyślnie wszystkie wyłączone):
  - **[BPM]** - bieżące tempo projektu, np. `[120]`
  - **[Key]** - dźwięk podstawowy i skala projektu, np. `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** - wyrenderowana długość bounce'a: jeden phrase to jeden takt (16 szesnastek), jedna ćwierćnuta to 4 szesnastki. Pełne phrase'y pokazują tylko takty, np. `[2]`; niepełne phrase'y pokazują `[0:2]` (dwie ćwierćnuty) lub `[0:1:3]` (jedna ćwierćnuta plus trzy szesnastki). Przy bounce'ach wielościeżkowych decyduje najdłuższa ścieżka. Zaznaczenie kończące się w połowie ćwierćnuty liczy się o jedną szesnastkę mniej: ostatni zaznaczony wiersz jest punktem cięcia, więc jego note-off przypada na koniec poprzedniej szesnastki.
  - Znaczniki są stosowane w kolejności `[BPM][Key][Length]` przed nazwą pliku.
- **Start**: renderuje zaznaczenie do pliku WAV w folderze eksportu (zob. *Lokalizacja eksportu* w sekcji ekranu Project)
- **Cancel**: powrót bez wykonywania bounce'a

Podczas renderowania bounce'a **OPT** go anuluje. Gdy bounce się zakończy lub zostanie anulowany, wraca ekran, z którego go uruchomiono.

Na desktopie key jazz wprowadza tu również bezpośrednie wpisywanie indeksów szesnastkowych oraz zaznaczanie
strzałkami z Shift w stylu Phrase, Delete/Backspace/Insert i Ctrl+C/X/V - zob.
[Key jazz](#key-jazz-tylko-desktop).

### Chain

Chain to uporządkowana lista 16-krokowych phrase'ów z opcjonalną transpozycją. Może zawierać do 16 phrase'ów, a ten sam phrase może wystąpić więcej niż raz. 2. kolumna ustawia transpozycję w półtonach.

Gwiazdka (`*`) pojawia się obok chaina, który jest ponownie użyty w projekcie. Chainy można klonować z ekranu Song.

#### Sterowanie

- **OPT + [LEFT/RIGHT]**: przechodzenie między ścieżkami
- **OPT + [UP/DOWN]**: przechodzenie między chainami na bieżącej ścieżce
- Zaznacz zakres, a następnie użyj **SHIFT + EDIT**: klonowanie phrase'ów
- Zaznacz zakres, a następnie dwukrotnie stuknij **EDIT**: otwiera ekran BOUNCE TO SAMPLE dla zaznaczonych wierszy chaina bieżącej ścieżki (zob. Bounce w sekcji Song)

Na desktopie key jazz wprowadza tu również bezpośrednie wpisywanie indeksów szesnastkowych - zob.
[Key jazz](#key-jazz-tylko-desktop).

### Phrase

Phrase to wzór ścieżki w tradycyjnym sekwencerze krokowym.

Phrase ma 16 kroków, po 1 wierszu na krok. Każdy wiersz zawiera nutę, instrument, głośność od `00` do `7F` oraz kolumny FX. Nuty używają notacji trackerowej, takiej jak `C-4` (nuta i oktawa).

Aby zatrzymać nutę, wstaw `NOTE OFF`, użyj efektu FX kill-note albo zagraj inną nutę bez ustawionego instrumentu.

Gdy chain zawiera kilka phrase'ów, ekran Phrase działa jak ciągły edytor wzorów. Przejście poniżej wiersza `F` lub powyżej wiersza `0` otwiera następny lub poprzedni phrase w chainie.

Gwiazdka (`*`) pojawia się obok phrase'a, który jest ponownie użyty w projekcie. Phrase'y można klonować z ekranu Chain.

Selektor FX pokazuje typowe polecenia oraz te obsługiwane przez instrument w danym wierszu. Dzięki temu unika się długiej globalnej listy poleceń specyficznych dla silników. ChooChooTracker obsługuje więcej efektów FX niż ChipNomad.

### Sterowanie

- **OPT + EDIT** na pustej nucie: wstawia `NOTE OFF`
- **EDIT + [UP/DOWN]** w kolumnie nazwy FX: otwiera ekran wyboru FX
- **OPT + [LEFT/RIGHT]**: przechodzenie między ścieżkami
- **OPT + [UP/DOWN]**: przechodzenie między phrase'ami w bieżącym chainie
- Zaznacz zakres w kolumnie instrumentu, a następnie użyj **SHIFT + EDIT**: klonowanie instrumentów
- Zaznacz zakres, a następnie użyj **EDIT + [UP/DOWN]**: rotacja wierszy phrase'a
- Zaznacz zakres, a następnie dwukrotnie stuknij **EDIT**: otwiera ekran BOUNCE TO SAMPLE dla zaznaczonych wierszy phrase'a bieżącej ścieżki (zob. Bounce w sekcji Song)

### Key jazz (tylko desktop)

Naciśnij **Esc** na ekranie Phrase, aby przełączyć key jazz, tryb wprowadzania nut
w stylu pianina na klawiaturze QWERTY (jak w [m8c](https://github.com/laamaa/m8c)):

- `Z S X D C V G B H N J M` i `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` wpisują nuty
  chromatycznie na ponad dwóch oktawach, wykorzystując fizyczne położenie każdego klawisza,
  więc układ jest taki sam na klawiaturach AZERTY, QWERTY i QWERTZ.
- Każde naciśnięcie klawisza zapisuje nutę w bieżącym wierszu i przechodzi do następnego wiersza.
- `[` / `]` przesuwają oktawę.
- **Shift + strzałki** zaznaczają zakres wierszy i kolumn; zwykła strzałka
  później czyści zaznaczenie.
- **Ctrl+C** / **Ctrl+X** kopiują lub wycinają zaznaczone kolumny albo
  nutę/instrument/głośność bieżącego wiersza, jeśli nic nie jest zaznaczone; **Ctrl+V** wkleja w
  miejscu kursora.
- **Delete** usuwa cały wiersz (wszystkie kolumny) i przesuwa resztę
  phrase'a w górę, aby wypełnić lukę; kursor zostaje w tym samym wierszu.
- **Backspace** usuwa element pod kursorem tylko w bieżącej kolumnie
  (lub w kolumnach zaznaczenia, jeśli jest aktywne), przesuwa wszystko, co jest
  poniżej, w górę, aby wypełnić lukę, i cofa kursor o jeden wiersz, jak w
  edytorze tekstu.
- **Insert** wstawia pusty wiersz, spychając resztę phrase'a w dół.
- **Ctrl+S** zapisuje projekt. Poza key jazz Ctrl+S nic nie robi.
- Ponowne naciśnięcie Esc wyłącza key jazz.

Gdy key jazz jest włączony, całkowicie przejmuje klawisze nut na ekranie
Phrase, w tym skróty Edit/Opt/Motion, które dzielą te klawisze - wyłącz go
klawiszem Esc, aby znów z nich korzystać.

Key jazz działa też, włączany niezależnie klawiszem Esc, na:

- **Song** i **Chain**: wpisz bezpośrednio indeks szesnastkowy chaina lub phrase'a
  (`0-9`, `A-F`) zamiast zwiększać go klawiszami Up/Down. Pierwsza wpisana cyfra
  w komórce zastępuje jej wartość; kolejne cyfry wpisane bez przesuwania kursora
  wsuwają się do niej (wpisanie "3", a potem "F" ustawia 3F). Na Song **Shift+strzałki** zaznaczają
  zakres, **Delete** usuwa całe wiersze i przesuwa resztę w górę, **Backspace**
  usuwa bieżące/zaznaczone kolumny i przesuwa to, co jest pod nimi, w górę,
  **Insert** wstawia pusty wiersz, a **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  kopiują/wycinają/wklejają zaznaczone wiersze (lub bieżący wiersz, jeśli nic nie jest
  zaznaczone) - te same skróty edycji struktury co na ekranie Phrase.
  To przejmuje Shift, więc Shift+Right/Up nie przeskakują już do Chain/Project,
  gdy key jazz jest tu aktywny - naciśnij Esc, aby odzyskać te skróty.
- **Każdy ekran z polem tekstowym**: pisz bezpośrednio na klawiaturze
  zamiast używać klawiatury ekranowej. Obejmuje to nazwę pliku projektu,
  tytuł i autora w Project, nazwę instrumentu, nazwę motywu kolorów,
  nazwę tabeli strojenia, nazwę pliku bounce'a oraz okna nazw (wpisanie
  nazwy, utworzenie folderu). Backspace działa jak w zwykłym polu tekstowym; Shift
  wpisuje wielkie litery. Esc przełącza go jednocześnie na wszystkich tych ekranach,
  a klawiatura ekranowa nadal otwiera się normalnie, gdy key jazz jest wyłączony.

## 5. Instruments

Wszystkie instrumenty mają kilka wspólnych parametrów:

- Typ instrumentu
- Nazwa, do 15 znaków
- Domyślna szybkość tabeli, w tickach na wiersz tabeli
- Transpozycja włączona lub wyłączona

Każdy instrument ma domyślną tabelę o tym samym numerze z zakresu `00-7F`. Możesz używać ich jako tabel pomocniczych, ale trzymanie tabel pomocniczych w zakresie `80-FE` pozwala uniknąć konfliktów i zamieszania. Tabele należą do głównych narzędzi projektowania dźwięku w ChooChooTracker.

### Sterowanie

- **OPT + [LEFT/RIGHT]**: przechodzenie między instrumentami
- **EDIT + PLAY**: podsłuch instrumentu
- **SHIFT + OPT**: kopiowanie instrumentu
- **SHIFT + EDIT**: wklejanie instrumentu

### AY Classic, AY Plus i AY Sample

To oryginalne silniki AY/YM z ChipNomad. AY Classic udostępnia sprzętowe kontrolki tonu, szumu i obwiedni. AY Plus dodaje programowe oscylatory i bogatszą modulację. AY Sample odtwarza sample przez poziomy głośności w stylu AY i różni się od silnika Sampler.

#### AY Classic

To przestarzały typ instrumentu z ChipNomad `v1.0.0`. Nowszy instrument AY Plus oferuje więcej funkcji.

Instrumenty AY Classic mają następujące parametry:

- Mixer: ton włączony lub wyłączony, szum włączony lub wyłączony, kształt obwiedni (`0-F`)
- Volume: programowo generowana obwiednia ADSR
- Automatyczny okres obwiedni: włączony lub wyłączony, z szybkością od `1:1` do `F:F`

**AY Quality** w Settings wpływa tylko na renderowanie AY/YM. Nie zmienia jakości Braids, Plaits ani Sampler. **Sample dithering** dotyczy kwantyzacji AY Sample.

Szczegóły układu znajdziesz w [dokumentacji ChipNomad do AY-3-8910](https://chipnomad.org/chips/ay-3-8910/).

#### AY Plus

AY Plus stosuje klasyczne pojęcia syntezatorowe do projektowania dźwięku AY-3-8910/YM2149F. AY można traktować jako syntezator z 5 oscylatorami:

- 3 oscylatory tonowe o przebiegu prostokątnym
- 1 oscylator szumu
- 1 generator obwiedni amplitudy, który może służyć jako oscylator

W tym modelu każdy instrument ma 3 oscylatory sprzętowe: ton, szum i obwiednię. AY Plus dodaje 4., programowy oscylator. Aktualizowanie rejestrów AY szybciej niż częstotliwość ticków projektu otwiera więcej możliwości projektowania dźwięku. Ze względu na sposób działania AY oscylatory są miksowane przez modulację pierścieniową. Wypróbuj różne przesunięcia wysokości, detuning i kombinacje oscylatorów, aby zobaczyć, co z tego wyjdzie.

Każdy oscylator tonowy można stroić niezależnie przesunięciami wysokości i dostrojenia.

Typy oscylatorów programowych i ich parametry:

- Pulse - fala prostokątna z regulowaną szerokością impulsu
  - FM Depth - prosta głębokość FM
  - Pulse width - pełny zakres to `00-FF`, a liczba kroków jest ustawiana na ekranie Project
  - Pulse low - steruje niską wartością w powtarzającej się parze, której wysoka wartość to zawsze `15`
- Sync Tone - efekt twardej synchronizacji dla oscylatora tonowego
  - FM Depth - prosta głębokość FM
- Sync Env - efekt twardej synchronizacji dla oscylatora obwiedni
  - FM Depth - prosta głębokość FM
  - Pulse width - stosunek czasu trwania 2 kształtów w parze obwiedni
  - Envelope Pair - 2 kształty obwiedni, między którymi następuje przełączanie
- Wavetable - 32-krokowe wavetable, edytowane na ekranie Wavetable
  - FM Depth - prosta głębokość FM
  - Wavetable index
- Tone FM - prosty efekt podobny do FM dla oscylatora tonowego
  - FM Depth - głębokość FM tonu
- Env FM - prosty efekt podobny do FM dla oscylatora obwiedni
  - FM Depth - głębokość FM obwiedni

Programowych oscylatorów Pulse i Wavetable nie można używać z oscylatorem Envelope. Oscylator Envelope wyłącza się automatycznie.

Proste FM szybko przełącza się między 2 wartościami okresu, 1 niższą i 1 wyższą. Jest to podobne do użycia fali prostokątnej jako modulatora FM. Pulse, Sync Tone, Sync Env i Wavetable używają stosunku `1:1` między nośną a modulatorem FM. Tone FM i Env FM mogą naśladować inne stosunki przez przesunięcia wysokości oscylatorów programowych.

#### AY Sample

Możesz wczytywać mono lub stereo pliki WAV w formatach `8-bit`, `16-bit`, `24-bit` lub `32-bit`. Są konwertowane do `8-bit` i obcinane do `16 KB`. Ten limit odpowiada rozmiarowi strony pamięci RAM ZX Spectrum.

AY odtwarza sample przez szybkie zapisy do rejestru głośności. Jego wyjście jest unipolarne, więc sample trzeba "podnieść do zera", aby uzyskać czystsze odtwarzanie. Usuwa to trzaski na końcu sampla i poprawia ciche ogony.

AY ma przetwornik DAC o rozdzielczości `4-bit` z nieliniową skalą głośności, więc sample `8-bit` są podczas odtwarzania konwertowane do `4-bit`. Opcja Sample dithering w Settings może poprawić postrzeganą głębię bitową. Dithering jest niepraktyczny na platformach retro, takich jak ZX Spectrum i Atari ST, więc wyłącz go przy pisaniu na prawdziwy sprzęt lub gdy chcesz uzyskać chrupiące brzmienie lo-fi.

Sample zachowują się jak kolejny programowy oscylator AY Plus, ale ich dodatkowe parametry sprawiają, że łatwiej obsługiwać je jako osobny typ instrumentu.

Parametry sampla:

- Sample rate - odtworzenie sampla z jego natywną częstotliwością daje `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

Sekcje oscylatorów Tone i Noise odpowiadają tym z AY Plus. Ze względu na sposób działania AY wszystkie oscylatory są miksowane przez modulację pierścieniową.

### Braids

Braids oferuje 47 modeli syntezy. Jego główne kontrolki to:

- **Model**: oscylator lub algorytm syntezy
- **Timbre** i **Color**: parametry makro zależne od modelu
- **Filter**: dodatkowy filtr LP/BP/HP, nachylenie `12` lub `24 dB`, cutoff i rezonans
- **ADSR**: attack, decay, sustain i release

Stuknij **Model**, aby wybrać z list modeli podzielonych na kategorie.

| Kategoria | Modele |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Parametry

Timbre i Color zależą od wybranego modelu. Pełną listę modeli i szczegóły znajdziesz w oryginalnej instrukcji Braids od Mutable Instruments, albo po prostu graj ze słuchu.

#### VCF i VCA

Braids zasila standardowy filtr wielotrybowy i obwiednię ADSR ChooChooTracker.

#### Ustawienia

Globalne ustawienia Braids są dostępne w ustawieniach aplikacji:

- **BITS**: zmniejsza rozdzielczość wyjścia
- **DRFT**: dodaje niestabilność wysokości oscylatora
- **SIGN**: odtwarza oryginalny algorytm niedoskonałości przebiegu Braids

### aChChid

**aChChid** to monofoniczny silnik acid bass oparty na Open303. `Square` i `Saw` używają jego natywnego oscylatora, filtra, obwiedni i zachowania akcentu TB-303. `Braids` zastępuje tylko oscylator, a następnie przechodzi przez tę samą ścieżkę filtra 303 i wzmacniacza. Udostępnia Model, Timbre, Color i Shaper zamiast Fine tune. Shaper przechodzi od łagodnego nasycenia do wavefoldingu. aChChid nie używa ujednoliconego postfiltra ani ADSR ChooChooTracker.

Cutoff i Decay używają edycji logarytmicznej: **EDIT + LEFT/RIGHT** przesuwa o jeden
muzyczny krok kontrolki, a **EDIT + UP/DOWN** o szesnaście kroków. Cutoff
obejmuje zakres od 200 Hz do 20 kHz; Decay od 200 ms do 2 s.

`F` w kolumnie głośności nuty wyzwala akcent. Zwykłe nuty używają bramki o długości pół kroku (3 ticki w domyślnym groove o 6 tickach). `ASL` przesuwa płynnie do tej nuty od poprzedniej wysokości bez ponownego wyzwalania obwiedni 303; automatycznie przytrzymuje poprzednią bramkę, a `ASL 00` daje glide `60 ms`. `ATY` utrzymuje swoją nutę otwartą przez cały bieżący krok. Nuty bez `ASL` zawsze wyzwalają się ponownie. Cele modulacji obejmują Decay i Accent, a w trybie fali Braids także Timbre i Color.

### Bogie

**Bogie** to natywny syntezator perkusji typu one-shot. Nuty trackera ustawiają wysokość, a głośność trackera/instrumentu ustawia poziom; każde wyzwolenie ma własny wewnętrzny decay, więc nie ma strony ADSR. Ma 12 modeli: `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` i `Clave`.

Jego sześć makr to **Decay**, **Tone**, **Sweep**, **Noise**, **FM** i **Drive**. Ich etykiety dostosowują się tam, gdzie to użyteczne (na przykład Noise w Kick to `Click`, a FM w Cowbell to metaliczna modulacja krzyżowa). Każde makro jest aktywne w każdym modelu, ale jego muzyczna rola zmienia się wraz z modelem: Sweep może zmieniać wysokość, odstępy między impulsami lub metaliczny rozrzut; FM może dodać modulację krzyżową, metaliczną warstwę lub alikwot przypominający modulację pierścieniową. Dolna połowa każdego zakresu przeznaczona jest do konwencjonalnych brzmień perkusyjnych; górna celowo otwiera szersze zmiany barwy i bardziej syntetyczne rezultaty.

Każdy model może używać współdzielonego filtra wielotrybowego LP/BP/HP, o charakterze Clean, Classic, Aggro lub Acid, nachyleniu `12`/`24 dB`, cutoff i rezonansie. Bogie obsługuje wszystkie swoje aktywne makra oraz Volume, Pitch, Cutoff i Resonance jako cele modulacji i nagrywania ruchu. Użyj krótkiego Decay z Cowbell dla ostrych ataków phonk; zwiększ FM dla twardszego, bardziej metalicznego dzwonka.

Na ekranie Modulation naciśnij **EDIT** na źródle, aby wybrać rodzinę (Envelopes, LFO lub Sticks), a potem źródło. **Left** i **Right** nadal przełączają źródła bezpośrednio.

### Sintered

**Sintered** to natywny cyfrowy syntezator perkusji typu one-shot. Nuty ustawiają jego wysokość podstawową; jego sześć modeli to `Knot`, `Shard`, `Burst`, `Comb`, `Logic` i `Melt`. Każdy model ma **Decay**, **Mod** i **Motion**, plus trzy kontrolki specyficzne dla modelu: Knot używa Ratio/Spread/Fold, Shard Ratio/Feedback/Bite, Burst Noise/Color/Feedback, Comb Time/Damping/Regen, Logic Rate/Pattern/Crush, a Melt Ratio/Chaos/Drive. **Mod** to zawsze wewnętrzna głębokość sprzężenia.

Motion to wewnętrzna obwiednia makro, a nie glide wysokości: wartości poniżej `80` używają kształtu AD, `80` ją wyłącza, a wartości powyżej `80` używają kształtu tylko z decay. Oddalanie się od środka przyspiesza obwiednię. Jej cele są stałe dla każdego modelu, więc Sintered pozostaje przewidywalny we wzorach trackera: Knot animuje Mod/Fold; Shard Feedback/Bite; Burst Noise/Color/Feedback; Comb Damping/Regen; Logic Rate/Crush; Melt Chaos/Drive. Wszystkie modele resetują swój stan przy wyzwoleniu nuty i ograniczają wewnętrzny feedback, aby uderzenia były powtarzalne.

### MME

**MME** (Multi Modulation Engine) to syntezator z dwoma oscylatorami poświęcony interakcjom między oscylatorami. Jego modele to `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` i `Vocode`. Oba oscylatory są wewnętrzne i podążają za nutami trackera; `Interval` ustawia ich wzajemną relację.

Sześć makr to **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** i **Shaper**. Waves wybiera muzyczne pary przebiegów A/B. Amount steruje podstawową interakcją modelu, Flow zmienia jej kierunek lub wariant algorytmu, a Feedback jest ograniczony, ale pod koniec swojego zakresu celowo robi się dziki. Shaper to współdzielony etap końcowy, który przechodzi od czystego brzmienia przez nasycenie do wavefoldingu.

MME używa współdzielonego filtra wielotrybowego LP/BP/HP i ADSR. Wszystkie sześć makr, Cutoff i Resonance są dostępne dla modulacji i nagrywania ruchu. Przytrzymaj **EDIT** na Model, aby otworzyć okno modeli.

### MIDI Out

**MIDI Out** (tylko desktop) steruje zewnętrznym urządzeniem MIDI zamiast syntezować dźwięk: wyzwolenie nuty wysyła prawdziwe MIDI Note On na **Channel** instrumentu (`1-16`) i, gdy nuta się kończy, odpowiadające mu Note Off. Głośność staje się velocity, a każdy głos ścieżki akordowej dostaje własne Note On/Off. Sam ChooChooTracker nie wytwarza żadnego dźwięku - najpierw wybierz urządzenie wyjściowe w [Settings](#12-settings). Pełny obraz, w tym podsłuch dźwięku z klawiatury MIDI, znajdziesz w [MIDI](#14-midi).

**Program** i **Bank high/low** (CC0/CC32) są opcjonalne - gdy są wyłączone, wyświetlają się jako `--`, a `Clear` wyłącza pole i zapamiętuje jego wartość. Po ustawieniu są wysyłane raz, tuż przed następną nutą, ilekroć różnią się od tego, co ostatnio przekazano temu kanałowi (nie przed każdą nutą, co powodowałoby ponowne wyzwalanie własnych obwiedni/patchu urządzenia odbiorczego).

**MC1 number**-**MC4 number** wybierają, na jaki numer CC (`00-7F`) wysyła każdy z czterech FX wiersza MC1-MC4 - zob. [MIDI Out FX](#midi-out-fx). Również `--`/opcjonalne; nieustawiony slot czyni jego FX nieaktywnym.

### Silniki subtraktywne

Silniki z tej kategorii mają wspólną architekturę VCO - VCF - VCA.

Dostępnych jest kilka typów VCO:

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Każdy instrument kieruje swój VCO do filtra wielotrybowego.

Filtr ma przełączane nachylenie `12 dB` lub `24 dB` i może działać jako dolnoprzepustowy (LP), pasmowoprzepustowy (BP) lub górnoprzepustowy (HP).

Dostępnych jest kilka charakterów filtra:

- **Clean**: sterylny i cyfrowy
- **Classic**: inspirowany klasycznymi amerykańskimi syntezatorami analogowymi
- **Aggro**: inspirowany ryczącymi japońskimi syntezatorami analogowymi
- **Acid**: inspirowany psychodelikami

Classic, Aggro i Acid potrafią w tej wersji alfa bardzo mocno rezonować. Zacznij od niskich wartości i podkręcaj delikatnie.

Filtr zasila VCA sterowany obwiednią ADSR. Kształt obwiedni może płynnie przechodzić między krzywymi wykładniczą, liniową i logarytmiczną.

#### Plaits i Plaits-Alt

**Plaits** oferuje 24 silniki, pogrupowane w oknie wyboru w następujący sposób:

| Kategoria | Silniki |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** to kolekcja alternatywnych modeli dla oryginalnego modułu Plaits. Większość z nich jest dziwniejsza lub bardziej eksperymentalna niż przeciętny VCO.

| Kategoria | Silniki |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

Parametry podążają za projektem Mutable:

- **Harmonic** steruje zależnościami harmonicznymi, balansem lub wyborem modelu wewnątrz silnika.
- **Timbre** zwykle przechodzi od ciemnych/rzadkich do jasnych/gęstych widm.
- **Morph** bada inny wymiar barwy.
- **Main/Aux** miksuje główne wyjście z alternatywnym wyjściem silnika.

Ich dokładne znaczenie zależy od silnika. Na przykład silniki akordowe używają ich do typu akordu, odwrócenia i przebiegu. Modele fizyczne używają ich do materiału, pobudzenia i decay, a silniki perkusyjne do tonu, charakteru i decay. Szczegóły znajdziesz w instrukcji użytkownika Plaits.

Dla `CHORD`, `DIATONIC CHORD` i `WT DIATONIC CHORD` Harmonic wyświetla nazwę wybranego akordu (`MIN`, `MAJ`, `MIN7`, `MAJ7` itd.) zamiast surowej wartości kontrolki. Braids `WAVE PARA` podobnie nazywa swoje układy głosów Color.

Stuknij **Engine**, aby wybierać z list silników podzielonych na kategorie.

**Env Mode** ma 2 tryby routingu:

- `TRIG` odtwarza moduł z podłączonym TRIG i niepodłączonym LEVEL
- `VCA` utrzymuje LEVEL otwarty i stosuje obwiednię ADSR trackera po głosie

#### Sampler

Ten czysty silnik Sample odtwarza mono lub stereo sample PCM wczytane do RAM.

- Stuknij **Sample**, aby wczytać nieskompresowany plik WAV PCM `8-bit`, `16-bit` lub `24-bit`. Naciśnij **PLAY** w przeglądarce plików, aby odsłuchać zaznaczony plik.
- Gdy sampel jest wczytany, obok jego nazwy pojawia się **EDIT**. Stuknij go, aby otworzyć ekran Sample Edit. **OPT** lub **SELECT + [LEFT]** wraca do instrumentu.
- Ekran Sample Edit pokazuje nazwę pliku (ze znacznikiem `*`, gdy sampel w RAM różni się od pliku na dysku), odczyt formatu (częstotliwość próbkowania, kanały), wysoki przebieg oraz pola **Region**, **Select**, **Slice**, **Process** i **File**.
- Edycje żyją tylko w RAM, dopóki nie zapisze ich jakaś ścieżka zapisu: projekt przechowuje ścieżkę pliku sampla i przy następnym wczytaniu projektu ładuje WAV z dysku, więc wyjście z trackera bez **Save**/**Save As** odrzuca operacje przetwarzania (znacznik `*` jest ostrzeżeniem). Wczytanie innego sampla do instrumentu również odrzuca niezapisane edycje i resetuje Region do pełnego sampla (Start `00`, End `FF`).
- **Region** ustawia znaczniki Start/End odtwarzania, dostępne też na ekranie instrumentu Sampler. Ustawiają one znormalizowane granice odtwarzania (`00-FF`); jeśli Start jest za End, sampel odtwarza się wspak. **EDIT + [LEFT/RIGHT]** dostraja znacznik co jeden (`01`) i powiększa przebieg wokół niego, dopóki **EDIT** jest przytrzymany; **EDIT + [UP/DOWN]** reguluje zgrubnie (krok 16) i przywraca widok całego sampla. **EDIT + OPT** resetuje znacznik do wartości domyślnej (Start `00`, End `FF`).
- Widok przebiegu powiększa się do stałego okna jednej sekundy audio wokół edytowanego znacznika i przesuwa się tak, aby go utrzymać w widoku; sample mieszczące się w oknie zostają w pełnym widoku 1:1. Powiększenie jest przejściowe: trwa, dopóki **EDIT** jest przytrzymany, a po zwolnieniu **EDIT** wraca pełny widok 1:1, podobnie jak po krokach zgrubnych. Wejście na ekran zawsze resetuje widok do całego sampla.
- **Select** ustawia zaznaczenie do przetwarzania w ramkach bezwzględnych, niezależnie od znaczników Start/End odtwarzania. **EDIT + [LEFT/RIGHT]** przesuwa uchwyt o dwadzieścia ramek i powiększa na nim widok, dopóki **EDIT** jest przytrzymany; **EDIT + [UP/DOWN]** skacze o `frameCount/64` (minimum 16) i wraca do widoku całego sampla. **EDIT (tap)** na uchwycie kopiuje do niego bieżącą pozycję znacznika Start lub End. **EDIT + OPT** na którymkolwiek uchwycie opróżnia całe zaznaczenie. Gdy zaznaczenie jest puste, oba uchwyty pokazują `-`; gdy uchwyty są odwrócone, zamieniają się automatycznie. Zaznaczenie jest stanem edytora tylko na czas sesji: nie jest zapisywane z projektem, a wejście na ekran wypełnia je zakresem Region odtwarzania (cały sampel przy domyślnych znacznikach).
- **Slice** to pojedynczy wiersz z czterema komórkami wartości - **Mode**, **Count**,
  **Slice** i **Frame** - zapisywany z instrumentem. Off odtwarza okno
  Start/End. Nuty phrase'a wybierają slice'y chromatycznie od **C-0**, a
  nuty poza ostatnim slice'em zawijają się do początku (nuta N odtwarza slice
  `N % count`, więc ciąg wykraczający poza ostatni slice wraca cyklicznie do pierwszego).
  Nuty ze slice'ami nie transponują wysokości ani nie używają kwantyzacji do skali, ale **CRD**
  zachowuje wybrany slice i transponuje jego głosy jako akord. Początki slice'ów
  są oznaczone na przebiegu krótkimi dwupikselowymi liniami (ciemnopomarańczowymi; linia
  bieżącego slice'a jest jaśniejsza), a slice pod kursorem otrzymuje
  czarne pasmo tła na przebiegu (odcień zaznaczenia nie jest rysowany nad
  pasmem, więc pozostaje w pełni czarne i wyraźnie odróżnia się od reszty
  podglądu).
  - **Mode** przełącza `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + left/right lub
    stuknięcie; EDIT + OPT wyłącza dzielenie). Przełączenie trybu inicjalizuje punkty
    slice'ów: **EQUAL** dzieli okno Start/End równo na **Count**
    części; **AUTO** uruchamia detekcję transjentów w oknie i umieszcza
    slice na każdym znalezionym początku dźwięku (do **Count**); **LAZY** zaczyna od jednego
    slice'a obejmującego całe okno. Przełączenie na LAZY odkłada bieżące
    ustawienie EQUAL/AUTO (liczbę i granice), a powrót do EQUAL lub
    AUTO przywraca je dosłownie - nie jest wymagane potwierdzenie i nic nie
    ginie. Slice i Stretch wzajemnie się wykluczają: włączenie jednego wyłącza
    drugie, a komórki Slice są przyciemnione, gdy czas trwania wyznacza Stretch.
  - **Count** pokazuje łączną liczbę slice'ów (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** zmienia ją o jeden (minimum 1, maksimum 64), **EDIT +
    [UP/DOWN]** przechodzi przez potęgi dwójki `2 / 4 / 8 / 16 / 32 /
    64` z zawijaniem. Każda zmiana przelicza podział: **EQUAL**
    dzieli okno ponownie równo, **AUTO** ponownie uruchamia detekcję z nowym
    celem (czułość detekcji wynika z liczby - więcej slice'ów
    oznacza czulszy próg). Pole pokazuje `-`, gdy tryb to
    OFF. W **LAZY** pozostaje jasne (liczy slice'y ustawione ręcznie), ale
    jest tylko do wyświetlania: kursor je pomija - przesunięcie w prawo z **Mode** lub
    w lewo ze **Slice** ląduje na **Slice**, zejście z markera **END**
    wiersza **Select** też tam ląduje, a dotknięcie pola nic nie robi
    (ręcznie ustawione slice'y nie mają podziału do przeliczenia). **EDIT + OPT** i
    zwykłe stuknięcie **EDIT** nic tu nie robią: wyłączenie dzielenia znajduje się w
    **Mode**, a usuwanie slice'a w **Slice**.
  - **Slice** to przeglądarka: pokazuje bieżący slice numerowany od 1
    (`01`..count). **EDIT + [LEFT/RIGHT]** przechodzi między slice'ami po
    jednym, **EDIT + [UP/DOWN]** skacze o cztery slice'y (oba ograniczone, bez
    zawijania); przeglądany slice jest oznaczony na przebiegu jaśniejszym
    znacznikiem i czarnym pasmem, a powiększony widok centruje się na
    początku slice'a. **EDIT + OPT** usuwa bieżący slice (łączy się z
    poprzednim; pierwszego slice'a nie można usunąć, a usunięcie
    ostatniego pozostałego slice'a wyłącza dzielenie - granice przetrwają
    przełączenie na OFF i wrócą po ponownym wybraniu trybu). **EDIT (tap)**
    odsłuchuje aktualnie wybrany slice jako jednorazowe odtworzenie (działa w
    każdym trybie dzielenia; odsłuch kończy się po zwolnieniu klawisza lub
    opuszczeniu ekranu). Pole pokazuje `-`, gdy tryb to OFF.
  - **Frame** pokazuje początkową ramkę bieżącego slice'a w zapisie szesnastkowym. **EDIT +
    [LEFT/RIGHT]** przesuwa ją o dziesięć ramek (powiększając widok na znaczniku, dopóki
    **EDIT** jest przytrzymany), **EDIT + [UP/DOWN]** przesuwa o sto i
    wraca do pełnego widoku. Znacznik pozostaje w obrębie własnego slice'a: nie
    może przekroczyć początku poprzedniego ani następnego slice'a. **EDIT + OPT** usuwa
    slice.
  - **B** wychodzi do ekranu instrumentu z wiersza Slice jak wszędzie
    indziej na tym ekranie; **SHIFT + LEFT** wychodzi z dowolnego miejsca.
  - **Przepływ pracy LAZY**: z wybranym LAZY stuknij **PLAY**, aby uruchomić jednorazowe
    odtwarzanie całego sampla - odtwarza się raz od początku i zatrzymuje
    samo w oryginalnej wysokości i szybkości sampla (zielony
    dwupikselowy znacznik podąża za pozycją na przebiegu); ponowne stuknięcie **PLAY**
    zatrzymuje je wcześniej. Podczas odtwarzania
    każde kliknięcie **EDIT** upuszcza slice w pozycji odtwarzania, bez względu na to,
    gdzie jest kursor (slice'y bliższe niż 50 ms od istniejącego są
    odrzucane komunikatem `Too close to slice`). Ponowne odtworzenie sampla zachowuje
    istniejące cięcia i dodaje nowe w każdej stukniętej pozycji, w
    kolejności - pole **Count** odświeża się przy każdym upuszczonym slice'u, a przeglądarka
    **Slice** podąża za nowym. Obowiązują te same zasady przeglądania i edycji co w
    innych trybach dla slice'ów ustawionych ręcznie: **EDIT +
    kierunek** na **Slice** je przegląda, **Frame** reguluje wybrany
    początek, **EDIT + OPT** usuwa bieżący slice. Komórka Frame jest przyciemniona,
    gdy upuszczanie podczas odtwarzania jest uzbrojone. **SHIFT + PLAY** nie jest przechwytywane
    w LAZY: uruchamia odtwarzanie phrase'a jak na każdym innym ekranie. Slice'y LAZY
    odtwarzają się w sekwencerze tak samo jak EQUAL i AUTO: nuty phrase'a
    mapują się chromatycznie od **C-0** na ręcznie ustawione slice'y,
    zawijając się po ostatnim.
  - **Wskazówki kombinacji**: gdy kursor spoczywa na komórce wiersza Slice i nie jest wyświetlany żaden inny
    komunikat, pasek stanu pokazuje wskazówkę dla tej komórki i utrzymuje ją,
    dopóki kursor tam spoczywa. W **Mode** wskazówka jest związana z
    aktywnym trybem i pokazuje tylko opis (nazwa trybu jest już w
    komórce): `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - przełączenie trybów
    natychmiast zmienia tekst, a OFF czyści pasek. W **Count** pokazuje
    `Adjust the number of slices`, w **Slice**
    `Browse slices`, w **Frame** `Adjust slice start`. Każdy komunikat o akcji
    zastępuje wskazówkę, dopóki nie wygaśnie.
- **Process** wybiera destrukcyjną operację edycji: **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** lub **Reverse** (EDIT + left/right przełącza, stuknięcie przełącza do przodu, EDIT + OPT ustawia brak). **GO** (ten sam wiersz) uruchamia wybraną operację na bieżącym zaznaczeniu lub na całym samplu, gdy zaznaczenie jest puste. **Crop** zachowuje tylko zaznaczenie; **Normalize** skaluje zaznaczenie tak, by jego szczyt osiągnął pełną skalę (oba kanały dzielą jedno wzmocnienie, więc obraz stereo jest zachowany); **Delete** usuwa zaznaczenie i łączy końce (usunięcie całego sampla jest odrzucane); **Silence** zeruje je; **Fade In**/**Fade Out** liniowo narastają od ciszy / opadają do ciszy; **Reverse** odtwarza zaznaczenie wspak (ramki są zamieniane w miejscu, oba kanały ramki przesuwają się razem, długość i znaczniki pozostają bez zmian). Crop i Delete wymagają zaznaczenia - przy pustym zaznaczeniu pokazują `Select region first`. Każda operacja na krótko wstrzymuje audio, zachowuje jednopoziomowe cofnięcie i oznacza sampel jako zmieniony w RAM: plik na dysku nie jest ruszany, dopóki nie zadziałają ścieżki Save (zob. niżej), a opuszczenie ekranu odrzuca slot cofania.
- **UNDO** (obok GO) przywraca sampel do stanu sprzed ostatniej operacji. Jest przyciemniony, dopóki nie zostanie wykonana operacja, przy kolejnych naciśnięciach przełącza się między stanem sprzed i po operacji i jest czyszczony przy ponownym wejściu na ekran.
- **File** zawiera ścieżki zapisu. Instrument przechowuje pełną ścieżkę pliku WAV, z którego został wczytany; te ścieżki zapisują ten plik lub kierują instrument na nowy. Nigdy nie zmieniają nazwy instrumentu.
  - Pole File przełącza między **Save** a **Save As** (EDIT + left/right lub stuknięcie); **GO** (ten sam wiersz) uruchamia wyświetlaną akcję.
  - **Save** nadpisuje plik WAV, z którego sampel został wczytany. Dla sampla
    bez slice'ów najpierw pyta `Overwrite <name>?`. Sampel ze slice'ami najpierw
    otwiera okno **SAVE SLICES** z pytaniem, do czego mają należeć punkty slice'ów:
    **SAVE TO SAMPLE** zapisuje je w WAV jako fragmenty cue (przenośne -
    programy DAW pokazują je jako znaczniki) i zachowuje je także w projekcie,
    **SAVE TO PROJECT** zapisuje zwykły WAV i zachowuje punkty tylko w
    projekcie, **CANCEL** przerywa. Pole wyboru `Don't ask again in this project`
    zapisuje wybrany cel w projekcie i od tej pory pomija
    okno (wybór jest zapisany jako `- Sample save choice:` w
    pliku .cct i można go zmienić tylko przez edycję pliku projektu). Okno
    jest pomijane dla sampli bez slice'ów. **Save** jest przyciemniony (i
    pomijany w nawigacji), dopóki nie zostanie wczytany sampel ze ścieżką pliku. Nieudany
    zapis zachowuje znacznik zmian i pokazuje błąd.
  - **Save As** zapisuje bieżący sampel do nowego pliku: wpisz nazwę pliku (wstępnie wypełnioną bieżącą nazwą bez rozszerzenia), a potem wybierz folder w przeglądarce. Sampel jest zapisywany jako `<folder>/<name>.wav`, instrument wskazuje nowy plik, a folder jest zapamiętywany jako domyślny folder sampli. Jest przyciemniony (i pomijany w nawigacji), dopóki nie zostaną wczytane dane sampla, więc świeżo wczytanemu samplowi można nadać ścieżkę pliku. Ścieżki dłuższe niż 255 znaków są odrzucane.
  - Znacznik `*` przed nazwą pliku oznacza, że sampel w RAM różni się od pliku na dysku (ustawia go każda operacja przetwarzania, a Save i Save As go czyszczą). Znacznik jest tylko na czas sesji: nie jest zapisywany z projektem i resetuje się przy ponownym wejściu na ekran. Jeśli kilka instrumentów odwołuje się do tego samego pliku WAV, zapis z jednego nadpisuje plik dla wszystkich.
- Na ekranie instrumentu Sample użyj **EDIT + [LEFT/RIGHT]**, aby wczytać poprzedni lub następny WAV w tym samym folderze.
- **Pitch** transponuje w półtonach (od `-48` do `+48`).
- **Loop** wybiera Off, Loop lub Ping-Pong.
- **Speed** steruje granularnym rozciąganiem w czasie od `0%` do `500%` (`100%` to normalna wartość).

Nieobsługiwane formaty WAV wyświetlają błąd. Przed importem skonwertuj nietypowe pliki do WAV `PCM8`, `PCM16` lub `PCM24`.

#### 2xSCWF

2xSCWF to podwójny oscylator przebiegów single-cycle. Wczytaj do każdego oscylatora 1 mono WAV zawierający dokładnie 1 okres. Oba oscylatory odczytują w przód i zawijają się na końcu cyklu.

Mix płynnie przechodzi między A i B.

Detune jest drobny i wykładniczy od unisona do `+200 ct`, a następnie przesuwa się w krokach półtonowych od `+3 st` do `+24 st`.

2xSCWF traktuje każdy plik jako tabelę 1-cyklową i odczytuje ją dokładnie raz na cykl oscylatora. Pozwala to miksować i detunować 2 przebiegi single-cycle.

Fabryczny folder `waveforms/AKWF/` zawiera przebiegi z biblioteki Adventure Kid. Wiele innych jest dostępnych online, jeśli chcesz rozszerzyć kolekcję.

#### BYOWTBL

BYOWTBL to podwójny oscylator wavetable zgodny z tablicami Serum. Każdy oscylator wczytuje mono WAV zawierający kolejne ramki single-cycle. `Pos A` i `Pos B` przeszukują swoje tablice niezależnie od `00` do `FF`. Silnik interpoluje liniowo wewnątrz każdej ramki i między sąsiednimi ramkami. `Mix` płynnie przechodzi między oscylatorami, a `Detune` przesuwa oscylator B.

## 6. Modulacja i nagrywanie ruchu

Każdy instrument może mieć do 4 slotów modulacji. Instrumenty audio mogą celować w **Instrument Pan** lub **Track Pan**; oba używają `00` dla lewej, `80` dla środka i `FF` dla prawej.

Dostępne typy modulacji to:

- ADSR - klasyczna obwiednia attack-decay-sustain-release
- AHD - obwiednia attack-hold-decay
- LFO - oscylator niskiej częstotliwości z kilkoma kształtami
- SLFO - LFO z mnożnikiem okresu dla wolnych cykli
- FLFO - LFO o częstotliwości audio, które może sięgać zakresu kHz; FLFO może mocno obciążać CPU
- SLIN - liniowe sterowanie joystickiem
- SRAT - sterowanie joystickiem oparte na szybkości zmian

Lista celów modulacji zależy od typu instrumentu.

Dla celu `Cutoff` Amount jest wykładnicze z odpowiedzią kwadratową: 25% osiąga 4,5 półtonu, 50% osiąga 18 półtonów, 75% osiąga 40,5 półtonu, a 100% osiąga sześć oktaw. Dzięki temu zachowana jest precyzyjna kontrola przy małych wartościach, a głębokie przemiatania pozostają możliwe.

Gdy obwiednia ADSR lub AHD celuje w Volume, staje się obwiednią głośności instrumentu. LFO celujące w Volume zamiast tego przesuwa bieżącą głośność wyjściową.

Wartość modulacji może być dodatnia lub ujemna. Jej zakres zależy od celu. Jeśli pełny zakres celu wynosi `127` lub mniej, jak w przypadku większości parametrów, wartość określa zakres bezwzględny. Szersze cele, takie jak wysokość dźwięku, używają wartości skalowanej.

Wszystkie parametry czasu trwania, w tym attack, hold, decay i okres, są mierzone w tickach.

Kształty LFO:

- Tri - bipolarny przebieg trójkątny
- Sin - bipolarny przebieg sinusoidalny
- UniTri - unipolarny przebieg trójkątny
- UniSin - unipolarny przebieg sinusoidalny
- RampDn - liniowe opadanie (przebieg piłokształtny)
- RampUp - liniowe narastanie (przebieg piłokształtny)
- ExpDn - wykładnicze opadanie (wykładniczy przebieg piłokształtny)
- ExpUp - wykładnicze narastanie (wykładniczy przebieg piłokształtny)
- Square - przebieg prostokątny
- Random - losowy przebieg sample-and-hold
- Wavetable - odczytuje jedną z 32-krokowych tablic falowych AY projektu. Wartości od `00` do `0F` są odwzorowywane od `-100%` do `+100%` bez interpolacji.

Typy wyzwalania LFO:

- Free - restartuje LFO tylko po zmianie instrumentu
- Retrig - restartuje LFO przy każdej nucie
- Phrase - restartuje LFO i SLFO, gdy odtwarzanie wchodzi w nowy phrase
- Chain - restartuje LFO i SLFO, gdy odtwarzanie wchodzi w nowy chain
- Hold - zatrzymuje się po 1 cyklu i utrzymuje ostatnią wartość
- Once - zatrzymuje się po 1 cyklu i wraca do zera

### Nagrywanie ruchu (Motion recording)

- **STICK LIVE** (domyślnie `L1`) stosuje modulację drążkami bez zmiany phrase'a. W **Settings > Stick live mode** `HOLD` (domyślnie) włącza ją, gdy przycisk jest przytrzymany; `TOGGLE` włącza ją jednym naciśnięciem i wyłącza następnym; `FREE` zostawia ją włączoną na stałe. Zwolnienie przycisku w trybie `TOGGLE` pozostawia ją włączoną.
- Podczas odtwarzania przytrzymaj **MOTION RECORD** (`L2`), aby zastosować modulację drążkami i nagrać zmienione cele jako bezwzględne wartości FX.
- Przytrzymaj **MOTION ERASE** (`R2`), aby usunąć pasujące FX celów z bieżącego wiersza.

`~` w prawym dolnym rogu oznacza aktywny Stick live. Record (`*`, lub `!` przy przepełnieniu) i Erase (`x`) mają pierwszeństwo we wskaźniku i w obu trybach pozostają chwilowe. Oba włączają przetwarzanie drążków na żywo, dopóki są przytrzymane; ich zwolnienie pozostawia osobno włączony przełącznik live nietknięty.

Tryb jest zapisywany z ustawieniami aplikacji, ale przełącznik live zawsze startuje jako wyłączony. Zmiana trybu czyści przełącznik: `HOLD` podąża za każdym aktualnie przytrzymanym przyciskiem Stick live, `TOGGLE` czeka na nowe naciśnięcie, a `FREE` jest aktywny od razu.

Nagrywanie ruchu zapisuje FX ścieżki w aktualnie odtwarzanym phrase'ie. Najpierw aktualizuje pasujące FX, a potem używa pustych slotów od prawej do lewej. Nigdy nie nadpisuje innego FX. Jeśli wszystkie 3 sloty są zajęte, ten ruch nie jest nagrywany na danym kroku. `!` w prawym dolnym rogu oznacza, że zmieniło się więcej celów, niż mogły pomieścić 3 kolumny FX.

Nagrywanie ruchu obsługuje cele Braids, Plaits, Sampler, 2xSCWF, BYOWTBL i Bogie, w tym aktywne makra Bogie i kontrolki filtra tam, gdzie mają zastosowanie.

## 7. Tables

Tabele są podstawowym narzędziem projektowania dźwięku w trackerach. W terminach Vortex Tracker łączą instrumenty i ornamenty, ale potrafią znacznie więcej. Jeśli znasz LSDj lub NerdSEQ, idea powinna być już znajoma.

Kolumna Pitch przyjmuje względne (`~`) lub bezwzględne (`=`) wartości wysokości dźwięku w półtonach. Kolumna Volume ma zakres `00`-`0F`, działa w każdym silniku na wierzchu jego obwiedni i może służyć jako bramka: na przemian `0F` i `00`, a potem pętla z `HOP 00`. 4 tory FX w większości odpowiadają torom w phrase'ie, choć kilka FX zachowuje się w tabelach inaczej.

Umieszczenie FX `TIC` w ostatnim wierszu tabeli ustawia szybkość tej kolumny i zastępuje domyślną szybkość tabeli instrumentu. Każda kolumna FX może działać z inną szybkością ticków. Kolumny Pitch i Volume podążają za szybkością 1. kolumny FX.

Tabela o 16 wierszach może wydawać się krótka przy długim instrumencie Vortex Tracker, ale FX `HOP` potrafi tworzyć warunkowe pętle w rodzaju "powtórz te wiersze 5 razy", a także pętle zagnieżdżone. Dzięki pętlom i niezależnym szybkościom kolumn FX te 16 wierszy może zajść zaskakująco daleko.

### Tryb retrigger

Pole `Retrig` u góry ekranu Table wybiera, kiedy już aktywna tabela startuje od nowa. `Inst` to domyślny i zgodny tryb: wiersz phrase'a zawierający instrument wybiera domyślną tabelę tego instrumentu i uruchamia ją od wiersza `00`. `Phrase` restartuje aktywną tabelę, gdy odtwarzanie wchodzi w phrase, `Chain` gdy wchodzi w chain, a `Free` zostawia ją działającą, dopóki ścieżka się nie zatrzyma lub wyraźne polecenie jej nie zastąpi.

Tryb strukturalny nigdy nie sięga wstecz w czasie. Jeśli ścieżka nie ma aktywnej tabeli instrumentu, pierwszy wiersz phrase'a, który wprowadza instrument, aktywuje jego tabelę dokładnie w tym wierszu, nawet jeśli jest to wiersz `4`, `8` lub `F` phrase'a. Późniejsze numery instrumentów nie zastępują aktywnej tabeli `Phrase`, `Chain` ani `Free`. Dzięki temu tabele nadają się do automatyzacji podobnej do LFO bez po cichu symulowanych wcześniejszych kroków phrase'a.

`TBL`, `TBX` i `RET` to zawsze wyraźne akcje: wybierają lub restartują natychmiast, niezależnie od trybu. Na granicy Phrase lub Chain restart następuje przed odczytaniem nowego wiersza phrase'a, więc `TBL` lub `TBX` w tym wierszu ma ostatnie słowo. FX wiersza `00` tabeli wykonują się więc ponownie normalnie; FX kumulatywne zachowują swoje zwykłe narastanie.

Tabele `00-7F` są zarezerwowane dla domyślnych tabel instrumentów. Tabele `80-FE` przeznaczone są dla tabel pomocniczych uruchamianych efektem `TBX`. Niższy zakres działa też dla tabel pomocniczych, ale użycie go w ten sposób może powodować nieoczekiwane konflikty i zamieszanie.

### Kopiowanie FX między Phrase i Table

Zaznacz tylko kolumny nazwy/wartości FX, a następnie skopiuj lub wytnij je z Phrase i wklej do dowolnego toru FX w Table, albo odwrotnie. Kopia zachowuje dokładnie zaznaczone komórki, więc zaznaczenie samych poleceń lub samych wartości pozostaje częściowe. Wklejanie zaczyna się od bieżącej komórki FX i jest przycinane na wierszu `F` oraz ostatnim torze FX miejsca docelowego. Nuty, numery instrumentów, wysokość dźwięku i głośność nigdy nie przechodzą między tymi dwoma ekranami.

### Sterowanie

- **EDIT + [UP/DOWN]** w kolumnie nazwy FX: otwiera ekran wyboru FX
- **OPT + [DIRECTION]**: przechodzenie między tabelami

## 8. Inne ekrany

### Instrument Pool

Instrument Pool pokazuje każdy instrument w projekcie i pozwala zmieniać ich kolejność. Podczas odtwarzania pokazuje też, które instrumenty są aktualnie aktywne.

#### Sterowanie

- **EDIT**: edytuje instrument i przechodzi do ekranu Instrument
- **SHIFT + OPT**: kopiuje instrument
- **SHIFT + EDIT**: wkleja instrument
- **EDIT + [UP/DOWN]**: zmienia kolejność instrumentów
- **EDIT + PLAY**: podsłuch instrumentu

### AY Wavetable

Wavetable to typ programowego oscylatora AY Plus. Projekt może zawierać do 256 tablic falowych, każda z 32 krokami. Wszystkie instrumenty wavetable dzielą ten sam zestaw przebiegów. Na tym ekranie je edytujesz.

Poziomy głośności AY są nieliniowe, więc przebieg widoku Standard jest rysowany tak, aby odpowiadał rzeczywistym poziomom wyjściowym. Dwukrotnie stuknij **OPT**, aby przełączyć między widokiem Standard a zapamiętanym **widokiem LFO**, używając tego samego skrótu co w Song/Live Mode: widok LFO rysuje surowe kroki `0`-`15` liniowo, z linią środkową między `7` a `8` jako punktem zerowym modulacji.

Ekran ma 2 wiersze logiczne. Górny wiersz zawiera przyciski **Load** i **Save**, a 2. zawiera edytor wavetable.

#### Sterowanie

- **OPT + [LEFT/RIGHT]**: poruszanie się po liście wavetable
- **OPT + [UP/DOWN]**: poruszanie się po liście co 16 przebiegów
- **SHIFT + OPT**: kopiuje wavetable
- **SHIFT + EDIT**: wkleja wavetable
- **OPT + PLAY**: przełącza między widokiem Standard i LFO

## 9. Tracker FX

FX ścieżki wymuszają wartości sekwencera lub silnika dźwięku na poszczególnych krokach. Inne groovebox'y często nazywają to "parameter locks". Każdy wiersz Phrase ma 3 sloty FX.

Każdy FX ma 3-literowe polecenie i wartość szesnastkową. Panel pomocy w aplikacji podaje krótki opis wybranego polecenia.

### Automatyzacja skali

`SCL XY` zmienia globalną skalę i dźwięk podstawowy odtwarzania bez zmiany nut zapisanych w phrase'ach. `X` wybiera skalę (`0` Chromatic, `1` Major, `2` Minor, aż do `C` Custom); `Y` wybiera dźwięk podstawowy (`0` C do `B` B). Polecenie dotyczy tylko FX phrase'ów, nie tabel. Jeśli kilka ścieżek wyda `SCL` w tym samym ticku, wygrywa ścieżka o najniższym numerze.

`SCL` należy do trybu Quantizer. Przy aktywnym **Note Lock** polecenie nie jest oferowane na liście FX, a każde `SCL` już zapisane w phrase'ach jest ignorowane, dopóki tryb jest włączony.

### Akordy

`CRD XY` zamienia nutę we własnym wierszu Phrase w akord. `Y` wybiera: `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9 lub `F` Min9. `X` wybiera jedno z 16 rozmieszczeń głosów: `0-3` to rosnący cykl zamkniętych odwróceń; `4-7` dodają drop-2; `8-B` dodają drop-3; a `C-F` łączą oba dropy. Cykl jest kontynuowany przez dodatkową oktawę dla trójdźwięków i power chordów, więc każda wartość jest inna; większe wartości stają się coraz szersze i mniej konwencjonalne. Na przykład `CRD 4Y` to drop-2 w pozycji zasadniczej, a `CRD DY` to pierwsze odwrócenie z oboma dropami. Na dole lub na górze tablicy wysokości głosy są ograniczane do jej zakresu. Polecenie nie jest trwałe: zapisz je w każdym wierszu z akordem. Każda wygenerowana nuta jest kwantyzowana niezależnie, gdy Scale jest aktywna, zarówno w trybie Quantizer, jak i Note Lock. Jest dostępne tylko dla silników programowych; instrumenty AY je ignorują i pozostają monofoniczne.

### Sequencer FX

| FX | Wartość | Szczegółowe działanie |
|---|---|---|
| `ARP` | `XY` | Arpeggiuje nutę podstawową, kroki `+X` i `+Y`. `37` daje wzór akordu molowego. |
| `ARC` | `XY` | `X` wybiera kierunek arpeggia lub tryb zakresu; `Y` to jego szybkość w tickach. |
| `PVB` | `XY` | Vibrato wysokości: `X` to szybkość, a `Y` głębokość. W trybie Linear głębokość używa kroków `10-cent`. |
| `PBN` | ze znakiem `XX` | Dodaje `XX` jednostek wysokości co wiersz phrase'a/tabeli; `FF` oznacza `-1`. Użyj `00`, aby zatrzymać. |
| `PSL` | `XX` ticków | Przesuwa płynnie od poprzedniej wysokości do nowej nuty w ciągu `XX` ticków. |
| `PIT` | ze znakiem `XX` | Skumulowane przesunięcie względne w krokach tablicy wysokości. |
| `FIN` | ze znakiem `XX` | Skumulowane drobne przesunięcie w centach przy Linear pitch, w przeciwnym razie w jednostkach okresu. |
| `PRD` | ze znakiem `XX` | Skumulowane względne przesunięcie okresu oscylatora. |
| `VOL` | ze znakiem `XX` | Skumulowane względne przesunięcie głośności (`FF` to -1, a nie pełna głośność). |
| `VSL` | ze znakiem `XX` | Dodaje `XX` do głośności w każdym wierszu phrase'a/tabeli. Użyj `00`, aby zatrzymać. |
| `PAN` | `00-FF` | Bezwzględna panorama instrumentu: lewo, środek (`80`), prawo. |
| `TPN` | `00-FF` | Bezwzględna panorama ścieżki miksera: lewo, środek (`80`), prawo. |
| `RET` | `XY` | Wyzwala ponownie co `Y` ticków; `X` stosuje zmianę głośności. `Y=0` zatrzymuje ponowne wyzwalanie. |
| `DEL` | `XX` ticków | Opóźnia note-on. Opóźnienie dłuższe niż bieżący krok groove pomija nutę. |
| `OFF` | `XX` ticków | Wysyła note-off po `XX` tickach i wchodzi w fazę release ADSR. |
| `KIL` | `XX` ticków | Twardo ubija głos po `XX` tickach bez uruchamiania release ADSR. |
| `TIC` | `XX` ticków | Ustawia ticki tabeli na wiersz. W tabeli zmienia szybkość tej kolumny FX. |
| `TBL` | `00-FE`, `FF` wyłącza | Natychmiast zastępuje i restartuje tabelę instrumentu; `FF` ją zatrzymuje. |
| `TBX` | `00-FE`, `FF` wyłącza | Natychmiast uruchamia lub zastępuje tabelę pomocniczą; `FF` ją zatrzymuje. |
| `THO` | wiersz `XX` | Przeskakuje wszystkimi kolumnami tabeli instrumentu do wiersza `XX`. |
| `TXH` | wiersz `XX` | Przeskakuje wszystkimi kolumnami tabeli pomocniczej do wiersza `XX`; nie jest używane z wnętrza tabeli. |
| `GRV` | groove `XX` | Wybiera groove dla bieżącej ścieżki. |
| `GGR` | groove `XX` | Wybiera groove dla każdej ścieżki. |
| `HOP` | `XY` | Skacze do wiersza `Y`, `X` razy; `X=0` zapętla bez końca. HOP w tabeli działa na własną kolumnę. |
| `SNG` | ze znakiem `XX` | Przesuwa odtwarzanie o `XX` wierszy Song. |
| `PRO` | `00-64` | Wylicza bezwzględne prawdopodobieństwo wyzwolenia od `0%` do `100%`. |
| `MOD` | `AB` | Wyzwala w przebiegu `A` cyklu o `B` przebiegach. |
| `SPD` | ze znakiem `XX` | Wybiera trwały stosunek zegara dla ścieżki; zob. tabelę szybkości poniżej. |
| `SLE` | `00-FF` ticków | Ustawia trwały czas glide dla ścieżki dla ciągłych FX silnika. `00` oznacza natychmiast. Ustawienie wraca do `00`, gdy odtwarzanie się zatrzymuje. |

### Audio FX sends

| FX | Wartość | Szczegółowe działanie |
|---|---|---|
| `RSN` | `00-FF` | Ustawia wysyłkę do reverb tej ścieżki do następnego wyzwolenia nuty. |
| `DSN` | `00-FF` | Ustawia wysyłkę do delay tej ścieżki do następnego wyzwolenia nuty. |

### ADSR / Trigger FX

| FX | Wartość | Szczegółowe działanie |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Nadpisują attack, decay, sustain, release lub kształt obwiedni. |
| `TDC` / `TCL` | `00-FF` | Nadpisują Decay lub Color w trybie Trigger w Plaits i Plaits-Alt. |

#### Warunki

Zainspirowany szwedzkimi "trig conditions", tracker obsługuje prawdopodobieństwa wyzwalania i warunki modulo.

| FX | Wartość | Znaczenie |
|---|---|---|
| `PRO` | `00-64` | Bezwzględne prawdopodobieństwo wyzwolenia od `0%` do `100%` |
| `MOD` | `AB` | Wyzwala w iteracji `A` z `B`; na przykład `12`, `22`, `14`, `34` |

Liczniki `MOD` są lokalne dla ścieżki i phrase'a. Nieprawidłowe kombinacje, takie jak `A` większe od `B`, nie wyzwalają.

`MOD34` wyzwala w 3. przebiegu każdego cyklu o 4 przebiegach. `MOD1F` wyzwala w 1. przebiegu każdego cyklu o 16 przebiegach.

#### Szybkość odtwarzania

Podobnie jak NerdSEQ, ChooChooTracker obsługuje niezależną szybkość odtwarzania dla każdej ścieżki.

`SPD` odczytuje swoją wartość jako bajt ze znakiem. `00` to normalna szybkość, wartości dodatnie przyspieszają ścieżkę, a ujemne ją zwalniają. Ustawienie pozostaje aktywne, dopóki inne polecenie `SPD` go nie zmieni.

| Wartość | Szybkość |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

Starsze projekty bez obsługi `SPD` ze znakiem zachowują swoje pierwotne mapowanie `00-10`, więc nadal brzmią tak, jak zostały zapisane.

Bardzo wysokie mnożniki mogą przekroczyć rozdzielczość planisty ticków trackera i nie były testowane na sprzęcie.

### Modulation FX

FX phrase'ów i tabel mogą zmieniać slot modulacji bez edytowania instrumentu:

| Wzór FX | Znaczenie |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Względne przesunięcie Amount dla slotów modulacji 1-4 |
| `M11`, `M12`, `M13`, `M14` | Względne przesunięcia P1-P4 dla slotu modulacji 1 |
| `M21`, `M22`, `M23`, `M24` | Względne przesunięcia P1-P4 dla slotu modulacji 2 |
| `M31`, `M32`, `M33`, `M34` | Względne przesunięcia P1-P4 dla slotu modulacji 3 |
| `M41`, `M42`, `M43`, `M44` | Względne przesunięcia P1-P4 dla slotu modulacji 4 |

Wartość jest interpretowana jako względna zmiana `8-bit` ze znakiem (`01` dodaje `1`, `FF` odejmuje `1`). Powtarzane polecenia kumulują się. Wartości efektywne są ograniczane do ich prawidłowego zakresu.

`P5` nie jest udostępniane jako FX phrase'a ani tabeli. Jest dostępne jako cel modulacji `M1–M4 Wavetable`, więc jeden modulator może przeszukiwać indeks AYWavetable innego.

| Typ modulacji | P1 | P2 | P3 | P4 | P5, gdy Shape to Wavetable |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Nieużywane |
| AHD | Attack | Hold | Decay | Nieużywane | Nieużywane |
| LFO | Shape | Trigger mode | Period | Nieużywane | Indeks wavetable AY |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | Indeks wavetable AY |
| FLFO | Shape | Trigger mode | Frequency (`1 Hz` do `20 kHz`) | Nieużywane | Indeks wavetable AY |

### Braids FX

| FX | Wartość | Znaczenie |
|---|---|---|
| `BMD` | `00-2E` | Model Braids |
| `BTM` | `00-FF` | Bezwzględny znormalizowany Timbre |
| `BCL` | `00-FF` | Bezwzględny znormalizowany Color |
| `BCF` | `00-FF` | Wykładniczy cutoff, od `20 Hz` do `20 kHz` |
| `BRS` | `00-FF` | Wykładniczy rezonans filtra |

### aChChid FX

| FX | Wartość | Znaczenie |
|---|---|---|
| `ASL` | `00-FF` | Płynne przejście do nuty bez ponownego wyzwalania. `00` to `60 ms`; wyższe wartości wydłużają glide. |
| `ADC` | `00-FF` | Nadpisanie decay: od `200 ms` do `2 s`. |
| `AAC` | `00-FF` | Wielkość akcentu: od zera do maksimum. |
| `ATM` | `00-FF` | Nadpisanie Timbre Braids; aktywne tylko w trybie fali Braids. |
| `ACL` | `00-FF` | Nadpisanie Color Braids; aktywne tylko w trybie fali Braids. |
| `ACF` | `00-FF` | Wykładniczy cutoff filtra 303, od `20 Hz` do `20 kHz`. |
| `ARS` | `00-FF` | Rezonans filtra 303, od zera do maksimum. |
| `AEM` | `00-FF` | Modulacja obwiednią filtra 303, od zera do maksimum. |

### Bogie FX

| FX | Wartość | Znaczenie |
|---|---|---|
| `DMD` | `00-0B` | Wybiera model Bogie do następnego wyzwolenia. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Nadpisują Decay, Tone, Sweep, Noise, FM lub Drive do następnego wyzwolenia. Wszystkie sześć makr jest dostępnych w każdym modelu. |
| `DCF` | `00-FF` | Cutoff filtra, mapowany logarytmicznie. |
| `DRS` | `00-FF` | Rezonans filtra. |

### Sintered FX

| FX | Wartość | Znaczenie |
|---|---|---|
| `SMD` | `00-05` | Wybiera model Sintered do następnego wyzwolenia. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Nadpisują Decay, Mod, pierwsze dwa makra modelu, Motion lub ostatnie makro modelu do następnego wyzwolenia. |
| `SCF`, `SRS` | `00-FF` | Cutoff i rezonans filtra. |

### MIDI Out FX

| FX | Wartość | Znaczenie |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Wysyła MIDI CC, przeskalowane do `0-127`. To, na jaki numer CC wysyła każdy slot, jest ustawiane dla każdego instrumentu (`MC1 number`-`MC4 number` na ekranie instrumentu), a nie przez sam FX - slot bez ustawionego numeru jest nieaktywny. |

### Plaits FX

| FX | Wartość | Znaczenie |
|---|---|---|
| `PMD` | `00-17` | Silnik Plaits |
| `PHA` | `00-FF` | Bezwzględny znormalizowany Harmonics |
| `PTM` | `00-FF` | Bezwzględny znormalizowany Timbre |
| `PMO` | `00-FF` | Bezwzględny znormalizowany Morph |
| `PAX` | `00-FF` | Miks Main/Aux: `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Wykładniczy cutoff, od `20 Hz` do `20 kHz` |
| `PRS` | `00-FF` | Wykładniczy rezonans filtra |

### Sample FX

| FX | Wartość | Znaczenie |
|---|---|---|
| `SPL` | `00-03` | Tryb odtwarzania: `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Odtwarza slice o podanym numerze (`01` = pierwszy slice) niezależnie od nuty; `00` zachowuje zwykłe mapowanie nut. Wymaga włączonego trybu slice w instrumencie |
| `SPT` | ze znakiem `XX` | Transpozycja sampla w półtonach |
| `SST` | `00-FF` | Znormalizowany początek odtwarzania |
| `STA` | `00-FF` | Alias `SST` (znormalizowany początek odtwarzania) |
| `SEN` | `00-FF` | Znormalizowany koniec odtwarzania |
| `SVL` | `00-FF` | Bezwzględna głośność sampla |
| `SCF` | `00-FF` | Wykładniczy cutoff, od `20 Hz` do `20 kHz` |
| `SRS` | `00-FF` | Wykładniczy rezonans filtra |
| `SSP` | `00-FF` | Szybkość sampla, mapowana od `0%` do `500%` |

### AY FX wspólne dla instrumentów AY

| FX | Wartość | Szczegółowe działanie |
|---|---|---|
| `AYM` | `XY` | `X` to kształt obwiedni; `Y` wybiera wyłączone/ton/szum/ton+szum (`0/1/2/3`). |
| `NOI` | ze znakiem `XX` | Skumulowane względne przesunięcie okresu szumu. |
| `NOA` | `00-1F`, `FF` | Bezwzględny okres szumu; `FF` oddaje priorytet okresu szumu wcześniejszym ścieżkom. |
| `ERT` | dowolna | Ponownie wyzwala bieżący sprzętowy kształt obwiedni. |
| `EAU` | `XY` | Automatyczny stosunek obwiedni `X:Y`; `X=0` ją wyłącza. |

### FX tylko dla AY Classic

| FX | Wartość | Szczegółowe działanie |
|---|---|---|
| `EVB` | `XY` | Vibrato okresu obwiedni: `X` szybkość, `Y` głębokość. |
| `EBN` | ze znakiem `XX` | Dodaje `XX` do okresu obwiedni co wiersz phrase'a/tabeli. |
| `ESL` | `XX` ticków | Przesuwa płynnie od poprzedniego okresu obwiedni do nowej wartości. |
| `ENT` | nuta `XX` | Ustawia okres obwiedni na podstawie nuty z tablicy wysokości pokazanej w UI. |
| `EPT` | ze znakiem `XX` | Skumulowane względne przesunięcie okresu obwiedni. |
| `EPL` | bajt `XX` | Ustawia młodszy bajt okresu obwiedni. |
| `EPH` | bajt `XX` | Ustawia starszy bajt okresu obwiedni. |

### AY Plus FX

| FX | Wartość | Szczegółowe działanie |
|---|---|---|
| `TNN` | nuta `XX` | Ustawia oscylator tonowy na nutę z tablicy wysokości, ignorując nutę wiersza. |
| `TNP` / `TNF` | ze znakiem `XX` | Skumulowane przesunięcie kroku wysokości / drobne przesunięcie tonu. |
| `TRT` | dowolna | Ponownie wyzwala fazę oscylatora tonowego tam, gdzie jest to obsługiwane. |
| `ENN` | nuta `XX` | Ustawia oscylator obwiedni na nutę z tablicy wysokości. |
| `ENP` / `ENF` | ze znakiem `XX` | Skumulowane przesunięcie kroku wysokości / drobne przesunięcie obwiedni. |
| `SFT` | `00-07` | Typ programowy: None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | nuta `XX` | Ustawia oscylator programowy na nutę z tablicy wysokości. |
| `SFP` / `SFF` | ze znakiem `XX` | Skumulowane przesunięcie kroku wysokości / drobne przesunięcie oscylatora programowego. |
| `SRT` | dowolna | Resetuje fazę oscylatora programowego. |
| `SFM` | ze znakiem `XX` | Skumulowane przesunięcie głębokości FM. |
| `PWM` | ze znakiem `XX` | Skumulowane przesunięcie szerokości impulsu. |
| `SPL` | ze znakiem `XX` | Skumulowane przesunięcie niskiego poziomu impulsu. |
| `SWT` | ze znakiem `XX` | Skumulowane przesunięcie indeksu wavetable. |

### AY Sample FX

AY Sample akceptuje wspólne polecenia AY oraz `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` i `SFF`, jak opisano powyżej.

| FX | Wartość | Szczegółowe działanie |
|---|---|---|
| `SMS` | `00-FF` | Ustawia początek odtwarzania starszego AY Sample na `XX x 64` próbek źródłowych. |

FX parametrów silnika dotyczą pasującego typu instrumentu i są resetowane przy następnym wyzwoleniu nuty, o ile nie zaznaczono inaczej. `SPD` jest celowo trwałe.

Selektor FX filtruje tę referencję do poleceń wspólnych oraz grupy obsługiwanej przez aktywny instrument. Kontekstowy panel pomocy podaje szczegóły wartości podczas edycji.

## 10. Mixer, Reverb i Delay

Mixer to skrajnie lewy ekran główny. Wyświetla obciążenie CPU i ostrzeżenia o przesterowaniu. Czerwony `!` w kolumnie **CLIP** oznacza ścieżkę, której suchy sygnał wypchnął miks poza bezpieczny zakres.

Używaj go do wyważania 8 ścieżek, kształtowania każdej ścieżki za pomocą Tilt EQ oraz wysyłania audio do reverb lub delay:

| Pole | Znaczenie |
|---|---|
| LVL | Poziom ścieżki po silniku, `000-100` |
| REV | Wysyłka do współdzielonego reverb Clouds, `000-100` |
| DLY | Wysyłka do współdzielonego delay ping-pong, `000-100` |
| TLT | Tilt EQ ścieżki, `00-FF`; `80` jest neutralne, niskie wartości faworyzują bas, a wysokie tony wysokie. Niskie wartości wprowadzają łagodne przesterowanie. |
| M | `*` wycisza tę ścieżkę |
| S | `*` włącza solo tej ścieżki |

Mixer jest oparty na ścieżkach, a nie na instrumentach. Jeśli ścieżka zmienia instrumenty, jej poziom, PAN, Tilt i wysyłki pozostają przypisane do ścieżki. Instrumenty mają też własne **Vol** i **Pan** w zakresie `00-FF` w górnym wierszu ekranu Instrument. PAN instrumentu jest stosowany jako pierwszy, potem PAN ścieżki. Oba zachowują istniejący obraz stereo na środku; przesunięte w lewo lub w prawo stopniowo tłumią przeciwną stronę.

### Master FX

Pozycja **MASTER FX** otwiera dwuslotowe podmenu Insert FX dla całego
miksu. Jego efekty działają po powrotach reverb i delay, przed głośnością
master. Wybierz moduł, edytuj jego parametry lub wyłącz go (bypass) dokładnie tak jak w
insercie ścieżki; naciśnij **OPT**, aby wrócić do Mixer. Ustawienia Master FX są zapisywane
z projektem i nie są automatyzowane przez phrase, tabelę ani modulację.

### Clouds Reverb

Użyj **SELECT + [UP]** z Mixer, aby otworzyć ekran ustawień reverb.

- **Return**: poziom mokrego reverb w miksie master.
- **Time**: sterowanie wybrzmiewaniem/czasem, `00-FF`.
- **Damping**: pochłanianie wysokich częstotliwości, `00-FF`.
- **Filter**: filtr dolnoprzepustowy stosowany przed reverb.

To jest sekcja reverb z Clouds, a nie kompletny granularny procesor Clouds.

### Ping-pong Delay

Użyj **SELECT + [DOWN]** z Mixer, aby otworzyć Delay, a następnie **SELECT + [UP]**, aby wrócić. Reverb działa w przeciwnym kierunku: **SELECT + [UP]** go otwiera, a **SELECT + [DOWN]** wraca do Mixer.

- **Return**: poziom mokrego delay.
- **To Reverb**: ilość mokrego sygnału delay wysyłana do współdzielonego reverb, `0-100%`.
- **Ticks**: czas opóźnienia w tickach trackera (domyślnie `9 ticks` = `1 beat`).
- **Feedback**: ilość sprzężenia krzyżowego, ograniczona do `95%`.
- **Filter**: filtr dolnoprzepustowy w ścieżce opóźnionego sygnału.

1. powtórzenie podąża za wejściem stereo. Późniejsze sprzężenie zwrotne przechodzi krzyżowo między kanałem lewym i prawym.

## 11. Ekran Project

Ekran Project udostępnia polecenia **Load**, **Save**, **New**, **Export**, **Manage** i **Scale** oraz metadane: nazwę pliku, tytuł i autora.

### Lokalizacja eksportu

Eksporty, stemy i bounce'y są zapisywane w folderze każdego projektu wewnątrz istniejącego już katalogu sampli aplikacji:

- **Domyślnie**: `<app folder>/samples/Exports/<project name>/`. Dopóki projekt nie ma nazwy, folderem jest `current-project`.
- W wersji webowej domyślną lokalizacją jest `/user/samples/Exports/<project name>/`.
- Na Androidzie folder sampli znajduje się w prywatnej przestrzeni roboczej aplikacji.

Wiersz **Folder** na ekranie EXPORT pokazuje aktywne miejsce docelowe:

- Stuknij **EDIT**, aby wybrać własny folder w przeglądarce folderów. Wybrany folder jest używany dokładnie tak, jak został wybrany (nie jest dodawany podfolder projektu) i jest zapamiętywany w ustawieniach.
- **EDIT + OPT** (clear) przywraca domyślną lokalizację.

Gdy projekt z nazwą jest zapisywany pod nową nazwą, domyślny folder eksportu jest przemianowywany tak, aby do niej pasować, dzięki czemu wcześniej wyeksportowane pliki zostają przy projekcie. Zmiana nazwy nie dotyczy własnego folderu, a folder nie jest przemianowywany przez autozapis. Jeśli folder o docelowej nazwie już istnieje, oba foldery są zachowywane. Jeśli tracker zgubił folder (na przykład po ponownym uruchomieniu), zapis projektu z nazwą przemianowuje zamiast tego istniejący folder `current-project`. Instrumenty działają dalej po zmianie nazwy: ścieżki wczytanych sampli wskazujące na przemianowany folder są aktualizowane automatycznie.

**Load** akceptuje również pliki `.mid`/`.midi`, importowane jako nowy projekt: nuty są grupowane według kanału MIDI (jeden kanał na ścieżkę, do liczby ścieżek), kwantyzowane do 4 wierszy na beat i umieszczane na jednym domyślnym instrumencie AY - numery programów MIDI nie mają odpowiednika w chiptune, więc potem wybierz prawdziwe instrumenty. Używane jest tylko pierwsze tempo pliku (jedna globalna częstotliwość ticków, bez zmian tempa w poszczególnych sekcjach). Wiersz **MIDI** na ekranie Export robi odwrotnie: zapisuje nuty, głośność oraz tempo/groove bieżącego aranżu jako Standard MIDI File (jedna ścieżka MIDI na ścieżkę trackera); instrumenty [MIDI Out](#midi-out) i inne FX wiersza poza głośnością i globalnym groove nie mają odpowiednika MIDI i nie są tłumaczone.

**Load** akceptuje również utwory Dirtywave M8 (`.m8s`, firmware 2.x-4.x), importowane jako nowy projekt. Przenoszona jest tylko struktura i nuty (wiersze song, chainy z ich transpozycją, phrase'y z nutą, velocity i numerem instrumentu, note-offy, tempo i nazwa utworu); nuty zachowują prawdziwą wysokość dźwięku (M8 nazywa oktawy o dwie wyżej: jego `C-6` to tutaj `C-4`). Każdy instrument używany przez phrase jest importowany pod nazwą instrumentu M8. Instrumenty M8 **MACROSYN** stają się instrumentami Braids (47 kształtów to modele Braids, z tymi samymi numerami; timbre, color, typ filtra, cutoff, rezonans i pan są przenoszone, a krzywa cutoff jest przybliżona). Instrumenty **SAMPLER** stają się instrumentami Sample: `.m8s` przechowuje tylko ścieżkę pliku WAV na karcie M8, więc plik jest wyszukiwany w folderze `.m8s` i jego folderach nadrzędnych (działa kopia układu karty) oraz w bieżącym folderze sampli (działa zarówno `Samples`, jak i `samples`); jeśli nie zostanie znaleziony, instrument zachowuje ścieżkę M8 i milczy. Importowane sample dostają +24 półtony w ustawieniu Pitch, ponieważ M8 odtwarza sampel w jego oryginalnej wysokości o dwie oktawy niżej niż ta aplikacja. **FMSYNTH** staje się instrumentem Genesis FM (zgrubne przybliżenie), **WAVSYNTH** i **HYPERSYNTH** stają się aChChid. **MIDI OUT** staje się instrumentem MIDI Out (kanał, bank, program i pierwsze cztery numery CC). Inne typy (zewnętrzne) stają się domyślnym instrumentem AY - potem wybierz prawdziwe brzmienia. FX i tabele są ignorowane.

Na desktopie wiersz **M8S** ekranu Export robi odwrotnie, w ograniczony sposób: stuknij go i wybierz istniejący `.m8s` jako **szablon** (zapamiętany do zamknięcia aplikacji, **EDIT + OPT** go zapomina). Jego song, chainy, phrase'y, tempo i tytuł są zastępowane, a wszystko inne zostaje bez zmian, z tą różnicą, że instrumenty Braids są zapisywane z powrotem jako MacroSynth, a instrumenty Sample jako Sampler (ścieżka `/Samples/<file name>`: skopiuj tam plik WAV na M8 samodzielnie), pod warunkiem że szablon zawiera instrument tego typu, z którego można skopiować pozostałe parametry. FX nie są eksportowane, a utworu używającego phrase'a `FF` lub wyższego nie można wyeksportować (M8 ma 255 phrase'ów). Wyeksportowany plik nie był jeszcze testowany na prawdziwym M8. Szczegóły w [M8 songs](../m8s-format.md).

Na desktopie key jazz pozwala wpisywać nazwę pliku, tytuł i autora bezpośrednio na klawiaturze zamiast używać klawiatury ekranowej - zob. [Key jazz](#key-jazz-tylko-desktop).

### Scale / Quantize

Ekran Scale steruje globalnym silnikiem skal 12-TET. Wiersz **Mode** wybiera jeden z dwóch trybów:

- **Quantizer** (domyślny): wprowadzanie phrase'ów pozostaje chromatyczne; po włączeniu nuty są zaokrąglane w dół tuż przed dotarciem do silnika dźwięku, więc zmiana dźwięku podstawowego lub skali reharmonizuje utwór bez edytowania jego phrase'ów.
- **Note Lock**: nuty wpisywane w phrase'ach są przyciągane do najbliższej nuty skali w trakcie wprowadzania, więc można zapisać tylko nuty skali. Nowe wpisy i obniżenia przyciągają w dół; `EDIT + RIGHT` / `EDIT + UP` podnoszą nutę do następnej nuty skali powyżej. Wprowadzanie na tym ekranie zawsze podąża za skalą projektu: zmiana dźwięku podstawowego lub skali natychmiast wpływa na nowo wprowadzane nuty na włączonych ścieżkach, a nuty już zapisane pozostają nietknięte. FX `SCL` jest w tym trybie niedostępny, a każdy już zapisany `SCL` jest ignorowany. Kwantyzacja zwykłych nut przy odtwarzaniu jest pomijana (wprowadzanie jest już zablokowane); kwantyzacja akordów przez `CRD` nadal obowiązuje.

Gdy kursor spoczywa na **Mode**, pasek stanu utrzymuje stały opis aktywnego trybu: `Quantizes notes on playback` dla **Quantizer**, `Snaps sequencer to scale` dla **Note Lock**. Przełączenie trybu natychmiast zmienia tekst; każdy komunikat o akcji zastępuje go, dopóki nie wygaśnie.

Oba tryby dzielą ten sam dźwięk podstawowy, skalę, edycję nut **Custom** i pola wyboru ścieżek. Każde z ośmiu pól wyboru ścieżek decyduje, których ścieżek dotyczy tryb. Silnik skal jest pomijany dla tablic wysokości innych niż 12-nutowe oraz dla instrumentów Sampler, których tryb **Slice** to EQUAL, AUTO lub LAZY (ich nuty wybierają slice'y chromatycznie od C-0); wejście MIDI nie jest częścią tej wersji.

- **Linear pitch** wybiera tryb tablicy wysokości. **Off** jest domyślne i jest ustawieniem zwalidowanym na sprzęcie dla poprawnego śledzenia oktaw w AY, Braids i Plaits.
- **Tick rate** ustawia taktowanie trackera i wyświetla odpowiadające mu BPM (`tick rate x 60 / 24`).
- ChooChooTracker zapisuje projekty jako `.cct`. Ten format nie jest zgodny z ChipNomad.

Użyj **Save** przed zmianą typów instrumentów lub wczytaniem innego projektu.

## 12. Settings

- **Repeat delay / speed** regulują powtarzanie przytrzymanych przycisków.
- **Stick live mode** wybiera `HOLD`, `TOGGLE` lub `FREE` dla już zmapowanego przycisku Stick live.
- **MIDI** otwiera podmenu [MIDI](#14-midi): wybór urządzenia i mapowanie kanałów MIDI In na instrumenty.
- **Synths** zawiera **AY Quality**, **Sample dithering** oraz globalne ustawienia **Braids BITS / DRFT / SIGN**.
- **Mixer** zawiera **Mix volume** i **Tilt pivot** dla każdego projektu z zakresu `250-4000 Hz` (domyślnie `1 kHz`), używany przez wszystkie kontrolki Tilt w Mixer.
- **Graphics** zawiera **Edit color theme**, **Load font**, **Persistent waveform**, **Track visuals**, **Waveform FPS** oraz aktywny renderer (`GPU` lub `Software`). **Waveform FPS** ustawia częstotliwość odświeżania oscyloskopu instrumentu i wizualizacji mixera od `01` do `60`. Track visuals wybiera **Detailed** (przebieg syntezatora i jego nakładki) lub **Audio waveform** (rzeczywiste zsumowane wyjście tej ścieżki) niezależnie dla każdej ścieżki; przebieg audio natychmiast czyści każdą wcześniejszą nakładkę ADSR po rozpoczęciu odtwarzania. Czcionki i motywy ChipNomad powinny działać.
- **Key mapping** dostosowuje sterowanie.
- **Support report** zapisuje mały diagnostyczny plik tekstowy z wersją aplikacji, platformą, ustawieniami audio i obciążeniem CPU, stanem odtwarzania/przepełnienia oraz zwięzłym podsumowaniem projektu. Nie zawiera danych sampli ani tekstu tytułu/autora projektu. Na Androidzie i w Web otwiera zwykły przepływ eksportu/pobierania; wersje desktopowe i na konsole przenośne zapisują `support-report.txt` w zapisywalnym folderze aplikacji.
- **Quit ChooChooTracker** kończy działanie w czysty sposób.

**Graphics → Boot screen** wybiera **Original** (domyślnie) lub **Night**. Stuknij EDIT
lub użyj EDIT + kierunek, aby zmienić; wybór jest zapisywany do następnego
ekranu tytułowego. Night używa oryginalnej sceny kolejowej z łagodniejszymi niebiesko-szarymi odcieniami,
proceduralną Drogą Mleczną z subtelnym migotaniem gwiazd i okazjonalnymi spadającymi gwiazdami.
Oryginalny krajobraz i pociąg zachowują swoje przewijane pętle.

## 13. Wydajność i rozwiązywanie problemów

### CPU

Koszt CPU zależy od aktywnych silników i efektów. Modele fizyczne Plaits i Clouds Reverb są cięższe niż podstawowe głosy AY. Sprawdź wydajność na docelowej konsoli, zwłaszcza przy 8 głosach Plaits i obu aktywnych wysyłkach. Odczyt CPU bliski `100%` może powodować trzaski lub chybione terminy audio.

Wskaźnik CPU to wygładzona miara czasu renderowania audio. Nie mierzy opóźnień przed uruchomieniem callbacku audio ani w wyjściu audio urządzenia, więc trzaski mogą występować nawet przy niskim odczycie.

Na Androidzie wyjście audio preferuje teraz AAudio, aby uniknąć sporadycznych trzasków obserwowanych z OpenSL ES na Pixelu 7a. To buforowane wyjście stawia na stabilne odtwarzanie i może dodać pewne opóźnienie reakcji. Płótno trackera zachowuje pełny układ 4:3 na kwadratowych i innych nietypowych proporcjach ekranu; niewykorzystane miejsce jest pokazywane jako ramki zamiast przycinania kolumn. Jeśli trzaski się utrzymują, zgłoś utwór, model telefonu, wersję Androida i to, czy używasz głośnika, słuchawek czy Bluetooth.

Jeśli napotkasz kliknięcia, awarie lub spowolnienia, użyj **Settings > Support report** i dołącz wygenerowany `support-report.txt` do zgłoszenia błędu. Jeśli problem wywołuje konkretny utwór, dołącz także jego `.cct`.

### Brak dźwięku

Sprawdź numer instrumentu, stan wyciszenia lub solo ścieżki, LVL ścieżki, Mix volume aplikacji i obwiednię instrumentu. W przypadku sampli sprawdź, czy oryginalny plik WAV nadal istnieje pod zapisaną ścieżką. W ArkOS naciśnij **MENU + L3**, aby przełączyć wyciszenie systemu operacyjnego. Jeśli to nie pomoże, uruchom ponownie sprzęt i ponownie podłącz interfejs audio.

### Sterowanie działa dziwnie

Dostosuj Repeat delay i Repeat speed w Settings. Jeśli jedno naciśnięcie przesuwa dwa razy, sprawdź, czy do danego kierunku jest przypisana tylko 1 fizyczna kontrolka, i zgłoś dokładny ekran oraz skrót.

## 14. MIDI

Desktop i PortMaster. Settings > **MIDI** otwiera to podmenu: **MIDI In** / **MIDI Out** wybierają podłączone urządzenie, przechodząc przez wykryte porty z `OFF` na obu końcach; oba kierunki wymagają wybranego urządzenia. Wybór urządzenia obowiązuje na czas sesji: nie jest zapisywany w settings.txt, ponieważ pozycja portu na liście może się zmieniać po restartach lub gdy urządzenia są podłączane w innej kolejności. Kontrolka **PLAY** aplikacji pozostaje dostępna z tego ekranu i uruchamia utwór. W PortMaster podłącz urządzenie USB MIDI przed uruchomieniem; musi być udostępnione przez system operacyjny konsoli przez ALSA.

- **Podsłuch dźwięku z klawiatury MIDI**: przy wybranym urządzeniu MIDI In granie nut na podłączonej klawiaturze odsłuchuje instrument na dowolnym ekranie, tak samo jak ekranowy skrót **EDIT + PLAY** na ekranie [Instrument](#5-instruments). To tylko podsłuch - nie wprowadza nut do utworu.
- **Channel mapping**: otwiera listę 16 kanałów MIDI; przypisz każdemu instrument (domyślnie `OFF`), aby nuty odbierane na tym kanale odsłuchiwały ten instrument niezależnie od tego, który jest wybrany na ekranie Instrument - np. kanał `01` -> instrument `05`. Kanał pozostawiony na `OFF` wraca do aktualnie wybranego instrumentu, czyli pierwotnego zachowania. To mapowanie jest zapisywane w settings.txt.
- **MIDI CC mapping**: mapuje przychodzący kontroler na parametr instrumentu, w tym jego **Volume**, lub na kontrolki globalne. Wiersze globalne pokazują `TRK 1`-`TRK 8` zamiast instrumentu i udostępniają dla ścieżki **Mute**, **Solo**, **Volume**, **Reverb send** i **Delay send**; **Song play/stop** nie ma celu. Nowe mapowanie ignoruje CC, które je nauczyło/skonfigurowało, i zaczyna działać przy następnym fizycznym ruchu, więc ustawianie mapowań podczas odtwarzania nie może zresetować patcha do zera. Mute i solo używają `0-63` = wyłączone, `64-127` = włączone.
- **Sterowanie zewnętrznym urządzeniem MIDI**: nadaj ścieżce typ instrumentu [MIDI Out](#midi-out) i ustaw jej Channel; wyzwalanie nut na tej ścieżce wysyła prawdziwe MIDI do wybranego urządzenia MIDI Out zamiast wytwarzać dźwięk w ChooChooTracker.

## 15. Podziękowania i licencje

ChooChooTracker jest forkiem ChipNomad i zachowuje jego podejście do licencjonowania MIT. Kod Braids, Plaits, Plaits-Alt, Clouds DSP, MME DSP wywodzącego się z Warps oraz stmlib wywodzi się z otwartoźródłowych wydań Mutable Instruments na zasadach odpowiednich uwag MIT. Plaits-Alt pochodzi z forka Plaits Lab lylepmills/eurorack; obowiązują zachowane tam uwagi o źródle. Silnik aChChid używa Open303 autorstwa Robina Schmidta, copyright 2009, na licencji MIT. Bogie i Sintered to oryginalne natywne implementacje. Wejście/wyjście MIDI używa RtMidi autorstwa Gary'ego P. Scavone'a, na licencji w stylu MIT. Dokładne informacje o autorstwie znajdziesz w dołączonych plikach licencji.

## Efekty insert ścieżek

Każda z ośmiu ścieżek ma dwa szeregowe sloty, **TF1 → TF2**. Z ekranu Instrument
użyj zwykłego gestu Shift+Up, aby otworzyć MOD, a następnie ponownie Shift+Up dla **F: Insert
FX**. Shift+Down wraca do MOD. OPT+Left/Right wybiera ścieżkę (1-8), nie
zmieniając selektora instrumentu. Nagłówek modułu otwiera istniejącą listę wyboru;
bypass zachowuje moduł, skonfigurowane wartości i automatyzację. Parametry są
wartościami bajtowymi: lewa kolumna 1-4, prawa kolumna 5-8. Tekst kontekstowy pokazuje pełne nazwy i
zdekodowane wartości. Stare projekty wczytują się z obydwoma slotami OFF.

Wyświetlacz nawigacji utrzymuje widoczną pełną kolumnę F-M-I-P na wszystkich
ekranach instrumentu, w tym Insert FX i puli instrumentów, i podświetla
bieżący ekran.

| Moduł | Parametry w kolejności |
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

Kontrolki ciągłe używają 00-FF, w tym poprawnego zera i maksimum FF. Kontrolki
bipolarne mają dokładną neutralną wartość przy 80. Tryby distortion to 00 Density, 01 Drive,
02 Spiral, 03 Mojo, 04 Dyno. Stosunki kompresora 00-07 to 1,5, 2, 3, 4, 6, 8, 16,
20:1; źródła detektora 00-02 to wejście stereo, lewy, prawy. Detector filter 80 jest
neutralny, poniżej jest dolnoprzepustowy, powyżej górnoprzepustowy. Nie ma zewnętrznego
sidechainu ani sidechainu między ścieżkami. Każdy slot może zawierać dowolny moduł; powtórzone moduły są niezależne.
Chorus, Flanger, Phaser i Rotary używają łagodnych domyślnych ustawień modulacji. Rotary Speed
używa pełnego zakresu **00-FF**: **00 = 0,01 Hz** (jeden obrót na 100 sekund),
**FF = 25 Hz** (dawne maksimum Chaos). Wykładnicze rozłożenie daje dokładniejszą
kontrolę nad powolnymi ruchami LFO. Nowe inserty Rotary mają domyślnie **8F**, około
0,8 Hz. To zastępuje cztery dyskretne wartości szybkości; istniejące ustawienia Rotary
i automatyzacja szybkości F11/F21 wymagają ponownego dostrojenia do nowej skali (dawne Slow/Fast/Hyper/
Chaos odpowiadają w przybliżeniu **8F/CB/E7/FF**). Pozostałe parametry bez zmian.
Głębokość bitowa Bitcrusher wynosi od 4 do 16 bitów, a
redukcja częstotliwości od 1x do 32x. Tryby Destruction to Fold, Clip i Crush.
Saturation i distortion zawierają kontrolki poziomu wyjściowego lub miksu, aby zrównoważyć je
z sygnałem nieprzetworzonym.
Parametric EQ ma półkowe podbicia/obniżenia 24 dB przy 60-500 Hz i 2-14 kHz oraz
dzwon o stałym Q 0,9 przy 250 Hz-5 kHz. Jego kontrolki gain są neutralne przy 80; gdy wszystkie
trzy są neutralne, jest to dokładny bypass. Aktywny EQ używa przezroczystego soft knee powyżej
pełnej skali i osiąga maksimum 1,5, aby ograniczyć skrajne podbicia.

Poziom ścieżki i tilt EQ poprzedzają inserty, więc oba wpływają na sygnał wchodzący do
kompresji i distortion. Wszystkie głosy/nuty akordu na ścieżce zasilają jeden łańcuch insertów.
Zarówno wysyłka do reverb, jak i do delay otrzymują jego wyjście. Efekty współdzielone, powroty,
kontrolki master oraz mute/solo zachowują dotychczasowe działanie. Inserty nie przetwarzają
audio z zewnętrznych urządzeń MIDI.

Polecenia insertów używają **F11-F18** dla TF1 i **F21-F28** dla TF2. Wybór FX w rolce
oraz okno FX Phrase/Table oferują tylko parametry używane przez skonfigurowane inserty
wybranej ścieżki. Okno grupuje je jako **TF1: nazwa efektu** i
**TF2: nazwa efektu**; sloty OFF są ukryte. Obejścione (bypass) inserty pozostają edytowalne.
W oknie FX Phrase i Table opis podąża za slotem inserta wybranej ścieżki: pokazuje moduł, nazwę parametru, zdekodowane końce zakresu i
zapisaną wartość bazową. Istniejące polecenia utworu są zachowywane, jeśli zmiana modułu
czyni je nieużywanymi; otwarcie ich selektora zaczyna od dostępnej kontrolki.
Podczas edycji wartości wskazówka na dole pokazuje jej zdekodowaną wartość i jednostki.
Dowolna kolumna FX w Phrase lub Table może adresować oba sloty; siatka Phrase zachowuje swoje
trzy kolumny. Te bezwzględne wartości czasu wykonania utrzymują się przez nuty, instrumenty,
phrase'y, chainy i zwykłe pętle. Nowe odtwarzanie/twarde zatrzymanie je resetuje. Edycja
wartości bazowej na stronie F czyści tylko nadpisanie tego parametru. MOD działa na
efektywną wartość bazową/automatyzacji bez zapisywania jej przesunięć. Wybór nowego
modułu przywraca wartości domyślne i czyści nadpisania tego slotu. Bypass zachowuje
istniejącą automatyzację, zawieszając przetwarzanie po krótkim fade'zie.

**Adresy odnoszą się do slotów ścieżki/pozycji parametrów, a nie do nazwanych efektów.** Phrase
użyty ponownie na innej ścieżce używa modułów tej ścieżki. Zmiana modułów może
zmienić znaczenie polecenia; polecenia nie są przepisywane. Adresy OFF/nieużywane
są bezpiecznymi no-opami i pozostają nietknięte w zapisanych danych Phrase/Table.

Lista wyboru MOD instrumentu zyskuje kategorię Insert FX. Źródła każdego instrumentu
sterują insertami na ścieżce, która go odtwarza, w tym połączonym sygnałem akordów.
Ten sam instrument na dwóch ścieżkach wpływa na niezależne łańcuchy. Obowiązują istniejące
taktowanie źródeł, wartości ze znakiem, reguły łączenia źródeł, tryby drążków i nagrywanie
ruchu. Kontrolki dyskretne są kwantyzowane i ograniczane do prawidłowych wyborów. Nie ma
nowego banku modulacji należącego do ścieżki.

Makeup kompresora zaczyna się od 0 dB. OTT zaczyna od około 25% depth, 25%
upward, 50% downward, 0 dB input i -3 dB output. Jego progi pasm, wzmocnienia i
częstotliwości podziału pozostają na natywnych ustawieniach domyślnych Rui-727; Depth to jego kontrolka wet/dry.
TAPESCAM zachowuje natywne pomocnicze ustawienia domyślne: nowa/szybka taśma, szum i
kompresja wyłączone, poszerzanie włączone. Obserwuj istniejące wskaźniki clippingu, dodając
drive/gain. Przejścia slotów używają ograniczonych fade'ów 5 ms przez sygnał suchy.

Zapisy przechowują identyfikatory modułów, bypass i skonfigurowane bajty w opcjonalnych, wersjonowanych
sekcjach `Track inserts: 1,8,2` i `Master inserts: 1,2`. Nadpisania czasu wykonania
i historie DSP nie są zapisywane.
Zachowaj kopie zapasowe, zanim otworzysz zapisy z insertami w starszych buildach. Źródła DSP,
rewizje źródeł i adaptacje znajdują się w `chipnomad_lib/external/insert_fx/SOURCES.txt`;
pełne uwagi MIT są dostarczane w `licenses/INSERT_FX.txt`.

Na R36H przy 48 kHz zmierzona para insertów na jednej ścieżce zajmowała około 4-5%
czasu rzeczywistego dla Compressor, 6% dla Distortion, 18% dla Doubler, 16% dla TAPESCAM
i 24% dla OTT, przed kosztem syntezatorów i efektów współdzielonych. Szesnaście aktywnych
instancji Doubler, TAPESCAM lub OTT przekroczyło czas rzeczywisty. Modulacja o częstotliwości audio dodaje
kolejny koszt. Zacznij od kilku insertów, obserwuj przeciążenie audio i wyłączaj (bypass)
nieużywane sloty; szesnaście dostępnych pozycji nie jest gwarantowanym budżetem CPU.

Strona Insert utrzymuje wskazówkę wybranego pola widoczną po zwolnieniu przycisku;
tymczasowe komunikaty nadal mogą ją zastąpić. Lista wyboru modułów pokazuje obok nazwy
typ efektu i projekt źródłowy każdego modułu.

### Natywne instrumenty OPLL i VRC7 (w rozwoju)

Selektor typu instrumentu ma grupę FM z **OPLL / MSX** (YM2413)
i **VRC7** (DS1001). Każdy oferuje teraz **73 programy** w przewijanych listach Bank / Preset:
oryginalne 15 brzmień, 40 dodatkowych odrębnych brzmień z tablic
YM2413/VRC7/YMF281B emu2413 oraz 18 dwuoperatorowych programów stworzonych przez ChooChoo.
Dokładnie zduplikowane bajty brzmień są pomijane w obrębie każdego silnika. Dodatkowe
palety używają programowalnego slotu brzmienia układu; nie rozszerzają fizycznej
pamięci ROM. Program zero oznacza brzmienie własne. EDIT + left/right wybiera
poprzedni lub następny preset w obrębie filtra banku. Fine ct reguluje strojenie od
-100 do +100 centów.

Na liście programów EDIT + PLAY odsłuchuje podświetlone brzmienie; zwolnij
przyciski, aby zatrzymać. EDIT zatwierdza, a OPT anuluje. Przeglądanie/odsłuch nie zmienia
instrumentu, tabeli ani utworu. Zwykły gest odsłuchu ze strony Instrument
nadal działa po wyborze. Natywne obwiednie układu zapewniają attack/release;
opcjonalne Amp env dodaje programowe ADSR (zob. Native chip controls poniżej).
Nie ma pełnego edytora operatorów.

Nowe instrumenty zapisują wszystkie osiem natywnych bajtów brzmienia, bank/nazwę, program i strojenie w
pliku utworu/instrumentu. Pliki zawierające te typy używają formatu 7.0 i wymagają
tego buildu lub nowszego. Wcześniejsze formaty pozostają czytelne; utwory bez tych typów
nadal zapisują się jako 5.0. Programy rytmiczne są odłożone. Rozszerzona biblioteka jest
zwalidowana na hoście; odsłuch przez człowieka i nowa kontrola na konsoli przenośnej czekają.

### AdLib / OPL2 i OPL3 (w rozwoju)

FM obejmuje także AdLib / OPL2 (YM3812) i OPL3 (YMF262). Bank filtruje
listę fabryczną; Preset otwiera grupy kategorii i widok All. EDIT + PLAY odsłuchuje
przed wyborem, EDIT wybiera, OPT anuluje, a EDIT + left/right na Preset
wczytuje poprzedni/następny pasujący wpis. Fine ct reguluje lokalne strojenie. Mode pokazuje
2 operator, 4 operator lub Dual voice. OPL2 ukrywa niezgodne patche OPL3;
OPL3 może odtwarzać wspólną kolekcję dwuoperatorową.

Tytuły okien banku i presetu wskazują bieżący silnik. Przełączenie silnika lub
slotu instrumentu resetuje filtr banku do **All banks**, także przy przełączaniu
między zgodnymi silnikami AdLib i OPL3. Powrót z okna w obrębie tego samego
slotu zachowuje wybrany filtr. Anulowanie importu banku DX7 przywraca
poprzedni filtr.

Potwierdzenie presetu publikuje też edycje wykonane przy zwolnieniu przycisku do następnego
ticka UI/audio. Odtwarzane utwory nie potrzebują już kolejnego naciśnięcia przycisku ani restartu
transportu, aby otrzymać ten patch. Wyjście nadal podąża za skonfigurowanym buforem audio
i zachowaniem obwiedni/retriggera silnika.

Kolekcja fabryczna zawiera 697 wpisów źródłowych z The Fat Man 2-op,
The Fat Man 4-op i DMXOPL3, z 589 znormalizowanymi unikalnymi tożsamościami patchy.
Aliasy zachowują nazwy źródłowe i atrybucję. Natywne dla źródła poziomy operatorów, flagi
głębokości LFO, przesunięcia nut, stałe wysokości perkusji i strojenie dual-voice
są zachowane. Głośność trackera używa amplitudy programowej; nie odtwarza
krzywych głośności ani zasad przesunięcia velocity oryginalnych odtwarzaczy MIDI. Szacunki
czasu release ze źródła nie ucinają podtrzymanych nut. Niektóre efekty mają
wolne ataki: przytrzymaj Seashore zamiast oczekiwać, że krótkie kliknięcie go ujawni.

Zasoby fabryczne znajdują się w `instruments/chips` obok istniejącej biblioteki instrumentów,
z uwagami w `licenses/chip-banks`. Wybór presetu pozostawia tabelę slotu i inserty
należące do ścieżki bez zmian. Nowo zapisane pliki używają 7.0 i
osadzają pełne brzmienie. Audio fabryczne jest testowane maszynowo na hoście i ARM64; odsłuch przez człowieka
nadal czeka.

### Natywne instrumenty Sega PSG i Game Boy (w rozwoju)

CHIP zawiera teraz Sega PSG, GB Pulse i GB Noise. Sega używa zegara master
NTSC i jego 16-bitowego sprzężenia szumu, w tym szumu pochodzącego z kanału tonowego.
Jego domyślny **Bass range: Extended** obniża wirtualny zegar w razie potrzeby, aby
grać poniżej granicy dzielnika układu wynoszącej około 109 Hz. Dzięki temu A-2, G-2
i F-2 pozostają odrębne. Wybierz **Chip**, aby uzyskać oryginalny zakres, w którym niższe nuty
zbiegają się na granicy dzielnika. Szum o stałej częstotliwości zachowuje oryginalny zegar;
szum pochodzący z tonu podąża za rozszerzoną wysokością. Stare instrumenty Sega wczytują się z
włączonym Extended; ustawienie jest zapisywane z instrumentem.
Instrumenty GB używają rejestrów pulse/szumu DMG; kanał wave Game Boya nie jest
udostępniony. Ich autorskie listy presetów zawierają teraz odpowiednio **24, 24 i 28** presety.
Dodatki obejmują sprzężony/periodyczny szum Sega, przemiatania wysokości GB,
krótką perkusję, metaliczny szum, drony i narastania. To oryginalne programy,
a nie dźwięki wyodrębnione z gier.
Strona oferuje natywne kontrolki mode/duty/width, obwiedni/sweep/szumu w zależności od
przypadku, plus programowe ADSR amplitudy. Są one zapisywane w natywnych plikach wersji 6.
Zautomatyzowane kontrole wydajności/audio ARM64 przechodzą w granicach zgłoszonego
obciążenia; odsłuch przez człowieka nadal czeka.

### DX7 FM (w rozwoju)

Wybierz **Type → FM → DX7 FM**; DX7 to siódma pozycja, po Arcade / YM2151.
DX7 FM używa sześcioperatorowego rdzenia MSFA. Istniejąca strona FM oferuje
Bank, Preset i Fine ct obok zwykłych ustawień instrumentu. Przeglądanie presetów
używa list banków/kategorii. Przytrzymaj EDIT+PLAY, aby usłyszeć podświetlone brzmienie; zwolnienie
zatrzymuje odsłuch. Samo EDIT zatwierdza przy zwolnieniu; OPT anuluje. Wczytanie brzmienia
kopiuje jego kompletny patch bez zmiany tabel trackera ani insertów FX ścieżki.

Katalog fabryczny zawiera obecnie **67 odrębnych patchy parametrycznych DX7**:
31 oryginalnych brzmień muzycznych OpenDX7, cztery unikalne brzmienia YSE CC0 (jego 32 sloty banku
powtarzają te cztery pod innymi nazwami) oraz 32 patche stworzone przez ChooChoo.
"Oryginalny" oznacza tu programy parametrów stworzone dla tego projektu; nie oznacza
oryginalnej zawartości fabrycznej Yamahy. YSE jest dostarczane jako cztery brzmienia, a nie 32
sztucznie różne nazwy.
Kategorie opisują zrecenzowaną intencję projektowania dźwięku. Niejednoznaczne nazwy pozostają
jako Unsorted. Numeryczne kontrole grywalności przeszły; akceptacja przez odsłuch
czeka. Odrębny cel 1000 brzmień z oczyszczonymi prawami do redystrybucji nie został osiągnięty.

Aby uzyskać trwałą osobistą bibliotekę, umieść pliki `.syx` w
**`instruments/banks/dx7/`** obok istniejącej biblioteki instrumentów. Podfoldery
są obsługiwane. Otwórz **DX7 → Bank**: każdy plik pojawia się jako nazwany bank, gotowy do
przeglądania presetów. Otwórz Bank ponownie po dodaniu lub usunięciu plików; nie jest potrzebna konwersja ani
krok Load Instrument. Standardowy oryginalny bank DX7/TX7 zawiera
**32 voices**. Plik z czterema komunikatami banku pojawia się jako cztery ponumerowane banki
(łącznie 128 voices). Pliki z pojedynczym voice są również akceptowane. Aplikacja natywna
czyta ten folder; build przeglądarkowy czyta swój wirtualny system plików, a nie dowolne
foldery na komputerze.

Złe lub nieobsługiwane pliki są pomijane z licznikiem na ekranie; prawidłowe banki pozostają
dostępne. Skanowanie jest ograniczone do 2048 plików, 1 MiB na plik, 64 MiB łącznie,
60 000 voices i ośmiu poziomów zagnieżdżonych folderów. Dowiązania symboliczne są ignorowane.
Wybrany patch należy do instrumentu: usunięcie jego banku źródłowego nie może
zmienić zapisanego utworu. Pełna biblioteka przeglądania pozostaje w tym folderze i
nie jest kopiowana w całości do każdego projektu.

LOAD INSTRUMENT akceptuje także zrzuty `.syx` oryginalnych DX7/TX7 z pojedynczym voice i banków 32-voice,
w tym ograniczone sekwencje takich obsługiwanych komunikatów. Sprawdza
ramkowanie, liczbę bajtów, dane siedmiobitowe, sumy kontrolne, zakresy parametrów i rozmiar pliku
przed otwarciem zaimportowanego banku w tej samej przeglądarce FM. Wybór patcha
zatwierdza; anulowanie zachowuje bieżący instrument utworu. Ten skrót bezpośredniego importu
zachowuje swoją listę przeglądania na czas sesji; do trwałego
przeglądania użyj folderu banków powyżej. Zapisz utwór lub `.cni`, aby zachować wybrane/edytowane patche. Zrzuty bez nagłówka,
złe sumy kontrolne, performance'y/rozszerzenia DX7II i inne rodziny Yamahy są
odrzucane. Import nigdy nie wysyła komunikatów MIDI do zewnętrznego sprzętu.

CNI i projekt w wersji 6 przechowują wszystkie 155 oryginalnych bajtów voice, pełną nazwę
wyświetlaną, natywne velocity uderzenia, strojenie i tożsamość źródła. Utwory nie potrzebują
zewnętrznego banku, aby się ponownie otworzyć. Starsze wydania ChooChoo nie mogą odczytać tych
plików w wersji 7; projekty tylko z istniejącymi typami nadal zapisują się jako wersja 5. Natywne velocity ma domyślnie
100; głośność trackera to wzmocnienie po syntezie i nie uderza obwiedni ponownie.
Obwiednie operatorów, tryb stałej częstotliwości, keyboard scaling, obwiednia wysokości i
LFO pozostają natywnym zachowaniem patcha. Opcjonalne wspólne ADSR amplitudy FM kształtuje
ich połączone wyjście bez zastępowania obwiedni operatorów.

Każda część ścieżki ma jedno LFO i cztery oddzielnie posiadane sloty głosów akordu, zgodnie z
istniejącą polityką akordów/zastępowania trackera. Ścieżki nie dzielą stanu nut.
Note-off zwalnia natywne obwiednie; cut/panic je czyści. Skalarny rdzeń MSFA
używa kwantów 64 próbek przy stałej wewnętrznej 44,1 kHz, ze strumieniowym FIR do
częstotliwości wyjściowej. Tablice wyszukiwania są inicjalizowane raz poza renderowaniem audio, tak aby
równoczesne renderery offline/live nie mogły zmieniać nawzajem swojej częstotliwości. Zdarzenia
działają od następnego kwantu wewnętrznego (do 1,45 ms), po czym następuje opóźnienie grupowe FIR
około 0,25 ms. Zbuforowane próbki są zachowywane między callbackami.
To linia MSFA Modern, a nie deklaracja bitowej zgodności ze sprzętem DX7 ani
emulacja Dexed Mark I. DX7 jest ograniczony do **16 aktywnych nut w całym utworze**,
w tym ogonów release, przy zachowaniu czterech własnych slotów akordu na ścieżkę.
Po przekroczeniu budżetu zabiera najpierw ogony release, potem najcichsze podtrzymane nuty,
potem świeże ataki. Równe ataki zachowują nuty podstawowe między ścieżkami przed rozszerzeniami
akordu, ze stabilnym rozstrzyganiem remisów według slotu/ścieżki. Ta polityka obowiązuje na każdej platformie, więc
ten sam utwór ma to samo ograniczone przydzielanie nut. Nie zmienia
polifonii innych instrumentów. Odsłuch presetów jest wyłączony podczas odtwarzania.
Limit wybrano na podstawie pomiarów R36H i zwalidowano w odtwarzaniu mieszanym.

#### Edytor operatorów FM

Instrumenty Genesis FM, Arcade FM, OPL2, OPL3, OPLL i VRC7 mają przycisk **EDIT**
na prawo od Fine ct. Otwiera on tabelę parametrów patcha:

- Genesis FM / Arcade FM: algorytm, feedback, pan, AMS, PMS, maska operatorów i
  ustawienia LFO, a następnie MUL, DT, TL, KS, AR, D1R, D2R, RR i D1L dla czterech
  operatorów, plus SSG (Genesis) lub DT2 i AM (Arcade).
- OPL2 / OPL3: feedback, connection i pan dla każdego voice, deep vibrato i tremolo,
  a następnie MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS i KSR dla każdego
  operatora. W OPL3 można tu zmienić tryb (2op, 4op, dual).
- OPLL / VRC7: własne rejestry brzmienia (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Edycja presetu ROM zmienia go w
  "Custom".

Left/Right i zwykłe klawisze edycji zmieniają wartości; instrument gra dalej
jak zwykle, więc słychać zmiany. Pasek stanu opisuje parametr pod
kursorem. OPT lub Left+Shift wraca do ekranu instrumentu. DX7 i SID
nie mają edytora operatorów.

Genesis FM (YM2612) i Arcade FM (YM2151) używają teraz tej samej przeglądarki FM Bank/Preset,
odsłuchu EDIT+PLAY, zatwierdzania/anulowania i kontrolek strojenia. Genesis ma teraz
**73 presety**: oryginalne 24 i 49 obsługiwanych programów melodycznych z banku CC0 16-Bit FM Music Station
od NeoSoundFonts. Arcade ma **81 presetów**: oryginalne 24 i 57 obsługiwanych, brzmiących programów z kolekcji
YMulator-Synth na licencji GPLv3. Oryginalna para ChooChoo dzieli swoje bazowe receptury; nowe
kolekcje źródłowe oferują oddzielne palety. To nazwane kolekcje projektowania dźwięku,
a nie deklarowane odtworzenia konkretnych ścieżek dźwiękowych z gier. Nieobsługiwane
przesunięcia nut, stałe klawisze perkusji, ustawienia szumu arcade i ciche programy źródłowe
są odnotowane jako wykluczenia w `expansion-manifest.json`. Ich kompletny
patch czteroperatorowy, ustawienia obwiedni, LFO, stereo i strojenia podróżują wewnątrz instrumentów i utworów wersji 6.
Natywna obwiednia układu steruje release; zwykłe wzmocnienie trackera i wysokość dźwięku
nie restartują jej. Wyjście DAC Genesis używa blokera DC 20 Hz. Tryb szumu i
tryb specjalnej częstotliwości kanału 3 wykraczają poza tę implementację instrumentu.

Dla lokalnego zewnętrznego banku użyj `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`. Akceptuje ścisłe
42-bajtowe TFI, tekst VOPM OPM, zweryfikowany format WOPLX i oryginalny SysEx DX7.
Wczytaj powstałe pliki `.cni` przez Load Instrument. Importy użytkownika są trzymane
oddzielnie od redystrybuowalnej biblioteki fabrycznej. Pliki OPM z włączonym szumem,
niezerowymi danymi częstotliwości szumu lub częściowym panoramowaniem są odrzucane z wyjaśnieniem;
wartości pan VOPM 0/64/127 stają się lewo/oba/prawo. Binarny WOPL nie jest obsługiwany.
Wspólny katalog FM zawiera **1064 wpisy**: 697 OPL, 67 DX7, 146
OPLL/VRC7, 73 Genesis i 81 Arcade. Kolejne 76 plików udostępnia presety Sega/Game Boy
przez zwykłą przeglądarkę plików, co daje **1140 dostarczanych natywnych presetów**.
Rewizje źródeł, skróty, pełne uwagi i oryginalne dane źródłowe towarzyszą
nowym kolekcjom w `licenses/chip-banks/expansion`.

### Przenośne kolekcje fabryczne i presety USER

Natywne instrumenty układów używają **Bank:**, aby wybrać ALL, dołączoną kolekcję lub
USER. **Preset:** wybiera brzmienie w obrębie tego wyboru. Przytrzymaj EDIT i naciśnij
Left lub Right w którymkolwiek wierszu, aby przełączać; krótkie stuknięcie EDIT otwiera jego listę wyboru.

Dołączone kolekcje to paczki ZIP w `instruments/FACTORY/`. Pliki USER trafiają
do `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` lub `gb-noise`.
Przeglądaj foldery bezpośrednio albo umieść tam ZIP ze zgodnymi presetami.
Pliki ZIP pozostają nienaruszone na dysku. Wszystkie te foldery akceptują zgodne pliki `.cni`; CNI to
przenośny format instrumentów ChooChoo/ChipNomad, a nie standard producenta układów.
Przeglądarka USER czyta także następujące formaty źródłowe, również wewnątrz plików ZIP:

| Folder USER | Dodatkowe formaty |
| --- | --- |
| `dx7` | Pojedyncze voice'y i banki 32-voice Yamaha DX7/TX7 `.syx` |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | Banki VOPM `.opm`, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | Własne patche Furnace `.fui` i stałe wybory ROM |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

Bank otwiera się jako zgodne voice'y. Nieobsługiwane pliki lub funkcje zgłaszają
powód; niezgodne voice'y w banku mieszanym są wykluczane, a ich numery źródłowe
pozostają stabilne. Wczytywanie jest transakcyjne: odrzucony preset pozostawia
bieżący instrument bez zmian. Presety źródłowe zastępują tabelę trackera instrumentu
pustą, z wyjątkiem SysEx DX7, który zachowuje istniejącą tabelę.
CNI przywraca swoją zapisaną tabelę. Wczytane patche należą do utworu i nadal
działają po przeniesieniu lub usunięciu paczki źródłowej.

Banki WOPL mogą zawierać nieużywane sloty bez jawnej flagi pustego. Przeglądarka
pomija nienazwane sloty, których aktywne operatory są w pełni stłumione i nie mają
attacku. Nienazwane grywalne voice'y pozostają dostępne jako `Program N [Bbank]` lub
`Drum N [Bbank]`, z użyciem numerów programów i banków źródła liczonych od zera.
Nienazwany pojedynczy instrument OPLI używa nazwy pliku. Nazwane ciche patche są
zachowywane.

Obsługa Furnace obejmuje starsze wersje z pojedynczym instrumentem 29-112 oraz oparte na funkcjach
wersje `FINS` 127-251. Obsługa DefleMask obejmuje instrumenty wersji 11 OPN, OPM, Sega PSG
i Game Boy. To importy instrumentów, a nie pełne odtwarzacze trackerów: animowane
makra FM/operatorów, ustawienia FM LFO utworu źródłowego, sample,
sekwencje poleceń sprzętowych Game Boya, względne makra pulse/filtra SID oraz zależności
ring/sync SID są obecnie odrzucane. Stały wybór ROM OPLL jest
obsługiwany; VRC7 używa własnego ROM dla tego wyboru. SID-Wizard `.swi` i pliki
muzyczne SID (`.sid`) nie są w tym buildzie importami instrumentów.

Obsługiwane sekwencje PSG/GB/SID są zapisywane w patchu i odtwarzane z częstotliwością 60 Hz dla
Furnace/DefleMask i 50 Hz dla GoatTracker. Te pliki nie niosą pełnej
konfiguracji odtwarzania utworu: użyj oryginalnego trackera, gdy potrzebne są
częstotliwość ticków właściwa dla utworu, routing kanałów, zachowanie hard-restart lub ustawienia zgodności. Dla Sega PSG makro duty wybiera głos szumu; bez niego
import używa głosu tonowego. Pliki Game Boy trzeba umieścić w zamierzonym folderze pulse lub
noise. Zaimportowane sekwencje pozostają na razie osadzonymi danymi odtwarzania; strona
instrumentu edytuje parametry bazowe, a nie tabele trackera źródłowego.

Przełączanie presetów USER trwa przez pliki, foldery, banki i paczki ZIP w
obu kierunkach, zawijając na końcach. Ponowne otwarcie przeglądarki USER podąża za
bieżącym wyborem. ALL zawiera brzmienia USER pod Unsorted dla silników z kategoriami;
USER pozostaje osobnym wyborem najwyższego poziomu. Sega PSG i Game Boy używają
płaskich list ALL/Factory i hierarchicznej przeglądarki USER, bez panelu
Unsorted. Starsze pliki DX7 w `instruments/banks/dx7/` pozostają dostępne w USER.

Silnik pokazuje tylko swoje zgodne dołączone kolekcje. Factory Presets
łączy dołączone zestawy brzmień OPLL/VRC7 dla wybranego silnika; DX7 łączy
oryginały ChooChoo i YSE. Pobrane nazwane kolekcje pozostają oddzielne.
Fat Man 2-op jest wymieniony pod OPL2, a Fat Man 4-op pod OPL3. Zgodne pliki OPL2
CNI mogą nadal być używane w folderach USER OPL3.

Wyjście silników natywnych używa stałej zmierzonej kompensacji wzmocnienia zamiast
normalizacji dla każdego presetu. Istniejące utwory z instrumentami natywnymi mogą więc brzmieć
na innym poziomie; sprawdź balans zapisanych miksów. Metoda kalibracji
i granice są opisane w `scripts/README_MEASURE.md`.

`native-chip-audition.cct` zapewnia krótki sekwencyjny odsłuch oryginalnych
trzynastu banków fabrycznych. Każda sekcja używa innego własnego instrumentu, więc działa także z
usuniętym folderem presetów. Dłuższe ignorowane pliki WAV banków i zmierzone poziomy są
wymienione w `docs/chip-preset-auditions.tsv`; subiektywny odsłuch nadal czeka.

### Native chip controls

Podglądy przebiegów instrumentów natywnych odświeżają się, gdy ich kontrolki są przerysowywane.
Instrumenty FM mogą nakładać swoją opcjonalną krzywą Amp env; SID zachowuje swój podgląd przebiegu
bez tej nakładki tylko dla FM, także podczas przeglądania banków i presetów.

Wszystkie siedem silników FM oferuje **Bright**, **Feedback** i **Amp env**. Bright
sięga od -63 do +63, przy czym zero zachowuje patch; zmienia
poziomy operatorów modulujących, zachowując poziomy nośnych. Jego słyszalny efekt zależy od
algorytmu. Feedback ma domyślnie **Preset** lub nadpisuje natywny feedback
wartością 0-7. Te kontrolki nie przepisują zapisanych natywnych bajtów operatorów.

Amp env ma domyślnie **Bypass**. Wybierz **ADSR**, aby dodać attack, decay, sustain,
release i shape wokół natywnego dźwięku. A/D/R używają wspólnego kwadratowego zakresu 0-5 sekund;
sustain biegnie od ciszy do pełnego poziomu. Natywne obwiednie nadal
działają, więc ta obwiednia nie może przedłużyć dźwięku poza jego natywny release. Przeglądanie
presetów zachowuje kontrolki amp i brzmienia FM danego slotu. Wszystkie głosy FM używają też
przejścia 3 ms przy starcie/retriggerze i wygładzania wzmocnienia trackera 1 ms, także
gdy Amp env jest w bypass. Twarde cut/panic pozostaje natychmiastowe.

Poniższe FX phrase'a pojawiają się również jako obsługiwane cele modulacji i nagrywania
ruchu. Wartości w kolumnie FX są szesnastkowe. Ustawienia działają na odtwarzanie
bez zmiany zapisanego instrumentu. Natywne kontrolki sweep i obwiedni GB
zatrzaskują się przy następnym wyzwoleniu nuty; duty/width i częstotliwość szumu mogą się zmieniać na żywo.

| Silniki | FX | Kontrolka |
|---|---|---|
| Native FM | `OL1`-`OL6` | Bezwzględne poziomy wyjściowe operatorów: modulator OPLL/VRC7 i OPL `00-3F`; nośna OPLL/VRC7 `00-0F`; Genesis/Arcade `00-7F`; DX7 `00-63` (0-99). Wyżej oznacza większe wyjście. Pojawiają się tylko obsługiwane operatory. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Attack, decay, release i tłumienie sustain operatora 1, `00-0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | Mnożnik częstotliwości operatora 1. |
| Genesis/Arcade | `LFR` | Natywna częstotliwość LFO: Genesis `00-07`, Arcade `00-FF`. |
| Arcade | `LAD`, `LPD 00-7F` | Oddzielne głębokości amplitudy i wysokości LFO. |
| Genesis/Arcade | `LAS`, `LEN` | Natywna czułość amplitudy `00-03` i włączenie LFO `00-01`. |
| Genesis/Arcade | `LPS 00-07` | Natywna czułość wysokości. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | Natywne attack, decay, sustain i release. Wartości czasu rosną w stronę `0F`; sustain rośnie w stronę pełnego poziomu. |
| SID | `SPR 01-10` | Częstotliwość cichego partnera od 1× do 16×; wpływa na modulację pierścieniową i hard sync. |
| Wszystkie native FM | `FBK 00-07` | Bezwzględny natywny feedback, inicjalizowany z instrumentu. |
| Wszystkie native FM z włączonym Amp env; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack, decay, sustain, release, shape. Nie włączają one wyłączonego (bypass) wzmacniacza FM. |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | Ton / biały szum / szum periodyczny; trzy stałe częstotliwości szumu lub częstotliwość pochodząca z tonu. |
| GB Pulse | `CMD 00-03` | Natywne duty pulse. |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | Szerokość szumu, dzielnik zegara i shift. |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | Okres sweep, shift, kierunek w dół. |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | Natywna obwiednia: poziom początkowy, okres, kierunek narastania. |

Wybór natywnego FX używa instrumentu w kolumnie `I` wiersza phrase'a lub
aktywnego instrumentu znalezionego przez przeszukanie wstecz, gdy `I` jest puste. Okno FX
pokazuje w zwartym bloku krótki opis kontrolki, jej bieżącą wartość presetu i prawidłowy zakres
poleceń. Wybór innego
natywnego efektu zaczyna od tej wartości; ponowne otwarcie tego samego efektu zachowuje jego
edytowaną wartość. Zaznaczenie wielu wierszy określa instrument każdego wiersza osobno.
Tabele używają kontekstu swojego instrumentu. Natywne edycje wartości zatrzymują się na swoich dozwolonych
końcach, w tym duty, szum, sweep, ADSR, feedback i poziomy operatorów.
Wartości są szesnastkowe: na przykład maksimum `63` w DX7 oznacza dziesiętnie 99.

Te polecenia FM używają bezwzględnych wartości natywnych. Polecenia operatorów zawsze dotyczą
operatora 1; `OMU 03` wybiera jego mnożnik 3. Kontrolki DX7 specyficzne dla trackera
to poziomy operatorów i feedback. Preset/Range zachowuje swój kolor informacyjny;
tytuły i opisy mają te same kolory co FM Feedback.

Live Modulation zachowuje pełny zestaw natywnych celów ze stałymi operatorami.
Nagrywanie ruchu zapisuje tylko kontrolki reprezentowane na zwartej liście trackera;
parametry operatorów są nagrywane tylko dla operatora 1. Inne cele modulacji
nadal działają na żywo, ale nie generują poleceń phrase'a.

`SCP` i `SCT` zachowują bajtowo skalowane odwzorowania na rejestry SID szersze niż 8 bitów.
`SMR 01-C8`, `SWV 01-08` i `SPR 01-10` odpowiadają teraz natywnej numeracji presetów.
Wyświetlane wartości presetów to ustawienia bazowe, niezależne od odtwarzanej obwiedni
lub LFO. `SLE` działa na poziomy operatorów i bezpośrednie parametry FM bez
ponownego wyzwalania nuty ani zmiany zapisanego patcha.

Wszystkie natywne FX mają opisowe tytuły, zakresy i zachowanie w liście wyboru FX
phrase'a, plus wskazówki wartości. `CMD` opisuje ton/szum, duty lub szerokość szumu
zależnie od wybranego instrumentu. Te kontrolki uzupełniają wspólne
grupy Track, Envelope i Modulation; sama grupa silnika nie jest pełnym
zestawem dostępnych efektów phrase'a.

Istniejące wspólne filtry LP/BP/HP w Braids, Plaits, PCM i innych obsługiwanych
silnikach to przetwarzanie programowe po syntezie. aChChid zamiast tego używa swojej natywnej
ścieżki filtra 303. Ta runda nie dodaje filtra do AY ani natywnego FM; jasność zmienia
samą syntezę FM. Inserty ścieżek pozostają dostępne do dodatkowego przetwarzania.

Natywne instrumenty zapisują się w CNI w wersji 7 (8, gdy obecne są polecenia bezwzględne),
a natywne utwory w CCT w wersji 9. Instrumenty zawierające zaimportowane sekwencje
używają CNI w wersji 9, a utwory je zawierające używają CCT w wersji 10. Starsze buildy
nie mogą otworzyć tych nowszych plików z sekwencjami. Zapisane bajty presetów FM i
ustawienia brzmienia na stronie instrumentu pozostają niezmienione.

### Wytyczne dotyczące obciążenia natywnych układów na konsolach przenośnych

Jest osiem ścieżek utworu i **dwa sloty efektów insert na ścieżkę**. Szesnaście
dostępnych slotów nie gwarantuje wystarczającej mocy CPU do uruchomienia szesnastu efektów.
Limit szesnastu aktywnych nut DX7 to osobny budżet syntezy dla całego utworu;
akordy i ogony release wliczają się do niego.

W pomiarach R36H cztery zapętlone ścieżki WAV, Sega PSG, GB Pulse, DX7 i
OPL3 z współdzielonymi wysyłkami i czterema insertami (dwa Compressory, Doubler, TAPESCAM)
wypadły lepiej niż gęste ośmiościeżkowe aranżacje FM. Wersja z pojedynczymi nutami
nie miała chybionych terminów renderowania w trzydziestosekundowym pomiarze 48 kHz/512 ramek;
dodanie czterodźwiękowego akordu DX7 dało jeden skok czasowy. Gęste utwory FM
z wieloma kosztownymi insertami przekroczyły budżet CPU. Zachowaj trochę zapasu,
obserwuj istniejący wskaźnik przeciążenia i dodawaj efekty tam, gdzie pomagają utworowi.
Aplikacja zachowuje każdą ścieżkę i slot; nie ma nowego twardego limitu liczby insertów.
Pełne pomiary i stan testów długotrwałych znajdziesz w `chip-instruments-report.md`.

Odpowiadający mu 70-sekundowy fizyczny test audio tego zrównoważonego aranżu z akordami
przeszedł bez chybionych terminów renderowania ani zarejestrowanych niedomiarów ALSA przy istniejącym
ustawieniu 48 kHz / 4906 ramek. Jego najgorszy callback trwał 81,819 ms wobec
terminu 102,208 ms. Testy używały osobnego wzmocnienia master 0,4 dla zapasu.
Zachowaj trasę bezpośredniej karty `AUDIODEV=plughw:0,0` zwykłego launchera: domyślny
współdzielony mixer systemu powodował niedomiary w próbach diagnostycznych. Żadne
ustawienie audio użytkownika nie zostało zmienione. Dziesięciominutowy test obciążeniowy z mniejszym buforem
nadal zarejestrował 28 skoków czasowych; pełne wyniki znajdują się w raporcie.

### Instrumenty SID i Phrase FX

Wybierz **CHIP → SID**. Bank oferuje **ChooChoo SID Originals** (32 autorskie
programy) i **SIDkit Effects** (24 efekty na licencji MIT). Oba używają tej samej
przeglądarki presetów i zapisują swój kompletny wybrany program w instrumencie
i utworze. Do udostępnienia utworu nie jest potrzebny żaden zewnętrzny bank. To natywne
programy parametrów SID z obwiedniami i recepturami ruchu, a nie sample audio ani
kompletne utwory C64. Oddzielnych dwunastu kandydatów badawczych GoatTracker
nie dostarczamy; ich tabele wave/pulse/filter wymagają dedykowanego importera/odtwarzacza.

Strona instrumentu udostępnia przebieg/szerokość impulsu, tryb filtra/cutoff/rezonans
i natywne kontrolki ADSR. SID używa przypiętego cyfrowego oscylatora floooh/chips, obwiedni
i filtra w stylu 6581 liczonego co cykl. Nie ma alternatywnych modeli układu,
wyborów rewizji ani dodanych profili charakteru. To generyczne przybliżenie,
a nie skalibrowany model analogowy R2/R3/R4/8580.

Każda nuta ma własny filtr. Ring/sync używają cichego oscylatora partnera, więc
współdzielenie filtra i interakcje trzech głosów różnią się od fizycznego SID.
Konsola przenośna ma budżet dla całego utworu wynoszący cztery zwykłe nuty SID.
Ring lub sync kosztuje dwie jednostki budżetu na nutę, co pozwala na dwie takie nuty albo jedną plus dwie
zwykłe nuty. Akordy i ogony release się wliczają. Zwolnione nuty są wycofywane
jako pierwsze; nowe nuty mają pierwszeństwo przed starszymi podtrzymanymi. Kosztowne inserty i
inne syntezatory nadal dzielą budżet CPU audio.

| FX | Zakres | Działanie |
| --- | --- | --- |
| `SCP` | `00-FF` | Baza szerokości impulsu, skalowana do natywnej 12-bitowej szerokości; ruch impulsu receptury pozostaje aktywny. |
| `SCT` | `00-FF` | Baza cutoff, skalowana do natywnego 11-bitowego cutoff; ruch filtra receptury pozostaje aktywny. |
| `SRN` | `00-0F` | Natywny rezonans. |
| `SWV` | `00-07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00-07` | Bity trybu filtra: 1 dolnoprzepustowy, 2 pasmowoprzepustowy, 4 górnoprzepustowy; 0 bypass. |
| `SMR` | `00-FF` | Zegar makra receptury, skalowany od 1 do 200 Hz. |
| `SRG` | `00-01` | Modulacja pierścieniowa trójkąta z cichym partnerem. |
| `SSY` | `00-01` | Synchronizacja oscylatora z cichym partnerem. |

Szerokość impulsu i cutoff obsługują `SLE`. Przebiegi dyskretne, przełączniki, rezonans
i feedback FM zachowują swoje użyteczne natywne kroki; szersze wartości bajtowe nie
stworzyłyby więcej stanów sprzętowych. `FBK` bezpośrednio wybiera feedback `00-07`.
Poziomy operatorów używają zakresów specyficznych dla silnika wymienionych powyżej. Korekty jasności i
master zachowują pełny zakres bajtowy.

### Przenośne kolekcje fabryczne i presety USER

Natywne instrumenty układów używają **Bank:**, aby wybrać ALL, dołączoną kolekcję lub
USER. **Preset:** wybiera brzmienie w obrębie tego wyboru. Przytrzymaj EDIT i naciśnij
Left lub Right w którymkolwiek wierszu, aby przełączać; krótkie stuknięcie EDIT otwiera jego listę wyboru.

Dołączone kolekcje to paczki ZIP w `instruments/FACTORY/`. Pliki USER trafiają
do `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` lub `gb-noise`.
Przeglądaj swoje foldery bezpośrednio albo umieść tam ZIP z folderami i zgodnymi presetami `.cni`.
DX7 akceptuje także obsługiwane pojedyncze voice'y `.syx` i banki 32-voice,
również wewnątrz plików ZIP. Pliki ZIP pozostają nienaruszone na dysku. Bezpośrednie wczytywanie WOPL/WOPLX nie jest obsługiwane;
użyj zgodnych presetów CNI lub najpierw skonwertuj obsługiwane formaty źródłowe offline.

Przełączanie presetów USER trwa przez pliki, foldery, banki i paczki ZIP w
obu kierunkach, zawijając na końcach. Ponowne otwarcie przeglądarki USER podąża za
bieżącym wyborem. ALL zawiera brzmienia USER pod Unsorted dla silników z kategoriami;
USER pozostaje osobnym wyborem najwyższego poziomu. Sega PSG i Game Boy używają
płaskich list ALL/Factory i hierarchicznej przeglądarki USER, bez panelu
Unsorted. Starsze pliki DX7 w `instruments/banks/dx7/` pozostają dostępne w USER.

Silnik pokazuje tylko swoje zgodne dołączone kolekcje. Factory Presets
łączy dołączone zestawy brzmień OPLL/VRC7 dla wybranego silnika; DX7 łączy
oryginały ChooChoo i YSE. Pobrane nazwane kolekcje pozostają oddzielne.
Fat Man 2-op jest wymieniony pod OPL2, a Fat Man 4-op pod OPL3. Zgodne pliki OPL2
CNI mogą nadal być używane w folderach USER OPL3.

Wyjście silników natywnych używa stałej zmierzonej kompensacji wzmocnienia zamiast
normalizacji dla każdego presetu. Istniejące utwory z instrumentami natywnymi mogą więc brzmieć
na innym poziomie; sprawdź balans zapisanych miksów. Metoda kalibracji
i granice są opisane w `scripts/README_MEASURE.md`.
