

#ifndef _OFFCS_COMM_CDR_CONTENTS_H
#define	_OFFCS_COMM_CDR_CONTENTS_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include"RF_defines.h"
	#include"RF_include.h"
	#include "CdrFileWriterModule.h"
	#include "offcs_com_defines.h"


	#define	OFFLINE_CS_SMALL_STRING_SIZE			32
	#define	OFFLINE_CS_MEDIUM_STRING_SIZE			64
	#define	OFFLINE_CS_LARGE_STRING_SIZE			128
	#define	OFFLINE_CS_ADDRESS_STRING_SIZE			130
	#define	OFFLINE_CS_EXTRA_LARGE_STRING_SIZE		256

	typedef	struct ResponseEvtData
	{
        unsigned int 		 			unTotalLen;
		unsigned int					unResultCode;
		unsigned int         			unAcctRecordNumber;
		unsigned char					ucSessionId[OFFLINE_CS_MEDIUM_STRING_SIZE];
	}t_ResponseEvtData;
	
	 typedef		t_ResponseEvtData	t_ResponseEvtDataFromCdrFWModule;
	 typedef		t_ResponseEvtData	t_ResponseEvtDataFromDBModule;
	 typedef		t_ResponseEvtData	t_ResponseEvtDataForRFModule;


    typedef struct Unsigned32List
	{
		int                    nNumberOfItemsInList;
		unsigned int       	   unItems[OFFLINE_CS_LIST_SIZE];
	}t_Unsigned32List;

	typedef struct EnumList
	{
		int                    nNumberOfItemsInList;
		int       	   		   nItems[OFFLINE_CS_LIST_SIZE];
	}t_EnumList;

	typedef struct UTF8StringList
	{
		int                    nNumberOfItemsInList;
		unsigned char         ucItems[OFFLINE_CS_LIST_SIZE][OFFLINE_CS_MEDIUM_STRING_SIZE];
	}t_UTF8StringList;


	typedef struct TimeQuotaMechanism
	{
		int					nTimeQuotaType;
		unsigned int		unBaseTimeInterval;
	}t_TimeQuotaMechanism;

	typedef struct UsedServiceUnit
	{
		int 			   	nReportingReason; 
		int			       	nTariffChangeUsage;
		unsigned int 	   	unCCTime;
		t_DiamUnsigned64 	oCCTotalOctets;
		t_DiamUnsigned64 	oCCInputOctets;
		t_DiamUnsigned64 	oCCOutputoctets;
		t_DiamUnsigned64 	OCCServiceSpecificUnits;
		t_Unsigned32List 	oListOfEventChargingTimeStamp;  
	}t_UsedServiceUnit;

	typedef struct UsedServiceUnitList
	{
		int                    nNumberOfItemsInList;
		t_UsedServiceUnit      oItems[OFFLINE_CS_LIST_SIZE];
	}t_UsedServiceUnitList;

	typedef struct UnitValue
	{
		t_DiamInteger64  oValueDigit;
		int			     nExponent;
	}t_UnitValue;

	typedef struct GSUPoolReference 
	{
		unsigned int	  unGSUPoolIdentifier; 
		int		  		  nCCUnitType;
		t_UnitValue 	  oUnitValue;
	}t_GSUPoolReference;

	typedef struct GSUPoolReferenceList
	{
		int                    nNumberOfItemsInList;
		t_GSUPoolReference     oItems[OFFLINE_CS_LIST_SIZE];
	}t_GSUPoolReferenceList;

	typedef struct RedirectServer
	{
		int					nRedirectAddressType;
		unsigned char*	 	pucRedirectServerAddress;
	}t_RedirectServer;

	typedef struct FinalUnitIndication
	{
		int	 		  		nFinalUnitAction;
		//#define OFFLINE_CS_RESTRICTION_FILTER_RULE					438 //IPFilterRule
		unsigned char		ucFilterId[OFFLINE_CS_MEDIUM_STRING_SIZE];
		t_RedirectServer  oRedirectserver;
	}t_FinalUnitIndication;

	typedef struct Trigger										
	{
		t_EnumList	 oListofTriggerType;
	}t_Trigger;

	typedef struct PSFurnishingChargingInfo
	{
		unsigned char		        uc3GPPChargingId[OFFLINE_CS_MEDIUM_STRING_SIZE];
        unsigned char			    ucPSFreeFormatData[OFFLINE_CS_MEDIUM_STRING_SIZE];
		int							nPSAppendFreeFormatData;
		// can have a list of other AVps, so how to handle it 
	}t_PSFurnishingChargingInfo;

	typedef struct AFCorrelationInformation			
	{
		//#define OFFLINE_CS_AF_CHARGING_IDENTIFIER					505// type will check from 214
		//#define OFFLINE_CS_FLOWS									510	// MAY OCCUR IN LIST,type will check from 214
	}t_AFCorrelationInformation;

	typedef struct AFCorrelationInformationList
	{
		int                    				  nNumberOfItemsInList;
		t_AFCorrelationInformation       	   oItems[OFFLINE_CS_LIST_SIZE];
	}t_AFCorrelationInformationList;

	typedef struct Envelope										
	{
		unsigned int 			unEnvelopeStartTime;
		unsigned int 			unEnvelopeEndTime;
		unsigned int 			unCCTime;
		t_DiamUnsigned64 		oCCTotalOctets;
		t_DiamUnsigned64 		oCCInputOctets;
		t_DiamUnsigned64 		oCCOutputoctets;
		t_DiamUnsigned64 		OCCServiceSpecificUnits;
     }t_Envelope;

	typedef struct EnvelopeList
	{
		int                    nNumberOfItemsInList;
		t_Envelope       	   oItems[OFFLINE_CS_LIST_SIZE];
	}t_EnvelopeList;

	typedef struct _ServiceSpecificInfo							
	{
		unsigned int 	   unServiceSpecificType; 
		unsigned char 	   ucServiceSpecificData[OFFLINE_CS_MEDIUM_STRING_SIZE];
	}t_ServiceSpecificInfo;

	typedef struct ServiceSpecificInfoList
	{
		int                    		nNumberOfItemsInList;
		t_ServiceSpecificInfo       oItems[OFFLINE_CS_LIST_SIZE];
	}t_ServiceSpecificInfoList;
    
	
	typedef struct MultipleServicesCreditControl
	{
		//#define OFFLINE_CS_GRANTED_SERVICE_UNIT					431// Grouped not present in 299 doc ,WILL LOOK TO IT AVPS LATER
		//#define OFFLINE_CS_REQUESTED_SERVICE_UNIT				    437//Grouped not present in 299 doc ,WILL LOOK TO IT AVPS LATER
		t_UsedServiceUnitList 				oListOfUsedServiceUnit;
        t_Unsigned32List 					oListOfServiceIdentifier;
		int									nTraffChangeUsage;
		unsigned int 	 					unRatingGroup;
		t_GSUPoolReferenceList 				oListOfGSUPoolreference;									
        unsigned int 						unValidityTime;
		unsigned int						unResultCode;
		t_FinalUnitIndication   			oFinalUnitIndication;
		unsigned int						unTimeQuotaThreshold;
		unsigned int 						unVolumeQuotaThreshold;
		unsigned int						unUnitQuotaThreshold; 
		unsigned int 						unQuotaHoldingTime;
		unsigned int						unQuotaConsumptionTime;												
		t_EnumList							oListOfReportingReason;
		t_Trigger							oTrigger;
		t_PSFurnishingChargingInfo			oPSFurnishingChargingInfo;
		unsigned char						ucRefundInformation[OFFLINE_CS_MEDIUM_STRING_SIZE]; 
		t_AFCorrelationInformationList		oListOfAFCorrelationInfo;
		t_EnvelopeList						oListOfEnvelope;
		int									nEnvelopeReporting; 
		t_TimeQuotaMechanism				oTimeQuotaMechanism;
		t_ServiceSpecificInfoList			oListOfServiceSpecificInfo;
		//MAY CONTAIN A LIST OF OTHER AVPS
	}t_MultipleServicesCreditControl;

	typedef struct MultipleServicesCreditControlList
	{
		int                    					nNumberOfItemsInList;
		t_MultipleServicesCreditControl       	oItems[OFFLINE_CS_LIST_SIZE];
	}t_MultipleServicesCreditControlList;



	typedef struct OfflineCharging
	{
		unsigned int							unQuotaConsumptionTime;
		t_TimeQuotaMechanism					oTimeQuotaMechanism;
		int	                  					nEnvlopeReporting;
		t_MultipleServicesCreditControlList		oMultiServicesCrdtControlList;
        // will add the fields after discussion
	}t_OfflineCharging;

    
	typedef struct PSInformation
	{
		unsigned char		   		uc3GPPChargingId[OFFLINE_CS_MEDIUM_STRING_SIZE];
		unsigned char   			uc3GPPPDPType[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char   			ucPDPAddress[OFFLINE_CS_EXTRA_LARGE_STRING_SIZE];
		unsigned char				uc3GPPGPRSNegotiatedQoSProfile[OFFLINE_CS_EXTRA_LARGE_STRING_SIZE];//type will check from 207
		unsigned char				ucSGSNAddress[OFFLINE_CS_EXTRA_LARGE_STRING_SIZE];
		unsigned char				ucGGSNAddress[OFFLINE_CS_EXTRA_LARGE_STRING_SIZE];
		unsigned char				ucCGAddress[OFFLINE_CS_EXTRA_LARGE_STRING_SIZE];
		unsigned char				uc3GPPImsiMccMnc[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char				uc3GPPGgsnMccMnc[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char				uc3GPPNsapi[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char				ucCalledStationId[OFFLINE_CS_MEDIUM_STRING_SIZE];
		unsigned char				ucSessionStopIndicator[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char				uc3GPPSelectionMode[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207 
		unsigned char				uc3GPPChargingCharacteristics[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char				uc3GPPSgsnMccMnc[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char				uc3GPPMsTimeZone[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		t_UTF8StringList			oChargingRuleBasedNameList;											  
        unsigned char				uc3GPPUserLocationInfo[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		unsigned char			 	uc3GPPRatType[OFFLINE_CS_MEDIUM_STRING_SIZE];//type will check from 207
		t_PSFurnishingChargingInfo	oPSFurnishingChargingInfo;
		int 						nPDPContextType;
		t_OfflineCharging			oOfflineCharging;
	}t_PSInformation;

	
	/////////////////////////////////////WLan AVP ///////////////////////

	typedef struct LocationType
	{	
		t_DiamEnum			oLocationEstimateType;
		t_DiamUTF8String	oDeferredLocationEventType;
	}t_LocationType;

	typedef struct WLANRadioContainer
	{	
        t_DiamOctetString	oOperatorName; 			//Currenlty its code and type is unknown
		t_LocationType		oLocationType;			//Grouped
		t_DiamOctetString	oLocationInformation;	//Currenlty its code and type is unknown
		t_DiamUnsigned32	oWLANTechonology;
	}t_WLANRadioContainer;

	typedef struct WLANInformation
	{	
		t_DiamUnsigned32		oWLANSessionId;
		t_DiamIPAddress			OPDGAddress;
		t_DiamUnsigned32		oPGDChargingId;
		t_DiamIPAddress			oWAGAddress;
		t_DiamOctetString		oWAGPLMNId;
		t_WLANRadioContainer	oWLANRadioContainer;
		t_DiamOctetString		oWLANUELocalIPAddress;
	}t_WLANInformation;
	
	

	/////////////////////////MMS-Information AVP/////////
	typedef struct AddressDomain
	{
        t_DiamUTF8String 	oDomainName;
		t_DiamOctetString	O3GppImsiMccmnc; //CURRENLTY ITs TYPE IS UNKNOWN WILL SEE IN 207
	}t_AddressDomain;

    typedef struct OriginatorAddress
	{
		t_DiamEnum			oAddressType;
		t_DiamUTF8String	oAddressData;
		t_AddressDomain		oAddressDomain;
	}t_OriginatorAddress;

	typedef struct OriginatorAddressList
	{
		int                    nNumberOfItemsInList;
		t_OriginatorAddress    oItems[OFFLINE_CS_LIST_SIZE];
	}t_OriginatorAddressList;

	typedef struct RecepientAddress
	{
		t_DiamEnum			oAddressType;
		t_DiamUTF8String	oAddressData;
		t_AddressDomain		oAddressDomain;
		t_DiamEnum			oAddresseeType;
	}t_RecepientAddress;

	typedef struct RecepientAddressList
	{
		int                    nNumberOfItemsInList;
		t_OriginatorAddress    oItems[OFFLINE_CS_LIST_SIZE];
	}t_RecepientAddressList;


	typedef struct AdditionalContentInformation
	{
        t_DiamEnum						OTypeNumber;
		t_DiamUTF8String				OAdditionalTypeInfo;
		t_DiamUnsigned32				OContentSize;
	}t_AdditionalContentInformation;

	typedef struct AdditionalContentInfoList
	{
		int                    				nNumberOfItemsInList;
		t_AdditionalContentInformation    	oItems[OFFLINE_CS_LIST_SIZE];
	}t_AdditionalContentInfoList;


	typedef struct  MMContentType
	{
		t_DiamEnum						OTypeNumber;
		t_DiamUTF8String				OAdditionalTypeInfo;
		t_DiamUnsigned32				OContentSize;
        t_AdditionalContentInfoList 	oListOfAdditionalContentInfo;
	}t_MMContentType;

	typedef struct  MessageClass
	{
		t_DiamEnum			oClassIdentifier;
		t_DiamUTF8String	oTokenText;
	}t_MessageClass;

    typedef struct MMSInformation
	{
		t_OriginatorAddressList		oListOfOriginatorAddress;
		t_RecepientAddressList		oListOfRecepientAddress;
		t_DiamTime					OSubmissionTime;
		t_MMContentType				oMMContentType;
		t_DiamEnum					oPriority;
		t_DiamUTF8String			oMessageId;
		t_DiamEnum					oMessageType;
		t_DiamUnsigned32			oMessageSize;
		t_MessageClass				oMessageClass;
		t_DiamEnum					oDeliveryReportRequested;
		t_DiamEnum					oReadReplyReportRequested;
		t_DiamEnum					oMMBoxStorageRequested;
		t_DiamUTF8String			oApplicId;
		t_DiamUTF8String			oReplyApplicId;
		t_DiamUTF8String			oAuxApplicInfo;
		t_DiamEnum					oContentClass;
		t_DiamEnum					oDRMContent;
		t_DiamEnum					oAdaptations;
		//							oVASPId;// type unknown
		//							oVASId; // type unknown
	}t_MMSInformation;
																		
	//////////////////////////////LCS-Information////////
	typedef struct LCSRequestorId										
	{																	
		t_DiamUTF8String	oLCSDataCodingScheme;							
		t_DiamUTF8String	oLCSRequestorIdString;
	}t_LCSRequestorId;

	typedef struct LCSClientName										
	{																	
		t_DiamUTF8String	oLCSDataCodingScheme;							
        t_DiamUTF8String	oLCSNameString;
        t_DiamEnum			oLCSFormatIndicator;
	}t_LCSClientName;

	typedef struct LCSClientId
	{
		t_DiamEnum			oLCSClientType;
		t_DiamUTF8String	oLCSClientExternalID;
		t_DiamUTF8String	oLCSClientDialedByMS;
		t_LCSClientName 	oLCSClientName;
		t_DiamUTF8String	oLCSAPN;
		t_LCSRequestorId	oLCSRequestorId;
	}t_LCSClientId;

	typedef struct LCSInformation
	{
         t_LCSClientId 		oLCSClientId;
		 t_LocationType		oLocationType;
		 t_DiamUTF8String	oLocationEstimate;
		 t_DiamUTF8String  oPositioningData;
		 //oIMSI;    //type currently unknown 
		 //oMSISDN;	//type currently unknown
	}t_LCSinformation;
	
   ////////////////////////////PoC-Information /////////////////
	typedef struct PoCUserRole
	 {
		t_DiamUTF8String	oPoCUserRoleIds;
		t_DiamEnum			oPoCUserRoleInfoUnits;
	 }t_PoCUserRole;

	 typedef struct ParticipantGroup
	 {
		t_DiamUTF8String    oCalledPartyAddress;
		t_DiamEnum			OParticipantAccessPriority; 
		t_DiamEnum			OUserParticipatingType;
	 }t_ParticipantGroup;

	 typedef struct ParticipantGroupList
	 {
		int                    	nNumberOfItemsInList;
		t_ParticipantGroup    	oItems[OFFLINE_CS_LIST_SIZE];
	 }t_ParticipantGroupList;

	typedef struct TalkBurstExchange
	{
        t_DiamTime			oPoCChangeTime;
		t_DiamUnsigned32	oNumberOfTalkBursts;
		t_DiamUnsigned32	oTalkBurstVolume;
		t_DiamUnsigned32	oTalkBurstTime;
		t_DiamUnsigned32	oNumberOfReceivedTalkBursts;
		t_DiamUnsigned32	oReceivedTalkBurstVolume;
		t_DiamUnsigned32	oReceivedTalkBurstTime;
		t_DiamUnsigned32	oNumberOfParticipants;
		t_DiamTime			oPoCChangeCondition;
	}t_TalkBurstExchange;

	typedef struct TalkBurstExchangeList
	 {
		int                    	nNumberOfItemsInList;
		t_TalkBurstExchange    	oItems[OFFLINE_CS_LIST_SIZE];
	 }t_TalkBurstExchangeList;

   typedef struct PoCInformation
	 {
		t_DiamEnum					oPoCServerRole;
		t_DiamEnum					oPoCSessionType;
		t_PoCUserRole				oPoCUserRole;
		t_DiamEnum					oPoCSessionInitiationtype;
		t_DiamEnum					oPoCEventType;
		t_DiamUnsigned32			oNumberOfParticipants;
        t_UTF8StringList			oListOfParticipantsInvolved;
		t_ParticipantGroupList  	oListOfParticipantGroup;
        t_TalkBurstExchangeList  	oListOfTalkBurstExchange;
		t_DiamUTF8String			oPoCControllingAddress; 
		t_DiamUTF8String			oPoCGroupName;
		t_DiamUTF8String			oPoCSessionId; 
		t_DiamUTF8String			oChargedParty;
	 }t_PoCInformation;

	 /////////////////////////////////////MBMS-Information/////////////
     typedef struct MBMSInformation
	 {
			/*oTMGI;
			oMBMSServiceType;
			oMBMSUserServiceType;
			oFileRepairSupported;
			oRequiredMBMSBearerCapabilities;
			oMBMS2G3GIndicator;
			oRAI;
            t_ListOfMBMSServiceArea;
			oMBMSSessionIdentity;*/
	 }t_MBMSInformation;
	 
	 /////////////////////////////////////////////////////////////////////////////////

	 typedef struct DestinationInterface
	 {
		 t_DiamUTF8String	oInterfaceId;
		 t_DiamUTF8String	oInterfaceText;
		 t_DiamUTF8String	oInterfacePort;
		 t_DiamUTF8String	oInterfaceType;
	 }t_DestinationInterface;

	 typedef struct originatorInterface
	 {
		 t_DiamUTF8String	oInterfaceId;
		 t_DiamUTF8String	oInterfaceText;
		 t_DiamUTF8String	oInterfacePort;
		 t_DiamUTF8String	oInterfaceType;
	 }t_originatorInterface;
     
	 typedef struct SMSInformation
	 {
         t_DiamEnum					oSMSNode;
         t_DiamIPAddress			oClientAddress;
		 t_DiamIPAddress			oOriginatorSCCPAddress;
		 t_DiamIPAddress			oRecipientSCCPAddress;
		 t_DiamIPAddress			oSMSCAddress;
		 t_DiamInteger32			oDataCodingScheme;
		 t_DestinationInterface  	oDestinationInterface;
		 t_DiamTime					oSMDischargeTime;
		 t_DiamTime					oSMMessageType;
		 t_originatorInterface		oOriginatorInterface;
		 t_DiamOctetString			oSMProtocolID;
		 t_DiamEnum					oReplyPathRequested;
		 t_DiamTime					oSMStatus;
		 t_DiamOctetString			oSMUserDataHeader;
		 t_DiamUnsigned32			oNumberOfMessagesSent;
	 }t_SMSInformation;

   

 typedef struct Event_Type
 {
     unsigned char			ucSipMethod[OFFLINE_CS_MEDIUM_STRING_SIZE];
     unsigned char		 	ucEvent[OFFLINE_CS_MEDIUM_STRING_SIZE]; 
     unsigned int        	unExpires;
 }t_Event_Type;

 typedef struct TimeStamps
 {
     unsigned int       unSipRequestTimeStamp;
     unsigned int       unSipResponseTimeStamp;
 }t_TimeStamps;

 typedef struct AppServerInformation
 {
     unsigned char      ucAppServer[OFFLINE_CS_MEDIUM_STRING_SIZE];
     t_UTF8StringList   oAppProvidedCalledPartyAddressList;
 }t_AppServerInformation;

 typedef struct AppServerInfoList
 {
     int                       nNumberOfitemsInList;
     t_AppServerInformation    oItems[OFFLINE_CS_LIST_SIZE];
 }t_AppServerInfoList;

 typedef struct InterOperatorIdentifier
 {
     unsigned char     ucOriginatingIOI[OFFLINE_CS_MEDIUM_STRING_SIZE];
     unsigned char     ucTerminatingIOI[OFFLINE_CS_MEDIUM_STRING_SIZE];
 }t_InterOperatorIdentifier;

 typedef struct InterOperatorIdentifierList
 {
     int                          nNumberOfitemsInList;
     t_InterOperatorIdentifier    oItems[OFFLINE_CS_LIST_SIZE];
 }t_InterOperatorIdentifierList;

 typedef struct _MessageBody
 {
    unsigned char    	ucContentType[OFFLINE_CS_LARGE_STRING_SIZE]; 
    unsigned int        nContentLength;
    unsigned char       ucContentDisposition[OFFLINE_CS_LARGE_STRING_SIZE];
    int          		nOriginator;
 }t_MessageBody;

 typedef struct MessageBodyList
 {
     int                          nNumberOfitemsInList;
     t_MessageBody                oItems[OFFLINE_CS_LIST_SIZE];
 }t_MessageBodyList;


 typedef struct SDPMediaComponents
 {
    unsigned char	    ucSDPMediaName[OFFLINE_CS_LARGE_STRING_SIZE];
    t_UTF8StringList    oSDPMediaDescriptionList;
    int          		nMediaInitiatorFlag;
    unsigned char       ucMediaInitiatorParty[OFFLINE_CS_LARGE_STRING_SIZE];
    unsigned char       ucAuthorizedQoS[OFFLINE_CS_LARGE_STRING_SIZE];
    unsigned char       uc3GPPChargingId[OFFLINE_CS_LARGE_STRING_SIZE];
    unsigned char       ucAccessNetChargingIdValue[OFFLINE_CS_LARGE_STRING_SIZE];
    
 }t_SDPMediaComponents;

 typedef struct SDPMediaComponentList
 {
     int                          nNumberOfitemsInList;
     t_SDPMediaComponents         oItems[OFFLINE_CS_LIST_SIZE];
 }t_SDPMediaComponentList;

	typedef struct IMSInformation
	{

		t_Event_Type                   	oEventType;
		int                     	 	nRoleOfNode;
		int                     		nNodeFunctionality;
		unsigned char	              	ucNodeAddress[OFFLINE_CS_ADDRESS_STRING_SIZE];
		unsigned char               	ucUserSessionId[OFFLINE_CS_MEDIUM_STRING_SIZE];
		t_UTF8StringList               	oCallingPartyAddressList;
		unsigned char                 	ucCalledPartyAddress[OFFLINE_CS_ADDRESS_STRING_SIZE];
		unsigned char                 	ucUserName[OFFLINE_CS_LARGE_STRING_SIZE];
		t_UTF8StringList               	oCalledAssertedIdentityList;
		unsigned char                 	ucNumberPortabilityRoutingInfo[OFFLINE_CS_LARGE_STRING_SIZE];
		t_TimeStamps                   	oTimeStamps;
		unsigned char                 	ucRequestedPartyAddress[OFFLINE_CS_ADDRESS_STRING_SIZE];
		unsigned char                 	ucCarrierSelectRoutingInformation[OFFLINE_CS_LARGE_STRING_SIZE];
		t_UTF8StringList               	oAssociatedURIList;
		unsigned char                 	ucAlternateChargedPartyAddress[OFFLINE_CS_ADDRESS_STRING_SIZE];
		unsigned char                 	ucIMSChargingIdentifier[OFFLINE_CS_MEDIUM_STRING_SIZE];
		unsigned char                 	ucServedPartyIPAddress[OFFLINE_CS_ADDRESS_STRING_SIZE];
		unsigned char                 	ucBearerService[OFFLINE_CS_MEDIUM_STRING_SIZE];
		unsigned char                 	ucServiceId[OFFLINE_CS_MEDIUM_STRING_SIZE];
		int                			   	nCausedCode;
		unsigned char                 	ucAccessNetworkInformation[OFFLINE_CS_MEDIUM_STRING_SIZE];
		unsigned char                 	ucIMSCommunicationServiceIdentifier[OFFLINE_CS_MEDIUM_STRING_SIZE];
		t_AppServerInfoList            	oAppServerInfoList;
		t_UTF8StringList               	oSDPSessionDescriptionList;
		unsigned char                 	ucServedPartyAddress[OFFLINE_CS_ADDRESS_STRING_SIZE];
		t_MessageBodyList              	oMessageBodyList;
		unsigned char					ucServiceContextId[OFFLINE_CS_MEDIUM_STRING_SIZE];
		t_ServiceSpecificInfoList       oServiceSpecificInfoList;
		t_SDPMediaComponentList        	oSDPMediaComponentsList;
		t_InterOperatorIdentifierList  	oInterOperatorIdentifierList;
	}t_IMSInformation;

	typedef struct SubscriptionId
	{
	 	int					nSubscriptionIdType;
		unsigned char		ucSubscriptionIdData[OFFLINE_CS_SMALL_STRING_SIZE];
    }t_SubscriptionId;

	typedef struct ServiceInformation
	{
		t_SubscriptionId		oSubscriptionId;
		t_PSInformation			oPSInformation;
		t_WLANInformation		oWLANInformation;
		t_IMSInformation		oIMSInformation;
		t_MMSInformation		oMMSInformation;
		t_LCSinformation		oLCSInformation;
		t_PoCInformation		oPoCInformation;
		t_MBMSInformation		oMBMSInformation;
		t_SMSInformation		oSMSInformation;
	}t_ServiceInformation;


 typedef struct CDR_Contents
 {
     unsigned char					ucSessionId[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 unsigned char					ucOriginHost[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 unsigned char					ucOriginRealm[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 unsigned char					ucDestinationRealm[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 int							nAcctRecordType;
	 unsigned int					unAcctRecordNumber;
	 unsigned int					unAcctApplicationId;
	 unsigned char					ucUserName[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 unsigned int					unAcctInterimInterval;
	 unsigned int					unOriginStateId;
	 unsigned int					unEventTimeStamp;
	 unsigned char					ucServiceContextId[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 _Bool							bRetransmission;
	 t_ServiceInformation			oServiceInfo;
	 int							nLocalRecordSequenceNumber; // extracted filed 
	 int							nRecordSequenceNumber; 		// extracted filed 

   }t_CDR_Contents;

   typedef t_CDR_Contents	t_MessageAVPsStructure;


   typedef struct MessageHeaderFieldStruct
   {
	   unsigned char		    ucFlags;
	   unsigned int            	unHopByHopID;
	   unsigned int            	unEndToEndID;
   }t_MessageHeaderFieldStruct;

   typedef struct MessageStructure
   {
	   t_MessageHeaderFieldStruct	oMsgHrdFieldsStruct;
	   t_MessageAVPsStructure		oMsgAVPsStruct;

   }t_MessageStructure;



   /*////////////////////////////////////////////////*/
   // data structure for encoded cdrs 

   	typedef struct Tag
	{
		unsigned char ucclass:2;
		unsigned char ucP_C:1;
		unsigned char ucTagNumber:5;
    }t_Tag;

	typedef struct EncodedDataHeader
	{
		t_Tag oTag;
		unsigned char ucLength;
    }t_EncodedDataHeader;

	typedef struct Release_VersionId
	{
		unsigned char VersionIdentifier:5;
		unsigned char ReleasIdentifier:3;
    }t_Release_VersionId;

	typedef struct DataRecordFormat_TSNumber
	{
		unsigned char TSNumber:5;
		unsigned char DataRcordFormat:3;
    }t_DataRecordFormat_TSNumber;

    
	typedef struct CDRHeader
	{
		unsigned short 					usCdrLength;
		t_Release_VersionId				oRelease_VersionId;
		t_DataRecordFormat_TSNumber		oDataRecordFormat_TSNumber;
	}t_CDRHeader;

	typedef struct CdrData
	{
		unsigned char ucBuff[OFFLINE_CS_ENCODED_CDR_BUF_SIZE_IN_BYTS];
		unsigned int unsize;
	}t_CdrData;


	typedef struct EncodedCDR
	{
		t_CDRHeader	oCdrHeader;
		t_CdrData	oCdrData;
		
	}t_EncodedCDR;
   

	typedef struct DataFromCGFToFileWriter
	{
		t_EncodedCDR 	oEncodedCDR;
		unsigned int	unAcctRecordNumber;
		unsigned char   ucSessionId[OFFLINE_CS_MEDIUM_STRING_SIZE];
	}t_DataFromCGFToFileWriter;

    
   #ifdef	__cplusplus
}
#endif

#endif	/* _OFFCS_COMM_CDR_CONTENTS_H */

