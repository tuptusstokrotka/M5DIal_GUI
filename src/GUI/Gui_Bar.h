#ifndef GUIBAR_H
#define GUIBAR_H

#pragma once

#include "Gui.h"

#define MID_RADIUS (r + (R - r) / 2)  // More precise middle radius
#define DotX(angle) (x + cos(radians(angle)) * MID_RADIUS)
#define DotY(angle) (y + sin(radians(angle)) * MID_RADIUS)

class Bar : public GUI_Element {
    unsigned int r;         // Inner radius
    unsigned int R;         // Outer radius
    unsigned int thickness; // Bar width
    int startAngle;
    int sweepAngle;

    bool dual_color = false;            // Use 2 colors for the Bar (split in half the range)
    uint16_t second_color = YELLOW;     // Second Bar color (dual color must be enabled)

    void Draw(float percent){
        int endAngle = startAngle + (int)(sweepAngle * percent);

        /* draw colored bar and start dot - DUAL COLOR */
        if(dual_color){
            if(percent <= 0.5){
                M5Dial.Display.fillArc(x, y, r, R, startAngle, endAngle, second_color);
                M5Dial.Display.fillCircle(DotX(startAngle), DotY(startAngle), (thickness / 2) + 1, second_color);
            }
            else{
                M5Dial.Display.fillArc(x, y, r, R, startAngle, endAngle, color);
                M5Dial.Display.fillCircle(DotX(startAngle), DotY(startAngle), (thickness / 2) + 1, color);
            }
        }
        else{
            M5Dial.Display.fillArc(x, y, r, R, startAngle, endAngle, color);
            M5Dial.Display.fillCircle(DotX(startAngle), DotY(startAngle), (thickness / 2) + 1, color);
        }
        /* draw white progress dot */
        M5Dial.Display.fillCircle(DotX(endAngle), DotY(endAngle), (thickness / 2) + 1, WHITE);
    }

public:
    Bar(int x, int y, unsigned int r, unsigned int thickness, int startAngle, int sweepAngle)
        : GUI_Element(x, y, r * 2, r * 2), r(r), thickness(thickness), startAngle(startAngle), sweepAngle(sweepAngle) {
            R = r + thickness;
    }
    ~Bar() override {}

    void EnableDualColor(bool enable){ dual_color = enable; }
    void SetDualColor(uint16_t color){ second_color = color; }
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
        long max  = *(values[1]);

        // Get the percent
        float percent = (max > 0) ? float(curr) / float(max) : 0;
        percent = constrain(percent, 0.0, 1.0);

        if(isnan(percent))
            return;

        // If values has not changed and force_update is false
        if(lastValue.F_LastValue == (percent) && !force_update)
            return;

        lastValue.F_LastValue = percent;
        Clear(percent);
        Draw(percent);
    }

    void Clear(float percent){
        int endAngle = startAngle + (int)(sweepAngle * percent);

        /* Black bars outside the bar to clear White Dot - Dummy but works */
        M5Dial.Display.fillArc(x, y, R+1, R+2, startAngle, startAngle+sweepAngle, bgcolor);
        M5Dial.Display.fillArc(x, y, r-1, r-2, startAngle, startAngle+sweepAngle, bgcolor);

        /* draw gray bar from progress till the end +/-1 to clear the orange residue */
        M5Dial.Display.fillArc(x, y, r, R, endAngle, startAngle + sweepAngle, bgcolor);
        /* draw gray end dot */
        M5Dial.Display.fillCircle(DotX(startAngle + sweepAngle), DotY(startAngle + sweepAngle), (thickness / 2) + 1, bgcolor);
    }
};

#endif //GUIBAR_H