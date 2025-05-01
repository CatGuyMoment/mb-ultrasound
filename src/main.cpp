#include <Arduino.h>

#include <M5Unified.h>
#include <Unit_Sonic.h>

SONIC_I2C sensor; //why is cpp oop so freaky like that

float min_distance = 800;

bool is_ready = true;
int updateMode = 0;

void clear(int numberToClear);


void setup() {
  // put your setup code here, to run once:
  M5.begin();

  sensor.begin();
  M5.Speaker.setVolume(50);
  
  M5.Display.print("READY");
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
  
  float Distance = sensor.getDistance();



  if (Distance < min_distance && is_ready) {
    M5.Speaker.tone(5000,200);
    M5.Speaker.tone(3000,200);
    M5.delay(100);
    delay(200);
    M5.Display.setCursor(0,0);


    updateMode = 1;
    clear(400);

    String payload = ("Sensor triggered! Distance:") + String(Distance);


    M5.Display.print(payload);

  
  } else {

   if (is_ready) {
    if (updateMode != 0) {
      updateMode = 0;
      clear(400);
      M5.Display.print("READY");
    }


   } else {
    if (updateMode != 2) {
      updateMode = 2;
      clear(400);
      M5.Display.print("DISABLED");
    }
    


   }



  }
  


}

void clear(int numberToClear) {

  char test[numberToClear] = "";
  for (int i = 0; i < numberToClear ;i++) {
    test[i] = ' ';
  };

  test[numberToClear] = '\0'; //how the fuck would i remember this... no genuinely


  M5.Display.setCursor(0,0);
  M5.Display.print(test);
  M5.Display.setCursor(0,0);
}