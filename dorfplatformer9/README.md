# Dorfplatformer 9 Native Edition

A C++17 desktop port that loads the six campaign layouts and the original artwork and audio from Benno111's reference bundle. It uses the SDL3, SDL3_image and SDL3_mixer stack from Dorfplatformer-Timetravel. The controller input and frame timing headers are reused from Timetravel commit `37b82e95317ed50f40abf48fab5b6ff18d8b5260` under the license in `third_party`.

## Build and run

Install a C++17 toolchain, Git, CMake 3.24 or later, and Python 3. On Linux, install the development libraries for the X11 or Wayland driver you want SDL to build. From this directory:

```sh
cmake -S . -B build -DDORF_FETCH_SDL=ON -DDORF_SAMPLES=../samples
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
./build/dorfplatformer9
```

With a multi-configuration generator, launch the executable from `build/Release` and pass `build/assets` as its first argument. If using preinstalled SDL packages, leave `DORF_FETCH_SDL` off. SDL3_image needs PNG and SVG support, and SDL3_mixer needs WAV and MP3 support. The reference bundle includes MP3 streams with `.wav` filenames. The importer retains the source names, object order, duplicate positions, and costume pivots.

## Play

| Action | Keyboard | Gamepad |
| --- | --- | --- |
| Start or choose an unlocked level | Enter / Left / Right | A / D-pad |
| Move | A / D or Left / Right | Left stick or D-pad |
| Jump | W, Up or Space | A or B |
| Crouch and enter pipes | S or Down | Down |
| Pause | Esc | Start or Back |

Touch controls appear during gameplay after touch input. Progress saves locally. At the title screen, F5 copies a compatible save code and F9 imports one. Import uses the original progression and power fields. Legacy map coordinates and costume fields have no native equivalent and export with default values.

The native campaign implements sequential progression, both pipe layers, the level-one shortcut, the day-two gate in level six, death and retry, power changes, the world clock, and a three-phase dragon encounter. Collision masks are sampled from the imported tile artwork. Actor behavior, timing and presentation are native approximations of the reference, and the Scratch editor and optional map are outside this build.

The Linux headless tests cover asset decoding, rendering each level, starting movement and jumping, campaign transitions, all 20 pipe pairs, save-code validation, replay unlock behavior, day reset, dragon phase transitions and world expiry. A full playthrough against the original TurboWarp runtime has not been verified.
