/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_Role_of_Node_H_
#define	_Role_of_Node_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Role_of_Node {
	Role_of_Node_originating	= 0,
	Role_of_Node_terminating	= 1,
	Role_of_Node_proxy	= 2,
	Role_of_Node_b2bua	= 3
} e_Role_of_Node;

/* Role-of-Node */
typedef long	 Role_of_Node_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Role_of_Node;
asn_struct_free_f Role_of_Node_free;
asn_struct_print_f Role_of_Node_print;
asn_constr_check_f Role_of_Node_constraint;
ber_type_decoder_f Role_of_Node_decode_ber;
der_type_encoder_f Role_of_Node_encode_der;
xer_type_decoder_f Role_of_Node_decode_xer;
xer_type_encoder_f Role_of_Node_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _Role_of_Node_H_ */