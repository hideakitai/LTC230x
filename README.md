# LTC230x

Arduino library for LTC230x ([LTC2301](https://www.analog.com/en/products/ltc2301.html), [LTC2305](https://www.analog.com/en/products/ltc2305.html), and [LTC2309](https://www.analog.com/en/products/ltc2309.html)) 12-Bit ADCs with I2C Interface

## Usage

```cpp
#include <Wire.h>
#include <LTC230x.hpp>

ltc230x::LTC230x ltc2309;

void setup()
{
    Wire.begin();

    using namespace ltc230x;
    ltc2309.begin(Wire, address::AD1_LOW_AD0_LOW);
    ltc2309.set_channel(channel::POSITIVE_0_NEGATIVE_1);
    ltc2309.set_unipolar_bipolar_mode(uni_bi::UNIPOLAR);
    ltc2309.set_sleep_mode(sleep::WAKE);
}

void loop()
{
    uint16_t raw_data = ltc2309.read_raw();
    int16_t adc_data = ltc2309.read();
    float voltage = ltc2309.read_voltage();

    // Use read data
}
```

## APIs

```cpp
/// @brief Initialize the LTC230x
/// @param wire TwoWire object
/// @param addr LTC230x I2C address
void begin(TwoWire &wire, address::Address addr);

/// @brief Read raw 16bit data from the LTC230x
/// @return Raw unsigned 16bit data
uint16_t read_raw();

/// @brief Read signed 12bit ADC data from the LTC230x
/// @return Signed 12bit ADC data
int16_t read();

/// @brief Read voltage from the LTC230x
/// @return Voltage [V]
float read_voltage();

/// @brief Set the LTC230x I2C address
/// @param addr LTC230x I2C address
void set_address(address::Address addr);

/// @brief Set the LTC230x channel
/// @param channel LTC230x channel
void set_channel(channel::Channel channel);

/// @brief Set the LTC230x unipolar/bipolar mode
/// @param uni_bi LTC230x unipolar/bipolar mode
void set_unipolar_bipolar_mode(uni_bi::UnipolarBipoarMode uni_bi);

/// @brief Set the LTC230x sleep mode
/// @param sleep LTC230x sleep mode
void set_sleep_mode(sleep::SleepMode sleep);
```

## Configurations

```cpp
namespace ltc230x {

namespace address {

enum Address : uint8_t
{
    AD1_LOW_AD0_LOW = 0x08,
    AD1_LOW_AD0_FLOAT = 0x09,
    AD1_LOW_AD0_HIGH = 0x0A,
    AD1_FLOAT_AD0_HIGH = 0x0B,
    AD1_FLOAT_AD0_FLOAT = 0x18,
    AD1_FLOAT_AD0_LOW = 0x19,
    AD1_HIGH_AD0_LOW = 0x1A,
    AD1_HIGH_AD0_FLOAT = 0x1B,
    AD1_HIGH_AD0_HIGH = 0x14
};

constexpr uint8_t GLOBAL = 0x6B;

} // namespace address

// bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0
// S/D   | O/S   | S1    | S0    | UNI   | SLP   | X     | X

namespace channel {

// bit 7 | bit 6 | bit 5 | bit 4
// S/D   | O/S   | S1    | S0
enum Channel : uint8_t
{
    POSITIVE_0_NEGATIVE_1 = 0x00, // default
    POSITIVE_2_NEGATIVE_3 = 0x10,
    POSITIVE_4_NEGATIVE_5 = 0x20,
    POSITIVE_6_NEGATIVE_7 = 0x30,
    NEGATIVE_0_POSITIVE_1 = 0x40,
    NEGATIVE_2_POSITIVE_3 = 0x50,
    NEGATIVE_4_POSITIVE_5 = 0x60,
    NEGATIVE_6_POSITIVE_7 = 0x70,
    POSITIVE_0_NEGATIVE_COM = 0x80,
    POSITIVE_1_NEGATIVE_COM = 0x90,
    POSITIVE_2_NEGATIVE_COM = 0xA0,
    POSITIVE_3_NEGATIVE_COM = 0xB0,
    POSITIVE_4_NEGATIVE_COM = 0xC0,
    POSITIVE_5_NEGATIVE_COM = 0xD0,
    POSITIVE_6_NEGATIVE_COM = 0xE0,
    POSITIVE_7_NEGATIVE_COM = 0xF0
};

} // namespace channel


namespace uni_bi {

// bit 3
// UNI
enum UnipolarBipoarMode : uint8_t
{
    BIPOLAR = 0x00, // default
    UNIPOLAR = 0x08
};

} // namespace uni_bi

namespace sleep {

// bit 2
// SLP
enum SleepMode : uint8_t
{
    WAKE = 0x00, // default
    SLEEP = 0x04
};

} // namespace sleep

} // namespace ltc230x
```

## License

MIT
