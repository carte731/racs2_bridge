/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: RACS2Brdige_std_msgs.proto */

#ifndef PROTOBUF_C_RACS2Brdige_5fstd_5fmsgs_2eproto__INCLUDED
#define PROTOBUF_C_RACS2Brdige_5fstd_5fmsgs_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _RACS2BridgeStdMsgs RACS2BridgeStdMsgs;


/* --- enums --- */


/* --- messages --- */

struct  _RACS2BridgeStdMsgs
{
  ProtobufCMessage base;
  /*
   * -- std_msgs/Bool ---------------------------
   */
  protobuf_c_boolean has_bool_data;
  protobuf_c_boolean bool_data;
  /*
   * -- std_msgs/Float32 ---------------------------
   */
  protobuf_c_boolean has_float_data;
  float float_data;
  /*
   * -- std_msgs/Float64 ---------------------------
   */
  protobuf_c_boolean has_double_data;
  double double_data;
  /*
   * -- std_msgs/Int32 ---------------------------
   */
  protobuf_c_boolean has_int32_data;
  int32_t int32_data;
  /*
   * -- std_msgs/Int64 ---------------------------
   */
  protobuf_c_boolean has_int64_data;
  int64_t int64_data;
  /*
   * -- std_msgs/String ---------------------------
   */
  char *string_data;
  /*
   * -- std_msgs/Uint32 ---------------------------
   */
  protobuf_c_boolean has_uint32_data;
  uint32_t uint32_data;
  /*
   * -- std_msgs/Uint64 ---------------------------
   */
  protobuf_c_boolean has_uint64_data;
  uint64_t uint64_data;
  /*
   * -- std_msgs/Float32MultiArray ---------------------------
   */
  size_t n_float32_array_data;
  float *float32_array_data;
  /*
   * -- std_msgs/Float64MultiArray ---------------------------
   */
  size_t n_float64_array_data;
  double *float64_array_data;
  /*
   * -- std_msgs/Int32MultiArray ---------------------------
   */
  size_t n_int32_array_data;
  int32_t *int32_array_data;
  /*
   * -- std_msgs/Int64MultiArray ---------------------------
   */
  size_t n_int64_array_data;
  int64_t *int64_array_data;
  /*
   * -- std_msgs/Uint32MultiArray ---------------------------
   */
  size_t n_uint32_array_data;
  uint32_t *uint32_array_data;
  /*
   * -- std_msgs/Uint64MultiArray ---------------------------
   */
  size_t n_uint64_array_data;
  uint64_t *uint64_array_data;
};
#define RACS2_BRIDGE_STD_MSGS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&racs2_bridge_std_msgs__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0,NULL, 0,NULL, 0,NULL, 0,NULL, 0,NULL, 0,NULL }


/* RACS2BridgeStdMsgs methods */
void   racs2_bridge_std_msgs__init
                     (RACS2BridgeStdMsgs         *message);
size_t racs2_bridge_std_msgs__get_packed_size
                     (const RACS2BridgeStdMsgs   *message);
size_t racs2_bridge_std_msgs__pack
                     (const RACS2BridgeStdMsgs   *message,
                      uint8_t             *out);
size_t racs2_bridge_std_msgs__pack_to_buffer
                     (const RACS2BridgeStdMsgs   *message,
                      ProtobufCBuffer     *buffer);
RACS2BridgeStdMsgs *
       racs2_bridge_std_msgs__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   racs2_bridge_std_msgs__free_unpacked
                     (RACS2BridgeStdMsgs *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*RACS2BridgeStdMsgs_Closure)
                 (const RACS2BridgeStdMsgs *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor racs2_bridge_std_msgs__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_RACS2Brdige_5fstd_5fmsgs_2eproto__INCLUDED */
