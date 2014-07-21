#ifndef _GRAPHICS_MGR_H
#define _GRAPHICS_MGR_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

/**
 * @brief Manager, der alle Grafiken verwaltet
 */
class GraphicsMgr {

public:
	/**
	 * @brief Breite einer Kachel
	 */
	static const int TILE_WIDTH = 64;

	/**
	 * @brief Breite einer Kachel
	 */
	static const int TILE_HEIGHT = 32;

private:
	/**
	 * @brief Referenz auf den SDL-Renderer
	 */
	SDL_Renderer* renderer;

	/**
	 * @brief Array von SDL-Texture-Zeigern auf die Tile-Grafiken
	 */
	SDL_Texture** tileTextures;

	/**
	 * @brief Array von SDL-Texture-Zeigern auf die Objekt-Grafiken
	 */
	SDL_Texture** objectTextures;

public:
	GraphicsMgr(SDL_Renderer* renderer);
	~GraphicsMgr();

	/**
	 * @brief Liefert eine Tile-Grafik
	 * @param index Index ins Array #tileTextures dessen Eintrag zurückgeliefert werden soll
	 */
	SDL_Texture* const getTileTexture(int index) const {
		return tileTextures[index];
	}

	/**
	 * @brief Liefert eine Object-Grafik
	 * @param index Index ins Array #objectTextures dessen Eintrag zurückgeliefert werden soll
	 */
	SDL_Texture* const getObjectTexture(int index) const {
		return objectTextures[index];
	}

private:
	/**
	 * @brief Initialisiert eine Tile-Grafik
	 * @param index Index ins Array #tileTextures unter dessen Eintrag die Grafik geladen werden soll
	 * @param filename Dateiname (ohne Pfad) der Tile-Grafik
	 */
	void initTileTexture(int index, const char* filename);

	/**
	 * @brief Initialisiert eine Object-Grafik
	 * @param index Index ins Array #objectTextures unter dessen Eintrag die Grafik geladen werden soll
	 * @param filename Dateiname (ohne Pfad) der Object-Grafik
	 */
	void initObjectTexture(int index, const char* filename);

};

#endif