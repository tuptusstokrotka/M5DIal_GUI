#ifndef GUIELEMENTS_H
#define GUIELEMENTS_H

#pragma once

#include <Arduino.h>
#include "M5Dial.h"

class GUI_Element {
protected:
    uint8_t x, y, w, h;             // Element position and size
    uint16_t color      = WHITE;    // Element color
    uint16_t bgcolor    = BLACK;    // Background color
    void* value         = nullptr;  // Pointer to external value
    bool isBound        = false;    // Track if is value has been bound

    union { // Stores last value for comparison (saves memory)
        int   I_LastValue;
        long  L_LastValue;
        float F_LastValue;
    } lastValue;
    char* S_LastValue = nullptr;    // Use char* for the last value

    void Clear(bool reset = false);

public:
    GUI_Element(int x = 0, int y = 0, int w = 0, int h = 0);
    virtual ~GUI_Element();

    virtual void Update(bool force_update = false) = 0;  // Pure virtual function

    template <typename T>
    void Bind(T& value) {
        // If value is of type char* and previously bound, handle it by freeing the memory
        if (this->value != nullptr && std::is_same<T, char*>::value) {
            delete[] static_cast<char*>(this->value);  // Only free memory for char* type
        }

        // Bind the reference to the value
        this->value = static_cast<void*>(&value);
    }

    template <typename T>
    void SetValue(T value) {
        if (this->value == nullptr) {
            this->value = static_cast<void*>(new T(value));     // Dynamically allocate for any type
        } else {
            *static_cast<T*>(this->value) = value;              // Update the existing value
        }
        Update(true);
    }

    void SetValue(const char* value) {
        // Allocate memory for the char array [120 chars]
        if (this->value == nullptr) {
            this->value = static_cast<void*>(new char[120]);
        }
        // Update the string value
        strcpy(static_cast<char*>(this->value), value);
        Update(true);  // Force an update when setting a string
    }

    void SetX(int x);
    void SetY(int y);
    void SetXY(int x, int y);

    void SetW(int w);
    void SetH(int h);

    void SetColor(const uint16_t color);
    void SetColor(uint8_t r, uint8_t g, uint8_t b);

    void SetBgColor(const uint16_t color);
    void SetBgColor(uint8_t r, uint8_t g, uint8_t b);
};

#endif