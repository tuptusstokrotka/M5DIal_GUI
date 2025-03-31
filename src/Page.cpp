#include "Page.h"
#include <M5Dial.h>


Page::~Page() {}

void Page::AddElement(GUI_Element* element) {
    elements.push_back(element);
}

void Page::UpdateElements(bool force_update) {
    if(elements.size() == 0)
        return;

    for (auto* el : elements) {
        el->Update(force_update);
    }
}
void Page::ClearPage(void) {
    // Instead of this, this could clean each element in loop, to prepare the next screen
    M5Dial.Display.clear();
}