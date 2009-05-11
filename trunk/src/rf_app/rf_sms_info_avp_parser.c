
#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include "offcs_app_defined_avp.h"



extern 		int  							 g_nDiamBaseTraceID ;
extern 		t_AVPDictionaryElement	 		 g_oDmBase_AVPDictionary[];
extern 		HRFAPPLICATIONCONTEXT            hRFApplicationContext;


t_Offline_CS_ReturnVal RF_AddSMSInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
														unsigned int        unTotalEvtDataLengthInBytes_i,
														unsigned int*       punNewEvtDataLengthInBytes_o,
														HAVP                hToAVP_i)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == ppuncDBEvtBuff_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppuncDBEvtBuff_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == punNewEvtDataLengthInBytes_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"punNewEvtDataLengthInBytes_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}
     
       
       unsigned int    	unOldTLVBuffLength       		=  0;
       unsigned int    	unNewTLVBuffLength       		=  0;
	   unsigned int  	unAVPCode					  	=  0;
	   unsigned int     unNewEvtDataLengthInBytes     	=  0;
	   unsigned int     unDBEvtDataLengthInBytes      	=  0;
       unsigned char*   pucTempEveData    				=  NULL;
       t_Dappcommon_tlv_header* poTemp_TLV_Header     	=  NULL ;

       unOldTLVBuffLength = unTotalEvtDataLengthInBytes_i;

	   pucTempEveData =  *ppuncDBEvtBuff_o;
	   unNewEvtDataLengthInBytes = (unTotalEvtDataLengthInBytes_i + DAPPCOMMON_TLV_RECHEADER_SIZE );  
	   pucTempEveData = (unsigned char*) realloc( (unsigned char*)pucTempEveData, unNewEvtDataLengthInBytes );

	   *ppuncDBEvtBuff_o               	= pucTempEveData;
	   unDBEvtDataLengthInBytes         = unNewEvtDataLengthInBytes;
	   *punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

	   poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_SMS_INFORMATION;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 


      HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
	  _Bool                 bConvToNetOrder      = 0;
      t_DiamEnum*			poTempEnum     		 = NULL;
	  t_DiamTime*			poTempTime			 = NULL;
	  t_DiamIPAddress*		poTempAddress	 	 = NULL;
	  t_DiamInteger32*		poTempInteger32		 = NULL;
	  t_DiamUnsigned32* 	poTempUnsigned32	 = NULL;
	  t_DiamOctetString*	poTempOctetString 	 = NULL;
      DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFApplicationContext->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case  OFFLINE_CS_SMS_NODE: //2016//Enumerated
							     poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),bConvToNetOrder,
																							OFFLINE_CS_SMS_NODE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding SMS-Node data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "SMS-Node AVP Found\n");
							  break;
						case  OFFLINE_CS_CLIENT_ADDRESS: //2018//Address 
								poTempAddress = (t_DiamIPAddress*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempAddress->poIPAddr->pucIPString,poTempAddress->unNumOfOctets,
																								 OFFLINE_CS_CLIENT_ADDRESS ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Client-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Client-Address AVP Found\n");
								break;
						case  OFFLINE_CS_ORIGINATOR_SCCP_ADDRESS://2008//Address
								 poTempAddress = (t_DiamIPAddress*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								  &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								  poTempAddress->poIPAddr->pucIPString,poTempAddress->unNumOfOctets,
																								  OFFLINE_CS_ORIGINATOR_SCCP_ADDRESS ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Originator-Sccp-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Originator-Sccp-Address AVP Found\n");
							    break;
						case  OFFLINE_CS_RECIPIENT_SCCP_ADDRESS://2010//Address.
								 poTempAddress = (t_DiamIPAddress*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								  &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								  poTempAddress->poIPAddr->pucIPString,poTempAddress->unNumOfOctets,
																								  OFFLINE_CS_RECIPIENT_SCCP_ADDRESS ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Recipient-Sccp-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Recipient-Sccp-Address AVP Found\n");
								break;
						case  OFFLINE_CS_SMSC_ADDRESS:// 2017//Address
								 poTempAddress = (t_DiamIPAddress*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								  &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								  poTempAddress->poIPAddr->pucIPString,poTempAddress->unNumOfOctets,
																								  OFFLINE_CS_SMSC_ADDRESS ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding SMSC-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "SMSC-Address AVP Found, Leaving \n");
								break;
						case  OFFLINE_CS_DATA_CODING_SCHEME://2001//Integer 32 
								 poTempInteger32 = (t_DiamInteger32*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempInteger32->pnInteger32),bConvToNetOrder,
																							OFFLINE_CS_DATA_CODING_SCHEME ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Data-Coding-Scheme AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Data-Coding-Scheme AVP Found\n");
								 break;
						case  OFFLINE_CS_DESTINATION_INTERFACE://2002//Grouped,
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Destination-Interface Grouped AVP Found, Leaving \n");
								 if (Offline_CS_RetVal_Success != RF_AddDestinationInterfaceAVPDataToEventData( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																												&unNewEvtDataLengthInBytes,hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Destination-Interface AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
                                 break;
						case  OFFLINE_CS_SM_DISCHARGE_TIME: //2012//Time.
								 poTempTime = (t_DiamTime*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempTime->punUnsigned32),bConvToNetOrder,
																							OFFLINE_CS_SM_DISCHARGE_TIME ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding SM-DisCharge-Time AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "SM-Dischage-Time AVP Found\n");
							  break;
						case  OFFLINE_CS_SM_MESSAGE_TYPE://2007//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),bConvToNetOrder,
																							OFFLINE_CS_SM_MESSAGE_TYPE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding SM-Message-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "SM-Message-Type AVP Found\n");
								break;
						case  OFFLINE_CS_ORIGINATOR_INTERFACE://2009//group
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Originator-Interface Grouped AVP Found \n");
								if (Offline_CS_RetVal_Success != RF_AddOriginatorInterfaceAVPDataToEventData( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																											  &unNewEvtDataLengthInBytes,hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Origonator-Interface AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case  OFFLINE_CS_SM_PROTOCOL_ID: //2013//OctetString
								 poTempOctetString = (t_DiamOctetString*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								  unDBEvtDataLengthInBytes,
																								  &unNewEvtDataLengthInBytes,
																								  unDBEvtDataLengthInBytes,
																								  poTempOctetString->pucOctetString,
																								  poTempOctetString->unNumOfOctets,
																								  OFFLINE_CS_SM_PROTOCOL_ID ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding SM-Protocol-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "SM-Protocol AVP Found\n");
								 break;
						case  OFFLINE_CS_REPLY_PATH_REQUESTED://2011//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_REPLY_PATH_REQUESTED ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Reply-Path-Requested AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Reply-Path-Requested AVP Found\n");
							    break;
						case  OFFLINE_CS_SM_STATUS: //2014//OctetString
								 poTempOctetString = (t_DiamOctetString*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								  &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								  poTempOctetString->pucOctetString,
																								  poTempOctetString->unNumOfOctets,
																								  OFFLINE_CS_SM_STATUS))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding SM-Status AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "SM-Status AVP Found \n");
								 break;
						case OFFLINE_CS_SM_USER_DATA_HEADER://2015//OctetString  
								poTempOctetString = (t_DiamOctetString*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempOctetString->pucOctetString,
																								 poTempOctetString->unNumOfOctets,
																								 OFFLINE_CS_SM_USER_DATA_HEADER ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding SM-User-Data-Header AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "SM-User-Data-Header AVP Found \n");
								 break;
						case  OFFLINE_CS_NUMBER_OF_MESSAGES_SENT://2019//Unsigned32 
								 poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempUnsigned32->punUnsigned32),bConvToNetOrder,
																							OFFLINE_CS_NUMBER_OF_MESSAGES_SENT))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding  Number-Of-Messages-Sent AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Number-Of-Messages-Sent AVP found \n");
								break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in SMS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
	}

t_Offline_CS_ReturnVal RF_AddOriginatorInterfaceAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
																	unsigned int        unTotalEvtDataLengthInBytes_i,
																	unsigned int*       punNewEvtDataLengthInBytes_o,
																	HAVP                hToAVP_i)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == ppuncDBEvtBuff_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppuncDBEvtBuff_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == punNewEvtDataLengthInBytes_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"punNewEvtDataLengthInBytes_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}
     
       
       unsigned int    	unOldTLVBuffLength       		=  0;
       unsigned int    	unNewTLVBuffLength       		=  0;
	   unsigned int  	unAVPCode					  	=  0;
	   unsigned int     unNewEvtDataLengthInBytes     	=  0;
	   unsigned int     unDBEvtDataLengthInBytes      	=  0;
       unsigned char*   pucTempEveData    				=  NULL;
       t_Dappcommon_tlv_header* poTemp_TLV_Header     	=  NULL ;

       unOldTLVBuffLength = unTotalEvtDataLengthInBytes_i;

	   pucTempEveData =  *ppuncDBEvtBuff_o;
	   unNewEvtDataLengthInBytes = (unTotalEvtDataLengthInBytes_i + DAPPCOMMON_TLV_RECHEADER_SIZE );  
	   pucTempEveData = (unsigned char*) realloc( (unsigned char*)pucTempEveData, unNewEvtDataLengthInBytes );

	   *ppuncDBEvtBuff_o               	= pucTempEveData;
	   unDBEvtDataLengthInBytes         = unNewEvtDataLengthInBytes;
	   *punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

	   poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_ORIGINATOR_INTERFACE;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

       HAVP                  hToAVP               = NULL;
	   void*                 pvNodeData           = NULL;
	   _Bool                 bConvToNetOrder      = 0;
       t_DiamEnum*			 poTempEnum     	  = NULL;
	   t_DiamUTF8String*	 poTempUTF8Str	 	  = NULL;
	   DMBase_SList*	     pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

	   while( NULL != pCurrentNode )
	   {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFApplicationContext->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case  OFFLINE_CS_INTERFACE_ID:		//2003//UTF8String 
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_INTERFACE_ID ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Interface-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Interface-Id AVP Found \n");
								 break;
						case  OFFLINE_CS_INTERFACE_TEXT:	//2005//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_INTERFACE_TEXT ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Interface-Text AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Interface-Text AVP Found \n");
								 break;
						case  OFFLINE_CS_INTERFACE_PORT:	//2004//UTF8String 
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_INTERFACE_PORT ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Interface-Port AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Interface-Port AVP Found \n");
								 break;
						case  OFFLINE_CS_INTERFACE_TYPE:	//2006//Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                bConvToNetOrder = 1;
                                if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_INTERFACE_TYPE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Interface-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Interface-Type AVP Found \n");
								break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP Received in Originator-Interface AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
 }

t_Offline_CS_ReturnVal RF_AddDestinationInterfaceAVPDataToEventData(  unsigned char**     ppuncDBEvtBuff_o ,
																	  unsigned int        unTotalEvtDataLengthInBytes_i,
																	  unsigned int*       punNewEvtDataLengthInBytes_o,
																	  HAVP                hToAVP_i)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == ppuncDBEvtBuff_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppuncDBEvtBuff_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == punNewEvtDataLengthInBytes_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"punNewEvtDataLengthInBytes_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer ,leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		unsigned int    	unOldTLVBuffLength       		=  0;
		unsigned int    	unNewTLVBuffLength       		=  0;
		unsigned int  		unAVPCode					  	=  0;
		unsigned int     	unNewEvtDataLengthInBytes     	=  0;
		unsigned int     	unDBEvtDataLengthInBytes      	=  0;
		unsigned char*   	pucTempEveData    				=  NULL;
		t_Dappcommon_tlv_header* poTemp_TLV_Header     		=  NULL ;

		unOldTLVBuffLength = unTotalEvtDataLengthInBytes_i;

		pucTempEveData =  *ppuncDBEvtBuff_o;
		unNewEvtDataLengthInBytes = (unTotalEvtDataLengthInBytes_i + DAPPCOMMON_TLV_RECHEADER_SIZE );  
		pucTempEveData = (unsigned char*) realloc( (unsigned char*)pucTempEveData, unNewEvtDataLengthInBytes );

		*ppuncDBEvtBuff_o                = pucTempEveData;
		unDBEvtDataLengthInBytes         = unNewEvtDataLengthInBytes;
		*punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_DESTINATION_INTERFACE;
		//poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

		HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
		_Bool                 bConvToNetOrder      = 0;
		t_DiamEnum*			  poTempEnum     	   = NULL;
		t_DiamUTF8String*	  poTempUTF8Str	 	   = NULL;
		DMBase_SList*	      pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

	   while( NULL != pCurrentNode )
	   {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFApplicationContext->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }
                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case  OFFLINE_CS_INTERFACE_ID:	//2003//UTF8String 
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_INTERFACE_ID ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Interface-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "TMGI AVP Empty case, Leaving \n");
								 break;
						case  OFFLINE_CS_INTERFACE_TEXT:	//2005//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_INTERFACE_TEXT ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Interface-Text AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Interface-Text AVP Found \n");
								 break;
						case  OFFLINE_CS_INTERFACE_PORT:	//2004//UTF8String 
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_INTERFACE_PORT ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Interface-Port AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Interface-Port AVP Found \n");
								 break;
						case  OFFLINE_CS_INTERFACE_TYPE:	//2006//Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                bConvToNetOrder = 1;
                                if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_INTERFACE_TYPE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Interface-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Interface-Type AVP Found \n");
								break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP Received in Destination-Interface Grouped AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
 }


