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
 *  An example of an internal (private) header file for SAMPLE Lib
 */
#ifndef BRIDGE_LIB_INTERNAL_H
#define BRIDGE_LIB_INTERNAL_H

/* Include all external/public definitions */
#include "bridge_lib.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/

#define BRIDGE_LIB_BUFFER_SIZE              16
#define RACS2_BRIDGE_MID                    0x1EFE
#define RACS2_USER_MSG_LNGTH                sizeof(racs2_user_msg_t)
#define RACS2_BRIDGE_USER_LISTENER_LNGTH    sizeof(racs2_user_msg_t)

/*************************************************************************
** Internal Data Structures
*************************************************************************/
extern char BRIDGE_LIB_Buffer[BRIDGE_LIB_BUFFER_SIZE];

/*************************************************************************
** Function Declarations
*************************************************************************/

/**
 * Library initialization routine/entry point
 */
int32 BRIDGE_LIB_Init(void);

#endif
