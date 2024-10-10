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

//#include "cfe_config.h"

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

racs2_user_msg_t ROS_Publish(char* topic, char* topicData){
    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    //void *buffer;
    uint8 *buffer;
    
    // Creating the message to return to requesting app
    racs2_user_msg_t RACS2_cFSMsg_outerShell;

    // Creating message that will contain Protobuff data for ROS
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg_innerShell;

    // DECLARATIONS
    // Plus 1 due to strlen() not counting NULL terminator
    len = strlen(topicData) + 1;

    // Creating space for protobuff Message
    RACS2_ProtoBuffMsg_innerShell = (RACS2BridgeStdMsgs *) malloc(sizeof(RACS2BridgeStdMsgs));

    // Initializing memory space for ProtoBuff message packing
    racs2_bridge_std_msgs__init(RACS2_ProtoBuffMsg_innerShell);

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg_innerShell->string_data = (char *) malloc(sizeof(char * BODY_DATA_MAX_LNGTH));

    // SETTING UP PROTOBUFF INTERNAL MESSAGE
    // Shallow-copying the input ROS2 topic to the cFS message portion (outer cFS message shell)
    //strcpy(RACS2_cFSMsg_outerShell.ros2_topic_name, topic);

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    strncpy(RACS2_ProtoBuffMsg_innerShell->string_data, topicData, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg_innerShell);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    buffer = malloc(len);

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg_innerShell, buffer);

    // COPYING AND PACKING PROTOBUFF MESSAGE INTO cFS MESSAGE
    // Deep-copying the input ROS2 topic to the cFS message portion (outer cFS message shell)
    //strcpy(RACS2_cFSMsg_outerShell.ros2_topic_name, topic);
    int itr;
    int topicLen = strlen(topic) + 1;
    for(itr = 0; itr < topicLen; itr++){
        RACS2_cFSMsg_outerShell.ros2_topic_name[itr] = buffer[itr];
    }

    // Declaring space to ensure deep-copy of ProtoBuff data to cFS body data
    //RACS2_cFSMsg_outerShell.body_data =  (uint8 *) malloc(sizeof(uint8 * len));
    for(itr = 0; itr < len; itr++){
        RACS2_cFSMsg_outerShell.body_data[itr] = buffer[itr];
    }

    // Copying the buffer space containing the ProtoBuff message to cFS message data space
    //strncpy(RACS2_cFSMsg_outerShell.body_data, buffer, len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg_outerShell.body_data_length = len;

    // CLEAN-UP
    // Freeing up memory from malloc() call to create interShell ProtoBuff message
    free(RACS2_ProtoBuffMsg_innerShell->string_data);
    free(RACS2_ProtoBuffMsg_innerShell);

    // Freeing up memory from buffer used to transfer data to cFS message
    free(buffer);
    
    // Return cFS message to requesting app
    return(RACS2_cFSMsg_outerShell);
}
