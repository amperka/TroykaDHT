/****************************************************************************/
//  Function:       Cpp file for TroykaDHT
//  Hardware:       DHT11, DHT21, DHT22
//  Arduino IDE:    Arduino-1.8.2
//  Author:         Igor Dementiev
//  Date:           Feb 22,2018
//  Version:        v1.0
//  by www.amperka.ru
/****************************************************************************/

#include "TroykaDHT.h"

DHT::DHT(uint8_t pin, uint8_t type) {
    _pin = pin;
    _type = type;
}

void DHT::begin() {
}

int8_t DHT::read() {
    // буффер данных
    uint8_t data[5];
    // бит данных
    uint8_t dataBit;
    // контрольнная сумма
    uint8_t checkSum;
    // очистка буффера
    for (int i = 0; i < 5; i++)
        data[i] = 0;
    // прижимаем шину к земле на 18 мс
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delay(18);
    // отпускаем шину
    pinMode(_pin, INPUT_PULLUP);
    // проверяем реакцию датчика
    // в течении 20-40 мкс датчик зажимает шину в 0 и держит 80 мкс 
    // потом отпускает на 80 мкс (формирует сигнал присутствия)
    if (pulseInLength(_pin, HIGH, 40) == 0) {
        _state = DHT_ERROR_NO_REPLY;
        return _state;
    }

    if (pulseInLength(_pin, LOW, 80) == 0) {
        _state = DHT_ERROR_NO_REPLY;
        return _state;
    }

    if (pulseInLength(_pin, HIGH, 80) == 0) {
        _state = DHT_ERROR_NO_REPLY;
        return _state;
    }

    // считываем 40 бит данных
    // с одинаковым стартом — зажатием шины около 50 мкс и битом данных
    // датчик «отпускает» шину на ~27мкс если хочет передать «0», или на ~70мкс если хочет передать «1»
    for (int i = 0; i < 40; i++) {
        pulseInLength(_pin, LOW, 50);
        dataBit = pulseInLength(_pin, HIGH, 100);
        if (dataBit) {
            data[i / 8] <<= 1;
            data[i / 8] += dataBit > 45 ? 1 : 0;
        } else { 
            _state = DHT_ERROR_TIMEOUT;
            return _state;
        }
    }

    // проверка контрольной суммы
    checkSum = data[0] + data[1] + data[2] + data[3];
    // проверка контрольнной суммы
    if (data[4] != checkSum) {
        _state = DHT_ERROR_CHECKSUM;
        return _state;
    }

    // запись данных
    switch (_type) {
        case DHT11:
            _humidity = data[0];
            _temperatureC = data[3] & 0x80 ? (data[2] + (1 - (data[3] & 0x7F) * 0.1)) * -1 : (data[2] + (data[3] & 0x7F) * 0.1);
            _temperatureF = (_temperatureC * 9.0 / 5.0) + 32.0;
            _temperatureK = _temperatureC + CELSIUS_TO_KELVIN;
            break;
        case DHT21:
            _humidity = ((data[0] << 8) + data[1]) * 0.1;
            _temperatureC = (((data[2] & 0x7F) << 8) + data[3]) * (data[2] & 0x80 ? -0.1 : 0.1);
            _temperatureF = (_temperatureC * 9.0 / 5.0) + 32.0;
            _temperatureK = _temperatureC + CELSIUS_TO_KELVIN;
            break;
        case DHT22:
            _humidity = ((data[0] << 8) + data[1]) * 0.1;
            _temperatureC = (((data[2] & 0x7F) << 8) + data[3]) * (data[2] & 0x80 ? -0.1 : 0.1);
            _temperatureF = (_temperatureC * 9.0 / 5.0) + 32.0;
            _temperatureK = _temperatureC + CELSIUS_TO_KELVIN;
            break;
    }

    _state = DHT_OK;
    return _state;
}

unsigned long DHT::pulseInLength(uint8_t pin, bool state, unsigned long timeout) {
    unsigned long startMicros = micros();
    // ожидание окончания текущего пульса
    while (digitalRead(pin) == state) {
        if (micros() - startMicros > timeout)
            return 0;
    }
    return micros() - startMicros;
}