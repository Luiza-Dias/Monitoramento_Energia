#include <mega16.h>
#include <delay.h>
#include <stdio.h>
#include <i2c.h>
#include <ds1307.h>
//leitura analogica:
#define ADC_VREF_TYPE 0x40 
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
delay_us(10);
ADCSRA|=0x40;
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}
// I2C
#asm
   .equ __i2c_port=0x15 ;PORTC
   .equ __sda_bit=1
   .equ __scl_bit=0
#endasm
// DS1307 Real Time Clock functions

#ifndef RXB8
#define RXB8 1
#endif

#ifndef TXB8
#define TXB8 0
#endif

#ifndef UPE
#define UPE 2
#endif

#ifndef DOR
#define DOR 3
#endif

#ifndef FE
#define FE 4
#endif

#ifndef UDRE
#define UDRE 5
#endif

#ifndef RXC
#define RXC 7
#endif

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART Receiver buffer
#define RX_BUFFER_SIZE 32
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index,rx_rd_index,rx_counter;
#else
unsigned int rx_wr_index,rx_rd_index,rx_counter;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;
bit rx_message; // Flag de recepcao de mensagem

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
      if ( data == '\r' )
      {
         data = 0;
         rx_message = 1;
      }
    
   rx_buffer[rx_wr_index++]=data;
#if RX_BUFFER_SIZE == 256
   // special case for receiver buffer size=256
   if (++rx_counter == 0)
      {
#else
   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
#endif
      rx_buffer_overflow=1;
      }
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
   char data;
   while (rx_counter==0);
   data=rx_buffer[rx_rd_index++];
   #if RX_BUFFER_SIZE != 256
   if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
   #endif
   #asm("cli")
   --rx_counter;
   #asm("sei")
   return data;
}
#pragma used-
#endif

// USART Transmitter buffer
#define TX_BUFFER_SIZE 32
char tx_buffer[TX_BUFFER_SIZE];

#if TX_BUFFER_SIZE <= 256
unsigned char tx_wr_index,tx_rd_index,tx_counter;
#else
unsigned int tx_wr_index,tx_rd_index,tx_counter;
#endif

// USART Transmitter interrupt service routine
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter)
   {
   --tx_counter;
   UDR=tx_buffer[tx_rd_index++];
#if TX_BUFFER_SIZE != 256
   if (tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c)
{
while (tx_counter == TX_BUFFER_SIZE);
#asm("cli")
if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer[tx_wr_index++]=c;
#if TX_BUFFER_SIZE != 256
   if (tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
#endif
   ++tx_counter;
   }
else
   UDR=c;
#asm("sei")
}
#pragma used-
#endif


void main(void)
{
unsigned char hour, min, sec, date, month, year;
  char message[32];
  int i,j;
  unsigned int valor_ad;
  float valor_tensao;
  float valor_corrente;

// Port B initialization
PORTB=0x00;
DDRB=0x01;

// Port C initialization
PORTC=0x00;
DDRC=0x00;

// Port D initialization
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
TCCR0=0x00;
TCNT0=0x00;

// Timer/Counter 1 initialization

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
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
MCUCR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Baud Rate: 9600
UCSRA=0x00;
UCSRB=0xD8;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x5f;

// Analog Comparator initialization
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x84;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

// I2C Bus initialization
i2c_init();

// DS1307 Real Time Clock initialization
rtc_init(0,0,0);

printf("AT");
delay_ms(5000);
printf("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
delay_ms(5000);
//printf("AT+SAPBR=3,1,\"APN\",\"timbrasil.br\"");
printf("AT+SAPBR=3,1,\"APN\",\"zap.vivo.com.br\"");
delay_ms(5000);
//printf("AT+SAPBR=3,1,\"USER\",\"tim\"");
printf("AT+SAPBR=3,1,\"USER\",\"vivo\"");
delay_ms(5000);
//printf("AT+SAPBR=3,1,\"PWD\",\"tim\"");
printf("AT+SAPBR=3,1,\"PWD\",\"vivo\"");
delay_ms(5000);
print("AT+SAPBR=1,1");
delay_ms(5000);

while (1)
      {
      valor_ad = read_adc(1);
      valor_tensao=valor_ad*5.00/1024;
      valor_corrente = (valor_tensao - 2.5)/0.066;
      
      printf("AT+HTTPINIT");
      delay_ms(500);
      printf("AT+HTTPPARA=\"TIMEOUT\",120");
      delay_ms(500);
      printf("AT+HTTPSSL=0");
      delay_ms(500);
      printf("AT+HTTPPARA=\"URL\",\"http://industrial.api.ubidots.com/api/v1.6/devices/monitoramento-energia/?token=BBUS-OD7MqFThSC1DocgNvL4qJkoe5BD6yB\"");
      delay_ms(500);
      printf(" AT+HTTPPARA=\"CONTENT\",\"application/json\"");
      delay_ms(500);
      printf("AT+HTTPDATA= 17,70000");
      delay_ms(500);
      printf("{\"corrente\": %.2f}", valor_corrente);
      delay_ms(500);
      printf("AT+HTTPACTION=1");
      delay_ms(2000);

      //printf("AT+HTTPDATA= 43,70000");
      //printf("{\"corrente\":{\"value\": %.2f,\"timestamp\":%d}}",valor_corrente,tmtp);



      //printf("Valor da Corrente=%f V\r\n",(valor_tensao - 2.5)/0.066);   
      //rtc_get_time(&hour,&min,&sec);   
      //rtc_get_date(&date, &month, &year);
      //printf("\r\n%d:%d:%d:%d:%d:%d\r\n",hour,min,sec,date,month,year);
      //delay_ms(1000);
      //PORTB.0=~PORTB.0;    
      /*
      if(rx_message)
      {
        rx_message=0;
        for (i=0, j=rx_counter; i<j ; i++) 
        {
            message[i]=getchar();
        }      
        hour=(message[0]-48)*10+(message[1]-48);
        min=(message[3]-48)*10+(message[4]-48);
        sec=(message[6]-48)*10+(message[7]-48);  
        
        date=(message[10]-48)*10+(message[11]-48);  
        month=(message[13]-48)*10+(message[14]-48);  
        year=(message[16]-48)*10+(message[17]-48); 
        
        //hour=message[0];
        //min=message[1];
        //sec=message[2];        
        printf("\r\nAJUSTANDO %d:%d:%d:%d:%d:%d\r\n",hour,min,sec,date,month,year);
        rtc_set_time(hour,min,sec);  
        rtc_set_date(date,month,year);
      }
      */
      } 

}