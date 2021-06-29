#include <Arduino.h>
#include "USITWI.h"

#define END() do { USICR &= ~(_BV(USICS1) | _BV(USIOIE)); USIDR = 0xFF; return 0x00; } while (0)

typedef uint8_t (*USITWI_DISPATCH)();

volatile bool USITWI_usisif;

static uint8_t _myaddr;
static uint8_t *_write_buffer;
static uint8_t _write_length;
static uint8_t _write_count;
static uint8_t *_read_buffer;
static uint8_t _read_length;
static uint8_t _read_count;
static USITWI_CALLBACK _write_cb;
static USITWI_DISPATCH _dispatch;

static uint8_t read_next();
static uint8_t write_next();

static uint8_t read_start()
{
  if (USIDR & 0x01)
    END();
  _dispatch = read_next;
  USIDR = _read_buffer[_read_count];
  return 0x00;
}

static uint8_t read_next()
{
  if (++_read_count >= _read_length)
    END();
  _dispatch = read_start;
  USIDR = 0xFF;
  return 0x0E;
}

static uint8_t write_start()
{
  _dispatch = write_next;
  USIDR = 0xFF;
  return 0x00;
}

static uint8_t write_next()
{
  _write_buffer[_write_count] = USIDR;
  if (++_write_count >= _write_length)
    END();
  _dispatch = write_start;
  USIDR = 0x7F;
  return 0x0E;
}

static void callback()
{
  if (_write_count)
  {
    _read_length = 0;
    if (_write_cb)
      (*_write_cb)(_write_buffer, _write_count);
    _write_count = 0;
  }
}

static uint8_t slave_address()
{
  uint8_t addr = USIDR;
  if ((addr == 0) || (_myaddr == (addr & ~0x01)))
  {
    callback();
    if (addr & 0x01)
    {
      if (_read_length != 0)
      {
        _dispatch = read_start;
        USIDR = 0x7F;
        return 0x0E;
      }
    }
    else
    {
      if (_write_length != 0)
      {
        _dispatch = write_start;
        USIDR = 0x7F;
        return 0x0E;
      }
    }
  }
  END();
}

ISR(USI_OVF_vect)
{
  uint8_t cnt = (*_dispatch)();
  USITWI_DELAY_TSUDAT(USITWI_100K);
  USISR = _BV(USIOIF) | cnt;
}

ISR(USI_START_vect)
{
  USITWI_usisif = true;
  _dispatch = slave_address;
  while (USI_PORT_IN(USI_SCL)) ;
  USIDR = 0xFF;
  USICR |= (_BV(USICS1) | _BV(USIOIE));
  USITWI_CLR_IRQ();
}

void USITWI_handle()
{
  USICR &= ~_BV(USISIE);
  if (USITWI_usisif && (USISR & _BV(USIPF)))
  {
    USITWI_usisif = false;
    USICR &= ~(_BV(USICS1) | _BV(USIOIE));
    USIDR = 0xFF;
    callback();
  }
  USICR |= _BV(USISIE);
}

void USITWI_init(uint8_t addr, uint8_t *buf, uint8_t len)
{
  _myaddr = addr << 1;
  _write_buffer = buf;
  _write_length = len;
  _write_count  = 0;
}

void USITWI_setCallback(USITWI_CALLBACK func)
{
  _write_cb = func;
}

void USITWI_setReadBytes(uint8_t *buf, uint8_t len)
{
  _read_buffer = buf;
  _read_length = len;
  _read_count  = 0;
}