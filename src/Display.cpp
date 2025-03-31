#include "Display.h"
#include <M5Dial.h>

Display::~Display() {}

void Display::AddPage(Page* page){
    pages.push_back(page);
}

void Display::SetPage(unsigned int page){
    // Skip if page has not been changed
    if(page == GetPage() || page >= pages.size())
        return;

    force_update = true;            // Set flag
    pages[this->page]->ClearPage(); // Clear the content of a Page
    this->page = page;              // Update page index
}

unsigned int Display::GetPage(void){ return this->page; }

void Display::Update(void){
    // check page vector exists
    if(page >= pages.size())
        return;

    // Current page -> Update all the elements
    pages[page]->UpdateElements(force_update);
    // Clear flag after successful page update
    force_update = false;
}
