SRC_DIRECTORY := src
SRC_CPP_DIRECTORY := $(SRC_DIRECTORY)/cpp
DATA_DIRECTORY := data
BUILD_DIRECTORY := build
OBJ_FILES := \
	$(BUILD_DIRECTORY)/config/BuildingConfigMgr.o \
	$(BUILD_DIRECTORY)/gui/GuiButton.o \
	$(BUILD_DIRECTORY)/gui/GuiMgr.o \
	$(BUILD_DIRECTORY)/gui/GuiPushButton.o \
	$(BUILD_DIRECTORY)/gui/GuiStaticElement.o \
	$(BUILD_DIRECTORY)/map/Building.o \
	$(BUILD_DIRECTORY)/map/Isle.o \
	$(BUILD_DIRECTORY)/map/Map.o \
	$(BUILD_DIRECTORY)/map/MapUtils.o \
	$(BUILD_DIRECTORY)/sound/SoundMgr.o \
	$(BUILD_DIRECTORY)/Game.o \
	$(BUILD_DIRECTORY)/Graphic.o \
	$(BUILD_DIRECTORY)/GraphicsMgr.o \
	$(BUILD_DIRECTORY)/main.o
	
MAP_HEADER_FILES := $(addprefix $(SRC_CPP_DIRECTORY)/map/,Building.h Map.h MapObject.h MapUtils.h Structure.h)

# Für das fertige Tileset für Tiled: Anzahl Kacheln pro Zeile
TILESET_WIDTH := 4

SDL_CFLAGS := `sdl2-config --cflags`
SDL_LDFLAGS := `sdl2-config --libs`

CC := /usr/bin/g++
CFLAGS := -std=c++11 -Wall -Werror -pedantic -g -O0 -I$(SRC_CPP_DIRECTORY) $(SDL_CFLAGS)
LDFLAGS := $(SDL_LDFLAGS) -lSDL2_image -lSDL2_mixer -lSDL2_ttf

CREATE_TARGET_DIRECTORY = mkdir -p $(@D)


.PHONY: all clean build-tiles clean-tiles build-gui clean-gui

all: build-tiles build-gui $(BUILD_DIRECTORY)/OpenIsles

clean: clean-tiles clean-gui
	rm -rf $(BUILD_DIRECTORY)
	

$(BUILD_DIRECTORY)/config/BuildingConfigMgr.o: $(SRC_CPP_DIRECTORY)/config/BuildingConfigMgr.cpp $(SRC_CPP_DIRECTORY)/config/BuildingConfigMgr.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<


$(BUILD_DIRECTORY)/gui/GuiButton.o: $(SRC_CPP_DIRECTORY)/gui/GuiButton.cpp $(SRC_CPP_DIRECTORY)/gui/GuiButton.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/gui/GuiMgr.o: $(SRC_CPP_DIRECTORY)/gui/GuiMgr.cpp $(SRC_CPP_DIRECTORY)/gui/GuiMgr.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/gui/GuiPushButton.o: $(SRC_CPP_DIRECTORY)/gui/GuiPushButton.cpp $(SRC_CPP_DIRECTORY)/gui/GuiPushButton.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/gui/GuiStaticElement.o: $(SRC_CPP_DIRECTORY)/gui/GuiStaticElement.cpp $(SRC_CPP_DIRECTORY)/gui/GuiStaticElement.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<

	
$(BUILD_DIRECTORY)/map/Building.o: $(SRC_CPP_DIRECTORY)/map/Building.cpp $(MAP_HEADER_FILES)
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/map/Isle.o: $(SRC_CPP_DIRECTORY)/map/Isle.cpp $(SRC_CPP_DIRECTORY)/map/Isle.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/map/Map.o: $(SRC_CPP_DIRECTORY)/map/Map.cpp $(MAP_HEADER_FILES) $(SRC_CPP_DIRECTORY)/GraphicsMgr.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/map/MapUtils.o: $(SRC_CPP_DIRECTORY)/map/MapUtils.cpp $(MAP_HEADER_FILES)
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<

	
$(BUILD_DIRECTORY)/sound/SoundMgr.o: $(SRC_CPP_DIRECTORY)/sound/SoundMgr.cpp $(SRC_CPP_DIRECTORY)/sound/SoundMgr.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
	
$(BUILD_DIRECTORY)/Game.o: $(SRC_CPP_DIRECTORY)/Game.cpp $(SRC_CPP_DIRECTORY)/Game.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/Graphic.o: $(SRC_CPP_DIRECTORY)/Graphic.cpp $(SRC_CPP_DIRECTORY)/Graphic.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/GraphicsMgr.o: $(SRC_CPP_DIRECTORY)/GraphicsMgr.cpp $(SRC_CPP_DIRECTORY)/GraphicsMgr.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BUILD_DIRECTORY)/main.o: $(SRC_CPP_DIRECTORY)/main.cpp $(SRC_CPP_DIRECTORY)/gui/GuiMgr.h $(SRC_CPP_DIRECTORY)/map/Map.h $(SRC_CPP_DIRECTORY)/sound/SoundMgr.h $(SRC_CPP_DIRECTORY)/GraphicsMgr.h
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ -c $<
	

$(BUILD_DIRECTORY)/OpenIsles: $(OBJ_FILES)
	$(CREATE_TARGET_DIRECTORY)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)


build-tiles:
	awk 'NF > 0 && /^[^#]/ { print "convert $(SRC_DIRECTORY)/xcf/tileset-src.xcf -crop 64x32+" $$2 "+" $$3 " $(DATA_DIRECTORY)/img/tiles/alpha-mask.png -alpha Off -compose CopyOpacity -composite $(DATA_DIRECTORY)/img/tiles/" $$4 }' $(DATA_DIRECTORY)/img/tiles/tiles.txt | bash
	convert -size $$(($(TILESET_WIDTH)*64))x256 canvas:transparent $(DATA_DIRECTORY)/img/tiles/tileset.png
	awk 'NF > 0 && /^[^#]/ { print "convert $(DATA_DIRECTORY)/img/tiles/tileset.png $(DATA_DIRECTORY)/img/tiles/" $$4 " -geometry +" (($$1-1)%$(TILESET_WIDTH))*64 "+" int(($$1-1)/$(TILESET_WIDTH))*32 " -composite $(DATA_DIRECTORY)/img/tiles/tileset.png" }' $(DATA_DIRECTORY)/img/tiles/tiles.txt | bash
	
clean-tiles:
	awk 'NF > 0 && /^[^#]/ { print "rm -f $(DATA_DIRECTORY)/img/tiles/" $$4 }' $(DATA_DIRECTORY)/img/tiles/tiles.txt | bash
	rm -f $(DATA_DIRECTORY)/img/tiles/tileset.png
	
	
clean-gui:
	rm -f $(DATA_DIRECTORY)/img/gui/panel.png
	rm -f $(DATA_DIRECTORY)/img/gui/statusbar.png
	
build-gui: $(DATA_DIRECTORY)/img/gui/panel.png $(DATA_DIRECTORY)/img/gui/statusbar.png

	
$(DATA_DIRECTORY)/img/gui/panel.png:
	$(CREATE_TARGET_DIRECTORY)
	convert -size 236x748 canvas:"#907f67" -mattecolor "#6f5038" -frame 10x10+5+5 $@
	
$(DATA_DIRECTORY)/img/gui/statusbar.png:
	$(CREATE_TARGET_DIRECTORY)
	convert -size 758x24 canvas:"#907f67" -mattecolor "#6f5038" -frame 5x5+2+2 $@
