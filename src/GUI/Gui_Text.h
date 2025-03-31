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

    void Draw(String& str){
        String text = Trim(str);
        M5Dial.Display.drawString(text, x, y);
    }

    /**
     * @brief Simple scrolling text via text trimming
     * @todo Use text to sprite conversion, then use M5dial scroll functions
     */
    void DrawScrolling(String& str){
        if(!scrolling)
            return;

        // do NOT scroll if the text is within the textbox width
        if(M5Dial.Display.textWidth(str) <= w)
            return;

        // Check timer
        if(last_update + SCROLL_DELAY > millis())
            return;

        String text = str;
        int loop_offset = 0;

        // Update scrolling text
        last_update = millis();

        if(scroll_loop){
            String delimiter = SCROLL_DELIMITER;
            text = text + delimiter + str;
            loop_offset = delimiter.length();
        }

        ++scroll_index %= (str.length() + loop_offset);
        text = text.substring(scroll_index, text.length());

        Clear();
        Draw(text);
    }

    String Trim(String& str){
        // Trimming disabled
        if(!trimming)
            return str;

        // Short string - fits inside textbox
        if(M5Dial.Display.textWidth(str) <= w)
            return str;

        // Trimming
        String trimmed = str;
        while (M5Dial.Display.textWidth(trimmed) > w && trimmed.length() > 0) {
            trimmed.remove(trimmed.length() - 1);  // Remove last character if it's too wide
        }
        return trimmed;
    }

public:
    Text(int x, int y, int w, const lgfx::GFXfont* font, bool scrolling = false)
        : GUI_Element(x, y, w), font(font), scrolling(scrolling) {
            h = M5Dial.Display.fontHeight(font); // Set Textbox according to the font height - less pixels to clear
        };
    ~Text() override {}

    void Update(bool force_update = false) override {
        // Value not assigned
        if (!boundValue) return;

        // Get Value
        String str = *static_cast<String*>(boundValue);

        // Config text - needed to estimate font width
        M5Dial.Display.setFont(font);
        M5Dial.Display.setTextColor(color);

        // Text has not changed and force_update is false
        //TODO make the code clean, fix this mess
        if(S_LastValue == str && !force_update){
            DrawScrolling(str);
            return;
        }

        // Text has changed
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


// A text class, but adds 0 to seconds and minutes - fix 1:2 -> 1:02 (62 seconds)
class Timer : public Text {
public:
    Timer(int x, int y, int w, const lgfx::GFXfont* font) : Text(x, y, w, font, false) {};
    ~Timer(){};

    String SecToStr(long time) {
        int hours   = (time / 3600);
        int minutes = (time % 3600) / 60;
        int seconds = (time % 60);

        if (hours > 0) {
            return String(hours) + ":" +
                   (minutes < 10 ? "0" : "") + String(minutes) + ":" +
                   (seconds < 10 ? "0" : "") + String(seconds);
        } else {
            return String(minutes) + ":" +
                   (seconds < 10 ? "0" : "") + String(seconds);
        }
    }

    // That is quite ugly, fix later
    // issue with the type difference -> long(timer) =/= string(text)
    void Update(bool force_update = false) override {
        // Value not assigned
        if (!boundValue) return;

        // Get Value
        long time = *static_cast<long*>(boundValue);
        String timeString = SecToStr(time);

        // If values has not changed and force_update is false
        if(S_LastValue == (timeString) && !force_update)
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

#endif //GUITEXT_H