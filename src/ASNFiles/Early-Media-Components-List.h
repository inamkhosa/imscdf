/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_Early_Media_Components_List_H_
#define	_Early_Media_Components_List_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TimeStamp.h"
#include <NULL.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <UniversalString.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SDP_Media_Component;

/* Early-Media-Components-List */
typedef struct Early_Media_Components_List {
	TimeStamp_t	*sDP_Offer_Timestamp	/* OPTIONAL */;
	TimeStamp_t	*sDP_Answer_Timestamp	/* OPTIONAL */;
	struct Early_Media_Components_List__sDP_Media_Components {
		A_SEQUENCE_OF(struct SDP_Media_Component) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *sDP_Media_Components;
	NULL_t	*mediaInitiatorFlag	/* OPTIONAL */;
	struct Early_Media_Components_List__sDP_Session_Description {
		A_SEQUENCE_OF(UniversalString_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *sDP_Session_Description;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Early_Media_Components_List_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Early_Media_Components_List;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SDP-Media-Component.h"

#endif	/* _Early_Media_Components_List_H_ */