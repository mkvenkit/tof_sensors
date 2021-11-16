/*
This example shows how to take simple range measurements with the VL53L1X. The
range readings are in units of mm.
*/

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

#define N 12
#define SZ N*N
#define WSZ 5

// 12 x 12 distance grid 
uint16_t data[SZ];

// We're going to set the maximum number of 5x5 regions 
// which means (16 - 5 + 1)^2 = 12 x 12 = 144 ROIs

#if 1
uint16_t roi_indices[] = {18, 26, 34, 42, 50, 58, 66, 74, 82, 90, 98, 106, 
                          19, 27, 35, 43, 51, 59, 67, 75, 83, 91, 99, 107,
                          20, 28, 36, 44, 52, 60, 68, 76, 84, 92, 100, 108, 
                          21, 29, 37, 45, 53, 61, 69, 77, 85, 93, 101, 109,
                          22, 30, 38, 46, 54, 62, 70, 78, 86, 94, 102, 110,
                          23, 31, 39, 47, 55, 63, 71, 79, 87, 95, 103, 111,
                          239, 231, 223, 215, 207, 199, 191, 183, 175, 167, 159, 151, 
                          238, 230, 222, 214, 206, 198, 190, 182, 174, 166, 158, 150,
                          237, 229, 221, 213, 205, 197, 189, 181, 173, 165, 157, 149,
                          236, 228, 220, 212, 204, 196, 188, 180, 172, 164, 156, 148,
                          235, 227, 219, 211, 203, 195, 187, 179, 171, 163, 155, 147,
                          234, 226, 218, 210, 202, 194, 186, 178, 170, 162, 154, 146};
#else
uint16_t roi_indices[] = { 36, 44, 52, 60, 68, 76, 84, 92,
                        37, 45, 53, 61, 69, 77, 85, 93,
                        38, 46, 54, 62, 70, 78, 86, 94, 
                        39, 47, 55, 63, 71, 79, 87, 95,
                        223, 215, 207, 199, 191, 183, 175, 167,
                        222, 214, 206, 198, 190, 182, 174, 166, 
                        221, 213, 205, 197, 189, 181, 173, 165,
                        220, 212, 204, 196, 188, 180, 172, 164};
                        
#endif 

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  
  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(20000);

  sensor.setROISize(WSZ, WSZ);
  
  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(21);

  for (int i = 0; i < SZ; i++) {
    data[i] = 0;
  }
}

void loop()
{
  for (int i = 0; i < SZ; i++) {
    sensor.setROICenter(roi_indices[i]);
    data[i] = (uint16_t)sensor.read();
    //Serial.println(data[i]);
  }
  //Serial.print(sensor.read());
  Serial.write((uint8_t*)data, 2*SZ);
  
  //if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  //Serial.println();
  //delay(50);
}
