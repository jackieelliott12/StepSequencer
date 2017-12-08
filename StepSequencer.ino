//this is a step sequencer
//copy of lab 7
int buttonPins[4] = { 36, 35, 34, 33 };
int nextButtonPin = 39;
int previousButtonPin = 38;
int ledPins[4] = { 2, 3, 4, 5 };
int totalLeds = 4;
int totalStepState = 3;
int tempo = 0;
boolean lastButtonState[4] = { LOW, LOW, LOW, LOW };
boolean buttonState[4] = { LOW, LOW, LOW, LOW };
boolean lastNextButtonState = LOW;
boolean nextButtonState = LOW;
boolean lastPreviousButtonState = LOW;
boolean previousButtonState = LOW;
boolean stepState[3][4] = {
  { HIGH, LOW, LOW, LOW },
  { LOW, HIGH, LOW, LOW },
  { LOW, LOW, HIGH, LOW }
};
int currentSequence = 0;

int currentStep = 0;
unsigned long lastStepTime = 0;

void setup() {
  for (int i = 0; i < totalLeds; i++) {
    pinMode(buttonPins[i], INPUT);
    pinMode(nextButtonPin, INPUT);
    pinMode(previousButtonPin, INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  sequence();
  checkButtons();
  checkChannelButtons();
  setLeds();
}

void checkChannelButtons() {
  lastNextButtonState = nextButtonState;
  nextButtonState = digitalRead(nextButtonPin);

  if (nextButtonState == HIGH && lastNextButtonState == LOW) {
    currentSequence = currentSequence + 1;
    if (currentSequence > 2) {
      currentSequence = 0;
    }
  }

  lastPreviousButtonState = previousButtonState;
  previousButtonState = digitalRead(previousButtonPin);

  if (previousButtonState == HIGH && lastPreviousButtonState == LOW) {
    currentSequence = currentSequence - 1;
    if (currentSequence < 0) {
      currentSequence = 2;
    }
  }
}

void checkButtons() {

  for (int i = 0; i < totalLeds; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPins[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (stepState[currentSequence][i] == false) {
        stepState[currentSequence][i] = true;
      } else if (stepState[currentSequence][i] == true) {
        stepState[currentSequence][i] = false;
      }
    }
  }
}

//void setLeds() {
//  for (int i = 0; i < totalLeds; i++) {
//    if(stepState[i] == true) {
//         digitalWrite(ledPins[i], HIGH);
//     }  else if(stepState[i] == false) {
//         digitalWrite(ledPins[i], LOW);
//     }
//  }
//}



void setLeds() {
  for (int i = 0; i < totalLeds; i++) {
    if (currentStep == i) {
      analogWrite(ledPins[i], 255);
    } else if (stepState[currentSequence][i] == true) {
      analogWrite(ledPins[i], 50);
    } else {
      analogWrite(ledPins[i], 0);
    }
  }
}

void sequence() {
  tempo = analogRead(A18);

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //    digitalWrite(ledPins[currentStep], LOW);  //turn off the current led

    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }

    if (stepState[0][currentStep] == HIGH) {
      usbMIDI.sendNoteOn(60, 127, 1);
    }  else if (stepState[0][currentStep] == LOW) {
      usbMIDI.sendNoteOff(60, 0, 1);
    }
    if (stepState[1][currentStep] == HIGH) {
      usbMIDI.sendNoteOn(62, 127, 1);
    }  else if (stepState[1][currentStep] == LOW) {
      usbMIDI.sendNoteOff(62, 0, 1);
    }
    if (stepState[2][currentStep] == HIGH) {
      usbMIDI.sendNoteOn(64, 127, 1);
    }  else if (stepState[2][currentStep] == LOW) {
      usbMIDI.sendNoteOff(64, 0, 1);
    }


    lastStepTime = millis();               //set lastStepTime to the current time
  }
}


