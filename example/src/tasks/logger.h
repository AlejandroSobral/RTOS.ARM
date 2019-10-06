
#ifndef _LOGGER_H
#define _LOGGER_H 1

#define MaximaAceleracionGravitacional 1300 // m/s * 100
#define MaximaAceleracionAngular 125 // º/s
#define MaximaInclinacionXY 40 // º
#define MaximaHumedad 40 // %
#define MaximaTemperatura 25 // ºC
#define XYZBuf 3
#define XYBuf 2
#define NMROFLAGS 6

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
