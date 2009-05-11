
#include "RF_defines.h"
#include "RF_include.h"
#include "DBModule.h"
#include"cdrfw_file_writer.h"
#include "CdrFileWriterModule.h"
#include "dappcommon_srvrconfload.h"
#include "offcs_com_cdr_contents.h"
#include "offcs_app_defined_avp.h"


extern int  g_nDiamBaseTraceID;


void* CdrFW_ThreadInit(AMPS_HANDLE 		hAMPSHandle_i,
					   void* 			hUIOAgentsHandle_i, 
					   void* 			pvUserData_i)
   {
	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	   //printf("Entering  CdrFW_ThreadInit() \n");

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
       unsigned int unFileRotationType = 0;
	   HCDRFILEWRITERTHREADCONTEXT hCDRFileWriterThreadCtxt = NULL;
	   HCDRFILEWRITERMODULECONTEXT	hMainCDRModContext = (HCDRFILEWRITERMODULECONTEXT) pvUserData_i;


		if ( DMBase_Retval_Success != DMBase_Malloc(hMainCDRModContext->hDiameterStackContext,
										(void**)&hCDRFileWriterThreadCtxt,sizeof(t_CdrFileWriterThreadContext))
			)
	
	
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," DMBase_Malloc failed for current thread ,Leaving \n");
			return NULL;
		}

		
		hCDRFileWriterThreadCtxt->hMainCDRModContext = hMainCDRModContext;
		hCDRFileWriterThreadCtxt->hUnitIOAgent = hUIOAgentsHandle_i;

        //printf("\n\n Inside Cdr file Writer \n\n");
		
        // create cdr file 
		if( Offline_CS_RetVal_Success != CdrFW_CreateNewCdrFile(hCDRFileWriterThreadCtxt ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating New CdrFile ,Leaving\n");
			return NULL;
		}

		unFileRotationType = hCDRFileWriterThreadCtxt->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.unFileRotationType;
		//printf(" /n File rotation Type is = %d \n",unFileRotationType);
		if( OFFLINE_CS_TIME_BASED_ROTATION == unFileRotationType )
		{
			//set timer to notify about the Expiry of cdr File Rotation Time , if Timebased File Rotation is Active
            DMBASE_TIMERID 	TimerID = NULL;
			unsigned int 	unExpiryTime = ( hCDRFileWriterThreadCtxt->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.unRotationTimerValueInMinutes * 60000);
			if( DMBase_Retval_Success!= DMBase_StartTimer( hMainCDRModContext->hDiameterStackContext,
														   unExpiryTime,
														   CdrFW_FileRotationNotifyHandler, 
														   (void*)hCDRFileWriterThreadCtxt,
														   &TimerID))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Setting timer for Cdr File Rotation, Leaving\n");
				return NULL;
			}

		}
		else if (OFFLINE_CS_NUM_OF_CDRS_BASED_ROTATION == unFileRotationType )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"CDRs Based Rotation type is set ,Leaving\n");
		}
		else
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Invalid Rotation type is set ,Leaving\n");
			return NULL;
		}

	    //printf("Leaving  CdrFW_ThreadInit() \n");
	
	    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
        return hCDRFileWriterThreadCtxt ;// it will return the pointer to created data
   }

void CdrFW_ThreadCleanup(void* pvCdrFWriterThreadCtxt_i)
{

   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

   if ( NULL == pvCdrFWriterThreadCtxt_i )
   {
	   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvCdrFWriterThreadCtxt_i  is NULL , leaving\n");
	   return;
   }

   HCDRFILEWRITERTHREADCONTEXT  hCdrFWriterThreadContext = (HCDRFILEWRITERTHREADCONTEXT) pvCdrFWriterThreadCtxt_i; 

   if ( NULL != hCdrFWriterThreadContext->oCdrFileObject.pFileHandle )
	  {
		  fclose( hCdrFWriterThreadContext->oCdrFileObject.pFileHandle);
	  }


      
	  DMBase_Free(  hCdrFWriterThreadContext->hMainCDRModContext->hDiameterStackContext ,
					(void*)&(hCdrFWriterThreadContext->oCdrFileObject.pucFileName) );



   HDIAMETERSTACKCONTEXT	hDiamStackContext = NULL;
   hDiamStackContext = ((HCDRFILEWRITERTHREADCONTEXT) pvCdrFWriterThreadCtxt_i)->hMainCDRModContext->hDiameterStackContext;

   DMBase_Free(hDiamStackContext,&pvCdrFWriterThreadCtxt_i);

   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
}


/******************************************************************************
** Function Name: 
**			CdrFWriter_EventValidationCallback
**  
** Description:
**		This function is a callback to validate the data that the cdr-FileWriter Module 
**      is going to send through any EVENT.
**
** Parameters:
**		  AMPS_APP_HANDLE    r_hAMPS_APP_HANDLE
**		  type: input
**			 detail : this is a handle to AMPS framework. 
**
**		 int                r_nEventID
**			type: input
**			detail: this is an integer conatins the ID of the Event for which 
**                  the data is being validated.
**				
**
**		 void*              r_pvEventPayload
**          type: input
**			detail: this is void pointer that points to the data for validation.
**
**
** Return:
**		Return  value specifying the success or failure of the function.
**
** 
*******************************************************************************
** Created By: 		: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
int	CdrFW_EventValidationCallback( AMPS_APP_HANDLE 		r_hAMPS_APP_HANDLE,
										int 					r_nEventID,
										void* 					r_pvEventPayload)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	
		return AMPS_SUCCESS;
}

/******************************************************************************
** Function Name: 
**			CdrFileWriterModule_ModuleInit
**  
** Description:
**		This is to initialize the CdrFileWiterModule.
**
** Parameters:
**		  AMPS_HANDLE     hAMPS_HANDLE_i
**			 type: input
**			 detail : this is a diameter context handle. 
**
**		 void*           pvOfflineChargingServerLocalContext_i
**			type: input
**			detail: This is a void pointer that points to the 
**                  OfflineChargingServer LocalContext.
**
** Return:
**		The return type is a void pointer.
**
** 
*******************************************************************************
** Created By: 		: Simab Shahid
*******************************************************************************
*******************************************************************************
*/                                                
AMPS_MODULE_HANDLE	CdrFileWriterModule_ModuleInit( AMPS_HANDLE     hAMPS_HANDLE_i,
												    void*           pvModuleinitParam_i)
{			

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");


		DMBASE_IOAGENTS_HANDLE 			   hIOAgentsHandle = NULL;			
		HCDRFILEWRITERMODULECONTEXT        hCdrFWriterModuleContext = NULL;

        if( NULL == hAMPS_HANDLE_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hAMPS_HANDLE is NULL pointer, Leaving\n");
            return ((void*)NULL);
        }

        if(NULL == pvModuleinitParam_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvModuleinitParam_i is NULL pointer, Leaving\n");
            return ((void*)NULL);
        }

		t_cdf_config_params*	poCDF_ConfParams = (t_cdf_config_params*)pvModuleinitParam_i;

        HDIAMETERSTACKCONTEXT    		hDiameterStackContext  = poCDF_ConfParams->hDiameterStackContext;
		t_cdf_CdrFile_config_params		ocdf_CdrFile_config_params = poCDF_ConfParams->ocdf_CdrFile_config_params;

		//printf("\n\n file rotation type = %d \n\n",ocdf_CdrFile_config_params.unFileRotationType);

        //Create RFApplication Context
        if(Offline_CS_RetVal_Success != CdrFW_CreateAndInitModuleContext( &hCdrFWriterModuleContext, hDiameterStackContext ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in creating CdrFileWriterModule Context, Leaving\n");
            return ((void*)NULL);
        }

		hCdrFWriterModuleContext->ocdf_config_params = *poCDF_ConfParams;
		if(DMBase_Retval_Success !=  DMBase_IOAgentsInit(hCdrFWriterModuleContext->hDiameterStackContext,
														 OFFLINE_CS_NO_OF_CDR_FILE_WRITER_THREAD, 
														 NULL,
														 CdrFW_ThreadInit,
														 CdrFW_ThreadCleanup, 
														 (void*)(hCdrFWriterModuleContext),
														 &hIOAgentsHandle) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_IOAgentsInit Fails, Leaving\n");
            return ((void*)NULL);
		}
		
		hCdrFWriterModuleContext->hIOAgentsHandle = hIOAgentsHandle;
        
        
        if(Offline_CS_RetVal_Success != CdrFW_RegisterForModuleEvents(hCdrFWriterModuleContext) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Registering for CdrFilewriter Module internal Events, Leaving\n");
            return ((void*)NULL);
        }

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return ((void*)hCdrFWriterModuleContext);
}

/******************************************************************************
** Function Name: 
**			CdrFW_CreateAndInitModuleContext
**  
** Description:
**		This function is creating (CdrFWriter Module Context by allocating memeory to 
**      Handle to CdrFWriter module Context)  and intializating it .
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o
**			 type: output
**			 detail : This is a pointer to CdrFWModuleContext Handle . 
**
**       HDIAMETERSTACKCONTEXT    hDiameterContext_i
**			 type: input
**			 detail : this is handle to Diameter Base Protocol Stacke Context. 
**
**       
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function.
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal CdrFW_CreateAndInitModuleContext( HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o,
														 HDIAMETERSTACKCONTEXT    		     hDiameterStackContext_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

        if(	NULL == phCdrFWriterModuleContext_o	)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"phCdrFWriterModuleContext_o is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(	NULL != *phCdrFWriterModuleContext_o )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"*phCdrFWriterModuleContext_o is Invalid , leaving\n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

		if(	NULL == hDiameterStackContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDiameterStackContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }


        unsigned int unSize   = sizeof(t_CdrFileWriterModuleContext);
        if(DMBase_Retval_Success != DMBase_Malloc(NULL, (void **)phCdrFWriterModuleContext_o,unSize))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Memory Allocation to phCdrFWriterModuleContext_o\n");
            return Offline_CS_RetVal_Failure;
        }

		(*phCdrFWriterModuleContext_o)->hDiameterStackContext = hDiameterStackContext_i;
		
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
        return Offline_CS_RetVal_Success;
 }

t_DiamAPIReturnVal  DMBase_FWrite(  HDIAMETERSTACKCONTEXT    	hDiameterContext_i,
									void* 						pvSource_o,
									unsigned int 				unSizeOfBlock_i, 
									unsigned int 				unNumberOfBlocks_i,
									FILE* 						pfile_i,
									unsigned int*				punNumberOfBlocksWritten_o )
{
		 DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

      
	  if( NULL == pvSource_o)
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvSource_o is null pointer, leaving \n");
		  return DMBase_Retval_Null_Pointer;
	  }

	  if( NULL == punNumberOfBlocksWritten_o)
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"punNumberOfBlocksWritten_o is null pointer, leaving\n");
		  return DMBase_Retval_Null_Pointer;
	  }

	  *punNumberOfBlocksWritten_o = fwrite(pvSource_o, unSizeOfBlock_i, unNumberOfBlocks_i,pfile_i);

	  if( 0 == *punNumberOfBlocksWritten_o )
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"File Write Problem , ZERO bytes written, leaving\n");
		  return DMBase_Retval_Failure;

	  }

	  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
      return DMBase_Retval_Success;

 }

t_DiamAPIReturnVal  DMBase_FRead( HDIAMETERSTACKCONTEXT    	hDiameterContext_i,
								  void* 					pvDestination_o,
								  unsigned int 				unSizeOfBlock_i, 
								  unsigned int 				unNumberOfBlocks_i,
								  FILE* 					pfile_i,
								  unsigned int*				punNumberOfBlocksRead_o)
  {
	  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

      
	  if( NULL == pvDestination_o)
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvDestination_o is null pointer, leaving \n");
		  return DMBase_Retval_Null_Pointer;
	  }

	  if( NULL == punNumberOfBlocksRead_o)
	  {
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvDestination_o is null pointer, leaving\n");
		  return DMBase_Retval_Null_Pointer;
	  }

	  *punNumberOfBlocksRead_o = fread(pvDestination_o, unSizeOfBlock_i, unNumberOfBlocks_i,pfile_i);

	  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
      return DMBase_Retval_Success;
  }

/******************************************************************************
** Function Name: 
**			CdrFW_DestroyModuleContext
**  
** Description:
**		This function is creating (CdrFWriter Module Context by allocating memeory to 
**      Handle to CdrFWriter module Context)  and intializating it .
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o
**			 type: output
**			 detail : This is a pointer to CdrFWModuleContext Handle . 
**
**       HDIAMETERSTACKCONTEXT    hDiameterContext_i
**			 type: input
**			 detail : this is handle to Diameter Base Protocol Stacke Context. 
**
**       
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function.
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal CdrFW_DestroyModuleContext(HCDRFILEWRITERMODULECONTEXT        phCdrFWriterModuleContext_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		if( NULL == phCdrFWriterModuleContext_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"phCdrFWriterModuleContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
        
        DMBase_Free( NULL ,(void*)phCdrFWriterModuleContext_i );
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
}

/******************************************************************************
** Function Name: 
**			CdrFW_RegisterForModuleEvents
**  
** Description:
**		This function is creating (CdrFWriter Module Context by allocating memeory to 
**      Handle to CdrFWriter module Context)  and intializating it .
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o
**			 type: output
**			 detail : This is a pointer to CdrFWModuleContext Handle . 
**
**       HDIAMETERSTACKCONTEXT    hDiameterContext_i
**			 type: input
**			 detail : this is handle to Diameter Base Protocol Stacke Context.
**       
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function. 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal CdrFW_RegisterForModuleEvents( HCDRFILEWRITERMODULECONTEXT         hCdrFWriterModuleContext_i)
 {

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n"); 

        if(NULL == hCdrFWriterModuleContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCdrFWriterModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        
        //set event handle validation callback before Registration
        AMPS_EvtSysSetEvtHandleValidationCallback( hCdrFWriterModuleContext_i->hDiameterStackContext->hFrameworkHandle, 
                                                   CdrFW_EventValidationCallback);
        

		if (AMPS_SUCCESS != AMPS_IOAgentsRegEvt(hCdrFWriterModuleContext_i->hIOAgentsHandle, 
												OFFLINE_CS_EVENT_TYPE_IS_ENCODED_CDR_FILE_WRITE_REQUEST, 
												CdrFW_EventHandlerForLogCdrFromCGF))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," AMPS_IOAgentsRegEvt failed for SH_APP_REQUEST_EVENT \n");
			return Offline_CS_RetVal_Failure;
		}
		else
		{
			//printf("\n\n Inside File Writer module , Registration for OFFLINE_CS_EVENT_TYPE_IS_ENCODED_CDR_FILE_WRITE_REQUEST succeeded");
		}

	

        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return Offline_CS_RetVal_Success;
 }

/******************************************************************************
** Function Name: 
**			CdrFW_EventHandlerForLogCdrFromCGF
**  
** Description:
**		This function is handling the Request Form RF Mudlue to write the cdrs 
**		in file.
**
** Parameters:
**		  AMPS_HANDLE      hAMPS_HANDLE_i
**			 type: input
**			 detail : This is a handle to AMPS frame work.
**
**        DB_MODULE_LOCAL_CONTEXT_HANDLE        hModule_Context_Handle_i
**           type: input
**            detail :   This is Handle to CdrFWriter Module Context.
**
**
**		 void*         r_pvEventHandle_i
**			type: input
**			detail: this is a void pointer which points
**				
**
**		void*         pvDataForCdrFWriterModule_i
**         type: input
**			detail: This is a void pointer which points to the 
**				     data has been sent to CdrFWriter module
**
**
** Return:
**      The return type of this function is  void. 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
void  CdrFW_EventHandlerForLogCdrFromCGF(AMPS_HANDLE r_hAMPSHandle, void* r_pvEventData,
										 void* r_pvEventHandle, void* r_pvEventPayload)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		//printf("\n\n\n\n CdrFWritr: Event recieved From CGF \n\n\n\n ");
        
        if( NULL == r_hAMPSHandle )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_hAMPSHandle is NULL \n");
            return;
        }

        if( NULL == r_pvEventData )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_pvEventData is NULL \n");
            return;
        }


        if( NULL == r_pvEventHandle )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_pvEventHandle is NULL \n");
            return;
        }


        if( NULL == r_pvEventPayload )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_pvEventPayload is NULL \n");
            return;
        }

		HCDRFILEWRITERTHREADCONTEXT hCDRFileWriterThreadCtxt = (HCDRFILEWRITERTHREADCONTEXT)  r_pvEventData;

	   HCDRFILEWRITERMODULECONTEXT	hCdrFWriterModuleContext = hCDRFileWriterThreadCtxt->hMainCDRModContext;

		int         					nResultCode       = 0;
		t_DataFromCGFToFileWriter* 		poDataFromCGFToFileWriter = NULL;
		
		poDataFromCGFToFileWriter	= (t_DataFromCGFToFileWriter*)r_pvEventPayload;

		hCDRFileWriterThreadCtxt->unEventsRecvdFromCGFCounter++;
		//printf("\n Hello\n");
		//printf("\n\n\n Number of Events received in File Witer Module form CGF = %d \n\n\n", hCDRFileWriterThreadCtxt->unEventsRecvdFromCGFCounter);
        
       /* if(Offline_CS_RetVal_Success != CdrFW_WriteCdrToFile( hCDRFileWriterThreadCtxt,
															  poDataFromCGFToFileWriter->oEncodedCDR ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Writiting CDR to File ,Leaving \n");
			nResultCode  = DIAMETER_OUT_OF_SPACE;
		}
		else
		{
			nResultCode  = DIAMETER_SUCCESS;
		}*/
		
		nResultCode  = DIAMETER_SUCCESS;
		
		t_ResponseEvtDataFromCdrFWModule   oResponseDataToRFModule;
		oResponseDataToRFModule.unAcctRecordNumber = poDataFromCGFToFileWriter->unAcctRecordNumber;
        oResponseDataToRFModule.unResultCode = nResultCode;
		strcpy(oResponseDataToRFModule.ucSessionId, poDataFromCGFToFileWriter->ucSessionId);

		if ( AMPS_SUCCESS != AMPS_IOAgentsSendEvt( hCDRFileWriterThreadCtxt->hUnitIOAgent,
					OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE, (void* )&(oResponseDataToRFModule),
															  sizeof(t_ResponseEvtDataFromCdrFWModule ))
		)
		{
			
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," AMPS_IOAgentsSendEvt FAILED for OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE \n");
			//printf("\n\n\n\n\n AMPS_IOAgentsSendEvt FAILED for OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE \n\n\n");
			return;
	
		}
		
		hCDRFileWriterThreadCtxt->unEventsSendToCGFCounter++;
		//printf("\n\n\n Number of Events Send From FileWriter Module to CGF = %d \n\n\n", hCDRFileWriterThreadCtxt->unEventsSendToCGFCounter);
		
       //Send Event to RFModule to inform about the status of Cdr File Writing Operation
		/*
		if( DMBase_Retval_Success != DMBASE_EvtSysSendIntEvt( hCdrFWriterModuleContext->hDiameterStackContext,
															  OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE,
															  (void* )&(oResponseDataToRFModule),
															  sizeof(t_ResponseEvtDataFromCdrFWModule) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending Operation Result Event to CGF Module\n");
			return Offline_CS_RetVal_Failure;
		}
		*/

       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	   return ; 
  }

/******************************************************************************
** Function Name: 
**			CdrFW_GenerateResponseToCGF
**  
** Description:
**		This function is generating response to CGF Mudlue to inform 
**      about the status of opeation of cdr writing in file.
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT    phCdrFWriterModuleContext_i
**			 type: input
**			 detail : This is a handle to Cdr File Writer Module Context.
**
**        unsigned int		unAcctRecordNumber_i
**           type: input
**            detail :   This parameter contains the number of Cdr to be
**						 write in data base.
**
**
**		 unsigned char*		pucSessionId_i
**			type: input
**			detail: this is a pointer to session-Id to which this cdr belongs.
**				
**
**		unsigned int			unResultCode_i
**         type: input
**			detail: This parameter contains the status value of the data base
**					operation.
**
** Return:
**      The return type t_Offline_CS_ReturnVal shows the success or failure of the 
**		function. 
*******************************************************************************
** Created By: 	Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal CdrFW_GenerateResponseToCGF( HCDRFILEWRITERMODULECONTEXT        	phCdrFWriterModuleContext_i,
													unsigned int					   	unAcctRecordNumber_i,
													unsigned char*					    pucSessionId_i,
													unsigned int						unResultCode_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if( NULL == phCdrFWriterModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"phCdrFWriterModuleContext_i is NULL \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		if( NULL == pucSessionId_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucSessionId_i is NULL \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		t_ResponseEvtDataFromCdrFWModule   oResponseDataToRFModule;

		oResponseDataToRFModule.unAcctRecordNumber = unAcctRecordNumber_i;
        oResponseDataToRFModule.unResultCode = unResultCode_i;
		strcpy(oResponseDataToRFModule.ucSessionId, pucSessionId_i);
		
		
       //Send Event to RFModule to inform about the status of Cdr File Writing Operation
		if( DMBase_Retval_Success != DMBASE_EvtSysSendIntEvt( phCdrFWriterModuleContext_i->hDiameterStackContext,
															  OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE,
															  (void* )&(oResponseDataToRFModule),
															  sizeof(t_ResponseEvtDataFromCdrFWModule) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending Operation Result Event to CGF Module\n");
			return Offline_CS_RetVal_Failure;
		}
		//printf("CdrFileWriter: Event Send to CGF Module \n");

		//Send Event to RFModule to inform about the status of Cdr File Writing Operation
		/*if( AMPS_SUCCESS != AMPS_IOAgentsSendEvt( phCdrFWriterModuleContext_i->hIOAgentsHandle,
												  OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE,
												  (void* )&(oResponseDataToRFModule),
												  sizeof(t_ResponseEvtDataFromCdrFWModule)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending Operation Result Event to Rf Module\n");
			return Offline_CS_RetVal_Failure;

		}*/
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success; 
}

/******************************************************************************
** Function Name: 
**			CdrFileWriterModule_ModuleCleanup
**  
** Description:
**		This function is a clean up function of CdrFWriterModule.
**		All the momory allocated to CdrFWriterModule is must to free here.
**
** Parameters:
**		  AMPS_HANDLE                 r_hAMPS_HANDLE
**			 type: input
**			 detail : this is handle to AMPS Frame work. 
**			
**		 AMPS_MODULE_HANDLE          r_hAMPS_MODULE_HANDLE
**			 type: input
**			 detail : this is handle to the CdrFWriterModule Context. 
** Return:
**		Its return value is void. 
*******************************************************************************
** Created By: 	Simab Shahid	
*******************************************************************************
*******************************************************************************
*/
void CdrFileWriterModule_ModuleCleanup( AMPS_HANDLE 				r_hAMPS_HANDLE,
									    AMPS_MODULE_HANDLE 		    r_hAMPS_MODULE_HANDLE)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if( NULL == r_hAMPS_MODULE_HANDLE )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_hAMPS_MODULE_HANDLE is NULL \n");
            return ;
        }

		HCDRFILEWRITERMODULECONTEXT      phCdrFWriterModuleContext = (HCDRFILEWRITERMODULECONTEXT)r_hAMPS_MODULE_HANDLE;

		if( Offline_CS_RetVal_Success != CdrFW_DestroyModuleContext(phCdrFWriterModuleContext))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in destroying Cdr File Writer Module Context \n");
            return ;
		}
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return ;
}


t_DiamAPIReturnVal DMBase_CreateFileObject( HDIAMETERSTACKCONTEXT    hDiameterContext_i,
										    char*					 pcFileName_i,
											void**					 ppvFileHadle_o,
											DMBase_FileMode			 FileMode_i )
{
		
		DMBASE_TRACE( g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
        
		if( NULL == hDiameterContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDiameterContext_i is NULL , leaving\n");
            return DMBase_Retval_Null_Pointer;
        }
		

		if(NULL == pcFileName_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pcFileName_i is NULL , leaving\n");
            return DMBase_Retval_Null_Pointer;
        }

		
		if(NULL == ppvFileHadle_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppvFileHadle_o is NULL , leaving\n");
            return DMBase_Retval_Null_Pointer;
        }

		if(NULL != *ppvFileHadle_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppvFileHadle_o is Incorrect , leaving\n");
            return DMBase_Retval_IncorrectParameters;
        }
	
		FILE*  pFileHandle = NULL;

		
		if( AMPS_SUCCESS != AMPS_CreateFileObject(	hDiameterContext_i->hFrameworkHandle, 
													pcFileName_i,
													(void**)&pFileHandle,
													FileMode_i ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating File object ,Leaving\n");
			return DMBase_Retval_Failure;
		}

		*ppvFileHadle_o = pFileHandle;
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return DMBase_Retval_Success;
 }


t_DiamAPIReturnVal DMBase_FileWrite( HDIAMETERSTACKCONTEXT    hDiameterContext_i,
                                     void*					  pvFileHadle_o,
									 char*					  pBuffTowrite_i,
									 int					  nSizeOfBuff_i,
									 int*					  pnNumBytesWritten_o )
{
		DMBASE_TRACE( g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		if( NULL == hDiameterContext_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDiameterContext_i is NULL , leaving\n");
			return DMBase_Retval_Null_Pointer;
		}
	
		if( NULL == pvFileHadle_o )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvFileHadle_o is NULL , leaving\n");
			return DMBase_Retval_Null_Pointer;
		}
	
		if( NULL == pBuffTowrite_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pBuffTowrite_i is NULL , leaving\n");
			return DMBase_Retval_Null_Pointer;
		}
	
		if( NULL == pnNumBytesWritten_o )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pnNumBytesWritten_o is NULL , leaving\n");
			return DMBase_Retval_Null_Pointer;
		}
	
		if( AMPS_SUCCESS != AMPS_FileWrite(hDiameterContext_i->hFrameworkHandle,
											pvFileHadle_o,pBuffTowrite_i,
											nSizeOfBuff_i,pnNumBytesWritten_o ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in writing to File ,Leaving\n");
			return DMBase_Retval_Failure;
		}
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return DMBase_Retval_Success;

}





