#ifndef GUIELEMENTS_H
#define GUIELEMENTS_H

#pragma once

#include <Arduino.h>
// lib_deps = m5stack/M5Dial@=1.0.2
#include "M5Dial.h"

class GUI_Element {
protected:
    uint8_t x, y, w, h;             // Element position and size
    uint16_t color      = WHITE;    // Element color
    uint16_t bgcolor    = BLACK;    // Background color
    void* boundValue    = nullptr;  // Pointer to external value

    union { // Stores last value for comparison (saves memory)
        int   I_LastValue;
        long  L_LastValue;
        float F_LastValue;
    } lastValue;
    String  S_LastValue = "";

    void Clear(bool reset = false);

public:
    GUI_Element(int x = 0, int y = 0, int w = 0, int h = 0);
    virtual ~GUI_Element();

    virtual void Update(bool force_update = false) = 0;  // Pure virtual function

    template <typename T>
    void Bind(T& value) {
        boundValue = static_cast<void*>(&value);
    }

    // template <typename T>
    // void SetValue(T value) {
    //     if(boundValue == nullptr){
    //         boundValue = new T;
    //         boundValue = static_cast<void*>(value);
    //     }
    // }

    void SetX(int x);
    void SetY(int y);
    void SetXY(int x, int y);

    void SetW(int w);
    void SetH(int h);

    void SetColor(const uint16_t color);
    void SetColor(uint8_t r, uint8_t g, uint8_t b);

    void SetBgColor(const uint16_t color);
    void SetBgColor(uint8_t r, uint8_t g, uint8_t b);

    void SetValue(int value);
    void SetValue(long value);
    void SetValue(float value);
    void SetValue(String value);
};

#endif