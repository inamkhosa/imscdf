/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_SIP_Method_H_
#define	_SIP_Method_H_


#include <asn_application.h>

/* Including external dependencies */
#include <UniversalString.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SIP-Method */
typedef UniversalString_t	 SIP_Method_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SIP_Method;
asn_struct_free_f SIP_Method_free;
asn_struct_print_f SIP_Method_print;
asn_constr_check_f SIP_Method_constraint;
ber_type_decoder_f SIP_Method_decode_ber;
der_type_encoder_f SIP_Method_encode_der;
xer_type_decoder_f SIP_Method_decode_xer;
xer_type_encoder_f SIP_Method_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _SIP_Method_H_ */