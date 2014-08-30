#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include "config/BuildingConfigMgr.h"
#include "game/Game.h"
#include "game/Player.h"
#include "gui/GuiMgr.h"
#include "map/Map.h"
#include "sound/SoundMgr.h"
#include "utils/FpsCounter.h"
#include "GraphicsMgr.h"


/*********************************************************************************************************************
 * globale Variablen                                                                                                 *
 *********************************************************************************************************************/

/**
 * @brief SDL-Renderer
 */
SDL_Renderer* renderer;

/**
 * @brief Grafik-Manager
 */
GraphicsMgr* graphicsMgr;

/**
 * @brief TTF-Schriftart zum Render der FPS
 */
TTF_Font* ttfFont;

/**
 * @brief aktuelle Position des Mauszeigers
 */
int mouseCurrentX, mouseCurrentY;

/**
 * @brief FPS-Counter
 */
FpsCounter* fpsCounter;

/**
 * @brief Zeichenketten-Puffer für 7 Zeilen Debug-Ausgabe
 */
std::string debugOutput[7];

/**
 * @brief die Karte
 */
Map* map;

/**
 * @brief die Benutzeroberfläche
 */
GuiMgr* guiMgr;

/**
 * @brief der Sound-Manager
 */
SoundMgr* soundMgr;

/**
 * @brief Die Konfiguration der Gebäude
 */
BuildingConfigMgr* buildingConfigMgr;

/**
 * @brief Zustand des Spiels
 */
Game* game;

/**
 * @brief Größe des Fensters
 */
extern const int windowWidth = 1024;
extern const int windowHeight = 768;

/**
 * @brief Bool-Flag, um die Mainloop zu breaken und das Spiel zu beenden
 */
bool quitGame = false;

/*********************************************************************************************************************
 * Prototypen                                                                                                        *
 *********************************************************************************************************************/

int main(int argc, char** argv);
void renderText(SDL_Renderer* renderer, std::string string, int x, int y, int align);
void drawFrame(SDL_Renderer* renderer);

/*********************************************************************************************************************
 * Implementierung                                                                                                   *
 *********************************************************************************************************************/

#define RENDERTEXT_HALIGN_LEFT 0x00
#define RENDERTEXT_HALIGN_RIGHT 0x01
#define RENDERTEXT_HALIGN_CENTER 0x02
#define RENDERTEXT_HALIGN_MASK 0x03
#define RENDERTEXT_VALIGN_TOP 0x00
#define RENDERTEXT_VALIGN_BOTTOM 0x10
#define RENDERTEXT_VALIGN_MIDDLE 0x20
#define RENDERTEXT_VALIGN_MASK 0x30
void renderText(SDL_Renderer* renderer, std::string string, int x, int y, int align) {
	SDL_Color fpsColor = { 255, 255, 255, 0 };
	SDL_Surface* surfaceText = TTF_RenderUTF8_Solid(ttfFont, string.data(), fpsColor);
    
	SDL_Rect rectDestination;
    rectDestination.w = surfaceText->w;
    rectDestination.h = surfaceText->h;
    
    switch (align & RENDERTEXT_HALIGN_MASK) {
        default: // fall-through
        case RENDERTEXT_HALIGN_LEFT:
            rectDestination.x = x;
            break;
        case RENDERTEXT_HALIGN_RIGHT:
            rectDestination.x = x - surfaceText->w;
            break;
        case RENDERTEXT_HALIGN_CENTER:
            rectDestination.x = x - surfaceText->w / 2;
            break;
    }
    
    switch (align & RENDERTEXT_VALIGN_MASK) {
        default: // fall-through
        case RENDERTEXT_VALIGN_TOP:
            rectDestination.y = y;
            break;
        case RENDERTEXT_VALIGN_BOTTOM:
            rectDestination.y = y - surfaceText->h;
            break;
        case RENDERTEXT_VALIGN_MIDDLE:
            rectDestination.y = y - surfaceText->h / 2;
            break;
    }
    
	SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	SDL_RenderCopy(renderer, textureText, NULL, &rectDestination);
	SDL_DestroyTexture(textureText);
}

void drawFrame(SDL_Renderer* renderer) {
	// Karte rendern
	map->renderMap(renderer);

	// UI rendern
	guiMgr->render(renderer);
    
    // Statuszeile
    const MapObject* selectedMapObject = map->getSelectedMapObject();
    if (selectedMapObject != nullptr) {
        const Building* selectedBuilding = reinterpret_cast<const Building*>(selectedMapObject);
        if (selectedBuilding != nullptr) {
            const BuildingConfig* buildingConfig = buildingConfigMgr->getConfig(selectedBuilding->getStructureType());
            renderText(renderer, buildingConfig->name, 753, 744, RENDERTEXT_HALIGN_RIGHT);
        }
    }
    
    // Minimap auf die GUI rendern
	map->renderMinimap(renderer);

	// Debugging-Infos rendern
	for (int i = 0; i < 7; i++) {
		if (debugOutput[i].empty()) {
			continue;
		}

		renderText(renderer, debugOutput[i], 10, 10 + 15 * i, RENDERTEXT_HALIGN_LEFT);
	}
}

int main(int argc, char** argv) {
	// Library-Initialisierung ///////////////////////////////////////////////////////////////////////////////////////

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
		std::cerr << "SDL could not be initialized: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	atexit(SDL_Quit);
	std::cout << "SDL initialisert" << std::endl;

	SDL_Surface* surfaceAppIcon = IMG_Load("data/img/appicon.png");
	if (surfaceAppIcon == nullptr) {
		std::cerr << "Could not load graphic 'data/img/appicon.png': " << IMG_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Window* window = SDL_CreateWindow("OpenIsles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth,
			windowHeight, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cerr << "SDL could not create window: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_SetWindowIcon(window, surfaceAppIcon);
	SDL_FreeSurface(surfaceAppIcon);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == nullptr) {
		std::cerr << "SDL could not create renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		return EXIT_FAILURE;
	}

	SDL_Texture* offscreenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
			windowWidth, windowHeight);

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cerr << "Could not init SDL-image: " << IMG_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	atexit(IMG_Quit);

	if (TTF_Init() != 0) {
		std::cerr << "Could not init SDL-TTF: " << TTF_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	atexit(TTF_Quit);

	ttfFont = TTF_OpenFont("data/font/DroidSans-Bold.ttf", 14);

	// Game-Initialisierung //////////////////////////////////////////////////////////////////////////////////////////

	soundMgr = new SoundMgr();
    buildingConfigMgr = new BuildingConfigMgr();
	graphicsMgr = new GraphicsMgr();
    guiMgr = new GuiMgr();
    fpsCounter = new FpsCounter(500);
    
    Player* myPlayer = new Player(PlayerColor::RED, "Spieler 1");
    game = new Game();
    game->addPlayer(myPlayer);
    game->addPlayer(new Player(PlayerColor::YELLOW, "Spieler 2"));
    game->addPlayer(new Player(PlayerColor::GREEN, "Spieler 3"));
    game->addPlayer(new Player(PlayerColor::BLUE, "Spieler 4"));
    game->setCurrentPlayer(myPlayer);
    
	map = new Map();

	// Mainloop //////////////////////////////////////////////////////////////////////////////////////////////////////

	while (!quitGame) {
		fpsCounter->startFrame();

		// Events handeln
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
            guiMgr->onEvent(event);
		}
        
        // Position des Mauszeigers holen
        SDL_GetMouseState(&mouseCurrentX, &mouseCurrentY);

		// Debug-Infos vorbereiten, damit wir sie später einfach nur ausgeben können
		debugOutput[0] = "FPS: average = " + std::to_string(fpsCounter->getFpsAvg()) + 
                ", current = " + std::to_string(fpsCounter->getFpsCurrent());
        
        int mapX, mapY, screenX, screenY;
        screenX = mouseCurrentX + map->getScreenOffsetX();
        screenY = mouseCurrentY + map->getScreenOffsetY();
        MapUtils::screenToMapCoords(screenX, screenY, mapX, mapY);
        
        debugOutput[1] = "mouse = (" + 
                std::to_string(mouseCurrentX) + ", " + std::to_string(mouseCurrentY) + "), map = (" +
                std::to_string(mapX) + ", " + std::to_string(mapY) + "), screen = (" +
                std::to_string(screenX) + ", " + std::to_string(screenY) + ")";
    
        if (map->getSelectedMapObject() != nullptr) {
            int mapX, mapY, mapWidth, mapHeight;
            int screenX, screenY, screenWidth, screenHeight;
            
            map->getSelectedMapObject()->getMapCoords(mapX, mapY, mapWidth, mapHeight);
            map->getSelectedMapObject()->getScreenCoords(screenX, screenY, screenWidth, screenHeight);
            
            debugOutput[2] = "selectedMapObject on mapCoords (" + 
                    std::to_string(mapX) + ", " + std::to_string(mapY) + "), size = (" +
                    std::to_string(mapWidth) + ", " + std::to_string(mapHeight) + ")";
            
            debugOutput[3] = "selectedMapObject on screenCoords (" + 
                    std::to_string(screenX) + ", " + std::to_string(screenY) + "), size = (" + 
                    std::to_string(screenWidth) + ", " + std::to_string(screenHeight) + ")";
        } else {
            debugOutput[2] = "";
            debugOutput[3] = "";
        }

		// Frame auf Offscreen-Texture zeichnen
		SDL_SetRenderTarget(renderer, offscreenTexture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
		SDL_RenderClear(renderer);
		drawFrame(renderer);

		// Frame fertig. Erst jetzt komplett in Fenster übertragen, um Flackern zu unterbinden
		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderCopy(renderer, offscreenTexture, nullptr, nullptr);
		SDL_RenderPresent(renderer);

		fpsCounter->endFrame();
	}

	// Game-Deinitialisierung ////////////////////////////////////////////////////////////////////////////////////////
    
    delete map;
    delete game;
    
    delete fpsCounter;
	delete guiMgr;
	delete graphicsMgr;
    delete buildingConfigMgr;
	delete soundMgr;

	// Library-Deinitialisierung /////////////////////////////////////////////////////////////////////////////////////

	TTF_CloseFont(ttfFont);

	SDL_DestroyTexture(offscreenTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
