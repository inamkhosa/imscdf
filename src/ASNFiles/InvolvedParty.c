/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#include <asn_internal.h>

#include "InvolvedParty.h"

static asn_TYPE_member_t asn_MBR_InvolvedParty_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct InvolvedParty, choice.sIP_URI),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UniversalString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"sIP-URI"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InvolvedParty, choice.tEL_URI),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UniversalString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"tEL-URI"
		},
};
static asn_TYPE_tag2member_t asn_MAP_InvolvedParty_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* sIP-URI at 341 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* tEL-URI at 342 */
};
static asn_CHOICE_specifics_t asn_SPC_InvolvedParty_specs_1 = {
	sizeof(struct InvolvedParty),
	offsetof(struct InvolvedParty, _asn_ctx),
	offsetof(struct InvolvedParty, present),
	sizeof(((struct InvolvedParty *)0)->present),
	asn_MAP_InvolvedParty_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_InvolvedParty = {
	"InvolvedParty",
	"InvolvedParty",
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
	asn_MBR_InvolvedParty_1,
	2,	/* Elements count */
	&asn_SPC_InvolvedParty_specs_1	/* Additional specs */
};
