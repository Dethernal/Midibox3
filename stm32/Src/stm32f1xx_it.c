/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PIC.h"
#include "midi_in_handler.h"
#include "midi.h"
#include "iobuffers.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static const int8_t ROTARY_TABLE[16] = {
    0, 1, -1, 0, -1, 0, 0 , 1, 1, 0, 0, -1, 0, -1, 1, 0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

    NVIC_DisableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(TIM4_IRQn);
    LL_TIM_EnableIT_UPDATE(TIM4);
    LL_TIM_EnableCounter(TIM4);

  /* USER CODE END EXTI9_5_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_5) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
    /* USER CODE BEGIN LL_EXTI_LINE_5 */
    /* USER CODE END LL_EXTI_LINE_5 */
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_6) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_6);
    /* USER CODE BEGIN LL_EXTI_LINE_6 */
    /* USER CODE END LL_EXTI_LINE_6 */
  }
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  NVIC_ClearPendingIRQ(EXTI9_5_IRQn);

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

    __disable_irq();
    PIC_Update(false);
    LL_TIM_ClearFlag_UPDATE(TIM2);
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    __enable_irq();
  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

    LL_TIM_DisableCounter(TIM4);
    LL_TIM_SetCounter(TIM4, 0);
    LL_TIM_DisableIT_UPDATE(TIM4);

    uint32_t new_encoder_pos =
            (LL_GPIO_IsInputPinSet(ENC_B_GPIO_Port, ENC_B_Pin) ? 1 : 0) +
            (LL_GPIO_IsInputPinSet(ENC_A_GPIO_Port, ENC_A_Pin) ? 2 : 0);
    if (new_encoder_pos != prev_encoder_pos) {
        uint8_t idx = new_encoder_pos + (prev_encoder_pos << 2);
        int8_t change = ROTARY_TABLE[idx];
        change_max_volume(change);

        prev_encoder_pos = new_encoder_pos;
    }
    LL_TIM_ClearFlag_UPDATE(TIM4);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_ClearPendingIRQ(TIM4_IRQn);

  /* USER CODE END TIM4_IRQn 0 */
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
      uint8_t data = LL_USART_ReceiveData8(USART1);
      handle_midi_byte_in(data);
    }
    if (direct_buffer.head != direct_buffer.tail) {
        if (LL_USART_IsActiveFlag_TXE(USART1)) {
            LL_USART_TransmitData8(USART1, ring_popb(&direct_buffer));
            if (direct_buffer.head != direct_buffer.tail) {
                LL_USART_EnableIT_TXE(USART1);
            }
        }
    } else {
        LL_USART_DisableIT_TXE(USART1);
        LL_USART_ClearFlag_TC(USART1);
    }
    NVIC_ClearPendingIRQ(USART1_IRQn);
    //

  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
    __disable_irq();
    if (midi_out_buff.head != midi_out_buff.tail) {
        if (LL_USART_IsActiveFlag_TXE(USART2)) {
            LL_USART_TransmitData8(USART2, ring_popb(&midi_out_buff));
            if (midi_out_buff.head != midi_out_buff.tail) {
                LL_USART_EnableIT_TXE(USART2);
            }
        }
    } else {
        LL_USART_DisableIT_TXE(USART2);
        LL_USART_ClearFlag_TC(USART2);
    }
    NVIC_ClearPendingIRQ(USART2_IRQn);
    __enable_irq();
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
