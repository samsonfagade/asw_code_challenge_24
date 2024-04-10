#ifndef LIS3MDL_H
#define LIS3MDL_H

#include "lis3mdlDefines.h"

/**
 * @brief Gets the currently configured full-scale range of the LIS3MDL sensor.
 *
 * This function retrieves the full-scale range setting of the LIS3MDL magnetometer sensor.
 * The full-scale range determines the maximum magnetic field strength that can be measured
 * before saturation occurs.
 *
 * @param pFullScaleRange A pointer to a lis3mdl_range_t variable where the retrieved full-scale range
 *                        will be stored.
 *
 * @return STATUS_OK on success, or an error code otherwise. Common error codes might
 *         include:
 *         - STATUS_ERROR - If there's a communication error with the sensor.
 *         - STATUS_OK    - If read success.
 */
status_t getFullScaleConfig(lis3mdl_range_t *pFullScaleRange);

/**
 * @brief Sets the data rate of the LIS3MDL sensor.
 *
 * This function sets the data rate of the LIS3MDL accelerometer sensor. The data rate
 * determines the frequency at which the sensor generates new data samples.
 *
 * @param dataRate The desired data rate for the sensor. This should be a value from
 *                  the lis3mdl_dataRate_t enumeration. Refer to the LIS3MDL datasheet
 *                  for specific data rate options and their corresponding values.
 *
 * @return STATUS_OK on success, or an error code otherwise. 
 *         - STATUS_ERROR - If write fail.
 *         - STATUS_OK    - If write success.
 */
status_t setDataRate(lis3mdl_dataRate_t const dataRate);

/**
 * @brief Gets the currently set data rate of the LIS3MDL sensor.
 *
 * This function retrieves the data rate that is currently configured for the LIS3MDL
 * accelerometer sensor. The data rate determines the frequency at which the sensor
 * generates new data samples.
 *
 * @return The currently set data rate of the sensor as a value from the
 *         lis3mdl_dataRate_t enumeration.
 *         - STATUS_ERROR - If there's a communication error with the sensor.
 *         - STATUS_OK    - If write success.
 */
status_t lis3mdl_dataRate_t getDataRate(void);

/**
 * @brief Configures the interrupt settings for the LIS3MDL sensor.
 *
 * This function allows you to configure the LIS3MDL sensor to generate interrupts
 * based on specific conditions. You can define a threshold value and enable interrupts
 * for individual axes (X, Y, Z) based on the provided configuration.
 *
 * @param thresholdValue The threshold value for the interrupt. When any enabled magnetic
 *                        axis measurement exceeds (or falls below, depending on polarity)
 *                        this value, an interrupt will be triggered.
 * 
 * @param pInterruptConfig A pointer to an SInterruptConfig structure that holds the
 *                          interrupt configuration settings. The structure members define:
 *                          - enableX, enableY, enableZ (bool): Enable interrupts for X, Y, and Z axes.
 *                          - polarity (bool): Sets the interrupt trigger polarity (high or low).
 *                          - latch (bool): Determines whether the interrupt signal is latched or non-latched.
 *                          - enableInt (bool): Enables or disables the overall interrupt generation.
 *
 * @return STATUS_OK on success, or an error code otherwise.
 *         - STATUS_ERROR - If invalid arguments are provided (e.g., negative threshold).
 *         - STATUS_OK    - If write success.
 */
status_t configureInterrupt(uint16_t const thresholdValue, SInterruptConfig const *pInterruptConfig);

/**
 * @brief Reads the magnetic field data for a specified axis from the LIS3MDL sensor.
 *
 * This function retrieves the magnetic field data measured along a specific axis
 * (X, Y, or Z) from the LIS3MDL magnetometer sensor. The data is returned in a
 * `SReadOutGauss` structure.
 *
 * @param pReadout A pointer to a `SReadOutGauss` structure where the read data will be stored.
 *
 * @return STATUS_OK on success, or an error code otherwise. 
 *         - STATUS_ERROR - If there's a communication error with the sensor.
 *         - STATUS_OK    - If read success.
 */
status_t read(SReadOutGauss *pReadout);

#endif // LIS3MDL_H
