/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_IMS_Charging_Identifier_H_
#define	_IMS_Charging_Identifier_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IMS-Charging-Identifier */
typedef OCTET_STRING_t	 IMS_Charging_Identifier_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IMS_Charging_Identifier;
asn_struct_free_f IMS_Charging_Identifier_free;
asn_struct_print_f IMS_Charging_Identifier_print;
asn_constr_check_f IMS_Charging_Identifier_constraint;
ber_type_decoder_f IMS_Charging_Identifier_decode_ber;
der_type_encoder_f IMS_Charging_Identifier_encode_der;
xer_type_decoder_f IMS_Charging_Identifier_decode_xer;
xer_type_encoder_f IMS_Charging_Identifier_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _IMS_Charging_Identifier_H_ */
