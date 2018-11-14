/*
===============================================================================
 Name        : Regularizador.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#define	MUESTRAS	512
#define FRECUENCIA	1000
#define GPDMA_CLK	204e6
#define pi 3.14159265

#include "chip.h"
#include <math.h>
#include <stdbool.h>

DMA_TransferDescriptor_t	LLI0;
DMA_TransferDescriptor_t	LLI1;

int* dmaSource = 0;
int buffers[2][MUESTRAS];
int aux[MUESTRAS];
int	PULSOS=GPDMA_CLK/(MUESTRAS*FRECUENCIA);
bool flags[5];
int valores[]={0, 511, 511, 290, 511, 265, 290, 228, 511, 257, 265, 185, 290, 186, 228, 204};

const int LLI_MASK = 	MUESTRAS		// transfer size (0 - 11) = 64
        				| (0 << 12)  	// source burst size (12 - 14) = 1
						| (0 << 15)  	// destination burst size (15 - 17) = 1
						| (2 << 18)  	// source width (18 - 20) = 32 bit
						| (2 << 21)  	// destination width (21 - 23) = 32 bit
						| (0 << 24)  	// source AHB select (24) = AHB 0
						| (1 << 25)  	// destination AHB select (25) = AHB 1
						| (1 << 26)  	// source increment (26) = increment
						| (0 << 27)  	// destination increment (27) = no increment
						| (0 << 28)  	// mode select (28) = access in user mode
						| (0 << 29)  	// (29) = access not bufferable
						| (0 << 30)  	// (30) = access not cacheable
						| (1 << 31); 	// terminal count interrupt disabled (deshabilita (0) / habilita (1));

void Config_LEDS(){

	LPC_SCU->SFSP[2][0] = SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP ;	//LED0_R
	LPC_SCU->SFSP[2][1] = SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP ;	//LED0_G
	LPC_SCU->SFSP[2][2] = SCU_MODE_FUNC4 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP ;	//LED0_B
	LPC_SCU->SFSP[2][10] = SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP ;	//LED1
	LPC_SCU->SFSP[2][11] = SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP ;	//LED2
	LPC_SCU->SFSP[2][12] = SCU_MODE_FUNC0 | SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP ;	//LED3

	LPC_GPIO_PORT->DIR[0] = (1 << 14);
	LPC_GPIO_PORT->DIR[1] = (1 << 11) | (1 << 12);
	LPC_GPIO_PORT->DIR[5] = (1 << 2) | (1 << 1) | (1 << 0);

}

void Config_TECS(){

	LPC_SCU->SFSP[1][0] = SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0 ;	//TEC_1
	LPC_SCU->SFSP[1][1] = SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0 ;	//TEC_2
	LPC_SCU->SFSP[1][2] = SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0 ;	//TEC_3
	LPC_SCU->SFSP[1][6] = SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0 ;	//TEC_4

}

void Config_INTERRUPT(){

	LPC_SCU->PINTSEL[0] = 	1 << 29 | 9 << 24	|
							0 << 21 | 9 << 16	|
							0 << 13 | 8 << 8	|
							0 << 5  | 4 << 0	;	//Configuro el GPIO para que me genere la interrupcion

	LPC_GPIO_PIN_INT->ISEL = 0;		//Configuro la interrupcion por flanco
	LPC_GPIO_PIN_INT->IENR = 0xF;	//Activo el GPIO para que genere interrupciones

	NVIC->ISER[1] = 0xF;
	NVIC->ISER[0] = (1 << 2);

}

void Config_DAC(){
	LPC_SCU->SFSP[4][4] = SCU_MODE_INACT;		//Desactivo las resistencias de pull-up y pull-down
	LPC_SCU->ENAIO[2] |= 1;					//Activo la funcion analogica del P4_4
	LPC_DAC->CNTVAL = (PULSOS & 0xFFFF);		//Configura el contador del DAC
	LPC_DAC->CTRL = 14;
}

void Config_DMA(){

	LLI0.src = (int) &buffers[0][0];
	LLI0.dst = (int) &(LPC_DAC->CR);
	LLI0.lli = (int) &LLI1;
	LLI0.ctrl = LLI_MASK;

	LLI1.src = (int) &buffers[1][0];
	LLI1.dst = (int) &(LPC_DAC->CR);
	LLI1.lli = (int) &LLI0;
	LLI1.ctrl = LLI_MASK;

	LPC_GPDMA->CH[0].SRCADDR	= LLI0.src;
	LPC_GPDMA->CH[0].DESTADDR	= LLI0.dst;
	LPC_GPDMA->CH[0].LLI		= LLI0.lli;
	LPC_GPDMA->CH[0].CONTROL	= LLI0.ctrl;

	LPC_GPDMA->CH[0].CONFIG		=	1   		// channel enabled (0)
									| (0 << 1)  	// source peripheral (1 - 5) = none
									| (0x0F << 6) 	// destination peripheral (6 - 10) = DAC
									| (1 << 11)  	// flow control (11 - 13) = mem to per
									| (0 << 14)  	// (14) = mask out error interrupt (0 = enmsacara)
									| (1 << 15)  	// (15) = mask out terminal count interrupt  (0 = enmsacara)
									| (0 << 16)  	// (16) = no locked transfers
									| (0 << 18); 	// (27) = no HALT (SE USA HASTA EL 18)

	LPC_GPDMA->CONFIG = 1;
}

void GPIO0_IRQHandler(void){
	LPC_GPIO_PIN_INT->IST |= 1;
	LPC_GPIO_PORT->NOT[5] = 1 << 2;

	flags[4] = true;
	flags[3] = !flags[3];
}

void GPIO1_IRQHandler(void){
	LPC_GPIO_PIN_INT->IST |= 1 << 1;
	LPC_GPIO_PORT->NOT[0] = 1 << 14;

	flags[4] = true;
	flags[2] = !flags[2];
}

void GPIO2_IRQHandler(void){
	LPC_GPIO_PIN_INT->IST |= 1 << 2;
	LPC_GPIO_PORT->NOT[1] = 1 << 11;

	flags[4] = true;
	flags[1] = !flags[1];
}

void GPIO3_IRQHandler(void){
	LPC_GPIO_PIN_INT->IST |= 1 << 3;
	LPC_GPIO_PORT->NOT[1] = 1 << 12;

	flags[4] = true;
	flags[0] = !flags[0];
}

void copiar(void){
	int i;
	for(i=0;i<MUESTRAS;i++)
		dmaSource[i]=aux[i];
}

void DMA_IRQHandler(void) {
	LPC_GPDMA -> INTTCCLEAR = 1;
	if(LPC_GPDMA->CH[0].LLI == LLI0.lli){
		dmaSource = &buffers[0][0];
	}
	if(LPC_GPDMA->CH[0].LLI == LLI1.lli){
		dmaSource = &buffers[1][0];
	}
	copiar();
}

void gen_sen(){
	int i, caso=0;
	caso=flags[3] << 3 | flags[2] << 2 | flags[1] << 1 | flags[0]; // CONCATENO 4 BITS
	for(i=0;i<MUESTRAS;i++){
		aux[i]=valores[caso]*(flags[0]*sin(2*pi*i/MUESTRAS)+flags[1]*sin(4*pi*i/MUESTRAS)+flags[2]*sin(8*pi*i/MUESTRAS)+flags[3]*sin(16*pi*i/MUESTRAS))+512;
		aux[i]=(aux[i] << 6);
	}
}


int main(void) {

	Config_LEDS();
	Config_TECS();
	Config_INTERRUPT();
	Config_DAC();
	Config_DMA();

	while(1){
		if(flags[4]){
			flags[4]=false;
			gen_sen();
		}
	}
}
