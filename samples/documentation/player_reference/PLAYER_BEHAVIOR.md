# Player behavior: native port specification

Static analysis of original Dorfplatformer9 project. Block IDs below refer to player_source.json. All coordinates use positive Y upward.

## Active call sequence

Player Tick (QV) -> Tick (q+) -> Animate Cat (p#) -> Keys (qs) and vertical collision sdfghj.- (rE). All four custom procedures have no arguments and warp=true. Tick probes +3 Y, selects PLAYER TEME from Teme if Y world=0 else teme, calls Animate Cat, then subtracts 3 Y. Player Tick decrements J B by 1 before Tick, then sets scale 25 for N else 50, checks Y < -300, sets ghost 100. When Play=0 resets sx,sy,Scroll x and position (-100,100), hides, stops script.

## Input

Walk[1]=right: axis>0 or (D and not A) or (Right and not Left) or touch[2]. Walk[2]=left: inverse equivalents and touch[1]. Walk[3]=W or Up or pad1 button1 or touch[3]. Walk[4]=OR of right sources minus OR of left sources. If S or Down or touch[4], powerup != N and theme !=4, clear all four. Keys runs AFTER jump counter check and before gravity: jump uses prior Walk state, vertical landing uses current Walk.

## Movement (Animate Cat p#)

If x<-240 set x=-220 and sx=18. Select mask 2, mask 3 if Down/touch[4]/touch Levels, override with mask 4 if theme=4; store costume in _______Cos. If Scroll x>999999999999999 set Day=3/3,E=5,broadcast Win. If jump>0 and FAll<1 increment jump by 0.1 and if jump<1.8 set sy=14/0.77. If prior Walk[3] false reset jump=0. FAll += 1/60; Fall +=1; Keys; show; select mask 5; subtract gravity (1 if backdrop #3, else 1.96); restore mask. Theme 4 clamps sy to >=-4 and sets sy=4 if Walk[3]. Call vertical collision, clear Set = 0. sx/=2.5; if axis==0 add 2*Walk[1]-2*Walk[2], else add 2*axis; sx*=2. fame+=abs(round(sx))/6; if rounded speed zero and theme !=4 reset fame=0. Horizontal loop abs(round(sx)) times, step sign(sx), check Levels, switch to probe costume 5, reverse step, restore saved mask, check collision and either reapply step or set Set = 0=1. After loop if Set = 0==1 set sx=0. Preserve source ordering and empty branches.

## Vertical collision (rE)

Store Y in thread variable variable. Restore saved mask. Repeat round(abs(sy)) times; step sign(sy) if rounded magnitude nonzero. On touching Levels set Set = 0=1, switch mask 5, reverse step if still touching, restore mask. Condition operator_not has missing operand (Ro); under ordinary Scratch empty Boolean semantics it is true. If sy>0 set jump=99,sy=0; else FAll=0,Fall=0,jump=0,sy=0; if current Walk[3] play Jump for N else Jump2, set jump=1. Clear Set = 0 and stop this script. Confirm stop behavior in warp procedure against TurboWarp.

## Camera

Scroll: if Play=1 and Final Boss=0 add sprite x to Scroll x, set x=0, if Scroll x<0 add it back to x and clamp Scroll x=0; export Cam x, My y, my x pos. The x>0 if block has EMPTY body; do not incorrectly gate the following statements. Scroll X imports Cam x into Scroll x, sets x=0, applies same negative clamp and exports coordinates, subtracts 3 Y. Final Boss bypasses horizontal scrolling.

## Events

Create: Win leve=0,Cam x=0,sy=0,fall=0,position(-100,100),show,Scroll x=0,hide. Died: Pause=1, broadcast Still, pitch=90, play Cymbal Echo until done, clear effects, Win leve=0,Cam x=0,sy=0,Powerup=B,fall=0,position(-100,100),show,Y world=0,Scroll x=0,hide,broadcast Lave,broadcast and wait Restart Level,Pause=0,stop sounds. Does not explicitly reset sx or Fall/FAll/jump here. Yump: sy=32 and sound Kill mopps. Up: stop other scripts, if y<-179 hide/delete clone; Set = 0=0. Lave: stop other scripts, hide Time monitor and self. Win: Win leve=1 and stop other scripts. Toi died: go to Died/Pipie.

## Visual body

Scale 25 N,50 otherwise. Color 0 or -54 for F if a____SS=0, else increment 5 each visual loop. Body Y=My y+11.875*(size/25)-1, X=my x pos. Fall>1 selects airborne costume 7, theme4 instead round(fame mod 1)+9 and stops script. Otherwise face Walk[4]*90 when nonzero and walking costume fame mod 5+1. Crouch Down/touch[4] if Teme!=4 and powerup!=N: subtract 7 Y, select dorf bewoner-B2. Preserve fractional costume selection semantics.

## Power and damage

Power Up: N->B->F; F with a____SS=0 sets a____SS=1 and plays sound. Power down: F->B->N->Died; N branch waits 9s then DAMAGE=0. Body flashes 67 times, ghost 100/0 with 0.02s wait each, DAMAGE=1 then 0. Other handlers also write DAMAGE: scheduling must be tested. Color contact blocks #3e0080 and #ffa9a9 exist but are detached top-level stacks with no event hat; do not automatically treat them as active collision rules. Enemy/boss interactions in source appendix.

## Timing and fidelity

Player physics uses per-tick constants, not delta time. Levels loop broadcasts Move all then Player Tick after pause/transition checks, waits 0. Nominal 60 tick interpretation comes from FAll +=1/60, not proof of actual fixed scheduler rate. Do not silently rescale to seconds. Preserve Scratch rounding (negative half ties toward positive infinity), 1-based arrays, costume selection, missing operands, event reentrancy and clone semantics. Detached editor stacks are retained in source but are not automatically executed. No runtime comparison performed.

## Player local variables and saved values


### Player

| Name | Saved value | ID |
|---|---|---|

| sy | 0 | `XtC,Vn=kX&#124;{cm-kif5KE` |

| sx | 4.317490571552463 | `$InH3rv+XW&#124;ay)sd1gz?` |

| fall | 0 | `1U=v4ykDKhyj%t6{2^!y` |

| fame | 43.66666666666667 | `5e4[QC/~6cG:p}0H:}V=` |

| Fall | 0 | `mU4H!oHv.!AjC{51sqem` |

| Set = 0 | 0 | `br3)Gmc4CXRju(*7YC?z` |

| Scroll x | 900 | `!`}bt:W7VJ6^9Y4W1J52` |

| _____JUMP | 0 | `u`=p]EIxx9Xs{{MTE3S0` |

| FAll | 0 | `0Xbn3G3pRX4T_46~ovm4` |

| _______Cos | 2 | `i#?K%5ka.+/Ico#*)#g1` |


Costumes (1-based):

- 1: dorf bewoner, center (43, 185), bitmap resolution 2, asset f525d50b9842c29e177909ceb5f37e44.png

- 2: dorf bewoner2, center (43, 185), bitmap resolution 2, asset f525d50b9842c29e177909ceb5f37e44.png

- 3: dorf bewoner4, center (18, 84), bitmap resolution 2, asset c5dea10c1ba10f5a5d5c68d34a77f78b.png

- 4: dorf bewoner-N5, center (41.58361466066711, 52.305029354196535), bitmap resolution 1, asset b478fbcdf5fde6b74d3e341f7e8a50ee.svg

- 5: dorf bewoner3, center (480, 360), bitmap resolution 2, asset 279daf26b7b43e0d5df0db327dc5ccce.png


### Player out

| Name | Saved value | ID |
|---|---|---|


Costumes (1-based):

- 1: dorf bewoner5, center (480, 360), bitmap resolution 2, asset 13acf40fcbcd9463222708eed62bebf8.png

- 2: dorf bewoner, center (13, 69), bitmap resolution 2, asset 4d16cf037516759e8368f1f79f21611a.png

- 3: dorf bewoner2, center (13, 69), bitmap resolution 2, asset 4d16cf037516759e8368f1f79f21611a.png

- 4: dorf bewoner3, center (13, 69), bitmap resolution 2, asset 920f73551d9a8db54d780e42790a8096.png

- 5: dorf bewoner12, center (13, 69), bitmap resolution 2, asset 920f73551d9a8db54d780e42790a8096.png

- 6: dorf bewoner9, center (13, 69), bitmap resolution 2, asset 920f73551d9a8db54d780e42790a8096.png

- 7: dorf bewoner4, center (13, 69), bitmap resolution 2, asset 4d16cf037516759e8368f1f79f21611a.png

- 8: dorf bewoner7, center (13, 69), bitmap resolution 2, asset 4d16cf037516759e8368f1f79f21611a.png

- 9: dorf bewoner-B2, center (5.188229513618666, 9.990756848248992), bitmap resolution 1, asset 1450d98cc1f1984d2b4a5eaefeae2e39.svg

- 10: dorf bewoner-N3, center (83, 26), bitmap resolution 2, asset 2e81dfbdc8a96fd36fee72e8262302c5.png

- 11: dorf bewoner-N4, center (83, 25), bitmap resolution 2, asset 25a12eb05d378c7d8dd8ba68a0d56c82.png

- 12: dorf bewoner6, center (480, 360), bitmap resolution 2, asset 279daf26b7b43e0d5df0db327dc5ccce.png


### Player body

| Name | Saved value | ID |
|---|---|---|

| sy | 0 | `!*YnBdsJRUKE~2@*@o&#124;x` |

| sx | 0 | `VMCMQ]yhn[sQQL[104g!` |

| fall | 0 | `=!).BJ0^2cgcL7w~~V=N` |

| fame | 0 | `K}aj~P_@u6`@@WiYW/KL` |

| boden | 0 | `4n0g}mYT@]Vf!El;Udm4` |

| Hert | 0 | `VnKx{TK#pmx7O!?s,3ET` |


Costumes (1-based):

- 1: dorf bewoner, center (46, 94), bitmap resolution 2, asset f8d49aec08684d6c6fc0247f26e1c7ae.png

- 2: dorf bewoner2, center (46, 94), bitmap resolution 2, asset 603b2ffe4961a669440875303a334322.png

- 3: dorf bewoner3, center (46, 94), bitmap resolution 2, asset 5f451e4f1bb7f4babfbc06088218621a.png

- 4: dorf bewoner12, center (46, 94), bitmap resolution 2, asset df3747f2ccf63449963819fe2e19559f.png

- 5: dorf bewoner9, center (46, 94), bitmap resolution 2, asset 5f451e4f1bb7f4babfbc06088218621a.png

- 6: dorf bewoner4, center (46, 94), bitmap resolution 2, asset c42e9cd6f0b149aaf7ba1bf30f26f4f8.png

- 7: dorf bewoner7, center (26.5, 47), bitmap resolution 1, asset f8d68c7d92fff786acf021033ff4e115.svg

- 8: dorf bewoner-B2, center (30, 47), bitmap resolution 2, asset 876221d6cae13c49e3df49c0bdfcdf29.png

- 9: dorf bewoner-N2, center (117, 37), bitmap resolution 2, asset 5f442f151aea679207a45db96a736348.png

- 10: dorf bewoner-N3, center (117, 37), bitmap resolution 2, asset 51170a22ba1cd03ba9cf385539f274cf.png

- 11: dorf bewoner5, center (480, 360), bitmap resolution 2, asset 279daf26b7b43e0d5df0db327dc5ccce.png


### Died/Pipie

| Name | Saved value | ID |
|---|---|---|

| ++1 | 0 | `%aO~)$cF4yRJw5LF)flj` |

| ____SY | -28 | `EO?n;^@Q%_XnOV#c05w0` |


Costumes (1-based):

- 1: Dead_N, center (43, 94), bitmap resolution 2, asset f4bff563da6ab5a153f91c35a388b51d.png

- 2: Dead_B, center (73, 160), bitmap resolution 2, asset 1f97b9948fffcd538dce0cca615db728.png

- 3: Dead_F, center (73, 160), bitmap resolution 2, asset 932c07465a1da65454c2dd3eb4c74edc.png

- 4: Pie_N, center (46, 94), bitmap resolution 2, asset 319b9ce07430332d2c96e2c4f502c43c.png

- 5: Pie_B, center (74, 154), bitmap resolution 2, asset bf90aea3559515b81cc54985906f753b.png

- 6: Pie_F, center (74, 154), bitmap resolution 2, asset 6a5b6231c128511c6f74f9bcad1fc4ed.png

- 7: Pie_N2, center (46, 94), bitmap resolution 2, asset acefc327cbffb8aca6398d82a9c7fcd5.png