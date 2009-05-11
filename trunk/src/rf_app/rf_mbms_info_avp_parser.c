#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include "offcs_app_defined_avp.h"

extern 		int  							 g_nDiamBaseTraceID ;
extern 		t_AVPDictionaryElement	 		 g_oDmBase_AVPDictionary[];
extern 		HRFAPPLICATIONCONTEXT            hRFApplicationContext;

t_Offline_CS_ReturnVal RF_AddMBMSInfoAVPDataToEventData(unsigned char**     ppuncDBEvtBuff_o,
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
		unsigned int     	unNewTLVBuffLength       		=  0;
		unsigned int  	 	unAVPCode					  	=  0;
		unsigned int     	unNewEvtDataLengthInBytes     	=  0;
		unsigned int     	unDBEvtDataLengthInBytes      	=  0;
		unsigned char*   	pucTempEveData    				=  NULL;
		t_Dappcommon_tlv_header* poTemp_TLV_Header     	=  NULL ;

		unOldTLVBuffLength = unTotalEvtDataLengthInBytes_i;

		pucTempEveData =  *ppuncDBEvtBuff_o;
		unNewEvtDataLengthInBytes = (unTotalEvtDataLengthInBytes_i + DAPPCOMMON_TLV_RECHEADER_SIZE );  
		pucTempEveData = (unsigned char*) realloc( (unsigned char*)pucTempEveData, unNewEvtDataLengthInBytes );

		*ppuncDBEvtBuff_o               	= pucTempEveData;
		unDBEvtDataLengthInBytes         = unNewEvtDataLengthInBytes;
		*punNewEvtDataLengthInBytes_o    = unNewEvtDataLengthInBytes;

		poTemp_TLV_Header = (t_Dappcommon_tlv_header*)(*ppuncDBEvtBuff_o + unTotalEvtDataLengthInBytes_i);
		poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_MBMS_INFORMATION;
		// poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 

		HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
		_Bool                 bConvToNetOrder      = 0;
		t_DiamEnum*			  poTempEnum     	   = NULL;
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
						case  OFFLINE_CS_TMGI:	//900//FOR TYPE SEE 207
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "TMGI AVP Empty case, Leaving \n");
							     break;
						case  OFFLINE_CS_MBMS_SERVICE_TYPE://906//FOR TYPE [207]
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "MBMS-Service AVP Empty case, Leaving \n");
								 break;
						case  OFFLINE_CS_MBMS_USER_SERVICE_TYPE://1225//Enumerated 
								 poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                bConvToNetOrder = 1;
                                if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																						   unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,
																						   unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),
																						   bConvToNetOrder,
																						   OFFLINE_CS_MBMS_USER_SERVICE_TYPE ) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding MBMS-User-Service-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "MBMS-User-Service-Type AVP Empty case, Leaving \n");
								break;
						case  OFFLINE_CS_FILE_REPAIRE_SUPPORTED://1224//Enumerated 
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                bConvToNetOrder = 1;
                                if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																						   unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,
																						   unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),
																						   bConvToNetOrder,
																						   OFFLINE_CS_FILE_REPAIRE_SUPPORTED) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding File-Repaire-Supported AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "File-Repaire-Supported  AVP Empty case, Leaving \n");
								break;
						case  OFFLINE_CS_REQUIRED_MBMS_BEARER_CAPABILITIES: //901//FOR TYPE [207]
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2, "Required-MBMS-Bearer-Capabilities AVP Empty case, Leaving \n");
								 break;
						case  OFFLINE_CS_MBMS_2G_3G_INDICATOR: //907//FOR TYPE [207]
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " MBMS-2G-3G-Indicator AVP Empty case, Leaving \n");
								 break;
						case  OFFLINE_CS_RAI://	909// FOR TYPE refer [207]
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, "RAI AVP Empty case, Leaving \n");
								 break;
						case  OFFLINE_CS_MBMS_SERVICE_AREA://903//FOR TYPE [207]
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, "MBMS-service-Area AVP Empty case, Leaving \n");
								 break;
						case  OFFLINE_CS_MBMS_SESSION_IDENTITY://908//FOR TYPE [207]
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, "MBMS-Session-Identity AVP Empty case, Leaving \n");
								 break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in MBMS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;

    } 


