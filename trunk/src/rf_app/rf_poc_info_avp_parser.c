
#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include "offcs_app_defined_avp.h"

extern 		int  							 g_nDiamBaseTraceID ;
extern 		t_AVPDictionaryElement	 		 g_oDmBase_AVPDictionary[];
extern 		HRFAPPLICATIONCONTEXT            hRFApplicationContext;


t_Offline_CS_ReturnVal RF_AddPoCInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_POC_INFORMATION;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

       HAVP                  hToAVP               = NULL;
	   void*                 pvNodeData           = NULL;
	   _Bool                 bConvToNetOrder      = 0;
       t_DiamEnum*			 poTempEnum     	  = NULL;
	   t_DiamUTF8String*	 poTempUTF8Str 		  = NULL;
	   t_DiamUnsigned32*	 poTempUnsigned32	  = NULL;
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
						case  OFFLINE_CS_POC_SERVER_ROLE://Enumerated
							    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Server-Role AVP found\n");
                                poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_POC_SERVER_ROLE ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PoC-Server-Role AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case  OFFLINE_CS_POC_SESSION_TYPE://Enumerated
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Session-Type AVP found\n");
                                 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),bConvToNetOrder,
																							OFFLINE_CS_POC_SESSION_TYPE ) )
								 {
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PoC-Session-Type AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_POC_USER_ROLE://Grouped.
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-User-Role AVP found\n");
								if (Offline_CS_RetVal_Success != RF_AddPoCUserRoleAVPDataToEventData( ppuncDBEvtBuff_o,
																									  unDBEvtDataLengthInBytes,
																									  &unNewEvtDataLengthInBytes,
																									  hToAVP ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PoC-User-Role AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_POC_SESSION_INITIATION_TYPE://Enumerated
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Session-Initiation-Type  AVP found\n");
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_POC_SESSION_INITIATION_TYPE ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PoC-Session-Initiation-Type AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						
						case   OFFLINE_CS_POC_EVENT_TYPE://Enumerated 	
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Event-Type AVP found\n");
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_POC_EVENT_TYPE ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PoC-Event-Type AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_NUMBER_OF_PARTICIPANTS://Unsigned32
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Number-Of-Participants  AVP found\n");
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),bConvToNetOrder,
																						   OFFLINE_CS_NUMBER_OF_PARTICIPANTS ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Number-Of-Participants AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_PARTICIPANTS_INVOLVED://UTF8String 
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Participants-Involved  AVP found\n");
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_PARTICIPANTS_INVOLVED ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Participants-Involved AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;

						case   OFFLINE_CS_PARTICIPANT_GROUP://Grouped
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Participants-Grouped Grouped   AVP found\n");
								if (Offline_CS_RetVal_Success != RF_AddParticipantGroupAVPDataToEventData( ppuncDBEvtBuff_o,
																										   unDBEvtDataLengthInBytes,
																										   &unNewEvtDataLengthInBytes,
																										   hToAVP ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Participants-Grouped AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;

						case   OFFLINE_CS_TALK_BURST_EXCHANGE://Grouped 
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Talk-Burst-Exchange Grouped  AVP found\n");
								if (Offline_CS_RetVal_Success != RF_AddTalkBurstExchangeAVPDataToEventData( ppuncDBEvtBuff_o,
																										   unDBEvtDataLengthInBytes,
																										   &unNewEvtDataLengthInBytes,
																										   hToAVP ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Talk-Burst-Exchange AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;

						case  OFFLINE_CS_POC_CONTROLLING_ADDRESS://UTF8String
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Controlling-Address  AVP found\n");
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_POC_CONTROLLING_ADDRESS ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding PoC-Controlling-Address AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;

						case  OFFLINE_CS_POC_GROUP_NAME: //UTF8String 
							   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Group-Name  AVP found\n");
							   poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
							   if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								OFFLINE_CS_POC_GROUP_NAME ))
							   {
								   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding PoC-Group-Name AVP data to TLV buffer, Leaving \n");
								   return Offline_CS_RetVal_Failure;
							   }
							   unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
							   break;

						case  OFFLINE_CS_POC_SESSION_ID://UTF8String.
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Session-Id  AVP found\n");
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_POC_SESSION_ID ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding PoC-Session-Id AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;

						case OFFLINE_CS_CHARGED_PARTY: //UTF8String.
							  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Charged-Party  AVP found\n");
							  poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
							  if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							   poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																							   OFFLINE_CS_CHARGED_PARTY ))
							  {
								  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Charged-Party AVP data to TLV buffer, Leaving \n");
								  return Offline_CS_RetVal_Failure;
							  }
							  unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
							  break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in LCS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
 }

t_Offline_CS_ReturnVal RF_AddPoCUserRoleAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_POC_USER_ROLE;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

       HAVP                  hToAVP               = NULL;
	   void*                 pvNodeData           = NULL;
	   _Bool                 bConvToNetOrder      = 0;
       t_DiamEnum*			 poTempEnum     	  = NULL;
	   t_DiamUTF8String*	 poTempUTF8Str 		  = NULL;
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
						case  OFFLINE_CS_POC_USER_ROLE_ID://UTF8String 
							    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-User-Role-Id AVP found\n");
                                poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_POC_USER_ROLE_ID ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding PoC-User-Role-Id AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						
						case   OFFLINE_CS_POC_USER_ROLE_INFO_UNIT://Enumerated
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-User-Role-Info-Unit  AVP found\n");
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_POC_USER_ROLE_INFO_UNIT ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PoC-User-Role-Info-Unit AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in LCS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;

  }

t_Offline_CS_ReturnVal RF_AddParticipantGroupAVPDataToEventData ( unsigned char**     ppuncDBEvtBuff_o ,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_PARTICIPANT_GROUP;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

       HAVP                  hToAVP               = NULL;
	   void*                 pvNodeData           = NULL;
	   _Bool                 bConvToNetOrder      = 0;
       t_DiamEnum*			 poTempEnum     	  = NULL;
	   t_DiamUTF8String*	 poTempUTF8Str 		  = NULL;
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
						case  OFFLINE_CS_CALLED_PARTY_ADDRESS://UTF8String 
							    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Called-Party-Address AVP found\n");
                                poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_CALLED_PARTY_ADDRESS ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Called-Party-Address AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						
						case   OFFLINE_CS_PARTICIPANT_ACCESS_PRIORITY://Enumerated
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Participant-Access-Priority  AVP found\n");
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_PARTICIPANT_ACCESS_PRIORITY ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Participant-Access-Priority AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_USER_PARTICIPANTING_TYPE://Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_USER_PARTICIPANTING_TYPE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding User-Participating-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"User-Participating-Type AVP found\n");
								break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in LCS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
	}


t_Offline_CS_ReturnVal RF_AddTalkBurstExchangeAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_TALK_BURST_EXCHANGE;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

       HAVP                  hToAVP               = NULL;
	   void*                 pvNodeData           = NULL;
	   _Bool                 bConvToNetOrder      = 0;
       t_DiamEnum*			 poTempEnum     	  = NULL;
	   t_DiamUnsigned32*	 poTempUnsigned32	  = NULL;
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
						case  OFFLINE_CS_POC_CHANGE_TIME://Unsigned32
							    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Changed-Time AVP found\n");
                                poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),bConvToNetOrder,
																						   OFFLINE_CS_POC_CHANGE_TIME ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding PoC-Changed-Time AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						
						case   OFFLINE_CS_NUMBER_OF_TALK_BURSTS://Unsigned32
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Number-Of-Talk-Bursts  AVP found\n");
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),bConvToNetOrder,
																						   OFFLINE_CS_NUMBER_OF_TALK_BURSTS ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Number-Of-Talk-Bursts AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_TALK_BURST_VOULME://Unsigned32
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Talk-Burst-Volume AVP found\n");
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),bConvToNetOrder,
																						   OFFLINE_CS_TALK_BURST_VOULME ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Talk-Burst-Volume AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case  OFFLINE_CS_TALK_BURST_TIME://Unsigned32
							    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Talk-Burst-Time  AVP found\n");
                                poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),bConvToNetOrder,
																						   OFFLINE_CS_TALK_BURST_TIME ) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Talk-Burst-Time AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						
						case   OFFLINE_CS_NUMBER_OF_RECEIVED_TALK_BURSTS://Unsigned32
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Number-Of-Received-Talk-Bursts  AVP found\n");
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),bConvToNetOrder,
																						   OFFLINE_CS_NUMBER_OF_RECEIVED_TALK_BURSTS ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Number-Of-Received-Talk-Bursts AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_RECEIVED_TALK_BURST_VOLUME://Unsigned32
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Received-Talk-Burst-Volume AVP found\n");
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempUnsigned32->punUnsigned32), bConvToNetOrder,
																							OFFLINE_CS_RECEIVED_TALK_BURST_VOLUME ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Received-Talk-Burst-Volume AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;

						case  OFFLINE_CS_RECEIVED_TALK_BURST_TIME://Unsigned32
							    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Received-Talk-Burst-Time AVP found\n");
                                poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempUnsigned32->punUnsigned32),
																							bConvToNetOrder,OFFLINE_CS_RECEIVED_TALK_BURST_TIME ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Received-Talk-Burst-Time AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						
						case   OFFLINE_CS_NUMBER_OF_PARTICIPANTS://Unsigned32
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Number-Of-Participants  AVP found\n");
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempUnsigned32->punUnsigned32),
																							bConvToNetOrder,OFFLINE_CS_NUMBER_OF_PARTICIPANTS ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Number-Of-Participants AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_POC_CHANGE_CONDITION://Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_POC_CHANGE_CONDITION ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PoC-Change-Condition AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PoC-Change-Condition AVP found\n");
								break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in LCS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;

	}
