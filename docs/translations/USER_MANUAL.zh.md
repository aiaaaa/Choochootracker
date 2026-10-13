# ChooChooTracker 用户手册

> 本文是 [`docs/USER_MANUAL.md`](../USER_MANUAL.md) 的简体中文译本，依据提交 `7c3ed6f` 的英文版翻译。如有出入，以英文版为准。
>
> **约定。** 应用的界面是英文的。因此，屏幕、菜单、字段和按键的名称（Song、Chain、Phrase、Instrument、Table、Mixer、**Settings**、**EDIT**、**OPT**、**SELECT** 等）、FX 代码（`HOP`、`TBX`、`M1A` 等）、模块和引擎的名称（ADSR、LFO、Braids、Plaits 等）以及文件扩展名均保留原文，方便你与屏幕上的显示对照。

ChooChooTracker 是一款面向掌机的 8 轨音乐 Tracker。

它沿用 LSDj 经典的工作流程和导航方式，并提供多种声音设计选项：

- AY 芯片模拟
- Mutable Instruments 的 Braids、Plaits 和 Plaits-Alt 振荡器，配有多模式滤波器
- PCM 采样、单周期波形和波表振荡器，配有多模式滤波器
- aChChid：基于 Open303 的多功能 Acid 合成器
- Bogie：原生的 12 种模型的鼓合成器，配有多模式滤波器
- MME：双振荡器多重调制合成器，配有多模式滤波器
- Sintered：原生的 6 种模型的实验性数字鼓合成器

## 1. 安装与文件

### ArkOS / PortMaster

通过 PortMaster 安装 `choochootracker.zip`，或将解压后的软件包复制到主机的 `ports` 目录。从 Ports 菜单启动 **ChooChooTracker**。

### Miyoo Mini / Mini Plus (Onion OS)

将 MiyooPorts 压缩包解压到 SD 卡根目录，使其 `Roms/PORTS/` 文件夹与 Onion 已有的文件夹合并。刷新收藏后，从 Ports 菜单启动 **ChooChooTracker**。替换已有安装之前，请先备份应用的 `projects/` 文件夹。

### Windows

将 `SDL2.dll` 和 `libwinpthread-1.dll` 放在同一目录，运行 `choochootracker.exe`。
按 **ALT + ENTER** 切换全屏。窗口可以调整大小；图像会在保持原始宽高比的前提下缩放。

### Web

浏览器页面会跟随当前的 Tracker 主题。已保存的设置（包括用 **Done** 确认的自定义按键映射）会立即写入此浏览器的存储。锁屏或切换应用之后，必要时点按任意控件即可恢复音频。**Restart audio** 会在不丢弃项目的前提下重新打开输出。**Reload app** 会把当前工作保存到 **Continue**，保存设置，等待浏览器存储写入完成后再重新加载；如果保存失败或超时，则取消重新加载。使用 **Download .cct** 获得可移植的备份。浏览器存储可能被单独清除，iOS 上控制器和音频路由的恢复无法保证。

浏览器版本以约 30 FPS 刷新显示，并使用固定的 1024 帧音频缓冲区，让音频在两次回调之间有更多时间。较早保存的缓冲区设置会被自动调整；项目和其他设置均予保留。输入轮询和应用定时器保持原有节奏。

### 目录配置

| 文件夹 | 内容 |
|---|---|
| `AY_Wavetables` | `.aywave` 格式的 AY 芯片波表 |
| `fonts` | `.cnfont` 格式的 ChipNomad 字体文件 |
| `instruments` | `.cni` 乐器预设，包括 ChipNomad 文件 |
| `pitch-tables` | `.csv` 格式的替代调音 |
| `projects` | `.cct` 格式的歌曲和项目文件 |
| `samples` | 用于 Sampler 引擎的 `.wav` 文件 |
| `SR_wavetables` | 用于 BYOWTBL 引擎的 Serum 格式波表 |
| `themes` | `.cth` 格式的 ChipNomad 主题 |
| `title` | 标题画面的素材 |
| `waveforms` | 用于 2xSCWF 引擎的 `.wav` 单周期波形 |

## 2. 操作

ChooChooTracker 配合带有方向键、2 个模拟摇杆和 7 个按键的手柄使用效果最佳。

| 逻辑控件 | 掌机默认 | Windows 默认 |
|---|---|---|
| **[LEFT/RIGHT/UP/DOWN]** | 方向键 | 方向键（键盘） |
| **EDIT** | A | X |
| **OPT** | B | Z |
| **PLAY** | Start | 空格键 |
| **SELECT** | Select | Shift |
| 调制轴 | 左、右摇杆 | 游戏控制器的摇杆 |
| **STICK LIVE** | L1 | Q |
| **MOTION RECORD** | L2 | W |
| **MOTION ERASE** | R2 | E |

映射可在 **Settings > Key mapping** 中更改。在 PortMaster 上，请先松开用来选择映射的按键，再按下新的按键。用 **Done** 保存重新映射，然后松开按键，再继续在 Settings 中操作。桌面布局的默认值只在 QWERTZ 地区使用 QWERTZ；波兰语键盘保持标准的 QWERTY 默认值。

Windows、Web 和 Android 用户也可以使用游戏控制器。在 Android 上，未连接手柄时会自动显示触控控件：竖屏时位于 Tracker 下方，包括方形方向键、A/EDIT、B/OPT、START/PLAY 和 SELECT/SHIFT 控件，再下面是单独的 Live/Motion 区域，有两个调制摇杆和临时性的 REC/DEL 控件。横屏时，方向键在左侧，动作按键在右侧。Android 上的项目和采样仅供应用私有使用；请用 Android 的文件选择器导入 `.cct` 项目和 `.wav` 采样，并选择 WAV 导出的保存位置。

### 导航

- 用 **[UP/DOWN/LEFT/RIGHT]** 移动光标。
- 按住 **SELECT + [DIRECTION]** 在屏幕之间切换。
- **SELECT + [LEFT/RIGHT]** 也可以像 Instrument Pool 那样，从分支页面回到相邻的 MSCPIT 屏幕：Project/Settings（包括 Settings 的子菜单）通向 Mixer/Chain；Groove 通向 Chain/Instrument；Modulation/Insert FX 通向 Phrase/Table；AY Wavetable 通向 Instrument/Table。在最左侧边缘，Reverb/Delay 用 SELECT + LEFT 返回 Mixer。现有的纵向导航保持不变。
- 按住 **OPT + [DIRECTION]** 进行各屏幕专用的导航。
- 在某个数值上，用 **EDIT + [LEFT/RIGHT]** 做细调，用 **EDIT + [UP/DOWN]** 做粗调。
- 在 Modulation 的 **Destination** 上，**LEFT/RIGHT** 可直接循环可用的目标；点按 **EDIT** 打开分组菜单。
- 点按 **EDIT** 输入数值或执行命令。
- 在支持的地方，连按 **EDIT** 两次可新建 chain、phrase 或乐器。
- 按住方向键会在 Settings 中配置的延迟之后重复。

### 选择与剪贴板

- **SELECT + OPT** 进入选择模式，并依次切换有用的选择范围。
- 用 **[DIRECTION]** 扩展范围。
- **OPT** 复制所选内容。
- **OPT + EDIT** 剪切。
- **SELECT + EDIT** 粘贴。
- **EDIT + [DIRECTION]** 同时编辑所选单元格。

在 Phrase 屏幕上，选区处于活动状态时，可以用节奏型填充音符列：

- **OPT + LEFT** 用固定的 16 步节奏型填充选区的音符单元格，并在每次按下时切换到下一个节奏型。最后一个节奏型之后，音符被清除，循环重新开始。节奏型按 phrase 的绝对行对齐，因此部分选区只会得到落在其中的步。
- **OPT + RIGHT** 用 3 到 8 个音符的随机节奏型填充选区的音符单元格（7 个和 8 个的概率较低），只要按住 **OPT** 就会无限循环。
- **OPT + UP** 对选区中已有的音符做变异：每按一次，选取其中的 1 到 3 个，并把每个音符随机移动一个八度以内的量（小幅移动比极端移动更常见）。启用 **Note Lock** 时，移动后的音符会被拉回到音阶内；开启了 **Slice** 的 Sampler 乐器保持半音阶。不会擦除任何内容：第 8 次按下会恢复循环开始时选区中的音符，随后循环重新开始。
- 在只覆盖音量列（没有音符、乐器或 FX 单元格）的选区上按 **OPT + UP**，改为随机化力度（velocity），而不是变异：第一次应用 Minimal 模式（每个音量最多偏移 +-20），第二次是 Medium（最多 +-50），第三次是 Random（完全随机的数值），第四次恢复循环开始时选区中的音量。松开 **OPT** 会让循环回到 Minimal。没有音量的行保持不变。
- **OPT + DOWN** 把选区中已切片（sliced）的 Sampler 乐器已放置的音符展开成半音阶序列：每个乐器的第一个音符（在音序器中最高的那个）保持原值，该乐器之后的每个音符都比前一个高一个半音，把一堆相同的音符变成连续的切片触发。多个已切片的乐器各自从自己的第一个音符独立展开；未切片的乐器的音符不受影响，结果也不会被吸附到项目的音阶上。展开所写的最后一个音符会成为输入时使用的最后一个音符，因此手动继续这个序列会接着下一个切片。只包含已切片乐器的选区在每次按下时都会展开；当同时存在未切片的音符时，第一次按下执行展开，从第二次起由随机琶音接管这些音符。
- 在没有已切片乐器音符的选区上按 **OPT + DOWN**，是对其中已有音符的随机琶音器。和弦从 CRD 调色板中选取：1 到 2 个音符的选区用 3 音和弦，3 个音符用 3 音或更多音的和弦，4 个及以上可用任意和弦；根音落在选区音域内项目音阶的一个随机音上（音阶关闭或该轨被排除时，则为任意半音阶音符），和弦音会像播放 CRD 时那样被量化到音阶中。每按一次，就把和弦重新分配到已放置的音符上：随机、从低到高、从高到低，然后上下往返；第 5 次按下恢复原来的音符，下一次按下则选取新的和弦。琶音激活期间，和弦名称显示在状态栏中，不会添加或删除音符，松开 **OPT** 会重置整个过程。
- 每次填充按下前都会先擦除选区的音符，所放置的音符带有最后使用的乐器；松开 **OPT** 会让各循环回到第一步。
- 选区处于活动状态且没有其他消息显示时，状态栏每 2 秒轮流显示这些组合键的提示：`EDIT + DIR =
  batch note edit`、`Double-tap EDIT: resample selection`、`OPT + LEFT =
  pattern fill`、`OPT + RIGHT = random fill`、`OPT + UP =
  mutate/randomize velocity` 和 `OPT + DOWN = random arp/slice spread`。进入或退出选择模式会重新开始轮换；任何操作消息都会取代提示，直到它消失。

### 播放

- **PLAY** 从光标处开始播放。再按一次停止。
- 播放时可以编辑音符、乐器和 chain；更改会在下一个音序器 tick 生效。
- 在 Song 屏幕之外使用时，**SELECT + PLAY** 启动所有轨道。
- **EDIT + PLAY** 在 Instrument Pool 中试听乐器。

## 3. 歌曲结构与屏幕地图

按住 **SELECT + [DIRECTION]** 在屏幕之间切换。

主导航行如下：

<pre>
R P   G M
<strong>M S C P I T</strong>
D S     P W
</pre>

中间一行：

- **M**：Mixer（分支到 Reverb、Delay）
- **S**：Song（分支到 Project、Settings）
- **C**：Chain
- **P**：Phrase（分支到 Groove）
- **I**：Instrument（分支到 Modulation 和 Instrument Pool）
- **T**：Table（分支到 AY Wavetable 编辑器）

上面一行：

- **R**：Reverb
- **P**：Project
- **G**：Groove
- **M**：Modulation

下面一行：

- **D**：Delay
- **S**：Settings
- **P**：Instrument Pool
- **W**：AY Wavetable 编辑器

ChooChooTracker 的歌曲遵循这样的层级结构：

```text
Song -> Chain -> Phrase row -> Note + Instrument + FX
```

一首歌有 8 条轨道。每条轨道包含一串 chain，每个 chain 包含一串 phrase。Phrase 是 16 步的音符序列。一个音符包含音高、乐器、力度，以及最多 3 个轨道 FX。乐器就是发声的部分。

### 可选的 16x24 字体与 NostromoAmberDa2

Settings → Load font → fonts/Pixel16x24 中包含 TechMonoAudit（由 Share Tech Mono 转换而来）、Departure Mono、Spleen 和 Cozette。每一款都提供完整的可打印 ASCII 字符集，采用 16×24 的单元格，适用于 640×480 的显示。由像素构成的字体保持其原生像素，或带留白的整数倍缩放；Cozette 仍然偏小，因为把它 13 像素的字形高度翻倍就会超出单元格。NostromoAmberDa2 主题可通过 Settings → Edit color theme → Load 使用。它保留了 Nostromo 的琥珀色背景和说明文字，主文字为红色，数值为更深的橙色，光标为青绿色。加载这些素材不会改变你所选的字体或主题。出处和许可请参见随附的 README.txt。

### Project 页面的间距

Project 页面把应用版本放在单独一行，并用空行把 Load、Save、New、Export 和 Manage 与项目元数据分隔开。光标位置和触控区域跟随所显示的字段；项目设置和文件操作不变。

### 音频显示的遥测

音频读数使用由界面持有的、最终混音和各轨贡献的快照。轨道采样和立体声峰值在轨道电平和 tilt 之后、共享效果返回和总增益之前采集。监测不会改变音频信号。音频回调发布固定大小的快照，不使用锁，也不分配内存；显示的历史约覆盖 21 ms。

### 常驻波形

Graphics → Persistent waveform 默认为 OFF。点按 EDIT 切换，或使用 EDIT + 方向键；该选择作为应用偏好保存。OFF 会恢复全高度页面和现有的乐器波形预览。启用后，编辑页面上方会显示一个两行高的波形，表示合并为单声道的最终混音。Instrument 和 Modulation 页面显示使用所选乐器的某条轨道的输出；如果多条轨道使用它，则以所选的 Song 轨道为准。Project、Settings、波表和弹出窗口保持全高度内容。列表在剩余的行内滚动。Instrument 的页脚保留 ADSR/采样预览，Modulation 的字段会被压缩，以便所有控件仍可使用。在两种波形模式下，它的触控区域都跟随所显示的字段。

### 播放钢琴

在右侧八条轨道行下方，有一架由代码绘制的像素钢琴，它会点亮正在发声的音级，折叠到一个八度内。它包含和弦音，并遵循静音或已停止的轨道。轮廓使用主题的数值颜色，所有活动的琴键使用波形颜色；空闲的琴键带有深色填充和淡淡的阴影。它的网格使用整数倍缩放和固定宽度的边框。钢琴是播放指示器，而不是音符输入控件。弹出窗口和波表编辑器在这个区域保留自己的内容。

### Mixer 电平表

Mixer 为每条轨道显示相邻的 **L/R** 峰值条，位于其 PAN 和 tilt 之后、共享效果和总增益之前。电平表的范围为 -48 到 0 dBFS，并在两次更新之间衰减。其低段和中段使用主题颜色，而最高段在接近满量程时为鲜红色。

## 4. Song、Chain 和 Phrase

### Song

Song 屏幕有 8 列，每条音频轨道 1 列。每个单元格引用一个 chain。一首歌最多可包含 256 行（`00-FF`）。

轨道从左到右排列，它们的 chain 则沿纵向排列。每条轨道相互独立，chain 可以包含 1 到 16 个 phrase，因此轨道之间可能失去同步。设计你的 chain，让它们保持在一起；或者，如果轨道需要，就让它们自由漂移。

通常会保留 1 个 chain 用于空的 phrase，一般是 `00` 或 `FE`。

可以通过创建“岛屿”，即由空行分隔的区段，在一个项目中保留多首子歌曲。

#### Live Mode

连按 **OPT** 两次进入或退出 Live Mode；`LIVE` 取代 Song 标题。在 Live Mode 中，当所选 chain 所在的轨道已停止时，**PLAY** 会在该轨道上启动它。对于正在播放的轨道，它会把所选 chain 排入队列，等到下一个 chain 边界；按 **PLAY** 两次，可在下一个 phrase 边界进行紧急切换。若选中的是空的 Song 单元格，则改为排入一次量化的停止。`+` 标记排队的 chain，`!` 标记紧急的 chain，`-` 标记排队的停止。

在多列上选择同一个 Song 行，可以把这些轨道一起排入队列或停止。多行选择会被有意拒绝。当 Song 范围循环处于活动状态时，Live 队列会被清除。Live 是一种演出模式，不会保存在项目中。

#### 操作

- **OPT + [UP/DOWN]**：向上或向下跳 16 个位置
- 选择一个范围，然后按 **SHIFT + EDIT**：浅克隆 chain，同时保留它们原来的 phrase
- 选择一个范围，然后连按 **SHIFT + EDIT** 两次：同时深克隆 chain 和它们的 phrase
- 选择一个范围，然后按 **EDIT + [UP/DOWN]**：向上或向下移动所选内容
- 点按 **OPT** 3 次：添加或移除高亮，用于视觉整理
- **OPT + SHIFT**：把当前轨道或所选列静音（先松开 **OPT**，静音才会保持）
- **OPT + PLAY**：把当前轨道或所选列设为独奏（先松开 **OPT**，独奏才会保持）
- **OPT + [LEFT/RIGHT]**：把当前轨道左侧或右侧的所有轨道设为独奏
- 选择一个范围，然后连按 **EDIT** 两次：打开 BOUNCE TO SAMPLE 屏幕（见下文的 Bounce）

#### Bounce

选择一个由 Song 行和列组成的范围，然后连按 **EDIT** 两次，打开 BOUNCE TO SAMPLE 屏幕。Bounce 包含所有选中的轨道：第一个 chain 在选区中较晚才开始的轨道，会静默等待，并在其第一个 chain 处加入，因此文件包含所有所选轨道的 chain。

该屏幕提供：

- **File name**：用字符键盘编辑。屏幕打开时，这里会建议下一个可用的序号（`001`、`002`、...）：即导出文件夹中尚不存在其 `.wav` 文件的第一个编号。可以随意编辑：名称会按原样使用。如果已存在同名文件，则会添加 `_001`、`_002`、... 后缀，因此现有文件绝不会被覆盖。加载或新建项目时，计数器会重置。
- **Sample rate** 和 **Bit depth**：与导出屏幕相同的选项
- **Include in a sample name**：三个复选框，在按下 **Start** 时把上下文标签加在文件名之前（默认全部关闭）：
  - **[BPM]** - 项目当前的速度，例如 `[120]`
  - **[Key]** - 项目的根音和音阶，例如 `[Cmaj]`、`[F#min Pent]`
  - **[Bars:Beats:16ths]** - bounce 的渲染长度：一个 phrase 等于一小节（16 个十六分音符），一拍是 4 个十六分音符。完整的 phrase 只显示小节数，例如 `[2]`；不完整的 phrase 显示为 `[0:2]`（两拍）或 `[0:1:3]`（一拍加三个十六分音符）。对于多轨 bounce，以最长的轨道为准。在拍中间结束的选区少算一个十六分音符：所选的最后一行是切点，因此它的 note-off 落在前一个十六分音符的末尾。
  - 标签按 `[BPM][Key][Length]` 的顺序加在文件名之前。
- **Start**：把所选内容渲染为导出文件夹中的一个 WAV 文件（见 Project 屏幕下的 *Export location*）
- **Cancel**：不做 bounce，直接返回

Bounce 渲染期间，**OPT** 会取消它。Bounce 完成或被取消后，会回到你开始 bounce 的屏幕。

在桌面版上，key jazz 还带来了这里的十六进制索引的直接输入、与 Phrase 相同的 Shift+方向键选择、Delete/Backspace/Insert 和 Ctrl+C/X/V - 参见 [Key jazz](#key-jazz-仅限桌面端)。

### Chain

Chain 是由 16 步 phrase 组成的有序列表，可选移调。它最多可包含 16 个 phrase，同一个 phrase 可以出现多次。第 2 列以半音为单位设置移调。

在项目中被重复使用的 chain 旁边会出现星号（`*`）。可以在 Song 屏幕上克隆 chain。

#### 操作

- **OPT + [LEFT/RIGHT]**：在轨道之间移动
- **OPT + [UP/DOWN]**：在当前轨道的 chain 之间移动
- 选择一个范围，然后按 **SHIFT + EDIT**：克隆 phrase
- 选择一个范围，然后连按 **EDIT** 两次：为当前轨道所选的 chain 行打开 BOUNCE TO SAMPLE 屏幕（见 Song 下的 Bounce）

在桌面版上，key jazz 还带来了这里的十六进制索引的直接输入 - 参见 [Key jazz](#key-jazz-仅限桌面端)。

### Phrase

Phrase 是传统步进音序器中的轨道模式。

一个 phrase 有 16 步，每步 1 行。每一行包含一个音符、一个乐器、`00` 到 `7F` 的音量，以及 FX 列。音符使用 Tracker 记谱法，例如 `C-4`（音名和八度）。

要停止一个音符，插入 `NOTE OFF`，使用 kill-note FX，或在未设置乐器的情况下弹奏另一个音符。

当 chain 包含多个 phrase 时，Phrase 屏幕的行为就像一个连续的模式编辑器。移到 `F` 行之下或 `0` 行之上，会打开 chain 中的下一个或上一个 phrase。

在项目中被重复使用的 phrase 旁边会出现星号（`*`）。可以在 Chain 屏幕上克隆 phrase。

FX 选择器显示常用的命令，加上该行乐器所支持的命令。这样就避免了一个按引擎区分命令的长长的全局列表。ChooChooTracker 支持的 FX 比 ChipNomad 更多。

### 操作

- 在空音符上按 **OPT + EDIT**：插入 `NOTE OFF`
- 在 FX 名称列上按 **EDIT + [UP/DOWN]**：打开 FX 选择屏幕
- **OPT + [LEFT/RIGHT]**：在轨道之间移动
- **OPT + [UP/DOWN]**：在当前 chain 的 phrase 之间移动
- 在乐器列中选择一个范围，然后按 **SHIFT + EDIT**：克隆乐器
- 选择一个范围，然后按 **EDIT + [UP/DOWN]**：轮转 phrase 的行
- 选择一个范围，然后连按 **EDIT** 两次：为当前轨道所选的 phrase 行打开 BOUNCE TO SAMPLE 屏幕（见 Song 下的 Bounce）

### Key jazz (仅限桌面端)

在 Phrase 屏幕上按 **Esc** 切换 key jazz，这是一种 QWERTY 钢琴风格的音符输入模式（类似 [m8c](https://github.com/laamaa/m8c)）：

- `Z S X D C V G B H N J M` 和 `Q 2 W 3 E R 5 T 6 Y 7 U I 9 O 0 P` 以半音阶方式输入音符，跨越两个多八度，使用每个键的物理位置，因此该布局在 AZERTY、QWERTY 和 QWERTZ 键盘上是相同的。
- 每次按键会把音符写在当前行，并移到下一行。
- `[` / `]` 移动八度。
- **Shift + 方向键** 选择一个由行和列组成的范围；之后单独按一下方向键会清除选区。
- **Ctrl+C** / **Ctrl+X** 复制或剪切所选的列，如果没有选择任何内容，则是当前行的音符/乐器/音量；**Ctrl+V** 在光标处粘贴。
- **Delete** 删除整行（所有列），并把 phrase 的其余部分上移以填补空缺；光标停留在同一行。
- **Backspace** 仅在当前列（若有选区，则是选区的各列）中删除光标下的元素，把其下方的内容上移以填补空缺，并像文本编辑器那样让光标后退一行。
- **Insert** 插入一个空行，把 phrase 的其余部分向下推。
- **Ctrl+S** 保存项目。在 key jazz 之外，Ctrl+S 不起作用。
- 再按一次 Esc 关闭 key jazz。

当 key jazz 开启时，它会完全接管 Phrase 屏幕上的音符键，包括共用这些键的 Edit/Opt/Motion 快捷键 - 用 Esc 关闭它，才能再次使用这些快捷键。

Key jazz 还可以在以下界面中使用，并用 Esc 独立切换：

- **Song** 和 **Chain**：直接输入 chain 或 phrase 的十六进制索引（`0-9`、`A-F`），而不是用 Up/Down 递增。在单元格上输入的第一个数字会替换其值；不移动而继续输入的后续数字会推入其中（输入“3”再输入“F”得到 3F）。在 Song 上，**Shift+方向键** 选择一个范围，**Delete** 删除整行并上移其余部分，**Backspace** 删除当前/所选的列并上移其下方的内容，**Insert** 插入一个空行，**Ctrl+C**/**Ctrl+X**/**Ctrl+V** 复制/剪切/粘贴所选的行（若无选择则是当前行）- 与 Phrase 屏幕相同的结构编辑快捷键。这会接管 Shift，因此在这里 key jazz 处于活动状态时，Shift+Right/Up 不再跳转到 Chain/Project - 按 Esc 可恢复这些快捷键。
- **每个带文本字段的屏幕**：直接在键盘上输入，而不是使用屏幕上的虚拟键盘。这涵盖 Project 的文件名、标题和作者，乐器名称，配色主题名称，音高表名称，bounce 文件名，以及名称对话框（输入名称、创建文件夹）。Backspace 的行为与普通文本字段相同；Shift 输入大写字母。Esc 会同时在所有这些屏幕上切换它，而当 key jazz 关闭时，虚拟键盘仍照常打开。

## 5. Instruments

所有乐器都有一些共同的参数：

- 乐器类型
- 名称，最多 15 个字符
- 默认的 table 速度，以每个 table 行的 tick 数计
- 移调开或关

每个乐器都有一个默认 table，编号与它相同，范围在 `00-7F`。你可以把它们用作辅助 table，但把辅助 table 放在 `80-FE` 范围内可以避免冲突和混淆。Table 是 ChooChooTracker 主要的声音设计工具之一。

### 操作

- **OPT + [LEFT/RIGHT]**：在乐器之间移动
- **EDIT + PLAY**：试听乐器
- **SHIFT + OPT**：复制乐器
- **SHIFT + EDIT**：粘贴乐器

### AY Classic、AY Plus 和 AY Sample

这些是 ChipNomad 原有的 AY/YM 引擎。AY Classic 提供硬件风格的音调、噪声和包络控制。AY Plus 增加了软件振荡器和更丰富的调制。AY Sample 通过 AY 风格的音量级别回放采样，与 Sampler 引擎不同。

#### AY Classic

这是 ChipNomad `v1.0.0` 遗留下来的乐器类型。较新的 AY Plus 乐器提供更多功能。

AY Classic 乐器有这些参数：

- Mixer：音调开或关，噪声开或关，包络形状（`0-F`）
- Volume：由软件生成的 ADSR 包络
- 自动包络周期：开或关，比例为 `1:1` 到 `F:F`

Settings 中的 **AY Quality** 只影响 AY/YM 的渲染，不会改变 Braids、Plaits 或 Sampler 的质量。**Sample dithering** 适用于 AY Sample 的量化。

芯片的详细信息请参见 [ChipNomad 的 AY-3-8910 文档](https://chipnomad.org/chips/ay-3-8910/)。

#### AY Plus

AY Plus 把经典的合成器概念应用于 AY-3-8910/YM2149F 的声音设计。你可以把 AY 看作一台有 5 个振荡器的合成器：

- 3 个方波音调振荡器
- 1 个噪声振荡器
- 1 个可以用作振荡器的幅度包络发生器

按照这个模型，每个乐器有 3 个硬件振荡器：音调、噪声和包络。AY Plus 增加了第 4 个软件振荡器。以高于项目 tick 速率的频率更新 AY 寄存器，会打开更多声音设计的可能。由于 AY 的工作方式，这些振荡器是以环形调制方式混合的。尝试不同的音高偏移、失谐和振荡器组合，看看会得到什么。

每个音调振荡器都可以用音高偏移和微调偏移独立调音。

软件振荡器的类型及其参数：

- Pulse - 脉宽可控的方波
  - FM Depth - 简单 FM 的深度
  - Pulse width - 完整范围是 `00-FF`，而步数在 Project 屏幕上设置
  - Pulse low - 控制重复对中的低值，其高值始终为 `15`
- Sync Tone - 用于音调振荡器的硬同步效果
  - FM Depth - 简单 FM 的深度
- Sync Env - 用于包络振荡器的硬同步效果
  - FM Depth - 简单 FM 的深度
  - Pulse width - 包络对中两种形状的时长比例
  - Envelope Pair - 在其间切换的 2 种包络形状
- Wavetable - 32 步的波表，在 Wavetable 屏幕上编辑
  - FM Depth - 简单 FM 的深度
  - Wavetable index
- Tone FM - 用于音调振荡器的类 FM 简单效果
  - FM Depth - 音调 FM 的深度
- Env FM - 用于包络振荡器的类 FM 简单效果
  - FM Depth - 包络 FM 的深度

Pulse 和 Wavetable 软件振荡器不能与 Envelope 振荡器同时使用。Envelope 振荡器会自动关闭。

简单 FM 会在 2 个周期值之间快速切换，一个较低，一个较高。这类似于把方波用作 FM 调制器。Pulse、Sync Tone、Sync Env 和 Wavetable 在 FM 载波与调制器之间使用 `1:1` 的比例。Tone FM 和 Env FM 可以通过软件振荡器的音高偏移来模仿其他比例。

#### AY Sample

可以加载 `8 位`、`16 位`、`24 位` 或 `32 位` 的单声道或立体声 WAV 文件。它们会被转换为 `8 位`，并截断到 `16 KB`。这个限制与 ZX Spectrum 的 RAM 页大小一致。

AY 通过对音量寄存器的快速写入来回放采样。它的输出是单极性的，所以采样需要“抬到零”，才能获得更干净的回放。这样可以消除采样末尾的爆音，并改善安静的尾音。

AY 的 DAC 分辨率为 `4 位`，音量刻度是非线性的，因此 `8 位` 采样在回放时会被转换为 `4 位`。Settings 中的 Sample dithering 选项可以改善感知的位深。抖动在 ZX Spectrum 和 Atari ST 这类复古平台上并不实用，所以为真实硬件创作，或追求粗粝的 lo-fi 声音时，请让它保持关闭。

采样的行为类似于 AY Plus 的另一个软件振荡器，但其额外的参数使它更适合作为单独的乐器类型来处理。

采样参数：

- Sample rate - 以原生速率回放采样会产生 `C-4`
- Sample start
- Sample length
- Sample loop start
- Sample pitch offset
- Sample fine pitch offset

Tone 和 Noise 振荡器的部分与 AY Plus 相同。由于 AY 的工作方式，所有振荡器都是以环形调制方式混合的。

### Braids

Braids 提供 47 种合成模型。它的主要控制有：

- **Model**：振荡器或合成算法
- **Timbre** 和 **Color**：取决于模型的宏参数
- **Filter**：额外的 LP/BP/HP 滤波器，斜率为 `12` 或 `24 dB`，带截止频率和共振
- **ADSR**：attack、decay、sustain 和 release

点按 **Model** 可从分类的模型列表中选择。

| 类别 | 模型 |
|---|---|
| Analog | `00 CSAW`, `01 MORPH`, `02 SAW-SQUARE`, `03 SINE-TRI`, `04 BUZZ`, `05 SQUARE-SUB`, `06 SAW-SUB`, `07 SQUARE-SYNC`, `08 SAW-SYNC` |
| Multi Osc | `09 TRIPLE-SAW`, `10 TRIPLE-SQR`, `11 TRIPLE-TRI`, `12 TRIPLE-SINE`, `13 TRIPLE-RING`, `14 SAW-SWARM`, `15 SAW-COMB`, `16 TOY` |
| Filter / Voice | `17 FILTER-LP`, `18 FILTER-PEAK`, `19 FILTER-BP`, `20 FILTER-HP`, `21 VOSIM`, `22 VOWEL`, `23 VOWEL-FOF`, `24 HARMONICS` |
| FM / Chaos | `25 FM`, `26 FEEDBACK-FM`, `27 CHAOTIC-FM` |
| Physical | `28 PLUCKED`, `29 BOWED`, `30 BLOWN`, `31 FLUTED`, `32 STRUCK-BELL`, `33 STRUCK-DRUM` |
| Drums | `34 KICK`, `35 CYMBAL`, `36 SNARE` |
| Wavetables | `37 WAVETABLES`, `38 WAVE-MAP`, `39 WAVE-LINE`, `40 WAVE-PARA` |
| Noise / Granular | `41 FILTER-NOISE`, `42 TWIN-PEAKS`, `43 CLOCK-NOISE`, `44 GRAN-CLOUD`, `45 PARTICLE`, `46 DIGI-MOD` |

#### 参数

Timbre 和 Color 取决于所选的模型。完整的模型列表和详细信息，请参阅 Mutable Instruments 的 Braids 原版手册，或者直接凭耳朵试。

#### VCF 与 VCA

Braids 馈入 ChooChooTracker 标准的多模式滤波器和 ADSR 包络。

#### 设置

全局的 Braids 设置位于应用设置中：

- **BITS**：降低输出分辨率
- **DRFT**：为振荡器增加音高不稳定性
- **SIGN**：重现原版 Braids 的波形不完美算法

### aChChid

**aChChid** 是基于 Open303 的单声部 Acid 贝斯引擎。`Square` 和 `Saw` 使用其原生的 TB-303 振荡器、滤波器、包络和重音行为。`Braids` 只替换振荡器，随后仍走同一条 303 滤波器和放大器通路。它提供 Model、Timbre、Color 和 Shaper，取代 Fine tune。Shaper 从柔和的饱和逐步过渡到波形折叠（wavefolding）。aChChid 不使用 ChooChooTracker 的统一后置滤波器或 ADSR。

Cutoff 和 Decay 采用对数方式编辑：**EDIT + LEFT/RIGHT** 移动一个音乐性的控制步长，而 **EDIT + UP/DOWN** 移动十六个步长。Cutoff 覆盖 200 Hz 到 20 kHz；Decay 覆盖 200 ms 到 2 s。

音符音量列中的 `F` 会触发重音。普通音符使用半步的门限（在默认的 6 tick groove 中为 3 个 tick）。`ASL` 从前一个音高滑向该音符，而不重新触发 303 的包络；它会自动保持前一个门限，`ASL 00` 产生 `60 ms` 的滑音。`ATY` 让其音符在整个当前步内保持开启。没有 `ASL` 的音符总会重新触发。调制目标包括 Decay 和 Accent，以及 Braids 波形模式下的 Timbre 和 Color。

### Bogie

**Bogie** 是原生的单次触发（one-shot）鼓合成器。Tracker 的音符设定音高，Tracker/乐器的音量设定电平；每次触发都有自己的内部衰减，所以没有 ADSR 页面。它有 12 种模型：`Kick`、`Snare`、`Hat`、`Clap`、`Tom`、`Rim`、`FM`、`Noise`、`Cowbell`、`Cymbal`、`Shaker` 和 `Clave`。

它的六个宏是 **Decay**、**Tone**、**Sweep**、**Noise**、**FM** 和 **Drive**。其标签会在有用的地方做出调整（例如，Kick 的 Noise 是 `Click`，Cowbell 的 FM 是金属感的交叉调制）。每个宏在每种模型上都有效，但其音乐作用会随模型而变：Sweep 可以改变音高、爆发间隔或金属感的扩散；FM 可以增加交叉调制、金属层或类环形调制的泛音。每个范围的下半部分适合常规的鼓声；上半部分则有意打开更宽的音色变形和更合成的效果。

每种模型都可以使用共享的 LP/BP/HP 多模式滤波器，具有 Clean、Classic、Aggro 或 Acid 的特性、`12`/`24 dB` 斜率、截止频率和共振。Bogie 支持其所有活动的宏，以及 Volume、Pitch、Cutoff 和 Resonance，作为调制和动作录制的目标。在 Cowbell 上使用较短的 Decay 可得到尖锐的 phonk 攻击；增加 FM 则得到更硬、更有金属感的铃声。

在 Modulation 屏幕上，在某个源上按 **EDIT** 先选择一个类别（Envelopes、LFO 或 Sticks），再选择源。**Left** 和 **Right** 仍然可以直接循环切换各个源。

### Sintered

**Sintered** 是原生的单次触发数字打击乐合成器。音符设定其基频；它的六种模型是 `Knot`、`Shard`、`Burst`、`Comb`、`Logic` 和 `Melt`。每种模型都有 **Decay**、**Mod** 和 **Motion**，外加三个模型专属的控制：Knot 使用 Ratio/Spread/Fold，Shard 使用 Ratio/Feedback/Bite，Burst 使用 Noise/Color/Feedback，Comb 使用 Time/Damping/Regen，Logic 使用 Rate/Pattern/Crush，Melt 使用 Ratio/Chaos/Drive。**Mod** 始终是内部的耦合深度。

Motion 是内部的宏包络，不是音高滑音：低于 `80` 的值使用 AD 形状，`80` 将其关闭，高于 `80` 的值使用只有衰减的形状。越远离中心，包络越快。它的目标按模型固定，因此 Sintered 在 Tracker 的模式中保持可预测：Knot 让 Mod/Fold 动起来；Shard 是 Feedback/Bite；Burst 是 Noise/Color/Feedback；Comb 是 Damping/Regen；Logic 是 Rate/Crush；Melt 是 Chaos/Drive。所有模型在音符触发时都会重置状态，并限制内部反馈，以获得可重复的打击。

### MME

**MME**（Multi Modulation Engine）是一台专注于振荡器之间相互作用的双振荡器合成器。它的模型有 `Ring`、`Fold`、`Cross`、`VPM`、`Sync`、`Logic` 和 `Vocode`。两个振荡器都是内部的，并跟随 Tracker 的音符；`Interval` 设定它们之间的关系。

六个宏是 **Waves**、**Interval**、**Amount**、**Flow**、**Feedback** 和 **Shaper**。Waves 选择音乐性的 A/B 波形组合。Amount 控制模型的核心相互作用，Flow 改变其方向或算法变体，Feedback 是有限制的，但在范围末端会有意变得狂野。Shaper 是共享的最终级，从干净过渡到饱和，再到波形折叠。

MME 使用共享的 LP/BP/HP 多模式滤波器和 ADSR。六个宏、Cutoff 和 Resonance 都可用于调制和动作录制。在 Model 上按住 **EDIT** 可打开模型弹窗。

### MIDI Out

**MIDI Out**（仅限桌面端）驱动外部 MIDI 设备，而不是合成音频：触发一个音符会在乐器的 **Channel**（`1-16`）上发送真正的 MIDI Note On，音符结束时发送相应的 Note Off。音量变成力度，和弦轨道的每个声部各自得到自己的 Note On/Off。ChooChooTracker 本身没有声音 - 请先在 [Settings](#12-settings) 中选择输出设备。完整的说明，包括用 MIDI 键盘试听声音，请参见 [MIDI](#14-midi)。

**Program** 和 **Bank high/low**（CC0/CC32）是可选的 - 关闭时显示为 `--`，`Clear` 会关闭某个字段并记住它的值。设置之后，只要它们与上次告知该通道的内容不同，就会在下一个音符之前发送一次（而不是在每个音符之前，那会重新触发接收设备自己的包络或音色）。

**MC1 number**-**MC4 number** 选择四个行 FX MC1-MC4 各自发送到哪个 CC 编号（`00-7F`） - 参见 [MIDI Out FX](#midi-out-fx)。同样是 `--`/可选；留空的槽位会使其 FX 不起作用。

### 减法合成引擎

这一类引擎共享 VCO 到 VCF 再到 VCA 的架构。

可用的 VCO 类型有若干种：

- **Braids**
- **Plaits**
- **Plaits-Alt**
- **Sampler**
- **2xSCWF**
- **BYOWTBL**

每个乐器把它的 VCO 馈入一个多模式滤波器。

滤波器的斜率可在 `12 dB` 和 `24 dB` 之间切换，可以用作低通（LP）、带通（BP）或高通（HP）滤波器。

滤波器有几种特性可选：

- **Clean**：冷峻而数码
- **Classic**：受经典美式模拟合成器启发
- **Aggro**：受咆哮的日式模拟合成器启发
- **Acid**：受迷幻药启发

在这个 Alpha 版本中，Classic、Aggro 和 Acid 可能会产生很强的共振。请从低值开始，慢慢调高。

滤波器馈入由 ADSR 包络控制的 VCA。包络形状可以在指数、线性和对数曲线之间渐变。

#### Plaits 与 Plaits-Alt

**Plaits** 提供 24 个引擎，在选择弹窗中按如下方式分组：

| 类别 | 引擎 |
|---|---|
| Analog / Waves | `00 VA VCF`, `01 PHASE DIST`, `05 WAVE TERRAIN`, `06 STRING MACH`, `08 VIRTUAL ANALOG`, `09 WAVESHAPING`, `11 FORMANT`, `12 HARMONIC`, `13 WAVETABLE`, `14 CHORD` |
| FM | `02 6-OP FM 1`, `03 6-OP FM 2`, `04 6-OP FM 3`, `10 2-OP FM` |
| Digital | `07 CHIPTUNE`, `15 SPEECH` |
| Texture / Noise | `16 SWARM`, `17 NOISE`, `18 PARTICLE` |
| Physical | `19 STRING`, `20 MODAL` |
| Drums | `21 BASS DRUM`, `22 SNARE DRUM`, `23 HI-HAT` |

**Plaits-Alt** 是为原版 Plaits 模块准备的一组替代模型。其中大多数比一般的 VCO 更古怪或更具实验性。

| 类别 | 引擎 |
|---|---|
| Granular / Micro | `GLISSON`, `PULSAR`, `GENDY`, `SCANNED`, `LOOPBACK` |
| Phase / Harmonic | `PHASE WEAVE`, `SIDEBAND BANK`, `UNDERTOW`, `ATTRACTOR`, `LOCKSTEP` |
| Acoustic / Physical | `REED PIPE`, `BRASS`, `SHAKERS`, `CLAPS`, `FRESHETS FORMANT` |
| Polyphony / Harmony | `DIATONIC CHORD`, `SCALE STACK`, `WT DIATONIC CHORD`, `WT SCALE STACK`, `HELIX` |
| Digital / Weird | `BYTEBEAT`, `RULEFIELD`, `SPECTRAL SPIRAL`, `PHASE FLOCK` |

这些参数遵循 Mutable 的设计：

- **Harmonic** 控制谐波关系、平衡，或引擎内的模型选择。
- **Timbre** 通常从暗淡/稀疏的频谱变化到明亮/密集的频谱。
- **Morph** 探索音色的另一个维度。
- **Main/Aux** 把主输出与引擎的备用输出混合。

它们的确切含义取决于引擎。例如，和弦引擎用它们来控制和弦类型、转位和波形。物理模型用它们来控制材质、激励和衰减，而鼓引擎用它们来控制音调、特性和衰减。详情请参阅 Plaits 用户手册。

对于 `CHORD`、`DIATONIC CHORD` 和 `WT DIATONIC CHORD`，Harmonic 显示所选和弦的名称（`MIN`、`MAJ`、`MIN7`、`MAJ7` 等），而不是其原始的控制值。Braids 的 `WAVE PARA` 同样为其 Color 的和声排列命名。

点按 **Engine** 可从分类的引擎列表中选择。

**Env Mode** 有 2 种路由：

- `TRIG` 重现连接了 TRIG、LEVEL 未接线时的模块
- `VCA` 保持 LEVEL 打开，并在声部之后应用 Tracker 的 ADSR

#### Sampler

这个干净的采样引擎回放加载到 RAM 中的单声道或立体声 PCM 采样。

- 点按 **Sample** 加载未压缩的 `8 位`、`16 位` 或 `24 位` PCM WAV。在浏览器中按 **PLAY** 试听高亮的文件。
- 加载采样后，其名称旁会出现 **EDIT**。点按它打开 Sample Edit 屏幕。**OPT** 或 **SELECT + [LEFT]** 返回乐器。
- Sample Edit 屏幕显示文件名（当 RAM 中的采样与磁盘上的文件不同时，带有 `*` 标记）、格式读数（采样率、声道数）、一个高大的波形，以及 **Region**、**Select**、**Slice**、**Process** 和 **File** 字段。
- 编辑只存在于 RAM 中，直到某个保存流程把它们写出：项目保存的是采样的文件路径，并在下次加载项目时从磁盘重新加载 WAV，所以没有 **Save**/**Save As** 就离开 Tracker 会丢弃处理操作（`*` 标记就是警告）。向乐器加载另一个采样，也会丢弃未保存的编辑，并把 Region 重置为整个采样（Start `00`，End `FF`）。
- **Region** 设置回放的 Start/End 标记，在 Sampler 乐器屏幕上也可以设置。它们设定归一化的回放边界（`00-FF`）；如果 Start 在 End 之后，采样会反向播放。**EDIT + [LEFT/RIGHT]** 以 1（`01`）为步长精细调整标记，并在按住 **EDIT** 期间围绕它放大波形；**EDIT + [UP/DOWN]** 进行粗调（步长 16），并让视图回到整个采样。**EDIT + OPT** 把标记重置为默认值（Start `00`，End `FF`）。
- 波形视图会放大到围绕所编辑标记的、固定的一秒音频窗口，并平移以保持它可见；能放进窗口的采样保持完整的 1:1 视图。放大是暂时的：只在按住 **EDIT** 期间有效，松开 **EDIT** 会回到完整的 1:1 视图，粗调步进也是如此。进入该屏幕总会把视图重置为整个采样。
- **Select** 以绝对帧设置处理选区，独立于回放的 Start/End 标记。**EDIT + [LEFT/RIGHT]** 把手柄移动二十帧，并在按住 **EDIT** 期间放大到它；**EDIT + [UP/DOWN]** 跳转 `frameCount/64`（最少 16）并回到整个采样的视图。在手柄上 **EDIT**（点按）会把当前 Start 或 End 标记的位置复制给它。在任一手柄上按 **EDIT + OPT** 会清空整个选区。选区为空时，两个手柄都显示 `-`；手柄反转时，会自动互换。选区是仅限会话的编辑器状态：它不随项目保存，进入屏幕时会用回放 Region 的跨度初始化（使用默认标记时就是整个采样）。
- **Slice** 是一行，包含四个数值单元格 - **Mode**、**Count**、**Slice** 和 **Frame** - 并随乐器一起保存。Off 播放 Start/End 窗口。Phrase 的音符从 **C-0** 开始按半音阶选择切片，超过最后一个切片的音符会绕回开头（音符 N 播放切片 `N % count`，所以超出最后一个切片的连续音符会循环回到第一个）。已切片的音符不会移调，也不使用 Scale 量化，但 **CRD** 会保留所选的切片，并把它的声部作为和弦移调。切片起点在波形上用两像素宽的短线标出（深橙色；当前切片的线更亮），光标下的切片在波形上会有一条黑色背景带（选区的着色不会画在这条带上，所以它保持纯黑，与预览的其余部分明显区分）。
  - **Mode** 循环 `OFF` / `EQUAL` / `AUTO` / `LAZY`（EDIT + 左/右，或点按；EDIT + OPT 关闭切片）。切换模式会初始化切片点：**EQUAL** 把 Start/End 窗口均分为 **Count** 份；**AUTO** 在窗口上运行瞬态检测，并在发现的每个起音处放置一个切片（最多 **Count** 个）；**LAZY** 从一个覆盖整个窗口的切片开始。切换到 LAZY 会暂存当前的 EQUAL/AUTO 设置（数量和边界），切回 EQUAL 或 AUTO 时原样恢复 - 不会要求确认，也不会丢失任何东西。Slice 和 Stretch 互斥：启用其中一个会禁用另一个，并且当 Stretch 控制时长时，Slice 单元格会变暗。
  - **Count** 显示切片的总数（`01`..`64`）。**EDIT + [LEFT/RIGHT]** 以 1 为步长改变它（最小 1，最大 64），**EDIT + [UP/DOWN]** 循环 2 的幂次数量 `2 / 4 / 8 / 16 / 32 / 64`，并回绕。每次更改都会重新计算划分：**EQUAL** 重新均分窗口，**AUTO** 以新的目标重新运行检测（检测灵敏度由数量推出 - 切片越多，阈值越灵敏）。模式为 OFF 时，该框显示 `-`。在 **LAZY** 中它保持明亮（它统计手动放置的切片），但只用于显示：光标会跳过它 - 从 **Mode** 向右或从 **Slice** 向左移动会落在 **Slice** 上，从 **Select** 行的 **END** 标记向下移动也会落在那里，点按该框什么也不会发生（手动放置的切片没有可重新计算的划分）。在这里 **EDIT + OPT** 和单纯的 **EDIT** 点按都不起作用：关闭切片在 **Mode** 上进行，删除切片在 **Slice** 上进行。
  - **Slice** 是浏览器：它显示当前切片，从 1 开始编号（`01`..count）。**EDIT + [LEFT/RIGHT]** 一次移动一个切片，**EDIT + [UP/DOWN]** 一次跳四个切片（都有边界限制，不回绕）；所浏览的切片在波形上用更亮的标记和黑色带指示，放大的视图会重新以切片起点为中心。**EDIT + OPT** 删除当前切片（它会合并到前一个切片；第一个切片无法删除，删除最后剩下的切片会关闭切片 - 边界在经过 OFF 之后依然保留，再次选择模式时会回来）。**EDIT**（点按）以单次回放试听当前选中的切片（在每种切片模式下都有效；松开按键或离开屏幕时试听停止）。模式为 OFF 时，该框显示 `-`。
  - **Frame** 以十六进制显示当前切片的起始帧。**EDIT + [LEFT/RIGHT]** 把它移动十帧（按住 **EDIT** 期间放大到标记），**EDIT + [UP/DOWN]** 移动一百帧并回到完整视图。标记停留在它自己的切片之内：不能越过前一个或后一个切片的起点。**EDIT + OPT** 删除该切片。
  - **B** 像这个屏幕上其他地方一样，从 Slice 行回到乐器屏幕；**SHIFT + LEFT** 可以从任何位置退出。
  - **LAZY 工作流程**：选中 LAZY 后，点按 **PLAY** 开始整个采样的单次回放 - 它从头播放一次，并以采样的原始音高和速度自行停止（一个绿色的两像素标记在波形上跟随位置）；再次点按 **PLAY** 会提前停止。播放期间，无论光标在哪里，每次点击 **EDIT** 都会在回放位置放下一个切片（与已有切片相距不到 50 ms 的切片会被 `Too close to slice` 拒绝）。再次播放采样会保留现有的切口，并在每个点按的位置依次添加新的切口 - **Count** 框随每个放下的切片刷新，**Slice** 浏览器跟随新的切片。与其他模式相同的浏览和编辑规则适用于手动放置的切片：在 **Slice** 上用 **EDIT + 方向键** 浏览它们，**Frame** 调整所选的起点，**EDIT + OPT** 删除当前切片。当播放中放置处于待命状态时，Frame 单元格会变暗。**SHIFT + PLAY** 在 LAZY 中不会被拦截：它像在其他屏幕上一样开始 phrase 的播放。LAZY 切片在音序器中的回放方式与 EQUAL 和 AUTO 完全相同：phrase 的音符从 **C-0** 开始按半音阶映射到手动放置的切片，超过最后一个后回绕。
  - **组合键提示**：当光标停留在 Slice 行的某个单元格上，且没有其他消息显示时，状态栏会显示该单元格的提示，并在光标停留期间一直保持。在 **Mode** 上，提示绑定到活动模式，只显示描述（模式名称已在单元格中）：`Divides sample in equal parts`、`Divides sample based on transients`、`Press Play and add slices with EDIT` - 切换模式会立即替换文字，OFF 则清空状态栏。在 **Count** 上显示 `Adjust the number of slices`，在 **Slice** 上显示 `Browse slices`，在 **Frame** 上显示 `Adjust slice start`。任何操作消息都会取代提示，直到它消失。
- **Process** 选择一个破坏性的编辑操作：**Crop**、**Normalize**、**Delete**、**Silence**、**Fade In**、**Fade Out** 或 **Reverse**（EDIT + 左/右循环，点按向前循环，EDIT + OPT 设为无）。同一行的 **GO** 对当前选区执行所选操作；选区为空时对整个采样执行。**Crop** 只保留选区；**Normalize** 缩放选区，使其峰值达到满量程（两个声道共用一个增益，以保持立体声像）；**Delete** 删除选区并把两端接上（拒绝删除整个采样）；**Silence** 把它归零；**Fade In**/**Fade Out** 让选区从静音线性渐入或渐出到静音；**Reverse** 让选区倒放（帧就地互换，一帧的两个声道一起移动，长度和标记保持不变）。Crop 和 Delete 需要选区 - 选区为空时显示 `Select region first`。每个操作都会短暂暂停音频，保留一级撤销，并在 RAM 中把采样标记为已更改：磁盘上的文件要到 Save 流程（见下文）才会被改动，离开屏幕会丢弃撤销槽。
- **UNDO**（在 GO 旁边）把采样换回上一次操作之前的状态。在执行操作之前它是暗的，反复按下时会在操作前和操作后的状态之间切换，重新进入屏幕时被清除。
- **File** 容纳保存流程。乐器保存它所加载的 WAV 的完整路径；这些流程会写入该文件，或让乐器指向一个新文件。它们从不触碰乐器名称。
  - File 字段在 **Save** 和 **Save As** 之间循环（EDIT + 左/右，或点按）；同一行的 **GO** 执行所显示的操作。
  - **Save** 覆盖采样加载来源的 WAV。对于没有切片的采样，会先询问 `Overwrite <name>?`。已切片的采样会先打开 **SAVE SLICES** 对话框，询问切片点应归属何处：**SAVE TO SAMPLE** 把它们作为 cue 块写入 WAV（可移植 - DAW 会把它们显示为标记），同时也保留在项目中；**SAVE TO PROJECT** 写入一个普通的 WAV，只在项目中保留这些点；**CANCEL** 中止。`Don't ask again in this project` 复选框会把所选的目的地与项目一起保存，此后跳过该对话框（该选择以 `- Sample save choice:` 保存在 .cct 中，只能通过编辑项目文件来更改）。没有切片的采样会跳过该对话框。在加载带有文件路径的采样之前，**Save** 是暗的（在导航中也会被跳过）。写入失败会保留已修改标记，并显示错误。
  - **Save As** 把当前采样写入一个新文件：输入文件名（预填当前名称，不含扩展名），然后在浏览器中选择一个文件夹。采样被写为 `<folder>/<name>.wav`，乐器指向新文件，该文件夹被记住为默认的采样文件夹。在加载采样数据之前它是暗的（在导航中也会被跳过），因此刚加载的采样也可以获得文件路径。超过 255 个字符的路径会被拒绝。
  - 文件名前的 `*` 标记表示 RAM 中的采样与磁盘上的文件不同（任何处理操作都会设置它，Save 和 Save As 会清除它）。该标记仅限会话：它不随项目保存，重新进入屏幕时会被重置。如果多个乐器引用同一个 WAV 文件，保存其中一个会为它们全部覆盖该文件。
- 在 Sample 乐器屏幕上，用 **EDIT + [LEFT/RIGHT]** 加载同一文件夹中的上一个或下一个 WAV。
- **Pitch** 以半音移调（`-48` 到 `+48`）。
- **Loop** 选择 Off、Loop 或 Ping-Pong。
- **Speed** 控制颗粒式的时间拉伸，范围 `0%` 到 `500%`（`100%` 为正常）。

不支持的 WAV 格式会显示错误。导入之前，请把不常见的文件转换为 `PCM8`、`PCM16` 或 `PCM24` WAV。

#### 2xSCWF

2xSCWF 是双单周期波形振荡器。向每个振荡器加载 1 个恰好包含 1 个周期的单声道 WAV。两个振荡器都向前读取，并在其周期结束处回绕。

Mix 在 A 和 B 之间交叉渐变。

Detune 从同音到 `+200 ct` 是精细的、指数式的，然后从 `+3 st` 到 `+24 st` 以半音为步长。

2xSCWF 把每个文件视为 1 个周期的表，每个振荡器周期恰好读取一次。这样你就可以混合并失谐 2 个单周期波形。

出厂的 `waveforms/AKWF/` 文件夹包含来自 Adventure Kid 库的波形。如果你想扩充收藏，网上还有很多。

#### BYOWTBL

BYOWTBL 是兼容 Serum 波表的双波表振荡器。每个振荡器加载一个包含连续单周期帧的单声道 WAV。`Pos A` 和 `Pos B` 各自独立地从 `00` 到 `FF` 扫描它们的表。引擎在每帧内以及相邻帧之间做线性插值。`Mix` 在两个振荡器之间交叉渐变，`Detune` 使振荡器 B 产生偏移。

## 6. 调制与动作录制

每个乐器最多可有 4 个调制槽。音频乐器可以以 **Instrument Pan** 或 **Track Pan** 为目标；两者都用 `00` 表示左，`80` 表示中，`FF` 表示右。

可用的调制类型有：

- ADSR - 经典的 attack-decay-sustain-release 包络
- AHD - attack-hold-decay 包络
- LFO - 具有多种波形的低频振荡器
- SLFO - 带周期倍数、用于缓慢周期的 LFO
- FLFO - 可达 kHz 范围的音频速率 LFO；FLFO 对 CPU 的负担可能很重
- SLIN - 线性摇杆控制
- SRAT - 基于速率的摇杆控制

调制目标的列表取决于乐器类型。

对于 `Cutoff` 目标，Amount 是指数式的，带二次响应：25% 达到 4.5 个半音，50% 达到 18 个半音，75% 达到 40.5 个半音，100% 达到六个八度。这样在较小的量时保持精细控制，同时仍允许深度的扫频。

当 ADSR 或 AHD 包络以 Volume 为目标时，它就成为乐器的音量包络。而以 Volume 为目标的 LFO 则是对当前的输出音量做偏移。

调制量可以是正的，也可以是负的。其范围取决于目标。如果目标的完整范围为 `127` 或更小（大多数参数如此），则该量定义的是绝对范围。像音高这样范围更大的目标，使用的是经过缩放的量。

所有时长参数，包括 attack、hold、decay 和周期，都以 tick 计量。

LFO 波形：

- Tri - 双极性三角波
- Sin - 双极性正弦波
- UniTri - 单极性三角波
- UniSin - 单极性正弦波
- RampDn - 线性下降斜坡（锯齿波）
- RampUp - 线性上升斜坡（锯齿波）
- ExpDn - 指数下降斜坡（指数锯齿波）
- ExpUp - 指数上升斜坡（指数锯齿波）
- Square - 方波
- Random - 采样保持的随机波
- Wavetable - 读取项目中 32 步 AY 波表之一。`00` 到 `0F` 的值映射到 `-100%` 到 `+100%`，不做插值。

LFO 触发类型：

- Free - 仅在乐器改变时重新启动 LFO
- Retrig - 在每个音符上重新启动 LFO
- Phrase - 当播放进入新的 phrase 时重新启动 LFO 和 SLFO
- Chain - 当播放进入新的 chain 时重新启动 LFO 和 SLFO
- Hold - 运行 1 个周期后停止，并保持最后的值
- Once - 运行 1 个周期后停止，并回到零

### 动作录制 (Motion recording)

- **STICK LIVE**（默认为 `L1`）在不改变 phrase 的情况下应用摇杆调制。在 **Settings > Stick live mode** 中，`HOLD`（默认）在按住时启用它；`TOGGLE` 按一次启用，再按一次禁用；`FREE` 让它始终保持启用。在 `TOGGLE` 下松开按键，它仍保持启用。
- 播放期间，按住 **MOTION RECORD**（`L2`）以应用摇杆调制，并把改变的目标录制为绝对的 FX 值。
- 按住 **MOTION ERASE**（`R2`），从当前行移除匹配的目标 FX。

右下角的 `~` 表示 Stick live 处于活动状态。Record（`*`，溢出时为 `!`）和 Erase（`x`）具有指示器优先级，并在两种模式下都保持瞬时。按住它们时，二者都会启用摇杆的实时处理；松开它们，则单独启用的实时开关保持不变。

该模式随应用设置一起保存，但实时开关在启动时总是关闭的。更改模式会清除开关：`HOLD` 跟随当前按住的任何 Stick live 按键，`TOGGLE` 等待新的一次按下，`FREE` 立即生效。

动作录制把轨道 FX 写入当前正在播放的 phrase。它先更新匹配的 FX，然后从右到左使用空槽。它从不覆盖不同的 FX。如果 3 个槽都满了，该动作就不会录制到这一步上。右下角的 `!` 表示改变的目标多于 3 个 FX 列所能容纳的数量。

动作录制支持 Braids、Plaits、Sampler、2xSCWF、BYOWTBL 和 Bogie 的目标，在适用的情况下，包括活动的 Bogie 宏和滤波器控制。

## 7. Tables

Table 是 Tracker 中的核心声音设计工具。用 Vortex Tracker 的说法，它们把乐器和装饰音合二为一，但能做的事要多得多。如果你了解 LSDj 或 NerdSEQ，这个想法应该已经很熟悉了。

Pitch 列接受以半音计的相对（`~`）或绝对（`=`）音高值。Volume 列为 `00`-`0F`，应用于每个引擎，叠加在其包络之上，并可用作门限：交替使用 `0F` 和 `00`，然后用 `HOP 00` 循环。4 条 FX 通道与 phrase 中的通道大体一致，不过有少数 FX 在 table 中的行为有所不同。

在 table 最后一行放一个 `TIC` FX，会设置该列的速度，并覆盖乐器的默认 table 速度。每个 FX 列可以以不同的 tick 速度运行。Pitch 和 Volume 列跟随第 1 个 FX 列的速度。

与一个很长的 Vortex Tracker 乐器相比，16 行的 table 看起来可能很短，但 `HOP` FX 可以创建诸如“重复这些行 5 次”的条件循环，以及嵌套循环。借助循环和相互独立的 FX 列速度，这 16 行能走得出人意料地远。

### Retrigger 模式

Table 屏幕顶部的 `Retrig` 字段选择已经处于活动状态的 table 何时重新启动。`Inst` 是默认的兼容模式：含有乐器的 phrase 行会选中该乐器的默认 table，并从第 `00` 行开始。`Phrase` 在播放进入某个 phrase 时重新启动活动的 table，`Chain` 在进入某个 chain 时重新启动，`Free` 则让它一直运行，直到轨道停止或某个显式命令替换它。

结构性模式从不在时间上向后追溯。如果某条轨道没有活动的乐器 table，那么第一个引入乐器的 phrase 行会在恰好那一行激活它的 table，即使那是 phrase 的第 `4`、`8` 或 `F` 行。之后的乐器编号不会替换处于活动状态的 `Phrase`、`Chain` 或 `Free` table。这使得 table 可以用作类似 LFO 的自动化，而不会悄悄地模拟更早的 phrase 步骤。

`TBL`、`TBX` 和 `RET` 始终是显式的动作：无论什么模式，它们都会立即选择或重新启动。在 Phrase 或 Chain 边界上，它的重新启动发生在读取新的 phrase 行之前，因此该行上的 `TBL` 或 `TBX` 有最终决定权。因此，table 第 `00` 行的 FX 会照常再次运行；累积型 FX 保持其通常的累积。

Table `00-7F` 保留给默认的乐器 table。Table `80-FE` 用于由 `TBX` 效果启动的辅助 table。较低的范围同样可用于辅助 table，但这样用可能导致意外的冲突和混淆。

### 在 Phrase 与 Table 之间复制 FX

只选择 FX 名称/数值列，然后从 Phrase 中复制或剪切它们，粘贴到 Table 的任何 FX 通道中，或者反过来。复制会保留确切所选的单元格，所以只含命令或只含数值的选择仍然是部分的。粘贴从当前的 FX 单元格开始，并在第 `F` 行和目标的最后一条 FX 通道处被截断。音符、乐器编号、音高和音量从不会在这两个屏幕之间传递。

### 操作

- 在 FX 名称列上按 **EDIT + [UP/DOWN]**：打开 FX 选择屏幕
- **OPT + [DIRECTION]**：在 table 之间移动

## 8. 其他屏幕

### Instrument Pool

Instrument Pool 显示项目中的每个乐器，并允许你重新排序。播放期间，它还显示哪些乐器当前处于活动状态。

#### 操作

- **EDIT**：编辑乐器并跳转到 Instrument 屏幕
- **SHIFT + OPT**：复制乐器
- **SHIFT + EDIT**：粘贴乐器
- **EDIT + [UP/DOWN]**：重新排序乐器
- **EDIT + PLAY**：试听乐器

### AY Wavetable

Wavetable 是 AY Plus 的一种软件振荡器类型。一个项目最多可包含 256 个波表，每个有 32 步。所有波表乐器共享同一组波形。这个屏幕就是用来编辑它们的地方。

AY 的音量级别是非线性的，所以 Standard 视图的波形是按实际输出电平绘制的。连按 **OPT** 两次，可在 Standard 视图与会被保留的 **LFO 视图** 之间切换，快捷键与 Song/Live Mode 相同：LFO 视图以线性方式绘制 `0` 到 `15` 的原始步值，`7` 与 `8` 之间的中线是调制的零点。

该屏幕有 2 个逻辑行。顶行包含 **Load** 和 **Save** 按钮，第 2 行包含波表编辑器。

#### 操作

- **OPT + [LEFT/RIGHT]**：在波表列表中移动
- **OPT + [UP/DOWN]**：每次 16 个波形地在列表中移动
- **SHIFT + OPT**：复制波表
- **SHIFT + EDIT**：粘贴波表
- **OPT + PLAY**：在 Standard 与 LFO 视图之间切换

## 9. Tracker FX

轨道 FX 在单个步骤上强制设定音序器或声音引擎的值。其他 groovebox 常把它们称为“参数锁定”（parameter locks）。每个 Phrase 行有 3 个 FX 槽。

每个 FX 有一个 3 个字母的命令和一个十六进制的值。应用内置的帮助面板会对所选命令给出简短的说明。

### 音阶自动化 (Scale)

`SCL XY` 在不改变 phrase 中所存音符的情况下，更改全局的播放音阶和根音。`X` 选择音阶（`0` Chromatic、`1` Major、`2` Minor，直到 `C` Custom）；`Y` 选择根音（`0` C 到 `B` B）。该命令只适用于 phrase 的 FX，不适用于 table。如果多条轨道在同一个 tick 发出 `SCL`，则编号最小的轨道胜出。

`SCL` 属于 Quantizer 模式。启用 **Note Lock** 时，FX 列表中不会提供该命令，并且已写入 phrase 的任何 `SCL` 在该模式开启期间都会被忽略。

### 和弦 (Chords)

`CRD XY` 把它所在 Phrase 行上的音符变成和弦。`Y` 选择：`0` Major、`1` Minor、`2` Dim、`3` Aug、`4` Sus2、`5` Sus4、`6` Power、`7` Maj7、`8` Min7、`9` Dom7、`A` Min7b5、`B` Dim7、`C` Add9、`D` MinAdd9、`E` Maj9 或 `F` Min9。`X` 选择 16 种声部排列（voicing）之一：`0-3` 是上升的密集转位循环；`4-7` 加入 drop-2；`8-B` 加入 drop-3；`C-F` 同时结合两种 drop。对于三和弦和 power chord，循环会再延续一个八度，因此每个值都互不相同；较大的值会逐渐变得更开阔，也更不常规。例如，`CRD 4Y` 是 drop-2 的原位和弦，`CRD DY` 是同时带两种 drop 的第一转位。在音高表的底部或顶部，声部会被限制在其范围内。该命令不是持久的：请在每个有和弦的行上写它。当 Scale 处于活动状态时，每个生成的音符都会被独立量化，Quantizer 和 Note Lock 两种模式都是如此。它只适用于软件引擎；AY 乐器会忽略它，并保持单声部。

### 音序器 FX

| FX | 值 | 详细行为 |
|---|---|---|
| `ARP` | `XY` | 对基础音、`+X` 步和 `+Y` 步做琶音。`37` 产生小和弦的模式。 |
| `ARC` | `XY` | `X` 选择琶音的方向或范围模式；`Y` 是以 tick 计的速度。 |
| `PVB` | `XY` | 音高颤音：`X` 是速度，`Y` 是深度。在 Linear 模式下，深度使用 `10 cent` 的步长。 |
| `PBN` | 有符号 `XX` | 在每个 phrase/table 行加上 `XX` 个音高单位；`FF` 表示 `-1`。用 `00` 停止。 |
| `PSL` | `XX` tick | 在 `XX` 个 tick 内从前一个音高滑向新的音符。 |
| `PIT` | 有符号 `XX` | 以音高表步数计的累积相对偏移。 |
| `FIN` | 有符号 `XX` | 累积的微调偏移：在 Linear 音高下以 cent 计，否则以周期单位计。 |
| `PRD` | 有符号 `XX` | 累积的相对振荡器周期偏移。 |
| `VOL` | 有符号 `XX` | 累积的相对音量偏移（`FF` 是 -1，不是满音量）。 |
| `VSL` | 有符号 `XX` | 在每个 phrase/table 行把 `XX` 加到音量上。用 `00` 停止。 |
| `PAN` | `00-FF` | 乐器的绝对声像：左、中（`80`）、右。 |
| `TPN` | `00-FF` | mixer 轨道的绝对声像：左、中（`80`）、右。 |
| `RET` | `XY` | 每 `Y` 个 tick 重新触发一次；`X` 应用音量变化。`Y=0` 停止重新触发。 |
| `DEL` | `XX` tick | 延迟 note-on。长于当前 groove 步的延迟会跳过该音符。 |
| `OFF` | `XX` tick | 在 `XX` 个 tick 后发送 note-off，并进入 ADSR 的 release 阶段。 |
| `KIL` | `XX` tick | 在 `XX` 个 tick 后立即切断声部，不经过 ADSR 的 release。 |
| `TIC` | `XX` tick | 设置每行的 table tick 数。在 table 中，它改变该 FX 列的速度。 |
| `TBL` | `00-FE`，`FF` 关闭 | 立即替换并重新启动乐器的 table；`FF` 使其停止。 |
| `TBX` | `00-FE`，`FF` 关闭 | 立即启动或替换辅助 table；`FF` 使其停止。 |
| `THO` | 行 `XX` | 把乐器 table 的所有列跳转到第 `XX` 行。 |
| `TXH` | 行 `XX` | 把辅助 table 的所有列跳转到第 `XX` 行；不在 table 内部使用。 |
| `GRV` | groove `XX` | 为当前轨道选择一个 groove。 |
| `GGR` | groove `XX` | 为每条轨道选择一个 groove。 |
| `HOP` | `XY` | 跳转到第 `Y` 行，共 `X` 次；`X=0` 表示永远循环。Table 的 HOP 作用于它自己的列。 |
| `SNG` | 有符号 `XX` | 把播放移动 `XX` 个 Song 行。 |
| `PRO` | `00-64` | 求值一个从 `0%` 到 `100%` 的绝对触发概率。 |
| `MOD` | `AB` | 在 `B` 次一轮的循环中的第 `A` 次触发。 |
| `SPD` | 有符号 `XX` | 选择一个持久的、每条轨道的时钟比例；见下面的速度表。 |
| `SLE` | `00-FF` tick | 为连续型的引擎 FX 设置持久的、每条轨道的滑音时间。`00` 为立即。播放停止时，该设置重置为 `00`。 |

### 音频 FX 发送 (Audio FX sends)

| FX | 值 | 详细行为 |
|---|---|---|
| `RSN` | `00-FF` | 设置这条轨道的混响发送，直到下一个音符触发。 |
| `DSN` | `00-FF` | 设置这条轨道的延迟发送，直到下一个音符触发。 |

### ADSR / 触发 FX

| FX | 值 | 详细行为 |
|---|---|---|
| `EAT` / `EDC` / `ESU` / `ERL` / `ESH` | `00-FF` | 覆盖 attack、decay、sustain、release 或包络形状。 |
| `TDC` / `TCL` | `00-FF` | 覆盖 Plaits 和 Plaits-Alt 的 Trigger 模式的 Decay 或 Color。 |

#### 条件

受瑞典的“trig conditions”启发，Tracker 支持触发概率和取模条件。

| FX | 值 | 含义 |
|---|---|---|
| `PRO` | `00-64` | 从 `0%` 到 `100%` 的绝对触发概率 |
| `MOD` | `AB` | 在 `B` 次中的第 `A` 次迭代触发；例如 `12`、`22`、`14`、`34` |

`MOD` 计数器局部于轨道和 phrase。无效的组合，如 `A` 大于 `B`，不会触发。

`MOD34` 在每 4 次循环的第 3 次触发。`MOD1F` 在每 16 次循环的第 1 次触发。

#### 播放速度

和 NerdSEQ 一样，ChooChooTracker 支持为每条轨道设置独立的播放速度。

`SPD` 把它的值读作有符号字节。`00` 是正常速度，正值使轨道更快，负值使其更慢。该设置一直有效，直到另一个 `SPD` 命令改变它。

| 值 | 速度 |
|---|---:|
| `00` | `x1` |
| `01` | `x2` |
| `02` | `x3` |
| `7F` | `x128` |
| `FF` | `/2` |
| `FE` | `/3` |
| `80` | `/129` |

不支持有符号 `SPD` 的旧项目保留其原来的 `00-10` 映射，因此仍按保存时的样子播放。

非常高的倍数可能超过 Tracker tick 调度器的分辨率，并且尚未在硬件上测试过。

### 调制 FX (Modulation FX)

Phrase 和 table 的 FX 可以在不编辑乐器的情况下更改某个调制槽：

| FX 模式 | 含义 |
|---|---|
| `M1A`, `M2A`, `M3A`, `M4A` | 调制槽 1-4 的 Amount 相对偏移 |
| `M11`, `M12`, `M13`, `M14` | 调制槽 1 的 P1-P4 相对偏移 |
| `M21`, `M22`, `M23`, `M24` | 调制槽 2 的 P1-P4 相对偏移 |
| `M31`, `M32`, `M33`, `M34` | 调制槽 3 的 P1-P4 相对偏移 |
| `M41`, `M42`, `M43`, `M44` | 调制槽 4 的 P1-P4 相对偏移 |

该值被解释为有符号的 `8 位` 相对变化（`01` 加 `1`，`FF` 减 `1`）。重复的命令会累积。有效值会被限制在其有效范围内。

`P5` 不作为 phrase 或 table 的 FX 公开。它作为调制目标 `M1–M4 Wavetable` 提供，因此一个调制器可以扫描另一个调制器的 AYWavetable 索引。

| 调制类型 | P1 | P2 | P3 | P4 | Shape 为 Wavetable 时的 P5 |
|---|---|---|---|---|---|
| ADSR | Attack | Decay | Sustain | Release | 未使用 |
| AHD | Attack | Hold | Decay | 未使用 | 未使用 |
| LFO | Shape | Trigger mode | Period | 未使用 | AY 波表索引 |
| SLFO | Shape | Trigger mode | Ticks | Multiplier | AY 波表索引 |
| FLFO | Shape | Trigger mode | Frequency（`1 Hz` 到 `20 kHz`） | 未使用 | AY 波表索引 |

### Braids FX

| FX | 值 | 含义 |
|---|---|---|
| `BMD` | `00-2E` | Braids 模型 |
| `BTM` | `00-FF` | 绝对的归一化 Timbre |
| `BCL` | `00-FF` | 绝对的归一化 Color |
| `BCF` | `00-FF` | 指数式的截止频率，`20 Hz` 到 `20 kHz` |
| `BRS` | `00-FF` | 指数式的滤波器共振 |

### aChChid FX

| FX | 值 | 含义 |
|---|---|---|
| `ASL` | `00-FF` | 不重新触发地滑向该音符。`00` 是 `60 ms`；更大的值会延长滑音。 |
| `ADC` | `00-FF` | Decay 覆盖：`200 ms` 到 `2 s`。 |
| `AAC` | `00-FF` | 重音量：从无到最大。 |
| `ATM` | `00-FF` | Braids Timbre 覆盖；仅在 Braids 波形模式下有效。 |
| `ACL` | `00-FF` | Braids Color 覆盖；仅在 Braids 波形模式下有效。 |
| `ACF` | `00-FF` | 303 滤波器的指数式截止频率，`20 Hz` 到 `20 kHz`。 |
| `ARS` | `00-FF` | 303 滤波器的共振，从无到最大。 |
| `AEM` | `00-FF` | 303 滤波器的包络调制，从无到最大。 |

### Bogie FX

| FX | 值 | 含义 |
|---|---|---|
| `DMD` | `00-0B` | 选择 Bogie 的模型，直到下一次触发。 |
| `DDC`, `DTO`, `DSW`, `DNO`, `DFM`, `DDR` | `00-FF` | 覆盖 Decay、Tone、Sweep、Noise、FM 或 Drive，直到下一次触发。六个宏在每种模型上都可用。 |
| `DCF` | `00-FF` | 滤波器截止频率，按对数映射。 |
| `DRS` | `00-FF` | 滤波器共振。 |

### Sintered FX

| FX | 值 | 含义 |
|---|---|---|
| `SMD` | `00-05` | 选择 Sintered 的模型，直到下一次触发。 |
| `SDC`, `SMP`, `SMA`, `SMB`, `SMO`, `SMC` | `00-FF` | 覆盖 Decay、Mod、模型的前两个宏、Motion 或模型的最后一个宏，直到下一次触发。 |
| `SCF`, `SRS` | `00-FF` | 滤波器的截止频率和共振。 |

### MIDI Out FX

| FX | 值 | 含义 |
|---|---|---|
| `MC1`-`MC4` | `00-FF` | 发送一个 MIDI CC，重新缩放到 `0-127`。每个槽发送到哪个 CC 编号，是按乐器设置的（乐器屏幕上的 `MC1 number`-`MC4 number`），而不是由 FX 本身设置 - 没有设定编号的槽不起作用。 |

### Plaits FX

| FX | 值 | 含义 |
|---|---|---|
| `PMD` | `00-17` | Plaits 引擎 |
| `PHA` | `00-FF` | 绝对的归一化 Harmonics |
| `PTM` | `00-FF` | 绝对的归一化 Timbre |
| `PMO` | `00-FF` | 绝对的归一化 Morph |
| `PAX` | `00-FF` | Main/Aux 混合：`00` 为 Main，`FF` 为 Aux |
| `PCF` | `00-FF` | 指数式的截止频率，`20 Hz` 到 `20 kHz` |
| `PRS` | `00-FF` | 指数式的滤波器共振 |

### Sample FX

| FX | 值 | 含义 |
|---|---|---|
| `SPL` | `00-03` | 回放模式：`00` Forward、`01` Reverse、`02` Loop、`03` Ping-Pong |
| `SLI` | `00-FF` | 与音符无关，播放编号对应的切片（`01` = 第一个切片）；`00` 保持正常的音符映射。需要在乐器上启用切片模式 |
| `SPT` | 有符号 `XX` | 以半音计的采样移调 |
| `SST` | `00-FF` | 归一化的回放起点 |
| `STA` | `00-FF` | `SST` 的别名（归一化的回放起点） |
| `SEN` | `00-FF` | 归一化的回放终点 |
| `SVL` | `00-FF` | 绝对的采样音量 |
| `SCF` | `00-FF` | 指数式的截止频率，`20 Hz` 到 `20 kHz` |
| `SRS` | `00-FF` | 指数式的滤波器共振 |
| `SSP` | `00-FF` | 采样速度，映射为 `0%` 到 `500%` |

### AY 乐器共用的 AY FX

| FX | 值 | 详细行为 |
|---|---|---|
| `AYM` | `XY` | `X` 是包络形状；`Y` 选择 off/tone/noise/tone+noise（`0/1/2/3`）。 |
| `NOI` | 有符号 `XX` | 累积的相对噪声周期偏移。 |
| `NOA` | `00-1F`、`FF` | 绝对的噪声周期；`FF` 把噪声周期的优先权让给更靠前的轨道。 |
| `ERT` | 任意 | 重新触发当前的硬件包络形状。 |
| `EAU` | `XY` | 自动包络比例 `X:Y`；`X=0` 将其禁用。 |

### 仅限 AY Classic 的 FX

| FX | 值 | 详细行为 |
|---|---|---|
| `EVB` | `XY` | 包络周期颤音：`X` 为速度，`Y` 为深度。 |
| `EBN` | 有符号 `XX` | 在每个 phrase/table 行把 `XX` 加到包络周期上。 |
| `ESL` | `XX` tick | 从前一个包络周期滑向新值。 |
| `ENT` | 音符 `XX` | 根据界面所示的音高表音符设置包络周期。 |
| `EPT` | 有符号 `XX` | 累积的相对包络周期偏移。 |
| `EPL` | 字节 `XX` | 设置包络周期的低字节。 |
| `EPH` | 字节 `XX` | 设置包络周期的高字节。 |

### AY Plus FX

| FX | 值 | 详细行为 |
|---|---|---|
| `TNN` | 音符 `XX` | 忽略行音符，把音调振荡器设定为音高表中的某个音符。 |
| `TNP` / `TNF` | 有符号 `XX` | 累积的音调音高步进/微调偏移。 |
| `TRT` | 任意 | 在支持的地方重新触发音调振荡器的相位。 |
| `ENN` | 音符 `XX` | 把包络振荡器设定为音高表中的某个音符。 |
| `ENP` / `ENF` | 有符号 `XX` | 累积的包络音高步进/微调偏移。 |
| `SFT` | `00-07` | 软件类型：None、Pulse、Sync Tone、Sync Env、Wavetable、Tone FM、Env FM、Sample。 |
| `SFN` | 音符 `XX` | 把软件振荡器设定为音高表中的某个音符。 |
| `SFP` / `SFF` | 有符号 `XX` | 累积的软件振荡器音高步进/微调偏移。 |
| `SRT` | 任意 | 重置软件振荡器的相位。 |
| `SFM` | 有符号 `XX` | 累积的 FM 深度偏移。 |
| `PWM` | 有符号 `XX` | 累积的脉宽偏移。 |
| `SPL` | 有符号 `XX` | 累积的脉冲低电平偏移。 |
| `SWT` | 有符号 `XX` | 累积的波表索引偏移。 |

### AY Sample FX

AY Sample 接受共用的 AY 命令，以及如上所述的 `TNN`、`TNP`、`TNF`、`TRT`、`SFN`、`SFP` 和 `SFF`。

| FX | 值 | 详细行为 |
|---|---|---|
| `SMS` | `00-FF` | 把传统的 AY Sample 回放起点设为 `XX x 64` 个源采样。 |

引擎参数 FX 适用于与之匹配的乐器类型，除非另有说明，否则会在下一个音符触发时重置。`SPD` 是有意设计为持久的。

FX 选择器会对这份参考做过滤，只显示常用的命令加上活动乐器所支持的那一组。上下文帮助面板在编辑时提供数值的详细信息。

## 10. Mixer、Reverb 与 Delay

Mixer 是最左边的主屏幕。它显示 CPU 负载和削波警告。**CLIP** 列中红色的 `!` 标出干信号把混音推出安全范围的那条轨道。

用它来平衡这 8 条轨道，用 Tilt EQ 塑造每条轨道，并把音频发送到混响或延迟：

| 字段 | 含义 |
|---|---|
| LVL | 引擎之后的轨道电平，`000-100` |
| REV | 发送到共享的 Clouds 混响，`000-100` |
| DLY | 发送到共享的 ping-pong 延迟，`000-100` |
| TLT | 每条轨道的 Tilt EQ，`00-FF`；`80` 为中性，低值偏向低音，高值偏向高音。低值会引入轻微的过载。 |
| M | `*` 使这条轨道静音 |
| S | `*` 使这条轨道独奏 |

Mixer 以轨道为单位，而不是以乐器为单位。如果某条轨道更换了乐器，它的电平、PAN、Tilt 和发送仍然附着在这条轨道上。乐器在 Instrument 屏幕的顶行也有自己 `00-FF` 范围的 **Vol** 和 **Pan**。先应用乐器的 PAN，再应用轨道的 PAN。两者在中间位置都保留原有的立体声像；向左或向右移动时，会逐渐衰减相反的一侧。

### Master FX

**MASTER FX** 条目会打开一个针对整个混音的双槽 Insert FX 子菜单。它的效果在混响和延迟的返回之后、总音量之前运行。选择一个模块，编辑它的参数，或像对轨道 insert 那样把它设为 bypass；按 **OPT** 返回 Mixer。Master FX 的设置随项目保存，不受 phrase、table 或调制的自动化控制。

### Clouds Reverb

在 Mixer 中用 **SELECT + [UP]** 打开混响设置屏幕。

- **Return**：总混音中湿混响的电平。
- **Time**：衰减/时间控制，`00-FF`。
- **Damping**：高频吸收，`00-FF`。
- **Filter**：应用于混响之前的低通滤波器。

这是 Clouds 的混响部分，不是完整的 Clouds 颗粒处理器。

### Ping-pong Delay

在 Mixer 中用 **SELECT + [DOWN]** 打开 Delay，然后用 **SELECT + [UP]** 返回。Reverb 的方向相反：**SELECT + [UP]** 打开它，**SELECT + [DOWN]** 返回 Mixer。

- **Return**：湿延迟的电平。
- **To Reverb**：送入共享混响的湿延迟信号的量，`0-100%`。
- **Ticks**：以 Tracker tick 计的延迟时间（默认 `9 ticks` = `1 拍`）。
- **Feedback**：交叉反馈量，上限为 `95%`。
- **Filter**：延迟信号通路中的低通滤波器。

第 1 次重复跟随立体声输入。之后的反馈会在左右声道之间交叉。

## 11. Project 屏幕

Project 屏幕提供 **Load**、**Save**、**New**、**Export**、**Manage** 和 **Scale** 命令，以及文件名、标题和作者的元数据。

### 导出位置 (Export location)

导出、stem 和 bounce 会写入应用已有的 samples 目录中、每个项目专属的文件夹：

- **默认**：`<app folder>/samples/Exports/<project name>/`。项目没有名称时，文件夹为 `current-project`。
- 在 Web 版本中，默认为 `/user/samples/Exports/<project name>/`。
- 在 Android 上，samples 文件夹位于应用的私有工作空间内。

EXPORT 屏幕的 **Folder** 行显示当前的目的地：

- 点按 **EDIT**，用文件夹浏览器选择自定义文件夹。所选文件夹按原样使用（不会添加项目子文件夹），并记录在设置中。
- **EDIT + OPT**（clear）恢复默认位置。

当有名称的项目以新名称保存时，默认的导出文件夹会随之重命名，使先前导出的文件与项目保持在一起。重命名不适用于自定义文件夹，自动保存也不会重命名文件夹。如果已存在同名的目标文件夹，则两个文件夹都会保留。如果 Tracker 失去了对该文件夹的跟踪（例如重启之后），保存有名称的项目会改为重命名现有的 `current-project` 文件夹。重命名之后乐器仍可继续工作：指向被重命名文件夹的已加载采样路径会自动更新。

**Load** 也接受 `.mid`/`.midi` 文件，并作为新项目导入：音符按 MIDI 通道分组（每条轨道一个通道，最多到轨道数），量化为每拍 4 行，并放在单个默认的 AY 乐器上 - MIDI 的程序号在 chiptune 里没有对应物，所以之后请选择真正的乐器。只使用文件中的第一个速度（一个全局的 tick 速率，没有按段落的速度变化）。Export 屏幕的 **MIDI** 行则相反：把当前编排的音符、音量和速度/groove 写成标准 MIDI 文件（每条 Tracker 轨道对应一条 MIDI 轨）；[MIDI Out](#midi-out) 乐器以及音量和全局 groove 之外的其他逐行 FX 在 MIDI 中没有对应物，不会被转换。

**Load** 也接受 Dirtywave M8 的歌曲（`.m8s`，固件 2.x-4.x），并作为新项目导入。只会带入结构和音符（歌曲行、带移调的 chain、含音符、力度和乐器编号的 phrase、note-off、速度和歌曲名）；音符保持其真实音高（M8 把八度命名得高两个：它的 `C-6` 在这里是 `C-4`）。phrase 所用的每个乐器，都以 M8 乐器的名称导入。M8 的 **MACROSYN** 乐器会变成 Braids 乐器（47 种形状就是 Braids 的模型，编号相同；timbre、color、滤波器类型、截止频率、共振和 pan 会被带过来，截止频率曲线是近似的）。**SAMPLER** 乐器会变成 Sample 乐器：`.m8s` 只保存 M8 卡上 WAV 的路径，所以会在 `.m8s` 所在的文件夹及其上级文件夹（卡的布局副本也行）和你当前的采样文件夹中查找该文件（`Samples` 或 `samples` 都可以）；找不到时，乐器保留 M8 的路径并保持静音。导入的采样会在其 Pitch 设置中加上 +24 个半音，因为 M8 以原始音高播放采样，比本应用低两个八度。**FMSYNTH** 变成 Genesis FM 乐器（粗略的近似），**WAVSYNTH** 和 **HYPERSYNTH** 变成 aChChid。**MIDI OUT** 变成 MIDI Out 乐器（通道、bank、程序和前四个 CC 编号）。其他类型（外部的）会变成默认的 AY 乐器 - 之后请选择真正的声音。FX 和 table 会被忽略。

在桌面版上，Export 屏幕的 **M8S** 行以有限的方式做相反的事：点按它，选择一个现有的 `.m8s` 作为**模板**（记住到退出为止，**EDIT + OPT** 会忘掉它）。它的歌曲、chain、phrase、速度和标题会被替换，其他一切保持原样，只是 Braids 乐器会被写回为 MacroSynth，Sample 乐器被写回为 Sampler（路径为 `/Samples/<file name>`：请自行把 WAV 复制到 M8 的那个位置），前提是模板中有该类型的乐器可供复制其他参数。FX 不会导出，使用 phrase `FF` 或更高的歌曲无法导出（M8 有 255 个 phrase）。导出的文件尚未在真正的 M8 上测试过。详情参见 [M8 songs](../m8s-format.md)。

在桌面版上，key jazz 可以让你直接用键盘输入文件名、标题和作者，而不必使用屏幕上的虚拟键盘 - 参见 [Key jazz](#key-jazz-仅限桌面端)。

### Scale / Quantize

Scale 屏幕控制全局的 12-TET 音阶引擎。**Mode** 行在两种模式之间选择：

- **Quantizer**（默认）：phrase 的输入保持半音阶；启用时，音符会在到达声音引擎之前立即向下取整，所以更改根音或音阶会重新和声化一首歌，而无需编辑它的 phrase。
- **Note Lock**：输入 phrase 的音符会在输入时被吸附到音阶中最近的音符，因此只能写入音阶内的音符。新输入和降低会向下吸附；`EDIT + RIGHT` / `EDIT + UP` 把音符抬到上面下一个音阶音。在这个屏幕上，输入始终遵循项目的音阶：更改根音或音阶会立即影响已启用轨道上新输入的音符，而已存储的音符保持不变。在这个模式下 `SCL` FX 不可用，已写入的任何 `SCL` 都会被忽略。普通音符的播放量化被绕过（输入已被锁定）；通过 `CRD` 的和弦量化仍然适用。

光标停在 **Mode** 上时，状态栏会持续显示当前模式的固定说明：**Quantizer** 为 `Quantizes notes on playback`，**Note Lock** 为 `Snaps sequencer to scale`。切换模式会立即替换文字；任何操作消息都会取代它，直到它消失。

两种模式共享相同的根音、音阶、**Custom** 音符编辑和轨道复选框。八个轨道复选框各自决定该模式适用于哪些轨道。对于音符数不是 12 的音高表，以及 **Slice** 模式为 EQUAL、AUTO 或 LAZY 的 Sampler 乐器（它们的音符从 C-0 开始按半音阶选择切片），音阶引擎会被绕过；MIDI 输入不属于这个版本。

- **Linear pitch** 选择音高表模式。**Off** 是默认值，也是经硬件验证、可让 AY、Braids 和 Plaits 正确跟踪八度的设置。
- **Tick rate** 设置 Tracker 的节奏，并显示对应的 BPM（`tick rate x 60 / 24`）。
- ChooChooTracker 把项目保存为 `.cct`。这种格式与 ChipNomad 不兼容。

在更改乐器类型或加载另一个项目之前，请使用 **Save**。

## 12. Settings

- **Repeat delay / speed** 调整按住按键时的重复。
- **Stick live mode** 为已映射的 Stick live 按键选择 `HOLD`、`TOGGLE` 或 `FREE`。
- **MIDI** 打开 [MIDI](#14-midi) 子菜单：设备选择，以及 MIDI In 通道与乐器的映射。
- **Synths** 包含 **AY Quality**、**Sample dithering** 和全局的 **Braids BITS / DRFT / SIGN** 设置。
- **Mixer** 包含 **Mix volume** 和每个项目的 `250-4000 Hz` **Tilt pivot**（默认 `1 kHz`），所有 Mixer 的 Tilt 控件都使用它。
- **Graphics** 包含 **Edit color theme**、**Load font**、**Persistent waveform**、**Track visuals**、**Waveform FPS** 和活动的渲染器（`GPU` 或 `Software`）。**Waveform FPS** 设置乐器示波器和 mixer 可视化的刷新率，范围从 `01` 到 `60`。Track visuals 为每条轨道独立地选择 **Detailed**（合成器波形及其叠加层）或 **Audio waveform**（该轨道的实际合并输出）；音频波形在播放开始时会立即清除之前的 ADSR 叠加层。ChipNomad 的字体和主题应该可以使用。
- **Key mapping** 自定义控件。
- **Support report** 写出一个小的诊断文本文件，其中含有应用版本、平台、音频设置和 CPU 负载、播放/溢出状态，以及项目的简要摘要。不包含采样数据或项目的标题/作者文本。在 Android 和 Web 上，它会打开常规的导出/下载流程；桌面和掌机版本把 `support-report.txt` 保存在应用的可写文件夹中。
- **Quit ChooChooTracker** 正常退出。

**Graphics → Boot screen** 选择 **Original**（默认）或 **Night**。点按 EDIT 或使用 EDIT + 方向键来更改；该选择会为下一次标题画面保存。Night 使用原版的铁路场景，色调更柔和，呈蓝灰色，并配有程序生成的银河，带有微妙的星光闪烁，偶尔还有流星。原版的风景和火车保持其滚动循环。

## 13. 性能与故障排除

### CPU

CPU 的开销取决于活动的引擎和效果。Plaits 的物理模型和 Clouds Reverb 比基本的 AY 声部更重。请在目标掌机上检查性能，尤其是同时有 8 个 Plaits 声部且两个发送都活动时。CPU 读数接近 `100%` 可能导致噼啪声或错过音频的截止时间。

CPU 显示的是对音频渲染时间的平滑测量。它不测量音频回调运行之前或设备音频输出中的延迟，所以即使读数较低，也可能出现噼啪声。

在 Android 上，音频输出现在优先使用 AAudio，以避免在 Pixel 7a 上用 OpenSL ES 时观察到的间歇性噼啪声。这种带缓冲的输出优先保证回放的稳定，可能增加一些响应延迟。在方形和其他不常见的屏幕比例下，Tracker 画布保持完整的 4:3 布局；未使用的空间以边框显示，而不是裁切列。如果噼啪声仍然存在，请报告歌曲、手机型号、Android 版本，以及你使用的是扬声器、耳机还是蓝牙。

如果遇到爆音、崩溃或变慢，请使用 **Settings > Support report**，并把生成的 `support-report.txt` 附在错误报告中。如果某首特定的歌曲会触发问题，也请附上那个 `.cct`。

### 没有声音

检查乐器编号、轨道的静音或独奏状态、轨道的 LVL、应用的 Mix volume 和乐器的包络。对于采样，检查原始 WAV 是否仍在其保存的路径上。在 ArkOS 上，按 **MENU + L3** 切换操作系统的静音。如果这没有帮助，请重启硬件，并重新连接你的音频接口。

### 操作感觉不对

在 Settings 中调整 Repeat delay 和 Repeat speed。如果一次按键移动了两次，请检查该方向是否只映射了 1 个物理控件，并报告确切的屏幕和快捷键。

## 14. MIDI

桌面版和 PortMaster。Settings > **MIDI** 打开这个子菜单：**MIDI In** / **MIDI Out** 选择一个已连接的设备，在检测到的端口之间循环，两端都是 `OFF`；两个方向都需要选择一个设备。设备的选择只对当前会话有效：它不会保存到 settings.txt，因为端口在列表中的位置可能在重启之间，或在设备以不同顺序插入时发生变化。应用的 **PLAY** 控件在这个屏幕上仍然可用，并会启动歌曲。在 PortMaster 上，请在启动前连接 USB MIDI 设备；它必须由掌机的操作系统通过 ALSA 暴露。

- **用 MIDI 键盘试听声音**：选择了 MIDI In 设备后，在所连接的键盘上弹奏音符，会在任何屏幕上试听一个乐器，与 [Instrument](#5-instruments) 屏幕上的屏幕快捷键 **EDIT + PLAY** 相同。这只是试听 - 它不会把音符输入歌曲。
- **Channel mapping**：打开 16 个 MIDI 通道的列表；为每个通道指定一个乐器（默认 `OFF`），这样在该通道上收到的音符就会试听那个乐器，而不管 Instrument 屏幕上选的是哪一个 - 例如通道 `01` -> 乐器 `05`。保持为 `OFF` 的通道会回退到当前选中的乐器，也就是原来的行为。这个映射保存在 settings.txt 中。
- **MIDI CC mapping**：把传入的控制器映射到乐器参数（包括其 **Volume**）或全局控件。全局行显示 `TRK 1`-`TRK 8` 而不是乐器，并为每条轨道提供 **Mute**、**Solo**、**Volume**、**Reverb send** 和 **Delay send**；**Song play/stop** 没有目标。新的映射会忽略学习/配置它的那个 CC，并在下一次实际移动时才生效，因此在播放期间设置映射不会把某个音色重置为零。Mute 和 solo 使用 `0-63` = off，`64-127` = on。
- **驱动外部 MIDI 设备**：为某条轨道指定 [MIDI Out](#midi-out) 乐器类型，并设置其 Channel；在该轨道上触发音符，会向所选的 MIDI Out 设备发送真正的 MIDI，而不是在 ChooChooTracker 中发声。

## 15. 致谢与许可

ChooChooTracker 是 ChipNomad 的分支，并保留其 MIT 许可方式。Braids、Plaits、Plaits-Alt、Clouds DSP、源自 Warps 的 MME DSP 和 stmlib 的代码，派生自 Mutable Instruments 的开源发布，适用其相应的 MIT 声明。Plaits-Alt 来自 lylepmills/eurorack 的 Plaits Lab 分支；其保留的源代码声明同样适用。aChChid 引擎使用 Robin Schmidt 的 Open303（版权 2009），采用 MIT 许可证。Bogie 和 Sintered 是原创的原生实现。MIDI I/O 使用 Gary P. Scavone 的 RtMidi，采用其类 MIT 的许可证。确切的署名请参见随附的许可证文件。

## 轨道 Insert 效果 (Track insert effects)

八条轨道中的每一条都有两个串联的槽，**TF1 → TF2**。从 Instrument 出发，用惯常的 Shift+Up 手势打开 MOD，再用一次 Shift+Up 打开 **F: Insert FX**。Shift+Down 返回 MOD。OPT+Left/Right 选择轨道（1-8），不会改变乐器选择器。模块的标题栏会打开现有的选择器；bypass 会保留模块、已配置的值和自动化。参数是字节值：左列 1-4，右列 5-8。上下文文字显示完整的名称和解码后的值。旧项目加载时，两个槽都是 OFF。

导航显示在包括 Insert FX 和 instrument pool 在内的所有乐器屏幕上，始终保持完整的 F-M-I-P 列可见，并高亮当前屏幕。

| 模块 | 参数（按顺序） |
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

连续型控件使用 00-FF，包括有效的零和最大值 FF。双极性控件的精确中性点在 80。Distortion 的模式为 00 Density、01 Drive、02 Spiral、03 Mojo、04 Dyno。Compressor 的比例 00-07 依次为 1.5、2、3、4、6、8、16、20:1；检测器源 00-02 依次为立体声输入、左、右。Detector filter 的 80 为中性，低于它为低通，高于它为高通。不提供外部的或跨轨道的 sidechain。两个槽中的任何一个都可以放任何模块；重复的模块相互独立。
Chorus、Flanger、Phaser 和 Rotary 使用柔和的调制默认值。Rotary Speed 使用完整的 **00-FF** 范围：**00 = 0.01 Hz**（每 100 秒转一圈），**FF = 25 Hz**（原来的 Chaos 最大值）。指数式的间隔让缓慢的 LFO 运动有更精细的控制。新的 Rotary insert 默认为 **8F**，约 0.8 Hz。这取代了四个离散的速度值；已有的 Rotary 设置和 F11/F21 的速度自动化需要按新的刻度重新调整（原来的 Slow/Fast/Hyper/Chaos 大致对应 **8F/CB/E7/FF**）。其他参数不变。
Bitcrusher 的位深范围为 4 到 16 位，采样率降低为 1x 到 32x。Destruction 的模式为 Fold、Clip 和 Crush。
Saturation 和 distortion 包含输出电平或 mix 控件，用于与未处理的信号取得平衡。
Parametric EQ 在 60-500 Hz 和 2-14 kHz 处有 24 dB 的提升/衰减搁架，外加一个位于 250 Hz-5 kHz、固定 Q 为 0.9 的钟形。它的增益控件在 80 为中性；三个都为中性时，它就是精确的 bypass。活动的 EQ 在满量程以上使用透明的软拐点，并在最大 1.5 处封顶，以抑制极端的提升。

轨道电平和 tilt EQ 位于 insert 之前，因此两者都会影响进入压缩和失真的信号。轨道上的所有声部/和弦音符都馈入同一条 insert 链。混响发送和延迟发送都接收它的输出。共享效果、返回、总控制和静音/独奏保留其现有的行为。Insert 不处理来自外部 MIDI 设备的音频。

Insert 命令对 TF1 使用 **F11-F18**，对 TF2 使用 **F21-F28**。Roll 的 FX 选择和 Phrase/Table 弹窗只提供所选轨道已配置的 insert 所使用的参数。弹窗把它们分组为 **TF1: 效果名称** 和 **TF2: 效果名称**；OFF 的槽被隐藏。被 bypass 的 insert 仍可编辑。
在 Phrase 和 Table 的 FX 弹窗中，说明跟随所选轨道的 insert 槽：它显示模块、参数名称、解码后的范围端点和保存的基础值。如果更换模块使已有的歌曲命令不再被用到，这些命令会被保留；打开它们的选择器时，会从一个可用的控件开始。
编辑数值时，底部的提示显示其解码后的值和单位。
Phrase 或 Table 的任何 FX 列都可以寻址任一个槽；Phrase 的网格仍保持三列。这些绝对的运行时数值会在音符、乐器、phrase、chain 和普通循环之间持续存在。重新开始播放/强制停止会重置它们。在 F 页面编辑某个基础值，只会清除该参数的覆盖。MOD 作用于有效的基础/自动化值，而不保存它的偏移。选择新的模块会恢复默认值，并清除该槽的覆盖。Bypass 保留已有的自动化，同时在一个短暂的淡出之后暂停处理。

**地址指的是轨道槽位和参数位置，而不是有名称的效果。** 在另一条轨道上重复使用的 Phrase 会使用那条轨道的模块。更换模块可能会改变一个命令的含义；命令不会被重写。OFF/未使用的地址是安全的无操作，并完整保留在已保存的 Phrase/Table 数据中。

乐器的 MOD 选择器增加了一个 Insert FX 类别。每个乐器的源控制播放它的那条轨道的 insert，包括合并的和弦信号。同一个乐器放在两条轨道上，会作用于相互独立的链。已有的源时序、有符号的量、源的组合规则、摇杆模式和动作录制均适用。离散的控件会被量化并限制在有效的选项内。没有归轨道所有的新调制库。

Compressor 的 makeup 从 0 dB 开始。OTT 从大约 25% 的 depth、25% 的 upward、50% 的 downward、0 dB 输入和 -3 dB 输出开始。它的频段阈值、增益和分频点保持 Rui-727 的原生默认值；Depth 是它的 wet/dry 控件。TAPESCAM 保留原生的辅助默认值：新磁带/高速、噪声和压缩关闭，展宽开启。添加 drive/增益时，请留意现有的削波指示。槽之间的转换使用经由干音频的、有限的 5 ms 淡变。

存档在可选的、带版本的 `Track inserts: 1,8,2` 和 `Master inserts: 1,2` 小节中保存模块 ID、bypass 和已配置的字节。运行时的覆盖和 DSP 历史不会保存。在较旧的版本中打开启用了 insert 的存档之前，请先备份。DSP 的署名、源码修订和改编记录在 `chipnomad_lib/external/insert_fx/SOURCES.txt` 中；完整的 MIT 声明随 `licenses/INSERT_FX.txt` 一同提供。

在 48 kHz 的 R36H 上，在一条轨道上测得的一对 insert，在合成器和共享效果的开销之外，大约占用实时的：Compressor 4-5%、Distortion 6%、Doubler 18%、TAPESCAM 16%、OTT 24%。十六个活动的 Doubler、TAPESCAM 或 OTT 实例超过了实时。音频速率的调制会增加更多开销。请从少量 insert 开始，注意音频过载，并把未用的槽设为 bypass；可用的十六个位置并不是有保证的 CPU 预算。

Insert 页面在松开按键之后仍会保留所选字段的提示；临时通知仍可能取而代之。模块选择器在每个模块的名称旁边显示其效果类型和来源项目。

### 原生 OPLL 和 VRC7 乐器（开发中）

Instrument Type 选择器有一个 FM 组，包括 **OPLL / MSX**（YM2413）和 **VRC7**（DS1001）。现在每个都在可滚动的 Bank / Preset 列表中提供 **73 个程序**：原有的 15 个音色，来自 emu2413 的 YM2413/VRC7/YMF281B 表的另外 40 个不同音色，以及 18 个 ChooChoo 创作的双算子程序。每个引擎内部会省略完全重复的音色字节。这些额外的调色板使用芯片可编程的音色槽；它们不会扩展物理 ROM。程序零表示自定义音色。EDIT + 左/右在 bank 过滤器内选择上一个或下一个预设。Fine ct 在 -100 到 +100 cent 之间调整调音。

在程序列表中，EDIT + PLAY 试听高亮的声音；松开按键即停止。EDIT 确认，OPT 取消。浏览/试听不会改变乐器、table 或歌曲。选择之后，Instrument 页面正常的试听手势仍然有效。原生芯片的包络提供 attack/release；可选的 Amp env 增加一个软件 ADSR（见下面的“原生芯片控件”）。没有完整的算子编辑器。

新的乐器会把全部八个原生音色字节、bank/名称、程序和微调保存在歌曲/乐器文件中。包含这些类型的文件使用 7.0 格式，需要此版本或更高版本。较早的格式仍然可读；不含这些类型的歌曲继续按 5.0 保存。节奏程序被推迟。扩展后的库已在主机上验证；人耳试听和一次新的掌机检查仍待进行。

### AdLib / OPL2 和 OPL3（开发中）

FM 还包括 AdLib / OPL2（YM3812）和 OPL3（YMF262）。Bank 过滤出厂列表；Preset 打开分类组和一个 All 视图。EDIT + PLAY 在选择之前试听，EDIT 选择，OPT 取消，在 Preset 上用 EDIT + 左/右加载上一个/下一个匹配的条目。Fine ct 调整局部调音。Mode 显示 2 算子、4 算子或 Dual voice。OPL2 会隐藏不兼容的 OPL3 音色；OPL3 可以播放共享的双算子集合。

Bank 和 preset 弹窗的标题标明了当前的引擎。切换引擎或乐器槽会把 bank 过滤器重置为 **All banks**，在兼容的 AdLib 和 OPL3 引擎之间切换也是如此。在同一个槽内从弹窗返回会保留所选的过滤器。取消 DX7 bank 的导入会恢复先前的过滤器。

预设的确认也会把在松开按键时所做的编辑发布到下一个界面/音频 tick。正在播放的歌曲不再需要另一次按键或传输的重新启动，就能收到那个音色。输出仍然遵循所配置的音频缓冲区以及引擎的包络/重新触发行为。

出厂集合包含来自 The Fat Man 2-op、The Fat Man 4-op 和 DMXOPL3 的 697 个源条目，对应 589 个归一化后唯一的音色。别名保留源名称和署名。保留了源的原生算子电平、LFO 深度标志、音符偏移、固定的打击乐音高和双声部调音。Tracker 的音量使用软件幅度；它不会重现原始 MIDI 播放器的音量曲线或力度偏移策略。来自源的 release 时长估计不会截断持续的音符。有些效果的 attack 很慢：请按住 Seashore，而不要期待一次短促的点击就能展现它。

出厂资源位于 `instruments/chips`，与现有的乐器库并列，声明位于 `licenses/chip-banks`。选择预设会保持该槽的 table 和归轨道所有的 insert 不变。重新保存的文件使用 7.0，并嵌入完整的音色。出厂音频已在主机和 ARM64 上做了机器测试；人耳试听仍待进行。

### 原生 Sega PSG 和 Game Boy 乐器（开发中）

CHIP 现在包括 Sega PSG、GB Pulse 和 GB Noise。Sega 使用 NTSC 主时钟及其 16 位噪声反馈，包括由音调通道派生的噪声。其默认的 **Bass range: Extended** 在需要时会降低虚拟时钟，以便弹奏低于芯片约 109 Hz 的分频器极限。这使 A-2、G-2 和 F-2 保持不同。选择 **Chip** 可得到原来的范围，在那里更低的音符会收敛到分频器的极限。固定速率的噪声保持原来的时钟；由音调派生的噪声则跟随扩展后的音高。旧的 Sega 乐器加载时会启用 Extended；该设置随乐器保存。
GB 乐器使用 DMG 的脉冲/噪声寄存器；不提供 Game Boy 的波形通道。它们创作的预设列表现在分别包含 **24、24 和 28** 个预设。新增的内容涵盖关联/周期性的 Sega 噪声、GB 的音高扫频、短促的打击乐、金属噪声、持续音和上升音。这些是原创的程序，不是从游戏中提取的声音。
该页面提供原生的模式/占空比/宽度、包络/扫频/噪声控件（视情况而定），外加一个软件幅度 ADSR。这些保存在版本 6 的原生文件中。在 ARM64 上自动化的性能/音频检查在所报告的负载限制内通过；人耳试听仍待进行。

### DX7 FM（开发中）

选择 **Type → FM → DX7 FM**；DX7 是第七个条目，在 Arcade / YM2151 之后。DX7 FM 使用六算子的 MSFA 内核。现有的 FM 页面除了通用的乐器设置之外，还提供 Bank、Preset 和 Fine ct。预设的浏览使用 bank/类别列表。按住 EDIT+PLAY 听高亮的声音；松开就停止试听。单独的 EDIT 在松开时确认；OPT 取消。加载声音会复制它完整的音色，而不会改变 Tracker 的 table 或轨道的 insert FX。

出厂目录目前包含 **67 个互不相同的 DX7 参数音色**：31 个 OpenDX7 原创的音乐性声音，4 个独特的 YSE CC0 声音（它的 32 个 bank 槽用不同的名字重复了这 4 个），以及 32 个 ChooChoo 创作的音色。这里的“原创”指的是为本项目创建的参数程序；并不表示 Yamaha 原厂的原创内容。YSE 以四个声音的形式提供，而不是 32 个人为区分的名字。
类别描述的是经过审阅的声音设计意图。含糊的名称仍归入 Unsorted。数值上的可演奏性检查已经通过；听感验收仍待进行。另外一个目标，即 1,000 个已获得再分发许可的声音，尚未达成。

要建立持久的个人库，请把 `.syx` 文件放在 **`instruments/banks/dx7/`** 中，位于现有乐器库旁边。支持子文件夹。打开 **DX7 → Bank**：每个文件都会显示为一个有名称的 bank，可供在 Preset 中浏览。添加或删除文件之后，请重新打开 Bank；不需要转换，也不需要 Load Instrument 这一步。一个标准的原版 DX7/TX7 bank 包含 **32 个声部**。包含四条 bank 消息的文件会显示为四个编号的 bank（共 128 个声部）。也接受单声部文件。原生应用读取这个文件夹；浏览器版本读取它的虚拟文件系统，而不是电脑上的任意文件夹。

有问题或不受支持的文件会被跳过，并在屏幕上显示数量；有效的 bank 仍然可用。扫描限制为 2,048 个文件、每个文件 1 MiB、总计 64 MiB、60,000 个声部和八层嵌套的文件夹。符号链接会被忽略。所选的音色归乐器所有：删除其来源 bank 不会改变已保存的歌曲。完整的浏览库保留在这个文件夹中，不会被整体复制到每个项目里。

LOAD INSTRUMENT 也接受原版 DX7/TX7 单声部和 32 声部 bank 的 `.syx` 转储，包括这些受支持消息的有限序列。它会检查帧结构、字节数、七位数据、校验和、参数范围和文件大小，然后才在同一个 FM 浏览器中打开导入的 bank。选择一个音色即确认；取消则保留当前歌曲的乐器。这个直接导入的快捷方式在会话期间保留其浏览列表；要持久地浏览，请使用上面的 bank 文件夹。请保存歌曲或一个 `.cni`，以保留所选/所编辑的音色。没有头部的转储、校验和错误的转储、DX7II 的演奏/扩展以及其他 Yamaha 系列都会被拒绝。导入从不向外部设备发送 MIDI 消息。

CNI 和项目版本 6 保存全部 155 个原始声部字节、完整的显示名称、原生的击键力度、微调和来源标识。歌曲重新打开时不需要外部 bank。较旧的 ChooChoo 版本无法读取这些版本 7 的文件；仅含已有内容的项目仍保存为版本 5。原生力度默认为 100；Tracker 的音量是合成之后的增益，不会重新敲击包络。算子包络、固定频率模式、keyboard scaling、音高包络和 LFO 仍是音色原生的行为。可选的通用 FM 幅度 ADSR 为它们的合并输出塑形，而不替换算子包络。

每个轨道部分有一个 LFO 和四个各自独立持有的和弦声部槽，与 Tracker 现有的和弦/替换策略一致。轨道之间不共享音符状态。Note-off 释放原生包络；cut/panic 清除它们。标量的 MSFA 内核以固定的内部 44.1 kHz 使用 64 个采样的量化单位，并通过流式 FIR 转换到输出速率。查找表在音频渲染之外只初始化一次，因此并发的离线渲染器和实时渲染器无法改变彼此的速率。事件在下一个内部量化单位（最多 1.45 ms）生效，随后是 FIR 约 0.25 ms 的群延迟。缓冲的采样会跨回调保留。这是 MSFA Modern 一脉，并不声称与 DX7 硬件逐位一致，也不是 Dexed Mark I 的模拟。DX7 限制为 **整首歌曲最多 16 个活动音符**，包括 release 尾音，同时每条轨道保留四个自己的和弦槽。超出预算时，它先丢弃 release 尾音，然后是最安静的持续音符，再然后是新的 attack。attack 相同时，跨轨道保留根音，优先于和弦扩展音，并以稳定的槽/轨道顺序决胜。这个策略适用于每个平台，因此同一首歌具有同样的、有界的音符分配。它不会改变其他乐器的复音数。播放期间预设试听被禁用。这个上限选自 R36H 的测量，并在混合播放中得到验证。

#### FM 算子编辑器

Genesis FM、Arcade FM、OPL2、OPL3、OPLL 和 VRC7 乐器在 Fine ct 的右边有一个 **EDIT** 按钮。它会打开一个音色参数的表格：

- Genesis FM / Arcade FM：algorithm、feedback、pan、AMS、PMS、算子掩码和 LFO 设置，然后是四个算子各自的 MUL、DT、TL、KS、AR、D1R、D2R、RR 和 D1L，另外还有 SSG（Genesis）或 DT2 和 AM（Arcade）。
- OPL2 / OPL3：每个声部的 feedback、connection 和 pan，deep vibrato 和 tremolo，然后是每个算子的 MUL、TL、AR、DR、SL、RR、waveform、KSL、VIB、TRM、SUS 和 KSR。在 OPL3 上，可以在这里更改模式（2op、4op、dual）。
- OPLL / VRC7：自定义音色的寄存器（feedback、MUL、TL、KSL、AR、DR、SL、RR、waveforms、AM、VIB、EG、KSR）。编辑 ROM 预设会把它变成“Custom”。

Left/Right 和通常的编辑键改变数值；乐器照常继续发声，让你能听到变化。状态栏描述光标下的参数。OPT 或 Left+Shift 返回乐器屏幕。DX7 和 SID 没有算子编辑器。

Genesis FM（YM2612）和 Arcade FM（YM2151）现在使用同样的 FM Bank/Preset 浏览器、EDIT+PLAY 试听、确认/取消和微调控件。Genesis 现在有 **73 个预设**：原有的 24 个，以及来自 NeoSoundFonts 的 CC0 16-Bit FM Music Station bank 的 49 个受支持的旋律程序。Arcade 有 **81 个预设**：原有的 24 个，以及来自 YMulator-Synth 的 GPLv3 合集的 57 个受支持且有声的程序。ChooChoo 原有的这一对共享其底层配方；新的来源合集提供各自独立的调色板。这些是有名称的声音设计合集，并不声称重现了任何特定游戏的原声。不受支持的音符偏移、固定的打击乐键、街机的噪声设置和无声的源程序，都在 `expansion-manifest.json` 中记为排除项。它们完整的四算子音色、包络、LFO、立体声和调音设置都包含在版本 6 的乐器和歌曲中。原生芯片的包络控制 release；普通的 Tracker 增益和音高不会重新触发它。Genesis 的 DAC 输出使用一个 20 Hz 的直流阻断器。噪声模式和通道 3 的特殊频率模式不在这个乐器实现的范围之内。

对于本地的外部 bank，请使用 `tools/chip_banks/import_bank.py SOURCE --output
NEW_DIRECTORY --writer tracker/build/tests/chip_factory`。它接受严格的 42 字节 TFI、VOPM 的 OPM 文本、经验证的 WOPLX 格式和原版 DX7 SysEx。通过 Load Instrument 加载生成的 `.cni` 文件。用户导入的内容与可分发的出厂库分开存放。启用了噪声、噪声频率数据非零或部分声像的 OPM 文件会被拒绝并附带说明；VOPM 的 pan 值 0/64/127 变成 左/两侧/右。不支持二进制的 WOPL。
共享的 FM 目录包含 **1,064 个条目**：697 个 OPL、67 个 DX7、146 个 OPLL/VRC7、73 个 Genesis 和 81 个 Arcade。另外 76 个文件通过普通的文件浏览器公开 Sega/Game Boy 的预设，总计 **1,140 个打包的原生预设**。来源修订、哈希值、完整声明和原始来源数据，随新合集一起放在 `licenses/chip-banks/expansion` 下。

### 便携的出厂合集与 USER 预设

原生芯片乐器使用 **Bank:** 在 ALL、一个内置合集或 USER 之间选择。**Preset:** 在该选择之内挑选一个声音。在任一行上按住 EDIT 并按 Left 或 Right 可循环切换；短按 EDIT 打开其选择器。

内置合集是 `instruments/FACTORY/` 下的 ZIP 包。USER 文件放在 `instruments/USER/<engine>/` 下：`dx7`、`opll`、`vrc7`、`opl2`、`opl3`、`genesis`、`arcade`、`sid`、`sega`、`gb-pulse` 或 `gb-noise`。可以直接浏览文件夹，或把兼容预设的 ZIP 放在那里。ZIP 在磁盘上保持原样。所有这些文件夹都接受兼容的 `.cni` 文件；CNI 是 ChooChoo/ChipNomad 的便携乐器格式，不是芯片制造商的标准。USER 浏览器还会读取下列来源格式，包括 ZIP 内的文件：

| USER 文件夹 | 附加格式 |
| --- | --- |
| `dx7` | Yamaha DX7/TX7 的 `.syx` 单声部和 32 声部 bank |
| `genesis` | `.tfi`、DefleMask 的 `.dmp`、Furnace 的 `.fui` |
| `arcade` | VOPM 的 `.opm` bank、DefleMask 的 `.dmp`、Furnace 的 `.fui` |
| `opl2`, `opl3` | `.wopl`、`.opli`、`.woplx`、`.sbi`、Furnace 的 `.fui` |
| `opll`, `vrc7` | Furnace 的 `.fui` 自定义音色和固定的 ROM 选择 |
| `sega`, `gb-pulse`, `gb-noise` | Furnace 的 `.fui`、DefleMask 的 `.dmp` |
| `sid` | GoatTracker 2 的 `.ins`（GTI5）、Furnace 的 `.fui` |

一个 bank 会展开成兼容的声部。不受支持的文件或功能会报告原因；混合 bank 中不兼容的声部会被排除，而它们的来源编号保持稳定。加载是事务性的：被拒绝的预设会让当前乐器保持不变。来源预设会用一个空的 table 替换乐器的 Tracker table，只有 DX7 SysEx 例外，它保留现有的 table。CNI 恢复它所保存的 table。加载的音色归歌曲所有，在移动或删除来源包之后仍可继续使用。

WOPL 的 bank 可能包含没有显式空白标志的未用槽。浏览器会省略没有名称、其活动算子被完全衰减且没有 attack 的槽。没有名称但可演奏的声部仍以 `Program N [Bbank]` 或 `Drum N [Bbank]` 的形式可用，使用来源中从零开始的程序编号和 bank 编号。没有名称的单个 OPLI 乐器使用其文件名。有名称的静音音色会被保留。

Furnace 的支持涵盖旧版的单乐器版本 29-112 和基于功能的 `FINS` 版本 127-251。DefleMask 的支持涵盖版本 11 的 OPN、OPM、Sega PSG 和 Game Boy 乐器。这些是乐器导入，而不是完整的 Tracker 播放器：带动画的 FM/算子宏、来源歌曲的 FM LFO 设置、采样、Game Boy 的硬件命令序列、相对的 SID pulse/filter 宏，以及 SID 的 ring/sync 依赖，目前都会被拒绝。支持固定的 OPLL ROM 选择；VRC7 对该选择使用它自己的 ROM。SID-Wizard 的 `.swi` 和 SID 音乐文件（`.sid`）在这个版本中不属于乐器导入。

受支持的 PSG/GB/SID 序列保存在音色中，Furnace/DefleMask 按 60 Hz 播放，GoatTracker 按 50 Hz 播放。这些文件不携带完整歌曲的回放配置：当需要它歌曲专属的 tick 速率、通道路由、hard-restart 行为或兼容性设置时，请使用原来的 Tracker。对于 Sega PSG，duty 宏选择噪声声部；没有它时，导入会使用音调声部。Game Boy 的文件必须放在预定的 pulse 或 noise 文件夹中。导入的序列目前仍是嵌入的回放数据；乐器页面编辑的是基础参数，而不是来源 Tracker 的 table。

USER 预设的循环会跨越文件、文件夹、bank 和 ZIP 包，双向进行，在两端回绕。重新打开 USER 浏览器会跟随当前的选择。对于有类别的引擎，ALL 把 USER 的声音包含在 Unsorted 之下；USER 仍是单独的顶层选项。Sega PSG 和 Game Boy 使用扁平的 ALL/Factory 列表和分层的 USER 浏览器，没有 Unsorted 面板。`instruments/banks/dx7/` 中较旧的 DX7 文件在 USER 中仍可访问。

引擎只显示与它兼容的内置合集。Factory Presets 合并所选引擎内置的 OPLL/VRC7 音色集；DX7 合并 ChooChoo 和 YSE 的原创。下载的有名称合集保持独立。Fat Man 2-op 列在 OPL2 之下，Fat Man 4-op 列在 OPL3 之下。兼容的 OPL2 CNI 文件仍然可以在 OPL3 的 USER 文件夹中使用。

原生引擎的输出使用固定的、经过测量的增益补偿，而不是每个预设的归一化。因此，已有的、使用原生乐器的歌曲可能以不同的电平播放；请检查你已保存混音的平衡。校准方法和范围记录在 `scripts/README_MEASURE.md` 中。

`native-chip-audition.cct` 提供对原有十三个出厂 bank 的简短顺序试听。每个部分使用不同的、自有的乐器，所以即使移除预设文件夹它也能工作。更长的、被 Git 忽略的 bank WAV 和测得的电平列在 `docs/chip-preset-auditions.tsv` 中；主观的试听仍待进行。

### 原生芯片控件 (Native chip controls)

原生乐器的波形预览会在其控件重绘时刷新。FM 乐器可以叠加其可选的 Amp env 曲线；SID 保留其波形预览，而不带这种仅限 FM 的叠加，在浏览 bank 和预设时也是如此。

所有七个 FM 引擎都提供 **Bright**、**Feedback** 和 **Amp env**。Bright 的范围为 -63 到 +63，零保持音色不变；它改变调制算子的电平，同时保留载波的电平。其听感效果取决于算法。Feedback 默认为 **Preset**，也可以用 0-7 覆盖原生的反馈。这些控件不会改写所保存的原生算子字节。

Amp env 默认为 **Bypass**。选择 **ADSR**，可在原生声音周围加上 attack、decay、sustain、release 和形状。A/D/R 使用共享的 0-5 秒二次范围；sustain 从静音到满电平。原生包络仍会运行，所以这个包络无法把声音延长到超过其原生的 release。预设的浏览会保留该槽的 FM 幅度和音色控件。所有 FM 声部还使用 3 ms 的起音/重新触发过渡和 1 ms 的 Tracker 增益平滑，Amp env 处于 bypass 时也是如此。Hard cut/panic 仍是即时的。

下面的 phrase FX 也作为受支持的 Modulation 和动作录制目标出现。FX 列中的值是十六进制。这些设置应用于回放，而不改变所保存的乐器。原生 GB 的扫频和包络控件在下一个音符触发时被锁存；duty/宽度和噪声频率可以实时改变。

| 引擎 | FX | 控件 |
|---|---|---|
| Native FM | `OL1`-`OL6` | 绝对的算子输出电平：OPLL/VRC7 调制器和 OPL 为 `00-3F`；OPLL/VRC7 载波为 `00-0F`；Genesis/Arcade 为 `00-7F`；DX7 为 `00-63`（0-99）。越高表示输出越大。只显示受支持的算子。 |
| OPLL/VRC7/OPL2/OPL3 | `OAR`, `ODR`, `ORR`, `OSL` | 算子 1 的 attack、decay、release 和 sustain 衰减，`00-0F`。 |
| OPLL/VRC7/OPL2/OPL3/Genesis/Arcade | `OMU 00-0F` | 算子 1 的频率倍数。 |
| Genesis/Arcade | `LFR` | 原生 LFO 速率：Genesis 为 `00-07`，Arcade 为 `00-FF`。 |
| Arcade | `LAD`, `LPD 00-7F` | 分开的 LFO 幅度深度和音高深度。 |
| Genesis/Arcade | `LAS`, `LEN` | 原生的幅度灵敏度 `00-03` 和 LFO 启用 `00-01`。 |
| Genesis/Arcade | `LPS 00-07` | 原生的音高灵敏度。 |
| SID | `SAT`, `SDE`, `SSU`, `SRL 00-0F` | 原生的 attack、decay、sustain 和 release。时间值向 `0F` 增大；sustain 向满电平增大。 |
| SID | `SPR 01-10` | 静音伙伴的频率，从 1× 到 16×；影响环形调制和硬同步。 |
| 所有原生 FM | `FBK 00-07` | 绝对的原生反馈，由乐器初始化。 |
| 启用 Amp env 的所有原生 FM；Sega/GB | `EAT`, `EDC`, `ESU`, `ERL`, `ESH 00-FF` | Attack、decay、sustain、release、形状。它们不会启用处于 bypass 的 FM 幅度。 |
| Sega PSG | `CMD 00-02`, `CNR 00-03` | 音调 / 白噪声 / 周期噪声；三种固定的噪声速率或由音调派生的速率。 |
| GB Pulse | `CMD 00-03` | 原生的脉冲占空比。 |
| GB Noise | `CMD 00-01`, `CND 00-07`, `CNS 00-0D` | 噪声宽度、时钟除数和移位。 |
| GB Pulse | `CSP 00-07`, `CSS 00-07`, `CSD 00-01` | 扫频周期、移位、向下方向。 |
| GB Pulse / Noise | `CEI 00-0F`, `CEP 00-07`, `CED 00-01` | 原生包络的初始电平、周期、上升方向。 |

原生 FX 的选择使用 phrase 行 `I` 列中的乐器；当 `I` 为空时，则使用向前查找到的活动乐器。FX 弹窗在一个紧凑的块中显示控件的简短说明、它当前的预设值和有效的命令范围。选择另一个原生效果时，会从那个值开始；重新打开同一个效果会保留其已编辑的值。多行选择会分别解析每一行的乐器。Table 使用它们的乐器上下文。原生数值的编辑在其合法端点处停止，包括 duty、噪声、扫频、ADSR、反馈和算子电平。这些值是十六进制的：例如，DX7 的最大值 `63` 表示十进制的 99。

这些 FM 命令使用绝对的原生值。算子命令始终针对算子 1；`OMU 03` 选择其倍数 3。DX7 专属于 Tracker 的控件是算子电平和反馈。Preset/Range 保持其信息颜色；标题和说明遵循与 FM Feedback 相同的颜色。

实时 Modulation 保留其完整的固定算子原生目标集合。动作录制只写入紧凑的 Tracker 列表中所表示的控件；算子参数只为算子 1 录制。其他调制目标在实时中继续工作，但不会生成 phrase 命令。

`SCP` 和 `SCT` 保留以字节缩放到宽于 8 位的 SID 寄存器的映射。`SMR 01-C8`、`SWV 01-08` 和 `SPR 01-10` 现在与原生的预设编号一致。显示的预设值是基础设置，与正在播放的包络或 LFO 无关。`SLE` 作用于算子电平和直接的 FM 参数，而不会重新触发音符或改变所保存的音色。

所有原生 FX 在 phrase 的 FX 选择器中都有描述性的标题、范围和行为，另有数值提示。`CMD` 根据所选的乐器，描述音调/噪声、duty 或噪声宽度。这些控件是对共享的 Track、Envelope 和 Modulation 组的补充；仅有引擎这一组，并不是可用 phrase 效果的完整集合。

Braids、Plaits、PCM 和其他受支持引擎上现有的共享 LP/BP/HP 滤波器，是合成之后的软件处理。aChChid 则使用它原生的 303 滤波器通路。这一轮没有为 AY 或原生 FM 增加滤波器；brightness 改变的是 FM 合成本身。Track insert 仍可用于额外的处理。

原生乐器以 CNI 版本 7 保存（存在绝对命令时为 8），原生歌曲以 CCT 版本 9 保存。包含导入序列的乐器使用 CNI 版本 9，包含它们的歌曲使用 CCT 版本 10。较旧的版本无法打开这些带序列的较新文件。已保存的 FM 预设字节和乐器页面的音色设置保持不变。

### 原生芯片的掌机负载指南

共有八条歌曲轨道，**每条轨道有两个 insert 效果槽**。可用的十六个槽并不保证有足够的 CPU 来运行十六个效果。DX7 的十六个活动音符的上限，是整首歌曲范围内独立的合成预算；和弦和 release 尾音都计入其中。

在 R36H 的测量中，四条循环的 WAV 轨道、Sega PSG、GB Pulse、DX7，以及带共享发送和四个 insert（两个 Compressor、Doubler、TAPESCAM）的 OPL3，表现优于密集的八轨 FM 编排。单音版本在其三十秒的 48 kHz/512 帧测量中，没有出现渲染截止时间的错失；加上一个四音的 DX7 和弦后，出现了一次时序尖峰。带有许多高开销 insert 的密集 FM 歌曲超出了 CPU 预算。请留出一些余量，留意现有的过载指示，并在对歌曲有帮助的地方添加效果。应用保留每条轨道和每个槽；对 insert 的数量没有新的硬性上限。完整的测量和持续测试的状态，请参见 `chip-instruments-report.md`。

对该均衡和弦编排所做的相应的 70 秒实体音频测试，在现有的 48 kHz / 4906 帧设置下通过，没有渲染截止时间的错失，也没有记录到 ALSA 的欠载。其最坏的一次回调为 81.819 ms，而截止时间为 102.208 ms。测试使用了单独的 0.4 的总增益来留出余量。请保留常规启动器的直连声卡路径 `AUDIODEV=plughw:0,0`：系统默认的共享混音器在诊断探测中产生了欠载。没有更改任何用户的音频设置。使用较小缓冲区的十分钟压力测试仍记录到 28 次时序尖峰；完整的结果见报告。

### SID 乐器与 Phrase FX

选择 **CHIP → SID**。Bank 提供 **ChooChoo SID Originals**（32 个创作的程序）和 **SIDkit Effects**（24 个采用 MIT 许可的效果）。两者使用同一个预设浏览器，并把所选程序的完整内容保存在乐器和歌曲中。分享歌曲时不需要外部的 bank。这些是带有包络和动作配方的原生 SID 参数程序，不是采样音频，也不是完整的 C64 歌曲。另外十二个 GoatTracker 研究候选项没有随附；它们的 wave/pulse/filter table 需要专门的导入器/播放器。

乐器页面提供波形/脉宽、滤波器模式/截止频率/共振和原生的 ADSR 控件。SID 使用版本已固定的 floooh/chips 的数字振荡器、包络和逐周期的 6581 风格滤波器。没有备选的芯片型号、修订版选择器，也没有额外增加的特性配置。这是一种通用的近似，不是经过校准的 R2/R3/R4/8580 模拟模型。

每个音符有它自己的滤波器。Ring/sync 使用一个静音的伙伴振荡器，所以滤波器共享和三声部的相互作用与真实的 SID 不同。掌机在整首歌曲范围内有四个普通 SID 音符的预算。Ring 或 sync 每个音符消耗两个预算单位，因此可以有两个这样的音符，或一个加两个普通音符。和弦和 release 尾音都计入。已释放的音符最先被淘汰；新的音符优先于较早持续的音符。高开销的 insert 和其他合成器仍然共享音频的 CPU 预算。

| FX | 范围 | 作用 |
| --- | --- | --- |
| `SCP` | `00-FF` | 脉宽基准，缩放到原生的 12 位宽度；配方的脉冲动作仍然有效。 |
| `SCT` | `00-FF` | 截止频率基准，缩放到原生的 11 位截止频率；配方的滤波器动作仍然有效。 |
| `SRN` | `00-0F` | 原生共振。 |
| `SWV` | `00-07` | Triangle、saw、tri+saw、pulse、tri+pulse、saw+pulse、tri+saw+pulse、noise。 |
| `SFI` | `00-07` | 滤波器模式位：1 低通，2 带通，4 高通；0 为 bypass。 |
| `SMR` | `00-FF` | 配方宏的时钟，缩放为 1 到 200 Hz。 |
| `SRG` | `00-01` | 与静音伙伴的三角波环形调制。 |
| `SSY` | `00-01` | 与静音伙伴的振荡器同步。 |

脉宽和截止频率支持 `SLE`。离散的波形、开关、共振和 FM 反馈保留其有用的原生步进；更宽的字节值不会产生更多的硬件状态。`FBK` 直接选择反馈 `00-07`。算子电平使用上面列出的各引擎专属的范围。Brightness 和总增益的调整保留完整的字节范围。

### 便携的出厂合集与 USER 预设

原生芯片乐器使用 **Bank:** 在 ALL、一个内置合集或 USER 之间选择。**Preset:** 在该选择之内挑选一个声音。在任一行上按住 EDIT 并按 Left 或 Right 可循环切换；短按 EDIT 打开其选择器。

内置合集是 `instruments/FACTORY/` 下的 ZIP 包。USER 文件放在 `instruments/USER/<engine>/` 下：`dx7`、`opll`、`vrc7`、`opl2`、`opl3`、`genesis`、`arcade`、`sid`、`sega`、`gb-pulse` 或 `gb-noise`。可以直接浏览你自己的文件夹，或把包含文件夹和兼容 `.cni` 预设的 ZIP 放在那里。DX7 还接受受支持的 `.syx` 单声部和 32 声部 bank，包括 ZIP 内的文件。ZIP 在磁盘上保持原样。不支持直接加载 WOPL/WOPLX；请使用兼容的 CNI 预设，或先离线转换受支持的来源格式。

USER 预设的循环会跨越文件、文件夹、bank 和 ZIP 包，双向进行，在两端回绕。重新打开 USER 浏览器会跟随当前的选择。对于有类别的引擎，ALL 把 USER 的声音包含在 Unsorted 之下；USER 仍是单独的顶层选项。Sega PSG 和 Game Boy 使用扁平的 ALL/Factory 列表和分层的 USER 浏览器，没有 Unsorted 面板。`instruments/banks/dx7/` 中较旧的 DX7 文件在 USER 中仍可访问。

引擎只显示与它兼容的内置合集。Factory Presets 合并所选引擎内置的 OPLL/VRC7 音色集；DX7 合并 ChooChoo 和 YSE 的原创。下载的有名称合集保持独立。Fat Man 2-op 列在 OPL2 之下，Fat Man 4-op 列在 OPL3 之下。兼容的 OPL2 CNI 文件仍然可以在 OPL3 的 USER 文件夹中使用。

原生引擎的输出使用固定的、经过测量的增益补偿，而不是每个预设的归一化。因此，已有的、使用原生乐器的歌曲可能以不同的电平播放；请检查你已保存混音的平衡。校准方法和范围记录在 `scripts/README_MEASURE.md` 中。
