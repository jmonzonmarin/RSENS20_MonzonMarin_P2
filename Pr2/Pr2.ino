#include <Arduino.h>
#include <Wire.h>

#define    MPU9250_ADDRESS            0x68    //Direccion del MPU
#define    GYRO_FULL_SCALE_2000_DPS   0x18    //Escala del giroscopo de 2000º/s
#define    ACC_FULL_SCALE_16_G        0x18    //Escala del acelerometro de +/-16g

int16_t aX, aY, aZ, gX, gY, gZ, aX_offset, aY_offset, aZ_offset, gX_offset, gY_offset, gZ_offset;

int periodoHolaMundo = 1000; 
int prioridadMundo = 1; 

int periodoLed = 200;
int prioridadLed = 2;

int led = 12;

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
   Wire.beginTransmission(Address);       //Indica la dirección del esclavo al que me quiero dirigir
   Wire.write(Register);                  //Indico el registro con el que me quiero comunicar
   Wire.endTransmission();
 
   Wire.requestFrom(Address, Nbytes);     //Leo del esclavo que he seleccionado Nbytes
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();        //Almaceno la lectura en un vector (Data).
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
   Wire.beginTransmission(Address);       //Inicio la comunicación con el esclavo que quiero
   Wire.write(Register);                  //Accedo al registro en el que quiero escribir
   Wire.write(Data);                      //Escribo los datos pertinentes. (Normalmente la configuración del registro)
   Wire.endTransmission();
}


void setup() {
  Serial.begin(115200);
  delay(100);

  Wire.begin();
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
  
  
  xTaskCreate(holaMundo,"task1",1000,NULL,prioridadMundo,NULL);   //Nombre de la función, Nombre descriptivo para la tarea 1, este valor indica el numero de palabras que el stack puede soportar, Parametros de entrada de la función, Prioridad de la función (mayor cuanto mayor sea el número), __ 
  xTaskCreate(ledMode,"task2",1000,NULL,prioridadLed,NULL);  
  //vTaskStartScheduler();
}

void loop() {
//  while (1){
//    // put your main code here, to run repeatedly:
//  }
    //delay(1000);

}

void holaMundo(void *pvParameters){         //declaro la tarea 1
  while (1){
    Serial.println("Hola mundo");
    vTaskDelay (periodoHolaMundo);
  }
}
  
void ledMode(void *pvParameters){         //declaro la tarea 2
  pinMode(led, OUTPUT); 
  while(1) {
    digitalWrite(led, HIGH); 
    vTaskDelay (periodoLed);              //¿Este delay hace que se dejen de ejecutar el resto de tareas?
    digitalWrite(led, LOW); 
    vTaskDelay (periodoLed);
  }
}
