#pragma once

#include "mbed.h"

class AS5600 {
public:
    AS5600(I2C &i2c, PinName dirPin = NC);

    

    // 初期化
    bool begin();

    // 生の角度（RAW_ANGLE）
    float readRawAngle();

    // スケーリングされた角度（ANGLE）
    float readScaledAngle();

    // 接続されているか確認（MAGNET DETECTED）
    bool isMagnetDetected();

    // AGC（自動ゲイン制御）値取得
    uint8_t readAGC();

    // DIRピン設定
    void setDirection(bool clockwise);

    uint16_t read16(char reg);
    uint8_t read8(char reg);

private:
    I2C &_i2c;

    DigitalOut _dir;
    PinName _dirPin;

    static constexpr int AS5600_ADDR = 0x36 << 1; // 7bit address left-shifted for mbed
    static constexpr char REG_RAW_ANGLE = 0x0C;
    static constexpr char REG_ANGLE = 0x0E;
    static constexpr char REG_STATUS = 0x0B;
    static constexpr char REG_AGC = 0x1A;

};
