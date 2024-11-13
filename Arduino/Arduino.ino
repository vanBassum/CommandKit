void setup() {
  // Start the Serial communication at a baud rate of 9600
  Serial.begin(115200);
  
  // Wait for Serial connection (helpful if using Serial Monitor)
  while (!Serial) ;
  
  Serial.println("Serial communication started");
}

void loop() {

}


