#ifndef GUIBATTERY_H
#define GUIBATTERY_H

#pragma once

#include "Gui.h"
#include "Gui_Text.h"

#define BATTERY_GRADIENT    // Smooth gradient  - Green -> Red
#define BATTERY_STEPS       // 4 static colors  - Green -> Yellow -> Orange -> Red

#define BATTERY_CRITICAL    15
#define BATTERY_LOW         25
#define BATTERY_MEDIUM      50
#define BATTERY_GOOD        100

class Battery : public GUI_Element {
    uint16_t BorderColor = WHITE;

    void Draw(float percent){
        /* Percentage battery icon */

        // Center the battery
        int offset = w / 2;
        // Rectangle boarder - white
        M5Dial.Display.drawRect(x-offset, y, w, h, BorderColor);
        // Battery positive pin (left small pixel bar)
        M5Dial.Display.fillRect(x-offset-3, y+h/4, 3, h/2, BorderColor);
        // Rectangle filled  - color based on the percent range
        M5Dial.Display.fillRect(x-offset+2 + (w-4)*(1.0-percent), y+2, (w-4)*percent, h-4, color);
    }

    void Percent2Color(float percent){
        (percent * 100 < BATTERY_CRITICAL) ? BorderColor = RED : BorderColor = WHITE;

        #ifdef BATTERY_GRADIENT
            // Calculate red and green intensity based on percentage
            uint8_t red   = (255 * (1.0 - percent));  // Red decreases as percent increases
            uint8_t green = (255 * percent);          // Green increases as percent increases
            uint8_t blue  = 0;                        // Blue stays off
            SetColor(red, green, blue);
            return;
        #endif

        #ifdef BATTERY_STEPS
            if(percent * 100 < BATTERY_CRITICAL)
                SetColor(RED);
            else if(percent * 100 < BATTERY_LOW)
                SetColor(ORANGE);
            else if(percent * 100 < BATTERY_MEDIUM)
                SetColor(YELLOW);
            else
                SetColor(GREEN);
            return;
        #endif
    }

public:
    Battery(int x, int y, int w, int h) : GUI_Element(x, y, w, h) { }
    ~Battery() override { }

    void Clear(bool reset = false){
        int offset = w / 2;
        // Black Bar that will cover whole GUI element
        M5Dial.Display.fillRect(x-offset, y, w, h, bgcolor);
    }

    void Update(bool force_update = false) override {
        // Check if value has changed
        if (!hasChanged(force_update))
            return;

        // Get Value
        VariantType val = getCurrentValue();
        std::string str;

        // Get the percent
        float percent;
        if (std::holds_alternative<float>(val)) {
            percent = std::get<float>(val);
        }else if (std::holds_alternative<double>(val)) {
            percent = std::get<double>(val);
        } else{
            percent = NAN;
        }

        float converted = constrain(percent / 100.0f, 0.0f, 1.0f);

        // Do not draw is value is incorrect
        if(isnan(percent))
            return;

        Clear();
        Percent2Color(converted);
        Draw(converted);
    }
};

#endif //GUIBATTERY_H