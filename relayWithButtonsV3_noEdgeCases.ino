int RL1 = 2;
int RL3 = 3;
int RL4 = 4;
int RL6 = 5;
int charge_input = 6;
int main_input = 7;
int forward_input = 8;
int reverse_input = 9;
boolean park_input = false;
boolean operation = false;

void setup() {
  pinMode(RL1, OUTPUT);
  pinMode(RL3, OUTPUT);
  pinMode(RL4, OUTPUT);
  pinMode(RL6, OUTPUT);
  pinMode(charge_input, INPUT_PULLUP);
  pinMode(main_input, INPUT_PULLUP);
  pinMode(forward_input, INPUT_PULLUP);
  pinMode(reverse_input, INPUT_PULLUP);

  Serial.begin(9600);
  allRelaysOpen();

}

void loop() {

  //check delay times

      if(charge_input == 1){ //Charging
        allRelaysOpen();
        //closed relays
        digitalWrite(RL6,HIGH);
        Serial.println("Charged");
      //break;
    }

    int park_input = park(forward_input, reverse_input);

    if(main_input == 1 && park_input == 1 && operation == false){ //ignition
        allRelaysOpen();
        digitalWrite(RL3, HIGH);
        delay(100000);
        operation = true;
        Serial.println("Ignition");

    }
      if(main_input == 1 && operation && forward_input == HIGH && reverse_input == LOW){ //operation
        allRelaysOpen();
        //closed relays
        digitalWrite(RL1, HIGH);
        Serial.println("Operation");
        }


        if(main_input == 1 && park_input == 1 && operation == true){ //parking after driving but leaving the ignition on
            allRelaysOpen();
            Serial.println("In park but ignition stays on");
        }

        if(main_input == 0 && park_input == 1){  //discharge
          allRelaysOpen();
          digitalWrite(RL4, HIGH);
          delay(5000);
          allRelaysOpen();
          operation = false;
          Serial.println("Discharged");

        }

}

int park(int forward_input, int reverse_input){
    if(forward_input == HIGH && reverse_input == HIGH){
        return 1;
    }
    else {
        return 0;
    }
}

void allRelaysOpen(){
  digitalWrite(RL1, LOW);
  digitalWrite(RL3, LOW);
  digitalWrite(RL4, LOW);
  digitalWrite(RL6,LOW);
  delay(100);
  
}
