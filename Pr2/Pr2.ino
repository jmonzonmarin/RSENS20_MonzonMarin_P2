#include <Arduino.h>
#include <Wire.h>

#define    MPU9250_ADDRESS            0x68    //Direccion del MPU
#define    GYRO_FULL_SCALE_2000_DPS   0x18    //Escala del giroscopo de 2000º/s
#define    ACC_FULL_SCALE_2_G         0x00    //Escala del acelerometro de +/-16g
#define    A_R         ((32768.0/2.0)/9.8)

int16_t aX, aY, aZ, gX, gY, gZ, aX_offset, aY_offset, aZ_offset, gX_offset, gY_offset, gZ_offset;

int periodoDatos = 100; 
int prioridadDatos = 1; 

int periodoLed = 200;
int prioridadLed = 2;

int pinLed = 12;
int contadorDatos = 0;

int pinSCL = 22;
int pinSDA = 21;

boolean led = false;

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

  Wire.begin(pinSDA, pinSCL);
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_2_G);
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
  
  
  xTaskCreate(recogeDatos,"task1",1000,NULL,prioridadDatos,NULL);   //Nombre de la función, Nombre descriptivo para la tarea 1, este valor indica el numero de palabras que el stack puede soportar, Parametros de entrada de la función, Prioridad de la función (mayor cuanto mayor sea el número), __ 
  xTaskCreate(ledMode,"task2",1000,NULL,prioridadLed,NULL);  
  //vTaskStartScheduler();
}

void loop() {

}

void recogeDatos(void *pvParameters){         //declaro la tarea 1
  while (1){
    uint8_t aceleracion[6];
    I2Cread(MPU9250_ADDRESS, 0x3B, 6, aceleracion);
      aX = (aceleracion[0] << 8 | aceleracion[1])/A_R; //  - aX_offset;
      aY = (aceleracion[2] << 8 | aceleracion[3])/A_R; //  - aY_offset;
      aZ = (aceleracion[4] << 8 | aceleracion[5])/A_R; //  - aZ_offset;
      
    uint8_t giroscopo[6];
    I2Cread(MPU9250_ADDRESS, 0x43, 6, giroscopo);
      gX =  (giroscopo[0] << 8 | giroscopo[1]); // - gX_offset;
      gY =  (giroscopo[2] << 8 | giroscopo[3]); // - gX_offset;
      gZ =  (giroscopo[4] << 8 | giroscopo[5]); // - gX_offset;
    vTaskDelay (periodoDatos);
    contadorDatos++;
    if (contadorDatos == 10){
      Serial.print(aX,DEC);
      Serial.print(";");
      Serial.print(aY,DEC);
      Serial.print(";");
      Serial.print(aZ,DEC);
      Serial.print(";");
      Serial.print(gX,DEC);
      Serial.print(";");
      Serial.print(gY,DEC);
      Serial.print(";");
      Serial.println(gZ,DEC);
      contadorDatos = 0;
      led = true;
    }
  }
}
  
void ledMode(void *pvParameters){         //declaro la tarea 2
  pinMode(pinLed, OUTPUT); 
  while(1) {
    if (led){
      digitalWrite(led, HIGH); 
      vTaskDelay (periodoLed);              //¿Este delay hace que se dejen de ejecutar el resto de tareas?
      digitalWrite(led, LOW); 
      vTaskDelay (periodoLed);
      Serial.println("Tarea LED");
      led = false;
    }
  }
}
