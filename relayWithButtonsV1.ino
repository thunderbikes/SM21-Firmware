int RL1 = 2;
int RL3 = 3;
int RL4 = 4;
int RL6 = 5;
int charge_input = 6;
int main_input = 7;
int forward_input = 8;
int reverse_input = 9;
boolean park_input = false;
boolean goodToMove = false;

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

      if(charge_input == 1):{ //Charging
        allRelaysOpen();
        //closed relays
        digitalWrite(RL6,HIGH);
        Serial.println("Charging: RL6");
      //break;
    }

    park_input = park(forward_input, reverse_input);

    if(main_input == 1 && park_input == true && goodToMove == false){ //starting ignition
        allRelaysOpen();
        digitalWrite(RL3, HIGH);
        delay(100000);
        boolean goodToMove = true;
    }
      if(main_input == 1 && goodToMove && forward_input == HIGH){ //moving bike after ignition
        allRelaysOpen();
        //closed relays
        digitalWrite(RL1, HIGH);
        Serial.println("Charging: RL1");
        }


        if(main_input == 1 && park_input && goodToMove == true){ //in motion trying to go to park to ignition
            allRelaysOpen();
            digitalWrite(RL3, HIGH);
            delay(100000);
            //boolean goodToMove = false;
        }

        if(main_input == 0 && park ){
            allRelaysOpen();
            goodToMove = false;
        }

}

boolean park(forward_input, reverse_input){
    if(forward_input == HIGH && reverse_input == HIGH){
        return true;
    }
    else {
        return false;
    }
}

void allRelaysOpen(){
  digitalWrite(RL1, LOW);
  digitalWrite(RL3, LOW);
  digitalWrite(RL4, LOW);
  digitalWrite(RL6,LOW);
  delay(100);
  
}
