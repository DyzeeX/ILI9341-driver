#include "ILI9341.h"

static void TFT_Reset(void)
{
  TFT_RST_LOW;
  HAL_Delay(10);
  TFT_RST_HIGH;
  HAL_Delay(120);
}

static void TFT_WriteCommand(uint8_t cmd)
{
  TFT_CS_LOW;
  TFT_RS_LOW;
  TFT_DATA_OUT(cmd);
  TFT_WR_LOW;
  TFT_WR_HIGH;
  TFT_CS_HIGH;
}

static void TFT_WriteData(uint8_t data)
{
  TFT_CS_LOW;
  TFT_RS_HIGH;
  TFT_DATA_OUT(data);
  TFT_WR_LOW;
  TFT_WR_HIGH;
  TFT_CS_HIGH;
}

static void TFT_SetAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{

    uint16_t real_x1 = 319 - x2;
    uint16_t real_x2 = 319 - x1;

    TFT_WriteCommand(0x2A);        // Column Address Set
    TFT_WriteData(real_x1 >> 8);
    TFT_WriteData(real_x1 & 0xFF);
    TFT_WriteData(real_x2 >> 8);
    TFT_WriteData(real_x2 & 0xFF);

    TFT_WriteCommand(0x2B);        // Page Address Set
    TFT_WriteData(y1 >> 8);
    TFT_WriteData(y1 & 0xFF);
    TFT_WriteData(y2 >> 8);
    TFT_WriteData(y2 & 0xFF);

    TFT_WriteCommand(0x2C);        // Memory Write
}

static void TFT_Init(void)
{
  TFT_RD_HIGH; // Keep RD high as we are not reading

  TFT_Reset();

  TFT_WriteCommand(0xEF);
  TFT_WriteData(0x03);
  TFT_WriteData(0x80);
  TFT_WriteData(0x02);

  TFT_WriteCommand(0xCF);
  TFT_WriteData(0x00);
  TFT_WriteData(0xC1);
  TFT_WriteData(0x30);

  TFT_WriteCommand(0xED);
  TFT_WriteData(0x64);
  TFT_WriteData(0x03);
  TFT_WriteData(0x12);
  TFT_WriteData(0x81);

  TFT_WriteCommand(0xE8);
  TFT_WriteData(0x85);
  TFT_WriteData(0x00);
  TFT_WriteData(0x78);

  TFT_WriteCommand(0xCB);
  TFT_WriteData(0x39);
  TFT_WriteData(0x2C);
  TFT_WriteData(0x00);
  TFT_WriteData(0x34);
  TFT_WriteData(0x02);

  TFT_WriteCommand(0xF7);
  TFT_WriteData(0x20);

  TFT_WriteCommand(0xEA);
  TFT_WriteData(0x00);
  TFT_WriteData(0x00);

  TFT_WriteCommand(0xC0); // Power control
  TFT_WriteData(0x23);    // VRH[5:0]

  TFT_WriteCommand(0xC1); // Power control
  TFT_WriteData(0x10);    // SAP[2:0]; BT[3:0]

  TFT_WriteCommand(0xC5); // VCM control
  TFT_WriteData(0x3E);
  TFT_WriteData(0x28);

  TFT_WriteCommand(0xC7); // VCM control2
  TFT_WriteData(0x86);

  TFT_WriteCommand(0x36); // Memory Access Control
  TFT_WriteData(0xA8);    // Adjust as needed for orientation

  TFT_WriteCommand(0x3A); // Pixel Format Set
  TFT_WriteData(0x66);    // 18-bit/pixel for 262K colors

  TFT_WriteCommand(0xB1); // Frame Control
  TFT_WriteData(0x00);
  TFT_WriteData(0x18);

  TFT_WriteCommand(0xB6); // Display Function Control
  TFT_WriteData(0x08);
  TFT_WriteData(0x82);
  TFT_WriteData(0x27);

  TFT_WriteCommand(0xF2); // 3Gamma Function Disable
  TFT_WriteData(0x00);

  TFT_WriteCommand(0x26); // Gamma curve selected
  TFT_WriteData(0x01);

  TFT_WriteCommand(0xE0); // Set Gamma Positive
  TFT_WriteData(0x0F);
  TFT_WriteData(0x31);
  TFT_WriteData(0x2B);
  TFT_WriteData(0x0C);
  TFT_WriteData(0x0E);
  TFT_WriteData(0x08);
  TFT_WriteData(0x4E);
  TFT_WriteData(0xF1);
  TFT_WriteData(0x37);
  TFT_WriteData(0x07);
  TFT_WriteData(0x10);
  TFT_WriteData(0x03);
  TFT_WriteData(0x0E);
  TFT_WriteData(0x09);
  TFT_WriteData(0x00);

  TFT_WriteCommand(0xE1); // Set Gamma Negative
  TFT_WriteData(0x00);
  TFT_WriteData(0x0E);
  TFT_WriteData(0x14);
  TFT_WriteData(0x03);
  TFT_WriteData(0x11);
  TFT_WriteData(0x07);
  TFT_WriteData(0x31);
  TFT_WriteData(0xC1);
  TFT_WriteData(0x48);
  TFT_WriteData(0x08);
  TFT_WriteData(0x0F);
  TFT_WriteData(0x0C);
  TFT_WriteData(0x31);
  TFT_WriteData(0x36);
  TFT_WriteData(0x0F);

  TFT_WriteCommand(0x11); // Exit Sleep
  HAL_Delay(120);

  TFT_WriteCommand(0x29); // Display on
  HAL_Delay(10);
}

static void TFT_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t b1, uint8_t b2, uint8_t b3) {
  TFT_SetAddressWindow(x, y, x + w - 1, y + h - 1);
  TFT_CS_LOW; TFT_RS_HIGH;
  uint32_t pixels = (uint32_t)w * h;
  for (uint32_t i = 0; i < pixels; i++) {
    TFT_DATA_OUT(b1); TFT_WR_LOW; TFT_WR_HIGH;
    TFT_DATA_OUT(b2); TFT_WR_LOW; TFT_WR_HIGH;
    TFT_DATA_OUT(b3); TFT_WR_LOW; TFT_WR_HIGH;
  }
  TFT_CS_HIGH;
}

static void TFT_SetPixel(uint16_t x, uint16_t y, uint8_t b1, uint8_t b2, uint8_t b3) {
  TFT_SetAddressWindow(x, y, x, y);
  TFT_CS_LOW; TFT_RS_HIGH;
  TFT_DATA_OUT(b1); TFT_WR_LOW; TFT_WR_HIGH;
  TFT_DATA_OUT(b2); TFT_WR_LOW; TFT_WR_HIGH;
  TFT_DATA_OUT(b3); TFT_WR_LOW; TFT_WR_HIGH;
  TFT_CS_HIGH;
}