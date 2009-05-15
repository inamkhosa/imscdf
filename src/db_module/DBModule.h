
#ifndef _DBMODULE_H
#define	_DBMODULE_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include"RF_defines.h"
	#include"RF_include.h"
	#include "offcs_com_cdr_contents.h"
	#include "dappcommon_srvrconfload.h"
 
	#define OFFLINE_CS_QUERY_TYPE_IS_INSERT                        1
    #define OFFLINE_CS_QUERY_TYPE_IS_UPDATE                        2
    
	#define OFFLINE_CS_NUMBER_OF_DB_THREADS						   1

	 typedef struct DBModuleContext
	 {
		HDIAMETERSTACKCONTEXT       hDiameterStackContext;
		t_cdf_db_config_params		oCdf_db_config_params;
		DMBASE_IOAGENTS_HANDLE 		hIOAgentsHandle;
	 }t_DBModuleContext ;

     typedef    t_DBModuleContext*      HDBMODULECONTEXT;

	 typedef struct DBModuleThreadContext
	 {
		DMBASE_DBE_HANDLE           hDBEnginHandle;
		HDBMODULECONTEXT			hMainDBModuleContext;
		AMPS_UIOAGENT_HANDLE		hUnitIOAgent;
	 }t_DBModuleThreadContext ;

	 typedef    t_DBModuleThreadContext*      HDBMODULETHREADCONTEXT;

	t_Offline_CS_ReturnVal Db_CreateAndInitModuleContext(	HDBMODULECONTEXT*        	phDBModuleContext_o,
															HDIAMETERSTACKCONTEXT       hDiameterContext_i);
	
	t_Offline_CS_ReturnVal Db_DestroyModuleContext(	HDBMODULECONTEXT*        phDBModuleContext_o);


	int	Db_EventValidationCallback(   AMPS_APP_HANDLE 	hAMPS_APP_HANDLE_i,
									  int 				nEventID_i, 
									  void* 			pvEventPayload_i);

	void  Db_DataBaseReqEvtHandler(AMPS_HANDLE r_hAMPSHandle, void* r_pvEventData,
								   void* r_pvEventHandle, void* r_pvEventPayload);

	t_Offline_CS_ReturnVal Db_WriteCdrToDataBase( HDBMODULETHREADCONTEXT     hDBThreadModContext_i,
											  t_CDR_Contents*	  poCDRContents_i );

	t_Offline_CS_ReturnVal Db_GenerateResponseToRFmodule( HDBMODULECONTEXT  		hDBModuleContext_i,
														  unsigned int			unAcctRecordNumber_i,
														  unsigned char*		pucSessionId_i,
														  unsigned int			unResultCode_i);
	
	t_Offline_CS_ReturnVal Db_RegisterForIntEvents(HDBMODULECONTEXT   phDBModuleContext_i);

	void PrintOctStr(unsigned char* puchTemp , unsigned int unSize);

	t_Offline_CS_ReturnVal  Db_Insert_SCSCFReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
												  t_CDR_Contents* 			poCDRContents_i );

	t_Offline_CS_ReturnVal  Db_Insert_PCSCFReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
												  t_CDR_Contents* 			poCDRContents_i );

	t_Offline_CS_ReturnVal  Db_Insert_ICSCFReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
												  t_CDR_Contents* 			poCDRContents_i );

	t_Offline_CS_ReturnVal  Db_Insert_MRFCReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
												  t_CDR_Contents* 			poCDRContents_i );

	t_Offline_CS_ReturnVal  Db_Insert_ASReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
											   t_CDR_Contents* 			poCDRContents_i );

	t_Offline_CS_ReturnVal  Db_Insert_MessageBodyList(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
													  unsigned char*			pucTableName_i,
													  int 						nTableId_i,
													  t_MessageBodyList 		oMessageBodyList_i );
	t_Offline_CS_ReturnVal Db_Insert_InterOperatorIdList(HDBMODULETHREADCONTEXT  			hDBThreadModContext_i,
														 unsigned char*						pucTableName_i,
														 int 								nTableId_i,
														 t_InterOperatorIdentifierList 		oInterOperatorIdentifierList_i );

	t_Offline_CS_ReturnVal  Db_Insert_AppServerInfoList(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
														unsigned char*				pucTableName_i,
														int 						nTableId_i,
														t_AppServerInfoList 		oAppServerInfoList_i );

	// OffLineCS Debugger Functions
	t_Offline_CS_ReturnVal RF_Debug_PrintDBEnginResult(DMBASE_DBEngineResult oDBEngineResult);

	/*



	
            for ( nIndex = 0 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.nNumberOfitemsInList;
				  nIndex++ )
			{

					sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","pcscf_inter_opt_identifier");

					
					strcpy(ColumnNamesPartOfSQLQuery,"pcscf_id");
					unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",nPcscfId);
					
	
					unStringSize = strlen( poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.oItems[nIndex].ucOriginatingIOI);
					if( unStringSize > 0  )
					{
					   strcat(ColumnNamesPartOfSQLQuery,",originating_ioi");
					   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
							   poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.oItems[nIndex].ucOriginatingIOI);
					}
	
					unStringSize = strlen( poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.oItems[nIndex].ucTerminatingIOI);
					if( unStringSize > 0  )
					{
					   strcat(ColumnNamesPartOfSQLQuery,",terminating_ioi");
					   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
							   poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.oItems[nIndex].ucTerminatingIOI);
					}

					strcpy(pucSQLQueryString,StartPartOFSQLQuery);
                    strcat(pucSQLQueryString,ColumnNamesPartOfSQLQuery);
					strcat(pucSQLQueryString,pcMiddlePartOFSQLQuery);
					strcat(pucSQLQueryString,ColumnVlauesPartOfSQLQuery);
					strcat(pucSQLQueryString,pcEndPartOFSQLQuery);

					// printf("\n\n Complete SQL Query = %s \n",pucSQLQueryString);
					if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
																	  hDBThreadModContext_i->hDBEnginHandle,
																	  pucSQLQueryString,
																	  strlen(pucSQLQueryString),      
																	  &oDBEngineResult_o))
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
						return Offline_CS_RetVal_Failure;
					}

			}




    */

	typedef struct P_CSCF_cdr_Contents
	{
	   int 					nRecord_Type;
	   _Bool					bRetransmission;
	   t_DiamOctetString		oOctStrSipMethod;
	   t_DiamOctetString       oOctStrExpiresInformation; // AVP code Unknoun
	   t_DiamOctetString		oOctStrEvent;
	   int                     nRoleOfNode;
	   t_DiamOctetString       oOctStrNodeAddress;
	   t_DiamOctetString       oOctStrSessionId;
	   t_DiamOctetString       oListOfCallingPartyAddress;
	   t_DiamOctetString       oListOfAssociatedURI;
	   t_DiamOctetString       oOctStrCalledPartyAddress;
	   t_DiamOctetString       oOctStrServedPartyAddress;
	   t_DiamOctetString       oOctStrNumberPortabilityRoutingInformation;
	   t_DiamOctetString       oOctStrCarrierSelectRoutingInformation;
	   t_DiamOctetString       oOctStrListOfSubscriptionId;
	   t_DiamOctetString       oOctStrServiceRequestTimeStamp;
	   t_DiamOctetString       oOctStrServiceDeliveryStartTime;
	   t_DiamOctetString       oOctStrServiceDeliveryEndTime;
	   t_DiamOctetString       oOctStrRecordOpeningTime;
	   t_DiamOctetString       oOctStrRecordClouserTime;
	   t_DiamOctetString       oOctStrOriginatingIOI;
	   t_DiamOctetString       oOctStrTerimnatingIOI;
	   int                     nLocalRecordSequenceNumber;
	   int                     nRecordSequenceNumber;
	   int                     nCauseForRecordClosing;
	   _Bool                   bIncompleteCDRIndication;
	   t_DiamOctetString       oOctStrIMSChargingIdentifier;
	   t_DiamOctetString       oOctStrSDPSessionDescription;
	   t_DiamOctetString       oOctStrSDPOfferTimeStamp;
	   t_DiamOctetString       oOctStrSDPAnswerTimeStamp;
	   t_DiamOctetString       oOctStrSDPMediaName;
	   t_DiamOctetString       oOctStrSDPMediaDescription;
	   t_DiamOctetString       oOctStrAccessCorrelationId;
	   _Bool                   bMediaInitiatorFlag;
	   t_DiamOctetString       oOctStrSipRequestTimeStamp;
	   t_DiamOctetString       oOctStrIPResponseTimeStamp;
	   t_DiamOctetString       oOctStrAuthorisedQoS;
	   t_DiamOctetString       oGGSNAddress;
	   int                     nServiceReasonReturnCode;
	   t_DiamOctetString       oOctStrContentType;
	   t_DiamOctetString       oOctStrContentDisposition;
	   int                     nContentLength;
	   int                     nOriginator;
	   t_DiamOctetString       oOctStrAccessNetworkInformation;
	   t_DiamOctetString       oOctStrInterOperatorIdentifier;

	}t_P_CSCF_cdr_Contents;

	/*
	t_Offline_CS_ReturnVal OffLineCS_DB_Create_S_CSCF_SQLInsertQueryStatement( HDBMODULECONTEXT     phDBModuleContext_i,
																			   char**              ppchQueryStatement_o,
																			   unsigned long*      pulLengthOfQuery_o,
																			   t_CDR_Contents      oCDRContents);

	*/
	void* cdf_db_ThreadInit(AMPS_HANDLE  hAMPSHandle_i, void* 	hUIOAgentsHandle_i,void* 	pvUserData_i);
	void cdf_db_ThreadCleanup(void* pvThreadCtxt_i);




	/*
    for( nIndex =0; nIndex<=nTempNumItems; nIndex++ )
        {
            
                if( ( NULL != oCallingPartyAddressList_i.oItems[nIndex].pucOctetString)&& (oCallingPartyAddressList_i.oItems[nIndex].unNumOfOctets))
                {

                        if( 0 == nIndex )
                        {       // dont put seperator Character ";" brfore the element if it is the first element of List 
                                strncpy(*ppcList_o,oCallingPartyAddressList_i.oItems[nIndex].pucOctetString, oCallingPartyAddressList_i.oItems[nIndex].unNumOfOctets);
                        }
                        else 
                        {       //  put seperator Character ";" before the element if it is an element other then  first element of List 
                                strcat(*ppcList_o,";");
                                strncat(*ppcList_o,oCallingPartyAddressList_i.oItems[nIndex].pucOctetString, oCallingPartyAddressList_i.oItems[nIndex].unNumOfOctets);
                        }
                }// End if 
        }//End for 
    */


#ifdef	__cplusplus
}
#endif

#endif	/* _DBMODULE_H */







 

 



