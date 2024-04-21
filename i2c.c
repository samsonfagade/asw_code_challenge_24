#include "i2c.h"

#include <stdint.h>
#include <stdio.h>

status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "read [%d] bytes from bus [%d] for register [%d]\n",
        length,
        bus_address,
        register_address);

    /* Setting the output to some arbitrary value */
    for (size_t i = 0; i < length; ++i)
    {
        buffer[i] = 0xff;
    }

    return STATUS_OK;
}

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "write [%d] bytes to bus [%d] for register [%d]\n\t",
        length,
        bus_address,
        register_address);

    for (size_t i = 0; i < length; ++i)
    {
        printf("%p", buffer);

    }
    printf("\n");
    return STATUS_OK;
}

status_t getFullScaleConfig(lis3mdl_range_t *pFullScaleRange)
{
    uint8_t data;
    status_t return_v = i2c_read(I2CADDR_DEFAULT, REG_CTRL_REG2, 1, &data);

    *pFullScaleRange = (lis3mdl_range_t)GET_BIT(data, REG_CTRL_REG2_BIT_MASK_FS1_FS0, REG_CTRL_REG2_BIT_POS_FS1_FS0);

    return return_v;
}

static status_t setPerformanceMode(lis3mdl_performancemode_t const mode)
{
    //  X and Y axes operating mode selection
    uint8_t data = SET_BIT(mode, REG_CTRL_REG1_BIT_MASK_OM1_OM0, REG_CTRL_REG1_BIT_POS_OM1_OM0);
    status_t return_v = i2c_write(I2CADDR_DEFAULT, REG_CTRL_REG1, 1, &data);

    // Z-axis operating mode selection
    data = SET_BIT(mode, REG_CTRL_REG4_BIT_MASK_OMZ1_OMZ0, REG_CTRL_REG4_BIT_POS_OMZ1_OMZ0);
    return_v &= i2c_write(I2CADDR_DEFAULT, REG_CTRL_REG4, 1, &data);

    return return_v;
}

status_t setDataRate(lis3mdl_dataRate_t const dataRate)
{
    // Before writing data rate set the performace mode first
    lis3mdl_performancemode_t peformanceMode;
    switch (dataRate)
    {
    case LIS3MDL_DATARATE_155_HZ:
        // set OP to UHP
        peformanceMode = LIS3MDL_ULTRAHIGHMODE;
        break;
    case LIS3MDL_DATARATE_300_HZ:
        // set OP to HP
        peformanceMode = LIS3MDL_HIGHMODE;
        break;

    case LIS3MDL_DATARATE_560_HZ:
        // set OP to MP
        peformanceMode = LIS3MDL_MEDIUMMODE;
        break;

    case LIS3MDL_DATARATE_1000_HZ:
        // set OP to LP
        peformanceMode = LIS3MDL_LOWPOWERMODE;
        break;
    default:
        break;
    }
    status_t return_v = setPerformanceMode(peformanceMode);
    uint8_t data = SET_BIT(dataRate, REG_CTRL_REG1_BIT_MASK_D02_DO1_DO0_FAST_ODR, REG_CTRL_REG1_BIT_POS_FAST_ODR);
    return_v &= i2c_write(I2CADDR_DEFAULT, REG_CTRL_REG1, 1, &data);

    return return_v;
}

lis3mdl_dataRate_t getDataRate(void)
{
    uint8_t data;
    status_t return_v = i2c_read(I2CADDR_DEFAULT, REG_CTRL_REG1, 1, &data);
    data = GET_BIT(data, REG_CTRL_REG1_BIT_MASK_D02_DO1_DO0_FAST_ODR, REG_CTRL_REG1_BIT_POS_FAST_ODR);

    return (lis3mdl_dataRate_t)data;
}

static status_t setIntThreshold(uint16_t value)
{
    value &= 0x7FFF; // high bit must be 0!
    status_t return_v = i2c_write(I2CADDR_DEFAULT, LIS3MDL_REG_INT_THS_L, 2, (uint8_t *)&value);
    return return_v;
}

static uint8_t initInterruptConfigParam(SInterruptConfig const *pInterruptConfig)
{
    uint8_t value = 0x08; // set default bits, see table 36
    value |= pInterruptConfig->enableX << 7;
    value |= pInterruptConfig->enableY << 6;
    value |= pInterruptConfig->enableZ << 5;
    value |= pInterruptConfig->polarity << 2;
    value |= pInterruptConfig->latch << 1;
    value |= pInterruptConfig->enableInt;
    return value;
}

status_t configureInterrupt(uint16_t const thresholdValue, SInterruptConfig const *pInterruptConfig)
{
    status_t return_v = setIntThreshold(thresholdValue);
    uint8_t configValue = initInterruptConfigParam(pInterruptConfig);
    return_v &= i2c_write(I2CADDR_DEFAULT, LIS3MDL_REG_INT_CFG, 1, &configValue);
    return return_v;
}

status_t read(SReadOutGauss *pReadout)
{
    uint8_t buffer[6];
    status_t return_v = i2c_read(I2CADDR_DEFAULT, LIS3MDL_REG_OUT_X_L, 6, (uint8_t *)&buffer);
    uint16_t x = COMBINE_LOW_HIGH_BYTE(buffer[0], buffer[1]),
             y = COMBINE_LOW_HIGH_BYTE(buffer[2], buffer[3]),
             z = COMBINE_LOW_HIGH_BYTE(buffer[4], buffer[5]);

    lis3mdl_range_t fullScaleRange;
    status_t return_v2 = getFullScaleConfig(&fullScaleRange);
    status_t return_ =  return_v2 |  return_v;
    float scale = 1; // LSB per gauss
    switch (fullScaleRange)
    {
    case RANGE_16_GAUSS:
        scale = 1711;
        break;
    case RANGE_12_GAUSS:
        scale = 2281;
        break;
    case RANGE_8_GAUSS:
        scale = 3421;
        break;
    case RANGE_4_GAUSS:
        scale = 6842;
        break;
    }
    
    pReadout->x_gauss = (float)x / scale;
    pReadout->y_gauss = (float)y / scale;
    pReadout->z_gauss = (float)z / scale;

    return return_v;
}

int main()
{
   return 0;
}