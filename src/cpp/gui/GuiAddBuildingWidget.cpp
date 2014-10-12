#include <game/Colony.h>
#include "SDL.h"
#include "config/BuildingConfigMgr.h"
#include "game/Game.h"
#include "gui/FontMgr.h"
#include "gui/GuiAddBuildingWidget.h"


static SDL_Color colorLightBrown = {223, 216, 183, 255};
static SDL_Color colorBlack = {0, 0, 0, 255};
static SDL_Color colorWhite = {255, 255, 255, 255};

// Aus main.cpp importiert
extern BuildingConfigMgr* buildingConfigMgr;
extern FontMgr* fontMgr;
extern Game* game;
extern GraphicsMgr* graphicsMgr;


GuiAddBuildingWidget::GuiAddBuildingWidget() {
    setCoords(8, 0, 210, 320);

    // TODO Child-Buttons zum Drehen hinzufügen
}

void GuiAddBuildingWidget::renderElement(SDL_Renderer* renderer) {
    StructureType structureType = game->getAddingStructure();
    if (structureType == NO_STRUCTURE) {
        // wenn wir nix platzieren, sollten das Widget eigentlich auch nicht sichtbar sein
        return;
    }

    int windowX, windowY;
    getWindowCoords(windowX, windowY);

    const BuildingConfig* buildingConfig = buildingConfigMgr->getConfig(structureType);

    // Gebäudename
    fontMgr->renderText(renderer, buildingConfig->getName(), windowX + width/2, windowY + 15,
        &colorLightBrown, &colorBlack, "DroidSans-Bold.ttf", 15, RENDERTEXT_HALIGN_CENTER);

    // Gebäude-Grafik
    Graphic* buildingGraphic = graphicsMgr->getGraphicForStructure(structureType);

    double scale; // ggf. verkleinert zeichnen, wenn das Gebäude zu groß is
    if (buildingGraphic->getWidth() <= 160) {
        scale = 1;
    } else if (buildingGraphic->getWidth() <= 240) {
        scale = 0.666666666;
    } else {
        scale = 0.5;
    }

    SDL_Rect rect = { 0, 0, (int) (buildingGraphic->getWidth() * scale), (int) (buildingGraphic->getHeight() * scale) };
    rect.x = windowX + 110 - (rect.w / 2);
    rect.y = windowY + 40;
    SDL_RenderCopy(renderer, buildingGraphic->getTexture(), nullptr, &rect);

    // produzierte Waren
    const BuildingProduction* buildingProduction = buildingConfig->getBuildingProduction();
    int productionY = windowY + 160;
    if (buildingProduction->input2 != GoodsType::NO_GOODS) {
        // input + input2 -> output

        graphicsMgr->getGraphicForGoodsMarketplaceIcon(buildingProduction->input)->drawAt(windowX + 29, productionY);
        graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_PLUS)->drawAt(windowX + 75, productionY);
        graphicsMgr->getGraphicForGoodsMarketplaceIcon(buildingProduction->input2)->drawAt(windowX + 90, productionY);
        graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_ARROW)->drawAt(windowX + 136, productionY);
        graphicsMgr->getGraphicForGoodsMarketplaceIcon(buildingProduction->output)->drawAt(windowX + 150, productionY);
    }
    else if (buildingProduction->input != GoodsType::NO_GOODS) {
        // input -> output

        graphicsMgr->getGraphicForGoodsMarketplaceIcon(buildingProduction->input)->drawAt(windowX + 60, productionY);
        graphicsMgr->getOtherGraphic(OtherGraphic::PRODUCTION_ARROW)->drawAt(windowX + 106, productionY);
        graphicsMgr->getGraphicForGoodsMarketplaceIcon(buildingProduction->output)->drawAt(windowX + 120, productionY);
    }
    else if (buildingProduction->output != GoodsType::NO_GOODS) {
        // output

        graphicsMgr->getGraphicForGoodsMarketplaceIcon(buildingProduction->output)->drawAt(windowX + 90, productionY);
    }

    // Baukosten
    int costsY = productionY + 60;

    graphicsMgr->getOtherGraphic(OtherGraphic::COINS)->drawAt(windowX, costsY);
    fontMgr->renderText(renderer, std::to_string(buildingConfig->getBuildingCosts()->coins), windowX + 35, costsY + 12,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

    costsY += 30;

    graphicsMgr->getGraphicForGoodsIcon(GoodsType::TOOLS)->drawAt(windowX, costsY);
    fontMgr->renderText(renderer, std::to_string(buildingConfig->getBuildingCosts()->tools), windowX + 35, costsY + 16,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

    graphicsMgr->getGraphicForGoodsIcon(GoodsType::WOOD)->drawAt(windowX + 60, costsY);
    fontMgr->renderText(renderer, std::to_string(buildingConfig->getBuildingCosts()->wood), windowX + 95, costsY + 16,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

    graphicsMgr->getGraphicForGoodsIcon(GoodsType::BRICKS)->drawAt(windowX + 120, costsY);
    fontMgr->renderText(renderer, std::to_string(buildingConfig->getBuildingCosts()->bricks), windowX + 155, costsY + 16,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 14, RENDERTEXT_HALIGN_LEFT | RENDERTEXT_VALIGN_MIDDLE);

}