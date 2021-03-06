/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_ICSCFRecord_H_
#define	_ICSCFRecord_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RecordType.h"
#include <NULL.h>
#include "SIP-Method.h"
#include "Role-of-Node.h"
#include "Session-Id.h"
#include "TimeStamp.h"
#include "LocalSequenceNumber.h"
#include "CauseForRecordClosing.h"
#include "IMS-Charging-Identifier.h"
#include <UTF8String.h>
#include <NativeInteger.h>
#include <OCTET_STRING.h>
#include "ServiceContextID.h"
#include "NumberPortabilityRouting.h"
#include "CarrierSelectRouting.h"
#include <constr_SET.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */

/*
 * Method of determining the components presence
 */
typedef enum ICSCFRecord_PR {
	ICSCFRecord_PR_recordType,	/* Member recordType is present */
	ICSCFRecord_PR_retransmission,	/* Member retransmission is present */
	ICSCFRecord_PR_sIP_Method,	/* Member sIP_Method is present */
	ICSCFRecord_PR_role_of_Node,	/* Member role_of_Node is present */
	ICSCFRecord_PR_nodeAddress,	/* Member nodeAddress is present */
	ICSCFRecord_PR_session_Id,	/* Member session_Id is present */
	ICSCFRecord_PR_list_Of_Calling_Party_Address,	/* Member list_Of_Calling_Party_Address is present */
	ICSCFRecord_PR_called_Party_Address,	/* Member called_Party_Address is present */
	ICSCFRecord_PR_serviceRequestTimeStamp,	/* Member serviceRequestTimeStamp is present */
	ICSCFRecord_PR_interOperatorIdentifiers,	/* Member interOperatorIdentifiers is present */
	ICSCFRecord_PR_localRecordSequenceNumber,	/* Member localRecordSequenceNumber is present */
	ICSCFRecord_PR_causeForRecordClosing,	/* Member causeForRecordClosing is present */
	ICSCFRecord_PR_incomplete_CDR_Indication,	/* Member incomplete_CDR_Indication is present */
	ICSCFRecord_PR_iMS_Charging_Identifier,	/* Member iMS_Charging_Identifier is present */
	ICSCFRecord_PR_serviceReasonReturnCode,	/* Member serviceReasonReturnCode is present */
	ICSCFRecord_PR_expiresInformation,	/* Member expiresInformation is present */
	ICSCFRecord_PR_list_Of_Associated_URI,	/* Member list_Of_Associated_URI is present */
	ICSCFRecord_PR_event,	/* Member event is present */
	ICSCFRecord_PR_accessNetworkInformation,	/* Member accessNetworkInformation is present */
	ICSCFRecord_PR_serviceContextID,	/* Member serviceContextID is present */
	ICSCFRecord_PR_numberPortabilityRouting,	/* Member numberPortabilityRouting is present */
	ICSCFRecord_PR_carrierSelectRouting,	/* Member carrierSelectRouting is present */
	ICSCFRecord_PR_s_CSCF_Information,	/* Member s_CSCF_Information is present */
} ICSCFRecord_PR;

/* Forward declarations */
struct NodeAddress;
struct ListOfInvolvedParties;
struct InvolvedParty;
struct InterOperatorIdentifierlist;
struct Incomplete_CDR_Indication;
struct S_CSCF_Information;

/* ICSCFRecord */
typedef struct ICSCFRecord {
	RecordType_t	 recordType;
	NULL_t	*retransmission	/* OPTIONAL */;
	SIP_Method_t	*sIP_Method	/* OPTIONAL */;
	Role_of_Node_t	*role_of_Node	/* OPTIONAL */;
	struct NodeAddress	*nodeAddress	/* OPTIONAL */;
	Session_Id_t	*session_Id	/* OPTIONAL */;
	struct ListOfInvolvedParties	*list_Of_Calling_Party_Address	/* OPTIONAL */;
	struct InvolvedParty	*called_Party_Address	/* OPTIONAL */;
	TimeStamp_t	*serviceRequestTimeStamp	/* OPTIONAL */;
	struct InterOperatorIdentifierlist	*interOperatorIdentifiers	/* OPTIONAL */;
	LocalSequenceNumber_t	*localRecordSequenceNumber	/* OPTIONAL */;
	CauseForRecordClosing_t	*causeForRecordClosing	/* OPTIONAL */;
	struct Incomplete_CDR_Indication	*incomplete_CDR_Indication	/* OPTIONAL */;
	IMS_Charging_Identifier_t	*iMS_Charging_Identifier	/* OPTIONAL */;
	UTF8String_t	*serviceReasonReturnCode	/* OPTIONAL */;
	long	*expiresInformation	/* OPTIONAL */;
	struct ListOfInvolvedParties	*list_Of_Associated_URI	/* OPTIONAL */;
	UTF8String_t	*event	/* OPTIONAL */;
	OCTET_STRING_t	*accessNetworkInformation	/* OPTIONAL */;
	ServiceContextID_t	*serviceContextID	/* OPTIONAL */;
	NumberPortabilityRouting_t	*numberPortabilityRouting	/* OPTIONAL */;
	CarrierSelectRouting_t	*carrierSelectRouting	/* OPTIONAL */;
	struct S_CSCF_Information	*s_CSCF_Information	/* OPTIONAL */;
	
	/* Presence bitmask: ASN_SET_ISPRESENT(pICSCFRecord, ICSCFRecord_PR_x) */
	unsigned int _presence_map
		[((23+(8*sizeof(unsigned int))-1)/(8*sizeof(unsigned int)))];
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ICSCFRecord_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ICSCFRecord;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "NodeAddress.h"
#include "ListOfInvolvedParties.h"
#include "InvolvedParty.h"
#include "InterOperatorIdentifierlist.h"
#include "Incomplete-CDR-Indication.h"
#include "S-CSCF-Information.h"

#endif	/* _ICSCFRecord_H_ */
