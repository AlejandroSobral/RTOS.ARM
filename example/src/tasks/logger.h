
#ifndef _LOGGER_H
#define _LOGGER_H 1

#define MaximaAceleracionGravitacional 18000 // m/s * 100 = 2G
#define MaximaAceleracionAngular 800 // º/s
#define MaximaInclinacionXY 40 // º
#define MaximaHumedad 85 // %
#define MaximaTemperatura 35 // ºC
#define XYZBuf 2 // Z lo miro para ver si giró
#define XYBuf 2
#define NMROFLAGS 7

#define AceleracionAngularExcedida 		0
#define AceleracionExcedida 			1
#define AnguloExcedido 					2
#define ExcesoHumedad 					3
#define ExcesoTemperatura 				4
#define BateriaBaja						5
#define Invertido180 					6

//0 = AceleracionAngular
//1=Aceleracion
//2=Orientacion
//3= Humedad
//4= Temperatura
//5=Bateria


// ------ Public data type declarations ----------------------------

// ------ Public function prototypes -------------------------------

void Logger(void);



#endif
