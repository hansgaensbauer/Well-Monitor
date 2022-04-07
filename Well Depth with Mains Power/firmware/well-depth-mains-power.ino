/*
 * Engineers Without Borders San Francisco Professional Chapter
 * 
 * Well Monitor Firmware with well depth sensing and mains power
 * See https://github.com/hansgaensbauer/Well-Monitor
 */

 //includes
 //#include <SPI.h>
 //#include <SD.h>
 //#include <MCP7940.h>  // Include the MCP7940 RTC library
 //#include "well-depth-mains-power.h" //custom h file with board-specific pin defines
// #include "TMP117.h"  //REMOVE

 //TMP117 tmp(0x48);   //REMOVE

 //MCP7940_Class MCP7940;                           // Create an instance of the MCP7940
// char inputBuffer[SPRINTF_BUFFER_SIZE];  // Buffer for sprintf()/sscanf()

 void setup() {
  //pin configuration
  pinMode(LEDPIN, OUTPUT);
  
  Serial.begin(9600);
  while(!Serial); //wait for Serial
  Serial.println("Checking Power Fail");

/*
  if (!SD.begin(SD_CS_N)){
    Serial.println("SD Initialization Failed");
    //flash the LED forever to indicate SD card failure
    sd_fail_handler();
  }

  //start the CSV file
  File logfile = SD.open("welldata.csv", FILE_WRITE);
  if (logfile){
    logfile.println("Sample Date, Sample Timestamp, Well Depth (m)");
    
  } else {
    sd_fail_handler(); //fail
  }
  while (!MCP7940.begin()); //Wait for the RTC

  //MCP7940 Code taken directly from SimpleBatteryBackup example in MCP7940 library
 
  if (MCP7940.getPowerFail()) {  // Check for a power failure
    Serial.println("Power failure mode detected!\n");
    Serial.print("Power failed at   ");
    DateTime now = MCP7940.getPowerDown();                      // Read when the power failed
    sprintf(inputBuffer, "....-%02d-%02d %02d:%02d:..",         // Use sprintf() to pretty print
            now.month(), now.day(), now.hour(), now.minute());  // date/time with leading zeros
    Serial.println(inputBuffer);

    //log the failure to the SD card
    logfile.print("Power failed at: ");
    logfile.println(inputBuffer);
    
    Serial.print(F("Power restored at "));
    now = MCP7940.getPowerUp();                                 // Read when the power restored
    sprintf(inputBuffer, "....-%02d-%02d %02d:%02d:..",         // Use sprintf() to pretty print
            now.month(), now.day(), now.hour(), now.minute());  // date/time with leading zeros
    Serial.println(inputBuffer);
    MCP7940.clearPowerFail();  // Reset the power fail switch
  } else {
    while (!MCP7940.deviceStatus()) {  // Turn oscillator on if necessary
      Serial.println(F("Oscillator is off, turning it on."));
      bool deviceStatus = MCP7940.deviceStart();  // Start oscillator and return state
      if (!deviceStatus) {                        // If it didn't start
        Serial.println(F("Oscillator did not start, trying again."));  // Show error and
        delay(1000);                                                   // wait for a second
      }                // of if-then oscillator didn't start
    }                  // of while the oscillator is off
    //determine whether the 
    MCP7940.adjust();  // Set to library compile Date/Time    
    Serial.println(F("Enabling battery backup mode"));
    MCP7940.setBattery(true);     // enable battery backup mode
    if (!MCP7940.getBattery()) {  // Check if successful
      Serial.println(F("Couldn't set Battery Backup, is this a MCP7940N?"));
    }                        // if-then battery mode couldn't be set
  }                          // of if-then-else we have detected a priorpower failure
  
  logfile.close();
*/
}

void loop() {
  delay(2000); //wait approximately one minute
  record();
}

/*
 * Samples well depth and time and writes a line to the SD card
 */
void record(){
  /*
  float temp = tmp.getTemperature();
  
  DateTime now = MCP7940.now();  // get the current time
  sprintf(inputBuffer, "%04d-%02d-%02d, %02d:%02d:%02d, ",
        now.year(),  // Use sprintf() to pretty print
        now.month(), now.day(), now.hour(), now.minute(),
        now.second());                         // date/time with leading zeros
  
  File logfile = SD.open("welldata.csv", FILE_WRITE);
  if (logfile){
    logfile.print(inputBuffer);
    logfile.print(temp);
    logfile.println();
    logfile.close();
  } else {
    sd_fail_handler(); //fail
  }
  */
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
  float current = PRESSURE_RANGE * (((rawreading / ADC_BITS) * VCC)/SENSE_RESISTANCE);

  if(current < 0.0035 || current > 0.021){  //check if the current is within acceptable range
    return -1;
  }

  float pressure = PRESSURE_RANGE * (current - ZERO_SCALE_CURRENT) / (FULL_SCALE_CURRENT - ZERO_SCALE_CURRENT);

  float well_depth = pressure * PSI_TO_MH2O;
  return well_depth;
}
