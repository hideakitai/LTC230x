#include <Wire.h>
#include <LTC230x.hpp>

ltc230x::LTC230x ltc2309;

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    delay(5000);

    Wire.begin();

    using namespace ltc230x;
    ltc2309.begin(Wire, address::AD1_LOW_AD0_LOW);
    ltc2309.set_channel(channel::POSITIVE_0_NEGATIVE_1);
    ltc2309.set_unipolar_bipolar_mode(uni_bi::UNIPOLAR);
    ltc2309.set_sleep_mode(sleep::WAKE);

    Serial.println("LTC2309 example start");
}

void loop()
{
    uint16_t raw_data = ltc2309.read_raw();
    int16_t adc_data = ltc2309.read();
    float voltage = ltc2309.read_voltage();

    Serial.print("Raw data : 0x");
    Serial.print(raw_data, HEX);
    Serial.print(", 12bit ADC data : ");
    Serial.print(adc_data);
    Serial.print(", Voltage : ");
    Serial.print(voltage);
    Serial.println(" [V]");

    delay(1000);
}
