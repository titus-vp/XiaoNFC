#ifndef EW_ICM_H
#define EW_ICM_H

#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

#include <imu_struct.pb.h>
#include <pb_common.h>
#include <pb.h>
#include <pb_encode.h>
#include "ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU

#define SERIAL_PORT Serial

#define SPI_PORT SPI // Your desired SPI port.       Used only when "USE_SPI" is defined
#define CS_PIN 0     // Which pin you connect CS to. Used only when "USE_SPI" is defined


static ICM_20948_SPI myICM; // If using SPI create an ICM_20948_SPI object

void debugPrint(const char* message);

void ICMinit();

bool ICMupdate(ImuData* imuData);

void printFormattedFloat(float val, uint8_t leading, uint8_t decimals);

void printScaledAGMT(ICM_20948_SPI *sensor);

void calculateSampleRate();

#endif