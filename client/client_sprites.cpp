#include "client_sprites.h"


const std::map<int, SDL2pp::Rect> SpritesManager::carPositionsGreen = {
    {0, SDL2pp::Rect(0, 0, 32, 32)}, {1, SDL2pp::Rect(32, 0, 32, 32)}, {2, SDL2pp::Rect(64, 0, 32, 32)}, {3, SDL2pp::Rect(96, 0, 32, 32)},
    {4, SDL2pp::Rect(128, 0, 32, 32)}, {5, SDL2pp::Rect(160, 0, 32, 32)}, {6, SDL2pp::Rect(192, 0, 32, 32)}, {7, SDL2pp::Rect(224, 0, 32, 32)},
    {8, SDL2pp::Rect(0, 32, 32, 32)}, {9, SDL2pp::Rect(32, 32, 32, 32)}, {10, SDL2pp::Rect(64, 32, 32, 32)}, {11, SDL2pp::Rect(96, 32, 32, 32)},
    {12, SDL2pp::Rect(128, 32, 32, 32)}, {13, SDL2pp::Rect(160, 32, 32, 32)}, {14, SDL2pp::Rect(192, 32, 32, 32)}, {15, SDL2pp::Rect(224, 32, 32, 32)}
};
const std::map<int, SDL2pp::Rect> SpritesManager::carPositionsRed = {
    {0, SDL2pp::Rect(0, 64, 40, 40)}, {1, SDL2pp::Rect(40, 64, 40, 40)}, {2, SDL2pp::Rect(80, 64, 40, 40)}, {3, SDL2pp::Rect(120, 64, 40, 40)},
    {4, SDL2pp::Rect(160, 64, 40, 40)}, {5, SDL2pp::Rect(200, 64, 40, 40)}, {6, SDL2pp::Rect(240, 64, 40, 40)}, {7, SDL2pp::Rect(280, 64, 40, 40)},
    {8, SDL2pp::Rect(0, 104, 40, 40)}, {9, SDL2pp::Rect(40, 104, 40, 40)}, {10, SDL2pp::Rect(80, 104, 40, 40)}, {11, SDL2pp::Rect(120, 104, 40, 40)},
    {12, SDL2pp::Rect(160, 104, 40, 40)}, {13, SDL2pp::Rect(200, 104, 40, 40)}, {14, SDL2pp::Rect(240, 104, 40, 40)}, {15, SDL2pp::Rect(280, 104, 40, 40)}
};
const std::map<int, SDL2pp::Rect> SpritesManager::carPositionsDescapotable = {
    {0, SDL2pp::Rect(0, 144, 40, 40)}, {1, SDL2pp::Rect(40, 144, 40, 40)}, {2, SDL2pp::Rect(80, 144, 40, 40)}, {3, SDL2pp::Rect(120, 144, 40, 40)},
    {4, SDL2pp::Rect(160, 144, 40, 40)}, {5, SDL2pp::Rect(200, 144, 40, 40)}, {6, SDL2pp::Rect(240, 144, 40, 40)}, {7, SDL2pp::Rect(280, 144, 40, 40)},
    {8, SDL2pp::Rect(0, 184, 40, 40)}, {9, SDL2pp::Rect(40, 184, 40, 40)}, {10, SDL2pp::Rect(80, 184, 40, 40)}, {11, SDL2pp::Rect(120, 184, 40, 40)},
    {12, SDL2pp::Rect(160, 184, 40, 40)}, {13, SDL2pp::Rect(200, 184, 40, 40)}, {14, SDL2pp::Rect(240, 184, 40, 40)}, {15, SDL2pp::Rect(280, 184, 40, 40)}
};
const std::map<int, SDL2pp::Rect> SpritesManager::carPositionsCeleste = {
    {0, SDL2pp::Rect(0, 224, 40, 40)}, {1, SDL2pp::Rect(40, 224, 40, 40)}, {2, SDL2pp::Rect(80, 224, 40, 40)}, {3, SDL2pp::Rect(120, 224, 40, 40)},
    {4, SDL2pp::Rect(160, 224, 40, 40)}, {5, SDL2pp::Rect(200, 224, 40, 40)}, {6, SDL2pp::Rect(240, 224, 40, 40)}, {7, SDL2pp::Rect(280, 224, 40, 40)},
    {8, SDL2pp::Rect(0, 264, 40, 40)}, {9, SDL2pp::Rect(40, 264, 40, 40)}, {10, SDL2pp::Rect(80, 264, 40, 40)}, {11, SDL2pp::Rect(120, 264, 40, 40)},
    {12, SDL2pp::Rect(160, 264, 40, 40)}, {13, SDL2pp::Rect(200, 264, 40, 40)}, {14, SDL2pp::Rect(240, 264, 40, 40)}, {15, SDL2pp::Rect(280, 264, 40, 40)}
};
const std::map<int, SDL2pp::Rect> SpritesManager::carPositionsJeep = {
    {0, SDL2pp::Rect(0, 304, 40, 40)}, {1, SDL2pp::Rect(40, 304, 40, 40)}, {2, SDL2pp::Rect(80, 304, 40, 40)}, {3, SDL2pp::Rect(120, 304, 40, 40)},
    {4, SDL2pp::Rect(160, 304, 40, 40)}, {5, SDL2pp::Rect(200, 304, 40, 40)}, {6, SDL2pp::Rect(240, 304, 40, 40)}, {7, SDL2pp::Rect(280, 304, 40, 40)},
    {8, SDL2pp::Rect(0, 344, 40, 40)}, {9, SDL2pp::Rect(40, 344, 40, 40)}, {10, SDL2pp::Rect(80, 344, 40, 40)}, {11, SDL2pp::Rect(120, 344, 40, 40)},
    {12, SDL2pp::Rect(160, 344, 40, 40)}, {13, SDL2pp::Rect(200, 344, 40, 40)}, {14, SDL2pp::Rect(240, 344, 40, 40)}, {15, SDL2pp::Rect(280, 344, 40, 40)}
};
const std::map<int, SDL2pp::Rect> SpritesManager::carPositionsCamioneta = {
    {0, SDL2pp::Rect(0, 384, 40, 40)}, {1, SDL2pp::Rect(40, 384, 40, 40)}, {2, SDL2pp::Rect(80, 384, 40, 40)}, {3, SDL2pp::Rect(120, 384, 40, 40)},
    {4, SDL2pp::Rect(160, 384, 40, 40)}, {5, SDL2pp::Rect(200, 384, 40, 40)}, {6, SDL2pp::Rect(240, 384, 40, 40)}, {7, SDL2pp::Rect(280, 384, 40, 40)},
    {8, SDL2pp::Rect(0, 424, 40, 40)}, {9, SDL2pp::Rect(40, 424, 40, 40)}, {10, SDL2pp::Rect(80, 424, 40, 40)}, {11, SDL2pp::Rect(120, 424, 40, 40)},
    {12, SDL2pp::Rect(160, 424, 40, 40)}, {13, SDL2pp::Rect(200, 424, 40, 40)}, {14, SDL2pp::Rect(240, 424, 40, 40)}, {15, SDL2pp::Rect(280, 424, 40, 40)}
};
const std::map<int, SDL2pp::Rect> SpritesManager::carPositionsCamion = {
    {0, SDL2pp::Rect(0, 464, 48, 48)}, {1, SDL2pp::Rect(48, 464, 48, 48)}, {2, SDL2pp::Rect(96, 464, 48, 48)}, {3, SDL2pp::Rect(144, 464, 48, 48)},
    {4, SDL2pp::Rect(192, 464, 48, 48)}, {5, SDL2pp::Rect(240, 464, 48, 48)}, {6, SDL2pp::Rect(288, 464, 48, 48)}, {7, SDL2pp::Rect(336, 464, 48, 48)},
    {8, SDL2pp::Rect(0, 512, 48, 48)}, {9, SDL2pp::Rect(48, 512, 48, 48)}, {10, SDL2pp::Rect(96, 512, 48, 48)}, {11, SDL2pp::Rect(144, 512, 48, 48)},
    {12, SDL2pp::Rect(192, 512, 48, 48)}, {13, SDL2pp::Rect(240, 512, 48, 48)}, {14, SDL2pp::Rect(288, 512, 48, 48)}, {15, SDL2pp::Rect(336, 512, 48, 48)}
};


// HUD: dígitos 0–9
const std::array<SDL2pp::Rect, 10> SpritesManager::DIGITS = {{
    SDL2pp::Rect(973, 358, 82, 100),   // 0
    SDL2pp::Rect(1053, 358, 82, 100),  // 1
    SDL2pp::Rect(1134, 358, 82, 100),  // 2
    SDL2pp::Rect(1219, 358, 82, 100),  // 3
    SDL2pp::Rect(1304, 358, 82, 100),  // 4
    SDL2pp::Rect(1385, 358, 82, 100),  // 5
    SDL2pp::Rect(1466, 358, 82, 100),  // 6
    SDL2pp::Rect(1548, 358, 82, 100),  // 7
    SDL2pp::Rect(1630, 358, 82, 100),  // 8
    SDL2pp::Rect(1712, 358, 82, 100)   // 9
}};

// HUD: paneles
const SDL2pp::Rect SpritesManager::PANEL_HP    = SDL2pp::Rect(23,  18, 847, 239);
const SDL2pp::Rect SpritesManager::PANEL_TIME  = SDL2pp::Rect(23, 295, 847, 239);
const SDL2pp::Rect SpritesManager::PANEL_SPEED = SDL2pp::Rect(23, 555, 847, 239);

// HUD: slots de dígitos HP
const SDL2pp::Rect SpritesManager::HEALTH_HUNDREDS = SDL2pp::Rect(387, 85, 82, 100);
const SDL2pp::Rect SpritesManager::HEALTH_TENS     = SDL2pp::Rect(478, 85, 82, 100);
const SDL2pp::Rect SpritesManager::HEALTH_UNITS    = SDL2pp::Rect(569, 85, 82, 100);

// HUD: corazones
const std::array<SDL2pp::Rect, 5> SpritesManager::HEART_SLOTS = {{
    SDL2pp::Rect( 60,  58, 80, 74),
    SDL2pp::Rect(154,  58, 80, 74),
    SDL2pp::Rect(248,  58, 80, 74),
    SDL2pp::Rect(109, 148, 80, 74),
    SDL2pp::Rect(200, 148, 80, 74),
}};
const SDL2pp::Rect SpritesManager::EMPTY_HEART = SDL2pp::Rect(972, 260, 80, 74);
const SDL2pp::Rect SpritesManager::FULL_HEART  = SDL2pp::Rect(1085,260, 80, 74);

// HUD: slots tiempo
const SDL2pp::Rect SpritesManager::TIME_MM_TENS   = SDL2pp::Rect(432, 358, 82, 100);
const SDL2pp::Rect SpritesManager::TIME_MM_UNITS  = SDL2pp::Rect(527, 358, 82, 100);
const SDL2pp::Rect SpritesManager::TIME_SS_TENS   = SDL2pp::Rect(651, 358, 82, 100);
const SDL2pp::Rect SpritesManager::TIME_SS_UNITS  = SDL2pp::Rect(742, 358, 82, 100);

// HUD: slots velocidad
const SDL2pp::Rect SpritesManager::SPEED_HUNDREDS = SDL2pp::Rect(342, 619, 82, 100);
const SDL2pp::Rect SpritesManager::SPEED_TENS     = SDL2pp::Rect(434, 619, 82, 100);
const SDL2pp::Rect SpritesManager::SPEED_UNITS    = SDL2pp::Rect(525, 619, 82, 100);
#include "client_sprites.h"

// (todas tus definiciones de mapas y const Rects arriba, esas están bien)

// =======================
//  Métodos de la clase
// =======================

const SDL2pp::Rect& SpritesManager::getCarRect(CarType carType, int frameId) const {
    switch (carType) {
    case CarType::VERDE:
        return carPositionsGreen.at(frameId);
    case CarType::ROJO:
        return carPositionsRed.at(frameId);
    case CarType::DESCAPOTABLE:
        return carPositionsDescapotable.at(frameId);
    case CarType::CELESTE:
        return carPositionsCeleste.at(frameId);
    case CarType::JEEP:
        return carPositionsJeep.at(frameId);
    case CarType::CAMIONETA:
        return carPositionsCamioneta.at(frameId);
    case CarType::CAMION:
        return carPositionsCamion.at(frameId);
    default:
        throw std::out_of_range("CarType inválido en getCarRect");
    }
}

// HUD: dígitos 0–9
const SDL2pp::Rect& SpritesManager::getDigitRect(int d) const {
    return DIGITS[d % 10];
}

// Paneles
const SDL2pp::Rect& SpritesManager::getHealthPanelRect() const { return PANEL_HP; }
const SDL2pp::Rect& SpritesManager::getTimePanelRect()   const { return PANEL_TIME; }
const SDL2pp::Rect& SpritesManager::getSpeedPanelRect()  const { return PANEL_SPEED; }

// Corazones
const SDL2pp::Rect& SpritesManager::getEmptyHeartRect() const { return EMPTY_HEART; }
const SDL2pp::Rect& SpritesManager::getFullHeartRect()  const { return FULL_HEART; }
const SDL2pp::Rect& SpritesManager::getHeartSlotRect(int idx) const {
    return HEART_SLOTS.at(idx);
}

// HP digits
const SDL2pp::Rect& SpritesManager::getHealthDigitRectHundreds() const { return HEALTH_HUNDREDS; }
const SDL2pp::Rect& SpritesManager::getHealthDigitRectTens()     const { return HEALTH_TENS; }
const SDL2pp::Rect& SpritesManager::getHealthDigitRectUnits()    const { return HEALTH_UNITS; }

// Time digits
const SDL2pp::Rect& SpritesManager::getTimeDigitRectMmTens()   const { return TIME_MM_TENS; }
const SDL2pp::Rect& SpritesManager::getTimeDigitRectMmUnits()  const { return TIME_MM_UNITS; }
const SDL2pp::Rect& SpritesManager::getTimeDigitRectSsTens()   const { return TIME_SS_TENS; }
const SDL2pp::Rect& SpritesManager::getTimeDigitRectSsUnits()  const { return TIME_SS_UNITS; }

// Speed digits
const SDL2pp::Rect& SpritesManager::getSpeedDigitRectHundreds() const { return SPEED_HUNDREDS; }
const SDL2pp::Rect& SpritesManager::getSpeedDigitRectTens()     const { return SPEED_TENS; }
const SDL2pp::Rect& SpritesManager::getSpeedDigitRectUnits()    const { return SPEED_UNITS; }

SpritesManager::SpritesManager() {}
