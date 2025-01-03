
/*
 * Board configuration (see examples 
 */
#if defined(USE_ESP32_DEV_MODULE)

  #warning "The LED of this board is not configured"
  #define BOARD_LED_PIN               LED_BUILTIN
  #define BOARD_BUTTON_PIN            0
  #define BOARD_BUTTON_ACTIVE_LOW     true
#endif


/*
 * Advanced options
 */

#define BUTTON_HOLD_TIME_INDICATION   3000
#define BUTTON_HOLD_TIME_ACTION       10000
#define BUTTON_PRESS_TIME_ACTION      50 
#define BOARD_PWM_MAX                 1023

#define BOARD_LEDC_CHANNEL         1

#define BOARD_LEDC_TIMER_BITS         10
#define BOARD_LEDC_BASE_FREQ          12000

#if !defined(CONFIG_DEVICE_PREFIX)
#define CONFIG_DEVICE_PREFIX          "Blynk"
#endif
#if !defined(CONFIG_AP_URL)
#define CONFIG_AP_URL                 "blynk.setup"
#endif
#if !defined(CONFIG_DEFAULT_SERVER)
#define CONFIG_DEFAULT_SERVER         "blynk.cloud"
#endif
#if !defined(CONFIG_DEFAULT_PORT)
#define CONFIG_DEFAULT_PORT           443
#endif

#define WIFI_CLOUD_MAX_RETRIES        500
#define WIFI_NET_CONNECT_TIMEOUT      50000
#define WIFI_CLOUD_CONNECT_TIMEOUT    50000
#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)

#define USE_PTHREAD

#define BLYNK_NO_DEFAULT_BANNER

#define DEBUG_PRINT(...)
#define DEBUG_PRINTF(...)

