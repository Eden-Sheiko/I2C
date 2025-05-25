/**
* \file           log.h
 * \brief          Simple logging macros for stderr output
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \brief           Log info-level message
     * \param[in]       fmt: Format string (printf-style)
     * \param[in]       ...: Additional arguments
     */
#define LOG_INFO(fmt, ...)    fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)

    /**
     * \brief           Log warning-level message
     * \param[in]       fmt: Format string (printf-style)
     * \param[in]       ...: Additional arguments
     */
#define LOG_WARN(fmt, ...)    fprintf(stderr, "[WARN] " fmt "\n", ##__VA_ARGS__)

    /**
     * \brief           Log error-level message
     * \param[in]       fmt: Format string (printf-style)
     * \param[in]       ...: Additional arguments
     */
#define LOG_ERROR(fmt, ...)   fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* LOG_H */
