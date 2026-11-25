# Opcode parameter inventory
All names are exact source keys. Counts and distinct observed values are not API limits. See blocks.json for complete raw records.

## `Gamepad_axisValue` (6 uses)
- inputs.b: 6 distinct source encodings. Examples: `[1,"R#"]`, `[1,"R)"]`, `[1,"R+"]`, `[1,"RV"]`, `[1,"RX"]`, `[1,"RZ"]`
- inputs.i: 6 distinct source encodings. Examples: `[1,"R!"]`, `[1,"R%"]`, `[1,"R*"]`, `[1,"R,"]`, `[1,"RW"]`, `[1,"RY"]`

## `Gamepad_buttonDown` (6 uses)
- inputs.b: 6 distinct source encodings. Examples: `[1,"#s"]`, `[1,"*j"]`, `[1,"*l"]`, `[1,";~"]`, `[1,"R."]`, `[1,"R:"]`
- inputs.i: 6 distinct source encodings. Examples: `[1,"#t"]`, `[1,"*k"]`, `[1,"*m"]`, `[1,"=a"]`, `[1,"R/"]`, `[1,"R;"]`

## `Gamepad_menu_axisMenu` (6 uses)
- fields.axisMenu: 1 distinct source encodings. Examples: `["1",null]`

## `Gamepad_menu_buttonMenu` (6 uses)
- fields.buttonMenu: 5 distinct source encodings. Examples: `["1",null]`, `["3",null]`, `["4",null]`, `["7",null]`, `["any",null]`

## `Gamepad_menu_padMenu` (12 uses)
- fields.padMenu: 2 distinct source encodings. Examples: `["1",null]`, `["any",null]`

## `argument_reporter_boolean` (2 uses)
- fields.VALUE: 1 distinct source encodings. Examples: `["Not",null]`

## `argument_reporter_string_number` (9 uses)
- fields.VALUE: 4 distinct source encodings. Examples: `["_",null]`, `["loop",null]`, `["step",null]`, `["value",null]`

## `control_create_clone_of` (34 uses)
- inputs.CLONE_OPTION: 34 distinct source encodings. Examples: `[1,")|"]`, `[1,"+E"]`, `[1,"+H"]`, `[1,"+J"]`, `[1,"+L"]`, `[1,"+O"]`, `[1,"+Q"]`, `[1,"+T"]`

## `control_create_clone_of_menu` (34 uses)
- fields.CLONE_OPTION: 1 distinct source encodings. Examples: `["_myself_",null]`

## `control_delete_this_clone` (54 uses)

## `control_forever` (48 uses)
- inputs.SUBSTACK: 44 distinct source encodings. Examples: `[2,"(n"]`, `[2,"-s"]`, `[2,".^"]`, `[2,"/o"]`, `[2,"A"]`, `[2,"Aw"]`, `[2,"Bz"]`, `[2,"F+"]`

## `control_if` (349 uses)
- inputs.CONDITION: 334 distinct source encodings. Examples: `[2,"!#"]`, `[2,"#\`"]`, `[2,"#f"]`, `[2,"#h"]`, `[2,"%B"]`, `[2,"%["]`, `[2,"%j"]`, `[2,"(+"]`
- inputs.SUBSTACK: 314 distinct source encodings. Examples: `[1,null]`, `[2,"!%"]`, `[2,"%C"]`, `[2,"(/"]`, `[2,"(="]`, `[2,")J"]`, `[2,")["]`, `[2,")p"]`

## `control_if_else` (133 uses)
- inputs.CONDITION: 128 distinct source encodings. Examples: `[2,"!?"]`, `[2,"!V"]`, `[2,"!]"]`, `[2,"!b"]`, `[2,"!m"]`, `[2,"!w"]`, `[2,"#]"]`, `[2,"#a"]`
- inputs.SUBSTACK: 117 distinct source encodings. Examples: `[1,null]`, `[2,"!^"]`, `[2,"!c"]`, `[2,"!n"]`, `[2,"!x"]`, `[2,"#^"]`, `[2,"#b"]`, `[2,"%;"]`
- inputs.SUBSTACK2: 114 distinct source encodings. Examples: `[1,null]`, `[2,"!_"]`, `[2,"#_"]`, `[2,"#c"]`, `[2,"%="]`, `[2,"%@"]`, `[2,"%c"]`, `[2,"(G"]`

## `control_repeat` (95 uses)
- inputs.SUBSTACK: 93 distinct source encodings. Examples: `[2,"!("]`, `[2,"!N"]`, `[2,"!W"]`, `[2,"!Z"]`, `[2,"%M"]`, `[2,"(x"]`, `[2,"+@"]`, `[2,"+]"]`
- inputs.TIMES: 44 distinct source encodings. Examples: `[1,[6,"1"]]`, `[1,[6,"10"]]`, `[1,[6,"100"]]`, `[1,[6,"111"]]`, `[1,[6,"12"]]`, `[1,[6,"14"]]`, `[1,[6,"15"]]`, `[1,[6,"2"]]`

## `control_repeat_until` (31 uses)
- inputs.CONDITION: 29 distinct source encodings. Examples: `[2,"!+"]`, `[2,"!-"]`, `[2,"!B"]`, `[2,"!d"]`, `[2,"!g"]`, `[2,"!o"]`, `[2,"!y"]`, `[2,".N"]`
- inputs.SUBSTACK: 27 distinct source encodings. Examples: `[2,"!."]`, `[2,"!C"]`, `[2,"!R"]`, `[2,"!e"]`, `[2,"!p"]`, `[2,"!z"]`, `[2,"B~"]`, `[2,"CD"]`

## `control_start_as_clone` (24 uses)

## `control_stop` (124 uses)
- fields.STOP_OPTION: 5 distinct source encodings. Examples: `["all",null]`, `["other scripts in sprite",null]`, `["other scripts in sprite"]`, `["this script",null]`, `["this script"]`
- mutation.children: 1 distinct source encodings. Examples: `[]`
- mutation.hasnext: 2 distinct source encodings. Examples: `"false"`, `"true"`
- mutation.tagName: 1 distinct source encodings. Examples: `"mutation"`

## `control_wait` (92 uses)
- inputs.DURATION: 50 distinct source encodings. Examples: `[1,[5,"0"]]`, `[1,[5,"0.02"]]`, `[1,[5,"0.1"]]`, `[1,[5,"0.2"]]`, `[1,[5,"0.3"]]`, `[1,[5,"0.4"]]`, `[1,[5,"0.5"]]`, `[1,[5,"0.9"]]`

## `control_wait_until` (66 uses)
- inputs.CONDITION: 62 distinct source encodings. Examples: `[2,"(p"]`, `[2,".}"]`, `[2,"/f"]`, `[2,"/g"]`, `[2,"BA"]`, `[2,"F)"]`, `[2,"F@"]`, `[2,"HH"]`

## `data_addtolist` (24 uses)
- fields.LIST: 8 distinct source encodings. Examples: `["Lev","^P|WhdLwGIDlF5Y0OS_?"]`, `["Level 2","DU#p{F94ptV1n9]J/?,R"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`, `["NO?","LL|H@(UR^E*vtUY_EloD"]`, `["SaveCode",";?#F0HF|u6}Ip-tv!3M="]`, `["Walk","U@E@7)+$eTXlzT04Yq=l"]`, `["a_input","q6=S#/S5(yV*Nja,0.%t"]`
- inputs.ITEM: 19 distinct source encodings. Examples: `[1,[10,"0"]]`, `[1,[10,"Code:"]]`, `[1,[10,"Saved"]]`, `[1,[10,"f to close list."]]`, `[1,[10,"thing"]]`, `[3,"UE",[10,"Ding"]]`, `[3,"e=",[10,"Ding"]]`, `[3,"e_",[10,"Ding"]]`

## `data_changevariableby` (94 uses)
- fields.VARIABLE: 45 distinct source encodings. Examples: `["%","9u4F!ZYt-oHC[?Hg?=6v"]`, `["A","7X^zFEWIiYI{k_7XtBoa"]`, `["A","B\`-A?YfgiAs*7b=v-yL)"]`, `["A","I!TA@\`!,7Hvq!4FFhN]Y"]`, `["Boss HP","1Ije0gC3;|A;a::D{6,("]`, `["Cam x","^Nv(;}F!rqY.QLvq|+dF"]`, `["DIED","Sv,AsXKhvtqtn68rJ.%_"]`, `["DIED","]KCngxXf4~-H=fOV1do="]`
- inputs.VALUE: 36 distinct source encodings. Examples: `[1,[4,"-0.01"]]`, `[1,[4,"-0.05"]]`, `[1,[4,"-0.1"]]`, `[1,[4,"-0.3"]]`, `[1,[4,"-1"]]`, `[1,[4,"-1.96"]]`, `[1,[4,"-2"]]`, `[1,[4,"-3"]]`

## `data_deletealloflist` (21 uses)
- fields.LIST: 8 distinct source encodings. Examples: `["Lev","^P|WhdLwGIDlF5Y0OS_?"]`, `["Level 2","DU#p{F94ptV1n9]J/?,R"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`, `["NO?","LL|H@(UR^E*vtUY_EloD"]`, `["SaveCode",";?#F0HF|u6}Ip-tv!3M="]`, `["Walk","U@E@7)+$eTXlzT04Yq=l"]`, `["a_input","q6=S#/S5(yV*Nja,0.%t"]`

## `data_deleteoflist` (8 uses)
- fields.LIST: 4 distinct source encodings. Examples: `["Level 2","DU#p{F94ptV1n9]J/?,R"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`, `["fpsdata","uGu(rz6#G-RbH6}wQO9o"]`
- inputs.INDEX: 2 distinct source encodings. Examples: `[1,[7,"61"]]`, `[3,[12,"MA","w$3zxK~3j/hz[]KKAUX4"],[7,"1"]]`

## `data_hidelist` (5 uses)
- fields.LIST: 1 distinct source encodings. Examples: `["SaveCode",";?#F0HF|u6}Ip-tv!3M="]`

## `data_hidevariable` (17 uses)
- fields.VARIABLE: 7 distinct source encodings. Examples: `["%","9u4F!ZYt-oHC[?Hg?=6v"]`, `["By","hV[y?w/IAyEM}IYOuU?)"]`, `["Day","5Hf_Uw[.2a#Q.9L^+k/g"]`, `["Speed Timer","czK^(#S;kYlsAup)A%Dr"]`, `["Teme","W1cir]R0=MPuyc7~;;8v"]`, `["Time","gY+pT-[?-CO#)ZJOZm}_"]`, `["teme","xApkNM4kMfC(uAg$U.o."]`

## `data_insertatlist` (1 uses)
- fields.LIST: 1 distinct source encodings. Examples: `["fpsdata","uGu(rz6#G-RbH6}wQO9o"]`
- inputs.INDEX: 1 distinct source encodings. Examples: `[1,[7,"1"]]`
- inputs.ITEM: 1 distinct source encodings. Examples: `[3,"=c",[10,"thing"]]`

## `data_itemnumoflist` (11 uses)
- fields.LIST: 3 distinct source encodings. Examples: `["Lev","^P|WhdLwGIDlF5Y0OS_?"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`
- inputs.ITEM: 4 distinct source encodings. Examples: `[1,[10,"19"]]`, `[1,[10,"24"]]`, `[1,[10,"Ding"]]`, `[3,"wC",[10,"thing"]]`

## `data_itemoflist` (115 uses)
- fields.LIST: 7 distinct source encodings. Examples: `["Level 2","DU#p{F94ptV1n9]J/?,R"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`, `["Levels","[cDvjLh\`5=F)Y_|XS~wF"]`, `["Walk","U@E@7)+$eTXlzT04Yq=l"]`, `["a_input","q6=S#/S5(yV*Nja,0.%t"]`, `["fpsdata","uGu(rz6#G-RbH6}wQO9o"]`
- inputs.INDEX: 33 distinct source encodings. Examples: `[1,[7,"1"]]`, `[1,[7,"2"]]`, `[1,[7,"3"]]`, `[1,[7,"4"]]`, `[1,[7,"5"]]`, `[1,[7,"60"]]`, `[3,".)",[7,"1"]]`, `[3,"=h",[7,"1"]]`

## `data_lengthoflist` (22 uses)
- fields.LIST: 4 distinct source encodings. Examples: `["Lev","^P|WhdLwGIDlF5Y0OS_?"]`, `["Level 2","DU#p{F94ptV1n9]J/?,R"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`

## `data_listcontainsitem` (18 uses)
- fields.LIST: 5 distinct source encodings. Examples: `["GGG","~(o%^q0*(evJ(E!gl!Zc"]`, `["Lev","^P|WhdLwGIDlF5Y0OS_?"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`, `["NO?","LL|H@(UR^E*vtUY_EloD"]`
- inputs.ITEM: 16 distinct source encodings. Examples: `[1,[10,"19"]]`, `[1,[10,"thing"]]`, `[3,"H!",[10,"thing"]]`, `[3,"HP",[10,"thing"]]`, `[3,"g@",[10,"thing"]]`, `[3,"g_",[10,"thing"]]`, `[3,"gr",[10,"thing"]]`, `[3,"gs",[10,"thing"]]`

## `data_replaceitemoflist` (20 uses)
- fields.LIST: 6 distinct source encodings. Examples: `["Level 2","DU#p{F94ptV1n9]J/?,R"]`, `["Level 3","MvC0Dpf@pqZu)e_Z}l,P"]`, `["Level","Vuf,!I%vy0}X6@x]j~t="]`, `["Levels","[cDvjLh\`5=F)Y_|XS~wF"]`, `["Walk","U@E@7)+$eTXlzT04Yq=l"]`, `["a_input","q6=S#/S5(yV*Nja,0.%t"]`
- inputs.INDEX: 15 distinct source encodings. Examples: `[1,[7,"1"]]`, `[1,[7,"2"]]`, `[1,[7,"3"]]`, `[1,[7,"4"]]`, `[3,".+",[7,"1"]]`, `[3,";%",[7,"1"]]`, `[3,";-",[7,"1"]]`, `[3,";=",[7,"1"]]`
- inputs.ITEM: 13 distinct source encodings. Examples: `[1,[10,"0"]]`, `[1,[10,"98765432"]]`, `[1,[10,"9999"]]`, `[1,[10,"false"]]`, `[1,[10,"true"]]`, `[3,"Xc",[10,"thing"]]`, `[3,"l\`",[10,"thing"]]`, `[3,"l}",[10,"thing"]]`

## `data_setvariableto` (417 uses)
- fields.VARIABLE: 98 distinct source encodings. Examples: `["%","9u4F!ZYt-oHC[?Hg?=6v"]`, `["++1","%aO~)$cF4yRJw5LF)flj"]`, `["A","7X^zFEWIiYI{k_7XtBoa"]`, `["A","B\`-A?YfgiAs*7b=v-yL)"]`, `["A","I!TA@\`!,7Hvq!4FFhN]Y"]`, `["AA","!D{;8\`Y*XR.$zsK]vo4u"]`, `["AA","%)Y-0g.^}{5=ck_:SdKp"]`, `["Boss HP","1Ije0gC3;|A;a::D{6,("]`
- inputs.VALUE: 131 distinct source encodings. Examples: `[1,[10,""]]`, `[1,[10,"-1"]]`, `[1,[10,"-4"]]`, `[1,[10,"0"]]`, `[1,[10,"0.4"]]`, `[1,[10,"0.5"]]`, `[1,[10,"0/3"]]`, `[1,[10,"1"]]`

## `data_showlist` (2 uses)
- fields.LIST: 1 distinct source encodings. Examples: `["SaveCode",";?#F0HF|u6}Ip-tv!3M="]`

## `data_showvariable` (11 uses)
- fields.VARIABLE: 6 distinct source encodings. Examples: `["By","hV[y?w/IAyEM}IYOuU?)"]`, `["Make","CU;6/LV:h\`a|q\`I#_4=F"]`, `["Speed Timer","czK^(#S;kYlsAup)A%Dr"]`, `["Teme","W1cir]R0=MPuyc7~;;8v"]`, `["Time","gY+pT-[?-CO#)ZJOZm}_"]`, `["teme","xApkNM4kMfC(uAg$U.o."]`

## `dtbyxeroname_dt` (59 uses)

## `dtbyxeroname_fps` (2 uses)

## `event_broadcast` (105 uses)
- inputs.BROADCAST_INPUT: 41 distinct source encodings. Examples: `[1,[11,"Back","z7)y$A/bOxlo?RHWkDN$"]]`, `[1,[11,"Create","q+Ohh\`p0lh^(S;YVv})S"]]`, `[1,[11,"Delete all mopss","O}*zvY]g]w:]0BsbSyS:"]]`, `[1,[11,"Died","T_(c0o2^S!SR,Bw%/zHS"]]`, `[1,[11,"END Time","Dz5y^ZLPna0U2e*w.+]8"]]`, `[1,[11,"GTC","DSm^FM@cZzGhd88bcA(!"]]`, `[1,[11,"Game Over Ani","*RNnX[bLky[vz@LqSP#@"]]`, `[1,[11,"Game over World","CJ.^[g3$DtWms=?[wfB,"]]`

## `event_broadcastandwait` (36 uses)
- inputs.BROADCAST_INPUT: 24 distinct source encodings. Examples: `[1,[11,"Died","T_(c0o2^S!SR,Bw%/zHS"]]`, `[1,[11,"In","L_ug7}Y9L,gLRm^v-lII"]]`, `[1,[11,"Intro Ani","js3)W6?n1E^,[Ud]mKSw"]]`, `[1,[11,"Kill mop by free","a1zG/x\`of\`zCd5/#H,h?"]]`, `[1,[11,"Load",".tSUlQ{j@z{0V,,IHCjS"]]`, `[1,[11,"Move all","PdbSuI~DZxf!WbXQ47cv"]]`, `[1,[11,"New Day","g5=V_A~Sa5J4*?}TfrV^"]]`, `[1,[11,"New game","v{D)qvxjW7{^}HDPxXg6"]]`

## `event_whenbroadcastreceived` (182 uses)
- fields.BROADCAST_OPTION: 58 distinct source encodings. Examples: `["Back to map","*lDoWGWc17eNJK-ecp@l"]`, `["Back","z7)y$A/bOxlo?RHWkDN$"]`, `["Create","q+Ohh\`p0lh^(S;YVv})S"]`, `["Delete all mopss","O}*zvY]g]w:]0BsbSyS:"]`, `["Died","T_(c0o2^S!SR,Bw%/zHS"]`, `["END Time","Dz5y^ZLPna0U2e*w.+]8"]`, `["GTC","DSm^FM@cZzGhd88bcA(!"]`, `["Game Over Ani","*RNnX[bLky[vz@LqSP#@"]`

## `event_whenflagclicked` (48 uses)

## `event_whengreaterthan` (1 uses)
- fields.WHENGREATERTHANMENU: 1 distinct source encodings. Examples: `["TIMER",null]`
- inputs.VALUE: 1 distinct source encodings. Examples: `[3,[12,"AA","!D{;8\`Y*XR.$zsK]vo4u"],[4,"10"]]`

## `event_whenkeypressed` (3 uses)
- fields.KEY_OPTION: 3 distinct source encodings. Examples: `["a",null]`, `["d",null]`, `["x",null]`

## `event_whenthisspriteclicked` (3 uses)

## `lmsTempVars2_changeThreadVariable` (3 uses)
- inputs.NUM: 3 distinct source encodings. Examples: `[1,[4,"-1"]]`, `[1,[4,"1"]]`, `[3,"ml",[4,"1"]]`
- inputs.VAR: 3 distinct source encodings. Examples: `[1,[10,"1"]]`, `[1,[10,"2"]]`, `[1,[10,"variable"]]`

## `lmsTempVars2_getThreadVariable` (12 uses)
- inputs.VAR: 4 distinct source encodings. Examples: `[1,[10,"1"]]`, `[1,[10,"2"]]`, `[1,[10,"variable"]]`, `[1,[10,"variable2"]]`

## `lmsTempVars2_setThreadVariable` (9 uses)
- inputs.STRING: 7 distinct source encodings. Examples: `[1,[10,"0"]]`, `[1,[10,"1"]]`, `[1,[10,"60"]]`, `[3,"=i",[10,"1"]]`, `[3,"Rb",[10,"0"]]`, `[3,[12,"Y world","Z0Z)oSZie{_2[nI(DBpB"],[10,"0"]]`, `[3,[12,"teme","xApkNM4kMfC(uAg$U.o."],[10,"0"]]`
- inputs.VAR: 4 distinct source encodings. Examples: `[1,[10,"1"]]`, `[1,[10,"2"]]`, `[1,[10,"variable"]]`, `[1,[10,"variable2"]]`

## `looks_backdropnumbername` (1 uses)
- fields.NUMBER_NAME: 1 distinct source encodings. Examples: `["number",null]`

## `looks_backdrops` (7 uses)
- fields.BACKDROP: 2 distinct source encodings. Examples: `["Blue Sky 2 4",null]`, `["Bühnenbild4",null]`

## `looks_changeeffectby` (29 uses)
- fields.EFFECT: 3 distinct source encodings. Examples: `["BRIGHTNESS",null]`, `["COLOR",null]`, `["GHOST",null]`
- inputs.CHANGE: 13 distinct source encodings. Examples: `[1,[4,"-1"]]`, `[1,[4,"-10"]]`, `[1,[4,"-5"]]`, `[1,[4,"0.5"]]`, `[1,[4,"1"]]`, `[1,[4,"10"]]`, `[1,[4,"5"]]`, `[1,[4,"50"]]`

## `looks_changesizeby` (12 uses)
- inputs.CHANGE: 9 distinct source encodings. Examples: `[1,[4,"-10"]]`, `[1,[4,"-2.2"]]`, `[1,[4,"10"]]`, `[1,[4,"2.2"]]`, `[3,"#K",[4,"-260"]]`, `[3,"J:",[4,"10"]]`, `[3,"Ke",[4,"10"]]`, `[3,"Sd",[4,"10"]]`

## `looks_cleargraphiceffects` (26 uses)

## `looks_costume` (112 uses)
- fields.COSTUME: 38 distinct source encodings. Examples: `["1-1",null]`, `["1_1",null]`, `["1_10",null]`, `["1_14",null]`, `["1_15",null]`, `["1_16",null]`, `["1_17",null]`, `["<0>",null]`

## `looks_costumenumbername` (39 uses)
- fields.NUMBER_NAME: 1 distinct source encodings. Examples: `["number",null]`

## `looks_goforwardbackwardlayers` (11 uses)
- fields.FORWARD_BACKWARD: 2 distinct source encodings. Examples: `["backward",null]`, `["forward",null]`
- inputs.NUM: 8 distinct source encodings. Examples: `[1,[7,"1"]]`, `[1,[7,"2"]]`, `[1,[7,"3"]]`, `[1,[7,"9"]]`, `[3,"#!",[7,"1"]]`, `[3,"#A",[7,"1"]]`, `[3,":y",[7,"1"]]`, `[3,";S",[7,"1"]]`

## `looks_gotofrontback` (44 uses)
- fields.FRONT_BACK: 2 distinct source encodings. Examples: `["back",null]`, `["front",null]`

## `looks_hide` (121 uses)

## `looks_nextcostume` (11 uses)

## `looks_say` (5 uses)
- inputs.MESSAGE: 3 distinct source encodings. Examples: `[1,[10,""]]`, `[3,[12,"Teme","W1cir]R0=MPuyc7~;;8v"],[10,"Hallo!"]]`, `[3,[12,"teme","xApkNM4kMfC(uAg$U.o."],[10,"Hallo!"]]`

## `looks_seteffectto` (63 uses)
- fields.EFFECT: 4 distinct source encodings. Examples: `["BRIGHTNESS",null]`, `["COLOR",null]`, `["GHOST",null]`, `["GHOST"]`
- inputs.VALUE: 9 distinct source encodings. Examples: `[1,[4,"-100"]]`, `[1,[4,"-20"]]`, `[1,[4,"-50"]]`, `[1,[4,"0"]]`, `[1,[4,"100"]]`, `[1,[4,"50"]]`, `[3,"%]",[4,"0"]]`, `[3,":+",[4,"0"]]`

## `looks_setsizeto` (48 uses)
- inputs.SIZE: 10 distinct source encodings. Examples: `[1,[4,"100"]]`, `[1,[4,"200"]]`, `[1,[4,"25"]]`, `[1,[4,"250"]]`, `[1,[4,"300"]]`, `[1,[4,"40"]]`, `[1,[4,"5"]]`, `[1,[4,"50"]]`

## `looks_show` (66 uses)

## `looks_size` (5 uses)

## `looks_switchbackdropto` (7 uses)
- inputs.BACKDROP: 7 distinct source encodings. Examples: `[1,"Xr"]`, `[3,[12,"Teme","W1cir]R0=MPuyc7~;;8v"],"Sr"]`, `[3,[12,"Teme","W1cir]R0=MPuyc7~;;8v"],"Su"]`, `[3,[12,"Teme","W1cir]R0=MPuyc7~;;8v"],"Tu"]`, `[3,[12,"Teme","W1cir]R0=MPuyc7~;;8v"],"Y!"]`, `[3,[12,"Teme","W1cir]R0=MPuyc7~;;8v"],"YX"]`, `[3,[12,"teme","xApkNM4kMfC(uAg$U.o."],"Tv"]`

## `looks_switchcostumeto` (112 uses)
- inputs.COSTUME: 112 distinct source encodings. Examples: `[1,"!P"]`, `[1,"#+"]`, `[1,"#@"]`, `[1,"#D"]`, `[1,"#H"]`, `[1,"#I"]`, `[1,"#J"]`, `[1,"#L"]`

## `looks_think` (7 uses)
- inputs.MESSAGE: 7 distinct source encodings. Examples: `[1,[10,""]]`, `[1,[10,"Hmm..."]]`, `[3,"VE",[10,"Hmm..."]]`, `[3,"g=",[10,"Hmm..."]]`, `[3,"g]",[10,"Hmm..."]]`, `[3,"g\`",[10,"Hmm..."]]`, `[3,"jU",[10,"Hmm..."]]`

## `motion_changexby` (25 uses)
- inputs.DX: 19 distinct source encodings. Examples: `[1,[4,"-1"]]`, `[1,[4,"1"]]`, `[1,[4,"10"]]`, `[1,[4,"20"]]`, `[3,")B",[4,"-3"]]`, `[3,",V",[4,"-3"]]`, `[3,":f",[4,"10"]]`, `[3,"NG",[4,"-3"]]`

## `motion_changeyby` (54 uses)
- inputs.DY: 25 distinct source encodings. Examples: `[1,[4,"-1"]]`, `[1,[4,"-10"]]`, `[1,[4,"-12"]]`, `[1,[4,"-20"]]`, `[1,[4,"-3"]]`, `[1,[4,"-5"]]`, `[1,[4,"-6"]]`, `[1,[4,"-7"]]`

## `motion_glidesecstoxy` (5 uses)
- inputs.SECS: 1 distinct source encodings. Examples: `[1,[4,"1"]]`
- inputs.X: 3 distinct source encodings. Examples: `[1,[4,"200"]]`, `[1,[4,"203"]]`, `[1,[4,"45"]]`
- inputs.Y: 4 distinct source encodings. Examples: `[1,[4,"41"]]`, `[3,"#}",[4,"0"]]`, `[3,"%{",[4,"0"]]`, `[3,"(^",[4,"0"]]`

## `motion_goto` (13 uses)
- inputs.TO: 13 distinct source encodings. Examples: `[1,"!H"]`, `[1,"!Q"]`, `[1,"%!"]`, `[1,"%E"]`, `[1,"(f"]`, `[1,"(~"]`, `[1,")?"]`, `[1,"*F"]`

## `motion_goto_menu` (13 uses)
- fields.TO: 7 distinct source encodings. Examples: `["Died/Pipie",null]`, `["Final Boss",null]`, `["Player body",null]`, `["Player out",null]`, `["Player",null]`, `["Player"]`, `["_random_",null]`

## `motion_gotoxy` (51 uses)
- inputs.X: 19 distinct source encodings. Examples: `[1,[4,"-100"]]`, `[1,[4,"0"]]`, `[1,[4,"178"]]`, `[1,[4,"95"]]`, `[1,[4,"999"]]`, `[3,"Ct",[4,"0"]]`, `[3,"De",[4,"0"]]`, `[3,"E[",[4,"0"]]`
- inputs.Y: 23 distinct source encodings. Examples: `[1,[4,"-200"]]`, `[1,[4,"-400"]]`, `[1,[4,"-50"]]`, `[1,[4,"-52"]]`, `[1,[4,"-98"]]`, `[1,[4,"0"]]`, `[1,[4,"100"]]`, `[1,[4,"41"]]`

## `motion_movesteps` (1 uses)
- inputs.STEPS: 1 distinct source encodings. Examples: `[3,[12,"sx","Z1)gtQ=pYs/rbLRU\`.g-"],[4,"10"]]`

## `motion_pointindirection` (11 uses)
- inputs.DIRECTION: 7 distinct source encodings. Examples: `[1,[8,"-90"]]`, `[3,"A*",[8,"90"]]`, `[3,"BY",[8,"90"]]`, `[3,"C,",[8,"90"]]`, `[3,"CY",[8,"90"]]`, `[3,"Sc",[8,"90"]]`, `[3,"YI",[8,"90"]]`

## `motion_setrotationstyle` (5 uses)
- fields.STYLE: 2 distinct source encodings. Examples: `["all around",null]`, `["left-right",null]`

## `motion_setx` (8 uses)
- inputs.X: 6 distinct source encodings. Examples: `[1,[4,"-220"]]`, `[1,[4,"0"]]`, `[3,")L",[4,"0"]]`, `[3,")P",[4,"0"]]`, `[3,"D)",[4,"-100"]]`, `[3,[12,"PIX","bs5l}ds{5k*ZNdx%XYf)"],[4,"0"]]`

## `motion_sety` (8 uses)
- inputs.Y: 6 distinct source encodings. Examples: `[1,[4,"0"]]`, `[1,[4,"250"]]`, `[3,"E-",[4,"-81"]]`, `[3,"Rz",[4,"-56"]]`, `[3,"nY",[4,"-81"]]`, `[3,"pM",[4,"-81"]]`

## `motion_xposition` (40 uses)

## `motion_yposition` (37 uses)

## `obviousalexsensing_getFingersTouching` (1 uses)

## `obviousalexsensing_touchingFinger` (4 uses)

## `operator_add` (61 uses)
- inputs.NUM1: 51 distinct source encodings. Examples: `[1,[4,""]]`, `[1,[4,"100"]]`, `[3,"%f",[4,""]]`, `[3,"*x",[4,""]]`, `[3,",a",[4,""]]`, `[3,"-t",[4,""]]`, `[3,"..",[4,""]]`, `[3,"./",[4,""]]`
- inputs.NUM2: 26 distinct source encodings. Examples: `[1,[4,""]]`, `[1,[4,"-9"]]`, `[1,[4,"0"]]`, `[1,[4,"0.7"]]`, `[1,[4,"1"]]`, `[1,[4,"25"]]`, `[1,[4,"250"]]`, `[1,[4,"36"]]`

## `operator_and` (22 uses)
- inputs.OPERAND1: 19 distinct source encodings. Examples: `[2,"Fl"]`, `[2,"K{"]`, `[2,"K|"]`, `[2,"La"]`, `[2,"Lb"]`, `[2,"Ps"]`, `[2,"XW"]`, `[2,"fn"]`
- inputs.OPERAND2: 20 distinct source encodings. Examples: `[2,";!"]`, `[2,";)"]`, `[2,";/"]`, `[2,";["]`, `[2,"BB"]`, `[2,"Fm"]`, `[2,"Pt"]`, `[2,"XX"]`

## `operator_divide` (60 uses)
- inputs.NUM1: 38 distinct source encodings. Examples: `[1,[4,""]]`, `[1,[4,"-300"]]`, `[1,[4,"-72"]]`, `[1,[4,"1"]]`, `[1,[4,"1.6"]]`, `[1,[4,"100"]]`, `[1,[4,"14"]]`, `[1,[4,"180"]]`
- inputs.NUM2: 25 distinct source encodings. Examples: `[1,[4,""]]`, `[1,[4,"0"]]`, `[1,[4,"0.77"]]`, `[1,[4,"10"]]`, `[1,[4,"100"]]`, `[1,[4,"1100"]]`, `[1,[4,"2"]]`, `[1,[4,"2.38"]]`

## `operator_equals` (317 uses)
- inputs.OPERAND1: 135 distinct source encodings. Examples: `[1,[10,""]]`, `[1,[10,"0"]]`, `[1,[10,"3"]]`, `[1,[10,"Make"]]`, `[1,[10,"Play&Make"]]`, `[3,"%U",[10,""]]`, `[3,"%W",[10,""]]`, `[3,"%r",[10,""]]`
- inputs.OPERAND2: 59 distinct source encodings. Examples: `[1,[10,""]]`, `[1,[10,"-1"]]`, `[1,[10,"0"]]`, `[1,[10,"0/3"]]`, `[1,[10,"1"]]`, `[1,[10,"1/3"]]`, `[1,[10,"100"]]`, `[1,[10,"1100"]]`

## `operator_gt` (134 uses)
- inputs.OPERAND1: 85 distinct source encodings. Examples: `[1,[10,"0"]]`, `[3,"!i",[10,""]]`, `[3,"*;",[10,""]]`, `[3,"*O",[10,""]]`, `[3,"/#",[10,""]]`, `[3,";n",[10,""]]`, `[3,";v",[10,""]]`, `[3,";{",[10,""]]`
- inputs.OPERAND2: 22 distinct source encodings. Examples: `[1,[10,"0"]]`, `[1,[10,"0.49"]]`, `[1,[10,"1"]]`, `[1,[10,"100"]]`, `[1,[10,"170"]]`, `[1,[10,"179"]]`, `[1,[10,"180"]]`, `[1,[10,"2"]]`

## `operator_join` (91 uses)
- inputs.STRING1: 40 distinct source encodings. Examples: `[1,[10,","]]`, `[1,[10,"1"]]`, `[1,[10,"<"]]`, `[1,[10,"Dead_"]]`, `[1,[10,"Pie"]]`, `[1,[10,"Pie_"]]`, `[1,[10,"f"]]`, `[1,[10,"l"]]`
- inputs.STRING2: 74 distinct source encodings. Examples: `[1,[10,","]]`, `[1,[10,"-"]]`, `[1,[10,"."]]`, `[1,[10,"/3"]]`, `[1,[10,"1"]]`, `[1,[10,"3"]]`, `[1,[10,">"]]`, `[1,[10,"[/code]"]]`

## `operator_length` (2 uses)
- inputs.STRING: 2 distinct source encodings. Examples: `[1,[10,"Apfel"]]`, `[3,"K%",[10,"Apfel"]]`

## `operator_letter_of` (23 uses)
- inputs.LETTER: 4 distinct source encodings. Examples: `[1,[6,"1"]]`, `[3,";p",[6,"1"]]`, `[3,";s",[6,"1"]]`, `[3,[12,"SaveLetter","JfNjL4\`+$/V_K.nh/E5:"],[6,"1"]]`
- inputs.STRING: 4 distinct source encodings. Examples: `[3,";t",[10,"Apfel"]]`, `[3,"KZ",[10,"Apfel"]]`, `[3,[12,"Day","5Hf_Uw[.2a#Q.9L^+k/g"],[10,"Apfel"]]`, `[3,[12,"SaveCode",".8+;N/djT7JbP9V|AjZ8"],[10,"apple"]]`

## `operator_lt` (55 uses)
- inputs.OPERAND1: 36 distinct source encodings. Examples: `[1,[10,"0"]]`, `[1,[10,"180"]]`, `[1,[10,"200"]]`, `[3,"!S",[10,""]]`, `[3,"!s",[10,""]]`, `[3,")u",[10,""]]`, `[3,"*L",[10,""]]`, `[3,",z",[10,""]]`
- inputs.OPERAND2: 27 distinct source encodings. Examples: `[1,[10,"-145"]]`, `[1,[10,"-179"]]`, `[1,[10,"-210"]]`, `[1,[10,"-240"]]`, `[1,[10,"-250"]]`, `[1,[10,"-300"]]`, `[1,[10,"-4"]]`, `[1,[10,"0"]]`

## `operator_mathop` (34 uses)
- fields.OPERATOR: 3 distinct source encodings. Examples: `["abs",null]`, `["ceiling",null]`, `["floor",null]`
- inputs.NUM: 33 distinct source encodings. Examples: `[3,")G",[4,""]]`, `[3,")K",[4,""]]`, `[3,")g",[4,""]]`, `[3,",.",[4,""]]`, `[3,",/",[4,""]]`, `[3,",I",[4,""]]`, `[3,",K",[4,""]]`, `[3,":m",[4,"3"]]`

## `operator_mod` (9 uses)
- inputs.NUM1: 8 distinct source encodings. Examples: `[1,[4,"3"]]`, `[3,"A.",[4,""]]`, `[3,"AY",[4,""]]`, `[3,"B#",[4,""]]`, `[3,"BQ",[4,""]]`, `[3,"GG",[4,""]]`, `[3,"VR",[4,""]]`, `[3,[12,"E","e5kHgnw0TxH%jU21otKV"],[4,"3"]]`
- inputs.NUM2: 5 distinct source encodings. Examples: `[1,[4,"1"]]`, `[1,[4,"2"]]`, `[1,[4,"3"]]`, `[1,[4,"5"]]`, `[3,"VS",[4,""]]`

## `operator_multiply` (82 uses)
- inputs.NUM1: 49 distinct source encodings. Examples: `[1,[4,""]]`, `[1,[4,"-27"]]`, `[1,[4,"3"]]`, `[1,[4,"60"]]`, `[1,[4,"720"]]`, `[1,[4,"95"]]`, `[3,"%s",[4,""]]`, `[3,"(V",[4,""]]`
- inputs.NUM2: 24 distinct source encodings. Examples: `[1,[4,"-1"]]`, `[1,[4,"-14"]]`, `[1,[4,"-2"]]`, `[1,[4,"0.08"]]`, `[1,[4,"0.25"]]`, `[1,[4,"100"]]`, `[1,[4,"1100"]]`, `[1,[4,"12"]]`

## `operator_not` (83 uses)
- inputs.OPERAND: 69 distinct source encodings. Examples: `[2,"!}"]`, `[2,"%p"]`, `[2,"%q"]`, `[2,"%v"]`, `[2,"%w"]`, `[2,"(!"]`, `[2,"(#"]`, `[2,"(T"]`

## `operator_or` (63 uses)
- inputs.OPERAND1: 60 distinct source encodings. Examples: `[2,"!h"]`, `[2,"!r"]`, `[2,"#p"]`, `[2,"+u"]`, `[2,",%"]`, `[2,",-"]`, `[2,";("]`, `[2,";."]`
- inputs.OPERAND2: 59 distinct source encodings. Examples: `[2,":Q"]`, `[2,";\`"]`, `[2,"A{"]`, `[2,"A}"]`, `[2,"B("]`, `[2,"BT"]`, `[2,"C%"]`, `[2,"CF"]`

## `operator_random` (5 uses)
- inputs.FROM: 3 distinct source encodings. Examples: `[1,[4,"-180"]]`, `[1,[4,"0"]]`, `[1,[4,"1"]]`
- inputs.TO: 3 distinct source encodings. Examples: `[1,[4,"180"]]`, `[1,[4,"6"]]`, `[1,[4,"90"]]`

## `operator_round` (26 uses)
- inputs.NUM: 24 distinct source encodings. Examples: `[1,[4,""]]`, `[3,")A",[4,""]]`, `[3,")x",[4,""]]`, `[3,",C",[4,""]]`, `[3,",E",[4,""]]`, `[3,"AX",[4,""]]`, `[3,"BP",[4,""]]`, `[3,"GH",[4,""]]`

## `operator_subtract` (139 uses)
- inputs.NUM1: 59 distinct source encodings. Examples: `[1,[4,""]]`, `[1,[4,"0.051300000000745055"]]`, `[1,[4,"143"]]`, `[1,[4,"1500"]]`, `[1,[4,"4829"]]`, `[3,")h",[4,""]]`, `[3,"*=",[4,""]]`, `[3,"+|",[4,""]]`
- inputs.NUM2: 86 distinct source encodings. Examples: `[1,[4,""]]`, `[1,[4,"-2"]]`, `[1,[4,"1"]]`, `[1,[4,"100"]]`, `[1,[4,"1100"]]`, `[1,[4,"2"]]`, `[1,[4,"36"]]`, `[1,[4,"6"]]`

## `pen_clear` (6 uses)

## `pen_stamp` (21 uses)

## `procedures_call` (77 uses)
- inputs.9sw+4LWE5jp%Z.^]|[4^: 2 distinct source encodings. Examples: `[1,[10,""]]`, `[1,[10,"|"]]`
- inputs.D`D:996P!3mqgp)88Y+c: 8 distinct source encodings. Examples: `[1,[10,"[/code]"]]`, `[1,[10,"[code]"]]`, `[3,"IC",[10,""]]`, `[3,"ID",[10,""]]`, `[3,"IE",[10,""]]`, `[3,"IG",[10,""]]`, `[3,[12,"Powerup","LuQ$oN$PbE}cUYjvU=:-"],[10,""]]`, `[3,[12,"___LEVELs","!hva2{7?mUfLvWM{Hc~a"],[10,""]]`
- inputs.Y_R/DI|R|:0qTWWecM?T: 2 distinct source encodings. Examples: `[1,[10,"-1"]]`, `[1,[10,"1"]]`
- inputs._:Pd^.hCup!+K+@BoWdN: 1 distinct source encodings. Examples: `[1,[10,"2"]]`
- inputs.zIGX00EvkcP+EqCJrv9(: 3 distinct source encodings. Examples: `[2,"*)"]`, `[2,"*,"]`, `[2,"*^"]`
- mutation.argumentids: 3 distinct source encodings. Examples: `"[\"D\`D:996P!3mqgp)88Y+c\",\"9sw+4LWE5jp%Z.^]|[4^\"]"`, `"[\"Y_R/DI|R|:0qTWWecM?T\",\"_:Pd^.hCup!+K+@BoWdN\",\"zIGX00EvkcP+EqCJrv9(\"]"`, `"[]"`
- mutation.children: 1 distinct source encodings. Examples: `[]`
- mutation.proccode: 23 distinct source encodings. Examples: `"Animate Cat"`, `"Clone"`, `"Day1"`, `"Down"`, `"ExtractNextPiece"`, `"Game"`, `"Keys"`, `"Load Save Code"`
- mutation.tagName: 1 distinct source encodings. Examples: `"mutation"`
- mutation.warp: 2 distinct source encodings. Examples: `"false"`, `"true"`

## `procedures_definition` (45 uses)
- inputs.custom_block: 45 distinct source encodings. Examples: `[1,"!O"]`, `[1,"#~"]`, `[1,"(C"]`, `[1,"(b"]`, `[1,")Q"]`, `[1,")e"]`, `[1,")t"]`, `[1,"*v"]`

## `procedures_prototype` (45 uses)
- inputs.9sw+4LWE5jp%Z.^]|[4^: 1 distinct source encodings. Examples: `[1,"/R"]`
- inputs.D`D:996P!3mqgp)88Y+c: 1 distinct source encodings. Examples: `[1,"/Q"]`
- inputs.Y_R/DI|R|:0qTWWecM?T: 1 distinct source encodings. Examples: `[1,"*~"]`
- inputs._:Pd^.hCup!+K+@BoWdN: 1 distinct source encodings. Examples: `[1,"+a"]`
- inputs.zIGX00EvkcP+EqCJrv9(: 1 distinct source encodings. Examples: `[1,"+b"]`
- mutation.argumentdefaults: 3 distinct source encodings. Examples: `"[\"\",\"\",\"false\"]"`, `"[\"\",\"\"]"`, `"[]"`
- mutation.argumentids: 3 distinct source encodings. Examples: `"[\"D\`D:996P!3mqgp)88Y+c\",\"9sw+4LWE5jp%Z.^]|[4^\"]"`, `"[\"Y_R/DI|R|:0qTWWecM?T\",\"_:Pd^.hCup!+K+@BoWdN\",\"zIGX00EvkcP+EqCJrv9(\"]"`, `"[]"`
- mutation.argumentnames: 3 distinct source encodings. Examples: `"[\"step\",\"loop\",\"Not\"]"`, `"[\"value\",\"_\"]"`, `"[]"`
- mutation.children: 1 distinct source encodings. Examples: `[]`
- mutation.proccode: 23 distinct source encodings. Examples: `"Animate Cat"`, `"Clone"`, `"Day1"`, `"Down"`, `"ExtractNextPiece"`, `"Game"`, `"Keys"`, `"Load Save Code"`
- mutation.tagName: 1 distinct source encodings. Examples: `"mutation"`
- mutation.warp: 2 distinct source encodings. Examples: `"false"`, `"true"`

## `runtimeoptions_getFramerate` (1 uses)

## `runtimeoptions_greenFlag` (2 uses)

## `runtimeoptions_setDimensions` (8 uses)
- inputs.height: 5 distinct source encodings. Examples: `[1,[4,"1080"]]`, `[1,[4,"1200"]]`, `[1,[4,"200"]]`, `[1,[4,"360"]]`, `[1,[4,"800"]]`
- inputs.width: 4 distinct source encodings. Examples: `[1,[4,"1920"]]`, `[1,[4,"360"]]`, `[1,[4,"480"]]`, `[1,[4,"800"]]`

## `runtimeoptions_setFramerate` (1 uses)
- inputs.fps: 1 distinct source encodings. Examples: `[1,[4,"60"]]`

## `sensing_answer` (2 uses)

## `sensing_askandwait` (1 uses)
- inputs.QUESTION: 1 distinct source encodings. Examples: `[1,[10,"Give code!"]]`

## `sensing_keyoptions` (51 uses)
- fields.KEY_OPTION: 18 distinct source encodings. Examples: `["a",null]`, `["any",null]`, `["d",null]`, `["down arrow",null]`, `["down arrow"]`, `["e",null]`, `["f",null]`, `["h",null]`

## `sensing_keypressed` (51 uses)
- inputs.KEY_OPTION: 51 distinct source encodings. Examples: `[1,"%T"]`, `[1,"%V"]`, `[1,"(R"]`, `[1,"(Y"]`, `[1,"*i"]`, `[1,"-D"]`, `[1,"-F"]`, `[1,"-H"]`

## `sensing_mousedown` (13 uses)

## `sensing_mousex` (2 uses)

## `sensing_mousey` (2 uses)

## `sensing_of` (25 uses)
- fields.PROPERTY: 7 distinct source encodings. Examples: `["Fall",null]`, `["costume #",null]`, `["direction",null]`, `["fame",null]`, `["sy",null]`, `["x position",null]`, `["y position",null]`
- inputs.OBJECT: 25 distinct source encodings. Examples: `[1,"!Y"]`, `[1,"%i"]`, `[1,"%l"]`, `[1,"%u"]`, `[1,"(K"]`, `[1,"(O"]`, `[1,"(X"]`, `[1,")*"]`

## `sensing_of_object_menu` (25 uses)
- fields.OBJECT: 4 distinct source encodings. Examples: `["Phone Controls",null]`, `["Player body",null]`, `["Player on map",null]`, `["Player",null]`

## `sensing_resettimer` (2 uses)

## `sensing_timer` (7 uses)

## `sensing_touchingcolor` (2 uses)
- inputs.COLOR: 2 distinct source encodings. Examples: `[1,[9,"#3e0080"]]`, `[1,[9,"#ffa9a9"]]`

## `sensing_touchingobject` (72 uses)
- inputs.TOUCHINGOBJECTMENU: 72 distinct source encodings. Examples: `[1,"!T"]`, `[1,"!U"]`, `[1,"!a"]`, `[1,"!j"]`, `[1,"!l"]`, `[1,"!t"]`, `[1,"!v"]`, `[1,")E"]`

## `sensing_touchingobjectmenu` (72 uses)
- fields.TOUCHINGOBJECTMENU: 13 distinct source encodings. Examples: `["Ball",null]`, `["Boss",null]`, `["Died/Pipie",null]`, `["Dragon",null]`, `["Enemy",null]`, `["Final Boss",null]`, `["Levels",null]`, `["Player body",null]`

## `sound_cleareffects` (3 uses)

## `sound_play` (30 uses)
- inputs.SOUND_MENU: 30 distinct source encodings. Examples: `[1,"!/"]`, `[1,"!D"]`, `[1,"!["]`, `[1,"!k"]`, `[1,"!u"]`, `[1,"!~"]`, `[1,"#g"]`, `[1,")="]`

## `sound_playuntildone` (30 uses)
- inputs.SOUND_MENU: 30 distinct source encodings. Examples: `[1,"#/"]`, `[1,"#j"]`, `[1,"#k"]`, `[1,"#n"]`, `[1,"#z"]`, `[1,"+n"]`, `[1,"+q"]`, `[1,"QJ"]`

## `sound_seteffectto` (13 uses)
- fields.EFFECT: 1 distinct source encodings. Examples: `["PITCH",null]`
- inputs.VALUE: 5 distinct source encodings. Examples: `[1,[4,"-20"]]`, `[1,[4,"0"]]`, `[1,[4,"25"]]`, `[1,[4,"5"]]`, `[1,[4,"90"]]`

## `sound_setvolumeto` (19 uses)
- inputs.VOLUME: 2 distinct source encodings. Examples: `[1,[4,"0"]]`, `[1,[4,"100"]]`

## `sound_sounds_menu` (60 uses)
- fields.SOUND_MENU: 23 distinct source encodings. Examples: `["6",null]`, `["A Guitar",null]`, `["Ball",null]`, `["Clang",null]`, `["Coin",null]`, `["Cymbal Echo",null]`, `["Drifting Intro",null]`, `["Drifting Looped",null]`

## `sound_stopallsounds` (34 uses)
