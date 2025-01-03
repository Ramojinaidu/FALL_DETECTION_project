
void indicator_run();

#if !defined(BOARD_LED_BRIGHTNESS)
#define BOARD_LED_BRIGHTNESS 255
#endif



#define DIMM(x)    ((uint32_t)(x)*(BOARD_LED_BRIGHTNESS)/255)
#define TO_PWM(x)  ((uint32_t)(x)*(BOARD_PWM_MAX)/255)

class Indicator {
public:
  Indicator() {
  }

  void init() {
    m_Counter = 0;
    initLED();
  }

  uint32_t run() {
    State currState = BlynkState::get();

    // Reset counter if indicator state changes
    if (m_PrevState != currState) {
      m_PrevState = currState;
      m_Counter = 0;
    }

    const long t = millis();
    if (g_buttonPressed) {
      if (t - g_buttonPressTime > BUTTON_HOLD_TIME_ACTION)     { return beatLED((int[]){ 100, 100 }); }
      if (t - g_buttonPressTime > BUTTON_HOLD_TIME_INDICATION) { return waveLED(1000); }
    }
    switch (currState) {
    case MODE_RESET_CONFIG:
    case MODE_WAIT_CONFIG:       return beatLED((int[]){ 50, 500 });
    case MODE_CONFIGURING:       return beatLED((int[]){ 200, 200 });
    case MODE_CONNECTING_NET:    return beatLED((int[]){ 50, 500 });
    case MODE_CONNECTING_CLOUD:  return beatLED((int[]){ 100, 100 });
    case MODE_RUNNING:           return waveLED(5000);
    case MODE_OTA_UPGRADE:       return beatLED((int[]){ 50, 50 });
    default:                     return beatLED((int[]){ 80, 100, 80, 1000 } );
    }
  }

protected:

  /*
   * LED drivers
   */
  // Single color LED
  #if defined(BOARD_LED_PIN)
    void initLED() {
      ledcAttach(BOARD_LED_PIN,BOARD_LEDC_BASE_FREQ,BOARD_LEDC_TIMER_BITS);
    }

    void setLED(uint32_t color) {
      ledcWrite(BOARD_LEDC_CHANNEL, TO_PWM(color));
    }
  #else
    #warning Invalid LED configuration.

    void initLED() {
    }

    void setLED(uint32_t color) {
    }

  #endif


  /*
   * Animations
   */

  uint32_t skipLED() {
    return 20;
  }

  template<typename T>
  uint32_t beatLED(const T& beat) {
    const uint8_t cnt = sizeof(beat)/sizeof(beat[0]);
    setLED((m_Counter % 2 == 0) ? BOARD_LED_BRIGHTNESS : 0);
    uint32_t next = beat[m_Counter % cnt];
    m_Counter = (m_Counter+1) % cnt;
    return next;
  }

  uint32_t waveLED(unsigned breathePeriod) {
    uint32_t brightness = (m_Counter < 128) ? m_Counter : 255 - m_Counter;

    setLED(DIMM(brightness*2));

    // This function relies on the 8-bit, unsigned m_Counter rolling over.
    m_Counter = (m_Counter+1) % 256;
    return breathePeriod / 256;
  }



private:
  uint8_t m_Counter;
  State   m_PrevState;
};

Indicator indicator;

/*
 * Animation timers
 */

#include <pthread.h>

pthread_t blinker;

void* indicator_thread(void*) {
  while (true) {
    uint32_t returnTime = indicator.run();
    returnTime = BlynkMathClamp(returnTime, 1, 10000);
    vTaskDelay(returnTime);
  }
}
void indicator_init() {
  indicator.init();
  pthread_create(&blinker, NULL, indicator_thread, NULL);
}


