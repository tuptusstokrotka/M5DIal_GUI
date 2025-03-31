#ifndef GUIBAR_H
#define GUIBAR_H

#pragma once

#include "Gui.h"

#define MID_RADIUS (r + (R - r) / 2)  // More precise middle radius
#define DotX(angle) (x + cos(radians(angle)) * MID_RADIUS)
#define DotY(angle) (y + sin(radians(angle)) * MID_RADIUS)

#define LINEAR   0
#define SYMETRIC 1

class Bar : public GUI_Element {
    unsigned int r;         // Inner radius
    unsigned int R;         // Outer radius
    unsigned int thickness; // Bar width
    int startAngle;
    int sweepAngle;

    bool mode             = LINEAR;
    uint16_t second_color = WHITE;     // Second Bar color (MODE_SYMETRIC must be enabled)

    void EnableMode(bool mode){ this->mode = mode; }

    void DrawWhiteDot(float& percent){
        int endAngle = startAngle + (int)(sweepAngle * percent);
        M5Dial.Display.fillCircle(DotX(endAngle), DotY(endAngle), (thickness / 2) + 1, WHITE);
    }

    void DrawLinear(float& percent){
        int endAngle = startAngle + (int)(sweepAngle * percent);
        /* draw colored bar and start dot */
        M5Dial.Display.fillArc(x, y, r, R, startAngle, endAngle, color);
        M5Dial.Display.fillCircle(DotX(startAngle), DotY(startAngle), (thickness / 2) + 1, color);

        /* draw white progress dot */
        DrawWhiteDot(percent);
    }
    void ClearLinear(float& percent){
        int endAngle = startAngle + (int)(sweepAngle * percent);

        /* Black bars outside the bar to clear White Dot - Dummy but works */
        M5Dial.Display.fillArc(x, y, R+1, R+2, startAngle, startAngle+sweepAngle, bgcolor);
        M5Dial.Display.fillArc(x, y, r-1, r-2, startAngle, startAngle+sweepAngle, bgcolor);

        /* draw gray bar from progress till the end +/-1 to clear the orange residue */
        M5Dial.Display.fillArc(x, y, r, R, endAngle, startAngle + sweepAngle, bgcolor);
        /* draw gray end dot */
        M5Dial.Display.fillCircle(DotX(startAngle + sweepAngle), DotY(startAngle + sweepAngle), (thickness / 2) + 1, bgcolor);
    }

    void DrawSymetric(float& percent){
        int midAngle = startAngle + (int)(sweepAngle * 0.5);
        int endAngle = startAngle + (int)(sweepAngle * percent);

        if(percent <= 0.5){
            M5Dial.Display.fillArc(x, y, r, R, endAngle, midAngle, second_color);
        }
        else{
            M5Dial.Display.fillArc(x, y, r, R, midAngle, endAngle, color);
        }

        /* draw white progress dot */
        DrawWhiteDot(percent);
    }
    void ClearSymetric(float& percent){
        int midAngle = startAngle + (int)(sweepAngle * 0.5);
        int endAngle = startAngle + (int)(sweepAngle * percent);

        /* Black bars outside the bar to clear White Dot - Dummy but works */
        M5Dial.Display.fillArc(x, y, R+1, R+2, startAngle, startAngle+sweepAngle, bgcolor);
        M5Dial.Display.fillArc(x, y, r-1, r-2, startAngle, startAngle+sweepAngle, bgcolor);

        if(percent <= 0.5){
            /* draw gray bar from start till the progress */
            M5Dial.Display.fillArc(x, y, r, R, startAngle, endAngle,                bgcolor);
            /* draw gray bar from mid till the end */
            M5Dial.Display.fillArc(x, y, r, R, midAngle,   startAngle + sweepAngle, bgcolor);
        }
        else{
            /* draw gray bar from start till the mid */
            M5Dial.Display.fillArc(x, y, r, R, startAngle, midAngle,                bgcolor);
            /* draw gray bar from progress till the end */
            M5Dial.Display.fillArc(x, y, r, R, endAngle,   startAngle + sweepAngle, bgcolor);
        }

        /* draw gray dots at the both ends */
        M5Dial.Display.fillCircle(DotX(startAngle),              DotY(startAngle + sweepAngle), (thickness / 2) + 1, bgcolor);
        M5Dial.Display.fillCircle(DotX(startAngle + sweepAngle), DotY(startAngle + sweepAngle), (thickness / 2) + 1, bgcolor);
    }


public:
    Bar(int x, int y, unsigned int r, unsigned int thickness, int sweepAngle)
    : GUI_Element(x, y, r * 2, r * 2), r(r), thickness(thickness), sweepAngle(sweepAngle) {
        startAngle = 90 + (360 - sweepAngle) / 2;
        R = r + thickness;
    }
    Bar(int x, int y, unsigned int r, unsigned int thickness, int startAngle, int sweepAngle)
        : GUI_Element(x, y, r * 2, r * 2), r(r), thickness(thickness), startAngle(startAngle), sweepAngle(sweepAngle) {
            R = r + thickness;
    }
    ~Bar() override {}

    inline void EnableLinearMode(){ EnableMode(LINEAR); }
    inline void EnableSymmetricMode(){ EnableMode(SYMETRIC); }

    void SetSecondColor(uint16_t color){ second_color = color; }
    /**
     * @brief Progress bar update function using bound value with force update refresh param.
     * @attention This uses the array of pointers.
     * `long* arr[] = { &elapsed, &duration };`
     */
    void Update(bool force_update = false) override {
        // Value not assigned
        if (!boundValue) return;

        // Get values
        long** values = static_cast<long**>(boundValue);
        long curr = *(values[0]);
        long low  = *(values[1]);
        long max  = *(values[2]);

        long range = (max - low);

        // Get the percent
        float percent = (range > 0) ? float(curr - low) / float(range) : 0;
        percent = constrain(percent, 0.0, 1.0);

        if(isnan(percent))
            return;

        // If values has not changed and force_update is false
        if(lastValue.F_LastValue == (percent) && !force_update)
            return;

        lastValue.F_LastValue = percent;
        Clear(percent);
        mode == LINEAR ? DrawLinear(percent) : DrawSymetric(percent);
    }

    void Clear(float& percent){
        mode == LINEAR ? ClearLinear(percent) : ClearSymetric(percent);
    }
};

#endif //GUIBAR_H