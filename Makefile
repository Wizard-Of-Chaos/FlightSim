CC = g++
CFLAGS = -Ilib/irrlicht/include/ -Ilib/bullet/src/ -Ilib/bullet/Extras/ -Ilib/irrklang/include/ -Isrc/systems/ -Isrc/core/ -Isrc/components/ -Isrc/utilities/ -Isrc/gui/hud/ -Isrc/systems/ai_systems/ -Isrc/gui/

BSPREF = lib/bullet/src
BEPREF = lib/bullet/Extras
IRLPREF = lib/irrlicht/lib/irrlicht
IRKPREF = lib/irrklang/lib/irrklang

LDFLAGS :=
LDFLAGS += -L $(BSPREF)/BulletSoftBody -lBulletSoftBody
LDFLAGS += -L $(BSPREF)/BulletDynamics -lBulletDynamics
LDFLAGS += -L $(BSPREF)/BulletCollision -lBulletCollision
LDFLAGS += -L $(BSPREF)/LinearMath -lLinearMath
LDFLAGS += -L $(BEPREF)/Serialize/BulletWorldImporter -lBulletWorldImporter
LDFLAGS += -L $(IRKPREF)/bin/linux-gcc-64/ -lIrrKlang
LDFLAGS += -L $(IRLPREF)/lib/Linux/ -lIrrlicht

INC = $(shell find src/ -type f -name '*.h')
SRC = $(shell find src/ -type f -name '*.cpp')
OBJ = $(patsubst %.cpp,%.o,$(SRC))
EXE = flightsim

.PHONY: bullet clean cleanbullet cleanirrlicht debug irrlicht release

release: CFLAGS += -O3
debug: CFLAGS += -g

release: $(OBJ) bullet irrlicht
	@echo linking...
	@$(CC) $(OBJ) -o $(EXE) $(LDFLAGS)

debug: $(OBJ) bullet irrlicht
	@echo linking debug...
	@$(CC) $(OBJ) -o $(EXE) $(LDFLAGS)

bullet:
	@cd lib/bullet && cmake -DBUILD_PYBULLET=OFF -DUSE_DOUBLE_PRECISION=OFF -DCMAKE_BUILD_TYPE=Release
	@cd ../../
	$(MAKE) -C lib/bullet

irrlicht:
	$(MAKE) -C $(IRLPREF)/source/Irrlicht sharedlib

$(OBJ): $(SRC) $(INC) Makefile

%.o: %.cpp
	@echo compiling $<...
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

cleanbullet:
	$(MAKE) -C lib/bullet clean

cleanirrlicht:
	$(MAKE) -C $(IRLPREF)/source/Irrlicht clean
