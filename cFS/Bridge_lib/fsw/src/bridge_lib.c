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
** File: bridge_lib.c
**
** Purpose: 
**   bridge CFS library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "bridge_lib.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/


/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 BRIDGE_LibInit(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BRIDGE_LibInit(void)
{
    
    OS_printf ("BRIDGE Lib Initialized.  Version %d.%d.%d.%d",
                BRIDGE_LIB_MAJOR_VERSION,
                BRIDGE_LIB_MINOR_VERSION, 
                BRIDGE_LIB_REVISION, 
                BRIDGE_LIB_MISSION_REV);
                
    return CFE_SUCCESS;
 
}/* End BRIDGE_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* BRIDGE Lib function                                             */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BRIDGE_Function(void) 
{

   return(CFE_SUCCESS);
   
} /* End bridge_Function */


void ROS_Publish(racs2_user_msg_t *RACS2_cFSMsg_outerShell, char* topic, char* topicData){
    
    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *buffer;


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
    RACS2_ProtoBuffMsg_innerShell->string_data = (char *) malloc(len * sizeof(char));

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

    int itr;
    int topicLen = strlen(topic) + 1;
    for(itr = 0; itr < topicLen; itr++){
        RACS2_cFSMsg_outerShell->ros2_topic_name[itr] = topic[itr];
    }

    // Deep-copying the buffer space containing the ProtoBuff message to cFS message data space
    //RACS2_cFSMsg_outerShell.body_data =  (uint8 *) malloc(sizeof(uint8 * len));
    for(itr = 0; itr < len; itr++){
        RACS2_cFSMsg_outerShell->body_data[itr] = buffer[itr];
    }

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg_outerShell->body_data_length = len;

    // CLEAN-UP
    // Freeing up memory from malloc() call to create interShell ProtoBuff message
    free(RACS2_ProtoBuffMsg_innerShell->string_data);
    free(RACS2_ProtoBuffMsg_innerShell);

    // Freeing up memory from buffer used to transfer data to cFS message
    free(buffer);
    
}

RACS2BridgeStdMsgs* protobuff_Pack_Checker(racs2_user_msg_t *RACS2_cFSMsg){

    // Checking if the cFS message is NULL
    if(RACS2_cFSMsg == NULL){
        return(NULL);
    }

    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Checks if cFS message has a protobuff message 
    if(RACS2_cFSMsg->isInitialized != TRUE){
    //if(TRUE){
        // Initializes the bool, allows it to be repacked later on
        RACS2_cFSMsg->isInitialized = TRUE;

        // If not, malloc and create a new message memory space.
        RACS2_ProtoBuffMsg = (RACS2BridgeStdMsgs *) malloc(sizeof(RACS2BridgeStdMsgs));


        // Initializing memory space for ProtoBuff message packing
        racs2_bridge_std_msgs__init(RACS2_ProtoBuffMsg);

    } else {
        // If Proto-Buff data exist, extract it add to it in the outer function.
        RACS2_ProtoBuffMsg = racs2_bridge_std_msgs__unpack(NULL, RACS2_cFSMsg->body_data_length, RACS2_cFSMsg->body_data);

        // Zeroing out previous data in outer cFS message
        RACS2_cFSMsg->body_data_length = 0;

        // Clearing old Proto-Buff data fields in outer cFS message
        memset(RACS2_cFSMsg->ros2_topic_name, '\0', ROS2_TOPIC_NAME_LNGTH);
        memset(RACS2_cFSMsg->body_data, 0, BODY_DATA_MAX_LNGTH);
    }

    return(RACS2_ProtoBuffMsg);

}

void cFS_Msg_Pack(racs2_user_msg_t *RACS2_cFSMsg, uint8_t *Proto_buffer, char* topic){
    // Copies buffer into cFS message

    // Variables
    int itr;
    int len;

    // Deep-copying the input ROS2 topic to the cFS message portion (outer cFS message shell)
    len = strlen(topic) + 1;
    for(itr = 0; itr < len; itr++){
        RACS2_cFSMsg->ros2_topic_name[itr] = topic[itr];
    }

    // Deep-copying the buffer space containing the ProtoBuff message to cFS message data space
    len = RACS2_cFSMsg->body_data_length;
    for(itr = 0; itr < len; itr++){
        RACS2_cFSMsg->body_data[itr] = Proto_buffer[itr];
    }

}

//// !!!!!!!!!!
//// IMPORTANT FREE PROTOBUFF MEMORY FOR EACH FUNCTION !!!
//// !!!!!!!!!!

void pack_PB_Bool(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    protobuf_c_boolean data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    //if(!protobuff_Pack_Checker(&RACS2_cFSMsg, &RACS2_ProtoBuffMsg)){
    //    return(NULL);
    //}
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->has_bool_data = true;

    // Saving the user-defined value to the data field
    RACS2_ProtoBuffMsg->bool_data = data;

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    //free(RACS2_ProtoBuffMsg->bool_data);
    //free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_float(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    float data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->has_float_data = true;

    // Saving the user-defined value to the data field
    RACS2_ProtoBuffMsg->float_data = data;

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    //free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_double(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    double data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->has_double_data = true;

    // Saving the user-defined value to the data field
    RACS2_ProtoBuffMsg->double_data = data;

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    //free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

// TO-DO: FIX 'int32_t' ERROR
void pack_PB_int32(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    int32_t data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->has_int32_data = true;

    // Saving the user-defined value to the data field
    RACS2_ProtoBuffMsg->int32_data = data;

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    //free(RACS2_ProtoBuffMsg->int32_data);
    //free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_int64(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    int64_t data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->has_int64_data = true;

    // Saving the user-defined value to the data field
    RACS2_ProtoBuffMsg->int64_data = data;

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    //free(RACS2_ProtoBuffMsg->int64_data);
    //free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_uint32(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    uint32_t data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->has_uint32_data = true;

    // Saving the user-defined value to the data field
    RACS2_ProtoBuffMsg->uint32_data = data;

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    //free(RACS2_ProtoBuffMsg->uint32_data);
    //free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_uint64(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    uint64_t data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->has_uint64_data = true;

    // Saving the user-defined value to the data field
    RACS2_ProtoBuffMsg->uint64_data = data;

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    //free(RACS2_ProtoBuffMsg->uint64_data);
    //free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_char_Arr(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    char* data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg, &RACS2_ProtoBuffMsg);
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    //if(!protobuff_Pack_Checker(RACS2_cFSMsg, &RACS2_ProtoBuffMsg)){
    //    return(NULL);
    //}

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Plus 1 due to strlen() not counting NULL terminator
    len = strlen(data) + 1;

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg->string_data = (char *) malloc(len * sizeof(char));

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    strncpy(RACS2_ProtoBuffMsg->string_data, data, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    free(RACS2_ProtoBuffMsg->string_data);
    free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_float_Arr(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    float* data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->n_float32_array_data = true;

    // Size of the number array
    len = sizeof(data);

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg->float32_array_data = (float *) malloc(len);

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    memcpy(RACS2_ProtoBuffMsg->float32_array_data, data, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    free(RACS2_ProtoBuffMsg->float32_array_data);
    free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_double_Arr(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    double* data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->n_float64_array_data = true;

    // Size of the number array
    len = sizeof(data);

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg->float64_array_data = (double *) malloc(len);

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    memcpy(RACS2_ProtoBuffMsg->float64_array_data, data, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    free(RACS2_ProtoBuffMsg->float64_array_data);
    free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_int32_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    int32_t* data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->n_int32_array_data = true;

    // Size of the number array
    len = sizeof(data);

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg->int32_array_data = (int32_t *) malloc(len);

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    memcpy(RACS2_ProtoBuffMsg->int32_array_data, data, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    free(RACS2_ProtoBuffMsg->int32_array_data);
    free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_int64_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    int64_t* data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->n_int64_array_data = true;

    // Size of the number array
    len = sizeof(data);

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg->int64_array_data = (int64_t *) malloc(len);

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    memcpy(RACS2_ProtoBuffMsg->int64_array_data, data, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    free(RACS2_ProtoBuffMsg->int64_array_data);
    free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_uint32_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    uint32_t* data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->n_uint32_array_data = true;

    // Size of the number array
    len = sizeof(data);

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg->uint32_array_data = (uint32_t *) malloc(len);

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    memcpy(RACS2_ProtoBuffMsg->uint32_array_data, data, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    free(RACS2_ProtoBuffMsg->uint32_array_data);
    free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void pack_PB_uint64_t_Arr(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    uint64_t* data){

    // VARIABLES
    // Input topicData length
    int len;

    // Data buffer for ProtoBuff message contents
    uint8_t *proto_buffer;

    // Create 'racs2_user_msg_t' object
    //RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg;

    // Call 'protobuff_Pack_Checker()' to see if cFS message has protobuff in it.
    RACS2BridgeStdMsgs *RACS2_ProtoBuffMsg = protobuff_Pack_Checker(RACS2_cFSMsg);

    // PACKING PROTOBUFF MESSAGE
    // Pack message for that data type

    // Setting the has field to true
    RACS2_ProtoBuffMsg->n_uint64_array_data = true;

    // Size of the number array
    len = sizeof(data);

    // Creating space for the internal message in the Protobuff message
    RACS2_ProtoBuffMsg->uint64_array_data = (uint64_t *) malloc(len);

    // Deep-copying the topic data from parameters to the internal ProtoBuff message
    memcpy(RACS2_ProtoBuffMsg->uint64_array_data, data, len);

    // Reassigning 'len' to get the new total length of the packed ProtoBuff message
    len = racs2_bridge_std_msgs__get_packed_size(RACS2_ProtoBuffMsg);

    // Prepping the buffer to copy the ProtoBuff data over to it 
    proto_buffer = malloc(len);

    // Specifying the length of the internal ProtoBuff massage
    RACS2_cFSMsg->body_data_length = len;

    // Copies and packs the internal ProtoBuff message into the buffer space
    racs2_bridge_std_msgs__pack(RACS2_ProtoBuffMsg, proto_buffer);

    // cFS MESSAGE PACKING
    // Call 'cFS_Msg_Pack()' to pack/repack message into cFS message
    cFS_Msg_Pack(RACS2_cFSMsg, proto_buffer, topic);

    // Freeing memory
    free(RACS2_ProtoBuffMsg->uint64_array_data);
    free(RACS2_ProtoBuffMsg);
    free(proto_buffer);

}

void PB_STUB(racs2_user_msg_t *RACS2_cFSMsg, \
                    char* topic, \
                    uint64_t* data){

    OS_printf("\n\nSTUB-TEST\n\n");
    // Does nothing, it's the default function call for the MACROs in the header
    return(NULL);
}

/************************/
/*  End of File Comment */
/************************/
