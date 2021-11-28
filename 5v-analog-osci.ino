// https://www.arduino.cc/en/Tutorial/BuiltInExamples/ReadAnalogVoltage

//--------------------------------------------------------------------------------------------------
// 14.11.2021 - Stephan Rau: initial version
//--------------------------------------------------------------------------------------------------
// Assumptions:
//  - max 5 V input (use voltage divider for higher voltages)
//  - do not convert to float value to have highes resolution possible (1024 equals 5 V)
//--------------------------------------------------------------------------------------------------

const byte RCV_PIN           = A0;   // analog input pin
const byte STATUS_PIN        = 13;   // on if waiting for trigger

// serial strings ----------------------------------------------------------------------------------
const String ENABLE_CMD     = "on";  // enable sniffing
const String DISABLE_CMD    = "off"; // disable sniffing
const char   CMD_END        = '\n';  // character for command end

// global variables --------------------------------------------------------------------------------
static byte    sniff_on      = 0;
static String  serialInStr;  // read string from serial port
static int     count = 0;
static int     sensorValue;
static int     lastSensorValue;
static int     sensorValueDelta;
static int     trigger_val = 500;
static int     print_cycles = 450;
static float voltage;

//--------------------------------------------------------------------------------------------------

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(500000); // use pretty high speed to see small pulses
  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, 0);
  pinMode(RCV_PIN, INPUT);
}

void loop() {

  // check and read command from PC
  if ( Serial.available() > 0 ) {
    serialInStr = Serial.readStringUntil(CMD_END);

    if ( serialInStr == DISABLE_CMD ) {
      sniff_on = 0;
      count = 0;
      digitalWrite(STATUS_PIN, 0);      
    }

    if ( serialInStr == ENABLE_CMD ) {
      sniff_on = 1;
      count = 0;
      digitalWrite(STATUS_PIN, 1);
    }
  }

  sensorValue = analogRead(RCV_PIN);

  if ( sniff_on == 1 ) {
    
    Serial.println(sensorValue);
    sensorValueDelta = lastSensorValue - sensorValue;
    sensorValueDelta = abs(sensorValueDelta);
    if ( sensorValueDelta > trigger_val ) {
      sniff_on = 2;
    }
  }

  if ( sniff_on == 2 ) {
    Serial.println(sensorValue);
    count++;
    if ( count > print_cycles ) {
      sniff_on = 0;
    }
  }

  lastSensorValue = sensorValue;
  
}
