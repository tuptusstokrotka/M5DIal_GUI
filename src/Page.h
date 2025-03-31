#ifndef DISPLAY_H
#define DISPLAY_H

#pragma once

#include <vector>
#include "GUI/Gui.h"

class Page {
    std::vector<GUI_Element*> elements;

public:
    void AddElement(GUI_Element* element);
    void UpdateElements(bool force_update = false);
    void ClearPage(void);

    // Variadic Template to Push elements via constructor
    template <typename... Args>
    Page(Args&... args){
        (AddElement(&args), ...);  // Expand pack and call AddElement for each argument
    }
    ~Page();
};

#endif