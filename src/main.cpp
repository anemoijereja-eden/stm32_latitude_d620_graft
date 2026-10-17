#include "api/Common.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include "RevEng_PAJ7620.h"
#include <Keyboard.h>
#include "tusb.h"

RevEng_PAJ7620 sensor = RevEng_PAJ7620();

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0 );
U8G2LOG u8g2log;

#define LOG_COLS 32
#define LOG_ROWS 10

// Allocate the required RAM matrix buffer
uint8_t u8log_buffer[LOG_COLS * LOG_ROWS];

void gestureIRQ()
{
  Gesture gesture;
  gesture = sensor.readGesture();
  switch (gesture)
    {
      case GES_FORWARD:
        {
          Keyboard.press(KEY_RETURN);
          break;
        }

      case GES_BACKWARD:
        {   
          Keyboard.press(KEY_TAB);
          break;
        }

      case GES_LEFT:
        {
          Keyboard.press(KEY_LEFT_ARROW);
          break;
        }

      case GES_RIGHT:
        {
          Keyboard.press(KEY_RIGHT_ARROW);
          break;
        }

      case GES_UP:
        {
          Keyboard.press(KEY_UP_ARROW);
          break;
        }

      case GES_DOWN:
        {
          Keyboard.press(KEY_DOWN_ARROW);
          break;
        }

      case GES_CLOCKWISE:
        {
          Keyboard.press('+');
          break;
        }

      case GES_ANTICLOCKWISE:
        {
          break;
        }

      case GES_WAVE:
        {
          break;
        }

      case GES_NONE:
        {
          break;
        }
    }
  Keyboard.releaseAll();
}

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
  u8g2log.begin(u8g2, LOG_COLS, LOG_ROWS, u8log_buffer);
  u8g2log.setRedrawMode(0);

  u8g2log.println("sys term init");
  u8g2log.println("HW gesture + display co-processor system initializing");

  pinMode(PB2, OUTPUT);
  digitalWrite(PB2, HIGH);
  u8g2log.println("led init");

  Keyboard.begin();
  u8g2log.println("USB HID init");

  pinMode(PB8, INPUT);
  attachInterrupt(digitalPinToInterrupt(PB8), gestureIRQ, FALLING);
  if( sensor.begin())
  {
    u8g2log.println("gesture sens init");
  } else {
    u8g2log.println("gesture sens init FAIL");
    u8g2log.println("system failure. halting.");
    __ASM volatile ("bkpt 0");
  }

  u8g2log.println("sys init complete.");
  u8g2log.setRedrawMode(1);
  u8g2log.print("starting in 3");
  delay(100);
  u8g2log.print(" 2");
  delay(100);
  u8g2log.print(" 1");
  delay(100);
  u8g2log.print(" GO!\n");
  u8g2log.setRedrawMode(0);

}

void loop() {
  // Check for incoming serial data, and send any found to the screen

}
