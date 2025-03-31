#ifndef DISPLAY_H
#define DISPLAY_H

#pragma once

#include <vector>
#include "Page.h"
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

class Display{
    unsigned int page = 0;
    std::vector<Page*> pages;

public:
    bool force_update = false;

    void AddPage(Page* page);
    void SetPage(unsigned int page);
    unsigned int GetPage(void);

    void Update(void);

    // Variadic Template to Push elements via constructor
    template <typename... Args>
    Display(Args&... args){
        (AddPage(&args), ...);      // Expand pack and call AddElement for each argument
    }
    ~Display();
};

#endif