#include <SDL.h>
#include "graphics/graphic/IPlainGraphic.h"
#include "gui/GuiMgr.h"
#include "gui/components/GuiButton.h"


GuiButton::GuiButton(const Context* const context) : GuiStaticElement(context) {
}

GuiButton::~GuiButton() {
}

void GuiButton::renderElement(IRenderer* renderer) {
    int windowX, windowY;
    getWindowCoords(windowX, windowY);
    
    IPlainGraphic* graphicToUse = (pressed) ? graphicPressed : graphic;
    graphicToUse->drawAt(windowX, windowY);
}

void GuiButton::onEventElement(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT &&
        hitTest(event.button.x, event.button.y)) {

        pressed = true;
    }
    
    if (event.type == SDL_MOUSEBUTTONUP &&
        event.button.button == SDL_BUTTON_LEFT &&
        pressed) {

        pressed = false;
        onClickFunction();
    }
}