/****************************************************************************/
//  Function:       Header file for TroykaDHT
//  Hardware:       DHT11, DHT21, DHT22
//  Arduino IDE:    Arduino-1.8.2
//  Author:         Igor Dementiev
//  Date:           Feb 22,2018
//  Version:        v1.0
//  by www.amperka.ru
/****************************************************************************/

#ifndef __TROYKA_DHT_H__
#define __TROYKA_DHT_H__

#include <Arduino.h>

#define DHT11 11
#define DHT21 21
#define DHT22 22

#define DHT_OK                   0
#define DHT_ERROR_CHECKSUM      -1
#define DHT_ERROR_TIMEOUT       -2
#define DHT_ERROR_NO_REPLY      -3

#define CELSIUS_TO_KELVIN   273.15

class DHT
{
public:
    explicit DHT(uint8_t pin, uint8_t type);
    void begin();
    int8_t read();
    int8_t getState() const { return _state; }
    float getTemperatureC() const { return _temperatureC; }
    float getTemperatureF() const { return _temperatureF; }
    float getTemperatureK() const { return _temperatureK; }
    float getHumidity() const { return _humidity; }
private:
    unsigned long pulseInLength(uint8_t pin, bool state, unsigned long timeout = 1000000L);
    uint8_t _pin;
    int8_t _type;
    int8_t _state;
    float _temperatureC;
    float _temperatureF;
    float _temperatureK;
    float _humidity;
};
#endif  // __TROYKA_DHT_H__