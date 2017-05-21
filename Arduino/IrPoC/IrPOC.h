// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _IrPOC_H_
#define _IrPOC_H_
#include "Arduino.h"
//add your includes for the project IrPOC here


//end of add your includes here


//add your function definitions for the project IrPOC here
int listenForIR(void);
void printpulses();
boolean irCompare(int numpulses, int Signal[]);


//Do not add code below this line
#endif /* _IrPOC_H_ */
