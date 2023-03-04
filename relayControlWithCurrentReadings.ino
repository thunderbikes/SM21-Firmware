const int RL1 = 2; //Operation
const int RL3 = 3; //Ignition
const int RL4 = 4; //Discharge
const int RL6 = 5; //Charging
const int charge_input = 6; //Charge Button
const int main_input = 7; //Ignition Keys
const int forward_input = 8; //Forward
const int reverse_input = 9; //Reverse
int charge_state = LOW; 
int main_state = LOW;
int forward_state = LOW;
int reverse_state = LOW;
int park_input = 0; //Park
float voltage = 0; //voltage value read by the arduino
float current = 0; //current value read by the arduino
boolean operation = false; //indicates whether the ignition process has occured yet

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

  charge_state = digitalRead(charge_input);
  main_state = digitalRead(main_input);
  forward_state = digitalRead(forward_input);
  reverse_state = digitalRead(reverse_input);

}

void loop() {

  //check delay times. Most important is the allRelaysOpen. Currently 1 sec 
  //As of right now, you can only be in one 'state' at a time. This means you cannot ignite the bike while charging etc
  //This also implies a priority. We will check for charging before operation etc. 
  //Some timings are estimates that I would like clarification on. Mostly Ignition
  //Some esitmated timings can be removed when voltage and current readings are implemented

  charge_state = digitalRead(charge_input);
  main_state = digitalRead(main_input);
  forward_state = digitalRead(forward_input);
  reverse_state = digitalRead(reverse_input);
    
  int park_input = park(forward_state, reverse_state);

  float voltage = (144*analogRead(A0))/1023.0; //gives the voltage.
  float current = (7.5*analogRead(A4))/1023.0; //gives the current.


  //CHARGING
  if(charge_state == HIGH && park_input == 1 && main_state == HIGH){ //Charging. The bike must be parked. The engine can be on. While Charging, you cannot do other actions //what is the voltage level we ar elooking for?
      allRelaysOpen(); //could be an issue if you want to do something else at the same time however I don't think that would be the case
      //closed relays
      digitalWrite(RL6, HIGH);
      while(charge_state == HIGH && park_input == 1 && current > 0.1){ //stops charging when current <= 0.1A
        Serial.print("Charging");
        delay(2000);
      }
      if(current < 0.1){
        allRelaysOpen();
        digitalWrite(RL1, HIGH);
      }
   }

  //IGNITION
  else if(main_state == HIGH && park_input == 1 && operation == false){ //ignition. Must be parked and not already 'ignited'
      allRelaysOpen();
      digitalWrite(RL3, HIGH); //how long should this be? is there a measurement to indicate when to stop it?
      delay(3000); //3 seconds accurate? ***********************************************************************************
      operation = true;
      Serial.print("Ignition");
  }

  //OPERATION
  else if(main_state == HIGH && operation && ((forward_state == HIGH && reverse_state == LOW) || (forward_state == LOW && reverse_state == HIGH))){ //operation. 
      //Must be not in park and the bike must have been ignited and the main power switch is in
      allRelaysOpen();
      //closed relays
      digitalWrite(RL1, HIGH);
      while(main_state == HIGH && operation && ((forward_state == HIGH && reverse_state == LOW) || (forward_state == LOW && reverse_state == HIGH))){
        Serial.print("In Operation");
        delay(2000);
      }        
  }

  //DISCHARGE
  else if(main_state == LOW && park_input == 1 && charge_state == LOW){  //discharge. Keys must leave, the bike must be in park and not charging
      //currently it will automatically discharge if parked, not charging and the bike is off. Currently would need a switch or to start the bike while charging to avoid.
      //Could add a large delay but doesnt seem like a good soltution
      allRelaysOpen();
      digitalWrite(RL4, HIGH);
      delay(5000); //Gives 5 seconds to discharge. Add measurments using the voltage and current readings later? Change Length?
      allRelaysOpen();
      operation = false; //discharging indicates the bike is no longer operating
      Serial.print("Discharged");
  }

  //IDLE
  else if(main_state == HIGH && park_input == 1){
      allRelaysOpen();
      //closed relays
      digitalWrite(RL1, HIGH);
      while(main_state == HIGH && park_input == 1 && operation == true){
        Serial.print("Idle");
        delay(2000);      
      }
      
  }

  //ERROR
  else {
    Serial.print("Not a valid state: ERROR"); //will light up error light at some point. Keeps the same relays open/closed as previous
  }
}

int park(int forward_input, int reverse_input){ //checks the triple rocker switch to see if its in the middle state
    if(forward_input == HIGH && reverse_input == HIGH){
        return 1;
    }
    else {
        return 0;
    }
}

void allRelaysOpen(){ //opens all the relays to avoid opening and closing at the same time
  digitalWrite(RL1, LOW);
  digitalWrite(RL3, LOW);
  digitalWrite(RL4, LOW);
  digitalWrite(RL6, LOW);
  delay(1000);
}
