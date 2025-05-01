#include <Arduino.h>

#include <M5Unified.h>
#include <Unit_Sonic.h>

SONIC_I2C sensor; //why is cpp oop so freaky like that

float min_distance = 200;

bool is_ready = true;
int updateMode = 0;

void clear(int numberToClear);


namespace printer {
  String previousText;

  void update(String newText ,uint32_t color, float size) {

    if (previousText) {
      M5.Display.setTextColor(BLACK);
      M5.Display.setCursor(0,0);
      M5.Display.print(previousText);
    }

    M5.Display.setTextSize(size);
    M5.Display.setTextColor(color);
    M5.Display.setCursor(0,0);
    M5.Display.print(newText);

    previousText = newText;
  }
}
// uint32_t  GREEN = M5.Display.color888(255,255,0)

uint32_t READY_COLOR = M5.Display.color888(0,255,0);
uint32_t DISABLED_COLOR = M5.Display.color888(255,0,0);
uint32_t DETECTED_COLOR = M5.Display.color888(255,255,255);

void setup() {
  // put your setup code here, to run once:
  M5.begin();

  sensor.begin();
  M5.Speaker.setVolume(50);
  
  printer::update("READY",READY_COLOR,10.0);

  M5.Speaker.tone(5000,200);
  
  M5.delay(100);
  M5.Speaker.stop();
  M5.Speaker.setVolume(255);

}

void loop() {
  M5.update();

  if (M5.BtnB.wasClicked()) {
    is_ready = !is_ready;
    M5.Speaker.setVolume(50);
    
    if (is_ready) {
      M5.Speaker.tone(5000,100);

    } else {
      M5.Speaker.tone(3000,100);

    }

    M5.delay(100);
    M5.Speaker.setVolume(255);
  }
  
  if (M5.BtnA.wasPressed() || M5.BtnC.wasPressed()) {
    updateMode = 1;
    int delta;

    if (M5.BtnA.wasPressed()) {
      delta = -20;
    } else {
      delta = 20;

    }
    min_distance += delta;
    String payload = String("NEW DISTANCE: ") + String(min_distance);


    printer::update(payload,DETECTED_COLOR,4);
    while ((!M5.BtnA.wasReleased()) && (!M5.BtnC.wasReleased())  )
    {
      M5.update();
    }
    
  }

  float Distance = sensor.getDistance();



  if (Distance < min_distance && is_ready) {
    M5.Speaker.tone(5000,200);
    M5.Speaker.tone(3000,200);
    delay(100);
    if (Distance * 2 > min_distance) {
      delay(200);
    }
    M5.Display.setCursor(0,0);


    updateMode = 1;

    String payload =  String(Distance);


    printer::update(payload,DETECTED_COLOR,8);

  
  } else {

   if (is_ready) {
    if (updateMode != 0) {
      updateMode = 0;
      printer::update(String("READY\n") + String(Distance),READY_COLOR,10);
    }


   } else {
    if (updateMode != 2) {
      updateMode = 2;
      printer::update("DISABLED",DISABLED_COLOR,6);
    }
    


   }



  }
  


}

