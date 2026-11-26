# Dorfplatformer 9 Native Edition

A C++17 native port that loads the six campaign layouts and the original artwork and audio from Benno111's reference bundle. It uses the SDL3, SDL3_image and SDL3_mixer stack from Dorfplatformer-Timetravel. The controller input and frame timing headers are reused from Timetravel commit `37b82e95317ed50f40abf48fab5b6ff18d8b5260` under the license in `third_party`.

## Build and run

Install a C++17 toolchain, Git, CMake 3.24 or later, and Python 3. Build commands below run from this directory unless stated otherwise. On Linux, install the development libraries for the X11 or Wayland driver you want SDL to build.

```sh
cmake -S . -B build -DDORF_FETCH_SDL=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
./build/dorfplatformer9
```

With a multi-configuration generator, launch the executable from `build/Release` and pass `build/assets` as its first argument. If using preinstalled SDL packages, leave `DORF_FETCH_SDL` off. SDL3_image needs PNG and SVG support, and SDL3_mixer needs WAV and MP3 support. The reference bundle includes MP3 streams with `.wav` filenames. Packaged filenames use readable names and source indexes, for example `01-stage-image-001-blue-sky-2.svg`. The importer reads the exact names from the source project and preserves object order, duplicate positions, and costume pivots.

## Packages

The Native builds GitHub Actions workflow produces these artifacts.

| Platform | Targets | Package |
| --- | --- | --- |
| Linux | x64 | Portable tar.gz |
| Windows | x64 and x86 | Portable ZIP and NSIS installer |
| Android | arm64-v8a, armeabi-v7a, x86_64, x86 | Universal debug APK and unsigned release APK |
| iOS | arm64 device and arm64 simulator | Unsigned device IPA and simulator app ZIP |

### Linux and Windows

After building, run the following on Linux:

```sh
cpack --config build/CPackConfig.cmake -C Release -G TGZ -B artifacts
```

Use Visual Studio's `-A x64` or `-A Win32` when configuring Windows, each with its own build directory. Run CPack with `-G ZIP` for a portable package or `-G NSIS` for an installer. The installer requires NSIS. Use `-DDORF_FETCH_SDL=ON` for portable packages with static SDL dependencies.

### Android

Install JDK 17, Gradle 8.13, and Android SDK command-line tools. Use an absolute SDL source path.

```sh
sdkmanager "platforms;android-36" "ndk;28.2.13676358" "cmake;3.31.6"
git clone --depth 1 --branch release-3.4.0 https://github.com/libsdl-org/SDL.git /path/to/SDL
gradle --project-dir android -PsdlSource=/path/to/SDL :app:assembleDebug :app:assembleRelease
```

APKs are under `android/app/build/outputs/apk`. Use `-Ppython=python` if the Python executable is named `python`. To sign a release APK locally, set `DORF_ANDROID_KEYSTORE` to the absolute keystore path and set `DORF_ANDROID_KEY_ALIAS`, `DORF_ANDROID_STORE_PASSWORD`, and `DORF_ANDROID_KEY_PASSWORD` in the build environment.

### iOS

Use Xcode on a Mac and choose `iphonesimulator` or `iphoneos` for the SDK. Use a separate build directory per SDK.

```sh
cmake -S . -B build-ios-sim -G Xcode -DDORF_FETCH_SDL=ON \
  -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator \
  -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=15.0 \
  -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED=NO
cmake --build build-ios-sim --config Release --target dorfplatformer9
```

The simulator ZIP contains a `.app` for an arm64 simulator. The device IPA contains an unsigned app. Device installation requires signing with a provisioning profile. To build a signed device app, use `iphoneos`, omit the flag disabling signing, and configure your development team and signing settings in Xcode.

## Play

| Action | Keyboard | Gamepad |
| --- | --- | --- |
| Navigate menus and select unlocked levels | Arrow keys or Tab, then Enter | D-pad, then A |
| Move | A / D or Left / Right | Left stick or D-pad |
| Jump | W or Up | A or B |
| Crouch and enter pipes | S or Down | Down |
| Fireball (full power) | X or Space | X or Y |
| Pause | Esc | Start or Back |

Gameplay uses a 16:9 viewport. The original day indicator and final-night timer use vector outlines of the source lettering. The larger original touch button sprites appear during gameplay after touch input. Progress saves locally. At the title screen, F5 copies a compatible save code and F9 imports one. Import uses the original progression and power fields. Legacy map coordinates and costume fields have no native equivalent and export with default values.

The native campaign implements sequential progression, both pipe layers, the level-one shortcut, the day-two gate in level six, death and retry, power changes, the world clock, and a three-phase dragon encounter. Collision masks are sampled from the imported tile artwork. Actor behavior, timing and presentation are native approximations of the reference, and the Scratch editor and optional map are outside this build.

The Linux headless tests cover asset decoding, rendering each level, starting movement and jumping, campaign transitions, all 20 pipe pairs, save-code validation, replay unlock behavior, day reset, dragon phase transitions and world expiry. A full playthrough against the original TurboWarp runtime has not been verified.
