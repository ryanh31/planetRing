//2021 holsopple / silovsky  studios

// https://www.digikey.com/htmldatasheets/production/2306332/0/0/1/ctc-dra-10-1.html?utm_adgroup=xGeneral&utm_source=google&utm_medium=cpc&utm_campaign=Dynamic%20Search_EN_Product&utm_term=&utm_content=xGeneral&gclid=Cj0KCQjw7pKFBhDUARIsAFUoMDYg0JJiZymidibnvxw7RpKeJbeMyGIQ4zdx4JOHhe102LzEzHpa21MaAgZrEALw_wcB
//https://www.omc-stepperonline.com/download/DM860T.pdf
//https://www.digikey.com/en/products/detail/dfrobot/DFR0260/7087143
//https://www.omc-stepperonline.com/download/DM860T.pdf
//8mm proximity sensor: MAE-AP-1A https://cdn.automationdirect.com/static/specs/ip69kproxmag1218maf.pdf

#include <AccelStepper.h>
#include <DMXSerial.h>


#define enable 13
#define dirPin 5 //dir-
#define stepPin 7  //pul-
#define motorInterfaceType 1
// Define a stepper and the pins it will use
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

const int LED =    13;  //
const int LED2 =    9;  // PWM output pin for testing


// variables for DMX start channel
int MaxDMXChannels = 9;
int DMXstartChan = 1 ;//set start channel DMX //planet == 1 //ring ==11

const int switchPin1 = 11;
int switchState1 = 0;         // variable for reading the pushbutton status

void setup()
{
  //DEFAULT DMX VALUES
  DMXSerial.init(DMXReceiver);

  for (int DMXchannel = DMXstartChan; DMXchannel < (DMXstartChan + 8); DMXchannel++) {
    // turn the pin on:
    DMXSerial.write(DMXchannel, 0);
    delay(1);
  }


  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enable, OUTPUT);


  // enable pwm outputs
  pinMode(LED,   OUTPUT); // sets the digital pin as output
  pinMode(switchPin1, INPUT_PULLUP);


  stepper.setMaxSpeed(400);
  stepper.setAcceleration(20);
}

void loop()
{

  int mode = DMXSerial.read(DMXstartChan);//DMX Channel 10 PLANET  DMX Start Channel 20 RING
  if ((mode == 0) ) { //0 isadora percent
    digitalWrite(LED, LOW);
    //stepper.setCurrentPosition(0);
  }
  // MODE 1  autohome
  if ((mode > 0) && (mode < 5 )) { //1 isadora percent
    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);

    do {
      // read the state of the pushbutton value:
      switchState1 = digitalRead(switchPin1);

      stepper.moveTo(-3000);//1600 FULL ROTATION
      stepper.setMaxSpeed(40);
      stepper.setAcceleration(10);
      stepper.run();
    }
    while (switchState1 == LOW);
    digitalWrite(LED, LOW);
    stepper.stop();
    stepper.setCurrentPosition(0);
    delay(100);
  }



  //  MODE 2
  else if ((mode == 5 )) { //isadora 2 %
    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);
    do {

      if (DMXSerial.read(DMXstartChan + 2) > 127) {
        stepper.moveTo(1600);//one rotation
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
        stepper.setAcceleration(20);
        stepper.run();
      }

      else {
        stepper.moveTo(-1600);//one rotation
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
        stepper.setAcceleration(20);
        stepper.run();
      }
    }
    while ( DMXSerial.read(DMXstartChan + 1) > 10);

  }



  //  MODE 3
  else if ((mode > 5 ) && (mode < 10)) { //isadora 3 %
    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);
    do {

      if (DMXSerial.read(DMXstartChan + 2) > 127) {
        stepper.moveTo(800 * DMXSerial.read(DMXstartChan + 4));//one rotation
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
        stepper.setAcceleration(20);
        stepper.run();
      }

      else {
        stepper.moveTo(-800 * DMXSerial.read(DMXstartChan + 4));//one rotation
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
        stepper.setAcceleration(20);
        stepper.run();
      }
    }
    while ( DMXSerial.read(DMXstartChan + 1) > 10);

  }





  //  MODE 4  GO TO CONSTANT SPEED,CHECK, DIRECTION, SPEED
  else if ((mode > 10 ) && (mode < 20 )) { //isadora 2 %
    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);
    do {

      if (DMXSerial.read(DMXstartChan + 2) > 10) {
        stepper.moveTo(stepper.currentPosition() + 8000);
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
        stepper.setAcceleration(20);
        stepper.run();
      }

    }
    while ( DMXSerial.read(DMXstartChan + 4) < 10);
    do {
      stepper.moveTo(stepper.currentPosition() - 0);
      stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
      stepper.setAcceleration(20);
      stepper.run();
    }

    while (DMXSerial.read(DMXstartChan + 1) > 10);
    // Serial.println("nada");
  }




  //  MODE 5  GO TO CONSTANT SPEED,CHECK, DIRECTION, SPEED counterclockwize
  else if ((mode > 20 ) && (mode < 30 )) { //isadora 2 %
    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);
    do {

      if (DMXSerial.read(DMXstartChan + 2) > 127) {
        stepper.moveTo(stepper.currentPosition() - 8000);
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
        stepper.setAcceleration(20);
        stepper.run();
      }

    }
    while ( DMXSerial.read(DMXstartChan + 4) < 10);
    do {
      stepper.moveTo(stepper.currentPosition() + 0);
      stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 800));
      stepper.setAcceleration(6);
      stepper.run();
    }

    while (DMXSerial.read(DMXstartChan + 2) != 0);
    // Serial.println("nada");
  }

  //NEW MODE 6
  ///MODE 6 LIVE RUN TO SPEED
  else if ((mode == 79)) { //isadora31
    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);
    do {

      if (DMXSerial.read(DMXstartChan + 2) > 127) {
        stepper.moveTo(stepper.currentPosition() + 6000);
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 600));
        stepper.setAcceleration(6);
        stepper.run();
      }

    }
    while ( DMXSerial.read(DMXstartChan + 4) < 10);
    do {
      stepper.moveTo(stepper.currentPosition() + 0);
      stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 600));
      stepper.setAcceleration(6);
      stepper.run();
    }

    while (DMXSerial.read(DMXstartChan + 2) != 0);
    // Serial.println("nada");
  }


  //NEW MODE 7 RUN TO FAST CCW
  else if ((mode > 30 && (mode < 40))) { //ISADORA 15
    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);
    do {

      if (DMXSerial.read(DMXstartChan + 2) > 127) {
        stepper.moveTo(stepper.currentPosition() - 6000);
        stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 600));
        stepper.setAcceleration(6);
        stepper.run();
      }

    }
    while ( DMXSerial.read(DMXstartChan + 4) < 10);
    do {
      stepper.moveTo(stepper.currentPosition() + 0);
      stepper.setMaxSpeed(map(DMXSerial.read(DMXstartChan + 3), 0, 255, 0, 600));
      stepper.setAcceleration(6);
      stepper.run();
    }

    while (DMXSerial.read(DMXstartChan + 2) != 0);
    // Serial.println("nada");
  }


  ///NEW MODE 9 LIVE RUN TO SPEED
  else if ((mode > 40 && (mode < 50))) { //ISADORA 18
    digitalWrite(LED, HIGH);
    //stepper.setSpeed(DMXSerial.read(DMXstartChan + 3) * 1 )); //speedVal
    stepper.setSpeed(DMXSerial.read(DMXstartChan + 3) * 1); //speedVal
    stepper.runSpeed();
    stepper.run();
  }


  //NEW MODE 10 RUN TO speed
  else if ((mode > 50 && (mode < 60))) { //ISADORA 22
    stepper.setCurrentPosition(0);

    digitalWrite(LED, HIGH);
    stepper.setSpeed(DMXSerial.read(DMXstartChan + 3) * -1); //speedVal
    stepper.runSpeed();
    stepper.run();

  }


  //new mode 11 autohome 2
  else if ((mode > 60 && (mode < 70))) { //ISADORA 26

    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);

    do {
      // read the state of the pushbutton value:
      switchState1 = digitalRead(switchPin1);

      stepper.moveTo(4000);//1600 FULL ROTATION
      stepper.setMaxSpeed(60);
      stepper.setAcceleration(20);
      stepper.run();
      //  stepper2.run();
    }
    while (switchState1 == LOW);
    digitalWrite(LED, LOW);
    stepper.stop();
    stepper.setCurrentPosition(0);
    delay(100);
  }

  else if (mode > 80 ) { //isadora 2 %    digitalWrite(LED, HIGH);
    stepper.setCurrentPosition(0);
    digitalWrite(LED, HIGH);

    do {
      //(DMXSerial.read(DMXstartChan + 2) > 127)


      stepper.moveTo(map(DMXSerial.read(DMXstartChan + 4), 0, 100, 0, 800));
      stepper.setMaxSpeed(800);//was 800
      stepper.setAcceleration(20);
      stepper.run();

    }
    while ( DMXSerial.read(DMXstartChan + 1) > 10);

  }




  delay(1);        // delay in between reads for stability
}
