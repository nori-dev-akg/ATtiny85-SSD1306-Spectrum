#include "SSD1306.h"

PROGMEM const uint8_t SSD1306_FONT[][8] =
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* �@ */
  {0x00,0x00,0x00,0x5f,0x00,0x00,0x00,0x00}, /* �I */
  {0x06,0x03,0x01,0x06,0x03,0x01,0x00,0x00}, /* �� */
  {0x20,0x62,0x3e,0x63,0x3e,0x23,0x02,0x00}, /* �� */
  {0x00,0x24,0x2a,0x7a,0x2f,0x2a,0x12,0x00}, /* �� */
  {0x42,0x25,0x12,0x08,0x24,0x52,0x21,0x00}, /* �� */
  {0x20,0x56,0x49,0x55,0x22,0x58,0x40,0x00}, /* �� */
  {0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x00}, /* �� */
  {0x00,0x00,0x00,0x00,0x1c,0x22,0x41,0x00}, /* �i */
  {0x41,0x22,0x1c,0x00,0x00,0x00,0x00,0x00}, /* �j */
  {0x00,0x22,0x14,0x7f,0x14,0x22,0x00,0x00}, /* �� */
  {0x08,0x08,0x08,0x7f,0x08,0x08,0x08,0x00}, /* �{ */
  {0x50,0x30,0x00,0x00,0x00,0x00,0x00,0x00}, /* �C */
  {0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00}, /* �| */
  {0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00}, /* �D */
  {0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00}, /* �^ */

  {0x00,0x3e,0x41,0x41,0x41,0x41,0x3e,0x00}, /* �O */
  {0x00,0x00,0x42,0x7f,0x40,0x00,0x00,0x00}, /* �P */
  {0x00,0x62,0x51,0x51,0x49,0x49,0x46,0x00}, /* �Q */
  {0x00,0x22,0x41,0x49,0x49,0x49,0x36,0x00}, /* �R */
  {0x00,0x30,0x28,0x24,0x22,0x7f,0x20,0x00}, /* �S */
  {0x00,0x2f,0x45,0x45,0x45,0x45,0x39,0x00}, /* �T */
  {0x00,0x3e,0x49,0x49,0x49,0x49,0x32,0x00}, /* �U */
  {0x00,0x01,0x01,0x61,0x19,0x05,0x03,0x00}, /* �V */
  {0x00,0x36,0x49,0x49,0x49,0x49,0x36,0x00}, /* �W */
  {0x00,0x26,0x49,0x49,0x49,0x49,0x3e,0x00}, /* �X */
  {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00}, /* �F */
  {0x00,0x00,0x56,0x36,0x00,0x00,0x00,0x00}, /* �G */
  {0x00,0x00,0x00,0x08,0x14,0x22,0x41,0x00}, /* �q */
  {0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x00}, /* �� */
  {0x41,0x22,0x14,0x08,0x00,0x00,0x00,0x00}, /* �r */
  {0x00,0x02,0x01,0x51,0x09,0x09,0x06,0x00}, /* �H */

  {0x1c,0x22,0x59,0x55,0x4d,0x12,0x0c,0x00}, /* �� */
  {0x60,0x18,0x16,0x11,0x16,0x18,0x60,0x00}, /* �` */
  {0x00,0x7f,0x49,0x49,0x49,0x49,0x36,0x00}, /* �a */
  {0x00,0x1c,0x22,0x41,0x41,0x41,0x22,0x00}, /* �b */
  {0x00,0x7f,0x41,0x41,0x41,0x22,0x1c,0x00}, /* �c */
  {0x00,0x7f,0x49,0x49,0x49,0x49,0x41,0x00}, /* �d */
  {0x00,0x7f,0x09,0x09,0x09,0x09,0x01,0x00}, /* �e */
  {0x00,0x1c,0x22,0x41,0x49,0x49,0x3a,0x00}, /* �f */
  {0x00,0x7f,0x08,0x08,0x08,0x08,0x7f,0x00}, /* �g */
  {0x00,0x00,0x41,0x7f,0x41,0x00,0x00,0x00}, /* �h */
  {0x00,0x20,0x40,0x40,0x40,0x40,0x3f,0x00}, /* �i */
  {0x00,0x7f,0x10,0x08,0x14,0x22,0x41,0x00}, /* �j */
  {0x00,0x7f,0x40,0x40,0x40,0x40,0x40,0x00}, /* �k */
  {0x7f,0x02,0x0c,0x30,0x0c,0x02,0x7f,0x00}, /* �l */
  {0x00,0x7f,0x02,0x04,0x08,0x10,0x7f,0x00}, /* �m */
  {0x00,0x1c,0x22,0x41,0x41,0x22,0x1c,0x00}, /* �n */

  {0x00,0x7f,0x09,0x09,0x09,0x09,0x06,0x00}, /* �o */
  {0x00,0x1c,0x22,0x41,0x51,0x22,0x5c,0x00}, /* �p */
  {0x00,0x7f,0x09,0x09,0x19,0x29,0x46,0x00}, /* �q */
  {0x00,0x26,0x49,0x49,0x49,0x49,0x32,0x00}, /* �r */
  {0x01,0x01,0x01,0x7f,0x01,0x01,0x01,0x00}, /* �s */
  {0x00,0x3f,0x40,0x40,0x40,0x40,0x3f,0x00}, /* �t */
  {0x03,0x0c,0x30,0x40,0x30,0x0c,0x03,0x00}, /* �u */
  {0x1f,0x60,0x18,0x06,0x18,0x60,0x1f,0x00}, /* �v */
  {0x41,0x22,0x14,0x08,0x14,0x22,0x41,0x00}, /* �w */
  {0x01,0x02,0x04,0x78,0x04,0x02,0x01,0x00}, /* �x */
  {0x00,0x41,0x61,0x51,0x49,0x45,0x43,0x00}, /* �y */
  {0x00,0x00,0x00,0x00,0x7f,0x41,0x41,0x00}, /* �m */
  {0x00,0x2b,0x2c,0x78,0x2c,0x2b,0x00,0x00}, /* �� */
  {0x41,0x41,0x7f,0x00,0x00,0x00,0x00,0x00}, /* �n */
  {0x00,0x00,0x02,0x01,0x02,0x00,0x00,0x00}, /* �O */
  {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00}, /* �Q */

  {0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00}, /* �M */
#if OPT_FONT_LOWER_CASE
  {0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00}, /* �� */
  {0x00,0x7f,0x48,0x44,0x44,0x38,0x00,0x00}, /* �� */
  {0x00,0x38,0x44,0x44,0x44,0x28,0x00,0x00}, /* �� */
  {0x00,0x38,0x44,0x44,0x48,0x7f,0x00,0x00}, /* �� */
  {0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00}, /* �� */
  {0x00,0x00,0x04,0x7e,0x05,0x01,0x00,0x00}, /* �� */
  {0x00,0x08,0x54,0x54,0x54,0x3c,0x00,0x00}, /* �� */
  {0x00,0x7f,0x08,0x04,0x04,0x78,0x00,0x00}, /* �� */
  {0x00,0x00,0x00,0x7d,0x00,0x00,0x00,0x00}, /* �� */
  {0x00,0x20,0x40,0x40,0x3d,0x00,0x00,0x00}, /* �� */
  {0x00,0x00,0x7f,0x10,0x28,0x44,0x00,0x00}, /* �� */
  {0x00,0x00,0x01,0x7f,0x00,0x00,0x00,0x00}, /* �� */
  {0x00,0x7c,0x04,0x78,0x04,0x78,0x00,0x00}, /* �� */
  {0x00,0x7c,0x08,0x04,0x04,0x78,0x00,0x00}, /* �� */
  {0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00}, /* �� */

  {0x00,0x7c,0x14,0x14,0x14,0x08,0x00,0x00}, /* �� */
  {0x00,0x08,0x14,0x14,0x14,0x7c,0x00,0x00}, /* �� */
  {0x00,0x7c,0x08,0x04,0x04,0x08,0x00,0x00}, /* �� */
  {0x00,0x48,0x54,0x54,0x54,0x24,0x00,0x00}, /* �� */
  {0x00,0x04,0x3e,0x44,0x44,0x20,0x00,0x00}, /* �� */
  {0x00,0x3c,0x40,0x40,0x20,0x7c,0x00,0x00}, /* �� */
  {0x00,0x0c,0x30,0x40,0x30,0x0c,0x00,0x00}, /* �� */
  {0x00,0x1c,0x60,0x18,0x60,0x1c,0x00,0x00}, /* �� */
  {0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00}, /* �� */
  {0x00,0x44,0x58,0x20,0x18,0x04,0x00,0x00}, /* �� */
  {0x00,0x44,0x64,0x54,0x4c,0x44,0x00,0x00}, /* �� */
#endif
  {0x00,0x00,0x00,0x08,0x36,0x41,0x41,0x00}, /* �o */
  {0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00}, /* �� */
  {0x41,0x41,0x36,0x08,0x00,0x00,0x00,0x00}, /* �p */
  {0x00,0x00,0x02,0x01,0x02,0x01,0x00,0x00}, /* ~  */
};

#if OPT_FONT_LARGE_DIGIT

PROGMEM const uint8_t FONT_LARGE_DIGIT[][19 * 3] = {
  { // 0
    0xFE,0xFE,0xFD,0xFD,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0xFD,0xFD,0xFE,0xFE,
    0xF7,0xF7,0xE3,0xE3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE3,0xE3,0xF7,0xF7,
    0x7F,0x7F,0xBF,0xBF,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xBF,0xBF,0x7F,0x7F,
  },
  { // 1
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFE,0xFE,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE3,0xE3,0xF7,0xF7,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x7F,0x7F,
  },
  { // 2
    0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0xFD,0xFD,0xFE,0xFE,
    0xF0,0xF0,0xE8,0xE8,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x0B,0x0B,0x07,0x07,
    0x7F,0x7F,0xBF,0xBF,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,
  },
  { // 3
    0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0xFD,0xFD,0xFE,0xFE,
    0x00,0x00,0x08,0x08,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEB,0xEB,0xF7,0xF7,
    0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xBF,0xBF,0x7F,0x7F,
  },
  { // 4
    0xFE,0xFE,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFE,0xFE,
    0x07,0x07,0x0B,0x0B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEB,0xEB,0xF7,0xF7,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x7F,0x7F,
  },
  { // 5
    0xFE,0xFE,0xFD,0xFD,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x01,0x01,0x00,0x00,
    0x07,0x07,0x0B,0x0B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xE8,0xE8,0xF0,0xF0,
    0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xBF,0xBF,0x7F,0x7F,
  },
  { // 6
    0xFE,0xFE,0xFD,0xFD,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x01,0x01,0x00,0x00,
    0xF7,0xF7,0xEB,0xEB,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xE8,0xE8,0xF0,0xF0,
    0x7F,0x7F,0xBF,0xBF,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xBF,0xBF,0x7F,0x7F,
  },
  { // 7
    0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0xFD,0xFD,0xFE,0xFE,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE3,0xE3,0xF7,0xF7,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x7F,0x7F,
  },
  { // 8
    0xFE,0xFE,0xFD,0xFD,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0xFD,0xFD,0xFE,0xFE,
    0xF7,0xF7,0xEB,0xEB,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEB,0xEB,0xF7,0xF7,
    0x7F,0x7F,0xBF,0xBF,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xBF,0xBF,0x7F,0x7F,
  },
  { // 9
    0xFE,0xFE,0xFD,0xFD,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0xFD,0xFD,0xFE,0xFE,
    0x07,0x07,0x0B,0x0B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEB,0xEB,0xF7,0xF7,
    0x00,0x00,0x80,0x80,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xBF,0xBF,0x7F,0x7F,
  },
};

PROGMEM const uint8_t FONT_LARGE_COLON[6 * 3] = {
  0x00,0xE0,0xF0,0xF0,0xE0,0x00,
  0x00,0x00,0x81,0x81,0x00,0x00,
  0x00,0x07,0x0F,0x0F,0x07,0x00,
};

#endif

#if OPT_FONT_MEDIUM_DIGIT

PROGMEM const uint8_t FONT_MEDIUM_DIGIT[][11 * 2] = {
  { // 0
    0xFE,0x7D,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x7D,0xFE,
    0x7F,0xBE,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xBE,0x7F,
  },
  { // 1
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xFE,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x7F,
  },
  { // 2
    0x00,0x01,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x7D,0xFE,
    0x7F,0xBE,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0x80,0x00,
  },
  { // 3
    0x00,0x01,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x7D,0xFE,
    0x00,0x80,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xBE,0x7F,
  },
  { // 4
    0xFE,0x7C,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x7C,0xFE,
    0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x3E,0x7F,
  },
  { // 5
    0xFE,0x7D,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x01,0x00,
    0x00,0x80,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xBE,0x7F,
  },
  { // 6
    0xFE,0x7D,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x01,0x00,
    0x7F,0xBE,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xBE,0x7F,
  },
  { // 7
    0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x7D,0xFE,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x7F,
  },
  { // 8
    0xFE,0x7D,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x7D,0xFE,
    0x7F,0xBE,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xBE,0x7F,
  },
  { // 9
    0xFE,0x7D,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x7D,0xFE,
    0x00,0x80,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xBE,0x7F,
  }
};

#endif