/* 
@author:        Diego Eduardo Lopez Rubio
@description:   6030 Group 
@functions:     None 
@environment:   KL25Z   
@date:          02/07/2012 
@comments:       
@version:       1.0 - Initial 
*/
  
#include "derivative.h" /* include peripheral declarations */ 
  
#define GPIO_PIN_MASK 0x1Fu 
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK))) 
  
#define PortLeds        GPIOC_PDOR 
#define PortLCD         GPIOC_PDOR 
  
#define PortEnable_1    GPIOB_PDOR |= 0x01 
#define PortEnable_0    GPIOB_PDOR &= 0xFE 
  
#define PortRS_1        GPIOB_PDOR |= 0x02 
#define PortRS_0        GPIOB_PDOR &= 0xFD 
  
  
#define ntime_1     600000
#define ntime_2     4000
#define ntime_3     60
  
#define nIns    0 
#define nData   1 
 
  
  
//-------------------------------------------------------------- 
//Declare Prototypes 
/* Functions */
void cfgPorts(void); 
void delay(long time); 
void initLCD(void); 
void sendCode(int Code, int PortData);
void cfgADC(void);
void cfgPWM(void);

  
/* Variables */
long x, y, z;  
int cen,dec,uni,cen2,dec2,uni2,cen3,dec3,uni3;  
int main(void) 
{ 
	   cfgPorts(); 
	   initLCD(); 
	   cfgPWM();
	   cfgADC();
	
	for(;;)
	{
		
		ADC0_SC1A=0;
		while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));
		
			delay(1000);
			GPIOC_PDOR = TPM0_C1V;
			TPM0_C1V =  ADC0_RA;
			
			cen = GPIOC_PDOR/100;
			dec = (GPIOC_PDOR-(cen*100))/10;
			uni = (GPIOC_PDOR-(cen*100)-(dec*10));
			
			dec = dec+0x30;
			cen = cen+0x30;
			uni = uni+0x30;
			
			sendCode(nIns,0x80);
			sendCode(nData,cen);
			sendCode(nData,dec);
			sendCode(nData,uni);
		

		ADC0_SC1A = 6;
		while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));
			
			delay(1000);
			GPIOC_PDOR = TPM2_C1V;
			TPM2_C1V =  ADC0_RA;
			
			cen2 = GPIOC_PDOR/100;
			dec2 = (GPIOC_PDOR-(cen2*100))/10;
			uni2 = (GPIOC_PDOR-(cen2*100)-(dec2*10));
			
			dec2 = dec2+0x30;
			cen2 = cen2+0x30;
			uni2 = uni2+0x30;
			
			sendCode(nIns,0x85);
			sendCode(nData,cen2);
			sendCode(nData,dec2);
			sendCode(nData,uni2);
		
		
		ADC0_SC1A = 7;
		while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));
		
			delay(1000);
			GPIOC_PDOR = TPM2_C0V;
			TPM2_C0V =  ADC0_RA;
			
			cen3 = GPIOC_PDOR/100;
			dec3 = (GPIOC_PDOR-(cen3*100))/10;
			uni3 = (GPIOC_PDOR-(cen3*100)-(dec3*10));
			
			dec3 = dec3+0x30;
			cen3 = cen3+0x30;
			uni3 = uni3+0x30;
			
			sendCode(nIns,0x89);
			sendCode(nData,cen3);
			sendCode(nData,dec3);
			sendCode(nData,uni3);
		
		
		
	}
	
    return 0;  
 
} 
  
void cfgPorts(void) 
{ 
	 SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	 SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	 SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	 SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
    //Also, we can use predefined masks created by freescale, such as: 
    //SIM_SCGC5_PORTB_MASK; 
      
    /* Set pins of PORTC as GPIO */
    PORTC_PCR0=(PORT_PCR_MUX(1)); 
    PORTC_PCR1=(PORT_PCR_MUX(1)); 
    PORTC_PCR2=(PORT_PCR_MUX(1)); 
    PORTC_PCR3=(PORT_PCR_MUX(1)); 
    PORTC_PCR4=(PORT_PCR_MUX(1)); 
    PORTC_PCR5=(PORT_PCR_MUX(1)); 
    PORTC_PCR6=(PORT_PCR_MUX(1)); 
    PORTC_PCR7=(PORT_PCR_MUX(1));    
  
      
    /* Set pins of PORTB as GPIO */
    PORTB_PCR0=(PORT_PCR_MUX(1)); 
    PORTB_PCR1=(PORT_PCR_MUX(1)); 
     
	/* Seet pins of PORTD as ADC0_SE6b */
	PORTD_PCR5=(0|PORT_PCR_MUX(0));
	PORTD_PCR6=(0|PORT_PCR_MUX(0));
	PORTE_PCR20=(0|PORT_PCR_MUX(0));
	
	//puertos pwm
	PORTB_PCR18=(PORT_PCR_MUX(3));
	PORTB_PCR19=(PORT_PCR_MUX(3));
	PORTD_PCR1=(PORT_PCR_MUX(4));
	
    //Reasure first PortB and PortC value 
	GPIOB_PDOR = 0x00;
    GPIOC_PDOR = 0x00;
	
    //Configure PortC as outputs 
    GPIOC_PDDR = 0xFF; 
          
    //*Configure PortB as outputs 
    GPIOB_PDDR = 0xFF; 
}
void cfgADC(void)      //input signal PTD5    00110   AD6b    
{
	//Turn on clock for ADC0
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	ADC0_CFG1 = 0;  	//0000 0000
	ADC0_CFG2 = 0x10;  	//0001 0000
	ADC0_SC2 = 0;   	//0000 0000
	ADC0_SC3 = 0x0D; 	//0000 1101
	
	//ADC Status and Control Registers 1

}
void cfgPWM(void)
{
	/* Select the CLK for the TPM Module */
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
	SIM_SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK); // Selects the MCGFLLCLK clock 
	
	//Turn on clock for TPM0
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;
	//Clear counter register
	TPM0_CNT = 0;
	//Set signal period to 1 ms
	TPM0_MOD = 255;
	//See page 552 for TPMx_SC configuration (freq = 1 MHz)
	TPM0_SC = 0x0F;			//0 0000 1011
	//See page 556 for TPMx_CnSC configuration
	TPM0_C1SC = 0x28;		//0010 1000

	
	
	TPM2_CNT = 0;
	//Set signal period to 1 ms
	TPM2_MOD = 255;
	//See page 552 for TPMx_SC configuration (freq = 1 MHz)
	TPM2_SC = 0x0F;			//0 0000 1011
	//See page 556 for TPMx_CnSC configuration
	TPM2_C1SC = 0x28;		//0010 1000
	

	TPM2_C0SC = 0x28;		//0010 1000
	
}

void delay (long time) 
{  
    x = time; 
    while(x>0) 
    { 
    x--; 
    } 
} 
  
void initLCD(void) 
{ 
    delay(ntime_2); 
    //Commands = {0x38, 0x38, 0x38, 0x0C, 0x01}; 
    sendCode(nIns, 0x38); 
	sendCode(nIns, 0x38); 
	sendCode(nIns, 0x38); 
	sendCode(nIns, 0x0C); 
	sendCode(nIns, 0x01); 
	
      
} 
  
void sendCode(int Code, int PortData) 
{   
    PortRS_0; 
    PortEnable_0; 
      
    PortLCD = PortData; 
      
    if (Code == nIns) 
    { 
        PortRS_0; 
		PortEnable_1;
		delay(ntime_3);
		PortEnable_0;
		PortRS_0;
		delay(ntime_2);
    } 
    else if (Code == nData) 
    { 
        PortRS_1; 
		PortEnable_1; 
        delay(ntime_3); 
        PortEnable_0; 
        PortRS_0; 
		delay(ntime_2);
	}
} 

