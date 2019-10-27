#include "include/display-oled.h"

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(MODE_I2C, D8, 1); // I2C declaration

void DisplayOLEDinit(void)
{
    oled.begin();         // Initialize the OLED

    oled.clear(PAGE);     // Clear the screen

    oled.setFontType(1);  // Set font to type 0

    oled.setCursor(0, 0); // Set cursor to top-left

    oled.print("Hello!");

    oled.display();

    delay(2000);
}

void DisplayOLEDupdateTemp(float tempCelsius)
{
    static char tempBuffer[32];

    oled.clear(PAGE);     // Clear the screen
    
    oled.setFontType(1);  // Set font to type 0
    
    oled.setCursor(0, 0); // Set cursor to top-left

    memset(tempBuffer, 0, sizeof(tempBuffer));

    snprintf(tempBuffer, sizeof(tempBuffer), "%.1f C", tempCelsius);

    oled.print(tempBuffer);
    
    oled.display();
}


