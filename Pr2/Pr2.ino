#include <Arduino.h>

int periodoHolaMundo = 1000; 
int prioridadMundo = 1; 

int periodoLed = 200;
int prioridadLed = 2;

int led = 12;

void holaMundo(void *pvParameters);
  
//void ledMode(void *pvParameters);

void setup() {
  Serial.begin(115200);
  delay(100);
  
  xTaskCreate(holaMundo,"task1",1000,NULL,prioridadMundo,NULL);   //Nombre de la función, Nombre descriptivo para la tarea 1, este valor indica el numero de palabras que el stack puede soportar, Parametros de entrada de la función, Prioridad de la función (mayor cuanto mayor sea el número), __ 
  xTaskCreate(ledMode,"task2",12000,NULL,prioridadLed,NULL);  
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
    vTaskDelay (1000);
  }
}
  
void ledMode(void *pvParameters){         //declaro la tarea 2
  pinMode(led, OUTPUT); 
  while(1) {
    Serial.println("tarea 2");
    digitalWrite(led, HIGH); 
    vTaskDelay (periodoLed);              //¿Este delay hace que se dejen de ejecutar el resto de tareas?
    digitalWrite(led, LOW); 
    vTaskDelay (periodoLed);
  }
}
