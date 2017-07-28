#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>

#define PIXELPIN 5
#define PIXELCOUNT 1
// The Feather M0 will not proceed past the serial print if no serial USB is connected. Undefine this to make it work when not connected to a computer.
// #define SERDEBUG 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void)
{
#ifndef ESP8266
  // Unfortunately this prevents the device from working standalone. Uncomment for debugging.
  // while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
#ifdef SERDEBUG
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");
#endif
  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
#ifdef SERDEBUG
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
#endif
    while (1);
  }

  /* Display some basic information on this sensor */
#ifdef SERDEBUG
  displaySensorDetails();
#endif


  // Neopixel initialization
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  int maxacc;
  accel.getEvent(&event);

#ifdef SERDEBUG
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m/s^2 ");

  /* Note: You can also get the raw (non unified values) for */
  /* the last data sample as follows. The .getEvent call populates */
  /* the raw values used below. */
  Serial.print("X Raw: "); Serial.print(accel.raw.x); Serial.print("  ");
  Serial.print("Y Raw: "); Serial.print(accel.raw.y); Serial.print("  ");
  Serial.print("Z Raw: "); Serial.print(accel.raw.z); Serial.println("");
#endif

  // Find the dominant direction, pick color based on it.
  maxacc = abs(event.acceleration.x);
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  if (abs(event.acceleration.y) > maxacc) {
    strip.setPixelColor(0, strip.Color(0, 255, 0));
    maxacc = abs(event.acceleration.y);
  }
  if (abs(event.acceleration.z) > maxacc) {
    strip.setPixelColor(0, strip.Color(0, 0, 255));
  }

  strip.show();

  /* Delay before the next sample */
  delay(50);
}
