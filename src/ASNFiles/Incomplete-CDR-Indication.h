/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_Incomplete_CDR_Indication_H_
#define	_Incomplete_CDR_Indication_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include "ACRInterimLost.h"
#include <constr_SET.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */

/*
 * Method of determining the components presence
 */
typedef enum Incomplete_CDR_Indication_PR {
	Incomplete_CDR_Indication_PR_aCRStartLost,	/* Member aCRStartLost is present */
	Incomplete_CDR_Indication_PR_aCRInterimLost,	/* Member aCRInterimLost is present */
	Incomplete_CDR_Indication_PR_aCRStopLost,	/* Member aCRStopLost is present */
} Incomplete_CDR_Indication_PR;

/* Incomplete-CDR-Indication */
typedef struct Incomplete_CDR_Indication {
	BOOLEAN_t	 aCRStartLost;
	ACRInterimLost_t	 aCRInterimLost;
	BOOLEAN_t	 aCRStopLost;
	
	/* Presence bitmask: ASN_SET_ISPRESENT(pIncomplete_CDR_Indication, Incomplete_CDR_Indication_PR_x) */
	unsigned int _presence_map
		[((3+(8*sizeof(unsigned int))-1)/(8*sizeof(unsigned int)))];
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Incomplete_CDR_Indication_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Incomplete_CDR_Indication;

#ifdef __cplusplus
}
#endif

#endif	/* _Incomplete_CDR_Indication_H_ */
