#include"RF_defines.h"
#include"RF_include.h"
#include"DBModule.h"
#include "offcs_app_defined_avp.h"
#include "rf_ims_info_avp_parser.h"

extern 		int  							 g_nDiamBaseTraceID ;

/******************************************************************************
** Function Name: 
**			Rf_AddIMSInfoAVPToCdrContntsStruct
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
t_Offline_CS_ReturnVal inline Rf_AddIMSInfoAVPToCdrContntsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																  HAVP					  	hToAVP_i,
																  t_IMSInformation*		poIMSInfoCDRContent_o)
                                                                                              
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

		if (NULL == poIMSInfoCDRContent_o)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poIMSInfoCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        
		if (NULL == hToAVP_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}
     
       unsigned int		unDestinationStrSize  = 0;
       unsigned int  	unAVPCode			  = 0;
	   unsigned int		unInteger			  = 0;

       t_DiamEnum*     		poEnum            = NULL;
	   t_DiamInteger32*		poInteger32 	  = NULL;
	   t_DiamUTF8String*	poTempUTF8str 	  = NULL;
	   t_DiamUnsigned32*	poTempunsigned32  = NULL;

       HAVP            	hToTempAVP   = NULL;
       void*           	pvNodeData   = NULL;
       DMBase_SList*	pCurrentNode = hToAVP_i->hCollectionOfAVPs;
	   

      while ( NULL != pCurrentNode )
      {

            pvNodeData = NULL;
			unInteger  = 0;
            if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                   pCurrentNode, &pvNodeData ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
				return Offline_CS_RetVal_Failure;
			}
            hToTempAVP = (HAVP)pvNodeData;
            unAVPCode  =  hToTempAVP->oAVPHeaderInst.unAVPCode;
            switch(unAVPCode)
            {
                    case OFFLINE_CS_NODE_FUNCTIONALITY:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Node-Functionality Event AVP found\n");
						poEnum = (t_DiamEnum*)hToTempAVP->pData;
						/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																						&unInteger,
																						*(poEnum->pnInteger32) ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}*/
						poIMSInfoCDRContent_o->nNodeFunctionality = *(poEnum->pnInteger32);
						break;
                    case OFFLINE_CS_EVENT_TYPE:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Event-Type Event AVP found\n");
						if (Offline_CS_RetVal_Success != Rf_AddEventTypeAVPToCdrContentsStruct( hRFModuleContext_i,
																								hToTempAVP,
																								&(poIMSInfoCDRContent_o->oEventType)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Event-Type AVP to Message structure , Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_ROLE_OF_NODE:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Role-of-Node AVP found\n");
						poInteger32 = (t_DiamInteger32*)hToTempAVP->pData;
						/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																						&unInteger,
																						*(poInteger32->pnInteger32) ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}*/
						poIMSInfoCDRContent_o->nRoleOfNode = *(poInteger32->pnInteger32);
						break;
                    case OFFLINE_CS_USER_SESSION_ID:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"User-Session-Id AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucUserSessionId,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding User-Session-Id AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_CALLED_PARTY_ADDRESS:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Called-Party-Address AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucCalledPartyAddress,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Called-Party-Address AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_CALLED_ASSERTED_IDENTITY:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Called-Asserted-Identity AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyOctetStringToStringList( &(poIMSInfoCDRContent_o->oCalledAssertedIdentityList),
																						 unDestinationStrSize,
																						 poTempUTF8str->pucOctetString,
																						 poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Called-Asserted-Identity AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_REQUESTED_PARTY_ADDRESS:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Requested-Party-Address AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucRequestedPartyAddress,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{	
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Requested-Party-Address AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
					case OFFLINE_CS_NUMBER_PORTABILITY_ROUTING_INFORMATION:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Number-Portability-Routing-Information AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucNumberPortabilityRoutingInfo,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Number-Portability-Routing-Information AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_CARRIER_SELECT_ROUTING_INFORMATION:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Carrier-Select-Routing-Information AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucCarrierSelectRoutingInformation,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Carrier-Select-Routing-Information AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_ALTERNATE_CHARGED_PARTY_ADDRESS:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Alternate-Charged-Party-Address AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucAlternateChargedPartyAddress,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Alternate-Charged-Party-Address AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;

                    case OFFLINE_CS_TIME_STAMPS:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Time-Stamps AVP found\n");
						if (Offline_CS_RetVal_Success != Rf_AddTimeStampsAVPToContentsStruct( hRFModuleContext_i,hToTempAVP,
																							  &(poIMSInfoCDRContent_o->oTimeStamps)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Time-Stamps  AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_CAUSE_CODE:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Cause-Code AVP found\n");
						poInteger32 = (t_DiamInteger32*)hToTempAVP->pData;
						/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																						&unInteger,
																						*(poInteger32->pnInteger32) ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}*/
						poIMSInfoCDRContent_o->nCausedCode = *(poInteger32->pnInteger32);
						break;

                    case OFFLINE_CS_IMS_CHARGING_IDENTIFIER:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"IMS-Charging-Identifier AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucIMSChargingIdentifier,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding IMS-Charging-Identifier AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_ASSOCIATED_URI:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Associated-URI AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyOctetStringToStringList( &(poIMSInfoCDRContent_o->oAssociatedURIList),
																						 unDestinationStrSize,
																						 poTempUTF8str->pucOctetString,
																						 poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Associated-URI AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_CALLING_PARTY_ADDRESS:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Calling-Party-Address AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyOctetStringToStringList( &(poIMSInfoCDRContent_o->oCallingPartyAddressList),
																						 unDestinationStrSize,
																						 poTempUTF8str->pucOctetString,
																						 poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Calling-Party-Address AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                     case OFFLINE_CS_APPLICATION_SERVER_INFORMATION:
						 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Application-Server-Information AVP found\n");
						 if (Offline_CS_RetVal_Success != Rf_AddAppServersInfoAVPToContentsStruct( hRFModuleContext_i,hToTempAVP,
																								   &(poIMSInfoCDRContent_o->oAppServerInfoList)) )
						 {
							 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Application-Server-Information AVP to Message structure, Leaving \n");
							 return Offline_CS_RetVal_Failure;
						 }
                         break;
					case OFFLINE_CS_INTER_OPERATOR_IDENTIFIER:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Inter-Operator-Identifier AVP found\n");
						if (Offline_CS_RetVal_Success != Rf_AddInterOperatorIdentifierAVPToContentsStruct( hRFModuleContext_i,hToTempAVP,
																										   &(poIMSInfoCDRContent_o->oInterOperatorIdentifierList)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Inter-Operator-Identifier AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
                    case OFFLINE_CS_MESSAGE_BODY:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Message-Body AVP found\n");
						if (Offline_CS_RetVal_Success != Rf_AddMessageBodyAVPToContentsStruct( hRFModuleContext_i,hToTempAVP,
																							   &(poIMSInfoCDRContent_o->oMessageBodyList)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding Message-Body AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;

					case OFFLINE_CS_SDP_SESSION_DESCRIPTION: 
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Session-Description AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyOctetStringToStringList( &(poIMSInfoCDRContent_o->oSDPSessionDescriptionList),
																						 unDestinationStrSize,
																						 poTempUTF8str->pucOctetString,
																						 poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Session-Description AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_BEARER_SERVICE:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Bearer-Service AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucBearerService,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Bearer-Service AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_ACCESS_NETWORK_INFORMATION:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Access-Network-Information AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucAccessNetworkInformation,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Access-Network-Information AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_IMS_COMMUNICATION_SERVICE_IDENTIFIER:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"IMS-Communication-Service-Identifier AVP found\n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucIMSCommunicationServiceIdentifier,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding IMS-Communication-Service-Identifier AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_SERVICE_ID:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Service-Id AVP found \n");
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucServiceId,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Service-Id AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
                    case OFFLINE_CS_SERVED_PARTY_ADDRESS:
						poTempUTF8str = (t_DiamUTF8String*)hToTempAVP->pData;
						unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
						if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poIMSInfoCDRContent_o->ucServedPartyAddress,
																					   unDestinationStrSize,
																					   poTempUTF8str->pucOctetString,
																					   poTempUTF8str->unNumOfOctets ))
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Served-Party-Address AVP to Message structure,Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
						break;
					case OFFLINE_CS_SDP_MEDIA_COMPONENT:
						if (Offline_CS_RetVal_Success != Rf_AddSDPMediaComponentsAVPToContentsStruct( hRFModuleContext_i,hToTempAVP,
																									  &(poIMSInfoCDRContent_o->oSDPMediaComponentsList)) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding SDPMedia-Components AVP to Message structure, Leaving \n");
							return Offline_CS_RetVal_Failure;
						}
                        break;
                
					default:
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2," Leaving \n");

            }//switch(unAVPCode)
            DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode, &pCurrentNode);

     }//end while ( NULL != pCurrentNode )

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
      return Offline_CS_RetVal_Success ;
 }

t_Offline_CS_ReturnVal inline Rf_AddEventTypeAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
															  HAVP					  	hToAVP_i,
															  t_Event_Type*				poEventTypeCDRContent_o)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if (NULL == hRFModuleContext_i)
	   {
			   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
			   return Offline_CS_RetVal_Null_Pointer;
	   }

	   if (NULL == poEventTypeCDRContent_o)
	   {
			   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poEventTypeCDRContent_o is NULL pointer , leaving\n");
			   return Offline_CS_RetVal_Null_Pointer;
	   }

	   if (NULL == hToAVP_i)
	   {
			   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
			   return Offline_CS_RetVal_Null_Pointer;
	   }


	  unsigned int			unDestinationStrSize =  0;
	  unsigned int  		unAVPCode			 =  0;
	  unsigned int 			unInteger			 =  0;
      HAVP                  hToAVP               = NULL;
      void*                 pvNodeData           = NULL;
      t_DiamUnsigned32*     poTempunsigned32     = NULL;
      t_DiamUTF8String*     poTempUTF8str        = NULL;
      DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
				unInteger   = 0;
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
                        case OFFLINE_CS_EVENT:
                                poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Event AVP found\n");
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poEventTypeCDRContent_o->ucEvent,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Event avp data to Message Structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;
                        case OFFLINE_CS_SIP_METHOD:
                                poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Sip-Method AVP found\n");
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poEventTypeCDRContent_o->ucSipMethod,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Sip-Method avp data to Message Structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
                                break;
                        case OFFLINE_CS_EXPIRES:
                                poTempunsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Expires AVP found\n");
								/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																								&unInteger,
																								*(poTempunsigned32->punUnsigned32) ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
                                poEventTypeCDRContent_o->unExpires = *(poTempunsigned32->punUnsigned32);
                                break;
						default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Event-type AVP, leaving\n");
                
                }// End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )
     

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
      return Offline_CS_RetVal_Success;

 }


t_Offline_CS_ReturnVal inline Rf_AddTimeStampsAVPToContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
															HAVP					hToAVP_i,
															t_TimeStamps*			poTimeStampCDRContent_o)
{

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

	  if (NULL == hRFModuleContext_i)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  if (NULL == poTimeStampCDRContent_o)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poTimeStampCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  if (NULL == hToAVP_i)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }
 
      HAVP                  hToAVP               = NULL;
      void*                 pvNodeData           = NULL;
      unsigned int          unAVPCode            = 0;
	  unsigned int			unInteger			 = 0;
      t_DiamUnsigned32*     poTempunsigned32     = NULL;
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
				unInteger   = 0;
                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
                        case OFFLINE_CS_SIP_REQUEST_TIME_STAMP:
                            poTempunsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
							/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																							&unInteger,
																							*(poTempunsigned32->punUnsigned32) ))
							{
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
								return Offline_CS_RetVal_Failure;
							}*/
							poTimeStampCDRContent_o->unSipRequestTimeStamp = *(poTempunsigned32->punUnsigned32); 
                            break;
                        case OFFLINE_CS_SIP_RESPONSE_TIME_STAMP:
                            poTempunsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
							/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																							&unInteger,
																							*(poTempunsigned32->punUnsigned32) ))
							{
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
								return Offline_CS_RetVal_Failure;
							}*/
                            poTimeStampCDRContent_o->unSipResponseTimeStamp = *(poTempunsigned32->punUnsigned32);    
                            break;
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Event-type AVP, leaving\n");
                
                }// End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )


      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
      return Offline_CS_RetVal_Success ;
 }

t_Offline_CS_ReturnVal inline Rf_AddAppServersInfoAVPToContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																HAVP					hToAVP_i,
																t_AppServerInfoList*	poAppServerListCDRContent_o)
{

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

	  if (NULL == hRFModuleContext_i )
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  if (NULL == poAppServerListCDRContent_o)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poAppServerListCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  if (NULL == hToAVP_i)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
	  unsigned int          unAVPCode            = 0;
	  unsigned int			unInteger			 = 0;
	  t_DiamUnsigned32*     poTempunsigned32     = NULL;
	  t_DiamUTF8String*     poTempUTF8str        = NULL;
	  DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
	  unsigned int 			unDestinationStrSize = 0;
	  int nIndex = 0;

	  if( OFFLINE_CS_LIST_SIZE > poAppServerListCDRContent_o->nNumberOfitemsInList )
	  {
		  nIndex = poAppServerListCDRContent_o->nNumberOfitemsInList ;
		  while( NULL != pCurrentNode )
		  {
			   pvNodeData  = NULL;
			   unInteger   = 0;
			   if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
																	  pCurrentNode ,&pvNodeData ) )
			   {
					   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
					   return Offline_CS_RetVal_Failure;
			   }
	
			   hToAVP      = (HAVP)pvNodeData;
			   unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
	
			  switch(unAVPCode)
			  {
				 case OFFLINE_CS_APPLICATION_SERVER:
					 poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
					 unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
					 if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poAppServerListCDRContent_o->oItems[nIndex].ucAppServer,
																					unDestinationStrSize,
																					poTempUTF8str->pucOctetString,
																					poTempUTF8str->unNumOfOctets ))
					 {
						 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Application-Server avp data to Message Structure,Leaving \n");
						 return Offline_CS_RetVal_Failure;
					 }
					 break;
			     case OFFLINE_CS_APPLICATION_PROVIDED_CALLED_PARTY_ADDRESS:
					 poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
					 unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
                     if( Offline_CS_RetVal_Success != Rf_CopyOctetStringToStringList( &(poAppServerListCDRContent_o->oItems[nIndex].oAppProvidedCalledPartyAddressList),
																					  unDestinationStrSize,
																					  poTempUTF8str->pucOctetString,
																					  poTempUTF8str->unNumOfOctets ))
					 {
						 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Application-Provided-Called-Party-Address avp data to Message Structure,Leaving \n");
						 return Offline_CS_RetVal_Failure;
					 }
					 break;
	
				default:
				  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Server-Information AVP, leaving\n");
                
			  }// End switch( unAVPCode )
					DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
		  }// end while( NULL != pCurrentNode )
		  poAppServerListCDRContent_o->nNumberOfitemsInList++;
	  }
	  else
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No More Space in Server-Information-List, leaving\n");
	  }

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
      return Offline_CS_RetVal_Success ;

 }


t_Offline_CS_ReturnVal inline Rf_AddInterOperatorIdentifierAVPToContentsStruct( HRFAPPLICATIONCONTEXT 				hRFModuleContext_i,
																		 HAVP								hToAVP_i,
																		 t_InterOperatorIdentifierList*		poInterOpratorIdentifierCDRContent_o)
{

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

	  if (NULL == hRFModuleContext_i)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  if (NULL == poInterOpratorIdentifierCDRContent_o)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poInterOpratorIdentifierCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  if (NULL == hToAVP_i)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

    
	  HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
	  unsigned int          unAVPCode            = 0;
	  t_DiamUnsigned32*     poTempunsigned32     = NULL;
	  t_DiamUTF8String*     poTempUTF8str        = NULL;
	  DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;
	  unsigned int			unDestinationStrSize = 0;
	  int nIndex = 0;

	  if( OFFLINE_CS_LIST_SIZE > poInterOpratorIdentifierCDRContent_o->nNumberOfitemsInList )
	  {
			nIndex = poInterOpratorIdentifierCDRContent_o->nNumberOfitemsInList;
			while( NULL != pCurrentNode )
			{
				   pvNodeData  = NULL;
				   if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,pCurrentNode ,&pvNodeData ) )
				   {
						   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
						   return Offline_CS_RetVal_Failure;
				   }
		
				   hToAVP      = (HAVP)pvNodeData;
				   unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
		
				   switch(unAVPCode)
				   {
							case OFFLINE_CS_ORIGINATING_IOI:
									poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poInterOpratorIdentifierCDRContent_o->oItems[nIndex].ucOriginatingIOI,
																								   unDestinationStrSize,
																								   poTempUTF8str->pucOctetString,
																								   poTempUTF8str->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Originating-IOI avp data to Message Strucutre,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							case OFFLINE_CS_TERMINATING_IOI:
									poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
									unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
									if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poInterOpratorIdentifierCDRContent_o->oItems[nIndex].ucTerminatingIOI,
																								   unDestinationStrSize,
																								   poTempUTF8str->pucOctetString,
																								   poTempUTF8str->unNumOfOctets ))
									{
										DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Terminating-IOI avp data to Message Strucutre,Leaving \n");
										return Offline_CS_RetVal_Failure;
									}
									break;
							default:
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Inter-Operator-identifier AVP, leaving\n");
					
				 }// End switch( unAVPCode )
				 DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
			}// end while( NULL != pCurrentNode )
			poInterOpratorIdentifierCDRContent_o->nNumberOfitemsInList++;
	  }
	  else
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No more space in Inter-Operator-identifier List, leaving\n");
	  }



	  


      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
      return Offline_CS_RetVal_Success ;
 }

t_Offline_CS_ReturnVal inline Rf_AddMessageBodyAVPToContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
															 HAVP					hToAVP_i,
															 t_MessageBodyList*		poMessageBodyCDRContent_o)

{
    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

	 if (NULL == hRFModuleContext_i )
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

	  if (NULL == poMessageBodyCDRContent_o)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poMessageBodyCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }
      
	  if (NULL == hToAVP_i)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
	  }

     
      HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
      unsigned int          unAVPCode            = 0;
	  unsigned int			unInteger			 = 0;
	  unsigned int 			unDestinationStrSize = 0;
	  t_DiamUnsigned32*     poTempunsigned32     = NULL;
	  t_DiamInteger32*  	poTempInteger32 	 = NULL;
	  t_DiamUTF8String*     poTempUTF8str        = NULL;
	  DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

	  int nIndex = 0;

	  
	  if( OFFLINE_CS_LIST_SIZE > poMessageBodyCDRContent_o->nNumberOfitemsInList )
	  {
			nIndex = poMessageBodyCDRContent_o->nNumberOfitemsInList ;
			while( NULL != pCurrentNode )
			{
				   pvNodeData  = NULL;
				   unInteger   = 0;
				   if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
																		  pCurrentNode ,&pvNodeData ) )
				   {
						   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
						   return Offline_CS_RetVal_Failure;
				   }
		
				   hToAVP      = (HAVP)pvNodeData;
				   unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
		
					switch(unAVPCode)
					{
							case OFFLINE_CS_CONTENT_TYPE:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageBodyCDRContent_o->oItems[nIndex].ucContentType,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Content-Type avp data to Message Strucutre,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}	
								break;
							case OFFLINE_CS_CONTENT_DISPOSITION:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageBodyCDRContent_o->oItems[nIndex].ucContentDisposition,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Content-Despositon avp data to Message Strucutre,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}	
								break;
							case OFFLINE_CS_CONTENT_LENGTH:
								poTempunsigned32 = (t_DiamUnsigned32*)hToAVP->pData;
								/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																								&unInteger,
																								*(poTempunsigned32->punUnsigned32) ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
								poMessageBodyCDRContent_o->oItems[nIndex].nContentLength = *(poTempunsigned32->punUnsigned32);
								break;
							case OFFLINE_CS_ORIGINATOR:
								poTempInteger32 = (t_DiamInteger32*)hToAVP->pData;
								/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																								&unInteger,
																								*(poTempInteger32->pnInteger32) ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
								poMessageBodyCDRContent_o->oItems[nIndex].nOriginator = *(poTempInteger32->pnInteger32);	
								break;
							default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in Message-Body AVP, leaving\n");
					
					 }// End switch( unAVPCode )
					 DMBase_SListGetNextNode( hRFModuleContext_i ->hDiameterStackContext,pCurrentNode,&pCurrentNode);
		   }// end while( NULL != pCurrentNode )
			poMessageBodyCDRContent_o->nNumberOfitemsInList++;
	  }
	  else
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No More space in Message-Body List, leaving\n");
	  }

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
	  return Offline_CS_RetVal_Success ;
 }

t_Offline_CS_ReturnVal inline Rf_AddSDPMediaComponentsAVPToContentsStruct( HRFAPPLICATIONCONTEXT 			hRFModuleContext_i,
																	HAVP							hToAVP_i,
																	t_SDPMediaComponentList*		poSDPMediaComponentsCDRContent_o)

{
    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

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

      if (NULL == poSDPMediaComponentsCDRContent_o)
	  {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poSDPMediaComponentsCDRContent_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
	  }

      
      

	  HAVP                  hToAVP               = NULL;
	  void*                 pvNodeData           = NULL;
	  _Bool                 bConvToNetOrder      = 0;
	  unsigned int          unAVPCode            = 0;
	  unsigned int			unInteger			 = 0;
	  unsigned int			unDestinationStrSize = 0;
	  t_DiamUnsigned32*     poTempunsigned32     = NULL;
	  t_DiamInteger32*  	poTempInteger32 	 = NULL;
	  t_DiamUTF8String*     poTempUTF8str        = NULL;
	  DMBase_SList*	        pCurrentNode         = hToAVP_i->hCollectionOfAVPs;

	  int nIndex = 0;

	  if( OFFLINE_CS_LIST_SIZE > poSDPMediaComponentsCDRContent_o->nNumberOfitemsInList )
	  {
		  nIndex = poSDPMediaComponentsCDRContent_o->nNumberOfitemsInList ;
		  while( NULL != pCurrentNode )
		  {
				   pvNodeData  = NULL;
				   unInteger   = 0;
				   if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
																		  pCurrentNode ,&pvNodeData ) )
				   {
						   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
						   return Offline_CS_RetVal_Failure;
				   }
		
				   hToAVP      = (HAVP)pvNodeData;
				   unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
		
					switch(unAVPCode)
					{
							case OFFLINE_CS_SDP_MEDIA_NAME:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poSDPMediaComponentsCDRContent_o->oItems[nIndex].ucSDPMediaName,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying SDP-Media-Name avp data to Message Structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;
							 case OFFLINE_CS_SDP_MEDIA_DESCRIPTION:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyOctetStringToStringList( &(poSDPMediaComponentsCDRContent_o->oItems[nIndex].oSDPMediaDescriptionList),
																								 unDestinationStrSize,
																								 poTempUTF8str->pucOctetString,
																								 poTempUTF8str->unNumOfOctets ))
							{
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying SDP-Media-Description avp data to Message Structure,Leaving \n");
								return Offline_CS_RetVal_Failure;
							}	
							break;
		
							case OFFLINE_CS_MEDIA_INITIATOR_FLAG:
								poTempInteger32 = (t_DiamInteger32*)hToAVP->pData;
								/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																								&unInteger,
																								*(poTempInteger32->pnInteger32) ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
								poSDPMediaComponentsCDRContent_o->oItems[nIndex].nMediaInitiatorFlag = *(poTempInteger32->pnInteger32);
								break;
		
							case OFFLINE_CS_MEDIA_INITIATOR_PARTY:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poSDPMediaComponentsCDRContent_o->oItems[nIndex].ucMediaInitiatorParty,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Media-Initiator-Party avp data to Message Structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;
		
							case OFFLINE_CS_AUTHORISED_QOS:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poSDPMediaComponentsCDRContent_o->oItems[nIndex].ucAuthorizedQoS,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Authorised-QoS avp data to Message Structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;
		
							case OFFLINE_CS_3GPP_CHARGING_ID:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poSDPMediaComponentsCDRContent_o->oItems[nIndex].uc3GPPChargingId,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying 3GPP-Charging-Id avp data to Message Structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;
		
							case OFFLINE_CS_ACCESS_NETWORK_CHARGING_IDENTIFIER_VALUE:
								poTempUTF8str = (t_DiamUTF8String*)hToAVP->pData;
								unDestinationStrSize = OFFLINE_CS_ADDRESS_STRING_SIZE;
								if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poSDPMediaComponentsCDRContent_o->oItems[nIndex].ucAccessNetChargingIdValue,
																							   unDestinationStrSize,
																							   poTempUTF8str->pucOctetString,
																							   poTempUTF8str->unNumOfOctets ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying Access-Network-Charging-Identifier-Value avp data to Message Structure,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}
								break;
							default:
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP received in SDPMediaComponent AVP, leaving\n");
		
				  }// End switch( unAVPCode )
				  DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);
			}// end while( NULL != pCurrentNode )
			poSDPMediaComponentsCDRContent_o->nNumberOfitemsInList++;
	  }
	  else
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No more Space in SDPMediaComponent List, leaving\n");
	  }

	  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
	  return Offline_CS_RetVal_Success ;
 }




