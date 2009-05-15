/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "GenericChargingDataTypes"
 * 	found in "generic_charging_DataTypes.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_IPAddress_H_
#define	_IPAddress_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum IPAddress_PR {
	IPAddress_PR_NOTHING,	/* No components present */
	IPAddress_PR_iPBinaryAddress,
	IPAddress_PR_iPTextRepresentedAddress
} IPAddress_PR;

/* Forward declarations */
struct IPBinaryAddress;
struct IPTextRepresentedAddress;

/* IPAddress */
typedef struct IPAddress {
	IPAddress_PR present;
	union IPAddress_u {
		struct IPBinaryAddress	*iPBinaryAddress;
		struct IPTextRepresentedAddress	*iPTextRepresentedAddress;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IPAddress_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IPAddress;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "IPBinaryAddress.h"
#include "IPTextRepresentedAddress.h"

#endif	/* _IPAddress_H_ */