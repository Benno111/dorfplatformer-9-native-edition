# Dorfplatformer9 native port bundle

Contains all five previously prepared reference sets, the original Scratch project graph, and all 232 unique original assets (125 SVG, 42 PNG, 65 WAV). Asset files retain original hash filenames; assets/manifest.json maps each sprite, costume and sound index to its file and original metadata.

## Start here
- documentation/port_reference/PORT_REFERENCE.md
- documentation/parameter_reference/PARAMETERS.md and OPCODE_PARAMETERS.md
- documentation/player_reference/PLAYER_BEHAVIOR.md
- documentation/progression_reference/PROGRESSION.md
- documentation/audio_reference/AUDIO_USAGE.md

## Source
source/project.json is the original project graph. To reconstruct an SB3, zip this file as project.json together with all files in assets/images and assets/audio at the archive root, then use the .sb3 extension. The original HTML and a duplicate SB3 are omitted to avoid duplicating assets.

Documentation is preserved as previously generated; static analysis is not runtime verification. This is a port reference, not a compiled native game.
