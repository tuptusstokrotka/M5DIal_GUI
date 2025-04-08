#ifndef GUINUMBER_H
#define GUINUMBER_H

#pragma once

#include "Gui.h"
#include "Gui_Text.h"

class Number : public Text {
    uint8_t precision = 2;

    std::string ConvertToString(VariantType value) {
        return std::visit([this](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
                char buffer[32];
                snprintf(buffer, sizeof(buffer), "%.*f", precision, static_cast<double>(arg));
                return std::string(buffer);
            }
            else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int> || std::is_same_v<T, long>) {
                return std::to_string(arg);
            }
            else {
                return "[Invalid Type]";
            }
        }, value);
    }

public:
    Number(int x, int y, int w, const lgfx::GFXfont* font) : Text(x, y, w, font, false) {};
    ~Number() {}

    void Update(bool force_update = false) override {
        // Value not assigned
        if (!hasChanged(force_update))
            return;

        // Get Value
        VariantType num = getCurrentValue();
        std::string str;

        // Convert the value to a string
        str = ConvertToString(num);

        // Config text - needed to estimate font width
        M5Dial.Display.setFont(font);

        Clear();
        Draw(str);
    }

    void SetPrecision(uint8_t precision){ this->precision = precision; }
};


class Timer : public Text {
public:
    Timer(int x, int y, int w, const lgfx::GFXfont* font) : Text(x, y, w, font, false) {};
    ~Timer(){};

    std::string SecToStr(long time) {
        int hours   = (time / 3600);
        int minutes = (time % 3600) / 60;
        int seconds = (time % 60);

        static char timeString[9];  // Static buffer to hold time string (HH:MM:SS)

        if (hours > 0) {
            snprintf(timeString, sizeof(timeString), "%d:%02d:%02d", hours, minutes, seconds);
        } else {
            snprintf(timeString, sizeof(timeString), "%02d:%02d", minutes, seconds);
        }

        return timeString;
    }

    void Update(bool force_update = false) override {
        // Value not assigned
        if (!hasChanged(force_update))
            return;

        // Get Value
        VariantType time = getCurrentValue();
        std::string str;

        if (std::holds_alternative<long>(time)) {
            str = SecToStr(std::get<long>(time));
        } else if (std::holds_alternative<int>(time)) {
            str = SecToStr(std::get<int>(time));
        } else {
            str = "[Invalid Type]";  // Default message in case of invalid type
        }

        // Config text - needed to estimate font width
        M5Dial.Display.setFont(font);

        Clear();
        Draw(str);
    }
};

#endif // GUINUMBER_H