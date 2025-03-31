#include "M5Dial.h"
#include "GUI/Gui_Text.h"

/*      NAME        X      Y       Width   Font */
Text    test_text  (120,   120,    140,    &FreeSerif12pt7b);

void setup() {
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);

    String name = "Hello, World!";
    test_text.Bind(name);
}

void loop() {
    test_text.Update();
    delay(1000);
}