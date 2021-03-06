/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#include <asn_internal.h>

#include "PCSCFRecord.h"

static asn_TYPE_member_t asn_MBR_list_Of_SDP_Media_Components_21[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_Media_Components_List,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_list_Of_SDP_Media_Components_tags_21[] = {
	(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_list_Of_SDP_Media_Components_specs_21 = {
	sizeof(struct PCSCFRecord__list_Of_SDP_Media_Components),
	offsetof(struct PCSCFRecord__list_Of_SDP_Media_Components, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_list_Of_SDP_Media_Components_21 = {
	"list-Of-SDP-Media-Components",
	"list-Of-SDP-Media-Components",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_list_Of_SDP_Media_Components_tags_21,
	sizeof(asn_DEF_list_Of_SDP_Media_Components_tags_21)
		/sizeof(asn_DEF_list_Of_SDP_Media_Components_tags_21[0]) - 1, /* 1 */
	asn_DEF_list_Of_SDP_Media_Components_tags_21,	/* Same as above */
	sizeof(asn_DEF_list_Of_SDP_Media_Components_tags_21)
		/sizeof(asn_DEF_list_Of_SDP_Media_Components_tags_21[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_list_Of_SDP_Media_Components_21,
	1,	/* Single element */
	&asn_SPC_list_Of_SDP_Media_Components_specs_21	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_list_Of_Message_Bodies_25[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_MessageBody,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_list_Of_Message_Bodies_tags_25[] = {
	(ASN_TAG_CLASS_CONTEXT | (24 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_list_Of_Message_Bodies_specs_25 = {
	sizeof(struct PCSCFRecord__list_Of_Message_Bodies),
	offsetof(struct PCSCFRecord__list_Of_Message_Bodies, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_list_Of_Message_Bodies_25 = {
	"list-Of-Message-Bodies",
	"list-Of-Message-Bodies",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_list_Of_Message_Bodies_tags_25,
	sizeof(asn_DEF_list_Of_Message_Bodies_tags_25)
		/sizeof(asn_DEF_list_Of_Message_Bodies_tags_25[0]) - 1, /* 1 */
	asn_DEF_list_Of_Message_Bodies_tags_25,	/* Same as above */
	sizeof(asn_DEF_list_Of_Message_Bodies_tags_25)
		/sizeof(asn_DEF_list_Of_Message_Bodies_tags_25[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_list_Of_Message_Bodies_25,
	1,	/* Single element */
	&asn_SPC_list_Of_Message_Bodies_specs_25	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_list_of_subscription_ID_32[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (17 << 2)),
		0,
		&asn_DEF_SubscriptionID,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_list_of_subscription_ID_tags_32[] = {
	(ASN_TAG_CLASS_CONTEXT | (31 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_list_of_subscription_ID_specs_32 = {
	sizeof(struct PCSCFRecord__list_of_subscription_ID),
	offsetof(struct PCSCFRecord__list_of_subscription_ID, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_list_of_subscription_ID_32 = {
	"list-of-subscription-ID",
	"list-of-subscription-ID",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_list_of_subscription_ID_tags_32,
	sizeof(asn_DEF_list_of_subscription_ID_tags_32)
		/sizeof(asn_DEF_list_of_subscription_ID_tags_32[0]) - 1, /* 1 */
	asn_DEF_list_of_subscription_ID_tags_32,	/* Same as above */
	sizeof(asn_DEF_list_of_subscription_ID_tags_32)
		/sizeof(asn_DEF_list_of_subscription_ID_tags_32[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_list_of_subscription_ID_32,
	1,	/* Single element */
	&asn_SPC_list_of_subscription_ID_specs_32	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_list_Of_Early_SDP_Media_Components_34[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_Early_Media_Components_List,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_list_Of_Early_SDP_Media_Components_tags_34[] = {
	(ASN_TAG_CLASS_CONTEXT | (32 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_list_Of_Early_SDP_Media_Components_specs_34 = {
	sizeof(struct PCSCFRecord__list_Of_Early_SDP_Media_Components),
	offsetof(struct PCSCFRecord__list_Of_Early_SDP_Media_Components, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_list_Of_Early_SDP_Media_Components_34 = {
	"list-Of-Early-SDP-Media-Components",
	"list-Of-Early-SDP-Media-Components",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_list_Of_Early_SDP_Media_Components_tags_34,
	sizeof(asn_DEF_list_Of_Early_SDP_Media_Components_tags_34)
		/sizeof(asn_DEF_list_Of_Early_SDP_Media_Components_tags_34[0]) - 1, /* 1 */
	asn_DEF_list_Of_Early_SDP_Media_Components_tags_34,	/* Same as above */
	sizeof(asn_DEF_list_Of_Early_SDP_Media_Components_tags_34)
		/sizeof(asn_DEF_list_Of_Early_SDP_Media_Components_tags_34[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_list_Of_Early_SDP_Media_Components_34,
	1,	/* Single element */
	&asn_SPC_list_Of_Early_SDP_Media_Components_specs_34	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_PCSCFRecord_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PCSCFRecord, recordType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RecordType,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordType"
		},
	{ ATF_POINTER, 32, offsetof(struct PCSCFRecord, retransmission),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"retransmission"
		},
	{ ATF_POINTER, 31, offsetof(struct PCSCFRecord, sIP_Method),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SIP_Method,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"sIP-Method"
		},
	{ ATF_POINTER, 30, offsetof(struct PCSCFRecord, role_of_Node),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Role_of_Node,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"role-of-Node"
		},
	{ ATF_POINTER, 29, offsetof(struct PCSCFRecord, nodeAddress),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_NodeAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"nodeAddress"
		},
	{ ATF_POINTER, 28, offsetof(struct PCSCFRecord, session_Id),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Session_Id,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"session-Id"
		},
	{ ATF_POINTER, 27, offsetof(struct PCSCFRecord, list_Of_Calling_Party_Address),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ListOfInvolvedParties,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"list-Of-Calling-Party-Address"
		},
	{ ATF_POINTER, 26, offsetof(struct PCSCFRecord, called_Party_Address),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_InvolvedParty,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"called-Party-Address"
		},
	{ ATF_POINTER, 25, offsetof(struct PCSCFRecord, serviceRequestTimeStamp),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeStamp,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"serviceRequestTimeStamp"
		},
	{ ATF_POINTER, 24, offsetof(struct PCSCFRecord, serviceDeliveryStartTimeStamp),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeStamp,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"serviceDeliveryStartTimeStamp"
		},
	{ ATF_POINTER, 23, offsetof(struct PCSCFRecord, serviceDeliveryEndTimeStamp),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeStamp,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"serviceDeliveryEndTimeStamp"
		},
	{ ATF_POINTER, 22, offsetof(struct PCSCFRecord, recordOpeningTime),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeStamp,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordOpeningTime"
		},
	{ ATF_POINTER, 21, offsetof(struct PCSCFRecord, recordClosureTime),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeStamp,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordClosureTime"
		},
	{ ATF_POINTER, 20, offsetof(struct PCSCFRecord, interOperatorIdentifiers),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_InterOperatorIdentifierlist,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"interOperatorIdentifiers"
		},
	{ ATF_POINTER, 19, offsetof(struct PCSCFRecord, localRecordSequenceNumber),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LocalSequenceNumber,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"localRecordSequenceNumber"
		},
	{ ATF_POINTER, 18, offsetof(struct PCSCFRecord, recordSequenceNumber),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"recordSequenceNumber"
		},
	{ ATF_POINTER, 17, offsetof(struct PCSCFRecord, causeForRecordClosing),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CauseForRecordClosing,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"causeForRecordClosing"
		},
	{ ATF_POINTER, 16, offsetof(struct PCSCFRecord, incomplete_CDR_Indication),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Incomplete_CDR_Indication,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"incomplete-CDR-Indication"
		},
	{ ATF_POINTER, 15, offsetof(struct PCSCFRecord, iMS_Charging_Identifier),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IMS_Charging_Identifier,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"iMS-Charging-Identifier"
		},
	{ ATF_POINTER, 14, offsetof(struct PCSCFRecord, list_Of_SDP_Media_Components),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_list_Of_SDP_Media_Components_21,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"list-Of-SDP-Media-Components"
		},
	{ ATF_POINTER, 13, offsetof(struct PCSCFRecord, gGSNaddress),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_NodeAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"gGSNaddress"
		},
	{ ATF_POINTER, 12, offsetof(struct PCSCFRecord, serviceReasonReturnCode),
		(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"serviceReasonReturnCode"
		},
	{ ATF_POINTER, 11, offsetof(struct PCSCFRecord, list_Of_Message_Bodies),
		(ASN_TAG_CLASS_CONTEXT | (24 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_list_Of_Message_Bodies_25,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"list-Of-Message-Bodies"
		},
	{ ATF_POINTER, 10, offsetof(struct PCSCFRecord, expiresInformation),
		(ASN_TAG_CLASS_CONTEXT | (26 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"expiresInformation"
		},
	{ ATF_POINTER, 9, offsetof(struct PCSCFRecord, list_Of_Associated_URI),
		(ASN_TAG_CLASS_CONTEXT | (27 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ListOfInvolvedParties,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"list-Of-Associated-URI"
		},
	{ ATF_POINTER, 8, offsetof(struct PCSCFRecord, event),
		(ASN_TAG_CLASS_CONTEXT | (28 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UTF8String,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"event"
		},
	{ ATF_POINTER, 7, offsetof(struct PCSCFRecord, accessNetworkInformation),
		(ASN_TAG_CLASS_CONTEXT | (29 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"accessNetworkInformation"
		},
	{ ATF_POINTER, 6, offsetof(struct PCSCFRecord, serviceContextID),
		(ASN_TAG_CLASS_CONTEXT | (30 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServiceContextID,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"serviceContextID"
		},
	{ ATF_POINTER, 5, offsetof(struct PCSCFRecord, list_of_subscription_ID),
		(ASN_TAG_CLASS_CONTEXT | (31 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_list_of_subscription_ID_32,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"list-of-subscription-ID"
		},
	{ ATF_POINTER, 4, offsetof(struct PCSCFRecord, list_Of_Early_SDP_Media_Components),
		(ASN_TAG_CLASS_CONTEXT | (32 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_list_Of_Early_SDP_Media_Components_34,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"list-Of-Early-SDP-Media-Components"
		},
	{ ATF_POINTER, 3, offsetof(struct PCSCFRecord, numberPortabilityRouting),
		(ASN_TAG_CLASS_CONTEXT | (34 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NumberPortabilityRouting,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"numberPortabilityRouting"
		},
	{ ATF_POINTER, 2, offsetof(struct PCSCFRecord, carrierSelectRouting),
		(ASN_TAG_CLASS_CONTEXT | (35 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CarrierSelectRouting,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"carrierSelectRouting"
		},
	{ ATF_POINTER, 1, offsetof(struct PCSCFRecord, servedPartyIPAddress),
		(ASN_TAG_CLASS_CONTEXT | (50 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ServedPartyIPAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"servedPartyIPAddress"
		},
};
static ber_tlv_tag_t asn_DEF_PCSCFRecord_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (17 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_PCSCFRecord_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* recordType at 84 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* retransmission at 85 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* sIP-Method at 86 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* role-of-Node at 87 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* nodeAddress at 88 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* session-Id at 89 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* list-Of-Calling-Party-Address at 90 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* called-Party-Address at 91 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 8, 0, 0 }, /* serviceRequestTimeStamp at 92 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 9, 0, 0 }, /* serviceDeliveryStartTimeStamp at 93 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 10, 0, 0 }, /* serviceDeliveryEndTimeStamp at 94 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 11, 0, 0 }, /* recordOpeningTime at 95 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 12, 0, 0 }, /* recordClosureTime at 96 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 13, 0, 0 }, /* interOperatorIdentifiers at 97 */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 14, 0, 0 }, /* localRecordSequenceNumber at 98 */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 15, 0, 0 }, /* recordSequenceNumber at 99 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 16, 0, 0 }, /* causeForRecordClosing at 100 */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 17, 0, 0 }, /* incomplete-CDR-Indication at 101 */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 18, 0, 0 }, /* iMS-Charging-Identifier at 102 */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 19, 0, 0 }, /* list-Of-SDP-Media-Components at 103 */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 20, 0, 0 }, /* gGSNaddress at 104 */
    { (ASN_TAG_CLASS_CONTEXT | (23 << 2)), 21, 0, 0 }, /* serviceReasonReturnCode at 105 */
    { (ASN_TAG_CLASS_CONTEXT | (24 << 2)), 22, 0, 0 }, /* list-Of-Message-Bodies at 106 */
    { (ASN_TAG_CLASS_CONTEXT | (26 << 2)), 23, 0, 0 }, /* expiresInformation at 108 */
    { (ASN_TAG_CLASS_CONTEXT | (27 << 2)), 24, 0, 0 }, /* list-Of-Associated-URI at 109 */
    { (ASN_TAG_CLASS_CONTEXT | (28 << 2)), 25, 0, 0 }, /* event at 110 */
    { (ASN_TAG_CLASS_CONTEXT | (29 << 2)), 26, 0, 0 }, /* accessNetworkInformation at 111 */
    { (ASN_TAG_CLASS_CONTEXT | (30 << 2)), 27, 0, 0 }, /* serviceContextID at 112 */
    { (ASN_TAG_CLASS_CONTEXT | (31 << 2)), 28, 0, 0 }, /* list-of-subscription-ID at 113 */
    { (ASN_TAG_CLASS_CONTEXT | (32 << 2)), 29, 0, 0 }, /* list-Of-Early-SDP-Media-Components at 114 */
    { (ASN_TAG_CLASS_CONTEXT | (34 << 2)), 30, 0, 0 }, /* numberPortabilityRouting at 115 */
    { (ASN_TAG_CLASS_CONTEXT | (35 << 2)), 31, 0, 0 }, /* carrierSelectRouting at 116 */
    { (ASN_TAG_CLASS_CONTEXT | (50 << 2)), 32, 0, 0 } /* servedPartyIPAddress at 117 */
};
static uint8_t asn_MAP_PCSCFRecord_mmap_1[(33 + (8 * sizeof(unsigned int)) - 1) / 8] = {
	(1 << 7) | (0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0 << 2) | (0 << 1) | (0 << 0),
	(0 << 7) | (0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0 << 2) | (0 << 1) | (0 << 0),
	(0 << 7) | (0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0 << 2) | (0 << 1) | (0 << 0),
	(0 << 7) | (0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0 << 2) | (0 << 1) | (0 << 0),
	(0 << 7)
};
static asn_SET_specifics_t asn_SPC_PCSCFRecord_specs_1 = {
	sizeof(struct PCSCFRecord),
	offsetof(struct PCSCFRecord, _asn_ctx),
	offsetof(struct PCSCFRecord, _presence_map),
	asn_MAP_PCSCFRecord_tag2el_1,
	33,	/* Count of tags in the map */
	asn_MAP_PCSCFRecord_tag2el_1,	/* Same as above */
	33,	/* Count of tags in the CXER map */
	0,	/* Whether extensible */
	(unsigned int *)asn_MAP_PCSCFRecord_mmap_1	/* Mandatory elements map */
};
asn_TYPE_descriptor_t asn_DEF_PCSCFRecord = {
	"PCSCFRecord",
	"PCSCFRecord",
	SET_free,
	SET_print,
	SET_constraint,
	SET_decode_ber,
	SET_encode_der,
	SET_decode_xer,
	SET_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_PCSCFRecord_tags_1,
	sizeof(asn_DEF_PCSCFRecord_tags_1)
		/sizeof(asn_DEF_PCSCFRecord_tags_1[0]), /* 1 */
	asn_DEF_PCSCFRecord_tags_1,	/* Same as above */
	sizeof(asn_DEF_PCSCFRecord_tags_1)
		/sizeof(asn_DEF_PCSCFRecord_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_PCSCFRecord_1,
	33,	/* Elements count */
	&asn_SPC_PCSCFRecord_specs_1	/* Additional specs */
};

