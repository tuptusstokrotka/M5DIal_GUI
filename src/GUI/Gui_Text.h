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
    bool scroll_loop = true;        // Adding the string at the end of scrolling
    unsigned int scroll_index = 0;  // Current 1st scrolling index
    unsigned long last_update = 0;  // Last update milliseconds

    void Draw(char* str){
        char* text = Trim(str);
        M5Dial.Display.drawString(text, x, y);
    }

    /**
     * @brief Simple scrolling text via text trimming
     * @todo Use text to sprite conversion, then use M5dial scroll functions
     */
    void DrawScrolling(char* str){
        if(!scrolling)
            return;

        // do NOT scroll if the text is within the textbox width
        if(M5Dial.Display.textWidth(str) <= w)
            return;

        // Check timer
        if(last_update + SCROLL_DELAY > millis())
            return;

        char* text = strdup(str);  // Copy the string for manipulation
        int loop_offset = 0;

        // Update scrolling text
        last_update = millis();

        if(scroll_loop){
            text = strcat(text, SCROLL_DELIMITER);
            loop_offset = strlen(SCROLL_DELIMITER);
        }

        ++scroll_index %= (strlen(str) + loop_offset);
        text = text + scroll_index;

        Clear();
        Draw(text);
    }

    char* Trim(char* str){
        // Trimming disabled
        if(!trimming)
            return str;

        // Short string - fits inside textbox
        if(M5Dial.Display.textWidth(str) <= w)
            return str;

        // Trimming
        char* trimmed = strdup(str);  // Copy string to avoid modifying the original
        while (M5Dial.Display.textWidth(trimmed) > w && strlen(trimmed) > 0) {
            trimmed[strlen(trimmed) - 1] = '\0';  // Remove last character if it's too wide
        }
        return trimmed;  // Returning the trimmed string
    }

public:
    Text(int x, int y, int w, const lgfx::GFXfont* font, bool scrolling = false)
        : GUI_Element(x, y, w), font(font), scrolling(scrolling) {
            h = M5Dial.Display.fontHeight(font); // Set Textbox according to the font height - less pixels to clear
        };
    ~Text() override {}

    void Update(bool force_update = false) override {
        // Value not assigned
        if (this->value == nullptr) return;

        // Get Value
        char* str = static_cast<char*>(this->value);

        // Config text - needed to estimate font width
        M5Dial.Display.setFont(font);
        M5Dial.Display.setTextColor(color);

        // Text has not changed and force_update is false
        if(S_LastValue == str && !force_update){
            DrawScrolling(str);
            return;
        }

        // Text has changed or force_update is true
        S_LastValue = str;
        scroll_index = 0;   // Reset the scroll index

        Clear();
        Draw(str);
    };

    void SetFont(const lgfx::GFXfont* font){
        this->font = font;
        h = M5Dial.Display.fontHeight(font); // Set Textbox according to the font height - less pixels to clear
    }

    void SetScrolling(bool scrolling, bool loop){
        this->scrolling = scrolling;
        this->scroll_loop = loop;
    }

    //TODO fix clearing the text if trimming is OFF - residues still on the screen
    //Dynamically get the text width, and assign new value to clear?
    void SetTrimming(bool trimming){ this->trimming = trimming; }
};

#endif //GUITEXT_H