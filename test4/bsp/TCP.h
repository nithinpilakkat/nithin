/****************************************************************************************/
/*                 PROJECTNAME       =              SmartNav             */
/*                 FILENAME                =              TCP.h                      */
/*                 WORKSPNAME       =              SmartNave              */
/*                 DESIGN ENG           =              NITHIN P                             */
/*                 DATE                        =             May 27, 2014                    */
/***************************************************************************************/

#ifndef  __TCP_h_
#define __TCP_h_

#include "Sim900.h"
#define Sec(a)  (a * 10)

enum FnState
{
	FnEnter,
	FnExit,
};
typedef struct {
	enum FnState  Function;
	unsigned long Delay;
	
}FnDelay;

extern volatile FnDelay   TCP_Delay;

extern volatile FnDelay CIPSTATUS_Delay;
#endif






