## Building on Linux
1. Download and unzip [Irrlicht](https://irrlicht.sourceforge.io/) under `lib/irrlicht/lib/`
2. Rename the resulting directory to `irrlicht` (full path `lib/irrlicht/lib/irrlicht`)
3. Download and unzip [IrrKlang](https://www.ambiera.at/irrklang/) under `lib/irrklang/lib/`
4. Rename the resulting directory to `irrklang` (full path `lib/irrklang/libirrklang`)
5. Run `make debug` or `make release` to get a debug or release build, respectively
6. Replace "Direct3D9" with "OpenGL" in `cfg/videoconfig.gdat`
7. Run game with `./start.sh` (can be run in GDB with `./start.sh debug`)
