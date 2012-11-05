/*
 * File:   MPU6050.cpp
 * Author: matt
 *
 * Created on 22 October 2012, 11:39
 */

#include "MPU6050.h"

MPU6050Class MPU6050Interface;

MPU6050Class::MPU6050Class()
{
    initialise_();
}

MPU6050Class::~MPU6050Class()
{

}

bool MPU6050Class::getSensors(s_rawData* rawData)
{
    uint8_t buf[14];
    I2CInterface.readRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, buf, 14);
    rawData->x = static_cast<int16_t>((buf[0]<<8)|buf[1]);
    rawData->y = static_cast<int16_t>((buf[2]<<8)|buf[3]);
    rawData->z = static_cast<int16_t>((buf[4]<<8)|buf[5]);
    rawData->temp = static_cast<int16_t>((buf[6]<<8)|buf[7]);
    rawData->p = static_cast<int16_t>((buf[8]<<8)|buf[9]);
    rawData->q = static_cast<int16_t>((buf[10]<<8)|buf[11]);
    rawData->r = static_cast<int16_t>((buf[12]<<8)|buf[13]);
}

bool MPU6050Class::checkConnection()
{
    I2CInterface.readRegister(MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I, buf_, 1);
    if((unsigned char)buf_[0] == 0x68)
    {
        std::cout << "MPU6050 connection verified" << std::endl;
        return true;
    }
    else
    {
        std::cout << "MPU6050 address read failed, expected 0x68, recieved " << std::hex << (unsigned int)buf_[0] << std::endl;
        return false;
    }
}

void MPU6050Class::initialise_()
{
    setPowerManagement1_(MPU6050_CLOCK_PLL_ZGYRO); //Clear sleep bit and set clock source
    setSampleRateDivider_(0);
    setGyroConfig_(MPU6050_GYRO_FS_2000);
    setAccelConfig_(MPU6050_ACCEL_FS_16);
}

bool MPU6050Class::setSampleRateDivider_(unsigned char value)
{
    return(I2CInterface.writeRegister(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, &value, 1));
}

bool MPU6050Class::setDLPFConfig_(unsigned char cutoff)
{
    return(I2CInterface.writeRegister(MPU6050_ADDRESS, MPU6050_RA_CONFIG, &cutoff, 1));
}

bool MPU6050Class::setGyroConfig_(unsigned char config)
{
    return(I2CInterface.writeRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, &config, 1));
}

bool MPU6050Class::setAccelConfig_(unsigned char config)
{
    return(I2CInterface.writeRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, &config, 1));
}

bool MPU6050Class::setPowerManagement1_(unsigned char config)
{
    return(I2CInterface.writeRegister(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, &config, 1));
}