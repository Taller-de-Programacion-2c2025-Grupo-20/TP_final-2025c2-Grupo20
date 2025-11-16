#pragma once
#include <map>
#include <array>
#include <SDL2pp/Rect.hh>
#include "../common/constants.h"

class SpritesManager {
public:
    SpritesManager();
    SpritesManager(const SpritesManager&) = delete;
    SpritesManager& operator=(const SpritesManager&) = delete;
    ~SpritesManager() = default;

    // Autos
    const SDL2pp::Rect& getCarRect(CarType carType, int frameId) const;

    // HUD: dígitos 0–9
    const SDL2pp::Rect& getDigitRect(int d) const;

    // HUD: paneles
    const SDL2pp::Rect& getHealthPanelRect() const;
    const SDL2pp::Rect& getTimePanelRect() const;
    const SDL2pp::Rect& getSpeedPanelRect() const;

    // HUD: corazones
    const SDL2pp::Rect& getEmptyHeartRect() const;
    const SDL2pp::Rect& getFullHeartRect() const;
    const SDL2pp::Rect& getHeartSlotRect(int idx) const;

    // HUD: posiciones de dígitos dentro de cada panel
    const SDL2pp::Rect& getHealthDigitRectHundreds() const;
    const SDL2pp::Rect& getHealthDigitRectTens() const;
    const SDL2pp::Rect& getHealthDigitRectUnits() const;

    const SDL2pp::Rect& getTimeDigitRectMmTens() const;
    const SDL2pp::Rect& getTimeDigitRectMmUnits() const;
    const SDL2pp::Rect& getTimeDigitRectSsTens() const;
    const SDL2pp::Rect& getTimeDigitRectSsUnits() const;

    const SDL2pp::Rect& getSpeedDigitRectHundreds() const;
    const SDL2pp::Rect& getSpeedDigitRectTens() const;
    const SDL2pp::Rect& getSpeedDigitRectUnits() const;

private:
    // Autos
    static const std::map<int, SDL2pp::Rect> carPositionsGreen;
    static const std::map<int, SDL2pp::Rect> carPositionsRed;
    static const std::map<int, SDL2pp::Rect> carPositionsDescapotable;
    static const std::map<int, SDL2pp::Rect> carPositionsCeleste;
    static const std::map<int, SDL2pp::Rect> carPositionsJeep;
    static const std::map<int, SDL2pp::Rect> carPositionsCamioneta;
    static const std::map<int, SDL2pp::Rect> carPositionsCamion;

    // HUD: dígitos
    static const std::array<SDL2pp::Rect, 10> DIGITS;

    // HUD: paneles
    static const SDL2pp::Rect PANEL_HP;
    static const SDL2pp::Rect PANEL_TIME;
    static const SDL2pp::Rect PANEL_SPEED;

    // HUD: slots de dígitos dentro de panel de HP
    static const SDL2pp::Rect HEALTH_HUNDREDS;
    static const SDL2pp::Rect HEALTH_TENS;
    static const SDL2pp::Rect HEALTH_UNITS;

    // HUD: corazones
    static const std::array<SDL2pp::Rect, 5> HEART_SLOTS;
    static const SDL2pp::Rect EMPTY_HEART;
    static const SDL2pp::Rect FULL_HEART;

    // HUD: slots de dígitos para tiempo
    static const SDL2pp::Rect TIME_MM_TENS;
    static const SDL2pp::Rect TIME_MM_UNITS;
    static const SDL2pp::Rect TIME_SS_TENS;
    static const SDL2pp::Rect TIME_SS_UNITS;

    // HUD: slots de dígitos para velocidad
    static const SDL2pp::Rect SPEED_HUNDREDS;
    static const SDL2pp::Rect SPEED_TENS;
    static const SDL2pp::Rect SPEED_UNITS;
};
