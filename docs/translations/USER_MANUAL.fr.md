# Manuel d'utilisation de ChooChooTracker

> Traduction française de [`docs/USER_MANUAL.md`](../USER_MANUAL.md), établie à partir de la version anglaise du commit `7c3ed6f`. En cas de divergence, la version anglaise fait foi.
>
> **Conventions.** L'interface de l'application est en anglais. Les noms d'écrans, de menus, de champs et de touches (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**, **OPT**, **SELECT**...), les codes FX (`HOP`, `TBX`, `M1A`...), les noms de modules et de moteurs (ADSR, LFO, Braids, Plaits...) et les extensions de fichiers sont donc laissés tels quels, pour que vous les retrouviez à l'écran.

ChooChooTracker est un tracker musical à 8 pistes pour consoles portables.

Il reprend le fonctionnement et la navigation classiques de LSDj, avec plusieurs options de conception sonore :

- émulation de puce AY
- oscillateurs Mutable Instruments Braids, Plaits et Plaits-Alt, avec un filtre multimode
- échantillons PCM, formes d'onde à cycle unique et oscillateurs à table d'ondes, avec un filtre multimode
- aChChid, un synthétiseur acid polyvalent basé sur Open303
- Bogie, un synthétiseur de batterie natif à 12 modèles, avec un filtre multimode
- MME, un synthétiseur à deux oscillateurs et multiples modulations, avec un filtre multimode
- Sintered, un synthétiseur de batterie numérique expérimental natif à six modèles

## 1. Installation et fichiers

### ArkOS / PortMaster

Installez `choochootracker.zip` avec PortMaster, ou copiez le paquet extrait dans le dossier `ports` de la console. Lancez **ChooChooTracker** depuis le menu Ports.

### Miyoo Mini / Mini Plus (Onion OS)

Extrayez l'archive MiyooPorts à la racine de la carte SD, de façon que ses dossiers `Roms/PORTS/` fusionnent avec ceux déjà présents dans Onion. Lancez **ChooChooTracker** depuis le menu Ports, après avoir actualisé la collection. Sauvegardez le dossier `projects/` de l'application avant de remplacer une installation existante.

### Windows

Lancez `choochootracker.exe` avec `SDL2.dll` et `libwinpthread-1.dll` dans le même dossier.
Appuyez sur **ALT + ENTER** pour basculer en plein écran. La fenêtre est redimensionnable ; l'image
s'adapte en conservant son rapport d'aspect d'origine.

### Web

La page du navigateur suit le thème actif du tracker. Les réglages enregistrés, y compris
les affectations de touches personnalisées confirmées avec **Done**, sont écrits sans délai dans le
stockage de ce navigateur. Après un verrouillage de l'écran ou un changement d'application, touchez un contrôle pour rétablir
le son si nécessaire. **Restart audio** rouvre la sortie audio sans perdre le
projet. **Reload app** enregistre le travail en cours dans **Continue**, enregistre les réglages,
et attend le stockage du navigateur avant de recharger ; un enregistrement qui échoue ou dépasse le délai
annule le rechargement. Utilisez **Download .cct** pour une sauvegarde portable. Le stockage du navigateur
peut être vidé indépendamment, et le rétablissement du routage audio et des manettes sous iOS
n'est pas garanti.

La version navigateur rafraîchit son affichage à environ 30 images par seconde et utilise un tampon audio fixe de
1024 images pour laisser plus de temps à l'audio entre deux appels. Les anciens réglages de tampon
enregistrés sont ajustés automatiquement ; les projets et les autres réglages sont
conservés. L'interrogation des entrées et les minuteries de l'application gardent leur cadence habituelle.

### Configuration des dossiers

| Dossier | Contenu |
|---|---|
| `AY_Wavetables` | tables d'ondes `.aywave` de la puce AY |
| `fonts` | fichiers de police ChipNomad `.cnfont` |
| `instruments` | préréglages d'instruments `.cni`, y compris les fichiers ChipNomad |
| `pitch-tables` | accordages alternatifs `.csv` |
| `projects` | fichiers de morceaux et de projets `.cct` |
| `samples` | fichiers `.wav` pour le moteur Sampler |
| `SR_wavetables` | tables d'ondes au format Serum pour le moteur BYOWTBL |
| `themes` | thèmes ChipNomad `.cth` |
| `title` | ressources de l'écran-titre |
| `waveforms` | formes d'onde `.wav` à cycle unique pour le moteur 2xSCWF |

## 2. Commandes

ChooChooTracker se joue mieux avec une manette dotée d'une croix directionnelle, de 2 sticks analogiques et de 7 boutons.

| Commande logique | Défaut sur console portable | Défaut sous Windows |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | croix directionnelle | touches fléchées |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Espace |
| **SELECT** | Select | Shift |
| Axes de modulation | sticks gauche et droit | sticks de la manette |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Les affectations se changent dans **Settings > Key mapping**. Sous PortMaster, relâchez
le bouton utilisé pour sélectionner une affectation avant d'appuyer sur le nouveau bouton. Enregistrez une
réaffectation avec **Done**, puis relâchez le bouton avant de continuer dans Settings.
Les dispositions de bureau n'utilisent QWERTZ par défaut que pour les régions QWERTZ ; les claviers polonais
gardent les valeurs par défaut QWERTY standard.

Les utilisateurs de Windows, du web et d'Android peuvent aussi employer une manette. Sous Android,
des commandes tactiles apparaissent automatiquement lorsqu'aucune manette n'est connectée : en portrait,
elles se placent sous le tracker, avec une croix directionnelle carrée, les commandes A/EDIT, B/OPT, START/PLAY et
SELECT/SHIFT, puis une section Live/Motion distincte avec deux sticks de modulation
et des commandes REC/DEL momentanées. En paysage, la croix reste à gauche
et les boutons d'action à droite. Les projets et les échantillons Android restent privés pour
l'application ; utilisez le sélecteur de fichiers d'Android pour importer des projets `.cct` et des échantillons `.wav`,
et pour choisir l'endroit où les exports WAV sont enregistrés.

### Navigation

- Utilisez **[UP/DOWN/LEFT/RIGHT]** pour déplacer le curseur.
- Maintenez **SELECT + [DIRECTION]** pour passer d'un écran à l'autre.
- **SELECT + [LEFT/RIGHT]** fait aussi sortir des pages annexes vers l'écran MSCPIT
  voisin, comme Instrument Pool : Project/Settings (y compris les sous-menus de Settings)
  mènent à Mixer/Chain ; Groove à Chain/Instrument ; Modulation/Insert FX à
  Phrase/Table ; AY Wavetable à Instrument/Table. Sur le bord gauche, Reverb/Delay
  reviennent au Mixer avec SELECT + LEFT. La navigation verticale existante reste inchangée.
- Maintenez **OPT + [DIRECTION]** pour la navigation propre à chaque écran.
- Sur une valeur, utilisez **EDIT + [LEFT/RIGHT]** pour des changements fins ou **EDIT + [UP/DOWN]** pour des changements grossiers.
- Sur une **Destination** de Modulation, **LEFT/RIGHT** parcourt directement les destinations disponibles ; touchez **EDIT** pour ouvrir son menu groupé.
- Touchez **EDIT** pour saisir une valeur ou activer une commande.
- Touchez **EDIT** deux fois pour créer une nouvelle chaîne, phrase ou instrument, lorsque c'est possible.
- Maintenir une direction répète le mouvement après le délai configuré dans Settings.

### Sélection et presse-papiers

- **SELECT + OPT** active le mode sélection et parcourt des plages de sélection utiles.
- Utilisez **[DIRECTION]** pour étendre la plage.
- **OPT** copie la sélection.
- **OPT + EDIT** la coupe.
- **SELECT + EDIT** la colle.
- **EDIT + [DIRECTION]** modifie ensemble les cellules sélectionnées.

Sur l'écran Phrase, lorsqu'une sélection est active, la colonne des notes peut être remplie
avec des motifs rythmiques :

- **OPT + LEFT** remplit les cellules de notes de la sélection avec un motif fixe de 16 pas
  et passe au motif suivant à chaque pression. Après le dernier motif, les
  notes sont effacées et le cycle recommence. Les motifs sont alignés sur les lignes
  absolues de la phrase : une sélection partielle ne reçoit donc que les pas qui tombent à l'intérieur.
- **OPT + RIGHT** remplit les cellules de notes de la sélection avec un motif aléatoire de
  3 à 8 notes (7 et 8 sont moins probables), en cycle sans fin tant que **OPT** est
  maintenu.
- **OPT + UP** fait muter les notes déjà placées dans la sélection : chaque pression
  en choisit 1 à 3 et décale chacune d'une valeur aléatoire à l'intérieur d'une octave
  (les petits décalages sont plus probables que les extrêmes). Avec **Note Lock** actif,
  les notes décalées sont ramenées dans la gamme ; les instruments Sampler avec
  **Slice** activé restent chromatiques. Rien n'est effacé : la 8e pression rétablit les
  notes que la sélection contenait au début du cycle, puis le cycle recommence.
- **OPT + UP** sur une sélection qui ne couvre que la colonne de volume (aucune cellule de note,
  d'instrument ou de FX) randomise les vélocités au lieu de faire muter : la
  première pression applique le motif Minimal (chaque volume varie d'au plus
  +-20), la deuxième Medium (jusqu'à +-50), la troisième Random (valeurs
  entièrement aléatoires), et la quatrième rétablit les volumes que la sélection contenait
  au début du cycle. Relâcher **OPT** ramène le cycle à Minimal.
  Les lignes sans volume ne sont pas touchées.
- **OPT + DOWN** étale les notes placées des instruments Sampler découpés (sliced) de
  la sélection en une suite chromatique : la première note de chaque instrument (la plus haute
  dans le séquenceur) garde sa valeur et chaque note suivante de cet
  instrument monte d'un demi-ton au-dessus de la précédente, ce qui transforme une pile de
  notes identiques en déclenchements de tranches consécutifs. Plusieurs instruments
  découpés s'étalent indépendamment à partir de leurs propres premières notes, les notes
  d'instruments sans découpage ne sont pas touchées, et le résultat n'est jamais
  ramené dans la gamme du projet. La dernière note écrite par l'étalement devient
  la dernière note utilisée pour la saisie, de sorte que poursuivre la suite à la main reprend
  à la tranche suivante. Une sélection qui ne contient que des instruments découpés
  s'étale à chaque pression ; lorsque des notes non découpées sont aussi présentes,
  l'étalement s'exécute à la première pression, puis l'arpège aléatoire prend en charge
  ces notes à partir de la deuxième.
- **OPT + DOWN** sur une sélection sans notes d'instruments découpés est un arpégiateur
  aléatoire pour les notes qui s'y trouvent déjà. Un accord est choisi
  dans la palette CRD - accords de 3 notes pour des sélections de 1 à 2 notes, accords de 3 notes ou plus
  pour 3 notes, n'importe quel accord pour 4 ou plus - avec sa fondamentale sur une note aléatoire
  de la gamme du projet dans le registre de la sélection (n'importe quelle note
  chromatique lorsque la gamme est désactivée ou que la piste en est exclue), et ses notes
  sont quantifiées dans la gamme comme le fait la lecture pour CRD. Chaque
  pression redistribue l'accord sur les notes placées : aléatoirement,
  du plus grave au plus aigu, du plus aigu au plus grave, puis en aller-retour ; la 5e pression
  rétablit les notes d'origine et la pression suivante choisit un nouvel accord.
  Le nom de l'accord s'affiche dans la barre d'état tant que l'arpège est actif, aucune
  note n'est ajoutée ni supprimée, et relâcher **OPT** réinitialise tout le
  processus.
- Chaque pression de remplissage efface les notes de la sélection avant d'écrire, et les notes
  placées reprennent le dernier instrument utilisé ; relâcher **OPT** ramène les cycles
  à leur première étape.
- Tant qu'une sélection est active et qu'aucun autre message n'est affiché, la barre
  d'état fait défiler toutes les 2 secondes un rappel de ces combinaisons : `EDIT + DIR =
  batch note edit`, `Double-tap EDIT: resample selection`, `OPT + LEFT =
  pattern fill`, `OPT + RIGHT = random fill`, `OPT + UP =
  mutate/randomize velocity` et `OPT + DOWN = random arp/slice spread`.
  Entrer dans le mode sélection ou en sortir relance la rotation ; tout message
  d'action remplace l'indice jusqu'à son expiration.

### Lecture

- **PLAY** démarre à partir du curseur. Appuyez de nouveau pour arrêter.
- Vous pouvez modifier des notes, des instruments et des chaînes pendant la lecture ; les changements s'entendent
  au prochain tick du séquenceur.
- **SELECT + PLAY** démarre toutes les pistes lorsqu'il est utilisé en dehors de l'écran Song.
- **EDIT + PLAY** écoute un instrument depuis l'Instrument Pool.

## 3. Structure d'un morceau et plan des écrans

Maintenez **SELECT + [DIRECTION]** pour passer d'un écran à l'autre.

La rangée de navigation principale est :

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Rangée centrale :

- **M** : Mixer (mène à Reverb, Delay)
- **S** : Song (mène à Project, Settings)
- **C** : Chain
- **P** : Phrase (mène à Groove)
- **I** : Instrument (mène à Modulation et à Instrument Pool)
- **T** : Table (mène à l'éditeur AY Wavetable)

Rangée du haut :

- **R** : Reverb
- **P** : Project
- **G** : Groove
- **M** : Modulation

Rangée du bas :

- **D** : Delay
- **S** : Settings
- **P** : Instrument Pool
- **W** : éditeur AY Wavetable

Un morceau de ChooChooTracker suit cette hiérarchie :

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

Un morceau compte 8 pistes. Chaque piste contient une suite de chaînes, et chaque chaîne contient une suite de phrases. Les phrases sont des séquences de 16 pas de notes. Une note comprend une hauteur, un instrument, une vélocité et jusqu'à 3 FX de piste. Un instrument est ce qui produit le son.

### Polices 16x24 optionnelles et NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 contient TechMonoAudit (converti à partir de Share Tech Mono), Departure Mono, Spleen et Cozette. Chacune fournit le jeu ASCII imprimable complet en cellules de 16×24 pour un affichage 640×480. Les polices construites en pixels gardent leurs pixels d'origine ou une mise à l'échelle entière avec marge ; Cozette reste plus petite, car doubler sa hauteur d'encre de 13 pixels dépasserait la cellule. Le thème NostromoAmberDa2 est disponible via Settings → Edit color theme → Load. Il conserve le fond ambre et le texte informatif de Nostromo, avec un texte principal rouge, des valeurs orange plus sombres et un curseur sarcelle. Charger ces ressources ne change ni la police ni le thème que vous avez choisis. Voir le fichier README.txt joint pour les attributions et les licences.

### Espacement de la page Project

La page Project place la version de l'application sur sa propre ligne et sépare Load, Save, New, Export et Manage des métadonnées du projet par des lignes vides. Les positions du curseur et les zones tactiles suivent les champs affichés ; les réglages du projet et les opérations sur les fichiers ne changent pas.

### Télémétrie de l'affichage audio

Les indicateurs audio utilisent un instantané, propre à l'interface, du mixage final et de la contribution de chaque piste. Les échantillons de piste et les crêtes stéréo sont capturés après le niveau de piste et le tilt, avant les retours d'effets partagés et le gain master. La surveillance ne modifie pas le signal audio. Le callback audio publie des instantanés de taille fixe, sans verrou ni allocation ; l'historique affiché couvre environ 21 ms.

### Forme d'onde persistante

Graphics → Persistent waveform est sur OFF par défaut. Touchez EDIT pour l'activer ou utilisez EDIT + direction ; le choix est enregistré comme préférence de l'application. OFF rétablit les pages pleine hauteur et les aperçus de forme d'onde d'instrument existants. Une fois activée, une forme d'onde sur deux lignes, au-dessus des pages d'édition, montre le mixage final sommé en mono. Les pages Instrument et Modulation montrent la sortie d'une piste qui utilise l'instrument sélectionné ; la piste Song sélectionnée l'emporte si plusieurs pistes l'utilisent. Project, Settings, les tables d'ondes et les fenêtres contextuelles gardent leur contenu en pleine hauteur. Les listes défilent dans les lignes restantes. Les pieds de page d'Instrument conservent les aperçus ADSR/échantillon, et les champs de Modulation sont compactés pour que tous les contrôles restent accessibles. Ses zones tactiles suivent les champs affichés dans les deux modes de forme d'onde.

### Piano de lecture

Un piano en pixels dessiné par le code, sous les huit lignes de pistes à droite, allume les classes de hauteur qui sonnent, repliées sur une octave. Il inclut les notes d'accord et respecte les pistes en sourdine ou arrêtées. Le contour utilise la couleur de valeur du thème et toutes les touches actives utilisent la couleur de la forme d'onde ; les touches au repos ont un remplissage sombre et de discrètes ombres. Sa grille utilise une mise à l'échelle entière et des bordures de largeur fixe. Le piano est un indicateur de lecture, pas une commande de saisie de notes. Les fenêtres contextuelles et l'éditeur de tables d'ondes gardent leur propre contenu dans cette zone.

### Vumètres du Mixer

Le Mixer affiche pour chaque piste des barres de crête **L/R** adjacentes, après son PAN et son tilt, avant les effets partagés et le gain master. Le vumètre couvre de -48 à 0 dBFS et décroît entre les mises à jour. Ses bandes basse et médiane utilisent les couleurs du thème, tandis que la bande haute est d'un rouge vif près de la pleine échelle.

## 4. Song, Chain et Phrase

### Song

L'écran Song contient 8 colonnes, une par piste audio. Chaque cellule renvoie à une chaîne. Un morceau peut contenir jusqu'à 256 lignes (`00-FF`).

Les pistes vont de gauche à droite, tandis que leurs chaînes s'enchaînent verticalement. Chaque piste est indépendante, et les chaînes peuvent contenir de 1 à 16 phrases : les pistes peuvent donc se désynchroniser. Concevez vos chaînes pour qu'elles restent ensemble, ou laissez-les dériver si c'est ce dont la piste a besoin.

Il est courant de réserver 1 chaîne pour les phrases vides, généralement `00` ou `FE`.

Vous pouvez garder plusieurs sous-morceaux dans un projet en créant des « îlots » : des sections séparées par des lignes vides.

#### Live Mode

Touchez **OPT** deux fois pour entrer dans le Live Mode ou en sortir ; `LIVE` remplace le titre Song. En Live Mode, **PLAY** démarre la chaîne sélectionnée sur sa propre piste lorsque celle-ci est arrêtée. Sur une piste en lecture, il met la chaîne sélectionnée en file d'attente pour la prochaine limite de chaîne ; appuyez deux fois sur **PLAY** pour un changement urgent à la prochaine limite de phrase. Une cellule Song vide sélectionnée met plutôt en file d'attente un arrêt quantifié. `+` marque une chaîne en attente, `!` une chaîne urgente et `-` un arrêt en attente.

Sélectionnez une ligne Song sur plusieurs colonnes pour mettre en file d'attente ou arrêter ces pistes ensemble. Les sélections sur plusieurs lignes sont volontairement refusées. Les files du Live sont effacées pendant qu'une boucle de plage Song est active. Le Live est un mode de performance et n'est pas enregistré dans le projet.

#### Commandes

- **OPT + [UP/DOWN]** : sauter de 16 positions vers le haut ou le bas
- Sélectionnez une plage, puis **SHIFT + EDIT** : cloner superficiellement les chaînes en gardant leurs phrases d'origine
- Sélectionnez une plage, puis touchez deux fois **SHIFT + EDIT** : cloner en profondeur les chaînes et leurs phrases
- Sélectionnez une plage, puis **EDIT + [UP/DOWN]** : déplacer la sélection vers le haut ou le bas
- Touchez **OPT** 3 fois : ajouter ou retirer une surbrillance, pour l'organisation visuelle
- **OPT + SHIFT** : mettre en sourdine la piste courante ou les colonnes sélectionnées (relâchez d'abord **OPT** pour garder la sourdine active)
- **OPT + PLAY** : mettre en solo la piste courante ou les colonnes sélectionnées (relâchez d'abord **OPT** pour garder le solo actif)
- **OPT + [LEFT/RIGHT]** : mettre en solo toutes les pistes à gauche ou à droite de la piste courante
- Sélectionnez une plage, puis touchez deux fois **EDIT** : ouvrir l'écran BOUNCE TO SAMPLE (voir Bounce ci-dessous)

#### Bounce

Sélectionnez une plage de lignes et de colonnes Song, puis touchez deux fois **EDIT** pour ouvrir l'écran BOUNCE TO SAMPLE. Le bounce inclut toutes les pistes sélectionnées : les pistes dont la première chaîne commence plus tard dans la sélection attendent en silence et rejoignent au début de leur première chaîne, si bien que le fichier contient les chaînes de toutes les pistes sélectionnées.

L'écran propose :

- **File name** : à modifier avec le clavier de caractères. À l'ouverture de l'écran, le prochain numéro de séquence libre (`001`, `002`, ...) est proposé ici : le premier numéro dont le fichier `.wav` n'existe pas encore dans le dossier d'export. Modifiez-le librement : le nom est utilisé tel quel. Si un fichier de ce nom existe déjà, un suffixe `_001`, `_002`, ... est ajouté afin que les fichiers existants ne soient jamais écrasés. Le compteur est remis à zéro lorsque vous chargez ou créez un projet.
- **Sample rate** et **Bit depth** : les mêmes options que l'écran d'export
- **Include in a sample name** : trois cases à cocher qui ajoutent au début du nom de fichier des étiquettes de contexte lorsque **Start** est pressé (toutes désactivées par défaut) :
  - **[BPM]** - le tempo actuel du projet, par exemple `[120]`
  - **[Key]** - la note fondamentale et la gamme du projet, par exemple `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** - la durée rendue du bounce : une phrase équivaut à une mesure (16 doubles croches), un temps fait 4 doubles croches. Les phrases entières n'affichent que les mesures, par exemple `[2]` ; les phrases partielles affichent `[0:2]` (deux temps) ou `[0:1:3]` (un temps plus trois doubles croches). Pour les bounces multipistes, la piste la plus longue décide. Une sélection qui se termine au milieu d'un temps compte une double croche de moins : la dernière ligne sélectionnée est le point de coupe, donc son note-off tombe à la fin de la double croche précédente.
  - Les étiquettes sont appliquées dans l'ordre `[BPM][Key][Length]` avant le nom de fichier.
- **Start** : rend la sélection dans un fichier WAV du dossier d'export (voir *Export location* dans l'écran Project)
- **Cancel** : revenir sans faire de bounce

Pendant le rendu du bounce, **OPT** l'annule. Lorsque le bounce se termine ou est annulé, l'écran d'où vous êtes parti revient.

Sur ordinateur, le key jazz apporte aussi ici la saisie directe d'index hexadécimaux, la sélection
par Shift+flèches comme dans Phrase, Delete/Backspace/Insert et Ctrl+C/X/V - voir
[Key jazz](#key-jazz-ordinateur-uniquement).

### Chain

Une chaîne est une liste ordonnée de phrases de 16 pas, avec transposition optionnelle. Elle peut contenir jusqu'à 16 phrases, et la même phrase peut apparaître plusieurs fois. La 2e colonne règle la transposition en demi-tons.

Un astérisque (`*`) apparaît à côté d'une chaîne réutilisée dans le projet. Vous pouvez cloner des chaînes depuis l'écran Song.

#### Commandes

- **OPT + [LEFT/RIGHT]** : passer d'une piste à l'autre
- **OPT + [UP/DOWN]** : passer d'une chaîne à l'autre dans la piste courante
- Sélectionnez une plage, puis **SHIFT + EDIT** : cloner des phrases
- Sélectionnez une plage, puis touchez deux fois **EDIT** : ouvrir l'écran BOUNCE TO SAMPLE pour les lignes de chaîne sélectionnées de la piste courante (voir Bounce sous Song)

Sur ordinateur, le key jazz apporte aussi ici la saisie directe d'index hexadécimaux - voir
[Key jazz](#key-jazz-ordinateur-uniquement).

### Phrase

Une phrase est le motif d'une piste dans un séquenceur pas à pas traditionnel.

Une phrase compte 16 pas, avec 1 ligne par pas. Chaque ligne contient une note, un instrument, un volume de `00` à `7F` et des colonnes de FX. Les notes utilisent la notation de tracker, comme `C-4` (note et octave).

Pour arrêter une note, insérez `NOTE OFF`, utilisez le FX d'arrêt de note, ou jouez une autre note sans instrument défini.

Lorsqu'une chaîne contient plusieurs phrases, l'écran Phrase fonctionne comme un éditeur de motif continu. Descendre sous la ligne `F` ou monter au-dessus de la ligne `0` ouvre la phrase suivante ou précédente de la chaîne.

Un astérisque (`*`) apparaît à côté d'une phrase réutilisée dans le projet. Vous pouvez cloner des phrases depuis l'écran Chain.

Le sélecteur de FX montre les commandes courantes ainsi que celles que l'instrument de cette ligne prend en charge. Cela évite une longue liste globale de commandes propres à chaque moteur. ChooChooTracker prend en charge plus de FX que ChipNomad.

### Commandes

- **OPT + EDIT** sur une note vide : insérer `NOTE OFF`
- **EDIT + [UP/DOWN]** sur une colonne de nom de FX : ouvrir l'écran de sélection de FX
- **OPT + [LEFT/RIGHT]** : passer d'une piste à l'autre
- **OPT + [UP/DOWN]** : passer d'une phrase à l'autre dans la chaîne courante
- Sélectionnez une plage dans la colonne d'instrument, puis **SHIFT + EDIT** : cloner les instruments
- Sélectionnez une plage, puis **EDIT + [UP/DOWN]** : faire pivoter les lignes de la phrase
- Sélectionnez une plage, puis touchez deux fois **EDIT** : ouvrir l'écran BOUNCE TO SAMPLE pour les lignes de phrase sélectionnées de la piste courante (voir Bounce sous Song)

### Key jazz (ordinateur uniquement)

Appuyez sur **Esc** sur l'écran Phrase pour activer ou désactiver le key jazz, un mode de saisie
de notes de type piano QWERTY (comme [m8c](https://github.com/laamaa/m8c)) :

- `Z S X D C V G B H N J M` et `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` saisissent des notes
  chromatiquement sur plus de deux octaves, en utilisant la position physique de chaque touche
  pour que la disposition soit la même sur les claviers AZERTY, QWERTY et QWERTZ.
- Chaque frappe écrit la note sur la ligne courante et passe à la ligne suivante.
- `[` / `]` décalent l'octave.
- **Shift + flèches** sélectionnent une plage de lignes et de colonnes ; une simple flèche
  ensuite efface la sélection.
- **Ctrl+C** / **Ctrl+X** copient ou coupent les colonnes sélectionnées, ou la
  note/instrument/volume de la ligne courante si rien n'est sélectionné ; **Ctrl+V** colle à
  la position du curseur.
- **Delete** retire la ou les lignes entières (toutes les colonnes) et remonte le reste de
  la phrase pour combler le trou ; le curseur reste sur la même ligne.
- **Backspace** retire l'élément sous le curseur dans la colonne courante
  seulement (ou les colonnes de la sélection, s'il y en a une), remonte tout ce qui
  se trouve en dessous pour combler le trou, et fait reculer le curseur d'une ligne, comme dans
  un éditeur de texte.
- **Insert** insère une ligne vide et repousse le reste de la phrase vers le bas.
- **Ctrl+S** enregistre le projet. En dehors du key jazz, Ctrl+S ne fait rien.
- Esc de nouveau désactive le key jazz.

Lorsque le key jazz est actif, il prend entièrement la main sur les touches de notes de l'écran
Phrase, y compris les raccourcis Edit/Opt/Motion qui partagent ces touches - désactivez-le
avec Esc pour les retrouver.

Le key jazz fonctionne aussi, activé indépendamment avec Esc, sur :

- **Song** et **Chain** : saisissez directement l'index hexadécimal d'une chaîne ou d'une phrase
  (`0-9`, `A-F`) au lieu d'incrémenter avec Up/Down. Le premier chiffre tapé
  sur une cellule remplace sa valeur ; les chiffres suivants tapés sans bouger sont décalés
  dedans (taper « 3 » puis « F » donne 3F). Sur Song, **Shift+flèches** sélectionnent une
  plage, **Delete** retire la ou les lignes entières et remonte le reste, **Backspace**
  retire la ou les colonnes courantes/sélectionnées et remonte ce qui se trouve en dessous,
  **Insert** insère une ligne vide, et **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  copient/coupent/collent les lignes sélectionnées (ou la ligne courante si rien n'est
  sélectionné) - ce sont les mêmes raccourcis d'édition de structure que sur l'écran Phrase.
  Cela prend la main sur Shift : Shift+Right/Up ne sautent donc plus vers Chain/Project
  tant que le key jazz est actif ici - Esc pour retrouver ces raccourcis.
- **Tout écran avec un champ de texte** : tapez directement au clavier
  au lieu d'utiliser le clavier virtuel à l'écran. Cela couvre le nom de fichier, le titre
  et l'auteur de Project, le nom de l'instrument, le nom du thème de couleurs,
  le nom de la table d'accordage, le nom de fichier du bounce, et les boîtes de dialogue de nom (saisie
  d'un nom, création de dossier). Backspace fonctionne comme dans un champ de texte normal ; Shift
  saisit des majuscules. Esc l'active sur tous ces écrans à la fois,
  et le clavier virtuel s'ouvre toujours normalement lorsque le key jazz est désactivé.

## 5. Instruments

Tous les instruments ont quelques paramètres communs :

- Type d'instrument
- Nom, jusqu'à 15 caractères
- Vitesse de table par défaut, en ticks par ligne de table
- Transposition activée ou non

Chaque instrument a une table par défaut portant le même numéro dans la plage `00-7F`. Vous pouvez les utiliser comme tables auxiliaires, mais garder les tables auxiliaires dans la plage `80-FE` évite les conflits et la confusion. Les tables sont parmi les principaux outils de conception sonore de ChooChooTracker.

### Commandes

- **OPT + [LEFT/RIGHT]** : passer d'un instrument à l'autre
- **EDIT + PLAY** : écouter l'instrument
- **SHIFT + OPT** : copier l'instrument
- **SHIFT + EDIT** : coller l'instrument

### AY Classic, AY Plus et AY Sample

Ce sont les moteurs AY/YM d'origine de ChipNomad. AY Classic expose des contrôles de tonalité, de bruit et d'enveloppe à la manière du matériel. AY Plus ajoute des oscillateurs logiciels et une modulation plus riche. AY Sample reproduit un échantillon à travers les niveaux de volume de type AY et se distingue du moteur Sampler.

#### AY Classic

C'est un type d'instrument hérité de ChipNomad `v1.0.0`. L'instrument AY Plus, plus récent, offre davantage de fonctions.

Les instruments AY Classic ont ces paramètres :

- Mixer : tonalité activée ou non, bruit activé ou non, forme d'enveloppe (`0-F`)
- Volume : enveloppe ADSR générée par logiciel
- Période d'enveloppe automatique : activée ou non, avec un rapport de `1:1` à `F:F`

**AY Quality** dans Settings n'affecte que le rendu AY/YM. Cela ne change pas la qualité de Braids, de Plaits ni de Sampler. **Sample dithering** s'applique à la quantification d'AY Sample.

Voir la [documentation ChipNomad sur l'AY-3-8910](https://chipnomad.org/chips/ay-3-8910/) pour les détails de la puce.

#### AY Plus

AY Plus applique les concepts classiques de la synthèse à la conception sonore de l'AY-3-8910/YM2149F. Vous pouvez voir l'AY comme un synthétiseur à 5 oscillateurs :

- 3 oscillateurs de tonalité à onde carrée
- 1 oscillateur de bruit
- 1 générateur d'enveloppe d'amplitude qui peut servir d'oscillateur

Selon ce modèle, chaque instrument a 3 oscillateurs matériels : tonalité, bruit et enveloppe. AY Plus en ajoute un 4e, logiciel. Mettre à jour les registres de l'AY plus vite que la cadence des ticks du projet ouvre davantage de possibilités de conception sonore. Du fait du fonctionnement de l'AY, les oscillateurs sont mélangés par modulation en anneau. Essayez différents décalages de hauteur, désaccords et combinaisons d'oscillateurs pour voir ce qui en sort.

Chaque oscillateur tonal peut être accordé indépendamment avec des décalages de hauteur et d'accord fin.

Types d'oscillateurs logiciels et leurs paramètres :

- Pulse - onde carrée à largeur d'impulsion réglable
  - FM Depth - profondeur de FM simple
  - Pulse width - la plage complète est `00-FF`, tandis que le nombre de pas se règle sur l'écran Project
  - Pulse low - règle la valeur basse d'une paire répétée dont la valeur haute est toujours `15`
- Sync Tone - effet de synchronisation forcée (hard sync) pour l'oscillateur de tonalité
  - FM Depth - profondeur de FM simple
- Sync Env - effet de synchronisation forcée pour l'oscillateur d'enveloppe
  - FM Depth - profondeur de FM simple
  - Pulse width - rapport de durée entre les 2 formes de la paire d'enveloppes
  - Envelope Pair - 2 formes d'enveloppe entre lesquelles basculer
- Wavetable - tables d'ondes de 32 pas, éditées sur l'écran Wavetable
  - FM Depth - profondeur de FM simple
  - Wavetable index
- Tone FM - effet de type FM simple pour l'oscillateur de tonalité
  - FM Depth - profondeur de FM sur la tonalité
- Env FM - effet de type FM simple pour l'oscillateur d'enveloppe
  - FM Depth - profondeur de FM sur l'enveloppe

Les oscillateurs logiciels Pulse et Wavetable ne peuvent pas être utilisés avec l'oscillateur Envelope. L'oscillateur Envelope se désactive automatiquement.

La FM simple bascule rapidement entre 2 valeurs de période, 1 plus basse et 1 plus haute. C'est comparable à l'emploi d'une onde carrée comme modulateur FM. Pulse, Sync Tone, Sync Env et Wavetable utilisent un rapport `1:1` entre la porteuse et le modulateur FM. Tone FM et Env FM peuvent imiter d'autres rapports grâce aux décalages de hauteur de l'oscillateur logiciel.

#### AY Sample

Vous pouvez charger des fichiers WAV mono ou stéréo en `8 bits`, `16 bits`, `24 bits` ou `32 bits`. Ils sont convertis en `8 bits` et tronqués à `16 Ko`. Cette limite correspond à la taille d'une page de RAM du ZX Spectrum.

L'AY joue les échantillons par des écritures rapides dans le registre de volume. Sa sortie est unipolaire : les échantillons doivent donc être « ramenés à zéro » pour une lecture plus propre. Cela supprime les clics de fin d'échantillon et améliore les queues de son faibles.

L'AY a une résolution de DAC de `4 bits` avec une échelle de volume non linéaire ; les échantillons de `8 bits` sont donc convertis en `4 bits` à la lecture. L'option Sample dithering dans Settings peut améliorer la profondeur de bits perçue. Le dithering n'est pas praticable sur des plates-formes rétro comme le ZX Spectrum et l'Atari ST : laissez-le désactivé lorsque vous écrivez pour du vrai matériel ou recherchez un son lo-fi croustillant.

Les échantillons se comportent comme un autre oscillateur logiciel d'AY Plus, mais leurs paramètres supplémentaires les rendent plus faciles à manier comme type d'instrument séparé.

Paramètres d'échantillon :

- Sample rate - jouer un échantillon à sa fréquence native produit un `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

Les sections d'oscillateurs Tone et Noise sont identiques à celles d'AY Plus. Du fait du fonctionnement de l'AY, tous les oscillateurs sont mélangés par modulation en anneau.

### Braids

Braids propose 47 modèles de synthèse. Ses principaux contrôles sont :

- **Model** : oscillateur ou algorithme de synthèse
- **Timbre** et **Color** : paramètres macro dépendant du modèle
- **Filter** : filtre supplémentaire LP/BP/HP, pente de `12` ou `24 dB`, fréquence de coupure et résonance
- **ADSR** : attack, decay, sustain et release

Touchez **Model** pour choisir dans des listes de modèles classées par catégories.

| Catégorie | Modèles |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Paramètres

Timbre et Color dépendent du modèle sélectionné. Reportez-vous au manuel original de Braids, de Mutable Instruments, pour la liste complète des modèles et leurs détails, ou jouez simplement à l'oreille.

#### VCF et VCA

Braids alimente le filtre multimode et l'enveloppe ADSR standard de ChooChooTracker.

#### Réglages

Les réglages globaux de Braids se trouvent dans les réglages de l'application :

- **BITS** : réduit la résolution de sortie
- **DRFT** : ajoute une instabilité de hauteur aux oscillateurs
- **SIGN** : reproduit l'algorithme d'imperfection de forme d'onde du Braids d'origine

### aChChid

**aChChid** est un moteur de basse acid monophonique basé sur Open303. `Square` et `Saw` utilisent l'oscillateur, le filtre, l'enveloppe et le comportement d'accent natifs du TB-303. `Braids` ne remplace que l'oscillateur, puis poursuit dans le même chemin de filtre et d'amplificateur 303. Il expose Model, Timbre, Color et Shaper à la place de Fine tune. Shaper passe progressivement d'une légère saturation au wavefolding. aChChid n'utilise ni le post-filtre unifié ni l'ADSR de ChooChooTracker.

Cutoff et Decay s'éditent de façon logarithmique : **EDIT + LEFT/RIGHT** avance d'un
pas de contrôle musical, tandis que **EDIT + UP/DOWN** avance de seize pas. Cutoff
couvre de 200 Hz à 20 kHz ; Decay couvre de 200 ms à 2 s.

Un `F` dans la colonne de volume de la note déclenche un accent. Les notes normales utilisent un gate d'un demi-pas (3 ticks dans le groove par défaut de 6 ticks). `ASL` glisse vers cette note depuis la hauteur précédente sans redéclencher l'enveloppe 303 ; il maintient automatiquement le gate précédent, et `ASL 00` donne un glissando de `60 ms`. `ATY` garde sa note ouverte pendant tout le pas courant. Les notes sans `ASL` redéclenchent toujours. Les destinations de modulation incluent Decay et Accent, plus Timbre et Color en mode d'onde Braids.

### Bogie

**Bogie** est un synthétiseur de batterie natif à déclenchement unique (one-shot). Les notes du tracker règlent la hauteur et le volume du tracker/de l'instrument règle le niveau ; chaque déclenchement a sa propre décroissance interne, il n'a donc pas de page ADSR. Il a 12 modèles : `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` et `Clave`.

Ses six macros sont **Decay**, **Tone**, **Sweep**, **Noise**, **FM** et **Drive**. Leurs libellés s'adaptent lorsque c'est utile (par exemple, le Noise de Kick devient `Click`, et le FM de Cowbell est une modulation croisée métallique). Chaque macro est active sur chaque modèle, mais son rôle musical change avec le modèle : Sweep peut modifier la hauteur, l'espacement des rafales ou l'étalement métallique ; FM peut ajouter une modulation croisée, une couche métallique ou une harmonique de type anneau. La moitié inférieure de chaque plage est destinée aux sons de batterie conventionnels ; la moitié supérieure ouvre volontairement des métamorphoses de timbre plus larges et des résultats plus synthétiques.

Chaque modèle peut utiliser le filtre multimode LP/BP/HP partagé, avec un caractère Clean, Classic, Aggro ou Acid, une pente de `12`/`24 dB`, une fréquence de coupure et une résonance. Bogie prend en charge toutes ses macros actives, plus Volume, Pitch, Cutoff et Resonance, comme destinations de modulation et d'enregistrement de mouvement. Utilisez un Decay court avec Cowbell pour des attaques phonk tranchantes ; augmentez FM pour une cloche plus dure et plus métallique.

Sur l'écran Modulation, appuyez sur **EDIT** sur une source pour choisir une famille (Envelopes, LFO ou Sticks), puis sa source. **Left** et **Right** parcourent toujours les sources directement.

### Sintered

**Sintered** est un synthétiseur de percussions numériques natif à déclenchement unique. Les notes règlent sa hauteur fondamentale ; ses six modèles sont `Knot`, `Shard`, `Burst`, `Comb`, `Logic` et `Melt`. Chaque modèle a **Decay**, **Mod** et **Motion**, plus trois contrôles propres au modèle : Knot utilise Ratio/Spread/Fold, Shard Ratio/Feedback/Bite, Burst Noise/Color/Feedback, Comb Time/Damping/Regen, Logic Rate/Pattern/Crush et Melt Ratio/Chaos/Drive. **Mod** est toujours la profondeur de couplage interne.

Motion est une enveloppe macro interne, pas un glissando de hauteur : les valeurs inférieures à `80` utilisent une forme AD, `80` la désactive, et les valeurs supérieures à `80` utilisent une forme à décroissance seule. S'éloigner du centre rend l'enveloppe plus rapide. Ses destinations sont fixées par modèle, de sorte que Sintered reste prévisible dans les motifs du tracker : Knot anime Mod/Fold ; Shard Feedback/Bite ; Burst Noise/Color/Feedback ; Comb Damping/Regen ; Logic Rate/Crush ; Melt Chaos/Drive. Tous les modèles réinitialisent leur état au déclenchement d'une note et bornent la rétroaction interne pour des coups reproductibles.

### MME

**MME** (Multi Modulation Engine) est un synthétiseur à deux oscillateurs dédié aux interactions entre oscillateurs. Ses modèles sont `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` et `Vocode`. Les deux oscillateurs sont internes et suivent les notes du tracker ; `Interval` règle leur relation.

Les six macros sont **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** et **Shaper**. Waves sélectionne des paires musicales de formes d'onde A/B. Amount contrôle l'interaction centrale du modèle, Flow change sa direction ou la variante de son algorithme, et Feedback est borné mais devient volontairement sauvage en fin de plage. Shaper est un étage final partagé qui passe du son propre à la saturation, puis au wavefolding.

MME utilise le filtre multimode LP/BP/HP partagé et l'ADSR. Les six macros, Cutoff et Resonance sont disponibles pour la modulation et l'enregistrement de mouvement. Maintenez **EDIT** sur Model pour ouvrir la fenêtre des modèles.

### MIDI Out

**MIDI Out** (ordinateur uniquement) pilote un appareil MIDI externe au lieu de synthétiser de l'audio : déclencher une note envoie un vrai Note On MIDI sur le **Channel** de l'instrument (`1-16`) puis, à la fin de la note, le Note Off correspondant. Le volume devient la vélocité, et chaque voix d'une piste d'accord reçoit ses propres Note On/Off. ChooChooTracker lui-même ne produit aucun son : choisissez d'abord l'appareil de sortie sous [Settings](#12-settings). Voir [MIDI](#14-midi) pour le tableau complet, y compris l'écoute d'un son depuis un clavier MIDI.

**Program** et **Bank high/low** (CC0/CC32) sont facultatifs - affichés `--` lorsqu'ils sont désactivés, `Clear` désactive un champ en mémorisant sa valeur. Lorsqu'ils sont définis, ils sont envoyés une fois, juste avant la note suivante, chaque fois qu'ils diffèrent de ce qui a été dit en dernier à ce canal (pas avant chaque note, ce qui redéclencherait les enveloppes ou le patch propres à l'appareil récepteur).

**MC1 number**-**MC4 number** choisissent le numéro de CC (`00-7F`) vers lequel chacun des quatre FX de ligne MC1-MC4 envoie - voir [MIDI Out FX](#fx-midi-out). Également `--`/facultatif ; un emplacement laissé vide rend son FX inerte.

### Moteurs soustractifs

Les moteurs de cette catégorie partagent une architecture VCO vers VCF vers VCA.

Plusieurs types de VCO sont disponibles :

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Chaque instrument envoie son VCO dans un filtre multimode.

Le filtre a une pente commutable de `12 dB` ou `24 dB` et peut fonctionner en passe-bas (LP), passe-bande (BP) ou passe-haut (HP).

Plusieurs caractères de filtre sont disponibles :

- **Clean** : stérile et numérique
- **Classic** : inspiré des synthétiseurs analogiques américains classiques
- **Aggro** : inspiré des synthétiseurs analogiques japonais rugissants
- **Acid** : inspiré des psychédéliques

Classic, Aggro et Acid peuvent devenir très résonants dans cette version alpha. Commencez bas et montez doucement.

Le filtre alimente un VCA contrôlé par une enveloppe ADSR. La forme de l'enveloppe peut évoluer entre des courbes exponentielle, linéaire et logarithmique.

#### Plaits et Plaits-Alt

**Plaits** propose 24 moteurs, regroupés dans la fenêtre de sélection comme suit :

| Catégorie | Moteurs |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** est une collection de modèles alternatifs pour le module Plaits d'origine. La plupart sont plus étranges ou plus expérimentaux que le VCO moyen.

| Catégorie | Moteurs |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

Les paramètres suivent la conception de Mutable :

- **Harmonic** contrôle les relations harmoniques, l'équilibre ou le choix du modèle à l'intérieur d'un moteur.
- **Timbre** va généralement de spectres sombres/clairsemés à des spectres brillants/denses.
- **Morph** explore une autre dimension du timbre.
- **Main/Aux** mélange la sortie principale avec la sortie alternative du moteur.

Leur sens précis dépend du moteur. Par exemple, les moteurs d'accords s'en servent pour le type d'accord, le renversement et la forme d'onde. Les modèles physiques s'en servent pour le matériau, l'excitation et la décroissance, tandis que les moteurs de batterie s'en servent pour le ton, le caractère et la décroissance. Reportez-vous au manuel d'utilisation de Plaits pour les détails.

Pour `CHORD`, `DIATONIC CHORD` et `WT DIATONIC CHORD`, Harmonic affiche le nom de l'accord sélectionné (`MIN`, `MAJ`, `MIN7`, `MAJ7`, etc.) plutôt que sa valeur de contrôle brute. Le `WAVE PARA` de Braids nomme de la même façon ses voicings de Color.

Touchez **Engine** pour choisir dans des listes de moteurs classées par catégories.

**Env Mode** a 2 routages :

- `TRIG` reproduit le module avec TRIG connecté et LEVEL non branché
- `VCA` maintient LEVEL ouvert et applique l'ADSR du tracker après la voix

#### Sampler

Ce moteur d'échantillons propre joue des échantillons PCM mono ou stéréo chargés en RAM.

- Touchez **Sample** pour charger un WAV PCM non compressé de `8 bits`, `16 bits` ou `24 bits`. Appuyez sur **PLAY** dans le navigateur pour écouter le fichier en surbrillance.
- Lorsqu'un échantillon est chargé, **EDIT** apparaît à côté de son nom. Touchez-le pour ouvrir l'écran Sample Edit. **OPT** ou **SELECT + [LEFT]** revient à l'instrument.
- L'écran Sample Edit affiche le nom de fichier (avec un marqueur `*` lorsque l'échantillon en RAM diffère du fichier sur disque), un indicateur de format (fréquence d'échantillonnage, canaux), une grande forme d'onde, et les champs **Region**, **Select**, **Slice**, **Process** et **File**.
- Les modifications ne vivent en RAM que jusqu'à ce qu'un enregistrement les écrive : le projet mémorise le chemin du fichier de l'échantillon et recharge le WAV depuis le disque au prochain chargement du projet, si bien que quitter le tracker sans **Save**/**Save As** abandonne les opérations de traitement (le marqueur `*` est l'avertissement). Charger un autre échantillon dans l'instrument abandonne aussi les modifications non enregistrées et remet la Region sur l'échantillon entier (Start `00`, End `FF`).
- **Region** règle les marqueurs Start/End de lecture, également disponibles sur l'écran de l'instrument Sampler. Ils fixent des limites de lecture normalisées (`00-FF`) ; si Start est après End, l'échantillon est joué à l'envers. **EDIT + [LEFT/RIGHT]** ajuste finement le marqueur par pas de un (`01`) et zoome sur la forme d'onde autour de lui tant que **EDIT** est maintenu ; **EDIT + [UP/DOWN]** ajuste grossièrement (pas de 16) et ramène la vue sur l'échantillon entier. **EDIT + OPT** remet le marqueur à sa valeur par défaut (Start `00`, End `FF`).
- La vue de la forme d'onde zoome sur une fenêtre fixe d'une seconde d'audio autour du marqueur édité et se déplace pour le garder visible ; les échantillons qui tiennent dans la fenêtre restent en vue complète 1:1. Le zoom est transitoire : il dure tant que **EDIT** est maintenu, et relâcher **EDIT** ramène à la vue complète 1:1, tout comme les pas grossiers. Entrer dans l'écran remet toujours la vue sur l'échantillon entier.
- **Select** fixe une sélection de traitement en frames absolues, indépendante des marqueurs Start/End de lecture. **EDIT + [LEFT/RIGHT]** déplace une poignée de vingt frames et zoome dessus tant que **EDIT** est maintenu ; **EDIT + [UP/DOWN]** saute de `frameCount/64` (minimum 16) et ramène à la vue de l'échantillon entier. **EDIT (appui bref)** sur une poignée lui copie la position actuelle du marqueur Start ou End. **EDIT + OPT** sur l'une ou l'autre des poignées vide toute la sélection. Lorsque la sélection est vide, les deux poignées affichent `-` ; lorsque les poignées sont inversées, elles s'échangent automatiquement. La sélection est un état d'éditeur propre à la session : elle n'est pas enregistrée avec le projet, et l'entrée dans l'écran l'initialise avec l'étendue de la Region de lecture (l'échantillon entier avec les marqueurs par défaut).
- **Slice** est une ligne unique de quatre cellules de valeur - **Mode**, **Count**,
  **Slice** et **Frame** - et est enregistrée avec l'instrument. Off joue la
  fenêtre Start/End. Les notes de phrase sélectionnent les tranches de façon chromatique à partir de **C-0**, et
  les notes au-delà de la dernière tranche reviennent au début (la note N joue la tranche
  `N % count`, si bien qu'une suite qui dépasse la dernière tranche reboucle sur la première).
  Les notes découpées ne transposent pas la hauteur et n'utilisent pas la quantification Scale, mais **CRD**
  conserve la tranche sélectionnée et transpose ses voix comme un accord. Les débuts de tranche
  sont repérés sur la forme d'onde par de courtes lignes de deux pixels de large (orange foncé ; la ligne
  de la tranche courante est plus claire), et la tranche sous le curseur reçoit une
  bande de fond noire sur la forme d'onde (la teinte de sélection n'est pas dessinée sur
  la bande, qui reste donc entièrement noire et nettement distincte du reste de
  l'aperçu).
  - **Mode** parcourt `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + gauche/droite ou
    appui bref ; EDIT + OPT désactive le découpage). Changer de mode initialise les points
    de tranche : **EQUAL** divise la fenêtre Start/End en **Count** parts égales ;
    **AUTO** lance une détection de transitoires sur la fenêtre et place une
    tranche sur chaque attaque trouvée (jusqu'à **Count**) ; **LAZY** démarre avec une
    tranche couvrant toute la fenêtre. Passer à LAZY met de côté le réglage
    EQUAL/AUTO courant (nombre et bornes), et revenir à EQUAL ou
    AUTO le restaure tel quel - aucune confirmation n'est demandée et rien n'est
    perdu. Slice et Stretch s'excluent mutuellement : activer l'un désactive
    l'autre, et les cellules Slice sont grisées tant que Stretch pilote la
    durée.
  - **Count** affiche le nombre total de tranches (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** le change d'un pas (minimum 1, maximum 64), **EDIT +
    [UP/DOWN]** parcourt les nombres en puissances de deux `2 / 4 / 8 / 16 / 32 /
    64` avec bouclage. Chaque changement recalcule la division : **EQUAL**
    redivise la fenêtre en parts égales, **AUTO** relance la détection avec la
    nouvelle cible (la sensibilité de détection est déduite du nombre - plus de tranches
    signifie un seuil plus sensible). La case affiche `-` lorsque le mode est
    OFF. En **LAZY**, elle reste lumineuse (elle compte les tranches placées à la main) mais
    sert uniquement à l'affichage : le curseur l'évite - aller à droite depuis **Mode** ou
    à gauche depuis **Slice** tombe sur **Slice**, descendre depuis le marqueur **END**
    de la ligne **Select** y tombe aussi, et toucher la case ne fait rien
    (les tranches placées à la main n'ont pas de division à recalculer). **EDIT + OPT** et
    un simple appui bref sur **EDIT** ne font rien ici : désactiver le découpage se fait
    sur **Mode**, supprimer une tranche se fait sur **Slice**.
  - **Slice** est le navigateur : il affiche la tranche courante, numérotée à partir de 1
    (`01`..count). **EDIT + [LEFT/RIGHT]** passe d'une tranche à l'autre une par
    une, **EDIT + [UP/DOWN]** saute de quatre tranches (dans les deux cas bornés, sans
    bouclage) ; la tranche parcourue est indiquée sur la forme d'onde par son marqueur
    plus clair et la bande noire, et la vue zoomée se recentre sur le début de
    la tranche. **EDIT + OPT** supprime la tranche courante (elle fusionne avec
    la précédente ; la première tranche ne peut pas être supprimée, et supprimer la
    dernière tranche restante désactive le découpage - les bornes survivent à un aller-retour
    par OFF et reviennent lorsqu'un mode est de nouveau choisi). **EDIT (appui bref)**
    écoute la tranche actuellement sélectionnée en lecture unique (fonctionne dans
    tous les modes de tranche ; l'écoute s'arrête lorsque la touche est relâchée ou que vous
    quittez l'écran). La case affiche `-` lorsque le mode est OFF.
  - **Frame** affiche en hexadécimal la frame de début de la tranche courante. **EDIT +
    [LEFT/RIGHT]** la décale de dix frames (en zoomant sur le marqueur tant que
    **EDIT** est maintenu), **EDIT + [UP/DOWN]** la décale de cent et
    ramène à la vue complète. Un marqueur reste à l'intérieur de sa propre tranche : il
    ne peut pas franchir le début de la tranche précédente ou suivante. **EDIT + OPT** supprime
    la tranche.
  - **B** revient à l'écran de l'instrument depuis la ligne Slice, comme partout
    ailleurs sur cet écran ; **SHIFT + LEFT** en sort depuis n'importe où.
  - **Flux de travail LAZY** : avec LAZY sélectionné, touchez **PLAY** pour démarrer une
    lecture unique de l'échantillon entier - il se joue une fois depuis le début et s'arrête
    de lui-même, à la hauteur et à la vitesse d'origine de l'échantillon (un marqueur vert de deux pixels
    suit la position sur la forme d'onde) ; toucher **PLAY** de nouveau
    l'arrête plus tôt. Pendant la lecture,
    chaque appui sur **EDIT** dépose une tranche à la position de lecture, où que
    soit le curseur (les tranches à moins de 50 ms d'une tranche existante sont
    refusées avec `Too close to slice`). Rejouer l'échantillon conserve
    les découpes existantes et en ajoute de nouvelles à chaque position tapée, dans
    l'ordre - la case **Count** se met à jour à chaque tranche déposée et le
    navigateur **Slice** suit la nouvelle. Les mêmes règles de navigation et d'édition que
    dans les autres modes s'appliquent aux tranches placées à la main : **EDIT +
    direction** sur **Slice** les parcourt, **Frame** ajuste le début
    sélectionné, **EDIT + OPT** supprime la tranche courante. La cellule Frame est grisée
    tant que le dépôt pendant la lecture est armé. **SHIFT + PLAY** n'est pas intercepté
    en LAZY : il lance la lecture de phrase comme sur tous les autres écrans. Les tranches
    LAZY sont jouées dans le séquenceur exactement comme EQUAL et AUTO : les notes de phrase
    sont associées chromatiquement à partir de **C-0** aux tranches placées à la main,
    avec bouclage au-delà de la dernière.
  - **Indices de combinaisons** : tant que le curseur repose sur une cellule de la ligne Slice et qu'aucun autre
    message n'est affiché, la barre d'état montre l'indice de cette cellule et le maintient
    tant que le curseur y reste. Sur **Mode**, l'indice est lié au
    mode actif et n'affiche que la description (le nom du mode est déjà dans
    la cellule) : `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - changer de mode
    remplace le texte immédiatement, et OFF vide la barre. Sur **Count**, elle
    affiche `Adjust the number of slices`, sur **Slice**
    `Browse slices`, sur **Frame** `Adjust slice start`. Tout message d'action
    remplace l'indice jusqu'à son expiration.
- **Process** sélectionne une opération d'édition destructive : **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** ou **Reverse** (EDIT + gauche/droite fait défiler, l'appui bref avance, EDIT + OPT n'en choisit aucune). **GO** (sur la même ligne) exécute l'opération choisie sur la sélection courante, ou sur l'échantillon entier lorsque la sélection est vide. **Crop** ne garde que la sélection ; **Normalize** met la sélection à l'échelle pour que son pic atteigne la pleine échelle (les deux canaux partagent un seul gain afin de préserver l'image stéréo) ; **Delete** supprime la sélection et raccorde les bouts (supprimer l'échantillon entier est refusé) ; **Silence** la met à zéro ; **Fade In**/**Fade Out** font monter ou descendre la sélection linéairement depuis ou vers le silence ; **Reverse** joue la sélection à l'envers (les frames sont échangées sur place, les deux canaux d'une frame se déplacent ensemble, la longueur et les marqueurs ne changent pas). Crop et Delete exigent une sélection - avec une sélection vide, ils affichent `Select region first`. Chaque opération met brièvement l'audio en pause, conserve un niveau d'annulation, et marque l'échantillon comme modifié en RAM : le fichier sur disque n'est pas touché avant les opérations Save (voir ci-dessous), et quitter l'écran abandonne l'emplacement d'annulation.
- **UNDO** (à côté de GO) remplace l'échantillon par l'état précédant la dernière opération. Il est grisé tant qu'aucune opération n'a été exécutée, bascule entre les états avant et après l'opération à chaque pression répétée, et est vidé lorsqu'on rentre de nouveau dans l'écran.
- **File** regroupe les opérations d'enregistrement. L'instrument mémorise le chemin complet du WAV d'où il a été chargé ; ces opérations écrivent ce fichier ou font pointer l'instrument vers un nouveau. Elles ne touchent jamais au nom de l'instrument.
  - Le champ File alterne entre **Save** et **Save As** (EDIT + gauche/droite ou appui bref) ; **GO** (sur la même ligne) exécute l'action affichée.
  - **Save** écrase le WAV d'où l'échantillon a été chargé. Pour un échantillon
    sans tranches, il demande d'abord `Overwrite <name>?`. Un échantillon découpé ouvre d'abord
    la boîte de dialogue **SAVE SLICES**, qui demande à quoi appartiennent les points de tranche :
    **SAVE TO SAMPLE** les écrit dans le WAV sous forme de blocs cue (portable -
    les DAW les affichent comme des marqueurs) et les garde aussi dans le projet,
    **SAVE TO PROJECT** écrit un WAV simple et garde les points
    dans le projet seulement, **CANCEL** annule. Une case `Don't ask again in this project`
    enregistre la destination choisie avec le projet et ignore ensuite
    la boîte de dialogue (le choix est enregistré sous `- Sample save choice:` dans
    le .cct et ne peut être changé qu'en modifiant le fichier du projet). La
    boîte de dialogue est ignorée pour les échantillons sans tranches. **Save** est grisé (et
    sauté dans la navigation) tant qu'aucun échantillon ayant un chemin de fichier n'est chargé. Une
    écriture qui échoue conserve le marqueur de modification et affiche l'erreur.
  - **Save As** écrit l'échantillon courant dans un nouveau fichier : saisissez un nom de fichier (prérempli avec le nom actuel sans extension), puis choisissez un dossier dans le navigateur. L'échantillon est écrit sous `<folder>/<name>.wav`, l'instrument pointe vers le nouveau fichier, et le dossier est mémorisé comme dossier d'échantillons par défaut. Il est grisé (et sauté dans la navigation) tant que des données d'échantillon ne sont pas chargées, de sorte qu'un échantillon fraîchement chargé puisse recevoir un chemin de fichier. Les chemins de plus de 255 caractères sont refusés.
  - Le marqueur `*` devant le nom de fichier signifie que l'échantillon en RAM diffère du fichier sur disque (toute opération de traitement le positionne, Save et Save As l'effacent). Le marqueur est propre à la session : il n'est pas enregistré avec le projet et se réinitialise lorsqu'on rentre de nouveau dans l'écran. Si plusieurs instruments référencent le même fichier WAV, en enregistrer un écrase le fichier pour tous.
- Sur l'écran de l'instrument Sample, utilisez **EDIT + [LEFT/RIGHT]** pour charger le WAV précédent ou suivant du même dossier.
- **Pitch** transpose par demi-tons (`-48` à `+48`).
- **Loop** choisit Off, Loop ou Ping-Pong.
- **Speed** contrôle l'étirement temporel granulaire de `0%` à `500%` (`100%` est normal).

Les formats WAV non pris en charge affichent une erreur. Convertissez les fichiers inhabituels en WAV `PCM8`, `PCM16` ou `PCM24` avant de les importer.

#### 2xSCWF

2xSCWF est un oscillateur à double forme d'onde à cycle unique. Chargez dans chaque oscillateur 1 WAV mono contenant exactement 1 période. Les deux oscillateurs lisent vers l'avant et rebouclent à la fin de leur cycle.

Mix fait un fondu enchaîné entre A et B.

Detune est fin et exponentiel de l'unisson jusqu'à `+200 ct`, puis progresse par demi-tons de `+3 st` à `+24 st`.

2xSCWF traite chaque fichier comme une table à 1 cycle et le lit exactement une fois par cycle d'oscillateur. Cela permet de mélanger et de désaccorder 2 ondes à cycle unique.

Le dossier d'usine `waveforms/AKWF/` contient des formes d'onde de la bibliothèque Adventure Kid. Beaucoup d'autres sont disponibles en ligne si vous voulez enrichir la collection.

#### BYOWTBL

BYOWTBL est un oscillateur à double table d'ondes compatible avec les tables Serum. Chaque oscillateur charge un WAV mono contenant des frames à cycle unique consécutives. `Pos A` et `Pos B` parcourent leurs tables indépendamment de `00` à `FF`. Le moteur interpole linéairement à l'intérieur de chaque frame et entre frames adjacentes. `Mix` fait un fondu enchaîné entre les oscillateurs, et `Detune` décale l'oscillateur B.

## 6. Modulation et enregistrement de mouvement

Chaque instrument peut avoir jusqu'à 4 slots de modulation. Les instruments audio peuvent cibler **Instrument Pan** ou **Track Pan** ; tous deux utilisent `00` pour la gauche, `80` pour le centre et `FF` pour la droite.

Les types de modulation disponibles sont :

- ADSR - enveloppe classique attack-decay-sustain-release
- AHD - enveloppe attack-hold-decay
- LFO - oscillateur basse fréquence à plusieurs formes
- SLFO - LFO avec un multiplicateur de période pour les cycles lents
- FLFO - LFO à fréquence audio pouvant atteindre le domaine des kHz ; le FLFO peut être lourd pour le CPU
- SLIN - contrôle linéaire par joystick
- SRAT - contrôle par joystick basé sur la vitesse de variation

La liste des destinations de modulation dépend du type d'instrument.

Pour une destination `Cutoff`, Amount est exponentiel avec une réponse quadratique : 25 % atteint 4,5 demi-tons, 50 % atteint 18 demi-tons, 75 % atteint 40,5 demi-tons, et 100 % atteint six octaves. Cela garde un contrôle fin aux faibles valeurs tout en permettant des balayages profonds.

Lorsqu'une enveloppe ADSR ou AHD cible Volume, elle devient l'enveloppe de volume de l'instrument. Un LFO ciblant Volume décale à la place le volume de sortie courant.

Le montant de modulation peut être positif ou négatif. Sa plage dépend de la destination. Si la plage complète de la destination est de `127` ou moins, comme pour la plupart des paramètres, le montant définit une plage absolue. Les destinations plus larges, comme la hauteur, utilisent un montant mis à l'échelle.

Tous les paramètres de durée, y compris attack, hold, decay et période, se mesurent en ticks.

Formes de LFO :

- Tri - onde triangulaire bipolaire
- Sin - onde sinusoïdale bipolaire
- UniTri - onde triangulaire unipolaire
- UniSin - onde sinusoïdale unipolaire
- RampDn - rampe linéaire descendante (onde en dents de scie)
- RampUp - rampe linéaire montante (onde en dents de scie)
- ExpDn - rampe exponentielle descendante (dents de scie exponentielle)
- ExpUp - rampe exponentielle montante (dents de scie exponentielle)
- Square - onde carrée
- Random - onde aléatoire échantillonnée-bloquée (sample-and-hold)
- Wavetable - lit l'une des tables d'ondes AY de 32 pas du projet. Les valeurs `00` à `0F` correspondent de `-100%` à `+100%`, sans interpolation.

Types de déclenchement du LFO :

- Free - ne redémarre le LFO que lorsque l'instrument change
- Retrig - redémarre le LFO à chaque note
- Phrase - redémarre LFO et SLFO lorsque la lecture entre dans une nouvelle phrase
- Chain - redémarre LFO et SLFO lorsque la lecture entre dans une nouvelle chaîne
- Hold - s'arrête après 1 cycle et conserve la dernière valeur
- Once - s'arrête après 1 cycle et revient à zéro

### Enregistrement de mouvement (Motion recording)

- **STICK LIVE** (`L1` par défaut) applique la modulation des sticks sans modifier la phrase. Dans **Settings > Stick live mode**, `HOLD` (le défaut) l'active tant qu'il est maintenu ; `TOGGLE` l'active à une pression et le désactive à la suivante ; `FREE` le laisse activé en permanence. Relâcher le bouton en `TOGGLE` le laisse activé.
- Pendant la lecture, maintenez **MOTION RECORD** (`L2`) pour appliquer la modulation des sticks et enregistrer les destinations modifiées sous forme de valeurs de FX absolues.
- Maintenez **MOTION ERASE** (`R2`) pour retirer de la ligne courante les FX de destination correspondants.

Le `~` en bas à droite indique que Stick live est actif. Record (`*`, ou `!` en cas de débordement) et Erase (`x`) ont la priorité d'indicateur et restent momentanés dans les deux modes. Les deux activent le traitement des sticks en direct tant qu'ils sont maintenus ; les relâcher laisse intact un basculement live activé séparément.

Le mode est enregistré avec les réglages de l'application, mais le basculement live démarre toujours désactivé au lancement. Changer de mode efface le basculement : `HOLD` suit tout bouton Stick live actuellement maintenu, `TOGGLE` attend une nouvelle pression, et `FREE` est actif immédiatement.

L'enregistrement de mouvement écrit des FX de piste dans la phrase en cours de lecture. Il met d'abord à jour les FX correspondants, puis utilise les emplacements vides de droite à gauche. Il n'écrase jamais un FX différent. Si les 3 emplacements sont pleins, ce mouvement n'est pas enregistré sur le pas. Un `!` dans le coin inférieur droit signifie que plus de destinations ont changé que les 3 colonnes de FX ne pouvaient en contenir.

L'enregistrement de mouvement prend en charge les destinations de Braids, Plaits, Sampler, 2xSCWF, BYOWTBL et Bogie, y compris les macros Bogie actives et les contrôles de filtre le cas échéant.

## 7. Tables

Les tables sont un outil de conception sonore central dans les trackers. Dans le vocabulaire de Vortex Tracker, elles combinent instruments et ornements, mais elles peuvent faire bien plus. Si vous connaissez LSDj ou NerdSEQ, l'idée devrait déjà vous être familière.

La colonne Pitch accepte des hauteurs relatives (`~`) ou absolues (`=`) en demi-tons. La colonne Volume va de `00` à `0F`, s'applique à tous les moteurs par-dessus leur enveloppe, et peut servir de gate : alternez `0F` et `00`, puis bouclez avec `HOP 00`. Les 4 voies de FX correspondent pour l'essentiel aux voies d'une phrase, même si quelques FX se comportent différemment dans les tables.

Placer un FX `TIC` sur la dernière ligne de la table règle la vitesse de cette colonne et remplace la vitesse de table par défaut de l'instrument. Chaque colonne de FX peut tourner à une vitesse de ticks différente. Les colonnes Pitch et Volume suivent la vitesse de la 1re colonne de FX.

Une table de 16 lignes peut paraître courte à côté d'un long instrument de Vortex Tracker, mais le FX `HOP` peut créer des boucles conditionnelles comme « répéter ces lignes 5 fois », ainsi que des boucles imbriquées. Avec des boucles et des vitesses de colonnes de FX indépendantes, ces 16 lignes peuvent aller étonnamment loin.

### Mode Retrigger

Le champ `Retrig` en haut de l'écran Table choisit quand une table déjà active redémarre. `Inst` est le mode par défaut et compatible : une ligne de phrase contenant un instrument sélectionne la table par défaut de cet instrument et la démarre à la ligne `00`. `Phrase` redémarre une table active lorsque la lecture entre dans une phrase, `Chain` lorsqu'elle entre dans une chaîne, et `Free` la laisse tourner jusqu'à l'arrêt de la piste ou jusqu'à ce qu'une commande explicite la remplace.

Un mode structurel ne remonte jamais dans le temps. Si une piste n'a pas de table d'instrument active, la première ligne de phrase qui introduit un instrument active sa table exactement à cette ligne, même s'il s'agit de la ligne `4`, `8` ou `F` d'une phrase. Les numéros d'instrument suivants ne remplacent pas une table `Phrase`, `Chain` ou `Free` active. Cela rend les tables utiles comme automation de type LFO, sans simuler silencieusement des pas de phrase antérieurs.

`TBL`, `TBX` et `RET` sont toujours des actions explicites : ils sélectionnent ou redémarrent immédiatement, quel que soit le mode. À une limite de Phrase ou de Chain, son redémarrage a lieu avant la lecture de la nouvelle ligne de phrase : un `TBL` ou `TBX` sur cette ligne a donc le dernier mot. Les FX de la ligne `00` de la table s'exécutent donc de nouveau normalement ; les FX cumulatifs gardent leur accumulation habituelle.

Les tables `00-7F` sont réservées aux tables d'instruments par défaut. Les tables `80-FE` sont destinées aux tables auxiliaires lancées par l'effet `TBX`. La plage basse fonctionne aussi pour des tables auxiliaires, mais l'utiliser ainsi peut provoquer des conflits et de la confusion inattendus.

### Copier des FX entre Phrase et Table

Sélectionnez uniquement des colonnes de nom/valeur de FX, puis copiez-les ou coupez-les depuis une Phrase et collez-les dans n'importe quelle voie de FX d'une Table, ou inversement. La copie conserve exactement les cellules sélectionnées : une sélection de commandes seules ou de valeurs seules reste donc partielle. Le collage commence à la cellule de FX courante et est tronqué à la ligne `F` et à la dernière voie de FX de la destination. Les notes, les numéros d'instrument, la hauteur et le volume ne passent jamais entre ces deux écrans.

### Commandes

- **EDIT + [UP/DOWN]** sur une colonne de nom de FX : ouvrir l'écran de sélection de FX
- **OPT + [DIRECTION]** : passer d'une table à l'autre

## 8. Autres écrans

### Instrument Pool

L'Instrument Pool affiche tous les instruments du projet et permet de les réordonner. Pendant la lecture, il montre aussi quels instruments sont actuellement actifs.

#### Commandes

- **EDIT** : éditer l'instrument et passer à l'écran Instrument
- **SHIFT + OPT** : copier l'instrument
- **SHIFT + EDIT** : coller l'instrument
- **EDIT + [UP/DOWN]** : réordonner les instruments
- **EDIT + PLAY** : écouter l'instrument

### AY Wavetable

Wavetable est un type d'oscillateur logiciel d'AY Plus. Un projet peut contenir jusqu'à 256 tables d'ondes, de 32 pas chacune. Tous les instruments à table d'ondes partagent le même jeu d'ondes. Cet écran sert à les éditer.

Les niveaux de volume de l'AY sont non linéaires : la forme d'onde de la vue Standard est donc dessinée pour correspondre aux niveaux de sortie réels. Touchez **OPT** deux fois pour passer entre la vue Standard et la **vue LFO** mémorisée, avec le même raccourci que Song/Live Mode : la vue LFO dessine linéairement les pas bruts de `0` à `15`, la ligne centrale entre `7` et `8` étant le zéro de la modulation.

L'écran a 2 lignes logiques. La ligne du haut contient les boutons **Load** et **Save**, tandis que la 2e contient l'éditeur de table d'ondes.

#### Commandes

- **OPT + [LEFT/RIGHT]** : parcourir la liste des tables d'ondes
- **OPT + [UP/DOWN]** : parcourir la liste par pas de 16 ondes
- **SHIFT + OPT** : copier la table d'ondes
- **SHIFT + EDIT** : coller la table d'ondes
- **OPT + PLAY** : basculer entre les vues Standard et LFO

## 9. FX de piste (Tracker FX)

Les FX de piste forcent des valeurs du séquenceur ou du moteur sonore sur des pas individuels. D'autres grooveboxes les appellent souvent « parameter locks ». Chaque ligne de Phrase a 3 emplacements de FX.

Chaque FX a une commande de 3 lettres et une valeur hexadécimale. Le panneau d'aide intégré à l'application donne une courte description de la commande sélectionnée.

### Automation de la gamme (Scale)

`SCL XY` change la gamme et la fondamentale globales de lecture sans modifier les notes enregistrées dans les phrases. `X` sélectionne une gamme (`0` Chromatic, `1` Major, `2` Minor, jusqu'à `C` Custom) ; `Y` sélectionne la fondamentale (`0` C jusqu'à `B` B). La commande ne s'applique qu'aux FX de phrase, pas aux tables. Si plusieurs pistes émettent `SCL` au même tick, la piste de plus petit numéro l'emporte.

`SCL` relève du mode Quantizer. Lorsque **Note Lock** est actif, la commande n'est pas proposée dans la liste des FX, et tout `SCL` déjà écrit dans des phrases est ignoré tant que ce mode est activé.

### Accords (Chords)

`CRD XY` transforme la note de sa propre ligne de Phrase en accord. `Y` sélectionne : `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9 ou `F` Min9. `X` sélectionne l'un des 16 voicings : `0-3` forment un cycle de renversements serrés ascendants ; `4-7` ajoutent le drop-2 ; `8-B` ajoutent le drop-3 ; et `C-F` combinent les deux drops. Le cycle se poursuit sur une octave supplémentaire pour les accords de trois notes et les power chords, de sorte que chaque valeur est distincte ; les valeurs plus grandes deviennent progressivement plus larges et moins conventionnelles. Par exemple, `CRD 4Y` est la position fondamentale en drop-2 et `CRD DY` est le premier renversement avec les deux drops. En bas ou en haut de la table de hauteurs, les voix sont bornées à sa plage. La commande n'est pas persistante : écrivez-la sur chaque ligne en accord. Chaque note générée est quantifiée indépendamment lorsque Scale est actif, dans les modes Quantizer et Note Lock. Elle n'est disponible que pour les moteurs logiciels ; les instruments AY l'ignorent et restent monophoniques.

### FX du séquenceur

| FX | Valeur | Comportement détaillé |
|---|---|---|
| `ARP` | `XY` | Arpège de la note de base, des pas `+X` et des pas `+Y`. `37` produit un motif d'accord mineur. |
| `ARC` | `XY` | `X` choisit la direction de l'arpège ou le mode de plage ; `Y` est sa vitesse en ticks. |
| `PVB` | `XY` | Vibrato de hauteur : `X` est la vitesse et `Y` la profondeur. En mode Linear, la profondeur utilise des pas de `10 cents`. |
| `PBN` | `XX` signé | Ajoute `XX` unités de hauteur à chaque ligne de phrase/table ; `FF` signifie `-1`. Utilisez `00` pour arrêter. |
| `PSL` | `XX` ticks | Glisse de la hauteur précédente vers la nouvelle note en `XX` ticks. |
| `PIT` | `XX` signé | Décalage relatif cumulé, en pas de table de hauteurs. |
| `FIN` | `XX` signé | Décalage fin cumulé, en cents avec la hauteur Linear, en unités de période sinon. |
| `PRD` | `XX` signé | Décalage relatif cumulé de la période de l'oscillateur. |
| `VOL` | `XX` signé | Décalage relatif cumulé du volume (`FF` vaut -1, pas le plein volume). |
| `VSL` | `XX` signé | Ajoute `XX` au volume à chaque ligne de phrase/table. Utilisez `00` pour arrêter. |
| `PAN` | `00-FF` | Panoramique absolu de l'instrument : gauche, centre (`80`), droite. |
| `TPN` | `00-FF` | Panoramique absolu de la piste du mixeur : gauche, centre (`80`), droite. |
| `RET` | `XY` | Redéclenche tous les `Y` ticks ; `X` applique un changement de volume. `Y=0` arrête le redéclenchement. |
| `DEL` | `XX` ticks | Retarde le note-on. Un retard plus long que le pas de groove courant fait sauter la note. |
| `OFF` | `XX` ticks | Envoie le note-off après `XX` ticks et entre dans une phase de release ADSR. |
| `KIL` | `XX` ticks | Coupe brutalement la voix après `XX` ticks sans exécuter le release ADSR. |
| `TIC` | `XX` ticks | Règle les ticks de table par ligne. Dans une table, il change la vitesse de cette colonne de FX. |
| `TBL` | `00-FE`, `FF` arrêt | Remplace et redémarre immédiatement la table de l'instrument ; `FF` l'arrête. |
| `TBX` | `00-FE`, `FF` arrêt | Démarre ou remplace immédiatement une table auxiliaire ; `FF` l'arrête. |
| `THO` | ligne `XX` | Fait sauter toutes les colonnes de la table d'instrument à la ligne `XX`. |
| `TXH` | ligne `XX` | Fait sauter toutes les colonnes de la table auxiliaire à la ligne `XX` ; il ne s'utilise pas depuis l'intérieur d'une table. |
| `GRV` | groove `XX` | Sélectionne un groove pour la piste courante. |
| `GGR` | groove `XX` | Sélectionne un groove pour toutes les pistes. |
| `HOP` | `XY` | Saute à la ligne `Y`, `X` fois ; `X=0` boucle indéfiniment. Le HOP de table affecte sa propre colonne. |
| `SNG` | `XX` signé | Déplace la lecture de `XX` lignes de Song. |
| `PRO` | `00-64` | Évalue une probabilité de déclenchement absolue de `0%` à `100%`. |
| `MOD` | `AB` | Se déclenche au passage `A` d'un cycle de `B` passages. |
| `SPD` | `XX` signé | Choisit un rapport d'horloge persistant par piste ; voir le tableau des vitesses ci-dessous. |
| `SLE` | `00-FF` ticks | Règle le temps de glissement persistant par piste pour les FX continus du moteur. `00` est immédiat. Le réglage revient à `00` à l'arrêt de la lecture. |

### Envois d'effets audio (Audio FX sends)

| FX | Valeur | Comportement détaillé |
|---|---|---|
| `RSN` | `00-FF` | Règle l'envoi de reverb de cette piste jusqu'au prochain déclenchement de note. |
| `DSN` | `00-FF` | Règle l'envoi de delay de cette piste jusqu'au prochain déclenchement de note. |

### FX ADSR / de déclenchement

| FX | Valeur | Comportement détaillé |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Remplacent l'attack, le decay, le sustain, le release ou la forme de l'enveloppe. |
| `TDC` / `TCL` | `00-FF` | Remplacent le Decay ou la Color du mode Trigger sur Plaits et Plaits-Alt. |

#### Conditions

Inspiré des « trig conditions » suédoises, le tracker prend en charge des probabilités de déclenchement et des conditions de modulo.

| FX | Valeur | Signification |
|---|---|---|
| `PRO` | `00-64` | Probabilité de déclenchement absolue de `0%` à `100%` |
| `MOD` | `AB` | Se déclenche à l'itération `A` sur `B` ; par exemple `12`, `22`, `14`, `34` |

Les compteurs `MOD` sont locaux à la piste et à la phrase. Les combinaisons invalides, comme `A` supérieur à `B`, ne se déclenchent pas.

`MOD34` se déclenche au 3e passage de chaque cycle de 4 passages. `MOD1F` se déclenche au 1er passage de chaque cycle de 16 passages.

#### Vitesse de lecture

Comme NerdSEQ, ChooChooTracker prend en charge une vitesse de lecture indépendante pour chaque piste.

`SPD` lit sa valeur comme un octet signé. `00` est la vitesse normale, les valeurs positives accélèrent la piste et les valeurs négatives la ralentissent. Le réglage reste actif jusqu'à ce qu'une autre commande `SPD` le change.

| Valeur | Vitesse |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

Les anciens projets sans prise en charge de `SPD` signé gardent leur correspondance d'origine `00-10` et sont donc joués comme enregistrés.

Des multiplicateurs très élevés peuvent dépasser la résolution de l'ordonnanceur de ticks du tracker et n'ont pas été testés sur le matériel.

### FX de modulation (Modulation FX)

Les FX de phrase et de table peuvent changer un slot de modulation sans modifier l'instrument :

| Motif de FX | Signification |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Décalage relatif d'Amount pour les slots de modulation 1 à 4 |
| `M11`, `M12`, `M13`, `M14` | Décalages relatifs P1 à P4 pour le slot de modulation 1 |
| `M21`, `M22`, `M23`, `M24` | Décalages relatifs P1 à P4 pour le slot de modulation 2 |
| `M31`, `M32`, `M33`, `M34` | Décalages relatifs P1 à P4 pour le slot de modulation 3 |
| `M41`, `M42`, `M43`, `M44` | Décalages relatifs P1 à P4 pour le slot de modulation 4 |

La valeur est interprétée comme un changement relatif signé sur `8 bits` (`01` ajoute `1`, `FF` soustrait `1`). Les commandes répétées s'accumulent. Les valeurs effectives sont bornées à leur plage valide.

`P5` n'est pas exposé comme FX de phrase ou de table. Il est disponible comme destination de modulation `M1–M4 Wavetable`, de sorte qu'un modulateur peut parcourir l'index AYWavetable d'un autre.

| Type de modulation | P1 | P2 | P3 | P4 | P5 quand Shape est Wavetable |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Inutilisé |
| AHD | Attack | Hold | Decay | Inutilisé | Inutilisé |
| LFO | Shape | Trigger mode | Period | Inutilisé | Index de table d'ondes AY |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | Index de table d'ondes AY |
| FLFO | Shape | Trigger mode | Frequency (`1 Hz` à `20 kHz`) | Inutilisé | Index de table d'ondes AY |

### FX Braids

| FX | Valeur | Signification |
|---|---|---|
| `BMD` | `00-2E` | Modèle Braids |
| `BTM` | `00-FF` | Timbre normalisé absolu |
| `BCL` | `00-FF` | Color normalisée absolue |
| `BCF` | `00-FF` | Fréquence de coupure exponentielle, de `20 Hz` à `20 kHz` |
| `BRS` | `00-FF` | Résonance du filtre exponentielle |

### FX aChChid

| FX | Valeur | Signification |
|---|---|---|
| `ASL` | `00-FF` | Glisse vers la note sans redéclencher. `00` vaut `60 ms` ; les valeurs plus élevées allongent le glissando. |
| `ADC` | `00-FF` | Remplace le decay : de `200 ms` à `2 s`. |
| `AAC` | `00-FF` | Quantité d'accent : de aucune au maximum. |
| `ATM` | `00-FF` | Remplace le Timbre de Braids ; actif seulement en mode d'onde Braids. |
| `ACL` | `00-FF` | Remplace la Color de Braids ; active seulement en mode d'onde Braids. |
| `ACF` | `00-FF` | Fréquence de coupure exponentielle du filtre 303, de `20 Hz` à `20 kHz`. |
| `ARS` | `00-FF` | Résonance du filtre 303, de aucune au maximum. |
| `AEM` | `00-FF` | Modulation par l'enveloppe du filtre 303, de aucune au maximum. |

### FX Bogie

| FX | Valeur | Signification |
|---|---|---|
| `DMD` | `00-0B` | Sélectionne le modèle Bogie jusqu'au prochain déclenchement. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Remplacent Decay, Tone, Sweep, Noise, FM ou Drive jusqu'au prochain déclenchement. Les six macros sont disponibles sur chaque modèle. |
| `DCF` | `00-FF` | Fréquence de coupure du filtre, à correspondance logarithmique. |
| `DRS` | `00-FF` | Résonance du filtre. |

### FX Sintered

| FX | Valeur | Signification |
|---|---|---|
| `SMD` | `00-05` | Sélectionne le modèle Sintered jusqu'au prochain déclenchement. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Remplacent Decay, Mod, les deux premières macros du modèle, Motion ou la dernière macro du modèle jusqu'au prochain déclenchement. |
| `SCF`, `SRS` | `00-FF` | Fréquence de coupure et résonance du filtre. |

### FX MIDI Out

| FX | Valeur | Signification |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Envoie un CC MIDI, remis à l'échelle de `0` à `127`. Le numéro de CC vers lequel chaque emplacement envoie se règle par instrument (`MC1 number`-`MC4 number` sur l'écran de l'instrument), pas par le FX lui-même - un emplacement sans numéro est inerte. |

### FX Plaits

| FX | Valeur | Signification |
|---|---|---|
| `PMD` | `00-17` | Moteur Plaits |
| `PHA` | `00-FF` | Harmonics normalisé absolu |
| `PTM` | `00-FF` | Timbre normalisé absolu |
| `PMO` | `00-FF` | Morph normalisé absolu |
| `PAX` | `00-FF` | Mélange Main/Aux : `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Fréquence de coupure exponentielle, de `20 Hz` à `20 kHz` |
| `PRS` | `00-FF` | Résonance du filtre exponentielle |

### FX Sample

| FX | Valeur | Signification |
|---|---|---|
| `SPL` | `00-03` | Mode de lecture : `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Joue la tranche numérotée (`01` = première tranche) quelle que soit la note ; `00` conserve l'association normale des notes. Exige le mode de découpage activé sur l'instrument |
| `SPT` | `XX` signé | Transposition de l'échantillon en demi-tons |
| `SST` | `00-FF` | Début de lecture normalisé |
| `STA` | `00-FF` | Alias de `SST` (début de lecture normalisé) |
| `SEN` | `00-FF` | Fin de lecture normalisée |
| `SVL` | `00-FF` | Volume absolu de l'échantillon |
| `SCF` | `00-FF` | Fréquence de coupure exponentielle, de `20 Hz` à `20 kHz` |
| `SRS` | `00-FF` | Résonance du filtre exponentielle |
| `SSP` | `00-FF` | Vitesse de l'échantillon, de `0%` à `500%` |

### FX AY communs aux instruments AY

| FX | Valeur | Comportement détaillé |
|---|---|---|
| `AYM` | `XY` | `X` est la forme d'enveloppe ; `Y` choisit off/tone/noise/tone+noise (`0/1/2/3`). |
| `NOI` | `XX` signé | Décalage relatif cumulé de la période du bruit. |
| `NOA` | `00-1F`, `FF` | Période du bruit absolue ; `FF` cède la priorité de la période du bruit aux pistes précédentes. |
| `ERT` | quelconque | Redéclenche la forme d'enveloppe matérielle courante. |
| `EAU` | `XY` | Rapport d'enveloppe automatique `X:Y` ; `X=0` le désactive. |

### FX réservés à AY Classic

| FX | Valeur | Comportement détaillé |
|---|---|---|
| `EVB` | `XY` | Vibrato de période d'enveloppe : `X` vitesse, `Y` profondeur. |
| `EBN` | `XX` signé | Ajoute `XX` à la période d'enveloppe à chaque ligne de phrase/table. |
| `ESL` | `XX` ticks | Glisse de la période d'enveloppe précédente vers la nouvelle valeur. |
| `ENT` | note `XX` | Règle la période d'enveloppe d'après la note de la table de hauteurs affichée par l'interface. |
| `EPT` | `XX` signé | Décalage relatif cumulé de la période d'enveloppe. |
| `EPL` | octet `XX` | Règle l'octet de poids faible de la période d'enveloppe. |
| `EPH` | octet `XX` | Règle l'octet de poids fort de la période d'enveloppe. |

### FX AY Plus

| FX | Valeur | Comportement détaillé |
|---|---|---|
| `TNN` | note `XX` | Règle l'oscillateur de tonalité sur une note de la table de hauteurs, en ignorant la note de la ligne. |
| `TNP` / `TNF` | `XX` signé | Décalage cumulé de la tonalité en pas de hauteur / fin. |
| `TRT` | quelconque | Redéclenche la phase de l'oscillateur de tonalité lorsque c'est pris en charge. |
| `ENN` | note `XX` | Règle l'oscillateur d'enveloppe sur une note de la table de hauteurs. |
| `ENP` / `ENF` | `XX` signé | Décalage cumulé de l'enveloppe en pas de hauteur / fin. |
| `SFT` | `00-07` | Type logiciel : None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | note `XX` | Règle l'oscillateur logiciel sur une note de la table de hauteurs. |
| `SFP` / `SFF` | `XX` signé | Décalage cumulé de l'oscillateur logiciel en pas de hauteur / fin. |
| `SRT` | quelconque | Réinitialise la phase de l'oscillateur logiciel. |
| `SFM` | `XX` signé | Décalage cumulé de la profondeur de FM. |
| `PWM` | `XX` signé | Décalage cumulé de la largeur d'impulsion. |
| `SPL` | `XX` signé | Décalage cumulé du niveau bas de l'impulsion. |
| `SWT` | `XX` signé | Décalage cumulé de l'index de table d'ondes. |

### FX AY Sample

AY Sample accepte les commandes AY communes ainsi que `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` et `SFF` comme décrit plus haut.

| FX | Valeur | Comportement détaillé |
|---|---|---|
| `SMS` | `00-FF` | Règle le début de lecture hérité d'AY Sample à `XX x 64` échantillons source. |

Les FX de paramètres de moteur s'appliquent à leur type d'instrument correspondant et sont réinitialisés au prochain déclenchement de note, sauf indication contraire. `SPD` est volontairement persistant.

Le sélecteur de FX filtre cette référence pour ne garder que les commandes courantes et le groupe pris en charge par l'instrument actif. Le panneau d'aide contextuel donne le détail des valeurs pendant l'édition.

## 10. Mixer, Reverb et Delay

Le Mixer est l'écran principal le plus à gauche. Il affiche la charge CPU et les avertissements d'écrêtage. Un `!` rouge dans la colonne **CLIP** marque la piste dont le signal sec a poussé le mixage au-delà de la plage sûre.

Servez-vous-en pour équilibrer les 8 pistes, façonner chaque piste avec le Tilt EQ, et envoyer l'audio vers la reverb ou le delay :

| Champ | Signification |
|---|---|
| LVL | Niveau de la piste après le moteur, `000-100` |
| REV | Envoi vers la reverb Clouds partagée, `000-100` |
| DLY | Envoi vers le delay ping-pong partagé, `000-100` |
| TLT | Tilt EQ par piste, `00-FF` ; `80` est neutre, les valeurs basses favorisent les graves et les valeurs hautes favorisent les aigus. Les valeurs basses introduisent un léger overdrive. |
| M | `*` met cette piste en sourdine |
| S | `*` met cette piste en solo |

Le mixeur est basé sur les pistes, pas sur les instruments. Si une piste change d'instrument, son niveau, son PAN, son Tilt et ses envois restent attachés à la piste. Les instruments ont aussi leurs propres **Vol** et **Pan** de `00-FF` sur la ligne du haut de l'écran Instrument. Le PAN de l'instrument est appliqué en premier, puis celui de la piste. Tous deux préservent une image stéréo existante au centre ; ils atténuent progressivement le côté opposé lorsqu'on les déplace vers la gauche ou la droite.

### Master FX

L'entrée **MASTER FX** ouvre un sous-menu Insert FX à deux emplacements pour tout le
mixage. Ses effets s'exécutent après les retours de reverb et de delay, avant le volume
master. Sélectionnez un module, éditez ses paramètres ou mettez-le en bypass exactement comme pour un
insert de piste ; appuyez sur **OPT** pour revenir au Mixer. Les réglages de Master FX sont enregistrés
avec le projet et ne sont pas automatisés par les phrases, les tables ou la modulation.

### Clouds Reverb

Utilisez **SELECT + [UP]** depuis le Mixer pour ouvrir l'écran des réglages de reverb.

- **Return** : niveau de reverb humide dans le mixage master.
- **Time** : contrôle de la décroissance/du temps, `00-FF`.
- **Damping** : absorption des hautes fréquences, `00-FF`.
- **Filter** : filtre passe-bas appliqué avant la reverb.

Il s'agit de la section de reverb de Clouds, pas du processeur granulaire Clouds complet.

### Ping-pong Delay

Utilisez **SELECT + [DOWN]** depuis le Mixer pour ouvrir Delay, puis **SELECT + [UP]** pour revenir. Reverb fonctionne dans le sens inverse : **SELECT + [UP]** l'ouvre et **SELECT + [DOWN]** revient au Mixer.

- **Return** : niveau de delay humide.
- **To Reverb** : quantité du signal de delay humide envoyée dans la reverb partagée, `0-100%`.
- **Ticks** : durée du delay en ticks du tracker (`9 ticks` = `1 temps` par défaut).
- **Feedback** : quantité de rétroaction croisée, limitée à `95%`.
- **Filter** : filtre passe-bas dans le chemin du signal retardé.

La 1re répétition suit l'entrée stéréo. Les rétroactions suivantes passent d'un canal gauche à un canal droit, et inversement.

## 11. Écran Project

L'écran Project propose les commandes **Load**, **Save**, **New**, **Export**, **Manage** et **Scale**, ainsi que les métadonnées de nom de fichier, de titre et d'auteur.

### Emplacement des exports (Export location)

Les exports, stems et bounces sont écrits dans un dossier propre à chaque projet, à l'intérieur du dossier de samples préexistant de l'application :

- **Par défaut** : `<app folder>/samples/Exports/<project name>/`. Tant que le projet n'a pas de nom, le dossier est `current-project`.
- Dans la version web, le défaut est `/user/samples/Exports/<project name>/`.
- Sous Android, le dossier de samples se trouve dans l'espace de travail privé de l'application.

La ligne **Folder** de l'écran EXPORT affiche la destination active :

- Touchez **EDIT** pour choisir un dossier personnalisé avec le navigateur de dossiers. Le dossier choisi est utilisé exactement tel quel (aucun sous-dossier de projet n'est ajouté) et est mémorisé dans les réglages.
- **EDIT + OPT** (clear) rétablit l'emplacement par défaut.

Lorsqu'un projet nommé est enregistré sous un nouveau nom, le dossier d'export par défaut est renommé en conséquence, ce qui garde les fichiers déjà exportés avec le projet. Le renommage ne s'applique pas à un dossier personnalisé, et le dossier n'est pas renommé par l'enregistrement automatique. Si un dossier portant le nom cible existe déjà, les deux dossiers sont conservés. Si le tracker a perdu la trace du dossier (par exemple après un redémarrage), enregistrer un projet nommé renomme à la place le dossier `current-project` existant. Les instruments continuent de fonctionner après un renommage : les chemins d'échantillons chargés qui pointent dans le dossier renommé sont mis à jour automatiquement.

**Load** accepte aussi les fichiers `.mid`/`.midi`, importés comme un nouveau projet : les notes sont regroupées par canal MIDI (un canal par piste, jusqu'au nombre de pistes), quantifiées à 4 lignes par temps, et placées sur un seul instrument AY par défaut - les numéros de programme MIDI n'ont pas d'équivalent chiptune, choisissez donc de vrais instruments ensuite. Seul le premier tempo du fichier est utilisé (une seule cadence de ticks globale, pas de changements de tempo par section). La ligne **MIDI** de l'écran Export fait l'inverse : elle écrit les notes, le volume et le tempo/groove de l'arrangement courant sous forme de fichier MIDI standard (une piste MIDI par piste du tracker) ; les instruments [MIDI Out](#midi-out) et les autres FX par ligne au-delà du volume et du groove global n'ont pas d'équivalent MIDI et ne sont pas traduits.

**Load** accepte aussi les morceaux Dirtywave M8 (`.m8s`, firmware 2.x-4.x), importés comme un nouveau projet. Seules la structure et les notes passent (lignes de song, chaînes avec leur transposition, phrases avec note, vélocité et numéro d'instrument, note-offs, tempo et nom du morceau) ; les notes gardent leur hauteur réelle (le M8 nomme les octaves deux plus haut : son `C-6` est `C-4` ici). Chaque instrument utilisé par une phrase est importé sous le nom de l'instrument M8. Les instruments M8 **MACROSYN** deviennent des instruments Braids (les 47 formes sont les modèles Braids, avec les mêmes numéros ; timbre, color, type de filtre, fréquence de coupure, résonance et pan sont repris, la courbe de coupure étant approximative). Les instruments **SAMPLER** deviennent des instruments Sample : le `.m8s` ne stocke que le chemin du WAV sur la carte du M8, si bien que le fichier est cherché sous le dossier du `.m8s` et ses parents (une copie de l'arborescence de la carte convient) et dans votre dossier de samples courant (`Samples` ou `samples` fonctionnent tous deux) ; s'il n'est pas trouvé, l'instrument garde le chemin du M8 et reste silencieux. Les échantillons importés reçoivent +24 demi-tons dans leur réglage Pitch, car le M8 joue un échantillon à sa hauteur d'origine deux octaves plus bas que cette application. **FMSYNTH** devient un instrument Genesis FM (approximation grossière), **WAVSYNTH** et **HYPERSYNTH** deviennent aChChid. **MIDI OUT** devient un instrument MIDI Out (canal, bank, programme et les quatre premiers numéros de CC). Les autres types (externes) deviennent un instrument AY par défaut - choisissez de vrais sons ensuite. Les FX et les tables sont ignorés.

Sur ordinateur, la ligne **M8S** de l'écran Export fait l'inverse, de façon limitée : touchez-la et choisissez un `.m8s` existant comme **modèle** (mémorisé jusqu'à ce que vous quittiez, **EDIT + OPT** l'oublie). Son morceau, ses chaînes, ses phrases, son tempo et son titre sont remplacés et tout le reste est conservé tel quel, sauf que les instruments Braids sont réécrits en MacroSynth et les instruments Sample en Sampler (chemin `/Samples/<file name>` : copiez vous-même le WAV à cet endroit sur le M8), à condition que le modèle ait un instrument de ce type où copier les autres paramètres. Les FX ne sont pas exportés, et un morceau utilisant la phrase `FF` ou au-delà ne peut pas être exporté (le M8 a 255 phrases). Le fichier exporté n'a pas encore été testé sur un vrai M8. Voir [M8 songs](../m8s-format.md) pour les détails.

Sur ordinateur, le key jazz permet de saisir le nom de fichier, le titre et l'auteur directement au clavier au lieu du clavier virtuel à l'écran - voir [Key jazz](#key-jazz-ordinateur-uniquement).

### Scale / Quantize

L'écran Scale contrôle le moteur de gamme global en 12-TET. La ligne **Mode** choisit entre deux modes :

- **Quantizer** (par défaut) : la saisie dans les phrases reste chromatique ; lorsqu'il est activé, les notes sont arrondies vers le bas immédiatement avant d'atteindre le moteur sonore, si bien que changer de fondamentale ou de gamme réharmonise un morceau sans modifier ses phrases.
- **Note Lock** : les notes saisies dans les phrases sont ramenées à la note la plus proche de la gamme au moment de la saisie, de sorte que seules les notes de la gamme peuvent être écrites. Les nouvelles saisies et les diminutions s'alignent vers le bas ; `EDIT + RIGHT` / `EDIT + UP` montent la note jusqu'à la note de gamme suivante au-dessus. La saisie suit toujours la gamme du projet sur cet écran : changer la fondamentale ou la gamme affecte immédiatement les notes nouvellement saisies sur les pistes activées, tandis que les notes déjà enregistrées restent intactes. Le FX `SCL` est indisponible dans ce mode et tout `SCL` déjà écrit est ignoré. La quantification de lecture des notes simples est contournée (la saisie est déjà verrouillée) ; la quantification des accords via `CRD` s'applique toujours.

Lorsque le curseur repose sur **Mode**, la barre d'état garde visible une description fixe du mode actif : `Quantizes notes on playback` pour **Quantizer**, `Snaps sequencer to scale` pour **Note Lock**. Changer de mode remplace le texte immédiatement ; tout message d'action le remplace jusqu'à son expiration.

Les deux modes partagent la même fondamentale, la même gamme, l'édition de notes **Custom** et les cases à cocher de pistes. Chacune des huit cases à cocher de pistes décide à quelles pistes le mode s'applique. Le moteur de gamme est contourné pour les tables de hauteurs qui n'ont pas 12 notes et pour les instruments Sampler dont le mode **Slice** est EQUAL, AUTO ou LAZY (leurs notes sélectionnent chromatiquement les tranches à partir de C-0) ; l'entrée MIDI ne fait pas partie de cette version.

- **Linear pitch** choisit le mode de la table de hauteurs. **Off** est le défaut et le réglage validé sur le matériel pour un suivi d'octave correct de l'AY, de Braids et de Plaits.
- **Tick rate** règle le tempo du tracker et affiche le BPM correspondant (`tick rate x 60 / 24`).
- ChooChooTracker enregistre les projets en `.cct`. Ce format n'est pas compatible avec ChipNomad.

Utilisez **Save** avant de changer de type d'instrument ou de charger un autre projet.

## 12. Settings

- **Repeat delay / speed** règlent la répétition d'un bouton maintenu.
- **Stick live mode** choisit `HOLD`, `TOGGLE` ou `FREE` pour le bouton Stick live déjà affecté.
- **MIDI** ouvre le sous-menu [MIDI](#14-midi) : choix de l'appareil et affectation des canaux MIDI In aux instruments.
- **Synths** contient **AY Quality**, **Sample dithering** et les réglages globaux **Braids BITS / DRFT / SIGN**.
- **Mixer** contient **Mix volume** et le **Tilt pivot** de `250-4000 Hz` propre à chaque projet (défaut `1 kHz`) utilisé par tous les contrôles Tilt du Mixer.
- **Graphics** contient **Edit color theme**, **Load font**, **Persistent waveform**, **Track visuals**, **Waveform FPS** et le moteur de rendu actif (`GPU` ou `Software`). **Waveform FPS** règle la fréquence de rafraîchissement de l'oscilloscope d'instrument et des visualiseurs du mixeur, de `01` à `60`. Track visuals choisit **Detailed** (la forme d'onde du synthé et ses surimpressions) ou **Audio waveform** (la sortie réelle sommée de cette piste) indépendamment pour chaque piste ; la forme d'onde audio efface immédiatement toute surimpression ADSR précédente au démarrage de la lecture. Les polices et thèmes ChipNomad devraient fonctionner.
- **Key mapping** personnalise les commandes.
- **Support report** écrit un petit fichier texte de diagnostic avec la version de l'application, la plate-forme, les réglages audio et la charge CPU, l'état de lecture/de débordement, et un résumé compact du projet. Il n'inclut ni données d'échantillons ni texte de titre/d'auteur du projet. Sous Android et Web, il ouvre le flux normal d'export/de téléchargement ; les versions pour ordinateur et consoles portables enregistrent `support-report.txt` dans le dossier inscriptible de l'application.
- **Quit ChooChooTracker** quitte proprement.

**Graphics → Boot screen** choisit **Original** (défaut) ou **Night**. Touchez EDIT
ou utilisez EDIT + direction pour changer ; le choix est enregistré pour le prochain
écran-titre. Night utilise la scène ferroviaire d'origine avec des tons gris-bleu plus doux,
une Voie lactée procédurale avec de subtils scintillements d'étoiles et de rares étoiles filantes.
Le décor et le train d'origine gardent leurs boucles de défilement.

## 13. Performances et dépannage

### CPU

Le coût en CPU dépend des moteurs et des effets actifs. Les modèles physiques de Plaits et la Clouds Reverb sont plus lourds que les voix AY de base. Vérifiez les performances sur la console cible, surtout avec 8 voix Plaits et les deux envois actifs. Une lecture de CPU proche de `100%` peut causer des craquements ou des échéances audio manquées.

L'affichage du CPU est une mesure lissée du temps de rendu audio. Il ne mesure pas les retards avant l'exécution du callback audio ni dans la sortie audio de l'appareil : des craquements peuvent donc se produire même avec une lecture basse.

Sous Android, la sortie audio préfère désormais AAudio pour éviter les craquements intermittents observés avec OpenSL ES sur le Pixel 7a. Cette sortie avec tampon privilégie une lecture stable et peut ajouter une certaine latence de réponse. Le canevas du tracker garde sa disposition 4:3 complète sur les écrans carrés et d'autres formats inhabituels ; l'espace inutilisé est affiché sous forme de bordures plutôt que de rogner des colonnes. Si les craquements persistent, signalez le morceau, le modèle de téléphone, la version d'Android et si vous utilisez le haut-parleur, un casque ou le Bluetooth.

Si vous rencontrez des claquements, des plantages ou des ralentissements, utilisez **Settings > Support report** et joignez le fichier `support-report.txt` généré au rapport de bug. Si un morceau précis provoque le problème, joignez aussi ce `.cct`.

### Pas de son

Vérifiez le numéro d'instrument, l'état de sourdine ou de solo de la piste, le LVL de la piste, le Mix volume de l'application et l'enveloppe de l'instrument. Pour les échantillons, vérifiez que le WAV d'origine existe toujours au chemin enregistré. Sous ArkOS, appuyez sur **MENU + L3** pour basculer la sourdine du système d'exploitation. Si cela ne règle rien, redémarrez le matériel et reconnectez votre interface audio.

### Les commandes semblent incorrectes

Ajustez Repeat delay et Repeat speed dans Settings. Si une seule pression déplace deux fois, vérifiez qu'une seule commande physique est affectée à cette direction et signalez l'écran et le raccourci exacts.

## 14. MIDI

Ordinateur et PortMaster. Settings > **MIDI** ouvre ce sous-menu : **MIDI In** / **MIDI Out** choisissent un appareil connecté, en parcourant les ports détectés avec `OFF` à chaque extrémité ; les deux directions nécessitent qu'un appareil soit sélectionné. Le choix de l'appareil est propre à la session : il n'est pas enregistré dans settings.txt, car la position d'un port dans la liste peut changer d'un redémarrage à l'autre ou lorsque les appareils sont branchés dans un ordre différent. La commande **PLAY** de l'application reste disponible depuis cet écran et lance le morceau. Sous PortMaster, connectez un appareil MIDI USB avant le lancement ; il doit être exposé par le système de la console portable via ALSA.

- **Écoute d'un son depuis un clavier MIDI** : avec un appareil MIDI In sélectionné, jouer des notes sur le clavier connecté fait écouter un instrument sur n'importe quel écran, comme le raccourci **EDIT + PLAY** à l'écran sur l'écran [Instrument](#5-instruments). C'est de l'écoute seulement - cela n'entre pas de notes dans le morceau.
- **Channel mapping** : ouvre une liste des 16 canaux MIDI ; affectez à chacun un instrument (`OFF` par défaut) pour que les notes reçues sur ce canal fassent écouter cet instrument, quel que soit celui sélectionné sur l'écran Instrument - par exemple canal `01` -> instrument `05`. Un canal laissé sur `OFF` retombe sur l'instrument actuellement sélectionné, le comportement d'origine. Cette affectation est enregistrée dans settings.txt.
- **MIDI CC mapping** : associe un contrôleur entrant à un paramètre d'instrument, y compris son **Volume**, ou à des contrôles globaux. Les lignes globales affichent `TRK 1`-`TRK 8` au lieu d'un instrument et proposent, par piste, **Mute**, **Solo**, **Volume**, **Reverb send** et **Delay send** ; **Song play/stop** n'a pas de cible. Une nouvelle affectation ignore le CC qui l'a apprise ou configurée et prend effet au prochain mouvement physique, si bien que configurer des affectations pendant la lecture ne peut pas remettre un patch à zéro. Mute et solo utilisent `0-63` = off, `64-127` = on.
- **Piloter un appareil MIDI externe** : donnez à une piste le type d'instrument [MIDI Out](#midi-out) et réglez son Channel ; déclencher des notes sur cette piste envoie du vrai MIDI à l'appareil MIDI Out sélectionné au lieu de produire du son dans ChooChooTracker.

## 15. Crédits et licences

ChooChooTracker est un fork de ChipNomad et conserve son approche de licence MIT. Le code de Braids, Plaits, Plaits-Alt, Clouds DSP, du DSP de MME dérivé de Warps et de stmlib provient des publications open source de Mutable Instruments, sous leurs mentions MIT applicables. Plaits-Alt provient du fork Plaits Lab de lylepmills/eurorack ; ses mentions de source conservées s'appliquent. Le moteur aChChid utilise Open303 de Robin Schmidt, copyright 2009, sous licence MIT. Bogie et Sintered sont des implémentations natives originales. L'E/S MIDI utilise RtMidi de Gary P. Scavone, sous sa licence de type MIT. Consultez les fichiers de licence fournis pour les attributions exactes.

## Effets d'insertion de piste (Track insert effects)

Chacune des huit pistes a deux emplacements en série, **TF1 → TF2**. Depuis Instrument,
utilisez le geste habituel Shift+Haut pour ouvrir MOD, puis Shift+Haut de nouveau pour **F: Insert
FX**. Shift+Bas revient à MOD. OPT+Gauche/Droite sélectionne la piste (1-8), sans
changer le sélecteur d'instrument. L'en-tête du module ouvre le sélecteur existant ;
le bypass conserve le module, les valeurs configurées et l'automation. Les paramètres sont des
valeurs d'un octet : colonne de gauche 1-4, colonne de droite 5-8. Le texte contextuel affiche les noms complets et
les valeurs décodées. Les anciens projets se chargent avec les deux emplacements sur OFF.

L'affichage de navigation garde la colonne F-M-I-P complète visible dans tous les
écrans d'instrument, y compris Insert FX et l'instrument pool, et met en évidence
l'écran courant.

| Module | Paramètres dans l'ordre |
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

Les contrôles continus utilisent 00-FF, y compris un zéro valide et un maximum FF. Les
contrôles bipolaires ont un neutre exact à 80. Les modes de Distortion sont 00 Density, 01 Drive,
02 Spiral, 03 Mojo, 04 Dyno. Les rapports du Compressor 00-07 sont 1,5, 2, 3, 4, 6, 8, 16,
20:1 ; les sources de détecteur 00-02 sont entrée stéréo, gauche, droite. Detector filter 80 est
neutre, en dessous c'est un passe-bas, au-dessus un passe-haut. Aucun sidechain externe/entre pistes
n'est fourni. Chaque emplacement peut contenir n'importe quel module ; les modules répétés sont indépendants.
Chorus, Flanger, Phaser et Rotary utilisent des valeurs de modulation par défaut douces. Rotary Speed
utilise toute la plage **00-FF** : **00 = 0,01 Hz** (une rotation par 100 secondes),
**FF = 25 Hz** (l'ancien maximum de Chaos). Un espacement exponentiel donne un contrôle plus fin
des mouvements lents du LFO. Les nouveaux inserts Rotary démarrent par défaut à **8F**, environ
0,8 Hz. Cela remplace les quatre valeurs de vitesse discrètes ; les réglages Rotary existants
et l'automation de vitesse F11/F21 doivent être réajustés à la nouvelle échelle (les anciens Slow/Fast/Hyper/
Chaos correspondent approximativement à **8F/CB/E7/FF**). Les autres paramètres ne changent pas.
La profondeur de bits du Bitcrusher va de 4 à 16 bits et
la réduction de fréquence de 1x à 32x. Les modes de Destruction sont Fold, Clip et Crush.
Saturation et distortion incluent des contrôles de niveau de sortie ou de mix pour les équilibrer
par rapport au signal non traité.
Le Parametric EQ a des shelves de boost/cut de 24 dB à 60-500 Hz et 2-14 kHz, plus une
cloche à Q fixe de 0,9 de 250 Hz à 5 kHz. Ses contrôles de gain sont neutres à 80 ; avec les
trois au neutre, c'est un bypass exact. L'EQ actif utilise un soft knee transparent au-dessus de
la pleine échelle et atteint un maximum de 1,5 pour contenir les boosts extrêmes.

Le niveau de piste et le tilt EQ précèdent les inserts : les deux affectent donc le signal qui entre
dans la compression et la distorsion. Toutes les voix/notes d'accord de la piste alimentent une seule
chaîne d'inserts. Les envois de reverb et de delay reçoivent tous deux sa sortie. Les effets partagés, les retours,
les contrôles master et le mute/solo gardent leur comportement existant. Les inserts ne
traitent pas l'audio provenant d'appareils MIDI externes.

Les commandes d'insert utilisent **F11-F18** pour TF1 et **F21-F28** pour TF2. La sélection de FX
du roll et la fenêtre Phrase/Table ne proposent que les paramètres utilisés par les inserts configurés
de la piste sélectionnée. La fenêtre les regroupe sous **TF1: nom de l'effet** et
**TF2: nom de l'effet** ; les emplacements OFF sont masqués. Les inserts en bypass restent éditables.
Dans la fenêtre de FX de Phrase et de Table, la description suit l'emplacement d'insert de
la piste sélectionnée : elle montre le module, le nom du paramètre, les extrémités décodées de la plage et
la valeur de base enregistrée. Les commandes de morceau existantes sont préservées si un changement de module les rend
inutilisées ; ouvrir leur sélecteur démarre sur un contrôle disponible.
Pendant l'édition d'une valeur, l'indice du bas affiche sa valeur décodée et ses unités.
Toute colonne de FX de Phrase ou de Table peut adresser l'un ou l'autre emplacement ; la grille de Phrase garde ses
trois colonnes. Ces valeurs absolues d'exécution persistent à travers les notes, les instruments, les
phrases, les chaînes et les boucles ordinaires. Une lecture neuve ou un arrêt brutal les réinitialise. Modifier
une valeur de base sur la page F n'efface que le remplacement de ce paramètre. MOD agit sur la
valeur de base/d'automation effective sans enregistrer ses décalages. Choisir un nouveau
module rétablit les valeurs par défaut et efface les remplacements de cet emplacement. Le bypass conserve
l'automation existante tout en suspendant le traitement après un court fondu.

**Les adresses désignent des emplacements de piste et des positions de paramètre, pas des effets nommés.** Une
Phrase réutilisée sur une autre piste utilise les modules de cette piste. Changer de module peut
changer le sens d'une commande ; les commandes ne sont pas réécrites. Les adresses OFF/inutilisées
sont des no-ops sans danger et restent intactes dans les données de Phrase/Table enregistrées.

Le sélecteur MOD de l'instrument ajoute une catégorie Insert FX. Les sources de chaque instrument
contrôlent les inserts de la piste qui le joue, y compris le signal d'accord combiné.
Le même instrument sur deux pistes affecte des chaînes indépendantes. Le timing existant des sources,
les montants signés, les règles de combinaison de sources, les modes de stick et l'enregistrement de mouvement
s'appliquent. Les contrôles discrets sont quantifiés et bornés à des choix valides. Il n'y a pas de
nouvelle banque de modulation propre à la piste.

Le makeup du Compressor démarre à 0 dB. OTT démarre à environ 25 % de depth, 25 %
d'upward, 50 % de downward, 0 dB en entrée et -3 dB en sortie. Ses seuils de bande, ses gains et
ses fréquences de coupure restent aux valeurs natives de Rui-727 ; Depth est son contrôle wet/dry.
TAPESCAM conserve ses valeurs auxiliaires natives : bande neuve/haute vitesse, bruit et
compression désactivés, élargissement activé. Surveillez les indicateurs d'écrêtage existants en ajoutant
du drive/gain. Les transitions d'emplacement utilisent des fondus bornés de 5 ms via l'audio sec.

Les sauvegardes stockent les identifiants de module, le bypass et les octets configurés dans les sections
versionnées facultatives `Track inserts: 1,8,2` et `Master inserts: 1,2`. Les remplacements d'exécution
et les historiques DSP ne sont pas enregistrés.
Gardez des sauvegardes avant d'ouvrir des fichiers avec inserts dans des versions plus anciennes. L'attribution du DSP,
les révisions de source et les adaptations se trouvent dans `chipnomad_lib/external/insert_fx/SOURCES.txt` ;
les mentions MIT complètes sont fournies dans `licenses/INSERT_FX.txt`.

Sur un R36H à 48 kHz, une paire d'inserts mesurée sur une piste a pris environ 4-5 %
du temps réel pour Compressor, 6 % pour Distortion, 18 % pour Doubler, 16 % pour TAPESCAM,
et 24 % pour OTT, avant le coût des synthés et des effets partagés. Seize instances actives de
Doubler, TAPESCAM ou OTT ont dépassé le temps réel. La modulation à fréquence audio ajoute
un coût supplémentaire. Commencez avec quelques inserts, surveillez la surcharge audio, et mettez en bypass
les emplacements inutilisés ; les seize positions disponibles ne sont pas un budget CPU garanti.

La page Insert garde visible l'indice du champ sélectionné après le relâchement du bouton ;
des avis temporaires peuvent toujours le remplacer. Le sélecteur de module montre pour chaque
module son type d'effet et son projet source à côté de son nom.

### Instruments natifs OPLL et VRC7 (développement)

Le sélecteur Instrument Type a un groupe FM avec **OPLL / MSX** (YM2413)
et **VRC7** (DS1001). Chacun propose désormais **73 programmes** dans des listes Bank / Preset
défilantes : les 15 timbres d'origine, 40 timbres distincts supplémentaires issus des tables
YM2413/VRC7/YMF281B d'emu2413, et 18 programmes à deux opérateurs écrits pour ChooChoo.
Les doublons exacts d'octets de timbre sont omis dans chaque moteur. Les palettes
supplémentaires utilisent l'emplacement de timbre programmable de la puce ; elles n'agrandissent pas la
ROM physique. Le programme zéro désigne un timbre personnalisé. EDIT + gauche/droite sélectionne le
préréglage précédent ou suivant dans le filtre de banque. Fine ct règle l'accord de
-100 à +100 cents.

Dans la liste des programmes, EDIT + PLAY fait écouter le son en surbrillance ; relâchez les
boutons pour arrêter. EDIT valide et OPT annule. Parcourir/écouter ne change pas
l'instrument, la table ni le morceau. Le geste d'écoute habituel de la page Instrument
continue de fonctionner après la sélection. Les enveloppes natives de la puce fournissent l'attack/release ;
l'Amp env facultatif ajoute un ADSR logiciel (voir Contrôles des puces natives plus bas).
Il n'y a pas d'éditeur d'opérateurs complet.

Les nouveaux instruments stockent les huit octets de timbre natifs, la banque/le nom, le programme et l'accord fin dans
le fichier de morceau/d'instrument. Les fichiers contenant ces types utilisent le format 7.0 et nécessitent
cette version ou une version ultérieure. Les formats antérieurs restent lisibles ; les morceaux sans ces types
continuent d'être enregistrés en 5.0. Les programmes de rythme sont reportés. La bibliothèque étendue est
validée sur l'hôte ; l'écoute humaine et une nouvelle vérification sur console portable restent en attente.

### AdLib / OPL2 et OPL3 (développement)

Le FM inclut aussi AdLib / OPL2 (YM3812) et OPL3 (YMF262). Bank filtre la
liste d'usine ; Preset ouvre des groupes par catégorie et une vue All. EDIT + PLAY fait écouter
avant la sélection, EDIT sélectionne, OPT annule, et EDIT + gauche/droite sur Preset
charge l'entrée correspondante précédente/suivante. Fine ct règle l'accord local. Mode affiche
2 opérateurs, 4 opérateurs ou Dual voice. OPL2 masque les patches OPL3 incompatibles ;
OPL3 peut jouer la collection partagée à deux opérateurs.

Les titres des fenêtres de banque et de preset identifient le moteur courant. Changer de moteur ou
d'emplacement d'instrument remet le filtre de banque sur **All banks**, y compris en passant
entre les moteurs compatibles AdLib et OPL3. Revenir d'une fenêtre dans le
même emplacement conserve le filtre choisi. Annuler l'import d'une banque DX7 rétablit le
filtre précédent.

La confirmation d'un preset publie aussi les modifications faites au relâchement du bouton pour le prochain
tick d'interface/audio. Les morceaux en cours de lecture n'ont plus besoin d'une autre pression de bouton ni d'un
redémarrage du transport pour recevoir ce patch. La sortie suit toujours le tampon audio configuré
et le comportement d'enveloppe/de redéclenchement du moteur.

La collection d'usine contient 697 entrées sources de The Fat Man 2-op,
The Fat Man 4-op et DMXOPL3, avec 589 identités de patch uniques normalisées.
Les alias conservent les noms et l'attribution d'origine. Les niveaux d'opérateurs natifs de la source, les
drapeaux de profondeur de LFO, les décalages de notes, les hauteurs de percussion fixes et l'accord des voix doubles
sont conservés. Le volume du tracker utilise une amplitude logicielle ; il ne reproduit pas
les courbes de volume ni les politiques de décalage de vélocité des lecteurs MIDI d'origine. Les
estimations de durée de release de la source ne coupent pas les notes tenues. Certains effets ont
des attaques lentes : maintenez Seashore plutôt que d'attendre qu'un court clic le révèle.

Les ressources d'usine se trouvent dans `instruments/chips`, à côté de la bibliothèque d'instruments
existante, avec les mentions sous `licenses/chip-banks`. La sélection d'un preset laisse la
table de l'emplacement et les inserts propres à la piste inchangés. Les fichiers nouvellement enregistrés utilisent 7.0 et
intègrent le timbre complet. L'audio d'usine est testé automatiquement sur l'hôte et en ARM64 ; l'écoute humaine
reste en attente.

### Instruments natifs Sega PSG et Game Boy (développement)

CHIP inclut maintenant Sega PSG, GB Pulse et GB Noise. Sega utilise l'horloge maître NTSC
et sa rétroaction de bruit sur 16 bits, y compris le bruit dérivé du canal de tonalité.
Son **Bass range: Extended** par défaut abaisse l'horloge virtuelle si nécessaire pour
jouer sous la limite du diviseur de la puce, d'environ 109 Hz. Cela garde A-2, G-2
et F-2 distincts. Choisissez **Chip** pour la plage d'origine, où les notes plus graves
convergent vers la limite du diviseur. Le bruit à fréquence fixe garde l'horloge d'origine ;
le bruit dérivé de la tonalité suit la hauteur étendue. Les anciens instruments Sega se chargent avec
Extended activé ; le réglage est enregistré avec l'instrument.
Les instruments GB utilisent les registres d'impulsion/de bruit du DMG ; aucun canal d'onde de Game Boy n'est
exposé. Leurs listes de presets composés contiennent maintenant respectivement **24, 24 et 28** presets.
Les ajouts couvrent le bruit Sega lié/périodique, les balayages de hauteur GB,
les percussions courtes, le bruit métallique, les drones et les montées. Ce sont des programmes originaux,
pas des sons extraits de jeux.
La page propose des contrôles natifs de mode/duty/largeur, d'enveloppe/sweep/bruit selon le cas,
plus un ADSR d'amplitude logiciel. Ils sont enregistrés dans les fichiers natifs de version 6.
Les vérifications automatisées de performance/audio ARM64 passent dans les limites de charge
indiquées ; l'écoute humaine reste en attente.

### DX7 FM (développement)

Choisissez **Type → FM → DX7 FM** ; DX7 est la septième entrée, après Arcade / YM2151.
DX7 FM utilise un noyau MSFA à six opérateurs. La page FM existante propose
Bank, Preset et Fine ct à côté des réglages communs d'instrument. La navigation dans les presets
utilise des listes par banque/catégorie. Maintenez EDIT+PLAY pour entendre le son en surbrillance ; relâcher
arrête l'écoute. EDIT seul valide au relâchement ; OPT annule. Charger un son
copie son patch complet sans modifier les tables du tracker ni les FX d'insert de piste.

Le catalogue d'usine contient actuellement **67 patches de paramètres DX7 distincts** :
31 sons musicaux originaux OpenDX7, quatre sons YSE CC0 uniques (ses 32 emplacements de banque
répètent ces quatre avec des noms différents), et 32 patches écrits pour ChooChoo.
Ici, « original » signifie des programmes de paramètres créés pour ce projet ; cela ne
signifie pas du contenu d'usine Yamaha original. YSE est fourni sous forme de quatre sons, pas de 32 noms
artificiellement différents.
Les catégories décrivent l'intention de conception sonore après examen. Les noms ambigus restent
Unsorted. Les vérifications numériques de jouabilité sont passées ; l'acceptation à l'écoute est
en attente. L'objectif distinct de 1 000 sons libres de redistribution n'est pas atteint.

Pour une bibliothèque personnelle persistante, placez des fichiers `.syx` dans
**`instruments/banks/dx7/`**, à côté de la bibliothèque d'instruments existante. Les sous-dossiers
sont pris en charge. Ouvrez **DX7 → Bank** : chaque fichier apparaît comme une banque nommée, prête pour
la navigation dans Preset. Rouvrez Bank après avoir ajouté ou retiré des fichiers ; aucune conversion ni
étape Load Instrument n'est nécessaire. Une banque DX7/TX7 originale standard contient
**32 voix**. Un fichier avec quatre messages de banque apparaît comme quatre banques numérotées
(128 voix au total). Les fichiers à voix unique sont aussi acceptés. L'application native
lit ce dossier ; la version navigateur lit son système de fichiers virtuel, pas des dossiers
arbitraires de l'ordinateur.

Les fichiers défectueux ou non pris en charge sont ignorés avec un décompte à l'écran ; les banques valides restent
disponibles. L'analyse est bornée à 2 048 fichiers, 1 Mio par fichier, 64 Mio au total,
60 000 voix et huit niveaux de dossiers imbriqués. Les liens symboliques sont ignorés. Le
patch sélectionné appartient à l'instrument : supprimer sa banque source ne peut pas
modifier le morceau enregistré. La bibliothèque de navigation complète reste dans ce dossier et
n'est pas copiée en bloc dans chaque projet.

LOAD INSTRUMENT accepte aussi les dumps `.syx` DX7/TX7 originaux à voix unique et à banque de 32 voix,
y compris des séquences bornées de ces messages pris en charge. Il vérifie le
cadrage, le nombre d'octets, les données sur sept bits, les sommes de contrôle, les plages de paramètres et la taille du fichier
avant d'ouvrir la banque importée dans le même navigateur FM. Sélectionner un patch
valide ; annuler conserve l'instrument actuel du morceau. Ce raccourci d'import direct
garde sa liste de navigation pour la session ; utilisez le dossier de banques ci-dessus pour une navigation
persistante. Enregistrez le morceau ou un `.cni` pour conserver les patches sélectionnés/modifiés. Les dumps sans en-tête,
les sommes de contrôle erronées, les performances/extensions DX7II et les autres familles Yamaha sont
refusés. L'import n'envoie jamais de messages MIDI à du matériel externe.

CNI et la version 6 des projets stockent les 155 octets de voix d'origine, le nom complet affiché,
la vélocité de frappe native, l'accord fin et l'identité de la source. Les morceaux n'ont pas besoin de
banque externe pour se rouvrir. Les anciennes versions de ChooChoo ne peuvent pas lire ces fichiers
de version 7 ; les projets qui n'en contiennent pas continuent d'être enregistrés en version 5. La vélocité native vaut
100 par défaut ; le volume du tracker est un gain post-synthèse et ne refrappe pas l'enveloppe.
Les enveloppes d'opérateurs, le mode à fréquence fixe, le keyboard scaling, l'enveloppe de hauteur et le
LFO restent un comportement natif du patch. L'ADSR d'amplitude FM commun facultatif façonne
leur sortie combinée sans remplacer les enveloppes d'opérateurs.

Chaque partie de piste a un LFO et quatre emplacements de voix d'accord distincts, conformément à
la politique existante d'accords/de remplacement du tracker. Les pistes ne partagent pas l'état des notes.
Note-off relâche les enveloppes natives ; cut/panic les efface. Le noyau MSFA scalaire
utilise des quanta de 64 échantillons à 44,1 kHz internes fixes, avec un FIR en flux continu vers
la fréquence de sortie. Les tables de consultation sont initialisées une seule fois en dehors du rendu audio, de sorte que
les moteurs de rendu hors ligne et en direct simultanés ne peuvent pas modifier la fréquence l'un de l'autre. Les événements
prennent effet au prochain quantum interne (jusqu'à 1,45 ms), suivi d'un délai de groupe du FIR
d'environ 0,25 ms. Les échantillons en tampon sont conservés d'un callback à l'autre.
C'est la lignée MSFA Modern, et non une prétention à l'identité bit à bit avec le matériel DX7 ou à une
émulation de Dexed Mark I. DX7 est limité à **16 notes actives sur tout le morceau**,
y compris les queues de release, tout en gardant quatre emplacements d'accord propres par piste.
En cas de dépassement, il retire d'abord les queues de release, puis les notes tenues les plus faibles,
puis les attaques fraîches. À attaques égales, les notes fondamentales sont conservées d'une piste à l'autre avant les
extensions d'accord, avec un départage stable par emplacement/piste. Cette politique s'applique sur toutes les
plates-formes pour qu'un même morceau ait la même allocation de notes bornée. Elle ne
change pas la polyphonie des autres instruments. L'écoute de preset est désactivée pendant la lecture.
La limite a été choisie à partir de mesures sur R36H et validée en lecture mixte.

#### Éditeur d'opérateurs FM

Les instruments Genesis FM, Arcade FM, OPL2, OPL3, OPLL et VRC7 ont un bouton **EDIT**
à droite de Fine ct. Il ouvre un tableau des paramètres du patch :

- Genesis FM / Arcade FM : algorithm, feedback, pan, AMS, PMS, masque d'opérateurs et
  réglages du LFO, puis MUL, DT, TL, KS, AR, D1R, D2R, RR et D1L pour les quatre
  opérateurs, plus SSG (Genesis) ou DT2 et AM (Arcade).
- OPL2 / OPL3 : feedback, connection et pan par voix, deep vibrato et tremolo,
  puis MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS et KSR par
  opérateur. Sur OPL3, le mode (2op, 4op, dual) peut être changé ici.
- OPLL / VRC7 : les registres de timbre personnalisé (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Modifier un preset de ROM le transforme en
  « Custom ».

Gauche/Droite et les touches d'édition habituelles changent les valeurs ; l'instrument continue de jouer
comme d'habitude pour que vous entendiez les changements. La barre d'état décrit le paramètre sous
le curseur. OPT ou Left+Shift revient à l'écran de l'instrument. DX7 et SID
n'ont pas d'éditeur d'opérateurs.

Genesis FM (YM2612) et Arcade FM (YM2151) utilisent désormais le même navigateur FM Bank/Preset,
la même écoute EDIT+PLAY, la même confirmation/annulation et les mêmes contrôles d'accord fin. Genesis a maintenant
**73 presets** : les 24 d'origine et 49 programmes mélodiques pris en charge issus de la banque CC0
16-Bit FM Music Station de NeoSoundFonts. Arcade a **81 presets** : les
24 d'origine et 57 programmes pris en charge et audibles issus de la collection GPLv3 de YMulator-Synth.
Le couple d'origine de ChooChoo partage ses recettes sous-jacentes ; les nouvelles
collections sources fournissent des palettes séparées. Ce sont des collections nommées de conception sonore,
pas des recréations prétendues de bandes originales de jeux précises. Les décalages de notes non pris en charge,
les touches de percussion fixes, les réglages de bruit d'arcade et les programmes sources silencieux
sont consignés comme exclusions dans `expansion-manifest.json`. Leurs réglages complets de patch à quatre opérateurs, d'enveloppe,
de LFO, de stéréo et d'accord voyagent dans les instruments et morceaux de version 6.
L'enveloppe native de la puce contrôle le release ; le gain ordinaire et la hauteur du tracker
ne la redéclenchent pas. La sortie DAC de Genesis utilise un filtre anti-continu de 20 Hz. Le mode bruit et
le mode de fréquence spéciale du canal 3 sont hors du périmètre de cette implémentation d'instrument.

Pour une banque externe locale, utilisez `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`. Il accepte le TFI strict de
42 octets, le texte VOPM OPM, le format WOPLX vérifié et le SysEx DX7 original.
Chargez les fichiers `.cni` obtenus via Load Instrument. Les imports utilisateur sont conservés
séparément de la bibliothèque d'usine distribuable. Les fichiers OPM avec le bruit activé,
des données de fréquence de bruit non nulles ou un panoramique partiel sont refusés avec une explication ;
les valeurs de pan VOPM 0/64/127 deviennent gauche/les deux/droite. Le WOPL binaire n'est pas pris en charge.
Le catalogue FM partagé contient **1 064 entrées** : 697 OPL, 67 DX7, 146
OPLL/VRC7, 73 Genesis et 81 Arcade. 76 autres fichiers exposent les presets Sega/Game Boy
via le navigateur de fichiers normal, soit **1 140 presets natifs fournis**.
Les révisions de source, les empreintes, les mentions complètes et les données sources d'origine accompagnent les
nouvelles collections sous `licenses/chip-banks/expansion`.

### Collections d'usine portables et presets USER

Les instruments de puces natives utilisent **Bank:** pour choisir ALL, une collection incluse ou
USER. **Preset:** choisit un son dans cette sélection. Maintenez EDIT et appuyez sur
Gauche ou Droite sur l'une ou l'autre ligne pour parcourir ; un appui bref sur EDIT ouvre son sélecteur.

Les collections incluses sont des paquets ZIP sous `instruments/FACTORY/`. Les fichiers USER vont
sous `instruments/USER/<engine>/` : `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` ou `gb-noise`.
Parcourez les dossiers directement ou placez-y un ZIP de presets compatibles. Les ZIP restent
intacts sur le disque. Tous ces dossiers acceptent les fichiers `.cni` compatibles ; CNI est
le format d'instrument portable de ChooChoo/ChipNomad, pas un standard de fabricant de puces.
Le navigateur USER lit aussi les formats sources suivants, y compris à l'intérieur des ZIP :

| Dossier USER | Formats supplémentaires |
| --- | --- |
| `dx7` | voix uniques et banques de 32 voix Yamaha DX7/TX7 `.syx` |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | banques VOPM `.opm`, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | patches personnalisés Furnace `.fui` et sélections ROM fixes |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

Une banque s'ouvre sur des voix compatibles. Les fichiers ou fonctions non pris en charge signalent
une raison ; les voix incompatibles d'une banque mixte sont exclues tandis que leurs numéros
sources restent stables. Le chargement est transactionnel : un preset refusé laisse
l'instrument courant inchangé. Les presets sources remplacent la table du tracker de l'instrument
par une table vide, sauf le SysEx DX7, qui préserve sa table existante.
CNI restaure sa table enregistrée. Les patches chargés appartiennent au morceau et continuent
de fonctionner après le déplacement ou la suppression du paquet source.

Les banques WOPL peuvent contenir des emplacements inutilisés sans drapeau explicite de vide. Le navigateur
omet les emplacements sans nom dont les opérateurs actifs sont totalement atténués et sans
attack. Les voix jouables sans nom restent disponibles sous la forme `Program N [Bbank]` ou
`Drum N [Bbank]`, d'après les numéros de programme et de banque de la source, comptés à partir de zéro.
Un instrument OPLI unique sans nom utilise son nom de fichier. Les patches silencieux nommés sont
conservés.

La prise en charge de Furnace couvre les versions 29-112 à instrument unique héritées et les versions 127-251
`FINS` basées sur des fonctionnalités. La prise en charge de DefleMask couvre les instruments OPN, OPM, Sega PSG
et Game Boy de la version 11. Ce sont des imports d'instruments, pas des lecteurs de tracker
complets : les macros FM/d'opérateurs animées, les réglages de LFO FM du morceau source, les échantillons,
les séquences de commandes matérielles Game Boy, les macros relatives de pulse/filtre SID et les dépendances
ring/sync de SID sont actuellement refusés. La sélection ROM constante d'OPLL est
prise en charge ; VRC7 utilise sa propre ROM pour cette sélection. Les `.swi` de SID-Wizard et les fichiers de
musique SID (`.sid`) ne sont pas des imports d'instruments dans cette version.

Les séquences PSG/GB/SID prises en charge sont stockées dans le patch et jouées à 60 Hz pour
Furnace/DefleMask et à 50 Hz pour GoatTracker. Ces fichiers ne portent pas la configuration de lecture
complète d'un morceau : utilisez le tracker d'origine lorsque la cadence de ticks propre au morceau, le routage des canaux,
le comportement de hard-restart ou les réglages de compatibilité sont
nécessaires. Pour Sega PSG, une macro de duty sélectionne la voix de bruit ; sans elle, l'import
utilise une voix de tonalité. Les fichiers Game Boy doivent être placés dans le dossier pulse ou
noise voulu. Les séquences importées restent pour l'instant des données de lecture intégrées ; la page
de l'instrument édite les paramètres de base, pas les tables du tracker source.

Le parcours des presets USER continue à travers les fichiers, dossiers, banques et paquets ZIP dans
les deux sens, avec bouclage aux extrémités. Rouvrir le navigateur USER suit la
sélection courante. ALL inclut les sons USER sous Unsorted pour les moteurs
catégorisés ; USER reste un choix de premier niveau distinct. Sega PSG et Game Boy utilisent
des listes ALL/Factory plates et un navigateur USER hiérarchique, sans panneau
Unsorted. Les anciens fichiers DX7 de `instruments/banks/dx7/` restent accessibles dans USER.

Un moteur n'affiche que ses collections incluses compatibles. Factory Presets
combine les jeux de timbres OPLL/VRC7 inclus pour le moteur sélectionné ; DX7 combine
les originaux ChooChoo et YSE. Les collections nommées téléchargées restent séparées.
Fat Man 2-op est listé sous OPL2 et Fat Man 4-op sous OPL3. Les fichiers CNI OPL2
compatibles peuvent toujours être utilisés dans les dossiers USER d'OPL3.

La sortie des moteurs natifs utilise une compensation de gain fixe mesurée plutôt qu'une
normalisation par preset. Les morceaux existants à instruments natifs peuvent donc sonner
à un niveau différent ; vérifiez l'équilibre de vos mixages enregistrés. La méthode d'étalonnage
et ses bornes sont documentées dans `scripts/README_MEASURE.md`.

`native-chip-audition.cct` fournit une courte écoute séquentielle des treize banques
d'usine d'origine. Chaque section utilise un instrument propre différent, de sorte qu'il fonctionne même
si le dossier de presets est retiré. Des WAV de banques plus longs, ignorés par Git, et les niveaux mesurés sont
listés dans `docs/chip-preset-auditions.tsv` ; l'écoute subjective reste en attente.

### Contrôles des puces natives (Native chip controls)

Les aperçus de forme d'onde des instruments natifs se rafraîchissent lorsque leurs contrôles sont redessinés.
Les instruments FM peuvent superposer leur courbe Amp env facultative ; SID garde son aperçu de forme d'onde
sans cette superposition réservée à la FM, y compris pendant la navigation dans les banques et les presets.

Les sept moteurs FM proposent **Bright**, **Feedback** et **Amp env**. Bright
va de -63 à +63, zéro préservant le patch ; il modifie les niveaux des opérateurs
modulateurs tout en conservant ceux des porteurs. Son effet audible dépend de
l'algorithme. Feedback vaut **Preset** par défaut, ou remplace la rétroaction native
par 0-7. Ces contrôles ne réécrivent pas les octets d'opérateurs natifs enregistrés.

Amp env vaut **Bypass** par défaut. Choisissez **ADSR** pour ajouter attack, decay, sustain,
release et forme autour du son natif. A/D/R utilisent la plage quadratique commune de 0 à 5 secondes ;
le sustain va du silence au plein niveau. Les enveloppes natives continuent de
s'exécuter : cette enveloppe ne peut donc pas prolonger un son au-delà de son release natif. La
navigation dans les presets préserve les contrôles d'amplitude FM et de timbre de l'emplacement. Toutes les voix FM utilisent aussi
une transition de 3 ms à l'attaque/au redéclenchement et un lissage du gain du tracker de 1 ms, y compris
lorsque Amp env est en bypass. Le hard cut/panic reste immédiat.

Les FX de phrase suivants apparaissent aussi comme destinations prises en charge de la Modulation et de l'enregistrement de mouvement.
Les valeurs de la colonne FX sont hexadécimales. Les réglages s'appliquent à la lecture
sans modifier l'instrument enregistré. Les contrôles de sweep et d'enveloppe natifs de la GB
sont verrouillés au prochain déclenchement de note ; le duty/la largeur et la fréquence du bruit peuvent changer en direct.

| Moteurs | FX | Contrôle |
|---|---|---|
| FM natif | `OL1`-`OL6` | Niveaux de sortie absolus des opérateurs : modulateur OPLL/VRC7 et OPL `00-3F` ; porteuse OPLL/VRC7 `00-0F` ; Genesis/Arcade `00-7F` ; DX7 `00-63` (0-99). Plus haut signifie une sortie plus forte. Seuls les opérateurs pris en charge apparaissent. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Attack, decay, release et atténuation du sustain de l'opérateur 1, `00-0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | Multiplicateur de fréquence de l'opérateur 1. |
| Genesis/Arcade | `LFR` | Fréquence du LFO natif : Genesis `00-07`, Arcade `00-FF`. |
| Arcade | `LAD`, `LPD 00-7F` | Profondeurs d'amplitude et de hauteur du LFO séparées. |
| Genesis/Arcade | `LAS`, `LEN` | Sensibilité d'amplitude native `00-03` et activation du LFO `00-01`. |
| Genesis/Arcade | `LPS 00-07` | Sensibilité de hauteur native. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | Attack, decay, sustain et release natifs. Les valeurs de temps augmentent vers `0F` ; le sustain augmente vers le plein niveau. |
| SID | `SPR 01-10` | Fréquence de l'oscillateur partenaire silencieux de 1× à 16× ; affecte la modulation en anneau et la synchronisation forcée. |
| Tout FM natif | `FBK 00-07` | Rétroaction native absolue, initialisée à partir de l'instrument. |
| Tout FM natif avec Amp env activé ; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack, decay, sustain, release, forme. Ils n'activent pas un ampli FM en bypass. |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | Tonalité / bruit blanc / bruit périodique ; trois fréquences de bruit fixes ou fréquence dérivée de la tonalité. |
| GB Pulse | `CMD 00-03` | Duty d'impulsion natif. |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | Largeur du bruit, diviseur d'horloge et décalage. |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | Période du sweep, décalage, direction descendante. |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | Niveau initial de l'enveloppe native, période, direction montante. |

La sélection de FX natifs utilise l'instrument de la colonne `I` de la ligne de phrase, ou
l'instrument actif trouvé en remontant lorsque `I` est vide. La fenêtre de FX
affiche une courte description du contrôle, sa valeur de preset courante et la plage de commande valide
dans un bloc compact. Choisir un autre
effet natif démarre à cette valeur ; rouvrir le même effet conserve sa
valeur modifiée. Une sélection de plusieurs lignes résout séparément l'instrument de chaque ligne.
Les tables utilisent le contexte de leur instrument. Les modifications de valeurs natives s'arrêtent à leurs
extrémités légales, y compris duty, bruit, sweep, ADSR, feedback et niveaux d'opérateurs.
Les valeurs sont hexadécimales : par exemple, le maximum `63` du DX7 signifie 99 en décimal.

Ces commandes FM utilisent des valeurs natives absolues. Les commandes d'opérateur ciblent toujours
l'opérateur 1 ; `OMU 03` sélectionne son multiplicateur 3. Les contrôles propres au tracker pour DX7
sont les niveaux d'opérateurs et le feedback. Preset/Range garde sa couleur d'information ;
les titres et descriptions suivent les mêmes couleurs que FM Feedback.

La Modulation en direct conserve son jeu complet de destinations natives à opérateurs fixes.
L'enregistrement de mouvement n'écrit que les contrôles représentés par la liste compacte du tracker ;
les paramètres d'opérateur ne s'enregistrent que pour l'opérateur 1. Les autres destinations de modulation
continuent de fonctionner en direct mais ne génèrent pas de commandes de phrase.

`SCP` et `SCT` gardent des correspondances mises à l'échelle sur un octet vers des registres SID de plus de 8 bits.
`SMR 01-C8`, `SWV 01-08` et `SPR 01-10` correspondent désormais à la numérotation native des presets.
Les valeurs de preset affichées sont des réglages de base, indépendants de l'enveloppe
ou du LFO en cours. `SLE` agit sur les niveaux d'opérateurs et les paramètres FM directs sans
redéclencher la note ni modifier le patch enregistré.

Tous les FX natifs ont des titres, plages et comportements descriptifs dans le sélecteur de FX de
phrase, plus des indices de valeur. `CMD` décrit tonalité/bruit, duty ou largeur du bruit
selon l'instrument sélectionné. Ces contrôles complètent les groupes communs Track,
Envelope et Modulation ; le groupe du moteur seul n'est pas l'ensemble complet
des effets de phrase disponibles.

Les filtres LP/BP/HP communs existants sur Braids, Plaits, PCM et d'autres moteurs pris en charge
sont un traitement logiciel après la synthèse. aChChid utilise à la place son chemin de filtre
303 natif. Cette version n'ajoute aucun filtre à l'AY ni à la FM native ; la brillance change
la synthèse FM elle-même. Les inserts de piste restent disponibles pour un traitement supplémentaire.

Les instruments natifs sont enregistrés en CNI version 7 (8 lorsque des commandes absolues sont présentes),
et les morceaux natifs en CCT version 9. Les instruments contenant des séquences importées
utilisent CNI version 9 et les morceaux qui en contiennent utilisent CCT version 10. Les versions plus anciennes
ne peuvent pas ouvrir ces fichiers plus récents contenant des séquences. Les octets de preset FM enregistrés et
les réglages de timbre de la page d'instrument restent inchangés.

### Charge de travail sur console portable pour les puces natives

Il y a huit pistes de morceau et **deux emplacements d'effets d'insertion par piste**. Les
seize emplacements disponibles ne garantissent pas assez de CPU pour exécuter seize effets.
La limite de seize notes actives du DX7 est un budget de synthèse distinct pour tout le morceau ;
les accords et les queues de release y comptent.

Dans les mesures sur R36H, quatre pistes WAV en boucle, Sega PSG, GB Pulse, DX7 et
OPL3 avec envois partagés et quatre inserts (deux Compressors, Doubler, TAPESCAM)
ont mieux fonctionné que des arrangements FM denses à huit pistes. La version à une seule note
n'a eu aucun dépassement d'échéance de rendu dans sa mesure de trente secondes à 48 kHz/512 frames ;
l'ajout d'un accord DX7 de quatre notes a produit un pic de timing. Les morceaux FM denses
avec de nombreux inserts coûteux ont dépassé le budget CPU. Gardez de la marge,
surveillez l'indicateur de surcharge existant, et ajoutez des effets là où ils aident le morceau.
L'application conserve chaque piste et chaque emplacement ; il n'y a pas de nouvelle limite stricte au nombre d'inserts.
Voir `chip-instruments-report.md` pour les mesures complètes et l'état des tests prolongés.

Le test audio physique de 70 secondes correspondant, sur cet arrangement d'accords équilibré,
a réussi sans dépassement d'échéance de rendu ni sous-alimentation ALSA enregistrée au
réglage existant de 48 kHz / 4906 frames. Son pire callback a duré 81,819 ms pour une
échéance de 102,208 ms. Les tests ont utilisé un gain master séparé de 0,4 pour garder de la marge.
Conservez la route directe vers la carte du lanceur habituel, `AUDIODEV=plughw:0,0` : le
mixeur partagé par défaut du système a produit des sous-alimentations dans les sondes de diagnostic. Aucun
réglage audio de l'utilisateur n'a été modifié. Le test de contrainte de dix minutes avec un tampon plus petit a tout de même enregistré
28 pics de timing ; les résultats complets figurent dans le rapport.

### Instruments SID et FX de phrase

Choisissez **CHIP → SID**. Bank propose **ChooChoo SID Originals** (32 programmes
composés) et **SIDkit Effects** (24 effets sous licence MIT). Les deux utilisent le même
navigateur de presets et enregistrent leur programme complet sélectionné dans l'instrument
et le morceau. Aucune banque externe n'est nécessaire pour partager le morceau. Ce sont des programmes de
paramètres SID natifs avec enveloppes et recettes de mouvement, pas de l'audio échantillonné ni
des morceaux C64 complets. Les douze candidats de recherche GoatTracker, distincts, ne sont
pas fournis ; leurs tables wave/pulse/filter demandent un importeur/lecteur dédié.

La page de l'instrument expose forme d'onde/largeur d'impulsion, mode de filtre/fréquence de coupure/résonance
et contrôles ADSR natifs. SID utilise l'oscillateur numérique, l'enveloppe et le filtre
par cycle de style 6581 de floooh/chips, épinglés à une version précise. Il n'y a ni modèles de puce alternatifs,
ni sélecteurs de révision, ni profils de caractère ajoutés. C'est une approximation générique,
pas un modèle analogique étalonné des R2/R3/R4/8580.

Chaque note a son propre filtre. Ring/sync utilisent un oscillateur partenaire silencieux, si bien que
le partage de filtre et les interactions à trois voix diffèrent d'un vrai SID.
La console portable a un budget de quatre notes SID ordinaires pour tout le morceau.
Ring ou sync coûte deux unités de budget par note, ce qui permet deux de ces notes, ou une plus deux
notes ordinaires. Les accords et les queues de release comptent. Les notes relâchées sont retirées
en premier ; les nouvelles notes ont priorité sur les anciennes notes tenues. Les inserts coûteux et
les autres synthés partagent toujours le budget CPU audio.

| FX | Plage | Action |
| --- | --- | --- |
| `SCP` | `00-FF` | Base de largeur d'impulsion, mise à l'échelle sur la largeur native de 12 bits ; le mouvement de pulse de la recette reste actif. |
| `SCT` | `00-FF` | Base de fréquence de coupure, mise à l'échelle sur la coupure native de 11 bits ; le mouvement de filtre de la recette reste actif. |
| `SRN` | `00-0F` | Résonance native. |
| `SWV` | `00-07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00-07` | Bits de mode de filtre : 1 passe-bas, 2 passe-bande, 4 passe-haut ; 0 bypass. |
| `SMR` | `00-FF` | Horloge de macro de recette, mise à l'échelle de 1 à 200 Hz. |
| `SRG` | `00-01` | Modulation en anneau triangulaire avec partenaire silencieux. |
| `SSY` | `00-01` | Synchronisation d'oscillateur avec partenaire silencieux. |

La largeur d'impulsion et la fréquence de coupure prennent en charge `SLE`. Les formes d'onde discrètes, les interrupteurs, la résonance
et la rétroaction FM gardent leurs pas natifs utiles ; des valeurs d'octet plus larges ne
créeraient pas plus d'états matériels. `FBK` sélectionne directement le feedback `00-07`.
Les niveaux d'opérateurs utilisent les plages propres à chaque moteur listées plus haut. Les réglages de brillance et
de master gardent toute la plage d'un octet.

### Collections d'usine portables et presets USER

Les instruments de puces natives utilisent **Bank:** pour choisir ALL, une collection incluse ou
USER. **Preset:** choisit un son dans cette sélection. Maintenez EDIT et appuyez sur
Gauche ou Droite sur l'une ou l'autre ligne pour parcourir ; un appui bref sur EDIT ouvre son sélecteur.

Les collections incluses sont des paquets ZIP sous `instruments/FACTORY/`. Les fichiers USER vont
sous `instruments/USER/<engine>/` : `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` ou `gb-noise`.
Parcourez vos dossiers directement, ou placez-y un ZIP de dossiers et de presets `.cni`
compatibles. DX7 accepte aussi les voix uniques et les banques de 32 voix `.syx` prises en charge,
y compris à l'intérieur des ZIP. Les ZIP restent intacts sur le disque. Le chargement direct de WOPL/WOPLX n'est pas pris en charge ;
utilisez des presets CNI compatibles ou convertissez d'abord hors ligne les formats sources pris en charge.

Le parcours des presets USER continue à travers les fichiers, dossiers, banques et paquets ZIP dans
les deux sens, avec bouclage aux extrémités. Rouvrir le navigateur USER suit la
sélection courante. ALL inclut les sons USER sous Unsorted pour les moteurs
catégorisés ; USER reste un choix de premier niveau distinct. Sega PSG et Game Boy utilisent
des listes ALL/Factory plates et un navigateur USER hiérarchique, sans panneau
Unsorted. Les anciens fichiers DX7 de `instruments/banks/dx7/` restent accessibles dans USER.

Un moteur n'affiche que ses collections incluses compatibles. Factory Presets
combine les jeux de timbres OPLL/VRC7 inclus pour le moteur sélectionné ; DX7 combine
les originaux ChooChoo et YSE. Les collections nommées téléchargées restent séparées.
Fat Man 2-op est listé sous OPL2 et Fat Man 4-op sous OPL3. Les fichiers CNI OPL2
compatibles peuvent toujours être utilisés dans les dossiers USER d'OPL3.

La sortie des moteurs natifs utilise une compensation de gain fixe mesurée plutôt qu'une
normalisation par preset. Les morceaux existants à instruments natifs peuvent donc sonner
à un niveau différent ; vérifiez l'équilibre de vos mixages enregistrés. La méthode d'étalonnage
et ses bornes sont documentées dans `scripts/README_MEASURE.md`.
