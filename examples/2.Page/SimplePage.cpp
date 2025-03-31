#include "M5Dial.h"
#include "GUI/Gui_Text.h"
#include "Page.h"

/*      NAME        X      Y       Width   Font                Scrolling */
Text    text_upper (120,   100,    140,    &FreeSerif12pt7b,   true);
Text    text_lower (120,   140,    140,    &FreeSerif12pt7b);

// Page page(text_upper, text_lower); // Add all elements
Page page(text_upper); // Add 1 element via constructor


void setup() {
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);

    String long_name = "This is veeeeery looong text";
    text_upper.Bind(long_name);

    String name = "Hello, World!";
    text_lower.Bind(name);

    page.AddElement(&text_lower); // Manually add next element to the page
}

void loop() {
    // Update all the elements on the screen
    page.UpdateElements();
    delay(1000);
}