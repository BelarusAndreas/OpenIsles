#include <string>
#include "game/Colony.h"
#include "game/Game.h"
#include "graphics/mgr/IFontMgr.h"
#include "gui/GuiMgr.h"
#include "gui/components/GuiResourcesBar.h"
#include "utils/Color.h"

static Color colorWhite = Color(255, 255, 255, 255);
static Color colorBlack = Color(0, 0, 0, 255);


GuiResourcesBar::GuiResourcesBar(const Context* const context) : GuiBase(context) {
}

GuiResourcesBar::~GuiResourcesBar() {
}

void GuiResourcesBar::renderElement(IRenderer* renderer) {
    // Münzenguthaben
    context->graphicsMgr->getGraphicSet("coin")->getStatic()->getGraphic()->drawAt(15, 8);

    Player* currentPlayer = context->game->getCurrentPlayer();
    std::string outputString = toString(currentPlayer->coins);
    if (drawBuildingCosts) {
        outputString += " (";
        outputString += toString(buildingCosts.coins);
        outputString += ")";
    }
    context->fontMgr->renderText(renderer, outputString, 42, 10,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 18, RENDERTEXT_HALIGN_LEFT);

    // Siedlung, wo der Cursor grade is
    const MapCoords& mapCoordsUnderMouse = context->guiMgr->getMapCoordsUnderMouse();

    MapTile* mapTileAtCursor = context->game->getMap()->getMapTileAt(mapCoordsUnderMouse);
    if (mapTileAtCursor == nullptr) {
        return;
    }

    Colony* colony = context->game->getColony(mapTileAtCursor->player, mapTileAtCursor->isle);
    if (colony == nullptr) {
        return;
    }

    // Waren (nur für den eigenen Spieler)
    if (mapTileAtCursor->player == currentPlayer) {
        GoodsType goodsToDraw[] = { GoodsType::TOOLS, GoodsType::WOOD, GoodsType::BRICKS };
        int x = 290;
        for (unsigned int i = 0; i < sizeof(goodsToDraw); i++, x += 110) {
            GoodsType goodsType = goodsToDraw[i];
            const std::string graphicSetName = context->graphicsMgr->getGraphicSetNameForGoodIcons(goodsType, false);
            context->graphicsMgr->getGraphicSet(graphicSetName)->getStatic()->getGraphic()->drawAt(x, 5);

            int goodsInventory = (int) colony->getGoods(goodsType).inventory;
            outputString = toString(goodsInventory);
            if (drawBuildingCosts) {
                outputString += " (";
                outputString += toString(
                    (goodsType == GoodsType::TOOLS) ? buildingCosts.tools :
                        (goodsType == GoodsType::WOOD) ? buildingCosts.wood :
                            (goodsType == GoodsType::BRICKS) ? buildingCosts.bricks : 0);
                outputString += ")";
            }

            context->fontMgr->renderText(renderer, outputString, x + 35, 10,
                &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 18, RENDERTEXT_HALIGN_LEFT);
        }
    }

    // Einwohnerzahl (immer anzeigen)
    const std::string graphicSetName =
        context->graphicsMgr->getGraphicSetNameForCoatOfArmsPopulation(mapTileAtCursor->player->getColor());
    const IGraphic* populationIconGraphic = context->graphicsMgr->getGraphicSet(graphicSetName)->getStatic()->getGraphic();
    populationIconGraphic->drawAt(655, 6);

    outputString = toString(colony->population);
    context->fontMgr->renderText(renderer, outputString, 690, 10,
        &colorWhite, &colorBlack, "DroidSans-Bold.ttf", 18, RENDERTEXT_HALIGN_LEFT);
}