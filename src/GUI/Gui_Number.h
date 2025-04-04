#ifndef GUINUMBER_H
#define GUINUMBER_H

#pragma once

#include "Gui.h"
#include "Gui_Text.h"

// A template number class
template <typename T>
class Number : public Text {
    // A helper function to convert any number type to C-style string
    void ConvertToString(char* buffer) {
        if (std::is_integral<T>::value) {
            // If T is an integer (e.g., int, long, etc.)
            snprintf(buffer, 32, "%ld", *static_cast<T*>(this->value));  // Use long format to handle int/long types
        } else if (std::is_floating_point<T>::value) {
            // If T is a floating point (e.g., float, double)
            snprintf(buffer, 32, "%.2f", *static_cast<T*>(this->value));  // Format float with 2 decimal places
        }
    }

    bool ValueHasChanged(T value){
        if constexpr (std::is_same<T, int>::value) {
            if (lastValue.I_LastValue == value) return false;
            lastValue.I_LastValue = value;
        } else if constexpr (std::is_same<T, long>::value) {
            if (lastValue.L_LastValue == value) return false;
            lastValue.L_LastValue = value;
        } else if constexpr (std::is_same<T, float>::value) {
            if (lastValue.F_LastValue == value) return false;
            lastValue.F_LastValue = value;
        }
        return true;
    }

public:
    Number(int x, int y, int w, const lgfx::GFXfont* font) : Text(x, y, w, font, false) {
        if constexpr (std::is_same<T, int>::value) {
            lastValue.I_LastValue = 0;
        } else if constexpr (std::is_same<T, long>::value) {
            lastValue.L_LastValue = 0;
        } else if constexpr (std::is_same<T, float>::value) {
            lastValue.F_LastValue = 0.0f;
        }
    };
    ~Number() {}

    void Update(bool force_update = false) override {
        // Value not assigned
        if (this->value == nullptr) return;

        // Check if the value has changed
        if(!ValueHasChanged(*static_cast<T*>(this->value)) && !force_update)
            return;

        // Convert the value to a string
        char str[32];
        ConvertToString(str);

        // Config text - needed to estimate font width
        M5Dial.Display.setFont(font);
        M5Dial.Display.setTextColor(color);

        // Clear previous text and draw the new one
        Text::Clear();
        Text::Draw(str);
    }
};


class Timer : public Text {
public:
    Timer(int x, int y, int w, const lgfx::GFXfont* font) : Text(x, y, w, font, false) {};
    ~Timer(){};

    char* SecToStr(long time) {
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
        if (!this->value) return;

        // Get Value
        long time = *static_cast<long*>(this->value);
        char* timeString = SecToStr(time);

        // If values have not changed and force_update is false
        if(S_LastValue == timeString && !force_update)
            return;

        // Text has changed
        S_LastValue = timeString;

        // Config text - needed to estimate font width
        M5Dial.Display.setFont(font);
        M5Dial.Display.setTextColor(color);

        Text::Clear();
        Text::Draw(timeString);
    }
};

#endif // GUINUMBER_H