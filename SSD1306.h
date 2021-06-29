#ifndef _SSD1306_H
#define _SSD1306_H

#include <Arduino.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#define OPT_DATA_PTR 0
#define OPT_CODE_PTR 1
#define OPT_EROM_PTR 2

#define OPT_FONT_LARGE_DIGIT  1
#define OPT_FONT_MEDIUM_DIGIT 1
#define OPT_FONT_LOWER_CASE   1

#define FONT_START_CHAR 0x20
#define FONT_END_CHAR   0x7E

#define SSD1306_MAX_WIDTH  128
#define SSD1306_MAX_HEIGHT 32

#define SSD1306_REPT 0x00
#define SSD1306_WORD 0x80
#define SSD1306_DATA 0x40
                                                          //+-------------------+--------+
                                                          //| SSD1306           | SH1106 |
                                                          //+-------------------+--------+
//#define SH1106_SET_PUMP_VOLTAGE_VALUE              0x30 //| (none)            |        |
//#define SH1106_DC_DC_OFF                           0x8A //| (none)            |        |
//#define SH1106_DC_DC_ON                            0x8B //| (none)            |        |
//#define SH1106_DC_DC_CONTROL_MODE_SET              0xAD //| (none)            |        |
//#define SH1106_READ_MODIFY_WRITE_START             0xE0 //| (none)            |        |
//#define SH1106_READ_MODIFY_WRITE_END               0xEE //| (none)            |        |
                                                          //+-------------------+--------+
#define SSD1306_SET_LOWER_COLUMN_START_ADDRESS       0x00 //|                   |        |
#define SSD1306_SET_HIGHER_COLUMN_START_ADDRESS      0x10 //|                   |        |
#define SSD1306_SET_MEMORY_ADDRESSING_MODE           0x20 //| 02                | (none) |
#define SSD1306_SET_COLUMN_ADDRESS                   0x21 //| 00:7F             | (none) |
#define SSD1306_SET_PAGE_ADDRESS                     0x22 //| 00:07             | (none) |
#define SSD1306_RIGHT_SCROLL                         0x26 //| 00:07:07:07:00:FF | (none) |
#define SSD1306_LEFT_SCROLL                          0x27 //| 00:07:07:07:00:FF | (none) |
#define SSD1306_VERTICAL_AND_RIGHT_SCROLL            0x29 //| 00:07:07:07:00    | (none) |
#define SSD1306_VERTICAL_AND_LEFT_SCROLL             0x2A //| 00:07:07:07:00    | (none) |
#define SSD1306_DEACTIVATE_SCROLL                    0x2E //|                   | (none) |
#define SSD1306_ACTIVATE_SCROLL                      0x2F //|                   | (none) |
#define SSD1306_SET_DISPLAY_START_LINE               0x40 //|                   |        |
#define SSD1306_SET_CONTRAST_CONTROL                 0x81 //| 7F                | FF     |
#define SSD1306_CHARGE_PUMP_SETTING                  0x8D //| 10                | (none) |
#define SSD1306_SET_SEGMENT_NORMAL                   0xA0 //|                   |        |
#define SSD1306_SET_SEGMENT_REMAP                    0xA1 //|                   |        |
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 //| 3F:7F             | (none) |
#define SSD1306_ENTIRE_DISPLAY_OFF                   0xA4 //|                   |        |
#define SSD1306_ENTIRE_DISPLAY_ON                    0xA5 //|                   |        |
#define SSD1306_SET_NORMAL_DISPLAY                   0xA6 //|                   |        |
#define SSD1306_SET_INVERSE_DISPLAY                  0xA7 //|                   |        |
#define SSD1306_SET_MULTIPLEX_RATIO                  0xA8 //|                   |        |
#define SSD1306_SET_DISPLAY_OFF                      0xAE //|                   |        |
#define SSD1306_SET_DISPLAY_ON                       0xAF //|                   |        |
#define SSD1306_SET_PAGE_START_ADDRESS               0xB0 //| B0-B7             | B0-BF  |
#define SSD1306_SET_COM_OUTPUT_SCAN_NORMAL           0xC0 //|                   |        |
#define SSD1306_SET_COM_OUTPUT_SCAN_REMAP            0xC8 //|                   |        |
#define SSD1306_SET_DISPLAY_OFFSET                   0xD3 //|                   |        |
#define SSD1306_SET_CLOCK_DIVIDE_OSCILLATOR          0xD5 //| 80                | 50     |
#define SSD1306_SET_PRECHARGE_PERIOD                 0xD9 //|                   |        |
#define SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION  0xDA //|                   |        |
#define SSD1306_SET_VCOMH_DESELECT_LEVEL             0xDB //| 20                | 35     |
#define SSD1306_NOP                                  0xE3 //|                   |        |
#define SSD1306_GDDRAM                               0xFF //|                   |        |
                                                          //+-------------------+--------+

extern PROGMEM const uint8_t SSD1306_FONT[][8];

#if OPT_FONT_LARGE_DIGIT
extern PROGMEM const uint8_t FONT_LARGE_DIGIT[][19 * 3];
extern PROGMEM const uint8_t FONT_LARGE_COLON[6 * 3];
#endif

#if OPT_FONT_MEDIUM_DIGIT
extern PROGMEM const uint8_t FONT_MEDIUM_DIGIT[][11 * 2];
#endif

template<typename IOBUS_T, IOBUS_T& IOBUS, uint8_t ADDR = 0xFF, uint8_t SIZE = 32>
class SSD1306
{
  private:
    typedef struct
    {
      int left, top, right, bottom;
    } RECT;

    typedef struct
    {
      int x, y;
    } POINT;

    uint8_t _g_buf[32];
    RECT    _g_cur;
    POINT   _g_org;
    bool    _g_wrt;
    uint8_t _g_wrt_pre[SSD1306_MAX_HEIGHT/8*SSD1306_MAX_WIDTH/32]; // lines * (SSD1306_MAX_WIDTH/SIZE)
    uint8_t _g_wrt_pre_i;

    uint8_t _buffer[SIZE];
    uint8_t _count;
    uint8_t _ctrlb;
    uint8_t _lines;
    uint8_t _y_off;
    uint8_t _y_pos;
    uint8_t _x_pos;
    
  protected:
    void ctrl(uint8_t cmd, uint8_t len)
    {
      if (cmd != SSD1306_GDDRAM)
      {
        if ((_ctrlb != SSD1306_WORD) || (_count + len + 2 >= sizeof(_buffer)))
          flush();
        _buffer[_count++] = _ctrlb = (len ? SSD1306_REPT : SSD1306_WORD);
        _buffer[_count++] = cmd;
      }
      else if ((_count == 0) || (_ctrlb != SSD1306_DATA))
      {
        if ((_ctrlb != SSD1306_WORD) || (_count + len + 1 >= sizeof(_buffer)))
          flush();
        _buffer[_count++] = _ctrlb = SSD1306_DATA;
      }
    }

    void data(uint8_t val)
    {
      if (_count == 0)
        _buffer[_count++] = _ctrlb;
      _buffer[_count] = val;
      if (++_count >= sizeof(_buffer))
        flush();
    }

    uint8_t read_byte(const uint8_t *address, uint8_t opt)
    {
      switch (opt)
      {
        case OPT_CODE_PTR:
          return pgm_read_byte_near(address);
        case OPT_EROM_PTR:
          return eeprom_read_byte(address);
      }
      return *address;
    }

    uint8_t *glyph(uint8_t c)
    {
      if ((c < FONT_START_CHAR) || (c > FONT_END_CHAR))
        c = 0;
      else
      {
#if OPT_FONT_LOWER_CASE
        c -= FONT_START_CHAR;
#else
        if ((c >= 'a') && (c <= 'z'))
          c -= FONT_START_CHAR + 'a' - 'A';
        else if (c > 'z')
          c -= FONT_START_CHAR + 'z' - 'a' + 1;
        else
          c -= FONT_START_CHAR;
#endif
      }
      return (uint8_t *)SSD1306_FONT[c];
    }

    void charge_pump(bool enable)
    {
      ctrl(SSD1306_CHARGE_PUMP_SETTING, 1);
      data(enable ? 0x14 : 0x10);
    }

    void multiplex(uint8_t nlines)
    {
      if ((nlines >= 2) && (nlines <= (SSD1306_MAX_HEIGHT >> 3)) && (nlines != _lines))
      {
        _lines = nlines;
        ctrl(SSD1306_SET_MULTIPLEX_RATIO, 1);
        data((nlines << 3) - 1);
      }
    }

    void address(uint8_t x, uint8_t y)
    {
      //
      // set column start address
      //
      ctrl(SSD1306_SET_LOWER_COLUMN_START_ADDRESS  | ((x >> 0) & 0x0F), 0);
      ctrl(SSD1306_SET_HIGHER_COLUMN_START_ADDRESS | ((x >> 4) & 0x0F), 0);
      //
      // set page start address
      //
      ctrl(SSD1306_SET_PAGE_START_ADDRESS | ((y + _y_off) & 0x07), 0);
    }

    bool ptInVisible(int x, int y)
    {
      return (x >= _g_cur.left) && (x < _g_cur.right) && (y >= _g_cur.top) && (y < _g_cur.bottom);
    }

    bool hasIntersectClipRect(int x0, int y0, int x1, int y1)
    {
      return (x0 < x1 ? (x0 >= _g_cur.right ) || (x1 <= _g_cur.left) : (x0 < _g_cur.left) || (x1 >= _g_cur.right ))
          || (y0 < y1 ? (y0 >= _g_cur.bottom) || (y1 <= _g_cur.top ) : (y0 < _g_cur.top ) || (y1 >= _g_cur.bottom))
          ? false : true;
    }

    bool intersectClipRect(RECT *rc)
    {
      if ((rc->left >= rc->right) || (rc->top >= rc->bottom))
        return false;
      if ((rc->left >= _g_cur.right) || (rc->right <= _g_cur.left))
        return false;
      if ((rc->top >= _g_cur.bottom) || (rc->bottom <= _g_cur.top))
        return false;
      if (rc->left < _g_cur.left)
        rc->left = _g_cur.left;
      if (rc->top < _g_cur.top)
        rc->top = _g_cur.top;
      if (rc->right > _g_cur.right)
        rc->right = _g_cur.right;
      if (rc->bottom > _g_cur.bottom)
        rc->bottom = _g_cur.bottom;
      return true;
    }

  public:
    SSD1306()
    : _ctrlb(SSD1306_WORD)
    , _lines(SSD1306_MAX_HEIGHT >> 3)
    , _y_off(0)
    , _y_pos(0)
    , _x_pos(0)
    {
    }

    void begin(uint8_t nlines = 8)
    {

    //-------------------
    // SSD1306 128x32

		// --- display "OFF", sleep mode
		display(false);	

		// clock divide ratio (0x00=1) and oscillator frequency (0x8)
        ctrl(SSD1306_SET_CLOCK_DIVIDE_OSCILLATOR, 1);
        data(0x80);
		
		// mux-ed lines
        //multiplex(nlines);
        ctrl(SSD1306_SET_MULTIPLEX_RATIO, 1);
        data(0x1F);
		
		// --- display offset
        ctrl(SSD1306_SET_DISPLAY_OFFSET, 1);
        data(0x00);
		
		// start line
        ctrl(SSD1306_SET_DISPLAY_START_LINE, 0);
		
		// [2] charge pump setting (p62): 0x014 enable, 0x010 disable
		charge_pump(true);
		
		// --- memory addressing: 0x02 page addressing mode
        ctrl(SSD1306_SET_MEMORY_ADDRESSING_MODE, 1);
        data(0x02);

		// --- column address 0 is mapped to SEG_0 ---
        ctrl(SSD1306_SET_SEGMENT_NORMAL, 0);
        //ctrl(SSD1306_SET_SEGMENT_REMAP, 0);
		
		// --- C0: scan dir normal, C8: reverse
        ctrl(SSD1306_SET_COM_OUTPUT_SCAN_NORMAL, 0);
        //ctrl(SSD1306_SET_COM_OUTPUT_SCAN_REMAP, 0);
        
		// com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5)
        ctrl(SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION, 1);
        data(0x02);
		
		// --- set contrast ---
        ctrl(SSD1306_SET_CONTRAST_CONTROL, 1);
        data(0x7F);
		
		// --- pre-charge period; at Reset is 0x22
        ctrl(SSD1306_SET_PRECHARGE_PERIOD, 1);
        data(0xF1);
		
		// --- fuzzy: VCOMH deselect level --- 0x30: set it to 0.83 Vcc
        ctrl(SSD1306_SET_VCOMH_DESELECT_LEVEL, 1);
        data(0x40);
		
		// --- just in case: Deactivate the scroll
        //ctrl(SSD1306_DEACTIVATE_SCROLL, 0);
        
		// --- output ram to display, not e.g. all "1"s etc. ---
        //ctrl(SSD1306_ENTIRE_DISPLAY_OFF, 0);
        
		// --- none inverted pixel brightness ---
        ctrl(SSD1306_SET_NORMAL_DISPLAY, 0);

		// --- display "ON" ---
		display(true);	

    //-------------------

/* SSD1306 128x64
      //
      // enable charge pump
      //
      charge_pump(true);
      //
      // set multiplex
      //
      multiplex(nlines);
      //
      // display clear
      //
      clear();
      //
      // display on
      //
      display(true);
      //
      // flush
      //
      flush();
      //
      // delay for SH1106
      //
      delay(150);
*/
    }

    void flush()
    {
      if (_count)
      {
        IOBUS.write(_buffer, _count, ADDR);
        _count = 0;
      }
    }

    uint8_t status()
    {
      uint8_t rv = 0xFF;
      IOBUS.read(&rv, sizeof(rv), ADDR);
      return rv;
    }

    void clear()
    {
      for (uint8_t y = 0; y < _lines; ++y)
      {
        cursor(0, y);
        ctrl(SSD1306_GDDRAM, 1);
        for (uint8_t x = 0; x < SSD1306_MAX_WIDTH; ++x)
          data(0);
      }
      cursor(0, 0);
    }

    void display(bool enable)
    {
      ctrl(enable ? SSD1306_SET_DISPLAY_ON : SSD1306_SET_DISPLAY_OFF, 0);
    }

    void flip(bool enable)
    {
      ctrl(enable ? SSD1306_SET_COM_OUTPUT_SCAN_REMAP : SSD1306_SET_COM_OUTPUT_SCAN_NORMAL, 0);
      ctrl(enable ? SSD1306_SET_SEGMENT_REMAP : SSD1306_SET_SEGMENT_NORMAL, 0);
    }

    void white(bool enable)
    {
      ctrl(enable ? SSD1306_ENTIRE_DISPLAY_ON : SSD1306_ENTIRE_DISPLAY_OFF, 0);
    }

    void invert(bool enable)
    {
      ctrl(enable ? SSD1306_SET_INVERSE_DISPLAY : SSD1306_SET_NORMAL_DISPLAY, 0);
    }

    void contrast(uint8_t val)
    {
      ctrl(SSD1306_SET_CONTRAST_CONTROL, 1);
      data(val);
    }

    void cursor(uint8_t x, uint8_t y)
    {
      if ((x < (SSD1306_MAX_WIDTH >> 3)) && (y < _lines))
      {
        _x_pos = x;
        _y_pos = y;
        address(x << 3, y);
      }
    }

    void scroll()
    {
      _y_off = (_y_off + 1) & 0x07;
      ctrl(SSD1306_SET_DISPLAY_START_LINE | (_y_off << 3), 0);
      address(0, _lines - 1);
      ctrl(SSD1306_GDDRAM, 1);
      for (uint8_t x = 0; x < SSD1306_MAX_WIDTH; ++x)
        data(0);
    }

    void newline()
    {
      _x_pos = 0;
      if (_y_pos + 1 < _lines)
        ++_y_pos;
      else
        scroll();
      cursor(_x_pos, _y_pos);
    }

    void putChar(char c)
    {
      switch (c)
      {
        //
        // back space
        //
        case '\b':
          if (_x_pos)
            cursor(--_x_pos, _y_pos);
          break;
        //
        // caridge return
        //
        case '\r':
          if (_x_pos)
            cursor(_x_pos = 0, _y_pos);
          break;
        //
        // line feed
        //
        case '\n':
          newline();
          break;
        //
        // character
        //
        default:
          if ((c >= FONT_START_CHAR) && (c <= FONT_END_CHAR))
          {
            ctrl(SSD1306_GDDRAM, 1);
            const uint8_t *p = glyph(c);
            for (uint8_t i = 0; i < 8; ++i)
              data(read_byte(p++, OPT_CODE_PTR));
            if (++_x_pos >= (SSD1306_MAX_WIDTH >> 3))
              newline();
          }
          break;
      }
    }

    void printStr(char *str)
    {
      char c;
      while (c = *str++)
        putChar(c);
    }

    template <typename T>
    bool printInt(T val)
    {
      char   buf[12];
      int8_t len = sizeof(buf);
      int8_t neg = (val < 0);
      buf[--len] = 0;
      if (neg)
        val = -val;
      do
      {
        if (--len < neg)
          return false;
        buf[len] = (char)(val % 10) | '0';
      }
      while ((val /= 10) >= 1);
      if (neg)
        buf[--len] = '-';
      printStr(&buf[len]);
      return true;
    }

    void beginPaint()
    {
      _g_cur.left   = 0;
      _g_cur.right  = sizeof(_g_buf);
      _g_cur.top    = 0;
      _g_cur.bottom = 8;
      _g_wrt = false;
      memset(_g_buf, 0, sizeof(_g_buf));

      _g_wrt_pre_i = 0;
    }

    
    bool endPaint()
    {
      //
      // flush page
      //
      if (_g_wrt || _g_wrt_pre[_g_wrt_pre_i])
      {
        address(_g_cur.left, _g_cur.top >> 3);
        ctrl(SSD1306_GDDRAM, 1);
        uint8_t *p = _g_buf;
        for (int8_t i = sizeof(_g_buf); i; --i)
          data(*p++);
        //_g_wrt = false;
        memset(_g_buf, 0, sizeof(_g_buf));
      }
      _g_wrt_pre[_g_wrt_pre_i] = _g_wrt;
      _g_wrt = false;
      _g_wrt_pre_i++;
      //
      // select next page
      //
      if (_g_cur.right < SSD1306_MAX_WIDTH)
      {
        _g_cur.left  += sizeof(_g_buf);
        _g_cur.right += sizeof(_g_buf);
      }
      else
      {
        if (_g_cur.bottom >= (_lines << 3))
        {
          address(_x_pos << 3, _y_pos);
          return true;
        }
        _g_cur.left   = 0;
        _g_cur.right  = sizeof(_g_buf);
        _g_cur.top    += 8;
        _g_cur.bottom += 8;
      }
      return false;
    }

    bool pixel(int x, int y)
    {
      if (ptInVisible(x, y))
      {
        _g_buf[x - _g_cur.left] |= _BV(y & 7);
        return _g_wrt = true;
      }
      return false;
    }

    void moveto(int x, int y)
    {
      _g_org.x = x;
      _g_org.y = y;  
    }

    void lineto(int x, int y)
    {
      int x0 = _g_org.x;
      int y0 = _g_org.y;
      int dx = abs(x - x0);
      int dy = abs(y - y0);
      int sx = (x0 < x ? 1 : -1);
      int sy = (y0 < y ? 1 : -1);
      int er = dx - dy;
      while (dx >= dy ? x0 != x : y0 != y)
      {
        if (!pixel(x0, y0))
        {
          if (!hasIntersectClipRect(x0, y0, x, y))
            break;
        }
        int e2 = er + er;
        if (e2 > -dy)
        {
          er -= dy;
          x0 += sx;
        }
        if (e2 < dx)
        {
          er += dx;
          y0 += sy;
        }
      }
      _g_org.x = x;
      _g_org.y = y;
    }

    void circle(int x, int y, int radius)
    {
      RECT rc = {x - radius + 1, y - radius + 1, x + radius, y + radius};
      if (intersectClipRect(&rc))
      {
        int x0 = radius - 1;
        int y0 = 0;
        int dx = 1;
        int dy = 1;
        radius <<= 1;
        int er = dx - radius + 4; // 4 = Correction value for small circle
        while (x0 >= y0)
        {
          pixel(x + x0, y + y0);
          pixel(x + y0, y + x0);
          pixel(x - y0, y + x0);
          pixel(x - x0, y + y0);
          pixel(x - x0, y - y0);
          pixel(x - y0, y - x0);
          pixel(x + y0, y - x0);
          pixel(x + x0, y - y0);
          if (er <= 0)
          {
            er += dy;
            dy += 2;
            ++y0;
          }
          if (er > 0)
          {
            dx += 2;
            er += dx - radius;
            --x0;
          }
        }
      }
    }

    void rect(int x, int y, int w, int h)
    {
      if ((w > 0) && (h > 0))
      {
        moveto(x, y);
        if (w == 1)
          lineto(x, y + h);
        else if (h == 1)
          lineto(x + w, y);
        else
        {
          --w;
          --h;
          lineto(x + w, y);
          lineto(x + w, y + h);
          lineto(x, y + h);
          lineto(x, y);
        }
      }
    }

    void fillRect(int x, int y, int w, int h)
    {
      RECT rc = {x, y, x + w, y + h};
      if (intersectClipRect(&rc))
      {
        uint8_t fill = ~(_BV(rc.top & 7) - 1);
        if (rc.bottom & 7)
          fill &= (_BV(rc.bottom & 7) - 1);
        do
          _g_buf[rc.left - _g_cur.left] |= fill;
        while (++rc.left < rc.right);
        _g_wrt = true;
      }
    }

    void drawImage(int x, int y, int w, int h, uint8_t *image, uint8_t opt = OPT_DATA_PTR, uint8_t row = 0)
    {
      RECT rc = {x, y, x + w, y + h};
      if (intersectClipRect(&rc))
      {
        y -= row;
        uint8_t b_off = rc.left - _g_cur.left;
        uint8_t i_off = rc.left - x + ((rc.top - y) >> 3) * w;
        uint8_t r_off = y & 7;
        uint8_t mask = (_BV(rc.top & 7) - 1);
        if (rc.bottom & 7)
          mask |= ~(_BV(rc.bottom & 7) - 1);
        do
        {
          uint8_t b = read_byte(&image[i_off], opt);
          if (r_off)
          {
            if (y == rc.top)
              b <<= r_off;
            else
            {
              b >>= 8 - r_off;
              if ((rc.bottom & 7) == 0)
                b |= read_byte(&image[i_off + w], opt) << r_off;
            }
          }
          _g_buf[b_off] |= b & ~mask;
          ++b_off;
          ++i_off;
        }
        while (++rc.left < rc.right);
        _g_wrt = true;
      }
    }

    void drawDigitAnimation(int x, int y, uint8_t num1, uint8_t num2, uint8_t anime, uint8_t size)
    {
      uint8_t w, h, *f, *n;
      if ((num1 < 10) && (num2 < 10))
      {
        switch (size)
        {
          default:
            w = 8;
            h = 8;
            f = (uint8_t *)&SSD1306_FONT[num1 + ('0' - FONT_START_CHAR)];
            n = (uint8_t *)&SSD1306_FONT[num2 + ('0' - FONT_START_CHAR)];
            break;
#if OPT_FONT_MEDIUM_DIGIT
          case 1:
            w = 11;
            h = 16;
            f = (uint8_t *)&FONT_MEDIUM_DIGIT[num1];
            n = (uint8_t *)&FONT_MEDIUM_DIGIT[num2];
            anime += anime;
            break;
#endif
#if OPT_FONT_LARGE_DIGIT
          case 2:
            w = 19;
            h = 24;
            f = (uint8_t *)&FONT_LARGE_DIGIT[num1];
            n = (uint8_t *)&FONT_LARGE_DIGIT[num2];
            anime += anime + anime;
            break;
#endif
        }
        if ((num1 == num2) || (anime == 0))
          drawImage(x, y, w, h, f, OPT_CODE_PTR);
        else
        {
          h -= anime;
          drawImage(x, y, w, anime, n, OPT_CODE_PTR, h);
          drawImage(x, y + anime, w, h, f, OPT_CODE_PTR);
        }
      }
    }

    void drawDigit(int x, int y, uint8_t num, uint8_t size)
    {
      uint8_t w, h, *f;
      if (num < 10)
      {
        switch (size)
        {
          default:
            w = 8;
            h = 8;
            f = (uint8_t *)&SSD1306_FONT[num + ('0' - FONT_START_CHAR)];
            break;
#if OPT_FONT_MEDIUM_DIGIT
          case 1:
            w = 11;
            h = 16;
            f = (uint8_t *)&FONT_MEDIUM_DIGIT[num];
            break;
#endif
#if OPT_FONT_LARGE_DIGIT
          case 2:
            w = 19;
            h = 24;
            f = (uint8_t *)&FONT_LARGE_DIGIT[num];
            break;
#endif
        }
        drawImage(x, y, w, h, f, OPT_CODE_PTR);
      }
    }

    void drawcColon(int x, int y, uint8_t size)
    {
      uint8_t w, h, *f;
      switch (size)
      {
        default:
          w = 8;
          h = 8;
          f = (uint8_t *)&SSD1306_FONT[':' - FONT_START_CHAR];
          break;
#if OPT_FONT_LARGE_DIGIT
        case 2:
          w = 6;
          h = 24;
          f = (uint8_t *)FONT_LARGE_COLON;
          break;
#endif
      }
        drawImage(x, y,  w, h, f, OPT_CODE_PTR);
    }

    void drawText(int x, int y, char *str)
    {
      RECT rc = {x, y, x + (strlen(str) << 3), y + 8};
      if (intersectClipRect(&rc))
      {
        uint8_t b_off = rc.left - _g_cur.left;
        uint8_t i_off = rc.left - x;
        uint8_t r_off = y & 7;
        uint8_t mask = _BV(rc.top & 7) - 1;
        if (rc.bottom & 7)
          mask |= ~(_BV(rc.bottom & 7) - 1);
        do
        {
          uint8_t b = str[i_off >> 3];
          b = read_byte(glyph(b) + (i_off & 7), OPT_CODE_PTR);
          if (r_off)
          {
            if (y == rc.top)
              b <<= r_off;
            else
              b >>= 8 - r_off;
          }
          _g_buf[b_off] |= b & ~mask;
          ++b_off;
          ++i_off;
        }
        while (++rc.left < rc.right);
        _g_wrt = true;
      }
    }
    
};

#endif // _SSD1306_H
