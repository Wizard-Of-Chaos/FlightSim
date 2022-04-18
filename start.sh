#! /bin/bash

export LD_LIBRARY_PATH=lib/bullet/src/BulletSoftBody:lib/bullet/src/BulletDynamics:lib/bullet/src/BulletCollision:lib/bullet/src/LinearMath:lib/bullet/Extras/Serialize/BulletWorldImporter:lib/irrklang/lib/irrklang/bin/linux-gcc-64:lib/irrlicht/lib/irrlicht/lib/Linux
export PATH=$PATH:.

if [[ $# -ge 1 ]] && [[ $1 = "debug" ]]; then
	gdb flightsim
else
	flightsim
fi
