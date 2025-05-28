/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <time.h>

#include "fsl_irtc.h"
#include "rtc_helper.h"

int Java_com_nxp_rtc_RtcNatives_getEpoch()
{
    struct tm my_tm = {0};

    irtc_config_t irtcConfig;
    irtc_datetime_t datetime;

    IRTC_GetDefaultConfig(&irtcConfig);
    if (IRTC_Init(SOC_RTC, &irtcConfig) == kStatus_Fail)
    {
        /* RTC not available. Return 2024-01-01 00:00:00 as default. */
        return 1704067200;
    }

    IRTC_GetDatetime(SOC_RTC, &datetime);

    my_tm.tm_year  = datetime.year - 1900;
    my_tm.tm_mon   = datetime.month - 1;
    my_tm.tm_mday  = datetime.day;
    my_tm.tm_hour  = datetime.hour;
    my_tm.tm_min   = datetime.minute;
    my_tm.tm_sec   = datetime.second;
    my_tm.tm_isdst = -1;

    return (int)mktime(&my_tm);
}
