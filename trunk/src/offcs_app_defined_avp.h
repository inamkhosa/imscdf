
/******************************************************************************
** Written by Simab Shahid
**
** Copyright (C) 2009 AdvancedIMS
**
** All rights reserved.
** 
*******************************************************************************
** FILE NAME      		: offcs_app_defined_avp.h
** DESCRIPTION    		: The file contains AVP defines for Offline Charging Server.
** PROJECT NAME   		: 
** TYPE           		: ANSI C
** CONTRIBUTOR(S) 		: Simab Shahid
*******************************************************************************
** $Id: AMPS_Defines.h,v 1.30 2008/04/01 12:40:39 zaka Exp $
** $Source: /work/cvsroot/amps/include/AMPS_Defines.h,v $
*******************************************************************************
*******************************************************************************
*/

#ifndef _OFFCS_APP_DEFINED_AVP_H
#define	_OFFCS_APP_DEFINED_AVP_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include"RF_defines.h"
	#include"RF_include.h"

	// 3GPP Defined AVPs 

    #define OFFLINE_CS_ACCESS_NETWORK_CHARGING_IDENTIFIER_VALUE     503 //OctectString 
    #define OFFLINE_CS_CALLED_PARTY_ADDRESS                         832 //UTF8String
    #define OFFLINE_CS_CALLING_PARTY_ADDRESS                        831 //UTF8String
    #define OFFLINE_CS_APPLICATION_SERVER_INFORMATION               850 //Grouped
    #define OFFLINE_CS_APPLICATION_SERVER                           836 //UTF8
    #define OFFLINE_CS_APPLICATION_PROVIDED_CALLED_PARTY_ADDRESS    837 //UTF8 
    #define OFFLINE_CS_AUTHORISED_QOS                               849 //UTF8
    #define OFFLINE_CS_BEARER_SERVICE                               854 //UTF8
    #define OFFLINE_CS_CONTENT_TYPE                                 826 //UTF8
    #define OFFLINE_CS_CONTENT_DISPOSITION                          828 //UTF8
    #define OFFLINE_CS_CONTENT_LENGTH                               827 //UTF8
    #define OFFLINE_CS_GGSN_ADDRESS                                 847 //ADDRESS
    #define OFFLINE_CS_PDG_CHARGING_ID                              896 //UNSIGNED32
    #define OFFLINE_CS_IMS_CHARGING_IDENTIFIER                      841 //UTF8
    #define OFFLINE_CS_INTER_OPERATOR_IDENTIFIER                    838 //GROUPED
    #define OFFLINE_CS_ORIGINATING_IOI                              839 //UFT8
    #define OFFLINE_CS_TERMINATING_IOI                              840 //UTF8
    #define OFFLINE_CS_MEDIA_INITIATOR_FLAG                         882 //ENUM
    #define OFFLINE_CS_SDP_SESSION_DESCRIPTION                      842 //UTF8
    #define OFFLINE_CS_MESSAGE_BODY                                 889 //GROUPED
    #define OFFLINE_CS_ORIGINATOR                                   864 //ENUM
    #define OFFLINE_CS_SDP_MEDIA_COMPONENT                          843 //GROUPED
    #define OFFLINE_CS_SIP_REQUEST_TIME_STAMP                       834 //TIME
    #define OFFLINE_CS_SIP_RESPONSE_TIME_STAMP                      835 //TIME
    #define OFFLINE_CS_NODE_FUNCTIONALITY                           862 //ENUM
    #define OFFLINE_CS_ROLE_OF_NODE                                 829 //ENUM
    #define OFFLINE_CS_EVENT                                        825 //UTF8String
    #define OFFLINE_CS_SDP_MEDIA_NAME                               844 //UTF8
    #define OFFLINE_CS_SDP_MEDIA_DESCRIPTION                        845 //UTF8
    #define OFFLINE_CS_CAUSE_CODE                                   861 //INTEGER32
    #define OFFLINE_CS_SERVICE_ID                                   855 //UTF8
    #define OFFLINE_CS_SERVED_PARTY_ADDRESS                         848 //ADDRESS
    #define OFFLINE_CS_SIP_METHOD                                   824 // UTF8
    #define OFFLINE_CS_SERVER_CAPABILITIES                          603 // 3GPP TS 29.229: "Cx and Dx Interfaces based on the Diameter protocol; Protocol Details".
    #define OFFLINE_CS_IMS_INFORMATION                              876 // Grouped
    #define OFFLINE_CS_EVENT_TYPE                                   823 // Grouped
    #define OFFLINE_CS_TIME_STAMPS                                  833 // Grouped
    #define OFFLINE_CS_SERVICE_CONTEXT_ID                           461 // UTF8
    #define OFFLINE_CS_EXPIRES                                      888 // UNSIGNED32
    #define OFFLINE_CS_ASSOCIATED_URI                               856 // UTF8String
    #define OFFLINE_CS_USER_SESSION_ID                              830 // UTF8
	#define OFFLINE_CS_PS_INFORMATION								874 // Grouped
    #define OFFLINE_CS_3GPP_CHARGING_ID                             2 	// octetString
	#define OFFLINE_CS_3GPP_PDP_TYPE                             	3 	// 
	#define OFFLINE_CS_PDP_ADDRESS	                             	1227// Address of type octetString
	#define OFFLINE_CS_3GPP_GPRS_NEGOTIATED_QOS_PROFILE             5  // octetString
	#define OFFLINE_CS_SGSN_ADDRESS			                        1228 //Address of type octetString
	#define OFFLINE_CS_CG_ADDRESS                             	 	846//Address of type octetString
	#define OFFLINE_CS_3GPP_IMSI_MCC_MNC                            8 //UNKNOWN TYPE WILL SEE IN 207
	#define OFFLINE_CS_3GPP_GGSN_MCC_MNC                            9 //octetString
	#define OFFLINE_CS_3GPP_NSAPI                              	 	10//octetString
	#define OFFLINE_CS_CALLED_STATION_ID                            30//UTF8String
	#define OFFLINE_CS_3GPP_SESSION_STOP_INDICATOR                  11//octetString
	#define OFFLINE_CS_3GPP_SELECTION_MODE                          12//octetString
	#define OFFLINE_CS_3GPP_CHARGING_CHARACTERISTICS                13//octetString
	#define OFFLINE_CS_3GPP_SGSN_MCC_MNC                            18//octetString
	#define OFFLINE_CS_3GPP_MS_TIMEZONE                             23//octetString
	#define OFFLINE_CS_CHARGING_RULE_BASE_NAME                      1004//octetString, MAY OCCUR IN LIST
	#define OFFLINE_CS_3GPP_USER_LOCATION_INFO                      22//SEE 207 FOR TYPE 
	#define OFFLINE_CS_3GPP_RAT_TYPE                             	21//octetString
	#define OFFLINE_CS_PS_FURNISH_CHARGING_INFORMATION            	865//GROUPED
	#define OFFLINE_CS_PS_FREE_FORMAT_DATA							866//OctetString
	#define OFFLINE_CS_PS_APPEND_FREE_FORMAT_DATA					867//enumerated 
	#define OFFLINE_CS_PDP_CONTEXT_TYPE 							1247//ENUM
	#define OFFLINE_CS_OFFLINE_CHARGING								1278//GROUPED
	#define OFFLINE_CS_QUOTA_CONSUMPTION_TIME						881 //Unsigned32
	#define OFFLINE_CS_TIME_QUOTA_MECHANISM							1270//Grouped
	#define OFFLINE_CS_TIME_QUOTA_TYPE								1271//Enumerated
	#define OFFLINE_CS_BASE_TIME_INTERVAL							1265//Unsigned32.
    #define OFFLINE_CS_MULTIPLE_SERVICES_CREDIT_CONTROL         	456//Grouped
	#define OFFLINE_CS_GRANTED_SERVICE_UNIT							431// Grouped not present in 299 doc ,WILL LOOK TO IT AVPS LATER
	#define OFFLINE_CS_REQUESTED_SERVICE_UNIT						437//Grouped not present in 299 doc ,WILL LOOK TO IT AVPS LATER
	#define OFFLINE_CS_USED_SERVICE_UNIT							446//Grouped, OffLine_cahrging AVP may contains its List
    #define OFFLINE_CS_REPORTING_REASON								872//Enumerated 
	#define OFFLINE_CS_TARIFF_CHANGE_USAGE							452//Enumerated
	#define OFFLINE_CS_CC_TIME                                  	420//Unsigned32
	#define OFFLINE_CS_CC_TOTAL_OCTETS								421//Unsigned64
	#define OFFLINE_CS_CC_INPUT_OCTETS  							412//Unsigned64
	#define OFFLINE_CS_CC_OUTPUT_OCTETS								414//Unsigned64
	#define OFFLINE_CS_CC_SERVICE_SPECIFIC_UNITS                	417//Unsigned64
	#define OFFLINE_CS_EVENT_CHARGING_TIMESTAMP                 	1258//Time, may contain list
    #define OFFLINE_CS_SERVICE_IDENTIFIER							439//Unsigned32,MAY CONTAIN LIST
	#define OFFLINE_CS_RATING_GROUP									432//Unsigned32									
	#define OFFLINE_CS_G_S_U_POOL_REFERENCE							457//GROUPED,MAY OCCUR IN LIST
    #define OFFLINE_CS_G_S_U_POOL_IDENTIFIER						453//Unsigned32 
	#define OFFLINE_CS_CC_UNIT_TYPE 								454//Enumerated
	#define OFFLINE_CS_UNIT_VALUE									445 //Grouped
    #define OFFLINE_CS_VALUE_DIGIT									447//Integer64
	#define OFFLINE_CS_EXPONENT										429//Integer32
    #define OFFLINE_CS_VALIDITY_TIME								448//Unsigned32
	#define OFFLINE_CS_FINAL_UNIT_INDICATION						430	//GROUPED
    #define OFFLINE_CS_FINAL_UNIT_ACTION							449//Enumerated
	#define OFFLINE_CS_RESTRICTION_FILTER_RULE						438 //IPFilterRule
	#define OFFLINE_CS_FILTER_ID									11//UTF8String
	#define OFFLINE_CS_REDIRECT_SERVER								434//Grouped
    #define OFFLINE_CS_REDIRECT_ADDRESS_TYPE						433//Enumerated
	#define OFFLINE_CS_REDIRECT_SERVER_ADDRESS						435//UTF8String
    #define OFFLINE_CS_TIME_QUOTA_THRESHOLD							868//Unsigned32
	#define OFFLINE_CS_VOLUME_QUOTA_THRESHOLD						869//Unsigned32
	#define OFFLINE_CS_UNIT_QUOTA_THRESHOLD							1226//Unsigned32 
	#define OFFLINE_CS_QUOTA_HOLDING_TIME							871//Unsigned32 
	#define OFFLINE_CS_REPORTING_REASON								872	// Enum, MAY OCCUR IN LIST
	#define OFFLINE_CS_TRIGGER										1264//Grouped
    #define OFFLINE_CS_TRIGGER_TYPE									870//ENUM, MAY OCCUR IN LIST
    #define OFFLINE_CS_PS_FURNISH_CHARGING_INFORMATION            	865//GROUPED
    #define OFFLINE_CS_PS_FREE_FORMAT_DATA							866//OctetString
	#define OFFLINE_CS_PS_APPEND_FREE_FORMAT_DATA					867//enumerated 
    #define OFFLINE_CS_REFUND_INFORMATION							1292//OctetString 
	#define OFFLINE_CS_AF_CORRELATION_INFORMATION					1276//Grouped ,MAY OCCUR IN LIST
    #define OFFLINE_CS_AF_CHARGING_IDENTIFIER						505//type will check from 214
	#define OFFLINE_CS_FLOWS										510//MAY OCCUR IN LIST,type will check from 214
    #define OFFLINE_CS_ENVELOP										1266// Grouped MAY OCCUR IN LIST
    #define OFFLINE_CS_ENVELOP_START_TIME							1269 //Time
	#define OFFLINE_CS_ENVELOP_END_TIME								1267 //Time
	#define OFFLINE_CS_CC_TIME                                		420//Unsigned32
	#define OFFLINE_CS_CC_TOTAL_OCTETS								421//Unsigned64
	#define OFFLINE_CS_CC_INPUT_OCTETS  							412//Unsigned64
	#define OFFLINE_CS_CC_OUTPUT_OCTETS								414//Unsigned64
	#define OFFLINE_CS_CC_SERVICE_SPECIFIC_UNITS                	417//Unsigned64*/
    #define OFFLINE_CS_ENVELOP_REPORTING 							1268//Enumerated 
	#define OFFLINE_CS_TIME_QUOTA_MECHANISM							1270// Grouped
    #define OFFLINE_CS_TIME_QUOTA_TYPE								1271//Enumerated
	#define OFFLINE_CS_BASE_TIME_INTERVAL							1265//Unsigned32.
    #define OFFLINE_CS_SERVICE_SPECIFIC_INFO						1249//Grouped,MAY OCCUR IN LIST
    #define OFFLINE_CS_SERVICE_SPECIFIC_TYPE 						1257//Unsigned32 
	#define OFFLINE_CS_SERVICE_SPECIFIC_DATA						863//UTF8STRING
	#define OFFLINE_CS_WLAN_INFORMATION								875 //Grouped.
    #define OFFLINE_CS_WLAN_SESSION_ID								1246//Unsigned32 
	#define OFFLINE_CS_PDG_ADDRESS 									895	//OCTETSTRING OF  type Address
	#define OFFLINE_CS_PDG_CHARGING_ID								896 // Unsigned32
	#define OFFLINE_CS_WAG_ADDRESS									890//OCTETSTRING of type Address
	#define OFFLINE_CS_WAG_PLMN_ID          						891//OctetString 
	#define OFFLINE_CS_WLAN_RADIO_CONTAINER							892//Grouped.
    #define OFFLINE_CS_OPERATOR_NAME								//FOR CODE AND TYPE REFER 204 DOC OF 299
	#define OFFLINE_CS_LOCATION_TYPE								1244 //Grouped 
    #define OFFLINE_CS_LOCATION_ESTIMATE_TYPE						1243//Enumerated 	 
	#define OFFLINE_CS_DEFERRED_LOCATION_EVENT_TYPE					1230//UTF8String
    #define OFFLINE_CS_LOCATION_INFORMATION							// FOR CODE AND TYPE SEE 403 DOC OF 299
	#define OFFLINE_CS_WLAN_TECHNOLOGY					    		893//Unsigned32.
    #define OFFLINE_CS_WLAN_UE_LOCAL_IP_ADDRESS     				894 //OCTETSTRING OF TYPE Address
	#define OFFLINE_CS_MMS_INFORMATION								877 // GROUPED
    #define OFFLINE_CS_ORIGINATOR_ADDRESS 							886//Grouped.
    #define OFFLINE_CS_ADDRESS_TYPE									899//Enumerated 
	#define OFFLINE_CS_ADDRESS_DATA									897//UTF8String
	#define OFFLINE_CS_ADDRESS_DOMAIN								898//Grouped 
    #define OFFLINE_CS_DOMAIN_NAME									1200//UTF8String
	#define  OFFLINE_CS_3GPP_IMSI_MCC_MNC       					8 //UNKNOWN TYPE WILL SEE IN 207
    #define OFFLINE_CS_RECIPIENT_ADDRESS        					1201//Grouped.
    #define OFFLINE_CS_ADDRESS_TYPE									899//Enumerated 
	#define OFFLINE_CS_ADDRESS_DATA									897//UTF8String
	#define OFFLINE_CS_ADDRESS_DOMAIN								898//Grouped 
    #define OFFLINE_CS_DOMAIN_NAME									1200//UTF8String
	#define  OFFLINE_CS_3GPP_IMSI_MCC_MNC       					8//UNKNOWN TYPE WILL SEE IN 207
    #define OFFLINE_CS_ADDRESSEE_TYPE								1208//Enumerated
    #define OFFLINE_CS_SUBMISSION_TIME								1202// TIME
	#define OFFLINE_CS_MM_CONTENT_TYPE								1203//Grouped 
    #define OFFLINE_CS_TYPE_NUMBER									1204//Enumerated 
	#define OFFLINE_CS_ADDITIONAL_TYPE_INFORMATION					1205//UTF8String
	#define OFFLINE_CS_CONTENT_SIZE									1206//Unsigned32
	#define OFFLINE_CS_ADDITIONAL_CONTENT_INFORMATION				1207// Grouped
    #define OFFLINE_CS_TYPE_NUMBER									1204//Enumerated 
	#define OFFLINE_CS_ADDITIONAL_TYPE_INFORMATION					1205//UTF8String
	#define OFFLINE_CS_CONTENT_SIZE									1206//Unsigned32
    #define OFFLINE_CS_PRIORITY									 	1209//Enumerated 
	#define OFFLINE_CS_MESSGAE_ID								 	1210//UTF8String 
	#define OFFLINE_CS_MESSAGE_TYPE								 	1211//Enumerated
	#define OFFLINE_CS_MESSAGE_SIZE							     	1212//Unsigned32
	#define OFFLINE_CS_MESSAGE_CLASS							 	1213//Grouped.
    #define OFFLINE_CS_CLASS_IDENTIFIER						  		1214//Enumerated 
    #define OFFLINE_CS_TOKEN_TEXT 							  		1215//UTF8String
    #define OFFLINE_CS_DELIVERY_REPORT_REQUESTED		 			1216//Enumerated 
	#define OFFLINE_CS_READ_REPLY_REPORT_REQUESTED		 			1222//Enumerated 
	#define OFFLINE_CS_MM_BOX_STORAGE_REQUESTED			 			1248//Enumerated 
	#define OFFLINE_CS_APPLIC_ID						 			1218//UTF8String 
	#define OFFLINE_CS_REPLY_APPLIC_ID					 			1223//UTF8String
	#define OFFLINE_CS_AUX_APPLIC_INFO					 			1219//UTF8String 
	#define OFFLINE_CS_CONTENT_CLASS					 			1220//Enumerated 
	#define OFFLINE_CS_DRM_CONTENT						 			1221//Enumerated 
	#define OFFLINE_CS_ADAPTATIONS						 			1217//Enumerated
	#define OFFLINE_CS_VASP_ID						     			1101//for type see 213 
	#define OFFLINE_CS_VAS_ID							 			1102//for type see 213
	#define OFFLINE_CS_LCS_INFORMATION								878//Grouped.
    #define OFFLINE_CS_LCS_CLIENT_ID	    						1232//Grouped 
    #define OFFLINE_CS_LCS_CLIENT_TYPE								1241//Enumerated
	#define OFFLINE_CS_LCS_CLIENT_EXTERNAL_ID						1234//UTF8String 
	#define OFFLINE_CS_LCS_CLIENT_DIALED_BY_MS						1233//UTF8String 
	#define OFFLINE_CS_LCS_CLIENT_NAME								1235//Grouped
    #define OFFLINE_CS_LCS_DATA_CODING_SCHEME						1236//UTF8String 
	#define OFFLINE_CS_LCS_NAME_STRING								1238//UTF8String 
	#define OFFLINE_CS_LCS_FORMAT_INDICATOR							1237//Enumerated 
    #define OFFLINE_CS_LCS_APN										1231//UTF8String 
	#define OFFLINE_CS_LCS_REQUESTOR_ID								1239//Grouped 
    #define OFFLINE_CS_LCS_DATA_CODING_SCHEME						1236//UTF8String 
	#define OFFLINE_CS_LCS_REQUESTOR_ID_STRING						1240//UTF8String 
    #define OFFLINE_CS_LOCATION_TYPE								1244 //Grouped 
    #define OFFLINE_CS_LOCATION_ESTIMATE_TYPE						1243//Enumerated 	 
	#define OFFLINE_CS_DEFERRED_LOCATION_EVENT_TYPE					1230//UTF8String
    #define OFFLINE_CS_LOCATION_ESTIMATE							1242//UTF8String 
	#define OFFLINE_CS_POSITIONING_DATA								1245//UTF8String 
	//#define OFFLINE_CS_IMSI     									not found
	//#define OFFLINE_CS_MSISDN										not found
	#define OFFLINE_CS_POC_INFORMATION								879//Grouped.
    #define OFFLINE_CS_POC_SERVER_ROLE								883//Enumerated 
	#define OFFLINE_CS_POC_SESSION_TYPE								884//Enumerated 
	#define OFFLINE_CS_POC_USER_ROLE								1252//Grouped.
    #define OFFLINE_CS_POC_USER_ROLE_ID								1253//UTF8String
	#define OFFLINE_CS_POC_USER_ROLE_INFO_UNIT						1254//Enumerated 
    #define OFFLINE_CS_POC_SESSION_INITIATION_TYPE					1277//Enumerated
	#define OFFLINE_CS_POC_EVENT_TYPE								2025//Enumerated 
	#define OFFLINE_CS_NUMBER_OF_PARTICIPANTS						885//Unsigned32 
	#define OFFLINE_CS_PARTICIPANTS_INVOLVED						887//UTF8String 
	#define OFFLINE_CS_PARTICIPANT_GROUP							1260//Grouped 
    //#define OFFLINE_CS_CALLED_PARTY_ADDRESS
	#define OFFLINE_CS_PARTICIPANT_ACCESS_PRIORITY					1259//Enumerated.
	#define OFFLINE_CS_USER_PARTICIPANTING_TYPE						1279//Enumerated
    #define OFFLINE_CS_TALK_BURST_EXCHANGE							1255//Grouped 
    #define OFFLINE_CS_POC_CHANGE_TIME								1262//Time 
	#define OFFLINE_CS_NUMBER_OF_TALK_BURSTS						1283//Unsigned32 
	#define OFFLINE_CS_TALK_BURST_VOULME							1287//Unsigned32
	#define OFFLINE_CS_TALK_BURST_TIME								1286//Unsigned32 
	#define OFFLINE_CS_NUMBER_OF_RECEIVED_TALK_BURSTS				1282//Unsigned32 
	#define OFFLINE_CS_RECEIVED_TALK_BURST_VOLUME					1285//Unsigned32 
	#define OFFLINE_CS_RECEIVED_TALK_BURST_TIME						1284//Unsigned32
	#define OFFLINE_CS_NUMBER_OF_PARTICIPANTS						885//Unsigned32
	#define OFFLINE_CS_POC_CHANGE_CONDITION							1261//Enumerated
    #define OFFLINE_CS_POC_CONTROLLING_ADDRESS						858//UTF8String 
	#define OFFLINE_CS_POC_GROUP_NAME								859//UTF8String 
	#define OFFLINE_CS_POC_SESSION_ID								1229//UTF8String.
	#define OFFLINE_CS_CHARGED_PARTY								857//UTF8String
	#define OFFLINE_CS_MBMS_INFORMATION								880//Grouped.
    #define OFFLINE_CS_TMGI											900//FOR TYPE SEE 207
	#define OFFLINE_CS_MBMS_SERVICE_TYPE   							906// FOR TYPE [207]
	#define OFFLINE_CS_MBMS_USER_SERVICE_TYPE						1225//Enumerated 
	#define OFFLINE_CS_FILE_REPAIRE_SUPPORTED						1224//Enumerated 
	#define OFFLINE_CS_REQUIRED_MBMS_BEARER_CAPABILITIES			901//FOR TYPE [207]
	#define OFFLINE_CS_MBMS_2G_3G_INDICATOR							907//FOR TYPE [207]
	#define OFFLINE_CS_RAI											909// FOR TYPE refer [207]
	#define OFFLINE_CS_MBMS_SERVICE_AREA							903//FOR TYPE [207]
	#define OFFLINE_CS_MBMS_SESSION_IDENTITY						908//FOR TYPE [207]
	#define OFFLINE_CS_SMS_INFORMATION 								2000//Grouped.
    #define OFFLINE_CS_SMS_NODE										2016//Enumerated 
	#define OFFLINE_CS_CLIENT_ADDRESS								2018//Address 			
	#define OFFLINE_CS_ORIGINATOR_SCCP_ADDRESS						2008//Address
	#define OFFLINE_CS_RECIPIENT_SCCP_ADDRESS						2010//Address.
	#define OFFLINE_CS_SMSC_ADDRESS									2017//Address 
	#define OFFLINE_CS_DATA_CODING_SCHEME							2001//Integer 32 
	#define OFFLINE_CS_DESTINATION_INTERFACE						2002//Grouped,
    #define OFFLINE_CS_INTERFACE_ID									2003//UTF8String 
	#define OFFLINE_CS_INTERFACE_TEXT								2005//UTF8String
	#define OFFLINE_CS_INTERFACE_PORT								2004//UTF8String 
	#define OFFLINE_CS_INTERFACE_TYPE								2006//Enumerated 
    #define OFFLINE_CS_SM_DISCHARGE_TIME							2012//Time.
	#define OFFLINE_CS_SM_MESSAGE_TYPE								2007//Enumerated 
	#define OFFLINE_CS_ORIGINATOR_INTERFACE							2009//group
    #define OFFLINE_CS_INTERFACE_ID									2003//UTF8String 
	#define OFFLINE_CS_INTERFACE_TEXT								2005//UTF8String
	#define OFFLINE_CS_INTERFACE_PORT								2004//UTF8String 
	#define OFFLINE_CS_INTERFACE_TYPE								2006//Enumerated 
    #define OFFLINE_CS_SM_PROTOCOL_ID								2013//Octet String 
	#define OFFLINE_CS_REPLY_PATH_REQUESTED							2011//Enumerated
	#define OFFLINE_CS_SM_STATUS									2014//OctetString 
	#define OFFLINE_CS_SM_USER_DATA_HEADER							2015//OctetString 
	#define OFFLINE_CS_NUMBER_OF_MESSAGES_SENT						2019//Unsigned32
	#define OFFLINE_CS_CALLED_ASSERTED_IDENTITY                     1250 // UTF8
    #define OFFLINE_CS_NUMBER_PORTABILITY_ROUTING_INFORMATION       2024 //UTF8
    #define OFFLINE_CS_CARRIER_SELECT_ROUTING_INFORMATION           2023 //UTF8
    #define OFFLINE_CS_ALTERNATE_CHARGED_PARTY_ADDRESS              1280 // UTF8
    #define OFFLINE_CS_REQUESTED_PARTY_ADDRESS                      1251 // UTF8String
    #define OFFLINE_CS_ACCESS_NETWORK_INFORMATION                   1263 // Octect String
    #define OFFLINE_CS_SDP_OFFER_TIME_STAMP                         1274 //TIME
    #define OFFLINE_CS_SDP_ANSWER_TIME_STAMP                        1275 // TIME
    #define OFFLINE_CS_IMS_COMMUNICATION_SERVICE_IDENTIFIER         508 //1281 // UTF8
    #define OFFLINE_CS_MEDIA_INITIATOR_PARTY                        1288 //UTF8
    #define OFFLINE_CS_SUBSCRIPTION_ID                              443 //GROUPED
    #define OFFLINE_CS_SUBSCRIPTION_ID_TYPE                         450 //ENUM
    #define OFFLINE_CS_SUBSCRIPTION_ID_DATA                         444 //UTF8
    #define OFFLINE_CS_AVP_SERVICE_CONTEXT_ID                      	461 //UTF8
    #define OFFLINE_CS_AVP_SERVICE_INFORMATION                     	873 //Grouped
	#define OFFLINE_CS_MBMS_INFORMATION                     		880 //Grouped																		
	#define OFFLINE_CS_SERVICE_GENERIC_INFORMATION					1256 //Grouped		   
	#define OFFLINE_CS_MSISDN 										701// octetString





	#ifdef	__cplusplus
}
#endif

#endif	/* _OFFCS_APP_DEFINED_AVP_H */
