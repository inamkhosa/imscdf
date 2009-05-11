#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include "offcs_app_defined_avp.h"

extern 		int  							 g_nDiamBaseTraceID ;
extern 		t_AVPDictionaryElement	 		 g_oDmBase_AVPDictionary[];
extern 		HRFAPPLICATIONCONTEXT            hRFApplicationContext;



t_Offline_CS_ReturnVal RF_AddMMSInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_MMS_INFORMATION;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

       HAVP                  hToAVP               = NULL;
	   void*                 pvNodeData           = NULL;
	   _Bool                 bConvToNetOrder      = 0;
       t_DiamEnum*			 poTempEnum     	  = NULL;
	   t_DiamTime*			 poTempTime			  = NULL;
	   t_DiamUnsigned32*	 poTempUnsigned32	  = NULL;
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
						case  OFFLINE_CS_ORIGINATOR_ADDRESS: //886//Grouped.
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Originator-address Grouped AVP Found\n");
                                if (Offline_CS_RetVal_Success != RF_AddOrignatorAddressAVPDatToEventData( ppuncDBEvtBuff_o,
																										   unDBEvtDataLengthInBytes,
																										   &unNewEvtDataLengthInBytes,
																										   hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Originator-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case  OFFLINE_CS_RECIPIENT_ADDRESS:  //1201//Grouped.
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Recipient-Address  Grouped AVP Found\n");
								if (Offline_CS_RetVal_Success != RF_AddRecepientAddressAVPDataToEventData( ppuncDBEvtBuff_o,
																										   unDBEvtDataLengthInBytes,
																										   &unNewEvtDataLengthInBytes,
																										   hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Recipient-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case  OFFLINE_CS_SUBMISSION_TIME://1202// TIME
								 poTempTime = (t_DiamTime*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempTime->punUnsigned32),
																							bConvToNetOrder,
																							OFFLINE_CS_SUBMISSION_TIME ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Submission-Time AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Submission-Time AVP Found\n");
								break;
						case  OFFLINE_CS_MM_CONTENT_TYPE://1203//Grouped 
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"MM-Content-Type AVP Found\n");
								if (Offline_CS_RetVal_Success != RF_AddMMContentTypeAVPDataToEventData( ppuncDBEvtBuff_o,
																										unDBEvtDataLengthInBytes,
																										&unNewEvtDataLengthInBytes,
																										hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding MM-Content-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case  OFFLINE_CS_PRIORITY:  //1209//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_PRIORITY ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Priority AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Priority AVP Found\n");
								break;
						case  OFFLINE_CS_MESSGAE_ID://1210//UTF8String 
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_MESSGAE_ID ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Message-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Message-Id AVP Found\n");
								 break;
						case  OFFLINE_CS_MESSAGE_TYPE://1211//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_MESSAGE_TYPE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Message-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Message-Type AVP Found\n");
								 break;
						case  OFFLINE_CS_MESSAGE_SIZE: //1212//Unsigned32
								 poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempUnsigned32->punUnsigned32),
																							bConvToNetOrder,
																							OFFLINE_CS_MESSAGE_SIZE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Message-Size AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Message-Size AVP Found\n");
								break;
						case  OFFLINE_CS_MESSAGE_CLASS://1213//Grouped.
								 if (Offline_CS_RetVal_Success != RF_AddMessageClassAVPDataToEventData( ppuncDBEvtBuff_o,
																										unDBEvtDataLengthInBytes,
																										&unNewEvtDataLengthInBytes,
																										hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Message-Class AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Message-Class AVP Found\n");
								break;
						case  OFFLINE_CS_DELIVERY_REPORT_REQUESTED://	1216//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_DELIVERY_REPORT_REQUESTED ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Delivery-Report-Requested AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Delivery-Report-Requested AVP Found\n");
								break;
						case  OFFLINE_CS_READ_REPLY_REPORT_REQUESTED://1222//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_READ_REPLY_REPORT_REQUESTED ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Readd-Reply-Report AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Read-Reply-Report-Requested AVP Found\n");
								break;
						case  OFFLINE_CS_MM_BOX_STORAGE_REQUESTED://1248//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_MM_BOX_STORAGE_REQUESTED ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding MM-Box-storage-Requested AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"MM-Box-Storage-Request AVP Found\n");
								break;
						case  OFFLINE_CS_APPLIC_ID://1218//UTF8String 
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_APPLIC_ID ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Applic-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Applic-Id AVP Found\n");
								 break;
						case  OFFLINE_CS_REPLY_APPLIC_ID://1223//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_REPLY_APPLIC_ID ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Reply-Applic-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Reply-Applic-Id AVP Found\n");
								 break;
						case  OFFLINE_CS_AUX_APPLIC_INFO://1219//UTF8String 
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_AUX_APPLIC_INFO ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Aux-Applic-Info AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Aux-Applic-Info AVP Found\n");
								 break;
						case  OFFLINE_CS_CONTENT_CLASS://1220//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_CONTENT_CLASS ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Content-Class AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Content-Class AVP Found\n");
								 break;
						case  OFFLINE_CS_DRM_CONTENT://1221//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_DRM_CONTENT ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding DRM-Content AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"DRM-Content AVP Found\n");
								break;
						case  OFFLINE_CS_ADAPTATIONS://1217//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_ADAPTATIONS ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Adaptations AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Adaptations AVP Found\n");
								 break;
						case  OFFLINE_CS_VASP_ID://1101//for type see 213
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, "Empty case, Leaving \n");
								 break;
						case  OFFLINE_CS_VAS_ID://1102//for type see 213
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, "Empty case, Leaving \n");
								 break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in MMS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;

   }

t_Offline_CS_ReturnVal RF_AddOrignatorAddressAVPDatToEventData( unsigned char**     ppuncDBEvtBuff_o,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_ORIGINATOR_ADDRESS;
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
						case  OFFLINE_CS_ADDRESS_TYPE: //899//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Address-Type AVP Found\n");
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_ADDRESS_TYPE ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Address-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 break;
						case  OFFLINE_CS_ADDRESS_DATA://897//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Address-Data AVP Found\n");
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_ADDRESS_DATA ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Address-Data AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 break;
						case  OFFLINE_CS_ADDRESS_DOMAIN://898//Grouped 
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Address-Domain AVP Found\n");
                                 if (Offline_CS_RetVal_Success != RF_AddAddressDomainAVPDataToEventData( ppuncDBEvtBuff_o,
																										 unDBEvtDataLengthInBytes,
																										 &unNewEvtDataLengthInBytes,
																										 hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Address-Domain AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;

						default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Originator-Address AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
		
   }

t_Offline_CS_ReturnVal RF_AddAddressDomainAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
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

		*ppuncDBEvtBuff_o               = pucTempEveData;
		unDBEvtDataLengthInBytes        = unNewEvtDataLengthInBytes;
	   *punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_ADDRESS_DOMAIN;
		// poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

		HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
		_Bool                 bConvToNetOrder      = 0;
		t_DiamEnum*			  poTempEnum     	   = NULL;
		t_DiamUTF8String*	  poTempUTF8Str 	   = NULL;
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
						case  OFFLINE_CS_DOMAIN_NAME://1200//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_DOMAIN_NAME ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Domain-Name AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Domain-Name AVP Found\n");
								 break;
						case  OFFLINE_CS_3GPP_IMSI_MCC_MNC: //8 //UNKNOWN TYPE WILL SEE IN 207
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, "Empty case, Leaving \n");
								 break;

						default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Address-Domain AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
       
	}

t_Offline_CS_ReturnVal RF_AddRecepientAddressAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
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

		*ppuncDBEvtBuff_o               	= pucTempEveData;
		unDBEvtDataLengthInBytes         = unNewEvtDataLengthInBytes;
	   *punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_RECIPIENT_ADDRESS;
		// poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

		HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
		_Bool                 bConvToNetOrder      = 0;
		t_DiamEnum*			  poTempEnum     	   = NULL;
		t_DiamUTF8String*	  poTempUTF8Str 	   = NULL;
		DMBase_SList*	      pCurrentNode          = hToAVP_i->hCollectionOfAVPs;

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
						case  OFFLINE_CS_ADDRESS_TYPE://899//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_ADDRESS_TYPE ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Address-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Address-Type AVP Found\n");
								 break;
						case  OFFLINE_CS_ADDRESS_DATA://897//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_ADDRESS_DATA ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Address-Data AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								  }
								  unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								  break;
						case  OFFLINE_CS_ADDRESS_DOMAIN://898//Grouped 
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Address-Domain AVP Found\n");
                                 if (Offline_CS_RetVal_Success != RF_AddAddressDomainAVPDataToEventData( ppuncDBEvtBuff_o,
																										 unDBEvtDataLengthInBytes,
																										 &unNewEvtDataLengthInBytes,
																										 hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Address-Domain AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								break;
						case  OFFLINE_CS_ADDRESSEE_TYPE://1208//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_ADDRESSEE_TYPE ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Addressee-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Addressee-Type AVP Found\n");
								 break;

						default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Recipient-Address AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;

   }

t_Offline_CS_ReturnVal RF_AddMMContentTypeAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
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
	   *punNewEvtDataLengthInBytes_o     = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_MM_CONTENT_TYPE;
		// poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

		HAVP                  hToAVP              = NULL;
		void*                 pvNodeData          = NULL;
		_Bool                 bConvToNetOrder     = 0;
		t_DiamEnum*			  poTempEnum     	  = NULL;
		t_DiamUTF8String*	  poTempUTF8Str 	  = NULL;
		t_DiamUnsigned32*	  poTempUnsigned32    = NULL;
		DMBase_SList*	      pCurrentNode        = hToAVP_i->hCollectionOfAVPs;

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
						case  OFFLINE_CS_TYPE_NUMBER://1204//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_TYPE_NUMBER ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Type-Number AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Type-Number AVP Found\n");
								 break;
						case  OFFLINE_CS_ADDITIONAL_TYPE_INFORMATION://1205//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_ADDITIONAL_TYPE_INFORMATION ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Additional-Type-Information AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								  }
								  unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Additional-Type-Information AVP Found\n");
								 break;
						case  OFFLINE_CS_CONTENT_SIZE://1206//Unsigned32
								 poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempUnsigned32->punUnsigned32),
																							bConvToNetOrder,
																							OFFLINE_CS_CONTENT_SIZE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Content-Size AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Content-Size AVP Found\n");
								 break;
						case  OFFLINE_CS_ADDITIONAL_CONTENT_INFORMATION://1207// Grouped
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Additional-Content-Information Grouped AVP Found\n");
								 if (Offline_CS_RetVal_Success != RF_AddAdditionalContentInfoAVPDataToEventData( ppuncDBEvtBuff_o,
																												 unDBEvtDataLengthInBytes,
																												 &unNewEvtDataLengthInBytes,
																												 hToAVP ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Content-Information AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							     }
                                 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
                                 break;

						default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in MM-Content-Type AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
	}

t_Offline_CS_ReturnVal RF_AddAdditionalContentInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
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

		*ppuncDBEvtBuff_o               	= pucTempEveData;
		unDBEvtDataLengthInBytes         = unNewEvtDataLengthInBytes;
	   *punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_ADDITIONAL_CONTENT_INFORMATION;
		// poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

		HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
		_Bool                 bConvToNetOrder      = 0;
		t_DiamEnum*			  poTempEnum     	   = NULL;
		t_DiamUTF8String*	  poTempUTF8Str 	   = NULL;
		t_DiamUnsigned32*	  poTempUnsigned32     = NULL;
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
						case  OFFLINE_CS_TYPE_NUMBER: //1204//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_TYPE_NUMBER ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Type-Number AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Type-Number AVP Found\n");
								 break;
						case  OFFLINE_CS_ADDITIONAL_TYPE_INFORMATION: //1205//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_ADDITIONAL_TYPE_INFORMATION ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Additional-Type-Information AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								  }
								  unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Additional-Type-Information AVP Found\n");
								 break;
						case  OFFLINE_CS_CONTENT_SIZE://1206//Unsigned32
								 poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempUnsigned32->punUnsigned32),
																							bConvToNetOrder,
																							OFFLINE_CS_CONTENT_SIZE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Content-Size AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Content-Size AVP Found\n");
								 break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Additional-Content-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
	}

t_Offline_CS_ReturnVal RF_AddMessageClassAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
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

		*ppuncDBEvtBuff_o               = pucTempEveData;
		unDBEvtDataLengthInBytes        = unNewEvtDataLengthInBytes;
	   *punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_MESSAGE_CLASS;
		// poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

		HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
		_Bool                 bConvToNetOrder      = 0;
		t_DiamEnum*			  poTempEnum     	   = NULL;
		t_DiamUTF8String*	  poTempUTF8Str 	   = NULL;
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
						case  OFFLINE_CS_CLASS_IDENTIFIER:// 	1214//Enumerated
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
								 bConvToNetOrder = 1;
								 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																							unDBEvtDataLengthInBytes,
																							&unNewEvtDataLengthInBytes,
																							unDBEvtDataLengthInBytes,
																							*(poTempEnum->pnInteger32),
																							bConvToNetOrder,
																							OFFLINE_CS_CLASS_IDENTIFIER ) )
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Class-Identifier AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Class-Identifier AVP Found\n");
								 break;
						case  OFFLINE_CS_TOKEN_TEXT:// 	1215//UTF8String
								 poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								 if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_TOKEN_TEXT ))
								  {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding Token-Text AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								  }
								  unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Token-Text AVP Found\n");
								 break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Message-Classs AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o      = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
	}
