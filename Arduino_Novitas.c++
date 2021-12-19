#include "RTClib.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 8 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 96 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

RTC_DS3231 rtc;
#define LED_PIN 6
#define BUTTON_PIN 2 //brightness
#define BUTTON_pin 13
byte lastButtonState = LOW;
byte anotherlastButtonState = LOW;

byte ledState = LOW;
unsigned long debounceDuration = 50; // millis
unsigned long lastTimeButtonStateChanged = 0;
unsigned long anotherlastTimeButtonStateChanged = 0;
void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUTTON_pin, INPUT);
    Serial.begin(9600);

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.setBrightness(200);

    if (!rtc.begin())
    {

        Serial.flush();
        abort();
    }

    if (rtc.lostPower())
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

DateTime future;
DateTime repeat(uint32_t(0));
int state = -1;
int brightness = 0;

void loop()
{

    //pixels.clear();

    pixels.show();

    DateTime now = rtc.now();
    bool trigger = 0;

    if (now >= repeat && repeat.unixtime() > 0)
    {
        trigger = 1;
        repeat = DateTime(uint32_t(0));
    }

    Serial.println(state);

    if (millis() - lastTimeButtonStateChanged > debounceDuration)
    { //brightness
        byte buttonState = digitalRead(BUTTON_PIN);
        if (buttonState != lastButtonState)
        {
            lastTimeButtonStateChanged = millis();
            lastButtonState = buttonState;
            if (buttonState == LOW)
            {
                switch (brightness)
                {
                case 0:
                    pixels.setBrightness(10);
                    break;
                case 1:
                    pixels.setBrightness(100);
                    break;
                case 2:
                    pixels.setBrightness(255);
                    break;
                }

                brightness = (brightness + 1) % 3;
            }
        }
    }

    if (millis() - anotherlastTimeButtonStateChanged > debounceDuration)
    { //on/off

        byte anotherbuttonState = digitalRead(BUTTON_pin);
        if (anotherbuttonState != anotherlastButtonState)
        {
            anotherlastTimeButtonStateChanged = millis();
            anotherlastButtonState = anotherbuttonState;
            if (anotherbuttonState == LOW)
            {
                state = (state + 1) % 4;
                trigger = 1;
            }
        }

        if (now >= future)
        {
            pixels.setPixelColor(5, pixels.Color(0, 0, 0));
            pixels.setPixelColor(4, pixels.Color(0, 0, 0));
            pixels.setPixelColor(3, pixels.Color(0, 0, 0));
        }
    }

    if (trigger)
    {
        switch (state)
        {
        case 0:
            pixels.setPixelColor(5, pixels.Color(255, 0, 150));
            pixels.setPixelColor(4, pixels.Color(255, 0, 150));
            pixels.setPixelColor(3, pixels.Color(255, 0, 150));
            future = now + TimeSpan(0, 3, 0, 0);
            repeat = now + TimeSpan(0, 24, 0, 0);

            break;

        case 1:
            pixels.setPixelColor(5, pixels.Color(255, 0, 150));
            pixels.setPixelColor(4, pixels.Color(255, 0, 150));
            pixels.setPixelColor(3, pixels.Color(255, 0, 150));
            future = now + TimeSpan(0, 9, 0, 0);
            repeat = now + TimeSpan(0, 24, 0, 0);

            break;
        case 2:
            pixels.setPixelColor(5, pixels.Color(255, 0, 150));
            pixels.setPixelColor(4, pixels.Color(255, 0, 150));
            pixels.setPixelColor(3, pixels.Color(255, 0, 150));
            future = now + TimeSpan(0, 12, 0, 0);
            repeat = now + TimeSpan(0, 24, 0, 0);

            break;
        case 3:
            pixels.setPixelColor(5, pixels.Color(0, 0, 0));
            pixels.setPixelColor(4, pixels.Color(0, 0, 0));
            pixels.setPixelColor(3, pixels.Color(0, 0, 0));
            break;
        }
    }
}