/*
===============================================================================
 Name        : encender_leds.c
 Author      : Fabian Nardone
 Version     : 1.0
 Description : main definition
===============================================================================
*/

#include <stdio.h>
#include "Edu-Ciaa.h"

int main(void) {

	int *P1_0 = (int *)((SCU_BASE)+(SFSP1_0));
	int *P1_1 = (int *)((SCU_BASE)+(SFSP1_1));
	int *P1_2 = (int *)((SCU_BASE)+(SFSP1_2));
	int *P1_6 = (int *)((SCU_BASE)+(SFSP1_6));

	int *P2_0  = (int *)((SCU_BASE)+(SFSP2_0));
	int *P2_1  = (int *)((SCU_BASE)+(SFSP2_1));
	int *P2_2  = (int *)((SCU_BASE)+(SFSP2_2));
	int *P2_10 = (int *)((SCU_BASE)+(SFSP2_10));
	int *P2_11 = (int *)((SCU_BASE)+(SFSP2_11));
	int *P2_12 = (int *)((SCU_BASE)+(SFSP2_12));

	char *GPIO0B	= (char *)((GPIO_BASE)+(GPIO_PORT0_B_OFFSET));
	int *GPIO0DIR	= (int *)((GPIO_BASE)+(GPIO_PORT0_DIR_OFFSET));
	int *GPIO0CLR	= (int *)((GPIO_BASE)+(GPIO_PORT0_CLR_OFFSET));
	int *GPIO0SET	= (int *)((GPIO_BASE)+(GPIO_PORT0_SET_OFFSET));

	char *GPIO1B	= (char *)((GPIO_BASE)+(GPIO_PORT1_B_OFFSET));
	int *GPIO1DIR 	= (int *)((GPIO_BASE)+(GPIO_PORT1_DIR_OFFSET));
	int *GPIO1CLR 	= (int *)((GPIO_BASE)+(GPIO_PORT1_CLR_OFFSET));
	int *GPIO1SET 	= (int *)((GPIO_BASE)+(GPIO_PORT1_SET_OFFSET));

	char *GPIO5B	= (char *)((GPIO_BASE)+(GPIO_PORT5_B_OFFSET));
	int *GPIO5DIR   = (int *)((GPIO_BASE)+(GPIO_PORT5_DIR_OFFSET));
	int *GPIO5CLR   = (int *)((GPIO_BASE)+(GPIO_PORT5_CLR_OFFSET));
	int *GPIO5SET   = (int *)((GPIO_BASE)+(GPIO_PORT5_SET_OFFSET));

	*P1_0 = (0x1 << 6) | (0x0);
	*P1_1 = (0x1 << 6) | (0x0);
	*P1_2 = (0x1 << 6) | (0x0);
	*P1_6 = (0x1 << 6) | (0x0);

	*P2_0 = (0x2 << 3) | (0x4);
	*P2_1 = (0x2 << 3) | (0x4);
	*P2_2 = (0x2 << 3) | (0x4);
	*P2_10 = (0x2 << 3) | (0x0);
	*P2_11 = (0x2 << 3) | (0x0);
	*P2_12 = (0x2 << 3) | (0x0);

	*GPIO0DIR = (1 << 14);
	*GPIO1DIR = ((1 << 11) | (1 << 12));
	*GPIO5DIR = ((1 << 2) | (1 << 1) | (1 << 0));

    while(1) {

    	if(*(GPIO0B + 4) == 1)
    		*GPIO5CLR |= (1 << 2);
    	else
    		*GPIO5SET |= (1 << 2);

    	if(*(GPIO0B + 8) == 1)
    		*GPIO0CLR |= (1 << 14);
    	else
    		*GPIO0SET |= (1 << 14);

    	if(*(GPIO0B + 9) == 1)
    		*GPIO1CLR |= (1 << 11);
    	else
    		*GPIO1SET |= (1 << 11);

    	if(*(GPIO1B + 9) == 1)
    		*GPIO1CLR |= (1 << 12);
    	else
    		*GPIO1SET |= (1 << 12);
    }
    return 0 ;
}
