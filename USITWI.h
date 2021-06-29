#ifndef _USITWI_H
#define _USITWI_H

#include <util/delay_basic.h>
#include "USIPORTS.h"

#define USITWI_100K 0
#define USITWI_400K 1

#define USITWI_SCL_WAIT() while (!USI_PORT_IN(USI_SCL))
#define USITWI_CLK_LOW()  USICR |= _BV(USITC) | _BV(USICLK)
#define USITWI_CLK_HIGH() USICR |= _BV(USITC)
#define USITWI_CLR_IRQ()  USISR = _BV(USISIF) | _BV(USIOIF) | _BV(USIPF)

#define USITWI_DELAY(t)        do { if (t) _delay_loop_1((uint8_t)(((t) * (F_CPU / 1000000) * 256 + 767) / 768)); } while (0)
#define USITWI_DELAY_THDSTA(m) USITWI_DELAY((m) ? 0.4 : 4.0)  // 0.6/4.0
#define USITWI_DELAY_TLOW(m)   USITWI_DELAY((m) ? 1.0 : 4.7)  // 1.3/4.7
#define USITWI_DELAY_THIGH(m)  USITWI_DELAY((m) ? 0.0 : 4.0)  // 0.6/4.0
#define USITWI_DELAY_TSUSTA(m) USITWI_DELAY((m) ? 0.0 : 4.7)  // 0.6/4.7
#define USITWI_DELAY_TSUSTO(m) USITWI_DELAY((m) ? 0.0 : 4.0)  // 0.6/4.0
#define USITWI_DELAY_TSUDAT(m) USITWI_DELAY((m) ? 0.1 : 0.25) // 0.1/0.25
#define USITWI_DELAY_TBUF(m)   USITWI_DELAY((m) ? 1.0 : 4.7)  // 1.3/4.7

typedef void (*USITWI_CALLBACK)(uint8_t *buf, uint8_t len);

extern volatile bool USITWI_usisif;
extern void USITWI_init(uint8_t addr, uint8_t *buf, uint8_t len);
extern void USITWI_setCallback(USITWI_CALLBACK func);
extern void USITWI_setReadBytes(uint8_t *buf, uint8_t len);
extern void USITWI_handle();

template<uint8_t TWI_CLK = USITWI_400K, uint8_t ADDR = 0x00, uint8_t SIZE = 16>
class USITWI
{
  private:
    uint8_t _buffer[SIZE];
    bool _usidc;
    
  protected:
    void start()
    {
      _usidc = false;
      //
      // wait bus free
      //
      do
      {
        handle();
        USICR &= ~_BV(USISIE);
      }
      while (USITWI_usisif || !USI_PORT_IN(USI_SDA) || !USI_PORT_IN(USI_SCL));
      //
      // start confition
      //
      USITWI_usisif = true;
      USIDR = 0x00;  // SDA LOW
      USITWI_DELAY_THDSTA(TWI_CLK);
      USI_PORT_LOW(USI_SCL);
      USICR |= _BV(USICS1);
      USITWI_CLR_IRQ();
      if (TWI_CLK == USITWI_100K)
        USITWI_DELAY_TLOW(TWI_CLK);
    }

    void stop(bool restart)
    {
      USICR &= ~_BV(USICS1);
      //
      // setup for repeated start condition
      //
      if (restart)
      {
        USIDR = 0xFF; // SDA HIGH;
        USITWI_DELAY_TSUDAT(TWI_CLK);
        USI_PORT_HIGH(USI_SCL);
        USITWI_SCL_WAIT();
        USITWI_DELAY_TSUSTA(TWI_CLK);
      }
      //
      // stop condition
      //
      else
      {
        USIDR = 0x00; // SDA LOW
        USITWI_DELAY_TSUDAT(TWI_CLK);
        USI_PORT_HIGH(USI_SCL);
        USITWI_SCL_WAIT();
        USITWI_DELAY_TSUSTO(TWI_CLK);
        USIDR = 0xFF; // SDA HIGH;
        USITWI_DELAY_TBUF(TWI_CLK);
      }
      USITWI_usisif = false;
      USICR |= _BV(USISIE);
    }

    uint8_t transfer(uint8_t data, uint8_t dc)
    {
      USIDR = data;
      USISR = _BV(USIOIF);
      while (1)
      {
        USITWI_CLK_HIGH();
        USITWI_SCL_WAIT();
        //
        // data collision ?
        //
        if (USISR & dc)
        {
          _usidc = true;
          USICR &= ~_BV(USICS1);
          USIDR = 0xFF;
          break;
        }
        USITWI_DELAY_THIGH(TWI_CLK);
        USITWI_CLK_LOW();
        if (TWI_CLK == USITWI_100K)
        {
          USITWI_DELAY_TLOW(TWI_CLK);
          if (USISR & _BV(USIOIF))
            break;
        }
        else
        {
          if (USISR & _BV(USIOIF))
            break;
          USITWI_DELAY_TLOW(TWI_CLK);
        }
      }
      USISR = _BV(USIOIF);
      return USIDR;
    }

    bool ack()
    {
      USI_PORT_HIGH(USI_SCL);
      USITWI_SCL_WAIT();
      USITWI_DELAY_THIGH(TWI_CLK);
      USI_PORT_LOW(USI_SCL);
      if (TWI_CLK == USITWI_100K)
        USITWI_DELAY_TLOW(TWI_CLK);
      return !(USIDR & 0x01);
    }

  public:
    USITWI()
    : _usidc(0)
    {
    }

    void begin()
    {
      USITWI_init(ADDR, _buffer, sizeof(_buffer));
      USITWI_CLR_IRQ();
      USIDR = 0xFF;
      USICR = _BV(USISIE) | _BV(USIWM1) | _BV(USIWM0);
      USI_PORT_HIGH(USI_SDA);
      USI_PORT_HIGH(USI_SCL);
      USI_DDR_OUT(USI_SDA);
      USI_DDR_OUT(USI_SCL);
    }

    void end()
    {
      USI_DDR_IN(USI_SDA);
      USI_DDR_IN(USI_SCL);
      USI_PORT_LOW(USI_SDA);
      USI_PORT_LOW(USI_SCL);
      USICR = 0;
    }

    uint8_t writeAndRead(uint8_t *wbuf, uint8_t wlen, uint8_t *rbuf, uint8_t rlen, uint8_t addr)
    {
      return write(wbuf, wlen, addr, true) == wlen ? read(rbuf, rlen, addr) : 0;
    }

    uint8_t write(const uint8_t *buf, uint8_t len, uint8_t addr, bool restart = false)
    {
      uint8_t cnt;
      addr <<= 1;
      do
      {
        cnt = 0;
        //
        // start condition
        //
        start();
        //
        // transmit slave address
        //
        transfer(addr, _BV(USIDC));
        //
        // loop
        //
        while (!_usidc)
        {
          //
          // receive ack
          //
          USIDR = 0xFF;
          //
          // ack
          //
          if (!ack())
            break;
          //
          // end of transfer ?
          //
          if (cnt >= len)
            break;
          //
          // transmit data
          //
          transfer(buf[cnt++], _BV(USIDC));
        }
      }
      while (_usidc);
      //
      // stop or repeated start condition
      //
      stop(restart && (cnt == len));
      return cnt;
    }

    uint8_t read(uint8_t *buf, uint8_t len, uint8_t addr, bool restart = false)
    {
      uint8_t cnt = 0;
      if (len)
      {
        addr = (addr << 1) | 0x01;
        do
        {
          cnt = 0;
          //
          // start condition
          //
          start();
          //
          // transmit slave address
          //
          transfer(addr, _BV(USIDC));
          //
          // loop
          //
          while (!_usidc)
          {
            //
            // transmit ack
            //
            USIDR = ((cnt > 0) && (cnt < len) ? 0x7F : 0xFF);
            //
            // ack
            //
            if (!ack())
              break;
            //
            // ack collision ?
            //
            if (cnt >= len)
            {
              _usidc = true;
              USICR &= ~_BV(USICS1);
              USIDR = 0xFF;
              break;
            }
            //
            // receive data
            //
            buf[cnt++] = transfer(0xFF, 0);
          }
        }
        while (_usidc);
        //
        // stop or repeated start condition
        //
        stop(restart && (cnt == len));
      }
      return cnt;
    }

    void handle()
    {
      return USITWI_handle();
    }

    void setCallback(USITWI_CALLBACK func)
    {
      USITWI_setCallback(func);
    }

    void setReadBytes(uint8_t *buf, uint8_t len)
    {
      USITWI_setReadBytes(buf, len);
    }
};

#endif // _USITWI_H