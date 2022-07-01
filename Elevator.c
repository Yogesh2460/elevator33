/******************************** ELEVATER INTERFACE ********************************************************

OBJECT:		 	To demonstrate the working of Elevater Interface.

CONNECTIONS: 	Connect the Elevater Interface to 26pin FRC connector cable given by ESA
		        	on ESA KEIL MCB1760 board. 

OUTPUT:		 	The movement of the Elevator can be seen as per the request.
																	// 07.2015//
*****************************************************************************************************************/

#include <lpc17xx.h>
#include "GLCD.h"

#define __FI  1
void delay()									/* Delay function between Glow of sucessive LEDS */
{ 					 
  int i,j;
  for(i=0;i<=10000;i++)
  {
      for(j=0;j<=1000;j++);
  }
}
void FloorDisplay(unsigned long i)
{
  switch(i)
  {
  case 0:	GLCD_DisplayString(6,0, __FI,"GND FLOOR REQUESTED");
				break;
	case 1:	GLCD_DisplayString(6,0, __FI,"1ST FLOOR REQUESTED");
				break;				 
	case 2:	GLCD_DisplayString(6,0, __FI,"2ND FLOOR REQUESTED");
				break;
	case 3:	GLCD_DisplayString(6,0, __FI,"3RD FLOOR REQUESTED");
				break;  
  case 4:	GLCD_DisplayString(6,0, __FI,"GND FLOOR REQUESTED");
				break;
	case 5:	GLCD_DisplayString(6,0, __FI,"1ST FLOOR REQUESTED");
				break;				 
	case 6:	GLCD_DisplayString(6,0, __FI,"2ND FLOOR REQUESTED");
				break;
	case 7:	GLCD_DisplayString(6,0, __FI,"3RD FLOOR REQUESTED");
				break;  

 }
}

int main()
{
  unsigned long	a;
  unsigned char temp,i=0,s,b,c,g,d,f;
  unsigned char floor[9]= {0x00,0x03,0x06,0x09,0x00,0x0E0,0x0D3,0xB6,0x79};
	signed char e;	
	
  LPC_SC->PCONP     |=   (1<<15);
  LPC_GPIO0->FIODIR |=   0x007F8000;	   //P0 AS output
  LPC_GPIO1->FIODIR &= ~(0x07F80000);	   // P1 as input
 
  #ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD      */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "        NIE         ");
  GLCD_DisplayString(1, 0, __FI, "      Mysuru        ");
  GLCD_DisplayString(2, 0, __FI, "  www.nie.ac.in     ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 0, __FI, "      ELEVATOR  ");
	 #endif		
	
  e=0x00;								/* Elevater at Ground Floor */
  								
  START:
		f = 0x00;
		f = e | 0xF0;
		LPC_GPIO0->FIOPIN = (f<<15);
		i=0;
	
		
  REQUEST: 
			for(s=0;s<=100;s++);
			a = LPC_GPIO1->FIOPIN & 0x00780000;				/* Get the status of the request Floor */
	    a >>= 19;
			b=a;
			b |= 0xF0;
			c = b;
			c = (~c);
			if (c==0) 					/* Check the request if not Requested */
			{
				GLCD_DisplayString(6,0, __FI,"WELCOME PRESS FLOOR NUMBER");
				goto REQUEST;				/* Get the Request again */
			}

  FIND_REQ:	
 			d = b;						/* Determine the Floor where service is Required */
 			b >>= 1;					 		
 			c = b;
 			if((d &= 0x01)==1)
  			{
				i += 1;
   				goto FIND_REQ;
  			}
			FloorDisplay(i);

  DECIDE: 
			delay();
   			if ((floor[i]-e)<0x00)		/* Decide elevater down movement */
      			goto DOWN	;

  			if ((floor[i]-e)==0)		/* Reset the requested Floor */
      			goto RESET ;   			
			GLCD_DisplayString(7,0, __FI," GOING UPWARDS ");
 			e += 1;	  				  	/*  Elevater Up movement */
			f = e;
			f |= 0xF0;
			LPC_GPIO0->FIOPIN = (LPC_GPIO0->FIOPIN & 0xFF807FFF) | (f<<15);
     	goto DECIDE;

  DOWN:	
			GLCD_DisplayString(7,0, __FI,"GOING DOWNWARDS");
  		e -= 1;						/* Elevater Down movement */
			f = e;
			f |= 0xF0;
			LPC_GPIO0->FIOPIN = (LPC_GPIO0->FIOPIN & 0xFF807FFF) | (f << 15);
			goto DECIDE;

  RESET: 
				GLCD_DisplayString(7,0, __FI," DESTINATION ");
  			temp = 0;					/* Corresponding request indicater LED Turn OFF */
			temp = 0x05;						
			i += temp;
			g = floor[i];
			LPC_GPIO0->FIOPIN = (LPC_GPIO0->FIOPIN & 0xFF807FFF) | (g<<15);
			goto START; 
}
