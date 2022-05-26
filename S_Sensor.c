#include <18F4550.h>
#fuses INTRC,NOPROTECT,NOWDT,CPUDIV1,NOMCLR
#use delay (clock=8M)

#BYTE SSPBUF = 0xFC9
#BYTE T1CON=0xFCD
#BYTE TMR1L=0xFCE
#BYTE TMR1H=0xFCF


int data=0;
int16 cuenta;
float tiempo,distancia=0;
BYTE timer_alta=0, timer_baja=0,distancia_entera=0, distancia_decimal=0;

#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   switch(data)
   {
      case 0:
         SSPBUF = timer_alta;
      break;
      
      case 1:
         SSPBUF = timer_baja;
      break;
      
      case 3:
         SSPBUF = distancia_entera;
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
   } 
}

void ultrasonico(){


output_high(pin_D0); 
               delay_us(10); 
          output_low(pin_D0);
          while(!input(pin_D1)) 
               {}
               TMR1L=0;
               TMR1H=0;
               TMR1L=0;
               TMR1H=0; 
          while(input(pin_D1))
               {}
timer_alta=TMR1H;
timer_baja=TMR1L;

cuenta=TMR1H<<8;
cuenta+=TMR1L;

tiempo=(cuenta*4); //tiempo(us)= (4/Fosc*prescaler)/(1x10-6)
distancia=(tiempo)/((58.30));
distancia_entera=(int)(distancia);



}
