/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_CauseForRecordClosing_H_
#define	_CauseForRecordClosing_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CauseForRecordClosing {
	CauseForRecordClosing_serviceDeliveryEndSuccessfully	= 0,
	CauseForRecordClosing_unSuccessfulServiceDelivery	= 1,
	CauseForRecordClosing_timeLimit	= 3,
	CauseForRecordClosing_serviceChange	= 4,
	CauseForRecordClosing_managementIntervention	= 5
} e_CauseForRecordClosing;

/* CauseForRecordClosing */
typedef long	 CauseForRecordClosing_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CauseForRecordClosing;
asn_struct_free_f CauseForRecordClosing_free;
asn_struct_print_f CauseForRecordClosing_print;
asn_constr_check_f CauseForRecordClosing_constraint;
ber_type_decoder_f CauseForRecordClosing_decode_ber;
der_type_encoder_f CauseForRecordClosing_encode_der;
xer_type_decoder_f CauseForRecordClosing_decode_xer;
xer_type_encoder_f CauseForRecordClosing_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _CauseForRecordClosing_H_ */