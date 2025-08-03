#include "AS5600.h"
#include "mbed.h"
#include <cstdint>
#include <stdint.h>



AS5600::AS5600(I2C &i2c, PinName dirPin) : _i2c(i2c) , _dir(dirPin) {
    _i2c.frequency(100000);
    _dirPin=dirPin;
}

bool AS5600::begin(bool flag_reset) {
    setDirection(true);//cwが正
    // 通信確認としてANGLEレジスタを読んでみる
    if(flag_reset){
        reset(read16(REG_RAW_ANGLE));
        ThisThread::sleep_for(1ms);
    }
    prev_ang = read16(REG_ANGLE);
    
    return prev_ang <= 4095;
}

int AS5600::reset(uint16_t angle){
    char data[3];
    data[0] = 0x01;               // ZPOS low byte register
    data[1] = (angle >> 8) & 0x0F; // high byte (only lower 4 bits used)
    data[2] = angle & 0xFF;       // low byte
    int chk = !_i2c.write(AS5600_ADDR, data, 3);

    flag_expandedAng = true;
    ThisThread::sleep_for(1ms);//データシートに書いてあった。。。
    return chk;
}

float AS5600::readRawAngle() {
    return (float)read16(REG_RAW_ANGLE) * ANGLE_SCALE;
}

float AS5600::readScaledAngle() {
    return (float)read16(REG_ANGLE) * ANGLE_SCALE;
}

int AS5600::readRawAngle16() {
    return read16(REG_RAW_ANGLE);
}

int AS5600::readScaledAngle16() {
    return read16(REG_ANGLE);
}
bool AS5600::isMagnetDetected() {
    uint8_t status = read8(REG_STATUS);
    return (status & 0x20); // MD bit
}

void AS5600::ExpandedAngle(float* angle){
    if(flag_expandedAng) {
        prev_ang = readScaledAngle16();
        flag_expandedAng = false;
    }

    ang16 = readScaledAngle16();
    delta = ang16 - prev_ang;
    if (delta > 2048) {
        // 例: 10 → 4090（逆回転）
        rotation_count--;
    } else if (delta < -2048) {
        // 例: 4090 → 10（正回転）
        rotation_count++;
    }
    prev_ang = ang16;
    *angle = rotation_count * 360.f + ang16 * ANGLE_SCALE;
}

uint8_t AS5600::readAGC() {
    return read8(REG_AGC);
}

void AS5600::setDirection(bool clockwise) {
    if (_dirPin!=NC) {
        _dir = !clockwise;
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
