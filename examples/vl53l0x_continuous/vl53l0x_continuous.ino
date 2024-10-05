#include <Wire.h>
#include <Adafruit_VL53L0X_SW.h>
#include <SoftWire.h>

#define USE_SOFTWIRE

// address we will assign if dual sensor is present
#define LOX_ADDRESS 0x30

// set the pins to shutdown
#define LOX1_PIN 6  // connect to SHOUT pin of the first Adafruit_VL53L0X

#ifdef USE_SOFTWIRE
  #define LOX_SDA 11  // connect NANO D11 pin to SDA pin of Adafruit_VL53L0X
  #define LOX_SCL 12  // connect NANO D12 pin to SCL pin of Adafruit_VL53L0X
#endif

char swTxBuffer[16];
char swRxBuffer[16];

Adafruit_VL53L0X_SW lox = Adafruit_VL53L0X_SW();

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }

#ifdef USE_SOFTWIRE
  SoftWire swi2c(LOX_SDA, LOX_SCL);
#endif

  Serial.println(A4);
  pinMode(LOX1_PIN, OUTPUT);

  digitalWrite(LOX1_PIN, LOW);
  delay(100);
  // all unreset
  digitalWrite(LOX1_PIN, HIGH);
  Serial.println(F("Pins are high"));

  Serial.println("Adafruit VL53L0X test");
  
#ifdef USE_SOFTWIRE
  Serial.println(F("using SOFT_WIRE"));
  swi2c.setTxBuffer(swTxBuffer, sizeof(swTxBuffer));
  swi2c.setRxBuffer(swRxBuffer, sizeof(swRxBuffer));
  swi2c.setDelay_us(5);
  swi2c.setTimeout(500);
  if (!lox.begin(LOX_ADDRESS, &swi2c)) {
#else
  Serial.println(F("using standard Wire library"));
  if (!lox.begin(LOX_ADDRESS, &Wire)) {
#endif
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
}
