#include "AMPS_API.h"
#include "DBModule.h"
#include "rf_msg_api.h"
#include"RF_defines.h"
#include "dappcommon_srvrconfload.h"
#include "offcs_com_cdr_contents.h"
#include "offcs_com_defines.h"
#include "offcs_app_defined_avp.h"

int  g_nDiamBaseTraceID = 0;

 /******************************************************************************
** Function Name: 
**			Rf_EvtValidationCallback
**  
** Description:
**		This function is to Validate the .
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT*    phRFApplicationContext_o
**			 type: output
**			 detail : this is pointer to  RFApplicationContext Handle  to which the
**                    memory  will be allocated. 
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  whether function has successfully Created  
**     DBModuleContext or any problem occured.
** 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
int	Db_EventValidationCallback( AMPS_APP_HANDLE    r_hAMPS_APP_HANDLE,
								int                r_nEventID,
								void*              r_pvEventPayload)
{
    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return AMPS_SUCCESS;
}
/******************************************************************************
** Function Name: 
**			RFModule_ModuleInit
**  
** Description:
**		This function is to inilialize an Rf Module 
**
** Parameters:
**		  AMPS_HANDLE     hAMPS_HANDLE
**			 type: input
**			 detail : This is Handle  to APMS Framework. 
**
**		  void*           pvOfflineChargingServerLocalContext_i
**			 type: input
**			 detail : This is void pointer to data send by its 
**					  Loader module through AMPS. 
**
** Return: Its rerurn value is a void pointer.
** 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
AMPS_MODULE_HANDLE	DBModule_ModuleInit( AMPS_HANDLE     hAMPS_HANDLE_i,
                                         void*           pvOfflineChargingServerLocalContext_i)
{

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

        HDBMODULECONTEXT    hDBModuleContext = NULL;

        if( NULL == hAMPS_HANDLE_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hAMPS_HANDLE is NULL pointer, Leaving\n");
            return ((void*)hDBModuleContext);
        }

        if(NULL == pvOfflineChargingServerLocalContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," pvOfflineChargingServerLocalContext is NULL pointer, Leaving\n");
            return ((void*)hDBModuleContext);
        }

		t_cdf_db_config_params*		poCDF_DB_ConfData = (t_cdf_db_config_params*) pvOfflineChargingServerLocalContext_i;


        HDIAMETERSTACKCONTEXT    hDiameterStackContext  = (HDIAMETERSTACKCONTEXT) poCDF_DB_ConfData->hDiameterStackContext;
		DMBASE_IOAGENTS_HANDLE 			   hIOAgentsHandle = NULL;	

        
        //Create DBModule Context
        if(Offline_CS_RetVal_Success != Db_CreateAndInitModuleContext( &hDBModuleContext, hDiameterStackContext ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_CreateAndInitModuleContext() Fail, Leaving\n");
            return ((void*)hDBModuleContext);
        }

		hDBModuleContext->oCdf_db_config_params = *poCDF_DB_ConfData;


		//Initialise AMPS IO Agent
		if(DMBase_Retval_Success !=  DMBase_IOAgentsInit(hDBModuleContext->hDiameterStackContext,
														 OFFLINE_CS_NUMBER_OF_DB_THREADS, 
														 NULL,
														 cdf_db_ThreadInit,
														 cdf_db_ThreadCleanup, 
														 (void*)(hDBModuleContext),
														 &hIOAgentsHandle) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_IOAgentsInit Fails, Leaving\n");
			return ((void*)NULL);
		}

		hDBModuleContext->hIOAgentsHandle = hIOAgentsHandle;

        if(Offline_CS_RetVal_Success != Db_RegisterForIntEvents( hDBModuleContext ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_RegisterForIntEvents() Fail, Leaving\n");
            return ((void*)hDBModuleContext );
        }
    
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
        return ((void*)hDBModuleContext);
 }

/******************************************************************************
** Function Name: 
**			DBModule_ModuleCleanup
**  
** Description:
**		This function is a clean up function of DBModule.All the momory allocated to 
**		DMModule is must to free here.
**
** Parameters:
**		  AMPS_HANDLE                 r_hAMPS_HANDLE
**			 type: input
**			 detail : this is handle to AMPS Frame work. 
**			
**		 AMPS_MODULE_HANDLE          r_hAMPS_MODULE_HANDLE
**			 type: input
**			 detail : this is handle to the DBModule Context. 
**
**
** Return:
**		Its return value is void. 
*******************************************************************************
** Created By: 	Simab Shahid	
*******************************************************************************
*******************************************************************************
*/
void DBModule_ModuleCleanup(AMPS_HANDLE             hAMPS_HANDLE_i, 
                            AMPS_MODULE_HANDLE      hDB_Module_Context_i)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return;
}

/******************************************************************************
** Function Name: 
**			Db_RegisterForIntEvents
**  
** Description:
**		DB Module is registering for all the inernal events(that it want to
**		receive from different modules of Offline Charging server) inside 
**		this function.
**
** Parameters:
**		  HDBMODULECONTEXT         hDBModuleContext_i
**			 type: input
**			 detail : this is a handle to DB Modlue Context.
**      
** Return:
**		An t_Offline_CS_ReturnVal value specifying the success or failure of the 
**     function.
**
** 
*******************************************************************************
** Created By: 	 Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Db_RegisterForIntEvents(HDBMODULECONTEXT   hDBModuleContext_i)
 {

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n"); 

        if(NULL == hDBModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! hOffLineCSDBModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        //set event handle validation callback before Registration
        AMPS_EvtSysSetEvtHandleValidationCallback( hDBModuleContext_i->hDiameterStackContext->hFrameworkHandle, 
													 Db_EventValidationCallback);


		if (AMPS_SUCCESS != AMPS_IOAgentsRegEvt(hDBModuleContext_i->hIOAgentsHandle, 
											OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_REQUEST, 
											Db_DataBaseReqEvtHandler))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," AMPS_IOAgentsRegEvt failed for OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_REQUEST \n");
			return Offline_CS_RetVal_Failure;
		}
		
     
       
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return Offline_CS_RetVal_Success;
 }
                   
/******************************************************************************
** Function Name: 
**			Db_DataBaseReqEvtHandler
**  
** Description:
**		This function is handling the Request Form RF Mudlue to write the cdrs 
**		in data base.
**
** Parameters:
**		  AMPS_HANDLE      hAMPS_HANDLE_i
**			 type: input
**			 detail : This is a handle to AMPS frame work.
**
**        DB_MODULE_LOCAL_CONTEXT_HANDLE        hModule_Context_Handle_i
**           type: input
**            detail :   This is Handle to DB Module Context.
**
**
**		 void*         r_pvEventHandle_i
**			type: input
**			detail: this is a void pointer which points
**				
**
**		void*         pvDataForDBModule_i
**         type: input
**			detail: This is a void pointer which points to the 
**				     data has been sent to the DB module
**
**
** Return:
**      The return type of this function is  void  .
**
** 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
void  Db_DataBaseReqEvtHandler(AMPS_HANDLE 			r_hAMPSHandle,
							   void* 				r_pvEventData,
							   void* 				r_pvEventHandle, 
							   void* 				r_pvEventPayload)

{

		//printf("\n\n\n\n EVENT HANDLER IN DB CALLED \n\n\n");
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

		if(NULL == r_hAMPSHandle)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_hAMPSHandle is NULL \n");
			return;
		}

        if(NULL == r_pvEventData)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_pvEventData is NULL \n");
            return;
        }

		 
		if(NULL == r_pvEventHandle)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_pvEventHandle is NULL \n");
            return;
        }

        if(NULL == r_pvEventPayload)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_pvEventPayload is NULL \n");
           // return; TODO!!!! Remove this!!
        }
		

		HDBMODULETHREADCONTEXT hDBModThreadCtxt = (HDBMODULETHREADCONTEXT)  r_pvEventData;

	    HDBMODULECONTEXT	hDBModMainContext = hDBModThreadCtxt->hMainDBModuleContext;

		

		int         		    nResultCode       = 0;
		unsigned int 		    unTLVBufferSize	  = 0;
		t_CDR_Contents*         poCDRContents	  = NULL;
        HDBMODULECONTEXT        hDBModuleContext  = NULL;
		
		poCDRContents 	= (t_CDR_Contents*)r_pvEventPayload;

        
        if(Offline_CS_RetVal_Success != Db_WriteCdrToDataBase( hDBModThreadCtxt,poCDRContents ) )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Writiting CDR to DataBase ,Leaving \n");
                // Write this CDR to File 
				nResultCode  = DIAMETER_UNABLE_TO_COMPLY;
		}
		else
		{
			nResultCode  = DIAMETER_SUCCESS;
		}

        
		t_ResponseEvtDataFromCdrFWModule   oResponseDataToRFModule;

		oResponseDataToRFModule.unAcctRecordNumber = poCDRContents->unAcctRecordNumber;
		oResponseDataToRFModule.unResultCode = nResultCode;
		strcpy(oResponseDataToRFModule.ucSessionId,poCDRContents->ucSessionId);


		//Send Event to cgf Module to inform about the status of Cdr File Writing Operation
		if ( AMPS_SUCCESS != AMPS_IOAgentsSendEvt( hDBModThreadCtxt->hUnitIOAgent,
												   OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE, 
												   (void* )&(oResponseDataToRFModule),
												   sizeof(t_ResponseEvtDataFromCdrFWModule))
		)
		{
			
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," AMPS_IOAgentsSendEvt FAILED for OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE \n");
			//printf("\n\n\n\n\n AMPS_IOAgentsSendEvt FAILED for OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE \n\n\n");
	
		}
		else
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG," AMPS_IOAgentsSendEvt SUCCEEDED for OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE \n");
			//printf("\n\n\n\n\n AMPS_IOAgentsSendEvt SUCCEEDED for OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE \n\n\n");
		}

        
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return ; 
       
}

/******************************************************************************
** Function Name: 
**			Db_WriteCdrToDataBase
**  
** Description:
**		This function is writing Cdr Contents to DataBase.
**
** Parameters:
**		  HDBMODULECONTEXT     hDBModuleContext_i,
**			 type: input
**			 detail : this is a handle to DB Module Context.
**
** 		 t_CDR_Contents	  oCDRContents_i
**			 type: input
**			 detail : this is a Object of Cdr Contents.      
** Return:
**		An t_Offline_CS_ReturnVal value specify the success or failure of
**	   the function.
** 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Db_WriteCdrToDataBase( HDBMODULETHREADCONTEXT     hDBThreadModContext_i,
											  t_CDR_Contents*	  		 poCDRContents_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		int nNodeFunctionality = 0;
	
		if(NULL == hDBThreadModContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		if(NULL == poCDRContents_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL , leaving\n");
			return Offline_CS_RetVal_Invalid_Pointer;
		}



		nNodeFunctionality = poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality;
		//printf(" nNodeFunctionality = poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality = %d \n",
			  // nNodeFunctionality = poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
		if ( OFFLINE_CS_NODE_FUNCTIONALITY_S_CSCF == nNodeFunctionality )
		{
            if( Offline_CS_RetVal_Success !=  Db_Insert_SCSCFReocrd( hDBThreadModContext_i,poCDRContents_i ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Inserting SCSCF Record to DB,Leaving \n");
				return Offline_CS_RetVal_Failure;
            }
		
		}
		else if (  OFFLINE_CS_NODE_FUNCTIONALITY_P_CSCF == nNodeFunctionality )
		{
		    if( Offline_CS_RetVal_Success !=  Db_Insert_PCSCFReocrd( hDBThreadModContext_i,poCDRContents_i ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Inserting PCSCF Record to DB,Leaving \n");
				return Offline_CS_RetVal_Failure;
            }
		
		}
		else if (  OFFLINE_CS_NODE_FUNCTIONALITY_I_CSCF == nNodeFunctionality )
		{
		    if( Offline_CS_RetVal_Success !=  Db_Insert_ICSCFReocrd( hDBThreadModContext_i,poCDRContents_i ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Inserting ICSCF Record to DB,Leaving \n");
				return Offline_CS_RetVal_Failure;
            }
		}
		else if (  OFFLINE_CS_NODE_FUNCTIONALITY_MRFC == nNodeFunctionality )
		{
		   if( Offline_CS_RetVal_Success !=  Db_Insert_MRFCReocrd( hDBThreadModContext_i,poCDRContents_i ) )
		   {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Inserting MRFC Record to DB,Leaving \n");
				return Offline_CS_RetVal_Failure;
            }
		}
		else if (  OFFLINE_CS_NODE_FUNCTIONALITY_AS == nNodeFunctionality )
		{
		   if( Offline_CS_RetVal_Success !=  Db_Insert_ASReocrd( hDBThreadModContext_i,poCDRContents_i ) )
		   {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Inserting AS Record to DB ,Leaving \n");
				return Offline_CS_RetVal_Failure;
            }
		}
		else
		{
		    // invalid value received in node functionality Cdr content
		    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unsupported value in node functionality AVP received ,Leaving\n");
		    return Offline_CS_RetVal_Failure;
        }
		
        
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
}

t_Offline_CS_ReturnVal  Db_Insert_SCSCFReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
											  t_CDR_Contents* 			poCDRContents_i )
{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

			if(NULL == hDBThreadModContext_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
			}
		
			if(NULL == poCDRContents_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
			}

			unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};
			unsigned char 	pucTableName[OFFLINE_CS_DATABASE_TABLE_NAME_SIZE_IN_BYTES] = {0};
			
    
			unsigned int	unStringSize =0;
			unsigned int    unTempLengthOfSQLQueryColumnNames  = 0;
			unsigned int    unTempLengthOfSQLQueryColumnVlaues = 0;
			unsigned int    unNewLengthOfSQLQueryColumnNames   = 0;
	
			
            char    StartPartOFSQLQuery[OFFLINE_CS_SQL_QUERY_START_PART_STRING_SIZE_IN_BYTES]={0};
			char    ColumnNamesPartOfSQLQuery[OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES]={0};
			char*   pcMiddlePartOFSQLQuery = ") VALUES (" ;
			char    ColumnVlauesPartOfSQLQuery[OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES]={0};
			char*   pcEndPartOFSQLQuery = ")";
	
			DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;

		   sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","scscf_records");

           
			if( poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality >= 0 )
			{
				strcat(ColumnNamesPartOfSQLQuery,"record_type");
				sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}

			if( poCDRContents_i->bRetransmission == 1 )
			{
				strcat(ColumnNamesPartOfSQLQuery,",retransmission");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			if( unStringSize > 0  )
			{
				
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",sip_method") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
               strcat(ColumnNamesPartOfSQLQuery,",sip_method");
               unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			   if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			   {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			   }
			   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode >= 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",role_of_node") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",role_of_node");
                unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode );
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",node_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",node_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",session_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",session_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",called_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",called_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_request_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_request_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_start_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_deliver_start_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp);
			}

		
			//copy service delivery end time stamp 
			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_end_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(ColumnNamesPartOfSQLQuery,",service_deliver_end_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
			{
				//copy record opening time   
                // get current time and sned cpoy into that field
				// // TODO : add code for cause for record closing 
			}


			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				// get current time and sned cpoy into that field
				// TODO : add code for incomplete  cdr indication  
			}
           

			if( poCDRContents_i->nLocalRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",local_record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",local_record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nLocalRecordSequenceNumber);
			}


			if( poCDRContents_i->nRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nRecordSequenceNumber);
			}

        
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ims_charging_identifier") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_charging_identifier");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ggsn_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ggsn_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_reason_return_code") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_reason_return_code");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",expires_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",expires_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",event") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",event");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",access_network_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",access_network_information");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_context_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_context_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",number_of_portability_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",number_of_portability_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",carrier_select_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",carrier_select_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			}


            unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserName);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",private_user_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",private_user_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserName)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucUserName);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSCommunicationServiceIdentifier);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ims_communication_service_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_communication_service_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSCommunicationServiceIdentifier)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSCommunicationServiceIdentifier);
			}



			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",requested_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",requested_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress);
			}

			int nIndex = 0;
			char* pccomma = "'";
			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_calling_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_calling_party_address");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				}
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}

			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_associated_uri") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_associated_uri");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}


			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_called_asserted_identity") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_called_asserted_identity");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex]);

				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);

			}

			
            
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_data") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_data");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData)+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			}

			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_type") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_type");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
					   poCDRContents_i->oServiceInfo.oSubscriptionId.nSubscriptionIdType);

			if( (strlen(StartPartOFSQLQuery)+ strlen(ColumnNamesPartOfSQLQuery)+ 
				 strlen(pcMiddlePartOFSQLQuery)+ strlen(ColumnVlauesPartOfSQLQuery) + strlen(pcEndPartOFSQLQuery)) > OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"SQL Query String is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
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

			

			strcpy(pucSQLQueryString,"SELECT @@IDENTITY");

			if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
															  hDBThreadModContext_i->hDBEnginHandle,
															  pucSQLQueryString,
															  strlen(pucSQLQueryString),      
															  &oDBEngineResult_o))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			
			
			char tempString[15];

			memcpy(tempString,oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].pchField,
				   oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField);

			tempString[oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField]= '\0';

			int nPcscfId = atoi(tempString);

			strcpy(pucTableName,"scscf_inter_opt_identifier");
			

			 if(Offline_CS_RetVal_Success !=  Db_Insert_InterOperatorIdList(hDBThreadModContext_i,
																			pucTableName,nPcscfId,
																			poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_InterOperatorIdList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }
            
			// printf(" SQL QUERY EXECUTED \n");
			
			strcpy(pucTableName,"scscf_message_body");

			 if(Offline_CS_RetVal_Success !=  Db_Insert_MessageBodyList(hDBThreadModContext_i,
																		pucTableName,nPcscfId,
																		poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_MessageBodyList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }

			 strcpy(pucTableName,"scscf_app_server_info");

			 if(Offline_CS_RetVal_Success !=  Db_Insert_AppServerInfoList(hDBThreadModContext_i,
																		pucTableName,nPcscfId,
																		poCDRContents_i->oServiceInfo.oIMSInformation.oAppServerInfoList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_AppServerInfoList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }

            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
			return Offline_CS_RetVal_Success;

}

t_Offline_CS_ReturnVal  Db_Insert_AppServerInfoList( HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
													 unsigned char*				pucTableName_i,
													 int 						nTableId_i,
													 t_AppServerInfoList 		oAppServerInfoList_i )
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		if(NULL == hDBThreadModContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}

		
		int 			nIndex = 0;
		int 			nInnerIndex = 0;
		unsigned int	unStringSize =0;
		char* pccomma = "'";
		char    		StartPartOFSQLQuery[50]={0};
		char    		ColumnNamesPartOfSQLQuery[10000]={0};
		char*   		pcMiddlePartOFSQLQuery = ") VALUES (" ;
		char    		ColumnVlauesPartOfSQLQuery[10000]={0};
		char*   		pcEndPartOFSQLQuery = ")";
		unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};
	
		DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;
	
		for ( nIndex = 0 ; nIndex < oAppServerInfoList_i.nNumberOfitemsInList; nIndex++ )
		{
	
				sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ",pucTableName_i);
	
	
				strcpy(ColumnNamesPartOfSQLQuery,"id");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",nTableId_i);
	
	
				unStringSize = strlen( oAppServerInfoList_i.oItems[nIndex].ucAppServer);
				if( unStringSize > 0  )
				{
				   strcat(ColumnNamesPartOfSQLQuery,",app_server_Involved"); 
				   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						   oAppServerInfoList_i.oItems[nIndex].ucAppServer);
				}
	
				nInnerIndex = 0;
				if( oAppServerInfoList_i.oItems[nIndex].oAppProvidedCalledPartyAddressList.nNumberOfItemsInList > 0)
				{
					if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",app_provided_call_party_address_list") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
						return Offline_CS_RetVal_Failure;
					}
					strcat(ColumnNamesPartOfSQLQuery,",app_provided_call_party_address_list");
					unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					if(( strlen(oAppServerInfoList_i.oItems[nIndex].oAppProvidedCalledPartyAddressList.ucItems[nInnerIndex])+ unStringSize ) >=
								OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
						     oAppServerInfoList_i.oItems[nIndex].oAppProvidedCalledPartyAddressList.ucItems[nInnerIndex]);
					for ( nInnerIndex = 1 ; nInnerIndex < oAppServerInfoList_i.oItems[nIndex].oAppProvidedCalledPartyAddressList.nNumberOfItemsInList;
						  nInnerIndex++ )
					{
						 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
						 if(( strlen(oAppServerInfoList_i.oItems[nIndex].oAppProvidedCalledPartyAddressList.ucItems[nInnerIndex])+ unStringSize ) >=
								OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
						 {
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
						   return Offline_CS_RetVal_Failure;
						 }
						 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
						   oAppServerInfoList_i.oItems[nIndex].oAppProvidedCalledPartyAddressList.ucItems[nInnerIndex]);

					}
					if(( strlen(oAppServerInfoList_i.oItems[nIndex].oAppProvidedCalledPartyAddressList.ucItems[nInnerIndex])+ strlen(pccomma) ) >=
								OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
						return Offline_CS_RetVal_Failure;
					}
					sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);

				}


				//////////////////////////////////////////
	
	
	
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

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;

}

t_Offline_CS_ReturnVal Db_Insert_InterOperatorIdList(HDBMODULETHREADCONTEXT  			hDBThreadModContext_i,
													 unsigned char*						pucTableName_i,
													 int 								nTableId_i,
													 t_InterOperatorIdentifierList 		oInterOperatorIdentifierList_i )
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	if(NULL == hDBThreadModContext_i)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	if(NULL == pucTableName_i)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucTableName_i is NULL , leaving\n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	int 			nIndex = 0;
	unsigned int	unStringSize =0;
	char    		StartPartOFSQLQuery[50]={0};
	char    		ColumnNamesPartOfSQLQuery[10000]={0};
	char*   		pcMiddlePartOFSQLQuery = ") VALUES (" ;
	char    		ColumnVlauesPartOfSQLQuery[10000]={0};
	char*   		pcEndPartOFSQLQuery = ")";
	unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};

	DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;

	for ( nIndex = 0 ; nIndex < oInterOperatorIdentifierList_i.nNumberOfitemsInList; nIndex++ )
	{

			sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ",pucTableName_i);


			strcpy(ColumnNamesPartOfSQLQuery,"id");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",nTableId_i);


            unStringSize = strlen( oInterOperatorIdentifierList_i.oItems[nIndex].ucOriginatingIOI);
			if( unStringSize > 0  )
			{
			   strcat(ColumnNamesPartOfSQLQuery,",originating_ioi"); 
			   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   oInterOperatorIdentifierList_i.oItems[nIndex].ucOriginatingIOI);
			}

			unStringSize = strlen( oInterOperatorIdentifierList_i.oItems[nIndex].ucTerminatingIOI);
			if( unStringSize > 0  )
			{
			   strcat(ColumnNamesPartOfSQLQuery,",terminating_ioi");
			   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   oInterOperatorIdentifierList_i.oItems[nIndex].ucTerminatingIOI);
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



	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success;
}

t_Offline_CS_ReturnVal  Db_Insert_MessageBodyList(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
												  unsigned char*			pucTableName_i,
												  int 						nTableId_i,
												  t_MessageBodyList 		oMessageBodyList_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		if(NULL == hDBThreadModContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}

		
		
		int 			nIndex = 0;
		unsigned int	unStringSize =0;
		char    		StartPartOFSQLQuery[50]={0};
		char    		ColumnNamesPartOfSQLQuery[10000]={0};
		char*   		pcMiddlePartOFSQLQuery = ") VALUES (" ;
		char    		ColumnVlauesPartOfSQLQuery[10000]={0};
		char*   		pcEndPartOFSQLQuery = ")";
		unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};
	
		DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;

		for ( nIndex = 0 ; nIndex < oMessageBodyList_i.nNumberOfitemsInList; nIndex++ )
		{

				sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ",pucTableName_i);

                
				strcpy(ColumnNamesPartOfSQLQuery,"id");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",nTableId_i);


				if( oMessageBodyList_i.oItems[nIndex].nContentLength > 0  )
				{
				   strcat(ColumnNamesPartOfSQLQuery,",content_length");
				   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",oMessageBodyList_i.oItems[nIndex].nContentLength);
				}

				unStringSize = strlen( oMessageBodyList_i.oItems[nIndex].ucContentType);
				if( unStringSize > 0  )
				{
				   strcat(ColumnNamesPartOfSQLQuery,",content_type");
				   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",oMessageBodyList_i.oItems[nIndex].ucContentType);
				}

				unStringSize = strlen( oMessageBodyList_i.oItems[nIndex].ucContentDisposition);
				if( unStringSize > 0  )
				{
				   strcat(ColumnNamesPartOfSQLQuery,",content_disposition");
				   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",oMessageBodyList_i.oItems[nIndex].ucContentDisposition);
				}

                
				if( oMessageBodyList_i.oItems[nIndex].nOriginator > 0  )
				{
				   strcat(ColumnNamesPartOfSQLQuery,",originator");
				   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",oMessageBodyList_i.oItems[nIndex].nOriginator);
				}

				strcpy(pucSQLQueryString,StartPartOFSQLQuery);
				strcat(pucSQLQueryString,ColumnNamesPartOfSQLQuery);
				strcat(pucSQLQueryString,pcMiddlePartOFSQLQuery);
				strcat(pucSQLQueryString,ColumnVlauesPartOfSQLQuery);
				strcat(pucSQLQueryString,pcEndPartOFSQLQuery);

				 //printf("\n\n Complete SQL Query = %s \n",pucSQLQueryString);
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


		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;

}

t_Offline_CS_ReturnVal  Db_Insert_PCSCFReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
											  t_CDR_Contents* 			poCDRContents_i )
{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
			if(NULL == hDBThreadModContext_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
			}
		
			if(NULL == poCDRContents_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
			}

			unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};
			unsigned char 	pucTableName[OFFLINE_CS_DATABASE_TABLE_NAME_SIZE_IN_BYTES] = {0};
			
    
			unsigned int	unStringSize =0;
			unsigned int    unTempLengthOfSQLQueryColumnNames  = 0;
			unsigned int    unTempLengthOfSQLQueryColumnVlaues = 0;
			unsigned int    unNewLengthOfSQLQueryColumnNames   = 0;
	
			
            char    StartPartOFSQLQuery[OFFLINE_CS_SQL_QUERY_START_PART_STRING_SIZE_IN_BYTES]={0};
			char    ColumnNamesPartOfSQLQuery[OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES]={0};
			char*   pcMiddlePartOFSQLQuery = ") VALUES (" ;
			char    ColumnVlauesPartOfSQLQuery[OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES]={0};
			char*   pcEndPartOFSQLQuery = ")";
	
			DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;

		   sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","pcscf_records");

           
			if( poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality > 0 )
			{
				strcat(ColumnNamesPartOfSQLQuery,"record_type");
				sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}

			if( poCDRContents_i->bRetransmission == 1 )
			{
				strcat(ColumnNamesPartOfSQLQuery,",retransmission");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			if( unStringSize > 0  )
			{
				
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",sip_method") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
               strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",sip_method");
               unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			   if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			   {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			   }
			   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode >= 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",role_of_node") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",role_of_node");
                unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode );
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",node_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",node_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",session_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",session_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",called_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",called_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_request_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_request_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_start_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_deliver_start_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp);
			}

		
			//copy service delivery end time stamp 
			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_end_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(ColumnNamesPartOfSQLQuery,",service_deliver_end_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
			{
				//copy record opening time   
                // get current time and sned cpoy into that field
				// // TODO : add code for cause for record closing 
			}


			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				// get current time and sned cpoy into that field
				// TODO : add code for incomplete  cdr indication  
			}
           

			if( poCDRContents_i->nLocalRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",local_record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",local_record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nLocalRecordSequenceNumber);
			}


			if( poCDRContents_i->nRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nRecordSequenceNumber);
			}

        
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ims_charging_identifier") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_charging_identifier");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ggsn_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ggsn_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_reason_return_code") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_reason_return_code");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",expires_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",expires_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",event") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",event");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",access_network_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",access_network_information");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_context_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_context_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",number_of_portability_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",number_of_portability_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",carrier_select_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",carrier_select_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServedPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",serverd_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",serverd_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServedPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucServedPartyAddress);
			}

			int nIndex = 0;
			char* pccomma = "'";
			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_calling_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_calling_party_address");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				}
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}

			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_associated_uri") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_associated_uri");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}

			
            
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_data") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_data");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData)+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			}

			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_type") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_type");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
					   poCDRContents_i->oServiceInfo.oSubscriptionId.nSubscriptionIdType);

			if( (strlen(StartPartOFSQLQuery)+ strlen(ColumnNamesPartOfSQLQuery)+ 
				 strlen(pcMiddlePartOFSQLQuery)+ strlen(ColumnVlauesPartOfSQLQuery) + strlen(pcEndPartOFSQLQuery)) > OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"SQL Query String is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            strcpy(pucSQLQueryString,StartPartOFSQLQuery);
		    strcat(pucSQLQueryString,ColumnNamesPartOfSQLQuery);
		    strcat(pucSQLQueryString,pcMiddlePartOFSQLQuery);
		    strcat(pucSQLQueryString,ColumnVlauesPartOfSQLQuery);
		    strcat(pucSQLQueryString,pcEndPartOFSQLQuery);

            //printf("\n\n Complete SQL Query = %s \n",pucSQLQueryString);
			
			
			if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
															  hDBThreadModContext_i->hDBEnginHandle,
															  pucSQLQueryString,
															  strlen(pucSQLQueryString),      
															  &oDBEngineResult_o))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

			

			strcpy(pucSQLQueryString,"SELECT @@IDENTITY");

			if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
															  hDBThreadModContext_i->hDBEnginHandle,
															  pucSQLQueryString,
															  strlen(pucSQLQueryString),      
															  &oDBEngineResult_o))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			
			
			char tempString[15];

			memcpy(tempString,oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].pchField,
				   oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField);

			tempString[oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField]= '\0';

			int nPcscfId = atoi(tempString);

			strcpy(pucTableName,"pcscf_inter_opt_identifier");
			

			 if(Offline_CS_RetVal_Success !=  Db_Insert_InterOperatorIdList(hDBThreadModContext_i,
																			pucTableName,nPcscfId,
																			poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_InterOperatorIdList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }
            
			//printf(" SQL QUERY EXECUTED \n");
			
			strcpy(pucTableName,"pcscf_message_body");

			 if(Offline_CS_RetVal_Success !=  Db_Insert_MessageBodyList(hDBThreadModContext_i,
																		pucTableName,nPcscfId,
																		poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_MessageBodyList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }

          
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		  return Offline_CS_RetVal_Success;
}

t_Offline_CS_ReturnVal  Db_Insert_ICSCFReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
											  t_CDR_Contents* 			poCDRContents_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if(NULL == hDBThreadModContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		if(NULL == poCDRContents_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL , leaving\n");
			return Offline_CS_RetVal_Invalid_Pointer;
		}


        unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};
		unsigned char 	pucTableName[OFFLINE_CS_DATABASE_TABLE_NAME_SIZE_IN_BYTES] = {0};
		

		unsigned int	unStringSize =0;
		unsigned int    unTempLengthOfSQLQueryColumnNames  = 0;
		unsigned int    unTempLengthOfSQLQueryColumnVlaues = 0;
		unsigned int    unNewLengthOfSQLQueryColumnNames   = 0;

		
		char    StartPartOFSQLQuery[OFFLINE_CS_SQL_QUERY_START_PART_STRING_SIZE_IN_BYTES]={0};
		char    ColumnNamesPartOfSQLQuery[OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES]={0};
		char*   pcMiddlePartOFSQLQuery = ") VALUES (" ;
		char    ColumnVlauesPartOfSQLQuery[OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES]={0};
		char*   pcEndPartOFSQLQuery = ")";

		DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;

	   sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","icscf_records");

	   
		if( poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality > 0 )
		{
			strcat(ColumnNamesPartOfSQLQuery,"record_type");
			sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
		}

		if( poCDRContents_i->bRetransmission == 1 )
		{
			strcat(ColumnNamesPartOfSQLQuery,",retransmission");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
		}


		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
		if( unStringSize > 0  )
		{
			
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",sip_method") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		   strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",sip_method");
		   unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
		   if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
		   {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
		   }
		   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
				   poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
		}

		if( poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode >= 0 )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",role_of_node") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(ColumnNamesPartOfSQLQuery,",role_of_node");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode );
		}

		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",node_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",node_address");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
		}

		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",session_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",session_id");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
		}

		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",called_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",called_party_address");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
				   poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
		}


		if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp > 0 )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_request_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(ColumnNamesPartOfSQLQuery,",service_request_time_stamp");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
					poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
		}


        if( poCDRContents_i->nLocalRecordSequenceNumber > 0 )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",local_record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(ColumnNamesPartOfSQLQuery,",local_record_sequence_number");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nLocalRecordSequenceNumber);
		}


        
		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ims_charging_identifier") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_charging_identifier");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
		}

       
		if( poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode > 0 )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_reason_return_code") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(ColumnNamesPartOfSQLQuery,",service_reason_return_code");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
					poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode);
		}

		if( poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires > 0 )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",expires_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(ColumnNamesPartOfSQLQuery,",expires_information");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
					poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires);
		}


		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",event") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",event");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
		}

		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",access_network_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",access_network_information");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
		}


		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_context_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_context_id");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
		}

		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",number_of_portability_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",number_of_portability_routing_information");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
		}


		unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
		if( unStringSize > 0  )
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",carrier_select_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",carrier_select_routing_information");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation)+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
		}

        
		int nIndex = 0;
		char* pccomma = "'";
		nIndex = 0;
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_calling_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_calling_party_address");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
				OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
				   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
			for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList;
				  nIndex++ )
			{
				 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
						OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
				   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);

			}
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ strlen(pccomma) ) >=
						OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			}
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
			
		}

		nIndex = 0;
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList > 0)
		{
			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_associated_uri") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_associated_uri");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ unStringSize ) >=
						OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			 {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			 }
			 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
				   poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex]);
			for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList;
				  nIndex++ )
			{
				 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ unStringSize ) >=
						OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
				   poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex]);

			}
			if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.ucItems[nIndex])+ strlen(pccomma) ) >=
						OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			 {
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
			   return Offline_CS_RetVal_Failure;
			 }
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
			
		}

        
		if( (strlen(StartPartOFSQLQuery)+ strlen(ColumnNamesPartOfSQLQuery)+ 
			 strlen(pcMiddlePartOFSQLQuery)+ strlen(ColumnVlauesPartOfSQLQuery) + strlen(pcEndPartOFSQLQuery)) > OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"SQL Query String is too small, leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		strcpy(pucSQLQueryString,StartPartOFSQLQuery);
		strcat(pucSQLQueryString,ColumnNamesPartOfSQLQuery);
		strcat(pucSQLQueryString,pcMiddlePartOFSQLQuery);
		strcat(pucSQLQueryString,ColumnVlauesPartOfSQLQuery);
		strcat(pucSQLQueryString,pcEndPartOFSQLQuery);

		//printf("\n\n Complete SQL Query = %s \n",pucSQLQueryString);
		
		
		if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
														  hDBThreadModContext_i->hDBEnginHandle,
														  pucSQLQueryString,
														  strlen(pucSQLQueryString),      
														  &oDBEngineResult_o))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		

		strcpy(pucSQLQueryString,"SELECT @@IDENTITY");

		if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
														  hDBThreadModContext_i->hDBEnginHandle,
														  pucSQLQueryString,
														  strlen(pucSQLQueryString),      
														  &oDBEngineResult_o))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
		
		char tempString[15];

		memcpy(tempString,oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].pchField,
			   oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField);

		tempString[oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField]= '\0';

		int nPcscfId = atoi(tempString);

		strcpy(pucTableName,"icscf_inter_opt_identifier");
		

		 if(Offline_CS_RetVal_Success !=  Db_Insert_InterOperatorIdList(hDBThreadModContext_i,
																		pucTableName,nPcscfId,
																		poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList ) )
		 {
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_InterOperatorIdList() Fails,Leaving\n");
			 return Offline_CS_RetVal_Failure;
		 }
		
		
		// printf("\n\n Complete SQL Query = %s \n",pucSQLQueryString);
		
		 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		 return Offline_CS_RetVal_Success;
}

t_Offline_CS_ReturnVal  Db_Insert_MRFCReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
											 t_CDR_Contents* 			poCDRContents_i )
{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
			if(NULL == hDBThreadModContext_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
			}
		
			if(NULL == poCDRContents_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
			}

			unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};
			unsigned char 	pucTableName[OFFLINE_CS_DATABASE_TABLE_NAME_SIZE_IN_BYTES] = {0};
			
    
			unsigned int	unStringSize =0;
			unsigned int    unTempLengthOfSQLQueryColumnNames  = 0;
			unsigned int    unTempLengthOfSQLQueryColumnVlaues = 0;
			unsigned int    unNewLengthOfSQLQueryColumnNames   = 0;
	
			
            char    StartPartOFSQLQuery[OFFLINE_CS_SQL_QUERY_START_PART_STRING_SIZE_IN_BYTES]={0};
			char    ColumnNamesPartOfSQLQuery[OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES]={0};
			char*   pcMiddlePartOFSQLQuery = ") VALUES (" ;
			char    ColumnVlauesPartOfSQLQuery[OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES]={0};
			char*   pcEndPartOFSQLQuery = ")";
	
			DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;

		   sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","mrfc_records");

           
			if( poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality > 0 )
			{
				strcat(ColumnNamesPartOfSQLQuery,"record_type");
				sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}

			if( poCDRContents_i->bRetransmission == 1 )
			{
				strcat(ColumnNamesPartOfSQLQuery,",retransmission");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			if( unStringSize > 0  )
			{
				
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",sip_method") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
               strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",sip_method");
               unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			   if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			   {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			   }
			   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode >= 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",role_of_node") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",role_of_node");
                unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode );
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",node_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",node_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",session_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",session_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",called_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",called_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_request_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_request_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_start_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_deliver_start_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp);
			}

		
			//copy service delivery end time stamp 
			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_end_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(ColumnNamesPartOfSQLQuery,",service_deliver_end_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
			{
				//copy record opening time   
                // get current time and sned cpoy into that field
				// // TODO : add code for cause for record closing 
			}


			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				// get current time and sned cpoy into that field
				// TODO : add code for incomplete  cdr indication  
			}
           

			if( poCDRContents_i->nLocalRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",local_record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",local_record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nLocalRecordSequenceNumber);
			}


			if( poCDRContents_i->nRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nRecordSequenceNumber);
			}

        
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ims_charging_identifier") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_charging_identifier");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ggsn_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ggsn_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_reason_return_code") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_reason_return_code");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",expires_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",expires_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",event") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",event");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",access_network_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",access_network_information");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_context_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_context_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",number_of_portability_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",number_of_portability_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",carrier_select_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",carrier_select_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",requested_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",requested_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress);
			}

			int nIndex = 0;
			char* pccomma = "'";
			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_calling_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_calling_party_address");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				}
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}

			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_called_asserted_identity") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_called_asserted_identity");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}

			
            
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_data") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_data");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData)+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			}

			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_type") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_type");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
					   poCDRContents_i->oServiceInfo.oSubscriptionId.nSubscriptionIdType);

			if( (strlen(StartPartOFSQLQuery)+ strlen(ColumnNamesPartOfSQLQuery)+ 
				 strlen(pcMiddlePartOFSQLQuery)+ strlen(ColumnVlauesPartOfSQLQuery) + strlen(pcEndPartOFSQLQuery)) > OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"SQL Query String is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
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

			

			strcpy(pucSQLQueryString,"SELECT @@IDENTITY");

			if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
															  hDBThreadModContext_i->hDBEnginHandle,
															  pucSQLQueryString,
															  strlen(pucSQLQueryString),      
															  &oDBEngineResult_o))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			
			
			char tempString[15];

			memcpy(tempString,oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].pchField,
				   oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField);

			tempString[oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField]= '\0';

			int nPcscfId = atoi(tempString);

			strcpy(pucTableName,"mrfc_inter_opt_identifier");
			

			 if(Offline_CS_RetVal_Success !=  Db_Insert_InterOperatorIdList(hDBThreadModContext_i,
																			pucTableName,nPcscfId,
																			poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_InterOperatorIdList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }
            
			// printf(" SQL QUERY EXECUTED \n");
			
			strcpy(pucTableName,"mrfc_app_server_info");

			 if(Offline_CS_RetVal_Success !=  Db_Insert_AppServerInfoList(hDBThreadModContext_i,
																		pucTableName,nPcscfId,
																		poCDRContents_i->oServiceInfo.oIMSInformation.oAppServerInfoList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_AppServerInfoList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }
			
            
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
			return Offline_CS_RetVal_Success;
 }

t_Offline_CS_ReturnVal  Db_Insert_ASReocrd(HDBMODULETHREADCONTEXT  	hDBThreadModContext_i,
										   t_CDR_Contents* 			poCDRContents_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
			if(NULL == hDBThreadModContext_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDBThreadModContext_i is NULL , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
			}
		
			if(NULL == poCDRContents_i)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
			}

			unsigned char   pucSQLQueryString[OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES] = {0};
			unsigned char 	pucTableName[OFFLINE_CS_DATABASE_TABLE_NAME_SIZE_IN_BYTES] = {0};
			
    
			unsigned int	unStringSize =0;
			unsigned int    unTempLengthOfSQLQueryColumnNames  = 0;
			unsigned int    unTempLengthOfSQLQueryColumnVlaues = 0;
			unsigned int    unNewLengthOfSQLQueryColumnNames   = 0;
	
			
            char    StartPartOFSQLQuery[OFFLINE_CS_SQL_QUERY_START_PART_STRING_SIZE_IN_BYTES]={0};
			char    ColumnNamesPartOfSQLQuery[OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES]={0};
			char*   pcMiddlePartOFSQLQuery = ") VALUES (" ;
			char    ColumnVlauesPartOfSQLQuery[OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES]={0};
			char*   pcEndPartOFSQLQuery = ")";
	
			DMBASE_DBEngineResult      oDBEngineResult_o = {0} ;

		   sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","as_records");

           
			if( poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality > 0 )
			{
				strcat(ColumnNamesPartOfSQLQuery,"record_type");
				sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}

			if( poCDRContents_i->bRetransmission == 1 )
			{
				strcat(ColumnNamesPartOfSQLQuery,",retransmission");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			if( unStringSize > 0  )
			{
				
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",sip_method") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
               strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",sip_method");
               unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			   if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			   {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			   }
			   sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode >= 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",role_of_node") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",role_of_node");
                unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode );
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",node_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",node_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",session_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",session_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",called_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",called_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_request_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_request_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_start_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_deliver_start_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp);
			}

		
			//copy service delivery end time stamp 
			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_deliver_end_time_stamp") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(ColumnNamesPartOfSQLQuery,",service_deliver_end_time_stamp");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%u'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp);
			}


			if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
			{
				//copy record opening time   
                // get current time and sned cpoy into that field
				// // TODO : add code for cause for record closing 
			}


			if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
			{
				// get current time and sned cpoy into that field
				// TODO : add code for incomplete  cdr indication  
			}
           

			if( poCDRContents_i->nLocalRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",local_record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",local_record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nLocalRecordSequenceNumber);
			}


			if( poCDRContents_i->nRecordSequenceNumber > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",record_sequence_number") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",record_sequence_number");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",poCDRContents_i->nRecordSequenceNumber);
			}

        
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ims_charging_identifier") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_charging_identifier");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",ggsn_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ggsn_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAlternateChargedPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",alternate_charged_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ggsn_address");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAlternateChargedPartyAddress)+ unStringSize ) >=
					OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				}
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucAlternateChargedPartyAddress);
			}


			if( poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_reason_return_code") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",service_reason_return_code");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.nCausedCode);
			}

			if( poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires > 0 )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",expires_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(ColumnNamesPartOfSQLQuery,",expires_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
						poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",event") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",event");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",access_network_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",access_network_information");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",service_context_id") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_context_id");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					    poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceContextId);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",number_of_portability_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",number_of_portability_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo);
			}


			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",carrier_select_routing_information") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",carrier_select_routing_information");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation);
			}

			unStringSize = strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",requested_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			    strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",requested_party_address");
			    unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress)+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
			    sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
					   poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress);
			}

			int nIndex = 0;
			char* pccomma = "'";
			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_calling_party_address") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_calling_party_address");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
				    OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
			    {
				    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
			    }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				}
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}

			nIndex = 0;
			if( poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.nNumberOfItemsInList > 0)
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",list_of_called_asserted_identity") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_called_asserted_identity");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex]);
				for ( nIndex = 1 ; nIndex < poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.nNumberOfItemsInList;
					  nIndex++ )
				{
					 unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
					 if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
					 {
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
					   return Offline_CS_RetVal_Failure;
					 }
					 sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],".%s",
					   poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex]);
	
				}
				if(( strlen(poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList.ucItems[nIndex])+ strlen(pccomma) ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],"%s",pccomma);
                
			}

			
            
			unStringSize = strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			if( unStringSize > 0  )
			{
				if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_data") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_data");
				unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
				if(( strlen(poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData)+ unStringSize ) >=
							OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES )
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn vlaues  string is too small, leaving\n");
				   return Offline_CS_RetVal_Failure;
				 }
				sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%s'",
						poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData);
			}

			if( (strlen(ColumnNamesPartOfSQLQuery) + strlen(",subscription_id_type") ) >= OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Culumn Names string is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",subscription_id_type");
			unStringSize = strlen(ColumnVlauesPartOfSQLQuery);
			sprintf(&ColumnVlauesPartOfSQLQuery[unStringSize],",'%d'",
					   poCDRContents_i->oServiceInfo.oSubscriptionId.nSubscriptionIdType);

			if( (strlen(StartPartOFSQLQuery)+ strlen(ColumnNamesPartOfSQLQuery)+ 
				 strlen(pcMiddlePartOFSQLQuery)+ strlen(ColumnVlauesPartOfSQLQuery) + strlen(pcEndPartOFSQLQuery)) > OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"SQL Query String is too small, leaving\n");
				return Offline_CS_RetVal_Failure;
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

			

			strcpy(pucSQLQueryString,"SELECT @@IDENTITY");

			if(DMBase_Retval_Success !=  DMBASE_DBEngineQuery(hDBThreadModContext_i->hMainDBModuleContext->hDiameterStackContext,
															  hDBThreadModContext_i->hDBEnginHandle,
															  pucSQLQueryString,
															  strlen(pucSQLQueryString),      
															  &oDBEngineResult_o))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"DMBASE_DBEngineQuery() Fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			
			
			char tempString[15];

			memcpy(tempString,oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].pchField,
				   oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField);

			tempString[oDBEngineResult_o.ppoAMPSDBEngineTable[0][0].nLengthOfField]= '\0';

			int nPcscfId = atoi(tempString);

			strcpy(pucTableName,"as_inter_opt_identifier");
			

			 if(Offline_CS_RetVal_Success !=  Db_Insert_InterOperatorIdList(hDBThreadModContext_i,
																			pucTableName,nPcscfId,
																			poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_InterOperatorIdList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }
            
			// printf(" SQL QUERY EXECUTED \n");
			
			 strcpy(pucTableName,"as_message_body");

			 if(Offline_CS_RetVal_Success !=  Db_Insert_MessageBodyList(hDBThreadModContext_i,
																		pucTableName,nPcscfId,
																		poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList ) )
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Db_Insert_MessageBodyList() Fails,Leaving\n");
				 return Offline_CS_RetVal_Failure;
			 }
			
            
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
			return Offline_CS_RetVal_Success;
}


/******************************************************************************
** Function Name: 
**			RF_Debug_PrintDBEnginResult
**  
** Description:
**		This function prints the results of returned by DBEnginQuery() function
**      this function is used just for Debugging Purpose.
**
** Parameters:
**		  DMBASE_DBEngineResult oDBEngineResult
**			 type: input
**			 detail : this is a object of structure of type DMBASE_DBEngineResult. 
**
**       
** Return:
**		An t_DiamAPIReturnVal value specify the success of the function.
** 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal RF_Debug_PrintDBEnginResult(DMBASE_DBEngineResult oDBEngineResult)
{
    
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

        int             nloopcounterRows = 0;
        int             nloopcounterColumns = 0;
        unsigned long   ulNumberOfResults   = oDBEngineResult.ulNumberOfResults;
        unsigned long   ulNumberOfRows      = oDBEngineResult.ulNumberOfRows;
        unsigned long   ulNumberOfColumns   = oDBEngineResult.ulNumberOfColumns;


        /*printf(" oDBEngineResult.ulNumberOfResults = %d \n",oDBEngineResult.ulNumberOfResults);
        printf(" oDBEngineResult.ulNumberOfRows = %d \n",oDBEngineResult.ulNumberOfRows);
        printf(" oDBEngineResult.ulNumberOfColumns = %d \n",oDBEngineResult.ulNumberOfColumns);*/

        for(nloopcounterRows = 0;nloopcounterRows<ulNumberOfRows; nloopcounterRows++)
        {
            for(nloopcounterColumns = 0;nloopcounterColumns<ulNumberOfColumns;nloopcounterColumns++)

           // printf(" First_Name  %u \t = oDBEngineResult.ppoAMPSDBEngineTable[nloopcounterRows][nloopcounterColumns].nLengthOfField");
           //printf(" ********** %d ",oDBEngineResult.ppoAMPSDBEngineTable[nloopcounterRows][nloopcounterColumns].nLengthOfField);
            PrintOctStr(oDBEngineResult.ppoAMPSDBEngineTable[nloopcounterRows][nloopcounterColumns] .pchField ,
                        oDBEngineResult.ppoAMPSDBEngineTable[nloopcounterRows][nloopcounterColumns].nLengthOfField);
            // printf("******** %s \n",oDBEngineResult.ppoAMPSDBEngineTable[nloopcounterRows][nloopcounterColumns] .pchField);
            //printf("\n");
            //printf(" First_Name  \t = ");
        }


        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
        return Offline_CS_RetVal_Success;
}



void PrintOctStr(unsigned char* puchTemp , unsigned int unSize)
{
	    //printf("\n");

	int i=0;
	for (i=0;i<unSize ; i++) 
	{
		//printf("%c",puchTemp[i]);
	}

	//printf("\n");
}

/******************************************************************************
** Function Name: 
**			Db_CreateAndInitModuleContext
**  
** Description:
**		This function is creating (DB Module Context by allocating memeory to 
**      Handle to DB module Context)  and intializating it .
**
** Parameters:
**		  HDBMODULECONTEXT*     	 	phDBModuleContext_o
**			 type: output
**			 detail : This is a pointer to DBModuleContext Handle . 
**
**       HDIAMETERSTACKCONTEXT    hDiameterContext_i
**			 type: input
**			 detail : this is handle to Diameter Base Protocol Stacke Context. 
**
**       
** Return:
**		An t_Offline_CS_ReturnVal value specifing  whether the RFApplicatioContect has 
**      been initialized successfully or any problem occured during initialization.
**
** 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Db_CreateAndInitModuleContext( HDBMODULECONTEXT*     	 	phDBModuleContext_o,
													  HDIAMETERSTACKCONTEXT    	hDiameterContext_i)
{

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");


        if(NULL == phDBModuleContext_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! phOffLineCSDBModuleContext_o is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL != *phDBModuleContext_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! *phOffLineCSDBModuleContext_o is Invalid , leaving\n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

		if(NULL == hDiameterContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! hDiameterContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }


        unsigned int unSize   = sizeof(t_DBModuleContext);
        if( DMBase_Retval_Success != DMBase_Malloc(NULL, (void **)phDBModuleContext_o,unSize))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Problem in Memory Allocation to phDBModuleContext_o\n");
            return Offline_CS_RetVal_Failure;
        }

		// now initilaize the Module context

		// set the diameter context  in module context
		(*phDBModuleContext_o)->hDiameterStackContext = hDiameterContext_i;

        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
        return Offline_CS_RetVal_Success;

 }

/******************************************************************************
** Function Name: 
**			Db_DestroyModuleContext
**  
** Description:
**		This function is destrying DB Module context by deallocating  
**      the allocated memory to it.
**
** Parameters:
**		  HDBMODULECONTEXT*        phDBModuleContext_i
**			 type: input
**			 detail : This is a pointer to handle of DB Module Local Context . 
**
**       
** Return:
**		An t_Offline_CS_ReturnVal value specify the success of the function. 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Db_DestroyModuleContext(HDBMODULECONTEXT*        phDBModuleContext_i)
{

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

        if(NULL == phDBModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! phDBModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if( NULL != *phDBModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! *phDBModuleContext_i is Invalid , leaving\n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

        

        DMBase_Free(NULL,(void*)phDBModuleContext_i);
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
        return Offline_CS_RetVal_Success;
}





t_Offline_CS_ReturnVal Offcs_Com_Create_P_CSCF_SQLInsertQueryStatement( HDBMODULECONTEXT     hRfAppDBModuleContext_i,
                                                                           char**                        ppchQueryStatement_o,
                                                                           unsigned long*                pulLengthOfQuery_o,
                                                                           t_CDR_Contents                oCDRContents)
{

         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

        /*if(NULL == hRfAppDBModuleContext_i)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! hRfAppDBModuleContext_i is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }


        if(NULL == ppchQueryStatement_o)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! ppchQueryStatement_o is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }

        if(NULL != *ppchQueryStatement_o)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! *ppchQueryStatement_o is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }
    
        if(NULL == pulLengthOfQuery_o)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! pulLengthOfQuery_o is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }



        unsigned int    unTempLengthOfSQLQueryColumnNames  = 0;
        unsigned int    unTempLengthOfSQLQueryColumnVlaues = 0;

        char*   pcList = NULL;
        char    TempUseBuffer[100];
        char   StartPartOFSQLQuery[50];
        char   ColumnNamesPartOfSQLQuery[10000];
        char*  pcMiddlePartOFSQLQuery = ") VALUES (" ;
        char   ColumnVlauesPartOfSQLQuery[10000];
        char*  pcEndPartOFSQLQuery = ")";

           sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","p_cscf_cdr_contents");

            if( (0!= oCDRContents.oRecordType.unNumOfOctets)&&( NULL!= oCDRContents.oRecordType.pnInteger32) )
            {
                strcat(ColumnNamesPartOfSQLQuery,"record_type");
                sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",*(oCDRContents.oRecordType.pnInteger32));
            }

            // TODO: Add code for retransmission here
            
            if( (0!= oCDRContents.oEventType.oOctStrSipMethod.unNumOfOctets)&&
                ( NULL!=oCDRContents.oEventType.oOctStrSipMethod.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oEventType.oOctStrSipMethod.pucOctetString,
                       oCDRContents.oEventType.oOctStrSipMethod.unNumOfOctets);
                TempUseBuffer[oCDRContents.oEventType.oOctStrSipMethod.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",sip_method");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

            if( (0!= oCDRContents.oEventType.oOctStrEvent.unNumOfOctets)&&
                ( NULL!=oCDRContents.oEventType.oOctStrEvent.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oEventType.oOctStrEvent.pucOctetString,
                       oCDRContents.oEventType.oOctStrEvent.unNumOfOctets);
                TempUseBuffer[oCDRContents.oEventType.oOctStrEvent.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",event");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

            if( (0!= oCDRContents.oEventType.oUnsigned32Expires.unNumOfOctets)&&
                ( NULL!=oCDRContents.oEventType.oUnsigned32Expires.punUnsigned32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",expires_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%d'",
                        *(oCDRContents.oEventType.oUnsigned32Expires.punUnsigned32));

            }

            if( (0!= oCDRContents.oRoleOfNode.unNumOfOctets)&&
                ( NULL!=oCDRContents.oRoleOfNode.pnInteger32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",role_of_node");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%d'",
                        *(oCDRContents.oRoleOfNode.pnInteger32));

            }


            if( (0!= oCDRContents.oNodeAddress.unNumOfOctets)&&
                ( NULL!=oCDRContents.oNodeAddress.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oNodeAddress.pucOctetString,
                       oCDRContents.oNodeAddress.unNumOfOctets);
                TempUseBuffer[oCDRContents.oNodeAddress.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",node_address");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }



            if( (0!= oCDRContents.oUserSessionId.unNumOfOctets)&&
                ( NULL!=oCDRContents.oUserSessionId.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oUserSessionId.pucOctetString,
                       oCDRContents.oUserSessionId.unNumOfOctets);
                TempUseBuffer[oCDRContents.oUserSessionId.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",session_id");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

            pcList = NULL;
            // Add List of Calling Party Address to SQL Query
            if( ( 0!= oCDRContents.oCallingPartyAddressList.nNumberOfItemsInList) )
            {
                Offcs_Com_GetList(&pcList,oCDRContents.oCallingPartyAddressList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_calling_party_address");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Called Party Address List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }

            // Add List of AssociatedURI List to SQL Query
            pcList = NULL;
            if( ( 0!= oCDRContents.oAssociatedURIList.nNumberOfItemsInList) )
            {
                Offcs_Com_GetList(&pcList,oCDRContents.oAssociatedURIList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_associated_uri");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Associated URI List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }



            if( (0!= oCDRContents.oCalledPartyAddress.unNumOfOctets)&&
                ( NULL!=oCDRContents.oCalledPartyAddress.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oCalledPartyAddress.pucOctetString,
                       oCDRContents.oCalledPartyAddress.unNumOfOctets);
                TempUseBuffer[oCDRContents.oCalledPartyAddress.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",called_party_address");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

           
            if( (0!= oCDRContents.oNumberPortabilityRoutingInfo.unNumOfOctets)&&
                ( NULL!=oCDRContents.oNumberPortabilityRoutingInfo.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oNumberPortabilityRoutingInfo.pucOctetString,
                       oCDRContents.oNumberPortabilityRoutingInfo.unNumOfOctets);
                TempUseBuffer[oCDRContents.oNumberPortabilityRoutingInfo.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",number_of_portability_routing_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }


            if( (0!= oCDRContents.oCarrierSelectRoutingInformation.unNumOfOctets)&&
                ( NULL!=oCDRContents.oCarrierSelectRoutingInformation.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oCarrierSelectRoutingInformation.pucOctetString,
                       oCDRContents.oCarrierSelectRoutingInformation.unNumOfOctets);
                TempUseBuffer[oCDRContents.oCarrierSelectRoutingInformation.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",carrier_select_routing_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }
  
         


            if( (0!= oCDRContents.oTimeStamps.oSipRequestTimeStamp.unNumOfOctets)&&
                ( NULL!= oCDRContents.oTimeStamps.oSipRequestTimeStamp.punUnsigned32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_request_time_stamp");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%u'",
                        *(oCDRContents.oTimeStamps.oSipRequestTimeStamp.punUnsigned32));
            }


            if( (0!= oCDRContents.oTimeStamps.oSipResponseTimeStamp.unNumOfOctets)&&
                ( NULL!= oCDRContents.oTimeStamps.oSipResponseTimeStamp.punUnsigned32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_deliver_start_time_stamp");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%u'",
                        *(oCDRContents.oTimeStamps.oSipResponseTimeStamp.punUnsigned32));
            }


            //TODO; Add code for Record openening Time Here 

            //TODO: Add code for Record Clouser Time Here

           
            pcList = NULL;
            if( ( 0!= oCDRContents.oInterOperatorIdentifierList.nNumberOfitemsInList ) )
            {
                Offcs_Com_GetListOfInteroperatorIdentifier(&pcList,oCDRContents.oInterOperatorIdentifierList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_inter_operator_identifier");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Inter-Operator-identifer  List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }

            if( (0!= oCDRContents.oIMSChargingIdentifier.unNumOfOctets)&& ( NULL!=oCDRContents.oIMSChargingIdentifier.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oIMSChargingIdentifier.pucOctetString, oCDRContents.oIMSChargingIdentifier.unNumOfOctets);
                TempUseBuffer[oCDRContents.oIMSChargingIdentifier.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_charging_identifier");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }


            pcList = NULL;
            if( ( 0!= oCDRContents.oSDPMediaComponentsList.nNumberOfitemsInList ) )
            {
                Offcs_Com_GetListOfSDPMediaComponents(&pcList,oCDRContents.oSDPMediaComponentsList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_sdp_media_components");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***SDP-Media-Components List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }


            if( (0!= oCDRContents.oCausedCode.unNumOfOctets)&& ( NULL!=oCDRContents.oCausedCode.pnInteger32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_reason_return_code");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%d'",
                        *(oCDRContents.oCausedCode.pnInteger32));

            }


            pcList = NULL;
            if( ( 0!= oCDRContents.oMessageBodyList.nNumberOfitemsInList ) )
            {
                Offcs_Com_GetListOfMessageBody( &pcList, oCDRContents.oMessageBodyList );
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_message_body");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Inter-Operator-identifer  List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }

            if( (0!= oCDRContents.oAccessNetworkInformation.unNumOfOctets)&& ( NULL!=oCDRContents.oAccessNetworkInformation.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oAccessNetworkInformation.pucOctetString, oCDRContents.oAccessNetworkInformation.unNumOfOctets);
                TempUseBuffer[oCDRContents.oAccessNetworkInformation.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",access_network_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }


            if( (0!= oCDRContents.oServiceContextId.unNumOfOctets)&& ( NULL!=oCDRContents.oServiceContextId.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oServiceContextId.pucOctetString, oCDRContents.oServiceContextId.unNumOfOctets);
                TempUseBuffer[oCDRContents.oServiceContextId.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_context_id");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }

            
            
            printf("******* ColumnVlauesPartOfSQLQuery %s \n\n",ColumnVlauesPartOfSQLQuery);

          unsigned int       unLengthOfStartPartOFSQLQuery         = strlen(StartPartOFSQLQuery);
         unsigned int       unLengthOfColumnNamesPartOfSQLQuery   = strlen(ColumnNamesPartOfSQLQuery);
         unsigned int       unLengthOfMiddlePartOFSQLQuery        = strlen(pcMiddlePartOFSQLQuery) ;
         unsigned int       unLengthOfColumnVlauesPartOfSQLQuery  = strlen(ColumnVlauesPartOfSQLQuery);
         unsigned int       unLengthOfEndPartOFSQLQuery           = strlen(pcEndPartOFSQLQuery);



         unsigned int unTotalQueryLength = ( unLengthOfStartPartOFSQLQuery  + unLengthOfColumnNamesPartOfSQLQuery + 
                                             unLengthOfMiddlePartOFSQLQuery + unLengthOfColumnVlauesPartOfSQLQuery + 
                                             unLengthOfEndPartOFSQLQuery );

        *pulLengthOfQuery_o  =  unTotalQueryLength;
           
        if(DMBase_Retval_Success != DMBase_Malloc(NULL,(void*)ppchQueryStatement_o,unTotalQueryLength))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Problem in Memory Allocation\n");
            return Offline_CS_RetVal_Failure;
        }
        
        DMBase_MemsetZero(*ppchQueryStatement_o,unTotalQueryLength);
        int nIndex = unLengthOfStartPartOFSQLQuery;

        memcpy(*ppchQueryStatement_o,StartPartOFSQLQuery,unLengthOfStartPartOFSQLQuery);

        nIndex = unLengthOfStartPartOFSQLQuery; 
        memcpy((*ppchQueryStatement_o +nIndex),ColumnNamesPartOfSQLQuery,unLengthOfColumnNamesPartOfSQLQuery);

        nIndex = nIndex + unLengthOfColumnNamesPartOfSQLQuery;
        memcpy((*ppchQueryStatement_o + nIndex ),pcMiddlePartOFSQLQuery,unLengthOfMiddlePartOFSQLQuery);

        nIndex = nIndex + unLengthOfMiddlePartOFSQLQuery;
        memcpy((*ppchQueryStatement_o + nIndex ),ColumnVlauesPartOfSQLQuery,unLengthOfColumnVlauesPartOfSQLQuery);

        nIndex = nIndex + unLengthOfColumnVlauesPartOfSQLQuery;
        memcpy((*ppchQueryStatement_o + nIndex ),pcEndPartOFSQLQuery,unLengthOfEndPartOFSQLQuery);

        //printf(" ******** %u \n\n",unTotalQueryLength);
       // printf(" Final Query is ***** %s \n\n",*ppchQueryStatement_o);*/

               
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
            return Offline_CS_RetVal_Success;
  }


void* cdf_db_ThreadInit(AMPS_HANDLE 		hAMPSHandle_i,
					   void* 			hUIOAgentsHandle_i, 
					   void* 			pvUserData_i)
   {
	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	   //printf("Entering  cdf_db_ThreadInit() \n");

	   if (NULL == hAMPSHandle_i) 
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hAMPSHandle_i is NULL , leaving\n");
		   return NULL;
	   }

	   if (NULL == hUIOAgentsHandle_i) 
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hUIOAgentsHandle_i is NULL , leaving\n");
		   return NULL;
	   }

	   if (NULL == pvUserData_i) 
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvUserData_i is NULL , leaving\n");
		   return NULL;
	   }
       
	   HDBMODULETHREADCONTEXT hDbModThreadCtxt = NULL;
	   HDBMODULECONTEXT	hMainDBModContext = (HDBMODULECONTEXT) pvUserData_i;


		if ( DMBase_Retval_Success != DMBase_Malloc(hMainDBModContext->hDiameterStackContext,
										(void**)&hDbModThreadCtxt,sizeof(t_DBModuleThreadContext))
			)
	
	
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," DMBase_Malloc failed for current thread ,Leaving \n");
			return NULL;
		}

		hDbModThreadCtxt->hMainDBModuleContext = hMainDBModContext;
		hDbModThreadCtxt->hUnitIOAgent = hUIOAgentsHandle_i;

        char                        pchHostName[] = "127.0.0.1";
        char                        pchUserName[] = "root";
        char                        pchPassword[]  = "NEWPASSWORD";
        char                        pchDataBaseName[] = "OfflineCharging";
        DMBASE_DBE_HANDLE           hBDEngineHandle= NULL ;


		//printf("\nBefore DBInit() \n");
        if( DMBase_Retval_Success != DMBASE_DBEngin_Init( hMainDBModContext->hDiameterStackContext,
                                                         pchHostName,
                                                         pchUserName,
                                                         pchPassword,
                                                         pchDataBaseName,
                                                         &hBDEngineHandle) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," DBEngin Initialization Fails , Leaving\n");
            return NULL;
        }

		hDbModThreadCtxt->hDBEnginHandle = hBDEngineHandle;

	/*	if(DMBase_Retval_Success != DMBASE_DBEngin_Init( hMainDBModContext->hDiameterStackContext,
														 hMainDBModContext->oCdf_db_config_params.oDBMSConfigParams.pucHostName,
														 hMainDBModContext->oCdf_db_config_params.oDBMSConfigParams.pucUserName,
														 hMainDBModContext->oCdf_db_config_params.oDBMSConfigParams.pucPassword,
														 hMainDBModContext->oCdf_db_config_params.oDBMSConfigParams.pucDatabaseName,
														 &(hDbModThreadCtxt->hDBEnginHandle) )
													)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," DBEngin Initialization Fails , Leaving\n");
			return NULL;
		}
	*/

	   //printf("Leaving  cdf_db_ThreadInit() \n");
	
	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
       return hDbModThreadCtxt ;// it will return the pointer to created data
 }

void cdf_db_ThreadCleanup(void* pvThreadCtxt_i)
{

   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

   if ( NULL == pvThreadCtxt_i )
   {
	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvCdrFWriterThreadCtxt_i  is NULL , leaving\n");
	   return;
   }

   HDBMODULETHREADCONTEXT  hDbModThreadContext = (HDBMODULETHREADCONTEXT) pvThreadCtxt_i; 

   
   

   HDIAMETERSTACKCONTEXT	hDiamStackContext = NULL;
   hDiamStackContext = ((HDBMODULETHREADCONTEXT) pvThreadCtxt_i)->hMainDBModuleContext->hDiameterStackContext;

   DMBASE_DBEngin_Cleanup(hDiamStackContext,hDbModThreadContext->hDBEnginHandle);

   
   DMBase_Free(hDiamStackContext,&pvThreadCtxt_i);

   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
}



/*
t_Offline_CS_ReturnVal OffLineCS_DB_Create_S_CSCF_SQLInsertQueryStatement( HOFFLINECSDBMODULECONTEXT     hRfAppDBModuleContext_i,
                                                                           char**                        ppchQueryStatement_o,
                                                                           unsigned long*                pulLengthOfQuery_o,
                                                                           t_CDR_Contents                oCDRContents)
{

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

        if(NULL == hRfAppDBModuleContext_i)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! hRfAppDBModuleContext_i is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }


        if(NULL == ppchQueryStatement_o)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! ppchQueryStatement_o is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }

        if(NULL != *ppchQueryStatement_o)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! *ppchQueryStatement_o is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }
    
        if(NULL == pulLengthOfQuery_o)
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! pulLengthOfQuery_o is NULL , leaving\n");
             return Offline_CS_RetVal_Null_Pointer;
         }



        unsigned int    unTempLengthOfSQLQueryColumnNames  = 0;
        unsigned int    unTempLengthOfSQLQueryColumnVlaues = 0;

        char*   pcList = NULL;
        char    TempUseBuffer[100];
        char   StartPartOFSQLQuery[50];
        char   ColumnNamesPartOfSQLQuery[10000];
        char*  pcMiddlePartOFSQLQuery = ") VALUES (" ;
        char   ColumnVlauesPartOfSQLQuery[10000];
        char*  pcEndPartOFSQLQuery = ")";

           sprintf(StartPartOFSQLQuery,"INSERT INTO %s ( ","s_cscf_cdr_contents");

            if( (0!= oCDRContents.oRecordType.unNumOfOctets)&&( NULL!= oCDRContents.oRecordType.pnInteger32) )
            {
                strcat(ColumnNamesPartOfSQLQuery,"record_type");
                sprintf(ColumnVlauesPartOfSQLQuery,"'%d'",*(oCDRContents.oRecordType.pnInteger32));
            }

            // TODO: Add code for retransmission here
            
            if( (0!= oCDRContents.oEventType.oOctStrSipMethod.unNumOfOctets)&&
                ( NULL!=oCDRContents.oEventType.oOctStrSipMethod.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oEventType.oOctStrSipMethod.pucOctetString,
                       oCDRContents.oEventType.oOctStrSipMethod.unNumOfOctets);
                TempUseBuffer[oCDRContents.oEventType.oOctStrSipMethod.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",sip_method");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

            if( (0!= oCDRContents.oEventType.oOctStrEvent.unNumOfOctets)&&
                ( NULL!=oCDRContents.oEventType.oOctStrEvent.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oEventType.oOctStrEvent.pucOctetString,
                       oCDRContents.oEventType.oOctStrEvent.unNumOfOctets);
                TempUseBuffer[oCDRContents.oEventType.oOctStrEvent.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",event");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

            if( (0!= oCDRContents.oEventType.oUnsigned32Expires.unNumOfOctets)&&
                ( NULL!=oCDRContents.oEventType.oUnsigned32Expires.punUnsigned32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",expires_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%d'",
                        *(oCDRContents.oEventType.oUnsigned32Expires.punUnsigned32));

            }

            if( (0!= oCDRContents.oRoleOfNode.unNumOfOctets)&&
                ( NULL!=oCDRContents.oRoleOfNode.pnInteger32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",role_of_node");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%d'",
                        *(oCDRContents.oRoleOfNode.pnInteger32));

            }


            if( (0!= oCDRContents.oNodeAddress.unNumOfOctets)&&
                ( NULL!=oCDRContents.oNodeAddress.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oNodeAddress.pucOctetString,
                       oCDRContents.oNodeAddress.unNumOfOctets);
                TempUseBuffer[oCDRContents.oNodeAddress.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",node_address");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }



            if( (0!= oCDRContents.oUserSessionId.unNumOfOctets)&&
                ( NULL!=oCDRContents.oUserSessionId.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oUserSessionId.pucOctetString,
                       oCDRContents.oUserSessionId.unNumOfOctets);
                TempUseBuffer[oCDRContents.oUserSessionId.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",session_id");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

            pcList = NULL;
            // Add List of Calling Party Address to SQL Query
            if( ( 0!= oCDRContents.oCallingPartyAddressList.nNumberOfItemsInList) )
            {
                OffLineCS_DB_GetList(&pcList,oCDRContents.oCallingPartyAddressList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_calling_party_address");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Called Party Address List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }

            // Add List of AssociatedURI List to SQL Query
            pcList = NULL;
            if( ( 0!= oCDRContents.oAssociatedURIList.nNumberOfItemsInList) )
            {
                OffLineCS_DB_GetList(&pcList,oCDRContents.oAssociatedURIList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_associated_uri");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Associated URI List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }



            if( (0!= oCDRContents.oCalledPartyAddress.unNumOfOctets)&&
                ( NULL!=oCDRContents.oCalledPartyAddress.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oCalledPartyAddress.pucOctetString,
                       oCDRContents.oCalledPartyAddress.unNumOfOctets);
                TempUseBuffer[oCDRContents.oCalledPartyAddress.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",called_party_address");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }


            if( (0!= oCDRContents.oRequestedPartyAddress.unNumOfOctets)&&
                ( NULL!=oCDRContents.oRequestedPartyAddress.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oRequestedPartyAddress.pucOctetString,
                       oCDRContents.oRequestedPartyAddress.unNumOfOctets);
                TempUseBuffer[oCDRContents.oRequestedPartyAddress.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",requested_party_address");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }

            if( (0!= oCDRContents.oNumberPortabilityRoutingInfo.unNumOfOctets)&&
                ( NULL!=oCDRContents.oNumberPortabilityRoutingInfo.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oNumberPortabilityRoutingInfo.pucOctetString,
                       oCDRContents.oNumberPortabilityRoutingInfo.unNumOfOctets);
                TempUseBuffer[oCDRContents.oNumberPortabilityRoutingInfo.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",number_of_portability_routing_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);

            }


            if( (0!= oCDRContents.oCarrierSelectRoutingInformation.unNumOfOctets)&&
                ( NULL!=oCDRContents.oCarrierSelectRoutingInformation.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oCarrierSelectRoutingInformation.pucOctetString,
                       oCDRContents.oCarrierSelectRoutingInformation.unNumOfOctets);
                TempUseBuffer[oCDRContents.oCarrierSelectRoutingInformation.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",carrier_select_routing_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }


            // Add List of Called Asserted Identity List to SQL Query
            pcList = NULL;
            if( ( 0!= oCDRContents.oCalledAssertedIdentityList.nNumberOfItemsInList) )
            {
                OffLineCS_DB_GetList(&pcList,oCDRContents.oCalledAssertedIdentityList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_called_asserted_identity");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Called Asserted Identity List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }


            if( (0!= oCDRContents.oPrivateUserId.unNumOfOctets)&&
                ( NULL!=oCDRContents.oPrivateUserId.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oPrivateUserId.pucOctetString,
                       oCDRContents.oPrivateUserId.unNumOfOctets);
                TempUseBuffer[oCDRContents.oPrivateUserId.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",private_user_id");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }


            if( (0!= oCDRContents.oTimeStamps.oSipRequestTimeStamp.unNumOfOctets)&&
                ( NULL!= oCDRContents.oTimeStamps.oSipRequestTimeStamp.punUnsigned32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_request_time_stamp");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%u'",
                        *(oCDRContents.oTimeStamps.oSipRequestTimeStamp.punUnsigned32));
            }


            if( (0!= oCDRContents.oTimeStamps.oSipResponseTimeStamp.unNumOfOctets)&&
                ( NULL!= oCDRContents.oTimeStamps.oSipResponseTimeStamp.punUnsigned32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_deliver_start_time_stamp");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%u'",
                        *(oCDRContents.oTimeStamps.oSipResponseTimeStamp.punUnsigned32));
            }


            //TODO; Add code for Record openening Time Here 

            //TODO: Add code for Record Clouser Time Here

            pcList = NULL;
            if( ( 0!= oCDRContents.oAppServerInfoList.nNumberOfitemsInList ) )
            {
                OffLineCS_DB_GetListOfAppServerInfo(&pcList,oCDRContents.oAppServerInfoList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_application_server_info");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Application Server Information List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }


            pcList = NULL;
            if( ( 0!= oCDRContents.oInterOperatorIdentifierList.nNumberOfitemsInList ) )
            {
                OffLineCS_DB_GetListOfInteroperatorIdentifier(&pcList,oCDRContents.oInterOperatorIdentifierList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_inter_operator_identifier");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Inter-Operator-identifer  List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }

            if( (0!= oCDRContents.oIMSChargingIdentifier.unNumOfOctets)&& ( NULL!=oCDRContents.oIMSChargingIdentifier.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oIMSChargingIdentifier.pucOctetString, oCDRContents.oIMSChargingIdentifier.unNumOfOctets);
                TempUseBuffer[oCDRContents.oIMSChargingIdentifier.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_charging_identifier");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }


            pcList = NULL;
            if( ( 0!= oCDRContents.oSDPMediaComponentsList.nNumberOfitemsInList ) )
            {
                OffLineCS_DB_GetListOfSDPMediaComponents(&pcList,oCDRContents.oSDPMediaComponentsList);
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_sdp_media_components");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***SDP-Media-Components List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }


            if( (0!= oCDRContents.oCausedCode.unNumOfOctets)&& ( NULL!=oCDRContents.oCausedCode.pnInteger32 ) )
            {
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_reason_return_code");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%d'",
                        *(oCDRContents.oCausedCode.pnInteger32));

            }


            pcList = NULL;
            if( ( 0!= oCDRContents.oMessageBodyList.nNumberOfitemsInList ) )
            {
                OffLineCS_DB_GetListOfMessageBody( &pcList, oCDRContents.oMessageBodyList );
                
                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",list_of_message_body");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",pcList);
                printf(" ***Inter-Operator-identifer  List ******* %s\n\n\n",pcList);
                if(NULL != pcList)
                {
                    free(pcList);
                }
            }

            if( (0!= oCDRContents.oAccessNetworkInformation.unNumOfOctets)&& ( NULL!=oCDRContents.oAccessNetworkInformation.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oAccessNetworkInformation.pucOctetString, oCDRContents.oAccessNetworkInformation.unNumOfOctets);
                TempUseBuffer[oCDRContents.oAccessNetworkInformation.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",access_network_information");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }


            if( (0!= oCDRContents.oServiceContextId.unNumOfOctets)&& ( NULL!=oCDRContents.oServiceContextId.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oServiceContextId.pucOctetString, oCDRContents.oServiceContextId.unNumOfOctets);
                TempUseBuffer[oCDRContents.oServiceContextId.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",service_context_id");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }

            if( (0!= oCDRContents.oIMSCommunicationServiceIdentifier.unNumOfOctets)&& ( NULL!=oCDRContents.oIMSCommunicationServiceIdentifier.pucOctetString ) )
            {
                memcpy(TempUseBuffer,oCDRContents.oIMSCommunicationServiceIdentifier.pucOctetString, oCDRContents.oIMSCommunicationServiceIdentifier.unNumOfOctets);
                TempUseBuffer[oCDRContents.oIMSCommunicationServiceIdentifier.unNumOfOctets]= '\0';

                unTempLengthOfSQLQueryColumnNames = strlen(ColumnNamesPartOfSQLQuery);
                strcat(&ColumnNamesPartOfSQLQuery[unTempLengthOfSQLQueryColumnNames],",ims_communication_service_id");
                unTempLengthOfSQLQueryColumnVlaues = strlen(ColumnVlauesPartOfSQLQuery);
                sprintf(&ColumnVlauesPartOfSQLQuery[unTempLengthOfSQLQueryColumnVlaues],",'%s'",TempUseBuffer);
            }

            
            printf("******* ColumnVlauesPartOfSQLQuery %s \n\n",ColumnVlauesPartOfSQLQuery);

          unsigned int       unLengthOfStartPartOFSQLQuery         = strlen(StartPartOFSQLQuery);
         unsigned int       unLengthOfColumnNamesPartOfSQLQuery   = strlen(ColumnNamesPartOfSQLQuery);
         unsigned int       unLengthOfMiddlePartOFSQLQuery        = strlen(pcMiddlePartOFSQLQuery) ;
         unsigned int       unLengthOfColumnVlauesPartOfSQLQuery  = strlen(ColumnVlauesPartOfSQLQuery);
         unsigned int       unLengthOfEndPartOFSQLQuery           = strlen(pcEndPartOFSQLQuery);



         unsigned int unTotalQueryLength = ( unLengthOfStartPartOFSQLQuery  + unLengthOfColumnNamesPartOfSQLQuery + 
                                             unLengthOfMiddlePartOFSQLQuery + unLengthOfColumnVlauesPartOfSQLQuery + 
                                             unLengthOfEndPartOFSQLQuery );

        *pulLengthOfQuery_o  =  unTotalQueryLength;
           
        if(DMBASE_Retval_Success != DMBase_Malloc(NULL,(void*)ppchQueryStatement_o,unTotalQueryLength))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Problem in Memory Allocation\n");
            return Offline_CS_RetVal_Failure;
        }
        
        DMBase_MemsetZero(*ppchQueryStatement_o,unTotalQueryLength);
        int nIndex = unLengthOfStartPartOFSQLQuery;

        memcpy(*ppchQueryStatement_o,StartPartOFSQLQuery,unLengthOfStartPartOFSQLQuery);

        nIndex = unLengthOfStartPartOFSQLQuery; 
        memcpy((*ppchQueryStatement_o +nIndex),ColumnNamesPartOfSQLQuery,unLengthOfColumnNamesPartOfSQLQuery);

        nIndex = nIndex + unLengthOfColumnNamesPartOfSQLQuery;
        memcpy((*ppchQueryStatement_o + nIndex ),pcMiddlePartOFSQLQuery,unLengthOfMiddlePartOFSQLQuery);

        nIndex = nIndex + unLengthOfMiddlePartOFSQLQuery;
        memcpy((*ppchQueryStatement_o + nIndex ),ColumnVlauesPartOfSQLQuery,unLengthOfColumnVlauesPartOfSQLQuery);

        nIndex = nIndex + unLengthOfColumnVlauesPartOfSQLQuery;
        memcpy((*ppchQueryStatement_o + nIndex ),pcEndPartOFSQLQuery,unLengthOfEndPartOFSQLQuery);

        //printf(" ******** %u \n\n",unTotalQueryLength);
       // printf(" Final Query is ***** %s \n\n",*ppchQueryStatement_o);

               
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
            return Offline_CS_RetVal_Success;
 }

*/

