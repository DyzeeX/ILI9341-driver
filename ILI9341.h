#ifndef __ILI9341_H
#define __ILI9341_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define TFT_WIDTH  320
#define TFT_HEIGHT 240

#define TFT_CS_LOW   GPIOE->BSRR = (uint32_t)GPIO_PIN_15 << 16U
#define TFT_CS_HIGH  GPIOE->BSRR = GPIO_PIN_15
#define TFT_RS_LOW   GPIOE->BSRR = (uint32_t)GPIO_PIN_12 << 16U
#define TFT_RS_HIGH  GPIOE->BSRR = GPIO_PIN_12
#define TFT_WR_LOW   GPIOE->BSRR = (uint32_t)GPIO_PIN_11 << 16U
#define TFT_WR_HIGH  GPIOE->BSRR = GPIO_PIN_11
#define TFT_RD_HIGH  GPIOE->BSRR = GPIO_PIN_10
#define TFT_RST_LOW  GPIOE->BSRR = (uint32_t)GPIO_PIN_8 << 16U
#define TFT_RST_HIGH GPIOE->BSRR = GPIO_PIN_8

#define TFT_DATA_OUT(byte) GPIOE->ODR = (GPIOE->ODR & 0xFF00U) | (byte)

static void TFT_Reset(void);
static void TFT_WriteCommand(uint8_t cmd);
static void TFT_WriteData(uint8_t data);
static void TFT_SetAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
static void TFT_Init(void);
static void TFT_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t b1, uint8_t b2, uint8_t b3);
static void TFT_SetPixel(uint16_t x, uint16_t y, uint8_t b1, uint8_t b2, uint8_t b3);

#endif /* __ILI9341_H */