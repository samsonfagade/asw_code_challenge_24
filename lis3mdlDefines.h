#ifndef LIS3MDL_DEFINES_H
#define LIS3MDL_DEFINES_H

#include <stdbool.h>

#define GET_BIT(data, mask, pos) ((data & mask) >> pos)
#define SET_BIT(data, mask, pos) ((data << pos) && mask)
#define COMBINE_LOW_HIGH_BYTE(LOW, HIGH) (LOW | (HIGH << 8))

// Get the full-scale configuration
#define I2CADDR_DEFAULT (0x1C)
#define REG_CTRL_REG2 0x21                  ///< Register address for control 2
#define REG_CTRL_REG2_BIT_MASK_FS1_FS0 0x60 ///< FS0 BIT 5
#define REG_CTRL_REG2_BIT_POS_FS1_FS0 5     ///< FS1 BIT 6

typedef enum
{
    RANGE_4_GAUSS = 0b00,  ///< +/- 4g (default value)
    RANGE_8_GAUSS = 0b01,  ///< +/- 8g
    RANGE_12_GAUSS = 0b10, ///< +/- 12g
    RANGE_16_GAUSS = 0b11, ///< +/- 16g
} lis3mdl_range_t;

typedef enum
{
    LIS3MDL_DATARATE_0_625_HZ = 0b0000, ///<  0.625 Hz
    LIS3MDL_DATARATE_1_25_HZ = 0b0010,  ///<  1.25 Hz
    LIS3MDL_DATARATE_2_5_HZ = 0b0100,   ///<  2.5 Hz
    LIS3MDL_DATARATE_5_HZ = 0b0110,     ///<  5 Hz
    LIS3MDL_DATARATE_10_HZ = 0b1000,    ///<  10 Hz
    LIS3MDL_DATARATE_20_HZ = 0b1010,    ///<  20 Hz
    LIS3MDL_DATARATE_40_HZ = 0b1100,    ///<  40 Hz
    LIS3MDL_DATARATE_80_HZ = 0b1110,    ///<  80 Hz
    LIS3MDL_DATARATE_155_HZ = 0b0001,   ///<  155 Hz (FAST_ODR + UHP)
    LIS3MDL_DATARATE_300_HZ = 0b0011,   ///<  300 Hz (FAST_ODR + HP)
    LIS3MDL_DATARATE_560_HZ = 0b0101,   ///<  560 Hz (FAST_ODR + MP)
    LIS3MDL_DATARATE_1000_HZ = 0b0111,  ///<  1000 Hz (FAST_ODR + LP)
} lis3mdl_dataRate_t;

/** The magnetometer performance mode */
typedef enum
{
    LIS3MDL_LOWPOWERMODE = 0b00,  ///< Low power mode
    LIS3MDL_MEDIUMMODE = 0b01,    ///< Medium performance mode
    LIS3MDL_HIGHMODE = 0b10,      ///< High performance mode
    LIS3MDL_ULTRAHIGHMODE = 0b11, ///< Ultra-high performance mode
} lis3mdl_performancemode_t;

#define REG_CTRL_REG1 0x20 ///< Register address for control 1
#define REG_CTRL_REG1_BIT_MASK_OM1_OM0 0x60
#define REG_CTRL_REG1_BIT_POS_OM1_OM0 5
#define REG_CTRL_REG1_BIT_MASK_D02_DO1_DO0_FAST_ODR 0x1E
#define REG_CTRL_REG1_BIT_POS_FAST_ODR 1

#define REG_CTRL_REG4 0x23 ///< Register address for control 4
#define REG_CTRL_REG4_BIT_MASK_OMZ1_OMZ0 0x0C
#define REG_CTRL_REG4_BIT_POS_OMZ1_OMZ0 2

#define LIS3MDL_REG_INT_CFG 0x30   ///< Interrupt configuration register
#define LIS3MDL_REG_INT_THS_L 0x32 ///< Low byte of the irq threshold

typedef struct SInterruptConfig
{
    bool enableX;
    bool enableY;
    bool enableZ;
    bool polarity;
    bool latch;
    bool enableInt;
} SInterruptConfig;

#define LIS3MDL_REG_OUT_X_L 0x28 ///< Register address for X axis lower byte

typedef struct SReadOutGauss
{
    float x_gauss, ///< The last read X mag in 'gauss'
        y_gauss,   ///< The last read Y mag in 'gauss'
        z_gauss;   ///< The last read Z mag in 'gauss'
} SReadOutGauss;

#endif // LIS3MDL_DEFINES_H
