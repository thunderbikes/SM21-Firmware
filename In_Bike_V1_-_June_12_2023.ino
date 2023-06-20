//Control Pins
const int charge_input = 50;
const int pump_input = 52;
const int main_input = 48;
const int reverse_input = 44;
const int forward_input = 40;
const int charge_safety_input = 53; //5V is OFF for this on
const int discharge_enable_input = 39; //5V is OFF for this on
const int MP03_input = 45;
const int MP02_input = 41;
const int MP01_input = 51;
const int MP_enable_input = 35;
const int throttle_output = 32;
const int pump_output = 34;

//ADC Pins
const int RL2_ADC = A0;
const int RL4_ADC = A2;
const int RL6_ADC = A3;
const int RL5_ADC = A7;
const int RL3_ADC = A8;
const int RL1_ADC = A5;
const int pump_ADC = A4; 

//Power Pins
const int RL5_output = 3; //Charging
const int RL3_output = 2; //Ignition 
const int RL4_output = 4; //Discharge
const int RL2_output = 5; //Operation
const int RL6_output = 7; //Charging
const int RL1_output = 8; //Operation


//States
int charge_state = LOW; 
int main_state = LOW;
int forward_state = LOW;
int reverse_state = LOW;
int park_input = 0; //Park
int voltage = 0; //voltage value read by the arduino
boolean operation = false; //indicates whether the ignition process has occured yet

void setup() {
  pinMode(RL1_output, OUTPUT);
  pinMode(RL2_output, OUTPUT);
  pinMode(RL3_output, OUTPUT);
  pinMode(RL4_output, OUTPUT);
  pinMode(RL5_output, OUTPUT);
  pinMode(RL6_output, OUTPUT);
  pinMode(throttle_output, OUTPUT);
  pinMode(pump_output, OUTPUT);
  pinMode(charge_input, INPUT);
  pinMode(main_input, INPUT);
  pinMode(forward_input, INPUT);
  pinMode(reverse_input, INPUT);
  pinMode(pump_input, INPUT);
  pinMode(charge_safety_input, INPUT);
  pinMode(discharge_enable_input, INPUT);
  pinMode(MP03_input, INPUT);
  pinMode(MP02_input, INPUT);
  pinMode(MP01_input, INPUT);
  pinMode(MP_enable_input, INPUT);
  pinMode(MP_enable_input, INPUT);

  Serial.begin(115200);
  allRelaysOpen();
  switch_read();

  if(charge_state == HIGH || park(forward_state,reverse_state) == 0 || main_state == 1){ 
    while(charge_state == HIGH || park(forward_state,reverse_state) == 0 || main_state == 1){
      switch_read();  
      Serial.println("A switch is on during startup. Set every switch to off to continue.");
      delay(500);
    }
  }

}

void loop() {

  //check delay times. Most important is the allRelaysOpen. Currently 1 sec 
  //As of right now, you can only be in one 'state' at a time. This means you cannot ignite the bike while charging etc
  //This also implies a priority. We will check for charging before operation etc. 
  //Some timings are estimates that I would like clarification on. Mostly Ignition
  //Some esitmated timings can be removed when voltage and current readings are implemented
 
  switch_read();
    
  int park_input = park(forward_state, reverse_state);

  int voltage = (144*5*analogRead(A0))/1024; //gives the voltage. Is the 144 still accurate with the less modules

  //CHARGING
  if(charge_state == HIGH && park(forward_state,reverse_state) == 1 && main_state == 0 ){ //Charging. The bike must be parked. The engine can be on. While Charging, you cannot do other actions //what is the voltage level we ar elooking for?
      allRelaysOpen(); //could be an issue if you want to do something else at the same time however I don't think that would be the case
      delay(1000);
      //closed relays
      digitalWrite(RL6_output, HIGH);
      digitalWrite(RL5_output, HIGH);//Check with BMS to see if this can be turned on, NEED TO ADD THAT FEATURE
      while(charge_state == HIGH && park(forward_state,reverse_state) == 1 && main_state == 0){ //add voltage and current readings to ensure it will not continue to charge after reaching max voltage/current
        switch_read();  
        Serial.println("Charging");
        delay(500);
      }
      allRelaysOpen();
   }
       
  //IGNITION
  else if(main_state == HIGH && park(forward_state,reverse_state) == 1 && charge_state == 0 && operation == false){ //ignition. Must be parked and not already 'ignited'
      allRelaysOpen();
      digitalWrite(RL3_output, HIGH);
      //how long should this be? is there a measurement to indicate when to stop it?
      Serial.println("Ignition started");
      delay(5000); //5 seconds to be sure
      operation = true;
      Serial.println("Ignition completed");
      allRelaysOpen();  
      switch_read();
}

//fwd=low,reverse=high = reverse
//fwd=high,reverse=low = fwd
//
  //OPERATION
  else if(main_state == HIGH  && operation == true && park(forward_state,reverse_state) == 0 && charge_state == 0){ //operation.
   
      //Must be not in park and the bike must have been ignited and the main power switch is in
      digitalWrite(RL3_output, LOW); //I know the function allRelaysOpen() can be called, but that would ever so slightly discharge the capacitor, so it's not being called to prevent that
      digitalWrite(RL4_output, LOW);
      digitalWrite(RL5_output, LOW);
      digitalWrite(RL6_output, LOW);
      //closed relays
      digitalWrite(RL1_output, HIGH);
      digitalWrite(RL2_output, HIGH); //Check with BMS to see if this can be turned on, NEED TO ADD THAT FEATURE
      digitalWrite(throttle_output, LOW);
      while(main_state == HIGH  && operation == true && park(forward_state,reverse_state) ==0 && charge_state == 0){
        switch_read();
        Serial.println("In Operation");
        delay(500);
      } 
  }

  //DISCHARGE
  else if(main_state == LOW && park(forward_state,reverse_state) == 1 && charge_state == LOW && operation == true)  {  //discharge. Keys must leave, the bike must be in park and not charging
      //currently it will automatically discharge if parked, not charging and the bike is off. Currently would need a switch or to start the bike while charging to avoid.
      //Could add a large delay but doesnt seem like a good soltution
      operation = false; //discharging indicates the bike is no longer operating
      allRelaysOpen();
      digitalWrite(RL4_output, HIGH);
      Serial.println("Discharge started");
      //while voltage and/or current is above 0?
      delay(90000); //Gives 90 seconds to discharge. Add measurments using the voltage and current readings later
      allRelaysOpen();
      Serial.println("Discharge ended");
  }

  //EDGE CASES
  else if(main_state == HIGH && park(forward_state,reverse_state) == 1 && charge_state ==0 && operation == true){ //parking after driving but leaving the bike on, RL1 and RL2 are closed to prevent capacitor from discharging
      main_state = digitalRead(main_input);
      digitalWrite(RL3_output, LOW); //I know the function allRelaysOpen() can be called, but that would ever so slightly discharge the capacitor, so it's not being called to prevent that
      digitalWrite(RL4_output, LOW);
      digitalWrite(RL5_output, LOW);
      digitalWrite(RL6_output, LOW);
      digitalWrite(RL1_output, HIGH);
      digitalWrite(RL2_output, HIGH);
      while(main_state == HIGH && park(forward_state, reverse_state) == 1 && charge_state == 0 && operation == true){
        forward_state = digitalRead(forward_input);
        reverse_state = digitalRead(reverse_input);
        delay(500);
        switch_read();
        //does not change any relays but is a required state. Want to account for edge cases
        Serial.println("In park but Bike stays on");
      }
  }


  else if (main_state == LOW && park(forward_state,reverse_state) == 1 && charge_state == 0 && operation ==false){
         while (main_state == LOW && park(forward_state,reverse_state) == 1 && charge_state == 0 && operation == false) {
        delay(500);
        switch_read();
        Serial.println("All switches off");
         }
  }
  //ERROR
  else {
    delay(1000);
    Serial.println("Not a valid state: ERROR"); //will light up error light at some point

  }
}

int park(int forward_state, int reverse_state){ //checks the triple rocker switch to see if its in the middle state
    if(forward_state == LOW && reverse_state == LOW){
        return 1;
    }
    else {
        return 0;
    }
}

void allRelaysOpen(){ //opens all the relays to avoid opening and closing at the same time
  digitalWrite(RL1_output, LOW);
  digitalWrite(RL2_output, LOW);
  digitalWrite(RL3_output, LOW);
  digitalWrite(RL4_output, LOW);
  digitalWrite(RL5_output, LOW);
  digitalWrite(RL6_output, LOW);
  digitalWrite(throttle_output, HIGH); //HIGH is off for the PMOS 
  delay(1000);
}

void switch_read(){
       charge_state = digitalRead(charge_input);
       main_state = digitalRead(main_input);
       forward_state = digitalRead(forward_input);
       reverse_state = digitalRead(reverse_input);
}
