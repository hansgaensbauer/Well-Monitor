/*
 * Engineers Without Borders San Francisco Professional Chapter
 * 
 * Well Monitor Firmware with well depth sensing and mains power
 * See https://github.com/hansgaensbauer/Well-Monitor
 */

 //includes
 #include <SPI.h>
 #include <SD.h>
 #include "well-depth-mains-power.h" //custom h file with board-specific pin defines

 void setup() {
  //pin configuration
  pinMode(LEDPIN, OUTPUT);
  
  Serial.begin(9600);
  while(!Serial); //wait for Serial

  if (!SD.begin(SD_CS_N)){
    Serial.println("SD Initialization Failed");

    //flash the LED forever to indicate SD card failure
    sd_fail_handler();
  }

  //start the CSV file
  File logfile = SD.open("welldata.csv", FILE_WRITE);
  if (logfile){
    logfile.println("Sample Time (ms), Well Depth (m)");
    logfile.close();
  } else {
    sd_fail_handler(); //fail
  }
}

void loop() {
  delay(1000);
  record();
}

/*
 * Samples well depth and time and writes a line to the SD card
 */
void record(){
  float depth = measure_well_depth();
  int sample_time = millis();
  
  File logfile = SD.open("welldata.csv", FILE_WRITE);
  if (logfile){
    logfile.print(sample_time);
    logfile.print(", ");
    logfile.print(depth);
    logfile.println();
    logfile.close();
  } else {
    sd_fail_handler(); //fail
  }
}

/*
 * Flashes the LED to alert the user to an SD card failure
 */
void sd_fail_handler(){
  while(true){
    delay(100);
    digitalWrite(LEDPIN, HIGH);
    delay(100);
    digitalWrite(LEDPIN, LOW);
  }
}

/*
 * Returns the well depth in meters, using the constants in the header file.
 */
float measure_well_depth(){
  int rawreading = analogRead(DEPTH_SENSE_INPUT);
  float current = PRESSURE_RANGE * (((rawreading / ADC_BITS) * VCC)/SENSE_RESISTANCE)

  if(current < 0.0035 || current > 0.021){  //check if the current is within acceptable range
    return -1;
  }

  float pressure = PRESSURE_RANGE * (current - ZERO_SCALE_CURRENT) / (FULL_SCALE_CURRENT - ZERO_SCALE_CURRENT);

  float well_depth = pressure * PSI_TO_MH2O;
  return well_depth;
}
