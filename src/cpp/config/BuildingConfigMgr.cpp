#include <string.h>
#include "config/BuildingConfigMgr.h"
#include "map/Structure.h"


BuildingConfigMgr::BuildingConfigMgr() {
    configs = new BuildingConfig*[StructureType::MAX_STRUCTURE];
    memset(configs, 0, StructureType::MAX_STRUCTURE * sizeof(BuildingConfig*));
    
    configs[StructureType::WAY_E] = new BuildingConfig();
    configs[StructureType::WAY_E]->name = "Weg";
    configs[StructureType::WAY_E]->catchmentArea = nullptr;
    configs[StructureType::WAY_E]->buildingCosts = { 5, 0, 0, 0 };
    
    configs[StructureType::WAY_N] = new BuildingConfig();
    configs[StructureType::WAY_N]->name = "Weg";
    configs[StructureType::WAY_N]->catchmentArea = nullptr;
    configs[StructureType::WAY_N]->buildingCosts = { 5, 0, 0, 0 };
    
    configs[StructureType::WAY_S] = new BuildingConfig();
    configs[StructureType::WAY_S]->name = "Weg";
    configs[StructureType::WAY_S]->catchmentArea = nullptr;
    configs[StructureType::WAY_S]->buildingCosts = { 5, 0, 0, 0 };
    
    configs[StructureType::WAY_W] = new BuildingConfig();
    configs[StructureType::WAY_W]->name = "Weg";
    configs[StructureType::WAY_W]->catchmentArea = nullptr;
    configs[StructureType::WAY_W]->buildingCosts = { 5, 0, 0, 0 };
    
    configs[StructureType::WAY_SW_NE] = new BuildingConfig();
    configs[StructureType::WAY_SW_NE]->name = "Weg";
    configs[StructureType::WAY_SW_NE]->catchmentArea = nullptr;
    configs[StructureType::WAY_SW_NE]->buildingCosts = { 5, 0, 0, 0 };
    
    configs[StructureType::WAY_NW_SE] = new BuildingConfig();
    configs[StructureType::WAY_NW_SE]->name = "Weg";
    configs[StructureType::WAY_NW_SE]->catchmentArea = nullptr;
    configs[StructureType::WAY_NW_SE]->buildingCosts = { 5, 0, 0, 0 };
    
    configs[StructureType::CHAPEL] = new BuildingConfig();
    configs[StructureType::CHAPEL]->name = "Kapelle";
    configs[StructureType::CHAPEL]->catchmentArea = new RectangleData<char>(6, 6);
    memcpy(configs[StructureType::CHAPEL]->catchmentArea->data, "011110111111111111111111111111011110", 36);
    configs[StructureType::CHAPEL]->buildingCosts = { 100, 2, 5, 0 };
    
    configs[StructureType::WEAPONSMITH] = new BuildingConfig();
    configs[StructureType::WEAPONSMITH]->name = "Schmied";
    configs[StructureType::WEAPONSMITH]->catchmentArea = new RectangleData<char>(4, 4);
    memcpy(configs[StructureType::WEAPONSMITH]->catchmentArea->data, "1111111111111111", 16);
    configs[StructureType::WEAPONSMITH]->buildingCosts = { 150, 3, 2, 5 };
    
    configs[StructureType::SIGNALFIRE] = new BuildingConfig();
    configs[StructureType::SIGNALFIRE]->name = "Signalturm";
    configs[StructureType::SIGNALFIRE]->catchmentArea = new RectangleData<char>(5, 5);
    memcpy(configs[StructureType::SIGNALFIRE]->catchmentArea->data, "0010001110111110111000100", 25);
    configs[StructureType::SIGNALFIRE]->buildingCosts = { 50, 1, 7, 0 };
    
    configs[StructureType::HERBARY] = new BuildingConfig();
    configs[StructureType::HERBARY]->name = "Kräuterfeld";
    configs[StructureType::HERBARY]->catchmentArea = new RectangleData<char>(11, 11);
    memcpy(configs[StructureType::HERBARY]->catchmentArea->data, "0000111000000011111000011111111100111111111011111111111111111111111111111111101111111110011111111100001111100000001110000", 121);
    configs[StructureType::HERBARY]->buildingCosts = { 100, 0, 0, 0 };
    
    configs[StructureType::BRICKYARD] = new BuildingConfig();
    configs[StructureType::BRICKYARD]->name = "Steinbruch";
    configs[StructureType::BRICKYARD]->catchmentArea = new RectangleData<char>(10, 6);
    memcpy(configs[StructureType::BRICKYARD]->catchmentArea->data, "001111110001111111101111111111111111111101111111100011111100", 60);
    configs[StructureType::BRICKYARD]->buildingCosts = { 100, 5, 5, 0 };
    
    configs[StructureType::BRICKYARD2] = new BuildingConfig();
    configs[StructureType::BRICKYARD2]->name = "Steinbruch (gedreht)";
    configs[StructureType::BRICKYARD2]->catchmentArea = new RectangleData<char>(6, 10);
    memcpy(configs[StructureType::BRICKYARD2]->catchmentArea->data, "001100011110111111111111111111111111111111111111011110001100", 60);
    configs[StructureType::BRICKYARD2]->buildingCosts = { 100, 5, 5, 0 };
    
    configs[StructureType::OFFICE1] = new BuildingConfig();
    configs[StructureType::OFFICE1]->name = "Kontor";
    configs[StructureType::OFFICE1]->catchmentArea = new RectangleData<char>(38, 38);
    memcpy(configs[StructureType::OFFICE1]->catchmentArea->data, "0000000000000001111111100000000000000000000000000111111111111111100000000000000000000111111111111111111110000000000000000011111111111111111111110000000000000011111111111111111111111111000000000001111111111111111111111111111000000000111111111111111111111111111111000000001111111111111111111111111111110000000111111111111111111111111111111110000011111111111111111111111111111111110000111111111111111111111111111111111100011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111100011111111111111111111111111111111110000111111111111111111111111111111111100000111111111111111111111111111111110000000111111111111111111111111111111000000001111111111111111111111111111110000000001111111111111111111111111111000000000001111111111111111111111111100000000000000111111111111111111111100000000000000000111111111111111111110000000000000000000011111111111111110000000000000000000000000011111111000000000000000", 1444);
    configs[StructureType::OFFICE1]->buildingCosts = { 100, 3, 6, 0 };
    
    configs[StructureType::MARKETPLACE] = new BuildingConfig();
    configs[StructureType::MARKETPLACE]->name = "Marktplatz";
    configs[StructureType::MARKETPLACE]->catchmentArea = new RectangleData<char>(38, 38);
    memcpy(configs[StructureType::MARKETPLACE]->catchmentArea->data, "0000000000000001111111100000000000000000000000000111111111111111100000000000000000000111111111111111111110000000000000000011111111111111111111110000000000000011111111111111111111111111000000000001111111111111111111111111111000000000111111111111111111111111111111000000001111111111111111111111111111110000000111111111111111111111111111111110000011111111111111111111111111111111110000111111111111111111111111111111111100011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111111111100111111111111111111111111111111111111001111111111111111111111111111111111110011111111111111111111111111111111111100011111111111111111111111111111111110000111111111111111111111111111111111100000111111111111111111111111111111110000000111111111111111111111111111111000000001111111111111111111111111111110000000001111111111111111111111111111000000000001111111111111111111111111100000000000000111111111111111111111100000000000000000111111111111111111110000000000000000000011111111111111110000000000000000000000000011111111000000000000000", 1444);
    configs[StructureType::MARKETPLACE]->buildingCosts = { 200, 4, 10, 0 };
    
    configs[StructureType::FORESTERS] = new BuildingConfig();
    configs[StructureType::FORESTERS]->name = "Forsthaus";
    configs[StructureType::FORESTERS]->catchmentArea = new RectangleData<char>(8, 8);
    memcpy(configs[StructureType::FORESTERS]->catchmentArea->data, "0011110001111110111111111111111111111111111111110111111000111100", 64);
    configs[StructureType::FORESTERS]->buildingCosts = { 50, 2, 0, 0 };
}

BuildingConfigMgr::~BuildingConfigMgr() {
    for(int i = 0; i < StructureType::MAX_STRUCTURE; i++) {
        if (configs[i] != nullptr) {
            delete configs[i];
        }
    }
    delete[] configs;
}