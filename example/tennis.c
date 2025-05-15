
/*

Copyright 2007 Windell H. Oskay
Distributed under the terms of the GNU General Public License, please see below.

-------------------------------------------------


Title:			tennis.c 
Author:			Windell Oskay
Date Created:   7/7/08
Last Modified:  7/15/08
Purpose:  Tennis for two

This is an AVR-GCC program designed for the Atmel ATmega168 series. 

More complete description here: http://www.evilmadscientist.com/article.php/tennis

Fuse settings: Default.





-------------------------------------------------
USAGE: How to compile and install



A makefile is provided to compile and install this program using AVR-GCC and avrdude.

To use it, follow these steps:
1. Update the header of the makefile as needed to reflect the type of AVR programmer that you use.
2. Open a terminal window and move into the directory with this file and the makefile.  
3. At the terminal enter
		make clean   <return>
		make all     <return>
		make install <return>
4. Make sure that avrdude does not report any errors.  If all goes well, the last few lines output by avrdude
should look something like this:

avrdude: verifying ...
avrdude: XXXX bytes of flash verified


avrdude: safemode: lfuse reads as E2
avrdude: safemode: hfuse reads as DF
avrdude: safemode: efuse reads as 1
avrdude: safemode: Fuses OK



avrdude done.  Thank you.


-------------------------------------------------

This code should be relatively straightforward, so not much documentation is provided.  If you'd like to ask 
questions, suggest improvements, or report success, please use the evilmadscientist forum:
http://www.evilmadscientist.com/forum/


-------------------------------------------------


 This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

-------------------------------------------------


*/


#include <avr/io.h> 
#include <math.h> 
#include <stdlib.h>		//gives rand() function
 
//gravitational acceleration (should be positive.)
#define g 0.8   
// TimeStep
#define ts 0.025 
#define historyLength 7 

int main ()
{ 

float sintable[64]; // tablas de senos y cosenos
float costable[64];

uint8_t xOldList[historyLength]; //historial (para animacion)
uint8_t yOldList[historyLength];

float xOld; // a few x & y position values
float yOld; // a few x & y position values

float VxOld; //  x & y velocity values 
float VyOld; //  x & y velocity values 

float Xnew, Ynew, VxNew, VyNew;

uint8_t deadball = 0;

uint8_t Langle, Rangle;

uint8_t xp = 0; // Donde se guardan los datos a dibujar
uint8_t yp = 0;     

unsigned int ADoutTemp;

uint8_t NewBall = 101;

unsigned int NewBallDelay = 0;

//Dummy variables: 
uint8_t k = 0;
uint8_t m = 0;

uint8_t Server = 0;

uint8_t ballside;
uint8_t Lused = 0;
uint8_t Rused = 0;

// Create trig look-up table to keep things snappy.
// 64 steps, total range: near pi.  Maybe a little more. 

m = 0;
while (m < 64)
{
	sintable[m] = sin((float) 0.0647 * (float) m - (float) 2.07);   
	costable[m] = cos((float) 0.0647 * (float) m - (float) 2.07);
    m++;
} 

yOld = 0;
VyOld = 0;

//Outputs:
DDRB = 255;
DDRD = 255;
PORTB = 0;
PORTD = 0; // Salidas para dibujar

//Inputs:
DDRC = 0; // Entradas para leer botones
PORTC = 0; // Pull-ups off.  We have our own external pull-ups anyway.

// ADC Setup
PRR &=  ~(_BV(ICF1));  //Allow ADC to be powered up

//ADC 3-5
ADMUX = Server * 4;	 
ADCSRA = 197;  // Enable ADC & start, prescale at 32

ballside = 0; //0 = Lused, 1 = Rused, 

for (;;)  // main loop										
{
	//127 es la mitad del campo (tmñ total == 254)
	if (ballside != (xOld >= 127)) //si xold >= 127 y ballside == 0 o si xold < 127 y ballside == 1
	{
		ballside = (xOld >= 127); // cambiar side

		if (ballside)
			Rused = 0; //ballside = 1
		else
			Lused = 0; //ballside = 0
	}

	if ( NewBall > 10 ) // If ball has run out of energy, make a new ball!
	{
		NewBall = 0;
		deadball = 0;
		
		NewBallDelay = 1; //boolean??
		
		Server = (ballside == 0);
		
		if (Server)
		{
			xOld = (float) 230; //server de la derecha
			VxOld = 0;// (float) -2*g; 
			ballside = 1;
			Rused = 0;
			Lused = 1; 
		}
		else
		{
			xOld = (float) 25; //server de la izquierda
			VxOld = 0;// (float) 2*g; 
			ballside = 0;
			Rused = 1;
			Lused = 0; 
		}
		
		yOld = (float) 110;     
	
		m = 0;
		while (m < historyLength)
		{
			xOldList[m] = xOld;
			yOldList[m] = yOld;

			m++;
		}
	}
  
	// Physics time!
	// x' = x + v*t + at*t/2
	// v' = v + a*t
	//
	// Horizontal (X) axis: No acceleration; a = 0.
	// Vertical (Y) axis: a = -g

	if ((ADCSRA & _BV(ADSC)) == 0)	// If ADC conversion has finished
	{	
		ADoutTemp = ADCW;			// Read out ADC value	
		
		/* We are using *ONE* ADC, but sequentially multiplexing it to sample
		the two different input lines.	*/
		
		if (ADMUX == 0) //que controlador esta activo
			Langle =  ADoutTemp >> 4; //ADoutTemp >> 2;
		else
			Rangle =  ADoutTemp >> 4; //ADoutTemp >> 2;

		// 64 angles allowed
		
		ADMUX = 4*(ballside);	// Either ch 0 or ch 4.
		
		ADCSRA |= _BV(ADSC);	// Start new ADC conversion 
	}

	if (NewBallDelay)
	{
		if (((PINC & 2U) == 0) || ((PINC & 32U) == 0)) //PINC 1 esta bajo y PINC 5 esta bajo
			NewBallDelay = 10000;
		
		NewBallDelay++;
	
		if (NewBallDelay > 5000)	// was 5000
			NewBallDelay = 0;

		m = 0;
		while (m < 255)
		{
			PORTD = yp; // dibujar
			PORTB = xp;
			m++;
		}
		
		VxNew = VxOld;
		VyNew = VyOld;
		Xnew =  xOld;
		Ynew = yOld;
 	}
 	else
	{
		Xnew = xOld + VxOld;
		Ynew = yOld + VyOld - 0.5*g*ts*ts;
		
		VyNew = VyOld - g*ts;
		VxNew = VxOld;
  
		// Bounce at walls
		
		if (Xnew < 0)
		{ 
			VxNew  *= -0.05; 
			VyNew *= 0.1; 
			Xnew = 0.1;
			deadball = 1;
			NewBall = 100;
  		}
 		if (Xnew > 255)
  		{ 
			VxNew  *= -0.05;  
			Xnew = 255;
			deadball = 1;
			NewBall = 100;
  		}
		if (Ynew <= 0)
		{
			Ynew = 0;
			
			if (VyNew*VyNew < 10)  
				NewBall++;
      
			if (VyNew < 0)
				VyNew *= -0.75;
		}
  
		if (Ynew >= 255) 
		{
			Ynew = 255;
			VyNew = 0;
		} 

		if (ballside)
		{
			if (Xnew < 127)
			{
				if (Ynew <= 63)
				{
					// Bounce off of net
					VxNew *= -0.5; 
					VyNew *= 0.5; 			
					Xnew = 128.00;
					deadball = 1;

				} 
			}
		}

		if (ballside == 0)
		{
			if (Xnew > 127)
			{
				if (Ynew <= 63)
				{
					// Bounce off of net
					VxNew *= -0.5;
					VyNew *= 0.5;
					Xnew = 126.00;
					deadball = 1;
				}
			}
		}
		// Simple routine to detect button presses: works, if the presses are slow enough.

	
		if (xOld < 120)
		{
			if ((PINC & 2U) == 0)
			{ 
				if ((Lused == 0) && (deadball == 0))
				{  
					VxNew = 1.5*g*costable[Langle];   
					VyNew = g + 1.5*g*sintable[Langle];
	
					Lused = 1; 
					NewBall = 0;
				}
			}
		}
	
		else if (xOld > 134)	// Ball on right side of screen
		{
			if ((PINC & 32U) == 0)
			{
				if ((Rused == 0) && (deadball == 0))
				{
					VxNew = -1.5*g*costable[Rangle];   
					VyNew = g + -1.5*g*sintable[Rangle];
					
					Rused = 1;
					NewBall = 0;
	
				}
			}
		}
	}

	//Figure out which point we're going to draw. 

	xp =  (int) floor(Xnew);

	yp =  (int) floor(Ynew);

	//yp = 511 - (int) floor(Ynew);

	//Dibujar suelo y red (Draw Ground and Net)

	k = 0;
	while (k < 20)
	{
		k++;

		m = 0;
		while (m < 127) // Suelo izquierda
		{
			PORTD = 0;		// Y-position
			PORTB = m;		// X-position

			m++; 
		}

		PORTB = 127;   // X-position of NET

		m = 0;
		while (m < 61) // Red sube
		{
			PORTD = m;		// Y-position

			m += 2; // de dos en dos??
		}
  
		while (m > 1) // Red baja
		{
			PORTD = m;		// Y-position

			m -= 2; 
		}

		PORTD = 0;		// Y-position
		PORTB = 127;	// Redundant, but allows time for scope trace to catch up.

		m = 127;
		while (m < 255) // Suelo derecha
		{
			PORTB = m;		// X-position
			PORTD = 0;		// Y-position

			m++; 
		}
	} // Repetir 20

	m = 0;
	while (m < historyLength)
	{
		k = 0;
		while (k < (4*m*m))
		{
			PORTB = xOldList[m];
			PORTD = yOldList[m];

			k++;
		}
		m++;
	}

	// Write the point to the buffer
	PORTD = yp;
	PORTB = xp; 

	m = 0;
	while (m < (historyLength - 1))
	{
		xOldList[m] = xOldList[m+1];
		yOldList[m] = yOldList[m+1];
		
		m++;
	}

	xOldList[(historyLength - 1)] = xp;
	yOldList[(historyLength - 1)] = yp;

	m = 0;
	while (m < 100)
	{ 
		PORTD = yp;
		PORTB = xp;
		m++;
	}

//Age variables for the next iteration
	VxOld = VxNew;
	VyOld = VyNew;
	
	xOld = Xnew;
	yOld = Ynew;

}
	return 0;
}
