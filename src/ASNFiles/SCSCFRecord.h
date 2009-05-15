/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "IMSChargingDataTypes"
 * 	found in "ims_charging-data_types.asn1"
 * 	`asn1c -fnative-types -fcompound-names -findirect-choice -fnative-types`
 */

#ifndef	_SCSCFRecord_H_
#define	_SCSCFRecord_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RecordType.h"
#include <NULL.h>
#include "SIP-Method.h"
#include "Role-of-Node.h"
#include "Session-Id.h"
#include <UniversalString.h>
#include "TimeStamp.h"
#include "LocalSequenceNumber.h"
#include <NativeInteger.h>
#include "CauseForRecordClosing.h"
#include "IMS-Charging-Identifier.h"
#include <UTF8String.h>
#include <OCTET_STRING.h>
#include "ServiceContextID.h"
#include "IMSCommunicationServiceIdentifier.h"
#include "NumberPortabilityRouting.h"
#include "CarrierSelectRouting.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SET.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */

/*
 * Method of determining the components presence
 */
typedef enum SCSCFRecord_PR {
	SCSCFRecord_PR_recordType,	/* Member recordType is present */
	SCSCFRecord_PR_retransmission,	/* Member retransmission is present */
	SCSCFRecord_PR_sIP_Method,	/* Member sIP_Method is present */
	SCSCFRecord_PR_role_of_Node,	/* Member role_of_Node is present */
	SCSCFRecord_PR_nodeAddress,	/* Member nodeAddress is present */
	SCSCFRecord_PR_session_Id,	/* Member session_Id is present */
	SCSCFRecord_PR_list_Of_Calling_Party_Address,	/* Member list_Of_Calling_Party_Address is present */
	SCSCFRecord_PR_called_Party_Address,	/* Member called_Party_Address is present */
	SCSCFRecord_PR_privateUserID,	/* Member privateUserID is present */
	SCSCFRecord_PR_serviceRequestTimeStamp,	/* Member serviceRequestTimeStamp is present */
	SCSCFRecord_PR_serviceDeliveryStartTimeStamp,	/* Member serviceDeliveryStartTimeStamp is present */
	SCSCFRecord_PR_serviceDeliveryEndTimeStamp,	/* Member serviceDeliveryEndTimeStamp is present */
	SCSCFRecord_PR_recordOpeningTime,	/* Member recordOpeningTime is present */
	SCSCFRecord_PR_recordClosureTime,	/* Member recordClosureTime is present */
	SCSCFRecord_PR_interOperatorIdentifiers,	/* Member interOperatorIdentifiers is present */
	SCSCFRecord_PR_localRecordSequenceNumber,	/* Member localRecordSequenceNumber is present */
	SCSCFRecord_PR_recordSequenceNumber,	/* Member recordSequenceNumber is present */
	SCSCFRecord_PR_causeForRecordClosing,	/* Member causeForRecordClosing is present */
	SCSCFRecord_PR_incomplete_CDR_Indication,	/* Member incomplete_CDR_Indication is present */
	SCSCFRecord_PR_iMS_Charging_Identifier,	/* Member iMS_Charging_Identifier is present */
	SCSCFRecord_PR_list_Of_SDP_Media_Components,	/* Member list_Of_SDP_Media_Components is present */
	SCSCFRecord_PR_gGSNaddress,	/* Member gGSNaddress is present */
	SCSCFRecord_PR_serviceReasonReturnCode,	/* Member serviceReasonReturnCode is present */
	SCSCFRecord_PR_list_Of_Message_Bodies,	/* Member list_Of_Message_Bodies is present */
	SCSCFRecord_PR_expiresInformation,	/* Member expiresInformation is present */
	SCSCFRecord_PR_list_Of_Associated_URI,	/* Member list_Of_Associated_URI is present */
	SCSCFRecord_PR_event,	/* Member event is present */
	SCSCFRecord_PR_accessNetworkInformation,	/* Member accessNetworkInformation is present */
	SCSCFRecord_PR_serviceContextID,	/* Member serviceContextID is present */
	SCSCFRecord_PR_list_of_subscription_ID,	/* Member list_of_subscription_ID is present */
	SCSCFRecord_PR_list_Of_Early_SDP_Media_Components,	/* Member list_Of_Early_SDP_Media_Components is present */
	SCSCFRecord_PR_iMSCommunicationServiceIdentifier,	/* Member iMSCommunicationServiceIdentifier is present */
	SCSCFRecord_PR_numberPortabilityRouting,	/* Member numberPortabilityRouting is present */
	SCSCFRecord_PR_carrierSelectRouting,	/* Member carrierSelectRouting is present */
	SCSCFRecord_PR_applicationServersInformation,	/* Member applicationServersInformation is present */
	SCSCFRecord_PR_requested_Party_Address,	/* Member requested_Party_Address is present */
	SCSCFRecord_PR_list_Of_Called_Asserted_Identity,	/* Member list_Of_Called_Asserted_Identity is present */
} SCSCFRecord_PR;

/* Forward declarations */
struct NodeAddress;
struct ListOfInvolvedParties;
struct InvolvedParty;
struct InterOperatorIdentifierlist;
struct Incomplete_CDR_Indication;
struct Media_Components_List;
struct MessageBody;
struct SubscriptionID;
struct Early_Media_Components_List;
struct ApplicationServersInformation;

/* SCSCFRecord */
typedef struct SCSCFRecord {
	RecordType_t	 recordType;
	NULL_t	*retransmission	/* OPTIONAL */;
	SIP_Method_t	*sIP_Method	/* OPTIONAL */;
	Role_of_Node_t	*role_of_Node	/* OPTIONAL */;
	struct NodeAddress	*nodeAddress	/* OPTIONAL */;
	Session_Id_t	*session_Id	/* OPTIONAL */;
	struct ListOfInvolvedParties	*list_Of_Calling_Party_Address	/* OPTIONAL */;
	struct InvolvedParty	*called_Party_Address	/* OPTIONAL */;
	UniversalString_t	*privateUserID	/* OPTIONAL */;
	TimeStamp_t	*serviceRequestTimeStamp	/* OPTIONAL */;
	TimeStamp_t	*serviceDeliveryStartTimeStamp	/* OPTIONAL */;
	TimeStamp_t	*serviceDeliveryEndTimeStamp	/* OPTIONAL */;
	TimeStamp_t	*recordOpeningTime	/* OPTIONAL */;
	TimeStamp_t	*recordClosureTime	/* OPTIONAL */;
	struct InterOperatorIdentifierlist	*interOperatorIdentifiers	/* OPTIONAL */;
	LocalSequenceNumber_t	*localRecordSequenceNumber	/* OPTIONAL */;
	long	*recordSequenceNumber	/* OPTIONAL */;
	CauseForRecordClosing_t	*causeForRecordClosing	/* OPTIONAL */;
	struct Incomplete_CDR_Indication	*incomplete_CDR_Indication	/* OPTIONAL */;
	IMS_Charging_Identifier_t	*iMS_Charging_Identifier	/* OPTIONAL */;
	struct SCSCFRecord__list_Of_SDP_Media_Components {
		A_SEQUENCE_OF(struct Media_Components_List) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *list_Of_SDP_Media_Components;
	struct NodeAddress	*gGSNaddress	/* OPTIONAL */;
	UTF8String_t	*serviceReasonReturnCode	/* OPTIONAL */;
	struct SCSCFRecord__list_Of_Message_Bodies {
		A_SEQUENCE_OF(struct MessageBody) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *list_Of_Message_Bodies;
	long	*expiresInformation	/* OPTIONAL */;
	struct ListOfInvolvedParties	*list_Of_Associated_URI	/* OPTIONAL */;
	UTF8String_t	*event	/* OPTIONAL */;
	OCTET_STRING_t	*accessNetworkInformation	/* OPTIONAL */;
	ServiceContextID_t	*serviceContextID	/* OPTIONAL */;
	struct SCSCFRecord__list_of_subscription_ID {
		A_SEQUENCE_OF(struct SubscriptionID) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *list_of_subscription_ID;
	struct SCSCFRecord__list_Of_Early_SDP_Media_Components {
		A_SEQUENCE_OF(struct Early_Media_Components_List) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *list_Of_Early_SDP_Media_Components;
	IMSCommunicationServiceIdentifier_t	*iMSCommunicationServiceIdentifier	/* OPTIONAL */;
	NumberPortabilityRouting_t	*numberPortabilityRouting	/* OPTIONAL */;
	CarrierSelectRouting_t	*carrierSelectRouting	/* OPTIONAL */;
	struct SCSCFRecord__applicationServersInformation {
		A_SEQUENCE_OF(struct ApplicationServersInformation) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *applicationServersInformation;
	struct InvolvedParty	*requested_Party_Address	/* OPTIONAL */;
	struct ListOfInvolvedParties	*list_Of_Called_Asserted_Identity	/* OPTIONAL */;
	
	/* Presence bitmask: ASN_SET_ISPRESENT(pSCSCFRecord, SCSCFRecord_PR_x) */
	unsigned int _presence_map
		[((37+(8*sizeof(unsigned int))-1)/(8*sizeof(unsigned int)))];
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SCSCFRecord_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SCSCFRecord;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "NodeAddress.h"
#include "ListOfInvolvedParties.h"
#include "InvolvedParty.h"
#include "InterOperatorIdentifierlist.h"
#include "Incomplete-CDR-Indication.h"
#include "Media-Components-List.h"
#include "MessageBody.h"
#include "SubscriptionID.h"
#include "Early-Media-Components-List.h"
#include "ApplicationServersInformation.h"

#endif	/* _SCSCFRecord_H_ */