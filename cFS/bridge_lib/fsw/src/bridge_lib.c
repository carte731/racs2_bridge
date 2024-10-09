/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *   Sample CFS library
 */

/*************************************************************************
** Includes
*************************************************************************/
#include "bridge_lib_version.h"
#include "bridge_lib_internal.h"

#include "cfe_config.h"

/* for "strncpy()" */
#include <string.h>

/*************************************************************************
** Private Data Structures
*************************************************************************/
char BRIDGE_LIB_Buffer[BRIDGE_LIB_BUFFER_SIZE];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BRIDGE_LIB_Init(void)
{
    char VersionString[BRIDGE_LIB_CFG_MAX_VERSION_STR_LEN];

    /*
     * Call a C library function, like strcpy(), and test its result.
     *
     * This is primary for a unit test example, to have more than
     * one code path to exercise.
     *
     * The specification for strncpy() indicates that it should return
     * the pointer to the destination buffer, so it should be impossible
     * for this to ever fail when linked with a compliant C library.
     */
    if (strncpy(BRIDGE_LIB_Buffer, "BRIDGE INIT DATA", sizeof(BRIDGE_LIB_Buffer) - 1) != BRIDGE_LIB_Buffer)
    {
        return CFE_STATUS_NOT_IMPLEMENTED;
    }

    /* ensure termination */
    BRIDGE_LIB_Buffer[sizeof(BRIDGE_LIB_Buffer) - 1] = 0;

    CFE_Config_GetVersionString(VersionString, BRIDGE_LIB_CFG_MAX_VERSION_STR_LEN, "Bridge Lib",
        BRIDGE_LIB_VERSION, BRIDGE_LIB_BUILD_CODENAME, BRIDGE_LIB_LAST_OFFICIAL);

    OS_printf("SAMPLE Lib Initialized.%s\n", VersionString);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Sample Lib function                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BRIDGE_LIB_Function(void)
{
    OS_printf("BRIDGE_LIB_Function called, buffer=\'%s\'\n", BRIDGE_LIB_Buffer);

    return CFE_SUCCESS;
}

racs2_user_msg_t ROS_Publish(char* topic, char* parameters){

}
