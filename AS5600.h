#pragma once

#include "mbed.h"
#include <cstdint>

class AS5600 {
public:
    AS5600(I2C &i2c, PinName dirPin = NC);

    // 初期化?
    bool begin(bool flag_reset = false);

    // 角度の初期化
    int reset(uint16_t angle);

    // 生の角度（RAW_ANGLE）
    //0~360°
    float readRawAngle();

    // スケーリングされた角度（ANGLE）
    //0~360°
    float readScaledAngle();

    // 生の角度（RAW_ANGLE）
    //0~4096
    uint16_t readRawAngle16();

    // スケーリングされた角度（ANGLE）
    //0~4096
    uint16_t readScaledAngle16();

    // 接続されているか確認（MAGNET DETECTED）
    bool isMagnetDetected();

    //-∞~∞に値の範囲を拡張
    //threadの中で動かす前提
    void ExpandedAngle(float* angle);

    // AGC（自動ゲイン制御）値取得
    uint8_t readAGC();

    // DIRピン設定
    void setDirection(bool clockwise);

    uint16_t read16(char reg);
    uint8_t read8(char reg);

    int _error=0;//0:正常-1:磁石がない

    static constexpr float ANGLE_SCALE = 360.0f / 4096.0f;

private:
    I2C &_i2c;

    DigitalOut _dir;
    PinName _dirPin;

    static constexpr int AS5600_ADDR = 0x36 << 1; // 7bit address left-shifted for mbed
    static constexpr char REG_RAW_ANGLE = 0x0C;
    static constexpr char REG_ANGLE = 0x0E;
    static constexpr char REG_STATUS = 0x0B;
    static constexpr char REG_AGC = 0x1A;
    static constexpr char ADR_ZPOS = 0x01;


    uint16_t prev_ang = 0;
    uint16_t ang16 = 0;
    int32_t rotation_count = 0;
    int16_t delta = 0;
    bool flag_expandedAng  = true;


};
