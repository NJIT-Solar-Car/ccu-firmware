#include "CAN/Inc/can_motor.h"

#include "semphr.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

SemaphoreHandle_t xCanTxMutex = NULL;

BaseType_t CCU_CAN_Init() {
  if (xCanTxMutex == NULL) {
    xCanTxMutex = xSemaphoreCreateMutex();
  }

  return (xCanTxMutex != NULL) ? pdPASS : pdFAIL;
}

void CCU_CAN_FilterConfig(CAN_FilterTypeDef *phcan1, CAN_FilterTypeDef *phcan2) {
  CAN_FilterTypeDef sFilterConfig;

  sFilterConfig.FilterMode  = CAN_FILTERMODE_IDLIST;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  sFilterConfig.FilterMaskIdLow  = 0;
  sFilterConfig.FilterMaskIdHigh = 0;

  /*
   * Using CAN filter mode IDLIST, you can pack 4 IDs into the ID high/low, and mask high/low
   * parameters. If using the mask, you can only pack 2 into a bank. Per CAN peripheral there's
   * 14 banks, so we have a total of 56 possible IDs, prob more than enough for our use. As such,
   * there's probably little use in modifying the code above; to add a new filter simply add a 
   * block of code below.
   */

  /* Kelly Controllers */
  sFilterConfig.FilterIdLow  = (0x73 << 5);   /* Left Motor (Check ref manual page 1081 for bit shift reasoning) */
  sFilterConfig.FilterIdHigh = (0x74 << 5);   /* Right Motor */
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
  HAL_CAN_ConfigFilter(phcan1, &sFilterConfig);

  /* MPPT Filter Bank */
  sFilterConfig.FilterIdLow  = (0x600 << 5);
  sFilterConfig.FilterIdHigh = (0x7F0 << 5);
  sFilterConfig.FilterBank = 1;
  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
  sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
  HAL_CAN_ConfigFilter(phcan2, &sFilterConfig);
}

HAL_StatusTypeDef CCU_CAN_Tx(CAN_HandleTypeDef *hcan,
                              CAN_TxHeaderTypeDef *pHeader, 
                              uint8_t aData[], 
                              uint32_t *pTxMailbox, 
                              TickType_t xTicksToWait) {
  // Make sure CAN mutex is initialized
  if (xCanTxMutex == NULL) {
    return HAL_ERROR;
  }

  // Check if CAN mutex is free, if not then wait max xTicksToWait ticks.
  if (xSemaphoreTake(xCanTxMutex, xTicksToWait) == pdTRUE) {
    HAL_StatusTypeDef ret;

    // Burn cycles while there's no free mailboxes
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {
      vTaskDelay(pdMS_TO_TICKS(1));
    }

    // Send the data into can tx mailbox
    ret = HAL_CAN_AddTxMessage(hcan, pHeader, aData, pTxMailbox);

    // Free the CAN mutex
    xSemaphoreGive(xCanTxMutex);

    return ret;
  } else {
    return HAL_TIMEOUT;
  }
}
