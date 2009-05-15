#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include"RFModule.h"
#include "offcs_app_defined_avp.h"
#include "rf_ps_info_avp_parser.h"

extern 		int  							 g_nDiamBaseTraceID ;

/******************************************************************************
** Function Name: 
**			RF_AddPSInfoAVPDataToEventData
**  
** Description:
**		This function is returning the specified AVP from the given 
**      message Handle if present.
**
** Parameters:
**		  HDIAMETERSTACKCONTEXT hDiameterStackContext_io
**			 type: input/output
**			 detail : this is a diameter context handle. 
**
**		 DMBase_SList** ppSList_io
**			type: input/output
**			detail: this is a pointer to a pointer which points
**				to start of list.
**
**		 void* pvData_i
**			type: input
**			detail: this is a pointer to data which is to be appended
**				to list.
** Return:
**		An t_DiamAPIReturnVal value specify whether data appended to list    
**		successfully or any problem occured. 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal Rf_AddPSInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
														  HAVP					  	hToAVP_i,
														  t_PSInformation*			poPSInfoCDRContent_o)

{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");
		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}
        
		if (NULL == poPSInfoCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poPSInfoCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

        HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
        t_DiamEnum*		      poTempEnum     	   = NULL;
		t_DiamOctetString*    poTempOctetstr   	   = NULL;
		unsigned int  		  unAVPCode			   =  0;
		DMBase_SList*	      pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
		unsigned int 		  unDestinationStrSize = 0;
		int 				  nItemNumber		   = 0;
		  while( NULL != pCurrentNode )
		  {
					pvNodeData  = NULL;
					if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
																		   pCurrentNode ,&pvNodeData ) )
					{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
							return Offline_CS_RetVal_Failure;
					}
	
					hToAVP      = (HAVP)pvNodeData;
					unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
					switch( unAVPCode )
					{
							case OFFLINE_CS_3GPP_CHARGING_ID:          		//2//OctetString
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-Charging-Id AVP Found\n");
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->uc3GPPChargingId,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding 3GPP-Charging-Id AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							case OFFLINE_CS_3GPP_PDP_TYPE:
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-PDP-Type AVP Found\n");
									// type unknown
									break;
							case OFFLINE_CS_PDP_ADDRESS:
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PDP-Address AVP Found\n");
									unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->ucPDPAddress,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding PDP-Address AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							case OFFLINE_CS_3GPP_GPRS_NEGOTIATED_QOS_PROFILE:  
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"3GPP-GPRS-Negotiated-QoS-Profile AVP Found\n");
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->uc3GPPGPRSNegotiatedQoSProfile,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding 3GPP-GPRS-Negotiated-QoS-Profile AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							case OFFLINE_CS_SGSN_ADDRESS:
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"SGSN-Address AVP Found\n");
									unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->ucSGSNAddress,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding SGSN-Address AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							case OFFLINE_CS_GGSN_ADDRESS:
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"GGSN-Address AVP Found\n");
									unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->ucGGSNAddress,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding GGSN-Address AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							case OFFLINE_CS_CG_ADDRESS:
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CG-Address AVP Found\n");
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->ucCGAddress,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding CG-Address AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
                                    break;
							case OFFLINE_CS_3GPP_MS_TIMEZONE: //octetString
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-MS-Timezone AVP Found\n");
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->uc3GPPMsTimeZone,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding 3GPP-MS-Timezone AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
                                    break;
							case OFFLINE_CS_CHARGING_RULE_BASE_NAME://octetString, MAY OCCUR IN LIST
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									nItemNumber = poPSInfoCDRContent_o->oChargingRuleBasedNameList.nNumberOfItemsInList;
									if( OFFLINE_CS_LIST_SIZE <= nItemNumber )
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No More Space in Rule-Based-Name List to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									if( poTempOctetstr->unNumOfOctets < (unDestinationStrSize-1) )
									{
										memcpy( &(poPSInfoCDRContent_o->oChargingRuleBasedNameList.ucItems[nItemNumber][0]),
											    poTempOctetstr->pucOctetString ,poTempOctetstr->unNumOfOctets);
										poPSInfoCDRContent_o->oChargingRuleBasedNameList.ucItems[nItemNumber][poTempOctetstr->unNumOfOctets] = '\0';
										poPSInfoCDRContent_o->oChargingRuleBasedNameList.nNumberOfItemsInList++;
									}
									else
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Destination Buffer size is too Small,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Charging-Rule-Base-Name AVP Found\n");
									break;
							case OFFLINE_CS_3GPP_IMSI_MCC_MNC:  //UNKNOWN TYPE WILL SEE IN 207
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-IMSI-MCC-MNC Empty case \n");
									// unknown
									break;
							case OFFLINE_CS_3GPP_GGSN_MCC_MNC:  
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									// unknown
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-GGSN-MCC-MNC AVP Found \n");
									break;
							case OFFLINE_CS_3GPP_NSAPI:
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									// unknown
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-NSAPI AVP Found \n");
									break;
							case OFFLINE_CS_CALLED_STATION_ID://UTF8String
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
                                    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Called-Station-Id AVP found \n");
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSInfoCDRContent_o->ucCalledStationId,
																								   unDestinationStrSize,
																								   poTempOctetstr->pucOctetString,
																								   poTempOctetstr->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Called-Station-Id AVP to Message structure,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							case OFFLINE_CS_3GPP_SESSION_STOP_INDICATOR:
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									//unknown
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-Session-Stop-Indicator  AVP found \n");
									break;
							case OFFLINE_CS_3GPP_SELECTION_MODE:    
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									//unknown
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-selection-Mode AVP found \n");
									break;
							case OFFLINE_CS_3GPP_CHARGING_CHARACTERISTICS:
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									//unknown
								    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-Charging-Characteristics AVP found \n");
									break;
							case OFFLINE_CS_3GPP_SGSN_MCC_MNC://octetString
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									//unknown
									 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-SGSN-MCC-MNC AVP found \n");
									 break;
							case OFFLINE_CS_3GPP_USER_LOCATION_INFO://SEE 207 FOR TYPE 
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-User-Location-info AVP Empty case \n");
									//unknown
									break;
							case OFFLINE_CS_3GPP_RAT_TYPE: 
									poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
									//unknown
									 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-Rat-Type AVP found \n");
									 break;
								case OFFLINE_CS_PS_FURNISH_CHARGING_INFORMATION://GROUPED
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"PS-Furnish-Charging-Information Grouped AVP Found\n");
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									if (Offline_CS_RetVal_Success != RF_AddPSFurnishChargingInfoAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																													   &(poPSInfoCDRContent_o->oPSFurnishingChargingInfo)) )
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding PS-Furnishing-Charging-Information AVP to Message structure, Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
                                    break;
							case OFFLINE_CS_PDP_CONTEXT_TYPE://ENUM
									poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                    poPSInfoCDRContent_o->nPDPContextType = *(poTempEnum->pnInteger32);
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PDP-Context-Type AVP found \n");
									break;
							case OFFLINE_CS_OFFLINE_CHARGING:
                                    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Offline-charging grouped AVP found \n");
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									if (Offline_CS_RetVal_Success != RF_AddOfflineChargingAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																												 &( poPSInfoCDRContent_o->oOfflineCharging )) )
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Offline-charging AVP to Message structure, Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							default:
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in PS-Information AVP, leaving\n");
	
					}//End switch( unAVPCode )
					DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
		   }// end while( NULL != pCurrentNode )
	
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
		   return Offline_CS_RetVal_Success;

 }



t_Offline_CS_ReturnVal RF_AddOfflineChargingAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																   HAVP					  	hToAVP_i,
																   t_OfflineCharging*		poOfflineChargingCDRContent_o)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        
		if (NULL == poOfflineChargingCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poOfflineChargingCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

        
	   unsigned int  		unAVPCode			 =  0;
	   unsigned int 		unDestinationStrSize =  0;
       HAVP                 hToAVP               = NULL;
	   void*                pvNodeData           = NULL;
	   t_DiamEnum*		    poTempEnum     	     = NULL;
	   t_DiamUnsigned32*    poTempUnsigned32   	 = NULL;
	   DMBase_SList*	    pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_QUOTA_CONSUMPTION_TIME:		//881 //Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poOfflineChargingCDRContent_o->unQuotaConsumptionTime = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Quota-Consumption-Time AVP found \n");
                              break;

						case OFFLINE_CS_TIME_QUOTA_MECHANISM:		//1270//Grouped
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Time-Quota-Mechanism Grouped  AVP found \n");
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if (Offline_CS_RetVal_Success != RF_AddTimeQuotaMechnicmAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																											  &( poOfflineChargingCDRContent_o->oTimeQuotaMechanism )) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Time-Quota-Mechanism AVP to Message structure, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;

						case OFFLINE_CS_ENVELOP_REPORTING:		//1268//Enum
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Envelop-Reporting  AVP found \n");
								poOfflineChargingCDRContent_o->nEnvlopeReporting = *(poTempEnum->pnInteger32);
                                break;

						case OFFLINE_CS_MULTIPLE_SERVICES_CREDIT_CONTROL: // 456//Grouped
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Multiple-Services-Credit-Control Grouped AVP found \n");
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if (Offline_CS_RetVal_Success != RF_AddMultipleServicesCreditControlAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																														   &( poOfflineChargingCDRContent_o->oMultiServicesCrdtControlList )) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Multiple-Services-Credit-Control AVP to Message structure, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;

                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in PS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
 }




t_Offline_CS_ReturnVal RF_AddMultipleServicesCreditControlAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 					hRFModuleContext_i,
																				 HAVP					  				hToAVP_i,
																				 t_MultipleServicesCreditControlList*	poMultiServCreditControlListCDRContent_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        
		if (NULL == poMultiServCreditControlListCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poMultiServCreditControlListCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

	   unsigned int  		unAVPCode			 =  0;
	   unsigned int 		unDestinationStrSize =  0;
	   HAVP                 hToAVP               = NULL;
	   void*                pvNodeData           = NULL;
	   t_DiamEnum*		    poTempEnum     	     = NULL;
	   t_DiamUnsigned32*    poTempUnsigned32   	 = NULL;
	   t_DiamOctetString*	poTempOctetstr 		 = NULL;
	   DMBase_SList*	    pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
	   int 					nIndex				 = 0;
	   int 					nItemNumber			 = 0;

	   if( OFFLINE_CS_LIST_SIZE <= poMultiServCreditControlListCDRContent_o->nNumberOfItemsInList )
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No more space in Multiple-Services-Credit-control List, leaving\n");
		   return Offline_CS_RetVal_Failure;
	   }
	   nIndex = poMultiServCreditControlListCDRContent_o->nNumberOfItemsInList;

	   while( NULL != pCurrentNode )
	   {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_GRANTED_SERVICE_UNIT:	 //431// Grouped not present in 299 doc 
							 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Granted-Service-Unit Grouped  AVP empty case \n");	
                             break;

						case OFFLINE_CS_REQUESTED_SERVICE_UNIT:	//437//Grouped not present in 299 doc 
							 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Requested-Service-unit Grouped AVP Empty case \n");	 
							   break;

						case OFFLINE_CS_USED_SERVICE_UNIT:	 //446//Grouped
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Used-Srvice-Unit Grouped  AVP found \n");
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if (Offline_CS_RetVal_Success != RF_AddUsedServiceUnitAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																											  &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfUsedServiceUnit )) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Used-Srvice-Unit AVP to Message structure, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;
						case OFFLINE_CS_TARIFF_CHANGE_USAGE:	//432//Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].nTraffChangeUsage = *(poTempUnsigned32->punUnsigned32); 
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Tariff-change-Usage AVP found \n");
                                break;
						case OFFLINE_CS_SERVICE_IDENTIFIER: 	//439//Unsigned32,MAY CONTAIN LIST
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2," Service-Identifier AVP found \n");
								// will process as it is list
                                break;
						case OFFLINE_CS_RATING_GROUP:	//432//Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unRatingGroup = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2," Rating-Group AVP found \n");
                                break;
						case OFFLINE_CS_G_S_U_POOL_REFERENCE:	//457//GROUPED,MAY OCCUR IN LIST
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"G-S-U-Pool-Reference AVP found \n");
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if (Offline_CS_RetVal_Success != RF_AddGSUPoolReferenceAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																											  &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfGSUPoolreference )) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding G-S-U-Pool-Reference AVP to Message structure, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;
						case OFFLINE_CS_VALIDITY_TIME: //448//Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unValidityTime = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Validity-Time AVP found \n");
                                break;
								case DMBASE_AVP_RESULT_CODE: 
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unResultCode = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Result-Code AVP found \n");
                                break;
								
						case OFFLINE_CS_FINAL_UNIT_INDICATION: //430	//GROUPED
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Final-Unit-Indication Grouped  AVP found \n");
								if (Offline_CS_RetVal_Success != RF_AddFinalUnitIndicationAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																											  &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oFinalUnitIndication )) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Final-Unit-Indication AVP to Message structure, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;
						case OFFLINE_CS_TIME_QUOTA_THRESHOLD:	//868//Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unTimeQuotaThreshold = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Time-Quota-Threshold AVP found \n");
                                break;
						case OFFLINE_CS_VOLUME_QUOTA_THRESHOLD: //869//Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unVolumeQuotaThreshold = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Volume-Quota-Threshold AVP found \n");
                                break;
						case OFFLINE_CS_UNIT_QUOTA_THRESHOLD:	   //1226//Unsigned32
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unUnitQuotaThreshold = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Unit-Quota-Threshold AVP found \n");
                                break;
						case OFFLINE_CS_QUOTA_HOLDING_TIME: 		//871//Unsigned32 
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unQuotaHoldingTime = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Quota-Holding-Time AVP found \n");
                                break;
						case OFFLINE_CS_QUOTA_CONSUMPTION_TIME: 
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poMultiServCreditControlListCDRContent_o->oItems[nIndex].unQuotaConsumptionTime = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Quota-Consumption-Time AVP found \n");
                                break;
						case OFFLINE_CS_REPORTING_REASON: 	//872 Enumerated // MAY OCCUR IN LIST
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								nItemNumber = poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfReportingReason.nNumberOfItemsInList;
								if( OFFLINE_CS_LIST_SIZE <= nItemNumber)
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No More Sapce in Reporting-Reason List,Leaving\n");
									return Offline_CS_RetVal_Failure;
								}
								poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfReportingReason.nItems[nItemNumber]= *(poTempEnum->pnInteger32);
								poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfReportingReason.nNumberOfItemsInList++;
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Reporting-Reason AVP found \n");
                                break;
						case OFFLINE_CS_TRIGGER:	//1264//Grouped
							   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Trigger Grouped AVP found \n");
							   if (Offline_CS_RetVal_Success != RF_AddTriggerAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																									&( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oTrigger )) )
							   {
								   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Trigger AVP to Message structure, Leaving \n");
								   return Offline_CS_RetVal_Failure;
							   }
							   break;
					case OFFLINE_CS_PS_FURNISH_CHARGING_INFORMATION: //865//GROUPED
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PS-Furnish-Charging-Info Grouped AVP found \n");
						if (Offline_CS_RetVal_Success != RF_AddPSFurnishChargingInfoAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																							 &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oPSFurnishingChargingInfo )) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding PS-Furnish-Charging-Info AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
					case OFFLINE_CS_REFUND_INFORMATION: //1292//OctetString 
						poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Refund-Information AVP found \n" );
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
                        if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMultiServCreditControlListCDRContent_o->oItems[nIndex].ucRefundInformation,
																					   unDestinationStrSize,
																					   poTempOctetstr->pucOctetString,
																					   poTempOctetstr->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding 3GPP-MS-Timezone AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
					case OFFLINE_CS_AF_CORRELATION_INFORMATION:// 1276//Grouped ,MAY OCCUR IN LIST
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"AF-Correlation-Information grouped AVP\n" );
						if (Offline_CS_RetVal_Success != RF_AddAFCorrelationInfoAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																									   &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfAFCorrelationInfo )) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding AF-Correlation-Information AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
					case OFFLINE_CS_ENVELOP:	//1266// Grouped MAY OCCUR IN LIST
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Envelop Grouped AVP found \n");
						if (Offline_CS_RetVal_Success != RF_AddEnvelopeAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																							  &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfEnvelope)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Envelop AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
					case OFFLINE_CS_ENVELOP_REPORTING:// 1268//Enumerated 
						poTempEnum = (t_DiamEnum*)hToAVP->pData;
						poMultiServCreditControlListCDRContent_o->oItems[nIndex].nEnvelopeReporting = *(poTempEnum->pnInteger32);
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Envelop-Reporting AVP found \n");
						break;
					case OFFLINE_CS_TIME_QUOTA_MECHANISM:	//1270// Grouped
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Time-Quota-Mechanism Grouped AVP found \n" );
						if (Offline_CS_RetVal_Success != RF_AddTimeQuotaMechnicmAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																							  &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oTimeQuotaMechanism)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Time-Quota-Mechanism AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
					case OFFLINE_CS_SERVICE_SPECIFIC_INFO: //1249//Grouped,MAY OCCUR IN LIST
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Service-Specific-Info Grouped AVP found \n" );
						if (Offline_CS_RetVal_Success != RF_AddServiceSpecificInfoAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																										 &( poMultiServCreditControlListCDRContent_o->oItems[nIndex].oListOfServiceSpecificInfo)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Service-Specific-Info AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
					default:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Multiple-Services-Credit-control Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i ->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

	   poMultiServCreditControlListCDRContent_o->nNumberOfItemsInList++;
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
  }


t_Offline_CS_ReturnVal RF_AddUsedServiceUnitAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 					hRFModuleContext_i,
																   HAVP					  					hToAVP_i,
																   t_UsedServiceUnitList*					poUsedServiceUnitListCDRContent_o)

{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Null_Pointer;
		}
	
		if (NULL == hToAVP_i)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Null_Pointer;
		}
	
		if (NULL == poUsedServiceUnitListCDRContent_o)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poUsedServiceUnitListCDRContent_o is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Invalid_Pointer;
		}

		 unsigned int  		   unAVPCode			=  0;
		 HAVP                  hToAVP               = NULL;
		 void*                 pvNodeData           = NULL;
		 t_DiamEnum*		   poTempEnum     	    = NULL;
		 t_DiamUnsigned64*     poTempUnsigned64   	= NULL;
		 t_DiamUnsigned32*     poTempUnsigned32   	= NULL;
		 DMBase_SList*	       pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
		 int nItemNumber =0;

		 int nIndex = poUsedServiceUnitListCDRContent_o->nNumberOfItemsInList;
		 if( OFFLINE_CS_LIST_SIZE <= nIndex )
		 {
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," No More Space in UsedServiceUnit List , leaving\n");
			 return Offline_CS_RetVal_Invalid_Pointer;
		 }
		 

		 while( NULL != pCurrentNode )
		 {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_REPORTING_REASON: //872//Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                poUsedServiceUnitListCDRContent_o->oItems[nIndex].nReportingReason = *(poTempEnum->pnInteger32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Reporting-Reason AVP found \n" );
								break;

						case OFFLINE_CS_TARIFF_CHANGE_USAGE:  //452//Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                poUsedServiceUnitListCDRContent_o->oItems[nIndex].nTariffChangeUsage = *(poTempEnum->pnInteger32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Tariff-Change-Usage AVP found \n" );
								break;

						case OFFLINE_CS_CC_TIME:	
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poUsedServiceUnitListCDRContent_o->oItems[nIndex].unCCTime = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-Time AVP found \n" );
                                break;
						case OFFLINE_CS_CC_TOTAL_OCTETS:	//421//Unsigned64
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-Total-Octets AVP found \n" );
                                break;

						case OFFLINE_CS_CC_INPUT_OCTETS:  //412//Unsigned64
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-Input-Octets AVP found \n" );
                                break;
						case OFFLINE_CS_CC_OUTPUT_OCTETS:  //414//Unsigned64
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-Output-Octets AVP found \n" );
                                break;
						case OFFLINE_CS_CC_SERVICE_SPECIFIC_UNITS: //417//Unsigned64
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Service-Specific-Units AVP found \n" );
                                break;
						case OFFLINE_CS_EVENT_CHARGING_TIMESTAMP: // 1258//Time, may contain list
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								nItemNumber = poUsedServiceUnitListCDRContent_o->oItems[nIndex].oListOfEventChargingTimeStamp.nNumberOfItemsInList;
								if( OFFLINE_CS_LIST_SIZE <= nItemNumber )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"No More Space in Event-Cahrging-TimeStamp List \n" );
									return Offline_CS_RetVal_Failure;
								}
								poUsedServiceUnitListCDRContent_o->oItems[nIndex].oListOfEventChargingTimeStamp.unItems[nItemNumber]= *(poTempUnsigned32->punUnsigned32);
								poUsedServiceUnitListCDRContent_o->oItems[nIndex].oListOfEventChargingTimeStamp.nNumberOfItemsInList++;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Event-Charging-TimeStamp AVP found \n" );
                                break;

                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in PS-Information AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
		 }// end while( NULL != pCurrentNode )

		 poUsedServiceUnitListCDRContent_o->nNumberOfItemsInList++;
		 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
		 return Offline_CS_RetVal_Success;

 }


t_Offline_CS_ReturnVal RF_AddGSUPoolReferenceAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 					hRFModuleContext_i,
																	HAVP					  				hToAVP_i,
																	t_GSUPoolReferenceList*					poGSUPoolReferenceListCDRContent_o)
{
	// The group members of this avp is not defined in 299 doc, but some of the members given below
	// are taken from some where esle but not sure about that wether correct or incorrect. so is not filling 
	// in cdr contents
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Null_Pointer;
		}
	
		if (NULL == hToAVP_i)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Null_Pointer;
		}
    
		if (NULL == poGSUPoolReferenceListCDRContent_o)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poGSUPoolReferenceListCDRContent_o is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Invalid_Pointer;
		}
     
		unsigned int  			unAVPCode			 =  0;
		HAVP                  	hToAVP               = NULL;
		void*                 	pvNodeData           = NULL;
        t_DiamEnum*		    	poTempEnum     	     = NULL;
		t_DiamUnsigned32*    	poTempUnsigned32   	 = NULL;
		DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

		int nIndex = poGSUPoolReferenceListCDRContent_o->nNumberOfItemsInList;
		
		if( OFFLINE_CS_LIST_SIZE <= nIndex )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," No more in GSUPoolReference List, leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		
		  while( NULL != pCurrentNode )
		  {
					pvNodeData  = NULL;
					if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
																		   pCurrentNode ,&pvNodeData ) )
					{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
							return Offline_CS_RetVal_Failure;
					}
	
					hToAVP      = (HAVP)pvNodeData;
					unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
					switch( unAVPCode )
					{
							case OFFLINE_CS_G_S_U_POOL_IDENTIFIER:	//453//Unsigned32
									poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
									poGSUPoolReferenceListCDRContent_o->oItems[nIndex].unGSUPoolIdentifier = *(poTempUnsigned32->punUnsigned32);
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"G-S-U-Pool-Identifier AVP found \n" );
									break;
	
							 case OFFLINE_CS_CC_UNIT_TYPE: //454//Enumerated
									poTempEnum = (t_DiamEnum*)hToAVP->pData;
									poGSUPoolReferenceListCDRContent_o->oItems[nIndex].nCCUnitType = *(poTempEnum->pnInteger32);
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-unit-Type AVP found \n" );
									break;
	
							case OFFLINE_CS_UNIT_VALUE:  //445 //Grouped
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Unit-Value Grouped AVP found \n" );
								if (Offline_CS_RetVal_Success != RF_AddUnitValueAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																									   &( poGSUPoolReferenceListCDRContent_o->oItems[nIndex].oUnitValue)) )
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Unit-Value AVP to Message structure, Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;
							default:
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in PS-Information AVP, leaving\n");
	
					}//End switch( unAVPCode )
					DMBase_SListGetNextNode( hRFModuleContext_i ->hDiameterStackContext,pCurrentNode,&pCurrentNode);
		   }// end while( NULL != pCurrentNode )

		  poGSUPoolReferenceListCDRContent_o->nNumberOfItemsInList++;
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
		  return Offline_CS_RetVal_Success;
 }



t_Offline_CS_ReturnVal RF_AddUnitValueAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
															 HAVP					  	hToAVP_i,
															 t_UnitValue*				poUnitValueCDRContent_o)
{
	// sinece this is member of G-S-U-Pool reference avp 
	// The group members of G-S-U-Pool reference avp is not defined in 299 doc, but some of the members given below
	// are taken from some where esle but not sure about that wether correct or incorrect. so is not filling 
	// in cdr contents , so is the case with this
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Null_Pointer;
		}
	
		if (NULL == hToAVP_i)
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Null_Pointer;
		}
    
		if ( NULL == poUnitValueCDRContent_o )
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poUnitValueCDRContent_o is NULL pointer , leaving\n");
			 return Offline_CS_RetVal_Invalid_Pointer;
		}
     
        unsigned int  			unAVPCode			 =  0;
        HAVP                  	hToAVP               = NULL;
        void*                 	pvNodeData           = NULL;
        _Bool                 	bConvToNetOrder      = 0;
        t_DiamInteger32*    	poTempInteger32   	 = NULL;
        t_DiamUnsigned64*    	poTempUnsigned64   	 = NULL;
        DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_EXPONENT:			//429//Integer32
								poTempInteger32 = (t_DiamInteger32*)hToAVP->pData;
                                poUnitValueCDRContent_o->nExponent = *(poTempInteger32->pnInteger32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Exponent AVP found \n" );
								break;

						case OFFLINE_CS_VALUE_DIGIT:	//447//Integer64
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Value-Digit AVP found \n" );
								break;

						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Unit-Value AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
	
}

  
t_Offline_CS_ReturnVal RF_AddFinalUnitIndicationAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																	   HAVP					  		hToAVP_i,
																	   t_FinalUnitIndication*		poFinalUnitIndicationCDRContent_o)
{
	// The group members of this reference avp is not defined in 299 doc, but some of the members given below
	// are taken from some where esle but not sure about that wether correct or incorrect. so is not filling 
	// in cdr contents 

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == poFinalUnitIndicationCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poFinalUnitIndicationCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}
  
       
	   unsigned int  	    unAVPCode			 =  0;
	   HAVP                 hToAVP               = NULL;
	   void*                pvNodeData           = NULL;
	   t_DiamEnum*		    poTempEnum     	     = NULL;
	   t_DiamOctetString*	poTempOctetstr 		 = NULL;
	   DMBase_SList*	    pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
	   unsigned int 		unDestinationStrSize = 0;

	   while( NULL != pCurrentNode )
	   {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {		// all these group members are get from any other message
						case OFFLINE_CS_FINAL_UNIT_ACTION:  //449//Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                poFinalUnitIndicationCDRContent_o->nFinalUnitAction = *(poTempEnum->pnInteger32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Final-Unit-Action AVP found \n" );
								break;

						case OFFLINE_CS_RESTRICTION_FILTER_RULE:	 //438 //IPFilterRule
								poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								/*if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poFinalUnitIndicationCDRContent_o->,
																							   unDestinationStrSize,
																							   poTempOctetstr->pucOctetString,
																							   poTempOctetstr->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Restriction-Filter_Rule AVP to Message structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Restriction-Filter-Rule AVP found \n" );
								break;
						case OFFLINE_CS_FILTER_ID: //11//UTF8String
								poTempOctetstr = (t_DiamOctetString*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Filter-Id AVP found \n" );
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poFinalUnitIndicationCDRContent_o->ucFilterId,
																							   unDestinationStrSize,
																							   poTempOctetstr->pucOctetString,
																							   poTempOctetstr->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Filter-Id AVP to Message structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;

						case OFFLINE_CS_REDIRECT_SERVER: //434//Grouped
							  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Redirect-Server Grouped AVP found \n" );
							  unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
							  if (Offline_CS_RetVal_Success != RF_AddRedirectServerAVPToCdrContentsStruct( hRFModuleContext_i,hToAVP,
																										 &(poFinalUnitIndicationCDRContent_o->oRedirectserver)) )
							  {
								  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Redirect-Server AVP to Message structure, Leaving \n");
								  return Offline_CS_RetVal_Failure;
							  }
                              break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Unit-Value AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success;
 }

t_Offline_CS_ReturnVal RF_AddRedirectServerAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																  HAVP					  		hToAVP_i,
																  t_RedirectServer*				poRedirectServerCDRContent_o)
{
	// The group members of this reference avp is not defined in 299 doc, but some of the members given below
	// are taken from some where esle but not sure about that wether correct or incorrect. so is not filling 
	// in cdr contents 
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == poRedirectServerCDRContent_o )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poRedirectServerCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}
     
	   unsigned int  	unAVPCode				 =  0;
	   unsigned int 	unDestinationStrSize     =  0;

       HAVP                  hToAVP               = NULL;
       void*                 pvNodeData           = NULL;
       t_DiamEnum*		     poTempEnum     	  = NULL;
       t_DiamUTF8String*	 poTempUTF8 		  = NULL;
       DMBase_SList*	     pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_REDIRECT_ADDRESS_TYPE:	//433//Enumerated
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                poRedirectServerCDRContent_o->nRedirectAddressType = *(poTempEnum->pnInteger32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Redirect-Address-Type AVP found \n" );
								break;
						case OFFLINE_CS_REDIRECT_SERVER_ADDRESS:   	//435//UTF8String
								poTempUTF8 = (t_DiamOctetString*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Redirect-Server-Address AVP found \n" );
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poRedirectServerCDRContent_o->pucRedirectServerAddress,
																							   unDestinationStrSize,
																							   poTempUTF8->pucOctetString,
																							   poTempUTF8->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Redirect-Server-Address AVP to Message structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Redirect-Server AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success; 
}


t_Offline_CS_ReturnVal RF_AddTriggerAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
														   HAVP					  		hToAVP_i,
														   t_Trigger*					poTriggerCDRContent_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		 if (NULL == hRFModuleContext_i)
		 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				 return Offline_CS_RetVal_Null_Pointer;
		 }
	
		 if (NULL == hToAVP_i)
		 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				 return Offline_CS_RetVal_Null_Pointer;
		 }
	
		 if (NULL == poTriggerCDRContent_o )
		 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poTriggerCDRContent_o is NULL pointer , leaving\n");
				 return Offline_CS_RetVal_Invalid_Pointer;
		 }

         unsigned int  			unAVPCode			 =   0;
		  HAVP                  hToAVP               = NULL;
		  void*                 pvNodeData           = NULL;
		  t_DiamEnum*		    poTempEnum     	     = NULL;
		  t_DiamUTF8String*		poTempUTF8 		 	 = NULL;
		  DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
		  int 					nIndex				 = 0;

	  while( NULL != pCurrentNode )
	  {
				pvNodeData  = NULL;
				if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
																	   pCurrentNode ,&pvNodeData ) )
				{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
						return Offline_CS_RetVal_Failure;
				}
	
				hToAVP      = (HAVP)pvNodeData;
				unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
				switch( unAVPCode )
				{
						case OFFLINE_CS_TRIGGER_TYPE:			//870//ENUM, MAY OCCUR IN LIST
							poTempEnum = (t_DiamEnum*)hToAVP->pData;
							if( OFFLINE_CS_LIST_SIZE <= poTriggerCDRContent_o->oListofTriggerType.nNumberOfItemsInList )
							{
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "No More Space in Trigger-Type List , Leaving \n");
								return Offline_CS_RetVal_Failure;
							}
							nIndex = poTriggerCDRContent_o->oListofTriggerType.nNumberOfItemsInList;
							poTriggerCDRContent_o->oListofTriggerType.nItems[nIndex]= *(poTempEnum->pnInteger32);
							poTriggerCDRContent_o->oListofTriggerType.nNumberOfItemsInList++;
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Trigger-Type AVP found \n" );
							break;
	
						default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP Received in Trigger-Type AVP, leaving\n");
	
				}//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
	   }// end while( NULL != pCurrentNode )
	
	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
	   return Offline_CS_RetVal_Success;
}


t_Offline_CS_ReturnVal RF_AddPSFurnishChargingInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 			hRFModuleContext_i,
																		 HAVP					  		hToAVP_i,
																		 t_PSFurnishingChargingInfo*	poPSFurnishingChargingInfoCDRContent_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		 if (NULL == hRFModuleContext_i)
		 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				 return Offline_CS_RetVal_Null_Pointer;
		 }
	
		 if (NULL == hToAVP_i)
		 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				 return Offline_CS_RetVal_Null_Pointer;
		 }
	
		 if (NULL == poPSFurnishingChargingInfoCDRContent_o )
		 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poPSFurnishingChargingInfoCDRContent_o is NULL pointer , leaving\n");
				 return Offline_CS_RetVal_Invalid_Pointer;
		 }

         unsigned int  	     unAVPCode			  =  0;
         HAVP                hToAVP               = NULL;
		 void*               pvNodeData           = NULL;
         t_DiamEnum*		 poTempEnum     	  = NULL;
		 t_DiamOctetString*	 poTempOctetstring 	  = NULL;
		 DMBase_SList*	     pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
		 unsigned int 		 unDestinationStrSize = 0;

      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {

						case OFFLINE_CS_3GPP_CHARGING_ID:          		//2//OctetString
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"3GPP-Charging-Id AVP Found\n");
								poTempOctetstring = (t_DiamOctetString*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSFurnishingChargingInfoCDRContent_o->uc3GPPChargingId,
																							   unDestinationStrSize,
																							   poTempOctetstring->pucOctetString,
																							   poTempOctetstring->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding 3GPP-Charging-Id AVP to Message structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;
						case OFFLINE_CS_PS_APPEND_FREE_FORMAT_DATA:		//867//enumerated 
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
                                poPSFurnishingChargingInfoCDRContent_o->nPSAppendFreeFormatData = *(poTempEnum->pnInteger32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"PS-Append-Free-Format-Data AVP Found\n");
								break;
						case OFFLINE_CS_PS_FREE_FORMAT_DATA:	 		//866//OctetString
								poTempOctetstring = (t_DiamOctetString*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"PS-Free-Format-Data AVP Found \n");
                                unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poPSFurnishingChargingInfoCDRContent_o->ucPSFreeFormatData,
																							   unDestinationStrSize,
																							   poTempOctetstring->pucOctetString,
																							   poTempOctetstring->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding PS-Free-Format-Data AVP to Message structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Redirect-Server AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext, pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success; 
	}

t_Offline_CS_ReturnVal RF_AddAFCorrelationInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 				hRFModuleContext_i,
																	 HAVP					  			hToAVP_i,
																	 t_AFCorrelationInformationList*	poAFCorrelationInfoListCDRContent_o){
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == poAFCorrelationInfoListCDRContent_o )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poAFCorrelationInfoListCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

	   unsigned int  		 unAVPCode				  =  0;
       HAVP                  hToAVP                   = NULL;
	   void*                 pvNodeData               = NULL;
	   t_DiamUnsigned32*	 poTempUnsigned32         = NULL;
	   t_DiamUnsigned64*	 poTempUnsigned64         = NULL;
       DMBase_SList*	     pCurrentNode             = hToAVP_i->hCollectionOfAVPs;

	   int nIndex = poAFCorrelationInfoListCDRContent_o->nNumberOfItemsInList ;

	   if(OFFLINE_CS_LIST_SIZE <=  nIndex)
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No More Sapce in AFCorrelationInfo List , leaving\n");
		   return Offline_CS_RetVal_Failure;
	   }


      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_AF_CHARGING_IDENTIFIER:
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"AF-Charging-Identifier AVP Empty Case\n");
							// unknown
							break;
						case OFFLINE_CS_FLOWS:
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Flows  AVP Empty Case\n");	
							// unknown
							break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Redirect-Server AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

	  poAFCorrelationInfoListCDRContent_o->nNumberOfItemsInList++ ;
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success; 

	/*#define OFFLINE_CS_AF_CHARGING_IDENTIFIER     505//type will check from 214
	#define OFFLINE_CS_FLOWS		      			510//MAY OCCUR IN LIST,type will check from 214*/
}



t_Offline_CS_ReturnVal RF_AddEnvelopeAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
															HAVP					  	hToAVP_i,
															t_EnvelopeList*				poEnvelopeListCDRContent_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");


		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == poEnvelopeListCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poEnvelopeListCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}
     
       unsigned int  			unAVPCode			 =  0;
	   HAVP                  	hToAVP               = NULL;
	   void*                 	pvNodeData           = NULL;
       t_DiamUnsigned32*		poTempUnsigned32     = NULL;
	   t_DiamUnsigned64*		poTempUnsigned64     = NULL;
       DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

	   int 	nIndex	= poEnvelopeListCDRContent_o->nNumberOfItemsInList;

	   if( OFFLINE_CS_LIST_SIZE <= nIndex )
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poEnvelopeListCDRContent_o is NULL pointer , leaving\n");
		   return Offline_CS_RetVal_Failure;
	   }
	   
	   while( NULL != pCurrentNode )
	   {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_ENVELOP_START_TIME:		//1269 //Time
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Envepol-start-Time AVP Found\n");
								poEnvelopeListCDRContent_o->oItems[nIndex].unEnvelopeStartTime = *(poTempUnsigned32->punUnsigned32);
								break;
						case OFFLINE_CS_ENVELOP_END_TIME:			//1267 //Time
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								poEnvelopeListCDRContent_o->oItems[nIndex].unEnvelopeEndTime = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Envelop-End-Time AVP Found\n");
								break;
                        case OFFLINE_CS_CC_TOTAL_OCTETS:			//421//Unsigned64
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-Total-Time AVP Found\n");
								break;
						case OFFLINE_CS_CC_INPUT_OCTETS:  		//412//Unsigned64 
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-Input-Octets AVP Found\n");
								break;
						case OFFLINE_CS_CC_OUTPUT_OCTETS:			//414//Unsigned64 
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"CC-Output-Octets AVP Found\n");
								break;
						case OFFLINE_CS_CC_SERVICE_SPECIFIC_UNITS:  //417//Unsigned64
								poTempUnsigned64 = (t_DiamUnsigned64*)hToAVP->pData;
                                
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Service-Specific-unit AVP Found\n");
								break;

                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Redirect-Server AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

	   poEnvelopeListCDRContent_o->nNumberOfItemsInList++;
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success; 
 }

t_Offline_CS_ReturnVal RF_AddTimeQuotaMechnicmAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																	 HAVP					  	hToAVP_i,
																	 t_TimeQuotaMechanism*		poTimeQuotaMechanismCDRContent_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");


		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == poTimeQuotaMechanismCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poTimeQuotaMechanismCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}
       
	    unsigned int      	  unAVPCode			  =  0;
        HAVP                  hToAVP               = NULL;
		void*                 pvNodeData           = NULL;
        t_DiamEnum*		      poTempEnum     	   = NULL;
		t_DiamUnsigned32*	  poTempUnsigned32 	   = NULL;
		DMBase_SList*	      pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_TIME_QUOTA_TYPE:		//1271//Enumerated 
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Time-Quota-Type AVP Found\n");
								poTimeQuotaMechanismCDRContent_o->nTimeQuotaType = *(poTempEnum->pnInteger32);
								break;
                        
						case OFFLINE_CS_BASE_TIME_INTERVAL:		//1265//Unsigned32.
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poTimeQuotaMechanismCDRContent_o->unBaseTimeInterval = *(poTempUnsigned32->punUnsigned32); ;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Time-Quota-Interval AVP Found\n");
                                break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Redirect-Server AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return Offline_CS_RetVal_Success; 
	}


t_Offline_CS_ReturnVal RF_AddServiceSpecificInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																	   HAVP					  		hToAVP_i,
																	   t_ServiceSpecificInfoList*	poServiceSpecificInfoListCDRContent_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == poServiceSpecificInfoListCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poServiceSpecificInfoListCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

       
		unsigned int  			unAVPCode			 =  0;
        HAVP                	hToAVP               = NULL;
		void*                 	pvNodeData           = NULL;
		t_DiamUnsigned32*		poTempUnsigned32     = NULL;
		t_DiamUTF8String*		poTempUTF8str	 	 = NULL;
		DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
		unsigned int 			unDestinationStrSize = 0;

		int nIndex = poServiceSpecificInfoListCDRContent_o->nNumberOfItemsInList;

		if( OFFLINE_CS_LIST_SIZE <=  nIndex)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poServiceSpecificInfoListCDRContent_o is NULL pointer , leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		while( NULL != pCurrentNode )
		{
                pvNodeData  = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentNode ,&pvNodeData ) )
                {
                     DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                     return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_SERVICE_SPECIFIC_TYPE: 	//1257//Unsigned32 
								poTempUnsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
                                poServiceSpecificInfoListCDRContent_o->oItems[nIndex].unServiceSpecificType = *(poTempUnsigned32->punUnsigned32);
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Service-Specific-Type AVP Found\n");
								break;
                        
						case OFFLINE_CS_SERVICE_SPECIFIC_DATA:	//873//UTF8STRING
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poServiceSpecificInfoListCDRContent_o->oItems[nIndex].ucServiceSpecificData,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Service-Specific-Data AVP to Message structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Service-Specific-Data AVP Found\n");
								break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Service-Specific-Info AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

		poServiceSpecificInfoListCDRContent_o->nNumberOfItemsInList++;
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
		return Offline_CS_RetVal_Success; 
  }

