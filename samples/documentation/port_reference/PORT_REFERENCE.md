# Dorfplatformer9 native port — source reference

Recovered from the uploaded SB3. This is a static analysis, not a tested native implementation.

## Sprite inventory

|Target|Block entries|Costumes|Sounds|Procedures|
|---|---:|---:|---:|---|
|Stage|257|7|1|Day1|
|Blank|4|1|1||
|Boss|231|8|1|Clone, Tick, Animate Cat, Prall|
|Enemy|265|11|3|Animate Cat, Tick, Clone|
|Player|537|5|4|Animate Cat, Keys, Tick, sdfghj.-|
|Teme Day|209|4|7||
|Levels|817|30|1|Tick, Clone, Maker, wertzuio|
|Fane+|89|1|4|Tick|
|fane|84|1|1|Tick|
|Sprite2|0|1|21||
|Figur6|18|3|1||
|Teme N|213|4|7||
|Dragon|233|4|2|fall|
|Power up|39|1|2||
|Menu|41|1|7||
|Dynamick|118|4|3||
|Player out|153|12|2|Tick|
|Tobo|6|1|1||
|Player body|210|11|2|Tick, Game|
|Ball|221|1|1|Down, Tick, Animate Cat|
|fane pos|36|1|1|Tick|
|Died/Pipie|185|7|4|Loop %s %s %b|
|Water|14|1|1||
|menu|106|6|1|Clone|
|quit|20|1|1||
|Final Boss|203|8|0|Clone, Tick, Animate Cat, Prall|
|Fade|14|1|1|Set|
|Sprite1|179|24|1|scan map, Stamp|
|Blank2|0|1|1||
|Save + Load|190|1|1|ExtractNextPiece, Make Save Code, Load Save Code, S|
|Sprite3|6|1|1||
|Figur3|25|1|0||
|Figur4|9|1|0||
|Sprite4|0|1|1||
|Game -Save + Load|103|1|1|ExtractNextPiece, Make Save Code, Load Save Code, Make part Value %s _ %s|
|Figur8|37|4|2||
|Figur1|127|9|0||
|Figur2|15|1|0||
|Intro/BadEnding|69|2|0||
|Figur7|43|5|0||
|Figur9|71|11|0|v|
|Figur5|12|1|0||
|Phone Controls|98|6|0||
|Sprite5|0|2|0||
|Sprite6|24|1|0|add up fps|

## Global variables

|Name|Initial serialized value (truncated if long)|
|---|---|
|Teme|1|
|Levels #|1|
|Cam x|900|
|Win leve|0|
|my x pos|0|
|My y|48|
|Travel|1|
|win|0|
|Powerup|F|
|DAMAGE|0|
|Plus|0|
|kill|true|
|Mode|Play|
|Make|0|
|Hit|3|
|Play|1|
|Pause|1|
|MA|0|
|By|1-1|
|Trap X|0|
|Y world|0|
|TRANZ|1|
|teme|2|
|PLAYER TEME|2|
|_PIE #||
|__COS|0|
|SaveLetter|11119|
|SavePartExtracted|2|
|SaveCode|[code]&#124;1-3&#124;139&#124;175&#124;211&#124;247&#124;391&#124;427&#124;463&#124;499&#124;607&#124;643&#124;679&#124;787&#124;823&#124;859&#124;895&#124;1579&#124;1615&#124;1651&#124;1687&#124;1723&#124;1759|
|Info 1|889|
|Info 2|688|
|Info 3|845|
|Level #|1|
|___LEVELs|0|
|Time|999999999|
|E|1|
|Day|2/3|
|N|1|
|%|44.880952380952664|
|a____SS|1|
|J B|-5142|
|Timer|463|
|ND|0|
|PIX|16|
|Speed Timer|307.445999999998|
|Final Boss|0|
|Boss HP|0|
|FPS|60.24459580478124|

## Global lists

|Name|Length|
|---|---:|
|Walk|4|
|Level|976|
|Level 2|976|
|Level 3|976|
|Output|1|
|NO?|126|
|Mop x|0|
|Mop y|0|
|SaveCode|2|
|Levels|6|
|Lev|976|
|GGG|4|
|a_input|5|

## Broadcasts

- Still
- Laut
- WIN
- Start
- In
- Out
- Trans
- Star level
- Setup level
- Win
- Player move out level & back to map
- Game over
- Move all
- Died
- Power Up
- Power down
- Kill mop by free
- Yump
- Delete all mopss
- Up
- Re start
- New menu
- Create
- Open
- Save
- Load
- New game
- Lave
- Setup moops
- Travel
- Trank
- Hide player
- Toi died
- Neue
- Player Tick
- Scroll X
- trans 2
- Scroll
- live hurt
- Back
- player x
- player:y
- lives
- player:cos
- Save Your Game
- Load Youer Game
- update
- Restart Level
- Back to map
- stop map
- intro
- Tran Lev
- Title Code
- Start Game
- Intro Ani
- Game over World
- Game Over Ani
- Time Reset
- New Day
- END Time
- Good Ending
- GTC
- start level v1
- boss

## Level data

Four sections split on `>`: `[code]`, name and X coordinates separated by `|`; Y coordinates; type IDs; two metadata values and `[/code]`. Keep object ordering and duplicate coordinates. Metadata semantics not yet traced.

- 1-1: 976 objects, metadata ['1', '2']
- 1-2: 773 objects, metadata ['1', '2']
- 1-3: 123 objects, metadata ['1', '3']
- 1-4: 835 objects, metadata ['1', '4']
- 1-5: 235 objects, metadata ['5', '3']
- 1-6: 1568 objects, metadata ['6', '2']

## Extensions

### dtbyxeroname
- `dtbyxeroname_dt`: 59 block entries
- `dtbyxeroname_fps`: 2 block entries
### runtimeoptions
- `runtimeoptions_getFramerate`: 1 block entries
- `runtimeoptions_greenFlag`: 2 block entries
- `runtimeoptions_setDimensions`: 8 block entries
- `runtimeoptions_setFramerate`: 1 block entries
### lmsTempVars2
- `lmsTempVars2_changeThreadVariable`: 3 block entries
- `lmsTempVars2_getThreadVariable`: 12 block entries
- `lmsTempVars2_setThreadVariable`: 9 block entries
### Gamepad
- `Gamepad_axisValue`: 6 block entries
- `Gamepad_buttonDown`: 6 block entries
- `Gamepad_menu_axisMenu`: 6 block entries
- `Gamepad_menu_buttonMenu`: 6 block entries
- `Gamepad_menu_padMenu`: 12 block entries
### pen
- `pen_clear`: 6 block entries
- `pen_stamp`: 21 block entries
### obviousalexsensing
- `obviousalexsensing_getFingersTouching`: 1 block entries
- `obviousalexsensing_touchingFinger`: 4 block entries

## Port requirements

- Preserve broadcast-and-wait completion, clone-local state, Scratch coercion and 1-based lists.
- Preserve Scratch coordinates (+Y up), costume rotation centers, alpha/color collision behavior, effects, layer order and pen stamps.
- Reproduce original delta-time behavior before introducing fixed-step physics.
- Player, Levels, Stage, enemies, boss and save handlers require behavioral parity tests.
- Keep level and progress encodings distinct; do not infer metadata semantics from initial values.
- `project.json` contains the complete block graph and asset references; `levels.json` contains decoded levels; `scripts.txt` indexes all block graphs with fields and inputs.
- No native game build or runtime parity testing has been performed.