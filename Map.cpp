#include "GraphicsMgr.h"
#include "Map.h"

/**
 * Die Karte stellt sich wiefolgt dar:
 *
 *                 y=0
 *             --/  (0,0)   x=0
 *       y <--/      ---        \--
 *                --/   \--        \--> x
 *             --/         \--
 *          --/               \--(w-1,0)
 *    (0,h-1)  --\         /--
 *                --\   /--
 *                   ---
 *                (w-1,h-1)
 *
 *
 * Koordinatensysteme:
 *   Obenstehend sind die Kachel- oder Map-Koordinaten (mapCoords) dargestellt.
 *   Bildschirm-Koordinaten (screenCoords) sind durch ein Pixel-Koordinatensystem definiert. Hierbei wird die
 *   Kachel mit mapCoords (0, 0) auf screenCoords (0, 0) gelegt.
 *   Die Bildschirm-Koordinate einer Kachel bestimmt die Position des Pixels links-oben an der Kachel.
 */

// Aus main.cpp importiert
extern GraphicsMgr* graphicsMgr;

Map::Map(unsigned int width, unsigned int height) :
		width(width), height(height) {

	tiles = new unsigned char[width * height];

	// TODO Karte laden statt einfach nur statisch eine Insel erzeugen
	for (unsigned int y = 0, i = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++, i++) {
			if (x < 3 || y < 3 || x >= width - 3 || y >= height - 3) {
				tiles[i] = 0;
			} else {
				tiles[i] = ((x + y) % 2 == 0) ? 1 : 2;
			}
		}
	}

	MapObject* dummyMapObject = new MapObject { 3, 3, 0 };
	mapObjects.push_front(dummyMapObject);

	dummyMapObject = new MapObject { 6, 3, 1 };
	mapObjects.push_front(dummyMapObject);

	dummyMapObject = new MapObject { 9, 3, 2 };
	mapObjects.push_front(dummyMapObject);

	dummyMapObject = new MapObject { 12, 3, 3 };
	mapObjects.push_front(dummyMapObject);

	dummyMapObject = new MapObject { 15, 3, 4 };
	mapObjects.push_front(dummyMapObject);

	dummyMapObject = new MapObject { 18, 3, 5 };
	mapObjects.push_front(dummyMapObject);

	screenOffsetX = 0;
	screenOffsetY = 0;
}

Map::~Map() {
	for (auto iter = mapObjects.cbegin(); iter != mapObjects.cend(); iter++) {
		MapObject* mapObject = *iter;
		delete mapObject;
	}
	mapObjects.clear();

	delete[] tiles;
}

void Map::mapToScreenCoords(unsigned int mapX, unsigned int mapY, int& screenX, int& screenY) {
	// Koordinatensystem wechseln
	screenX = (mapX - mapY) * (GraphicsMgr::TILE_WIDTH / 2);
	screenY = (mapX + mapY) * (GraphicsMgr::TILE_HEIGHT / 2);

	// Scrolling-Offset anwenden
	screenX -= screenOffsetX;
	screenY -= screenOffsetY;
}

void Map::renderMap(SDL_Renderer* renderer) {
	// Kacheln rendern
	SDL_Rect rectDestination = { 0, 0, GraphicsMgr::TILE_WIDTH, GraphicsMgr::TILE_HEIGHT };
	for (unsigned int mapY = 0; mapY < height; mapY++) {
		for (unsigned int mapX = 0; mapX < width; mapX++) {
			mapToScreenCoords(mapX, mapY, rectDestination.x, rectDestination.y);
			SDL_RenderCopy(renderer, graphicsMgr->getTile(getTileAt(mapX, mapY))->getTexture(), NULL, &rectDestination);
		}
	}

	// Objekte rendern
	// TODO Reihenfolge muss entsprechend von hinten nach vorne passieren und nicht wild durcheinander
	for (auto iter = mapObjects.cbegin(); iter != mapObjects.cend(); iter++) {
		MapObject* mapObject = *iter;
		Graphic* graphic = graphicsMgr->getObject(mapObject->object);

		SDL_Rect rect = { 0, 0, graphic->getWidth(), graphic->getHeight() };
		mapToScreenCoords(mapObject->mapX, mapObject->mapY, rect.x, rect.y);

		// Grafik an die richtige Stelle schieben. Das muss ausgehend von der zu belegenden Tile-Fläche berechnet werden.
		rect.x -= graphic->getWidth() - (graphic->getMapWidth() + 1) * (GraphicsMgr::TILE_WIDTH / 2);
		rect.y -= graphic->getHeight()
				- (graphic->getMapWidth() + graphic->getMapHeight()) * (GraphicsMgr::TILE_HEIGHT / 2);

		SDL_Texture* objectTexture = graphic->getTexture();
		SDL_SetTextureAlphaMod(objectTexture, 192);
		SDL_SetTextureColorMod(objectTexture, 255, 64, 64);
		SDL_RenderCopy(renderer, objectTexture, NULL, &rect);

		SDL_RenderDrawRect(renderer, &rect);
	}

	// Bildfläche anzeigen
	SDL_RenderPresent(renderer);
}

void Map::scroll(int screenOffsetX, int screenOffsetY) {
	this->screenOffsetX += screenOffsetX;
	this->screenOffsetY += screenOffsetY;
}
