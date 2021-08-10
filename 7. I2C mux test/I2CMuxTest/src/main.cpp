#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define NUMBER_OF_SENSORS 1
#define NUMBER_OF_BUS 1
#define NUMBER_OF_AXIS 3
#define NUMBER_OF_CALIBRATION_READ 100

#define Z_ACELAROMETER_CALIBRATION 9.8000
#define XY_ACELAROMETER_CALIBRATION 0.0000

// Mux address
#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

float* calibrateAccelerometer(Adafruit_MPU6050 *mpu);

Adafruit_MPU6050 mpu[2];

int sel = 0 ;

//sensor events
sensors_event_t a, g, t;

//others
bool calibration = false;
float xOffset[NUMBER_OF_SENSORS], yOffset[NUMBER_OF_SENSORS], zOffset[NUMBER_OF_SENSORS];

// standard Arduino setup()
void setup()
{
    while (!Serial);
    delay(1000);

    Wire.begin();
    
    Serial.begin(115200);
    Serial.println("\nTCAScanner ready!");
    
    // for (uint8_t t=0; t<8; t++) {
    //   tcaselect(t);
    //   Serial.print("TCA Port #"); Serial.println(t);

    //   for (uint8_t addr = 0; addr<=127; addr++) {
    //     if (addr == TCAADDR) continue;

    //     Wire.beginTransmission(addr);
    //     if (!Wire.endTransmission()) {
    //       Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
    //     }
    //   }
    // }
    // Serial.println("done\n");

  for(int i = 0; i < NUMBER_OF_BUS; i++){
    //tcaselect(i);

  while (!mpu[0].begin(0x68)) {// para endereço 0x69 AD0 tem que estar ligado a VCC
    Serial.println("Failed to find MPU6050 chip");
    delay(10);
    }

    // while (!mpu[1].begin(0x69)) {// para endereço 0x69 AD0 tem que estar ligado a VCC
    // Serial.println("Failed to find MPU6050 chip");
    // delay(10);
    // }
  }

  for (size_t i = 0; i < NUMBER_OF_SENSORS; i++){
    float *temp = calibrateAccelerometer(&mpu[i]);
      xOffset[i] = temp[0];
      yOffset[i] = temp[1];
      zOffset[i] = temp[2];
  }
  
  

  //selecionar os ranges e filtro
  mpu[0].setAccelerometerRange(MPU6050_RANGE_8_G);
  //mpu[1].setAccelerometerRange(MPU6050_RANGE_8_G);
    //Serial.println(mpu.getAccelerometerRange());
  mpu[0].setGyroRange(MPU6050_RANGE_500_DEG);
  //mpu[1].setGyroRange(MPU6050_RANGE_500_DEG);
    //Serial.println(mpu.getGyroRange());
  mpu[0].setFilterBandwidth(MPU6050_BAND_10_HZ);
  //mpu[1].setFilterBandwidth(MPU6050_BAND_10_HZ);
    //Serial.println(mpu.getFilterBandwidth());

}

void loop() {

  tcaselect(0);
  delay(100);
  mpu[sel].getEvent(&a, &g, &t);
  // Print acelerómetro 
  Serial.printf("Acelaration I2C %d:\n",sel);
  Serial.print(a.acceleration.x + xOffset[sel], 4);
  Serial.print(", ");
  Serial.print(a.acceleration.y + yOffset[sel], 4);
  Serial.print(", ");
  Serial.println(a.acceleration.z + + zOffset[sel], 4);
  Serial.println("");

  //sel = sel != 0 ? 0 : 1;

  // if (sel == 0) sel++;
  //   else sel = 0;
  

  delay(2000);
}

/*

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Try to initialize!
  while (!mpu.begin(0x69)) {// para endereço 0x69 AD0 tem que estar ligado a VCC
    Serial.println("Failed to find MPU6050 chip");
    delay(10);
  }
  //selecionar os ranges e filtro
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    //Serial.println(mpu.getAccelerometerRange());
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    //Serial.println(mpu.getGyroRange());
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
    //Serial.println(mpu.getFilterBandwidth());

  Serial.println("--------------Loop start--------------");
  Serial.println();
}

void loop() {

  if (!calibration){
    float* test = calibrateAccelerometer();
    xOffset = test[0] > XY_ACELAROMETER_CALIBRATION ? -test[0] : test[0]; 
    yOffset = test[1] > XY_ACELAROMETER_CALIBRATION ? -test[1] : test[1]; 
    zOffset = test[2] > Z_ACELAROMETER_CALIBRATION ? -(test[2] - Z_ACELAROMETER_CALIBRATION) : Z_ACELAROMETER_CALIBRATION - test[2]; 
    calibration = true;

    Serial.print("Offset Value:\nx = ");
    Serial.print(xOffset,4);
    Serial.print(", y = ");
    Serial.print(yOffset,4);
    Serial.print(", z = ");
    Serial.println(zOffset,4);
    Serial.println("");
 
    Serial.println("Acelaration:");
    Serial.print(a.acceleration.x + xOffset,4);
    Serial.print(", ");
    Serial.print(a.acceleration.y + yOffset,4);
    Serial.print(", ");
    Serial.println(a.acceleration.z + zOffset,4);
  
  }else{
    // put your main code here, to run repeatedly:
    mpu.getEvent(&a, &g, &t); 

    if((a.acceleration.z + zOffset) > 8)
      Serial.println("Posição inicial \n");
    else if ((a.acceleration.y + yOffset) < -9)
      Serial.println("Posição para baixo \n");
    else if ((a.acceleration.y + yOffset) > 6.5)// && (a.acceleration.z + zOffset) < 5)
      Serial.println("Posição para cima\n");
    else{
      Serial.println("Erro:");
      Serial.print(a.acceleration.x + xOffset,4);
      Serial.print(", ");
      Serial.print(a.acceleration.y + yOffset,4);
      Serial.print(", ");
      Serial.println(a.acceleration.z + zOffset,4);
    }

    delay(1000);
  }
}*/

float* calibrateAccelerometer(Adafruit_MPU6050 *mpu){

  float* averageValue = (float*) malloc(sizeof(float) * NUMBER_OF_AXIS);
  memset(averageValue,0,sizeof(float) * NUMBER_OF_AXIS);

  for (size_t i = 0; i < NUMBER_OF_CALIBRATION_READ; i++){
    mpu->getEvent(&a,&g,&t);
    averageValue[0] += a.acceleration.x;
    averageValue[1] += a.acceleration.y;
    averageValue[2] += a.acceleration.z;
    delay(50);    
  }

  for (size_t i = 0; i < NUMBER_OF_AXIS; i++)
    averageValue[i] = averageValue[i]/NUMBER_OF_CALIBRATION_READ;
  
  Serial.println("Average values:");
  Serial.print("x = ");
  Serial.print(averageValue[0],4);
  Serial.print(", y = ");
  Serial.print(averageValue[1],4);
  Serial.print(", z = ");
  Serial.println(averageValue[2],4);
  Serial.println("");

  averageValue[0] = 
                  averageValue[0] > XY_ACELAROMETER_CALIBRATION ? -averageValue[0] : averageValue[0]; 
  averageValue[1] = 
                  averageValue[1] > XY_ACELAROMETER_CALIBRATION ? -averageValue[1] : averageValue[1]; 
  averageValue[2] = 
                  averageValue[2] > Z_ACELAROMETER_CALIBRATION ? -(averageValue[2] - Z_ACELAROMETER_CALIBRATION) : Z_ACELAROMETER_CALIBRATION - averageValue[2];

  return averageValue;
}