/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "GenericChargingDataTypes"
 * 	found in "generic_charging_DataTypes.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#include <asn_internal.h>

#include "IPAddress.h"

static asn_TYPE_member_t asn_MBR_IPAddress_1[] = {
	{ ATF_POINTER, 0, offsetof(struct IPAddress, choice.iPBinaryAddress),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_IPBinaryAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"iPBinaryAddress"
		},
	{ ATF_POINTER, 0, offsetof(struct IPAddress, choice.iPTextRepresentedAddress),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_IPTextRepresentedAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"iPTextRepresentedAddress"
		},
};
static asn_TYPE_tag2member_t asn_MAP_IPAddress_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* iPBinV4Address at 163 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 0, 0, 0 }, /* iPBinV6Address at 164 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 1, 0, 0 }, /* iPTextV4Address at 172 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 1, 0, 0 } /* iPTextV6Address at 173 */
};
static asn_CHOICE_specifics_t asn_SPC_IPAddress_specs_1 = {
	sizeof(struct IPAddress),
	offsetof(struct IPAddress, _asn_ctx),
	offsetof(struct IPAddress, present),
	sizeof(((struct IPAddress *)0)->present),
	asn_MAP_IPAddress_tag2el_1,
	4,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_IPAddress = {
	"IPAddress",
	"IPAddress",
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
	asn_MBR_IPAddress_1,
	2,	/* Elements count */
	&asn_SPC_IPAddress_specs_1	/* Additional specs */
};

