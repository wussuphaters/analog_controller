#define outputA 14
#define outputB 27
int counter = 0; 
int aState;
int aLastState;

void setup() { 
 pinMode (outputA,INPUT);
 pinMode (outputB,INPUT);
 
 Serial.begin (9600);
 // Reads the initial state of the outputA
 aLastState = digitalRead(outputA);   
} 
void loop() { 
 aState = digitalRead(outputA); // Reads the "current" state of the outputA
 if (aState != aLastState){     
   // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
   if (digitalRead(outputB) != aState) { 
     counter ++;
   } else {
     counter --;
   }
   Serial.print("Position: ");
   Serial.println(counter);
 } 
 aLastState = aState; // Updates the previous state of the outputA with the current state
}
