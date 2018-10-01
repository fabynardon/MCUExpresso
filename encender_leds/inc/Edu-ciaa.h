#define SCU_BASE				0x40086000	// Direccion del System Control Unit

#define	SFSP1_0					0x080		// Offset del registro de configurcion del pin
#define	SFSP1_1					0x084		// Offset del registro de configurcion del pin
#define	SFSP1_2					0x088		// Offset del registro de configurcion del pin
#define	SFSP1_6					0x098		// Offset del registro de configurcion del pin

#define	SFSP2_0					0x100		// Offset del registro de configurcion del pin
#define	SFSP2_1					0x104		// Offset del registro de configurcion del pin
#define	SFSP2_2					0x108		// Offset del registro de configurcion del pin
#define	SFSP2_10				0x128		// Offset del registro de configurcion del pin
#define	SFSP2_11				0x12C		// Offset del registro de configurcion del pin
#define	SFSP2_12				0x130		// Offset del registro de configurcion del pin

#define GPIO_BASE				0x400F4000	// Direccion del GPIO

#define GPIO_PORT0_B_OFFSET		0x0000		// Offset del registro byte (B) del puerto 0
#define	GPIO_PORT0_DIR_OFFSET	0x2000		// Offset del registro de direccion (DIR) del puerto 0
#define GPIO_PORT0_CLR_OFFSET	0x2280		// Offset del registro clear (CLR) del puerto 0
#define GPIO_PORT0_SET_OFFSET	0x2200		// Offset del registro set (SET) del puerto 0

#define GPIO_PORT1_B_OFFSET		0x0020		// Offset del registro byte (B) del puerto 0
#define	GPIO_PORT1_DIR_OFFSET	0x2004		// Offset del registro de direccion (DIR) del puerto 0
#define GPIO_PORT1_CLR_OFFSET	0x2284		// Offset del registro clear (CLR) del puerto 0
#define GPIO_PORT1_SET_OFFSET	0x2204		// Offset del registro set (SET) del puerto 0

#define GPIO_PORT5_B_OFFSET		0x00A0		// Offset del registro byte (B) del puerto 0
#define	GPIO_PORT5_DIR_OFFSET	0x2014		// Offset del registro de direccion (DIR) del puerto 0
#define GPIO_PORT5_CLR_OFFSET	0x2294		// Offset del registro clear (CLR) del puerto 0
#define GPIO_PORT5_SET_OFFSET	0x2214		// Offset del registro set (SET) del puerto 0
