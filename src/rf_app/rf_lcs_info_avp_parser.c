
#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include "offcs_app_defined_avp.h"

extern 		int  							 g_nDiamBaseTraceID ;
extern 		t_AVPDictionaryElement	 		 g_oDmBase_AVPDictionary[];
extern 		HRFAPPLICATIONCONTEXT            hRFApplicationContext;


t_Offline_CS_ReturnVal RF_AddLCSInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_LCS_INFORMATION;
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
						case  OFFLINE_CS_LCS_CLIENT_ID://1232//Grouped
							    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Client-Id Grouped AVP found\n");
                                if (Offline_CS_RetVal_Success != RF_AddLCSClientIdAVPDataToEventData( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																									  &unNewEvtDataLengthInBytes,hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Client-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 break;
						case  OFFLINE_CS_LOCATION_TYPE://	1244 //Grouped 
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Location-Type Grouped AVP found\n");
                                 if (Offline_CS_RetVal_Success != RF_AddLocationTypeAVPDataToEventData( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																										&unNewEvtDataLengthInBytes,hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Location-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 break;
						case   OFFLINE_CS_LOCATION_ESTIMATE://1242//UTF8String
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Location-Estimate AVP found\n");
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LOCATION_ESTIMATE ))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Location-Estimate AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_POSITIONING_DATA://1245//UTF8String
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Positioning-Data AVP found\n");
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_POSITIONING_DATA ))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Positioning-Data AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						/*case   OFFLINE_CS_IMSI: //not found
								 break;*/
						case   OFFLINE_CS_MSISDN:	//not found	
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"MSISDN  AVP found\n");
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_MSISDN ))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Positioning-Data AVP data to TLV buffer, Leaving \n");
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




/*t_Offline_CS_ReturnVal RF_AddLocationTypeAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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

		if (NULL != *ppuncDBEvtBuff_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"*ppuncDBEvtBuff_o is not NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
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
		unsigned int  		unAVPCode					  	=  0;
		unsigned int     	unNewEvtDataLengthInBytes     	=  0;
		unsigned int     	unDBEvtDataLengthInBytes      	=  0;
		unsigned char*   	pucTempEveData    				=  NULL;
		t_Dappcommon_tlv_header* poTemp_TLV_Header     		=  NULL ;

		unOldTLVBuffLength = unTotalEvtDataLengthInBytes_i;

		pucTempEveData =  *ppuncDBEvtBuff_o;
		unNewEvtDataLengthInBytes = (unTotalEvtDataLengthInBytes_i + DAPPCOMMON_TLV_RECHEADER_SIZE );  
		pucTempEveData = (unsigned char*) realloc( (unsigned char*)pucTempEveData, unNewEvtDataLengthInBytes );

		*ppuncDBEvtBuff_o               	= pucTempEveData;
		unDBEvtDataLengthInBytes         = unNewEvtDataLengthInBytes;
		*punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)*ppuncDBEvtBuff_o;
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_LOCATION_TYPE;
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
						case  OFFLINE_CS_LOCATION_ESTIMATE_TYPE:	//1243//Enumerated 
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																						   unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,
																						   unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),
																						   bConvToNetOrder,
																						   OFFLINE_CS_LOCATION_ESTIMATE_TYPE ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Location-Estimate-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
                        case  OFFLINE_CS_DEFERRED_LOCATION_EVENT_TYPE: //1230//UTF8String
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_DEFERRED_LOCATION_EVENT_TYPE ))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Deferred-Location-Event-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;	
								break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Location-Type AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;

	}*/


t_Offline_CS_ReturnVal RF_AddLCSClientIdAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_LCS_CLIENT_ID;
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
						case   OFFLINE_CS_LCS_CLIENT_TYPE:		//1241//Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								bConvToNetOrder = 1;
								if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),bConvToNetOrder,
																						   OFFLINE_CS_LCS_CLIENT_TYPE ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding LCS-Client-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Client-Client-Type AVP found\n");
								break;
                        case   OFFLINE_CS_LCS_CLIENT_EXTERNAL_ID:	//1234//UTF8String
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LCS_CLIENT_EXTERNAL_ID ))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding LCS-Client-External-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Client-External-Id  AVP found\n");
								break;
						case   OFFLINE_CS_LCS_CLIENT_DIALED_BY_MS://1233//UTF8String 
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LCS_CLIENT_DIALED_BY_MS ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding LCS-Client-Dialed-By-MS AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								  }
								  unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Client-Dialed-By-MS AVP found\n");
								  break;
						case   OFFLINE_CS_LCS_CLIENT_NAME://	1235//Grouped
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Client-Name Grouped AVP found\n");
								if (Offline_CS_RetVal_Success != RF_AddLCSClientNameAVPDataToEventData( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																										&unNewEvtDataLengthInBytes, hToAVP ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Client-Name AVP data to TLV buffer, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case   OFFLINE_CS_LCS_APN://1231//UTF8String 
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes, unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString, poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LCS_APN ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding LCS-APN AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								  }
								  unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-APN  AVP found\n");
								  break;
						case   OFFLINE_CS_LCS_REQUESTOR_ID:// 1239//Grouped 
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Requestor-Id Grouped AVP found\n");
								 if (Offline_CS_RetVal_Success != RF_AddLCSRequestorIdAVPDataToEventData( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																										  &unNewEvtDataLengthInBytes, hToAVP ))
								 {
									 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding LCS-Requestor-Id AVP data to TLV buffer, Leaving \n");
									 return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in LCS-Client-Id AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
  }


t_Offline_CS_ReturnVal RF_AddLCSClientNameAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_LCS_CLIENT_NAME;
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
						case  OFFLINE_CS_LCS_DATA_CODING_SCHEME:// 1236//UTF8String  
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LCS_DATA_CODING_SCHEME ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding LCS-Data-Coding-Scheme AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Data-Coding-Scheme AVP found\n");
								 break;
                        case  OFFLINE_CS_LCS_NAME_STRING:// 1238// UTF8String 
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LCS_NAME_STRING ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding LCS-Name-String-Scheme AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Name-String AVP found\n");
								 break;
						case  OFFLINE_CS_LCS_FORMAT_INDICATOR:// 1237//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),bConvToNetOrder,
																							OFFLINE_CS_LCS_FORMAT_INDICATOR ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding LCS-Format-Indicator AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Formate-Indicator AVP found\n");
								 break;
                        
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP Received in LCS-Client-Name AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
   }

t_Offline_CS_ReturnVal RF_AddLCSRequestorIdAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_LCS_REQUESTOR_ID;
		// poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

       HAVP                  hToAVP               = NULL;
	   void*                 pvNodeData           = NULL;
	   _Bool                 bConvToNetOrder      = 0;
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
						case  OFFLINE_CS_LCS_DATA_CODING_SCHEME://	1236//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o, unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes, unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LCS_DATA_CODING_SCHEME ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding LCS-Data-Coding-Scheme AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Data-Codeing-Scheme AVP found\n");
								 break;
                        case  OFFLINE_CS_LCS_REQUESTOR_ID_STRING://   1240//UTF8String 
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_LCS_REQUESTOR_ID_STRING ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding LCS-Requestor-Id-String AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"LCS-Requestor-Id-String AVP found\n");
								 break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP Received in LCS-Requestor-Id AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
 } 
