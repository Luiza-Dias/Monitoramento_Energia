/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Advanced
Automatic Program Generator
© Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 20/06/2013
Author  : www.Eca.ir *** www.Webkade.ir
Company : 
Comments: 


Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 14,745600 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega16.h>

#include <delay.h>

// Standard Input/Output functions
#include <stdio.h>

#define ADC_VREF_TYPE 0x40 //Alteracao

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}

// Declare your global variables here

void main(void)
{
// Declare your local variables here
unsigned int valor_ad;
float Valor_tensao;

// Input/Output Ports initialization
// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=Out 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=0 
PORTB=0x00;
DDRB=0x01;

// Port C initialization
// Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
TCCR0=0x00;
TCNT0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
MCUCR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: Off
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 19200
UCSRA=0x00;
UCSRB=0x08;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x2F;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 921,600 kHz
// ADC Voltage Reference: Int., cap. on AREF
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x84;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;
//////////////////////////////////////////////////////////
//https://github.com/Abdurraziq/ZMPT101B-arduino/blob/master/src/ZMPT101B.cpp
/*int getZeroPoint()
{
	uint32_t Vsum = 0;
	uint32_t measurements_count = 0;
	uint32_t t_start = micros();

	while (micros() - t_start < period)
	{
		Vsum += analogRead(pin);
		measurements_count++;
	}

	return Vsum / measurements_count;
}

/// @brief Calculate root mean square (RMS) of AC valtage
/// @param loopCount Loop count to calculate
/// @return root mean square (RMS) of AC valtage
float ZMPT101B::getRmsVoltage(uint8_t loopCount)
{
	double readingVoltage = 0.0f;

	for (uint8_t i = 0; i < loopCount; i++)
	{
		int zeroPoint = this->getZeroPoint();

		int32_t Vnow = 0;
		uint32_t Vsum = 0;
		uint32_t measurements_count = 0;
		uint32_t t_start = micros();

		while (micros() - t_start < period)
		{
			Vnow = analogRead(pin) - zeroPoint;
			Vsum += (Vnow * Vnow);
			measurements_count++;
		}

		readingVoltage += sqrt(Vsum / measurements_count) / ADC_SCALE * VREF * sensitivity;
	}

	return readingVoltage / loopCount;
}
*/
//////////////////////////////////////////////////////////
while (1)
      {
      // Place your code here 
      valor_ad = read_adc(1);  //Alteracao
      printf("\r\nValor da conversão do AD=%d\r\n",valor_ad);
      Valor_tensao=valor_ad*5.00/1024; //Alteracao
      printf("Valor da Tensão=%f V\r\n",Valor_tensao);
      PORTB.0=~PORTB.0;
      delay_ms(1000);
      
      }
}
