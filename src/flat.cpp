#include "flat.h"
#include "pins.h"
#include "AlpacaCoverCalibrator.h"
#define PWM_resolution 10
// creates pwm instance
ESP32_FAST_PWM *PWM_Instance;
float pwm_frequency = 1000.0f;
float pwm_dutyCycle = 0.0f;
uint8_t pwm_channel = 0;

void Flat::begin(void)
{
    pinMode(ry1pin, OUTPUT);
    // assigns PWM frequency of 1.0 KHz and a duty cycle of 0%, channel 0, 12-bit resolution
    PWM_Instance = new ESP32_FAST_PWM(pwmpin, pwm_frequency, pwm_dutyCycle, pwm_channel, PWM_resolution);
}

void Flat::pwr_on(void)
{
    digitalWrite(ry1pin, HIGH);
}
void Flat::pwr_off(void)
{
    digitalWrite(ry1pin, LOW);
}
bool Flat::pwr_stat(void)
{
    int stat = digitalRead(ry1pin);
    if (stat == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}
// duty cycle is percent on so MaxBrightness must be 100
// dim The required brightness in the range 0 to MaxBrightness 
void Flat::setdimlevel(int dim)
{
    pwm_dutyCycle = float(dim);
    PWM_Instance->setPWM(pwmpin, pwm_frequency, pwm_dutyCycle);
    Serial.print("PWM duty cycle ");
    Serial.println(pwm_dutyCycle);
}
int Flat::dimlevel(void)
{
    return (pwm_dutyCycle);
}