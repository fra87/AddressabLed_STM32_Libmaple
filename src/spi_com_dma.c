/**
  ******************************************************************************
  * @file    spi_com_dma.c
  * @author  WI6LABS
  * @version V1.0.0
  * @date    05-August-2019
  * @brief   provide the SPI interface, derived from spi_com.c from ST
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "core_debug.h"
#include "stm32_def.h"
#include "spi_com_dma.h"
#include "PinAF_STM32F1.h"
#include "pinconfig.h"

#ifdef __cplusplus
extern "C" {
#endif
#if defined(HAL_SPI_MODULE_ENABLED)

/**
  * @brief This function is implemented by user to send data over SPI interface
  * @param  obj : pointer to spi_t structure
  * @param  Data : data to be sent
  * @param  len : length in bytes of the data to be sent
  * @param  Timeout: Timeout duration in tick
  * @retval status of the send operation (0) in case of error
  */
spi_status_e spi_dma_send(spi_t *obj, uint8_t *Data, uint16_t len, uint32_t Timeout)
{
  spi_status_e ret = SPI_OK;
  HAL_StatusTypeDef hal_status;

  if ((obj == NULL) || (len == 0)) {
    return SPI_ERROR;
  }

  hal_status = HAL_SPI_Transmit_DMA(&(obj->handle), Data, len);

  if (hal_status == HAL_TIMEOUT) {
    ret = SPI_TIMEOUT;
  } else if (hal_status != HAL_OK) {
    ret = SPI_ERROR;
  }

  return ret;
}

/**
  * @brief This function is implemented by user to send/receive data over
  *         SPI interface
  * @param  obj : pointer to spi_t structure
  * @param  tx_buffer : tx data to send before reception
  * @param  rx_buffer : data to receive
  * @param  len : length in byte of the data to send and receive
  * @param  Timeout: Timeout duration in tick
  * @retval status of the send operation (0) in case of error
  */
spi_status_e spi_dma_transfer(spi_t *obj, uint8_t *tx_buffer,
                          uint8_t *rx_buffer, uint16_t len, uint32_t Timeout)
{
  spi_status_e ret = SPI_OK;
  HAL_StatusTypeDef hal_status;

  if ((obj == NULL) || (len == 0)) {
    return SPI_ERROR;
  }

  hal_status = HAL_SPI_TransmitReceive_DMA(&(obj->handle), tx_buffer, rx_buffer, len);

  if (hal_status == HAL_TIMEOUT) {
    ret = SPI_TIMEOUT;
  } else if (hal_status != HAL_OK) {
    ret = SPI_ERROR;
  }

  return ret;
}
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
