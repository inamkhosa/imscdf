/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "GenericChargingDataTypes"
 * 	found in "generic_charging_DataTypes.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_SubscriptionIDType_H_
#define	_SubscriptionIDType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SubscriptionIDType {
	SubscriptionIDType_endUserE164	= 0,
	SubscriptionIDType_endUserIMSI	= 1,
	SubscriptionIDType_endUserSIPURI	= 2,
	SubscriptionIDType_endUserNAI	= 3,
	SubscriptionIDType_endUserPRIVATE	= 4
} e_SubscriptionIDType;

/* SubscriptionIDType */
typedef long	 SubscriptionIDType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SubscriptionIDType;
asn_struct_free_f SubscriptionIDType_free;
asn_struct_print_f SubscriptionIDType_print;
asn_constr_check_f SubscriptionIDType_constraint;
ber_type_decoder_f SubscriptionIDType_decode_ber;
der_type_encoder_f SubscriptionIDType_encode_der;
xer_type_decoder_f SubscriptionIDType_decode_xer;
xer_type_encoder_f SubscriptionIDType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _SubscriptionIDType_H_ */
