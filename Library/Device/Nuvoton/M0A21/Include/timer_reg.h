/**************************************************************************//**
 * @file     timer_reg.h
 * @version  V1.00
 * @brief    TIMER register definition header file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __TIMER_REG_H__
#define __TIMER_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/**
   @addtogroup REGISTER Control Register
   @{
*/

/**
    @addtogroup TIMER Timer Controller (TIMER)
    Memory Mapped Structure for TIMER Controller
@{ */

typedef struct
{


    /**
     * @var TIMER_T::CTL
     * Offset: 0x00/0x20  Timer0~3 Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[7:0]   |PSC       |Prescale Counter
     * |        |          |Timer input clock or event source is divided by (PSC+1) before it is fed to the timer up counter
     * |        |          |If this field is 0 (PSC = 0), then there is no scaling.
     * |        |          |Note: Updating prescale counter value will reset internal 8-bit prescale counter and 24-bit up counter value.
     * |[8]     |TRGPDMA   |Trigger PDMA Enable Bit
     * |        |          |If this bit is set to 1, timer time-out interrupt or capture interrupt can trigger PDMA.
     * |        |          |0 = Timer interrupt trigger PDMA Disabled.
     * |        |          |1 = Timer interrupt trigger PDMA Enabled.
     * |        |          |Note: If TRGSSEL (TIMERx_CTL[18]) = 0, time-out interrupt signal will trigger PDMA.
     * |        |          |If TRGSSEL (TIMERx_CTL[18]) = 1, capture interrupt signal will trigger PDMA.
     * |[10]    |INTRGEN   |Inter-timer Trigger Mode Enable Bit
     * |        |          |Setting this bit will enable the inter-timer trigger capture function.
     * |        |          |The Timer0/2 will be in event counter mode and counting with external clock source or event
     * |        |          |Also, Timer1/3 will be in trigger-counting mode of capture function.
     * |        |          |0 = Inter-Timer Trigger mode Disabled.
     * |        |          |1 = Inter-Timer Trigger mode Enabled.
     * |        |          |Note: For Timer1/3, this bit is ignored and the read back value is always 0.
     * |[16]    |CAPSRC    |Capture Pin Source Selection
     * |        |          |0 = Capture Function source is from TMx_EXT (x= 0~3) pin.
     * |        |          |1 = Capture Function source is from internal ACMP output signal or LIRC
     * |        |          |User can set INTERCAPSEL (TIMERx_EXTCTL[10:8]) to decide which internal ACMP output signal or LIRC is timer capture source.
     * |[18]    |TRGSSEL   |Trigger Source Select Bit
     * |        |          |This bit is used to select trigger source is from Timer time-out interrupt signal or capture interrupt signal.
     * |        |          |0 = Timer time-out interrupt signal is used to trigger PWM, DAC, ADC and PDMA.
     * |        |          |1 = Capture interrupt signal is used to trigger PWM, DAC, ADC and PDMA.
     * |[19]    |TRGPWM    |Trigger PWM Enable Bit
     * |        |          |If this bit is set to 1, timer time-out interrupt or capture interrupt can trigger PWM.
     * |        |          |0 = Timer interrupt trigger PWM Disabled.
     * |        |          |1 = Timer interrupt trigger PWM Enabled.
     * |        |          |Note: If TRGSSEL (TIMERx_CTL[18]) = 0, time-out interrupt signal will trigger PWM.
     * |        |          |If TRGSSEL (TIMERx_CTL[18]) = 1, capture interrupt signal will trigger PWM.
     * |[20]    |TRGDAC    |Trigger DAC Enable Bit
     * |        |          |If this bit is set to 1, timer time-out interrupt or capture interrupt can trigger DAC.
     * |        |          |0 = Timer interrupt trigger DAC Disabled.
     * |        |          |1 = Timer interrupt trigger DAC Enabled.
     * |        |          |Note: If TRGSSEL (TIMERx_CTL[18]) = 0, time-out interrupt signal will trigger DAC.
     * |        |          |If TRGSSEL (TIMERx_CTL[18]) = 1, capture interrupt signal will trigger DAC.
     * |[21]    |TRGADC    |Trigger ADC Enable Bit
     * |        |          |If this bit is set to 1, timer time-out interrupt or capture interrupt can trigger ADC.
     * |        |          |0 = Timer interrupt trigger ADC Disabled.
     * |        |          |1 = Timer interrupt trigger ADC Enabled.
     * |        |          |Note: If TRGSSEL (TIMERx_CTL[18]) = 0, time-out interrupt signal will trigger ADC.
     * |        |          |If TRGSSEL (TIMERx_CTL[18]) = 1, capture interrupt signal will trigger ADC.
     * |[22]    |TGLPINSEL |Toggle-output Pin Select
     * |        |          |0 = Toggle mode output to Tx (Timer Event Counter Pin).
     * |        |          |1 = Toggle mode output to Tx_EXT (Timer External Capture Pin).
     * |[23]    |WKEN      |Wake-up Function Enable Bit
     * |        |          |If this bit is set to 1, while timer interrupt flag TIF (TIMERx_INTSTS[0]) is 1 and INTEN (TIMERx_CTL[29]) is enabled, the timer interrupt signal will generate a wake-up trigger event to CPU.
     * |        |          |0 = Wake-up function Disabled if timer interrupt signal generated.
     * |        |          |1 = Wake-up function Enabled if timer interrupt signal generated.
     * |[24]    |EXTCNTEN  |Event Counter Mode Enable Bit
     * |        |          |This bit is for external counting pin function enabled.
     * |        |          |0 = Event counter mode Disabled.
     * |        |          |1 = Event counter mode Enabled.
     * |        |          |Note 1: When timer is used as an event counter, this bit should be set to 1 and select PCLKx (x=0~1) as timer clock source.
     * |        |          |Note 2: When TMR0/TMR2 INTRGEN is set to 1, this bit is forced to 1
     * |        |          |When INTRGEN is 1 and TMR1/TMR3 CAPIF (TIMERx_EINTSTS[0]) is 1, this bit is forced to 0.
     * |[25]    |ACTSTS    |Timer Active Status Bit (Read Only)
     * |        |          |This bit indicates the 24-bit up counter status.
     * |        |          |0 = 24-bit up counter is not active.
     * |        |          |1 = 24-bit up counter is active.
     * |        |          |Note: This bit may be active when CNT 0 transition to CNT 1.
     * |[26]    |RSTCNT    |Timer Counter Reset Bit
     * |        |          |Setting this bit will reset the 24-bit up counter value CNT (TIMERx_CNT[23:0]) and also force CNTEN (TIMERx_CTL[30]) to 0 if ACTSTS (TIMERx_CTL[25]) is 1.
     * |        |          |0 = No effect.
     * |        |          |1 = Reset internal 8-bit prescale counter, 24-bit up counter value and CNTEN bit.
     * |        |          |Note: This bit will be auto cleared.
     * |[28:27] |OPMODE    |Timer Counting Mode Select
     * |        |          |00 = The timer controller is operated in One-shot mode.
     * |        |          |01 = The timer controller is operated in Periodic mode.
     * |        |          |10 = The timer controller is operated in Toggle-output mode.
     * |        |          |11 = The timer controller is operated in Continuous Counting mode.
     * |[29]    |INTEN     |Timer Interrupt Enable Bit
     * |        |          |0 = Timer time-out interrupt Disabled.
     * |        |          |1 = Timer time-out interrupt Enabled.
     * |        |          |Note: If this bit is enabled, when the timer time-out interrupt flag TIF is set to 1, the timer interrupt signal is generated and inform to CPU.
     * |[30]    |CNTEN     |Timer Counting Enable Bit
     * |        |          |0 = Stops/Suspends counting.
     * |        |          |1 = Starts counting.
     * |        |          |Note 1: In stop status, and then setting CNTEN to 1 will enable the 24-bit up counter to keep counting from the last stop counting value.
     * |        |          |Note 2: This bit is auto-cleared by hardware in one-shot mode (TIMER_CTL[28:27] = 00) when the timer time-out interrupt flag TIF (TIMERx_INTSTS[0]) is generated.
     * |        |          |Note 3: Setting enable/disable this bit needs 2 * TMR_CLK period to become active
     * |        |          |User can read ACTSTS (TIMERx_CTL[25]) to check enable/disable command is completed or not.
     * |[31]    |ICEDEBUG  |ICE Debug Mode Acknowledge Disable Bit (Write Protect)
     * |        |          |0 = ICE debug mode acknowledgement effects TIMER counting.
     * |        |          |TIMER counter will be held while CPU is held by ICE.
     * |        |          |1 = ICE debug mode acknowledgement Disabled.
     * |        |          |TIMER counter will keep going no matter CPU is held by ICE or not.
     * |        |          |Note: This bit is write protected. Refer to the SYS_REGLCTL register.
     * @var TIMER_T::CMP
     * Offset: 0x04/0x24  Timer0~3 Comparator Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[23:0]  |CMPDAT    |Timer Comparator Value
     * |        |          |CMPDAT is a 24-bit compared value register
     * |        |          |When the internal 24-bit up counter value is equal to CMPDAT value, the TIF (TIMERx_INTSTS[0] Timer Interrupt Flag) will set to 1.
     * |        |          |Time-out period = (Period of timer clock input) * (8-bit PSC + 1) * (24-bit CMPDAT).
     * |        |          |Note 1: Never write 0x0 or 0x1 in CMPDAT field, or the core will run into unknown state.
     * |        |          |Note 2: When timer is operating at continuous counting mode, the 24-bit up counter will keep counting continuously even if user writes a new value into CMPDAT field
     * |        |          |But if timer is operating at other modes, the 24-bit up counter will restart counting from 0 and using the newest CMPDAT value to be the timer compared value while user writes a new value into CMPDAT field.
     * @var TIMER_T::INTSTS
     * Offset: 0x08/0x28  Timer0~3 Interrupt Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |TIF       |Timer Interrupt Flag
     * |        |          |This bit indicates the interrupt flag status of Timer while 24-bit timer up counter CNT (TIMERx_CNT[23:0]) value reaches CMPDAT (TIMERx_CMP[23:0]) value.
     * |        |          |0 = No effect.
     * |        |          |1 = CNT value matches the CMPDAT value.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     * |[1]     |TWKF      |Timer Wake-up Flag
     * |        |          |This bit indicates the interrupt wake-up flag status of timer.
     * |        |          |0 = Timer does not cause CPU wake-up.
     * |        |          |1 = CPU wake-up from Idle or Power-down mode if timer time-out interrupt signal generated.
     * |        |          |Note: This bit is cleared by writing 1 to it.
     * @var TIMER_T::CNT
     * Offset: 0x0C/0x2C  Timer0~3 Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[23:0]  |CNT       |Timer Data Register
     * |        |          |Read this register to get CNT value. For example:
     * |        |          |If EXTCNTEN (TIMERx_CTL[24]) is 0, user can read CNT value for getting current 24-bit counter value.
     * |        |          |If EXTCNTEN (TIMERx_CTL[24]) is 1, user can read CNT value for getting current 24-bit event input counter value.
     * @var TIMER_T::CAP
     * Offset: 0x10/0x30  Timer0~3 Capture Data Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[23:0]  |CAPDAT    |Timer Capture Data Register
     * |        |          |When CAPEN (TIMERx_EXTCTL[3]) bit is set, and a transition on TMx_EXT pin matched the CAPEDGE (TIMERx_EXTCTL[14:12]) setting, CAPIF (TIMERx_EINTSTS[0]) will set to 1 and the current timer counter value CNT (TIMERx_CNT[23:0]) will be auto-loaded into this CAPDAT field.
     * @var TIMER_T::EXTCTL
     * Offset: 0x14/0x34  Timer0~3 External Control Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CNTPHASE  |Timer External Count Phase
     * |        |          |This bit indicates the detection phase of external counting pin TMx (x= 0~3).
     * |        |          |0 = A falling edge of external counting pin will be counted.
     * |        |          |1 = A rising edge of external counting pin will be counted.
     * |[3]     |CAPEN     |Timer Capture Enable Bit
     * |        |          |This bit enables the capture input function.
     * |        |          |0 =Capture source Disabled.
     * |        |          |1 =Capture source Enabled.
     * |        |          |Note: TMR1/TMR3 CAPEN will be forced to 1 when TMR0/TMR2 INTRGEN is enabled.
     * |[4]     |CAPFUNCS  |Capture Function Selection
     * |        |          |0 = External Capture Mode Enabled.
     * |        |          |1 = External Reset Mode Enabled.
     * |        |          |Note 1: When CAPFUNCS is 0, transition on TMx_EXT (x= 0~3) pin is using to save current 24-bit timer counter value (CNT value) to CAPDAT field.
     * |        |          |Note 2: When CAPFUNCS is 1, transition on TMx_EXT (x= 0~3) pin is using to save current 24-bit timer counter value (CNT value) to CAPDAT field then CNT value will be reset immediately.
     * |[5]     |CAPIEN    |Timer External Capture Interrupt Enable Bit
     * |        |          |0 = TMx_EXT (x= 0~3) pin, LIRC, or ACMP detection Interrupt Disabled.
     * |        |          |1 = TMx_EXT (x= 0~3) pin, LIRC, or ACMP detection Interrupt Enabled.
     * |        |          |Note: CAPIEN is used to enable timer external interrupt
     * |        |          |If CAPIEN enabled, timer will rise an interrupt when CAPIF (TIMERx_EINTSTS[0]) is 1.
     * |        |          |For example, while CAPIEN = 1, CAPEN = 1, and CAPEDGE = 00, a 1 to 0 transition on the Tx_EXT (x= 0~3) pin, or ACMP will cause the CAPIF to be set then the interrupt signal is generated and sent to NVIC to inform CPU.
     * |[6]     |CAPDBEN   |Timer External Capture Pin De-bounce Enable Bit
     * |        |          |0 = TMx_EXT (x= 0~3) pin de-bounce or ACMP output de-bounce Disabled.
     * |        |          |1 = TMx_EXT (x= 0~3) pin de-bounce or ACMP output de-bounce Enabled.
     * |        |          |Note: If this bit is enabled, the edge detection of TMx_EXT pin or ACMP output is detected with de-bounce circuit.
     * |[7]     |CNTDBEN   |Timer Counter Pin De-bounce Enable Bit
     * |        |          |0 = TMx (x= 0~3) pin de-bounce Disabled.
     * |        |          |1 = TMx (x= 0~3) pin de-bounce Enabled.
     * |        |          |Note: If this bit is enabled, the edge detection of TMx pin is detected with de-bounce circuit.
     * |[10:8]  |INTERCAPSEL|Internal Capture Source Selection to Trigger Capture Function
     * |        |          |000 = Capture Function source is from internal ACMP0 output signal.
     * |        |          |001 = Capture Function source is from internal ACMP1 output signal.
     * |        |          |101 = Capture Function source is from LIRC.
     * |        |          |Others = Reserved.
     * |        |          |Note: these bits only available when CAPSRC (TIMERx_CTL[16]) is 1.
     * |[14:12] |CAPEDGE   |Timer External Capture Pin Edge Detect
     * |        |          |When first capture event is generated, the CNT (TIMERx_CNT[23:0]) will be reset to 0 and first CAPDAT (TIMERx_CAP[23:0]) will be set to 0.
     * |        |          |Disable Single Pulse Mode :
     * |        |          |000 = Capture event occurred when detect falling edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |001 = Capture event occurred when detect rising edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |010 = Capture event occurred when detect both falling and rising edge transfer on TMx_EXT (x= 0~3) pin, and first capture event occurred at falling edge transfer.
     * |        |          |011 = Capture event occurred when detect both rising and falling edge transfer on TMx_EXT (x= 0~3) pin, and first capture event occurred at rising edge transfer.
     * |        |          |110 = First capture event occurred at falling edge, follows capture events are at rising edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |111 = First capture event occurred at rising edge, follows capture events are at falling edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |100, 101 = Reserved.
     * |        |          |Enable Single Pulse Mode:
     * |        |          |000 = Measure falling edge &agrave; falling edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |001 = Measure rising edge &agrave; rising edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |110 = Measure falling edge &agrave; rising edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |111 = Measure rising edge &agrave; falling edge transfer on TMx_EXT (x= 0~3) pin.
     * |        |          |010, 011, 100, 101 = Reserved.
     * |[16]    |ECNTSSEL  |Event Counter Source Selection to Trigger Event Counter Function
     * |        |          |0 = Event Counter input source is from TMx (x= 0~3) pin.
     * |        |          |1 = Reserved.
     * |[20]    |CASIGMEN  |Capture Single Measure Mode Enable Bit
     * |        |          |Enable Single Pulse Mode function, user can write one to SIGST that can start measure full period or half period on TMx_EXT(x=0~3),.
     * |        |          |0 = Single Measure Mode Disabled.
     * |        |          |1 = Single Measure Mode Enabled.
     * |        |          |Note: these bits only available when CAPEN (TIMERx_EXTCTL[3]) is 1.
     * |[21]    |SIGST     |Single Measure Start Bit
     * |        |          |User can write 1u2019b1 to this bit to let timer start measure TMx_EXT pin
     * |        |          |When capture measure event finishes this bit will auto clear by hardware.
     * @var TIMER_T::EINTSTS
     * Offset: 0x18/0x38  Timer0~3 External Interrupt Status Register
     * ---------------------------------------------------------------------------------------------------
     * |Bits    |Field     |Descriptions
     * | :----: | :----:   | :---- |
     * |[0]     |CAPIF     |Timer External Capture Interrupt Flag
     * |        |          |This bit indicates the timer external capture interrupt flag status.
     * |        |          |0 = TMx_EXT (x= 0~3) pin interrupt did not occur.
     * |        |          |1 = TMx_EXT (x= 0~3) pin interrupt occurred.
     * |        |          |Note 1: This bit is cleared by writing 1 to it.
     * |        |          |Note 2: When CAPEN (TIMERx_EXTCTL[3]) bit is set and a transition on Tx_EXT (x= 0~3) pin matched the CAPEDGE (TIMERx_EXTCTL[14:12]) setting, this bit will set to 1 by hardware.
     * |        |          |Note 3: There is a new incoming capture event detected before CPU clearing the CAPIF status
     * |        |          |If the above condition occurred, the Timer will keep register TIMERx_CAP unchanged and drop the new capture value.
     */
    __IO uint32_t CTL;                   /*!< [0x0000/0x0020] Timer0~3 Control and Status Register                               */
    __IO uint32_t CMP;                   /*!< [0x0004/0x0024] Timer0~3 Compare Register                                          */
    __IO uint32_t INTSTS;                /*!< [0x0008/0x0028] Timer0~3 Interrupt Status Register                                 */
    __IO uint32_t CNT;                   /*!< [0x000c/0x002c] Timer0~3 Data Register                                             */
    __I  uint32_t CAP;                   /*!< [0x0010/0x0030] Timer0~3 Capture Data Register                                     */
    __IO uint32_t EXTCTL;                /*!< [0x0014/0x0034] Timer0~3 External Control Register                                 */
    __IO uint32_t EINTSTS;               /*!< [0x0018/0x0038] Timer0~3 External Interrupt Status Register                        */

} TIMER_T;

/**
    @addtogroup TMR01_CONST TMR01 Bit Field Definition
    Constant Definitions for TMR01 Controller
@{ */

#define TIMER_CTL_PSC_Pos                (0)                                               /*!< TIMER_T::CTL: PSC Position             */
#define TIMER_CTL_PSC_Msk                (0xfful << TIMER_CTL_PSC_Pos)                     /*!< TIMER_T::CTL: PSC Mask                 */

#define TIMER_CTL_TRGPDMA_Pos            (8)                                               /*!< TIMER_T::CTL: TRGPDMA Position         */
#define TIMER_CTL_TRGPDMA_Msk            (0x1ul << TIMER_CTL_TRGPDMA_Pos)                  /*!< TIMER_T::CTL: TRGPDMA Mask             */

#define TIMER_CTL_INTRGEN_Pos            (10)                                              /*!< TIMER_T::CTL: INTRGEN Position         */
#define TIMER_CTL_INTRGEN_Msk            (0x1ul << TIMER_CTL_INTRGEN_Pos)                  /*!< TIMER_T::CTL: INTRGEN Mask             */

#define TIMER_CTL_CAPSRC_Pos             (16)                                              /*!< TIMER_T::CTL: CAPSRC Position          */
#define TIMER_CTL_CAPSRC_Msk             (0x1ul << TIMER_CTL_CAPSRC_Pos)                   /*!< TIMER_T::CTL: CAPSRC Mask              */

#define TIMER_CTL_TRGSSEL_Pos            (18)                                              /*!< TIMER_T::CTL: TRGSSEL Position         */
#define TIMER_CTL_TRGSSEL_Msk            (0x1ul << TIMER_CTL_TRGSSEL_Pos)                  /*!< TIMER_T::CTL: TRGSSEL Mask             */

#define TIMER_CTL_TRGPWM_Pos             (19)                                              /*!< TIMER_T::CTL: TRGPWM Position          */
#define TIMER_CTL_TRGPWM_Msk             (0x1ul << TIMER_CTL_TRGPWM_Pos)                   /*!< TIMER_T::CTL: TRGPWM Mask              */

#define TIMER_CTL_TRGDAC_Pos             (20)                                              /*!< TIMER_T::CTL: TRGDAC Position          */
#define TIMER_CTL_TRGDAC_Msk             (0x1ul << TIMER_CTL_TRGDAC_Pos)                   /*!< TIMER_T::CTL: TRGDAC Mask              */

#define TIMER_CTL_TRGADC_Pos             (21)                                              /*!< TIMER_T::CTL: TRGADC Position          */
#define TIMER_CTL_TRGADC_Msk             (0x1ul << TIMER_CTL_TRGADC_Pos)                   /*!< TIMER_T::CTL: TRGADC Mask              */

#define TIMER_CTL_TGLPINSEL_Pos          (22)                                              /*!< TIMER_T::CTL: TGLPINSEL Position       */
#define TIMER_CTL_TGLPINSEL_Msk          (0x1ul << TIMER_CTL_TGLPINSEL_Pos)                /*!< TIMER_T::CTL: TGLPINSEL Mask           */

#define TIMER_CTL_WKEN_Pos               (23)                                              /*!< TIMER_T::CTL: WKEN Position            */
#define TIMER_CTL_WKEN_Msk               (0x1ul << TIMER_CTL_WKEN_Pos)                     /*!< TIMER_T::CTL: WKEN Mask                */

#define TIMER_CTL_EXTCNTEN_Pos           (24)                                              /*!< TIMER_T::CTL: EXTCNTEN Position        */
#define TIMER_CTL_EXTCNTEN_Msk           (0x1ul << TIMER_CTL_EXTCNTEN_Pos)                 /*!< TIMER_T::CTL: EXTCNTEN Mask            */

#define TIMER_CTL_ACTSTS_Pos             (25)                                              /*!< TIMER_T::CTL: ACTSTS Position          */
#define TIMER_CTL_ACTSTS_Msk             (0x1ul << TIMER_CTL_ACTSTS_Pos)                   /*!< TIMER_T::CTL: ACTSTS Mask              */

#define TIMER_CTL_RSTCNT_Pos             (26)                                              /*!< TIMER_T::CTL: RSTCNT Position          */
#define TIMER_CTL_RSTCNT_Msk             (0x1ul << TIMER_CTL_RSTCNT_Pos)                   /*!< TIMER_T::CTL: RSTCNT Mask              */

#define TIMER_CTL_OPMODE_Pos             (27)                                              /*!< TIMER_T::CTL: OPMODE Position          */
#define TIMER_CTL_OPMODE_Msk             (0x3ul << TIMER_CTL_OPMODE_Pos)                   /*!< TIMER_T::CTL: OPMODE Mask              */

#define TIMER_CTL_INTEN_Pos              (29)                                              /*!< TIMER_T::CTL: INTEN Position           */
#define TIMER_CTL_INTEN_Msk              (0x1ul << TIMER_CTL_INTEN_Pos)                    /*!< TIMER_T::CTL: INTEN Mask               */

#define TIMER_CTL_CNTEN_Pos              (30)                                              /*!< TIMER_T::CTL: CNTEN Position           */
#define TIMER_CTL_CNTEN_Msk              (0x1ul << TIMER_CTL_CNTEN_Pos)                    /*!< TIMER_T::CTL: CNTEN Mask               */

#define TIMER_CTL_ICEDEBUG_Pos           (31)                                              /*!< TIMER_T::CTL: ICEDEBUG Position        */
#define TIMER_CTL_ICEDEBUG_Msk           (0x1ul << TIMER_CTL_ICEDEBUG_Pos)                 /*!< TIMER_T::CTL: ICEDEBUG Mask            */

#define TIMER_CMP_CMPDAT_Pos             (0)                                               /*!< TIMER_T::CMP: CMPDAT Position          */
#define TIMER_CMP_CMPDAT_Msk             (0xfffffful << TIMER_CMP_CMPDAT_Pos)              /*!< TIMER_T::CMP: CMPDAT Mask              */

#define TIMER_INTSTS_TIF_Pos             (0)                                               /*!< TIMER_T::INTSTS: TIF Position          */
#define TIMER_INTSTS_TIF_Msk             (0x1ul << TIMER_INTSTS_TIF_Pos)                   /*!< TIMER_T::INTSTS: TIF Mask              */

#define TIMER_INTSTS_TWKF_Pos            (1)                                               /*!< TIMER_T::INTSTS: TWKF Position         */
#define TIMER_INTSTS_TWKF_Msk            (0x1ul << TIMER_INTSTS_TWKF_Pos)                  /*!< TIMER_T::INTSTS: TWKF Mask             */

#define TIMER_CNT_CNT_Pos                (0)                                               /*!< TIMER_T::CNT: CNT Position             */
#define TIMER_CNT_CNT_Msk                (0xfffffful << TIMER_CNT_CNT_Pos)                 /*!< TIMER_T::CNT: CNT Mask                 */

#define TIMER_CAP_CAPDAT_Pos             (0)                                               /*!< TIMER_T::CAP: CAPDAT Position          */
#define TIMER_CAP_CAPDAT_Msk             (0xfffffful << TIMER_CAP_CAPDAT_Pos)              /*!< TIMER_T::CAP: CAPDAT Mask              */

#define TIMER_EXTCTL_CNTPHASE_Pos        (0)                                               /*!< TIMER_T::EXTCTL: CNTPHASE Position     */
#define TIMER_EXTCTL_CNTPHASE_Msk        (0x1ul << TIMER_EXTCTL_CNTPHASE_Pos)              /*!< TIMER_T::EXTCTL: CNTPHASE Mask         */

#define TIMER_EXTCTL_CAPEN_Pos           (3)                                               /*!< TIMER_T::EXTCTL: CAPEN Position        */
#define TIMER_EXTCTL_CAPEN_Msk           (0x1ul << TIMER_EXTCTL_CAPEN_Pos)                 /*!< TIMER_T::EXTCTL: CAPEN Mask            */

#define TIMER_EXTCTL_CAPFUNCS_Pos        (4)                                               /*!< TIMER_T::EXTCTL: CAPFUNCS Position     */
#define TIMER_EXTCTL_CAPFUNCS_Msk        (0x1ul << TIMER_EXTCTL_CAPFUNCS_Pos)              /*!< TIMER_T::EXTCTL: CAPFUNCS Mask         */

#define TIMER_EXTCTL_CAPIEN_Pos          (5)                                               /*!< TIMER_T::EXTCTL: CAPIEN Position       */
#define TIMER_EXTCTL_CAPIEN_Msk          (0x1ul << TIMER_EXTCTL_CAPIEN_Pos)                /*!< TIMER_T::EXTCTL: CAPIEN Mask           */

#define TIMER_EXTCTL_CAPDBEN_Pos         (6)                                               /*!< TIMER_T::EXTCTL: CAPDBEN Position      */
#define TIMER_EXTCTL_CAPDBEN_Msk         (0x1ul << TIMER_EXTCTL_CAPDBEN_Pos)               /*!< TIMER_T::EXTCTL: CAPDBEN Mask          */

#define TIMER_EXTCTL_CNTDBEN_Pos         (7)                                               /*!< TIMER_T::EXTCTL: CNTDBEN Position      */
#define TIMER_EXTCTL_CNTDBEN_Msk         (0x1ul << TIMER_EXTCTL_CNTDBEN_Pos)               /*!< TIMER_T::EXTCTL: CNTDBEN Mask          */

#define TIMER_EXTCTL_INTERCAPSEL_Pos     (8)                                               /*!< TIMER_T::EXTCTL: INTERCAPSEL Position  */
#define TIMER_EXTCTL_INTERCAPSEL_Msk     (0x7ul << TIMER_EXTCTL_INTERCAPSEL_Pos)           /*!< TIMER_T::EXTCTL: INTERCAPSEL Mask      */

#define TIMER_EXTCTL_CAPEDGE_Pos         (12)                                              /*!< TIMER_T::EXTCTL: CAPEDGE Position      */
#define TIMER_EXTCTL_CAPEDGE_Msk         (0x7ul << TIMER_EXTCTL_CAPEDGE_Pos)               /*!< TIMER_T::EXTCTL: CAPEDGE Mask          */

#define TIMER_EXTCTL_ECNTSSEL_Pos        (16)                                              /*!< TIMER_T::EXTCTL: ECNTSSEL Position     */
#define TIMER_EXTCTL_ECNTSSEL_Msk        (0x1ul << TIMER_EXTCTL_ECNTSSEL_Pos)              /*!< TIMER_T::EXTCTL: ECNTSSEL Mask         */

#define TIMER_EXTCTL_CASIGMEN_Pos        (20)                                              /*!< TIMER_T::EXTCTL: CASIGMEN Position     */
#define TIMER_EXTCTL_CASIGMEN_Msk        (0x1ul << TIMER_EXTCTL_CASIGMEN_Pos)              /*!< TIMER_T::EXTCTL: CASIGMEN Mask         */

#define TIMER_EXTCTL_SIGST_Pos           (21)                                              /*!< TIMER_T::EXTCTL: SIGST Position        */
#define TIMER_EXTCTL_SIGST_Msk           (0x1ul << TIMER_EXTCTL_SIGST_Pos)                 /*!< TIMER_T::EXTCTL: SIGST Mask            */

#define TIMER_EINTSTS_CAPIF_Pos          (0)                                               /*!< TIMER_T::EINTSTS: CAPIF Position       */
#define TIMER_EINTSTS_CAPIF_Msk          (0x1ul << TIMER_EINTSTS_CAPIF_Pos)                /*!< TIMER_T::EINTSTS: CAPIF Mask           */

/**@}*/ /* TIMER_CONST */
/**@}*/ /* end of TIMER register group */
/**@}*/ /* end of REGISTER group */

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif /* __TIMER_REG_H__ */
