/*************************************************************************
**
**      GSC-18128-1, "Core Flight Executive Version 6.7"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
** File: bridge_lib.h
**
** Purpose: 
**   Specification for the bridge library functions.
**
*************************************************************************/
#ifndef _bridge_lib_h_
#define _bridge_lib_h_

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include <stdio.h>
#include <string.h>
#include "racs2_user_msg.h"
#include "bridge_lib_version.h"
#include "RACS2Brdige_std_msgs.pb-c.h"

/************************************************************************
** Type Definitions
*************************************************************************/

#define ros_msg_pack(RACS2_cFSMsg, topic, data) _Generic((data), \
    bool : pack_PB_Bool, \
    float : pack_PB_float, \
    double : pack_PB_double, \
    int32_t : pack_PB_int32, \
    int64_t : pack_PB_int64, \
    char* : pack_PB_char_Arr, \
    uint32_t : pack_PB_uint32, \
    uint64_t : pack_PB_uint64, \
    float* : pack_PB_float_Arr, \
    double* : pack_PB_double_Arr, \
    int32_t* : pack_PB_int32_t_Arr, \
    int64_t* : pack_PB_int64_t_Arr, \
    uint32_t* : pack_PB_uint32_t_Arr, \
    uint64_t* : pack_PB_uint64_t_Arr, \
    default: PB_STUB)(RACS2_cFSMsg, topic, data) \


/*************************************************************************
** Exported Functions
*************************************************************************/
/************************************************************************/
/** \brief bridge Lib Function 
**  
**  \par Description
**        This is a bridge function
**
**  \par Assumptions, External Events, and Notes:
**        None
**       
**  \returns
**  \retstmt Returns #CFE_SUCCESS \endcode
**  \endreturns
** 
*************************************************************************/
int32 BRIDGE_Function( void ); 

void ROS_Publish(racs2_user_msg_t *RACS2_cFSMsg_outerShell, char* topic, char* topicData);

RACS2BridgeStdMsgs* protobuff_Pack_Checker(racs2_user_msg_t *RACS2_cFSMsg);
void cFS_Msg_Pack(racs2_user_msg_t *RACS2_cFSMsg, uint8_t *Proto_buffer, char* topic);

void pack_PB_Bool(racs2_user_msg_t *RACS2_cFSMsg, char* topic, protobuf_c_boolean data);
void pack_PB_float(racs2_user_msg_t *RACS2_cFSMsg, char* topic, float data);
void pack_PB_double(racs2_user_msg_t *RACS2_cFSMsg, char* topic, double data);
void pack_PB_int32(racs2_user_msg_t *RACS2_cFSMsg, char* topic, int32_t data);
void pack_PB_int64(racs2_user_msg_t *RACS2_cFSMsg, char* topic, int64_t data);
void pack_PB_uint32(racs2_user_msg_t *RACS2_cFSMsg, char* topic,  uint32_t data);
void pack_PB_uint64(racs2_user_msg_t *RACS2_cFSMsg, char* topic, uint64_t data);
void pack_PB_char_Arr(racs2_user_msg_t *RACS2_cFSMsg, char* topic, char* data);
void pack_PB_float_Arr(racs2_user_msg_t *RACS2_cFSMsg, char* topic, float* data);
void pack_PB_double_Arr(racs2_user_msg_t *RACS2_cFSMsg, char* topic, double* data);
void pack_PB_int32_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, char* topic,  int32_t* data);
void pack_PB_int64_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, char* topic, int64_t* data);
void pack_PB_uint32_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, char* topic, uint32_t* data);
void pack_PB_uint64_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, char* topic, uint64_t* data);
void PB_STUB(racs2_user_msg_t *RACS2_cFSMsg, char* topic, uint64_t* data);
#endif /* _bridge_lib_h_ */

/************************/
/*  End of File Comment */
/************************/
