#ifndef GUITEXT_H
#define GUITEXT_H

#pragma once

#include "Gui.h"
#include <lgfx/v1/lgfx_fonts.hpp>

#define SCROLL_DELAY 500            // Delay between scrolling each letter (ms)
#define SCROLL_DELIMITER "      "   // Scrolling delimiter between the end and beginning of a string

class Text : public GUI_Element {
protected:
    /* Font */
    const lgfx::v1::GFXfont* font;  // Store font as a pointer
    /* Trimming */
    bool trimming = true;           // Trim text out of the textbox
    /* Scrolling */
    bool scrolling = false;         // Enable scrolling
    bool scroll_loop = true;        // Add delimiter and seamless scrolling in loop
    unsigned int scroll_index = 0;  // Current 1st scrolling index
    unsigned long last_update = 0;  // Last update milliseconds

    void Draw(const std::string& str) {
        M5Dial.Display.setTextColor(color);
        M5Dial.Display.drawString(str.c_str(), x, y);
    }

    bool CheckScrolling(void){
        // Scrolling not set
        if(!scrolling)
            return false;

        // Timer has passed
        if(pdTICKS_TO_MS(xTaskGetTickCount() - last_update) < SCROLL_DELAY)
            return false;

        last_update = xTaskGetTickCount();
        return true;
    }
    std::string GetScrolling(const std::string& str){
        // do NOT scroll if the text is within the textbox width
        if(M5Dial.Display.textWidth(str.c_str()) <= w)
            return str;

        std::string scrolled = str;

        // Scrolling in loop with delimiter?
        int loop_offset = 0; // Add delimiter length to the scrolling width
        if(scroll_loop){
            scrolled = scrolled + SCROLL_DELIMITER + scrolled;
            loop_offset = strlen(SCROLL_DELIMITER);
        }

        ++scroll_index %= (strlen(str.c_str()) + loop_offset);
        scrolled.erase(scrolled.begin(), scrolled.begin()+scroll_index);

        return scrolled;
    }
    std::string GetTrimmed(const std::string& str){
        // string width is within the textbox - do NOT trim
        if(M5Dial.Display.textWidth(str.c_str()) <= w)
            return str;

        // Trimming to fit the box
        std::string trimmed = str;
        while (M5Dial.Display.textWidth(trimmed.c_str()) > w && !trimmed.empty()) {
            trimmed.pop_back();  // Remove last character if it's too wide
        }
        return trimmed;
    }

public:
    Text(int x, int y, int w, const lgfx::GFXfont* font, bool scrolling = false)
    : GUI_Element(x, y, w), font(font), scrolling(scrolling) {
        h = M5Dial.Display.fontHeight(font); // Set Textbox height according to the font height - less pixels to clear
    };
    ~Text() {};

    void Update(bool force_update = false) override {
        // Check if value has changed - Scrolling is not triggered
        if (!hasChanged(force_update) && !CheckScrolling())
            return;

        // Get value & save last value
        VariantType val = getCurrentValue();
        std::string str;

        if (std::holds_alternative<std::string>(val)) {
            str = std::get<std::string>(val);
        } else {
            str = "[Invalid Type]";
        }

        // Config text font - needed to estimate font width
        M5Dial.Display.setFont(font);

        if(scrolling){
            str = GetScrolling(str);
        }

        if(trimming)
            str = GetTrimmed(str);

        Clear();
        Draw(str);
    }

    void SetFont(const lgfx::GFXfont* font){
        this->font = font;
        h = M5Dial.Display.fontHeight(font); // Set Textbox according to the font height - less pixels to clear
    }

    void SetScrolling(bool scrolling, bool loop){
        this->scrolling = scrolling;
        this->scroll_loop = loop;
    }

    void SetTrimming(bool trimming){ this->trimming = trimming; }

};

#endif //GUITEXT_H