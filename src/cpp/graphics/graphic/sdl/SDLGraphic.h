#ifndef _SDL_GRAPHIC_H
#define _SDL_GRAPHIC_H

#include <SDL.h>
#include <SDL_image.h>
#include "graphics/graphic/IGraphic.h"
#include "graphics/renderer/IRenderer.h"

/**
 * @brief Basisklasse für alle Grafiken. Kann nix außer die Grafik zu laden und entladen
 */
class SDLGraphic : public virtual IGraphic {

protected: // Dependencies
	IRenderer* const renderer;

protected:
	/**
	 * @brief Dateiname der Grafik (durch ConfigMgr verwaltet)
	 */
	const char* filename;

	/**
	 * @brief Breite der Grafik
	 */
	int width;

	/**
	 * @brief Höhe der Grafik
	 */
	int height;

	/**
	 * @brief SDL-Texture der geladenen Grafik
	 */
	SDL_Texture* texture;

	/**
	 * @brief SDL-Surface
	 */
	SDL_Surface* surface;

public:
    /**
     * @brief Konstruktor. Lädt die Grafik.
	 * @param renderer (Dependency)
     * @param filename Dateiname der zu ladenden Grafik
     */
    SDLGraphic(IRenderer* const renderer, const char* filename);

    /**
     * @brief Destruktor. Entlädt die Grafik und gibt Speicher frei.
     */
	virtual ~SDLGraphic();

	/**
	 * @brief Liefert die Breite der Grafik zurück
	 * @return Breite der Grafik
	 */
    virtual int getWidth() const {
		return width;
	}

	/**
	 * @brief Liefert die Höhe der Grafik zurück
	 * @return Höhe der Grafik
	 */
	virtual int getHeight() const {
		return height;
	}

	/**
	 * @brief Liest die Farbwerte eines bestimmten Pixels aus und setzt sie in die r-, g-, b- und a-Parameter.
	 * @param x x-Koordinate (IN)
	 * @param y y-Koordinate (IN)
	 * @param r wird auf den Rot-Wert des Pixels gesetzt (OUT)
	 * @param g wird auf den Grün-Wert des Pixels gesetzt (OUT)
	 * @param b wird auf den Blau-Wert des Pixels gesetzt (OUT)
	 * @param a wird auf den Alpha-Wert des Pixels gesetzt (OUT) 0 = voll transparent, 255 = absolut undurchsichtig
	 */
	virtual void getPixel(int x, int y, unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);

};

#endif