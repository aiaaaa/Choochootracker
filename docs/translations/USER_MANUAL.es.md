# Manual de usuario de ChooChooTracker

> Traducción al español de [`docs/USER_MANUAL.md`](../USER_MANUAL.md), hecha a partir de la versión inglesa del commit `7c3ed6f`. Si hay diferencias, prevalece la versión inglesa.
>
> **Convenciones.** La interfaz de la aplicación está en inglés. Por eso los nombres de pantallas, menús, campos y botones (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**, **OPT**, **SELECT**...), los códigos de FX (`HOP`, `TBX`, `M1A`...), los nombres de módulos y motores (ADSR, LFO, Braids, Plaits...) y las extensiones de archivo se dejan sin traducir, para que los reconozcas en pantalla.

ChooChooTracker es un tracker musical de 8 pistas para consolas portátiles.

Sigue el flujo de trabajo y el sistema de navegación clásicos de LSDj, con varias opciones de diseño sonoro:

- emulación del chip AY
- osciladores Mutable Instruments Braids, Plaits y Plaits-Alt, con un filtro multimodo
- muestras PCM, formas de onda de ciclo único y osciladores de tabla de ondas, con un filtro multimodo
- aChChid, un versátil sintetizador acid basado en Open303
- Bogie, un sintetizador de batería nativo de 12 modelos, con un filtro multimodo
- MME, un sintetizador de dos osciladores y modulación múltiple, con un filtro multimodo
- Sintered, un sintetizador de batería digital experimental nativo de seis modelos

## 1. Instalación y archivos

### ArkOS / PortMaster

Instala `choochootracker.zip` con PortMaster, o copia el paquete extraído en el directorio `ports` de la consola. Inicia **ChooChooTracker** desde el menú Ports.

### Miyoo Mini / Mini Plus (Onion OS)

Extrae el archivo MiyooPorts en la raíz de la tarjeta SD, de modo que sus carpetas `Roms/PORTS/` se fusionen con las que ya tiene Onion. Inicia **ChooChooTracker** desde el menú Ports después de actualizar la colección. Haz una copia de seguridad de la carpeta `projects/` de la aplicación antes de reemplazar una instalación existente.

### Windows

Ejecuta `choochootracker.exe` con `SDL2.dll` y `libwinpthread-1.dll` en el mismo directorio.
Pulsa **ALT + ENTER** para alternar la pantalla completa. La ventana se puede redimensionar; la imagen
se escala manteniendo su relación de aspecto original.

### Web

La página del navegador sigue el tema activo del tracker. Los ajustes guardados, incluidas
las asignaciones de teclas personalizadas confirmadas con **Done**, se escriben enseguida en el
almacenamiento de este navegador. Tras bloquear la pantalla o cambiar de aplicación, toca un control para recuperar
el audio si hace falta. **Restart audio** vuelve a abrir la salida sin descartar el
proyecto. **Reload app** guarda el trabajo actual en **Continue**, guarda los ajustes
y espera al almacenamiento del navegador antes de recargar; si el guardado falla o agota el tiempo,
la recarga se cancela. Usa **Download .cct** para una copia de seguridad portátil. El almacenamiento del navegador
puede borrarse de forma independiente, y la recuperación del enrutamiento de mandos y audio en iOS
no está garantizada.

La versión de navegador actualiza la pantalla a unos 30 FPS y usa un búfer de audio fijo de
1024 fotogramas para dar más margen al audio entre llamadas. Los ajustes de búfer antiguos
guardados se corrigen automáticamente; los proyectos y los demás ajustes se
conservan. El sondeo de entradas y los temporizadores de la aplicación mantienen su cadencia habitual.

### Configuración de directorios

| Carpeta | Qué contiene |
|---|---|
| `AY_Wavetables` | tablas de ondas `.aywave` del chip AY |
| `fonts` | archivos de fuente ChipNomad `.cnfont` |
| `instruments` | presets de instrumento `.cni`, incluidos los archivos de ChipNomad |
| `pitch-tables` | afinaciones alternativas `.csv` |
| `projects` | archivos de canciones y proyectos `.cct` |
| `samples` | archivos `.wav` para el motor Sampler |
| `SR_wavetables` | tablas de ondas en formato Serum para el motor BYOWTBL |
| `themes` | temas de ChipNomad `.cth` |
| `title` | recursos de la pantalla de título |
| `waveforms` | formas de onda `.wav` de ciclo único para el motor 2xSCWF |

## 2. Controles

ChooChooTracker funciona mejor con un mando que tenga cruceta, 2 sticks analógicos y 7 botones.

| Control lógico | Valor por defecto en consola portátil | Valor por defecto en Windows |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | cruceta | teclas de flecha |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Espacio |
| **SELECT** | Select | Shift |
| Ejes de modulación | sticks izquierdo y derecho | sticks del mando |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Las asignaciones se pueden cambiar en **Settings > Key mapping**. En PortMaster, suelta
el botón que usaste para seleccionar una asignación antes de pulsar el botón nuevo. Guarda una
reasignación con **Done** y suelta el botón antes de seguir en Settings.
Las distribuciones de escritorio por defecto usan QWERTZ solo en las regiones QWERTZ; los teclados polacos
mantienen los valores por defecto QWERTY estándar.

Los usuarios de Windows, web y Android también pueden usar un mando. En Android,
los controles táctiles aparecen automáticamente cuando no hay ningún mando conectado: en vertical
se sitúan debajo del tracker, con una cruceta cuadrada, los controles A/EDIT, B/OPT, START/PLAY y
SELECT/SHIFT, y después una sección Live/Motion aparte con dos sticks de modulación
y controles REC/DEL momentáneos. En horizontal, la cruceta queda a la izquierda
y los botones de acción a la derecha. Los proyectos y muestras de Android son privados de
la aplicación; usa el selector de archivos de Android para importar proyectos `.cct` y muestras `.wav`,
y para elegir dónde se guardan las exportaciones WAV.

### Navegación

- Usa **[UP/DOWN/LEFT/RIGHT]** para mover el cursor.
- Mantén pulsado **SELECT + [DIRECTION]** para moverte entre pantallas.
- **SELECT + [LEFT/RIGHT]** también saca de las páginas secundarias hacia la pantalla MSCPIT
  vecina, como Instrument Pool: Project/Settings (incluidos los submenús de Settings)
  llevan a Mixer/Chain; Groove a Chain/Instrument; Modulation/Insert FX a
  Phrase/Table; AY Wavetable a Instrument/Table. En el borde izquierdo, Reverb/Delay
  vuelven al Mixer con SELECT + LEFT. La navegación vertical existente no cambia.
- Mantén pulsado **OPT + [DIRECTION]** para la navegación propia de cada pantalla.
- Sobre un valor, usa **EDIT + [LEFT/RIGHT]** para cambios finos o **EDIT + [UP/DOWN]** para cambios gruesos.
- Sobre una **Destination** de modulación, **LEFT/RIGHT** recorre directamente los destinos disponibles; toca **EDIT** para abrir su menú agrupado.
- Toca **EDIT** para introducir un valor o activar un comando.
- Toca **EDIT** dos veces para crear una nueva cadena, frase o instrumento donde se admita.
- Mantener una dirección pulsada la repite tras el retardo configurado en Settings.

### Selección y portapapeles

- **SELECT + OPT** entra en el modo de selección y recorre rangos de selección útiles.
- Usa **[DIRECTION]** para ampliar el rango.
- **OPT** copia la selección.
- **OPT + EDIT** la corta.
- **SELECT + EDIT** la pega.
- **EDIT + [DIRECTION]** edita juntas las celdas seleccionadas.

En la pantalla Phrase, con una selección activa, la columna de notas se puede rellenar
con patrones rítmicos:

- **OPT + LEFT** rellena las celdas de notas de la selección con un patrón fijo de 16 pasos
  y pasa al siguiente patrón en cada pulsación. Tras el último patrón, las
  notas se borran y el ciclo vuelve a empezar. Los patrones están alineados con las filas
  absolutas de la frase, por lo que una selección parcial solo recibe los pasos que caen dentro de ella.
- **OPT + RIGHT** rellena las celdas de notas de la selección con un patrón aleatorio de
  3 a 8 notas (7 y 8 son menos probables), en ciclo indefinido mientras se mantenga
  pulsado **OPT**.
- **OPT + UP** muta las notas ya colocadas en la selección: cada pulsación
  elige de 1 a 3 de ellas y desplaza cada una una cantidad aleatoria dentro de una octava
  (los desplazamientos pequeños son más probables que los extremos). Con **Note Lock** activo,
  las notas desplazadas se ajustan de nuevo a la escala; los instrumentos Sampler con
  **Slice** activado siguen siendo cromáticos. No se borra nada: la 8.ª pulsación restaura las
  notas que contenía la selección al empezar el ciclo, y después el ciclo vuelve a empezar.
- **OPT + UP** sobre una selección que cubre solo la columna de volumen (sin celdas de nota,
  instrumento ni FX) aleatoriza las velocidades en lugar de mutar: la
  primera pulsación aplica el patrón Minimal (cada volumen se desplaza hasta
  +-20), la segunda Medium (hasta +-50), la tercera Random (valores
  totalmente aleatorios), y la cuarta restaura los volúmenes que contenía la selección
  al empezar el ciclo. Soltar **OPT** devuelve el ciclo a Minimal.
  Las filas sin volumen no se tocan.
- **OPT + DOWN** extiende las notas colocadas de los instrumentos Sampler cortados (sliced) de
  la selección en una serie cromática: la primera nota de cada instrumento (la más alta
  en el secuenciador) conserva su valor y cada nota siguiente de ese
  instrumento sube un semitono sobre la anterior, convirtiendo una pila de
  notas idénticas en disparos de slices consecutivos. Varios instrumentos
  cortados se extienden de forma independiente a partir de sus propias primeras notas, las notas
  de instrumentos sin corte no se tocan, y el resultado nunca se
  ajusta a la escala del proyecto. La última nota que escribe la extensión pasa a ser
  la última nota usada en la entrada, de modo que seguir la serie a mano retoma
  el siguiente slice. Una selección que contiene solo instrumentos cortados
  se extiende en cada pulsación; cuando también hay notas sin cortar,
  la extensión se ejecuta en la primera pulsación y, a partir de la segunda, el arpegio aleatorio se encarga de
  esas notas.
- **OPT + DOWN** sobre una selección sin notas de instrumentos cortados es un arpegiador
  aleatorio para las notas ya colocadas allí. Se elige un acorde
  de la paleta CRD - acordes de 3 notas para selecciones de 1 a 2 notas, acordes de 3 o más notas
  para 3, cualquier acorde para 4 o más - con su fundamental en una nota aleatoria
  de la escala del proyecto dentro del registro de la selección (cualquier nota
  cromática cuando la escala está desactivada o la pista está excluida), y sus notas
  se cuantizan a la escala como hace la reproducción para CRD. Cada
  pulsación vuelve a repartir el acorde entre las notas colocadas: aleatoriamente,
  de la más grave a la más aguda, de la más aguda a la más grave, y luego de ida y vuelta; la 5.ª pulsación
  restaura las notas originales y la siguiente elige un acorde nuevo.
  El nombre del acorde aparece en la barra de estado mientras el arpegio está activo, no se
  añaden ni se quitan notas, y soltar **OPT** reinicia todo el
  proceso.
- Cada pulsación de relleno borra las notas de la selección antes de escribir, y las notas
  colocadas llevan el último instrumento usado; soltar **OPT** devuelve los ciclos
  a su primer paso.
- Mientras hay una selección activa y no se muestra otro mensaje, la barra
  de estado va mostrando cada 2 segundos un recordatorio de estas combinaciones: `EDIT + DIR =
  batch note edit`, `Double-tap EDIT: resample selection`, `OPT + LEFT =
  pattern fill`, `OPT + RIGHT = random fill`, `OPT + UP =
  mutate/randomize velocity` y `OPT + DOWN = random arp/slice spread`.
  Entrar en el modo de selección o salir de él reinicia la rotación; cualquier mensaje
  de acción sustituye a la pista hasta que expira.

### Reproducción

- **PLAY** empieza desde el cursor. Púlsalo de nuevo para detener.
- Puedes editar notas, instrumentos y cadenas durante la reproducción; los cambios se oyen
  en el siguiente tick del secuenciador.
- **SELECT + PLAY** inicia todas las pistas cuando se usa fuera de la pantalla Song.
- **EDIT + PLAY** escucha un instrumento desde Instrument Pool.

## 3. Estructura de la canción y mapa de pantallas

Mantén pulsado **SELECT + [DIRECTION]** para moverte entre pantallas.

La fila de navegación principal es:

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Fila central:

- **M**: Mixer (da acceso a Reverb, Delay)
- **S**: Song (da acceso a Project, Settings)
- **C**: Chain
- **P**: Phrase (da acceso a Groove)
- **I**: Instrument (da acceso a Modulation y a Instrument Pool)
- **T**: Table (da acceso al editor AY Wavetable)

Fila superior:

- **R**: Reverb
- **P**: Project
- **G**: Groove
- **M**: Modulation

Fila inferior:

- **D**: Delay
- **S**: Settings
- **P**: Instrument Pool
- **W**: editor AY Wavetable

Una canción de ChooChooTracker sigue esta jerarquía:

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

Una canción tiene 8 pistas. Cada pista contiene una secuencia de cadenas, y cada cadena contiene una secuencia de frases. Las frases son secuencias de 16 pasos de notas. Una nota contiene una altura, un instrumento, una velocidad y hasta 3 FX de pista. Un instrumento es lo que produce el sonido.

### Fuentes opcionales de 16x24 y NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 contiene TechMonoAudit (convertida a partir de Share Tech Mono), Departure Mono, Spleen y Cozette. Cada una ofrece el conjunto ASCII imprimible completo en celdas de 16×24 para una pantalla de 640×480. Las fuentes construidas con píxeles conservan sus píxeles originales o un escalado entero con relleno; Cozette sigue siendo más pequeña porque duplicar su altura de tinta de 13 píxeles superaría la celda. El tema NostromoAmberDa2 está disponible en Settings → Edit color theme → Load. Conserva el fondo ámbar y el texto informativo de Nostromo, con texto principal rojo, valores naranja más oscuros y un cursor verde azulado. Cargar estos recursos no cambia la fuente ni el tema que hayas elegido. Consulta el README.txt que los acompaña para las atribuciones y licencias.

### Espaciado de la página Project

La página Project pone la versión de la aplicación en su propia línea y separa Load, Save, New, Export y Manage de los metadatos del proyecto con filas en blanco. Las posiciones del cursor y las zonas táctiles siguen los campos mostrados; los ajustes del proyecto y las operaciones con archivos no cambian.

### Telemetría de la visualización de audio

Los indicadores de audio usan una instantánea, propia de la interfaz, de la mezcla final y de la contribución de cada pista. Las muestras de pista y los picos estéreo se capturan después del nivel de pista y del tilt, antes de los retornos de efectos compartidos y de la ganancia master. La monitorización no modifica la señal de audio. El callback de audio publica instantáneas de tamaño fijo sin bloqueos ni asignación de memoria; el historial mostrado abarca unos 21 ms.

### Forma de onda persistente

Graphics → Persistent waveform está en OFF por defecto. Toca EDIT para alternarlo o usa EDIT + dirección; la elección se guarda como preferencia de la aplicación. OFF restablece las páginas a altura completa y las vistas previas de forma de onda de instrumento existentes. Cuando se activa, una forma de onda de dos filas sobre las páginas de edición muestra la mezcla final sumada a mono. Las páginas Instrument y Modulation muestran la salida de una pista que usa el instrumento seleccionado; si varias pistas lo usan, gana la pista de Song seleccionada. Project, Settings, las tablas de ondas y las ventanas emergentes conservan su contenido a altura completa. Las listas se desplazan dentro de las filas restantes. Los pies de las páginas Instrument conservan las vistas previas de ADSR/muestra, y los campos de Modulation se compactan para mantener accesibles todos los controles. Sus zonas táctiles siguen los campos mostrados en ambos modos de forma de onda.

### Piano de reproducción

Un piano de píxeles dibujado por código, bajo las ocho filas de pistas de la derecha, ilumina las clases de altura que suenan, plegadas en una octava. Incluye las notas de acorde y respeta las pistas silenciadas o detenidas. El contorno usa el color de valor del tema y todas las teclas activas usan el color de la forma de onda; las teclas inactivas tienen un relleno oscuro y sombras sutiles. Su cuadrícula usa escalado entero y bordes de ancho fijo. El piano es un indicador de reproducción, no un control de entrada de notas. Las ventanas emergentes y el editor de tablas de ondas conservan su propio contenido en esta zona.

### Medidores del Mixer

El Mixer muestra barras de pico **L/R** adyacentes para cada pista, después de su PAN y su tilt, antes de los efectos compartidos y de la ganancia master. El medidor abarca de -48 a 0 dBFS y decae entre actualizaciones. Sus bandas baja y media usan los colores del tema, mientras que la banda superior es de un rojo brillante cerca de la escala completa.

## 4. Song, Chain y Phrase

### Song

La pantalla Song contiene 8 columnas, 1 por pista de audio. Cada celda hace referencia a una cadena. Una canción puede contener hasta 256 filas (`00-FF`).

Las pistas avanzan de izquierda a derecha, mientras que sus cadenas avanzan en vertical. Cada pista es independiente, y las cadenas pueden contener de 1 a 16 frases, por lo que las pistas pueden desincronizarse. Diseña tus cadenas para que se mantengan juntas, o déjalas divagar si eso es lo que necesita la pista.

Es habitual reservar 1 cadena para frases vacías, normalmente `00` o `FE`.

Puedes mantener varias subcanciones en un proyecto creando «islas»: secciones separadas por filas vacías.

#### Live Mode

Toca **OPT** dos veces para entrar en el Live Mode o salir de él; `LIVE` sustituye al título Song. En Live Mode, **PLAY** inicia la cadena seleccionada en su propia pista cuando esa pista está detenida. En una pista que está sonando, pone en cola la cadena seleccionada para el siguiente límite de cadena; pulsa **PLAY** dos veces para un cambio urgente en el siguiente límite de frase. Una celda de Song vacía seleccionada pone en cola, en su lugar, una parada cuantizada. `+` marca una cadena en cola, `!` una cadena urgente y `-` una parada en cola.

Selecciona una fila de Song en varias columnas para poner en cola o detener esas pistas juntas. Las selecciones de varias filas se rechazan deliberadamente. Las colas de Live se borran mientras hay un bucle de rango de Song activo. Live es un modo de interpretación y no se guarda en el proyecto.

#### Controles

- **OPT + [UP/DOWN]**: saltar 16 posiciones hacia arriba o abajo
- Selecciona un rango y luego **SHIFT + EDIT**: clonar superficialmente las cadenas conservando sus frases originales
- Selecciona un rango y luego toca dos veces **SHIFT + EDIT**: clonar en profundidad tanto las cadenas como sus frases
- Selecciona un rango y luego **EDIT + [UP/DOWN]**: mover la selección hacia arriba o abajo
- Toca **OPT** 3 veces: añadir o quitar un resaltado, para la organización visual
- **OPT + SHIFT**: silenciar la pista actual o las columnas seleccionadas (suelta primero **OPT** para que el silencio siga activo)
- **OPT + PLAY**: poner en solo la pista actual o las columnas seleccionadas (suelta primero **OPT** para que el solo siga activo)
- **OPT + [LEFT/RIGHT]**: poner en solo todas las pistas a la izquierda o a la derecha de la pista actual
- Selecciona un rango y luego toca dos veces **EDIT**: abrir la pantalla BOUNCE TO SAMPLE (véase Bounce más abajo)

#### Bounce

Selecciona un rango de filas y columnas de Song y toca dos veces **EDIT** para abrir la pantalla BOUNCE TO SAMPLE. El bounce incluye todas las pistas seleccionadas: las pistas cuya primera cadena empieza más tarde dentro de la selección esperan en silencio y se incorporan al comienzo de su primera cadena, de modo que el archivo contiene las cadenas de todas las pistas seleccionadas.

La pantalla ofrece:

- **File name**: se edita con el teclado de caracteres. Al abrir la pantalla, aquí se propone el siguiente número de secuencia libre (`001`, `002`, ...): el primer número cuyo archivo `.wav` todavía no existe en la carpeta de exportación. Edítalo libremente: el nombre se usa tal cual. Si ya existe un archivo con ese nombre, se añade un sufijo `_001`, `_002`, ... para que los archivos existentes nunca se sobrescriban. El contador se reinicia al cargar o crear un proyecto.
- **Sample rate** y **Bit depth**: las mismas opciones que la pantalla de exportación
- **Include in a sample name**: tres casillas que anteponen etiquetas de contexto al nombre del archivo al pulsar **Start** (todas desactivadas por defecto):
  - **[BPM]** - el tempo actual del proyecto, por ejemplo `[120]`
  - **[Key]** - la nota fundamental y la escala del proyecto, por ejemplo `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** - la duración renderizada del bounce: una frase equivale a un compás (16 semicorcheas), un tiempo son 4 semicorcheas. Las frases completas solo muestran compases, por ejemplo `[2]`; las frases parciales muestran `[0:2]` (dos tiempos) o `[0:1:3]` (un tiempo más tres semicorcheas). En los bounces de varias pistas decide la pista más larga. Una selección que termina a mitad de un tiempo cuenta una semicorchea menos: la última fila seleccionada es el punto de corte, por lo que su note-off cae al final de la semicorchea anterior.
  - Las etiquetas se aplican en el orden `[BPM][Key][Length]` antes del nombre del archivo.
- **Start**: renderiza la selección en un archivo WAV de la carpeta de exportación (véase *Export location* en la pantalla Project)
- **Cancel**: volver sin hacer bounce

Mientras se renderiza el bounce, **OPT** lo cancela. Cuando el bounce termina o se cancela, vuelve la pantalla desde la que lo iniciaste.

En ordenador, el key jazz también aporta aquí la escritura directa de índices hexadecimales, la selección
con Shift+flechas como en Phrase, Delete/Backspace/Insert y Ctrl+C/X/V - véase
[Key jazz](#key-jazz-solo-en-ordenador).

### Chain

Una cadena es una lista ordenada de frases de 16 pasos, con transposición opcional. Puede contener hasta 16 frases, y la misma frase puede aparecer más de una vez. La 2.ª columna fija la transposición en semitonos.

Aparece un asterisco (`*`) junto a una cadena que se reutiliza en el proyecto. Puedes clonar cadenas desde la pantalla Song.

#### Controles

- **OPT + [LEFT/RIGHT]**: moverse entre pistas
- **OPT + [UP/DOWN]**: moverse entre cadenas de la pista actual
- Selecciona un rango y luego **SHIFT + EDIT**: clonar frases
- Selecciona un rango y luego toca dos veces **EDIT**: abrir la pantalla BOUNCE TO SAMPLE para las filas de cadena seleccionadas de la pista actual (véase Bounce en Song)

En ordenador, el key jazz también aporta aquí la escritura directa de índices hexadecimales - véase
[Key jazz](#key-jazz-solo-en-ordenador).

### Phrase

Una frase es el patrón de una pista en un secuenciador de pasos tradicional.

Una frase tiene 16 pasos, con 1 fila por paso. Cada fila contiene una nota, un instrumento, un volumen de `00` a `7F` y columnas de FX. Las notas usan la notación de tracker, como `C-4` (nota y octava).

Para detener una nota, inserta `NOTE OFF`, usa el FX de corte de nota, o toca otra nota sin instrumento definido.

Cuando una cadena contiene varias frases, la pantalla Phrase funciona como un editor de patrones continuo. Bajar por debajo de la fila `F` o subir por encima de la fila `0` abre la frase siguiente o anterior de la cadena.

Aparece un asterisco (`*`) junto a una frase que se reutiliza en el proyecto. Puedes clonar frases desde la pantalla Chain.

El selector de FX muestra los comandos comunes más los que admite el instrumento de esa fila. Así se evita una larga lista global de comandos específicos de cada motor. ChooChooTracker admite más FX que ChipNomad.

### Controles

- **OPT + EDIT** sobre una nota vacía: insertar `NOTE OFF`
- **EDIT + [UP/DOWN]** sobre una columna de nombre de FX: abrir la pantalla de selección de FX
- **OPT + [LEFT/RIGHT]**: moverse entre pistas
- **OPT + [UP/DOWN]**: moverse entre frases de la cadena actual
- Selecciona un rango en la columna de instrumento y luego **SHIFT + EDIT**: clonar instrumentos
- Selecciona un rango y luego **EDIT + [UP/DOWN]**: rotar las filas de la frase
- Selecciona un rango y luego toca dos veces **EDIT**: abrir la pantalla BOUNCE TO SAMPLE para las filas de frase seleccionadas de la pista actual (véase Bounce en Song)

### Key jazz (solo en ordenador)

Pulsa **Esc** en la pantalla Phrase para activar o desactivar el key jazz, un modo de entrada
de notas al estilo de un piano QWERTY (como [m8c](https://github.com/laamaa/m8c)):

- `Z S X D C V G B H N J M` y `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` escriben notas
  cromáticamente a lo largo de más de dos octavas, usando la posición física de cada tecla,
  de modo que la distribución es la misma en teclados AZERTY, QWERTY y QWERTZ.
- Cada pulsación escribe la nota en la fila actual y pasa a la siguiente.
- `[` / `]` cambian la octava.
- **Shift + flechas** seleccionan un rango de filas y columnas; una flecha simple
  después borra la selección.
- **Ctrl+C** / **Ctrl+X** copian o cortan las columnas seleccionadas, o la
  nota/instrumento/volumen de la fila actual si no hay nada seleccionado; **Ctrl+V** pega en
  el cursor.
- **Delete** quita la fila o filas enteras (todas las columnas) y sube el resto de
  la frase para tapar el hueco; el cursor se queda en la misma fila.
- **Backspace** quita el elemento bajo el cursor solo en la columna actual
  (o las columnas de la selección, si hay una activa), sube lo que haya
  debajo para tapar el hueco y hace retroceder el cursor una fila, como en
  un editor de texto.
- **Insert** inserta una fila en blanco y empuja el resto de la frase hacia abajo.
- **Ctrl+S** guarda el proyecto. Fuera del key jazz, Ctrl+S no hace nada.
- Esc de nuevo desactiva el key jazz.

Mientras el key jazz está activo, toma por completo las teclas de nota de la pantalla
Phrase, incluidos los atajos Edit/Opt/Motion que comparten esas teclas - desactívalo
con Esc para volver a usarlos.

El key jazz también funciona, activado de forma independiente con Esc, en:

- **Song** y **Chain**: escribe directamente el índice hexadecimal de una cadena o de una frase
  (`0-9`, `A-F`) en lugar de incrementar con Up/Down. El primer dígito escrito
  en una celda sustituye su valor; los dígitos siguientes, escritos sin moverse, se desplazan
  dentro de él (escribir «3» y luego «F» da 3F). En Song, **Shift+flechas** seleccionan un
  rango, **Delete** quita la fila o filas enteras y sube el resto, **Backspace**
  quita la columna o columnas actuales/seleccionadas y sube lo que hay debajo,
  **Insert** inserta una fila en blanco, y **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  copian/cortan/pegan las filas seleccionadas (o la fila actual si no hay nada
  seleccionado) - son los mismos atajos de edición de estructura que en la pantalla Phrase.
  Esto toma Shift, así que Shift+Right/Up ya no saltan a Chain/Project
  mientras el key jazz esté activo aquí - pulsa Esc para recuperar esos atajos.
- **Cualquier pantalla con un campo de texto**: escribe directamente en el teclado
  en lugar de usar el teclado virtual en pantalla. Esto cubre el nombre de archivo, el título
  y el autor de Project, el nombre del instrumento, el nombre del tema de color,
  el nombre de la tabla de afinación, el nombre de archivo del bounce y los cuadros de diálogo de nombre (introducir
  nombre, crear carpeta). Backspace funciona como en un campo de texto normal; Shift
  escribe mayúsculas. Esc lo alterna en todas estas pantallas a la vez,
  y el teclado virtual sigue abriéndose con normalidad cuando el key jazz está desactivado.

## 5. Instruments

Todos los instrumentos tienen algunos parámetros comunes:

- Tipo de instrumento
- Nombre, de hasta 15 caracteres
- Velocidad de tabla por defecto, en ticks por fila de tabla
- Transposición activada o desactivada

Cada instrumento tiene una tabla por defecto con el mismo número en el rango `00-7F`. Puedes usarlas como tablas auxiliares, pero mantener las tablas auxiliares en el rango `80-FE` evita conflictos y confusión. Las tablas son una de las principales herramientas de diseño sonoro de ChooChooTracker.

### Controles

- **OPT + [LEFT/RIGHT]**: moverse entre instrumentos
- **EDIT + PLAY**: escuchar el instrumento
- **SHIFT + OPT**: copiar el instrumento
- **SHIFT + EDIT**: pegar el instrumento

### AY Classic, AY Plus y AY Sample

Estos son los motores AY/YM originales de ChipNomad. AY Classic expone controles de tono, ruido y envolvente al estilo del hardware. AY Plus añade osciladores por software y una modulación más rica. AY Sample reproduce una muestra a través de los niveles de volumen de tipo AY y es distinto del motor Sampler.

#### AY Classic

Es un tipo de instrumento heredado de ChipNomad `v1.0.0`. El instrumento AY Plus, más reciente, ofrece más funciones.

Los instrumentos AY Classic tienen estos parámetros:

- Mixer: tono activado o no, ruido activado o no, forma de la envolvente (`0-F`)
- Volume: envolvente ADSR generada por software
- Periodo de envolvente automático: activado o no, con una proporción de `1:1` a `F:F`

**AY Quality** en Settings solo afecta al renderizado de AY/YM. No cambia la calidad de Braids, Plaits ni Sampler. **Sample dithering** se aplica a la cuantización de AY Sample.

Consulta la [documentación de ChipNomad sobre el AY-3-8910](https://chipnomad.org/chips/ay-3-8910/) para los detalles del chip.

#### AY Plus

AY Plus aplica conceptos clásicos de síntesis al diseño sonoro del AY-3-8910/YM2149F. Puedes pensar en el AY como un sintetizador de 5 osciladores:

- 3 osciladores de tono de onda cuadrada
- 1 oscilador de ruido
- 1 generador de envolvente de amplitud que puede usarse como oscilador

Según este modelo, cada instrumento tiene 3 osciladores de hardware: tono, ruido y envolvente. AY Plus añade un 4.º oscilador, de software. Actualizar los registros del AY más rápido que la frecuencia de ticks del proyecto abre más posibilidades de diseño sonoro. Por la forma en que funciona el AY, los osciladores se mezclan mediante modulación en anillo. Prueba distintos desplazamientos de altura, desafinaciones y combinaciones de osciladores para ver qué sale.

Cada oscilador tonal puede afinarse de forma independiente con desplazamientos de altura y de afinación fina.

Tipos de osciladores de software y sus parámetros:

- Pulse - onda cuadrada con ancho de pulso controlable
  - FM Depth - profundidad de FM simple
  - Pulse width - el rango completo es `00-FF`, mientras que el número de pasos se fija en la pantalla Project
  - Pulse low - controla el valor bajo de un par repetido cuyo valor alto es siempre `15`
- Sync Tone - efecto de sincronización forzada (hard sync) para el oscilador de tono
  - FM Depth - profundidad de FM simple
- Sync Env - efecto de sincronización forzada para el oscilador de envolvente
  - FM Depth - profundidad de FM simple
  - Pulse width - proporción de duración entre las 2 formas del par de envolventes
  - Envelope Pair - 2 formas de envolvente entre las que alternar
- Wavetable - tablas de ondas de 32 pasos, editadas en la pantalla Wavetable
  - FM Depth - profundidad de FM simple
  - Wavetable index
- Tone FM - efecto de tipo FM simple para el oscilador de tono
  - FM Depth - profundidad de FM del tono
- Env FM - efecto de tipo FM simple para el oscilador de envolvente
  - FM Depth - profundidad de FM de la envolvente

Los osciladores de software Pulse y Wavetable no pueden usarse con el oscilador Envelope. El oscilador Envelope se desactiva automáticamente.

La FM simple alterna rápidamente entre 2 valores de periodo, 1 más bajo y 1 más alto. Es parecido a usar una onda cuadrada como modulador de FM. Pulse, Sync Tone, Sync Env y Wavetable usan una proporción `1:1` entre la portadora y el modulador de FM. Tone FM y Env FM pueden imitar otras proporciones mediante los desplazamientos de altura del oscilador de software.

#### AY Sample

Puedes cargar archivos WAV mono o estéreo de `8 bits`, `16 bits`, `24 bits` o `32 bits`. Se convierten a `8 bits` y se truncan a `16 KB`. Este límite coincide con el tamaño de una página de RAM del ZX Spectrum.

El AY reproduce muestras mediante escrituras rápidas en el registro de volumen. Su salida es unipolar, así que las muestras deben «subirse hasta cero» para una reproducción más limpia. Esto elimina los clics al final de la muestra y mejora las colas silenciosas.

El AY tiene un DAC de resolución de `4 bits` con una escala de volumen no lineal, de modo que las muestras de `8 bits` se convierten a `4 bits` durante la reproducción. La opción Sample dithering de Settings puede mejorar la profundidad de bits percibida. El dithering no es práctico en plataformas retro como el ZX Spectrum y el Atari ST, así que déjalo desactivado cuando escribas para hardware real o busques un sonido lo-fi crujiente.

Las muestras se comportan como otro oscilador de software de AY Plus, pero sus parámetros adicionales hacen que sea más fácil manejarlas como un tipo de instrumento aparte.

Parámetros de muestra:

- Sample rate - reproducir una muestra a su frecuencia nativa produce un `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

Las secciones de osciladores Tone y Noise coinciden con las de AY Plus. Por la forma en que funciona el AY, todos los osciladores se mezclan mediante modulación en anillo.

### Braids

Braids ofrece 47 modelos de síntesis. Sus controles principales son:

- **Model**: oscilador o algoritmo de síntesis
- **Timbre** y **Color**: parámetros macro que dependen del modelo
- **Filter**: filtro adicional LP/BP/HP, pendiente de `12` o `24 dB`, frecuencia de corte y resonancia
- **ADSR**: attack, decay, sustain y release

Toca **Model** para elegir entre listas de modelos por categorías.

| Categoría | Modelos |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Parámetros

Timbre y Color dependen del modelo seleccionado. Consulta el manual original de Braids, de Mutable Instruments, para la lista completa de modelos y sus detalles, o simplemente toca de oído.

#### VCF y VCA

Braids alimenta el filtro multimodo y la envolvente ADSR estándar de ChooChooTracker.

#### Ajustes

Los ajustes globales de Braids están en los ajustes de la aplicación:

- **BITS**: reduce la resolución de salida
- **DRFT**: añade inestabilidad de altura a los osciladores
- **SIGN**: reproduce el algoritmo de imperfección de forma de onda del Braids original

### aChChid

**aChChid** es un motor de bajo acid monofónico basado en Open303. `Square` y `Saw` usan el oscilador, el filtro, la envolvente y el comportamiento de acento nativos del TB-303. `Braids` solo sustituye el oscilador y después continúa por el mismo camino de filtro y amplificador del 303. Expone Model, Timbre, Color y Shaper en lugar de Fine tune. Shaper pasa progresivamente de una saturación suave al plegado de onda (wavefolding). aChChid no usa el postfiltro unificado ni el ADSR de ChooChooTracker.

Cutoff y Decay se editan de forma logarítmica: **EDIT + LEFT/RIGHT** avanza un
paso de control musical, mientras que **EDIT + UP/DOWN** avanza dieciséis pasos. Cutoff
cubre de 200 Hz a 20 kHz; Decay cubre de 200 ms a 2 s.

Una `F` en la columna de volumen de la nota dispara un acento. Las notas normales usan un gate de medio paso (3 ticks en el groove por defecto de 6 ticks). `ASL` desliza hacia esa nota desde la altura anterior sin redisparar la envolvente del 303; mantiene automáticamente el gate anterior, y `ASL 00` da un glissando de `60 ms`. `ATY` mantiene su nota abierta durante todo el paso actual. Las notas sin `ASL` siempre se redisparan. Los destinos de modulación incluyen Decay y Accent, además de Timbre y Color en el modo de onda Braids.

### Bogie

**Bogie** es un sintetizador de batería nativo de un solo disparo (one-shot). Las notas del tracker fijan la altura y el volumen del tracker/instrumento fija el nivel; cada disparo tiene su propio decaimiento interno, por lo que no tiene página de ADSR. Tiene 12 modelos: `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` y `Clave`.

Sus seis macros son **Decay**, **Tone**, **Sweep**, **Noise**, **FM** y **Drive**. Sus etiquetas se adaptan donde es útil (por ejemplo, el Noise de Kick es `Click`, y el FM de Cowbell es una modulación cruzada metálica). Todas las macros están activas en todos los modelos, pero su papel musical cambia con el modelo: Sweep puede alterar la altura, el espaciado de ráfagas o la dispersión metálica; FM puede añadir modulación cruzada, una capa metálica o un armónico de tipo anillo. La mitad inferior de cada rango está pensada para sonidos de batería convencionales; la mitad superior abre deliberadamente metamorfosis tímbricas más amplias y resultados más sintéticos.

Cada modelo puede usar el filtro multimodo LP/BP/HP compartido, con carácter Clean, Classic, Aggro o Acid, pendiente de `12`/`24 dB`, frecuencia de corte y resonancia. Bogie admite todas sus macros activas más Volume, Pitch, Cutoff y Resonance como destinos de modulación y de grabación de movimiento. Usa un Decay corto con Cowbell para ataques phonk afilados; sube FM para una campana más dura y metálica.

En la pantalla Modulation, pulsa **EDIT** sobre una fuente para elegir una familia (Envelopes, LFO o Sticks) y luego la fuente. **Left** y **Right** siguen recorriendo las fuentes directamente.

### Sintered

**Sintered** es un sintetizador de percusión digital nativo de un solo disparo. Las notas fijan su altura fundamental; sus seis modelos son `Knot`, `Shard`, `Burst`, `Comb`, `Logic` y `Melt`. Todos los modelos tienen **Decay**, **Mod** y **Motion**, además de tres controles propios del modelo: Knot usa Ratio/Spread/Fold, Shard Ratio/Feedback/Bite, Burst Noise/Color/Feedback, Comb Time/Damping/Regen, Logic Rate/Pattern/Crush y Melt Ratio/Chaos/Drive. **Mod** es siempre la profundidad de acoplamiento interna.

Motion es una envolvente macro interna, no un glissando de altura: los valores por debajo de `80` usan una forma AD, `80` la desactiva, y los valores por encima de `80` usan una forma de solo decaimiento. Alejarse del centro hace la envolvente más rápida. Sus destinos están fijados por modelo, de modo que Sintered sigue siendo predecible en los patrones del tracker: Knot anima Mod/Fold; Shard Feedback/Bite; Burst Noise/Color/Feedback; Comb Damping/Regen; Logic Rate/Crush; Melt Chaos/Drive. Todos los modelos reinician su estado al disparar una nota y acotan la realimentación interna para obtener golpes repetibles.

### MME

**MME** (Multi Modulation Engine) es un sintetizador de dos osciladores dedicado a las interacciones entre osciladores. Sus modelos son `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` y `Vocode`. Ambos osciladores son internos y siguen las notas del tracker; `Interval` fija su relación.

Las seis macros son **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** y **Shaper**. Waves selecciona pares musicales de formas de onda A/B. Amount controla la interacción central del modelo, Flow cambia su dirección o la variante de su algoritmo, y Feedback está acotado pero se vuelve deliberadamente salvaje al final de su rango. Shaper es una etapa final compartida que pasa de limpio a saturación y luego a wavefolding.

MME usa el filtro multimodo LP/BP/HP compartido y el ADSR. Las seis macros, Cutoff y Resonance están disponibles para la modulación y la grabación de movimiento. Mantén **EDIT** sobre Model para abrir la ventana de modelos.

### MIDI Out

**MIDI Out** (solo en ordenador) controla un dispositivo MIDI externo en lugar de sintetizar audio: disparar una nota envía un Note On MIDI real en el **Channel** del instrumento (`1-16`) y, cuando la nota termina, el Note Off correspondiente. El volumen se convierte en velocidad, y cada voz de una pista de acordes recibe sus propios Note On/Off. ChooChooTracker no produce ningún sonido por sí mismo: selecciona antes el dispositivo de salida en [Settings](#12-settings). Consulta [MIDI](#14-midi) para el panorama completo, incluida la escucha de sonidos desde un teclado MIDI.

**Program** y **Bank high/low** (CC0/CC32) son opcionales - se muestran como `--` cuando están desactivados, `Clear` desactiva un campo recordando su valor. Cuando están definidos, se envían una vez, justo antes de la siguiente nota, siempre que difieran de lo último que se le dijo a ese canal (no antes de cada nota, lo que volvería a disparar las envolventes o el patch propios del dispositivo receptor).

**MC1 number**-**MC4 number** eligen a qué número de CC (`00-7F`) envía cada uno de los cuatro FX de fila MC1-MC4 - véase [MIDI Out FX](#fx-de-midi-out). También `--`/opcional; una ranura sin definir deja su FX inerte.

### Motores sustractivos

Los motores de esta categoría comparten una arquitectura VCO a VCF a VCA.

Hay varios tipos de VCO disponibles:

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Cada instrumento envía su VCO a un filtro multimodo.

El filtro tiene una pendiente conmutable de `12 dB` o `24 dB` y puede funcionar como paso bajo (LP), paso banda (BP) o paso alto (HP).

Hay varios caracteres de filtro disponibles:

- **Clean**: estéril y digital
- **Classic**: inspirado en los sintetizadores analógicos americanos clásicos
- **Aggro**: inspirado en los rugientes sintetizadores analógicos japoneses
- **Acid**: inspirado en los psicodélicos

Classic, Aggro y Acid pueden resonar muchísimo en esta versión alfa. Empieza bajo y súbelos con suavidad.

El filtro alimenta un VCA controlado por una envolvente ADSR. La forma de la envolvente puede transformarse entre curvas exponencial, lineal y logarítmica.

#### Plaits y Plaits-Alt

**Plaits** ofrece 24 motores, agrupados en la ventana de selección de este modo:

| Categoría | Motores |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** es una colección de modelos alternativos para el módulo Plaits original. La mayoría son más extraños o más experimentales que el VCO medio.

| Categoría | Motores |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

Los parámetros siguen el diseño de Mutable:

- **Harmonic** controla las relaciones armónicas, el equilibrio o la elección de modelo dentro de un motor.
- **Timbre** suele ir de espectros oscuros/dispersos a brillantes/densos.
- **Morph** explora otra dimensión del timbre.
- **Main/Aux** mezcla la salida principal con la salida alternativa del motor.

Su significado exacto depende del motor. Por ejemplo, los motores de acordes los usan para el tipo de acorde, la inversión y la forma de onda. Los modelos físicos los usan para el material, la excitación y el decaimiento, mientras que los motores de batería los usan para el tono, el carácter y el decaimiento. Consulta el manual de usuario de Plaits para los detalles.

Para `CHORD`, `DIATONIC CHORD` y `WT DIATONIC CHORD`, Harmonic muestra el nombre del acorde seleccionado (`MIN`, `MAJ`, `MIN7`, `MAJ7`, etc.) en lugar de su valor de control en bruto. El `WAVE PARA` de Braids nombra del mismo modo sus voicings de Color.

Toca **Engine** para elegir entre listas de motores por categorías.

**Env Mode** tiene 2 enrutamientos:

- `TRIG` reproduce el módulo con TRIG conectado y LEVEL sin conectar
- `VCA` mantiene LEVEL abierto y aplica el ADSR del tracker después de la voz

#### Sampler

Este motor de muestras limpio reproduce muestras PCM mono o estéreo cargadas en RAM.

- Toca **Sample** para cargar un WAV PCM sin comprimir de `8 bits`, `16 bits` o `24 bits`. Pulsa **PLAY** en el explorador para escuchar el archivo resaltado.
- Cuando hay una muestra cargada, aparece **EDIT** junto a su nombre. Tócalo para abrir la pantalla Sample Edit. **OPT** o **SELECT + [LEFT]** vuelve al instrumento.
- La pantalla Sample Edit muestra el nombre de archivo (con un marcador `*` cuando la muestra en RAM difiere del archivo en disco), un indicador de formato (frecuencia de muestreo, canales), una forma de onda alta y los campos **Region**, **Select**, **Slice**, **Process** y **File**.
- Las ediciones solo viven en RAM hasta que un flujo de guardado las escribe: el proyecto guarda la ruta del archivo de la muestra y vuelve a cargar el WAV desde disco la próxima vez que se cargue el proyecto, de modo que salir del tracker sin **Save**/**Save As** descarta las operaciones de proceso (el marcador `*` es la advertencia). Cargar otra muestra en el instrumento también descarta las ediciones no guardadas y restablece Region a la muestra completa (Start `00`, End `FF`).
- **Region** fija los marcadores de reproducción Start/End, también disponibles en la pantalla del instrumento Sampler. Establecen límites de reproducción normalizados (`00-FF`); si Start está después de End, la muestra se reproduce al revés. **EDIT + [LEFT/RIGHT]** ajusta finamente el marcador de uno en uno (`01`) y hace zoom en la forma de onda a su alrededor mientras se mantiene **EDIT**; **EDIT + [UP/DOWN]** ajusta de forma gruesa (paso de 16) y devuelve la vista a la muestra completa. **EDIT + OPT** restablece el marcador a su valor por defecto (Start `00`, End `FF`).
- La vista de la forma de onda hace zoom hasta una ventana fija de un segundo de audio alrededor del marcador editado y se desplaza para mantenerlo visible; las muestras que caben dentro de la ventana permanecen en la vista completa 1:1. El zoom es transitorio: dura mientras se mantiene **EDIT**, y soltar **EDIT** devuelve a la vista completa 1:1, igual que los pasos gruesos. Entrar en la pantalla siempre restablece la vista a la muestra completa.
- **Select** fija una selección de proceso en fotogramas absolutos, independiente de los marcadores de reproducción Start/End. **EDIT + [LEFT/RIGHT]** mueve un tirador veinte fotogramas y hace zoom sobre él mientras se mantiene **EDIT**; **EDIT + [UP/DOWN]** salta `frameCount/64` (mínimo 16) y devuelve a la vista de muestra completa. **EDIT (toque)** sobre un tirador le copia la posición actual del marcador Start o End. **EDIT + OPT** sobre cualquiera de los dos tiradores vacía toda la selección. Cuando la selección está vacía, ambos tiradores muestran `-`; cuando los tiradores están invertidos, se intercambian automáticamente. La selección es estado del editor solo de la sesión: no se guarda con el proyecto, y al entrar en la pantalla se inicializa con el tramo de la Region de reproducción (la muestra completa con los marcadores por defecto).
- **Slice** es una sola fila con cuatro celdas de valor - **Mode**, **Count**,
  **Slice** y **Frame** - y se guarda con el instrumento. Off reproduce la
  ventana Start/End. Las notas de la frase seleccionan slices de forma cromática desde **C-0**, y
  las notas más allá del último slice vuelven al principio (la nota N reproduce el slice
  `N % count`, de modo que una serie que pasa del último slice vuelve al primero).
  Las notas cortadas no transponen la altura ni usan la cuantización Scale, pero **CRD**
  conserva el slice seleccionado y transpone sus voces como un acorde. Los inicios de slice
  se marcan en la forma de onda con líneas cortas de dos píxeles de ancho (naranja oscuro; la línea
  del slice actual es más brillante), y el slice bajo el cursor recibe una
  banda de fondo negra sobre la forma de onda (el tinte de selección no se dibuja sobre
  la banda, por lo que sigue siendo completamente negra y claramente distinta del resto de
  la vista previa).
  - **Mode** recorre `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + izquierda/derecha o
    toque; EDIT + OPT desactiva el corte). Cambiar de modo inicializa los puntos
    de slice: **EQUAL** divide la ventana Start/End en partes iguales según **Count**;
    **AUTO** ejecuta una detección de transitorios sobre la ventana y coloca un
    slice en cada ataque que encuentra (hasta **Count**); **LAZY** empieza con un
    slice que cubre toda la ventana. Pasar a LAZY guarda aparte el ajuste actual de
    EQUAL/AUTO (cantidad y límites), y volver a EQUAL o
    AUTO lo restaura tal cual - no se pide confirmación y no se
    pierde nada. Slice y Stretch son mutuamente excluyentes: activar uno desactiva
    el otro, y las celdas de Slice se atenúan mientras Stretch controla la
    duración.
  - **Count** muestra el número total de slices (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** lo cambia de uno en uno (mínimo 1, máximo 64), **EDIT +
    [UP/DOWN]** recorre las cantidades en potencias de dos `2 / 4 / 8 / 16 / 32 /
    64` con vuelta al principio. Cada cambio recalcula la división: **EQUAL**
    vuelve a dividir la ventana en partes iguales, **AUTO** vuelve a ejecutar la detección con el nuevo
    objetivo (la sensibilidad de detección se deriva de la cantidad - más slices
    significan un umbral más sensible). La casilla muestra `-` cuando el modo es
    OFF. En **LAZY** se mantiene brillante (cuenta los slices colocados a mano) pero
    es solo de visualización: el cursor la salta - ir a la derecha desde **Mode** o
    a la izquierda desde **Slice** aterriza en **Slice**, bajar desde el marcador **END**
    de la fila **Select** también aterriza ahí, y tocar la casilla no hace nada
    (los slices colocados a mano no tienen una división que recalcular). **EDIT + OPT** y
    un simple toque de **EDIT** no hacen nada aquí: desactivar el corte se hace
    en **Mode**, y borrar un slice se hace en **Slice**.
  - **Slice** es el explorador: muestra el slice actual, numerado desde 1
    (`01`..count). **EDIT + [LEFT/RIGHT]** pasa de un slice a otro de uno en
    uno, **EDIT + [UP/DOWN]** salta cuatro slices (ambos acotados, sin
    vuelta al principio); el slice explorado se indica en la forma de onda con su marcador
    más brillante y la banda negra, y la vista ampliada se recentra en el inicio del
    slice. **EDIT + OPT** borra el slice actual (se fusiona con
    el anterior; el primer slice no se puede borrar, y borrar el
    último slice restante desactiva el corte - los límites sobreviven a un viaje de ida y vuelta
    por OFF y regresan cuando se vuelve a elegir un modo). **EDIT (toque)**
    escucha el slice seleccionado en reproducción única (funciona en
    todos los modos de slice; la escucha se detiene al soltar la tecla o al
    salir de la pantalla). La casilla muestra `-` cuando el modo es OFF.
  - **Frame** muestra en hexadecimal el fotograma inicial del slice actual. **EDIT +
    [LEFT/RIGHT]** lo desplaza diez fotogramas (haciendo zoom sobre el marcador mientras
    se mantiene **EDIT**), **EDIT + [UP/DOWN]** lo desplaza cien y
    devuelve a la vista completa. Un marcador permanece dentro de su propio slice: no
    puede cruzar el inicio del slice anterior ni del siguiente. **EDIT + OPT** borra
    el slice.
  - **B** vuelve a la pantalla del instrumento desde la fila Slice, como en todas
    partes de esta pantalla; **SHIFT + LEFT** sale desde cualquier punto.
  - **Flujo de trabajo LAZY**: con LAZY seleccionado, toca **PLAY** para iniciar una
    reproducción única de la muestra completa - suena una vez desde el principio y se detiene
    sola, a la altura y velocidad originales de la muestra (un marcador verde de dos píxeles
    sigue la posición en la forma de onda); tocar **PLAY** otra vez
    la detiene antes. Mientras suena,
    cada clic de **EDIT** deja un slice en la posición de reproducción, esté donde esté
    el cursor (los slices a menos de 50 ms de uno existente se
    rechazan con `Too close to slice`). Volver a reproducir la muestra conserva
    los cortes existentes y añade otros nuevos en cada posición tocada, por
    orden - la casilla **Count** se actualiza con cada slice dejado y el
    explorador **Slice** sigue al nuevo. A los slices colocados a mano se les aplican las mismas reglas de
    exploración y edición que en los demás modos: **EDIT +
    dirección** sobre **Slice** los recorre, **Frame** ajusta el inicio
    seleccionado, **EDIT + OPT** borra el slice actual. La celda Frame se atenúa
    mientras está armado el depósito durante la reproducción. **SHIFT + PLAY** no se intercepta
    en LAZY: inicia la reproducción de la frase como en cualquier otra pantalla. Los slices
    LAZY suenan en el secuenciador igual que EQUAL y AUTO: las notas de la frase
    se asignan cromáticamente desde **C-0** a los slices colocados a mano,
    con vuelta al principio pasado el último.
  - **Pistas de combinaciones**: mientras el cursor descansa sobre una celda de la fila Slice y no se muestra otro
    mensaje, la barra de estado muestra la pista de esa celda y la mantiene
    mientras el cursor siga ahí. En **Mode** la pista está ligada al
    modo activo y muestra solo la descripción (el nombre del modo ya está en
    la celda): `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - cambiar de modo
    cambia el texto de inmediato, y OFF vacía la barra. En **Count** muestra
    `Adjust the number of slices`, en **Slice**
    `Browse slices`, en **Frame** `Adjust slice start`. Cualquier mensaje de acción
    sustituye a la pista hasta que expira.
- **Process** selecciona una operación de edición destructiva: **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** o **Reverse** (EDIT + izquierda/derecha recorre, el toque avanza, EDIT + OPT no elige ninguna). **GO** (en la misma fila) ejecuta la operación seleccionada sobre la selección actual, o sobre la muestra completa cuando la selección está vacía. **Crop** conserva solo la selección; **Normalize** escala la selección para que su pico alcance la escala completa (ambos canales comparten una ganancia para preservar la imagen estéreo); **Delete** elimina la selección y une los extremos (se rechaza borrar la muestra completa); **Silence** la pone a cero; **Fade In**/**Fade Out** suben o bajan la selección linealmente desde/hacia el silencio; **Reverse** reproduce la selección al revés (los fotogramas se intercambian en su sitio, ambos canales de un fotograma se mueven juntos, la longitud y los marcadores no cambian). Crop y Delete requieren una selección - con una selección vacía muestran `Select region first`. Cada operación pausa brevemente el audio, conserva un nivel de deshacer y marca la muestra como cambiada en RAM: el archivo en disco no se toca hasta los flujos de Save (véase más abajo), y salir de la pantalla descarta la ranura de deshacer.
- **UNDO** (junto a GO) devuelve la muestra al estado anterior a la última operación. Está atenuado hasta que se ejecuta una operación, alterna entre los estados previo y posterior a la operación con pulsaciones repetidas, y se vacía al volver a entrar en la pantalla.
- **File** contiene los flujos de guardado. El instrumento guarda la ruta completa del WAV desde el que se cargó; estos flujos escriben ese archivo o hacen que el instrumento apunte a uno nuevo. Nunca tocan el nombre del instrumento.
  - El campo File alterna entre **Save** y **Save As** (EDIT + izquierda/derecha o toque); **GO** (en la misma fila) ejecuta la acción mostrada.
  - **Save** sobrescribe el WAV desde el que se cargó la muestra. Para una muestra
    sin slices pregunta primero `Overwrite <name>?`. Una muestra cortada abre primero
    el diálogo **SAVE SLICES**, que pregunta a qué pertenecen los puntos de slice:
    **SAVE TO SAMPLE** los escribe en el WAV como bloques cue (portátil -
    los DAW los muestran como marcadores) y también los conserva en el proyecto,
    **SAVE TO PROJECT** escribe un WAV simple y conserva los puntos
    solo en el proyecto, **CANCEL** aborta. Una casilla `Don't ask again in this project`
    guarda el destino elegido con el proyecto y omite
    el diálogo a partir de entonces (la elección se guarda como `- Sample save choice:` en
    el .cct y solo se puede cambiar editando el archivo del proyecto). El
    diálogo se omite para las muestras sin slices. **Save** está atenuado (y
    se salta en la navegación) hasta que se carga una muestra con ruta de archivo. Una
    escritura fallida conserva el marcador de modificación y muestra el error.
  - **Save As** escribe la muestra actual en un archivo nuevo: introduce un nombre de archivo (rellenado con el nombre actual sin extensión) y elige una carpeta en el explorador. La muestra se escribe como `<folder>/<name>.wav`, el instrumento apunta al archivo nuevo y la carpeta se recuerda como carpeta de muestras por defecto. Está atenuado (y se salta en la navegación) hasta que se cargan datos de muestra, de modo que a una muestra recién cargada se le pueda dar una ruta de archivo. Las rutas de más de 255 caracteres se rechazan.
  - El marcador `*` antes del nombre del archivo significa que la muestra en RAM difiere del archivo en disco (cualquier operación de proceso lo activa; Save y Save As lo borran). El marcador es solo de la sesión: no se guarda con el proyecto y se restablece al volver a entrar en la pantalla. Si varios instrumentos hacen referencia al mismo archivo WAV, guardar uno sobrescribe el archivo para todos.
- En la pantalla del instrumento Sample, usa **EDIT + [LEFT/RIGHT]** para cargar el WAV anterior o siguiente de la misma carpeta.
- **Pitch** transpone por semitonos (`-48` a `+48`).
- **Loop** elige Off, Loop o Ping-Pong.
- **Speed** controla el estiramiento temporal granular de `0%` a `500%` (`100%` es normal).

Los formatos WAV no admitidos muestran un error. Convierte los archivos inusuales a WAV `PCM8`, `PCM16` o `PCM24` antes de importarlos.

#### 2xSCWF

2xSCWF es un oscilador doble de forma de onda de ciclo único. Carga en cada oscilador 1 WAV mono que contenga exactamente 1 periodo. Ambos osciladores leen hacia delante y vuelven al principio al final de su ciclo.

Mix hace un fundido cruzado entre A y B.

Detune es fino y exponencial desde el unísono hasta `+200 ct`, y después avanza por semitonos de `+3 st` a `+24 st`.

2xSCWF trata cada archivo como una tabla de 1 ciclo y lo lee exactamente una vez por ciclo de oscilador. Esto permite mezclar y desafinar 2 ondas de ciclo único.

La carpeta de fábrica `waveforms/AKWF/` contiene formas de onda de la biblioteca Adventure Kid. Hay muchas más disponibles en línea si quieres ampliar la colección.

#### BYOWTBL

BYOWTBL es un oscilador doble de tabla de ondas compatible con las tablas de Serum. Cada oscilador carga un WAV mono que contiene fotogramas de ciclo único consecutivos. `Pos A` y `Pos B` recorren sus tablas de forma independiente de `00` a `FF`. El motor interpola linealmente dentro de cada fotograma y entre fotogramas adyacentes. `Mix` hace un fundido cruzado entre los osciladores, y `Detune` desplaza el oscilador B.

## 6. Modulación y grabación de movimiento

Cada instrumento puede tener hasta 4 ranuras de modulación. Los instrumentos de audio pueden apuntar a **Instrument Pan** o **Track Pan**; ambos usan `00` para la izquierda, `80` para el centro y `FF` para la derecha.

Los tipos de modulación disponibles son:

- ADSR - envolvente clásica attack-decay-sustain-release
- AHD - envolvente attack-hold-decay
- LFO - oscilador de baja frecuencia con varias formas
- SLFO - LFO con un multiplicador de periodo para ciclos lentos
- FLFO - LFO de frecuencia de audio que puede llegar al rango de los kHz; el FLFO puede exigir mucho a la CPU
- SLIN - control lineal por joystick
- SRAT - control por joystick basado en la velocidad de cambio

La lista de destinos de modulación depende del tipo de instrumento.

Para un destino `Cutoff`, Amount es exponencial con una respuesta cuadrática: el 25% alcanza 4,5 semitonos, el 50% alcanza 18 semitonos, el 75% alcanza 40,5 semitonos y el 100% alcanza seis octavas. Esto mantiene un control fino con cantidades bajas y aun así permite barridos profundos.

Cuando una envolvente ADSR o AHD apunta a Volume, se convierte en la envolvente de volumen del instrumento. Un LFO que apunta a Volume, en cambio, desplaza el volumen de salida actual.

La cantidad de modulación puede ser positiva o negativa. Su rango depende del destino. Si el rango completo del destino es de `127` o menos, como en la mayoría de los parámetros, la cantidad define un rango absoluto. Los destinos más amplios, como la altura, usan una cantidad escalada.

Todos los parámetros de duración, incluidos attack, hold, decay y periodo, se miden en ticks.

Formas del LFO:

- Tri - onda triangular bipolar
- Sin - onda sinusoidal bipolar
- UniTri - onda triangular unipolar
- UniSin - onda sinusoidal unipolar
- RampDn - rampa lineal descendente (onda de diente de sierra)
- RampUp - rampa lineal ascendente (onda de diente de sierra)
- ExpDn - rampa exponencial descendente (diente de sierra exponencial)
- ExpUp - rampa exponencial ascendente (diente de sierra exponencial)
- Square - onda cuadrada
- Random - onda aleatoria de muestreo y retención (sample-and-hold)
- Wavetable - lee una de las tablas de ondas AY de 32 pasos del proyecto. Los valores `00` a `0F` se asignan de `-100%` a `+100%`, sin interpolación.

Tipos de disparo del LFO:

- Free - reinicia el LFO solo cuando cambia el instrumento
- Retrig - reinicia el LFO en cada nota
- Phrase - reinicia LFO y SLFO cuando la reproducción entra en una frase nueva
- Chain - reinicia LFO y SLFO cuando la reproducción entra en una cadena nueva
- Hold - se detiene tras 1 ciclo y mantiene el último valor
- Once - se detiene tras 1 ciclo y vuelve a cero

### Grabación de movimiento (Motion recording)

- **STICK LIVE** (`L1` por defecto) aplica la modulación de los sticks sin modificar la frase. En **Settings > Stick live mode**, `HOLD` (el valor por defecto) la activa mientras se mantiene pulsado; `TOGGLE` la activa con una pulsación y la desactiva con la siguiente; `FREE` la deja activada de forma permanente. Soltar el botón en `TOGGLE` la deja activada.
- Durante la reproducción, mantén **MOTION RECORD** (`L2`) para aplicar la modulación de los sticks y grabar los destinos modificados como valores de FX absolutos.
- Mantén **MOTION ERASE** (`R2`) para eliminar de la fila actual los FX de destino coincidentes.

La `~` de la esquina inferior derecha indica que Stick live está activo. Record (`*`, o `!` en caso de desbordamiento) y Erase (`x`) tienen prioridad de indicador y siguen siendo momentáneos en ambos modos. Los dos activan el procesamiento de sticks en directo mientras se mantienen pulsados; soltarlos deja intacto un conmutador live activado por separado.

El modo se guarda con los ajustes de la aplicación, pero el conmutador live siempre empieza desactivado al iniciar. Cambiar de modo borra el conmutador: `HOLD` sigue cualquier botón de Stick live que se esté manteniendo, `TOGGLE` espera una pulsación nueva y `FREE` está activo de inmediato.

La grabación de movimiento escribe FX de pista en la frase que se está reproduciendo. Primero actualiza los FX coincidentes y luego usa las ranuras vacías de derecha a izquierda. Nunca sobrescribe un FX distinto. Si las 3 ranuras están llenas, ese movimiento no se graba en el paso. Un `!` en la esquina inferior derecha significa que han cambiado más destinos de los que caben en las 3 columnas de FX.

La grabación de movimiento admite los destinos de Braids, Plaits, Sampler, 2xSCWF, BYOWTBL y Bogie, incluidas las macros de Bogie activas y los controles de filtro cuando corresponda.

## 7. Tables

Las tablas son una herramienta central de diseño sonoro en los trackers. En términos de Vortex Tracker, combinan instrumentos y ornamentos, pero pueden hacer mucho más. Si conoces LSDj o NerdSEQ, la idea ya debería resultarte familiar.

La columna Pitch acepta valores de altura relativos (`~`) o absolutos (`=`) en semitonos. La columna Volume va de `00` a `0F`, se aplica a todos los motores por encima de su envolvente y puede usarse como gate: alterna `0F` y `00` y luego haz un bucle con `HOP 00`. Las 4 vías de FX coinciden en su mayor parte con las vías de una frase, aunque unos pocos FX se comportan de forma distinta en las tablas.

Poner un FX `TIC` en la última fila de la tabla fija la velocidad de esa columna y anula la velocidad de tabla por defecto del instrumento. Cada columna de FX puede funcionar a una velocidad de ticks distinta. Las columnas Pitch y Volume siguen la velocidad de la 1.ª columna de FX.

Una tabla de 16 filas puede parecer corta al lado de un instrumento largo de Vortex Tracker, pero el FX `HOP` puede crear bucles condicionales como «repetir estas filas 5 veces», además de bucles anidados. Con bucles y velocidades de columna de FX independientes, esas 16 filas pueden llegar sorprendentemente lejos.

### Modo Retrigger

El campo `Retrig` de la parte superior de la pantalla Table elige cuándo se reinicia una tabla que ya está activa. `Inst` es el modo por defecto y compatible: una fila de frase que contiene un instrumento selecciona la tabla por defecto de ese instrumento y la inicia en la fila `00`. `Phrase` reinicia una tabla activa cuando la reproducción entra en una frase, `Chain` cuando entra en una cadena, y `Free` la deja funcionando hasta que la pista se detiene o un comando explícito la sustituye.

Un modo estructural nunca retrocede en el tiempo. Si una pista no tiene tabla de instrumento activa, la primera fila de frase que introduce un instrumento activa su tabla exactamente en esa fila, aunque sea la fila `4`, `8` o `F` de una frase. Los números de instrumento posteriores no sustituyen una tabla `Phrase`, `Chain` o `Free` activa. Esto hace que las tablas sean útiles como automatización de tipo LFO sin simular en silencio pasos de frase anteriores.

`TBL`, `TBX` y `RET` son siempre acciones explícitas: seleccionan o reinician de inmediato, sea cual sea el modo. En un límite de Phrase o de Chain, su reinicio ocurre antes de leer la nueva fila de frase, de modo que un `TBL` o `TBX` en esa fila tiene la última palabra. Por tanto, los FX de la fila `00` de la tabla se ejecutan de nuevo con normalidad; los FX acumulativos conservan su acumulación habitual.

Las tablas `00-7F` están reservadas para las tablas de instrumento por defecto. Las tablas `80-FE` están pensadas para tablas auxiliares iniciadas por el efecto `TBX`. El rango bajo también sirve para tablas auxiliares, pero usarlo así puede causar conflictos y confusión inesperados.

### Copiar FX entre Phrase y Table

Selecciona solo columnas de nombre/valor de FX, luego cópialas o córtalas desde una Phrase y pégalas en cualquier vía de FX de una Table, o al revés. La copia conserva exactamente las celdas seleccionadas, por lo que una selección solo de comandos o solo de valores sigue siendo parcial. El pegado empieza en la celda de FX actual y se recorta en la fila `F` y en la última vía de FX del destino. Las notas, los números de instrumento, la altura y el volumen nunca pasan entre estas dos pantallas.

### Controles

- **EDIT + [UP/DOWN]** sobre una columna de nombre de FX: abrir la pantalla de selección de FX
- **OPT + [DIRECTION]**: moverse entre tablas

## 8. Otras pantallas

### Instrument Pool

El Instrument Pool muestra todos los instrumentos del proyecto y permite reordenarlos. Durante la reproducción, también muestra qué instrumentos están activos en ese momento.

#### Controles

- **EDIT**: editar el instrumento y saltar a la pantalla Instrument
- **SHIFT + OPT**: copiar el instrumento
- **SHIFT + EDIT**: pegar el instrumento
- **EDIT + [UP/DOWN]**: reordenar instrumentos
- **EDIT + PLAY**: escuchar el instrumento

### AY Wavetable

Wavetable es un tipo de oscilador de software de AY Plus. Un proyecto puede contener hasta 256 tablas de ondas, cada una de 32 pasos. Todos los instrumentos de tabla de ondas comparten el mismo conjunto de ondas. En esta pantalla se editan.

Los niveles de volumen del AY no son lineales, así que la forma de onda de la vista Standard se dibuja para coincidir con los niveles de salida reales. Toca **OPT** dos veces para alternar entre la vista Standard y la **vista LFO** persistente, con el mismo atajo que en Song/Live Mode: la vista LFO dibuja linealmente los pasos en bruto de `0` a `15`, con la línea central entre `7` y `8` como el cero de la modulación.

La pantalla tiene 2 filas lógicas. La fila superior contiene los botones **Load** y **Save**, mientras que la 2.ª contiene el editor de la tabla de ondas.

#### Controles

- **OPT + [LEFT/RIGHT]**: recorrer la lista de tablas de ondas
- **OPT + [UP/DOWN]**: recorrer la lista de 16 en 16 ondas
- **SHIFT + OPT**: copiar la tabla de ondas
- **SHIFT + EDIT**: pegar la tabla de ondas
- **OPT + PLAY**: alternar entre las vistas Standard y LFO

## 9. FX de pista (Tracker FX)

Los FX de pista fuerzan valores del secuenciador o del motor de sonido en pasos concretos. Otras grooveboxes suelen llamarlos «parameter locks». Cada fila de Phrase tiene 3 ranuras de FX.

Cada FX tiene un comando de 3 letras y un valor hexadecimal. El panel de ayuda integrado en la aplicación da una breve descripción del comando seleccionado.

### Automatización de la escala (Scale)

`SCL XY` cambia la escala y la fundamental globales de reproducción sin cambiar las notas guardadas en las frases. `X` selecciona una escala (`0` Chromatic, `1` Major, `2` Minor, hasta `C` Custom); `Y` selecciona la fundamental (`0` C hasta `B` B). El comando solo se aplica a los FX de frase, no a las tablas. Si varias pistas emiten `SCL` en el mismo tick, gana la pista de número más bajo.

`SCL` pertenece al modo Quantizer. Con **Note Lock** activo, el comando no se ofrece en la lista de FX, y cualquier `SCL` ya escrito en frases se ignora mientras ese modo esté activado.

### Acordes (Chords)

`CRD XY` convierte la nota de su propia fila de Phrase en un acorde. `Y` selecciona: `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9 o `F` Min9. `X` selecciona uno de 16 voicings: `0-3` son un ciclo ascendente de inversiones cerradas; `4-7` añaden drop-2; `8-B` añaden drop-3; y `C-F` combinan ambos drops. El ciclo continúa por una octava adicional en las tríadas y los power chords, de modo que cada valor es distinto; los valores mayores se vuelven progresivamente más abiertos y menos convencionales. Por ejemplo, `CRD 4Y` es posición fundamental en drop-2 y `CRD DY` es primera inversión con ambos drops. En el extremo inferior o superior de la tabla de alturas, las voces se acotan a su rango. El comando no es persistente: escríbelo en cada fila con acorde. Cada nota generada se cuantiza de forma independiente cuando Scale está activo, tanto en el modo Quantizer como en Note Lock. Solo está disponible para los motores de software; los instrumentos AY lo ignoran y siguen siendo monofónicos.

### FX del secuenciador

| FX | Valor | Comportamiento detallado |
|---|---|---|
| `ARP` | `XY` | Arpegia la nota base, los pasos `+X` y los pasos `+Y`. `37` produce un patrón de acorde menor. |
| `ARC` | `XY` | `X` elige la dirección del arpegio o el modo de rango; `Y` es su velocidad en ticks. |
| `PVB` | `XY` | Vibrato de altura: `X` es la velocidad e `Y` la profundidad. En modo Linear, la profundidad usa pasos de `10 cents`. |
| `PBN` | `XX` con signo | Suma `XX` unidades de altura en cada fila de frase/tabla; `FF` significa `-1`. Usa `00` para detener. |
| `PSL` | `XX` ticks | Desliza desde la altura anterior hasta la nueva nota en `XX` ticks. |
| `PIT` | `XX` con signo | Desplazamiento relativo acumulado, en pasos de la tabla de alturas. |
| `FIN` | `XX` con signo | Desplazamiento fino acumulado, en cents con altura Linear y en unidades de periodo en caso contrario. |
| `PRD` | `XX` con signo | Desplazamiento relativo acumulado del periodo del oscilador. |
| `VOL` | `XX` con signo | Desplazamiento relativo acumulado del volumen (`FF` es -1, no volumen máximo). |
| `VSL` | `XX` con signo | Suma `XX` al volumen en cada fila de frase/tabla. Usa `00` para detener. |
| `PAN` | `00-FF` | Panorama absoluto del instrumento: izquierda, centro (`80`), derecha. |
| `TPN` | `00-FF` | Panorama absoluto de la pista del mixer: izquierda, centro (`80`), derecha. |
| `RET` | `XY` | Redispara cada `Y` ticks; `X` aplica un cambio de volumen. `Y=0` detiene el redisparo. |
| `DEL` | `XX` ticks | Retrasa el note-on. Un retraso mayor que el paso de groove actual omite la nota. |
| `OFF` | `XX` ticks | Envía el note-off tras `XX` ticks y entra en una fase de release del ADSR. |
| `KIL` | `XX` ticks | Corta bruscamente la voz tras `XX` ticks sin ejecutar el release del ADSR. |
| `TIC` | `XX` ticks | Fija los ticks de tabla por fila. En una tabla, cambia la velocidad de esa columna de FX. |
| `TBL` | `00-FE`, `FF` apagado | Sustituye y reinicia de inmediato la tabla del instrumento; `FF` la detiene. |
| `TBX` | `00-FE`, `FF` apagado | Inicia o sustituye de inmediato una tabla auxiliar; `FF` la detiene. |
| `THO` | fila `XX` | Salta todas las columnas de la tabla de instrumento a la fila `XX`. |
| `TXH` | fila `XX` | Salta todas las columnas de la tabla auxiliar a la fila `XX`; no se usa desde dentro de una tabla. |
| `GRV` | groove `XX` | Selecciona un groove para la pista actual. |
| `GGR` | groove `XX` | Selecciona un groove para todas las pistas. |
| `HOP` | `XY` | Salta a la fila `Y`, `X` veces; `X=0` hace bucle indefinido. El HOP de tabla afecta a su propia columna. |
| `SNG` | `XX` con signo | Mueve la reproducción `XX` filas de Song. |
| `PRO` | `00-64` | Evalúa una probabilidad de disparo absoluta de `0%` a `100%`. |
| `MOD` | `AB` | Se dispara en la pasada `A` de un ciclo de `B` pasadas. |
| `SPD` | `XX` con signo | Selecciona una relación de reloj persistente por pista; véase la tabla de velocidades más abajo. |
| `SLE` | `00-FF` ticks | Fija el tiempo de glissando persistente por pista para los FX continuos del motor. `00` es inmediato. El ajuste vuelve a `00` al detener la reproducción. |

### Envíos de efectos de audio (Audio FX sends)

| FX | Valor | Comportamiento detallado |
|---|---|---|
| `RSN` | `00-FF` | Fija el envío de reverb de esta pista hasta el siguiente disparo de nota. |
| `DSN` | `00-FF` | Fija el envío de delay de esta pista hasta el siguiente disparo de nota. |

### FX de ADSR / disparo

| FX | Valor | Comportamiento detallado |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Sustituyen el attack, decay, sustain, release o la forma de la envolvente. |
| `TDC` / `TCL` | `00-FF` | Sustituyen el Decay o el Color del modo Trigger en Plaits y Plaits-Alt. |

#### Condiciones

Inspirado en las «trig conditions» suecas, el tracker admite probabilidades de disparo y condiciones de módulo.

| FX | Valor | Significado |
|---|---|---|
| `PRO` | `00-64` | Probabilidad de disparo absoluta de `0%` a `100%` |
| `MOD` | `AB` | Se dispara en la iteración `A` de `B`; por ejemplo `12`, `22`, `14`, `34` |

Los contadores `MOD` son locales a la pista y a la frase. Las combinaciones no válidas, como `A` mayor que `B`, no se disparan.

`MOD34` se dispara en la 3.ª pasada de cada ciclo de 4 pasadas. `MOD1F` se dispara en la 1.ª pasada de cada ciclo de 16 pasadas.

#### Velocidad de reproducción

Como NerdSEQ, ChooChooTracker admite una velocidad de reproducción independiente para cada pista.

`SPD` lee su valor como un byte con signo. `00` es la velocidad normal, los valores positivos aceleran la pista y los negativos la ralentizan. El ajuste permanece activo hasta que otro comando `SPD` lo cambia.

| Valor | Velocidad |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

Los proyectos antiguos sin compatibilidad con `SPD` con signo conservan su asignación original `00-10`, así que suenan como se guardaron.

Los multiplicadores muy altos pueden superar la resolución del planificador de ticks del tracker y no se han probado en hardware.

### FX de modulación (Modulation FX)

Los FX de frase y de tabla pueden cambiar una ranura de modulación sin editar el instrumento:

| Patrón de FX | Significado |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Desplazamiento relativo de Amount para las ranuras de modulación 1-4 |
| `M11`, `M12`, `M13`, `M14` | Desplazamientos relativos P1-P4 para la ranura de modulación 1 |
| `M21`, `M22`, `M23`, `M24` | Desplazamientos relativos P1-P4 para la ranura de modulación 2 |
| `M31`, `M32`, `M33`, `M34` | Desplazamientos relativos P1-P4 para la ranura de modulación 3 |
| `M41`, `M42`, `M43`, `M44` | Desplazamientos relativos P1-P4 para la ranura de modulación 4 |

El valor se interpreta como un cambio relativo con signo de `8 bits` (`01` suma `1`, `FF` resta `1`). Los comandos repetidos se acumulan. Los valores efectivos se acotan a su rango válido.

`P5` no se expone como FX de frase ni de tabla. Está disponible como el destino de modulación `M1–M4 Wavetable`, de modo que un modulador puede recorrer el índice AYWavetable de otro.

| Tipo de modulación | P1 | P2 | P3 | P4 | P5 cuando Shape es Wavetable |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Sin uso |
| AHD | Attack | Hold | Decay | Sin uso | Sin uso |
| LFO | Shape | Trigger mode | Period | Sin uso | Índice de tabla de ondas AY |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | Índice de tabla de ondas AY |
| FLFO | Shape | Trigger mode | Frequency (`1 Hz` a `20 kHz`) | Sin uso | Índice de tabla de ondas AY |

### FX de Braids

| FX | Valor | Significado |
|---|---|---|
| `BMD` | `00-2E` | Modelo de Braids |
| `BTM` | `00-FF` | Timbre normalizado absoluto |
| `BCL` | `00-FF` | Color normalizado absoluto |
| `BCF` | `00-FF` | Frecuencia de corte exponencial, de `20 Hz` a `20 kHz` |
| `BRS` | `00-FF` | Resonancia del filtro exponencial |

### FX de aChChid

| FX | Valor | Significado |
|---|---|---|
| `ASL` | `00-FF` | Desliza hacia la nota sin redisparar. `00` son `60 ms`; los valores más altos alargan el glissando. |
| `ADC` | `00-FF` | Sustitución del decay: de `200 ms` a `2 s`. |
| `AAC` | `00-FF` | Cantidad de acento: de ninguna al máximo. |
| `ATM` | `00-FF` | Sustitución del Timbre de Braids; activa solo en el modo de onda Braids. |
| `ACL` | `00-FF` | Sustitución del Color de Braids; activa solo en el modo de onda Braids. |
| `ACF` | `00-FF` | Frecuencia de corte exponencial del filtro del 303, de `20 Hz` a `20 kHz`. |
| `ARS` | `00-FF` | Resonancia del filtro del 303, de ninguna al máximo. |
| `AEM` | `00-FF` | Modulación por envolvente del filtro del 303, de ninguna al máximo. |

### FX de Bogie

| FX | Valor | Significado |
|---|---|---|
| `DMD` | `00-0B` | Selecciona el modelo de Bogie hasta el siguiente disparo. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Sustituyen Decay, Tone, Sweep, Noise, FM o Drive hasta el siguiente disparo. Las seis macros están disponibles en todos los modelos. |
| `DCF` | `00-FF` | Frecuencia de corte del filtro, con asignación logarítmica. |
| `DRS` | `00-FF` | Resonancia del filtro. |

### FX de Sintered

| FX | Valor | Significado |
|---|---|---|
| `SMD` | `00-05` | Selecciona el modelo de Sintered hasta el siguiente disparo. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Sustituyen Decay, Mod, las dos primeras macros del modelo, Motion o la última macro del modelo hasta el siguiente disparo. |
| `SCF`, `SRS` | `00-FF` | Frecuencia de corte y resonancia del filtro. |

### FX de MIDI Out

| FX | Valor | Significado |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Envía un CC MIDI, reescalado a `0-127`. A qué número de CC envía cada ranura se fija por instrumento (`MC1 number`-`MC4 number` en la pantalla del instrumento), no en el propio FX - una ranura sin número está inerte. |

### FX de Plaits

| FX | Valor | Significado |
|---|---|---|
| `PMD` | `00-17` | Motor de Plaits |
| `PHA` | `00-FF` | Harmonics normalizado absoluto |
| `PTM` | `00-FF` | Timbre normalizado absoluto |
| `PMO` | `00-FF` | Morph normalizado absoluto |
| `PAX` | `00-FF` | Mezcla Main/Aux: `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Frecuencia de corte exponencial, de `20 Hz` a `20 kHz` |
| `PRS` | `00-FF` | Resonancia del filtro exponencial |

### FX de Sample

| FX | Valor | Significado |
|---|---|---|
| `SPL` | `00-03` | Modo de reproducción: `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Reproduce el slice numerado (`01` = primer slice) con independencia de la nota; `00` mantiene la asignación normal de notas. Requiere el modo de corte activado en el instrumento |
| `SPT` | `XX` con signo | Transposición de la muestra en semitonos |
| `SST` | `00-FF` | Inicio de reproducción normalizado |
| `STA` | `00-FF` | Alias de `SST` (inicio de reproducción normalizado) |
| `SEN` | `00-FF` | Final de reproducción normalizado |
| `SVL` | `00-FF` | Volumen absoluto de la muestra |
| `SCF` | `00-FF` | Frecuencia de corte exponencial, de `20 Hz` a `20 kHz` |
| `SRS` | `00-FF` | Resonancia del filtro exponencial |
| `SSP` | `00-FF` | Velocidad de la muestra, asignada de `0%` a `500%` |

### FX de AY compartidos por los instrumentos AY

| FX | Valor | Comportamiento detallado |
|---|---|---|
| `AYM` | `XY` | `X` es la forma de la envolvente; `Y` elige off/tone/noise/tone+noise (`0/1/2/3`). |
| `NOI` | `XX` con signo | Desplazamiento relativo acumulado del periodo del ruido. |
| `NOA` | `00-1F`, `FF` | Periodo de ruido absoluto; `FF` cede la prioridad del periodo del ruido a las pistas anteriores. |
| `ERT` | cualquiera | Redispara la forma de envolvente de hardware actual. |
| `EAU` | `XY` | Relación de envolvente automática `X:Y`; `X=0` la desactiva. |

### FX exclusivos de AY Classic

| FX | Valor | Comportamiento detallado |
|---|---|---|
| `EVB` | `XY` | Vibrato del periodo de la envolvente: `X` velocidad, `Y` profundidad. |
| `EBN` | `XX` con signo | Suma `XX` al periodo de la envolvente en cada fila de frase/tabla. |
| `ESL` | `XX` ticks | Desliza desde el periodo de envolvente anterior hasta el nuevo valor. |
| `ENT` | nota `XX` | Fija el periodo de la envolvente a partir de la nota de la tabla de alturas mostrada por la interfaz. |
| `EPT` | `XX` con signo | Desplazamiento relativo acumulado del periodo de la envolvente. |
| `EPL` | byte `XX` | Fija el byte bajo del periodo de la envolvente. |
| `EPH` | byte `XX` | Fija el byte alto del periodo de la envolvente. |

### FX de AY Plus

| FX | Valor | Comportamiento detallado |
|---|---|---|
| `TNN` | nota `XX` | Fija el oscilador de tono a una nota de la tabla de alturas, ignorando la nota de la fila. |
| `TNP` / `TNF` | `XX` con signo | Desplazamiento acumulado del tono en pasos de altura / fino. |
| `TRT` | cualquiera | Redispara la fase del oscilador de tono donde se admita. |
| `ENN` | nota `XX` | Fija el oscilador de envolvente a una nota de la tabla de alturas. |
| `ENP` / `ENF` | `XX` con signo | Desplazamiento acumulado de la envolvente en pasos de altura / fino. |
| `SFT` | `00-07` | Tipo de software: None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | nota `XX` | Fija el oscilador de software a una nota de la tabla de alturas. |
| `SFP` / `SFF` | `XX` con signo | Desplazamiento acumulado del oscilador de software en pasos de altura / fino. |
| `SRT` | cualquiera | Reinicia la fase del oscilador de software. |
| `SFM` | `XX` con signo | Desplazamiento acumulado de la profundidad de FM. |
| `PWM` | `XX` con signo | Desplazamiento acumulado del ancho de pulso. |
| `SPL` | `XX` con signo | Desplazamiento acumulado del nivel bajo del pulso. |
| `SWT` | `XX` con signo | Desplazamiento acumulado del índice de tabla de ondas. |

### FX de AY Sample

AY Sample acepta los comandos AY compartidos más `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` y `SFF` como se ha descrito arriba.

| FX | Valor | Comportamiento detallado |
|---|---|---|
| `SMS` | `00-FF` | Fija el inicio de reproducción heredado de AY Sample en `XX x 64` muestras de origen. |

Los FX de parámetros de motor se aplican a su tipo de instrumento correspondiente y se restablecen en el siguiente disparo de nota, salvo que se indique lo contrario. `SPD` es persistente a propósito.

El selector de FX filtra esta referencia para mostrar los comandos comunes más el grupo que admite el instrumento activo. El panel de ayuda contextual da los detalles de los valores mientras se edita.

## 10. Mixer, Reverb y Delay

El Mixer es la pantalla principal situada más a la izquierda. Muestra la carga de CPU y las advertencias de recorte. Un `!` rojo en la columna **CLIP** marca la pista cuya señal seca ha llevado la mezcla más allá del rango seguro.

Úsalo para equilibrar las 8 pistas, dar forma a cada pista con el Tilt EQ y enviar audio a la reverb o al delay:

| Campo | Significado |
|---|---|
| LVL | Nivel de la pista tras el motor, `000-100` |
| REV | Envío a la reverb Clouds compartida, `000-100` |
| DLY | Envío al delay ping-pong compartido, `000-100` |
| TLT | Tilt EQ por pista, `00-FF`; `80` es neutro, los valores bajos favorecen los graves y los altos los agudos. Los valores bajos introducen una suave saturación. |
| M | `*` silencia esta pista |
| S | `*` pone esta pista en solo |

El mixer está basado en pistas, no en instrumentos. Si una pista cambia de instrumento, su nivel, PAN, Tilt y envíos siguen unidos a la pista. Los instrumentos también tienen sus propios **Vol** y **Pan** de `00-FF` en la fila superior de la pantalla Instrument. El PAN del instrumento se aplica primero y después el de la pista. Ambos preservan una imagen estéreo existente en el centro; atenúan progresivamente el lado opuesto cuando se mueven a la izquierda o a la derecha.

### Master FX

La entrada **MASTER FX** abre un submenú Insert FX de dos ranuras para toda la
mezcla. Sus efectos se ejecutan después de los retornos de reverb y delay, antes del
volumen master. Selecciona un módulo, edita sus parámetros o ponlo en bypass exactamente igual que en un
insert de pista; pulsa **OPT** para volver al Mixer. Los ajustes de Master FX se guardan
con el proyecto y no se automatizan con frases, tablas ni modulación.

### Clouds Reverb

Usa **SELECT + [UP]** desde el Mixer para abrir la pantalla de ajustes de reverb.

- **Return**: nivel de reverb húmeda en la mezcla master.
- **Time**: control de decaimiento/tiempo, `00-FF`.
- **Damping**: absorción de altas frecuencias, `00-FF`.
- **Filter**: filtro paso bajo aplicado antes de la reverb.

Esta es la sección de reverb de Clouds, no el procesador granular Clouds completo.

### Ping-pong Delay

Usa **SELECT + [DOWN]** desde el Mixer para abrir Delay y luego **SELECT + [UP]** para volver. Reverb funciona en sentido contrario: **SELECT + [UP]** la abre y **SELECT + [DOWN]** vuelve al Mixer.

- **Return**: nivel de delay húmedo.
- **To Reverb**: cantidad de la señal de delay húmeda enviada a la reverb compartida, `0-100%`.
- **Ticks**: tiempo de delay en ticks del tracker (`9 ticks` = `1 tiempo` por defecto).
- **Feedback**: cantidad de realimentación cruzada, limitada al `95%`.
- **Filter**: filtro paso bajo en el camino de la señal retardada.

La 1.ª repetición sigue a la entrada estéreo. La realimentación posterior se cruza entre los canales izquierdo y derecho.

## 11. Pantalla Project

La pantalla Project ofrece los comandos **Load**, **Save**, **New**, **Export**, **Manage** y **Scale**, junto con los metadatos de nombre de archivo, título y autor.

### Ubicación de las exportaciones (Export location)

Las exportaciones, stems y bounces se escriben en una carpeta propia de cada proyecto dentro del directorio de muestras preexistente de la aplicación:

- **Por defecto**: `<app folder>/samples/Exports/<project name>/`. Mientras el proyecto no tiene nombre, la carpeta es `current-project`.
- En la versión web, el valor por defecto es `/user/samples/Exports/<project name>/`.
- En Android, la carpeta de muestras está dentro del espacio de trabajo privado de la aplicación.

La fila **Folder** de la pantalla EXPORT muestra el destino activo:

- Toca **EDIT** para elegir una carpeta personalizada con el explorador de carpetas. La carpeta elegida se usa exactamente como se seleccionó (no se añade subcarpeta de proyecto) y se recuerda en los ajustes.
- **EDIT + OPT** (clear) restablece la ubicación por defecto.

Cuando un proyecto con nombre se guarda con un nombre nuevo, la carpeta de exportación por defecto se renombra para coincidir, de modo que los archivos exportados anteriormente se quedan con el proyecto. El renombrado no se aplica a una carpeta personalizada, y el guardado automático no renombra la carpeta. Si ya existe una carpeta con el nombre de destino, se conservan ambas. Si el tracker ha perdido la pista de la carpeta (por ejemplo, tras un reinicio), guardar un proyecto con nombre renombra en su lugar la carpeta `current-project` existente. Los instrumentos siguen funcionando después de un renombrado: las rutas de muestras cargadas que apuntan a la carpeta renombrada se actualizan automáticamente.

**Load** también acepta archivos `.mid`/`.midi`, importados como un proyecto nuevo: las notas se agrupan por canal MIDI (un canal por pista, hasta el número de pistas), se cuantizan a 4 filas por tiempo y se colocan en un único instrumento AY por defecto - los números de programa MIDI no tienen equivalente chiptune, así que elige instrumentos reales después. Solo se usa el primer tempo del archivo (una única velocidad de ticks global, sin cambios de tempo por sección). La fila **MIDI** de la pantalla Export hace lo contrario: escribe las notas, el volumen y el tempo/groove del arreglo actual como un archivo MIDI estándar (una pista MIDI por pista del tracker); los instrumentos [MIDI Out](#midi-out) y otros FX por fila más allá del volumen y del groove global no tienen equivalente MIDI y no se traducen.

**Load** también acepta canciones Dirtywave M8 (`.m8s`, firmware 2.x-4.x), importadas como un proyecto nuevo. Solo pasan la estructura y las notas (filas de la canción, cadenas con su transposición, frases con nota, velocidad y número de instrumento, note-offs, tempo y nombre de la canción); las notas conservan su altura real (el M8 nombra las octavas dos más altas: su `C-6` es `C-4` aquí). Cada instrumento usado por una frase se importa con el nombre del instrumento del M8. Los instrumentos **MACROSYN** del M8 se convierten en instrumentos Braids (las 47 formas son los modelos de Braids, con los mismos números; timbre, color, tipo de filtro, frecuencia de corte, resonancia y pan se trasladan, y la curva de corte es aproximada). Los instrumentos **SAMPLER** se convierten en instrumentos Sample: el `.m8s` solo guarda la ruta del WAV en la tarjeta del M8, así que el archivo se busca bajo la carpeta del `.m8s` y sus padres (sirve una copia de la estructura de la tarjeta) y en tu carpeta de muestras actual (`Samples` o `samples` funcionan ambas); si no se encuentra, el instrumento conserva la ruta del M8 y queda en silencio. Las muestras importadas reciben +24 semitonos en su ajuste Pitch, ya que el M8 reproduce una muestra a su altura original dos octavas por debajo de lo que lo hace esta aplicación. **FMSYNTH** se convierte en un instrumento Genesis FM (aproximación tosca), **WAVSYNTH** y **HYPERSYNTH** se convierten en aChChid. **MIDI OUT** se convierte en un instrumento MIDI Out (canal, banco, programa y los cuatro primeros números de CC). Los demás tipos (externos) se convierten en un instrumento AY por defecto - elige sonidos reales después. Los FX y las tablas se ignoran.

En ordenador, la fila **M8S** de la pantalla Export hace lo contrario, de forma limitada: tócala y elige un `.m8s` existente como **plantilla** (se recuerda hasta que salgas, **EDIT + OPT** la olvida). Su canción, cadenas, frases, tempo y título se sustituyen y todo lo demás se conserva tal cual, salvo que los instrumentos Braids se reescriben como MacroSynth y los instrumentos Sample como Sampler (ruta `/Samples/<file name>`: copia tú mismo el WAV allí en el M8), siempre que la plantilla tenga un instrumento de ese tipo del que copiar los demás parámetros. Los FX no se exportan, y una canción que use la frase `FF` o superior no puede exportarse (el M8 tiene 255 frases). El archivo exportado aún no se ha probado en un M8 real. Consulta [M8 songs](../m8s-format.md) para los detalles.

En ordenador, el key jazz permite escribir el nombre de archivo, el título y el autor directamente en el teclado en lugar de usar el teclado virtual en pantalla - véase [Key jazz](#key-jazz-solo-en-ordenador).

### Scale / Quantize

La pantalla Scale controla el motor de escala global de 12-TET. La fila **Mode** elige entre dos modos:

- **Quantizer** (por defecto): la entrada en las frases sigue siendo cromática; cuando está activado, las notas se redondean hacia abajo justo antes de llegar al motor de sonido, de modo que cambiar la fundamental o la escala reharmoniza una canción sin editar sus frases.
- **Note Lock**: las notas escritas en las frases se ajustan a la nota más cercana de la escala al introducirlas, de modo que solo se pueden escribir notas de la escala. Las entradas nuevas y las disminuciones se ajustan hacia abajo; `EDIT + RIGHT` / `EDIT + UP` suben la nota hasta la siguiente nota de la escala por encima. La entrada siempre sigue la escala del proyecto en esta pantalla: cambiar la fundamental o la escala afecta de inmediato a las notas recién introducidas en las pistas activadas, mientras que las notas ya guardadas no se tocan. El FX `SCL` no está disponible en este modo y cualquier `SCL` ya escrito se ignora. La cuantización de reproducción de las notas simples se omite (la entrada ya está bloqueada); la cuantización de acordes mediante `CRD` se sigue aplicando.

Con el cursor sobre **Mode**, la barra de estado mantiene visible una descripción fija del modo activo: `Quantizes notes on playback` para **Quantizer**, `Snaps sequencer to scale` para **Note Lock**. Alternar el modo cambia el texto de inmediato; cualquier mensaje de acción lo sustituye hasta que expira.

Ambos modos comparten la misma fundamental, escala, edición de notas **Custom** y casillas de pista. Cada una de las ocho casillas de pista decide a qué pistas se aplica el modo. El motor de escala se omite para las tablas de alturas que no tienen 12 notas y para los instrumentos Sampler cuyo modo **Slice** es EQUAL, AUTO o LAZY (sus notas seleccionan slices de forma cromática desde C-0); la entrada MIDI no forma parte de esta versión.

- **Linear pitch** selecciona el modo de la tabla de alturas. **Off** es el valor por defecto y el ajuste validado en hardware para un seguimiento de octavas correcto de AY, Braids y Plaits.
- **Tick rate** fija el tempo del tracker y muestra los BPM correspondientes (`tick rate x 60 / 24`).
- ChooChooTracker guarda los proyectos como `.cct`. Este formato no es compatible con ChipNomad.

Usa **Save** antes de cambiar los tipos de instrumento o cargar otro proyecto.

## 12. Settings

- **Repeat delay / speed** ajustan la repetición de un botón mantenido.
- **Stick live mode** selecciona `HOLD`, `TOGGLE` o `FREE` para el botón de Stick live ya asignado.
- **MIDI** abre el submenú [MIDI](#14-midi): selección de dispositivo y asignación de canales MIDI In a instrumentos.
- **Synths** contiene **AY Quality**, **Sample dithering** y los ajustes globales **Braids BITS / DRFT / SIGN**.
- **Mixer** contiene **Mix volume** y el **Tilt pivot** de `250-4000 Hz` propio de cada proyecto (por defecto `1 kHz`) que usan todos los controles Tilt del Mixer.
- **Graphics** contiene **Edit color theme**, **Load font**, **Persistent waveform**, **Track visuals**, **Waveform FPS** y el renderizador activo (`GPU` o `Software`). **Waveform FPS** fija la frecuencia de actualización del osciloscopio del instrumento y de los visualizadores del mixer, de `01` a `60`. Track visuals elige **Detailed** (la forma de onda del sintetizador y sus superposiciones) o **Audio waveform** (la salida real sumada de esa pista) de forma independiente para cada pista; la forma de onda de audio borra de inmediato cualquier superposición ADSR previa al empezar la reproducción. Las fuentes y los temas de ChipNomad deberían funcionar.
- **Key mapping** personaliza los controles.
- **Support report** escribe un pequeño archivo de texto de diagnóstico con la versión de la aplicación, la plataforma, los ajustes de audio y la carga de CPU, el estado de reproducción/desbordamiento y un resumen compacto del proyecto. No incluye datos de muestras ni el texto del título/autor del proyecto. En Android y Web abre el flujo normal de exportación/descarga; las versiones de ordenador y de consola portátil guardan `support-report.txt` en la carpeta con permiso de escritura de la aplicación.
- **Quit ChooChooTracker** sale limpiamente.

**Graphics → Boot screen** selecciona **Original** (por defecto) o **Night**. Toca EDIT
o usa EDIT + dirección para cambiarlo; la elección se guarda para la próxima
pantalla de título. Night usa la escena ferroviaria original con tonos gris azulado más suaves,
una Vía Láctea procedural con sutiles centelleos de estrellas y alguna estrella fugaz ocasional.
El paisaje y el tren originales conservan sus bucles de desplazamiento.

## 13. Rendimiento y solución de problemas

### CPU

El coste de CPU depende de los motores y efectos activos. Los modelos físicos de Plaits y la Clouds Reverb son más pesados que las voces AY básicas. Comprueba el rendimiento en la consola de destino, sobre todo con 8 voces de Plaits y los dos envíos activos. Una lectura de CPU cercana al `100%` puede provocar chasquidos o plazos de audio incumplidos.

La pantalla de CPU es una medida suavizada del tiempo de renderizado de audio. No mide los retrasos antes de que se ejecute el callback de audio ni en la salida de audio del dispositivo, por lo que pueden producirse chasquidos incluso con una lectura baja.

En Android, la salida de audio ahora prefiere AAudio para evitar los chasquidos intermitentes observados con OpenSL ES en el Pixel 7a. Esta salida con búfer prioriza una reproducción estable y puede añadir algo de latencia de respuesta. El lienzo del tracker mantiene su disposición 4:3 completa en pantallas cuadradas y otras proporciones inusuales; el espacio no usado se muestra como bordes en lugar de recortar columnas. Si los chasquidos persisten, indica la canción, el modelo de teléfono, la versión de Android y si usas el altavoz, auriculares o Bluetooth.

Si encuentras chasquidos, cierres inesperados o ralentizaciones, usa **Settings > Support report** y adjunta el `support-report.txt` generado al informe de error. Si una canción concreta provoca el problema, adjunta también ese `.cct`.

### Sin sonido

Comprueba el número de instrumento, el estado de silencio o solo de la pista, el LVL de la pista, el Mix volume de la aplicación y la envolvente del instrumento. Para las muestras, comprueba que el WAV original sigue existiendo en su ruta guardada. En ArkOS, pulsa **MENU + L3** para alternar el silencio del sistema operativo. Si eso no ayuda, reinicia el hardware y vuelve a conectar tu interfaz de audio.

### Los controles se sienten mal

Ajusta Repeat delay y Repeat speed en Settings. Si una sola pulsación mueve dos veces, comprueba que solo 1 control físico esté asignado a esa dirección e informa de la pantalla y el atajo exactos.

## 14. MIDI

Ordenador y PortMaster. Settings > **MIDI** abre este submenú: **MIDI In** / **MIDI Out** eligen un dispositivo conectado, recorriendo los puertos detectados con `OFF` en ambos extremos; ambas direcciones necesitan un dispositivo seleccionado. La selección de dispositivo es por sesión: no se guarda en settings.txt, porque la posición de un puerto en la lista puede cambiar entre reinicios o cuando los dispositivos se conectan en un orden distinto. El control **PLAY** de la aplicación sigue disponible desde esta pantalla e inicia la canción. En PortMaster, conecta un dispositivo MIDI USB antes de iniciar; debe estar expuesto por el sistema operativo de la consola portátil a través de ALSA.

- **Escucha de sonidos desde un teclado MIDI**: con un dispositivo MIDI In seleccionado, tocar notas en el teclado conectado hace sonar un instrumento en cualquier pantalla, igual que el atajo en pantalla **EDIT + PLAY** en la pantalla [Instrument](#5-instruments). Es solo escucha: no introduce notas en la canción.
- **Channel mapping**: abre una lista de los 16 canales MIDI; asigna un instrumento a cada uno (`OFF` por defecto) para que las notas recibidas en ese canal hagan sonar ese instrumento con independencia de cuál esté seleccionado en la pantalla Instrument - por ejemplo, canal `01` -> instrumento `05`. Un canal dejado en `OFF` vuelve al instrumento seleccionado en ese momento, el comportamiento original. Esta asignación se guarda en settings.txt.
- **MIDI CC mapping**: asigna un controlador entrante a un parámetro de instrumento, incluido su **Volume**, o a controles globales. Las filas globales muestran `TRK 1`-`TRK 8` en lugar de un instrumento y ofrecen, por pista, **Mute**, **Solo**, **Volume**, **Reverb send** y **Delay send**; **Song play/stop** no tiene destino. Una asignación nueva ignora el CC que la aprendió o configuró y surte efecto en el siguiente movimiento físico, de modo que configurar asignaciones durante la reproducción no puede poner un patch a cero. Mute y solo usan `0-63` = off, `64-127` = on.
- **Controlar un dispositivo MIDI externo**: da a una pista el tipo de instrumento [MIDI Out](#midi-out) y fija su Channel; disparar notas en esa pista envía MIDI real al dispositivo MIDI Out seleccionado en lugar de producir sonido en ChooChooTracker.

## 15. Créditos y licencias

ChooChooTracker es un fork de ChipNomad y conserva su enfoque de licencia MIT. El código de Braids, Plaits, Plaits-Alt, Clouds DSP, el DSP de MME derivado de Warps y stmlib procede de los lanzamientos de código abierto de Mutable Instruments, bajo sus avisos MIT aplicables. Plaits-Alt procede del fork Plaits Lab de lylepmills/eurorack; se aplican sus avisos de código fuente conservados. El motor aChChid usa Open303 de Robin Schmidt, copyright 2009, bajo la licencia MIT. Bogie y Sintered son implementaciones nativas originales. La E/S MIDI usa RtMidi de Gary P. Scavone, bajo su licencia de estilo MIT. Consulta los archivos de licencia incluidos para las atribuciones exactas.

## Efectos de inserción de pista (Track insert effects)

Cada una de las ocho pistas tiene dos ranuras en serie, **TF1 → TF2**. Desde Instrument,
usa el gesto habitual Shift+Arriba para abrir MOD y luego Shift+Arriba de nuevo para **F: Insert
FX**. Shift+Abajo vuelve a MOD. OPT+Izquierda/Derecha selecciona la pista (1-8), sin
cambiar el selector de instrumento. La cabecera del módulo abre el selector existente;
el bypass conserva el módulo, los valores configurados y la automatización. Los parámetros son
valores de un byte: columna izquierda 1-4, columna derecha 5-8. El texto contextual muestra los nombres completos y
los valores decodificados. Los proyectos antiguos se cargan con ambas ranuras en OFF.

La pantalla de navegación mantiene visible la columna completa F-M-I-P en todas las
pantallas de instrumento, incluidas Insert FX y el instrument pool, y resalta
la pantalla actual.

| Módulo | Parámetros en orden |
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

Los controles continuos usan 00-FF, incluido un cero válido y el máximo FF. Los
controles bipolares tienen un neutro exacto en 80. Los modos de Distortion son 00 Density, 01 Drive,
02 Spiral, 03 Mojo, 04 Dyno. Las proporciones del Compressor 00-07 son 1,5, 2, 3, 4, 6, 8, 16,
20:1; las fuentes del detector 00-02 son entrada estéreo, izquierda, derecha. Detector filter 80 es
neutro, por debajo es un paso bajo y por encima un paso alto. No se proporciona sidechain
externo ni entre pistas. Cualquiera de las dos ranuras puede contener cualquier módulo; los módulos repetidos son independientes.
Chorus, Flanger, Phaser y Rotary usan valores de modulación por defecto suaves. Rotary Speed
usa el rango completo **00-FF**: **00 = 0,01 Hz** (una rotación cada 100 segundos),
**FF = 25 Hz** (el antiguo máximo de Chaos). El espaciado exponencial da un control más fino
del movimiento lento del LFO. Los nuevos inserts Rotary parten por defecto de **8F**, aproximadamente
0,8 Hz. Esto sustituye los cuatro valores de velocidad discretos; los ajustes Rotary existentes
y la automatización de velocidad F11/F21 necesitan reajustarse a la nueva escala (los antiguos Slow/Fast/Hyper/
Chaos se corresponden aproximadamente con **8F/CB/E7/FF**). Los demás parámetros no cambian.
La profundidad de bits del Bitcrusher va de 4 a 16 bits y
la reducción de frecuencia de 1x a 32x. Los modos de Destruction son Fold, Clip y Crush.
Saturation y distortion incluyen controles de nivel de salida o de mix para equilibrarlos
con la señal sin procesar.
El Parametric EQ tiene shelves de realce/recorte de 24 dB en 60-500 Hz y 2-14 kHz, más una
campana de Q fija de 0,9 entre 250 Hz y 5 kHz. Sus controles de ganancia son neutros en 80; con los
tres en neutro es un bypass exacto. El EQ activo usa un soft knee transparente por encima de la
escala completa y alcanza un máximo de 1,5 para contener los realces extremos.

El nivel de pista y el tilt EQ preceden a los inserts, así que ambos afectan a la señal que entra en
la compresión y la distorsión. Todas las voces/notas de acorde de la pista alimentan una sola
cadena de inserts. Los envíos de reverb y de delay reciben su salida. Los efectos compartidos, los retornos,
los controles master y el silencio/solo conservan su comportamiento existente. Los inserts no
procesan el audio de dispositivos MIDI externos.

Los comandos de insert usan **F11-F18** para TF1 y **F21-F28** para TF2. La selección de FX
del roll y la ventana Phrase/Table solo ofrecen los parámetros usados por los inserts configurados
de la pista seleccionada. La ventana los agrupa como **TF1: nombre del efecto** y
**TF2: nombre del efecto**; las ranuras OFF se ocultan. Los inserts en bypass siguen siendo editables.
En la ventana de FX de Phrase y Table, la descripción sigue a la ranura de insert de
la pista seleccionada: muestra el módulo, el nombre del parámetro, los extremos decodificados del rango y
el valor base guardado. Los comandos de canción existentes se conservan si un cambio de módulo los deja
sin uso; al abrir su selector se empieza en un control disponible.
Mientras se edita un valor, la pista inferior muestra su valor decodificado y sus unidades.
Cualquier columna de FX de Phrase o Table puede dirigirse a cualquiera de las dos ranuras; la cuadrícula de Phrase mantiene sus
tres columnas. Estos valores absolutos de ejecución persisten a través de notas, instrumentos,
frases, cadenas y bucles ordinarios. Una reproducción nueva o una parada brusca los restablece. Editar
un valor base en la página F borra solo la sustitución de ese parámetro. MOD actúa sobre el
valor base/de automatización efectivo sin guardar sus desplazamientos. Seleccionar un nuevo
módulo restablece los valores por defecto y borra las sustituciones de esa ranura. El bypass conserva
la automatización existente mientras suspende el procesamiento tras un breve fundido.

**Las direcciones se refieren a ranuras de pista y posiciones de parámetro, no a efectos con nombre.** Una
Phrase reutilizada en otra pista usa los módulos de esa pista. Cambiar de módulos puede
cambiar el significado de un comando; los comandos no se reescriben. Las direcciones OFF/sin uso
son no-ops seguras y permanecen intactas en los datos de Phrase/Table guardados.

El selector MOD del instrumento añade una categoría Insert FX. Las fuentes de cada instrumento
controlan los inserts de la pista que lo toca, incluida la señal combinada del acorde.
El mismo instrumento en dos pistas afecta a cadenas independientes. Se aplican la temporización existente de las fuentes,
las cantidades con signo, las reglas de combinación de fuentes, los modos de stick y la grabación de
movimiento. Los controles discretos se cuantizan y se acotan a opciones válidas. No hay
ningún banco de modulación nuevo propiedad de la pista.

El makeup del Compressor empieza en 0 dB. OTT empieza con aproximadamente un 25% de depth, un 25%
de upward, un 50% de downward, 0 dB de entrada y -3 dB de salida. Sus umbrales de banda, ganancias y
frecuencias de cruce permanecen en los valores nativos de Rui-727; Depth es su control wet/dry.
TAPESCAM conserva sus valores auxiliares nativos: cinta nueva/de alta velocidad, ruido y
compresión desactivados, ensanchamiento activado. Vigila los indicadores de recorte existentes al añadir
drive/ganancia. Las transiciones de ranura usan fundidos acotados de 5 ms a través del audio seco.

Los archivos guardados almacenan los ID de módulo, el bypass y los bytes configurados en las
secciones opcionales con versión `Track inserts: 1,8,2` y `Master inserts: 1,2`. Las sustituciones de ejecución
y los historiales de DSP no se guardan.
Haz copias de seguridad antes de abrir archivos con inserts en versiones anteriores. La atribución del DSP,
las revisiones de código fuente y las adaptaciones están en `chipnomad_lib/external/insert_fx/SOURCES.txt`;
los avisos MIT completos se incluyen en `licenses/INSERT_FX.txt`.

En un R36H a 48 kHz, un par medido de inserts en una pista consumió aproximadamente un 4-5%
del tiempo real para Compressor, un 6% para Distortion, un 18% para Doubler, un 16% para TAPESCAM
y un 24% para OTT, antes del coste de los sintetizadores y los efectos compartidos. Dieciséis instancias
activas de Doubler, TAPESCAM u OTT superaron el tiempo real. La modulación a frecuencia de audio añade
más coste. Empieza con unos pocos inserts, vigila la sobrecarga de audio y pon en bypass
las ranuras sin usar; las dieciséis posiciones disponibles no son un presupuesto de CPU garantizado.

La página Insert mantiene visible la pista del campo seleccionado después de soltar el botón;
los avisos temporales aún pueden ocupar su lugar. El selector de módulos muestra el tipo de efecto de cada
módulo y su proyecto de origen junto a su nombre.

### Instrumentos nativos OPLL y VRC7 (desarrollo)

El selector Instrument Type tiene un grupo FM con **OPLL / MSX** (YM2413)
y **VRC7** (DS1001). Cada uno ofrece ahora **73 programas** en listas Bank / Preset
con desplazamiento: los 15 timbres originales, 40 timbres distintos adicionales de las tablas
YM2413/VRC7/YMF281B de emu2413 y 18 programas de dos operadores creados para ChooChoo.
Los duplicados exactos de bytes de timbre se omiten dentro de cada motor. Las paletas
adicionales usan la ranura de timbre programable del chip; no amplían la
ROM física. El programa cero identifica un timbre personalizado. EDIT + izquierda/derecha selecciona el
preset anterior o siguiente dentro del filtro de banco. Fine ct ajusta la afinación
de -100 a +100 cents.

En la lista de programas, EDIT + PLAY escucha el sonido resaltado; suelta los
botones para detener. EDIT confirma y OPT cancela. Explorar/escuchar no cambia
el instrumento, la tabla ni la canción. El gesto de escucha normal de la página Instrument
sigue funcionando tras la selección. Las envolventes nativas del chip aportan attack/release;
el Amp env opcional añade un ADSR de software (véase Controles de los chips nativos más abajo).
No hay un editor de operadores completo.

Los instrumentos nuevos guardan los ocho bytes de timbre nativos, el banco/nombre, el programa y la afinación fina en
el archivo de canción/instrumento. Los archivos que contienen estos tipos usan el formato 7.0 y necesitan
esta versión o una posterior. Los formatos anteriores siguen siendo legibles; las canciones sin estos tipos
siguen guardándose como 5.0. Los programas de ritmo se aplazan. La biblioteca ampliada está
validada en el host; la escucha humana y una nueva comprobación en consola portátil siguen pendientes.

### AdLib / OPL2 y OPL3 (desarrollo)

El FM también incluye AdLib / OPL2 (YM3812) y OPL3 (YMF262). Bank filtra la
lista de fábrica; Preset abre grupos por categoría y una vista All. EDIT + PLAY escucha
antes de seleccionar, EDIT selecciona, OPT cancela, y EDIT + izquierda/derecha sobre Preset
carga la entrada coincidente anterior/siguiente. Fine ct ajusta la afinación local. Mode muestra
2 operadores, 4 operadores o Dual voice. OPL2 oculta los patches de OPL3 incompatibles;
OPL3 puede reproducir la colección compartida de dos operadores.

Los títulos de las ventanas de banco y de preset identifican el motor actual. Cambiar de motor o de
ranura de instrumento restablece el filtro de banco a **All banks**, incluido el cambio
entre los motores compatibles AdLib y OPL3. Volver de una ventana dentro de la misma
ranura conserva el filtro elegido. Cancelar la importación de un banco DX7 restablece el
filtro anterior.

La confirmación de un preset también publica las ediciones hechas al soltar el botón en el siguiente
tick de interfaz/audio. Las canciones en reproducción ya no necesitan otra pulsación de botón ni un
reinicio del transporte para recibir ese patch. La salida sigue el búfer de audio configurado
y el comportamiento de envolvente/redisparo del motor.

La colección de fábrica contiene 697 entradas de origen de The Fat Man 2-op,
The Fat Man 4-op y DMXOPL3, con 589 identidades de patch únicas normalizadas.
Los alias conservan los nombres de origen y la atribución. Se conservan los niveles de operador nativos de la fuente,
los indicadores de profundidad de LFO, los desplazamientos de nota, las alturas de percusión fijas y la afinación de voces dobles.
El volumen del tracker usa amplitud por software; no reproduce
las curvas de volumen ni las políticas de desplazamiento de velocidad de los reproductores MIDI originales. Las
estimaciones de duración de release de la fuente no cortan las notas sostenidas. Algunos efectos tienen
ataques lentos: mantén Seashore en lugar de esperar que un clic breve lo revele.

Los recursos de fábrica están en `instruments/chips` junto a la biblioteca de instrumentos
existente, con los avisos en `licenses/chip-banks`. La selección de un preset deja sin cambios la
tabla de la ranura y los inserts propios de la pista. Los archivos guardados de nuevo usan 7.0 y
incorporan el timbre completo. El audio de fábrica se prueba automáticamente en el host y en ARM64; la escucha
humana sigue pendiente.

### Instrumentos nativos Sega PSG y Game Boy (desarrollo)

CHIP incluye ahora Sega PSG, GB Pulse y GB Noise. Sega usa el reloj maestro NTSC
y su realimentación de ruido de 16 bits, incluido el ruido derivado del canal de tono.
Su **Bass range: Extended** por defecto baja el reloj virtual cuando hace falta para
tocar por debajo del límite del divisor del chip, de aproximadamente 109 Hz. Esto mantiene A-2, G-2
y F-2 distintos. Elige **Chip** para el rango original, donde las notas más graves
convergen en el límite del divisor. El ruido de frecuencia fija conserva el reloj original;
el ruido derivado del tono sigue la altura ampliada. Los instrumentos Sega antiguos se cargan con
Extended activado; el ajuste se guarda con el instrumento.
Los instrumentos GB usan los registros de pulso/ruido del DMG; no se expone ningún canal de onda de Game Boy.
Sus listas de presets de autoría contienen ahora **24, 24 y 28** presets
respectivamente. Las adiciones cubren el ruido Sega enlazado/periódico, barridos de altura de GB,
percusión corta, ruido metálico, drones y subidas. Son programas originales,
no sonidos extraídos de juegos.
La página ofrece controles nativos de modo/duty/anchura, envolvente/barrido/ruido según
corresponda, además de un ADSR de amplitud por software. Se guardan en archivos nativos de versión 6.
Las comprobaciones automatizadas de rendimiento/audio en ARM64 pasan dentro de los límites de
carga indicados; la escucha humana sigue pendiente.

### DX7 FM (desarrollo)

Selecciona **Type → FM → DX7 FM**; DX7 es la séptima entrada, después de Arcade / YM2151.
DX7 FM usa un núcleo MSFA de seis operadores. La página FM existente ofrece
Bank, Preset y Fine ct junto con los ajustes comunes del instrumento. La exploración de presets
usa listas por banco/categoría. Mantén EDIT+PLAY para oír el sonido resaltado; al soltar
se detiene la escucha. EDIT solo confirma al soltar; OPT cancela. Cargar un sonido
copia su patch completo sin cambiar las tablas del tracker ni los FX de insert de pista.

El catálogo de fábrica contiene actualmente **67 patches de parámetros DX7 distintos**:
31 sonidos musicales originales de OpenDX7, cuatro sonidos únicos YSE CC0 (sus 32 ranuras de banco
repiten esos cuatro con nombres distintos) y 32 patches creados para ChooChoo.
Aquí, «original» significa programas de parámetros creados para este proyecto; no
significa contenido de fábrica original de Yamaha. YSE se entrega como cuatro sonidos, no como 32
nombres artificialmente distintos.
Las categorías describen la intención de diseño sonoro revisada. Los nombres ambiguos permanecen
Unsorted. Las comprobaciones numéricas de tocabilidad han pasado; la aceptación auditiva está
pendiente. El objetivo aparte de 1.000 sonidos libres de redistribución no se ha alcanzado.

Para una biblioteca personal persistente, coloca archivos `.syx` en
**`instruments/banks/dx7/`** junto a la biblioteca de instrumentos existente. Se admiten
subcarpetas. Abre **DX7 → Bank**: cada archivo aparece como un banco con nombre, listo para
explorarlo en Preset. Vuelve a abrir Bank después de añadir o quitar archivos; no hace falta conversión ni
el paso Load Instrument. Un banco DX7/TX7 original estándar contiene
**32 voces**. Un archivo con cuatro mensajes de banco aparece como cuatro bancos numerados
(128 voces en total). También se aceptan archivos de una sola voz. La aplicación nativa
lee esta carpeta; la versión de navegador lee su sistema de archivos virtual, no carpetas
arbitrarias del ordenador.

Los archivos defectuosos o no admitidos se omiten mostrando un recuento en pantalla; los bancos válidos siguen
disponibles. El análisis está acotado a 2.048 archivos, 1 MiB por archivo, 64 MiB en total,
60.000 voces y ocho niveles de carpetas anidadas. Los enlaces simbólicos se ignoran. El
patch seleccionado pertenece al instrumento: eliminar su banco de origen no puede
cambiar la canción guardada. La biblioteca de exploración completa permanece en esta carpeta y
no se copia en bloque a cada proyecto.

LOAD INSTRUMENT también acepta volcados `.syx` originales de DX7/TX7 de una sola voz y de banco de 32 voces,
incluidas secuencias acotadas de esos mensajes admitidos. Comprueba
el encuadre, el recuento de bytes, los datos de siete bits, las sumas de comprobación, los rangos de parámetros y el tamaño del archivo
antes de abrir el banco importado en el mismo explorador FM. Seleccionar un patch
confirma; cancelar conserva el instrumento actual de la canción. Este atajo de importación directa
conserva su lista de exploración durante la sesión; usa la carpeta de bancos de arriba para una exploración
persistente. Guarda la canción o un `.cni` para conservar los patches seleccionados/editados. Los volcados sin cabecera,
las sumas de comprobación erróneas, las actuaciones/extensiones de DX7II y otras familias de Yamaha
se rechazan. La importación nunca envía mensajes MIDI a equipos externos.

CNI y la versión 6 de proyecto guardan los 155 bytes de voz originales, el nombre completo mostrado,
la velocidad de ataque nativa, la afinación fina y la identidad de origen. Las canciones no necesitan
ningún banco externo para volver a abrirse. Las versiones antiguas de ChooChoo no pueden leer estos archivos
de versión 7; los proyectos que no los usan siguen guardándose como versión 5. La velocidad nativa es por defecto
100; el volumen del tracker es una ganancia posterior a la síntesis y no vuelve a golpear la envolvente.
Las envolventes de operador, el modo de frecuencia fija, el keyboard scaling, la envolvente de altura y el
LFO siguen siendo comportamiento nativo del patch. El ADSR de amplitud FM común opcional da forma
a su salida combinada sin sustituir las envolventes de operador.

Cada parte de pista tiene un LFO y cuatro ranuras de voz de acorde de propiedad independiente, conforme
a la política existente de acordes/sustitución del tracker. Las pistas no comparten el estado de las notas.
Note-off libera las envolventes nativas; cut/panic las borra. El núcleo MSFA escalar
usa cuantos de 64 muestras a 44,1 kHz internos fijos, con un FIR en streaming hacia
la frecuencia de salida. Las tablas de consulta se inicializan una sola vez fuera del renderizado de audio, de modo que
los renderizadores simultáneos sin conexión y en directo no pueden cambiar la frecuencia del otro. Los eventos
surten efecto en el siguiente cuanto interno (hasta 1,45 ms), seguido del retardo de grupo del FIR
de aproximadamente 0,25 ms. Las muestras en búfer se conservan entre callbacks.
Es el linaje MSFA Modern, no una afirmación de identidad bit a bit con el hardware DX7 ni una
emulación de Dexed Mark I. DX7 está limitado a **16 notas activas en toda la canción**,
incluidas las colas de release, conservando cuatro ranuras de acorde propias por pista.
Cuando se supera el presupuesto, descarta primero las colas de release, luego las notas sostenidas más silenciosas
y después los ataques recientes. A igualdad de ataques, se conservan las notas fundamentales entre pistas antes que las
extensiones de acorde, con un desempate estable por ranura/pista. Esta política se aplica en todas las
plataformas, de modo que una misma canción tiene la misma asignación de notas acotada. No
cambia la polifonía de otros instrumentos. La escucha de presets está desactivada durante la reproducción.
El límite se eligió a partir de mediciones en R36H y se validó en reproducción mixta.

#### Editor de operadores FM

Los instrumentos Genesis FM, Arcade FM, OPL2, OPL3, OPLL y VRC7 tienen un botón **EDIT**
a la derecha de Fine ct. Abre una tabla con los parámetros del patch:

- Genesis FM / Arcade FM: algorithm, feedback, pan, AMS, PMS, máscara de operadores y
  ajustes del LFO, luego MUL, DT, TL, KS, AR, D1R, D2R, RR y D1L de los cuatro
  operadores, más SSG (Genesis) o DT2 y AM (Arcade).
- OPL2 / OPL3: feedback, connection y pan por voz, deep vibrato y tremolo,
  luego MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS y KSR por
  operador. En OPL3 aquí se puede cambiar el modo (2op, 4op, dual).
- OPLL / VRC7: los registros de timbre personalizado (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Editar un preset de ROM lo convierte en
  «Custom».

Izquierda/Derecha y las teclas de edición habituales cambian los valores; el instrumento sigue sonando
como de costumbre para que oigas los cambios. La barra de estado describe el parámetro bajo
el cursor. OPT o Left+Shift vuelve a la pantalla del instrumento. DX7 y SID
no tienen editor de operadores.

Genesis FM (YM2612) y Arcade FM (YM2151) usan ahora el mismo explorador FM Bank/Preset,
la misma escucha EDIT+PLAY, la misma confirmación/cancelación y los mismos controles de afinación fina. Genesis tiene ahora
**73 presets**: los 24 originales y 49 programas melódicos admitidos del banco CC0
16-Bit FM Music Station de NeoSoundFonts. Arcade tiene **81 presets**: los
24 originales y 57 programas admitidos que suenan de la colección GPLv3 de YMulator-Synth.
La pareja original de ChooChoo comparte sus recetas de base; las nuevas
colecciones de origen aportan paletas separadas. Son colecciones de diseño sonoro con nombre,
no recreaciones declaradas de bandas sonoras de juegos concretos. Los desplazamientos de nota no admitidos,
las teclas de percusión fijas, los ajustes de ruido de arcade y los programas de origen silenciosos
se registran como exclusiones en `expansion-manifest.json`. Sus ajustes completos de patch de cuatro operadores, envolvente,
LFO, estéreo y afinación viajan dentro de los instrumentos y canciones de versión 6.
La envolvente nativa del chip controla el release; la ganancia y la altura ordinarias del tracker
no la reinician. La salida DAC de Genesis usa un bloqueador de continua de 20 Hz. El modo de ruido y
el modo de frecuencia especial del canal 3 quedan fuera de esta implementación de instrumento.

Para un banco externo local, usa `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`. Acepta TFI estricto de
42 bytes, texto VOPM OPM, el formato WOPLX verificado y SysEx DX7 original.
Carga los archivos `.cni` resultantes mediante Load Instrument. Las importaciones de usuario se mantienen
separadas de la biblioteca de fábrica distribuible. Los archivos OPM con ruido activado,
datos de frecuencia de ruido distintos de cero o panorámica parcial se rechazan con una explicación;
los valores de pan VOPM 0/64/127 se convierten en izquierda/ambos/derecha. El WOPL binario no se admite.
El catálogo FM compartido contiene **1.064 entradas**: 697 OPL, 67 DX7, 146
OPLL/VRC7, 73 Genesis y 81 Arcade. Otros 76 archivos exponen los presets de Sega/Game Boy
a través del explorador de archivos normal, con un total de **1.140 presets nativos empaquetados**.
Las revisiones de origen, los hashes, los avisos completos y los datos de origen originales acompañan a las
nuevas colecciones en `licenses/chip-banks/expansion`.

### Colecciones de fábrica portátiles y presets USER

Los instrumentos de chips nativos usan **Bank:** para elegir ALL, una colección incluida o
USER. **Preset:** elige un sonido dentro de esa selección. Mantén EDIT y pulsa
Izquierda o Derecha en cualquiera de las dos filas para recorrer; un toque breve de EDIT abre su selector.

Las colecciones incluidas son paquetes ZIP en `instruments/FACTORY/`. Los archivos USER van
en `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` o `gb-noise`.
Explora las carpetas directamente o coloca allí un ZIP de presets compatibles. Los ZIP permanecen
intactos en disco. Todas estas carpetas aceptan archivos `.cni` compatibles; CNI es
el formato de instrumento portátil de ChooChoo/ChipNomad, no un estándar de ningún fabricante de chips.
El explorador USER también lee los siguientes formatos de origen, incluso dentro de ZIP:

| Carpeta USER | Formatos adicionales |
| --- | --- |
| `dx7` | Yamaha DX7/TX7 `.syx`, voces sueltas y bancos de 32 voces |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | bancos VOPM `.opm`, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | patches personalizados Furnace `.fui` y selecciones fijas de ROM |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

Un banco se abre en voces compatibles. Los archivos o funciones no admitidos indican
un motivo; las voces incompatibles de un banco mixto se excluyen mientras sus números
de origen permanecen estables. La carga es transaccional: un preset rechazado deja
el instrumento actual sin cambios. Los presets de origen sustituyen la tabla del tracker del instrumento
por una vacía, salvo el SysEx de DX7, que conserva su tabla existente.
CNI restaura su tabla guardada. Los patches cargados pertenecen a la canción y siguen
funcionando después de mover o eliminar el paquete de origen.

Los bancos WOPL pueden contener ranuras sin usar sin un indicador explícito de vacío. El explorador
omite las ranuras sin nombre cuyos operadores activos están totalmente atenuados y no tienen
attack. Las voces tocables sin nombre siguen disponibles como `Program N [Bbank]` o
`Drum N [Bbank]`, con los números de programa y de banco de la fuente contados desde cero.
Un instrumento OPLI único sin nombre usa su nombre de archivo. Los patches silenciosos con nombre se
conservan.

La compatibilidad con Furnace cubre las versiones heredadas de un solo instrumento 29-112 y las versiones
`FINS` basadas en funciones 127-251. La compatibilidad con DefleMask cubre los instrumentos de la versión 11 de OPN, OPM, Sega PSG
y Game Boy. Son importaciones de instrumentos, no reproductores de tracker
completos: las macros de FM/operadores animadas, los ajustes de LFO de FM de la canción de origen, las muestras,
las secuencias de comandos de hardware de Game Boy, las macros relativas de pulse/filter de SID y las dependencias
de ring/sync de SID se rechazan actualmente. La selección constante de ROM de OPLL
está admitida; VRC7 usa su propia ROM para esa selección. Los `.swi` de SID-Wizard y los archivos de
música SID (`.sid`) no son importaciones de instrumentos en esta versión.

Las secuencias PSG/GB/SID admitidas se guardan en el patch y se reproducen a 60 Hz para
Furnace/DefleMask y a 50 Hz para GoatTracker. Esos archivos no llevan la configuración de reproducción completa
de una canción: usa el tracker original cuando necesites su velocidad de ticks propia de la canción, el enrutamiento de canales,
el comportamiento de hard-restart o los ajustes de compatibilidad. En Sega PSG, una macro de duty selecciona la voz de ruido; sin ella, la importación
usa una voz de tono. Los archivos de Game Boy deben colocarse en la carpeta de pulse o
noise que corresponda. Las secuencias importadas siguen siendo por ahora datos de reproducción incrustados; la página
del instrumento edita los parámetros base, no las tablas del tracker de origen.

El recorrido de presets USER continúa a través de archivos, carpetas, bancos y paquetes ZIP en
ambos sentidos, con vuelta al principio en los extremos. Volver a abrir el explorador USER sigue
la selección actual. ALL incluye los sonidos USER bajo Unsorted para los motores
con categorías; USER sigue siendo una opción independiente de nivel superior. Sega PSG y Game Boy usan
listas ALL/Factory planas y un explorador USER jerárquico, sin panel
Unsorted. Los archivos DX7 antiguos de `instruments/banks/dx7/` siguen siendo accesibles en USER.

Un motor solo muestra sus colecciones incluidas compatibles. Factory Presets
combina los conjuntos de timbres OPLL/VRC7 incluidos para el motor seleccionado; DX7 combina
los originales de ChooChoo y YSE. Las colecciones con nombre descargadas permanecen separadas.
Fat Man 2-op aparece en OPL2 y Fat Man 4-op en OPL3. Los archivos CNI de OPL2
compatibles aún pueden usarse en las carpetas USER de OPL3.

La salida de los motores nativos usa una compensación de ganancia fija medida en lugar de una
normalización por preset. Por eso las canciones existentes con instrumentos nativos pueden sonar
a un nivel distinto; revisa el equilibrio de tus mezclas guardadas. El método de calibración
y sus límites están documentados en `scripts/README_MEASURE.md`.

`native-chip-audition.cct` ofrece una breve escucha secuencial de los trece
bancos de fábrica originales. Cada sección usa un instrumento propio distinto, por lo que funciona incluso
con la carpeta de presets eliminada. Los WAV de banco más largos, ignorados por Git, y los niveles medidos están
listados en `docs/chip-preset-auditions.tsv`; la escucha subjetiva sigue pendiente.

### Controles de los chips nativos (Native chip controls)

Las vistas previas de forma de onda de los instrumentos nativos se actualizan cuando se redibujan sus controles.
Los instrumentos FM pueden superponer su curva Amp env opcional; SID conserva su vista previa de forma de onda
sin esa superposición exclusiva de FM, también al explorar bancos y presets.

Los siete motores FM ofrecen **Bright**, **Feedback** y **Amp env**. Bright
va de -63 a +63, y cero conserva el patch; cambia los niveles de los operadores
moduladores conservando los de las portadoras. Su efecto audible depende del
algoritmo. Feedback vale **Preset** por defecto, o sustituye la realimentación nativa
por un valor de 0-7. Estos controles no reescriben los bytes de operador nativos guardados.

Amp env vale **Bypass** por defecto. Selecciona **ADSR** para añadir attack, decay, sustain,
release y forma alrededor del sonido nativo. A/D/R usan el rango cuadrático común de 0-5 segundos;
el sustain va del silencio al nivel completo. Las envolventes nativas siguen
funcionando, así que esta envolvente no puede alargar un sonido más allá de su release nativo. La exploración
de presets conserva los controles de amplitud FM y de timbre de la ranura. Todas las voces FM también usan
una transición de 3 ms al inicio/redisparo y un suavizado de la ganancia del tracker de 1 ms, incluso
cuando Amp env está en bypass. El hard cut/panic sigue siendo inmediato.

Los siguientes FX de frase también aparecen como destinos admitidos de Modulation y de grabación de movimiento.
Los valores de la columna FX son hexadecimales. Los ajustes se aplican a la reproducción
sin cambiar el instrumento guardado. Los controles nativos de barrido y de envolvente de GB
se fijan en el siguiente disparo de nota; el duty/anchura y la frecuencia del ruido pueden cambiar en directo.

| Motores | FX | Control |
|---|---|---|
| Native FM | `OL1`-`OL6` | Niveles de salida absolutos de los operadores: modulador OPLL/VRC7 y OPL `00-3F`; portadora OPLL/VRC7 `00-0F`; Genesis/Arcade `00-7F`; DX7 `00-63` (0-99). Más alto significa más salida. Solo aparecen los operadores admitidos. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Attack, decay, release y atenuación del sustain del operador 1, `00-0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | Multiplicador de frecuencia del operador 1. |
| Genesis/Arcade | `LFR` | Frecuencia del LFO nativo: Genesis `00-07`, Arcade `00-FF`. |
| Arcade | `LAD`, `LPD 00-7F` | Profundidades de LFO de amplitud y de altura por separado. |
| Genesis/Arcade | `LAS`, `LEN` | Sensibilidad de amplitud nativa `00-03` y activación del LFO `00-01`. |
| Genesis/Arcade | `LPS 00-07` | Sensibilidad de altura nativa. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | Attack, decay, sustain y release nativos. Los valores de tiempo aumentan hacia `0F`; el sustain aumenta hacia el nivel completo. |
| SID | `SPR 01-10` | Frecuencia del compañero silencioso de 1× a 16×; afecta a la modulación en anillo y a la sincronización forzada. |
| Todo FM nativo | `FBK 00-07` | Realimentación nativa absoluta, inicializada a partir del instrumento. |
| Todo FM nativo con Amp env activado; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack, decay, sustain, release, forma. No activan un ampli FM en bypass. |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | Tono / ruido blanco / ruido periódico; tres frecuencias de ruido fijas o frecuencia derivada del tono. |
| GB Pulse | `CMD 00-03` | Duty de pulso nativo. |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | Anchura del ruido, divisor de reloj y desplazamiento. |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | Periodo del barrido, desplazamiento, dirección descendente. |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | Nivel inicial de la envolvente nativa, periodo, dirección ascendente. |

La selección de FX nativos usa el instrumento de la columna `I` de la fila de frase, o
el instrumento activo encontrado mirando hacia atrás cuando `I` está en blanco. La ventana de FX
muestra una breve descripción del control, su valor actual del preset y el rango de comando válido
en un bloque compacto. Seleccionar un efecto
nativo distinto empieza en ese valor; volver a abrir el mismo efecto conserva su
valor editado. Una selección de varias filas resuelve por separado el instrumento de cada fila.
Las tablas usan el contexto de su instrumento. Las ediciones de valores nativos se detienen en sus
extremos legales, incluidos duty, ruido, barrido, ADSR, feedback y niveles de operador.
Los valores son hexadecimales: por ejemplo, el máximo `63` de DX7 significa 99 en decimal.

Estos comandos FM usan valores nativos absolutos. Los comandos de operador siempre apuntan al
operador 1; `OMU 03` selecciona su multiplicador 3. Los controles propios del tracker para DX7
son los niveles de operador y el feedback. Preset/Range conserva su color informativo;
los títulos y descripciones siguen los mismos colores que FM Feedback.

La modulación en directo conserva su conjunto completo de destinos nativos de operador fijo.
La grabación de movimiento solo escribe los controles representados en la lista compacta del tracker;
los parámetros de operador se graban solo para el operador 1. Los demás destinos de modulación
siguen funcionando en directo pero no generan comandos de frase.

`SCP` y `SCT` conservan asignaciones escaladas a byte hacia registros de SID de más de 8 bits.
`SMR 01-C8`, `SWV 01-08` y `SPR 01-10` coinciden ahora con la numeración nativa de los presets.
Los valores de preset mostrados son ajustes base, independientes de la envolvente
o del LFO en reproducción. `SLE` actúa sobre los niveles de operador y los parámetros FM directos sin
redisparar la nota ni cambiar el patch guardado.

Todos los FX nativos tienen títulos, rangos y comportamiento descriptivos en el selector de FX
de frase, además de pistas de valor. `CMD` describe tono/ruido, duty o anchura de ruido
según el instrumento seleccionado. Estos controles complementan los grupos compartidos Track,
Envelope y Modulation; el grupo del motor por sí solo no es el conjunto completo
de efectos de frase disponibles.

Los filtros LP/BP/HP compartidos existentes en Braids, Plaits, PCM y otros motores admitidos
son procesamiento por software posterior a la síntesis. aChChid usa en su lugar su camino de filtro
303 nativo. Esta ronda no añade ningún filtro a AY ni a FM nativo; el brillo cambia
la propia síntesis FM. Los inserts de pista siguen disponibles para procesamiento adicional.

Los instrumentos nativos se guardan en CNI versión 7 (8 cuando hay comandos absolutos),
y las canciones nativas en CCT versión 9. Los instrumentos que contienen secuencias importadas
usan CNI versión 9 y las canciones que las contienen usan CCT versión 10. Las versiones anteriores
no pueden abrir esos archivos más nuevos con secuencias. Los bytes de preset FM guardados y
los ajustes de timbre de la página del instrumento permanecen sin cambios.

### Carga de trabajo en consolas portátiles para los chips nativos

Hay ocho pistas de canción y **dos ranuras de efectos de inserción por pista**. Las
dieciséis ranuras disponibles no garantizan CPU suficiente para ejecutar dieciséis efectos.
El límite de dieciséis notas activas de DX7 es un presupuesto de síntesis aparte para toda la canción;
los acordes y las colas de release cuentan para él.

En las mediciones en R36H, cuatro pistas WAV en bucle, Sega PSG, GB Pulse, DX7 y
OPL3 con envíos compartidos y cuatro inserts (dos Compressors, Doubler, TAPESCAM)
funcionaron mejor que los arreglos FM densos de ocho pistas. La versión de una sola nota
no tuvo ningún plazo de renderizado incumplido en su medición de treinta segundos a 48 kHz/512 fotogramas;
añadir un acorde DX7 de cuatro notas produjo un pico de temporización. Las canciones FM densas
con muchos inserts costosos superaron el presupuesto de CPU. Conserva algo de margen,
vigila el indicador de sobrecarga existente y añade efectos donde ayuden a la canción.
La aplicación conserva todas las pistas y ranuras; no hay un nuevo límite estricto al número de inserts.
Consulta `chip-instruments-report.md` para las mediciones completas y el estado de las pruebas prolongadas.

La prueba física de audio de 70 segundos correspondiente de ese arreglo de acordes equilibrado
pasó sin plazos de renderizado incumplidos ni subdesbordamientos de ALSA registrados con el ajuste existente
de 48 kHz / 4906 fotogramas. Su peor callback fue de 81,819 ms frente a un
plazo de 102,208 ms. Las pruebas usaron una ganancia master aparte de 0,4 para dejar margen.
Mantén la ruta directa a la tarjeta del lanzador habitual, `AUDIODEV=plughw:0,0`: el
mezclador compartido por defecto del sistema produjo subdesbordamientos en las sondas de diagnóstico. No se cambió ningún
ajuste de audio del usuario. La prueba de esfuerzo de diez minutos con un búfer más pequeño aún registró
28 picos de temporización; los resultados completos están en el informe.

### Instrumentos SID y FX de frase

Elige **CHIP → SID**. Bank ofrece **ChooChoo SID Originals** (32 programas
de autoría) y **SIDkit Effects** (24 efectos con licencia MIT). Ambos usan el mismo
explorador de presets y guardan su programa completo seleccionado dentro del instrumento
y de la canción. No hace falta ningún banco externo para compartir la canción. Son programas
de parámetros SID nativos con envolventes y recetas de movimiento, no audio muestreado ni
canciones completas de C64. Los doce candidatos de investigación de GoatTracker, aparte, no
se incluyen; sus tablas wave/pulse/filter necesitan un importador/reproductor dedicado.

La página del instrumento expone forma de onda/ancho de pulso, modo de filtro/frecuencia de corte/resonancia
y controles ADSR nativos. SID usa el oscilador digital, la envolvente y el filtro
por ciclo de estilo 6581 de floooh/chips, fijados a una versión concreta. No hay modelos de chip alternativos,
selectores de revisión ni perfiles de carácter añadidos. Es una aproximación genérica,
no un modelo analógico calibrado de R2/R3/R4/8580.

Cada nota tiene su propio filtro. Ring/sync usan un oscilador compañero silencioso, de modo que
el reparto del filtro y las interacciones de tres voces difieren de un SID físico.
La consola portátil tiene un presupuesto de cuatro notas SID ordinarias para toda la canción.
Ring o sync cuesta dos unidades de presupuesto por nota, lo que permite dos de esas notas, o una más dos
notas ordinarias. Los acordes y las colas de release cuentan. Las notas liberadas se retiran
primero; las notas nuevas tienen prioridad sobre las notas sostenidas más antiguas. Los inserts costosos y
otros sintetizadores siguen compartiendo el presupuesto de CPU de audio.

| FX | Rango | Acción |
| --- | --- | --- |
| `SCP` | `00-FF` | Base del ancho de pulso, escalada a la anchura nativa de 12 bits; el movimiento de pulso de la receta sigue activo. |
| `SCT` | `00-FF` | Base de la frecuencia de corte, escalada al corte nativo de 11 bits; el movimiento de filtro de la receta sigue activo. |
| `SRN` | `00-0F` | Resonancia nativa. |
| `SWV` | `00-07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00-07` | Bits del modo de filtro: 1 paso bajo, 2 paso banda, 4 paso alto; 0 bypass. |
| `SMR` | `00-FF` | Reloj de macro de receta, escalado de 1 a 200 Hz. |
| `SRG` | `00-01` | Modulación en anillo triangular con compañero silencioso. |
| `SSY` | `00-01` | Sincronización de oscilador con compañero silencioso. |

El ancho de pulso y la frecuencia de corte admiten `SLE`. Las formas de onda discretas, los interruptores, la resonancia
y la realimentación FM conservan sus pasos nativos útiles; valores de byte más amplios no
crearían más estados de hardware. `FBK` selecciona directamente el feedback `00-07`.
Los niveles de operador usan los rangos propios de cada motor indicados arriba. Los ajustes de brillo y
de master conservan el rango de byte completo.

### Colecciones de fábrica portátiles y presets USER

Los instrumentos de chips nativos usan **Bank:** para elegir ALL, una colección incluida o
USER. **Preset:** elige un sonido dentro de esa selección. Mantén EDIT y pulsa
Izquierda o Derecha en cualquiera de las dos filas para recorrer; un toque breve de EDIT abre su selector.

Las colecciones incluidas son paquetes ZIP en `instruments/FACTORY/`. Los archivos USER van
en `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` o `gb-noise`.
Explora tus carpetas directamente, o coloca allí un ZIP de carpetas y de presets `.cni`
compatibles. DX7 también acepta los `.syx` admitidos de voces sueltas y bancos de 32 voces,
incluso dentro de ZIP. Los ZIP permanecen intactos en disco. La carga directa de WOPL/WOPLX no está admitida;
usa presets CNI compatibles o convierte antes sin conexión los formatos de origen admitidos.

El recorrido de presets USER continúa a través de archivos, carpetas, bancos y paquetes ZIP en
ambos sentidos, con vuelta al principio en los extremos. Volver a abrir el explorador USER sigue
la selección actual. ALL incluye los sonidos USER bajo Unsorted para los motores
con categorías; USER sigue siendo una opción independiente de nivel superior. Sega PSG y Game Boy usan
listas ALL/Factory planas y un explorador USER jerárquico, sin panel
Unsorted. Los archivos DX7 antiguos de `instruments/banks/dx7/` siguen siendo accesibles en USER.

Un motor solo muestra sus colecciones incluidas compatibles. Factory Presets
combina los conjuntos de timbres OPLL/VRC7 incluidos para el motor seleccionado; DX7 combina
los originales de ChooChoo y YSE. Las colecciones con nombre descargadas permanecen separadas.
Fat Man 2-op aparece en OPL2 y Fat Man 4-op en OPL3. Los archivos CNI de OPL2
compatibles aún pueden usarse en las carpetas USER de OPL3.

La salida de los motores nativos usa una compensación de ganancia fija medida en lugar de una
normalización por preset. Por eso las canciones existentes con instrumentos nativos pueden sonar
a un nivel distinto; revisa el equilibrio de tus mezclas guardadas. El método de calibración
y sus límites están documentados en `scripts/README_MEASURE.md`.
