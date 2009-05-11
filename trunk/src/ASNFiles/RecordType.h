/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "GenericChargingDataTypes"
 * 	found in "generic_charging_DataTypes.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_RecordType_H_
#define	_RecordType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RecordType {
	RecordType_moCallRecord	= 0,
	RecordType_mtCallRecord	= 1,
	RecordType_roamingRecord	= 2,
	RecordType_incGatewayRecord	= 3,
	RecordType_outGatewayRecord	= 4,
	RecordType_transitCallRecord	= 5,
	RecordType_moSMSRecord	= 6,
	RecordType_mtSMSRecord	= 7,
	RecordType_moSMSIWRecord	= 8,
	RecordType_mtSMSGWRecord	= 9,
	RecordType_ssActionRecord	= 10,
	RecordType_hlrIntRecord	= 11,
	RecordType_locUpdateHLRRecord	= 12,
	RecordType_locUpdateVLRRecord	= 13,
	RecordType_commonEquipRecord	= 14,
	RecordType_moTraceRecord	= 15,
	RecordType_mtTraceRecord	= 16,
	RecordType_termCAMELRecord	= 17,
	RecordType_sgsnPDPRecord	= 18,
	RecordType_ggsnPDPRecord	= 19,
	RecordType_sgsnMMRecord	= 20,
	RecordType_sgsnSMORecord	= 21,
	RecordType_sgsnSMTRecord	= 22,
	RecordType_mtLCSRecord	= 23,
	RecordType_moLCSRecord	= 24,
	RecordType_niLCSRecord	= 25,
	RecordType_sgsnMtLCSRecord	= 26,
	RecordType_sgsnMoLCSRecord	= 27,
	RecordType_sgsnNiLCSRecord	= 28,
	RecordType_mMO1SRecord	= 30,
	RecordType_mMO4FRqRecord	= 31,
	RecordType_mMO4FRsRecord	= 32,
	RecordType_mMO4DRecord	= 33,
	RecordType_mMO1DRecord	= 34,
	RecordType_mMO4RRecord	= 35,
	RecordType_mMO1RRecord	= 36,
	RecordType_mMOMDRecord	= 37,
	RecordType_mMR4FRecord	= 38,
	RecordType_mMR1NRqRecord	= 39,
	RecordType_mMR1NRsRecord	= 40,
	RecordType_mMR1RtRecord	= 41,
	RecordType_mMR1AFRecord	= 42,
	RecordType_mMR4DRqRecord	= 43,
	RecordType_mMR4DRsRecord	= 44,
	RecordType_mMR1RRRecord	= 45,
	RecordType_mMR4RRqRecord	= 46,
	RecordType_mMR4RRsRecord	= 47,
	RecordType_mMRMDRecord	= 48,
	RecordType_mMFRecord	= 49,
	RecordType_mMBx1SRecord	= 50,
	RecordType_mMBx1VRecord	= 51,
	RecordType_mMBx1URecord	= 52,
	RecordType_mMBx1DRecord	= 53,
	RecordType_mM7SRecord	= 54,
	RecordType_mM7DRqRecord	= 55,
	RecordType_mM7DRsRecord	= 56,
	RecordType_mM7CRecord	= 57,
	RecordType_mM7RRecord	= 58,
	RecordType_mM7DRRqRecord	= 59,
	RecordType_mM7DRRsRecord	= 60,
	RecordType_mM7RRqRecord	= 61,
	RecordType_mM7RRsRecord	= 62,
	RecordType_sCSCFRecord	= 63,
	RecordType_pCSCFRecord	= 64,
	RecordType_iCSCFRecord	= 65,
	RecordType_mRFCRecord	= 66,
	RecordType_mGCFRecord	= 67,
	RecordType_bGCFRecord	= 68,
	RecordType_aSRecord	= 69,
	RecordType_iBCFRecord	= 82,
	RecordType_egsnPDPRecord	= 70,
	RecordType_lCSGMORecord	= 71,
	RecordType_lCSRGMTRecord	= 72,
	RecordType_lCSHGMTRecord	= 73,
	RecordType_lCSVGMTRecord	= 74,
	RecordType_lCSGNIRecord	= 75,
	RecordType_sgsnMBMSRecord	= 76,
	RecordType_ggsnMBMSRecord	= 77,
	RecordType_sUBBMSCRecord	= 78,
	RecordType_cONTENTBMSCRecord	= 79,
	RecordType_pPFRecord	= 80,
	RecordType_cPFRecord	= 81
} e_RecordType;

/* RecordType */
typedef long	 RecordType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RecordType;
asn_struct_free_f RecordType_free;
asn_struct_print_f RecordType_print;
asn_constr_check_f RecordType_constraint;
ber_type_decoder_f RecordType_decode_ber;
der_type_encoder_f RecordType_encode_der;
xer_type_decoder_f RecordType_decode_xer;
xer_type_encoder_f RecordType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _RecordType_H_ */
