# Manual do usuário do ChooChooTracker

> Tradução para o português (Brasil) de [`docs/USER_MANUAL.md`](../USER_MANUAL.md), feita a partir da versão em inglês do commit `7c3ed6f`. Em caso de divergência, vale a versão em inglês.
>
> **Convenções.** A interface do aplicativo é em inglês. Por isso, os nomes de telas, menus, campos e botões (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**, **OPT**, **SELECT**...), os códigos de FX (`HOP`, `TBX`, `M1A`...), os nomes de módulos e engines (ADSR, LFO, Braids, Plaits...) e as extensões de arquivo ficam sem tradução, para que você os reconheça na tela.

O ChooChooTracker é um tracker musical de 8 trilhas para consoles portáteis.

Ele segue o fluxo de trabalho e o sistema de navegação clássicos do LSDj, com várias opções de design de som:

- emulação do chip AY
- osciladores Mutable Instruments Braids, Plaits e Plaits-Alt, com filtro multimodo
- samples PCM, formas de onda de ciclo único e osciladores wavetable, com filtro multimodo
- aChChid, um sintetizador acid versátil baseado no Open303
- Bogie, um sintetizador nativo de bateria com 12 modelos e filtro multimodo
- MME, um sintetizador de dois osciladores com multimodulação e filtro multimodo
- Sintered, um sintetizador nativo de bateria digital experimental com seis modelos

## 1. Instalação e arquivos

### ArkOS / PortMaster

Instale o `choochootracker.zip` pelo PortMaster ou copie o pacote extraído para o diretório `ports` do console. Inicie o **ChooChooTracker** pelo menu Ports.

### Miyoo Mini / Mini Plus (Onion OS)

Extraia o arquivo MiyooPorts na raiz do cartão SD, de modo que suas pastas `Roms/PORTS/` se mesclem com as pastas já existentes do Onion. Inicie o **ChooChooTracker** pelo menu Ports depois de atualizar a coleção. Faça backup da pasta `projects/` do aplicativo antes de substituir uma instalação existente.

### Windows

Execute `choochootracker.exe` com `SDL2.dll` e `libwinpthread-1.dll` no mesmo diretório.
Pressione **ALT + ENTER** para alternar a tela cheia. A janela pode ser redimensionada; a imagem
é escalada mantendo a proporção original.

### Web

A página do navegador acompanha o tema ativo do tracker. As configurações salvas, incluindo
os mapeamentos de teclas personalizados confirmados com **Done**, são gravadas rapidamente no armazenamento
deste navegador. Depois de bloquear a tela ou trocar de aplicativo, toque em um controle para recuperar
o áudio, se necessário. **Restart audio** reabre a saída sem descartar o
projeto. **Reload app** salva o trabalho atual em **Continue**, salva as configurações
e espera o armazenamento do navegador antes de recarregar; uma gravação que falhe ou estoure o tempo
cancela o recarregamento. Use **Download .cct** para um backup portátil. O armazenamento do navegador
pode ser limpo de forma independente, e a recuperação do roteamento de controle/áudio no iOS não é
garantida.

A versão para navegador atualiza a tela a cerca de 30 FPS e usa um buffer de áudio fixo
de 1024 frames para dar mais tempo ao áudio entre os callbacks. As configurações de
buffer salvas anteriormente são ajustadas automaticamente; projetos e demais configurações
são preservados. A leitura das entradas e os timers do aplicativo mantêm o ritmo habitual.

### Configuração de diretórios

| Pasta | O que contém |
|---|---|
| `AY_Wavetables` | wavetables do chip AY em `.aywave` |
| `fonts` | arquivos de fonte ChipNomad `.cnfont` |
| `instruments` | presets de instrumento `.cni`, incluindo arquivos do ChipNomad |
| `pitch-tables` | afinações alternativas em `.csv` |
| `projects` | arquivos de música e de projeto `.cct` |
| `samples` | arquivos `.wav` para a engine Sampler |
| `SR_wavetables` | wavetables em formato Serum para a engine BYOWTBL |
| `themes` | temas ChipNomad `.cth` |
| `title` | recursos da tela de título |
| `waveforms` | formas de onda de ciclo único `.wav` para a engine 2xSCWF |

## 2. Controles

O ChooChooTracker funciona melhor com um gamepad que tenha D-pad, 2 analógicos e 7 botões.

| Controle lógico | Padrão no portátil | Padrão no Windows |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | D-pad | Teclas de seta |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | Espaço |
| **SELECT** | Select | Shift |
| Eixos de modulação | Analógicos esquerdo e direito | Analógicos do controle |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

Os mapeamentos podem ser alterados em **Settings > Key mapping**. No PortMaster, solte
o botão usado para selecionar um mapeamento antes de pressionar o novo botão. Salve um
remapeamento com **Done** e depois solte o botão antes de continuar em Settings.
Os padrões de layout para desktop usam QWERTZ apenas nas regiões QWERTZ; os teclados poloneses
mantêm os padrões QWERTY.

Usuários de Windows, web e Android também podem usar um controle de jogo. No Android,
os controles de toque aparecem automaticamente quando nenhum gamepad está conectado: no modo retrato
ficam abaixo do tracker, com um D-pad quadrado e os controles A/EDIT, B/OPT, START/PLAY e
SELECT/SHIFT, seguidos de uma seção separada Live/Motion com dois analógicos de modulação
e controles momentâneos REC/DEL. No modo paisagem, o D-pad fica à esquerda
e os botões de ação à direita. Os projetos e samples do Android permanecem privados do
aplicativo; use o seletor de arquivos do Android para importar projetos `.cct` e samples `.wav`
e para escolher onde as exportações WAV são salvas.

### Navegação

- Use **[UP/DOWN/LEFT/RIGHT]** para mover o cursor.
- Segure **SELECT + [DIRECTION]** para se mover entre telas.
- SELECT + [LEFT/RIGHT] também sai das páginas ramificadas para a tela vizinha do MSCPIT,
  como o Instrument Pool: Project/Settings (incluindo os submenus de Settings)
  levam a Mixer/Chain; Groove a Chain/Instrument; Modulation/Insert FX a
  Phrase/Table; AY Wavetable a Instrument/Table. Na borda esquerda, Reverb/Delay
  voltam ao Mixer com SELECT + LEFT. A navegação vertical existente permanece.
- Segure **OPT + [DIRECTION]** para a navegação específica de cada tela.
- Em um valor, use **EDIT + [LEFT/RIGHT]** para ajustes finos ou **EDIT + [UP/DOWN]** para ajustes grossos.
- Em um **Destination** de modulação, **LEFT/RIGHT** percorre diretamente os destinos disponíveis; toque em **EDIT** para abrir o menu agrupado.
- Toque em **EDIT** para inserir um valor ou ativar um comando.
- Toque duas vezes em **EDIT** para criar um novo chain, phrase ou instrumento onde houver suporte.
- Manter uma direção pressionada a repete após o atraso configurado em Settings.

### Seleção e área de transferência

- **SELECT + OPT** entra no modo de seleção e percorre intervalos de seleção úteis.
- Use **[DIRECTION]** para estender o intervalo.
- **OPT** copia a seleção.
- **OPT + EDIT** recorta.
- **SELECT + EDIT** cola.
- **EDIT + [DIRECTION]** edita juntas as células selecionadas.

Na tela Phrase, com uma seleção ativa, a coluna de notas pode ser preenchida
com padrões rítmicos:

- **OPT + LEFT** preenche as células de nota da seleção com um padrão fixo de 16 passos
  e avança para o próximo padrão a cada pressionamento. Depois do último padrão, as
  notas são apagadas e o ciclo recomeça. Os padrões são alinhados às linhas absolutas
  do phrase, então uma seleção parcial só recebe os passos que caem dentro
  dela.
- **OPT + RIGHT** preenche as células de nota da seleção com um padrão aleatório de
  3 a 8 notas (7 e 8 são menos prováveis), girando indefinidamente enquanto **OPT** estiver
  pressionado.
- **OPT + UP** muta as notas já colocadas na seleção: a cada pressionamento,
  escolhe de 1 a 3 delas e desloca cada uma por um valor aleatório dentro de uma oitava
  (deslocamentos pequenos são mais prováveis que extremos). Com **Note Lock** ativo,
  as notas deslocadas são ajustadas de volta à escala; instrumentos Sampler com
  **Slice** ligado permanecem cromáticos. Nada é apagado: o 8º pressionamento restaura as
  notas que a seleção tinha quando o ciclo começou, e então o ciclo recomeça.
- **OPT + UP** em uma seleção que cobre apenas a coluna de volume (sem células de nota,
  instrumento ou FX) randomiza as velocities em vez de mutar: o
  primeiro pressionamento aplica o padrão Minimal (cada volume varia em até
  +-20), o segundo Medium (até +-50), o terceiro Random (valores completamente
  aleatórios) e o quarto restaura os volumes que a seleção tinha
  quando o ciclo começou. Soltar **OPT** reinicia o ciclo em Minimal.
  Linhas sem volume ficam intocadas.
- **OPT + DOWN** espalha as notas colocadas de instrumentos Sampler com slices
  na seleção em uma sequência cromática: a primeira nota de cada instrumento (a mais alta
  no sequenciador) mantém seu valor e cada nota seguinte desse
  instrumento sobe um semitom em relação à anterior, transformando uma pilha de
  notas idênticas em disparos consecutivos de slices. Vários instrumentos com slices
  se espalham de forma independente a partir de suas próprias primeiras notas, as notas em
  instrumentos sem slices ficam intocadas, e o resultado nunca é
  ajustado à escala do projeto. A última nota que o espalhamento escreve passa a ser
  a última nota usada na entrada de notas, então continuar a sequência manualmente
  retoma no próximo slice. Uma seleção com apenas instrumentos com slices
  se espalha a cada pressionamento; quando também há notas sem slices,
  o espalhamento ocorre no primeiro pressionamento e o arpejo aleatório assume
  essas notas a partir do segundo.
- **OPT + DOWN** em uma seleção sem notas de instrumentos com slices é um arpejador
  aleatório para as notas já colocadas ali. Um acorde é escolhido na
  paleta CRD - acordes de 3 notas para seleções de 1-2 notas, acordes de 3+ notas
  para 3, qualquer acorde para 4 ou mais - com a fundamental em uma nota aleatória
  da escala do projeto dentro do registro da seleção (qualquer nota
  cromática quando a escala está desligada ou a trilha está excluída dela), e suas
  notas são quantizadas para a escala como a reprodução faz para CRD. A cada
  pressionamento o acorde é redistribuído pelas notas colocadas: aleatório,
  do mais grave ao mais agudo, do mais agudo ao mais grave, depois subindo e descendo; o 5º
  pressionamento restaura as notas originais e o seguinte escolhe um novo acorde.
  O nome do acorde aparece na barra de status enquanto o arpejo está ativo, nenhuma
  nota é adicionada ou removida, e soltar **OPT** reinicia todo o
  processo.
- Cada pressionamento de preenchimento apaga as notas da seleção antes de escrever, e as notas
  colocadas recebem o último instrumento usado; soltar **OPT** reinicia os ciclos
  no primeiro passo.
- Enquanto uma seleção está ativa e nenhuma outra mensagem é exibida, a barra
  de status alterna a cada 2 segundos um lembrete destas combinações: `EDIT + DIR =
  batch note edit`, `Double-tap EDIT: resample selection`, `OPT + LEFT =
  pattern fill`, `OPT + RIGHT = random fill`, `OPT + UP =
  mutate/randomize velocity` e `OPT + DOWN = random arp/slice spread`.
  Entrar ou sair do modo de seleção reinicia o rodízio; qualquer mensagem
  de ação substitui a dica até expirar.

### Reprodução

- **PLAY** inicia a partir do cursor. Pressione de novo para parar.
- Você pode editar notas, instrumentos e chains durante a reprodução; as mudanças são ouvidas
  no próximo tick do sequenciador.
- **SELECT + PLAY** inicia todas as trilhas quando usado fora da tela Song.
- **EDIT + PLAY** pré-escuta um instrumento a partir do Instrument Pool.

## 3. Estrutura da música e mapa de telas

Segure **SELECT + [DIRECTION]** para se mover entre telas.

A linha principal de navegação é:

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

Linha central:

- **M**: Mixer (ramifica para Reverb, Delay)
- **S**: Song (ramifica para Project, Settings)
- **C**: Chain
- **P**: Phrase (ramifica para Groove)
- **I**: Instrument (ramifica para Modulation e Instrument Pool)
- **T**: Table (ramifica para o editor AY Wavetable)

Linha superior:

- **R**: Reverb
- **P**: Project
- **G**: Groove
- **M**: Modulation

Linha inferior:

- **D**: Delay
- **S**: Settings
- **P**: Instrument Pool
- **W**: editor AY Wavetable

Uma música do ChooChooTracker segue esta hierarquia:

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

Uma música tem 8 trilhas. Cada trilha contém uma sequência de chains, e cada chain contém uma sequência de phrases. Os phrases são sequências de 16 passos de notas. Uma nota contém uma altura, um instrumento, uma velocity e até 3 FX de trilha. O instrumento é o que produz o som.

### Fontes opcionais 16x24 e NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 contém TechMonoAudit (convertida da Share Tech Mono), Departure Mono, Spleen e Cozette. Cada uma fornece o conjunto completo de ASCII imprimível em células 16×24 para uma tela de 640×480. As fontes construídas com pixels mantêm seus pixels nativos ou escala inteira com preenchimento; a Cozette continua menor porque dobrar sua altura de 13 pixels de tinta excederia a célula. O tema NostromoAmberDa2 está disponível em Settings → Edit color theme → Load. Ele mantém o fundo âmbar Nostromo e o texto informativo, com texto principal vermelho, valores em laranja mais escuro e cursor azul-petróleo. Carregar os recursos não altera a fonte ou o tema que você selecionou. Veja o README.txt que os acompanha para atribuição e licenças.

### Espaçamento da página Project

A página Project coloca a versão do aplicativo em uma linha própria e separa Load, Save, New, Export e Manage dos metadados do projeto com linhas em branco. As posições do cursor e as áreas de toque seguem os campos exibidos; as configurações do projeto e as operações com arquivos não mudam.

### Telemetria da exibição de áudio

As leituras de áudio usam um instantâneo, pertencente à interface, da mix final e das contribuições individuais das trilhas. As amostras das trilhas e os picos estéreo são capturados depois do nível da trilha e do tilt, antes dos retornos de efeitos compartilhados e do ganho master. O monitoramento não altera o sinal de áudio. O callback de áudio publica instantâneos de tamanho fixo sem locks nem alocação; o histórico exibido cobre cerca de 21 ms.

### Persistent waveform

Graphics → Persistent waveform vem DESLIGADO (OFF) por padrão. Toque em EDIT para alternar, ou use EDIT + direção; a escolha é salva como preferência do aplicativo. OFF restaura as páginas em altura total e as prévias existentes da forma de onda do instrumento. Quando ativada, uma forma de onda de duas linhas acima das páginas de edição mostra a mix final somada em mono. As páginas Instrument e Modulation mostram a saída de uma trilha que usa o instrumento selecionado; a trilha selecionada em Song prevalece se várias trilhas o usarem. Project, Settings, wavetable e popups mantêm seu conteúdo em altura total. As listas rolam dentro das linhas restantes. Os rodapés de instrumento mantêm as prévias de ADSR/sample, e os campos de Modulation são compactados para manter todos os controles acessíveis. Suas áreas de toque seguem os campos exibidos nos dois modos de forma de onda.

### Playback piano

Um piano de pixels desenhado por código, abaixo das oito linhas de trilha do lado direito, acende as classes de altura que estão soando, dobradas em uma oitava. Ele inclui as notas de acordes e respeita trilhas silenciadas/paradas. O contorno usa a cor de valor do tema e todas as teclas ativas usam a cor da forma de onda; as teclas inativas têm preenchimento escuro e sombras sutis. Sua grade usa escala inteira e bordas de largura fixa. O piano é um indicador de reprodução, não um controle de entrada de notas. Os popups e o editor de wavetable mantêm seu próprio conteúdo nessa área.

### Mixer meters

O Mixer mostra barras de pico **L/R** lado a lado para cada trilha, depois de seu PAN e tilt, antes dos efeitos compartilhados e do ganho master. O medidor cobre de -48 a 0 dBFS e decai entre as atualizações. As faixas inferior e intermediária usam cores do tema, enquanto a faixa superior fica vermelho-vivo perto do fundo de escala.

## 4. Song, Chain e Phrase

### Song

A tela Song contém 8 colunas, 1 por trilha de áudio. Cada célula se refere a um chain. Uma música pode conter até 256 linhas (`00-FF`).

As trilhas correm da esquerda para a direita, enquanto seus chains correm na vertical. Cada trilha é independente e os chains podem conter de 1 a 16 phrases, então as trilhas podem sair de sincronia. Projete seus chains para ficarem juntos, ou deixe-os divagar se for isso que a trilha pede.

É comum reservar 1 chain para phrases vazios, geralmente `00` ou `FE`.

Você pode manter várias sub-músicas em um projeto criando "ilhas": seções separadas por linhas vazias.

#### Live Mode

Toque duas vezes em **OPT** para entrar ou sair do Live Mode; `LIVE` substitui o título Song. No Live Mode, **PLAY** inicia o chain selecionado em sua própria trilha quando essa trilha está parada. Em uma trilha tocando, ele enfileira o chain selecionado para o próximo limite de chain; pressione **PLAY** duas vezes para uma troca urgente no próximo limite de phrase. Uma célula vazia de Song selecionada enfileira uma parada quantizada. `+` marca um chain enfileirado, `!` um chain urgente e `-` uma parada enfileirada.

Selecione uma linha de Song em várias colunas para enfileirar ou parar essas trilhas juntas. Seleções de várias linhas são rejeitadas de propósito. As filas do Live são limpas enquanto um loop de intervalo de Song está ativo. O Live é um modo de performance e não é salvo no projeto.

#### Controles

- **OPT + [UP/DOWN]**: pula 16 posições para cima ou para baixo
- Selecione um intervalo e use **SHIFT + EDIT**: clona superficialmente os chains mantendo seus phrases originais
- Selecione um intervalo e toque duas vezes em **SHIFT + EDIT**: clona profundamente tanto os chains quanto seus phrases
- Selecione um intervalo e use **EDIT + [UP/DOWN]**: move a seleção para cima ou para baixo
- Toque em **OPT** 3 vezes: adiciona ou remove um destaque para organização visual
- **OPT + SHIFT**: silencia a trilha atual ou as colunas selecionadas (solte **OPT** primeiro para manter o mute ativo)
- **OPT + PLAY**: solo da trilha atual ou das colunas selecionadas (solte **OPT** primeiro para manter o solo ativo)
- **OPT + [LEFT/RIGHT]**: solo de todas as trilhas à esquerda ou à direita da trilha atual
- Selecione um intervalo e toque duas vezes em **EDIT**: abre a tela BOUNCE TO SAMPLE (veja Bounce abaixo)

#### Bounce

Selecione um intervalo de linhas e colunas de Song e toque duas vezes em **EDIT** para abrir a tela BOUNCE TO SAMPLE. O bounce inclui todas as trilhas selecionadas: as trilhas cujo primeiro chain começa mais tarde na seleção esperam em silêncio e entram em seu primeiro chain, então o arquivo contém os chains de todas as trilhas selecionadas.

A tela oferece:

- **File name**: edite com o teclado de caracteres. Quando a tela abre, o próximo número de sequência livre (`001`, `002`, ...) é proposto aqui: o primeiro número cujo arquivo `.wav` ainda não existe na pasta de exportação. Edite à vontade - o nome é usado como está. Se já existir um arquivo com esse nome, é acrescentado um sufixo `_001`, `_002`, ... para que arquivos existentes nunca sejam sobrescritos. O contador é reiniciado quando você carrega ou cria um projeto.
- **Sample rate** e **Bit depth**: as mesmas opções da tela de exportação
- **Include in a sample name**: três caixas de seleção que acrescentam tags de contexto ao início do nome do arquivo quando **Start** é pressionado (todas desligadas por padrão):
  - **[BPM]** - o andamento atual do projeto, por exemplo `[120]`
  - **[Key]** - a nota fundamental e a escala do projeto, por exemplo `[Cmaj]`, `[F#min Pent]`
  - **[Bars:Beats:16ths]** - a duração renderizada do bounce: um phrase equivale a um compasso (16 semicolcheias), um tempo são 4 semicolcheias. Phrases completos mostram apenas compassos, por exemplo `[2]`; phrases parciais mostram `[0:2]` (dois tempos) ou `[0:1:3]` (um tempo mais três semicolcheias). Em bounces de várias trilhas, a mais longa decide. Uma seleção que termina no meio de um tempo conta uma semicolcheia a menos: a última linha selecionada é o ponto de corte, então seu note-off cai no fim da semicolcheia anterior.
  - As tags são aplicadas na ordem `[BPM][Key][Length]` antes do nome do arquivo.
- **Start**: renderiza a seleção em um arquivo WAV na pasta de exportação (veja *Local de exportação* na seção da tela Project)
- **Cancel**: volta sem fazer o bounce

Enquanto o bounce é renderizado, **OPT** o cancela. Quando o bounce termina ou é cancelado, a tela de onde você o iniciou volta.

No desktop, o key jazz também traz aqui a digitação direta de índices hexadecimais, além da seleção
com Shift+setas no estilo Phrase, Delete/Backspace/Insert e Ctrl+C/X/V - veja
[Key jazz](#key-jazz-somente-desktop).

### Chain

Um chain é uma lista ordenada de phrases de 16 passos com transposição opcional. Pode conter até 16 phrases, e o mesmo phrase pode aparecer mais de uma vez. A 2ª coluna define a transposição em semitons.

Um asterisco (`*`) aparece ao lado de um chain reutilizado no projeto. Você pode clonar chains na tela Song.

#### Controles

- **OPT + [LEFT/RIGHT]**: move entre trilhas
- **OPT + [UP/DOWN]**: move entre chains na trilha atual
- Selecione um intervalo e use **SHIFT + EDIT**: clona phrases
- Selecione um intervalo e toque duas vezes em **EDIT**: abre a tela BOUNCE TO SAMPLE para as linhas de chain selecionadas da trilha atual (veja Bounce em Song)

No desktop, o key jazz também traz aqui a digitação direta de índices hexadecimais - veja
[Key jazz](#key-jazz-somente-desktop).

### Phrase

Um phrase é o padrão da trilha em um sequenciador de passos tradicional.

Um phrase tem 16 passos, com 1 linha por passo. Cada linha contém uma nota, um instrumento, um volume de `00` a `7F` e colunas de FX. As notas usam notação de tracker, como `C-4` (nota e oitava).

Para parar uma nota, insira `NOTE OFF`, use o FX de kill-note ou toque outra nota sem instrumento definido.

Quando um chain contém vários phrases, a tela Phrase funciona como um editor de padrão contínuo. Mover-se abaixo da linha `F` ou acima da linha `0` abre o próximo phrase ou o anterior no chain.

Um asterisco (`*`) aparece ao lado de um phrase reutilizado no projeto. Você pode clonar phrases na tela Chain.

O seletor de FX mostra os comandos comuns mais os suportados pelo instrumento daquela linha. Isso evita uma longa lista global de comandos específicos de engines. O ChooChooTracker suporta mais FX que o ChipNomad.

### Controles

- **OPT + EDIT** em uma nota vazia: insere `NOTE OFF`
- **EDIT + [UP/DOWN]** em uma coluna de nome de FX: abre a tela de seleção de FX
- **OPT + [LEFT/RIGHT]**: move entre trilhas
- **OPT + [UP/DOWN]**: move entre phrases no chain atual
- Selecione um intervalo na coluna de instrumento e use **SHIFT + EDIT**: clona instrumentos
- Selecione um intervalo e use **EDIT + [UP/DOWN]**: rotaciona as linhas do phrase
- Selecione um intervalo e toque duas vezes em **EDIT**: abre a tela BOUNCE TO SAMPLE para as linhas de phrase selecionadas da trilha atual (veja Bounce em Song)

### Key jazz (somente desktop)

Pressione **Esc** na tela Phrase para alternar o key jazz, um modo de entrada de notas
no estilo piano em teclado QWERTY (como o [m8c](https://github.com/laamaa/m8c)):

- `Z S X D C V G B H N J M` e `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` digitam notas
  cromaticamente por mais de duas oitavas, usando a posição física de cada tecla,
  de modo que o layout é o mesmo em teclados AZERTY, QWERTY e QWERTZ.
- Cada tecla pressionada escreve a nota na linha atual e passa para a próxima linha.
- `[` / `]` deslocam a oitava.
- **Shift + setas** selecionam um intervalo de linhas e colunas; uma seta simples
  depois limpa a seleção.
- **Ctrl+C** / **Ctrl+X** copiam ou recortam as colunas selecionadas, ou a
  nota/instrumento/volume da linha atual se nada estiver selecionado; **Ctrl+V** cola na
  posição do cursor.
- **Delete** remove a(s) linha(s) inteira(s) (todas as colunas) e desloca o resto
  do phrase para cima para preencher o vão; o cursor permanece na mesma linha.
- **Backspace** remove o elemento sob o cursor apenas na coluna atual
  (ou nas colunas da seleção, se houver uma ativa), desloca o que está
  abaixo para cima para preencher o vão e recua o cursor uma linha, como em um
  editor de texto.
- **Insert** insere uma linha em branco, empurrando o resto do phrase para baixo.
- **Ctrl+S** salva o projeto. Fora do key jazz, Ctrl+S não faz nada.
- Esc de novo desliga o key jazz.

Enquanto o key jazz está ligado, ele assume por completo as teclas de nota na tela
Phrase, inclusive os atalhos de Edit/Opt/Motion que compartilham essas teclas - desligue-o
com Esc para usá-los novamente.

O key jazz também funciona, alternado de forma independente com Esc, em:

- **Song** e **Chain**: digite diretamente o índice hexadecimal de um chain ou phrase
  (`0-9`, `A-F`) em vez de incrementar com Up/Down. O primeiro dígito digitado
  em uma célula substitui seu valor; os dígitos seguintes digitados sem mover o cursor
  entram nela (digitar "3" e depois "F" define 3F). Em Song, **Shift+setas** selecionam um
  intervalo, **Delete** remove linhas inteiras e desloca o resto para cima, **Backspace**
  remove a(s) coluna(s) atual/selecionadas e desloca o que está abaixo delas para cima,
  **Insert** insere uma linha em branco, e **Ctrl+C**/**Ctrl+X**/**Ctrl+V**
  copiam/recortam/colam as linhas selecionadas (ou a linha atual se nada estiver
  selecionado) - os mesmos atalhos de edição de estrutura da tela Phrase.
  Isso assume o Shift, então Shift+Right/Up deixam de pular para Chain/Project
  enquanto o key jazz está ativo aqui - use Esc para recuperar esses atalhos.
- **Toda tela com campo de texto**: digite diretamente no teclado
  em vez de usar o teclado virtual da tela. Isso abrange o nome do arquivo do
  Project, o título e o autor, o nome do instrumento, o nome do tema de cores,
  o nome da tabela de afinação, o nome do arquivo de bounce e os diálogos de nome (inserir
  nome, criar pasta). O Backspace funciona como em um campo de texto normal; o Shift
  digita letras maiúsculas. O Esc alterna isso em todas essas telas ao mesmo tempo,
  e o teclado virtual continua abrindo normalmente quando o key jazz está desligado.

## 5. Instruments

Todos os instrumentos têm alguns parâmetros em comum:

- Tipo de instrumento
- Nome, com até 15 caracteres
- Velocidade padrão da tabela, em ticks por linha da tabela
- Transposição ligada ou desligada

Cada instrumento tem uma tabela padrão com o mesmo número na faixa `00-7F`. Você pode usá-las como tabelas auxiliares, mas manter as tabelas auxiliares na faixa `80-FE` evita conflitos e confusão. As tabelas estão entre as principais ferramentas de design de som do ChooChooTracker.

### Controles

- **OPT + [LEFT/RIGHT]**: move entre instrumentos
- **EDIT + PLAY**: pré-escuta o instrumento
- **SHIFT + OPT**: copia o instrumento
- **SHIFT + EDIT**: cola o instrumento

### AY Classic, AY Plus e AY Sample

Estas são as engines AY/YM originais do ChipNomad. O AY Classic expõe controles de tom, ruído e envelope no estilo do hardware. O AY Plus acrescenta osciladores de software e modulação mais rica. O AY Sample reproduz um sample através dos níveis de volume do estilo AY e é distinto da engine Sampler.

#### AY Classic

Este é um tipo de instrumento legado do ChipNomad `v1.0.0`. O instrumento AY Plus, mais novo, oferece mais recursos.

Os instrumentos AY Classic têm estes parâmetros:

- Mixer: tom ligado ou desligado, ruído ligado ou desligado, forma do envelope (`0-F`)
- Volume: envelope ADSR gerado por software
- Período automático do envelope: ligado ou desligado, com uma taxa de `1:1` a `F:F`

**AY Quality** em Settings afeta apenas a renderização de AY/YM. Não muda a qualidade de Braids, Plaits ou Sampler. **Sample dithering** se aplica à quantização do AY Sample.

Veja a [documentação do AY-3-8910 do ChipNomad](https://chipnomad.org/chips/ay-3-8910/) para detalhes do chip.

#### AY Plus

O AY Plus aplica conceitos clássicos de sintetizador ao design de som do AY-3-8910/YM2149F. Você pode pensar no AY como um sintetizador com 5 osciladores:

- 3 osciladores de tom de onda quadrada
- 1 oscilador de ruído
- 1 gerador de envelope de amplitude que pode ser usado como oscilador

Nesse modelo, cada instrumento tem 3 osciladores de hardware: tom, ruído e envelope. O AY Plus acrescenta um 4º oscilador, de software. Atualizar os registradores do AY mais rápido que a taxa de ticks do projeto abre mais opções de design de som. Pela forma como o AY funciona, os osciladores são misturados por modulação em anel. Experimente diferentes deslocamentos de altura, desafinações e combinações de osciladores para ver o que surge.

Cada oscilador tonal pode ser afinado de forma independente com deslocamentos de altura e de ajuste fino.

Tipos de oscilador de software e seus parâmetros:

- Pulse - onda quadrada com largura de pulso controlável
  - FM Depth - profundidade de FM simples
  - Pulse width - a faixa completa é `00-FF`, enquanto o número de passos é definido na tela Project
  - Pulse low - controla o valor baixo em um par repetido cujo valor alto é sempre `15`
- Sync Tone - efeito de hard sync para o oscilador de tom
  - FM Depth - profundidade de FM simples
- Sync Env - efeito de hard sync para o oscilador de envelope
  - FM Depth - profundidade de FM simples
  - Pulse width - razão de duração entre as 2 formas do par de envelopes
  - Envelope Pair - 2 formas de envelope entre as quais alternar
- Wavetable - wavetables de 32 passos, editadas na tela Wavetable
  - FM Depth - profundidade de FM simples
  - Wavetable index
- Tone FM - efeito simples tipo FM para o oscilador de tom
  - FM Depth - profundidade da FM do tom
- Env FM - efeito simples tipo FM para o oscilador de envelope
  - FM Depth - profundidade da FM do envelope

Os osciladores de software Pulse e Wavetable não podem ser usados com o oscilador Envelope. O oscilador Envelope é desligado automaticamente.

A FM simples alterna rapidamente entre 2 valores de período, 1 menor e 1 maior. Isso é semelhante a usar uma onda quadrada como modulador de FM. Pulse, Sync Tone, Sync Env e Wavetable usam uma razão `1:1` entre a portadora e o modulador de FM. Tone FM e Env FM podem imitar outras razões por meio dos deslocamentos de altura do oscilador de software.

#### AY Sample

Você pode carregar arquivos WAV mono ou estéreo em `8-bit`, `16-bit`, `24-bit` ou `32-bit`. Eles são convertidos para `8-bit` e truncados em `16 KB`. Esse limite corresponde ao tamanho da página de RAM do ZX Spectrum.

O AY reproduz samples por meio de escritas rápidas no registrador de volume. Sua saída é unipolar, então os samples precisam ser "elevados a zero" para uma reprodução mais limpa. Isso remove cliques no fim do sample e melhora as caudas silenciosas.

O AY tem DAC de resolução `4-bit` com escala de volume não linear, então os samples de `8-bit` são convertidos para `4-bit` durante a reprodução. A opção Sample dithering em Settings pode melhorar a profundidade de bits percebida. O dithering não é prático em plataformas retrô como o ZX Spectrum e o Atari ST, então deixe-o desligado ao escrever para hardware real ou ao buscar um som lo-fi crocante.

Os samples se comportam como mais um oscilador de software do AY Plus, mas seus parâmetros extras tornam mais fácil tratá-los como um tipo de instrumento separado.

Parâmetros do sample:

- Sample rate - tocar um sample em sua taxa nativa produz um `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

As seções de oscilador Tone e Noise correspondem às do AY Plus. Pela forma como o AY funciona, todos os osciladores são misturados por modulação em anel.

### Braids

O Braids oferece 47 modelos de síntese. Seus principais controles são:

- **Model**: oscilador ou algoritmo de síntese
- **Timbre** e **Color**: parâmetros macro que dependem do modelo
- **Filter**: filtro LP/BP/HP adicional, inclinação de `12` ou `24 dB`, cutoff e ressonância
- **ADSR**: attack, decay, sustain e release

Toque em **Model** para escolher entre listas de modelos categorizadas.

| Categoria | Modelos |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### Parâmetros

Timbre e Color dependem do modelo selecionado. Consulte o manual original do Braids da Mutable Instruments para a lista completa de modelos e detalhes, ou simplesmente toque de ouvido.

#### VCF e VCA

O Braids alimenta o filtro multimodo e o envelope ADSR padrão do ChooChooTracker.

#### Configurações

As configurações globais do Braids estão disponíveis nas configurações do aplicativo:

- **BITS**: reduz a resolução da saída
- **DRFT**: acrescenta instabilidade de altura ao oscilador
- **SIGN**: reproduz o algoritmo original de imperfeição de forma de onda do Braids

### aChChid

O **aChChid** é uma engine monofônica de baixo acid baseada no Open303. `Square` e `Saw` usam seu oscilador, filtro, envelope e comportamento de accent nativos do TB-303. `Braids` substitui apenas o oscilador e depois segue pelo mesmo caminho de filtro 303 e amplificador. Ele expõe Model, Timbre, Color e Shaper no lugar de Fine tune. O Shaper evolui da saturação suave ao wavefolding. O aChChid não usa o pós-filtro unificado nem o ADSR do ChooChooTracker.

Cutoff e Decay usam edição logarítmica: **EDIT + LEFT/RIGHT** move um
passo musical de controle, enquanto **EDIT + UP/DOWN** move dezesseis passos. O Cutoff
cobre de 200 Hz a 20 kHz; o Decay cobre de 200 ms a 2 s.

Um `F` na coluna de volume da nota dispara um accent. As notas normais usam um gate de meio passo (3 ticks no groove padrão de 6 ticks). `ASL` desliza até essa nota a partir da altura anterior sem redisparar o envelope do 303; ele segura automaticamente o gate anterior, e `ASL 00` dá um glide de `60 ms`. `ATY` mantém sua nota aberta durante todo o passo atual. Notas sem `ASL` sempre redisparam. Os destinos de modulação incluem Decay e Accent, além de Timbre e Color no modo de onda do Braids.

### Bogie

O **Bogie** é um sintetizador nativo de bateria one-shot. As notas do tracker definem a altura e o volume do tracker/instrumento define o nível; cada disparo tem seu próprio decay interno, então não há página ADSR. Ele tem 12 modelos: `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Rim`, `FM`, `Noise`, `Cowbell`, `Cymbal`, `Shaker` e `Clave`.

Seus seis macros são **Decay**, **Tone**, **Sweep**, **Noise**, **FM** e **Drive**. Seus rótulos se adaptam onde é útil (por exemplo, o Noise do Kick é `Click`, e o FM do Cowbell é modulação cruzada metálica). Todo macro está ativo em todos os modelos, mas seu papel musical muda com o modelo: o Sweep pode alterar a altura, o espaçamento das rajadas ou a dispersão metálica; o FM pode acrescentar modulação cruzada, uma camada metálica ou um harmônico tipo anel. A metade inferior de cada faixa é pensada para sons de bateria convencionais; a metade superior abre de propósito morphs de timbre mais amplos e resultados mais sintéticos.

Todo modelo pode usar o filtro multimodo LP/BP/HP compartilhado, com caráter Clean, Classic, Aggro ou Acid, inclinação de `12`/`24 dB`, cutoff e ressonância. O Bogie suporta todos os seus macros ativos, mais Volume, Pitch, Cutoff e Resonance como destinos de modulação e de gravação de movimento. Use Decay curto com o Cowbell para ataques phonk incisivos; aumente o FM para um sino mais duro e metálico.

Na tela Modulation, pressione **EDIT** em uma fonte para escolher uma família (Envelopes, LFO ou Sticks) e depois a fonte. **Left** e **Right** continuam percorrendo as fontes diretamente.

### Sintered

O **Sintered** é um sintetizador nativo de percussão digital one-shot. As notas definem sua altura fundamental; seus seis modelos são `Knot`, `Shard`, `Burst`, `Comb`, `Logic` e `Melt`. Todo modelo tem **Decay**, **Mod** e **Motion**, mais três controles específicos do modelo: o Knot usa Ratio/Spread/Fold, o Shard Ratio/Feedback/Bite, o Burst Noise/Color/Feedback, o Comb Time/Damping/Regen, o Logic Rate/Pattern/Crush e o Melt Ratio/Chaos/Drive. **Mod** é sempre a profundidade de acoplamento interna.

Motion é um envelope macro interno, não um glide de altura: valores abaixo de `80` usam uma forma AD, `80` o desativa, e valores acima de `80` usam uma forma só de decay. Afastar-se do centro torna o envelope mais rápido. Seus destinos são fixos por modelo, então o Sintered continua previsível em padrões de tracker: o Knot anima Mod/Fold; o Shard Feedback/Bite; o Burst Noise/Color/Feedback; o Comb Damping/Regen; o Logic Rate/Crush; o Melt Chaos/Drive. Todos os modelos reiniciam seu estado ao disparar uma nota e limitam o feedback interno para que as batidas sejam repetíveis.

### MME

O **MME** (Multi Modulation Engine) é um sintetizador de dois osciladores dedicado às interações entre osciladores. Seus modelos são `Ring`, `Fold`, `Cross`, `VPM`, `Sync`, `Logic` e `Vocode`. Os dois osciladores são internos e seguem as notas do tracker; `Interval` define a relação entre eles.

Os seis macros são **Waves**, **Interval**, **Amount**, **Flow**, **Feedback** e **Shaper**. Waves seleciona pares musicais de formas de onda A/B. Amount controla a interação central do modelo, Flow muda sua direção ou variante de algoritmo, e Feedback é limitado, mas fica propositalmente selvagem no fim de sua faixa. O Shaper é um estágio final compartilhado que vai do som limpo, passando pela saturação, até o wavefolding.

O MME usa o filtro multimodo LP/BP/HP compartilhado e o ADSR. Os seis macros, Cutoff e Resonance estão disponíveis para modulação e gravação de movimento. Segure **EDIT** em Model para abrir o popup de modelos.

### MIDI Out

O **MIDI Out** (somente desktop) controla um dispositivo MIDI externo em vez de sintetizar áudio: disparar uma nota envia um MIDI Note On real no **Channel** do instrumento (`1-16`) e, quando a nota termina, o Note Off correspondente. O volume vira velocity, e cada voz de uma trilha de acorde recebe seu próprio Note On/Off. Não há áudio para ouvir no próprio ChooChooTracker - selecione antes o dispositivo de saída em [Settings](#12-settings). Veja [MIDI](#14-midi) para o quadro completo, incluindo a pré-escuta de som a partir de um teclado MIDI.

**Program** e **Bank high/low** (CC0/CC32) são opcionais - aparecem como `--` quando desligados, e `Clear` desliga um campo e lembra seu valor. Quando definidos, são enviados uma vez, logo antes da próxima nota, sempre que diferirem do que aquele canal recebeu por último (não antes de toda nota, o que redisparia os envelopes/patch do próprio dispositivo receptor).

**MC1 number**-**MC4 number** escolhem para qual número de CC (`00-7F`) cada um dos quatro FX de linha MC1-MC4 envia - veja [MIDI Out FX](#midi-out-fx). Também `--`/opcional; um slot deixado sem definição torna seu FX inerte.

### Engines subtrativas

As engines desta categoria compartilham uma arquitetura VCO, VCF, VCA.

Vários tipos de VCO estão disponíveis:

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

Cada instrumento alimenta seu VCO em um filtro multimodo.

O filtro tem inclinação comutável de `12 dB` ou `24 dB` e pode funcionar como passa-baixa (LP), passa-banda (BP) ou passa-alta (HP).

Vários caracteres de filtro estão disponíveis:

- **Clean**: estéril e digital
- **Classic**: inspirado nos sintetizadores analógicos americanos clássicos
- **Aggro**: inspirado nos sintetizadores analógicos japoneses rugidores
- **Acid**: inspirado em psicodélicos

Classic, Aggro e Acid podem ficar seriamente ressonantes nesta alpha. Comece baixo e aumente com delicadeza.

O filtro alimenta um VCA controlado por um envelope ADSR. A forma do envelope pode transitar entre curvas exponencial, linear e logarítmica.

#### Plaits e Plaits-Alt

O **Plaits** oferece 24 engines, agrupadas no popup de seleção da seguinte forma:

| Categoria | Engines |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

O **Plaits-Alt** é uma coleção de modelos alternativos para o módulo Plaits original. A maioria é mais estranha ou mais experimental que o VCO médio.

| Categoria | Engines |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

Os parâmetros seguem o design da Mutable:

- **Harmonic** controla relações harmônicas, balanço ou escolha de modelo dentro de uma engine.
- **Timbre** em geral vai de espectros escuros/esparsos a brilhantes/densos.
- **Morph** explora outra dimensão tímbrica.
- **Main/Aux** mistura a saída principal com a saída alternativa da engine.

Seu significado preciso depende da engine. Por exemplo, as engines de acordes os usam para tipo de acorde, inversão e forma de onda. Os modelos físicos os usam para material, excitação e decay, enquanto as engines de bateria os usam para tom, caráter e decay. Consulte o manual do usuário do Plaits para detalhes.

Para `CHORD`, `DIATONIC CHORD` e `WT DIATONIC CHORD`, Harmonic exibe o nome do acorde selecionado (`MIN`, `MAJ`, `MIN7`, `MAJ7` e assim por diante) em vez do valor bruto do controle. O `WAVE PARA` do Braids nomeia de forma semelhante suas disposições de vozes de Color.

Toque em **Engine** para escolher entre listas de engines categorizadas.

**Env Mode** tem 2 roteamentos:

- `TRIG` reproduz o módulo com TRIG conectado e LEVEL sem cabo
- `VCA` mantém LEVEL aberto e aplica o ADSR do tracker depois da voz

#### Sampler

Esta engine Sample limpa reproduz samples PCM mono ou estéreo carregados na RAM.

- Toque em **Sample** para carregar um WAV PCM não comprimido de `8-bit`, `16-bit` ou `24-bit`. Pressione **PLAY** no navegador para pré-escutar o arquivo destacado.
- Quando um sample está carregado, **EDIT** aparece ao lado de seu nome. Toque nele para abrir a tela Sample Edit. **OPT** ou **SELECT + [LEFT]** volta ao instrumento.
- A tela Sample Edit mostra o nome do arquivo (com um marcador `*` quando o sample na RAM difere do arquivo em disco), uma leitura do formato (taxa de amostragem, canais), uma forma de onda alta e os campos **Region**, **Select**, **Slice**, **Process** e **File**.
- As edições vivem apenas na RAM até que um fluxo de salvamento as grave: o projeto guarda o caminho do arquivo do sample e recarrega o WAV do disco no próximo carregamento do projeto, então sair do tracker sem **Save**/**Save As** descarta as operações de processamento (o marcador `*` é o aviso). Carregar um sample diferente no instrumento também descarta as edições não salvas e reinicia a Region para o sample inteiro (Start `00`, End `FF`).
- **Region** define os marcadores Start/End da reprodução, também disponíveis na tela do instrumento Sampler. Eles definem limites de reprodução normalizados (`00-FF`); se Start estiver depois de End, o sample toca em reverso. **EDIT + [LEFT/RIGHT]** ajusta finamente o marcador em passos de um (`01`) e dá zoom na forma de onda ao redor dele enquanto **EDIT** estiver pressionado; **EDIT + [UP/DOWN]** ajusta de forma grossa (passo 16) e devolve a visão ao sample inteiro. **EDIT + OPT** restaura o marcador ao padrão (Start `00`, End `FF`).
- A visão da forma de onda dá zoom em uma janela fixa de um segundo de áudio ao redor do marcador editado e se desloca para mantê-lo visível; samples que cabem dentro da janela ficam na visão 1:1 completa. O zoom é transitório: dura enquanto **EDIT** estiver pressionado, e soltar **EDIT** volta à visão 1:1 completa, assim como os passos grossos. Entrar na tela sempre reinicia a visão para o sample inteiro.
- **Select** define uma seleção de processamento em frames absolutos, independente dos marcadores Start/End da reprodução. **EDIT + [LEFT/RIGHT]** move uma alça vinte frames e dá zoom nela enquanto **EDIT** estiver pressionado; **EDIT + [UP/DOWN]** salta `frameCount/64` (mínimo 16) e volta à visão do sample inteiro. **EDIT (tap)** em uma alça copia para ela a posição atual do marcador Start ou End. **EDIT + OPT** em qualquer alça esvazia toda a seleção. Quando a seleção está vazia, as duas alças mostram `-`; quando as alças estão invertidas, elas trocam automaticamente. A seleção é um estado de editor apenas da sessão: não é salva com o projeto, e entrar na tela a inicializa com o intervalo de Region da reprodução (o sample inteiro com os marcadores padrão).
- **Slice** é uma única linha com quatro células de valor - **Mode**, **Count**,
  **Slice** e **Frame** - e é salvo com o instrumento. Off toca a janela
  Start/End. As notas do phrase selecionam slices cromaticamente a partir de **C-0**, e
  as notas além do último slice voltam ao início (a nota N toca o slice
  `N % count`, então uma sequência além do último slice volta ciclicamente ao primeiro).
  As notas com slices não transpõem a altura nem usam a quantização de Scale, mas o **CRD**
  mantém o slice selecionado e transpõe suas vozes como acorde. Os inícios dos slices
  são marcados na forma de onda com linhas curtas de dois pixels de largura (laranja escuro; a linha
  do slice atual é mais brilhante), e o slice sob o cursor recebe
  uma faixa de fundo preta na forma de onda (a tonalidade da seleção não é desenhada sobre
  a faixa, de modo que ela fica totalmente preta e claramente distinta do resto
  da prévia).
  - **Mode** alterna entre `OFF` / `EQUAL` / `AUTO` / `LAZY` (EDIT + left/right ou
    toque; EDIT + OPT desliga o fatiamento). Trocar de modo inicializa os pontos
    de slice: **EQUAL** divide a janela Start/End igualmente em **Count**
    partes; **AUTO** executa uma detecção de transientes sobre a janela e coloca um
    slice em cada ataque que encontra (até **Count**); **LAZY** começa com um
    slice cobrindo a janela inteira. Mudar para LAZY guarda a configuração atual de
    EQUAL/AUTO (contagem e limites), e voltar para EQUAL ou
    AUTO a restaura literalmente - nenhuma confirmação é pedida e nada
    se perde. Slice e Stretch são mutuamente exclusivos: ativar um desativa
    o outro, e as células de Slice ficam esmaecidas enquanto o Stretch determina a
    duração.
  - **Count** mostra o número total de slices (`01`..`64`). **EDIT +
    [LEFT/RIGHT]** o altera de um em um (mínimo 1, máximo 64), **EDIT +
    [UP/DOWN]** percorre as contagens em potências de dois `2 / 4 / 8 / 16 / 32 /
    64` com retorno circular. Toda mudança recalcula a divisão: **EQUAL**
    divide a janela de novo igualmente, **AUTO** reexecuta a detecção com o novo
    alvo (a sensibilidade da detecção deriva da contagem - mais slices
    significam um limiar mais sensível). A caixa mostra `-` quando o modo é
    OFF. Em **LAZY** ela permanece brilhante (conta os slices colocados à mão), mas
    é somente de exibição: o cursor a pula - mover para a direita a partir de **Mode** ou
    para a esquerda a partir de **Slice** cai em **Slice**, descer a partir do marcador
    **END** da linha **Select** também cai ali, e tocar na caixa não faz nada
    (slices colocados à mão não têm divisão a recalcular). **EDIT + OPT** e
    um simples toque em **EDIT** não fazem nada aqui: desligar o fatiamento fica em
    **Mode**, e excluir um slice fica em **Slice**.
  - **Slice** é o navegador: mostra o slice atual numerado a partir de 1
    (`01`..count). **EDIT + [LEFT/RIGHT]** move entre slices um de cada
    vez, **EDIT + [UP/DOWN]** salta quatro slices (ambos limitados, sem
    retorno circular); o slice navegado é indicado na forma de onda por seu
    marcador mais brilhante e pela faixa preta, e a visão com zoom se recentraliza no
    início do slice. **EDIT + OPT** exclui o slice atual (ele se funde ao
    anterior; o primeiro slice não pode ser excluído, e excluir o
    último slice restante desliga o fatiamento - os limites sobrevivem a uma
    ida e volta por OFF e retornam quando um modo é escolhido de novo). **EDIT (tap)**
    pré-escuta o slice selecionado como reprodução one-shot (funciona em
    todo modo de fatiamento; a pré-escuta para quando a tecla é solta ou quando você
    sai da tela). A caixa mostra `-` quando o modo é OFF.
  - **Frame** mostra o frame inicial do slice atual em hexadecimal. **EDIT +
    [LEFT/RIGHT]** o ajusta de dez em dez frames (dando zoom no marcador enquanto
    **EDIT** estiver pressionado), **EDIT + [UP/DOWN]** ajusta de cem em cem e
    volta à visão completa. Um marcador permanece dentro de seu próprio slice: ele
    não pode cruzar o início do slice anterior nem do seguinte. **EDIT + OPT** exclui
    o slice.
  - **B** sai para a tela do instrumento a partir da linha Slice como em todo
    o resto desta tela; **SHIFT + LEFT** sai de qualquer lugar.
  - **Fluxo de trabalho LAZY**: com LAZY selecionado, toque em **PLAY** para iniciar uma reprodução
    one-shot do sample inteiro - ela toca uma vez desde o início e para
    sozinha na altura e velocidade originais do sample (um marcador verde de
    dois pixels acompanha a posição na forma de onda); tocar em **PLAY** de novo
    a interrompe antes. Enquanto ela toca,
    cada clique em **EDIT** deixa um slice na posição de reprodução, não importa
    onde esteja o cursor (slices a menos de 50 ms de um já existente são
    rejeitados com `Too close to slice`). Tocar o sample de novo mantém
    os cortes existentes e acrescenta novos em cada posição tocada, em
    ordem - a caixa **Count** é atualizada a cada slice colocado e o navegador
    **Slice** segue o novo. Valem as mesmas regras de navegação e edição dos
    outros modos para os slices colocados à mão: **EDIT +
    direção** em **Slice** os navega, **Frame** ajusta o início
    selecionado, **EDIT + OPT** exclui o slice atual. A célula Frame fica esmaecida
    enquanto o drop durante a reprodução está armado. **SHIFT + PLAY** não é interceptado
    no LAZY: ele inicia a reprodução do phrase como em qualquer outra tela. Os slices LAZY
    tocam no sequenciador exatamente como EQUAL e AUTO: as notas do phrase
    se mapeiam cromaticamente a partir de **C-0** sobre os slices colocados à mão,
    voltando ao início depois do último.
  - **Dicas de combinação**: enquanto o cursor repousa em uma célula da linha Slice e nenhuma outra
    mensagem é exibida, a barra de status mostra a dica daquela célula e a mantém
    enquanto o cursor estiver ali. Em **Mode** a dica está ligada ao
    modo ativo e mostra apenas a descrição (o nome do modo já está na
    célula): `Divides sample in equal parts`, `Divides sample based on
    transients`, `Press Play and add slices with EDIT` - trocar de modo
    troca o texto imediatamente, e OFF limpa a barra. Em **Count** mostra
    `Adjust the number of slices`, em **Slice**
    `Browse slices`, em **Frame** `Adjust slice start`. Qualquer mensagem de ação
    substitui a dica até expirar.
- **Process** seleciona uma operação de edição destrutiva: **Crop**, **Normalize**, **Delete**, **Silence**, **Fade In**, **Fade Out** ou **Reverse** (EDIT + left/right percorre, o toque avança, EDIT + OPT define nenhuma). **GO** (na mesma linha) executa a operação selecionada sobre a seleção atual, ou sobre o sample inteiro quando a seleção está vazia. **Crop** mantém só a seleção; **Normalize** escala a seleção para que seu pico alcance o fundo de escala (os dois canais compartilham um único ganho para preservar a imagem estéreo); **Delete** remove a seleção e une as pontas (excluir o sample inteiro é rejeitado); **Silence** a zera; **Fade In**/**Fade Out** fazem rampas lineares a partir de/para o silêncio; **Reverse** toca a seleção de trás para a frente (os frames são trocados no lugar, os dois canais de um frame se movem juntos, o comprimento e os marcadores não mudam). Crop e Delete exigem uma seleção - com uma seleção vazia mostram `Select region first`. Toda operação pausa o áudio brevemente, mantém um nível de desfazer e marca o sample como alterado na RAM: o arquivo em disco não é tocado até os fluxos de Save (veja abaixo), e sair da tela descarta o slot de desfazer.
- **UNDO** (ao lado de GO) devolve o sample ao estado anterior à última operação. Fica esmaecido até que uma operação seja executada, alterna entre os estados pré e pós-operação a cada pressionamento e é limpo quando a tela é reaberta.
- **File** contém os fluxos de salvamento. O instrumento guarda o caminho completo do WAV de onde foi carregado; esses fluxos gravam esse arquivo ou apontam o instrumento para um novo. Eles nunca tocam no nome do instrumento.
  - O campo File alterna entre **Save** e **Save As** (EDIT + left/right ou toque); **GO** (na mesma linha) executa a ação mostrada.
  - **Save** sobrescreve o WAV de onde o sample foi carregado. Para um sample
    sem slices, pergunta antes `Overwrite <name>?`. Um sample com slices primeiro
    abre o diálogo **SAVE SLICES**, perguntando a quem os pontos de slice pertencem:
    **SAVE TO SAMPLE** os grava no WAV como chunks cue (portátil -
    as DAWs os mostram como marcadores) e também os mantém no projeto,
    **SAVE TO PROJECT** grava um WAV simples e mantém os pontos apenas no
    projeto, **CANCEL** aborta. Uma caixa `Don't ask again in this project`
    guarda o destino escolhido com o projeto e dispensa o
    diálogo a partir de então (a escolha é salva como `- Sample save choice:` no
    .cct e só pode ser alterada editando o arquivo do projeto). O diálogo
    é dispensado para samples sem slices. **Save** fica esmaecido (e
    é pulado na navegação) até que um sample com caminho de arquivo seja carregado. Uma
    gravação que falha mantém o marcador de alteração e mostra o erro.
  - **Save As** grava o sample atual em um novo arquivo: digite um nome de arquivo (preenchido com o nome atual sem extensão) e depois escolha uma pasta no navegador. O sample é gravado como `<folder>/<name>.wav`, o instrumento aponta para o novo arquivo e a pasta é lembrada como pasta de samples padrão. Fica esmaecido (e é pulado na navegação) até que haja dados de sample carregados, de modo que um sample recém-carregado possa receber um caminho de arquivo. Caminhos com mais de 255 caracteres são rejeitados.
  - O marcador `*` antes do nome do arquivo significa que o sample na RAM difere do arquivo em disco (qualquer operação de processamento o ativa; Save e Save As o limpam). O marcador é apenas da sessão: não é salvo com o projeto e é reiniciado quando a tela é reaberta. Se vários instrumentos referenciarem o mesmo arquivo WAV, salvar a partir de um sobrescreve o arquivo para todos eles.
- Na tela do instrumento Sample, use **EDIT + [LEFT/RIGHT]** para carregar o WAV anterior ou o próximo na mesma pasta.
- **Pitch** transpõe em semitons (`-48` a `+48`).
- **Loop** seleciona Off, Loop ou Ping-Pong.
- **Speed** controla o time-stretching granular de `0%` a `500%` (`100%` é o normal).

Formatos de WAV não suportados exibem um erro. Converta arquivos incomuns para WAV `PCM8`, `PCM16` ou `PCM24` antes de importá-los.

#### 2xSCWF

O 2xSCWF é um oscilador duplo de formas de onda de ciclo único. Carregue em cada oscilador 1 WAV mono contendo exatamente 1 período. Os dois osciladores leem para a frente e dão a volta no fim do ciclo.

Mix faz o crossfade entre A e B.

Detune é fino e exponencial do uníssono até `+200 ct`, depois passa a andar em passos de semitom de `+3 st` a `+24 st`.

O 2xSCWF trata cada arquivo como uma tabela de 1 ciclo e a lê exatamente uma vez por ciclo do oscilador. Isso permite misturar e desafinar 2 ondas de ciclo único.

A pasta de fábrica `waveforms/AKWF/` contém formas de onda da biblioteca Adventure Kid. Há muitas outras disponíveis online se você quiser ampliar a coleção.

#### BYOWTBL

O BYOWTBL é um oscilador duplo de wavetable compatível com tabelas Serum. Cada oscilador carrega um WAV mono contendo frames consecutivos de ciclo único. `Pos A` e `Pos B` percorrem suas tabelas de forma independente de `00` a `FF`. A engine interpola linearmente dentro de cada frame e entre frames adjacentes. `Mix` faz o crossfade entre os osciladores, e `Detune` desloca o oscilador B.

## 6. Modulação e gravação de movimento

Cada instrumento pode ter até 4 slots de modulação. Instrumentos de áudio podem ter como destino **Instrument Pan** ou **Track Pan**; ambos usam `00` para a esquerda, `80` para o centro e `FF` para a direita.

Os tipos de modulação disponíveis são:

- ADSR - envelope clássico de attack-decay-sustain-release
- AHD - envelope de attack-hold-decay
- LFO - oscilador de baixa frequência com várias formas
- SLFO - LFO com multiplicador de período para ciclos lentos
- FLFO - LFO em taxa de áudio que pode chegar à faixa de kHz; o FLFO pode pesar na CPU
- SLIN - controle linear por joystick
- SRAT - controle por joystick baseado em taxa

A lista de destinos de modulação depende do tipo de instrumento.

Para um destino `Cutoff`, Amount é exponencial com resposta quadrática: 25% alcança 4,5 semitons, 50% alcança 18 semitons, 75% alcança 40,5 semitons e 100% alcança seis oitavas. Isso mantém o controle fino em quantidades baixas e ainda permite varreduras profundas.

Quando um envelope ADSR ou AHD tem Volume como destino, ele se torna o envelope de volume do instrumento. Um LFO com Volume como destino, em vez disso, desloca o volume de saída atual.

A quantidade de modulação pode ser positiva ou negativa. Sua faixa depende do destino. Se a faixa completa do destino for `127` ou menos, como na maioria dos parâmetros, a quantidade define uma faixa absoluta. Destinos mais amplos, como a altura, usam uma quantidade escalonada.

Todos os parâmetros de duração, incluindo attack, hold, decay e período, são medidos em ticks.

Formas do LFO:

- Tri - onda triangular bipolar
- Sin - onda senoidal bipolar
- UniTri - onda triangular unipolar
- UniSin - onda senoidal unipolar
- RampDn - rampa linear descendente (onda dente de serra)
- RampUp - rampa linear ascendente (onda dente de serra)
- ExpDn - rampa exponencial descendente (dente de serra exponencial)
- ExpUp - rampa exponencial ascendente (dente de serra exponencial)
- Square - onda quadrada
- Random - onda aleatória de sample-and-hold
- Wavetable - lê uma das wavetables AY de 32 passos do projeto. Os valores `00` a `0F` mapeiam de `-100%` a `+100%` sem interpolação.

Tipos de disparo do LFO:

- Free - reinicia o LFO somente quando o instrumento muda
- Retrig - reinicia o LFO a cada nota
- Phrase - reinicia LFO e SLFO quando a reprodução entra em um novo phrase
- Chain - reinicia LFO e SLFO quando a reprodução entra em um novo chain
- Hold - para após 1 ciclo e mantém o último valor
- Once - para após 1 ciclo e volta a zero

### Gravação de movimento (Motion recording)

- **STICK LIVE** (`L1` por padrão) aplica a modulação dos sticks sem alterar o phrase. Em **Settings > Stick live mode**, `HOLD` (o padrão) o ativa enquanto pressionado; `TOGGLE` o ativa em um pressionamento e o desativa no seguinte; `FREE` o deixa permanentemente ativado. Soltar o botão em `TOGGLE` o deixa ativado.
- Durante a reprodução, segure **MOTION RECORD** (`L2`) para aplicar a modulação dos sticks e gravar os destinos alterados como valores absolutos de FX.
- Segure **MOTION ERASE** (`R2`) para remover da linha atual os FX de destino correspondentes.

O `~` no canto inferior direito indica Stick live ativo. Record (`*`, ou `!` em caso de estouro) e Erase (`x`) têm prioridade no indicador e permanecem momentâneos nos dois modos. Ambos ativam o processamento dos sticks ao vivo enquanto pressionados; soltá-los deixa intacto um alternador live ativado separadamente.

O modo é salvo com as configurações do aplicativo, mas o alternador live sempre começa desligado na inicialização. Mudar de modo limpa o alternador: `HOLD` acompanha qualquer botão Stick live atualmente pressionado, `TOGGLE` espera um novo pressionamento e `FREE` fica ativo imediatamente.

A gravação de movimento escreve FX de trilha no phrase que está tocando no momento. Primeiro atualiza os FX correspondentes e depois usa slots vazios da direita para a esquerda. Nunca sobrescreve um FX diferente. Se os 3 slots estiverem cheios, aquele movimento não é gravado no passo. Um `!` no canto inferior direito significa que mais destinos mudaram do que as 3 colunas de FX puderam comportar.

A gravação de movimento suporta destinos de Braids, Plaits, Sampler, 2xSCWF, BYOWTBL e Bogie, incluindo macros ativos do Bogie e controles de filtro quando aplicáveis.

## 7. Tables

As tabelas são uma ferramenta central de design de som em trackers. Em termos de Vortex Tracker, elas combinam instrumentos e ornamentos, mas podem fazer muito mais. Se você conhece o LSDj ou o NerdSEQ, a ideia já deve parecer familiar.

A coluna Pitch aceita valores de altura relativos (`~`) ou absolutos (`=`) em semitons. A coluna Volume vai de `00` a `0F`, se aplica a toda engine por cima de seu envelope e pode ser usada como gate: alterne `0F` e `00` e depois faça o loop com `HOP 00`. As 4 faixas de FX correspondem em boa parte às de um phrase, embora alguns FX se comportem de forma diferente nas tabelas.

Colocar um FX `TIC` na última linha da tabela define a velocidade dessa coluna e sobrepõe a velocidade padrão de tabela do instrumento. Cada coluna de FX pode rodar em uma velocidade de ticks diferente. As colunas Pitch e Volume seguem a velocidade da 1ª coluna de FX.

Uma tabela de 16 linhas pode parecer curta ao lado de um longo instrumento do Vortex Tracker, mas o FX `HOP` pode criar loops condicionais como "repita estas linhas 5 vezes", além de loops aninhados. Com loops e velocidades independentes das colunas de FX, essas 16 linhas podem ir surpreendentemente longe.

### Modo retrigger

O campo `Retrig` no topo da tela Table seleciona quando uma tabela já ativa reinicia. `Inst` é o modo padrão e compatível: uma linha de phrase contendo um instrumento seleciona a tabela padrão desse instrumento e a inicia na linha `00`. `Phrase` reinicia uma tabela ativa quando a reprodução entra em um phrase, `Chain` quando entra em um chain, e `Free` a deixa rodando até a trilha parar ou um comando explícito a substituir.

Um modo estrutural nunca volta no tempo. Se uma trilha não tem tabela de instrumento ativa, a primeira linha de phrase que introduz um instrumento ativa sua tabela exatamente nessa linha, mesmo que seja a linha `4`, `8` ou `F` de um phrase. Números de instrumento posteriores não substituem uma tabela `Phrase`, `Chain` ou `Free` ativa. Isso torna as tabelas úteis como automação tipo LFO sem simular em silêncio passos anteriores do phrase.

`TBL`, `TBX` e `RET` são sempre ações explícitas: selecionam ou reiniciam imediatamente, seja qual for o modo. Em um limite de Phrase ou Chain, a reinicialização ocorre antes de a nova linha do phrase ser lida, então um `TBL` ou `TBX` nessa linha tem a palavra final. Os FX da linha `00` da tabela, portanto, voltam a rodar normalmente; os FX cumulativos mantêm seu acúmulo habitual.

As tabelas `00-7F` são reservadas para as tabelas padrão dos instrumentos. As tabelas `80-FE` são destinadas a tabelas auxiliares iniciadas pelo efeito `TBX`. A faixa inferior também funciona para tabelas auxiliares, mas usá-la assim pode causar conflitos inesperados e confusão.

### Copiar FX entre Phrase e Table

Selecione apenas colunas de nome/valor de FX, depois copie ou recorte-as de um Phrase e cole-as em qualquer faixa de FX de uma Table, ou faça o inverso. A cópia mantém exatamente as células selecionadas, então uma seleção só de comandos ou só de valores continua parcial. A colagem começa na célula de FX atual e é cortada na linha `F` e na última faixa de FX do destino. Notas, números de instrumento, altura e volume nunca passam entre essas duas telas.

### Controles

- **EDIT + [UP/DOWN]** em uma coluna de nome de FX: abre a tela de seleção de FX
- **OPT + [DIRECTION]**: move entre tabelas

## 8. Outras telas

### Instrument Pool

O Instrument Pool mostra todos os instrumentos do projeto e permite reordená-los. Durante a reprodução, também mostra quais instrumentos estão ativos no momento.

#### Controles

- **EDIT**: edita o instrumento e pula para a tela Instrument
- **SHIFT + OPT**: copia o instrumento
- **SHIFT + EDIT**: cola o instrumento
- **EDIT + [UP/DOWN]**: reordena os instrumentos
- **EDIT + PLAY**: pré-escuta o instrumento

### AY Wavetable

Wavetable é um tipo de oscilador de software do AY Plus. Um projeto pode conter até 256 wavetables, cada uma com 32 passos. Todos os instrumentos de wavetable compartilham o mesmo conjunto de ondas. É nesta tela que você as edita.

Os níveis de volume do AY são não lineares, então a forma de onda da visão Standard é desenhada para corresponder aos níveis de saída reais. Toque duas vezes em **OPT** para alternar entre a visão Standard e a **visão LFO** persistida, usando o mesmo atalho do Song/Live Mode: a visão LFO desenha os passos brutos `0`-`15` de forma linear, com a linha central entre `7` e `8` como ponto zero da modulação.

A tela tem 2 linhas lógicas. A linha superior contém os botões **Load** e **Save**, enquanto a 2ª contém o editor de wavetable.

#### Controles

- **OPT + [LEFT/RIGHT]**: percorre a lista de wavetables
- **OPT + [UP/DOWN]**: percorre a lista de 16 em 16 ondas
- **SHIFT + OPT**: copia a wavetable
- **SHIFT + EDIT**: cola a wavetable
- **OPT + PLAY**: alterna entre a visão Standard e a LFO

## 9. Tracker FX

Os FX de trilha forçam valores do sequenciador ou da engine de som em passos individuais. Outras grooveboxes costumam chamá-los de "parameter locks". Cada linha de Phrase tem 3 slots de FX.

Cada FX tem um comando de 3 letras e um valor hexadecimal. O painel de ajuda do aplicativo dá uma breve descrição do comando selecionado.

### Automação de escala

`SCL XY` muda a escala e a fundamental globais da reprodução sem alterar as notas armazenadas nos phrases. `X` seleciona uma escala (`0` Chromatic, `1` Major, `2` Minor, até `C` Custom); `Y` seleciona a fundamental (`0` C até `B` B). O comando se aplica apenas a FX de phrase, não a tabelas. Se várias trilhas emitirem `SCL` no mesmo tick, vence a trilha de número mais baixo.

`SCL` pertence ao modo Quantizer. Com **Note Lock** ativo, o comando não é oferecido na lista de FX, e qualquer `SCL` já escrito em phrases é ignorado enquanto o modo estiver ligado.

### Acordes

`CRD XY` transforma a nota de sua própria linha de Phrase em um acorde. `Y` seleciona: `0` Major, `1` Minor, `2` Dim, `3` Aug, `4` Sus2, `5` Sus4, `6` Power, `7` Maj7, `8` Min7, `9` Dom7, `A` Min7b5, `B` Dim7, `C` Add9, `D` MinAdd9, `E` Maj9 ou `F` Min9. `X` seleciona uma das 16 disposições de vozes: `0-3` são um ciclo ascendente de inversões fechadas; `4-7` acrescentam drop-2; `8-B` acrescentam drop-3; e `C-F` combinam os dois drops. O ciclo continua por uma oitava extra para tríades e power chords, de modo que todo valor é distinto; valores maiores ficam progressivamente mais abertos e menos convencionais. Por exemplo, `CRD 4Y` é drop-2 em posição fundamental e `CRD DY` é a primeira inversão com os dois drops. No limite inferior ou superior da tabela de alturas, as vozes são limitadas à sua faixa. O comando não é persistente: escreva-o em todas as linhas com acorde. Cada nota gerada é quantizada de forma independente quando Scale está ativa, tanto no modo Quantizer quanto no Note Lock. Está disponível apenas para engines de software; os instrumentos AY o ignoram e continuam monofônicos.

### Sequencer FX

| FX | Valor | Comportamento detalhado |
|---|---|---|
| `ARP` | `XY` | Arpeja a nota base, passos `+X` e `+Y`. `37` produz um padrão de acorde menor. |
| `ARC` | `XY` | `X` seleciona a direção do arpejo ou o modo de faixa; `Y` é sua velocidade em ticks. |
| `PVB` | `XY` | Vibrato de altura: `X` é a velocidade e `Y` a profundidade. No modo Linear, a profundidade usa passos de `10-cent`. |
| `PBN` | `XX` com sinal | Soma `XX` unidades de altura a cada linha de phrase/tabela; `FF` significa `-1`. Use `00` para parar. |
| `PSL` | `XX` ticks | Desliza da altura anterior para a nova nota em `XX` ticks. |
| `PIT` | `XX` com sinal | Deslocamento relativo acumulado em passos da tabela de alturas. |
| `FIN` | `XX` com sinal | Deslocamento fino acumulado em cents com Linear pitch, em unidades de período nos demais casos. |
| `PRD` | `XX` com sinal | Deslocamento relativo acumulado do período do oscilador. |
| `VOL` | `XX` com sinal | Deslocamento relativo acumulado de volume (`FF` é -1, não volume máximo). |
| `VSL` | `XX` com sinal | Soma `XX` ao volume a cada linha de phrase/tabela. Use `00` para parar. |
| `PAN` | `00-FF` | Panorama absoluto do instrumento: esquerda, centro (`80`), direita. |
| `TPN` | `00-FF` | Panorama absoluto da trilha do mixer: esquerda, centro (`80`), direita. |
| `RET` | `XY` | Redispara a cada `Y` ticks; `X` aplica uma mudança de volume. `Y=0` interrompe o redisparo. |
| `DEL` | `XX` ticks | Atrasa o note-on. Um atraso maior que o passo atual do groove pula a nota. |
| `OFF` | `XX` ticks | Envia o note-off após `XX` ticks e entra em um estágio de release do ADSR. |
| `KIL` | `XX` ticks | Mata a voz de forma abrupta após `XX` ticks sem executar o release do ADSR. |
| `TIC` | `XX` ticks | Define os ticks da tabela por linha. Em uma tabela, muda a velocidade dessa coluna de FX. |
| `TBL` | `00-FE`, `FF` desliga | Substitui e reinicia imediatamente a tabela do instrumento; `FF` a para. |
| `TBX` | `00-FE`, `FF` desliga | Inicia ou substitui imediatamente uma tabela auxiliar; `FF` a para. |
| `THO` | linha `XX` | Salta todas as colunas da tabela do instrumento para a linha `XX`. |
| `TXH` | linha `XX` | Salta todas as colunas da tabela auxiliar para a linha `XX`; não é usado de dentro de uma tabela. |
| `GRV` | groove `XX` | Seleciona um groove para a trilha atual. |
| `GGR` | groove `XX` | Seleciona um groove para todas as trilhas. |
| `HOP` | `XY` | Salta para a linha `Y`, `X` vezes; `X=0` repete para sempre. O HOP de tabela afeta sua própria coluna. |
| `SNG` | `XX` com sinal | Move a reprodução em `XX` linhas de Song. |
| `PRO` | `00-64` | Avalia uma probabilidade absoluta de disparo de `0%` a `100%`. |
| `MOD` | `AB` | Dispara na passagem `A` de um ciclo de `B` passagens. |
| `SPD` | `XX` com sinal | Seleciona uma razão de clock persistente por trilha; veja a tabela de velocidades abaixo. |
| `SLE` | `00-FF` ticks | Define o tempo de glide persistente por trilha para FX contínuos da engine. `00` é imediato. A configuração volta a `00` quando a reprodução para. |

### Audio FX sends

| FX | Valor | Comportamento detalhado |
|---|---|---|
| `RSN` | `00-FF` | Define o send de reverb desta trilha até o próximo disparo de nota. |
| `DSN` | `00-FF` | Define o send de delay desta trilha até o próximo disparo de nota. |

### ADSR / Trigger FX

| FX | Valor | Comportamento detalhado |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | Sobrepõem attack, decay, sustain, release ou a forma do envelope. |
| `TDC` / `TCL` | `00-FF` | Sobrepõem Decay ou Color do modo Trigger no Plaits e no Plaits-Alt. |

#### Condições

Inspirado nas "trig conditions" suecas, o tracker suporta probabilidades de disparo e condições de módulo.

| FX | Valor | Significado |
|---|---|---|
| `PRO` | `00-64` | Probabilidade absoluta de disparo de `0%` a `100%` |
| `MOD` | `AB` | Dispara na iteração `A` de `B`; por exemplo `12`, `22`, `14`, `34` |

Os contadores de `MOD` são locais à trilha e ao phrase. Combinações inválidas, como `A` maior que `B`, não disparam.

`MOD34` dispara na 3ª passagem de cada ciclo de 4 passagens. `MOD1F` dispara na 1ª passagem de cada ciclo de 16 passagens.

#### Velocidade de reprodução

Como o NerdSEQ, o ChooChooTracker suporta uma velocidade de reprodução independente para cada trilha.

`SPD` lê seu valor como um byte com sinal. `00` é a velocidade normal, valores positivos tornam a trilha mais rápida e valores negativos a tornam mais lenta. A configuração permanece ativa até que outro comando `SPD` a altere.

| Valor | Velocidade |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

Projetos antigos sem suporte a `SPD` com sinal mantêm seu mapeamento original `00-10`, então continuam tocando como foram salvos.

Multiplicadores muito altos podem exceder a resolução do agendador de ticks do tracker e não foram testados em hardware.

### Modulation FX

Os FX de phrase e de tabela podem alterar um slot de modulação sem editar o instrumento:

| Padrão de FX | Significado |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | Deslocamento relativo de Amount para os slots de modulação 1-4 |
| `M11`, `M12`, `M13`, `M14` | Deslocamentos relativos de P1-P4 para o slot de modulação 1 |
| `M21`, `M22`, `M23`, `M24` | Deslocamentos relativos de P1-P4 para o slot de modulação 2 |
| `M31`, `M32`, `M33`, `M34` | Deslocamentos relativos de P1-P4 para o slot de modulação 3 |
| `M41`, `M42`, `M43`, `M44` | Deslocamentos relativos de P1-P4 para o slot de modulação 4 |

O valor é interpretado como uma mudança relativa de `8-bit` com sinal (`01` soma `1`, `FF` subtrai `1`). Comandos repetidos se acumulam. Os valores efetivos são limitados à sua faixa válida.

`P5` não é exposto como FX de phrase ou de tabela. Está disponível como o destino de modulação `M1–M4 Wavetable`, de modo que um modulador pode varrer o índice de AYWavetable de outro.

| Tipo de modulação | P1 | P2 | P3 | P4 | P5 quando Shape é Wavetable |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | Não usado |
| AHD | Attack | Hold | Decay | Não usado | Não usado |
| LFO | Shape | Trigger mode | Period | Não usado | Índice da wavetable AY |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | Índice da wavetable AY |
| FLFO | Shape | Trigger mode | Frequency (`1 Hz` a `20 kHz`) | Não usado | Índice da wavetable AY |

### Braids FX

| FX | Valor | Significado |
|---|---|---|
| `BMD` | `00-2E` | Modelo do Braids |
| `BTM` | `00-FF` | Timbre normalizado absoluto |
| `BCL` | `00-FF` | Color normalizado absoluto |
| `BCF` | `00-FF` | Cutoff exponencial, de `20 Hz` a `20 kHz` |
| `BRS` | `00-FF` | Ressonância exponencial do filtro |

### aChChid FX

| FX | Valor | Significado |
|---|---|---|
| `ASL` | `00-FF` | Desliza até a nota sem redisparar. `00` é `60 ms`; valores maiores prolongam o glide. |
| `ADC` | `00-FF` | Sobreposição do decay: de `200 ms` a `2 s`. |
| `AAC` | `00-FF` | Quantidade de accent: de nenhuma ao máximo. |
| `ATM` | `00-FF` | Sobreposição do Timbre do Braids; ativa apenas no modo de onda do Braids. |
| `ACL` | `00-FF` | Sobreposição do Color do Braids; ativa apenas no modo de onda do Braids. |
| `ACF` | `00-FF` | Cutoff exponencial do filtro 303, de `20 Hz` a `20 kHz`. |
| `ARS` | `00-FF` | Ressonância do filtro 303, de nenhuma ao máximo. |
| `AEM` | `00-FF` | Modulação por envelope do filtro 303, de nenhuma ao máximo. |

### Bogie FX

| FX | Valor | Significado |
|---|---|---|
| `DMD` | `00-0B` | Seleciona o modelo do Bogie até o próximo disparo. |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | Sobrepõem Decay, Tone, Sweep, Noise, FM ou Drive até o próximo disparo. Os seis macros estão disponíveis em todos os modelos. |
| `DCF` | `00-FF` | Cutoff do filtro, mapeado de forma logarítmica. |
| `DRS` | `00-FF` | Ressonância do filtro. |

### Sintered FX

| FX | Valor | Significado |
|---|---|---|
| `SMD` | `00-05` | Seleciona o modelo do Sintered até o próximo disparo. |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | Sobrepõem Decay, Mod, os dois primeiros macros do modelo, Motion ou o último macro do modelo até o próximo disparo. |
| `SCF`, `SRS` | `00-FF` | Cutoff e ressonância do filtro. |

### MIDI Out FX

| FX | Valor | Significado |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | Envia um MIDI CC, reescalado para `0-127`. O número de CC para o qual cada slot envia é definido por instrumento (`MC1 number`-`MC4 number` na tela do instrumento), não pelo próprio FX - um slot sem número definido fica inerte. |

### Plaits FX

| FX | Valor | Significado |
|---|---|---|
| `PMD` | `00-17` | Engine do Plaits |
| `PHA` | `00-FF` | Harmonics normalizado absoluto |
| `PTM` | `00-FF` | Timbre normalizado absoluto |
| `PMO` | `00-FF` | Morph normalizado absoluto |
| `PAX` | `00-FF` | Mistura Main/Aux: `00` Main, `FF` Aux |
| `PCF` | `00-FF` | Cutoff exponencial, de `20 Hz` a `20 kHz` |
| `PRS` | `00-FF` | Ressonância exponencial do filtro |

### Sample FX

| FX | Valor | Significado |
|---|---|---|
| `SPL` | `00-03` | Modo de reprodução: `00` Forward, `01` Reverse, `02` Loop, `03` Ping-Pong |
| `SLI` | `00-FF` | Toca o slice numerado (`01` = primeiro slice) independentemente da nota; `00` mantém o mapeamento normal de notas. Exige o modo slice ativado no instrumento |
| `SPT` | `XX` com sinal | Transposição do sample em semitons |
| `SST` | `00-FF` | Início de reprodução normalizado |
| `STA` | `00-FF` | Alias de `SST` (início de reprodução normalizado) |
| `SEN` | `00-FF` | Fim de reprodução normalizado |
| `SVL` | `00-FF` | Volume absoluto do sample |
| `SCF` | `00-FF` | Cutoff exponencial, de `20 Hz` a `20 kHz` |
| `SRS` | `00-FF` | Ressonância exponencial do filtro |
| `SSP` | `00-FF` | Velocidade do sample, mapeada de `0%` a `500%` |

### FX AY compartilhados pelos instrumentos AY

| FX | Valor | Comportamento detalhado |
|---|---|---|
| `AYM` | `XY` | `X` é a forma do envelope; `Y` seleciona desligado/tom/ruído/tom+ruído (`0/1/2/3`). |
| `NOI` | `XX` com sinal | Deslocamento relativo acumulado do período de ruído. |
| `NOA` | `00-1F`, `FF` | Período de ruído absoluto; `FF` cede a prioridade do período de ruído às trilhas anteriores. |
| `ERT` | qualquer | Redispara a forma de envelope de hardware atual. |
| `EAU` | `XY` | Razão automática de envelope `X:Y`; `X=0` a desliga. |

### FX exclusivos do AY Classic

| FX | Valor | Comportamento detalhado |
|---|---|---|
| `EVB` | `XY` | Vibrato do período do envelope: `X` velocidade, `Y` profundidade. |
| `EBN` | `XX` com sinal | Soma `XX` ao período do envelope a cada linha de phrase/tabela. |
| `ESL` | `XX` ticks | Desliza do período de envelope anterior para o novo valor. |
| `ENT` | nota `XX` | Define o período do envelope a partir da nota da tabela de alturas mostrada pela interface. |
| `EPT` | `XX` com sinal | Deslocamento relativo acumulado do período do envelope. |
| `EPL` | byte `XX` | Define o byte baixo do período do envelope. |
| `EPH` | byte `XX` | Define o byte alto do período do envelope. |

### AY Plus FX

| FX | Valor | Comportamento detalhado |
|---|---|---|
| `TNN` | nota `XX` | Define o oscilador de tom para uma nota da tabela de alturas, ignorando a nota da linha. |
| `TNP` / `TNF` | `XX` com sinal | Deslocamento acumulado de passo de altura / ajuste fino do tom. |
| `TRT` | qualquer | Redispara a fase do oscilador de tom onde houver suporte. |
| `ENN` | nota `XX` | Define o oscilador de envelope para uma nota da tabela de alturas. |
| `ENP` / `ENF` | `XX` com sinal | Deslocamento acumulado de passo de altura / ajuste fino do envelope. |
| `SFT` | `00-07` | Tipo de software: None, Pulse, Sync Tone, Sync Env, Wavetable, Tone FM, Env FM, Sample. |
| `SFN` | nota `XX` | Define o oscilador de software para uma nota da tabela de alturas. |
| `SFP` / `SFF` | `XX` com sinal | Deslocamento acumulado de passo de altura / ajuste fino do oscilador de software. |
| `SRT` | qualquer | Reinicia a fase do oscilador de software. |
| `SFM` | `XX` com sinal | Deslocamento acumulado da profundidade de FM. |
| `PWM` | `XX` com sinal | Deslocamento acumulado da largura de pulso. |
| `SPL` | `XX` com sinal | Deslocamento acumulado do nível baixo do pulso. |
| `SWT` | `XX` com sinal | Deslocamento acumulado do índice de wavetable. |

### AY Sample FX

O AY Sample aceita os comandos AY compartilhados mais `TNN`, `TNP`, `TNF`, `TRT`, `SFN`, `SFP` e `SFF`, como descrito acima.

| FX | Valor | Comportamento detalhado |
|---|---|---|
| `SMS` | `00-FF` | Define o início da reprodução do AY Sample legado como `XX x 64` amostras de origem. |

Os FX de parâmetros de engine se aplicam ao tipo de instrumento correspondente e são reiniciados no próximo disparo de nota, a menos que se diga o contrário. `SPD` é persistente de propósito.

O seletor de FX filtra esta referência para os comandos comuns mais o grupo suportado pelo instrumento ativo. O painel de ajuda contextual fornece detalhes dos valores durante a edição.

## 10. Mixer, Reverb e Delay

O Mixer é a tela principal mais à esquerda. Ele exibe a carga da CPU e avisos de clipping. Um `!` vermelho na coluna **CLIP** marca a trilha cujo sinal seco empurrou a mix além da faixa segura.

Use-o para equilibrar as 8 trilhas, dar forma a cada trilha com o Tilt EQ e enviar áudio ao reverb ou ao delay:

| Campo | Significado |
|---|---|
| LVL | Nível da trilha após a engine, `000-100` |
| REV | Send para o reverb Clouds compartilhado, `000-100` |
| DLY | Send para o delay ping-pong compartilhado, `000-100` |
| TLT | Tilt EQ por trilha, `00-FF`; `80` é neutro, valores baixos favorecem os graves e valores altos favorecem os agudos. Valores baixos introduzem uma leve saturação. |
| M | `*` silencia esta trilha |
| S | `*` coloca esta trilha em solo |

O mixer é baseado em trilhas, não em instrumentos. Se uma trilha troca de instrumento, seu nível, PAN, Tilt e sends continuam ligados à trilha. Os instrumentos também têm seus próprios **Vol** e **Pan** de `00-FF` na linha superior da tela Instrument. O PAN do instrumento é aplicado primeiro, depois o PAN da trilha. Ambos preservam uma imagem estéreo existente no centro; eles atenuam progressivamente o lado oposto quando movidos para a esquerda ou a direita.

### Master FX

A entrada **MASTER FX** abre um submenu de Insert FX de dois slots para a mix
completa. Seus efeitos rodam depois dos retornos de reverb e delay, antes do volume
master. Selecione um módulo, edite seus parâmetros ou coloque-o em bypass exatamente como em um
insert de trilha; pressione **OPT** para voltar ao Mixer. As configurações do Master FX são salvas
com o projeto e não são automatizadas por phrase, tabela ou modulação.

### Clouds Reverb

Use **SELECT + [UP]** a partir do Mixer para abrir a tela de configurações do reverb.

- **Return**: nível do reverb molhado na mix master.
- **Time**: controle de decay/tempo, `00-FF`.
- **Damping**: absorção de altas frequências, `00-FF`.
- **Filter**: filtro passa-baixa aplicado antes do reverb.

Esta é a seção de reverb do Clouds, não o processador granular completo do Clouds.

### Ping-pong Delay

Use **SELECT + [DOWN]** a partir do Mixer para abrir o Delay e depois **SELECT + [UP]** para voltar. O Reverb funciona na direção oposta: **SELECT + [UP]** o abre e **SELECT + [DOWN]** volta ao Mixer.

- **Return**: nível do delay molhado.
- **To Reverb**: quantidade do sinal molhado do delay enviada ao reverb compartilhado, `0-100%`.
- **Ticks**: tempo de delay em ticks do tracker (`9 ticks` = `1 beat` por padrão).
- **Feedback**: quantidade de feedback cruzado, limitada a `95%`.
- **Filter**: filtro passa-baixa no caminho do sinal atrasado.

A 1ª repetição segue a entrada estéreo. O feedback posterior cruza entre os canais esquerdo e direito.

## 11. Tela Project

A tela Project oferece os comandos **Load**, **Save**, **New**, **Export**, **Manage** e **Scale**, junto com metadados de nome de arquivo, título e autor.

### Local de exportação

As exportações, stems e bounces são gravados em uma pasta por projeto dentro do diretório de samples já existente do aplicativo:

- **Padrão**: `<app folder>/samples/Exports/<project name>/`. Enquanto o projeto não tem nome, a pasta é `current-project`.
- Na versão web, o padrão é `/user/samples/Exports/<project name>/`.
- No Android, a pasta de samples fica dentro do espaço de trabalho privado do aplicativo.

A linha **Folder** na tela EXPORT mostra o destino ativo:

- Toque em **EDIT** para escolher uma pasta personalizada com o navegador de pastas. A pasta escolhida é usada exatamente como selecionada (nenhuma subpasta do projeto é adicionada) e fica guardada nas configurações.
- **EDIT + OPT** (clear) volta ao local padrão.

Quando um projeto com nome é salvo com um novo nome, a pasta de exportação padrão é renomeada para corresponder, mantendo os arquivos exportados antes junto com o projeto. A renomeação não se aplica a uma pasta personalizada, e a pasta não é renomeada pelo autosave. Se já existir uma pasta com o nome de destino, as duas pastas são mantidas. Se o tracker perdeu a pista da pasta (por exemplo, depois de uma reinicialização), salvar um projeto com nome renomeia em vez disso a pasta `current-project` existente. Os instrumentos continuam funcionando depois de uma renomeação: os caminhos de samples carregados que apontam para dentro da pasta renomeada são atualizados automaticamente.

**Load** também aceita arquivos `.mid`/`.midi`, importados como um novo projeto: as notas são agrupadas por canal MIDI (um canal por trilha, até o número de trilhas), quantizadas em 4 linhas por beat e colocadas em um único instrumento AY padrão - os números de programa MIDI não têm equivalente em chiptune, então escolha instrumentos reais depois. Somente o primeiro andamento do arquivo é usado (uma taxa de ticks global, sem mudanças de andamento por seção). A linha **MIDI** da tela Export faz o inverso: grava as notas, o volume e o andamento/groove do arranjo atual como um Standard MIDI File (uma trilha MIDI por trilha do tracker); os instrumentos [MIDI Out](#midi-out) e outros FX por linha além do volume e do groove global não têm equivalente em MIDI e não são traduzidos.

**Load** também aceita músicas do Dirtywave M8 (`.m8s`, firmware 2.x-4.x), importadas como um novo projeto. Só vêm a estrutura e as notas (linhas de song, chains com sua transposição, phrases com nota, velocity e número de instrumento, note-offs, andamento e nome da música); as notas mantêm sua altura real (o M8 nomeia as oitavas duas acima: seu `C-6` é `C-4` aqui). Cada instrumento usado por um phrase é importado com o nome do instrumento do M8. Os instrumentos **MACROSYN** do M8 viram instrumentos Braids (as 47 formas são os modelos do Braids, com os mesmos números; timbre, color, tipo de filtro, cutoff, ressonância e pan são levados, e a curva de cutoff é aproximada). Os instrumentos **SAMPLER** viram instrumentos Sample: o `.m8s` só guarda o caminho do WAV no cartão do M8, então o arquivo é procurado na pasta do `.m8s` e em suas pastas superiores (uma cópia da estrutura do cartão funciona) e na sua pasta de samples atual (`Samples` ou `samples` funcionam); se não for encontrado, o instrumento mantém o caminho do M8 e fica em silêncio. Os samples importados recebem +24 semitons em sua configuração de Pitch, pois o M8 toca um sample em sua altura original duas oitavas abaixo deste aplicativo. **FMSYNTH** vira um instrumento Genesis FM (aproximação grosseira), **WAVSYNTH** e **HYPERSYNTH** viram aChChid. **MIDI OUT** vira um instrumento MIDI Out (canal, bank, programa e os quatro primeiros números de CC). Outros tipos (externos) viram um instrumento AY padrão - escolha sons reais depois. FX e tabelas são ignorados.

No desktop, a linha **M8S** da tela Export faz o inverso, de forma limitada: toque nela e escolha um `.m8s` existente como **modelo** (lembrado até você sair, **EDIT + OPT** o esquece). Sua song, chains, phrases, andamento e título são substituídos, e todo o resto é mantido como está, exceto que os instrumentos Braids são gravados de volta como MacroSynth e os instrumentos Sample como Sampler (caminho `/Samples/<file name>`: copie o WAV para lá no M8 por conta própria), desde que o modelo tenha um instrumento desse tipo de onde copiar os demais parâmetros. Os FX não são exportados, e uma música que use o phrase `FF` ou acima não pode ser exportada (o M8 tem 255 phrases). O arquivo exportado ainda não foi testado em um M8 real. Veja [M8 songs](../m8s-format.md) para os detalhes.

No desktop, o key jazz permite digitar o nome do arquivo, o título e o autor diretamente no teclado em vez de usar o teclado virtual da tela - veja [Key jazz](#key-jazz-somente-desktop).

### Scale / Quantize

A tela Scale controla o mecanismo global de escalas 12-TET. A linha **Mode** escolhe entre dois modos:

- **Quantizer** (padrão): a entrada de phrases continua cromática; quando ativado, as notas são arredondadas para baixo imediatamente antes de chegar à engine de som, então mudar a fundamental ou a escala reharmoniza uma música sem editar seus phrases.
- **Note Lock**: as notas digitadas nos phrases são ajustadas à nota mais próxima da escala no momento em que são inseridas, então só notas da escala podem ser escritas. Entradas novas e reduções ajustam para baixo; `EDIT + RIGHT` / `EDIT + UP` elevam a nota para a próxima nota da escala acima. A entrada sempre segue a escala do projeto nesta tela: mudar a fundamental ou a escala afeta de imediato as notas recém-inseridas nas trilhas habilitadas, enquanto as notas já armazenadas permanecem intocadas. O FX `SCL` fica indisponível neste modo e qualquer `SCL` já escrito é ignorado. A quantização de reprodução de notas simples é dispensada (a entrada já está travada); a quantização de acordes via `CRD` continua valendo.

Com o cursor em **Mode**, a barra de status mantém visível uma descrição fixa do modo ativo: `Quantizes notes on playback` para **Quantizer**, `Snaps sequencer to scale` para **Note Lock**. Alternar o modo troca o texto de imediato; qualquer mensagem de ação o substitui até expirar.

Os dois modos compartilham a mesma fundamental, escala, edição de notas **Custom** e caixas de seleção de trilhas. Cada uma das oito caixas de trilha decide a quais trilhas o modo se aplica. O mecanismo de escalas é dispensado para tabelas de altura sem 12 notas e para instrumentos Sampler cujo modo **Slice** seja EQUAL, AUTO ou LAZY (suas notas selecionam slices cromaticamente a partir de C-0); a entrada MIDI não faz parte desta versão.

- **Linear pitch** seleciona o modo da tabela de alturas. **Off** é o padrão e a configuração validada em hardware para o correto acompanhamento de oitavas no AY, Braids e Plaits.
- **Tick rate** define o tempo do tracker e exibe o BPM correspondente (`tick rate x 60 / 24`).
- O ChooChooTracker salva projetos como `.cct`. Esse formato não é compatível com o ChipNomad.

Use **Save** antes de mudar tipos de instrumento ou carregar outro projeto.

## 12. Settings

- **Repeat delay / speed** ajustam a repetição de botões mantidos pressionados.
- **Stick live mode** seleciona `HOLD`, `TOGGLE` ou `FREE` para o botão Stick live já mapeado.
- **MIDI** abre o submenu [MIDI](#14-midi): seleção de dispositivo e o mapeamento de canais MIDI In para instrumentos.
- **Synths** contém **AY Quality**, **Sample dithering** e as configurações globais **Braids BITS / DRFT / SIGN**.
- **Mixer** contém **Mix volume** e o **Tilt pivot** por projeto de `250-4000 Hz` (padrão `1 kHz`) usado por todos os controles Tilt do Mixer.
- **Graphics** contém **Edit color theme**, **Load font**, **Persistent waveform**, **Track visuals**, **Waveform FPS** e o renderizador ativo (`GPU` ou `Software`). **Waveform FPS** define a taxa de atualização do osciloscópio do instrumento e das visualizações do mixer, de `01` a `60`. Track visuals seleciona **Detailed** (a forma de onda do sintetizador e suas sobreposições) ou **Audio waveform** (a saída somada real dessa trilha) de forma independente para cada trilha; a forma de onda de áudio limpa de imediato qualquer sobreposição de ADSR anterior quando a reprodução começa. As fontes e temas do ChipNomad devem funcionar.
- **Key mapping** personaliza os controles.
- **Support report** grava um pequeno arquivo de texto de diagnóstico com a versão do aplicativo, a plataforma, as configurações de áudio e a carga da CPU, o estado de reprodução/estouro e um resumo compacto do projeto. Não inclui dados de samples nem o texto de título/autor do projeto. No Android e na Web, abre o fluxo normal de exportação/download; as versões desktop e para portáteis salvam `support-report.txt` na pasta gravável do aplicativo.
- **Quit ChooChooTracker** encerra de forma limpa.

**Graphics → Boot screen** seleciona **Original** (padrão) ou **Night**. Toque em EDIT
ou use EDIT + direção para alterar; a escolha é salva para a próxima tela
de título. O Night usa a cena ferroviária original com tons azul-acinzentados mais suaves,
uma Via Láctea procedural com sutis cintilações de estrelas e ocasionais estrelas cadentes.
O cenário e o trem originais mantêm seus loops de rolagem.

## 13. Desempenho e solução de problemas

### CPU

O custo de CPU depende das engines e efeitos ativos. Os modelos físicos do Plaits e o Clouds Reverb são mais pesados que as vozes básicas do AY. Verifique o desempenho no console de destino, especialmente com 8 vozes de Plaits e os dois sends ativos. Uma leitura de CPU próxima de `100%` pode causar estalos ou perda de prazos de áudio.

O indicador de CPU é uma medida suavizada do tempo de renderização de áudio. Ele não mede atrasos antes de o callback de áudio rodar nem na saída de áudio do dispositivo, então estalos podem ocorrer mesmo com uma leitura baixa.

No Android, a saída de áudio agora prefere o AAudio, para evitar o estalo intermitente observado com o OpenSL ES no Pixel 7a. Essa saída com buffer prioriza a reprodução estável e pode acrescentar alguma latência de resposta. A tela do tracker mantém seu layout 4:3 completo em proporções quadradas e outras incomuns; o espaço não usado é mostrado como bordas em vez de cortar colunas. Se os estalos persistirem, informe a música, o modelo do celular, a versão do Android e se você está usando o alto-falante, fones de ouvido ou Bluetooth.

Se você encontrar cliques, travamentos ou lentidão, use **Settings > Support report** e anexe o `support-report.txt` gerado ao relatório de bug. Se uma música específica provocar o problema, anexe também esse `.cct`.

### Sem som

Verifique o número do instrumento, o estado de mute ou solo da trilha, o LVL da trilha, o Mix volume do aplicativo e o envelope do instrumento. Para samples, verifique se o WAV original ainda existe no caminho salvo. No ArkOS, pressione **MENU + L3** para alternar o mute do sistema operacional. Se isso não ajudar, reinicie o hardware e reconecte sua interface de áudio.

### Os controles parecem errados

Ajuste Repeat delay e Repeat speed em Settings. Se um único pressionamento move duas vezes, verifique se só 1 controle físico está mapeado para essa direção e informe a tela e o atalho exatos.

## 14. MIDI

Desktop e PortMaster. Settings > **MIDI** abre este submenu: **MIDI In** / **MIDI Out** escolhem um dispositivo conectado, percorrendo as portas detectadas com `OFF` em ambas as pontas; as duas direções precisam de um dispositivo selecionado. A seleção de dispositivo vale por sessão: ela não é salva em settings.txt, já que a posição de uma porta na lista pode mudar entre reinicializações ou quando os dispositivos são conectados em outra ordem. O controle **PLAY** do aplicativo continua disponível nesta tela e inicia a música. No PortMaster, conecte um dispositivo USB MIDI antes de iniciar; ele precisa ser exposto pelo sistema operacional do portátil via ALSA.

- **Pré-escuta de som a partir de um teclado MIDI**: com um dispositivo MIDI In selecionado, tocar notas no teclado conectado pré-escuta um instrumento em qualquer tela, como o atalho **EDIT + PLAY** na tela [Instrument](#5-instruments). É somente pré-escuta - não insere notas na música.
- **Channel mapping**: abre uma lista dos 16 canais MIDI; atribua a cada um um instrumento (`OFF` por padrão) para que as notas recebidas nesse canal pré-escutem esse instrumento, independentemente de qual esteja selecionado na tela Instrument - por exemplo, canal `01` -> instrumento `05`. Um canal deixado em `OFF` volta ao instrumento atualmente selecionado, o comportamento original. Esse mapeamento é salvo em settings.txt.
- **MIDI CC mapping**: mapeia um controlador recebido para um parâmetro de instrumento, incluindo seu **Volume**, ou para controles globais. As linhas globais mostram `TRK 1`-`TRK 8` em vez de um instrumento e oferecem **Mute**, **Solo**, **Volume**, **Reverb send** e **Delay send** da trilha; **Song play/stop** não tem destino. Um novo mapeamento ignora o CC que o aprendeu/configurou e passa a valer no próximo movimento físico, então configurar mapeamentos durante a reprodução não pode zerar um patch. Mute e solo usam `0-63` = desligado, `64-127` = ligado.
- **Controlar um dispositivo MIDI externo**: dê a uma trilha o tipo de instrumento [MIDI Out](#midi-out) e defina seu Channel; disparar notas nessa trilha envia MIDI real ao dispositivo MIDI Out selecionado em vez de produzir som no ChooChooTracker.

## 15. Créditos e licenças

O ChooChooTracker é um fork do ChipNomad e mantém sua abordagem de licenciamento MIT. O código do Braids, Plaits, Plaits-Alt, Clouds DSP, do MME DSP derivado do Warps e da stmlib deriva dos lançamentos de código aberto da Mutable Instruments sob os avisos MIT aplicáveis. O Plaits-Alt vem do fork Plaits Lab de lylepmills/eurorack; os avisos de origem mantidos ali se aplicam. A engine aChChid usa o Open303 de Robin Schmidt, copyright 2009, sob a licença MIT. Bogie e Sintered são implementações nativas originais. A E/S MIDI usa o RtMidi de Gary P. Scavone, sob sua licença de estilo MIT. Veja os arquivos de licença empacotados para as atribuições exatas.

## Efeitos insert de trilha

Cada uma das oito trilhas tem dois slots em série, **TF1 → TF2**. A partir de Instrument,
use o gesto habitual Shift+Up para abrir MOD e, de novo, Shift+Up para **F: Insert
FX**. Shift+Down volta ao MOD. OPT+Left/Right seleciona a trilha (1-8), sem
mudar o seletor de instrumento. O cabeçalho do módulo abre a lista de escolha existente;
o bypass mantém o módulo, os valores configurados e a automação. Os parâmetros são
valores de byte: coluna esquerda 1-4, coluna direita 5-8. O texto de contexto mostra os nomes completos e
os valores decodificados. Projetos antigos carregam com os dois slots em OFF.

A exibição de navegação mantém a coluna F-M-I-P completa visível em todas as telas
de instrumento, inclusive Insert FX e o instrument pool, e destaca
a tela atual.

| Módulo | Parâmetros em ordem |
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

Os controles contínuos usam 00-FF, incluindo o zero válido e o máximo FF. Os controles
bipolares têm um neutro exato em 80. Os modos de distortion são 00 Density, 01 Drive,
02 Spiral, 03 Mojo, 04 Dyno. As razões do compressor 00-07 são 1,5, 2, 3, 4, 6, 8, 16,
20:1; as fontes do detector 00-02 são entrada estéreo, esquerda, direita. O Detector filter 80 é
neutro, abaixo é passa-baixa, acima é passa-alta. Não há sidechain externo
nem entre trilhas. Qualquer slot pode conter qualquer módulo; módulos repetidos são independentes.
Chorus, Flanger, Phaser e Rotary usam padrões de modulação suaves. O Rotary Speed
usa toda a faixa **00-FF**: **00 = 0,01 Hz** (uma rotação a cada 100 segundos),
**FF = 25 Hz** (o antigo máximo do Chaos). O espaçamento exponencial dá um controle mais
fino sobre movimentos lentos do LFO. Novos inserts Rotary vêm por padrão em **8F**, cerca de
0,8 Hz. Isso substitui os quatro valores discretos de velocidade; as configurações existentes do Rotary
e a automação de velocidade F11/F21 precisam ser reajustadas para a nova escala (os antigos Slow/Fast/Hyper/
Chaos correspondem aproximadamente a **8F/CB/E7/FF**). Os demais parâmetros não mudam.
A profundidade de bits do Bitcrusher vai de 4 a 16 bits e
a redução de taxa de 1x a 32x. Os modos do Destruction são Fold, Clip e Crush.
Saturation e distortion incluem controles de nível de saída ou mix para equilibrá-los
com o sinal não processado.
O Parametric EQ tem shelves de 24 dB de reforço/corte em 60-500 Hz e 2-14 kHz, mais um
sino de Q fixo 0,9 em 250 Hz-5 kHz. Seus controles de gain são neutros em 80; com os
três neutros ele é um bypass exato. O EQ ativo usa um soft knee transparente acima do
fundo de escala e atinge um máximo de 1,5 para conter reforços extremos.

O nível da trilha e o tilt EQ precedem os inserts, então ambos afetam o sinal que entra na
compressão e na distortion. Todas as vozes/notas de acorde da trilha alimentam uma única cadeia de inserts.
Tanto o send de reverb quanto o de delay recebem sua saída. Efeitos compartilhados, retornos,
controles master e mute/solo mantêm o comportamento existente. Os inserts não processam
áudio de dispositivos MIDI externos.

Os comandos de insert usam **F11-F18** para o TF1 e **F21-F28** para o TF2. A seleção de FX do roll
e o popup de FX de Phrase/Table oferecem apenas os parâmetros usados pelos inserts configurados
da trilha selecionada. O popup os agrupa como **TF1: nome do efeito** e
**TF2: nome do efeito**; os slots OFF ficam ocultos. Inserts em bypass continuam editáveis.
No popup de FX de Phrase e Table, a descrição segue o slot de insert da trilha selecionada: mostra o módulo, o nome do parâmetro, os extremos decodificados da faixa e o
valor base salvo. Os comandos existentes da música são preservados se uma mudança de módulo
os tornar inutilizados; abrir seu seletor começa em um controle disponível.
Ao editar um valor, a dica na parte inferior mostra seu valor decodificado e as unidades.
Qualquer coluna de FX de Phrase ou Table pode endereçar qualquer slot; a grade do Phrase mantém suas
três colunas. Esses valores absolutos de runtime persistem através de notas, instrumentos,
phrases, chains e loops comuns. Uma nova reprodução/parada forçada os reinicia. Editar
um valor base na página F limpa apenas a sobreposição desse parâmetro. O MOD atua sobre o
valor base/de automação efetivo sem salvar seus deslocamentos. Selecionar um novo
módulo restaura os padrões e limpa as sobreposições daquele slot. O bypass mantém a
automação existente enquanto suspende o processamento após um breve fade.

**Os endereços se referem a slots de trilha/posições de parâmetro, não a efeitos nomeados.** Um
Phrase reutilizado em outra trilha usa os módulos dessa trilha. Trocar módulos pode
mudar o significado de um comando; os comandos não são reescritos. Endereços OFF/não usados
são no-ops seguros e permanecem intactos nos dados salvos de Phrase/Table.

A lista de escolha MOD do instrumento ganha uma categoria Insert FX. As fontes de cada instrumento
controlam os inserts da trilha que o toca, incluindo o sinal combinado dos acordes.
O mesmo instrumento em duas trilhas afeta cadeias independentes. Valem o timing das fontes existente,
as quantidades com sinal, as regras de combinação de fontes, os modos de stick e a gravação
de movimento. Os controles discretos são quantizados e limitados a escolhas válidas. Não há
novo banco de modulação pertencente à trilha.

O makeup do compressor começa em 0 dB. O OTT começa em cerca de 25% de depth, 25%
de upward, 50% de downward, 0 dB de input e -3 dB de output. Seus limiares de banda, ganhos e
crossovers permanecem nos padrões nativos do Rui-727; Depth é seu controle wet/dry.
O TAPESCAM mantém os padrões auxiliares nativos: fita nova/de alta velocidade, ruído e
compressão desligados, alargamento ligado. Observe os indicadores de clipping existentes ao adicionar
drive/gain. As transições de slot usam fades limitados de 5 ms por meio de áudio seco.

Os arquivos salvos guardam IDs de módulo, bypass e bytes configurados nas seções opcionais e versionadas
`Track inserts: 1,8,2` e `Master inserts: 1,2`. As sobreposições de runtime
e os históricos de DSP não são salvos.
Mantenha backups antes de abrir arquivos com inserts em builds mais antigos. A atribuição do DSP,
as revisões de origem e as adaptações estão em `chipnomad_lib/external/insert_fx/SOURCES.txt`;
os avisos MIT completos acompanham em `licenses/INSERT_FX.txt`.

Em um R36H a 48 kHz, um par medido de inserts em uma trilha consumiu cerca de 4-5%
do tempo real para o Compressor, 6% para o Distortion, 18% para o Doubler, 16% para o TAPESCAM
e 24% para o OTT, antes do custo dos sintetizadores e dos efeitos compartilhados. Dezesseis
instâncias ativas de Doubler, TAPESCAM ou OTT excederam o tempo real. A modulação em taxa de áudio acrescenta
ainda mais custo. Comece com poucos inserts, observe a sobrecarga de áudio e coloque em bypass
os slots não usados; as dezesseis posições disponíveis não são um orçamento de CPU garantido.

A página Insert mantém visível a dica do campo selecionado depois de soltar o botão;
avisos temporários ainda podem tomar seu lugar. A lista de escolha de módulos mostra, ao lado do nome,
o tipo de efeito e o projeto de origem de cada módulo.

### Instrumentos nativos OPLL e VRC7 (em desenvolvimento)

O seletor de tipo de instrumento tem um grupo FM com **OPLL / MSX** (YM2413)
e **VRC7** (DS1001). Cada um oferece agora **73 programas** em listas roláveis de Bank / Preset:
os 15 timbres originais, 40 timbres distintos adicionais das tabelas
YM2413/VRC7/YMF281B do emu2413 e 18 programas de dois operadores criados pelo ChooChoo.
Bytes de timbre exatamente duplicados são omitidos dentro de cada engine. As paletas
adicionais usam o slot de timbre programável do chip; não expandem a
ROM física. O programa zero identifica um timbre personalizado. EDIT + left/right seleciona o
preset anterior ou o próximo dentro do filtro de bank. Fine ct ajusta a afinação de
-100 a +100 cents.

Na lista de programas, EDIT + PLAY pré-escuta o som destacado; solte os
botões para parar. EDIT confirma e OPT cancela. Navegar/pré-escutar não altera
o instrumento, a tabela nem a música. O gesto normal de pré-escuta da página Instrument
continua funcionando depois da seleção. Os envelopes nativos do chip fornecem attack/release;
o Amp env opcional acrescenta um ADSR de software (veja Native chip controls abaixo).
Não há um editor completo de operadores.

Os novos instrumentos guardam os oito bytes nativos de timbre, bank/nome, programa e afinação fina no
arquivo da música/instrumento. Arquivos que contêm esses tipos usam o formato 7.0 e exigem
esta build ou posterior. Os formatos anteriores continuam legíveis; músicas sem esses tipos
continuam sendo salvas como 5.0. Os programas de ritmo estão adiados. A biblioteca expandida está
validada no host; a audição humana e uma nova verificação em portátil continuam pendentes.

### AdLib / OPL2 e OPL3 (em desenvolvimento)

O FM também inclui AdLib / OPL2 (YM3812) e OPL3 (YMF262). Bank filtra a
lista de fábrica; Preset abre grupos de categorias e uma visão All. EDIT + PLAY pré-escuta
antes de selecionar, EDIT seleciona, OPT cancela, e EDIT + left/right em Preset
carrega a entrada correspondente anterior/seguinte. Fine ct ajusta a afinação local. Mode mostra
2 operator, 4 operator ou Dual voice. O OPL2 oculta os patches incompatíveis do OPL3;
o OPL3 pode tocar a coleção compartilhada de dois operadores.

Os títulos dos popups de bank e preset identificam a engine atual. Trocar de engine ou
de slot de instrumento reinicia o filtro de bank para **All banks**, inclusive ao alternar
entre as engines compatíveis AdLib e OPL3. Voltar de um popup dentro do mesmo
slot mantém o filtro escolhido. Cancelar a importação de um bank DX7 restaura o
filtro anterior.

A confirmação de preset também publica as edições feitas ao soltar o botão no próximo
tick de UI/áudio. As músicas em reprodução não precisam mais de outro pressionamento de botão nem de uma
reinicialização do transporte para receber esse patch. A saída ainda segue o buffer de áudio configurado
e o comportamento de envelope/retrigger da engine.

A coleção de fábrica contém 697 entradas de origem do The Fat Man 2-op,
The Fat Man 4-op e DMXOPL3, com 589 identidades de patch únicas normalizadas.
Os aliases mantêm os nomes de origem e a atribuição. Os níveis de operador nativos da origem, as flags de
profundidade de LFO, os deslocamentos de nota, as alturas fixas de percussão e a afinação dual-voice
são mantidos. O volume do tracker usa amplitude de software; não reproduz
as curvas de volume nem as políticas de deslocamento de velocity dos players MIDI originais. As
estimativas de duração de release da origem não cortam notas sustentadas. Alguns efeitos têm
attacks lentos: segure Seashore em vez de esperar que um clique curto o revele.

Os recursos de fábrica ficam em `instruments/chips` junto à biblioteca de instrumentos existente,
com os avisos em `licenses/chip-banks`. A seleção de preset deixa inalterados a tabela do
slot e os inserts da trilha. Os arquivos recém-salvos usam 7.0 e
embutem o timbre completo. O áudio de fábrica é testado por máquina no host e em ARM64; a audição humana
continua pendente.

### Instrumentos nativos Sega PSG e Game Boy (em desenvolvimento)

O CHIP agora inclui Sega PSG, GB Pulse e GB Noise. O Sega usa o clock master
NTSC e seu feedback de ruído de 16 bits, incluindo o ruído derivado do canal de tom.
Seu **Bass range: Extended** padrão reduz o clock virtual quando necessário para
tocar abaixo do limite do divisor do chip, de cerca de 109 Hz. Isso mantém A-2, G-2
e F-2 distintos. Escolha **Chip** para a faixa original, em que as notas mais graves
convergem para o limite do divisor. O ruído de taxa fixa mantém o clock original;
o ruído derivado do tom segue a altura estendida. Instrumentos Sega antigos carregam com
Extended ativado; a configuração é salva com o instrumento.
Os instrumentos GB usam os registradores de pulse/ruído do DMG; nenhum canal wave do Game Boy
é exposto. Suas listas de presets autorais agora contêm **24, 24 e 28** presets,
respectivamente. As adições cobrem ruído Sega ligado/periódico, varreduras de altura do GB,
percussão curta, ruído metálico, drones e subidas. São programas originais,
não sons extraídos de jogos.
A página oferece controles nativos de mode/duty/width, envelope/sweep/ruído conforme
aplicável, além de um ADSR de amplitude de software. Eles são salvos em arquivos nativos de versão 6.
As verificações automatizadas de desempenho/áudio em ARM64 passam dentro dos limites de carga
informados; a audição humana continua pendente.

### DX7 FM (em desenvolvimento)

Selecione **Type → FM → DX7 FM**; o DX7 é a sétima entrada, depois do Arcade / YM2151.
O DX7 FM usa um núcleo MSFA de seis operadores. A página FM existente oferece
Bank, Preset e Fine ct junto com as configurações comuns de instrumento. A navegação de presets
usa listas de bank/categoria. Segure EDIT+PLAY para ouvir o som destacado; soltar
interrompe a pré-escuta. EDIT sozinho confirma ao soltar; OPT cancela. Carregar um som
copia seu patch completo sem alterar as tabelas do tracker nem os insert FX da trilha.

O catálogo de fábrica contém atualmente **67 patches de parâmetros DX7 distintos**:
31 sons musicais originais do OpenDX7, quatro sons únicos CC0 do YSE (seus 32 slots de bank
repetem esses quatro com nomes diferentes) e 32 patches criados pelo ChooChoo.
Aqui, "original" significa programas de parâmetros criados para este projeto; não
significa conteúdo original de fábrica da Yamaha. O YSE é distribuído como quatro sons, não como 32
nomes artificialmente diferentes.
As categorias descrevem a intenção de design de som revisada. Nomes ambíguos continuam
como Unsorted. As verificações numéricas de tocabilidade passaram; a aceitação por audição
está pendente. A meta separada de 1.000 sons liberados para redistribuição não foi atingida.

Para uma biblioteca pessoal persistente, coloque arquivos `.syx` em
**`instruments/banks/dx7/`** junto à biblioteca de instrumentos existente. Subpastas
são suportadas. Abra **DX7 → Bank**: cada arquivo aparece como um bank nomeado, pronto para
a navegação de presets. Reabra Bank depois de adicionar ou remover arquivos; não é preciso conversão nem
a etapa de Load Instrument. Um bank original padrão do DX7/TX7 contém
**32 voices**. Um arquivo com quatro mensagens de bank aparece como quatro banks numerados
(128 voices no total). Arquivos de um único voice também são aceitos. O aplicativo nativo
lê esta pasta; a build para navegador lê seu sistema de arquivos virtual, não pastas
arbitrárias do computador.

Arquivos ruins ou não suportados são pulados, com uma contagem na tela; os banks válidos continuam
disponíveis. A varredura é limitada a 2.048 arquivos, 1 MiB por arquivo, 64 MiB no total,
60.000 voices e oito níveis de pastas aninhadas. Links simbólicos são ignorados. O
patch selecionado pertence ao instrumento: remover seu bank de origem não pode
alterar a música salva. A biblioteca de navegação completa permanece nesta pasta e
não é copiada por inteiro para cada projeto.

LOAD INSTRUMENT também aceita dumps `.syx` do DX7/TX7 original de um único voice e de banks de 32 voices,
incluindo sequências limitadas dessas mensagens suportadas. Ele verifica
enquadramento, contagem de bytes, dados de sete bits, checksums, faixas de parâmetros e tamanho do arquivo
antes de abrir o bank importado no mesmo navegador FM. Selecionar um patch
confirma; cancelar mantém o instrumento atual da música. Este atalho de importação direta
mantém sua lista de navegação durante a sessão; para navegação persistente,
use a pasta de banks acima. Salve a música ou um `.cni` para reter os patches selecionados/editados. Dumps sem cabeçalho,
checksums ruins, performances/extensões do DX7II e outras famílias da Yamaha são
rejeitados. A importação nunca envia mensagens MIDI para equipamentos externos.

O CNI e a versão 6 do projeto guardam todos os 155 bytes originais do voice, o nome completo de exibição,
a velocity nativa de ataque, a afinação fina e a identidade da origem. As músicas não precisam de
bank externo para reabrir. Versões mais antigas do ChooChoo não conseguem ler esses
arquivos de versão 7; projetos apenas com tipos existentes ainda são salvos como versão 5. A velocity nativa vem por padrão em
100; o volume do tracker é ganho pós-síntese e não reataca o envelope.
Os envelopes de operador, o modo de frequência fixa, o keyboard scaling, o envelope de altura e o
LFO continuam sendo comportamento nativo do patch. O ADSR de amplitude FM comum opcional molda
a saída combinada sem substituir os envelopes dos operadores.

Cada parte de trilha tem um LFO e quatro slots de voz de acorde de propriedade separada, em linha com
a política existente de acordes/substituição do tracker. As trilhas não compartilham estado de nota.
O note-off libera os envelopes nativos; cut/panic os limpa. O núcleo MSFA escalar
usa quanta de 64 amostras a 44,1 kHz interno fixo, com um FIR em streaming até
a taxa de saída. As tabelas de consulta são inicializadas uma vez fora da renderização de áudio, de modo que
renderizadores offline/live simultâneos não possam alterar a taxa um do outro. Os eventos
passam a valer no próximo quantum interno (até 1,45 ms), seguido do atraso de grupo do FIR
de cerca de 0,25 ms. As amostras em buffer são retidas entre callbacks.
Esta é a linhagem MSFA Modern, não uma alegação de fidelidade bit a bit ao hardware DX7 nem de uma
emulação do Dexed Mark I. O DX7 é limitado a **16 notas ativas em toda a música**,
incluindo caudas de release, mantendo quatro slots de acorde próprios por trilha.
Quando excede o orçamento, ele toma primeiro as caudas de release, depois as notas sustentadas mais fracas
e depois os ataques novos. Ataques iguais retêm as notas fundamentais entre trilhas antes das extensões
de acorde, com desempate estável por slot/trilha. Essa política vale em todas as plataformas, para que a
mesma música tenha a mesma alocação limitada de notas. Ela não muda a
polifonia de outros instrumentos. A pré-escuta de presets fica desativada durante a reprodução.
O limite foi escolhido a partir de medições no R36H e validado em reprodução mista.

#### Editor de operadores FM

Os instrumentos Genesis FM, Arcade FM, OPL2, OPL3, OPLL e VRC7 têm um botão **EDIT**
à direita de Fine ct. Ele abre uma tabela com os parâmetros do patch:

- Genesis FM / Arcade FM: algoritmo, feedback, pan, AMS, PMS, máscara de operadores e
  configurações de LFO, depois MUL, DT, TL, KS, AR, D1R, D2R, RR e D1L para os quatro
  operadores, mais SSG (Genesis) ou DT2 e AM (Arcade).
- OPL2 / OPL3: feedback, connection e pan por voice, deep vibrato e tremolo,
  depois MUL, TL, AR, DR, SL, RR, waveform, KSL, VIB, TRM, SUS e KSR por
  operador. No OPL3, o modo (2op, 4op, dual) pode ser alterado aqui.
- OPLL / VRC7: os registradores de timbre personalizado (feedback, MUL, TL, KSL, AR, DR, SL,
  RR, waveforms, AM, VIB, EG, KSR). Editar um preset de ROM o transforma em
  "Custom".

Left/Right e as teclas de edição habituais alteram valores; o instrumento continua tocando
como de costume para que você ouça as mudanças. A barra de status descreve o parâmetro sob
o cursor. OPT ou Left+Shift volta à tela do instrumento. DX7 e SID
não têm editor de operadores.

Genesis FM (YM2612) e Arcade FM (YM2151) agora usam o mesmo navegador FM Bank/Preset,
a pré-escuta EDIT+PLAY, os controles de confirmar/cancelar e de afinação fina. O Genesis agora
tem **73 presets**: os 24 originais e 49 programas melódicos suportados do banco CC0
16-Bit FM Music Station da NeoSoundFonts. O Arcade tem **81 presets**: os 24 originais e 57 programas
suportados e sonoros da coleção GPLv3 do YMulator-Synth. O par original do ChooChoo compartilha suas receitas de base; as novas
coleções de origem oferecem paletas separadas. São coleções nomeadas de design de som,
não recriações alegadas de trilhas sonoras específicas de jogos. Deslocamentos de nota,
teclas fixas de percussão, configurações de ruído arcade e programas de origem silenciosos
não suportados são registrados como exclusões em `expansion-manifest.json`. O patch
completo de quatro operadores, os envelopes, o LFO, as configurações de estéreo e afinação viajam dentro de instrumentos e músicas de versão 6.
O envelope nativo do chip controla o release; o ganho comum do tracker e a altura
não o reiniciam. A saída DAC do Genesis usa um bloqueador de DC de 20 Hz. O modo de ruído e
o modo de frequência especial do canal 3 estão fora desta implementação de instrumento.

Para um bank externo local, use `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`. Ele aceita TFI
estrito de 42 bytes, texto VOPM OPM, o formato WOPLX verificado e SysEx do DX7 original.
Carregue os arquivos `.cni` resultantes por Load Instrument. As importações do usuário são mantidas
separadas da biblioteca de fábrica distribuível. Arquivos OPM com ruído habilitado,
dados de frequência de ruído diferentes de zero ou panorama parcial são rejeitados com uma explicação;
os valores de pan do VOPM 0/64/127 viram esquerda/ambos/direita. O WOPL binário não é suportado.
O catálogo FM compartilhado contém **1.064 entradas**: 697 OPL, 67 DX7, 146
OPLL/VRC7, 73 Genesis e 81 Arcade. Outros 76 arquivos expõem os presets Sega/Game Boy
pelo navegador de arquivos normal, somando **1.140 presets nativos empacotados**.
As revisões de origem, hashes, avisos completos e dados de origem originais acompanham
as novas coleções em `licenses/chip-banks/expansion`.

### Coleções de fábrica portáteis e presets USER

Os instrumentos de chips nativos usam **Bank:** para escolher ALL, uma coleção incluída ou
USER. **Preset:** escolhe um som dentro dessa seleção. Segure EDIT e pressione
Left ou Right em qualquer uma das linhas para percorrer; um toque curto em EDIT abre sua lista de escolha.

As coleções incluídas são pacotes ZIP em `instruments/FACTORY/`. Os arquivos USER vão
em `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` ou `gb-noise`.
Navegue diretamente pelas pastas ou coloque ali um ZIP de presets compatíveis.
Os ZIPs permanecem intactos em disco. Todas essas pastas aceitam arquivos `.cni` compatíveis; o CNI é
o formato portátil de instrumentos do ChooChoo/ChipNomad, não um padrão de fabricante de chips.
O navegador USER também lê os seguintes formatos de origem, inclusive dentro de ZIPs:

| Pasta USER | Formatos adicionais |
| --- | --- |
| `dx7` | voices únicos e banks de 32 voices Yamaha DX7/TX7 `.syx` |
| `genesis` | `.tfi`, DefleMask `.dmp`, Furnace `.fui` |
| `arcade` | banks VOPM `.opm`, DefleMask `.dmp`, Furnace `.fui` |
| `opl2`, `opl3` | `.wopl`, `.opli`, `.woplx`, `.sbi`, Furnace `.fui` |
| `opll`, `vrc7` | patches personalizados Furnace `.fui` e seleções fixas de ROM |
| `sega`, `gb-pulse`, `gb-noise` | Furnace `.fui`, DefleMask `.dmp` |
| `sid` | GoatTracker 2 `.ins` (GTI5), Furnace `.fui` |

Um bank abre em voices compatíveis. Arquivos ou recursos não suportados informam um
motivo; voices incompatíveis em um bank misto são excluídos, enquanto seus números de origem
permanecem estáveis. O carregamento é transacional: um preset rejeitado deixa o
instrumento atual inalterado. Os presets de origem substituem a tabela do tracker do instrumento
por uma vazia, exceto o SysEx do DX7, que preserva sua tabela existente.
O CNI restaura sua tabela salva. Os patches carregados pertencem à música e continuam
funcionando depois de mover ou apagar o pacote de origem.

Os banks WOPL podem conter slots não usados sem uma flag explícita de vazio. O navegador
omite slots sem nome cujos operadores ativos estão totalmente atenuados e não têm
attack. Voices tocáveis sem nome continuam disponíveis como `Program N [Bbank]` ou
`Drum N [Bbank]`, usando os números de programa e de bank da origem, contados a partir de zero.
Um instrumento OPLI único sem nome usa seu nome de arquivo. Patches silenciosos com nome são
mantidos.

O suporte ao Furnace cobre versões legadas de instrumento único 29-112 e versões `FINS`
baseadas em recursos 127-251. O suporte ao DefleMask cobre instrumentos da versão 11 de OPN, OPM, Sega PSG
e Game Boy. São importações de instrumentos, não players de tracker completos: macros animadas
de FM/operadores, configurações de LFO de FM da música de origem, samples,
sequências de comandos de hardware do Game Boy, macros relativas de pulse/filtro do SID e dependências de
ring/sync do SID são atualmente rejeitadas. A seleção constante de ROM do OPLL é
suportada; o VRC7 usa sua própria ROM para essa seleção. O SID-Wizard `.swi` e os arquivos
de música SID (`.sid`) não são importações de instrumento nesta build.

As sequências PSG/GB/SID suportadas são guardadas no patch e tocadas a 60 Hz para
Furnace/DefleMask e a 50 Hz para GoatTracker. Esses arquivos não carregam a configuração de
reprodução completa de uma música: use o tracker original quando forem necessários a
taxa de ticks específica da música, o roteamento de canais, o comportamento de hard-restart ou as configurações de compatibilidade. Para o Sega PSG, uma macro de duty seleciona a voz de ruído; sem ela, a
importação usa uma voz de tom. Os arquivos Game Boy devem ser colocados na pasta pulse ou
noise pretendida. As sequências importadas, por enquanto, continuam como dados de reprodução embutidos; a
página do instrumento edita parâmetros base, não as tabelas do tracker de origem.

O ciclo de presets USER continua através de arquivos, pastas, banks e pacotes ZIP em
ambas as direções, dando a volta nas pontas. Reabrir o navegador USER segue a
seleção atual. ALL inclui sons USER em Unsorted para engines com categorias;
USER continua sendo uma escolha separada de nível superior. Sega PSG e Game Boy usam
listas planas ALL/Factory e um navegador USER hierárquico, sem painel
Unsorted. Arquivos DX7 mais antigos em `instruments/banks/dx7/` continuam acessíveis em USER.

Uma engine mostra apenas suas coleções incluídas compatíveis. Factory Presets
combina os conjuntos de timbres OPLL/VRC7 incluídos para a engine selecionada; o DX7 combina
os originais do ChooChoo e do YSE. As coleções nomeadas baixadas permanecem separadas.
O Fat Man 2-op aparece em OPL2 e o Fat Man 4-op em OPL3. Arquivos CNI compatíveis de OPL2
ainda podem ser usados nas pastas USER do OPL3.

A saída das engines nativas usa compensação de ganho fixa e medida em vez de
normalização por preset. Músicas existentes com instrumentos nativos podem, portanto, tocar
em outro nível; reveja o equilíbrio das mixes salvas. O método de calibração
e seus limites estão documentados em `scripts/README_MEASURE.md`.

`native-chip-audition.cct` oferece uma curta audição sequencial pelos treze
banks de fábrica originais. Cada seção usa um instrumento próprio diferente, então funciona com
a pasta de presets removida. WAVs de banks mais longos, ignorados pelo controle de versão, e níveis medidos estão
listados em `docs/chip-preset-auditions.tsv`; a audição subjetiva continua pendente.

### Native chip controls

As prévias de forma de onda dos instrumentos nativos são atualizadas quando seus controles são redesenhados.
Os instrumentos FM podem sobrepor sua curva opcional de Amp env; o SID mantém sua prévia de
forma de onda sem essa sobreposição exclusiva do FM, inclusive ao navegar por banks e presets.

Todas as sete engines FM oferecem **Bright**, **Feedback** e **Amp env**. Bright
vai de -63 a +63, com zero preservando o patch; ele altera os níveis dos
operadores moduladores mantendo os níveis das portadoras. Seu efeito audível depende do
algoritmo. Feedback vem por padrão em **Preset**, ou sobrepõe o feedback nativo
com 0-7. Esses controles não reescrevem os bytes de operador nativos salvos.

Amp env vem por padrão em **Bypass**. Selecione **ADSR** para acrescentar attack, decay, sustain,
release e shape ao redor do som nativo. A/D/R usam a faixa quadrática compartilhada de 0-5 segundos;
o sustain vai do silêncio ao nível total. Os envelopes nativos continuam
rodando, então este envelope não pode estender um som além de seu release nativo. A navegação
de presets preserva os controles de amp FM e de timbre do slot. Todas as vozes FM também usam
uma transição de 3 ms no início/retrigger e um suavizamento de 1 ms do ganho do tracker, inclusive
quando Amp env está em bypass. O cut/panic forçado continua imediato.

Os seguintes FX de phrase também aparecem como destinos suportados de Modulation e de gravação
de movimento. Os valores na coluna FX são hexadecimais. As configurações se aplicam à reprodução
sem alterar o instrumento salvo. Os controles nativos de sweep e envelope do GB
são capturados no próximo disparo de nota; duty/width e a frequência do ruído podem mudar ao vivo.

| Engines | FX | Controle |
|---|---|---|
| Native FM | `OL1`-`OL6` | Níveis de saída absolutos dos operadores: modulador OPLL/VRC7 e OPL `00-3F`; portadora OPLL/VRC7 `00-0F`; Genesis/Arcade `00-7F`; DX7 `00-63` (0-99). Mais alto significa mais saída. Somente os operadores suportados aparecem. |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | Attack, decay, release e atenuação de sustain do operador 1, `00-0F`. |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | Multiplicador de frequência do operador 1. |
| Genesis/Arcade | `LFR` | Taxa nativa do LFO: Genesis `00-07`, Arcade `00-FF`. |
| Arcade | `LAD`, `LPD 00-7F` | Profundidades separadas de amplitude e altura do LFO. |
| Genesis/Arcade | `LAS`, `LEN` | Sensibilidade de amplitude nativa `00-03` e habilitação do LFO `00-01`. |
| Genesis/Arcade | `LPS 00-07` | Sensibilidade de altura nativa. |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | Attack, decay, sustain e release nativos. Os valores de tempo aumentam em direção a `0F`; o sustain aumenta em direção ao nível total. |
| SID | `SPR 01-10` | Frequência do parceiro silencioso de 1× a 16×; afeta a modulação em anel e o hard sync. |
| Todas as native FM | `FBK 00-07` | Feedback nativo absoluto, inicializado a partir do instrumento. |
| Todas as native FM com Amp env ativado; Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack, decay, sustain, release, shape. Eles não ativam um amp FM em bypass. |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | Tom / ruído branco / ruído periódico; três taxas de ruído fixas ou taxa derivada do tom. |
| GB Pulse | `CMD 00-03` | Duty de pulse nativo. |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | Largura do ruído, divisor de clock e shift. |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | Período do sweep, shift, direção descendente. |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | Envelope nativo: nível inicial, período, direção ascendente. |

A seleção de FX nativos usa o instrumento da coluna `I` da linha do phrase, ou o
instrumento ativo encontrado olhando para trás quando `I` está em branco. O popup de FX
mostra, em um bloco compacto, uma breve descrição do controle, seu valor de preset atual e o intervalo válido de
comandos. Selecionar um
efeito nativo diferente começa nesse valor; reabrir o mesmo efeito preserva seu
valor editado. Uma seleção de várias linhas resolve o instrumento de cada linha separadamente.
As tabelas usam o contexto de seu instrumento. As edições de valor nativas param em seus extremos
legais, inclusive duty, ruído, sweep, ADSR, feedback e níveis de operador.
Os valores são hexadecimais: por exemplo, o máximo `63` do DX7 significa 99 em decimal.

Esses comandos FM usam valores nativos absolutos. Os comandos de operador sempre se dirigem ao
operador 1; `OMU 03` seleciona seu multiplicador 3. Os controles específicos do tracker do DX7
são níveis de operador e feedback. Preset/Range mantém sua cor de informação;
títulos e descrições seguem as mesmas cores do FM Feedback.

O Live Modulation mantém seu conjunto completo de destinos nativos de operador fixo.
A gravação de movimento escreve apenas os controles representados na lista compacta do tracker;
os parâmetros de operador são gravados apenas para o operador 1. Outros destinos de modulação
continuam funcionando ao vivo, mas não geram comandos de phrase.

`SCP` e `SCT` mantêm mapeamentos escalonados por byte para os registradores do SID mais largos que 8 bits.
`SMR 01-C8`, `SWV 01-08` e `SPR 01-10` agora correspondem à numeração nativa dos presets.
Os valores de preset exibidos são configurações base, independentes do envelope ou
do LFO em reprodução. `SLE` funciona nos níveis de operador e nos parâmetros FM diretos sem
redisparar a nota nem alterar o patch armazenado.

Todos os FX nativos têm títulos descritivos, faixas e comportamento no seletor de FX
de phrase, além de dicas de valor. `CMD` descreve tom/ruído, duty ou largura de ruído
conforme o instrumento selecionado. Esses controles complementam os grupos compartilhados
Track, Envelope e Modulation; o grupo da engine sozinho não é o conjunto completo
de efeitos de phrase disponíveis.

Os filtros LP/BP/HP compartilhados existentes no Braids, Plaits, PCM e outras engines
suportadas são processamento de software depois da síntese. O aChChid usa em vez disso seu caminho
nativo de filtro 303. Esta rodada não acrescenta filtro ao AY nem ao FM nativo; o brilho altera
a própria síntese FM. Os inserts de trilha continuam disponíveis para processamento adicional.

Os instrumentos nativos são salvos em CNI versão 7 (8 quando há comandos absolutos),
e as músicas nativas em CCT versão 9. Instrumentos que contêm sequências importadas
usam CNI versão 9 e músicas que as contêm usam CCT versão 10. Builds mais antigas
não conseguem abrir esses arquivos mais novos com sequências. Os bytes de preset FM armazenados e
as configurações de timbre da página do instrumento permanecem inalterados.

### Orientações de carga de trabalho dos chips nativos em portáteis

Há oito trilhas de música e **dois slots de efeitos insert por trilha**. Os dezesseis
slots disponíveis não garantem CPU suficiente para rodar dezesseis efeitos.
O limite de dezesseis notas ativas do DX7 é um orçamento de síntese separado para toda a música;
acordes e caudas de release contam para ele.

Nas medições no R36H, quatro trilhas WAV em loop, Sega PSG, GB Pulse, DX7 e
OPL3 com sends compartilhados e quatro inserts (dois Compressors, Doubler, TAPESCAM)
tiveram desempenho melhor que arranjos FM densos de oito trilhas. A versão de nota única
não teve prazos de renderização perdidos em sua medição de trinta segundos a 48 kHz/512 frames;
acrescentar um acorde de quatro notas do DX7 gerou um pico de timing. Músicas FM densas
com muitos inserts caros excederam o orçamento de CPU. Preserve alguma folga,
observe o indicador de sobrecarga existente e acrescente efeitos onde ajudem a música.
O aplicativo mantém todas as trilhas e slots; não há um novo limite rígido para a quantidade de inserts.
Veja `chip-instruments-report.md` para as medições completas e o estado dos testes prolongados.

O teste de áudio físico correspondente, de 70 segundos, desse arranjo equilibrado de acordes
passou sem prazos de renderização perdidos nem underruns de ALSA registrados na configuração
existente de 48 kHz / 4906 frames. Seu pior callback foi de 81,819 ms contra um
prazo de 102,208 ms. Os testes usaram um ganho master separado de 0,4 para folga.
Mantenha a rota de placa direta `AUDIODEV=plughw:0,0` do launcher habitual: o
mixer compartilhado padrão do sistema produziu underruns nas sondas de diagnóstico. Nenhuma
configuração de áudio do usuário foi alterada. O teste de estresse de dez minutos com buffer menor
ainda registrou 28 picos de timing; os resultados completos estão no relatório.

### Instrumentos SID e Phrase FX

Escolha **CHIP → SID**. Bank oferece **ChooChoo SID Originals** (32 programas
autorais) e **SIDkit Effects** (24 efeitos sob licença MIT). Ambos usam o mesmo
navegador de presets e salvam seu programa selecionado completo dentro do instrumento
e da música. Nenhum bank externo é necessário ao compartilhar a música. São programas
nativos de parâmetros do SID com envelopes e receitas de movimento, não áudio sampleado nem
músicas completas de C64. Os doze candidatos de pesquisa separados do GoatTracker
não são distribuídos; suas tabelas de wave/pulse/filter precisam de um importador/player dedicado.

A página do instrumento expõe forma de onda/largura de pulso, modo de filtro/cutoff/ressonância
e controles ADSR nativos. O SID usa o oscilador digital, o envelope
e o filtro por ciclo no estilo 6581 fixados do floooh/chips. Não há modelos alternativos de chip,
seletores de revisão nem perfis de caráter adicionais. É uma aproximação genérica,
não um modelo analógico calibrado de R2/R3/R4/8580.

Cada nota tem seu próprio filtro. Ring/sync usam um oscilador parceiro silencioso, então
o compartilhamento de filtro e as interações de três vozes diferem de um SID físico.
O portátil tem um orçamento para toda a música de quatro notas SID comuns.
Ring ou sync custam duas unidades de orçamento por nota, permitindo duas dessas notas, ou uma mais duas
notas comuns. Acordes e caudas de release contam. As notas liberadas são retiradas
primeiro; as notas novas têm prioridade sobre as notas sustentadas mais antigas. Inserts caros e
outros sintetizadores continuam compartilhando o orçamento de CPU de áudio.

| FX | Faixa | Ação |
| --- | --- | --- |
| `SCP` | `00-FF` | Base da largura de pulso, escalada para a largura nativa de 12 bits; o movimento de pulso da receita continua ativo. |
| `SCT` | `00-FF` | Base do cutoff, escalada para o cutoff nativo de 11 bits; o movimento de filtro da receita continua ativo. |
| `SRN` | `00-0F` | Ressonância nativa. |
| `SWV` | `00-07` | Triangle, saw, tri+saw, pulse, tri+pulse, saw+pulse, tri+saw+pulse, noise. |
| `SFI` | `00-07` | Bits do modo de filtro: 1 passa-baixa, 2 passa-banda, 4 passa-alta; 0 bypass. |
| `SMR` | `00-FF` | Clock de macro da receita, escalado de 1 a 200 Hz. |
| `SRG` | `00-01` | Modulação em anel de triângulo com parceiro silencioso. |
| `SSY` | `00-01` | Sync de oscilador com parceiro silencioso. |

Largura de pulso e cutoff suportam `SLE`. Formas de onda discretas, chaves, ressonância
e feedback de FM mantêm seus passos nativos úteis; valores de byte mais amplos não
criariam mais estados de hardware. `FBK` seleciona diretamente o feedback `00-07`.
Os níveis de operador usam as faixas específicas de cada engine listadas acima. Os ajustes de brilho e
de master mantêm a faixa completa de byte.

### Coleções de fábrica portáteis e presets USER

Os instrumentos de chips nativos usam **Bank:** para escolher ALL, uma coleção incluída ou
USER. **Preset:** escolhe um som dentro dessa seleção. Segure EDIT e pressione
Left ou Right em qualquer uma das linhas para percorrer; um toque curto em EDIT abre sua lista de escolha.

As coleções incluídas são pacotes ZIP em `instruments/FACTORY/`. Os arquivos USER vão
em `instruments/USER/<engine>/`: `dx7`, `opll`, `vrc7`, `opl2`, `opl3`,
`genesis`, `arcade`, `sid`, `sega`, `gb-pulse` ou `gb-noise`.
Navegue diretamente por suas pastas, ou coloque ali um ZIP de pastas e presets `.cni`
compatíveis. O DX7 também aceita voices únicos `.syx` suportados e banks de 32 voices,
inclusive dentro de ZIPs. Os ZIPs permanecem intactos em disco. O carregamento direto de WOPL/WOPLX não é suportado;
use presets CNI compatíveis ou converta antes, offline, os formatos de origem suportados.

O ciclo de presets USER continua através de arquivos, pastas, banks e pacotes ZIP em
ambas as direções, dando a volta nas pontas. Reabrir o navegador USER segue a
seleção atual. ALL inclui sons USER em Unsorted para engines com categorias;
USER continua sendo uma escolha separada de nível superior. Sega PSG e Game Boy usam
listas planas ALL/Factory e um navegador USER hierárquico, sem painel
Unsorted. Arquivos DX7 mais antigos em `instruments/banks/dx7/` continuam acessíveis em USER.

Uma engine mostra apenas suas coleções incluídas compatíveis. Factory Presets
combina os conjuntos de timbres OPLL/VRC7 incluídos para a engine selecionada; o DX7 combina
os originais do ChooChoo e do YSE. As coleções nomeadas baixadas permanecem separadas.
O Fat Man 2-op aparece em OPL2 e o Fat Man 4-op em OPL3. Arquivos CNI compatíveis de OPL2
ainda podem ser usados nas pastas USER do OPL3.

A saída das engines nativas usa compensação de ganho fixa e medida em vez de
normalização por preset. Músicas existentes com instrumentos nativos podem, portanto, tocar
em outro nível; reveja o equilíbrio das mixes salvas. O método de calibração
e seus limites estão documentados em `scripts/README_MEASURE.md`.
