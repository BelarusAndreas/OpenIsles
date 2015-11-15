#include "config/ConfigMgr.h"
#include "economics/EconomicsMgr.h"
#include "game/Game.h"


EconomicsMgr::EconomicsMgr(const Context* const context) : ContextAware(context) {
}

EconomicsMgr::~EconomicsMgr() {
}

void EconomicsMgr::updateProduction(Building* building) {
    const MapObjectType* mapObjectType = building->getMapObjectType();

    // Produziert eh nix bzw. Lager schon voll? Dann nix zu tun.
    if (!mapObjectType->buildingProduction.output.isUsed() ||
        building->productionSlots.output.isInventoryFull()) {

        return;
    }

    // Input-Güter reichen?
    auto areAllInputGoodsHere = [&]() {
        if (mapObjectType->buildingProduction.input.isUsed()) {
            if (building->productionSlots.input.inventory < mapObjectType->inputAmountForProduction) {
                return false;
            }

            if (mapObjectType->buildingProduction.input2.isUsed()) {
                if (building->productionSlots.input2.inventory < mapObjectType->input2AmountForProduction) {
                    return false;
                }
            }
        }
        return true;
    };

    if (!areAllInputGoodsHere()) {
        return;
    }

    // Waren die Waren grade frisch angeliefert?
    auto setNextGoodsProductionTicks = [&]() {
        building->hasRecievedAlInputGoodsSinceLastUpdate = true;
        building->nextGoodsProductionTicks = context->game->getTicks() +
            (unsigned long) (mapObjectType->secondsToProduce * TICKS_PER_SECOND);
    };

    if (!building->hasRecievedAlInputGoodsSinceLastUpdate) {
        // Ja. Dann beginnt ab jetzt die Produktionszeit.
        setNextGoodsProductionTicks();
        return;
    }

    // Prüfen, ob es Zeit ist, was zu produzieren
    if (context->game->getTicks() < building->nextGoodsProductionTicks) {
        return;
    }

    // Ja? Ok, dann produzieren
    if (mapObjectType->buildingProduction.input.isUsed()) {
        building->productionSlots.input.decreaseInventory(mapObjectType->inputAmountForProduction);

        if (mapObjectType->buildingProduction.input2.isUsed()) {
            building->productionSlots.input2.decreaseInventory(mapObjectType->input2AmountForProduction);
        }
    }

    building->productionSlots.output.increaseInventory(1);

    // Sind noch genug Input-Güter da? Dann sofort das Helper-Flag setzen, damit sofort wieder die Uhr tickt
    if (areAllInputGoodsHere()) {
        setNextGoodsProductionTicks();
    } else {
        building->hasRecievedAlInputGoodsSinceLastUpdate = false;
    }
}

/**
 * @page finances Finanzen-Berechnung
 *
 * Nomenklatur
 * ===========
 *
 * - "Steuersatz" = Dezimalbruch aus Config-XML (bzw. Anno&nbsp;1602-COD-File)
 * - "Steuerprozent" = angezeigte Steuer-Prozentzahl im Spiel
 * - "Zyklus" = 10 Sekunden Spielzeit. Das ist das Intervall, in welchem Finanzenaktualisiert werden.
 *
 * Überblick Formeln
 * =================
 *
 * Um die Berechnung der Finanzen möglichst genauso wie bei Anno&nbsp;1602 zu machen, würden zwei Formeln
 * untersucht:
 * - a) Um mit dem Steuersatz aus dem COD-File hinzukommen, muss man die Basis auf "34% Steuer" (im Spiel angezeigt)
 *      setzen.
 * - b) Die Formel aus <a href="http://www.annozone.de/Charlie/Cod/numerik.html]">AnnoZone</a>
 *
 * Beide Formeln können mittels einer Hilfstabelle (`doc/taxes-helper.ods`) verglichen werden.
 * Variante b) ist zwar genauer, aber komplizierter zu verstehen. Es wird deshalb Variante a) implementiert.
 *
 * Implementierung
 * ===============
 *
 * Die Formel ist einfach:
 * Der Steuersatz wird mit der Einwohnerzahl multipliziert und ergibt damit genau die Steuern für die Einstellung
 * "34% Steuern". Andere Einstellungen im Spiel werden darauf basierend gemacht. Mit 17% kommt genau die Hälfte raus,
 * bei 68% würde das doppelte rauskommen. Das Ergebnis wird ggf. abgerundet.
 *
 * Pro Bevölkerungsgruppe werden die Steuern wie obig berechnet. Am Ende werden alle (abgerundeten) Teilergebnisse
 * zusammengezählt.
 */

void EconomicsMgr::updatePlayerStatus() {
    const std::set<PopulationTier>& allPopulationTiers = context->configMgr->getAllPopulationTiers();

    unsigned long taxesIncomeSumPerPlayer[4] = { 0, 0, 0, 0 };
    unsigned long populationSumPerPlayer[4] = { 0, 0, 0, 0 };

    // Alle Kolonien durchgehen
    for (auto iter : context->game->getColonies()) {
        const Player* player = iter.first.first;
        const Colony* colony = iter.second;
        int playerIndex = player->getColorIndex();

        // Steuern: alle Bevölkerungsgruppen einzeln betrachten
        unsigned long taxesIncomeColony = 0;

        for (const PopulationTier& populationTier : allPopulationTiers) {
            const ColonyPopulationTier& colonyPopulationTier = colony->populationTiers.at(&populationTier);

            // double -> ulong-Cast zum Abschneiden der Dezimalstellen
            unsigned long taxesIncomeFromPopulationTier = (unsigned long) (
                (double) colonyPopulationTier.population *
                (double) populationTier.taxesPerInhabitant *
                    ((double) colonyPopulationTier.taxesPercent / (double) 34)
            );

            taxesIncomeColony += taxesIncomeFromPopulationTier;
        }

        taxesIncomeSumPerPlayer[playerIndex] += taxesIncomeColony;

        // Einwohner
        populationSumPerPlayer[playerIndex] += colony->population;
    }

    // TODO Betriebskosten

    for (int playerIndex = 0; playerIndex < 4; playerIndex++) {
        PlayerStatus& playerStatus = context->game->getPlayer(playerIndex)->playerStatus;
        playerStatus.population = populationSumPerPlayer[playerIndex];
        playerStatus.taxesIncome = taxesIncomeSumPerPlayer[playerIndex];
    }
}

void EconomicsMgr::updateFinances() {
    for (int playerIndex = 0; playerIndex < 4; playerIndex++) {
        Player* player = context->game->getPlayer(playerIndex);

        unsigned long income = (player->playerStatus.taxesIncome / 6); // abrunden
        player->coins += income;
    }

    // TODO Betriebskosten
}