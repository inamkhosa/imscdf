/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#include <asn_internal.h>

#include "InterOperatorIdentifiers.h"

static asn_TYPE_member_t asn_MBR_InterOperatorIdentifiers_1[] = {
	{ ATF_POINTER, 2, offsetof(struct InterOperatorIdentifiers, originatingIOI),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UniversalString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"originatingIOI"
		},
	{ ATF_POINTER, 1, offsetof(struct InterOperatorIdentifiers, terminatingIOI),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UniversalString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"terminatingIOI"
		},
};
static ber_tlv_tag_t asn_DEF_InterOperatorIdentifiers_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_InterOperatorIdentifiers_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* originatingIOI at 353 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* terminatingIOI at 354 */
};
static asn_SEQUENCE_specifics_t asn_SPC_InterOperatorIdentifiers_specs_1 = {
	sizeof(struct InterOperatorIdentifiers),
	offsetof(struct InterOperatorIdentifiers, _asn_ctx),
	asn_MAP_InterOperatorIdentifiers_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_InterOperatorIdentifiers = {
	"InterOperatorIdentifiers",
	"InterOperatorIdentifiers",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_InterOperatorIdentifiers_tags_1,
	sizeof(asn_DEF_InterOperatorIdentifiers_tags_1)
		/sizeof(asn_DEF_InterOperatorIdentifiers_tags_1[0]), /* 1 */
	asn_DEF_InterOperatorIdentifiers_tags_1,	/* Same as above */
	sizeof(asn_DEF_InterOperatorIdentifiers_tags_1)
		/sizeof(asn_DEF_InterOperatorIdentifiers_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_InterOperatorIdentifiers_1,
	2,	/* Elements count */
	&asn_SPC_InterOperatorIdentifiers_specs_1	/* Additional specs */
};

