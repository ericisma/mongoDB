/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     envoy/config/core/v3/proxy_protocol.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#ifndef ENVOY_CONFIG_CORE_V3_PROXY_PROTOCOL_PROTO_UPBDEFS_H_
#define ENVOY_CONFIG_CORE_V3_PROXY_PROTOCOL_PROTO_UPBDEFS_H_

#include "upb/reflection/def.h"
#include "upb/reflection/def_pool_internal.h"
#include "upb/port/def.inc"
#ifdef __cplusplus
extern "C" {
#endif

#include "upb/reflection/def.h"

#include "upb/port/def.inc"

extern _upb_DefPool_Init envoy_config_core_v3_proxy_protocol_proto_upbdefinit;

UPB_INLINE const upb_MessageDef *envoy_config_core_v3_ProxyProtocolPassThroughTLVs_getmsgdef(upb_DefPool *s) {
  _upb_DefPool_LoadDefInit(s, &envoy_config_core_v3_proxy_protocol_proto_upbdefinit);
  return upb_DefPool_FindMessageByName(s, "envoy.config.core.v3.ProxyProtocolPassThroughTLVs");
}

UPB_INLINE const upb_MessageDef *envoy_config_core_v3_ProxyProtocolConfig_getmsgdef(upb_DefPool *s) {
  _upb_DefPool_LoadDefInit(s, &envoy_config_core_v3_proxy_protocol_proto_upbdefinit);
  return upb_DefPool_FindMessageByName(s, "envoy.config.core.v3.ProxyProtocolConfig");
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#include "upb/port/undef.inc"

#endif  /* ENVOY_CONFIG_CORE_V3_PROXY_PROTOCOL_PROTO_UPBDEFS_H_ */
