/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "GenericChargingDataTypes"
 * 	found in "generic_charging_DataTypes.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_NodeAddress_H_
#define	_NodeAddress_H_


#include <asn_application.h>

/* Including external dependencies */
#include <UniversalString.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NodeAddress_PR {
	NodeAddress_PR_NOTHING,	/* No components present */
	NodeAddress_PR_iPAddress,
	NodeAddress_PR_domainName
} NodeAddress_PR;

/* Forward declarations */
struct IPAddress;

/* NodeAddress */
typedef struct NodeAddress {
	NodeAddress_PR present;
	union NodeAddress_u {
		struct IPAddress	*iPAddress;
		UniversalString_t	 domainName;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NodeAddress_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NodeAddress;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "IPAddress.h"

#endif	/* _NodeAddress_H_ */