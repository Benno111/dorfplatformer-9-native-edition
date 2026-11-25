# Dorfplatformer9 audio port specification

Static source analysis; no runtime listening or verification. Duration values are sampleCount/rate metadata. Preserve sound bank order, per-sprite volume and effects, and global stop semantics.

## Music
Menu green flag plays Drifting Intro until done, then repeats Drifting Looped until done. Re start repeats sound selector 6 (exact name exists in Menu). Dynamick green flag plays Coin; Game over plays Game Over, waits 5 seconds, restarts.

Teme Day and Teme N each start at volume 0 and pitch 0 on Create, stop all sounds, and loop independently. Normal world uses Teme, other world uses teme (distinct IDs). Day controller skips its main playback branch when Final Boss > 0; night controller plays selector 6 in that state. At Time=100 both normal branches request recording1 then pitch 25; recording1 is absent in day bank. Special a____SS > 0: day plays Jungle 12 times then clears special; night has empty special branch. Both have empty-condition sub-branches that must not be treated as unconditional.

Move all: day volume 100 if special > 0 or (Play=1 and N=0), otherwise 0. Night volume 100 if not special and Play=1 and (N=1 or Final Boss>0), otherwise 0. No crossfade in these handlers. Still mutes both, Laut and Setup level stop all and set 100. Separate Create monitor clears effects if special; otherwise checks changes in teme and Y world using thread-local variables and stops all sounds on change, sets pitch 5 on day 3 else 0. This can override pitch 25. Died, Lave, WIN, Good Ending, Game Over Ani stop other scripts in sprite; see actual graph for additional actions and exact capitalization. Some handlers are empty or disconnected. Do not turn them into active events.

## Effects
Player Died sets pitch 90 and plays Cymbal Echo until done, clears effects and later stops all; Yump starts Kill mopps; normal jump starts Jump, other power states Jump2. Died/Pipie Died starts Kill player and Cymbal Echo; Travel starts Travel and later stops all; Game Over Ani starts Kill player then waits for Cymbal Echo. Fane+ Win starts Elec Piano Loop. Power up sprite plays Power down or Power up; special activation stops all then starts Power up. Boss/Enemy use Kill, including Kill mop by free. Dragon start level v1 has five Kill playback sites. Ball Create plays Ball. Figur8 New Day plays Engine if day number >2 else Clang. Water has a detached loop with movement/jump and Play=1 conditions; do not assume active without runtime verification. Final Boss has Kill references but no local sounds. Stage A Guitar is detached.

## Runtime semantics
Implement asynchronous start versus coroutine wait-until-done, global stop-all, per-target sound bank resolution (name first, numerical selector fallback subject to original runtime rules), independent playback clocks, per-target pitch/volume. Pitch rate approximation for Scratch-style effects is 2^(pitch/120); validate against original runtime. Do not block physics while waiting for sound. Preserve concurrent stop and event ordering. Avoid automatic synchronization or crossfade if fidelity is required.

## Asset inventory

### Stage

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | A Guitar | 1.417 | 48000 | `0c9c1e651d8aef6d5262c2963d793947.wav` |

### Blank

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Blank | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Boss

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Kill | 0.361 | 48000 | `f0fa1266992c0307c094adf8c2c9e344.wav` |

### Enemy

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Kill | 0.361 | 48000 | `f0fa1266992c0307c094adf8c2c9e344.wav` |
| 2 | Jump | 0.337 | 48000 | `60fe1c2e28c61a1ad39b787720213491.wav` |
| 3 | Jump2 | 0.145 | 48000 | `73bd2ea6a8081bd264cc246853a14945.wav` |

### Player

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Cymbal Echo | 8.569 | 48000 | `b5134abf271035aca57e956b9f16fa23.wav` |
| 2 | Jump | 0.145 | 48000 | `1682e4eba59078285fa39a00d7dffb95.wav` |
| 3 | Jump2 | 0.361 | 48000 | `2687716aea8ce9d58151a6bdc4d9c01e.wav` |
| 4 | Kill mopps | 0.265 | 48000 | `93156dbeae58629427138952baaf54f6.wav` |

### Teme Day

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Level 1 | 97.585 | 48000 | `cc34bf9540ea410089b4056ac97ab99e.wav` |
| 2 | Level 2 | 31.993 | 48000 | `adbc03834f15abf65071122a057b280a.wav` |
| 3 | Level 3 | 84.097 | 48000 | `6bfe1c556b68fc405acd2c727e145355.wav` |
| 4 | Level 4 | 150.649 | 48000 | `537d51ab06e7565f27997346820adc13.wav` |
| 5 | Level 5 | 34.849 | 48000 | `b017f025b98f5ba40a93cf3d2d1edd50.wav` |
| 6 | Boss | 88.753 | 48000 | `32ca291f6a534daac63f7653e64227d7.wav` |
| 7 | Jungle | 3.456 | 48000 | `5b88631fbda82ed20d60c0290c0af257.wav` |

### Levels

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Fane+

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |
| 2 | Collect | 0.433 | 48000 | `7192db4bcb5b996bdc8be614cd563e82.wav` |
| 3 | Collect2 | 4.585 | 48000 | `96cde69df5a162e3d1346cc201e67e12.wav` |
| 4 | Elec Piano Loop | 3.937 | 48000 | `48daf1b2fe9d4da1f6fbea5afb17dd8a.wav` |

### fane

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Sprite2

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Apocalypse Intro (Full) | 13.513 | 48000 | `e782ec08ebc9e632edee5bd7af8c9fa0.wav` |
| 2 | Apocalypse Intro (Short) | 2.665 | 48000 | `2844da2438a04d9dffcccdaa12df7ac4.wav` |
| 3 | Apocalypse Intro (Short)2 | 111.145 | 48000 | `cd3e54c62c1e723b1fdc3378aee2a259.wav` |
| 4 | Boss_Intro | 11.953 | 48000 | `7291bee32398d5f585808fc3af789545.wav` |
| 5 | Boss_Intro2 | 88.753 | 48000 | `d92783a0055fbfa7d49626dfea03b813.wav` |
| 6 | Apocalypse Loop | 108.433 | 48000 | `bee525f0940a1462eb1ee305148355a6.wav` |
| 7 | Boss_Loop | 76.753 | 48000 | `5b0e9599f866a900ea503cf1d3be5840.wav` |
| 8 | Candy_Desert | 34.873 | 48000 | `c45dd72296f2cfdd801090dd67ce4624.wav` |
| 9 | Candy_Desert_Underscore | 34.849 | 48000 | `149068db041170bfb86d20f0230ec8cf.wav` |
| 10 | Drifting Intro | 8.305 | 48000 | `7e13fa27dc896fbb20a36feb8cf0f9d8.wav` |
| 11 | Drifting Intro2 | 8.305 | 48000 | `7e13fa27dc896fbb20a36feb8cf0f9d8.wav` |
| 12 | Game Over | 15.841 | 48000 | `8d729a9b78dbb290db98f9cb6efd0180.wav` |
| 13 | Garden Rain | 97.585 | 48000 | `a15825769676d962e3295d21649412e9.wav` |
| 14 | Lazy Town | 36.169 | 48000 | `0e782cf18ddb56c37fea0563bac0ba6d.wav` |
| 15 | Mini Boss Intro | 11.473 | 48000 | `4728f072d400cc8a8771ba46a050be89.wav` |
| 16 | Mini Boss Intro2 | 103.657 | 48000 | `f704b1768c62244e931b8e1aca3564bf.wav` |
| 17 | Drifting Looped | 142.273 | 48000 | `5d7afac1cfac511aabe91559cc8f67a7.wav` |
| 18 | Mini Boss Loop | 92.113 | 48000 | `d967701fe46a7fe89683b25329885ab7.wav` |
| 19 | Spooky_Lair | 31.945 | 48000 | `43116c8b8c1154a118a8844613fafa85.wav` |
| 20 | Spooky_Lair_No_Drums | 31.945 | 48000 | `845ee92076869f88f59bf58db5e75687.wav` |
| 21 | White Marble | 84.073 | 48000 | `683bd3f6f281d17e1bc6da0ad40f2dff.wav` |

### Figur6

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Plopp | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Teme N

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Level 1N | 111.121 | 48000 | `a5096ecdf206eba46d99baa9fa18a3bf.wav` |
| 2 | Level 2N | 31.993 | 48000 | `c6aa208929e29e8380085901621eec9d.wav` |
| 3 | Level 3N | 36.169 | 48000 | `75126af342f9f8f78b1fb4d34710365e.wav` |
| 4 | Level 4N | 103.657 | 48000 | `9be0e36d5b3f8df9ce662f53c8e9a015.wav` |
| 5 | Level 5N | 34.849 | 48000 | `e669744a22101d97a8eefb72fdf6a231.wav` |
| 6 | Level 6N | 88.753 | 48000 | `160bcf6f2c8b0afe0475dee8cf5dbdd0.wav` |
| 7 | recording1 | 0.673 | 48000 | `4a694ef4257cc55446163e6e5d71cc32.wav` |

### Dragon

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Magic Spell | 1.945 | 48000 | `ccb3696747aba7d743c1ef711c07dba2.wav` |
| 2 | Kill | 0.361 | 48000 | `f0fa1266992c0307c094adf8c2c9e344.wav` |

### Power up

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Power down | 0.793 | 48000 | `ff25f16d24113038787ad7a26b97076e.wav` |
| 2 | Power up | 0.769 | 48000 | `57c1ae7711d2ec4b4ca70054e4f1fefe.wav` |

### Menu

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | 6 | 105.097 | 48000 | `6dd0c0b9b9ee0328b3e7c063aa5b4b27.wav` |
| 2 | Coin | 0.145 | 48000 | `d6bcfef36269bdac0fa075d45ab7beb2.wav` |
| 3 | Drifting Looped | 142.273 | 48000 | `a0907f50ea78ef9dd9b4e175f6ab6521.wav` |
| 4 | Drifting Looped2 | 142.273 | 48000 | `a0907f50ea78ef9dd9b4e175f6ab6521.wav` |
| 5 | Drifting Intro | 8.329 | 48000 | `d7e916fb816cdc05ce8edb8bae85ed0c.wav` |
| 6 | Drifting Intro2 | 150.649 | 48000 | `2df085ff65ef41dc3a9325dbdfb1cf09.wav` |
| 7 | Garden Rain | 97.585 | 48000 | `a15825769676d962e3295d21649412e9.wav` |

### Dynamick

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Plopp | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |
| 2 | Coin | 2.041 | 48000 | `9696530bad3fc9b4f413122efed7c1cc.wav` |
| 3 | Game Over | 15.841 | 48000 | `8d729a9b78dbb290db98f9cb6efd0180.wav` |

### Player out

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Cymbal Echo | 8.569 | 48000 | `811b22ef163fd9f01cda43012407ffe2.wav` |
| 2 | Jump | 0.145 | 48000 | `28234b9d18953d85278b893260a6929c.wav` |

### Tobo

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Plopp | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Player body

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Cymbal Echo | 8.569 | 48000 | `811b22ef163fd9f01cda43012407ffe2.wav` |
| 2 | Jump | 0.145 | 48000 | `28234b9d18953d85278b893260a6929c.wav` |

### Ball

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Ball | 0.097 | 48000 | `8f688c90849988aa69dc0d8fa24df64d.wav` |

### fane pos

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Died/Pipie

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Kill player | 0.193 | 48000 | `5d73c8f19de4a295ad72171b804d9be7.wav` |
| 2 | Travel | 1.273 | 48000 | `a2d172377198740527a11307f6475f10.wav` |
| 3 | Cymbal Echo | 8.545 | 48000 | `f57a3b67fa86c81e12882ee2169fdd69.wav` |
| 4 | Disconnect | 1.273 | 48000 | `a7025fa93c1ad189c45816f329d566f4.wav` |

### Water

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Water | 1.057 | 48000 | `4435c7d81fa5775ace442a36a5605dc6.wav` |

### menu

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Plopp | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### quit

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Plopp | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Final Boss

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|

### Fade

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Plopp | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Sprite1

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Blank2

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Plopp | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Save + Load

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Sprite3

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Sprite4

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Game -Save + Load

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | pop | 0.000 | 48000 | `9bd32df3fd066f2225d7cb7b0b762f6f.wav` |

### Figur8

| Index | Sound | Seconds | Rate | Asset |
|---:|---|---:|---:|---|
| 1 | Clang | 1.105 | 48000 | `2a01caffd6f5f73bb9ac39e41534aedb.wav` |
| 2 | Engine | 7.729 | 48000 | `f8caabe2472efc5ad4822e41915efb6a.wav` |

## Every audio block

Root-hat association is structural, not proof of runtime reachability. Full inputs and ancestor IDs are in audio_blocks.json and full graph in audio_source.json.

| Sprite | Block ID | Opcode | Root opcode | Root event fields |
|---|---|---|---|---|
| Stage | LB | sound_play | sound_play | {} |
| Stage | LC | sound_sounds_menu | sound_play | {} |
| Boss | dY | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Kill mop by free', 'a1zG/x`of`zCd5/#H,h?']} |
| Boss | M~ | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Kill mop by free', 'a1zG/x`of`zCd5/#H,h?']} |
| Boss | ow | sound_play | sound_play | {} |
| Boss | Np | sound_sounds_menu | sound_play | {} |
| Boss | d( | sound_play | procedures_definition | {} |
| Boss | Ny | sound_sounds_menu | procedures_definition | {} |
| Boss | d[ | sound_play | procedures_definition | {} |
| Boss | NP | sound_sounds_menu | procedures_definition | {} |
| Enemy | d\| | sound_play | procedures_definition | {} |
| Enemy | Od | sound_sounds_menu | procedures_definition | {} |
| Enemy | eq | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Kill mop by free', 'a1zG/x`of`zCd5/#H,h?']} |
| Enemy | OR | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Kill mop by free', 'a1zG/x`of`zCd5/#H,h?']} |
| Enemy | eB | sound_play | control_if | {} |
| Enemy | O} | sound_sounds_menu | control_if | {} |
| Player | q; | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Player | fs | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Player | QJ | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Player | q= | sound_cleareffects | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Player | QK | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Player | rd | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Yump', 'ppA%hc:6cf-GSVCM:D\|5']} |
| Player | QM | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Yump', 'ppA%hc:6cf-GSVCM:D\|5']} |
| Player | Q\| | sound_stopallsounds | data_setvariableto | {'VARIABLE': ['Pause', 'n/r0_n(EOodfCN{rd7v+']} |
| Player | rU | sound_play | procedures_definition | {} |
| Player | Rw | sound_sounds_menu | procedures_definition | {} |
| Player | rV | sound_play | procedures_definition | {} |
| Player | Rx | sound_sounds_menu | procedures_definition | {} |
| Teme Day | R^ | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Still', '=l~0ac2uj;i:FTX_=!C1']} |
| Teme Day | r: | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Laut', '5#S;:vaKEtzud8woDejV']} |
| Teme Day | R` | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Laut', '5#S;:vaKEtzud8woDejV']} |
| Teme Day | r] | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Setup level', '$[8pc%VA7Tj[tpuIl/4I']} |
| Teme Day | Sl | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Setup level', '$[8pc%VA7Tj[tpuIl/4I']} |
| Teme Day | r{ | sound_playuntildone | looks_show | {} |
| Teme Day | Sn | sound_sounds_menu | looks_show | {} |
| Teme Day | r} | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Star level', 'YQMYk.pDz=R`cbC{r[(j']} |
| Teme Day | sa | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Star level', 'YQMYk.pDz=R`cbC{r[(j']} |
| Teme Day | Ss | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Star level', 'YQMYk.pDz=R`cbC{r[(j']} |
| Teme Day | sc | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | sd | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | se | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | f/ | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | Sy | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | Sx | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | sg | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | SB | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | f; | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | SD | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | f? | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | SF | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | f= | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | SG | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | f@ | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | SI | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | SN | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Teme Day | SO | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Teme Day | SL | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Teme Day | ?c | sound_stopallsounds | sound_stopallsounds | {} |
| Teme Day | so | sound_stopallsounds | control_if_else | {} |
| Teme Day | ss | sound_stopallsounds | control_if_else | {} |
| Teme Day | sw | sound_stopallsounds | control_if_else | {} |
| Teme Day | ?g | sound_stopallsounds | sound_stopallsounds | {} |
| Teme Day | S; | sound_cleareffects | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | S? | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | S[ | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | S] | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | S^ | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme Day | sH | sound_playuntildone | control_if_else | {} |
| Teme Day | S{ | sound_sounds_menu | control_if_else | {} |
| Teme Day | sI | sound_playuntildone | control_if_else | {} |
| Teme Day | S\| | sound_sounds_menu | control_if_else | {} |
| Teme Day | Tc | sound_stopallsounds | sound_stopallsounds | {} |
| Teme Day | Tf | sound_seteffectto | control_if_else | {} |
| Teme Day | Th | sound_seteffectto | control_if_else | {} |
| Teme Day | sN | sound_stopallsounds | control_forever | {} |
| Teme Day | sP | sound_stopallsounds | control_forever | {} |
| Teme Day | Tm | sound_playuntildone | sound_playuntildone | {} |
| Teme Day | Tn | sound_sounds_menu | sound_playuntildone | {} |
| Teme Day | Tp | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Levels | Wc | sound_stopallsounds | event_broadcastandwait | {} |
| Levels | Wt | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Levels | Ww | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Fane+ | hM | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Win', '?zr{oOqdOh0lSoID.Z$s']} |
| Fane+ | Xx | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Win', '?zr{oOqdOh0lSoID.Z$s']} |
| Teme N | Yz | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Still', '=l~0ac2uj;i:FTX_=!C1']} |
| Teme N | xJ | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Laut', '5#S;:vaKEtzud8woDejV']} |
| Teme N | YB | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Laut', '5#S;:vaKEtzud8woDejV']} |
| Teme N | xP | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Setup level', '$[8pc%VA7Tj[tpuIl/4I']} |
| Teme N | YR | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Setup level', '$[8pc%VA7Tj[tpuIl/4I']} |
| Teme N | xT | sound_playuntildone | looks_show | {} |
| Teme N | YT | sound_sounds_menu | looks_show | {} |
| Teme N | xV | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Star level', 'YQMYk.pDz=R`cbC{r[(j']} |
| Teme N | xX | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Star level', 'YQMYk.pDz=R`cbC{r[(j']} |
| Teme N | YY | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Star level', 'YQMYk.pDz=R`cbC{r[(j']} |
| Teme N | xZ | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | x! | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | x# | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | x( | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y% | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | h[ | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y* | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y) | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | h] | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y- | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | x* | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y. | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | h^ | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y/ | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | x, | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y: | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Y[ | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Teme N | Y^ | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Teme N | Y_ | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Teme N | Y? | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Teme N | ?: | sound_stopallsounds | sound_stopallsounds | {} |
| Teme N | x= | sound_stopallsounds | control_if_else | {} |
| Teme N | x] | sound_stopallsounds | control_if_else | {} |
| Teme N | x{ | sound_stopallsounds | control_if_else | {} |
| Teme N | ?@ | sound_stopallsounds | sound_stopallsounds | {} |
| Teme N | Zs | sound_cleareffects | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Zu | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Zw | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Zx | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | Zy | sound_seteffectto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Teme N | yh | sound_playuntildone | control_if_else | {} |
| Teme N | ZB | sound_sounds_menu | control_if_else | {} |
| Teme N | yi | sound_playuntildone | control_if_else | {} |
| Teme N | ZC | sound_sounds_menu | control_if_else | {} |
| Teme N | ZH | sound_stopallsounds | sound_stopallsounds | {} |
| Teme N | ZK | sound_seteffectto | control_if_else | {} |
| Teme N | ZM | sound_seteffectto | control_if_else | {} |
| Teme N | yn | sound_stopallsounds | control_forever | {} |
| Teme N | yp | sound_stopallsounds | control_forever | {} |
| Teme N | yr | sound_playuntildone | control_if_else | {} |
| Teme N | ZQ | sound_sounds_menu | control_if_else | {} |
| Teme N | ZU | sound_setvolumeto | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Move all', 'PdbSuI~DZxf!WbXQ47cv']} |
| Dragon | yv | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | yJ | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | Z? | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | yQ | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | Z~ | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | y+ | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | !k | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | y= | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | !u | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | y{ | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | !D | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['start level v1', 'x=6WrdpLC_$NaR\|uJK#c']} |
| Dragon | zv | sound_play | looks_show | {} |
| Dragon | !/ | sound_sounds_menu | looks_show | {} |
| Power up | iD | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Power down', '=`MW3DW.bklbb66O+JLv']} |
| Power up | ![ | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Power down', '=`MW3DW.bklbb66O+JLv']} |
| Power up | iF | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Power Up', '_nI^XD^*2sBJw3pdss4D']} |
| Power up | !~ | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Power Up', '_nI^XD^*2sBJw3pdss4D']} |
| Power up | zF | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Power Up', '_nI^XD^*2sBJw3pdss4D']} |
| Power up | zG | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Power Up', '_nI^XD^*2sBJw3pdss4D']} |
| Power up | #g | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Power Up', '_nI^XD^*2sBJw3pdss4D']} |
| Menu | iH | sound_playuntildone | event_whenflagclicked | {} |
| Menu | #j | sound_sounds_menu | event_whenflagclicked | {} |
| Menu | zR | sound_playuntildone | event_whenflagclicked | {} |
| Menu | #k | sound_sounds_menu | event_whenflagclicked | {} |
| Menu | z# | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Re start', 'jLAo,jd[iw9:z:v)]mHy']} |
| Menu | #n | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Re start', 'jLAo,jd[iw9:z:v)]mHy']} |
| Dynamick | iN | sound_playuntildone | event_whenflagclicked | {} |
| Dynamick | #z | sound_sounds_menu | event_whenflagclicked | {} |
| Dynamick | iZ | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Game over', 'Lv;3B\|_C,v[OF`hw0-%K']} |
| Dynamick | #/ | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Game over', 'Lv;3B\|_C,v[OF`hw0-%K']} |
| Ball | jG | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Ball | )= | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Create', 'q+Ohh`p0lh^(S;YVv})S']} |
| Died/Pipie | jW | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Died/Pipie | *I | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Died/Pipie | jX | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Died/Pipie | *J | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Died', 'T_(c0o2^S!SR,Bw%/zHS']} |
| Died/Pipie | j( | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Travel', '+0F]Ov7fp-w8l8wYd99m']} |
| Died/Pipie | *Z | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Travel', '+0F]Ov7fp-w8l8wYd99m']} |
| Died/Pipie | *[ | sound_stopallsounds | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Travel', '+0F]Ov7fp-w8l8wYd99m']} |
| Died/Pipie | j] | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Game Over Ani', '*RNnX[bLky[vz@LqSP#@']} |
| Died/Pipie | +l | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Game Over Ani', '*RNnX[bLky[vz@LqSP#@']} |
| Died/Pipie | j^ | sound_playuntildone | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Game Over Ani', '*RNnX[bLky[vz@LqSP#@']} |
| Died/Pipie | +n | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Game Over Ani', '*RNnX[bLky[vz@LqSP#@']} |
| Water | En | sound_playuntildone | control_forever | {} |
| Water | +q | sound_sounds_menu | control_forever | {} |
| Final Boss | ky | sound_play | procedures_definition | {} |
| Final Boss | ,O | sound_sounds_menu | procedures_definition | {} |
| Final Boss | kP | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Kill mop by free', 'a1zG/x`of`zCd5/#H,h?']} |
| Final Boss | -f | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['Kill mop by free', 'a1zG/x`of`zCd5/#H,h?']} |
| Figur8 | IX | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['New Day', 'g5=V_A~Sa5J4*?}TfrV^']} |
| Figur8 | /% | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['New Day', 'g5=V_A~Sa5J4*?}TfrV^']} |
| Figur8 | IY | sound_play | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['New Day', 'g5=V_A~Sa5J4*?}TfrV^']} |
| Figur8 | /( | sound_sounds_menu | event_whenbroadcastreceived | {'BROADCAST_OPTION': ['New Day', 'g5=V_A~Sa5J4*?}TfrV^']} |
