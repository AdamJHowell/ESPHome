#include "esphome.h"
#include "DFRobot_SHT20.h"

using namespace esphome;

class SHT20 : public PollingComponent {
 public:

  DFRobot_SHT20 myself;
  Sensor *temperature_sensor = new Sensor();
  Sensor *humidity_sensor = new Sensor();

  // Update every 5s (value in ms)
  SHT20() : PollingComponent(5000) { }

  void setup() override {
    myself.initSHT20();    // Init SHT20 Sensor
    delay(100);
    myself.checkSHT20();   // Check SHT20 Sensor
  }

  void update() override {
    float temperature = myself.readTemperature();
    temperature_sensor->publish_state(temperature);

    float humidity = myself.readHumidity();
    humidity_sensor->publish_state(humidity);
  }
};
