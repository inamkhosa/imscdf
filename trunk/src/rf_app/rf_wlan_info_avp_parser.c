
#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include "offcs_app_defined_avp.h"



extern 		int  							 g_nDiamBaseTraceID ;
extern 		t_AVPDictionaryElement	 		 g_oDmBase_AVPDictionary[];
extern 		HRFAPPLICATIONCONTEXT            hRFApplicationContext;


t_Offline_CS_ReturnVal RF_AddWLANInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_WLAN_INFORMATION;
	  
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 


      HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
	  _Bool                 bConvToNetOrder      = 0;
      t_DiamUnsigned32*		poTempUnsigned32     = NULL;
	  t_DiamUTF8String*		poTempOctetString 	 = NULL;
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
						case OFFLINE_CS_WLAN_SESSION_ID:  //1246//Unsigned32 
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                bConvToNetOrder = 1;
                                if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																						   unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,
																						   unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),
																						   bConvToNetOrder,
																						   OFFLINE_CS_WLAN_SESSION_ID))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Problem Occured While adding WLAN-Session-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"WLAN_Session-Id AVP found\n");
                                break;
                        
						case OFFLINE_CS_PDG_ADDRESS: //	895	//OCTETSTRING OF  type Address
								poTempOctetString = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempOctetString->pucOctetString,
																								 poTempOctetString->unNumOfOctets,
																								 OFFLINE_CS_PDG_ADDRESS ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding PDG-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PDG-Address AVP found\n");
                                 break;

						case OFFLINE_CS_PDG_CHARGING_ID: //	896 // Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                bConvToNetOrder = 1;
                                if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																						   unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,
																						   unDBEvtDataLengthInBytes,
																						   *(poTempUnsigned32->punUnsigned32),
																						   bConvToNetOrder,
																						   OFFLINE_CS_PDG_CHARGING_ID))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding PDG-Charging-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PDG-Charging-Id AVP found\n");
                                break;
						case OFFLINE_CS_WAG_ADDRESS: //890//OCTETSTRING of type Address
								poTempOctetString = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempOctetString->pucOctetString,
																								 poTempOctetString->unNumOfOctets,
																								 OFFLINE_CS_WAG_ADDRESS ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding WAG-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"WAG-Address AVP found\n");
                                 break;
						case OFFLINE_CS_WAG_PLMN_ID: //	891//OctetString 
								poTempOctetString = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempOctetString->pucOctetString,
																								 poTempOctetString->unNumOfOctets,
																								 OFFLINE_CS_WAG_PLMN_ID ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding WAG-PLMN-Id AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"WAG-PLMN-Id AVP found\n");
                                 break;
						case OFFLINE_CS_WLAN_RADIO_CONTAINER: //892//Grouped.
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"WLAN-Radio-Container AVP found\n");
								if (Offline_CS_RetVal_Success != RF_AddWLANRadioContainerAVPDataToEventData( ppuncDBEvtBuff_o,
																											 unDBEvtDataLengthInBytes,
																											 &unNewEvtDataLengthInBytes,
																											 hToAVP ))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding WLAN-Radio-Container AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							    }
                                unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
                                break;
						case OFFLINE_CS_WLAN_UE_LOCAL_IP_ADDRESS: //894 //OCTETSTRING OF TYPE Address
								poTempOctetString = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempOctetString->pucOctetString,
																								 poTempOctetString->unNumOfOctets,
																								 OFFLINE_CS_WLAN_UE_LOCAL_IP_ADDRESS ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding WLAN-UE-Local-IP-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"WLAN-UE-Local-IPAddress AVP found\n");
                                 break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in WLAN-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

	   
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
}


        
t_Offline_CS_ReturnVal RF_AddWLANRadioContainerAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
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

	   poTemp_TLV_Header = (t_Dappcommon_tlv_header*)*ppuncDBEvtBuff_o;
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_WLAN_RADIO_CONTAINER;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 


      HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
	  _Bool                 bConvToNetOrder      = 0;
      t_DiamUnsigned32*		poTempUnsigned32     = NULL;
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
						/*case  OFFLINE_CS_OPERATOR_NAME: //FOR CODE AND TYPE REFER 204 DOC OF 299
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"This Switch case is curretly Empty, Leaving \n");
								 break;*/
                        
						case  OFFLINE_CS_LOCATION_TYPE: //1244 //Grouped
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Location-Type AVP found\n");
                                 if (Offline_CS_RetVal_Success != RF_AddLocationTypeAVPDataToEventData( ppuncDBEvtBuff_o,
																										unDBEvtDataLengthInBytes,
																										&unNewEvtDataLengthInBytes,
																										hToAVP ))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding Location-Type AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
							    }
                                unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
                                break;

				/*case  OFFLINE_CS_LOCATION_INFORMATION: //FOR CODE AND TYPE SEE 403 DOC OF 299
						 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"This Switch case is curretly Empty, Leaving \n");
								break;*/
				case  OFFLINE_CS_WLAN_TECHNOLOGY: //893//Unsigned32.
						 poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
						 bConvToNetOrder = 1;
						 if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																					unDBEvtDataLengthInBytes,
																					&unNewEvtDataLengthInBytes,
																					unDBEvtDataLengthInBytes,
																					*(poTempUnsigned32->punUnsigned32),
																					bConvToNetOrder,
																					OFFLINE_CS_WLAN_TECHNOLOGY))
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem Occured While adding  WLAN-Techonology AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"WLAN-Techonology AVP found\n");
								break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in WLAN-Radio-Container AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
  }

t_Offline_CS_ReturnVal RF_AddLocationTypeAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
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

	   poTemp_TLV_Header = (t_Dappcommon_tlv_header*)*ppuncDBEvtBuff_o;
       poTemp_TLV_Header->unAppSpecificTag = OFFLINE_CS_LOCATION_TYPE;
       // poTemp_TLV_Header->unRecordType = ; This should be Grouped in this case 


      HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
	  _Bool                 bConvToNetOrder      = 0;
      t_DiamEnum*			poTempEnum     		 = NULL;
      t_DiamUTF8String*		poTempUTF8Str	 	 = NULL;
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
                        case OFFLINE_CS_LOCATION_ESTIMATE_TYPE: //1243//Enumerated 
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                bConvToNetOrder = 1;
                                if(DappCommon_Retval_Success!= DappCommon_AddIntToTLVBuff( ppuncDBEvtBuff_o,
																						   unDBEvtDataLengthInBytes,
																						   &unNewEvtDataLengthInBytes,
																						   unDBEvtDataLengthInBytes,
																						   *(poTempEnum->pnInteger32),
																						   bConvToNetOrder,
																						   OFFLINE_CS_LOCATION_ESTIMATE_TYPE) )
								{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, ",problem Occured While adding 3-GPP-Charging data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								}
								unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Location-Estimate-Type AVP found\n");
								break;
						case OFFLINE_CS_DEFERRED_LOCATION_EVENT_TYPE: //1230//UTF8String
								poTempUTF8Str = (t_DiamUTF8String*)hToAVP->pData;
								if(DappCommon_Retval_Success!= DappCommon_AddOctStringToTLVBuff( ppuncDBEvtBuff_o,
																								 unDBEvtDataLengthInBytes,
																								 &unNewEvtDataLengthInBytes,
																								 unDBEvtDataLengthInBytes,
																								 poTempUTF8Str->pucOctetString,
																								 poTempUTF8Str->unNumOfOctets,
																								 OFFLINE_CS_DEFERRED_LOCATION_EVENT_TYPE ))
								 {
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While adding WLAN-UE-Local-IP-Address AVP data to TLV buffer, Leaving \n");
										return Offline_CS_RetVal_Failure;
								 }
								 unDBEvtDataLengthInBytes = unNewEvtDataLengthInBytes;
								 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Deferred-Location-Event-Type AVP found\n");
								break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in WLAN-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFApplicationContext ->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       *punNewEvtDataLengthInBytes_o       = unNewTLVBuffLength = unNewEvtDataLengthInBytes;
       poTemp_TLV_Header->unRecordLength  = (unNewTLVBuffLength - unOldTLVBuffLength);

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
 }
