# M0A21BSP_USCI_SPI_PDMA
 M0A21BSP_USCI_SPI_PDMA

update @ 2022/03/16

1. Initial USCI0 SPI on M0A23 EVB ,

	- USCI_SPI0_SS (PC.7)    
	
	- USCI_SPI0_CLK (PC.6)
	
	- USCI_SPI0_MISO (PC.4)
	
	- USCI_SPI0_MOSI (PC.5)

2. use global define KEIL project , to separate SPI MASTER and SPI slave ( USCI_SPI_MASTER , USCI_SPI_SLAVE)

3. at master side , press digit '1' , will send SPI TX
	
4. below is MASTER and SLAVE MOSI MISO capture , 

below is LA capture , 
![image](https://github.com/released/M0A21BSP_USCI_SPI_PDMA/blob/main/uspi_tx_rx.jpg)

below is log messgae
![image](https://github.com/released/M0A21BSP_USCI_SPI_PDMA/blob/main/log.jpg)

