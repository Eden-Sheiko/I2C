/**
 * \file           i2c_log.h
 * \brief          Simple logging macros for stdout/stderr output
 * \author         Eden Sheiko
 * \var            0.9.0
 */

#ifndef I2C_LOG_H
#define I2C_LOG_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \brief           Log info-level message
     * \param[in]       fmt: Format string (printf-style)
     * \param[in]       ...: Additional arguments
     */
#define LOG_INFO(fmt, ...)    fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__)

    /**
     * \brief           Log warning-level message
     * \param[in]       fmt: Format string (printf-style)
     * \param[in]       ...: Additional arguments
     */
#define LOG_WARN(fmt, ...)    fprintf(stdout, "[WARN] " fmt "\n", ##__VA_ARGS__)

    /**
     * \brief           Log error-level message
     * \param[in]       fmt: Format string (printf-style)
     * \param[in]       ...: Additional arguments
     */
#define LOG_ERROR(fmt, ...)   fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* I2C_LOG_H */
