/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#include <asn_internal.h>

#include "TrunkGroupID.h"

static asn_TYPE_member_t asn_MBR_TrunkGroupID_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TrunkGroupID, choice.incoming),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UniversalString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"incoming"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TrunkGroupID, choice.outgoing),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UniversalString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"outgoing"
		},
};
static asn_TYPE_tag2member_t asn_MAP_TrunkGroupID_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* incoming at 442 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* outgoing at 443 */
};
static asn_CHOICE_specifics_t asn_SPC_TrunkGroupID_specs_1 = {
	sizeof(struct TrunkGroupID),
	offsetof(struct TrunkGroupID, _asn_ctx),
	offsetof(struct TrunkGroupID, present),
	sizeof(((struct TrunkGroupID *)0)->present),
	asn_MAP_TrunkGroupID_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_TrunkGroupID = {
	"TrunkGroupID",
	"TrunkGroupID",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_TrunkGroupID_1,
	2,	/* Elements count */
	&asn_SPC_TrunkGroupID_specs_1	/* Additional specs */
};

