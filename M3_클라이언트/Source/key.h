#ifndef __KEY_H_
#define __KEY_H_
  
  #include "M3_addr.h"
  #include "macro.h"
  #include "my_types.h"

  void Key_Poll_Init(void);
  U32 Key_Wait_Key_Pressed(void);
  void Key_Wait_Key_Released(void);
  U32 Key_Wait_Key_Pressed(void);

#endif
