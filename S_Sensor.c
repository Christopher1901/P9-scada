#include <18F4550.h>
#fuses INTRC,NOPROTECT,NOWDT,CPUDIV1,NOMCLR
#use delay (clock=8M)
#use rs232(rcv=pin_c7, xmit=pin_c6, baud=9600, bits=8, parity=N)

#BYTE SSPBUF = 0xFC9
#BYTE T1CON=0xFCD
#BYTE TMR1L=0xFCE
#BYTE TMR1H=0xFCF


int data=0;
int16 cuenta;
float tiempo,distancia=0;

#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   switch(data)
   {
      case 0:
         SSPBUF = TMR1L;
      break;
      
      case 1:
         SSPBUF = TMR1H;
      break;
 
   }  
   
}
void ultrasonico();
void main()
{
   
   //FUNCIONES PARA LA COMUNICACIÓN SPI
   setup_spi(spi_slave | spi_L_to_H);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   //VARIABLES PARA EL SENSOR ULTRASONICO
   
   
   T1CON=0b11110001;    // <7>16bits  <5-4>Prescaler 8 <3>TMR1 Osc disabled <2>Ignored <1>Internal clock source <0>Enable TMR1
   
   
   while(TRUE)
   {
      ultrasonico();
      printf("DISTANCIA: %3.2f\n\r",distancia);
      DELAY_MS(1000);
   } 
}

void ultrasonico(){


output_high(pin_B0); 
               delay_us(10); 
          output_low(pin_B0);
          while(!input(pin_B1)) 
               {}
               TMR1L=0;
               TMR1H=0;
               TMR1L=0;
               TMR1H=0; 
          while(input(pin_B1))
               {}
               cuenta=TMR1H<<8;

cuenta+=TMR1L;
tiempo=(cuenta*(8))/3; //tiempo(us)= (4/Fosc*prescaler)/(1x10-6)
distancia=(tiempo)/((58.30));
}
