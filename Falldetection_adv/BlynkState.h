
enum State {
  MODE_WAIT_CONFIG,
  MODE_CONFIGURING,
  MODE_CONNECTING_NET,
  MODE_CONNECTING_CLOUD,
  MODE_RUNNING,
  MODE_OTA_UPGRADE,
  MODE_SWITCH_TO_STA,
  MODE_RESET_CONFIG,
  MODE_ERROR,

  MODE_MAX_VALUE
};

namespace BlynkState
{
  volatile State state = MODE_MAX_VALUE;

  State get()        { return state; }
  bool  is (State m) { return (state == m); }
  void  set(State m);
};

