#include "esphome.h"

#define SEESAW_HW_ID_CODE 0x55 ///< seesaw HW ID code

/** Module Base Addreses
 *  The module base addresses for different seesaw modules.
 */
enum
{
    SEESAW_STATUS_BASE = 0x00,
    SEESAW_TOUCH_BASE = 0x0F,
};

/** status module function address registers
 */
enum
{
    SEESAW_STATUS_HW_ID = 0x01,
    SEESAW_STATUS_VERSION = 0x02,
    SEESAW_STATUS_OPTIONS = 0x03,
    SEESAW_STATUS_TEMP = 0x04,
    SEESAW_STATUS_SWRST = 0x7F,
};

/** touch module function address registers
 */
enum {
    SEESAW_TOUCH_CHANNEL_OFFSET = 0x10,
};

class StemmaSoilSensor : public PollingComponent
{
    public:
        // Constructor
        StemmaSoilSensor() : PollingComponent (300000)
        {
            this->i2c_addr = 0x36;
        }

        Sensor * temperature_sensor = new Sensor();
        Sensor * moisture_sensor = new Sensor();

        uint8_t i2c_addr;

        bool setupFailed = true;

        // This will be called by App.setup ()
        void setup() override
        {
            // Start the seesaw.
            // Perform software reset.
            this->write8 (SEESAW_STATUS_BASE, SEESAW_STATUS_SWRST, 0xFF);
            delay (500);

            // Check for seesaw.
            uint8_t c = this->read8(SEESAW_STATUS_BASE, SEESAW_STATUS_HW_ID);
            if (c != SEESAW_HW_ID_CODE)
            {
                ESP_LOGE("soil_sensor", "Failed to connect to soil sensor.");
                // TODO: inform of failure
            }

            this->setupFailed = false;
            ESP_LOGI("soil_sensor", "Successfully reset soil sensor.");
            // TODO: inform of success

            this->temperature_sensor->set_unit_of_measurement("°F");
            this->temperature_sensor->set_accuracy_decimals(0);

            this->moisture_sensor->set_unit_of_measurement("moistcap");
            this->moisture_sensor->set_accuracy_decimals(0);
        }

        // This will be called every update_interval milliseconds.
        void update() override
        {
            if (this->setupFailed)
                return;

            float tempC = this->getTemp();
            uint16_t capread = this->touchRead(0);

            float tempF = (tempC * 1.8) + 32.0;

            this->temperature_sensor->publish_state(tempF);
            this->moisture_sensor->publish_state(capread);
        }

        // Get the temperature of the seesaw board in degrees Celsius
        float getTemp()
        {
            uint8_t buf[4];
            this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP, buf, 4, 1000);
            int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
            return (1.0 / (1UL << 16)) * ret;
        }

        // Read the current analog value of the capacitative sensor.
        uint16_t touchRead(uint8_t pin)
        {
            uint8_t buf[2];
            uint8_t p = pin;
            uint16_t ret = 65535;
            do {
                delay(1);
                this->read(SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET + p, buf, 2, 1000);
                ret = ((uint16_t)buf[0] << 8) | buf[1];
            } while (ret == 65535);
            return ret;
        }

        // Read a specified number of bytes into a buffer from the seesaw.
        void read(uint8_t regHigh, uint8_t regLow, uint8_t *buf, uint8_t num, uint16_t delay)
        {
            uint8_t pos = 0;

            // on arduino we need to read in 32 byte chunks
            while (pos < num) {
                uint8_t read_now = min(32, num - pos);
                Wire.beginTransmission((uint8_t)this->i2c_addr);
                Wire.write((uint8_t)regHigh);
                Wire.write((uint8_t)regLow);
                Wire.endTransmission();

                // TODO: tune this
                delayMicroseconds(delay);

                Wire.requestFrom((uint8_t)this->i2c_addr, read_now);

                for (int i = 0; i < read_now; i++) {
                    buf[pos] = Wire.read();
                    pos++;
                }
            }
        }

        // Read 1 byte from the specified seesaw register
        uint8_t read8(byte regHigh, byte regLow, uint16_t delay = 125)
        {
            uint8_t ret;
            this->read(regHigh, regLow, &ret, 1, delay);

            return ret;
        }

        // Write a specified number of bytes to the seesaw from the passed buffer.
        void write(uint8_t regHigh, uint8_t regLow, uint8_t *buf, uint8_t num)
        {
            Wire.beginTransmission((uint8_t)this->i2c_addr);
            Wire.write((uint8_t)regHigh);
            Wire.write((uint8_t)regLow);
            Wire.write((uint8_t *)buf, num);
            Wire.endTransmission();
        }

        // Write one byte to specified seesaw register.
        void write8(byte regHigh, byte regLow, byte value)
        {
            this->write(regHigh, regLow, &value, 1);
        }
};
