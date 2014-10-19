#include <stdexcept>
#include <iostream>
#include "graphics/Animation.h"

extern SDL_Renderer* renderer;

Animation::Animation(const char* filename, unsigned int framesCount, double fps) :
    PlainGraphic(filename), framesCount(framesCount), fps(fps) {

    // Prüfen, ob die Grafik ordentlich ist. Die Breite muss exakt ein Vielfaches der Framesanzahl sein
    if (width % framesCount != 0) {
        std::cerr << "Could not load animation '" << filename << "': frames do not match" << std::endl;
        throw new std::runtime_error("Could not load animation");
    }

    frameWidth = width / framesCount;
}

void Animation::drawFrameAt(int x, int y, unsigned int frame) {
    if (frame < 0 || frame >= framesCount) {
        std::cerr << "Illegal frame " << frame << " for animation '" << filename << "'" << std::endl;
        throw new std::runtime_error("Illegal frame");
    }

    SDL_Rect rectSource = { (int)frame * frameWidth, 0, frameWidth, height };
    SDL_Rect rectDestination = { x, y, frameWidth, height };
    SDL_RenderCopy(renderer, texture, &rectSource, &rectDestination);
}