#pragma once
#ifndef LTC230X_HPP
#define LTC230X_HPP

#include <Arduino.h>
#include <Wire.h>

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
    AD1_HIGH_AD0_HIGH = 0x28
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

class LTC230x
{
    TwoWire *wire;
    address::Address addr;
    channel::Channel channel;
    uni_bi::UnipolarBipoarMode uni_bi;
    sleep::SleepMode sleep;

public:
    /// @brief Initialize the LTC230x
    /// @param wire TwoWire object
    /// @param addr LTC230x I2C address
    void begin(TwoWire &wire, address::Address addr)
    {
        this->wire = &wire;
        this->addr = addr;
    }

    /// @brief Read raw 16bit data from the LTC230x
    /// @return Raw unsigned 16bit data
    uint16_t read_raw()
    {
        uint8_t din_cmd = this->channel | this->uni_bi | this->sleep;
        uint16_t raw_data {};
        uint8_t err = this->read_word(this->addr, din_cmd, raw_data);
        if (err != 0) {
            Serial.print("I2C error: ");
            Serial.println(err);
            return 0;
        }
        return raw_data;
    }

    /// @brief Read signed 12bit ADC data from the LTC230x
    /// @return Signed 12bit ADC data
    int16_t read()
    {
        uint16_t raw_data = this->read_raw();
        return static_cast<int16_t>(raw_data) >> 4;
    }

    /// @brief Read voltage from the LTC230x
    /// @return Voltage [V]
    float read_voltage()
    {
        static constexpr float FS = 4.096f;
        static constexpr float LSB = FS / 4096.f;
        int16_t adc_data = this->read();
        return static_cast<float>(adc_data) * LSB;
    }

    /// @brief Set the LTC230x I2C address
    /// @param addr LTC230x I2C address
    void set_address(address::Address addr)
    {
        this->addr = addr;
    }

    /// @brief Set the LTC230x channel
    /// @param channel LTC230x channel
    void set_channel(channel::Channel channel)
    {
        this->channel = channel;
    }

    /// @brief Set the LTC230x unipolar/bipolar mode
    /// @param uni_bi LTC230x unipolar/bipolar mode
    void set_unipolar_bipolar_mode(uni_bi::UnipolarBipoarMode uni_bi)
    {
        this->uni_bi = uni_bi;
    }

    /// @brief Set the LTC230x sleep mode
    /// @param sleep LTC230x sleep mode
    void set_sleep_mode(sleep::SleepMode sleep)
    {
        this->sleep = sleep;
    }

private:
    /// @brief Read a word from the LTC230x
    /// @param addr The I2C address of the LTC230x
    /// @param din The Din command
    /// @param data The data read from the LTC230x
    /// @return I2C error code
    ///         https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
    ///         0: success.
    ///         1: data too long to fit in transmit buffer.
    ///         2: received NACK on transmit of address.
    ///         3: received NACK on transmit of data.
    ///         4: other error.
    ///         5: timeout
    uint8_t read_word(uint8_t addr, uint8_t din, uint16_t &data)
    {
        this->wire->beginTransmission(addr);
        this->wire->write(din);
        uint8_t err = this->wire->endTransmission();
        if (err != 0) {
            return err;
        }

        this->wire->requestFrom(addr, 2);
        data = this->wire->read() << 8;


        return 0;
    }
};

} // namespace ltc230x

#endif // LTC230X_HPP
