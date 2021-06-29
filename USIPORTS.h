#ifndef _USIPORTS_H
#define _USIPORTS_H

#define USI_PORT_HIGH(p) (USI_PORT(p##_X) |=  _BV(p##_N))
#define USI_PORT_LOW(p)  (USI_PORT(p##_X) &= ~_BV(p##_N))
#define USI_PORT_INV(p)  (USI_PIN(p##_X)  |=  _BV(p##_N))
#define USI_PORT_IN(p)   (USI_PIN(p##_X)  &   _BV(p##_N))
#define USI_DDR_OUT(p)   (USI_DDR(p##_X)  |=  _BV(p##_N))
#define USI_DDR_IN(p)    (USI_DDR(p##_X)  &= ~_BV(p##_N))

#define USI_PORT(p) USI_PORT_REG(p)
#define USI_PIN(p)  USI_PIN_REG(p)
#define USI_DDR(p)  USI_DDR_REG(p)

#define USI_PORT_REG(p) PORT##p
#define USI_PIN_REG(p)  PIN##p
#define USI_DDR_REG(p)  DDR##p

#define USI_SDA_X USI_DI_X
#define USI_SDA_N USI_DI_N
#define USI_SCL_X USI_USCK_X
#define USI_SCL_N USI_USCK_N

#ifndef USI_PIN_POSITION
#define USI_PIN_POSITION 0
#endif

#if defined(__AVR_ATmega165__ ) || defined(__AVR_ATmega165A__ ) || defined(__AVR_ATmega165P__ ) || defined(__AVR_ATmega165PA__ ) || \
    defined(__AVR_ATmega169__ ) || defined(__AVR_ATmega169A__ ) || defined(__AVR_ATmega169P__ ) || defined(__AVR_ATmega169PA__ ) || \
    defined(__AVR_ATmega325__ ) || defined(__AVR_ATmega325A__ ) || defined(__AVR_ATmega325P__ ) || defined(__AVR_ATmega325PA__ ) || \
    defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3250A__) || defined(__AVR_ATmega3250P__) || defined(__AVR_ATmega3250PA__) || \
    defined(__AVR_ATmega329__ ) || defined(__AVR_ATmega329A__ ) || defined(__AVR_ATmega329P__ ) || defined(__AVR_ATmega329PA__ ) || \
    defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290P__) || defined(__AVR_ATmega3290PA__) || \
    defined(__AVR_ATmega645__ ) || defined(__AVR_ATmega645A__ ) || defined(__AVR_ATmega645P__ ) || \
    defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450P__) || \
    defined(__AVR_ATmega649__ ) || defined(__AVR_ATmega649A__ ) || defined(__AVR_ATmega649P__ ) || \
    defined(__AVR_ATmega6490__) || defined(__AVR_ATmega6490A__) || defined(__AVR_ATmega6490P__)

  #define USI_DI_X   E
  #define USI_DI_N   5
  #define USI_DO_X   E
  #define USI_DO_N   6
  #define USI_USCK_X E
  #define USI_USCK_N 4

  #define USI_OVF_vect USI_OVERFLOW_vect

#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny4313__)

  #define USI_DI_X   B
  #define USI_DI_N   5
  #define USI_DO_X   B
  #define USI_DO_N   6
  #define USI_USCK_X B
  #define USI_USCK_N 7

  #define USI_OVF_vect USI_OVERFLOW_vect

#elif defined(__AVR_ATtiny24A__) || defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny84A__)

  #define USI_DI_X   A
  #define USI_DI_N   6
  #define USI_DO_X   A
  #define USI_DO_N   5
  #define USI_USCK_X A
  #define USI_USCK_N 4

  #define USI_START_vect USI_STR_vect

#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)

  #define USI_DI_X   A
  #define USI_DI_N   6
  #define USI_DO_X   A
  #define USI_DO_N   5
  #define USI_USCK_X A
  #define USI_USCK_N 4

#elif defined(__AVR_ATtiny1634__)

  #define USI_DI_X   B
  #define USI_DI_N   1
  #define USI_DO_X   B
  #define USI_DO_N   2
  #define USI_USCK_X C
  #define USI_USCK_N 1

#elif defined(__AVR_ATtiny43U__)

  #define USI_DI_X   B
  #define USI_DI_N   4
  #define USI_DO_X   B
  #define USI_DO_N   5
  #define USI_USCK_X B
  #define USI_USCK_N 6

#elif defined(__AVR_ATtiny26__)

  #define USI_DI_X   B
  #define USI_DI_N   0
  #define USI_DO_X   B
  #define USI_DO_N   1
  #define USI_USCK_X B
  #define USI_USCK_N 2

  #define USI_START_vect USI_STRT_vect

#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)

  #define USI_DI_X   B
  #define USI_DI_N   0
  #define USI_DO_X   B
  #define USI_DO_N   1
  #define USI_USCK_X B
  #define USI_USCK_N 2

#elif defined(__AVR_ATtiny261__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny861__)

  #if (USI_PIN_POSITION == 0)
    #define USI_DI_X   B
    #define USI_DI_N   0
    #define USI_DO_X   B
    #define USI_DO_N   1
    #define USI_USCK_X B
    #define USI_USCK_N 2
  #else
    #define USI_DI_X   A
    #define USI_DI_N   0
    #define USI_DO_X   A
    #define USI_DO_N   1
    #define USI_USCK_X A
    #define USI_USCK_N 2
  #endif

#elif defined(__AVR_ATtiny87__ ) || defined(__AVR_ATtiny167__) || \
      defined(__AVR_ATA6616C__ ) || defined(__AVR_ATA6617C__ ) || defined(__AVR_ATA664251__)

  #if (USI_PIN_POSITION == 0)
    #define USI_DI_X   B
    #define USI_DI_N   0
    #define USI_DO_X   B
    #define USI_DO_N   1
    #define USI_USCK_X B
    #define USI_USCK_N 2
  #else
    #define USI_DI_X   A
    #define USI_DI_N   4
    #define USI_DO_X   A
    #define USI_DO_N   2
    #define USI_USCK_X A
    #define USI_USCK_N 5
  #endif

#endif

#endif //_USIPORTS_H