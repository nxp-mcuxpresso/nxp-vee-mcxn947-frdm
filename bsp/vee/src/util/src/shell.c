/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "fsl_component_serial_manager.h"
#include "fsl_irtc.h"
#include "fsl_shell.h"

#include "rtc_helper.h"
#include "shell.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shell_settime(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shell_gettime(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
SHELL_COMMAND_DEFINE(settime,
        "\r\n\"settime time date\"\r\n Usage:\r\n    time: hh:mm:ss\r\n    date: dd-mm-yyyy\r\n",
        shell_settime,
        2);
SHELL_COMMAND_DEFINE(gettime,
        "\r\n\"gettime\"\r\n",
        shell_gettime,
        0);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

extern serial_handle_t g_serialHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void set_irtc_time(const struct tm *my_tm)
{
    irtc_config_t irtcConfig;
    irtc_datetime_t datetime;

    datetime.year   = my_tm->tm_year;
    datetime.month  = my_tm->tm_mon;
    datetime.day    = my_tm->tm_mday;
    datetime.hour   = my_tm->tm_hour;
    datetime.minute = my_tm->tm_min;
    datetime.second = my_tm->tm_sec;
    datetime.weekDay = 2;

    IRTC_GetDefaultConfig(&irtcConfig);
    if (IRTC_Init(SOC_RTC, &irtcConfig) == kStatus_Fail)
    {
        SHELL_Printf(s_shellHandle, "Error storing time to RTC\r\n");
        return;
    }

    IRTC_SetDatetime(SOC_RTC, &datetime);
}

static void get_irtc_time(struct tm *my_tm)
{
    irtc_config_t irtcConfig;
    irtc_datetime_t datetime;

    IRTC_GetDefaultConfig(&irtcConfig);
    if (IRTC_Init(SOC_RTC, &irtcConfig) == kStatus_Fail)
    {
        return;
    }

    IRTC_GetDatetime(SOC_RTC, &datetime);

    my_tm->tm_year  = datetime.year;
    my_tm->tm_mon   = datetime.month;
    my_tm->tm_mday  = datetime.day;
    my_tm->tm_hour  = datetime.hour;
    my_tm->tm_min   = datetime.minute;
    my_tm->tm_sec   = datetime.second;
}

static shell_status_t shell_settime(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    struct tm my_tm;
    char *time = argv[1];
    char *date = argv[2];
    int n;

    n = sscanf(time, "%d:%d:%d", &my_tm.tm_hour, &my_tm.tm_min, &my_tm.tm_sec);
    if (n != 3) {
        SHELL_Printf(s_shellHandle, "Error parsing time\r\n");
        return kStatus_SHELL_Error;
    }

    n = sscanf(date, "%d-%d-%d", &my_tm.tm_mday, &my_tm.tm_mon, &my_tm.tm_year);
    if (n != 3) {
        SHELL_Printf(s_shellHandle, "Error parsing date\r\n");
        return kStatus_SHELL_Error;
    }

    SHELL_Printf(s_shellHandle, " hour:   %d\r\n", my_tm.tm_hour);
    SHELL_Printf(s_shellHandle, " minute: %d\r\n", my_tm.tm_min);
    SHELL_Printf(s_shellHandle, " second: %d\r\n", my_tm.tm_sec);
    SHELL_Printf(s_shellHandle, " year:   %d\r\n", my_tm.tm_year);
    SHELL_Printf(s_shellHandle, " month:  %d\r\n", my_tm.tm_mon);
    SHELL_Printf(s_shellHandle, " day:    %d\r\n", my_tm.tm_mday);

    set_irtc_time(&my_tm);

    return kStatus_SHELL_Success;
}

static shell_status_t shell_gettime(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    time_t epoch;
    struct tm my_tm;

    get_irtc_time(&my_tm);

    SHELL_Printf(s_shellHandle, "read from RTC:  hour:   %d\r\n", my_tm.tm_hour);
    SHELL_Printf(s_shellHandle, "read from RTC:  minute: %d\r\n", my_tm.tm_min);
    SHELL_Printf(s_shellHandle, "read from RTC:  second: %d\r\n", my_tm.tm_sec);
    SHELL_Printf(s_shellHandle, "read from RTC:  year:   %d\r\n", my_tm.tm_year);
    SHELL_Printf(s_shellHandle, "read from RTC:  month:  %d\r\n", my_tm.tm_mon);
    SHELL_Printf(s_shellHandle, "read from RTC:  day:    %d\r\n", my_tm.tm_mday);

    my_tm.tm_year -= 1900;
    my_tm.tm_mon  -= 1;
    my_tm.tm_isdst = -1;

    epoch = mktime(&my_tm);

    SHELL_Printf(s_shellHandle, "read from RTC: epoch: %d\r\n", epoch);

    return kStatus_SHELL_Success;
}

int shell_init(void)
{
    int ret = 0;

    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];

    SHELL_Init(s_shellHandle, g_serialHandle, "NXP PA > ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(settime));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(gettime));

    return ret;
}
