#ifndef GUIELEMENTS_H
#define GUIELEMENTS_H

#pragma once

#include "M5Dial.h"

#include <functional>
#include <variant>
#include <string>

class GUI_Element {
public:
    using VariantType = std::variant<std::monostate, int, unsigned int, long, float, double, std::string>;

protected:
    uint8_t x, y, w, h;             // Element position and size
    uint16_t color      = WHITE;    // Element color
    uint16_t bgcolor    = BLACK;    // Background color

    bool isBound = false;           // true     → external variable
                                    // false    → local value

    std::function<VariantType()> getter;  // Bind     → dynamic getter
    VariantType ownedValue;               // SetValue → stored local value
    VariantType lastValue;                // Last Value for comparison

    VariantType getCurrentValue() const;
    bool hasChanged(bool force);
    void Clear(bool reset = false);

public:
    GUI_Element(int x = 0, int y = 0, int w = 0, int h = 0);
    virtual ~GUI_Element() = default;

    template<typename T>
    void Bind(T& ext_var) {
        getter = [&ext_var]() -> VariantType {      // Lambda function
            return VariantType(ext_var);
        };
        isBound = true;
        ownedValue = std::monostate{};              // Clear the ownedValue
    }

    template<typename T>
    void SetValue(const T& val) {
        ownedValue = VariantType(val);
        isBound = false;

        // Push new value to the screen
        Update(true);
    }

    virtual void Update(bool force_update = false) = 0;

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