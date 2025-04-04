#include "Gui.h"

GUI_Element::GUI_Element(int x, int y, int w, int h)
    : x(x), y(y), w(w), h(h) { }

GUI_Element::~GUI_Element() {
    if (this->value != nullptr) {
        delete[] static_cast<char*>(this->value);
    }
}

void GUI_Element::Clear(bool reset){
    // Black Bar that will cover whole GUI element
    M5Dial.Display.fillRect(x-(w/2), y-(h/2), w, h, bgcolor);

    if(!reset)
        return;

    // Clear Last values
    if (S_LastValue != nullptr) {
        delete[] S_LastValue;
        S_LastValue = nullptr;
    }
    // Default reset for union
    lastValue.I_LastValue = 0;
}

void GUI_Element::SetX(int x){ this->x = x; }
void GUI_Element::SetY(int y){ this->y = y; }
void GUI_Element::SetXY(int x, int y){ SetX(x); SetY(y); }

void GUI_Element::SetW(int w){ this->w = w; }
void GUI_Element::SetH(int h){ this->h = h; }

void GUI_Element::SetColor(const uint16_t color){ this->color = color; }
void GUI_Element::SetColor(uint8_t r, uint8_t g, uint8_t b){ this->color = M5Dial.Display.color565(r,g,b); }

void GUI_Element::SetBgColor(const uint16_t color){ this->bgcolor = color; }
void GUI_Element::SetBgColor(uint8_t r, uint8_t g, uint8_t b){ this->bgcolor = M5Dial.Display.color565(r,g,b); }
