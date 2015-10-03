#ifndef _GUI_STATUS_BAR_H
#define _GUI_STATUS_BAR_H

#include <SDL_events.h>
#include <cstring>
#include "Context.h"
#include "graphics/renderer/IRenderer.h"
#include "gui/components/GuiStaticElement.h"


/**
 * @brief GUI-Element, was die Statusleiste darstellt.
 */
class GuiStatusBar : public GuiStaticElement {

private:
    /**
     * @brief anzuzeigender Text
     */
    std::string text;

public:
    /**
     * @brief Konstruktor.
     * @param context Dependency
     */
    GuiStatusBar(const Context* const context);

    virtual ~GuiStatusBar() override;

    /**
     * @brief Zeichnet das Element (ohne Kinder)
     */
    virtual void renderElement(IRenderer* renderer) override;

    /**
     * @brief Ändert den anzuzeigenden Text
     * @param text anzuzeigender Text
     */
    void setText(const std::string& text) {
        this->text = text;
    }
};

#endif