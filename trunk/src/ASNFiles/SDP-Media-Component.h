/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_SDP_Media_Component_H_
#define	_SDP_Media_Component_H_


#include <asn_application.h>

/* Including external dependencies */
#include <UniversalString.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SDP_Media_Description;
struct AccessCorrelationID;

/* SDP-Media-Component */
typedef struct SDP_Media_Component {
	UniversalString_t	*sDP_Media_Name	/* OPTIONAL */;
	struct SDP_Media_Description	*sDP_Media_Descriptions	/* OPTIONAL */;
	struct AccessCorrelationID	*accessCorrelationID	/* OPTIONAL */;
	UniversalString_t	*authorized_QoS	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SDP_Media_Component_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SDP_Media_Component;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SDP-Media-Description.h"
#include "AccessCorrelationID.h"

#endif	/* _SDP_Media_Component_H_ */