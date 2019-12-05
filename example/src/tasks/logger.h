
#ifndef _LOGGER_H
#define _LOGGER_H 1

#define MaximaAceleracionGravitacional 17500 // m/s * 100 = 2,5G
#define MaximaAceleracionAngular 800 // º/s
#define MaximaInclinacionXY 40 // º
#define MaximaHumedad 85 // %
#define MaximaTemperatura 30 // ºC
#define XYZBuf 2 // Z lo miro para ver si giró
#define XYBuf 2
#define NMROFLAGS 7

#define FlagAceleracionAngular 		0
#define FlagAceleracion 			1
#define FlagAceleracionOrientacion 	2
#define FlagAceleracionHumedad 		3
#define FlagAceleracionTemperatura 	4
#define FlagAceleracionBateria		5

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
