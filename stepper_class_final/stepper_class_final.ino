#include <arduino.h>

class stepper{

    int steps_per_rev;                   //steps per revolution of the stepper motor
    int step_pin;
    int step_pin_state = LOW;
    int dir_pin;
    int ms1;                             //for switching between types of steps
    int ms2;
    int ms3;
    int dir_val;                         //for noting the current direction of rotation
    int steps = 0;                           //for counting the number of steps taken
    long double current_time = 0;
    long double previous_time = 0;
    int step_resl = 1;                    //step resolution default = 1
  public:
  
  
    stepper(int steps_per_rev, int step_pin, int dir_pin, int ms1, int ms2, int ms3) {           //constructor

    }

    void attach_stepper() {
      
      pinMode(step_pin, OUTPUT);
      pinMode(dir_pin, OUTPUT);
      pinMode(ms1, OUTPUT);           
      pinMode(ms2, OUTPUT);
      pinMode(ms3, OUTPUT);
      
    }

    void step_resolution(int step_res) {
      step_resl = step_res;                                                                     //setting step resolution
      switch (step_res) {                                                                      
        case 1 : digitalWrite(ms1, LOW); digitalWrite(ms2, LOW); digitalWrite(ms3, LOW);          //full step
          break;
        case 2 : digitalWrite(ms1, HIGH); digitalWrite(ms2, LOW); digitalWrite(ms3, LOW);         //half step
          break;
        case 4 : digitalWrite(ms1, LOW); digitalWrite(ms2, HIGH); digitalWrite(ms3, LOW);         //quarter step
          break;
        case 8 : digitalWrite(ms1, HIGH); digitalWrite(ms2, HIGH); digitalWrite(ms3, LOW);        //one eighth step
          break;
      }
    }

    float interv_between_steps(float Joy_val) {            //delay time interval between each pulse to the step pin;

      float rps = 10 * abs(Joy_val);
      float steps_p_sec = steps_per_rev * rps*step_resl;
      long double interval = 1000000 * steps_p_sec/2;             //this interval is in microseconds
      return interval;
    }

    void dir(float Joy_val) {                         //Joy_val is the fraction (-1 to 1)
      int val;
      if (Joy_val > 0) {                            //checking the direction of rotation
        val = HIGH;
      }
      if (Joy_val < 0) {
        val = LOW;
      }
      digitalWrite(dir_pin, val);                  //for setting direction of rotation
      dir_val = val;
    }

    void drive(float Joy_val) {
      
      long double interval = interv_between_steps(Joy_val);
      dir(Joy_val);                                     //initialising the direction of rotation
      current_time = micros();                          //storing the current time in microseconds
      if (current_time - previous_time >= interval) {   //previous_time is the last time a pulse was given to the step pin
                                                      
    previous_time = current_time;

                                                      // if step pin is high, turn it to low and vice-versa
    if (step_pin_state == LOW) {
      step_pin_state = HIGH;
    } else {
      step_pin_state = LOW;
      if(dir_val == HIGH) {
        steps++;
      }else if(dir_val == LOW){                       //counting the number of steps
        steps--;
      }
      
    }

    
    digitalWrite(step_pin, step_pin_state);
  }


    }

    void debug() {                                       //debugging
      Serial.println("Number of steps = ");
      Serial.println(steps);                             //prints total no. of positive steps taken
      Serial.println('\n');
      Serial.println("Direction of rotation = ");
      Serial.println(dir_val);                           //prints the current direction of rotation
      Serial.println('\n');
      Serial.print((long int)current_time);              //current time as integer
      

    }

};

 stepper s(200, 9, 10, 11, 12, 13);
void setup() {
  pinMode(5, OUTPUT);
 pinMode(6, OUTPUT);
 digitalWrite(6,HIGH);
 Serial.begin(9600);
  }
  
void loop() {
    digitalWrite(5, HIGH);
    delay(10);
    digitalWrite(5, LOW);
    delay(10);
    Serial.println(digitalRead(5));
    s.debug();
}
