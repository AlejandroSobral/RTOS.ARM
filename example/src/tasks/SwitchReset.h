#ifndef _GPIO_SWITCH_H
#define _GPIO_SWITCH_H 1


// ------ Public constants -----------------------------------------


// ------ Public data type -----------------------------------------


// ------ Public function prototypes -------------------------------
void Switch_Reset_Init(void);
void Switch_Reset(void);
#define SWITCH_RESET_PIN_MODE (IOCON_MODE_PULLUP)
#define SWITCH_RESET_PIN_FUNC (IOCON_FUNC0)
#define SW_PRESSED 0
#define SW_NOT_PRESSED 1
#define RESET_SWITCH_PORT (4)
#define RESET_SWITCH_PIN  (29)
#define SwitchResetON 1
#define SwitchResetOFF 0

#endif
