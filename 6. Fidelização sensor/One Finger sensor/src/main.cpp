#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define Z_ACELAROMETER_CALIBRATION 9.8000
#define XY_ACELAROMETER_CALIBRATION 0.0000
#define NUMBER_OF_AXIS 3
#define NUMBER_OF_CALIBRATION_READ 100

float* calibrateAccelerometer();

Adafruit_MPU6050 mpu;
float acel_x[256] = {};
float acel_y[256] = {};
float acel_z[256] = {};
boolean prt;
//int i=0, j=0;

//sensor events
sensors_event_t a, g, t;

//others
bool calibration = false;
float xOffset, yOffset, zOffset;

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

    // Print acelerómetro 
    // Serial.println("Acelaration:");
    // Serial.print(a.acceleration.x + xOffset,4);
    // Serial.print(", ");
    // Serial.print(a.acceleration.y + yOffset,4);
    // Serial.print(", ");
    // Serial.println(a.acceleration.z + zOffset,4);

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

    

    // Print giroscópio /
    // Serial.println("Gyro:");
    // Serial.print(g.gyro.x,4);
    // Serial.print(", ");
    // Serial.print(g.gyro.y,4);
    // Serial.print(", ");
    // Serial.print(g.gyro.z,4);
    // Serial.println("\n");

    delay(1000);
  }
}

float* calibrateAccelerometer(){

  float* averageValue = (float*) malloc(sizeof(float) * NUMBER_OF_AXIS);
  memset(averageValue,0,sizeof(float) * NUMBER_OF_AXIS);
  //float averageValue[3] = {0,0,0};

  Serial.println("A iniciar calibração...");

  for (size_t i = 0; i < NUMBER_OF_CALIBRATION_READ; i++){
    mpu.getEvent(&a,&g,&t);
    averageValue[0] += a.acceleration.x;
    averageValue[1] += a.acceleration.y;
    averageValue[2] += a.acceleration.z;
    delay(50);
    // if (i > 1){
    //   averageValue[0] = averageValue[0]/2;  
    //   averageValue[0] = averageValue[1]/2;  
    //   averageValue[0] = averageValue[2]/2;  
    // }
    
  }

  Serial.println("Total values:");
  Serial.print("x = ");
  Serial.print(averageValue[0],4);
  Serial.print(", y = ");
  Serial.print(averageValue[1],4);
  Serial.print(", z = ");
  Serial.println(averageValue[2],4);
  Serial.println("");

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

  Serial.println("Fim da calibração");

  return averageValue;
}