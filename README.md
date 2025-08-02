# AS5600
磁気非接触式アブソリュートロータリーエンコーダーas5600のライブラリです。 \
磁石はセンサから1mm程離してください \
VCC:5V \
OUT:なし \
GND:GND
DIR:DigitalOut(L:CWが正,H:CCWが正で値取得)
SCL:SCL
SDA:SDA
GPO:GND(3.3Vで永久書き込みモード、ただ使わないで)
以下サンプルプログラムです


include "mbed.h"
#include "AS5600.h"

I2C i2c(PB_7, PB_6); // SDA, SCL
AS5600 sensor(i2c);//DIRピンはGND(CWが正)
UnbufferedSerial pc(USBTX,USBRX,115200);
int main() {
    sensor.begin();

    while (true) {
        float angle = sensor.readScaledAngle();
        bool magnetOK = sensor.isMagnetDetected();
        printf("Angle: %d.%d | Magnet OK: %s\n", (int)angle,(int)(angle*100.)%100, magnetOK ? "Yes" : "No");
        ThisThread::sleep_for(1ms);
    }
}
