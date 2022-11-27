int RL1 = 2;
int RL3 = 3;
int RL4 = 4;
int RL6 = 5;
int input;

void setup() {
  pinMode(RL1, OUTPUT);
  pinMode(RL3, OUTPUT);
  pinMode(RL4, OUTPUT);
  pinMode(RL6, OUTPUT);
  Serial.begin(9600);
  allRelaysOpen();
}

void loop() {
  if (Serial.available()>0){
    input = Serial.read();
      
    switch (input){
      case '1':{ //Charging
        allRelaysOpen();
        //closed relays
        digitalWrite(RL6,HIGH);
        Serial.println("Charging: RL6");
      break;
    }
      case '2':{ //Discharge
        allRelaysOpen();
        //closed relays
        digitalWrite(RL4,HIGH);
        Serial.println("Charging: RL4");
      break;
    }
      case '3':{ //Operation
        allRelaysOpen();
        //closed relays
        digitalWrite(RL1, HIGH);
        Serial.println("Charging: RL1");
      break;
      }
      case '4': { //Ignition
        allRelaysOpen();
        //closed relays
        digitalWrite(RL3,HIGH);
        Serial.println("Charging: RL3");
      break;
      }
      default: {
        allRelaysOpen();
        break;
      }
    }
  }
}

void allRelaysOpen(){
  digitalWrite(RL1, LOW);
  digitalWrite(RL3, LOW);
  digitalWrite(RL4, LOW);
  digitalWrite(RL6,LOW);
  delay(100);
  
}
