#include "AS5600.h"
#include "mbed.h"
AS5600::AS5600(I2C &i2c, PinName dirPin) : _i2c(i2c) {
    if (dirPin != NC) {
        _dir = new DigitalOut(dirPin);
    } else {
        _dir = nullptr;
    }
}

bool AS5600::begin() {
    // 通信確認としてANGLEレジスタを読んでみる
    uint16_t angle = readScaledAngle();
    return angle <= 4095;
}

uint16_t AS5600::readRawAngle() {
    return read16(REG_RAW_ANGLE);
}

uint16_t AS5600::readScaledAngle() {
    return read16(REG_ANGLE);
}

bool AS5600::isMagnetDetected() {
    uint8_t status = read8(REG_STATUS);
    return (status & 0x20); // MD bit
}

uint8_t AS5600::readAGC() {
    return read8(REG_AGC);
}

void AS5600::setDirection(bool clockwise) {
    if (_dir) {
        *_dir = !clockwise;
    }
}

uint16_t AS5600::read16(char reg) {
    char data[2];
    _i2c.write(AS5600_ADDR, &reg, 1, true);
    _i2c.read(AS5600_ADDR, data, 2);
    return (data[0] << 8) | data[1];
}

uint8_t AS5600::read8(char reg) {
    char data;
    _i2c.write(AS5600_ADDR, &reg, 1, true);
    _i2c.read(AS5600_ADDR, &data, 1);
    return data;
}
