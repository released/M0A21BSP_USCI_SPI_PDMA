/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include <string.h>
#include "NuMicro.h"

#include "project_config.h"


/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
volatile uint32_t BitFlag = 0;
volatile uint32_t counter_tick = 0;

#define USPI_MASTER_TX_DMA_CH 		(0)
#define USPI_MASTER_RX_DMA_CH 		(1)
#define USPI_MASTER_OPENED_CH   	    ((1 << USPI_MASTER_TX_DMA_CH) | (1 << USPI_MASTER_RX_DMA_CH))

#define USPI_SLAVE_TX_DMA_CH  		(2)
#define USPI_SLAVE_RX_DMA_CH  		(3)
#define USPI_SLAVE_OPENED_CH   		((1 << USPI_SLAVE_TX_DMA_CH) | (1 << USPI_SLAVE_RX_DMA_CH))

#define USCI_SPI_FREQ               (200000ul)

#define DATA_NUM	                (4)

uint8_t g_au8MasterToSlaveTestPattern[DATA_NUM]={0};
uint8_t g_au8SlaveToMasterTestPattern[DATA_NUM]={0};
uint8_t g_au8MasterRxBuffer[DATA_NUM]={0};
uint8_t g_au8SlaveRxBuffer[DATA_NUM]={0};

enum
{
	USPI_TX = 0,
	USPI_RX = 1,		
};

/*_____ M A C R O S ________________________________________________________*/

// put under IDE global define
// #define USCI_SPI_MASTER
// #define USCI_SPI_SLAVE

/*_____ F U N C T I O N S __________________________________________________*/

void tick_counter(void)
{
	counter_tick++;
}

uint32_t get_tick(void)
{
	return (counter_tick);
}

void set_tick(uint32_t t)
{
	counter_tick = t;
}

void compare_buffer(uint8_t *src, uint8_t *des, int nBytes)
{
    uint16_t i = 0;	
	
    for (i = 0; i < nBytes; i++)
    {
        if (src[i] != des[i])
        {
            printf("error idx : %4d : 0x%2X , 0x%2X\r\n", i , src[i],des[i]);
			set_flag(flag_error , ENABLE);
        }
    }

	if (!is_flag_set(flag_error))
	{
    	printf("%s finish \r\n" , __FUNCTION__);	
		set_flag(flag_error , DISABLE);
	}

}

void reset_buffer(void *dest, unsigned int val, unsigned int size)
{
    uint8_t *pu8Dest;
//    unsigned int i;
    
    pu8Dest = (uint8_t *)dest;

	#if 1
	while (size-- > 0)
		*pu8Dest++ = val;
	#else
	memset(pu8Dest, val, size * (sizeof(pu8Dest[0]) ));
	#endif
	
}

void copy_buffer(void *dest, void *src, unsigned int size)
{
    uint8_t *pu8Src, *pu8Dest;
    unsigned int i;
    
    pu8Dest = (uint8_t *)dest;
    pu8Src  = (uint8_t *)src;


	#if 0
	  while (size--)
	    *pu8Dest++ = *pu8Src++;
	#else
    for (i = 0; i < size; i++)
        pu8Dest[i] = pu8Src[i];
	#endif
}

void dump_buffer(uint8_t *pucBuff, int nBytes)
{
    uint16_t i = 0;
    
    printf("dump_buffer : %2d\r\n" , nBytes);    
    for (i = 0 ; i < nBytes ; i++)
    {
        printf("0x%2X," , pucBuff[i]);
        if ((i+1)%8 ==0)
        {
            printf("\r\n");
        }            
    }
    printf("\r\n\r\n");
}

void  dump_buffer_hex(uint8_t *pucBuff, int nBytes)
{
    int     nIdx, i;

    nIdx = 0;
    while (nBytes > 0)
    {
        printf("0x%04X  ", nIdx);
        for (i = 0; i < 16; i++)
            printf("%02X ", pucBuff[nIdx + i]);
        printf("  ");
        for (i = 0; i < 16; i++)
        {
            if ((pucBuff[nIdx + i] >= 0x20) && (pucBuff[nIdx + i] < 127))
                printf("%c", pucBuff[nIdx + i]);
            else
                printf(".");
            nBytes--;
        }
        nIdx += 16;
        printf("\n");
    }
    printf("\n");
}

void delay(uint16_t dly)
{
/*
	delay(100) : 14.84 us
	delay(200) : 29.37 us
	delay(300) : 43.97 us
	delay(400) : 58.5 us	
	delay(500) : 73.13 us	
	
	delay(1500) : 0.218 ms (218 us)
	delay(2000) : 0.291 ms (291 us)	
*/

	while( dly--);
}


void delay_ms(uint16_t ms)
{
	TIMER_Delay(TIMER0, 1000*ms);
}

void USPI_Master_Tx(uint8_t* u8bffer , uint16_t len)
{
    #if 0
    uint16_t i = 0;

    USPI_EnableInt(USPI0, USPI_TXEND_INT_MASK);
    mRxDataCount = 0;

    while (i < len)
    {
        #if 1
        USPI_WRITE_TX(USPI0, u8bffer[i++]);
        while(USPI_IS_BUSY(USPI0));   
        #else
        while(!USPI_GET_TX_EMPTY_FLAG(USPI0));	
        USPI_WRITE_TX(USPI0, u8bffer[i++]);	
        #endif
    }
    #endif
}

void USPI_Slave_PDMA_ClrBuffer(uint8_t TxRx)
{
    uint16_t i = 0;

	if (TxRx == USPI_TX)
	{
	    for (i = 0; i < DATA_NUM; i++)
	    {
	        g_au8SlaveToMasterTestPattern[i] = 0xFF;
	    }
	}
	else
	{
	    for (i = 0; i < DATA_NUM; i++)
	    {
	        g_au8SlaveRxBuffer[i] = 0xFF;
	    }
	}
}

void USPI_Slave_PDMA_PreInit(void)
{
	USPI_Slave_PDMA_ClrBuffer(USPI_TX);
	USPI_Slave_PDMA_ClrBuffer(USPI_RX);
	
    PDMA_Open(PDMA, USPI_SLAVE_OPENED_CH);

	//RX
    PDMA_SetTransferCnt(PDMA,USPI_SLAVE_RX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
    /* Set source/destination address and attributes */
    PDMA_SetTransferAddr(PDMA,USPI_SLAVE_RX_DMA_CH, (uint32_t)&USPI0->RXDAT, PDMA_SAR_FIX, (uint32_t)g_au8SlaveRxBuffer, PDMA_DAR_INC);
    /* Set request source; set basic mode. */
    PDMA_SetTransferMode(PDMA,USPI_SLAVE_RX_DMA_CH, PDMA_USCI0_RX, FALSE, 0);
    /* Single request type. SPI only support PDMA single request type. */
    PDMA_SetBurstType(PDMA,USPI_SLAVE_RX_DMA_CH, PDMA_REQ_SINGLE, 0);
    /* Disable table interrupt */
    PDMA->DSCT[USPI_SLAVE_RX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;

	//TX
    PDMA_SetTransferCnt(PDMA,USPI_SLAVE_TX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
    /* Set source/destination address and attributes */
    PDMA_SetTransferAddr(PDMA,USPI_SLAVE_TX_DMA_CH, (uint32_t)g_au8SlaveToMasterTestPattern, PDMA_SAR_INC, (uint32_t)&USPI0->TXDAT, PDMA_DAR_FIX);
    /* Set request source; set basic mode. */
    PDMA_SetTransferMode(PDMA,USPI_SLAVE_TX_DMA_CH, PDMA_USCI0_TX, FALSE, 0);
    /* Single request type. SPI only support PDMA single request type. */
    PDMA_SetBurstType(PDMA,USPI_SLAVE_TX_DMA_CH, PDMA_REQ_SINGLE, 0);
    /* Disable table interrupt */
    PDMA->DSCT[USPI_SLAVE_TX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;

    USPI_TRIGGER_RX_PDMA(USPI0);
    USPI_TRIGGER_TX_PDMA(USPI0);

    PDMA_EnableInt(PDMA, USPI_SLAVE_TX_DMA_CH, PDMA_INT_TRANS_DONE);
    PDMA_EnableInt(PDMA, USPI_SLAVE_RX_DMA_CH, PDMA_INT_TRANS_DONE);
    NVIC_EnableIRQ(PDMA_IRQn);	
	
}

void USPI_Slave_PDMA_Enable(uint8_t TxRx)
{
    uint16_t i = 0;
    static uint8_t cnt = 0;

	if (TxRx == USPI_TX)
	{
		//prepare slave TX data
        #if 1
        copy_buffer(g_au8SlaveToMasterTestPattern,g_au8SlaveRxBuffer,DATA_NUM);
        #else
		if ((g_au8SlaveRxBuffer[0] == 0xAA)&& 
			(g_au8SlaveRxBuffer[1] == 0xDD))
		{
		    for (i = 0; i < DATA_NUM; i++)
		    {
		        g_au8SlaveToMasterTestPattern[i] = i + (cnt++);
		    }
		}
        #endif
	
		//TX
	    PDMA_SetTransferCnt(PDMA,USPI_SLAVE_TX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
   	 	PDMA_SetTransferAddr(PDMA,USPI_SLAVE_TX_DMA_CH, (uint32_t)g_au8SlaveToMasterTestPattern, PDMA_SAR_INC, (uint32_t)&USPI0->TXDAT, PDMA_DAR_FIX);		
	    /* Set request source; set basic mode. */
	    PDMA_SetTransferMode(PDMA,USPI_SLAVE_TX_DMA_CH, PDMA_USCI0_TX, FALSE, 0);
	    USPI_TRIGGER_TX_PDMA(USPI0);
		
//		USPI_Slave_PDMA_ClrBuffer(USPI_RX);
	}
	else
	{	
		//RX
	    PDMA_SetTransferCnt(PDMA,USPI_SLAVE_RX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
	    /* Set request source; set basic mode. */
	    PDMA_SetTransferMode(PDMA,USPI_SLAVE_RX_DMA_CH, PDMA_USCI0_RX, FALSE, 0);
	    USPI_TRIGGER_RX_PDMA(USPI0);	
		
//		USPI_Slave_PDMA_ClrBuffer(USPI_TX);		
	}
}

void USPI_Master_PDMA_PreInit(void)
{
    uint16_t i = 0;
	
	//prepare data
    for (i=0; i < DATA_NUM; i++)
    {
        g_au8MasterToSlaveTestPattern[i] = i;
        g_au8MasterRxBuffer[i] = 0xFF;
    }

    PDMA_Open(PDMA, USPI_MASTER_OPENED_CH);

	//TX
    PDMA_SetTransferCnt(PDMA,USPI_MASTER_TX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
    /* Set source/destination address and attributes */
    PDMA_SetTransferAddr(PDMA,USPI_MASTER_TX_DMA_CH, (uint32_t)g_au8MasterToSlaveTestPattern, PDMA_SAR_INC, (uint32_t)&USPI0->TXDAT, PDMA_DAR_FIX);
    /* Set request source; set basic mode. */
    PDMA_SetTransferMode(PDMA,USPI_MASTER_TX_DMA_CH, PDMA_USCI0_TX, FALSE, 0);
    /* Single request type. SPI only support PDMA single request type. */
    PDMA_SetBurstType(PDMA,USPI_MASTER_TX_DMA_CH, PDMA_REQ_SINGLE, 0);
    /* Disable table interrupt */
    PDMA->DSCT[USPI_MASTER_TX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;

	//RX	
    PDMA_SetTransferCnt(PDMA,USPI_MASTER_RX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
    /* Set source/destination address and attributes */
    PDMA_SetTransferAddr(PDMA,USPI_MASTER_RX_DMA_CH, (uint32_t)&USPI0->RXDAT, PDMA_SAR_FIX, (uint32_t)g_au8MasterRxBuffer, PDMA_DAR_INC);
    /* Set request source; set basic mode. */
    PDMA_SetTransferMode(PDMA,USPI_MASTER_RX_DMA_CH, PDMA_USCI0_RX, FALSE, 0);
    /* Single request type. SPI only support PDMA single request type. */
    PDMA_SetBurstType(PDMA,USPI_MASTER_RX_DMA_CH, PDMA_REQ_SINGLE, 0);
    /* Disable table interrupt */
    PDMA->DSCT[USPI_MASTER_RX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;

    USPI_TRIGGER_RX_PDMA(USPI0);
    USPI_TRIGGER_TX_PDMA(USPI0);

    PDMA_EnableInt(PDMA, USPI_MASTER_TX_DMA_CH, PDMA_INT_TRANS_DONE);
    PDMA_EnableInt(PDMA, USPI_MASTER_RX_DMA_CH, PDMA_INT_TRANS_DONE);
    NVIC_EnableIRQ(PDMA_IRQn);
	
}

void USPI_Master_PDMA_Enable(uint8_t TxRx)
{
    uint16_t i = 0;
    static uint8_t cnt = 0;

	if (TxRx == USPI_TX)
	{
		//prepare master TX data
		g_au8MasterToSlaveTestPattern[0] = 0xAA;
		g_au8MasterToSlaveTestPattern[1] = 0xDD;

	    for (i = 2; i < DATA_NUM ; i++)
	    {
	        g_au8MasterToSlaveTestPattern[i] = (cnt++);
	    }
	
		//TX
	    PDMA_SetTransferCnt(PDMA,USPI_MASTER_TX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
		PDMA_SetTransferAddr(PDMA,USPI_MASTER_TX_DMA_CH, (uint32_t)g_au8MasterToSlaveTestPattern, PDMA_SAR_INC, (uint32_t)&USPI0->TXDAT, PDMA_DAR_FIX);		
	    /* Set request source; set basic mode. */
	    PDMA_SetTransferMode(PDMA,USPI_MASTER_TX_DMA_CH, PDMA_USCI0_TX, FALSE, 0);
	    USPI_TRIGGER_TX_PDMA(USPI0);	
	}
	else
	{
		//RX	
		PDMA_SetTransferCnt(PDMA,USPI_MASTER_RX_DMA_CH, PDMA_WIDTH_8, DATA_NUM);
		/* Set request source; set basic mode. */
		PDMA_SetTransferMode(PDMA,USPI_MASTER_RX_DMA_CH, PDMA_USCI0_RX, FALSE, 0);
		USPI_TRIGGER_RX_PDMA(USPI0);
	}
}

void PDMA_IRQHandler(void)
{
	#if defined (USCI_SPI_MASTER)	
    uint32_t status = PDMA_GET_INT_STATUS(PDMA);
	
    if (status & PDMA_INTSTS_ABTIF_Msk)   /* abort */
    {
		#if 1
        PDMA_CLR_ABORT_FLAG(PDMA, PDMA_GET_ABORT_STS(PDMA));
		#else
        if (PDMA_GET_ABORT_STS(PDMA) & (1 << SPI_MASTER_TX_DMA_CH))
        {
        }
        PDMA_CLR_ABORT_FLAG(PDMA, (1 << SPI_MASTER_TX_DMA_CH));
        if (PDMA_GET_ABORT_STS(PDMA) & (1 << SPI_MASTER_RX_DMA_CH))
        {
        }
        PDMA_CLR_ABORT_FLAG(PDMA, (1 << SPI_MASTER_RX_DMA_CH));
		#endif
    }
    else if (status & PDMA_INTSTS_TDIF_Msk)     /* done */
    {
        if((PDMA_GET_TD_STS(PDMA) & USPI_MASTER_OPENED_CH) == USPI_MASTER_OPENED_CH)
        {
            /* Clear PDMA transfer done interrupt flag */
            PDMA_CLR_TD_FLAG(PDMA, USPI_MASTER_OPENED_CH);

			//insert process
			USPI_DISABLE_TX_PDMA(USPI0);
			USPI_DISABLE_RX_PDMA(USPI0);

            set_flag(flag_USPI_RX_Finish , ENABLE);
            
        }        		
    }
    else if (status & (PDMA_INTSTS_REQTOF0_Msk | PDMA_INTSTS_REQTOF1_Msk))     /* Check the DMA time-out interrupt flag */
    {
        PDMA_CLR_TMOUT_FLAG(PDMA,USPI_MASTER_TX_DMA_CH);
        PDMA_CLR_TMOUT_FLAG(PDMA,USPI_MASTER_RX_DMA_CH);
    }
    else
    {

    }

	#elif defined (USCI_SPI_SLAVE)
    uint32_t status = PDMA_GET_INT_STATUS(PDMA);
	
    if (status & PDMA_INTSTS_ABTIF_Msk)   /* abort */
    {
		#if 1
        PDMA_CLR_ABORT_FLAG(PDMA, PDMA_GET_ABORT_STS(PDMA));
		#else
        if (PDMA_GET_ABORT_STS(PDMA) & (1 << SPI_SLAVE_TX_DMA_CH))
        {
        }
        PDMA_CLR_ABORT_FLAG(PDMA, (1 << SPI_SLAVE_TX_DMA_CH));
        if (PDMA_GET_ABORT_STS(PDMA) & (1 << SPI_SLAVE_RX_DMA_CH))
        {
        }
        PDMA_CLR_ABORT_FLAG(PDMA, (1 << SPI_SLAVE_RX_DMA_CH));
		#endif
    }
    else if (status & PDMA_INTSTS_TDIF_Msk)     /* done */
    {
        if((PDMA_GET_TD_STS(PDMA) & USPI_SLAVE_OPENED_CH) == USPI_SLAVE_OPENED_CH)
        {
            /* Clear PDMA transfer done interrupt flag */
            PDMA_CLR_TD_FLAG(PDMA, USPI_SLAVE_OPENED_CH);

			//insert process
			USPI_DISABLE_TX_PDMA(USPI0);
			USPI_DISABLE_RX_PDMA(USPI0);

            printf("[g_au8SlaveToMasterTestPattern]\r\n");
            dump_buffer(g_au8SlaveToMasterTestPattern , DATA_NUM); 
            printf("[g_au8SlaveRxBuffer]\r\n");            
            dump_buffer(g_au8SlaveRxBuffer , DATA_NUM);

            USPI_ClearRxBuf(USPI0);
            USPI_ClearTxBuf(USPI0);

			USPI_Slave_PDMA_Enable(USPI_TX);
			USPI_Slave_PDMA_Enable(USPI_RX);			
        }        		
    }
    else if (status & (PDMA_INTSTS_REQTOF0_Msk | PDMA_INTSTS_REQTOF1_Msk))     /* Check the DMA time-out interrupt flag */
    {
        PDMA_CLR_TMOUT_FLAG(PDMA,USPI_SLAVE_TX_DMA_CH);
        PDMA_CLR_TMOUT_FLAG(PDMA,USPI_SLAVE_RX_DMA_CH);
    }
    else
    {

    }

	#endif
	
}



/*
    EVVB : 
    USCI_SPI0_SS (PC.7)    
    USCI_SPI0_CLK (PC.6)
    USCI_SPI0_MISO (PC.4)
    USCI_SPI0_MOSI (PC.5)

    target : 
    PD.7 --> CS
    PD.6 --> MISO
    PD.5 --> MOSI
    PD.4 --> SCLK
*/
void USCI_SPI_Init (void)
{
    #if defined (USCI_SPI_MASTER)
    USPI_Open(USPI0, USPI_MASTER, USPI_MODE_0, 8, USCI_SPI_FREQ);
    /* Enable the automatic hardware slave selection function and configure USCI_SPI_SS pin as low-active. */
    USPI_EnableAutoSS(USPI0, 0, USPI_SS_ACTIVE_LOW);

    USPI_ClearRxBuf(USPI0);
    USPI_ClearTxBuf(USPI0);
    USPI_Master_PDMA_PreInit();

    #endif

    #if defined (USCI_SPI_SLAVE)
    USPI_Open(USPI0, USPI_SLAVE, USPI_MODE_0, 8, 0);
    /* Configure USCI_SPI_SS pin as low-active. */
    USPI0->CTLIN0 = (USPI0->CTLIN0 & ~USPI_CTLIN0_ININV_Msk) | USPI_CTLIN0_ININV_Msk;

    USPI_ClearRxBuf(USPI0);
    USPI_ClearTxBuf(USPI0);  
    //  USPI_WRITE_TX(USPI0, 0xFFF);    /* Dummy Write to prevent TX under run */    

    USPI_Slave_PDMA_PreInit();

    #endif
     
}


void USPI_Master_Process(void)
{
    if (is_flag_set(flag_USPI_RX_Finish))
    {
        set_flag(flag_USPI_RX_Finish , DISABLE);      

        printf("[g_au8MasterToSlaveTestPattern]\r\n");
        dump_buffer(g_au8MasterToSlaveTestPattern , DATA_NUM); 

        printf("[g_au8MasterRxBuffer]\r\n");       
        dump_buffer(g_au8MasterRxBuffer , DATA_NUM);                          
    }

	if (is_flag_set(flag_USPI_Transmit))
	{	
		set_flag(flag_USPI_Transmit , DISABLE);
        USPI_ClearRxBuf(USPI0);
        USPI_ClearTxBuf(USPI0);
        USPI_Master_PDMA_Enable(USPI_TX);
        USPI_Master_PDMA_Enable(USPI_RX);
    }
}

void process(void)
{
    #if defined (USCI_SPI_MASTER)
    USPI_Master_Process();
    #endif


}

void GPIO_Init (void)
{
    SYS->GPA_MFP0 = (SYS->GPA_MFP0 & ~(SYS_GPA_MFP0_PA2MFP_Msk)) | (SYS_GPA_MFP0_PA2MFP_GPIO);
    SYS->GPC_MFP0 = (SYS->GPC_MFP0 & ~(SYS_GPC_MFP0_PC2MFP_Msk)) | (SYS_GPC_MFP0_PC2MFP_GPIO);
		
	// EVM LED_R
    GPIO_SetMode(PA, BIT2, GPIO_MODE_OUTPUT);

	// EVM button
    GPIO_SetMode(PC, BIT2, GPIO_MODE_OUTPUT);	
}


void TMR1_IRQHandler(void)
{
	// static uint32_t LOG = 0;
	
    if(TIMER_GetIntFlag(TIMER1) == 1)
    {
        TIMER_ClearIntFlag(TIMER1);
		tick_counter();

		if ((get_tick() % 1000) == 0)
		{
        	// printf("%s : %4d\r\n",__FUNCTION__,LOG++);
			PA2 ^= 1;
		}

		if ((get_tick() % 50) == 0)
		{

		}	
    }
}


void TIMER1_Init(void)
{
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1000);
    TIMER_EnableInt(TIMER1);
    NVIC_EnableIRQ(TMR1_IRQn);	
    TIMER_Start(TIMER1);
}

void UARTx_Process(void)
{
	uint8_t res = 0;
	res = UART_READ(UART0);

	if (res == 'x' || res == 'X')
	{
		NVIC_SystemReset();
	}

	if (res > 0x7F)
	{
		printf("invalid command\r\n");
	}
	else
	{
		switch(res)
		{
			case '1':
                set_flag(flag_USPI_Transmit , ENABLE);
				break;

			case 'X':
			case 'x':
			case 'Z':
			case 'z':
				NVIC_SystemReset();		
				break;
		}
	}
}

void UART0_IRQHandler(void)
{

    if(UART_GET_INT_FLAG(UART0, UART_INTSTS_RDAINT_Msk | UART_INTSTS_RXTOINT_Msk))     /* UART receive data available flag */
    {
        while(UART_GET_RX_EMPTY(UART0) == 0)
        {
            UARTx_Process();
        }
    }

    if(UART0->FIFOSTS & (UART_FIFOSTS_BIF_Msk | UART_FIFOSTS_FEF_Msk | UART_FIFOSTS_PEF_Msk | UART_FIFOSTS_RXOVIF_Msk))
    {
        UART_ClearIntFlag(UART0, (UART_INTSTS_RLSINT_Msk| UART_INTSTS_BUFERRINT_Msk));
    }	
}

void UART0_Init(void)
{
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
    UART_EnableInt(UART0, UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk);
    NVIC_EnableIRQ(UART0_IRQn);
	
	#if (_debug_log_UART_ == 1)	//debug
	printf("CLK_GetCPUFreq : %8d\r\n",CLK_GetCPUFreq());
	printf("CLK_GetHXTFreq : %8d\r\n",CLK_GetHXTFreq());
	printf("CLK_GetLXTFreq : %8d\r\n",CLK_GetLXTFreq());	
	printf("CLK_GetPCLK0Freq : %8d\r\n",CLK_GetPCLK0Freq());
	printf("CLK_GetPCLK1Freq : %8d\r\n",CLK_GetPCLK1Freq());	
	#endif	

}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

//    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
//    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

//	CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);
//    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);	

//	CLK_EnableXtalRC(CLK_PWRCTL_LXTEN_Msk);
//    CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);	

    /* Select HCLK clock source as HIRC and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    CLK_EnableModuleClock(TMR1_MODULE);
  	CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_HIRC, 0);

    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Set PB multi-function pins for UART0 RXD=PB.6 and TXD=PB.4 */
    SYS->GPB_MFP1 = (SYS->GPB_MFP1 & ~(SYS_GPB_MFP1_PB4MFP_Msk | SYS_GPB_MFP1_PB6MFP_Msk)) |        \
                    (SYS_GPB_MFP1_PB4MFP_UART0_TXD | SYS_GPB_MFP1_PB6MFP_UART0_RXD);

    CLK_EnableModuleClock(PDMA_MODULE);
    CLK_EnableModuleClock(USCI0_MODULE);

    SYS->GPC_MFP1 = (SYS->GPC_MFP1 & ~(SYS_GPC_MFP1_PC4MFP_Msk | SYS_GPC_MFP1_PC5MFP_Msk | SYS_GPC_MFP1_PC6MFP_Msk | SYS_GPC_MFP1_PC7MFP_Msk)) |
                    (SYS_GPC_MFP1_PC4MFP_USCI0_DAT1 | SYS_GPC_MFP1_PC5MFP_USCI0_DAT0 | SYS_GPC_MFP1_PC6MFP_USCI0_CLK | SYS_GPC_MFP1_PC7MFP_USCI0_CTL0);


   /* Update System Core Clock */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();
}

int main()
{
    SYS_Init();

	UART0_Init();
	GPIO_Init();
	TIMER1_Init();

    USCI_SPI_Init();

    /* Got no where to go, just loop forever */
    while(1)
    {
        process();

    }
}

/*** (C) COPYRIGHT 2017 Nuvoton Technology Corp. ***/
