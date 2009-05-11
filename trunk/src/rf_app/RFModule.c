#include"RF_defines.h"
#include"RF_include.h"
#include"RFModule.h"
#include "offcs_com_cdr_contents.h"
#include "offcs_com_defines.h"
#include "offcs_app_defined_avp.h"
#include "dappcommon_srvrconfload.h"


extern int  g_nDiamBaseTraceID;
HRFAPPLICATIONCONTEXT            hRFApplicationContext = NULL;

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
**     RFAplicationContext or any problem occured.
**
** 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
int	Rf_EvtValidationCallback( AMPS_APP_HANDLE     r_hAMPS_APP_HANDLE,
                              int                 r_nEventID,
                              void*               r_pvEventPayload)
{
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");


        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
        return AMPS_SUCCESS;
}

/******************************************************************************
** Function Name: 
**			RFModule_ModuleCleanup
**  
** Description:
**		This function is a clean up function of RfModule.All the momory allocated to 
**		RfModule is must to free here.
**
** Parameters:
**		  AMPS_HANDLE                 r_hAMPS_HANDLE
**			 type: input
**			 detail : this is handle to AMPS Frame work. 
**			
**		 AMPS_MODULE_HANDLE          r_hAMPS_MODULE_HANDLE
**			 type: input
**			 detail : this is handle to the RfModule . 
**
**
** Return:
**		Its return value is void.
**
** 
*******************************************************************************
** Created By: 	Simab Shahid	
*******************************************************************************
*******************************************************************************
*/
void RFModule_ModuleCleanup( AMPS_HANDLE                 r_hAMPS_HANDLE,
                             AMPS_MODULE_HANDLE          r_hAMPS_MODULE_HANDLE)
{
     DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

	 HRFAPPLICATIONCONTEXT     hRFModuleContext = NULL;

	 if( NULL == r_hAMPS_MODULE_HANDLE)
	 {
		 return;
	 }

	 hRFModuleContext = (HRFAPPLICATIONCONTEXT)r_hAMPS_MODULE_HANDLE;
	 //printf(" Module Cleane up \n");

	 AMPS_HashTableDeleteAllEntriesEx(r_hAMPS_HANDLE,
									  hRFModuleContext->HandleToSessionTable,
									  Rf_SessionTableProcessNodeDataCallback,
									  hRFModuleContext);
    AMPS_HashTableDelete (r_hAMPS_HANDLE, hRFModuleContext->HandleToSessionTable);
    DMBase_Free(NULL,(void*)&hRFModuleContext);
    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
    return ;

}

void  Rf_SessionTableProcessNodeDataCallback( AMPS_HANDLE 			r_hAMPS_HANDLE,
											  AMPS_APP_HANDLE 		r_hAMPS_APP_HANDLE,
											  AMPS_APP_DATA_HANDLE 	r_hAMPS_APP_DATA_HANDLE)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	//printf(" Rf_SessionTableProcessNodeDataCallback call back fired \n");

    if(NULL == r_hAMPS_HANDLE )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter r_hAMPS_HANDLE, Leaving \n");
		return ;
	}

	if(NULL == r_hAMPS_APP_HANDLE )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter r_hAMPS_APP_HANDLE, Leaving \n");
		return ;
	}

   if(NULL == r_hAMPS_APP_DATA_HANDLE )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter r_hAMPS_APP_DATA_HANDLE , Leaving \n");
		return ;
	}

	t_SessionObject*			poSessionObject = NULL;
	HRFAPPLICATIONCONTEXT     	hRFModuleContext = NULL;

	hRFModuleContext = (HRFAPPLICATIONCONTEXT)r_hAMPS_APP_HANDLE;

	poSessionObject = (t_SessionObject*)r_hAMPS_APP_DATA_HANDLE;

	if ( NULL != poSessionObject->pListOfUnprocessedACRInfo )
	{
			// Make free the list of unprocessed ACRInfo inside the Session Object
			if(DMBase_Retval_Success !=  DMBase_SListFree( hRFModuleContext->hDiameterStackContext,
														  &(poSessionObject->pListOfUnprocessedACRInfo),
														  Rf_LListFreeLinkDataCallback))
			{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Freeing of SList node  Fails , Leaving \n");
					return ;
			}
			poSessionObject->pListOfUnprocessedACRInfo = NULL;
	}


	// before destroying the session ,timer for that session must be killed
	if(Offline_CS_RetVal_Success != Rf_KillSessionTimer(hRFModuleContext,poSessionObject))
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Killing of Session timer Fails , Leaving \n");
		return ;
	}

	DMBase_Free( hRFModuleContext->hDiameterStackContext,(void*)&(poSessionObject->poLastReceivedACRInfo));

	DMBase_Free(hRFModuleContext->hDiameterStackContext, (void*)&(poSessionObject->poTimerDataForCurrSess) );

	DMBase_Free(hRFModuleContext->hDiameterStackContext, (void*)&poSessionObject );
	
	// TODO:then delete the session from session Table
	/*if(DMBase_Retval_Success != DMBase_HashTableRemove( hRFModuleContext_i->hDiameterStackContext,
														hRFModuleContext_i->HandleToSessionTable, 
														poDiamHashTableKey_i) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_HashTableRemove() Fails , Leaving \n");
		return Offline_CS_RetVal_Failure;
	}*/


	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
     return ;
}

/*typedef void(*AMPS_HashTableProcessNodeDataCallback)(AMPS_HANDLE r_hAMPS_HANDLE, 
													 AMPS_APP_HANDLE r_hAMPS_APP_HANDLE,
													  AMPS_APP_DATA_HANDLE r_hAMPS_APP_DATA_HANDLE);
													  */
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
**			 detail : This is void pointer to data send by its Loader module through AMPS. 
**
** Return: Its rerurn value is a void pointer.
** 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
AMPS_MODULE_HANDLE	RFModule_ModuleInit(AMPS_HANDLE     hAMPS_HANDLE,
                                        void*           pvOfflineChargingServerLocalContext_i)
{

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		HRFAPPLICATIONCONTEXT     hRFModuleContext = NULL;

        if( NULL == hAMPS_HANDLE )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hAMPS_HANDLE is NULL pointer, Leaving\n");
            return ((void*)hRFModuleContext);
        }

        if(NULL == pvOfflineChargingServerLocalContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvOfflineChargingServerLocalContext_i is NULL pointer, Leaving\n");
            return ((void*)hRFModuleContext);
        }

		t_cdf_config_params*	poCDF_ConfParams = (t_cdf_config_params*) pvOfflineChargingServerLocalContext_i;

		
        HDIAMETERSTACKCONTEXT    hDiameterStackContext  = poCDF_ConfParams->hDiameterStackContext;


        // Highly recommanded to Make All RF Module Initializations Here(inside Rf_InitModuleContext())       
        if(DMBase_Retval_Success != Rf_CreateAndInitModuleContext(&hRFModuleContext,hDiameterStackContext ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Rf_CreateAndInitModuleContext() Fails, Leaving\n");
            return ((void*)hRFModuleContext);
        }

		
       // Register RF Application callback 
       if(DMBase_Retval_Success != DMBase_RegisterApplicationCallback(hRFModuleContext->hDiameterStackContext,
                                                                      RF_APPLICATION_ID,Rf_ProcessACRMsg,NULL,
                                                                      (void*)hRFModuleContext))
       {
           DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CallBack registration Fails, Leaving\n");
           return ((void*)hRFModuleContext);
       }

       // Be Registered for all Rf Module internal Avents Here inside Rf_RegisterForModuleEvents()       
        if(Offline_CS_RetVal_Success != Rf_RegisterForModuleEvents( hRFModuleContext ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Registration for Module Rf module Internal events Fails, Leaving\n");
            return ((void*)hRFModuleContext);
        }

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
        return ((void*)hRFModuleContext);
  }

/******************************************************************************
** Function Name: 
**			Rf_CreateAndInitModuleContext
**  
** Description:
**		This function is creating (RF Appliation Context by allocating memeory to 
**      Handle to Rf Appliation Context)  and intializating it .
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT*    phRFApplicationContext_o
**			 type: output
**			 detail : This is a pointer to RFApplicationContext Handle . 
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
t_Offline_CS_ReturnVal Rf_CreateAndInitModuleContext(HRFAPPLICATIONCONTEXT*    phRFModuleContext_o,
													 HDIAMETERSTACKCONTEXT     hDiameterContext_i)

 {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");


		if(NULL == phRFModuleContext_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"phRFModuleContext_o is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL != *phRFModuleContext_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"*phRFModuleContext_o is Invalid Parameter, leaving\n");
            return Offline_CS_RetVal_Invalid_Parameter;
        }

		if(NULL == hDiameterContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDiameterContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }


		unsigned int unSize   = sizeof(t_RFApplicationContext);
		if(DMBase_Retval_Success != DMBase_Malloc(NULL, (void **)phRFModuleContext_o,unSize))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Allocating memory to Rf Module context\n");
			return Offline_CS_RetVal_Failure;
		}
        
        
		(*phRFModuleContext_o)->hDiameterStackContext = hDiameterContext_i;

		if( Offline_CS_RetVal_Success !=Rf_CreateSessionTable(*phRFModuleContext_o) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Creating Session Table\n");
			return Offline_CS_RetVal_Failure;
		}

		// will be set from configuration file later
		(*phRFModuleContext_o)->bLoggingInCdrFileEnabled = 1;
		(*phRFModuleContext_o)->bLoggingInDBEnabled	  = 0;

        // TODO: Highly recommanded to make further RF App initlizations Here 


		// just for testing purpose only 

        
		/*FILE *fileptr = NULL;
		fileptr = fopen("TestCdrFile.txt","r");

		char a[10];
		if (fileptr != NULL)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Cdr File Could not be Created, file of same name allready exists\n");
			fclose(fileptr);
            return Offline_CS_RetVal_Failure;
		}

		// now open the file with append Mod 
		fileptr = fopen("TestCdrFile.txt","a");

		if( NULL == fileptr )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Cdr File Could not be Created\n");
            return Offline_CS_RetVal_Failure;
		}

		printf(" New file Created \n");
		
		(*phRFModuleContext_o)->fptr = fileptr ;*/
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return Offline_CS_RetVal_Success;

 }
/******************************************************************************
** Function Name: 
**			Rf_CreateSessionTable
**  
** Description:
**		This function is creating SessionTable and setting its handle in 
**		phRFModuleContext Context.
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT    hRFModuleContext_o
**			 type: input/output	
**			 detail : This is a Handle to Rf Module Context. 
**       
** Return:
**		An t_Offline_CS_ReturnVal value specifing  whether Session Table has been created     
**		successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_CreateSessionTable(HRFAPPLICATIONCONTEXT    hRFModuleContext_o)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");


    DMBASE_HASH_HANDLE       HandleToSessionTable   = NULL ;

    if(NULL == hRFModuleContext_o)
    {
           DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_io is NULL , leaving\n");
           return Offline_CS_RetVal_Null_Pointer;
     }
         
    if(DMBase_Retval_Success != DMBase_CreateHashTable(hRFModuleContext_o->hDiameterStackContext,
                                                       OFFLINE_CS_HASHTABLE_MAX_SIZE, &HandleToSessionTable,
													   NULL))
    {
          DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Session Table\n");
          return Offline_CS_RetVal_Failure;
    }
    hRFModuleContext_o->HandleToSessionTable = HandleToSessionTable;
		
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
	return Offline_CS_RetVal_Success;

}
/******************************************************************************
** Function Name: 
**			Rf_RegisterForModuleEvents
**  
** Description:
**		Inside this function Rf Module is registering for all Interenal Events, that it 
**		wants to catch from all other modules of Offline Charging server.
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT   hRFModuleContext_i
**			 type: input
**			 detail : This is a Handle to Rf Module context. 
**       
** Return:
**		An t_Offline_CS_ReturnVal value specifing  whether the registeration for all 
**      Rf Module internal Events has been made sucessfully or any 
**      problem occured during registration.
**
** 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal Rf_RegisterForModuleEvents(HRFAPPLICATIONCONTEXT   hRFModuleContext_i)
 {

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n"); 

        if(NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        //set event handle validation callback before Registration
		if( DMBase_Retval_Success != DMBase_EvtSysSetEvtHandleValidationCallback( hRFModuleContext_i->hDiameterStackContext,
																				  Rf_EvtValidationCallback ) )
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_EvtSysSetEvtHandleValidationCallback() Fail, Leaving\n");
			 return Offline_CS_RetVal_Failure;
		}      


        //Rf Module is registering for DBResponse Event,
		// which will be triggered  by DB Module After Completeion 
        //of DataBase Operation requested by RF Module
        if( DMBase_Retval_Success != DMBase_EvtSysRegExtEvt( hRFModuleContext_i->hDiameterStackContext,
															 OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_RESPONSE_FROM_CGF,
															 Rf_CGFResponseEvtHandler,
															 (void*)hRFModuleContext_i) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Registeration For Response Event Fail, Leaving\n");
			return Offline_CS_RetVal_Failure;
		} 
        
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
        return Offline_CS_RetVal_Success;
 }





/******************************************************************************
** Function Name: 
**			Rf_CGFResponseEvtHandler
**  
** Description:
**		This function is  handler to  response event gentreted  by the 
**      CGF Module to inform about the status of Cdrs writing operation
**		in Cdr file and DB Module.
**
** Parameters:
**		  AMPS_HANDLE 		hAMPS_HANDLE_i
**			 type: input
**			 detail : This is a Handle to AMPS Framework. 
**
**       AMPS_APP_HANDLE 	    hAMPS_APP_HANDLE_i
**			 type: input
**			 detail : This is handle to data that Rf Module has sent to it.
**
**      void* 			    pvEventHandle_i
**			 type: input
**			 detail :
**
**      void* 			    pvDBResponseEvtData_i
**          type: input
**			 detail : This is void pointer to the data send by CdrFileWriter Module 
**                    to RFModule in response of Request to write  Cdrs in  file.                    
**       
** Return:
**		its return type is void
** 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
void  Rf_CGFResponseEvtHandler( AMPS_HANDLE 		    hAMPS_HANDLE_i,
								AMPS_APP_HANDLE 	    hAMPS_APP_HANDLE_i, 
								void* 			    	pvEventHandle_i,
								void* 			    	pvEvtData_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG,"Event received from CGF Module \n");


		//printf("\n\n\n FINALLY !!!! Rf has received reply from CGF \n\n\n\n\n ");

		

        _Bool                   		bACRInfo                 = 0;
		t_ACRInfo*              		poACRInfo                = NULL;
        HDIAMETERMESSAGE        		hACAMsg                  = NULL;
        t_SessionObject*	    		poSessionObject          = NULL;
        HRFAPPLICATIONCONTEXT     		hRFModuleContext 	 	 = NULL;

		if( NULL == hAMPS_HANDLE_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hAMPS_HANDLE_i is Null,Leaving\n");
            return;
        }

        if( NULL == pvEvtData_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Null Pointer pvEvtData_i is received form CdrFileWriter Module,Leaving\n");
            return;
        }

		if( NULL == hAMPS_APP_HANDLE_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Null Pointer hAMPS_APP_HANDLE_i is received ,Leaving\n");
            return;
        }

		
		hRFModuleContext = (HRFAPPLICATIONCONTEXT)hAMPS_APP_HANDLE_i;
        t_ResponseEvtDataFromCdrFWModule*   poResponseDataToRFModule;	

		poResponseDataToRFModule = (t_ResponseEvtDataForRFModule*)pvEvtData_i;

		// do something here for handling response from db and cdrfw module

		hRFModuleContext->unRecvdfrmCGFEventCounter++;
		//printf("\n\n Events recvd in Rf from CGF  = %d \n",hRFModuleContext->unRecvdfrmCGFEventCounter);

        
        t_DiamHashTableKey	oDiamHashTableKey ;
		int nSize = strlen(poResponseDataToRFModule->ucSessionId);
		memcpy(oDiamHashTableKey.puchKey,poResponseDataToRFModule->ucSessionId,nSize);
		oDiamHashTableKey.unKeyLength = nSize;
		if( Offline_CS_RetVal_Success != Rf_SearchObjectInSessionTable( hRFModuleContext,
																		&oDiamHashTableKey,
																		&poSessionObject) )
		{
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Searching for session Object in Session Table Fails, Leaving\n");
		   return;
		}

		

	   if( NULL == poSessionObject )
	   {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " No Object found in session table against the session key , Leaving\n");
			return;
	   }

       
	   if( Offline_CS_RetVal_Success != Rf_SearchACRInfoInSessionObject( hRFModuleContext, poSessionObject,
																		 poResponseDataToRFModule->unAcctRecordNumber,
																		 &poACRInfo, &bACRInfo) )
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Search for ACRInfo in session Object Fails, Leaving\n");
			 return;
		}

	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG, "poResponseDataToRFModule->unAcctRecordNumber = %d \n",poResponseDataToRFModule->unAcctRecordNumber);

      
	   if( NULL == poACRInfo )
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " No ACRInfo found in session Object  , Leaving\n");
		   return;
	   }
      

	   HPEERTABLEENTRY		    hPeerTableEntry = poSessionObject->hPeerTabEntry;

	   if( Offline_CS_RetVal_Success != Rf_GenerateACAMsg(  hRFModuleContext, hPeerTableEntry,
															poResponseDataToRFModule->unResultCode,
															poSessionObject->ucSessionID, poACRInfo))
	   {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Rf_GenerateACAMsg() Fails, Leaving\n");
			
	   }
      
      // Now check the ACR number , if it is Stop_Record Delete the whole Session and close the CDR
	   if( DMBASE_STOP_RECORD == poACRInfo->nAcctRecordType )
	   {
	 			//printf(" Stop record recieved \n");
				// Delete entire Session from sesion table , as the last ACR of that Session 
				// has been Received and process  successfully 
				poSessionObject->hPeerTabEntry = NULL;
				if( Offline_CS_RetVal_Success != Rf_DeleteSession( hRFModuleContext, poSessionObject,
																   &oDiamHashTableKey ))
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Session Deletion Fails, Leaving\n");
					return;
				}
	   }
	   else
	   {
				// Delete the ACRInfo related to Last received ACR from the session table, as it has been processed successfully
				if( Offline_CS_RetVal_Success != Rf_DeleteACRInfoFromSessionTable( hRFModuleContext,poSessionObject, &oDiamHashTableKey,
																				   poResponseDataToRFModule->unAcctRecordNumber ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Deletion of ACRInfo From Session Table Fails, Leaving\n");
					return;
				}
	   }

	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG, "Send Message to Peer \n");

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
       return;

}
/******************************************************************************
** Function Name: 
**			Rf_InterimIntervalExpiryNotifyHandler
**  
** Description:
**		This is a handler function to InterimIntervalExpiry Timer.
**      this function is called each time when the timer for 
        InterimInterval expires.
**
** Parameters:
**		  void* pvHandle_i
**			 type: input
**			 detail : . 
**
**       void* pvCorrID
**			 type: input
**			 detail : this is a void pointer to data received by the Timer
**                    handler function From the application which has set the 
**                    timer. 
**       
** Return:
**		An t_Offline_CS_ReturnVal value specifing  whether function is returning    
**		successfully or any problem occured. 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
int Rf_InterimIntervalExpiryNotifyHandler(void* pvHandle_i, void* pvTimerData_i )
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG,"Entering \n");
  
        int                     nResultCode         	= 0;
        _Bool				    bRecordFound        	= 0;
        t_ACRInfo*              poACRInfo           	= NULL;
        HRFAPPLICATIONCONTEXT	hRfModuleContext 		= NULL;
		
        if( NULL == pvTimerData_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "pvTimerData_i is Null Pointer, Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		if( NULL == pvHandle_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "pvHandle_i is Null Pointer , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

        t_TimerDataObject* poTimerData =(t_TimerDataObject*)pvTimerData_i;
		hRfModuleContext = (HRFAPPLICATIONCONTEXT)(poTimerData->hRfModuleContext);

		
		 t_DiamHashTableKey	oDiamHashTableKey ;
		 int nSize = strlen(poTimerData->pucSessionId);
		 memcpy(oDiamHashTableKey.puchKey,poTimerData->pucSessionId,nSize);
		 oDiamHashTableKey.unKeyLength = nSize;
		 t_SessionObject*	poSessionObject = NULL;
		 if(Offline_CS_RetVal_Success != Rf_SearchObjectInSessionTable( hRfModuleContext,&oDiamHashTableKey,
																		&poSessionObject))
		 {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Searching Session Object In Session Table  Fails, Leaving\n");
            return Offline_CS_RetVal_Failure;
		 }

		 if(NULL == poSessionObject)
		 {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Timer is expired for unkown session, Leaving\n");
            return Offline_CS_RetVal_Failure;
		 }

		 if(Offline_CS_RetVal_Success != Rf_SearchACRInfoInSessionObject( hRfModuleContext,poSessionObject,
																		  poTimerData->unAcctRecordNumber,
																		  &poACRInfo,&bRecordFound))
		 {
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Search ACRInfo In Session Object Fails, Leaving\n");
			 return Offline_CS_RetVal_Failure;
		 }

		 if( 0 == bRecordFound )
		 {
			 //DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Requested ACRInfo not found in Session Object , Leaving\n");
			 poACRInfo = poSessionObject->poLastReceivedACRInfo;
		 }


		 HPEERTABLEENTRY	    hPeerTabEntry  = poSessionObject->hPeerTabEntry ;
		 // DMBASE_INTERIM_INTERVAL_EXPIRE is application Defined result code
		 nResultCode = DMBASE_INTERIM_INTERVAL_EXPIRE;
		/* if( Offline_CS_RetVal_Success != Rf_GenerateACAMsg( hRfModuleContext, hPeerTabEntry,nResultCode,
															 poSessionObject->ucSessionID,poACRInfo))
		 {
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Rf_GenerateACAMsg Failed, Leaving\n");
			 //return Offline_CS_RetVal_Failure;
		 }*/

		 //note , the session needs to be deleted whether or not the ACA sending was successful , hence 
		 //fall thrrough instead of returning from error above.
		
	   

	   
		 // Since the expected ACR has not been recieved during the defined Time Interval,
		 // hence Deleting the  entire Session related to that ACR from sesion table 
		 //printf("\n\n Inside Interim Interval Time Out Handler \n\n");
		 if( Offline_CS_RetVal_Success != Rf_DeleteSession( hRfModuleContext,poSessionObject,
															&oDiamHashTableKey ))
		 {
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Deletion of  Session Fails, Leaving\n");
			 return Offline_CS_RetVal_Failure;
		 }
		  //printf("\n\n Leaving Interim Interval Time Out Handler \n\n");

       
		 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," ACA Message has been sent to Peer, Leaving\n");
		 return Offline_CS_RetVal_Success;
  }
/******************************************************************************
** Function Name: 
**			Rf_ProcessACRMessage
**  
** Description:
**		This function Processes ACR message received by the Rf Application      
**
** Parameters:
**        HPEERTABLEENTRY               hPeer_i
**            type: Input
**            Detail:   This is Handle to PeerTable Entry
**
**		  HDIAMETERMESSAGE        hRFMsg_i
**			 type: input
**			 detail : This is a Handle to Message Received By RF Application. 	 
**
**		  HRFAPPLICATIONCONTEXT         hRFApplicationContext
**			 type: input/output
**			 detail : This is void pointer to data. This data has been 
**					  sent by Rf Module during registeration of this callback.
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  wether the recieved ACR has been 
**      processed successfully or any problem occurred during processing.
** 
*******************************************************************************
** Created By: 	Simab Shahid	 
*******************************************************************************
*******************************************************************************
*/
t_DiamAPIReturnVal  Rf_ProcessACRMsg( HPEERTABLEENTRY               hPeer_i, 
                                      HDIAMETERMESSAGE              hACRMsg_i,
                                      void*                         pvhRFModuleContext_i)
{
		_Bool							bRecordFound = 0;
        t_DiamOctetString               oRcvdSessionID; 
        t_DiamUTF8String*               poSessionID             = NULL;        
        t_ACRInfo*                      poACRInfo               = NULL;
        DMBASE_HASHTABLE_KEY_HANDLE     hSessionTableKey        = NULL;
        HRFAPPLICATIONCONTEXT           hRFModuleContext   		= NULL;
		t_TimerDataObject*   			poTimerData 			= NULL;
		DMBASE_TIMERID 					TimerID 				= NULL;
		t_DiamAPIReturnVal				oReturnValue			= DMBase_Retval_Success;
		

		
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

        if( NULL == pvhRFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvhRFModuleContext_i is Null Pointer , Leaving \n");
            return DMBase_Retval_Null_Pointer;
        }

        if( NULL == hPeer_i )
        {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hPeer_i is NULL pinter, Leaving\n");
             return DMBase_Retval_Null_Pointer;
        }

        if ( NULL == hACRMsg_i )
        {    
            // Nothing to do with the Empty Message :)
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Empty Message Received, Leaving \n");
			return DMBase_Retval_Null_Pointer;
        }

        
		hRFModuleContext = (HRFAPPLICATIONCONTEXT)pvhRFModuleContext_i;

		hRFModuleContext->unReceiveCallBackCounter++;
		//printf("\n\n Message Received Counter  = %d \n\n\n",hRFModuleContext->unReceiveCallBackCounter);

		// TODO: Have to dump complete message here according to the display mode set 
       
		t_MessageStructure	  oMessageStructure;
		if(DMBase_Retval_Success != DMBase_MemsetZero( &oMessageStructure, sizeof(t_MessageStructure) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Message Structure with Zeros ,Leaving \n");
			return DMBase_Retval_Failure;
		}

		if( Offline_CS_RetVal_Success != Rf_FillMessageStructure( hRFModuleContext, hACRMsg_i,&oMessageStructure) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Filling Message Structure Fails,Leaving \n");
			//printf("\n\nRf_FillMessageStructure() \n\n");
			return DMBase_Retval_Failure;
			
		}

        
	/*	if (Offline_CS_RetVal_Success != Rf_ValidateACRMessage( hRFModuleContext, hACRMsg_i, hPeer_i) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Message Validation Fails ,Leaving \n");
			return DMBase_Retval_Failure;
		}
	*/
		

		if( DMBASE_START_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType )
		{
			hRFModuleContext->unStartMsgCounter++;
        }
		else if (DMBASE_STOP_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType )
		{
			hRFModuleContext->unStopMsgCounter++;
		}
		else
		{
			hRFModuleContext->unInvalidMsgCounter++;
		}
		//printf("\n\n Start Message Received  = %d \n\n ",hRFModuleContext->unStartMsgCounter);
		//printf("\n\n Stop Message Received  = %d \n\n ",hRFModuleContext->unStopMsgCounter);
		//printf("\n\n Invalid Message Received  = %d \n\n ",hRFModuleContext->unInvalidMsgCounter);

        t_DiamHashTableKey	oDiamHashTableKey ;
		int nSize = strlen(oMessageStructure.oMsgAVPsStruct.ucSessionId);
		memcpy(oDiamHashTableKey.puchKey,oMessageStructure.oMsgAVPsStruct.ucSessionId,nSize);
		oDiamHashTableKey.unKeyLength = nSize;
		t_SessionObject*		poSessionObject = NULL;
		if( Offline_CS_RetVal_Success != Rf_SearchObjectInSessionTable( hRFModuleContext,&oDiamHashTableKey, &poSessionObject))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Searching Object in Session Table Fails, leaving\n");
			return DMBase_Retval_Failure;
		}

		if( ((NULL == poSessionObject)&&(DMBASE_START_RECORD != oMessageStructure.oMsgAVPsStruct.nAcctRecordType)) )
		{
			/*if( DMBase_Retval_Success != DMBase_GeneratePermanentFailure( hRFModuleContext->hDiameterStackContext,
																		  hPeer_i,hACRMsg_i,DIAMETER_UNKNOWN_SESSION_ID,
																		  NULL ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Generating-Permanent Failure message for Unknown-Session-Id ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}*/
            hRFModuleContext->unStopDiscarded++;
			//printf("\n\n Stop  Messages Discarded Counter  = %d \n\n",hRFModuleContext->unStopDiscarded);
			

			hRFModuleContext->unDiscardedMessageCounter++;
			//printf("\n\n Discarded Messages Counter  = %d \n\n",hRFModuleContext->unDiscardedMessageCounter);
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Invalid Message Received , Leaving \n");
			return DMBase_Retval_Failure;
		}
        
        if( NULL != poSessionObject )
		{
			if( DMBase_Retval_Success != Rf_SearchACRInfoInSessionObject(hRFModuleContext,poSessionObject,
																		 oMessageStructure.oMsgAVPsStruct.unAcctRecordNumber,
																		 &poACRInfo,&bRecordFound) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Searching ACRInfo in sessionObject , Leaving \n");
				return DMBase_Retval_Failure;
			}
			if( NULL != poACRInfo )
			{
				 // this is case of receiving of duplicate ACR message , discarding it
				if(DMBASE_START_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType)
				{
					hRFModuleContext->unStartDiscarded++;
					//printf("\n\n Start  Messages Discarded Counter  = %d \n\n",hRFModuleContext->unStartDiscarded);
				}
				else if(DMBASE_STOP_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType)
				{
					hRFModuleContext->unStopDiscarded++;
					//printf("\n\n Stop  Messages Discarded Counter  = %d \n\n",hRFModuleContext->unStopDiscarded);
				}
				 hRFModuleContext->unDiscardedMessageCounter++;
				// printf("\n\n Discarded Messages Counter  = %d \n\n",hRFModuleContext->unDiscardedMessageCounter);
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACR Alleady exsists in Session Table , Leaving \n");
				 return DMBase_Retval_Failure;
            }
		}

		

        // If the ACR is Interim Message , no need to store in Session table ,
		// generate ACA after updating the Lastreceived message portion of session object.
		if( ( (DMBASE_START_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType)&&(NULL == poSessionObject)) )
		{
				
                //Create Accounting Record Info Object
				poACRInfo = NULL;
				if( Offline_CS_RetVal_Success != Rf_CreateACRInfo( hRFModuleContext ,oMessageStructure, hPeer_i,&poACRInfo) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in creating ACRInfo  Fails , Leaving \n");
					return DMBase_Retval_Failure;
				}
	
				// Add ACRInfo to new Session in table
				 if( Offline_CS_RetVal_Success != Rf_AddACRInfoToNewSession( hRFModuleContext, &oDiamHashTableKey,
																			 oMessageStructure.oMsgAVPsStruct.ucSessionId, 
																			 hPeer_i,&poSessionObject,
																			 oMessageStructure.oMsgAVPsStruct.unAcctRecordNumber,
																			 (void *)poACRInfo ))
				 {
					 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding ACRInfo to new Session , Leaving\n");
					 return DMBase_Retval_Failure;
				 }

				 hRFModuleContext->unSessionCreateCounter++;
				 //printf("\n\n New Session Created  = %d \n\n ",hRFModuleContext->unSessionCreateCounter);

				 // new code start 

				if( DMBase_Retval_Success != DMBase_Malloc(hRFModuleContext->hDiameterStackContext, (void*)&poTimerData, sizeof(t_TimerDataObject)) )
				{
					 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Allocating memory to Timer object, Leaving\n");
					 return DMBase_Retval_Failure;
				}
				strncpy(poTimerData->pucSessionId,oMessageStructure.oMsgAVPsStruct.ucSessionId,OFFLINE_CS_MEDIUM_STRING_SIZE);
				poTimerData->unAcctRecordNumber   = oMessageStructure.oMsgAVPsStruct.unAcctRecordNumber;
				poTimerData->hRfModuleContext	  = hRFModuleContext;

				if( DMBase_Retval_Success!= DMBase_StartTimer( hRFModuleContext->hDiameterStackContext,
															   OFFLINE_CS_EXPIRY_TIME_IN_MILLI_SECONDS,
															   Rf_InterimIntervalExpiryNotifyHandler, 
															   (void*)poTimerData,
															   &TimerID))
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "RF_SetTimerIDInToSessionObject() Fails, Leaving\n");
					return DMBase_Retval_Failure;
				}

                 
				 if( Offline_CS_RetVal_Success != Rf_SetTimerIDInSessionObject( hRFModuleContext,poSessionObject,
																				 TimerID,poTimerData ))
				 {
					  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in setting Timer Id in session Object, Leaving\n");
					  return DMBase_Retval_Failure;
				 }

				  // Send Event to CGF Module to store cdr contents in database and cdr file 
				 if( DMBase_Retval_Success !=DMBASE_EvtSysSendExtEvt( hRFModuleContext->hDiameterStackContext, 
					    											  OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_REQUEST_TO_CGF, 
																	  (void*)&(oMessageStructure.oMsgAVPsStruct), 
																	  sizeof(t_MessageAVPsStructure) ))
				 {
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Sending Event to Data Base Module,Leaving\n");
					return DMBase_Retval_Failure;
                 }
				 hRFModuleContext->unSendEventToCGFCounter++;
				// printf("\n\n Number of Events send to CGF Module  = %d \n",hRFModuleContext->unSendEventToCGFCounter);

				  // new code end
			
		}//new if check 
		else if (((DMBASE_INTERIM_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType)&&(NULL != poSessionObject)))
		{

				if (NULL != poSessionObject->TimerID) 
				{
					if( DMBase_Retval_Success!= DMBase_KillTimer( hRFModuleContext->hDiameterStackContext,
																  poSessionObject->TimerID ))
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_KillTimer() Fails, leaving\n");
						return Offline_CS_RetVal_Failure;
					}
				}
	
				if( DMBase_Retval_Success != DMBase_Malloc(hRFModuleContext->hDiameterStackContext, (void*)&poTimerData, sizeof(t_TimerDataObject)) )
				{
					 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Allocating memory to Timer object, Leaving\n");
					 return DMBase_Retval_Failure;
				}
				strncpy(poTimerData->pucSessionId,oMessageStructure.oMsgAVPsStruct.ucSessionId,OFFLINE_CS_MEDIUM_STRING_SIZE);
				poTimerData->unAcctRecordNumber   = oMessageStructure.oMsgAVPsStruct.unAcctRecordNumber;
				poTimerData->hRfModuleContext	  = hRFModuleContext;
	
				//start timer
				if( DMBase_Retval_Success!= DMBase_StartTimer( hRFModuleContext->hDiameterStackContext,
															   OFFLINE_CS_EXPIRY_TIME_IN_MILLI_SECONDS,
															   Rf_InterimIntervalExpiryNotifyHandler, 
															   (void*)poTimerData,
															   &TimerID))
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "RF_SetTimerIDInToSessionObject() Fails, Leaving\n");
					return DMBase_Retval_Failure;
				}
	
				if (NULL == TimerID) 
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Timer Initialization Fails , Leaving \n");
					return DMBase_Retval_Failure;
				}
	
	
				if( Offline_CS_RetVal_Success != Rf_SetTimerIDInSessionObject( hRFModuleContext,poSessionObject,
																			   TimerID,poTimerData ))
				{
					 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in setting Timer Id in session Object, Leaving\n");
					 return DMBase_Retval_Failure;
				}
	
				/*if( Offline_CS_RetVal_Success != Rf_GenerateInterimACAMsg( hRFModuleContext, hPeer_i,oMessageStructure ))
				{
					 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Logging CDR, Leaving\n");
					 return DMBase_Retval_Failure;
				}*/

		}
		else if( ( (DMBASE_STOP_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType)&&(NULL != poSessionObject) ) )
		{
				
                //Create Accounting Record Info Object
				poACRInfo = NULL;
				if( Offline_CS_RetVal_Success != Rf_CreateACRInfo( hRFModuleContext ,oMessageStructure, hPeer_i,&poACRInfo) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in creating ACRInfo  Fails , Leaving \n");
					return DMBase_Retval_Failure;
				}
	
				// Add ACRInfo to existing Session in Table
				 if( Offline_CS_RetVal_Success != Rf_AddACRInfoToExistingSession( hRFModuleContext,poSessionObject,(void *)poACRInfo ) )
				 {
					 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Adding ACRInfo to existing Session, Leaving\n");
					 return DMBase_Retval_Failure;
				 }
				 hRFModuleContext->unAddMsgToExistingSession++;
				// printf("\n\n Number of Messages Added to Existing Session  = %d \n\n ",hRFModuleContext->unAddMsgToExistingSession);

				 // new code start
				 //KILL the old timer here.
				if (NULL != poSessionObject->TimerID) 
				{
					if( DMBase_Retval_Success!= DMBase_KillTimer( hRFModuleContext->hDiameterStackContext,
																  poSessionObject->TimerID ))
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_KillTimer() Fails, leaving\n");
						return Offline_CS_RetVal_Failure;
					}
					poSessionObject->TimerID= NULL;
				}

				 
				if( NULL != poSessionObject->poTimerDataForCurrSess)
				{
					DMBase_Free( hRFModuleContext->hDiameterStackContext,
								 (void**)&(poSessionObject->poTimerDataForCurrSess) );
				}

				 
				 // Send Event to CGF Module to store cdr contents in database and cdr file 
				if( DMBase_Retval_Success !=DMBASE_EvtSysSendExtEvt( hRFModuleContext->hDiameterStackContext, 
																	 OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_REQUEST_TO_CGF, 
																	 (void*)&(oMessageStructure.oMsgAVPsStruct), 
																	 sizeof(t_MessageAVPsStructure) ))
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Sending Event to Data Base Module,Leaving\n");
					return DMBase_Retval_Failure;
                }
				hRFModuleContext->unSendEventToCGFCounter++;
				//printf("\n\n Number of Events send to CGF Module  = %d \n",hRFModuleContext->unSendEventToCGFCounter);

				// new code end

				 
		}
		else
		{
		   // do nothing

			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Do nothing ,Leaving \n");
			return DMBase_Retval_Failure;
        }
       
       
/*		if( DMBase_Retval_Success != DMBase_Malloc(hRFModuleContext->hDiameterStackContext, (void*)&poTimerData, sizeof(t_TimerDataObject)) )
		{
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Allocating memory to Timer object, Leaving\n");
			 return DMBase_Retval_Failure;
		}
		strncpy(poTimerData->pucSessionId,oMessageStructure.oMsgAVPsStruct.ucSessionId,OFFLINE_CS_MEDIUM_STRING_SIZE);
        poTimerData->unAcctRecordNumber   = oMessageStructure.oMsgAVPsStruct.unAcctRecordNumber;
		poTimerData->hRfModuleContext	  = hRFModuleContext;
		
       //set timer to notify about the Expiry of Interim Interval 
		

	
		//KILL the old timer here.
		if (NULL != poSessionObject->TimerID) 
		{
			if( DMBase_Retval_Success!= DMBase_KillTimer( hRFModuleContext->hDiameterStackContext,
														  poSessionObject->TimerID ))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_KillTimer() Fails, leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		//start timer
		if( DMBase_Retval_Success!= DMBase_StartTimer( hRFModuleContext->hDiameterStackContext,
													   OFFLINE_CS_EXPIRY_TIME_IN_MILLI_SECONDS,
													   Rf_InterimIntervalExpiryNotifyHandler, 
													   (void*)poTimerData,
													   &TimerID))
        {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "RF_SetTimerIDInToSessionObject() Fails, Leaving\n");
			return DMBase_Retval_Failure;
		}

         if (NULL == TimerID) 
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Timer Initialization Fails , Leaving \n");
             return DMBase_Retval_Failure;
         }

         
         if( Offline_CS_RetVal_Success != Rf_SetTimerIDInSessionObject( hRFModuleContext,poSessionObject,
																		TimerID,poTimerData ))
         {
             DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in setting Timer Id in session Object, Leaving\n");
             return DMBase_Retval_Failure;
         }
 
		 if( DMBASE_INTERIM_RECORD == oMessageStructure.oMsgAVPsStruct.nAcctRecordType )
		 {
			 if( Offline_CS_RetVal_Success != Rf_GenerateInterimACAMsg( hRFModuleContext, hPeer_i,oMessageStructure ))
			 {
				 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Logging CDR, Leaving\n");
				 return DMBase_Retval_Failure;
			 }
		 }
		 else
		 {
				// Send Event to CGF Module to store cdr contents in database and cdr file 
				if( DMBase_Retval_Success !=DMBASE_EvtSysSendExtEvt( hRFModuleContext->hDiameterStackContext, 
																	 OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_REQUEST_TO_CGF, 
																	 (void*)&(oMessageStructure.oMsgAVPsStruct), 
																	 sizeof(t_MessageAVPsStructure) ))
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Sending Event to Data Base Module,Leaving\n");
					return DMBase_Retval_Failure;
                }
				hRFModuleContext->unSendEventToCGFCounter++;
				printf("\n\n Number of Events send to CGF Module  = %d \n",hRFModuleContext->unSendEventToCGFCounter);

		 }

		 */
        
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
         return DMBase_Retval_Success;

 }
/******************************************************************************
** Function Name: 
**			Rf_GenerateInterimACAMsg
**  
** Description:		        
**
** Parameters:
**        
**        HRFAPPLICATIONCONTEXT    hRFModuleContext_i
**            type: Input
**            Detail:   This is a handle to context of Rf Module .
**
**		  t_MessageStructure		oMessageStructure_i
**			 type: input
**			 detail : This is of message structure .		  
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the return status of the Function.
** 
*******************************************************************************
** Created By: 	Simab Shahid	 
*******************************************************************************
*******************************************************************************
*/	
t_Offline_CS_ReturnVal  Rf_GenerateInterimACAMsg( HRFAPPLICATIONCONTEXT    		hRFModuleContext_i,
												  HPEERTABLEENTRY			 	hPeer_i,
												  t_MessageStructure		 	oMessageStructure_i )
{
		t_ACA_Params                    oACA_Param;
		HDIAMETERMESSAGE				hACAMsg		= NULL;
        unsigned char                   ucACRMsgFlags;
        unsigned char                   ucACRMsgVersion;      
        unsigned int                    unACRMsgHopByHopID;
        unsigned int                    unACRMsgEndToEndID;
        
       
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

		if( NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		if( NULL == hPeer_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hPeer_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		unsigned int    unDiaMsgCommandCode     = RF_MSG_CMD_CODE_ACR ;
		unsigned int    unDiaMsgApplicationID   = RF_APPLICATION_ID;
		unsigned char   ucDiaMsgVersion         = RF_MSG_VERSION;

		HAVPCOLLECTION		   hAVPCollection  = NULL;
		HMSGALLOCATIONCONTEXT hMsgAllocContext    = NULL;
        HDIAMETERSTACKCONTEXT hDiameterStackContext = hRFModuleContext_i->hDiameterStackContext;


		// Creating ACA message
         ucACRMsgFlags           = oMessageStructure_i.oMsgHrdFieldsStruct.ucFlags;
         unACRMsgHopByHopID      = oMessageStructure_i.oMsgHrdFieldsStruct.unHopByHopID;
         unACRMsgEndToEndID      = oMessageStructure_i.oMsgHrdFieldsStruct.unEndToEndID;




		 if( RfAPI_Retval_Success != DMBase_CreateDiameterMessage( hDiameterStackContext ,&hACAMsg ,
																   ucDiaMsgVersion , unDiaMsgCommandCode ,  
																   unDiaMsgApplicationID,ucACRMsgFlags,
																   unACRMsgHopByHopID, unACRMsgEndToEndID) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACA Message Creation fail , Leaving \n");
			return RfAPI_Retval_Failure ;
		}
	

		 // Clear the R-Flag in ACA Header Field
		_Bool bSetFlag = 0;
	   if( RfAPI_Retval_Success != DMBase_SetDiaMsgRFlag( hDiameterStackContext, hACAMsg, bSetFlag ) )
	   {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACA Message Header P-flag clearing fail , Leaving \n");
			goto ExitMe; 
	   }


	   if(DMBase_Retval_Success!=  DMBase_Get_AllocationContext_ForMessage( hDiameterStackContext,
																			hACAMsg,&hMsgAllocContext))
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_Get_AllocationContext_ForMessage() Fails , Leaving \n");
		   goto ExitMe; 
	   }


        if(DMBase_Retval_Success!= DMBase_MemsetZero(&oACA_Param, sizeof(t_ACR_Params)))// TODO: t_ACR_Params to t_ACA_Params
        {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_MemsetZero() Fails , Leaving \n");
			return Offline_CS_RetVal_Failure;
        }

          
                    
        
        // Create Result Code AVP
		if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hDiameterStackContext,
																   DMBASE_AVP_RESULT_CODE,
																   &( oACA_Param.hAVP_ResultCode ),
																   DIAMETER_SUCCESS,
																	hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Unsigned32 AVP ,leaving\n");
			goto ExitMe; 
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_ResultCode );
		
        //To Create Session ID AVP 
        if( DMBase_Retval_Success !=  DMBase_CreateUTF8StringAVP( hDiameterStackContext,
																  DMBASE_AVP_SESSION_ID,
																  &(oACA_Param.hAVP_SessionID),
																  (oMessageStructure_i.oMsgAVPsStruct.ucSessionId ),
																  strlen(oMessageStructure_i.oMsgAVPsStruct.ucSessionId),
																  hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating session-Id AVP ,leaving\n");
			goto ExitMe; 
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_SessionID );

		// create Origin_Host AVP
		if( DMBase_Retval_Success !=  DMBase_CreateUTF8StringAVP( hDiameterStackContext,
																  DMBASE_AVP_ORIGIN_HOST,
																  &(oACA_Param.hAVP_OriginHost),
																  hDiameterStackContext->oHostName.pucOctetString,
																  hDiameterStackContext->oHostName.unNumOfOctets,
																  hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Orgigin-Host AVP ,leaving\n");
			goto ExitMe; 
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_OriginHost );

		// create Origin-realm AVP
		if( DMBase_Retval_Success !=  DMBase_CreateUTF8StringAVP( hDiameterStackContext,
																  DMBASE_AVP_ORIGIN_REALM,
																  &(oACA_Param.hAVP_OriginRealm),
																  hDiameterStackContext->oRealmName.pucOctetString,
																  hDiameterStackContext->oRealmName.unNumOfOctets,
																  hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Origin-Realm AVP ,leaving\n");
			goto ExitMe; 
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_OriginRealm );

     
        // create Acct_Record_Type AVP 
		if( DMBase_Retval_Success !=   DMBase_CreateEnumAVP( hDiameterStackContext,
															 DMBASE_AVP_ACCOUNTING_RECORD_TYPE,
															 &(oACA_Param.hAVP_AccountingRecordType),
															 oMessageStructure_i.oMsgAVPsStruct.nAcctRecordType,
															 hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Acct-Record-Type AVP ,leaving\n");
			goto ExitMe; 
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_AccountingRecordType );


		// create Acct_Record_Number AVP
        if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hDiameterStackContext,
																   DMBASE_AVP_ACCOUNTING_RECORD_NUMBER,
																   &(oACA_Param.hAVP_AccountingRecordNumber),
																   oMessageStructure_i.oMsgAVPsStruct.unAcctRecordNumber,
																   hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Acct-Record-Number AVP ,leaving\n");
			goto ExitMe; 
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_AccountingRecordNumber );

        // create DMBASE_AVP_ACCT_INTERIM_INTERVAL  AVP 
		if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hDiameterStackContext,
																   DMBASE_AVP_ACCT_INTERIM_INTERVAL,
																   &(oACA_Param.hAVP_AcctInterimInterval),
																   OFFLINE_CS_ACCT_INTERIM_INTERVAL_VALUE_IN_SECONDS,
																   hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Unsigned32 AVP ,leaving\n");
			goto ExitMe; 
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_AcctInterimInterval );


		if( RfAPI_Retval_Success != DMBase_AddAVPCollectionToDiameterMessage( hDiameterStackContext,
																			  hACAMsg, hAVPCollection) )
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding AVP collection to ACA Message, Leaving \n");
		   goto ExitMe; 
	   }
	
         
		 if( DMBase_Retval_Success != DMBase_SendMessageToPeer( hDiameterStackContext, hPeer_i, hACAMsg) )
		 {
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occur While setting EndToEndId in ACA Message , Leaving \n");
             goto ExitMe;
		 }

		 ExitMe:
		  DMBase_DestroyDiaMessage(hDiameterStackContext, &hACAMsg);

         //Also get list of Proxy Info AVPs from ACR message and add it to the structure
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
         return Offline_CS_RetVal_Success ;
}

/******************************************************************************
** Function Name: 
**			Rf_CopyOctetStringToStringList
**  
** Description:
**		        
**
** Parameters:
**        
**        HDIAMETERMESSAGE				hACRMsg_i
**            type: Input
**            Detail:   This is a handle to message received by Rf Module .
**
**		  t_CDR_Contents 				poCDRContents_o
**			 type: output
**			 detail : This is pointer to object of cdr cdr contents structure, that will be filled 
**					  inside that function from the avp contents of received message.		  
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the return status of the Function.
** 
*******************************************************************************
** Created By: 	Simab Shahid	 
*******************************************************************************
*******************************************************************************
*/	
t_Offline_CS_ReturnVal inline Rf_CopyOctetStringToStringList( t_UTF8StringList* 	poOctetStringList_o,
															  unsigned int 			unDestinationStrSize_i,
															  unsigned char* 		pucDiamSourceString_i,
															  unsigned int 			unSourceStrSize_i )
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

	if( NULL == poOctetStringList_o )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poOctetStringList_o is Null Pointer , Leaving \n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	if( NULL == pucDiamSourceString_i )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucDiamSourceString_i is Null Pointer , Leaving \n");
		return Offline_CS_RetVal_Null_Pointer;
	}
	int nListIndex = 0;

	if( OFFLINE_CS_LIST_SIZE > poOctetStringList_o->nNumberOfItemsInList )
	{
		nListIndex = poOctetStringList_o->nNumberOfItemsInList;

		if( unSourceStrSize_i <= (unDestinationStrSize_i-1) )
		{
			memcpy( &(poOctetStringList_o->ucItems[nListIndex][0]),pucDiamSourceString_i,unSourceStrSize_i);
			poOctetStringList_o->ucItems[nListIndex][unDestinationStrSize_i] = '\0';
			poOctetStringList_o->nNumberOfItemsInList++;
		}
		else
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Destination String size is too small , Leaving \n");
			return Offline_CS_RetVal_Failure;
		}

	}
	else
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"No More Space in The List , Leaving \n");
		return Offline_CS_RetVal_Failure;
	}

	

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
	return Offline_CS_RetVal_Success ;

}

/******************************************************************************
** Function Name: 
**			Rf_CopyDiamStringToCharArray
**  
** Description:
**		        
**
** Parameters:
**        
**        HDIAMETERMESSAGE				hACRMsg_i
**            type: Input
**            Detail:   This is a handle to message received by Rf Module .
**
**		  t_CDR_Contents 				poCDRContents_o
**			 type: output
**			 detail : This is pointer to object of cdr cdr contents structure, that will be filled 
**					  inside that function from the avp contents of received message.		  
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the return status of the Function.
** 
*******************************************************************************
** Created By: 	Simab Shahid	 
*******************************************************************************
*******************************************************************************
*/	

t_Offline_CS_ReturnVal inline Rf_CopyDiamStringToCharArray( unsigned char* 	pucDestinationString_o,
															unsigned int 	unDestinationStrSize_i,
															unsigned char* 	pucDiamSourceString_i,
															unsigned int 	unSourceStrSize_i )
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

	if( NULL == pucDestinationString_o )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucDestinationString_o is Null Pointer , Leaving \n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	if( NULL == pucDiamSourceString_i )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucDiamSourceString_i is Null Pointer , Leaving \n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	if( unSourceStrSize_i <= (unDestinationStrSize_i-1) )
	{
		memcpy(pucDestinationString_o,pucDiamSourceString_i,unSourceStrSize_i);
		pucDestinationString_o[unSourceStrSize_i] = '\0';
    }
	else
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Destination String size is too small , Leaving \n");
		return Offline_CS_RetVal_Failure;
	}

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
	return Offline_CS_RetVal_Success ;

}
/******************************************************************************
** Function Name: 
**			Rf_FillMessageStructure
**  
** Description:
**		      Inside this function Rf Module is sending events to Cdr file writer module
**			  and and DB module for writing the cdr in File and DataBase, if the
**			  configuration setting is enabled for any or all of these operations.      
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT         hRFModuleContext_i
**			 type: input/output
**			 detail : This is handle to Rf module context.
**
**
**        HDIAMETERMESSAGE				hACRMsg_i
**            type: Input
**            Detail:   This is a handle to message received by Rf Module .
**
**		  t_CDR_Contents 				poCDRContents_o
**			 type: output
**			 detail : This is pointer to object of cdr cdr contents structure, that will be filled 
**					  inside that function from the avp contents of received message.		  
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the return status of the Function.
** 
*******************************************************************************
** Created By: 	Simab Shahid	 
*******************************************************************************
*******************************************************************************
*/  

t_Offline_CS_ReturnVal Rf_FillMessageStructure( HRFAPPLICATIONCONTEXT           hRFModuleContext_i,
												HDIAMETERMESSAGE				hACRMsg_i,
												t_MessageStructure* 			poMessageStructure_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");
	
		if( NULL == hRFModuleContext_i )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is Null Pointer , Leaving \n");
				return DMBase_Retval_Null_Pointer;
		}
	
		if( NULL == hACRMsg_i )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hACRMsg_i is Null Pointer , Leaving \n");
				return DMBase_Retval_Null_Pointer;
		}
	
		if( NULL == poMessageStructure_o )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poMessageStructure_o is Null Pointer , Leaving \n");
				return DMBase_Retval_Null_Pointer;
		}

		unsigned int		unDestinationStrSize		=  0;
        unsigned int        unAVPCode                   =  0;
        
        HAVP                hToAVP_ACR                  =  NULL;
        void*               pvCurrentNodeData           =  NULL;
        DMBase_SList*       pCurrentAVPOfACR            =  NULL;
        t_DiamInteger32*    pODiamInteger32             =  NULL;
        t_DiamUnsigned32*   pODiamUnsigned32            =  NULL;
        t_DiamOctetString*  pODiamOctetString           =  NULL;


		// First of all save the header fields of the message in message strucutre 
		poMessageStructure_o->oMsgHrdFieldsStruct.ucFlags = hACRMsg_i->oDiaHeader.ucFlags;
		poMessageStructure_o->oMsgHrdFieldsStruct.unEndToEndID = hACRMsg_i->oDiaHeader.unEndToEndID;
		poMessageStructure_o->oMsgHrdFieldsStruct.unHopByHopID = hACRMsg_i->oDiaHeader.unHopByHopID;

		unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
        pCurrentAVPOfACR  =  hACRMsg_i->hCollectionOfAVPs;
        while( NULL != pCurrentAVPOfACR)
        {
                pvCurrentNodeData = NULL;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                       pCurrentAVPOfACR,&pvCurrentNodeData) )
                {
                    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured While getting data From ACR AVP List Node, Leaving \n");
                    return Offline_CS_RetVal_Failure;
                }
				
                hToAVP_ACR      = (HAVP)pvCurrentNodeData;
                unAVPCode       =  hToAVP_ACR->oAVPHeaderInst.unAVPCode;
				
                switch(unAVPCode)
                {		DMBASE_TRACE(g_nDiamBaseTraceID, AMPS_TRACE_LEVEL_INFO,"Session-Id AVP Found\n");
                        case DMBASE_AVP_SESSION_ID:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Session-Id AVP Found\n");
								pODiamOctetString  = (t_DiamOctetString*)hToAVP_ACR->pData;
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageStructure_o->oMsgAVPsStruct.ucSessionId,
																							   unDestinationStrSize,
																							   pODiamOctetString->pucOctetString,
																							   pODiamOctetString->unNumOfOctets ))
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Session-Id avp data to Cdr Contents,Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;

                        case DMBASE_AVP_ORIGIN_HOST:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Origin-Host AVP Found \n");
								pODiamOctetString  = (t_DiamOctetString*)hToAVP_ACR->pData;
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageStructure_o->oMsgAVPsStruct.ucOriginHost,
																							   unDestinationStrSize,
																							   pODiamOctetString->pucOctetString,
																							   pODiamOctetString->unNumOfOctets ) )
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Origin-Host avp data to Cdr Contents,Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;

                        case DMBASE_AVP_ORIGIN_REALM:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"ORigin-Realm AVP Found\n");
								pODiamOctetString  = (t_DiamOctetString*)hToAVP_ACR->pData;
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageStructure_o->oMsgAVPsStruct.ucOriginRealm,
																							   unDestinationStrSize,
																							   pODiamOctetString->pucOctetString,
																							   pODiamOctetString->unNumOfOctets ))
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying ORigin-Realm avp data to Cdr Contents,Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;

                        case DMBASE_AVP_DESTINATION_REALM:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Destination-Realm AVP Found\n");
								pODiamOctetString  = (t_DiamOctetString*)hToAVP_ACR->pData;
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageStructure_o->oMsgAVPsStruct.ucDestinationRealm,
																							   unDestinationStrSize,
																							   pODiamOctetString->pucOctetString,
																							   pODiamOctetString->unNumOfOctets ))
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Destination-Realm avp data to Cdr Contents,Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;

                        case DMBASE_AVP_ACCOUNTING_RECORD_TYPE:
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Accounting-Record-Type AVP Found\n");
							pODiamInteger32 = (t_DiamInteger32*)hToAVP_ACR->pData;
							/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																							&(poMessageStructure_o->oMsgAVPsStruct.nAcctRecordType),
																							*(pODiamInteger32->pnInteger32) ))
							{
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
								return Offline_CS_RetVal_Failure;
							}*/
							poMessageStructure_o->oMsgAVPsStruct.nAcctRecordType = *(pODiamInteger32->pnInteger32);
                            break;

                        case DMBASE_AVP_ACCOUNTING_RECORD_NUMBER:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Accounting-Record-Number AVP Found\n");
                                pODiamUnsigned32 = (t_DiamUnsigned32*)hToAVP_ACR->pData;
								/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																								&(poMessageStructure_o->oMsgAVPsStruct.unAcctRecordNumber),
																								*(pODiamUnsigned32->punUnsigned32) ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
								poMessageStructure_o->oMsgAVPsStruct.unAcctRecordNumber = *(pODiamUnsigned32->punUnsigned32);
                                break;

                        case DMBASE_AVP_ACCT_APPLICATION_ID:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Acct-Application-Id AVP Found\n");
								pODiamUnsigned32 = (t_DiamUnsigned32*)hToAVP_ACR->pData;
                                poMessageStructure_o->oMsgAVPsStruct.unAcctApplicationId = *(pODiamUnsigned32->punUnsigned32);
                                break;

                        case DMBASE_AVP_USER_NAME:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"User-Name AVP Found\n");
                                pODiamOctetString  = (t_DiamOctetString*)hToAVP_ACR->pData;
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageStructure_o->oMsgAVPsStruct.ucUserName,
																							   unDestinationStrSize,
																							   pODiamOctetString->pucOctetString,
																							   pODiamOctetString->unNumOfOctets ))
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Session-Id avp data to Cdr Contents,Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;

                        case DMBASE_AVP_ACCT_INTERIM_INTERVAL:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Acct-Interim-Interval AVP Found\n");
							pODiamUnsigned32 = (t_DiamUnsigned32*)hToAVP_ACR->pData;
							/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																							&(poMessageStructure_o->oMsgAVPsStruct.unAcctInterimInterval),
																							*(pODiamUnsigned32->punUnsigned32) ))
							{
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
								return Offline_CS_RetVal_Failure;
							}*/
							poMessageStructure_o->oMsgAVPsStruct.unAcctInterimInterval = *(pODiamUnsigned32->punUnsigned32);
                            break;

                        case DMBASE_AVP_ORIGIN_STATE_ID:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Origin-State-Id AVP Found\n");
								pODiamUnsigned32 = (t_DiamUnsigned32*)hToAVP_ACR->pData;
                                poMessageStructure_o->oMsgAVPsStruct.unOriginStateId = *(pODiamUnsigned32->punUnsigned32);
                                break;

						case DMBASE_AVP_EVENT_TIMESTAMP:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Event-TimeStamp AVP Found\n");
								pODiamUnsigned32 = (t_DiamUnsigned32*)hToAVP_ACR->pData;
								/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																								&(poMessageStructure_o->oMsgAVPsStruct.unEventTimeStamp),
																								*(pODiamUnsigned32->punUnsigned32) ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
								poMessageStructure_o->oMsgAVPsStruct.unEventTimeStamp = *(pODiamUnsigned32->punUnsigned32);
                                break;

                        case OFFLINE_CS_AVP_SERVICE_CONTEXT_ID:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG_2,"Service-Context-Id AVP Found\n");
                                pODiamOctetString  = (t_DiamOctetString*)hToAVP_ACR->pData;
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageStructure_o->oMsgAVPsStruct.ucServiceContextId,
																							   unDestinationStrSize,
																							   pODiamOctetString->pucOctetString,
																							   pODiamOctetString->unNumOfOctets ) )
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Service-Context-Id avp data to Cdr Contents,Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;

                        case OFFLINE_CS_AVP_SERVICE_INFORMATION:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Service-Information AVP Found\n");
                                if (Offline_CS_RetVal_Success != Rf_AddServiceInfoAVPToCdrContents( hRFModuleContext_i,hToAVP_ACR,&(poMessageStructure_o->oMsgAVPsStruct) ) )
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured while adding Service-Information AVP Contents to Event Data, Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;

                                //TODO; Currently in this block of code the ACR message is being paresd for 
                                // all the AVPs(both defined in RFC 3588 and 3GPP) defined.
                                // Inorder to parse ACR Message  for newly Added AVPs Write Code here

                        default:
                                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Unknown AVP (%d) ACR Message \n",unAVPCode);

                }// End switch(unAVPCode)
                DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext, pCurrentAVPOfACR, &pCurrentAVPOfACR);

        }// End  while( NULL != pCurrentAVPOfACR)
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
		return Offline_CS_RetVal_Success ;

 }

t_Offline_CS_ReturnVal Rf_AddSubscriptionIdAVPToCdrContents(  HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
															  HAVP					  	hToAVP_ACR_i,
															  t_MessageAVPsStructure*	poMessageAVPsStructure_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

		if (NULL == hRFModuleContext_i )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}

        if (NULL == poMessageAVPsStructure_o )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," poMessageAVPsStructure_o is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Invalid_Pointer;
		}

		if (NULL == hToAVP_ACR_i)
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," hToAVP_i is NULL pointer , leaving\n");
				return Offline_CS_RetVal_Null_Pointer;
		}
     
       
       
	   unsigned int  	     unAVPCode			 =  0;
	   unsigned int			 unInteger           =  0;
       HAVP                  hToAVP              = NULL;
	   void*                 pvNodeData          = NULL;
       t_DiamEnum*		     poTempEnum     	 = NULL;
	   t_DiamOctetString*	 pODiamOctetString   = NULL;
       DMBase_SList*	     pCurrentNode        = hToAVP_ACR_i->hCollectionOfAVPs;

	   unsigned int unDestinationStrSize = OFFLINE_CS_MEDIUM_STRING_SIZE;
      while( NULL != pCurrentNode )
      {
                pvNodeData  = NULL;
				unInteger   = 0;
                if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pvNodeData ) )
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }

                hToAVP      = (HAVP)pvNodeData;
                unAVPCode   = hToAVP->oAVPHeaderInst.unAVPCode;
                switch( unAVPCode )
                {
						case OFFLINE_CS_SUBSCRIPTION_ID_DATA:
								pODiamOctetString  = (t_DiamOctetString*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Subscription-Id-Data AVP found\n");
                                if( Offline_CS_RetVal_Success != Rf_CopyDiamStringToCharArray( poMessageAVPsStructure_o->oServiceInfo.oSubscriptionId.ucSubscriptionIdData,
																							   unDestinationStrSize,
																							   pODiamOctetString->pucOctetString,
																							   pODiamOctetString->unNumOfOctets ))
                                {
                                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in copying SubscriptionId-Data avp data to Cdr Contents,Leaving \n");
                                        return Offline_CS_RetVal_Failure;
                                }
                                break;
                         
						case OFFLINE_CS_SUBSCRIPTION_ID_TYPE:		
								poTempEnum = (t_DiamEnum*)hToAVP->pData;
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Subscription-Id-Type AVP found\n");
								/*if( Offline_CS_RetVal_Success != Rf_CopyIntToIntInNetByteOrder( hRFModuleContext_i,
																								&unInteger,
																								*(poTempEnum->pnInteger32) ))
								{
									DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in copying Accounting-Record-Type avp data to Cdr Contents,Leaving \n");
									return Offline_CS_RetVal_Failure;
								}*/
								poMessageAVPsStructure_o->oServiceInfo.oSubscriptionId.nSubscriptionIdType = *(poTempEnum->pnInteger32);
                                break;
						
                        default:
								DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Unknown AVP Received in Subscription-Id AVP, leaving\n");

                }//End switch( unAVPCode )
				DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,
										 pCurrentNode,&pCurrentNode);
       }// end while( NULL != pCurrentNode )

      DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
      return Offline_CS_RetVal_Success;
}
t_Offline_CS_ReturnVal Rf_AddServiceInfoAVPToCdrContents(  HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
														   HAVP					  	hToAVP_ACR_i,
														   t_MessageAVPsStructure*	poMessageAVPsStructure_o)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

	if( NULL == hRFModuleContext_i )
	{
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is Null Pointer , Leaving \n");
            return DMBase_Retval_Null_Pointer;
	}

	if( NULL == hToAVP_ACR_i )
	{
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hToAVP_ACR_i is Null Pointer , Leaving \n");
            return DMBase_Retval_Null_Pointer;
	}

	if( NULL == poMessageAVPsStructure_o )
	{
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poMessageAVPsStructure_o is Null Pointer , Leaving \n");
            return DMBase_Retval_Null_Pointer;
	}

	
	   unsigned int  	unAVPCode	 = 0;
	   HAVP            	hToTempAVP   = NULL;
       void*           	pvNodeData   = NULL;
       DMBase_SList*	pCurrentNode = hToAVP_ACR_i->hCollectionOfAVPs;


      while ( NULL != pCurrentNode )
      {

            pvNodeData = NULL;
            if ( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                   pCurrentNode, &pvNodeData ) )
			{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_SListGetNodeData() fails , Leaving \n");
					return Offline_CS_RetVal_Failure;
			}
            hToTempAVP = (HAVP)pvNodeData;
            unAVPCode  =  hToTempAVP->oAVPHeaderInst.unAVPCode;
         
            switch(unAVPCode)
            {
                    case OFFLINE_CS_SUBSCRIPTION_ID:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Subscription-Id Grouped AVP found\n");
							if(Offline_CS_RetVal_Success != Rf_AddSubscriptionIdAVPToCdrContents(hRFModuleContext_i, hToTempAVP,poMessageAVPsStructure_o))
							{	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Subscription-Id AVP data To cdr Contents,Leaving \n");
								return Offline_CS_RetVal_Failure;
							}
                            break;
					case OFFLINE_CS_PS_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"PS-Information Grouped AVP found\n");
							if(Offline_CS_RetVal_Success != Rf_AddPSInfoAVPToCdrContentsStruct(hRFModuleContext_i, hToTempAVP,&(poMessageAVPsStructure_o->oServiceInfo.oPSInformation)))
							{	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in PS-Information AVP Data to cdr Contents, Leaving\n");
								return Offline_CS_RetVal_Failure;
							}
                            break;
					case OFFLINE_CS_WLAN_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"WLAN-Information Grouped AVP found\n");
                            break;
                    case OFFLINE_CS_IMS_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"IMS-Information Grouped AVP found\n");
							if(Offline_CS_RetVal_Success != Rf_AddIMSInfoAVPToCdrContntsStruct(hRFModuleContext_i,
																								hToTempAVP,
																								&(poMessageAVPsStructure_o->oServiceInfo.oIMSInformation)))
							{	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding IMS-Information AVP Data to cdr Contents, Leaving\n");
								return Offline_CS_RetVal_Failure;
							}
                            break;
					case OFFLINE_CS_MMS_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"MMS-Information Grouped AVP found\n");
                            break;
					case OFFLINE_CS_LCS_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"LCS-Information Grouped AVP found\n");
                            break;
					case OFFLINE_CS_POC_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"PoC-Information Grouped AVP found\n");
                            break;
                    case OFFLINE_CS_MBMS_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"MBMS-Information Grouped AVP found \n");
                            break;
					case OFFLINE_CS_SMS_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"SMS-Information Grouped AVP found\n");
                            break;
					case OFFLINE_CS_SERVICE_GENERIC_INFORMATION:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Service-Generic-Information Grouped AVP found\n");
                            break;

                    default:
                            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Unknown-AVP found inside Service-Information Grouped AVP\n");

             }//switch(unAVPCode)
                 
             DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,pCurrentNode,&pCurrentNode);

        }//while( NULL != pCurrentAVP)

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
	return Offline_CS_RetVal_Success ;
}

/******************************************************************************
** Function Name: 
**			Rf_ValidateACRMessage
**  
** Description:
**		This function is Validating received ACR Message.        
**
** Parameters:
**        HRFAPPLICATIONCONTEXT         hRFApplicationContext
**			 type: input
**			 detail : This is a handle to RF Module context handle. 
**
**		  HDIAMETERMESSAGE       	     hACRMsg_i
**			 type: input
**			 detail : This is a handle to Received Message. 
**
**		  HPEERTABLEENTRY				 hPeer_i
**			 type: input
**			 detail : This is a handle to Peer entry. 
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  wether the ACR Message 
**      has been validated Successfully or the received Message is 
**      Invalide ACR.
**
** 
*******************************************************************************
** Created By: Seemab Shahid	 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_ValidateACRMessage( HRFAPPLICATIONCONTEXT          hRFModuleContext_i,
                                              HDIAMETERMESSAGE       	     hACRMsg_i,
											  HPEERTABLEENTRY				 hPeer_i)
{
	// Important : Currently this function is only validating the mandatory AVPs
	// defined by base protocol validation of application specific AVPs is future task.

    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

	if(NULL == hRFModuleContext_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer , leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }

	if(NULL == hACRMsg_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hACRMsg_i is NULL pointer , leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }

	if(NULL == hPeer_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hPeer_i is NULL pointer , leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }

	unsigned int unCommandCode = 0;
    if(DMBase_Retval_Success !=  DMBase_GetDiaMsgCmdCodeFromDiamMsg( hRFModuleContext_i->hDiameterStackContext, 
																	 hACRMsg_i ,&unCommandCode ))
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in getting Command code from message,Leaving \n");
		return Offline_CS_RetVal_Failure ;

	}
    
    t_DiamAPIReturnVal oRetvalue = DMBase_Retval_Success;


	if ( DMBASE_MSG_CMD_CODE_ACR != unCommandCode )
	{
		DMBase_GenerateProtocolErr(hRFModuleContext_i->hDiameterStackContext,
								   hPeer_i,hACRMsg_i,DIAMETER_COMMAND_UNSUPPORTED,NULL);
		return Offline_CS_RetVal_Command_Not_found; 
	}

	// Now have to validate the message for AVPs 
    if( Offline_CS_RetVal_Success != Rf_ValidateACRMsgAVPs(hRFModuleContext_i, hACRMsg_i, hPeer_i) )
	{
		
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Leaving \n");
		return Offline_CS_RetVal_Failure ;
	}


	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
    return Offline_CS_RetVal_Success ; 
}
/******************************************************************************
** Function Name: 
**			Rf_ValidateACRMsgAVPs
**  
** Description:
**		This function is Validating the avps of received ACR Message.        
**
** Parameters:
**        HRFAPPLICATIONCONTEXT         hRFApplicationContext
**			 type: input
**			 detail : This is a handle to RF Module context handle. 
**
**		  HDIAMETERMESSAGE       	     hACRMsg_i
**			 type: input
**			 detail : This is a handle to Received Message. 
**
**		  HPEERTABLEENTRY				 hPeer_i
**			 type: input
**			 detail : This is a handle to Peer entry. 
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the success or failur 
**		of Massage of AVP validation. 
*******************************************************************************
** Created By: Seemab Shahid	 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_ValidateACRMsgAVPs(HRFAPPLICATIONCONTEXT			  	hRFModuleContext_i,
											 HDIAMETERMESSAGE                	hACRMsg_i,
											 HPEERTABLEENTRY				  	hPeer_i)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");
	
		if( NULL == hRFModuleContext_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer ,leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		if( NULL == hACRMsg_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hACRMsg_i is NULL pointer ,leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		if( NULL == hPeer_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hPeer_i is NULL pointer ,leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
	
		HAVP 					hAVP_Validation 	= NULL;
		DMBase_SList*   		poFailedAVPsList 	= NULL;
		// t_ObjectOfAVPDetails to t_AVPDetails
		//OOrigin_Host_Details to oOrigin_Host_Details
		t_ObjectOfAVPDetails	OSession_Id_Details;
		t_ObjectOfAVPDetails	OOrigin_Host_Details;
		t_ObjectOfAVPDetails	OOrigin_Realm_Details;
		t_ObjectOfAVPDetails	ODestination_Realm_Details;
		t_ObjectOfAVPDetails	OAcct_Record_Type_Details;
		t_ObjectOfAVPDetails	OAcct_Record_Number_Details;
		
	
        if( DMBase_Retval_Success !=  DMBase_MemsetZero(&OSession_Id_Details,sizeof(t_ObjectOfAVPDetails) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in memory setting to Zero ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		if( Offline_CS_RetVal_Success != Rf_GetAVPDetailsforValidation( hRFModuleContext_i, 
																		 hACRMsg_i,DMBASE_AVP_SESSION_ID,
																		 &OSession_Id_Details) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in getting details of session-Id AVP from Message ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}
	
		if( 0 == OSession_Id_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddExpectedAVPToList( hRFModuleContext_i, 
																	  DMBASE_AVP_SESSION_ID,
																	  &poFailedAVPsList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding session-Id AVP to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
	
	

		if( DMBase_Retval_Success !=  DMBase_MemsetZero(&OOrigin_Host_Details,sizeof(t_ObjectOfAVPDetails) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in memory setting to Zero ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		if( Offline_CS_RetVal_Success != Rf_GetAVPDetailsforValidation( hRFModuleContext_i, 
																			 hACRMsg_i,DMBASE_AVP_ORIGIN_HOST,
																			 &OOrigin_Host_Details) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in getting details of Origin-Host AVP from Message ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		if( 0 == OOrigin_Host_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddExpectedAVPToList( hRFModuleContext_i, 
																	  DMBASE_AVP_ORIGIN_HOST,
																	  &poFailedAVPsList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Origin-Host AVP to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			
		}
	



		if( DMBase_Retval_Success !=  DMBase_MemsetZero(&OOrigin_Realm_Details,sizeof(t_ObjectOfAVPDetails) ))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in memory setting to Zero ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		if( Offline_CS_RetVal_Success != Rf_GetAVPDetailsforValidation( hRFModuleContext_i, 
																		hACRMsg_i,DMBASE_AVP_ORIGIN_REALM,
																		&OOrigin_Realm_Details) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in getting details of Origin-Realm AVP from Message ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
        if( 0 == OOrigin_Realm_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddExpectedAVPToList( hRFModuleContext_i, 
																	  DMBASE_AVP_ORIGIN_REALM,
																	  &poFailedAVPsList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Origin-Realm AVP to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
        }
	

		if( DMBase_Retval_Success !=  DMBase_MemsetZero(&ODestination_Realm_Details,sizeof(t_ObjectOfAVPDetails) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in memory setting to Zero ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		if( Offline_CS_RetVal_Success != Rf_GetAVPDetailsforValidation( hRFModuleContext_i, 
																		hACRMsg_i,DMBASE_AVP_DESTINATION_REALM,
																		&ODestination_Realm_Details) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in getting details of Destination-Realm AVP from Message ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
        if( 0 == ODestination_Realm_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddExpectedAVPToList( hRFModuleContext_i, 
																		   DMBASE_AVP_DESTINATION_REALM,
																		   &poFailedAVPsList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Destination-Realm AVP to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}



		if( DMBase_Retval_Success !=  DMBase_MemsetZero(&OAcct_Record_Type_Details,sizeof(t_ObjectOfAVPDetails) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in memory setting to Zero ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		if( Offline_CS_RetVal_Success != Rf_GetAVPDetailsforValidation( hRFModuleContext_i, 
																			 hACRMsg_i,DMBASE_AVP_ACCOUNTING_RECORD_TYPE,
																			 &OAcct_Record_Type_Details) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in getting details of Acct-Record-Type AVP from Message ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
        if( 0 == OAcct_Record_Type_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddExpectedAVPToList( hRFModuleContext_i, 
																		   DMBASE_AVP_ACCOUNTING_RECORD_TYPE,
																		   &poFailedAVPsList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Acct-Record-Type AVP to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
	

		if( DMBase_Retval_Success !=  DMBase_MemsetZero(&OAcct_Record_Number_Details,sizeof(t_ObjectOfAVPDetails) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in memory setting to Zero ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		if( Offline_CS_RetVal_Success != Rf_GetAVPDetailsforValidation( hRFModuleContext_i, 
																			 hACRMsg_i,DMBASE_AVP_ACCOUNTING_RECORD_NUMBER,
																			 &OAcct_Record_Number_Details) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in getting details of Acct-Record-Number AVP from Message ,leaving\n");
			return Offline_CS_RetVal_Failure;
		}

        if( 0 == OAcct_Record_Number_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddExpectedAVPToList( hRFModuleContext_i, 
																		   DMBASE_AVP_ACCOUNTING_RECORD_NUMBER,
																		   &poFailedAVPsList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Acct-Record-Type AVP to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}



		if( NULL != poFailedAVPsList)
		{
            if( DMBase_Retval_Success != DMBase_GeneratePermanentFailure( hRFModuleContext_i->hDiameterStackContext,
																		  hPeer_i,hACRMsg_i,DIAMETER_MISSING_AVP,
																		  poFailedAVPsList ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Generating-Permanent Failure message for missing AVP ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            return  Offline_CS_RetVal_AVP_Missing;
		}

		// control will reach to this point when all the mandatory AVPs will present in the message 
        // Now starting checking for their mandatory AVPs for multiple occurences.
		
		if( 1 < OSession_Id_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddAVPToFailedAVPList( hRFModuleContext_i,
																	   &poFailedAVPsList,
																	   DMBASE_AVP_SESSION_ID,
																	   OSession_Id_Details.hAVP_Validation) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding first occurrence Session-Id avp to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( 1 < OOrigin_Host_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddAVPToFailedAVPList( hRFModuleContext_i,
																	   &poFailedAVPsList,
																	   DMBASE_AVP_ORIGIN_HOST,
																	   OOrigin_Host_Details.hAVP_Validation) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding first occurrence Origin-Host avp to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( 1 < OOrigin_Realm_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddAVPToFailedAVPList( hRFModuleContext_i,
																	   &poFailedAVPsList,
																	   DMBASE_AVP_ORIGIN_REALM,
																	   OOrigin_Realm_Details.hAVP_Validation) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding first occurrence Origin-Realm avp to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( 1 < ODestination_Realm_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddAVPToFailedAVPList( hRFModuleContext_i,
																	   &poFailedAVPsList,
																	   DMBASE_AVP_DESTINATION_REALM,
																	   ODestination_Realm_Details.hAVP_Validation) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding first occurrence Destination-Realm avp to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		if( 1 < OAcct_Record_Type_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddAVPToFailedAVPList( hRFModuleContext_i,
																	   &poFailedAVPsList,
																	   DMBASE_AVP_ACCOUNTING_RECORD_TYPE,
																	   OAcct_Record_Type_Details.hAVP_Validation ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding first occurrence Acct-Record-Type avp to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		if( 1 < OAcct_Record_Number_Details.unAVPCount )
		{
			if( Offline_CS_RetVal_Success != Rf_AddAVPToFailedAVPList( hRFModuleContext_i,
																	   &poFailedAVPsList,
																	   DMBASE_AVP_ACCOUNTING_RECORD_NUMBER,
																	   OAcct_Record_Number_Details.hAVP_Validation ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding first occurrence Acct-Record-Number avp to failed avp list ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( NULL != poFailedAVPsList)
		{
			if( DMBase_Retval_Success != DMBase_GeneratePermanentFailure( hRFModuleContext_i->hDiameterStackContext,
																		  hPeer_i,hACRMsg_i,DIAMETER_AVP_OCCURS_TOO_MANY_TIMES,
																		  poFailedAVPsList ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Generating-Permanent Failure message for AVP Occurres too many times ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			
			return Offline_CS_RetVal_AVP_Occurs_Too_Many_Times ;
		}


		// the control will reach to this point when all the mandatory avps will be present in message 
		// and its number of occurrences will be exactly according to the specification
		// so here the data  of that mandatory avps will check

		// here currently only the data of Acct-Record-Number AVP can only be checked 

		t_DiamEnum*    pOAcctRecordType = (t_DiamEnum*)(OAcct_Record_Type_Details.hAVP_Validation->pData);
		int nRecordType = *(pOAcctRecordType->pnInteger32);
						
		// check for the Acct Record Type First as here only
		// session based record is handling for time being
		if( DMBASE_EVENT_RECORD == nRecordType )
		{
			// Not handling this case 
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Accounting Message With Event Record Type is received , Leaving \n");
			return Offline_CS_RetVal_Event_Acct_Record_Type;
		}
		else if ( ( DMBASE_START_RECORD != nRecordType) && (DMBASE_INTERIM_RECORD != nRecordType) && (DMBASE_STOP_RECORD != nRecordType) ) 
		{
			if( Offline_CS_RetVal_Success != Rf_AddAVPToFailedAVPList( hRFModuleContext_i,
																	   &poFailedAVPsList,
																	   DMBASE_AVP_ACCOUNTING_RECORD_TYPE,
																	   OAcct_Record_Type_Details.hAVP_Validation ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in adding Acct-Record-Type avp to failed avp list for its invalid data ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}
       	}


		if( NULL != poFailedAVPsList )
		{
			if( DMBase_Retval_Success != DMBase_GeneratePermanentFailure( hRFModuleContext_i->hDiameterStackContext,
																		  hPeer_i,hACRMsg_i,DIAMETER_INVALID_AVP_VALUE,
																		  poFailedAVPsList ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Generating-Permanent Failure message for AVP Having invalid value ,leaving\n");
				return Offline_CS_RetVal_Failure;
			}

			return Offline_CS_RetVal_Invalid_AVP_Value ;
		}



		/// check if the Acct-Record is other then start-Record then the session for that ACR 
		/// must  exist in session table if not Generate UNKNOWN_SESSION_ID Error.
		if( DMBASE_START_RECORD != nRecordType )
		{

                t_DiamOctetString*				poSessionID = NULL;
                poSessionID = (t_DiamOctetString*)OSession_Id_Details.hAVP_Validation->pData;

				t_DiamHashTableKey	oDiamHashTableKey ;
                memcpy(oDiamHashTableKey.puchKey,poSessionID->pucOctetString,poSessionID->unNumOfOctets);
				oDiamHashTableKey.unKeyLength = poSessionID->unNumOfOctets;
				// to check that the Session allready exists
				t_SessionObject* poSessionObject = NULL;
				if( Offline_CS_RetVal_Success != Rf_SearchObjectInSessionTable( hRFModuleContext_i,
																				&oDiamHashTableKey,
																				&poSessionObject) )
				{
					 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Rf_SearchObjectInSessionTable() Fails , Leaving \n");
					 return DMBase_Retval_Failure;
				}


				// if Session does not exists  
				if( NULL == poSessionObject )
				{
					if( DMBase_Retval_Success != DMBase_GeneratePermanentFailure( hRFModuleContext_i->hDiameterStackContext,
																				  hPeer_i,hACRMsg_i,DIAMETER_UNKNOWN_SESSION_ID,
																				  poFailedAVPsList ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Generating-Permanent Failure message for Unknown-Session-Id ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
			
					return Offline_CS_RetVal_Invalid_AVP_Value ;
				}
		}
		

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
		return Offline_CS_RetVal_Success ;

 }
/******************************************************************************
** Function Name: 
**			Rf_AddAVPToFailedAVPList
**  
** Description:
**		This function is is just adding a failed avp(during message avp validation)
**		to failed avp list.        
**
** Parameters:
**        HRFAPPLICATIONCONTEXT         hRFApplicationContext
**			 type: input
**			 detail : This is a handle to RF Module context handle. 
**
**		  DMBase_SList**     	ppoFailedAVPsList_i
**			 type: input
**			 detail : This is a pointer to pointer of Failed avp list. 
**
**		  unsigned int 			unAVPCode_i
**			 type: input
**			 detail : Code of avp failed during validation.
**
**		  HAVP					hFirstOccurrenceOfAVP_i 
**			type: input
**			 detail : handle to avp failed during validation and to be added to 
**					  failed AVP list .
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the success or failur 
**		of Massage of AVP validation. 
*******************************************************************************
** Created By: Seemab Shahid	 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_AddAVPToFailedAVPList( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
												 DMBase_SList**     	ppoFailedAVPsList_i,
												 unsigned int 			unAVPCode_i,
												 HAVP					hFirstOccurrenceOfAVP_i)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");
	
		if	( NULL == hRFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer ,leaving\n");
			return Offline_CS_RetVal_Null_Pointer; 
		}
	
		if	( NULL == ppoFailedAVPsList_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppoFailedAVPsList_i is NULL pointer ,leaving\n");
			return Offline_CS_RetVal_Null_Pointer; 
		}
	
		if	( NULL == hFirstOccurrenceOfAVP_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hFirstOccurrenceOfAVP_i is NULL pointer ,leaving\n");
			return Offline_CS_RetVal_Null_Pointer; 
		}

		HMSGALLOCATIONCONTEXT hMsgAllocContext    = NULL;
		switch(unAVPCode_i)
		{
				case DMBASE_AVP_SESSION_ID:
				{
                        HAVP hSessionIdAVP = NULL;
						t_DiamOctetString* poOctetStrValue = NULL;
						poOctetStrValue = (t_DiamOctetString*)hFirstOccurrenceOfAVP_i->pData;
						if( DMBase_Retval_Success !=   DMBase_CreateUTF8StringAVP( hRFModuleContext_i->hDiameterStackContext,
																				   DMBASE_AVP_SESSION_ID, &hSessionIdAVP,
																				   poOctetStrValue->pucOctetString,
																				   poOctetStrValue->unNumOfOctets,
																				   hMsgAllocContext ) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating UTF8String AVP ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
	
						if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																		 ppoFailedAVPsList_i, (void*)hSessionIdAVP) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of sessionId AVP to list of Failed avp ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
				}
				break;
				case DMBASE_AVP_ORIGIN_HOST:
				{
                        HAVP hOriginHostAVP = NULL;
						t_DiamOctetString* poOctetStrValue = NULL;
						poOctetStrValue = (t_DiamOctetString*)hFirstOccurrenceOfAVP_i->pData;
						if( DMBase_Retval_Success !=   DMBase_CreateOctetStringAVP( hRFModuleContext_i->hDiameterStackContext,
																					DMBASE_AVP_ORIGIN_HOST, &hOriginHostAVP,
																					poOctetStrValue->pucOctetString, poOctetStrValue->unNumOfOctets,
																					hMsgAllocContext ) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating OctetString AVP ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
	
						if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																		 ppoFailedAVPsList_i, (void*)hOriginHostAVP) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Origin-Host AVP to list of Failed avp ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
				}            
				break;
				case DMBASE_AVP_ORIGIN_REALM:
				{
                        HAVP hOriginRealmAVP = NULL;
						t_DiamOctetString* poOctetStrValue = NULL;
						poOctetStrValue = (t_DiamOctetString*)hFirstOccurrenceOfAVP_i->pData;
						if( DMBase_Retval_Success !=   DMBase_CreateOctetStringAVP( hRFModuleContext_i->hDiameterStackContext,
																				   DMBASE_AVP_ORIGIN_REALM, &hOriginRealmAVP,
																				   poOctetStrValue->pucOctetString, poOctetStrValue->unNumOfOctets,
																					hMsgAllocContext ) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating OctetString AVP ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
	
						if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																		 ppoFailedAVPsList_i, (void*)hOriginRealmAVP) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Origin-Realm AVP to list of Failed avp ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
				}     
				break;
				case DMBASE_AVP_DESTINATION_REALM:
				{
                        HAVP hDestinationRealmAVP = NULL;
						t_DiamOctetString* poOctetStrValue = NULL;
						poOctetStrValue = (t_DiamOctetString*)hFirstOccurrenceOfAVP_i->pData;
						if( DMBase_Retval_Success !=   DMBase_CreateOctetStringAVP( hRFModuleContext_i->hDiameterStackContext,
																					DMBASE_AVP_DESTINATION_REALM, &hDestinationRealmAVP,
																					poOctetStrValue->pucOctetString, poOctetStrValue->unNumOfOctets,
																					hMsgAllocContext ) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating OctetString AVP ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
	
						if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																		 ppoFailedAVPsList_i, (void*)hDestinationRealmAVP) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Destination-Realm AVP to list of Failed avp ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
				}          
				break;
				case DMBASE_AVP_ACCOUNTING_RECORD_TYPE:
				{
                        HAVP hAcctRecordAVP = NULL;
						t_DiamInteger32* poIntAVPValue = NULL;
						poIntAVPValue = (t_DiamInteger32*)hFirstOccurrenceOfAVP_i->pData;
						if( DMBase_Retval_Success !=   DMBase_CreateEnumAVP( hRFModuleContext_i->hDiameterStackContext,
																			 DMBASE_AVP_ACCOUNTING_RECORD_TYPE,
																			 &hAcctRecordAVP,*(poIntAVPValue->pnInteger32),
																			 hMsgAllocContext ) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Enum AVP ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
	
						if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																		 ppoFailedAVPsList_i, (void*)hAcctRecordAVP) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Acct-Record-Type AVP to list of Failed avp ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
				}          
				break;        
				case DMBASE_AVP_ACCOUNTING_RECORD_NUMBER:
				{
                        HAVP hAcctRecordNumAVP = NULL;
						t_DiamUnsigned32* poUnsignedAVPValue = NULL;
						poUnsignedAVPValue = (t_DiamUnsigned32*)hFirstOccurrenceOfAVP_i->pData;
						if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hRFModuleContext_i->hDiameterStackContext,
																				   DMBASE_AVP_ACCOUNTING_RECORD_NUMBER, &hAcctRecordNumAVP,
																				   *(poUnsignedAVPValue->punUnsigned32),
																				   hMsgAllocContext) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Unsigned32 AVP ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
	
						if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																		 ppoFailedAVPsList_i, (void*)hAcctRecordNumAVP) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Acct-Record-Number AVP to list of Failed avp ,leaving\n");
							return Offline_CS_RetVal_Failure;
						}
	
				}          
				break; 
		}//switch(unAVPCode_i)
	
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
		return Offline_CS_RetVal_Success ;

}
/******************************************************************************
** Function Name: 
**			Rf_GetAVPDetailsforValidation
**  
** Description:
**		This function is getting the details about specified avp from message.
**		these details will later be utilized for that particuler AVP.        
**
** Parameters:
**        HRFAPPLICATIONCONTEXT         hRFModuleContext_i
**			 type: input
**			 detail : This is a handle to RF Module context handle. 
**
**		  HDIAMETERMESSAGE                	hACRMsg_i
**			 type: input
**			 detail : Handle to received message.
**
**		  unsigned int 			unAVPCode_i
**			 type: input
**			 detail : Code of avp failed during validation.
**
**		  t_ObjectOfAVPDetails*				pOAVPsDetails_i
**			type: input
**			 detail : Pointer to object in which the required details about of the
**					  specified AVP will be stored .
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the success or failur 
**		of Massage of AVP validation. 
*******************************************************************************
** Created By: Seemab Shahid	 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_GetAVPDetailsforValidation(HRFAPPLICATIONCONTEXT			  	hRFModuleContext_i,
													 HDIAMETERMESSAGE                	hACRMsg_i,
													 unsigned int 						unAVPCode_i,
													 t_ObjectOfAVPDetails*				pOAVPsDetails_i)
{ 
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

	if	( NULL == hRFModuleContext_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer ,leaving\n");
        return Offline_CS_RetVal_Null_Pointer; 
    }

	if	(	NULL == hACRMsg_i	)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hACRMsg_i is NULL pointer ,leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }

	if	(	NULL == pOAVPsDetails_i	)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pOAVPsDetails_i is NULL pointer ,leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }


	DMBase_SList*		       pCurrentNode = hACRMsg_i->hCollectionOfAVPs ;
	void*                      pData = NULL;

	while( NULL != pCurrentNode )
	{
			HAVP  hAVP = NULL ;
			unsigned int  unCurrentAVPCode ;
                    
			if( Offline_CS_RetVal_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext, pCurrentNode , &pData) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData fails , Leaving \n");
				return Offline_CS_RetVal_Failure;
			}

			hAVP = (HAVP )pData ;
			unCurrentAVPCode = hAVP->oAVPHeaderInst.unAVPCode ;
			if( unCurrentAVPCode == unAVPCode_i ) 
			{
				if( 0 == pOAVPsDetails_i->unAVPCount)
				{
					//it will store only the first occurence of the AVP in case its occurences are multiple
					pOAVPsDetails_i->hAVP_Validation = hAVP; 
				}
				
				pOAVPsDetails_i->unAVPCount++;
				//todo: remove break
				//break;
			}
			DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext , pCurrentNode , &pCurrentNode);
	}
          
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
    return Offline_CS_RetVal_Success ;                          

}
/******************************************************************************
** Function Name: 
**			Rf_AddExpectedAVPToList
**  
** Description:
**		This function is creating sample of avp according to the avp code received as input,
**		and adds it to the list of expected avps. 
**
**        HRFAPPLICATIONCONTEXT         hRFApplicationContext
**			 type: input
**			 detail : This is a handle to RF Module context handle. 
**
**        unsigned int 			unAVPCode_i
**			 type: input
**			 detail : Code of avp failed during validation.
**
**		  DMBase_SList**     		ppoFailedAVPsList_i
**			type: input
**			 detail : Pointer to pointer to List of expected AVPs .
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the success or failur 
**		of Massage of AVP validation. 
*******************************************************************************
** Created By: Seemab Shahid	 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_AddExpectedAVPToList( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
											    unsigned int 			unAVPCode_i,
											    DMBase_SList**     		ppoFailedAVPsList_i)
{

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

	if	( NULL == hRFModuleContext_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL pointer ,leaving\n");
        return Offline_CS_RetVal_Null_Pointer; 
    }

	if	( NULL == ppoFailedAVPsList_i )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppoFailedAVPsList_i is NULL pointer ,leaving\n");
        return Offline_CS_RetVal_Null_Pointer; 
    }

	HMSGALLOCATIONCONTEXT hMsgAllocContext = NULL;
    switch(unAVPCode_i)
    {
			case DMBASE_AVP_SESSION_ID:
			{
					unsigned char ucSessionId[] = "DataPortionOfSessionId";
					HAVP hSessionIdAVP = NULL;
					if( DMBase_Retval_Success !=   DMBase_CreateUTF8StringAVP( hRFModuleContext_i->hDiameterStackContext,
																			   DMBASE_AVP_SESSION_ID, &hSessionIdAVP,
																			   ucSessionId, strlen(ucSessionId),
																			   hMsgAllocContext ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating UTF8String AVP ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
		
					if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																	 ppoFailedAVPsList_i, (void*)hSessionIdAVP) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of sessionId AVP to list of Failed avp ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
			}
			break;
			case DMBASE_AVP_ORIGIN_HOST:
			{
					unsigned char ucOriginHost[] = "OriginHost";
					HAVP hOriginHostAVP = NULL;
					if( DMBase_Retval_Success !=   DMBase_CreateOctetStringAVP( hRFModuleContext_i->hDiameterStackContext,
																			   DMBASE_AVP_ORIGIN_HOST, &hOriginHostAVP,
																			   ucOriginHost, strlen(ucOriginHost),
																				hMsgAllocContext ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating OctetString AVP ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
		
					if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																	 ppoFailedAVPsList_i, (void*)hOriginHostAVP) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Origin-Host AVP to list of Failed avp ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
			}            
			break;
			case DMBASE_AVP_ORIGIN_REALM:
			{
					unsigned char ucOriginRealm[] = "OriginRealm";
					HAVP hOriginRealmAVP = NULL;
					if( DMBase_Retval_Success !=   DMBase_CreateOctetStringAVP( hRFModuleContext_i->hDiameterStackContext,
																			   DMBASE_AVP_ORIGIN_REALM, &hOriginRealmAVP,
																			   ucOriginRealm, strlen(ucOriginRealm),
																				hMsgAllocContext) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating OctetString AVP ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
		
					if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																	 ppoFailedAVPsList_i, (void*)hOriginRealmAVP) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Origin-Realm AVP to list of Failed avp ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
			}     
			break;
			case DMBASE_AVP_DESTINATION_REALM:
			{
					unsigned char ucDestinationRealm[] = "DestinationRealm";
					HAVP hDestinationRealmAVP = NULL;
					if( DMBase_Retval_Success !=   DMBase_CreateOctetStringAVP( hRFModuleContext_i->hDiameterStackContext,
																				DMBASE_AVP_DESTINATION_REALM, &hDestinationRealmAVP,
																				ucDestinationRealm, strlen(ucDestinationRealm),
																				hMsgAllocContext ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating OctetString AVP ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
		
					if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																	 ppoFailedAVPsList_i, (void*)hDestinationRealmAVP) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Destination-Realm AVP to list of Failed avp ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
			}          
			break;
			case DMBASE_AVP_ACCOUNTING_RECORD_TYPE:
			{
					int nAcctrecordType = DMBASE_START_RECORD;
					HAVP hAcctRecordAVP = NULL;
		
					if( DMBase_Retval_Success !=   DMBase_CreateEnumAVP( hRFModuleContext_i->hDiameterStackContext,
																		 DMBASE_AVP_ACCOUNTING_RECORD_TYPE, &hAcctRecordAVP,
																		 nAcctrecordType,
																		 hMsgAllocContext ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Enum AVP ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
		
					if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																	 ppoFailedAVPsList_i, (void*)hAcctRecordAVP) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Acct-Record-Type AVP to list of Failed avp ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
			}          
			break;        
			case DMBASE_AVP_ACCOUNTING_RECORD_NUMBER:
			{
					unsigned int unAcctRecordNumber = 0;
					HAVP hAcctRecordNumAVP = NULL;
		
					if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hRFModuleContext_i->hDiameterStackContext,
																			   DMBASE_AVP_ACCOUNTING_RECORD_NUMBER, &hAcctRecordNumAVP,
																			   unAcctRecordNumber,
																			   hMsgAllocContext ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Unsigned32 AVP ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
		
					if( DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
																	 ppoFailedAVPsList_i, (void*)hAcctRecordNumAVP) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Appending sample of Acct-Record-Number AVP to list of Failed avp ,leaving\n");
						return Offline_CS_RetVal_Failure;
					}
				
			}          
			break; 
    }//switch(unAVPCode_i)
   
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving \n");
    return Offline_CS_RetVal_Success ;
}

/******************************************************************************
** Function Name: 
**			Rf_GenerateACAMsg
**  
** Description:
**		This function returns a hnadle to newly created ACA message 
**      in response of Valid ACR Message Received by RF Application
**      
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT     hRFModuleContext_i
**			 type: input/output
**			 detail : this is a RF Application context handle. 
**
**
**		 HDIAMETERMESSAGE*	    phACAMsg_o
**			 type: output
**			 detail : this is a pointer to  Handle to newly created 
**					  answer message.
**
** 
**		 int       unRecultCode_i
**			type: input
**		 detail: this is a integer holding value of result code that will 
**				be set in the response message.
**
**		 t_DiamOctetString*      pOSessionID_i
**			 type: input
**		 detail: this is a pointer to object of session id.
**
**		 t_ACRInfo*                     poACRInfo_i
**			type: input
**		 detail: this is a pointer to object of ACRInfo.
** Return:
**		An t_DiamAPIReturnVal value specifung wheather the function is 
**     is returnuing successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_GenerateACAMsg( HRFAPPLICATIONCONTEXT          hRFModuleContext_i,
										  HPEERTABLEENTRY			 	 hPeer_i,
										  int                            unRecultCode_i,// TODO: correct speling
										  unsigned char*             	 pucSessionID_i,// TODO: make it Object
										  t_ACRInfo*                     poACRInfo_i)
{   
        t_ACA_Params                    oACA_Param;
        unsigned char                   ucACRMsgFlags;
        unsigned char                   ucACRMsgVersion;      
        unsigned int                    unACRMsgHopByHopID;
        unsigned int                    unACRMsgEndToEndID;
		HDIAMETERMESSAGE	           	hACAMsg = NULL;
        t_Offline_CS_ReturnVal			oRetval = Offline_CS_RetVal_Success;
		FILE*							fpMSGDump = NULL;
		
       
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

		if( NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }
		
        
		if (NULL == pucSessionID_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucSessionID_i is Invalid , leaving\n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

		if (NULL == poACRInfo_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poACRInfo_i is Invalid , leaving\n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

		if (NULL == hPeer_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poACRInfo_i is Invalid , leaving\n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

		#ifdef CDF_DUMP_ACA_MSG_TO_CONSOLE_AND_FILE
			fpMSGDump = fopen("/root/ACAMsgDump.txt","a");
	
			if (NULL == fpMSGDump) 
			{
				printf("\n\n\n UNABLE to open file , leaving \n\n\n\n");
				return Offline_CS_RetVal_Failure;
			}
		#endif

		

        unsigned int    unDiaMsgCommandCode     = RF_MSG_CMD_CODE_ACR ;
		unsigned int    unDiaMsgApplicationID   = RF_APPLICATION_ID;
		unsigned char   ucDiaMsgVersion         = RF_MSG_VERSION;
		HMSGALLOCATIONCONTEXT hMsgAllocContext = NULL;
		HAVPCOLLECTION		   hAVPCollection  = NULL;
		HDIAMETERSTACKCONTEXT hDiameterStackContext = hRFModuleContext_i->hDiameterStackContext;


		// Creating ACA message
         
         ucACRMsgFlags           = poACRInfo_i->ucFlags;
         unACRMsgHopByHopID      = poACRInfo_i->unHopByHopID;
         unACRMsgEndToEndID      = poACRInfo_i->unEndToEndID;


		 if( RfAPI_Retval_Success != DMBase_CreateDiameterMessage( hDiameterStackContext , &hACAMsg ,
																   ucDiaMsgVersion , unDiaMsgCommandCode ,  
																   unDiaMsgApplicationID,ucACRMsgFlags,
																   unACRMsgHopByHopID, unACRMsgEndToEndID) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACA Message Creation fail , Leaving \n");
			return RfAPI_Retval_Failure ;
		}
	

		 // Clear the R-Flag in ACA Header Field
		_Bool bSetFlag = 0;
	   if( RfAPI_Retval_Success != DMBase_SetDiaMsgRFlag( hDiameterStackContext, hACAMsg, bSetFlag ) )
	   {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACA Message Header P-flag clearing fail , Leaving \n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ; 
	   }


	   if(DMBase_Retval_Success!=  DMBase_Get_AllocationContext_ForMessage( hDiameterStackContext,
																			hACAMsg,&hMsgAllocContext))
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_Get_AllocationContext_ForMessage() Fails , Leaving \n");
		   oRetval = RfAPI_Retval_Failure;
		   goto ExitMe ;
	   }

       if(DMBase_Retval_Success!= DMBase_MemsetZero(&oACA_Param, sizeof(t_ACR_Params)))// TODO: t_ACR_Params to t_ACA_Params
	   {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_MemsetZero() Fails , Leaving \n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
	   }
                    
        
        // Create Result Code AVP
	   if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hDiameterStackContext,
																  DMBASE_AVP_RESULT_CODE,
																  &( oACA_Param.hAVP_ResultCode ),
																  unRecultCode_i,
																  hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Unsigned32 AVP ,leaving\n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_ResultCode);
		
       //To Create Session ID AVP 
        if( DMBase_Retval_Success !=  DMBase_CreateUTF8StringAVP( hDiameterStackContext,
																  DMBASE_AVP_SESSION_ID,
																  &(oACA_Param.hAVP_SessionID),
																  pucSessionID_i,
																  strlen(pucSessionID_i),
																  hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating session-Id AVP ,leaving\n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_SessionID );

		// create Origin_Host AVP
		if( DMBase_Retval_Success !=  DMBase_CreateUTF8StringAVP( hDiameterStackContext,
																  DMBASE_AVP_ORIGIN_HOST,
																  &(oACA_Param.hAVP_OriginHost),
																  hDiameterStackContext->oHostName.pucOctetString,
																  hDiameterStackContext->oHostName.unNumOfOctets,
																  hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Orgigin-Host AVP ,leaving\n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_OriginHost );

		// create Origin-realm AVP
		if( DMBase_Retval_Success !=  DMBase_CreateUTF8StringAVP( hDiameterStackContext,
																  DMBASE_AVP_ORIGIN_REALM,
																  &(oACA_Param.hAVP_OriginRealm),
																  hDiameterStackContext->oRealmName.pucOctetString,
																  hDiameterStackContext->oRealmName.unNumOfOctets,
																  hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Origin-Realm AVP ,leaving\n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_OriginRealm );

     
        // create Acct_Record_Type AVP 
        if( DMBase_Retval_Success !=   DMBase_CreateEnumAVP( hDiameterStackContext,
															 DMBASE_AVP_ACCOUNTING_RECORD_TYPE,
															 &(oACA_Param.hAVP_AccountingRecordType),
															 poACRInfo_i->nAcctRecordType,
															 hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Acct-Record-Type AVP ,leaving\n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_AccountingRecordType );

		// create Acct_Record_Number AVP
       if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hDiameterStackContext,
																   DMBASE_AVP_ACCOUNTING_RECORD_NUMBER,
																   &(oACA_Param.hAVP_AccountingRecordNumber),
																   poACRInfo_i->unAcctRecordNumber,
																   hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Acct-Record-Number AVP ,leaving\n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_AccountingRecordNumber );
 
        // create DMBASE_AVP_ACCT_INTERIM_INTERVAL  AVP 
		if( DMBase_Retval_Success !=   DMBase_CreateUnsigned32AVP( hDiameterStackContext,
																   DMBASE_AVP_ACCT_INTERIM_INTERVAL,
																   &(oACA_Param.hAVP_AcctInterimInterval),
																   OFFLINE_CS_ACCT_INTERIM_INTERVAL_VALUE_IN_SECONDS,
																   hMsgAllocContext ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating Unsigned32 AVP ,leaving\n");
			oRetval = RfAPI_Retval_Failure;
			goto ExitMe ;
		}
		DMBase_AppendSList(hDiameterStackContext, &hAVPCollection, (void*)oACA_Param.hAVP_AcctInterimInterval );


		if( RfAPI_Retval_Success != DMBase_AddAVPCollectionToDiameterMessage( hDiameterStackContext,
																			  hACAMsg,
																			  hAVPCollection) )
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding AVP collection to ACA Message, Leaving \n");
		   oRetval = RfAPI_Retval_Failure;
		   goto ExitMe ; 
	   }

		#ifdef CDF_DUMP_ACA_MSG_TO_CONSOLE_AND_FILE
			fprintf(fpMSGDump,"\n\n\n\n BEGIN DUMP OF NEW ACA MESSAGE BEFORE SEND \n\n\n\n");
		   DMBase_Debug_DumpDiamMsgContents(hDiameterStackContext,fpMSGDump,hACAMsg);
		   fprintf(fpMSGDump,"\n\n\n\n END DUMP OF NEW ACA MESSAGE BEFORE SEND \n\n\n\n");
	   #endif
	   

		if( DMBase_Retval_Success != DMBase_SendMessageToPeer( hDiameterStackContext, hPeer_i, hACAMsg) )
		 {
			 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in sending ACA Message to Peer , Leaving \n");
			 oRetval = RfAPI_Retval_Failure;
             goto ExitMe ;
		 }

		/*	fprintf(fpMSGDump,"\n\n\n\n BEGIN DUMP OF NEW ACA MESSAGE AFTER  SEND \n\n\n\n");
       DMBase_Debug_DumpDiamMsgContents(hDiameterStackContext,fpMSGDump,hACAMsg);
	   fprintf(fpMSGDump,"\n\n\n\n END DUMP OF NEW ACA MESSAGE AFTER SEND \n\n\n\n");
	   */
	
		ExitMe:
		  DMBase_DestroyDiaMessage(hDiameterStackContext, &hACAMsg);
		#ifdef CDF_DUMP_ACA_MSG_TO_CONSOLE_AND_FILE
			if (NULL != fpMSGDump) 
			{
				fclose(fpMSGDump);
				fpMSGDump = NULL;
			}
		#endif
          
       //Also get list of Proxy Info AVPs from ACR message and add it to the structure
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
       return oRetval;    
    }

/******************************************************************************
** Function Name: 
**			Rf_CreateACRInfo
**  
** Description:
**		This function extracts the data that needs to store in session object,
**      from Received ACR Messsage and returns that data in form of 
**      structure as output parameter .
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT   hRFModuleContext_i
**			 type: input/output
**			 detail : this is a RF Application context handle. 
**
**		 HDIAMETERMESSAGE        hRFMsg_i
**			 type: input
**			 detail : this is a Handle to Message Received By RF Application.
**
**		HPEERTABLEENTRY		 hPeerTabEntry_i
**			 type: input
**			 detail : this is a Handle to Peer table entry.
**
**		 t_AcctRecord**          ppSessionData_o
**			type: output
**			detail: this is a pointer to pointer , which points to session data extracted 
**                  from the ACR Message
**				
**
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  the success or failure of the function. 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_CreateACRInfo( HRFAPPLICATIONCONTEXT   hRFModuleContext_i,
										 t_MessageStructure	  	 oMessageStructure_i,
										 HPEERTABLEENTRY		 hPeerTabEntry_i,
										 t_ACRInfo**             ppACRInfo_o)
{
        int                     unSize              = 0;
        HAVP                    hToTempAVP          =  NULL; 
        t_DiamOctetString*      pOSessionID         =  NULL;
        t_DiamEnum*             pOAcctRecordType    =  NULL;
        t_DiamUnsigned32*       pOAcctRecordNumber  =  NULL;
        t_DiamUnsigned32*       pOAcctApplicationID =  NULL;
        t_DiamTime*             pOEventTimeStamp    =  NULL;
        

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

        if(NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i, Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        
        if (NULL == ppACRInfo_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ppACRInfo_o is Null Parameter , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }
        
        if (NULL != *ppACRInfo_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "*ppACRInfo_o is Invalid  Parameter ,Leaving \n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }
       
        //allocate memory  to SessionDataObj 
        unSize = (sizeof(t_ACRInfo));
        if(DMBase_Retval_Success!= DMBase_Malloc( hRFModuleContext_i->hDiameterStackContext,(void*)ppACRInfo_o, unSize) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in allocating memeory to ACRInfo ,Leaving \n");
            return Offline_CS_RetVal_Failure;
        }

		(*ppACRInfo_o)->ucFlags = oMessageStructure_i.oMsgHrdFieldsStruct.ucFlags;
		(*ppACRInfo_o)->unHopByHopID = oMessageStructure_i.oMsgHrdFieldsStruct.unHopByHopID;
		(*ppACRInfo_o)->unEndToEndID = oMessageStructure_i.oMsgHrdFieldsStruct.unEndToEndID;
		(*ppACRInfo_o)->nAcctRecordType = oMessageStructure_i.oMsgAVPsStruct.nAcctRecordType;
		(*ppACRInfo_o)->unAcctRecordNumber = oMessageStructure_i.oMsgAVPsStruct.unAcctRecordNumber;
		(*ppACRInfo_o)->unAcctApplicationID = oMessageStructure_i.oMsgAVPsStruct.unAcctApplicationId;
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal Rf_AddACRInfoToNewSession( HRFAPPLICATIONCONTEXT		        	hRFModuleContext_i,
												  DMBASE_HASHTABLE_KEY_HANDLE       	hSessionTableKey_i,
												  unsigned char*	                  	pucSessionID_i,
												  HPEERTABLEENTRY	                    hPeerTableEntry_i,
												  t_SessionObject**						ppoSessionObject_o,
												  unsigned int                      	unSessionRecordNumber,
												  t_ACRInfo*		       	            poACRInfo_i )
{

		t_SessionObject*    poSessionObject = NULL;

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");


        if(NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL == hSessionTableKey_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hSessionTableKey_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL == pucSessionID_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter pucSessionID_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }


        if(NULL == hPeerTableEntry_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hPeerTableEntry_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL == poACRInfo_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poACRInfo_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        
		// Session does not exist 
		int nSize   = sizeof(t_SessionObject);
		if(DMBase_Retval_Success != DMBase_Malloc(hRFModuleContext_i->hDiameterStackContext,
												  (void **)&poSessionObject,nSize) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Problem in Memory Allocation.\n");
			return Offline_CS_RetVal_Failure;
		}

		nSize   = sizeof( t_ACRInfo );
		if(DMBase_Retval_Success != DMBase_Malloc( hRFModuleContext_i->hDiameterStackContext,
												  (void **)&(poSessionObject->poLastReceivedACRInfo),nSize) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Problem in Memory Allocation.\n");
			return Offline_CS_RetVal_Failure;
		}

	
		if(DMBase_Retval_Success != Rf_InsertObjectToSessionTable( hRFModuleContext_i,hSessionTableKey_i,
																   poSessionObject))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem Occured while Inserting session object to Session Table , Leaving \n");
			return Offline_CS_RetVal_Failure;
		}

        // copy SessionID to the newly allocated structure
        strcpy(poSessionObject->ucSessionID,pucSessionID_i);        
        
		// set peer entry in session object
		poSessionObject->hPeerTabEntry = hPeerTableEntry_i;

		// Add ACRInfo to session object 
	   if(DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
													   &(poSessionObject->pListOfUnprocessedACRInfo),
													   (void*)poACRInfo_i ))
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " A Problem Occurred while Appending ACRInfo to List\n");
		   return Offline_CS_RetVal_Failure;
	   }

		// This function is updateing the ACRInfo in session object
		//with the last received ACR info. This information will be 
 		//used in case the session object is empty and the timer for the 
		//next ACR got expired. so the application will be using this info 
		//in generating response to peer. 
		poSessionObject->poLastReceivedACRInfo->ucFlags 				= poACRInfo_i->ucFlags;
		poSessionObject->poLastReceivedACRInfo->unEndToEndID 			= poACRInfo_i->unEndToEndID;
		poSessionObject->poLastReceivedACRInfo->unHopByHopID 			= poACRInfo_i->unHopByHopID;
		poSessionObject->poLastReceivedACRInfo->unAcctApplicationID 	= poACRInfo_i->unAcctApplicationID;
		poSessionObject->poLastReceivedACRInfo->nAcctRecordType     	= poACRInfo_i->nAcctRecordType;
		poSessionObject->poLastReceivedACRInfo->unAcctRecordNumber  	= poACRInfo_i->unAcctRecordNumber;

		*ppoSessionObject_o = poSessionObject;
       
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
        return Offline_CS_RetVal_Success ;


}

t_Offline_CS_ReturnVal Rf_AddACRInfoToExistingSession( HRFAPPLICATIONCONTEXT		 hRFModuleContext_i,
													   t_SessionObject*	      	 	 poSessionObject_o,
													   t_ACRInfo*		             poACRInfo_i)
{



	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");

		
		_Bool               	bRecordFound          	= 0;
		t_ACRInfo*		        poTempACRInfo           = NULL;
		unsigned int 			unAcctRecordNumber		= 0;
        DMBase_SList*           pNodeOfReqACRInfo       = NULL;

        if(NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL ,leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

       if(NULL == poSessionObject_o)
       {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poSessionObject_o is NULL ,leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
       }

       if(NULL == poACRInfo_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poACRInfo_i is NULL ,leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        // Add ACRInfo to session object 
	   if(DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
													   &(poSessionObject_o->pListOfUnprocessedACRInfo),
													   (void*)poACRInfo_i ))
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " A Problem Occurred while Appending ACRInfo to List\n");
		   return Offline_CS_RetVal_Failure;
	   }


	   // This function is updateing the ACRInfo in session object
		//with the last received ACR info. This information will be 
 		//used in case the session object is empty and the timer for the 
		//next ACR got expired. so the application will be using this info 
		//in generating response to peer. 
		poSessionObject_o->poLastReceivedACRInfo->ucFlags 				= poACRInfo_i->ucFlags;
		poSessionObject_o->poLastReceivedACRInfo->unEndToEndID 			= poACRInfo_i->unEndToEndID;
		poSessionObject_o->poLastReceivedACRInfo->unHopByHopID 			= poACRInfo_i->unHopByHopID;
		poSessionObject_o->poLastReceivedACRInfo->unAcctApplicationID 	= poACRInfo_i->unAcctApplicationID;
		poSessionObject_o->poLastReceivedACRInfo->nAcctRecordType     	= poACRInfo_i->nAcctRecordType;
		poSessionObject_o->poLastReceivedACRInfo->unAcctRecordNumber  	= poACRInfo_i->unAcctRecordNumber;


	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
	   return Offline_CS_RetVal_Success;

}


/******************************************************************************
** Function Name: 
**			Rf_SearchObjectInSessionTable
**  
** Description:
**		This function searches the session object in session Table.
** Parameters:
**
**		 HRFAPPLICATIONCONTEXT			    hRFModuleContext_io
**			type: input
**			 detail : this is a handle to Rf Module Context.
**
**		  DMBASE_HASHTABLE_KEY_HANDLE       hSessionTableKey_i
**			 type: input
**			 detail : this is a handle to session table key. 
**
**		t_SessionObject**			        ppoSessionObject_o
**			 type: input
**			 detail : this is pointer to pointer that points  object
**					  sessionobject. 
**		
**** Return:
**		An t_Offline_CS_ReturnVal value specifying the success or failure of function. 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_SearchObjectInSessionTable(	HRFAPPLICATIONCONTEXT			hRFModuleContext_io,
														DMBASE_HASHTABLE_KEY_HANDLE		hSessionTableKey_i,
														t_SessionObject**			    ppoSessionObject_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

		void* pData = NULL;

        if(NULL == hRFModuleContext_io)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_io , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }
       
        if(NULL == hSessionTableKey_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hSessionTableKey , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }
         
        if(NULL == ppoSessionObject_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter ppoSessionObject_o , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }
       
        if(NULL != *ppoSessionObject_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Invalid Parameter *ppoSessionObject_o , Leaving \n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

		
			
        DMBase_HashTableSearch( hRFModuleContext_io->hDiameterStackContext,  
                                hRFModuleContext_io->HandleToSessionTable, 
                                hSessionTableKey_i, &pData);
		
       *ppoSessionObject_o =(t_SessionObject*)pData;

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
        return Offline_CS_RetVal_Success ;

}

/******************************************************************************
** Function Name: 
**			Rf_AddACRInfoToSessionTable
**  
** Description:
**		This function searches the ACRInfo in session Table.
** Parameters:
**
**		 HRFAPPLICATIONCONTEXT             hRFModuleContext_i
**			type: input
**			 detail : this is a handle to Rf Module Context.
**
**		  DMBASE_HASHTABLE_KEY_HANDLE       hSessionTableKey_i
**			 type: input
**			 detail : this is a handle to session table key. 
**
**		t_SessionObject*	 poSessionObject_i
**			type: input
**			 detail : this is a pointer that points to Session Object that 
**					  will be inserted into session table.
**		 
**** Return:
**		An t_Offline_CS_ReturnVal value specifying the success or failure of function. 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_InsertObjectToSessionTable(HRFAPPLICATIONCONTEXT			    hRFModuleContext_i,
                                                     DMBASE_HASHTABLE_KEY_HANDLE		hSessionTableKey_i,
                                                     t_SessionObject*			        poSessionObject_i)
{
	    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

        if(NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL == hSessionTableKey_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hSessionTableKey_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL == poSessionObject_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionTableObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(DMBase_Retval_Success != DMBase_HashTableInsert( hRFModuleContext_i->hDiameterStackContext, 
                                                            hRFModuleContext_i->HandleToSessionTable,
                                                            (void*)poSessionObject_i, hSessionTableKey_i))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Hash Table Insertion Problem\n");
            return Offline_CS_RetVal_Failure;
        }

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving\n");
        return Offline_CS_RetVal_Success ;
}
/******************************************************************************
** Function Name: 
**			Rf_DeleteSession
**  
** Description:
**		This function is deteting whole session from session table.
** Parameters:
**
**		 HRFAPPLICATIONCONTEXT             hRFModuleContext_i
**			type: input
**			 detail : this is a handle to Rf Module Context.
**
**		  DMBASE_HASHTABLE_KEY_HANDLE       hSessionTableKey_i
**			 type: input
**			 detail : this is a handle to session table key. 
**		
**** Return:
**		An t_Offline_CS_ReturnVal value specifying the success or failure of function. 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal Rf_DeleteSession( HRFAPPLICATIONCONTEXT		            hRFModuleContext_i,
                                         t_SessionObject* 						poSessionObject_i,
										 t_DiamHashTableKey*					poDiamHashTableKey_i )
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");

        t_SessionObject*		poSessionObject = NULL;

        if(NULL == hRFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i, Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		if(NULL == poSessionObject_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_i, Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

       if(NULL == poDiamHashTableKey_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poDiamHashTableKey_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }
/*

        if( Offline_CS_RetVal_Success != Rf_SearchObjectInSessionTable( hRFModuleContext_i,
																		hSessionTableKey_i,
																		&poSessionObject ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Rf_SearchObjectInSessionTable Fails , Leaving \n");
            return Offline_CS_RetVal_Failure;
        }

        if( NULL == poSessionObject)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Session Object not found , Leaving \n");
            return Offline_CS_RetVal_Data_Not_Found_In_SessionTable;
        }
*/
        if (NULL != poSessionObject_i->pListOfUnprocessedACRInfo)
        {
                // Make free the list of unprocessed ACRInfo inside the Session Object
                if(DMBase_Retval_Success !=  DMBase_SListFree( hRFModuleContext_i->hDiameterStackContext,
                                                              &(poSessionObject_i->pListOfUnprocessedACRInfo),
                                                              Rf_LListFreeLinkDataCallback))
                {
                        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " Freeing of SList node  Fails , Leaving \n");
                        return Offline_CS_RetVal_Failure;
                }
                poSessionObject_i->pListOfUnprocessedACRInfo = NULL;
        }
        

        // before destroying the session ,timer for that session must be killed
        if(Offline_CS_RetVal_Success != Rf_KillSessionTimer(hRFModuleContext_i,poSessionObject_i))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Killing of Session timer Fails , Leaving \n");
            return Offline_CS_RetVal_Failure;
        }

        DMBase_Free( hRFModuleContext_i->hDiameterStackContext,(void*)&(poSessionObject_i->poLastReceivedACRInfo));

		DMBase_Free(hRFModuleContext_i->hDiameterStackContext, (void*)&(poSessionObject_i->poTimerDataForCurrSess) );

		DMBase_Free(hRFModuleContext_i->hDiameterStackContext, (void*)&poSessionObject_i );
        // now destroy the session Object itself
        /*if(Offline_CS_RetVal_Success != Rf_DestroySessionObject(hRFModuleContext_i,&poSessionObject))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Rf_DestroySessionObject() Fails , Leaving \n");
            return Offline_CS_RetVal_Failure;
        }*/
		// move to destroy sessionObject
        // TODO:then delete the session from session Table
        if(DMBase_Retval_Success != DMBase_HashTableRemove( hRFModuleContext_i->hDiameterStackContext,
                                                            hRFModuleContext_i->HandleToSessionTable, 
                                                            poDiamHashTableKey_i) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_HashTableRemove() Fails , Leaving \n");
            return Offline_CS_RetVal_Failure;
        }

		hRFModuleContext_i->unSessionDeleteCounter++;
				 //printf("\n\nExisting Session Deleted  = %d \n\n ",hRFModuleContext_i->unSessionDeleteCounter);

        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
        return Offline_CS_RetVal_Success;
 }

/******************************************************************************
** Function Name: 
**			Rf_KillSessionTimer
**  
** Description:
**		This function is killing the timer inside the session , set 
**		after the receival of last ACR related to that session.		
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT   hRFModuleContext_i
**			 type: input
**			 detail : this is a handle to Rf module context. 
**
**		 t_SessionObject*        poSessionObject_i
**			type: input
**			detail: this is a pointer to  sessionObject.
**          
** Return:
**		An t_Offline_CS_ReturnVal value specify the success or failure of function. 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal  Rf_KillSessionTimer( HRFAPPLICATIONCONTEXT   hRFModuleContext_i,
											 t_SessionObject*        poSessionObject_i)
{

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");


        if(NULL == hRFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL == poSessionObject_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }


        if(NULL != poSessionObject_i->TimerID )
        {
            if( DMBase_Retval_Success!= DMBase_KillTimer( hRFModuleContext_i->hDiameterStackContext,
                                                          poSessionObject_i->TimerID ))
            {
                DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," DMBase_KillTimer() Fails, leaving\n");
                return Offline_CS_RetVal_Failure;
            }
			poSessionObject_i->TimerID= NULL;
        }
        

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
        return Offline_CS_RetVal_Success;
}


/******************************************************************************
** Function Name: 
**			Rf_CreateSessionObject
**  
** Description:
**		This function appends node to the link list.
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
**
**
** Return:
**		An t_DiamAPIReturnVal value specify whether data appended to list    
**		successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_SetSessionIDInSessionObject( HRFAPPLICATIONCONTEXT	            hRFModuleContext_i,
                                                       t_SessionObject*	                    poSessionObject_o,
                                                       t_DiamOctetString*                   pOSessionID_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");

        /*t_DiamOctetString*      poTempSessionID = NULL;

        if(NULL == hRFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if( NULL == poSessionObject_o )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_o , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if( NULL == pOSessionID_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(DMBase_Retval_Success!= DMBase_Malloc(hRFModuleContext_i->hDiameterStackContext,
												 (void*)&poTempSessionID,sizeof(t_DiamOctetString)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_Malloc() Fails , Leaving \n");
            return Offline_CS_RetVal_Failure;
		}

		if(DMBase_Retval_Success!= DMBase_Malloc(hRFModuleContext_i->hDiameterStackContext,
												 (void*)&(poTempSessionID->pucOctetString)
												 ,sizeof(pOSessionID_i->unNumOfOctets)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_Malloc() Fails , Leaving \n");
            return Offline_CS_RetVal_Failure;
		}

        
        // copy SessionID to the newly allocated structure
        memcpy((poTempSessionID->pucOctetString),(pOSessionID_i->pucOctetString),pOSessionID_i->unNumOfOctets );        
        
        // Copy the size of Data to the Newly Allocated structure as well 
        poTempSessionID->unNumOfOctets = pOSessionID_i->unNumOfOctets;

        poSessionObject_o->pOSessionID= poTempSessionID;
	*/
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
        return Offline_CS_RetVal_Success;
}
/******************************************************************************
** Function Name: 
**			Rf_DestroySessionObject
**  
** Description:
**		This function is destroying Session Object.
**
** Parameters:
**		  HRFAPPLICATIONCONTEXT	        hRFModuleContext_i
**			 type: input
**			 detail : This is a handle to Rf Module context. 
**
**		t_SessionObject**		ppoSessionObject_i
**			type: input
**			detail: this is a pointer to a pointer which points
**					to SessionObject wants to delete.
** Return:
**		An t_DiamAPIReturnVal value specify whether data appended to list    
**		successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_DestroySessionObject(HRFAPPLICATIONCONTEXT	        hRFModuleContext_i,
											   t_SessionObject**			    ppoSessionObject_i )
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");

        if(NULL == hRFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if( NULL == ppoSessionObject_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if( NULL == *ppoSessionObject_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter *poSessionObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		// TODO:remove all the dybamic object

		// Here destroy the whole session table first

		// then destroy the DbModule Context

        // Now free the SessionObject pointer 
        DMBase_Free(hRFModuleContext_i->hDiameterStackContext, (void*)ppoSessionObject_i );
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
        return Offline_CS_RetVal_Success;
}
/******************************************************************************
** Function Name: 
**			Rf_AddACRInfoToSessionObject
**  
** Description:
**		This function is adding ACRInfo to Session Object.
**
** Parameters:
**       HRFAPPLICATIONCONTEXT	        hRFModuleContext_i
**			 type: input
**			 detail : This is a handle to Rf Module context. 
**
**		t_SessionObject*		poSessionObject_i
**			type: input
**			detail: this is a pointer  which points
**					to SessionObject to which the ACRInfo wants to add.
**
**		 t_ACRInfo*		poACRInfo_i
**			type: input
**			detail: this is a pointer to object to ACRinfo want to add
**					to SessionObject.
**
**
** Return:
**		An t_Offline_CS_ReturnVal value specify the success or failure of function 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_AddACRInfoToSessionObject( HRFAPPLICATIONCONTEXT              hRFModuleContext_i,
													 t_SessionObject*	                poSessionObject_o,
													 t_ACRInfo*		                    poACRInfo_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");

		
		_Bool               	bRecordFound          	= 0;
		t_ACRInfo*		        poTempACRInfo           = NULL;
		unsigned int 			unAcctRecordNumber		= 0;
        DMBase_SList*           pNodeOfReqACRInfo       = NULL;

        if(NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! hRFModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

       if(NULL == poSessionObject_o)
       {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! poSessionObject_o is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
       }

       if(NULL == poACRInfo_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," Error! poACRInfo_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		unAcctRecordNumber = poACRInfo_i->unAcctRecordNumber;
	    Rf_SearchACRInfoInSessionObject( hRFModuleContext_i, poSessionObject_o,
									     unAcctRecordNumber, &poTempACRInfo,
										 &bRecordFound);
	   
	   if(bRecordFound)
       {
           // ACRInfo Allready present in SessionObject
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Session Record Allready Present In Session Table fails , Leaving \n");
		   return Offline_CS_RetVal_Data_Found_In_SessionTable;
	   }

       // ACRInfo not found in SessionObject so add it into Session Object
       if(DMBase_Retval_Success != DMBase_AppendSList( hRFModuleContext_i->hDiameterStackContext,
													   &(poSessionObject_o->pListOfUnprocessedACRInfo),
													   (void*)poACRInfo_i ))
	   {
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " A Problem Occurred while Appending ACRInfo to List\n");
		   return Offline_CS_RetVal_Failure;
	   }

	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
	   return Offline_CS_RetVal_Success;

}

/******************************************************************************
** Function Name: 
**			Rf_SearchACRInfoInSessionObject
**  
** Description:
**		This function is searches ACRInfo in Session Table.
**
** Parameters:
**       HRFAPPLICATIONCONTEXT	        hRFModuleContext_i
**			 type: input
**			 detail : This is a handle to Rf Module context. 
**
**		t_SessionObject*		poSessionObject_i
**			type: input
**			detail: this is a pointer  which points
**					to SessionObject in which the ACRInfo will be searched.
**
**		unsigned int 		unACRInfoNumber_i
**			type: input
**			detail: this is a Number of ACRinfo required to to search 
**					 in SessionObject.
**		
**		 t_ACRInfo*		poACRInfo_i
**			type: input
**			detail: this is a pointer to object to ACRinfo want to add
**					to SessionObject.
**
**		 _Bool*		pbRecordFound_o
**			type: input
**			detail: Shows the Search result.
**
**
** Return:
**		An t_Offline_CS_ReturnVal value specify the success or failure of function 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_SearchACRInfoInSessionObject( HRFAPPLICATIONCONTEXT    	hRFModuleContext_i,
                                                        t_SessionObject*	        poSessionObject_i,
                                                        unsigned int 				unACRInfoNumber_i,
                                                        t_ACRInfo**		            ppoACRInfo_o,
                                                        _Bool*					    pbRecordFound_o )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");

		DMBase_SList*     		pCurrentNode  			= NULL;
		_Bool               	bFound          		= 0;
		void*					pvNodeData				= NULL;
		unsigned int      		unTempACRInfoNum   	    = 0;
		t_ACRInfo*  	        poTempACRInfo           = NULL;


        if(NULL == hRFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

        if(NULL == poSessionObject_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

        if(NULL == ppoACRInfo_o )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter ppoACRInfo_o , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

        if(NULL != *ppoACRInfo_o )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Invalid  Parameter *ppoACRInfo_o , Leaving \n");
            return Offline_CS_RetVal_Invalid_Parameter;
         }

		pCurrentNode = (DMBase_SList*)(poSessionObject_i->pListOfUnprocessedACRInfo);

    
		while( NULL != pCurrentNode )
		{
                       
			if( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i->hDiameterStackContext,
                                                                  pCurrentNode , &pvNodeData) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData() fails , Leaving \n");
				return Offline_CS_RetVal_Failure;
			}
                          
			poTempACRInfo = (t_ACRInfo*)pvNodeData;
			unTempACRInfoNum = poTempACRInfo->unAcctRecordNumber;
			if( unTempACRInfoNum == unACRInfoNumber_i ) 
			{
				*ppoACRInfo_o = poTempACRInfo; 
                bFound = 1;
                break;
			}
			DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext, pCurrentNode ,&pCurrentNode);
	   }// end while
       *pbRecordFound_o = bFound;
	   

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
       return Offline_CS_RetVal_Success;
}
/******************************************************************************
** Function Name: 
**			Rf_SearchACRInfoNodeInSessionObject
**  
** Description:
**		This function is searches ACRInfo node in Session Table.
**
** Parameters:
**       HRFAPPLICATIONCONTEXT	        hRFModuleContext_i
**			 type: input
**			 detail : This is a handle to Rf Module context. 
**
**		t_SessionObject*		poSessionObject_i
**			type: input
**			detail: this is a pointer  which points
**					to SessionObject in which the ACRInfo node will be searched.
**
**		unsigned int 		unACRInfoNumber_i
**			type: input
**			detail: this is a Number of ACRinfo node required to to search 
**					 in SessionObject.
**
**		 _Bool*		pbRecordFound_o
**			type: input
**			detail: Shows the Search result.
**
****		 DMBase_SList**      ppoListNodeFound_o
**			type: input
**			detail: this is a pointer to pointer which will point ACRinfo node
**					want to search in SessionObject.
**
** Return:
**		An t_Offline_CS_ReturnVal value specify the success or failure of function 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_SearchACRInfoNodeInSessionObject( HRFAPPLICATIONCONTEXT    	hRFModuleContext_i,
															t_SessionObject*	        poSessionObject_i,
															unsigned int 				unAcctRecordNumber_i,
															_Bool*					    pbRecordFound_o,
															DMBase_SList**              ppoListNodeFound_o)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Entering.\n");

		if(NULL == hRFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

		if(NULL == poSessionObject_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

		if(NULL == pbRecordFound_o )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter pbRecordFound_o , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

		if(NULL == ppoListNodeFound_o )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter ppoListNodeFound_o , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
         }

		DMBase_SList*     		pCurrentNode  			= NULL;
		_Bool               	bFound          		= 0;
		void*					pvNodeData				= NULL;
		unsigned int      		unTempAcctRecordNum   	= 0;
		t_ACRInfo*  	        poAccountingRecord      = NULL;

		pCurrentNode = (DMBase_SList*)(poSessionObject_i->pListOfUnprocessedACRInfo);
            
		while( NULL != pCurrentNode )
		{
                       
			if( DMBase_Retval_Success != DMBase_SListGetNodeData( hRFModuleContext_i ->hDiameterStackContext,
																  pCurrentNode , &pvNodeData) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_SListGetNodeData fails , Leaving \n");
				return Offline_CS_RetVal_Failure;
			}
                          
			poAccountingRecord = (t_ACRInfo*)pvNodeData;
			unTempAcctRecordNum = poAccountingRecord->unAcctRecordNumber;
			if( unTempAcctRecordNum == unAcctRecordNumber_i ) 
			{
                 bFound = 1;
                *ppoListNodeFound_o = pCurrentNode;
				break;
			}
			DMBase_SListGetNextNode( hRFModuleContext_i->hDiameterStackContext,
									 pCurrentNode ,&pCurrentNode);
	   }// end while

        *pbRecordFound_o = bFound;
	   

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO, " Leaving.\n");
	return Offline_CS_RetVal_Success;
}

/******************************************************************************
** Function Name: 
**			Rf_DeleteSessionTable
**  
** Description:
**		This function appends node to the link list.
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
**
**
** Return:
**		An t_Offline_CS_ReturnVal value specify the rerurn status of function. 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_DeleteSessionTable(HRFAPPLICATIONCONTEXT             hRFModuleContext_i)
{

     DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering\n");

     if(NULL == hRFModuleContext_i )
     {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
        return Offline_CS_RetVal_Null_Pointer;
     }

     if( NULL == hRFModuleContext_i->HandleToSessionTable )
     {
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
         return Offline_CS_RetVal_Null_Pointer;

     }

    if( DMBase_Retval_Success != DMBase_HashTableDelete(hRFModuleContext_i->hDiameterStackContext,
                                                        hRFModuleContext_i->HandleToSessionTable ) )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DMBase_HashTableDelete failed , Leaving \n");
        return Offline_CS_RetVal_Failure;
    }


     DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
     return Offline_CS_RetVal_Success ; 


}
/******************************************************************************
** Function Name: 
**			Rf_DeleteACRInfoFromSessionTable
**  
** Description:
**		This function appends node to the link list.
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
**
**
** Return:
**		An t_DiamAPIReturnVal value specify whether data appended to list    
**		successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal Rf_DeleteACRInfoFromSessionTable( HRFAPPLICATIONCONTEXT              hRFModuleContext_i,
														 t_SessionObject*					poSessionObject_i,
                                                         DMBASE_HASHTABLE_KEY_HANDLE        hSessionTableKey_i,
                                                         unsigned int                       unSessionRecodNumber_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

        _Bool               bRecordFound            = 0;
        t_SessionObject*    poSessionObject    	    = NULL;
        t_ACRInfo*		    poACRInfo               = NULL;
        DMBase_SList*       pListNodeToDelete       = NULL;

        if(NULL == hRFModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hRFModuleContext_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL == hSessionTableKey_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter hSessionTableKey_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		if(NULL == poSessionObject_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter poSessionObject_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

      /*  if( Offline_CS_RetVal_Success != Rf_SearchObjectInSessionTable( hRFModuleContext_i,
                                                                        hSessionTableKey_i,
                                                                        &poSessionObject_i ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Rf_SearchObjectInSessionTable() Fail , Leaving \n");
            return Offline_CS_RetVal_Failure;
        }
		*/

        if( NULL != poSessionObject)
        {
                if( Offline_CS_RetVal_Success != Rf_SearchACRInfoNodeInSessionObject(hRFModuleContext_i,
                                                                                     poSessionObject_i,
                                                                                     unSessionRecodNumber_i,
                                                                                     &bRecordFound, 
                                                                                     &pListNodeToDelete ) )
                {
                    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Rf_SearchACRInfoInSessionObject() Fail , Leaving \n");
                    return Offline_CS_RetVal_Failure;
                }
                
        }

        if( 1== bRecordFound)
        {
			if( DMBase_Retval_Success !=  DMBase_SListRemove( hRFModuleContext_i->hDiameterStackContext,
															  &(poSessionObject_i->pListOfUnprocessedACRInfo)  , 
															  pListNodeToDelete, Rf_LListFreeLinkDataCallback) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in removing ACRInfo Node From List , Leaving \n");
				return Offline_CS_RetVal_Failure;
			}
                
            /* printf("\n\n\n\n\t\t*_*_*_*_*_ Rf_Debug_PrintSessionTable  After Deletion *_*_*_*_*\n\n\n");
             Rf_Debug_PrintSessionTable(hRFModuleContext_i);
			 */
        }
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return Offline_CS_RetVal_Success;
}
/******************************************************************************
** Function Name: 
**			Rf_LListFreeLinkDataCallback
**  
** Description:
**		This function appends node to the link list.
**
** Parameters:
**		  void**    ppvDataToFree
**			 type: input
**			 detail : this is a pointer to pointer , that that needs to be free. 
**       
** Return:
**		Its return type is integer specifying the return status of the function.
**
** 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
int Rf_LListFreeLinkDataCallback( void**    ppvDataToFree)
{
    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");


    if(NULL == ppvDataToFree)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter ppvDataToFree , Leaving \n");
        return Offline_CS_RetVal_Null_Pointer;
    }

    if(NULL == *ppvDataToFree)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter *ppvDataToFree , Leaving \n");
        return Offline_CS_RetVal_Null_Pointer;
    }

    t_ACRInfo*  poACRInfo  = (t_ACRInfo*)*ppvDataToFree;
    
	DMBase_Free(NULL,(void*)&poACRInfo);
    
    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
    return Offline_CS_RetVal_Success;
}
/******************************************************************************
** Function Name: 
**			Rf_DestroyACRInfo
**  
** Description:
**		This function appends node to the link list.
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
**
**
** Return:
**		An t_DiamAPIReturnVal value specify whether data appended to list    
**		successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: 	Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal Rf_DestroyACRInfo(t_ACRInfo** ppoACRInfo_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

        if(NULL == ppoACRInfo_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter ppoACRInfo_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }
        
        if(NULL == *ppoACRInfo_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter *ppoACRInfo_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }
       
/*
        if( NULL == ((*ppoACRInfo_i)->pOSessionID) )
        {
            printf("********************************Here \n\n\n");
            if ( NULL != (*ppoACRInfo_i)->pOSessionID->pucOctetString )
            {
                free((*ppoACRInfo_i)->pOSessionID->pucOctetString);
                (*ppoACRInfo_i)->pOSessionID->pucOctetString = NULL;
            }
            free((*ppoACRInfo_i)->pOSessionID );
            (*ppoACRInfo_i)->pOSessionID = NULL;
        }

         
        if( NULL != (*ppoACRInfo_i)->pOAcctRecordNumber )
        {
            if ( NULL != (*ppoACRInfo_i)->pOAcctRecordNumber->punUnsigned32 )
            {
                free((*ppoACRInfo_i)->pOAcctRecordNumber->punUnsigned32);
                (*ppoACRInfo_i)->pOAcctRecordNumber->punUnsigned32 = NULL;
             }
            free((*ppoACRInfo_i)->pOAcctRecordNumber );
            (*ppoACRInfo_i)->pOAcctRecordNumber = NULL;
        }

        if( NULL != (*ppoACRInfo_i)->pOAcctRecordType )
        {
            if ( NULL != (*ppoACRInfo_i)->pOAcctRecordType->pnInteger32 )
            {
                free((*ppoACRInfo_i)->pOAcctRecordType->pnInteger32);
                (*ppoACRInfo_i)->pOAcctRecordType->pnInteger32 = NULL;
            }
            free((*ppoACRInfo_i)->pOAcctRecordType );
            (*ppoACRInfo_i)->pOAcctRecordType = NULL;
        }

        if( NULL != (*ppoACRInfo_i)->pOAcctRecordType )
        {
            if ( NULL != (*ppoACRInfo_i)->pOAcctRecordType->pnInteger32 )
            {
                free((*ppoACRInfo_i)->pOAcctRecordType->pnInteger32);
                (*ppoACRInfo_i)->pOAcctRecordType->pnInteger32 = NULL;
            }
            free((*ppoACRInfo_i)->pOAcctRecordType );
            (*ppoACRInfo_i)->pOAcctRecordType = NULL;
        }

        free(*ppoACRInfo_i);
        *ppoACRInfo_i = NULL;*/

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return Offline_CS_RetVal_Success;
}
/******************************************************************************
** Function Name: 
**			DMBASE_EvtSysSendExtEvt
**  
** Description:
**		This is a event triggering function .
**
** Parameters:
**		  AMPS_HANDLE     hAMPSHandle_i
**			 type: input
**			 detail : this is a handle to AMPS framework. 
**
**		 int             nEventType_i,
**			type: input
**			detail: this is a integer variable contains the type of Event.
**
**		 void*           pvEventData_i
**			type: input
**			detail: this is a pointer to data which the Aplication Wants
**                  to send through the event.
**				
**      unsigned int    unSizeofEventData_i
**          Type: input
**          detail: this is size of the the Data that the application is 
**                  sending through the events.
** Return:
**		An t_DiamAPIReturnVal value specify whether the event has been fired    
**		successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: Simab Shahid 
*******************************************************************************
*******************************************************************************
*/
// Base Protocol Wrapper Function
t_DiamAPIReturnVal DMBASE_EvtSysSendExtEvt(HDIAMETERSTACKCONTEXT    hDiameterContext_i, 
                                           int             			nEventType_i,
                                           void*           			pvEventData_i, 
                                           unsigned int    			unSizeofEventData_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

        if( (NULL == pvEventData_i)&&(unSizeofEventData_i >0) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Either unSizeofEventData_i or pvEventData_i is invalid parameter, Leaving \n");
            return DMBase_Retval_IncorrectParameters ;
        }

        if(AMPS_SUCCESS != AMPS_EvtSysSendExtEvt(hDiameterContext_i->hFrameworkHandle, 
                                                 nEventType_i, 
                                                 pvEventData_i, 
                                                 unSizeofEventData_i))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"EvtSysSendExtEvt Fails, Leaving \n");
            return DMBase_Retval_Failure ;
        }

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return DMBase_Retval_Success ;
}


/******************************************************************************
** Function Name: 
**			Rf_SetTimerIDInSessionObject
**  
** Description:
**		This function is setting  timerId of newly set timer()
**      in session Object, if the timer is allready set in session object, then it
**      just kill the Previous  Timer, remove it from session object 
**		and set new timer ID in Session Object        
**
** Parameters:
**        HRFAPPLICATIONCONTEXT         hRFApplicationContext
**			 type: input
**			 detail : this is a RF Application context handle. 
**
**
**		  DMBASE_HASHTABLE_KEY_HANDLE      hSessionTableKey_i
**			 type: input
**			 detail : this is a Handle to Session Table Key.
**
**
**        AMPS_TIMER_ID                    TimerID_i 
**          type: input
**			 detail : this is a ID of New Timer.
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing  wether the timerID has been set successfull
**      or some problem occurs .
**
** 
*******************************************************************************
** Created By: Simab Shahid	 
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal Rf_SetTimerIDInSessionObject(HRFAPPLICATIONCONTEXT            hRFModuleContext_i,
                                                    t_SessionObject*      			 poSessionObject_i,
                                                    AMPS_TIMER_ID                    TimerID_i,
													t_TimerDataObject* 				 poTimerData_i )
{

    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

    if(NULL == hRFModuleContext_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hRFModuleContext_i is NULL , leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }

	 if(NULL == poTimerData_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poTimerData_i is NULL , leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }

    
    if(NULL == TimerID_i)
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"TimerID_i is NULL , leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
    }

   if(NULL == poSessionObject_i )
   {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poSessionObject_i is NULL , leaving\n");
        return Offline_CS_RetVal_Null_Pointer;
   }

    // if timer id is allready set in Sesion Object 
    // then first delete the Timer then set new timer ID 

	if( NULL != poSessionObject_i->poTimerDataForCurrSess)
	{
		DMBase_Free( hRFModuleContext_i->hDiameterStackContext,
					 (void**)&(poSessionObject_i->poTimerDataForCurrSess) );
	}
  
    poSessionObject_i->TimerID = TimerID_i;
	poSessionObject_i->poTimerDataForCurrSess = poTimerData_i; 

    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
    return Offline_CS_RetVal_Success ; 
}






