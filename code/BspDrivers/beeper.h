/*
 *  key driver file
 */

#ifndef __BEEPER_H
#define __BEEPER_H

#ifdef __cplusplus
 extern "C" {
#endif

#define BEEPER_ENABLE 1
#define BEEPER_DISABLE 0

extern void beeper_init(void);
extern void beeper_control(uint8_t status);


#ifdef __cplusplus
}
#endif


#endif
