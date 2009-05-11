
#include "RF_defines.h"
#include "RF_include.h"
#include "DBModule.h"
#include"cdrfw_file_writer.h"
#include "CGF.h"
#include "dappcommon_srvrconfload.h"
#include "offcs_com_cdr_contents.h"
#include "offcs_app_defined_avp.h"
#include "IMSRecord.h"


extern int  g_nDiamBaseTraceID;

/******************************************************************************
** Function Name: 
**			CGF_EventValidationCallback
**  
** Description:
**		This function is a callback to validate the data that the CGF Module 
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
*******************************************************************************
** Created By: 		: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
int	CGF_EventValidationCallback( AMPS_APP_HANDLE 		r_hAMPS_APP_HANDLE,
								 int 					r_nEventID,
								 void* 					r_pvEventPayload)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	
		return AMPS_SUCCESS;
}

/******************************************************************************
** Function Name: 
**			CGF_ModuleInit
**  
** Description:
**		This is to initialize the CGF Module.
**
** Parameters:
**		  AMPS_HANDLE     hAMPS_HANDLE_i
**			 type: input
**			 detail : this is a diameter context handle. 
**
**		 void*           pvModuleinitParam_i
**			type: input
**			detail: This is a void pointer that points to instance of 
**                   structure of type t_cdf_config_params
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
AMPS_MODULE_HANDLE	CGF_ModuleInit( AMPS_HANDLE     hAMPS_HANDLE_i,
									void*           pvModuleinitParam_i)
{			

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

        HCGFMODULECONTEXT        hCGFModuleContext = NULL;

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

        HDIAMETERSTACKCONTEXT    hDiameterStackContext  = poCDF_ConfParams->hDiameterStackContext;

        //Create RFApplication Context
        if(Offline_CS_RetVal_Success != CGF_CreateAndInitModuleContext( &hCGFModuleContext, hDiameterStackContext ) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in creating hCGFModuleContext Context, Leaving\n");
            return ((void*)NULL);
        }

        // Be Registered for all CGF Module Events Here  inside CGF_RegisterForModuleEvents()       
        if(Offline_CS_RetVal_Success != CGF_RegisterForModuleEvents(hCGFModuleContext) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Registering for CGF Module Events, Leaving\n");
            return ((void*)NULL);
        }

		//printf(" Registered for Event \r\n");

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return ((void*)hCGFModuleContext);
}

/******************************************************************************
** Function Name: 
**			CGF_CreateAndInitModuleContext
**  
** Description:
**		This function is creating (CGF Module Context by allocating memeory to 
**      Handle to CGF module Context)  and intializating it .
**
** Parameters:
**		  HCGFMODULECONTEXT*        phCGFModuleContext_o
**			 type: output
**			 detail : This is a pointer to CGFModuleContext Handle . 
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
t_Offline_CS_ReturnVal CGF_CreateAndInitModuleContext( HCGFMODULECONTEXT*        phCGFModuleContext_o,
													   HDIAMETERSTACKCONTEXT     hDiameterStackContext_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

        if(NULL == phCGFModuleContext_o)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"phCGFModuleContext_o is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        if(NULL != *phCGFModuleContext_o )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"*phCGFModuleContext_o is Invalid , leaving\n");
            return Offline_CS_RetVal_Invalid_Pointer;
        }

		if(NULL == hDiameterStackContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDiameterStackContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }


        unsigned int unSize   = sizeof(t_CGFModuleContext);
        if(DMBase_Retval_Success != DMBase_Malloc(NULL, (void **)phCGFModuleContext_o,unSize))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in Memory Allocation to phCGFModuleContext_o\n");
            return Offline_CS_RetVal_Failure;
        }

		(*phCGFModuleContext_o)->hDiameterStackContext = hDiameterStackContext_i;
      
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
        return Offline_CS_RetVal_Success;
 }

/******************************************************************************
** Function Name: 
**			CGF_DestroyModuleContext
**  
** Description:
**		This function is destroying CGF Module Context by De-allocating memeory  
**      of CGF module Context  and all of its members .
**
** Parameters:
**		  HCGFMODULECONTEXT*        phCdrFWriterModuleContext_o
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
t_Offline_CS_ReturnVal CGF_DestroyModuleContext(HCGFMODULECONTEXT*        phCGFModuleContext_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		if( NULL == phCGFModuleContext_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"phCGFModuleContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
        
        DMBase_Free( NULL ,(void*)phCGFModuleContext_i );
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
}

/******************************************************************************
** Function Name: 
**			CGF_RegisterForModuleEvents
**  
** Description:
**		This function is registering CGFModule for for all events in which 
**      in which CGF Module is interested to receive.
**
** Parameters:
**		  HCGFMODULECONTEXT*        hCGFModuleContext_i
**			 type: output
**			 detail : This is a Handle to hCGFModuleContext  . 
**              
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function. 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
t_Offline_CS_ReturnVal CGF_RegisterForModuleEvents( HCGFMODULECONTEXT         hCGFModuleContext_i)
 {

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n"); 

        if( NULL == hCGFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

        
        //set event handle validation callback before Registration
        AMPS_EvtSysSetEvtHandleValidationCallback( hCGFModuleContext_i->hDiameterStackContext->hFrameworkHandle, 
                                                   CGF_EventValidationCallback );


        if ( AMPS_SUCCESS != AMPS_EvtSysRegExtEvt(hCGFModuleContext_i->hDiameterStackContext->hFrameworkHandle,
												  OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_REQUEST_TO_CGF,
												  CGF_EvtHandlerForRFModule,
												  (void*)hCGFModuleContext_i))
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," AMPS_EvtSysRegExtEvt Failed for  OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_REQUEST_TO_CGF , leaving \n");
				return Offline_CS_RetVal_Failure;
		}


		if ( AMPS_SUCCESS != AMPS_EvtSysRegExtEvt(hCGFModuleContext_i->hDiameterStackContext->hFrameworkHandle,
												  OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE,
												  CGF_EvtHandlerForCdrFWriter,
												  (void*)hCGFModuleContext_i))
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," AMPS_EvtSysRegExtEvt Failed for  OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE , leaving \n");
				return Offline_CS_RetVal_Failure;
		}

		if ( AMPS_SUCCESS != AMPS_EvtSysRegExtEvt( hCGFModuleContext_i->hDiameterStackContext->hFrameworkHandle,
												   OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE,
												   CGF_EvtHandlerForDBModule,
												   (void*)hCGFModuleContext_i))
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," AMPS_EvtSysRegExtEvt Failed for  OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE , leaving \n");
				return Offline_CS_RetVal_Failure;
		}

        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return Offline_CS_RetVal_Success;
 }

/******************************************************************************
** Function Name: 
**			CGF_EvtHandlerForCdrFWriter
**  
** Description:
**		This function is  handler to  response event gentreted  by the 
**      Cdr File Writer Module to inform about the status of Cdrs writing 
**		operation in Cdr file .
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
void  CGF_EvtHandlerForCdrFWriter( AMPS_HANDLE 		    	hAMPS_HANDLE_i,
								   AMPS_APP_HANDLE 	    	hAMPS_APP_HANDLE_i, 
								   void* 			    	pvEventHandle_i,
								   void* 			    	pvEvtData_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");
		//printf("\n Event Received from File Witer Module in CGF \n");

		//printf("\n\n\n\n HELLO WORLD !!! CGF: Event Recieved from Cdr File Writer Module \n\n\n\n\n ");
        
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

		HCGFMODULECONTEXT    hCGFModuleContext    = NULL;
		
        hCGFModuleContext    = (HCGFMODULECONTEXT)hAMPS_APP_HANDLE_i;

        t_ResponseEvtDataFromCdrFWModule*   poResponseDataToRFModule;	

		poResponseDataToRFModule = (t_ResponseEvtDataForRFModule*)pvEvtData_i;

		
		hCGFModuleContext->unRecvdfrmFileWriterModuleEventCounter++;
		//printf(" \n\n\n Event Received in CGF from  File Writer Module  = %d \n\n\n ", hCGFModuleContext->unRecvdfrmFileWriterModuleEventCounter);

		if( AMPS_SUCCESS != AMPS_EvtSysSendExtEvt( hCGFModuleContext->hDiameterStackContext->hFrameworkHandle,
												   OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_RESPONSE_FROM_CGF,
												   (void* )poResponseDataToRFModule,
												   sizeof(t_ResponseEvtDataForRFModule) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending Operation Result Event to Rf Module\n");
			return ;
		}
		else
		{
			//printf(
		}

		
		hCGFModuleContext->unSendEventToRFCounter++;
		//printf(" \n\n\n Event Send from CGF to RF Module  = %d \n\n\n ", hCGFModuleContext->unSendEventToRFCounter );
		//printf(" CGF: Event send to RF Module \n");

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return;

}

void  CGF_EvtHandlerForDBModule( AMPS_HANDLE 		    	hAMPS_HANDLE_i,
								 AMPS_APP_HANDLE 	    	hAMPS_APP_HANDLE_i, 
								 void* 			    		pvEventHandle_i,
								 void* 			    		pvEvtData_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		//printf("\n\n\n\n HELLO WORLD !!! CGF: Event Recieved from DB Module \n\n\n\n\n ");
        
		if( NULL == hAMPS_HANDLE_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hAMPS_HANDLE_i is Null,Leaving\n");
            return;
        }

        if( NULL == pvEvtData_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Null Pointer pvEvtData_i is received form DB Module,Leaving\n");
            return;
        }

		if( NULL == hAMPS_APP_HANDLE_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Null Pointer hAMPS_APP_HANDLE_i is received ,Leaving\n");
            return;
        }

		if( NULL == pvEvtData_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Null Pointer hAMPS_APP_HANDLE_i is received ,Leaving\n");
            return;
        }

		HCGFMODULECONTEXT    hCGFModuleContext    = NULL;
		
        hCGFModuleContext    = (HCGFMODULECONTEXT)hAMPS_APP_HANDLE_i;

        t_ResponseEvtDataFromCdrFWModule*   poResponseDataToRFModule;	

		poResponseDataToRFModule = (t_ResponseEvtDataForRFModule*)pvEvtData_i;

		if( AMPS_SUCCESS != AMPS_EvtSysSendExtEvt( hCGFModuleContext->hDiameterStackContext->hFrameworkHandle,
												   OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_RESPONSE_FROM_CGF,
												   (void* )poResponseDataToRFModule,
												   sizeof(t_ResponseEvtDataForRFModule) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending Operation Result Event to Rf Module\n");
			return ;
		}
		//printf(" CGF: Response Event of DB write send to RF Module \n");

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return;

}

/******************************************************************************
** Function Name: 
**			CGF_EvtHandlerForRFModule
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
void  CGF_EvtHandlerForRFModule( AMPS_HANDLE                           hAMPS_HANDLE,
								 DB_MODULE_LOCAL_CONTEXT_HANDLE        hModule_Context_Handle_i, 
								 void*                                 r_pvEventHandle_i, 
								 void*                                 pvDataForCGFModule_i)
{

		//printf("\n\n\n\n\n Received Event in CGF from RF \n\n\n\n\n");
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_DEBUG,"CGF:Hello Event Received from RF Module\n");

        
    
        if(NULL == hModule_Context_Handle_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hModule_Context_Handle_i is NULL \n");
            return;
        }

        if(NULL == pvDataForCGFModule_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pvDataForCGFModule_i is NULL \n");
            return;
        }

		
        t_CDR_Contents*      poCDRContents	    ;
        HCGFMODULECONTEXT    hCGFModuleContext    = NULL;
		
		poCDRContents 	= (t_CDR_Contents*)pvDataForCGFModule_i;

		
		hCGFModuleContext    	= (HCGFMODULECONTEXT)hModule_Context_Handle_i;
		t_DataFromCGFToFileWriter oDataFromCGFToFileWriter,temp;
		t_CDR_Contents			oCDR_Contents = {0};
		

		hCGFModuleContext->unRecvdfrmRFEventCounter++;
		//printf(" \n\n\n Event Received in CGF Form RF Module  = %d \n\n\n ", hCGFModuleContext->unRecvdfrmRFEventCounter);

		if(DMBase_Retval_Success !=  DMBase_MemsetZero(&oDataFromCGFToFileWriter,sizeof(t_DataFromCGFToFileWriter)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oEncodedCDR with zeros, Leaving \n");
            return ;
		}

		if(DMBase_Retval_Success !=  DMBase_MemsetZero(&oCDR_Contents,sizeof(t_CDR_Contents)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDR_Contents with zeros, Leaving \n");
            return ;
		}
	
		// first of all send Cdr contents to DB Module before apply encoding Here

		oCDR_Contents = (t_CDR_Contents)*poCDRContents;

		//printf("\n\n\noCDR_Contents.nAcctRecordType = %d \n\n\n" ,oCDR_Contents.nAcctRecordType);


		//printf("\n\n\n  sizeof(t_CDR_Contents) = %d \n\n\n" ,sizeof(t_CDR_Contents));
		//printf("\n\n About to send event to DB Module \n\n ");

		if(DMBase_Retval_Success != DMBASE_EvtSysSendIntEvt(hCGFModuleContext->hDiameterStackContext, 
															OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_REQUEST, 
															(unsigned char*)&oCDR_Contents, 
															 sizeof(t_CDR_Contents)))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending event to DB Module, Leaving \n");
			return ;
		}
		else
		{
			//printf("\n\n\n\n\n Send OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_REQUEST to DB Module From CGF\n\n\n\n\n");
		}

		//printf("\n\n Event SENT to DB Module \n\n ");


	
       /* if( Offline_CS_RetVal_Success !=  CGF_CreateEncodedCDR( hCGFModuleContext,poCDRContents,&(oDataFromCGFToFileWriter.oEncodedCDR) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Encoding CDR ,Leaving \n");
			return;
		}
	*/
		oDataFromCGFToFileWriter.unAcctRecordNumber = poCDRContents->unAcctRecordNumber;
		strcpy(oDataFromCGFToFileWriter.ucSessionId,poCDRContents->ucSessionId);

		
		if(DMBase_Retval_Success != DMBASE_EvtSysSendIntEvt(hCGFModuleContext->hDiameterStackContext, 
															OFFLINE_CS_EVENT_TYPE_IS_ENCODED_CDR_FILE_WRITE_REQUEST, 
															(unsigned char*)&(oDataFromCGFToFileWriter), 
															sizeof(t_DataFromCGFToFileWriter)))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending event to CdrFilewriter Module, Leaving \n");
            return ;
        }
		else
		{
			//printf("\n\n\n\n\n Send OFFLINE_CS_EVENT_TYPE_IS_ENCODED_CDR_FILE_WRITE_REQUEST to FileWriter From CGF\n\n\n\n\n");
		}

		hCGFModuleContext->unSendEventToFileWriterModuleCounter++;
		//printf(" \n\n\n Event Send from CGF to File Writer Module  = %d \n\n\n ", hCGFModuleContext->unSendEventToFileWriterModuleCounter);

	
        /*

		t_ResponseEvtDataFromCdrFWModule   oResponseDataToRFModule;	

		oResponseDataToRFModule.unAcctRecordNumber = poCDRContents->unAcctRecordNumber;
		strcpy(oResponseDataToRFModule.ucSessionId,poCDRContents->ucSessionId);
		if( AMPS_SUCCESS != AMPS_EvtSysSendIntEvt( hCGFModuleContext->hDiameterStackContext->hFrameworkHandle,
												   OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_RESPONSE_FROM_CGF,
												   (void* )&oResponseDataToRFModule,
												   sizeof(t_ResponseEvtDataForRFModule) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending Operation Result Event to Rf Module\n");
			return ;
		}
		*/
	
		
        //printf("CGF:Event send to CDR File Writer Module \n");
		
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return ; 
  }



t_Offline_CS_ReturnVal inline CGF_CreateEncodedCDR( HCGFMODULECONTEXT 	hCGFModuleContext_i,
													t_CDR_Contents*		poCDRContents_i,
													t_EncodedCDR* 		poEncodedCDR_o)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		if(NULL == hCGFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL \n");
            return Offline_CS_RetVal_Null_Pointer;
		}
	
		if(NULL == poCDRContents_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL \n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		if( NULL == poEncodedCDR_o )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poEncodedCDR_o is NULL \n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
        
		char cErrorBuff[20] = {0};
        asn_enc_rval_t EncoderReturnVal = {0};       /* Encoder return value  */
        asn_dec_rval_t DecoderReturnVal = {0};		/* Decoder return value*/
		t_Offline_CS_ReturnVal oRetVal = Offline_CS_RetVal_Success;
		
		int nNodeFunctionality = 0;
		int nCdrHeaderSize = sizeof(t_CDRHeader);
		poEncodedCDR_o->oCdrHeader.oRelease_VersionId.VersionIdentifier 		= OFFLINE_CS_VERSION_IDETIFIER;
		poEncodedCDR_o->oCdrHeader.oRelease_VersionId.ReleasIdentifier  		= OFFLINE_CS_RELEASE_IDETIFIER;
		poEncodedCDR_o->oCdrHeader.oDataRecordFormat_TSNumber.DataRcordFormat 	= OFFLINE_CS_DATA_RECORD_FORMAT;
		poEncodedCDR_o->oCdrHeader.oDataRecordFormat_TSNumber.TSNumber		  	= OFFLINE_CS_DATA_TS_NUMBER;
	
        
		IMSRecord_t* pImsRecord = NULL;
	
        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) &pImsRecord,sizeof(IMSRecord_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			goto ExitMe;
		}

        nNodeFunctionality = poCDRContents_i->oServiceInfo.oIMSInformation.nNodeFunctionality;
        
		// here  make encoding then copying
	   if ( OFFLINE_CS_NODE_FUNCTIONALITY_S_CSCF == nNodeFunctionality )
	   {
				pImsRecord->present = IMSRecord_PR_sCSCFRecord ;
			    if( Offline_CS_RetVal_Success !=  CGF_CreateASN_SCSCFReocrd( hCGFModuleContext_i,poCDRContents_i,
																			&(pImsRecord->choice.sCSCFRecord) ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Filling SCSCF Structure,Leaving \n");
					oRetVal = Offline_CS_RetVal_Failure;
					goto ExitMe;
				}
		   
	   }
	   else if (  OFFLINE_CS_NODE_FUNCTIONALITY_P_CSCF == nNodeFunctionality )
	   {
		   pImsRecord->present = IMSRecord_PR_pCSCFRecord ;
		   if( Offline_CS_RetVal_Success !=  CGF_CreateASN_PCSCFReocrd( hCGFModuleContext_i,
																		poCDRContents_i,
																		&(pImsRecord->choice.pCSCFRecord) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Encoding PCSCF CDR Contents,Leaving \n");
				oRetVal = Offline_CS_RetVal_Failure;
				goto ExitMe;
			}
		   
	   }
	   else if (  OFFLINE_CS_NODE_FUNCTIONALITY_I_CSCF == nNodeFunctionality )
	   {
		   pImsRecord->present = IMSRecord_PR_iCSCFRecord ;
		   if( Offline_CS_RetVal_Success !=  CGF_CreateASN_ICSCFReocrd( hCGFModuleContext_i,
																		poCDRContents_i,
																		&(pImsRecord->choice.iCSCFRecord) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Encoding ICSCF CDR Contents,Leaving \n");
				oRetVal = Offline_CS_RetVal_Failure;
				goto ExitMe;
			}
	   }
	   else if (  OFFLINE_CS_NODE_FUNCTIONALITY_MRFC == nNodeFunctionality )
	   {
		   pImsRecord->present = IMSRecord_PR_mRFCRecord ;
		   if( Offline_CS_RetVal_Success !=  CGF_CreateASN_MRFCReocrd( hCGFModuleContext_i,
																	   poCDRContents_i,
																	   &(pImsRecord->choice.mRFCRecord) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Encoding MRFC CDR Contents,Leaving \n");
				oRetVal = Offline_CS_RetVal_Failure;
				goto ExitMe;
			}
	   }
	   else if (  OFFLINE_CS_NODE_FUNCTIONALITY_AS == nNodeFunctionality )
	   {
		   pImsRecord->present = IMSRecord_PR_aSRecord ;
		   if( Offline_CS_RetVal_Success !=  CGF_CreateASN_ASReocrd( hCGFModuleContext_i,
																	 poCDRContents_i,
																	 &(pImsRecord->choice.aSRecord) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Encoding AS CDR Contents,Leaving \n");
				oRetVal = Offline_CS_RetVal_Failure;
				goto ExitMe;
			}
	   }
	   else
	   {
		   // invalid value received in node functionality Cdr content
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Unsupported value in node functionality AVP received ,Leaving\n");
		   oRetVal = Offline_CS_RetVal_Failure;
		   goto ExitMe;
	   }

       EncoderReturnVal = der_encode(&asn_DEF_IMSRecord,pImsRecord, CGF_Write_EncodedContents, poEncodedCDR_o);

       if(EncoderReturnVal.encoded == -1)
		{
			//sprintf(cErrorBuff,"Could not encode Rectangle (at %s)\n", EncoderReturnVal.failed_type ? EncoderReturnVal.failed_type->name : "unknown");
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"%s ,Leaving\n",cErrorBuff);
			oRetVal = Offline_CS_RetVal_Failure;
			goto ExitMe;
        }
	    else
	    {
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Cdr Contents Encoded Successfully\n");
        }

		ExitMe:
		ASN_STRUCT_FREE(asn_DEF_IMSRecord, pImsRecord);

		

	/*IMSRecord_t* pImsRecordForDecode = NULL;
		DecoderReturnVal = ber_decode(0, &asn_DEF_IMSRecord,(void **)&pImsRecordForDecode, 
									  poEncodedCDR_o->oCdrData.ucBuff, poEncodedCDR_o->oCdrData.unsize);
       if(DecoderReturnVal.code != RC_OK)
       {
           sprintf(cErrorBuff,"Broken pImsRecord  encoding at byte %ld\n", (long)DecoderReturnVal.consumed);
		   DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"%s ,Leaving\n",cErrorBuff);
		   return Offline_CS_RetVal_Failure ;
        }


	   //Print the decoded IMSRecord in Human Readable format 
		asn_fprint(stdout,&asn_DEF_IMSRecord,pImsRecordForDecode);
        //Print the decoded IMSRecord in as XML 
        xer_fprint(stdout, &asn_DEF_IMSRecord, pImsRecordForDecode); 

		ASN_STRUCT_FREE(asn_DEF_IMSRecord, pImsRecordForDecode);

	*/
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return oRetVal ;
 }

static int CGF_Write_EncodedContents(const void *buffer, size_t size, void *app_key)
 {
	 t_EncodedCDR* 	poEncodedCDR = (t_EncodedCDR*)app_key;

	 if((NULL == app_key)|| (NULL == buffer)  )
	 {
		 return -1;
	 }
    
     if( (size + poEncodedCDR->oCdrData.unsize) > OFFLINE_CS_ENCODED_CDR_BUF_SIZE_IN_BYTS  )
	 {
		 return -1;
	 }
	 
	 memcpy(&(poEncodedCDR->oCdrData.ucBuff[poEncodedCDR->oCdrData.unsize]),buffer,size);
     poEncodedCDR->oCdrData.unsize = (poEncodedCDR->oCdrData.unsize +size);
	 poEncodedCDR->oCdrHeader.usCdrLength = poEncodedCDR->oCdrData.unsize;
	 return 0;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNNodeAddress( HCGFMODULECONTEXT 	hCGFModuleContext_i,
													    NodeAddress_t** 		ppASNNodeAddress_o, 
														unsigned char* 		pucsourceString_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
		if( NULL == hCGFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL ,Leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}

		if( NULL == ppASNNodeAddress_o)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppASNNodeAddress_o is NULL ,Leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
       
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) ppASNNodeAddress_o,sizeof(NodeAddress_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		
        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) &((*ppASNNodeAddress_o)->choice.iPAddress),
													  sizeof(IPAddress_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		(*ppASNNodeAddress_o)->present = NodeAddress_PR_iPAddress;


	
        
         if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) &((*ppASNNodeAddress_o)->choice.iPAddress->choice.iPBinaryAddress),
													  sizeof(IPBinaryAddress_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		 (*ppASNNodeAddress_o)->choice.iPAddress->present= IPAddress_PR_iPBinaryAddress;


	
		
        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) &((*ppASNNodeAddress_o)->choice.iPAddress->choice.iPBinaryAddress->choice.iPBinV4Address.buf),
													  4) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		(*ppASNNodeAddress_o)->choice.iPAddress->choice.iPBinaryAddress->present = IPBinaryAddress_PR_iPBinV4Address;
		// here check will be perfomed for ipv6 and ipv4 address then will be assigned memory accordingly 
	
		(*ppASNNodeAddress_o)->choice.iPAddress->choice.iPBinaryAddress->choice.iPBinV4Address.size = 4;
	
		memcpy( (*ppASNNodeAddress_o)->choice.iPAddress->choice.iPBinaryAddress->choice.iPBinV4Address.buf,
				pucsourceString_i,
				(*ppASNNodeAddress_o)->choice.iPAddress->choice.iPBinaryAddress->choice.iPBinV4Address.size);


		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNInvolvePartyAddress(HCGFMODULECONTEXT		hCGFModuleContext_i,
															   InvolvedParty_t** 		ppInvolvedParty_o,
															   unsigned char*			pucStringValue_i )
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	if( NULL == hCGFModuleContext_i)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL ,Leaving\n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	if( NULL == ppInvolvedParty_o)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppInvolvedParty_o is NULL ,Leaving\n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	if( NULL == pucStringValue_i)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucStringValue_i is NULL ,Leaving\n");
		return Offline_CS_RetVal_Null_Pointer;
	}


	if( DMBase_Retval_Success !=  DMBase_Malloc(hCGFModuleContext_i->hDiameterStackContext,
												(void*)ppInvolvedParty_o,sizeof(InvolvedParty_t)) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}

	int nSize = 0;
	nSize = strlen(pucStringValue_i);

    if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
												 (void*) &((*ppInvolvedParty_o)->choice.tEL_URI.buf),
												 nSize ) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}
	(*ppInvolvedParty_o)->present = InvolvedParty_PR_tEL_URI;//InvolvedParty_PR_sIP_URI;

	(*ppInvolvedParty_o)->choice.tEL_URI.size = nSize;
	memcpy( ((*ppInvolvedParty_o)->choice.tEL_URI.buf),pucStringValue_i,(*ppInvolvedParty_o)->choice.tEL_URI.size );


	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNServiceSpecificInfoList( HCGFMODULECONTEXT							hCGFModuleContext_i,
																	struct ASRecord__serviceSpecificInfo**		ppoServiceSpecificInfoList_o,
																	t_ServiceSpecificInfoList					oServiceSpecificInfoList_i)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
												 (void*) ppoServiceSpecificInfoList_o,
												 sizeof(struct ASRecord__serviceSpecificInfo) )) 
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}

	int nIndex = 0;
	int nSize = 0;
	unsigned char * pucStringValue = NULL;
	ServiceSpecificInfo_t* pServiceSpecificInfo = NULL;

	for( nIndex = 0; nIndex< oServiceSpecificInfoList_i.nNumberOfItemsInList ;nIndex++)
	{
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*) &pServiceSpecificInfo,
														 sizeof(ServiceSpecificInfo_t)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

			pucStringValue = oServiceSpecificInfoList_i.oItems[nIndex].ucServiceSpecificData;
			nSize = strlen(pucStringValue);
			if( 0 < nSize)
			{
                if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																	&(pServiceSpecificInfo->serviceSpecificData),
																	pucStringValue ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," CGF_CreateASNString() Fails,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			}

			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*) &(pServiceSpecificInfo->serviceSpecificType),
														  sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			*(pServiceSpecificInfo->serviceSpecificType) = oServiceSpecificInfoList_i.oItems[nIndex].unServiceSpecificType;

			ASN_SEQUENCE_ADD(*ppoServiceSpecificInfoList_o,pServiceSpecificInfo );

	  }


	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNListOfInvolveParty( HCGFMODULECONTEXT 			hCGFModuleContext_i,
															   ListOfInvolvedParties_t** 	ppListOfInvolvedParties_o,
															   t_UTF8StringList				oUTF8StringList_i)
{

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");

	if( NULL == hCGFModuleContext_i)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL ,Leaving\n");
		return Offline_CS_RetVal_Null_Pointer;
	}

	if( NULL == ppListOfInvolvedParties_o)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppInvolvedParty_o is NULL ,Leaving\n");
		return Offline_CS_RetVal_Null_Pointer;
	}

    
    if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
												 (void*) ppListOfInvolvedParties_o,
												 sizeof(ListOfInvolvedParties_t)) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}
	
    int nIndex = 0;
	int nSize = 0;
	unsigned char * pucStringValue = NULL;
	InvolvedParty_t* pInvolvedParty = NULL;

    for( nIndex = 0; nIndex< oUTF8StringList_i.nNumberOfItemsInList ;nIndex++)
	{
       
			pucStringValue = oUTF8StringList_i.ucItems[nIndex];
			nSize = strlen(pucStringValue);
			if( nSize > 0 )
			{
				if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																				 &pInvolvedParty,pucStringValue ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
	
			}
			
			ASN_SEQUENCE_ADD(*ppListOfInvolvedParties_o,pInvolvedParty );
      }

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;

}

t_Offline_CS_ReturnVal inline CGF_FreeASNListOfIntrOptId(HCGFMODULECONTEXT					hCGFModuleContext_i,
														InterOperatorIdentifierlist_t**  	ppInterOperatorIdentifierlist_o)
{

}

t_Offline_CS_ReturnVal inline CGF_FreeASNListOfSDPMediaComp( HCGFMODULECONTEXT									hCGFModuleContext_i,
															 struct SCSCFRecord__list_Of_SDP_Media_Components** 	pplistOfSDP_Media_Components_i)
{

}

t_Offline_CS_ReturnVal inline CGF_FreeASNListOfEarlySDPMediaComp(HCGFMODULECONTEXT											hCGFModuleContext_i,
																struct SCSCFRecord__list_Of_Early_SDP_Media_Components** 	pplistOfEarlySDPMediaComponents_i)
{

}

t_Offline_CS_ReturnVal inline CGF_FreeASNListOfMessageBody(HCGFMODULECONTEXT								hCGFModuleContext_i,
														  struct SCSCFRecord__list_Of_Message_Bodies 		**ppMessageList_o )
{

}

t_Offline_CS_ReturnVal inline CGF_FreeASNListOfSubscriptionId(HCGFMODULECONTEXT								hCGFModuleContext_i,
															 struct SCSCFRecord__list_of_subscription_ID** 	pplistofsubscriptionID_o )
{

}


t_Offline_CS_ReturnVal inline CGF_CreateASNTimeStamp(HCGFMODULECONTEXT 	hCGFModuleContext_i,
															  TimeStamp_t** pTimeStamp_o,
															  unsigned int unTimeStamp_i)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

    if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													     (void*)pTimeStamp_o,sizeof(TimeStamp_t)) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}

    if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													     (void*)&((*pTimeStamp_o)->buf),4) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}
	(*pTimeStamp_o)->size = 4;
	memcpy((*pTimeStamp_o)->buf,&unTimeStamp_i,(*pTimeStamp_o)->size);

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNInt( HCGFMODULECONTEXT 			hCGFModuleContext_i,
												LocalSequenceNumber_t**		ppoLocalSequenceNumber_o,
												int		 					nLocalSequenceNumber_i)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
												(void*)ppoLocalSequenceNumber_o,
												 sizeof(LocalSequenceNumber_t)) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}

    if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
												(void*)&((*ppoLocalSequenceNumber_o)->buf),sizeof(int)) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}

	*((*ppoLocalSequenceNumber_o)->buf) = nLocalSequenceNumber_i;

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success ;

}


t_Offline_CS_ReturnVal inline CGF_CreateASNString(HCGFMODULECONTEXT 	hCGFModuleContext_i,
												  OCTET_STRING_t** 		ppASNOctetString_o, 
												  unsigned char* 		pucStringValue_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
		if( NULL == hCGFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL ,Leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}

		if( NULL == ppASNOctetString_o)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppASNOctetString_o is NULL ,Leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		// no need of this as this check will be performed befor calling this function
		if( NULL == pucStringValue_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucStringValue_i is NULL ,Leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
	
        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)ppASNOctetString_o,sizeof(OCTET_STRING_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		unsigned int unSize = strlen(pucStringValue_i);
        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&((*ppASNOctetString_o)->buf),unSize ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		(*ppASNOctetString_o)->size = unSize;
	
		memcpy((*ppASNOctetString_o)->buf,pucStringValue_i,(*ppASNOctetString_o)->size);
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;
 }

t_Offline_CS_ReturnVal inline CGF_CreateASNListOfInterOpId( HCGFMODULECONTEXT 					hCGFModuleContext_i,
															InterOperatorIdentifierlist_t**  	ppInterOperatorIdentifierlist_o,
															t_InterOperatorIdentifierList		oInterOptIdList_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");

		int nSize = 0;
		unsigned char * pucStringValue = NULL;

        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)ppInterOperatorIdentifierlist_o,
													 sizeof(InterOperatorIdentifierlist_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
	
		
		// Now make a node to add into the list 
		InterOperatorIdentifiers_t* pInterOperatorIdentifier = NULL;

		unsigned int index = oInterOptIdList_i.nNumberOfitemsInList ;

		for(index = 0; index< oInterOptIdList_i.nNumberOfitemsInList; index++)
		{
                if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
															 (void*)&pInterOperatorIdentifier,
															 sizeof(InterOperatorIdentifiers_t)) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
			
				//copy Origimngating-IOI to ASN Origimngating-IOI
				pucStringValue = oInterOptIdList_i.oItems[index].ucOriginatingIOI;
				nSize = strlen(pucStringValue);
				if(nSize > 0)
				{
					if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																		&(pInterOperatorIdentifier->originatingIOI),
																		pucStringValue ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," CGF_CreateASNString() Fails,Leaving\n");
						return Offline_CS_RetVal_Failure;
					}

				}
				
			
				//copy Terminating-IOI to ASN Terminating-IOI
				pucStringValue = oInterOptIdList_i.oItems[index].ucTerminatingIOI;
				nSize = strlen(pucStringValue);
				if(nSize > 0)
				{
					if(Offline_CS_RetVal_Success != CGF_CreateASNString( hCGFModuleContext_i,
																		 &(pInterOperatorIdentifier->terminatingIOI),
																		 pucStringValue ) )
					{
						DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR," CGF_CreateASNString() Fails,Leaving\n");
						return Offline_CS_RetVal_Failure;
					}

				}
                
				ASN_SEQUENCE_ADD(*ppInterOperatorIdentifierlist_o ,pInterOperatorIdentifier );
        }

	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;
 }

t_Offline_CS_ReturnVal inline CGF_CreateASNMessageBodyList( HCGFMODULECONTEXT 							hCGFModuleContext_i,
															struct SCSCFRecord__list_Of_Message_Bodies 	**ppMessageList_o,
															t_MessageBodyList 							oMessageBodyList_i)
{

	
	int nSize = 0;
	unsigned char * pucStringValue = NULL;
	
	
	if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
												 (void*)ppMessageList_o,
												 sizeof(struct SCSCFRecord__list_Of_Message_Bodies) ) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}

	
	MessageBody_t* pMessageBody = NULL;
	unsigned int unIndex = oMessageBodyList_i.nNumberOfitemsInList ;

	for(unIndex = 0; unIndex< oMessageBodyList_i.nNumberOfitemsInList ; unIndex++)
	{
            if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(pMessageBody),
														 sizeof(MessageBody_t) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

			pucStringValue = oMessageBodyList_i.oItems[unIndex].ucContentType;
			nSize = strlen(pucStringValue);
			if( 0 < nSize )
			{
				if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
															 (void*)&(pMessageBody->content_Type.buf),
															 nSize ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				pMessageBody->content_Type.size = nSize;
				memcpy(pMessageBody->content_Type.buf,pucStringValue,pMessageBody->content_Type.size);

			}
			
            pMessageBody->content_Length = oMessageBodyList_i.oItems[unIndex].nContentLength;
			
			pucStringValue = oMessageBodyList_i.oItems[unIndex].ucContentDisposition;
			nSize = strlen(pucStringValue);
			if( 0 < nSize )
			{
				if(Offline_CS_RetVal_Success !=  CGF_CreateASNString(hCGFModuleContext_i,
																	 &(pMessageBody->content_Disposition),
																	 pucStringValue) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucStringValue() Fails ,Leaving \n");
					return Offline_CS_RetVal_Failure;
				}
			}

			/*
			if( 0 < nSize)
			{
				if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																				 &pInvolvedParty,pucStringValue ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
	
			} */
            
			/*InvolvedParty_t* pInvolvedParty = NULL;
			pInvolvedParty = malloc(sizeof(InvolvedParty_t));
			if(NULL == pInvolvedParty)
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in allocting memory to pImsRecord->choice.sCSCFRecord ,Leaving \n");
				return Offline_CS_RetVal_Failure;
			}
			pInvolvedParty->present = InvolvedParty_PR_tEL_URI;//InvolvedParty_PR_sIP_URI;
			pInvolvedParty->choice.tEL_URI.buf = malloc(strlen(pucStringValue));
			if(NULL == pInvolvedParty->choice.tEL_URI.buf )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in allocting memory to pImsRecord->choice.sCSCFRecord ,Leaving \n");
				return Offline_CS_RetVal_Failure;
			}
			pInvolvedParty->choice.tEL_URI.size = strlen(pucStringValue);
			memcpy(pInvolvedParty->choice.tEL_URI.buf,pucStringValue,pInvolvedParty->choice.tEL_URI.size);
			
			pMessageBody->originator = pInvolvedParty; */
			
			ASN_SEQUENCE_ADD(*ppMessageList_o,pMessageBody );
            
	}

    DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNListOfSubscriptionId(HCGFMODULECONTEXT 	hCGFModuleContext_i,
																struct SCSCFRecord__list_of_subscription_ID** 	pplistofsubscriptionID_o,
																t_SubscriptionId 								oSubscriptionId_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

        unsigned int nSize = 0;
		unsigned char* pucStringValue = NULL;
		SubscriptionID_t* poSubscriptionId = NULL;

        
        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)pplistofsubscriptionID_o,
													 sizeof(struct SCSCFRecord__list_of_subscription_ID) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
		
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(poSubscriptionId),sizeof(SubscriptionID_t) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		poSubscriptionId->subscriptionIDType = oSubscriptionId_i.nSubscriptionIdType;
        
     // fill subscriptionData field of ASN Subscriptionid 
		pucStringValue = oSubscriptionId_i.ucSubscriptionIdData;
		nSize = strlen(pucStringValue);
		if( 0< nSize )
		{
            if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(poSubscriptionId->subscriptionIDData.buf),
														 nSize ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			poSubscriptionId->subscriptionIDData.size = nSize;
			memcpy(poSubscriptionId->subscriptionIDData.buf,pucStringValue,
				   poSubscriptionId->subscriptionIDData.size);
		}
        
		ASN_SEQUENCE_ADD(*pplistofsubscriptionID_o,poSubscriptionId );

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNListOfSDPMediaComp(HCGFMODULECONTEXT										hCGFModuleContext_i,
															  struct SCSCFRecord__list_Of_SDP_Media_Components** 	pplistOfSDPMediaComponents_o,
															  t_SDPMediaComponentList								oSDPMediaComponentList_i)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		/*int nSize =0 ;
		unsigned int unTimeStampValue = 0;
		unsigned int unvalue = 123;
		Media_Components_List_t* pMedia_Component = NULL;


        if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													(void*)pplistOfSDPMediaComponents_o,
													sizeof(struct SCSCFRecord__list_Of_SDP_Media_Components) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		int nOuterListNum = oSDPMediaComponentList_i.nNumberOfitemsInList ;
		int nInnerListNum = 0,OuterListIndex = 0, InnerListIndex = 0;

		for ( OuterListIndex = 0; OuterListIndex <nOuterListNum ; OuterListIndex++ )
		{
				if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
															 (void*)&(pMedia_Component),
															 sizeof(Media_Components_List_t) ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
	
	
				unTimeStampValue = oSDPMediaComponentList_i.oItems[].;
				if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,
																				&(pMedia_Component->sIP_Request_Timestamp),
																				unTimeStampValue ) )
				{
					printf( " CGF_CreateASNString Fails \n");
					return Offline_CS_RetVal_Failure;
				}
	
	
	
				pMedia_Components_List->sIP_Response_Timestamp = malloc(sizeof(TimeStamp_t));
				if(NULL == pMedia_Components_List->sIP_Response_Timestamp )
				{
					return ;
				}
				pMedia_Components_List->sIP_Response_Timestamp->buf = malloc(nSize);
				pMedia_Components_List->sIP_Response_Timestamp->size = nSize;
				memcpy(pMedia_Components_List->sIP_Response_Timestamp->buf,&unvalue,
					   pMedia_Components_List->sIP_Response_Timestamp->size);
	
				pMedia_Components_List->mediaInitiatorFlag = malloc(sizeof(NULL_t));
	
				//copy 
				pMedia_Components_List->mediaInitiatorParty = malloc(sizeof(InvolvedParty_t));
				if(NULL == pMedia_Components_List->mediaInitiatorParty )
				{
	
				}
				nSize = strlen(pucStringValue);
				pMedia_Components_List->mediaInitiatorParty->present = InvolvedParty_PR_tEL_URI;
				pMedia_Components_List->mediaInitiatorParty->choice.tEL_URI.buf = malloc(nSize);
				pMedia_Components_List->mediaInitiatorParty->choice.tEL_URI.size = nSize;
				memcpy(pMedia_Components_List->mediaInitiatorParty->choice.tEL_URI.buf,pucStringValue,
					   pMedia_Components_List->mediaInitiatorParty->choice.tEL_URI.size);
	
	
	
				pMedia_Components_List->sDP_Session_Description = malloc(sizeof(struct Media_Components_List__sDP_Session_Description));
				if(NULL == pMedia_Components_List->sDP_Session_Description )
				{
					return 0;
				} 
	
				//this code will be run in a loop ------>
				OCTET_STRING_t* pSdpSessionDesp = malloc(sizeof(OCTET_STRING_t));
				if(NULL == pSdpSessionDesp)
				{
					return ;
				}
				nSize = strlen(pucStringValue);
				pSdpSessionDesp->buf = malloc(nSize);
				if(NULL == pSdpSessionDesp->buf)
				{
					return ;
				}
				pSdpSessionDesp->size = nSize;
				memcpy(pSdpSessionDesp->buf , pucStringValue , nSize );
	
				ASN_SEQUENCE_ADD(pMedia_Components_List->sDP_Session_Description,pSdpSessionDesp );
				// -------->
	
	
	
				//this code will be run in a loop ------>
				OCTET_STRING_t* pSdpSessionDesp = malloc(sizeof(OCTET_STRING_t));
				if(NULL == pSdpSessionDesp)
				{
					return ;
				}
				nSize = strlen(pucStringValue);
				pSdpSessionDesp->buf = malloc(nSize);
				if(NULL == pSdpSessionDesp->buf)
				{
					return ;
				}
				pSdpSessionDesp->size = nSize;
				memcpy(pSdpSessionDesp->buf , pucStringValue , nSize );
	
				ASN_SEQUENCE_ADD(pMedia_Components_List->sDP_Session_Description,pSdpSessionDesp );
				// -------->


				ASN_SEQUENCE_ADD(*pplist_Of_SDP_Media_Components_o,pMedia_Components_List );
	}*/
		
		
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASNAppServerInfoList(HCGFMODULECONTEXT 	hCGFModuleContext_i,
														   struct SCSCFRecord__applicationServersInformation** 	ppAppServersInfo_o,
														   t_AppServerInfoList								 	oAppServerInfoList_i)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	int nSize = 0;
	InvolvedParty_t* pInvolvedParty = NULL;
	t_UTF8StringList	oUTF8StringList;
	unsigned char * pucStringValue = NULL;
	
	
	if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)ppAppServersInfo_o,
														 sizeof(struct SCSCFRecord__applicationServersInformation) ) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
		return Offline_CS_RetVal_Failure;
	}

	ApplicationServersInformation_t* poAppServersInfo = NULL;
	unsigned int unIndex = 0 ,unInnerIndex = 0;

	for(unIndex = 0; unIndex< oAppServerInfoList_i.nNumberOfitemsInList ; unIndex++)
	{
			
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(poAppServersInfo),
														 sizeof(ApplicationServersInformation_t) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

			pucStringValue = oAppServerInfoList_i.oItems[unIndex].ucAppServer;
			nSize = strlen(pucStringValue);
			if( 0 < nSize)
			{
                if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,
												 &(poAppServersInfo->applicationServersInvolved),
												 pucStringValue ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNNodeAddress() fails ,Leaving \n");
					return Offline_CS_RetVal_Failure;
				}
            }

			unsigned int unListItems = oAppServerInfoList_i.oItems[unIndex].oAppProvidedCalledPartyAddressList.nNumberOfItemsInList;

			if( 0 < unListItems )
			{
				if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(poAppServersInfo->applicationProvidedCalledParties),
														 sizeof(struct ApplicationServersInformation__applicationProvidedCalledParties) ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}

				for(unInnerIndex =0; unInnerIndex <unListItems; unInnerIndex++)
				{
	
					pucStringValue = oAppServerInfoList_i.oItems[unIndex].oAppProvidedCalledPartyAddressList.ucItems[unInnerIndex];
					nSize = strlen(pucStringValue);
					if( 0 < nSize)
					{
						if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																						 &pInvolvedParty,pucStringValue ) )
						{
							DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
							return Offline_CS_RetVal_Failure;
						}
			
					}
					// copy list of calling party address to ASN list of calling party address
					ASN_SEQUENCE_ADD(poAppServersInfo->applicationProvidedCalledParties,pInvolvedParty );
				}

			}
            
            ASN_SEQUENCE_ADD(*ppAppServersInfo_o,poAppServersInfo );
	}

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success ;
}


t_Offline_CS_ReturnVal inline CGF_CreateASN_SCSCFReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
														 t_CDR_Contents*	poCDRContents_i,
														 SCSCFRecord_t** 	ppoScscfRecord_o )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering \n");

		if(NULL == hCGFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL \n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		if(NULL == poCDRContents_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL \n");
			return Offline_CS_RetVal_Null_Pointer;
		}
	
		if( NULL == ppoScscfRecord_o)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppoScscfRecord_o is NULL \n");
			return Offline_CS_RetVal_Null_Pointer;
		}

		long 				lValue           = 0 ;
		unsigned int 		nSize			 = 0;
		unsigned int 		unTimeStampValue = 0;
		unsigned char*      pucStringValue   = NULL;
        SCSCFRecord_t*		pTempscscfRecord = NULL;
		t_UTF8StringList	oUTF8StringList;
		t_InterOperatorIdentifierList oInterOptIdList;

		
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) ppoScscfRecord_o,sizeof(SCSCFRecord_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
		pTempscscfRecord = *ppoScscfRecord_o;

		int* ValueOfLongType = NULL;
		// fill the recordType field of sCSCFRecord
        pTempscscfRecord->recordType = OFFLINE_CS_NODE_FUNCTIONALITY_S_CSCF;

		// fill the retransmission field of sCSCFRecord
		if(OFFLINE_CS_TRUE == poCDRContents_i->bRetransmission )
		{
            if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*) &(pTempscscfRecord->retransmission),
														  sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->sIP_Method),pucStringValue ) )
			{
				//printf( " CGF_CreateASNString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}


		// coyp role-of-node
		if( 0 <= poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode )
		{
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(pTempscscfRecord->role_of_Node),
														 OFFLINE_CS_SIZE_OF_INTEGER_LONG_IN_BYTES ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            *(pTempscscfRecord->role_of_Node) = poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode;
		}
		
        


        // copy Node address
		pucStringValue =poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(pTempscscfRecord->nodeAddress),pucStringValue ) )
			{
				//printf( " CGF_CreateASNNodeAddress() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

	// copy session id field 
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId; 
		if( 0 < strlen(pucStringValue))
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(pTempscscfRecord->session_Id),
																pucStringValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}
		
        // copy list of calling party address to ASN list of calling party address
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(pTempscscfRecord->list_Of_Calling_Party_Address),
																			oUTF8StringList) )
			{
				//printf( " CGF_CopyListOfStringToASNListOfString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		
		}
	

        // copy called-party address to ASN called-party address
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(pTempscscfRecord->called_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

        
        //copy private user Id to ASN Private user Id
		pucStringValue = poCDRContents_i->ucUserName;
		nSize = strlen(pucStringValue);
		if( 0 < nSize  )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(pTempscscfRecord->privateUserID),
																pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

       //copy service request time stamp to ASN service request time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->serviceRequestTimeStamp),unTimeStampValue ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNTimeStamp() fails ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		// copy service delivery start time stamp to ASN service delivery start time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->serviceDeliveryStartTimeStamp),unTimeStampValue ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNTimeStamp() fails ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

 
		//copy service delivery end time stamp to ASN service delivery end time stamp
		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->serviceDeliveryEndTimeStamp),unTimeStampValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNTimeStamp() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		
		if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
		{
			//copy record opening time to ASN record opening time  
			// get current time and sned cpoy into that field
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->recordOpeningTime),unTimeStampValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNTimeStamp() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			// get current time and sned cpoy into that field
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&pTempscscfRecord->recordClosureTime,unTimeStampValue ))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNTimeStamp() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
    

		// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		nSize = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.nNumberOfitemsInList;
        if( 0 < nSize  )
		{
			oInterOptIdList = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInterOpId(hCGFModuleContext_i,
																		 &(pTempscscfRecord->interOperatorIdentifiers),
																		 oInterOptIdList ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInterOpId() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}
	
		//copy local record sequence number to ASN local record sequence number

		if( 0 < poCDRContents_i->nLocalRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(pTempscscfRecord->localRecordSequenceNumber),
															poCDRContents_i->nLocalRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}


		if( 0 < poCDRContents_i->nRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(pTempscscfRecord->recordSequenceNumber),
															poCDRContents_i->nRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

        
		//copy Incomplete cdr indication to ASN Incomplete cdr indication
       if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(pTempscscfRecord->incomplete_CDR_Indication),
													 sizeof(Incomplete_CDR_Indication_t) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		pTempscscfRecord->incomplete_CDR_Indication->aCRInterimLost = 1;

		//copy IMS-charging-Identifier to ASN  IMS-charging-Identifier
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier;
        nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																	  &(pTempscscfRecord->iMS_Charging_Identifier),
																	  pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString(),Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		
		// copy GGsnAddress
		pucStringValue = poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(pTempscscfRecord->gGSNaddress),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNNodeAddress() ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
	
		
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList.nNumberOfitemsInList > 0 )
		{
			t_MessageBodyList oMessageBodyList = poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNMessageBodyList(hCGFModuleContext_i,
																		 &(pTempscscfRecord->list_Of_Message_Bodies),
																		 oMessageBodyList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNMessageBodyList() ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
	

		// fill associated-URI field of IMSRecord
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList > 0 )
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(pTempscscfRecord->list_Of_Associated_URI),
																			oUTF8StringList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInvolveParty() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
      
		// fill event field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent;
		nSize = strlen(pucStringValue);
		if(  nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->event),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}
        
		// fill AccessNetworkInformation field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->accessNetworkInformation),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		// fill ServiceContextId field of IMSRecord
		pucStringValue = poCDRContents_i->ucServiceContextId;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->serviceContextID),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString()fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

        // fill List of Subscription-Id  field of SCSCFRecord
		t_SubscriptionId oSubscriptionId = poCDRContents_i->oServiceInfo.oSubscriptionId;
		pucStringValue = poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData;
		
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfSubscriptionId(hCGFModuleContext_i,&(pTempscscfRecord->list_of_subscription_ID),
																			  (poCDRContents_i->oServiceInfo.oSubscriptionId) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfSubscriptionId() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}


 
		// fill list of early-SDP-Media-Components here 


		// fill IMS-Communication-service  identifier
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSCommunicationServiceIdentifier;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->iMSCommunicationServiceIdentifier),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		// fill NumberPortabilityRoutingInformation field of SCSCFRecord
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString( hCGFModuleContext_i,
																 &(pTempscscfRecord->numberPortabilityRouting),
																 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		// fill Carrier Select Routing Information field of SCSCFRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(pTempscscfRecord->carrierSelectRouting),
																pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

        // fill List of Application-Server List  field of SCSCFRecord
		t_AppServerInfoList oAppServerInfoList = poCDRContents_i->oServiceInfo.oIMSInformation.oAppServerInfoList;
        if( oAppServerInfoList.nNumberOfitemsInList > 0 )
		{
			if( Offline_CS_RetVal_Success != CGF_CreateASNAppServerInfoList(hCGFModuleContext_i,
																			&(pTempscscfRecord->applicationServersInformation),
																			oAppServerInfoList ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNAppServerInfoList() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(pTempscscfRecord->requested_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

	
        // fill Called-Asserted-Identities field of SCSCFRecord
		oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList;
		if(oUTF8StringList.nNumberOfItemsInList > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(pTempscscfRecord->list_Of_Called_Asserted_Identity),
																			oUTF8StringList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInvolveParty() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
				
			}

		}
 

		//pTempscscfRecord->causeForRecordClosing = malloc( sizeof(long));

		//if(NULL == pTempscscfRecord->causeForRecordClosing)
		//{
			//return;
		//}
		//memcpy(pTempscscfRecord->causeForRecordClosing,&unTestValue,size );

				// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		/*if(1 != CGF_CopyListOfSDPMediaCompToASNListOfSDPMediaComp(&(pTempscscfRecord->list_Of_SDP_Media_Components),
																  pTestString ) )
		{
			printf( " CGF_CreateASNListOfInterOpId() Fails \n");
			return;
		}*/




		// not found will have to add 
		//poCDRContents_i->oServiceInfo.oIMSInformation.
		/*if(1 != CGF_CreateASNString(&(pTempscscfRecord->serviceReasonReturnCode),pucStringValue ) )
		{
			printf( " CGF_CreateASNString Fails \n");
			return;
		}*/


			/// copy Expires information
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(pTempscscfRecord->expiresInformation),sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			*(pTempscscfRecord->expiresInformation) = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires;
	
		
        //ASN_STRUCT_FREE(asn_DEF_InvolvedParty, ptestPart);
		
		//asn_fprint(stdout,&asn_DEF_InvolvedParty,ptestPart);

        //char a[12];
		//gets(a); 
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	   return Offline_CS_RetVal_Success ;
}

t_Offline_CS_ReturnVal inline CGF_CreateASN_PCSCFReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
														 t_CDR_Contents*	poCDRContents_i,
														 PCSCFRecord_t** 	ppoPcscfRecord_o)
{
		if(NULL == hCGFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL \n");
			return Offline_CS_RetVal_Failure;
		}
	
		if(NULL == poCDRContents_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL \n");
			return Offline_CS_RetVal_Failure;
		}
	
		if(NULL == ppoPcscfRecord_o)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppoPcscfRecord_o is NULL \n");
			return Offline_CS_RetVal_Failure;
		}
    
		long 				lValue           = 0 ;
		unsigned int 		nSize			 = 0;
		unsigned int 		unTimeStampValue = 0;
		unsigned char*      pucStringValue   = NULL;
        PCSCFRecord_t*		pTempscscfRecord = NULL;
		t_UTF8StringList	oUTF8StringList;
		t_InterOperatorIdentifierList oInterOptIdList;

		
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) ppoPcscfRecord_o,sizeof(PCSCFRecord_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
		pTempscscfRecord = *ppoPcscfRecord_o;

		int* ValueOfLongType = NULL;
		// fill the recordType field of sCSCFRecord
        pTempscscfRecord->recordType = OFFLINE_CS_NODE_FUNCTIONALITY_P_CSCF;

		// fill the retransmission field of sCSCFRecord
		if(OFFLINE_CS_TRUE == poCDRContents_i->bRetransmission )
		{
            if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*) &(pTempscscfRecord->retransmission),
														  sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->sIP_Method),pucStringValue ) )
			{
				//printf( " CGF_CreateASNString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}

		// coyp role-of-node
		if( 0 <= poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode )
		{
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(pTempscscfRecord->role_of_Node),
														 OFFLINE_CS_SIZE_OF_INTEGER_LONG_IN_BYTES ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            *(pTempscscfRecord->role_of_Node) = poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode;
			
		}
        
        // copy Node address
		pucStringValue =poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(pTempscscfRecord->nodeAddress),pucStringValue ) )
			{
				//printf( " CGF_CreateASNNodeAddress() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

	// copy session id field 
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId; 
		if( 0 < strlen(pucStringValue))
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(pTempscscfRecord->session_Id),
																pucStringValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}
		
        // copy list of calling party address to ASN list of calling party address
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(pTempscscfRecord->list_Of_Calling_Party_Address),
																			oUTF8StringList) )
			{
				//printf( " CGF_CopyListOfStringToASNListOfString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		
		}


        // copy called-party address to ASN called-party address
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(pTempscscfRecord->called_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

       

       //copy service request time stamp to ASN service request time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->serviceRequestTimeStamp),unTimeStampValue ) )
		{
			//printf( " CGF_CreateASNString Fails \n");
			return Offline_CS_RetVal_Failure;
		}

		// copy service delivery start time stamp to ASN service delivery start time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->serviceDeliveryStartTimeStamp),unTimeStampValue ) )
		{
			//printf( " CGF_CreateASNString Fails \n");
			return Offline_CS_RetVal_Failure;
		}

 
		//copy service delivery end time stamp to ASN service delivery end time stamp
		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->serviceDeliveryEndTimeStamp),unTimeStampValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
		{
			//copy record opening time to ASN record opening time  
			unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp;
            if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempscscfRecord->recordOpeningTime),unTimeStampValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			// get current time and sned cpoy into that field
            if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&pTempscscfRecord->recordClosureTime,unTimeStampValue ))
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}


        
		// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		nSize = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.nNumberOfitemsInList;
        if( 0 < nSize  )
		{
			
			oInterOptIdList = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInterOpId(hCGFModuleContext_i,
																		 &(pTempscscfRecord->interOperatorIdentifiers),
																		 oInterOptIdList ) )
			{
				//printf( " CGF_CreateASNListOfInterOpId() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}
	


		if( 0 < poCDRContents_i->nLocalRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(pTempscscfRecord->localRecordSequenceNumber),
															poCDRContents_i->nLocalRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}


		if( 0 < poCDRContents_i->nRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(pTempscscfRecord->recordSequenceNumber),
															poCDRContents_i->nRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}



		//copy Incomplete cdr indication to ASN Incomplete cdr indication
       if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(pTempscscfRecord->incomplete_CDR_Indication),
													 sizeof(Incomplete_CDR_Indication_t) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		pTempscscfRecord->incomplete_CDR_Indication->aCRInterimLost = 1;

		//copy IMS-charging-Identifier to ASN  IMS-charging-Identifier
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier;
        nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(pTempscscfRecord->iMS_Charging_Identifier),
																 pucStringValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
	
		// copy GGsnAddress
		pucStringValue = poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(pTempscscfRecord->gGSNaddress),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNNodeAddress() ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
	
		
	if( poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList.nNumberOfitemsInList > 0 )
		{
			t_MessageBodyList oMessageBodyList = poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNMessageBodyList(hCGFModuleContext_i,
																		 (Message_Body_t*)&(pTempscscfRecord->list_Of_Message_Bodies),
																		 oMessageBodyList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNMessageBodyList() ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
	

		// fill associated-URI field of IMSRecord
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList > 0 )
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,&(pTempscscfRecord->list_Of_Associated_URI),
															  oUTF8StringList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInvolveParty() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
        
		// fill event field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent;
		nSize = strlen(pucStringValue);
		if(  nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->event),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}
        
		// fill AccessNetworkInformation field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->accessNetworkInformation),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		// fill ServiceContextId field of IMSRecord
		pucStringValue = poCDRContents_i->ucServiceContextId;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->serviceContextID),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString()fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

        // fill List of Subscription-Id  field of SCSCFRecord
		t_SubscriptionId oSubscriptionId = poCDRContents_i->oServiceInfo.oSubscriptionId;
		pucStringValue = poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData;
		
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfSubscriptionId(hCGFModuleContext_i,
																			  (Subscription_Id_t*)&(pTempscscfRecord->list_of_subscription_ID),
																			  (poCDRContents_i->oServiceInfo.oSubscriptionId) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfSubscriptionId() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

 
	/*	// fill list of early-SDP-Media-Components here 


		// fill IMS-Communication-service  identifier
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSCommunicationServiceIdentifier;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->iMSCommunicationServiceIdentifier),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
	*/

		// fill NumberPortabilityRoutingInformation field of SCSCFRecord
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->numberPortabilityRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		// fill Carrier Select Routing Information field of SCSCFRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempscscfRecord->carrierSelectRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}



		// write code for served party address
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucServedPartyAddress;
		nSize = strlen(pucStringValue);
        
		if(nSize > 0 )
		{
				if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
															 (void*)&(pTempscscfRecord->servedPartyIPAddress),
															 sizeof(IPAddress_t)) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
				pTempscscfRecord->servedPartyIPAddress->present = IPAddress_PR_iPTextRepresentedAddress;
		
				if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
															 (void*)&(pTempscscfRecord->servedPartyIPAddress->choice.iPTextRepresentedAddress),
															 sizeof(struct IPTextRepresentedAddress)) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}
		
				pTempscscfRecord->servedPartyIPAddress->choice.iPTextRepresentedAddress->present = 
																IPTextRepresentedAddress_PR_iPTextV4Address;
				if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																	(OCTET_STRING_t**)&(pTempscscfRecord->servedPartyIPAddress->choice.iPTextRepresentedAddress->choice.iPTextV4Address),
																	pucStringValue ) )
				{
					DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
					return Offline_CS_RetVal_Failure;
				}

		}
		
        
				// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		/*if(1 != CGF_CopyListOfSDPMediaCompToASNListOfSDPMediaComp(&(pTempscscfRecord->list_Of_SDP_Media_Components),
																  pTestString ) )
		{
			printf( " CGF_CreateASNListOfInterOpId() Fails \n");
			return;
		}*/




		// not found will have to add 
		//poCDRContents_i->oServiceInfo.oIMSInformation.
		/*if(1 != CGF_CreateASNString(&(pTempscscfRecord->serviceReasonReturnCode),pucStringValue ) )
		{
			printf( " CGF_CreateASNString Fails \n");
			return;
		}*/


			
			/// copy Expires information
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(pTempscscfRecord->expiresInformation),sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			*(pTempscscfRecord->expiresInformation) = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires;



		

		
        //ASN_STRUCT_FREE(asn_DEF_InvolvedParty, ptestPart);
		
		//asn_fprint(stdout,&asn_DEF_InvolvedParty,ptestPart);

        //char a[12];
		//gets(a); 
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	   return Offline_CS_RetVal_Success ;


}
t_Offline_CS_ReturnVal inline CGF_CreateASN_ICSCFReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
														 t_CDR_Contents*		poCDRContents_i,
														 ICSCFRecord_t** 	ppoIcscfRecord_o )
{
		if(NULL == hCGFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL \n");
			return;
		}
	
		if(NULL == poCDRContents_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL \n");
			return;
		}
	
		if(NULL == ppoIcscfRecord_o )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppoIcscfRecord_o is NULL \n");
			return;
		}
	
        
        
		long 				lValue           = 0 ;
		unsigned int 		nSize			 = 0;
		unsigned int 		unTimeStampValue = 0;
		unsigned char*      pucStringValue   = NULL;
        ICSCFRecord_t*		pTempIcscfRecord = NULL;
		t_UTF8StringList	oUTF8StringList;
		t_InterOperatorIdentifierList oInterOptIdList;

		
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) ppoIcscfRecord_o,sizeof(ICSCFRecord_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
		pTempIcscfRecord = *ppoIcscfRecord_o;

		int* ValueOfLongType = NULL;
		// fill the recordType field of sCSCFRecord
        pTempIcscfRecord->recordType = OFFLINE_CS_NODE_FUNCTIONALITY_I_CSCF;

		// fill the retransmission field of sCSCFRecord
		if(OFFLINE_CS_TRUE == poCDRContents_i->bRetransmission )
		{
            if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*) &(pTempIcscfRecord->retransmission),
														  sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempIcscfRecord->sIP_Method),pucStringValue ) )
			{
				//printf( " CGF_CreateASNString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}

		
		// coyp role-of-node
		if( 0 <= poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode )
		{
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(pTempIcscfRecord->role_of_Node),
													 OFFLINE_CS_SIZE_OF_INTEGER_LONG_IN_BYTES ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			*(pTempIcscfRecord->role_of_Node) = poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode;

		}
        
		

        // copy Node address
		pucStringValue =poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(pTempIcscfRecord->nodeAddress),pucStringValue ) )
			{
				//printf( " CGF_CreateASNNodeAddress() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

	// copy session id field 
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId; 
		if( 0 < strlen(pucStringValue))
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(pTempIcscfRecord->session_Id),
																pucStringValue ) )
			{
				printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}
		
        // copy list of calling party address to ASN list of calling party address
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(pTempIcscfRecord->list_Of_Calling_Party_Address),
																			oUTF8StringList) )
			{
				//printf( " CGF_CopyListOfStringToASNListOfString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		
		}
	

        // copy called-party address to ASN called-party address
		/*pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(pTempIcscfRecord->called_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}*/

        
        
		

       //copy service request time stamp to ASN service request time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(pTempIcscfRecord->serviceRequestTimeStamp),unTimeStampValue ) )
		{
			//printf( " CGF_CreateASNString Fails \n");
			return Offline_CS_RetVal_Failure;
		}

		
		// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		nSize = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.nNumberOfitemsInList;
        if( 0 < nSize  )
		{
			
			oInterOptIdList = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInterOpId(hCGFModuleContext_i,
																		 &(pTempIcscfRecord->interOperatorIdentifiers),
																		 oInterOptIdList ) )
			{
				printf( " CGF_CreateASNListOfInterOpId() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}
	

		if( 0 < poCDRContents_i->nLocalRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(pTempIcscfRecord->localRecordSequenceNumber),
															poCDRContents_i->nLocalRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}


        
		//copy Incomplete cdr indication to ASN Incomplete cdr indication
       if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(pTempIcscfRecord->incomplete_CDR_Indication),
													 sizeof(Incomplete_CDR_Indication_t) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		pTempIcscfRecord->incomplete_CDR_Indication->aCRInterimLost = 1;

		//copy IMS-charging-Identifier to ASN  IMS-charging-Identifier
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier;
        nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																	  &(pTempIcscfRecord->iMS_Charging_Identifier),
																	  pucStringValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		
	  
		
        

		// fill associated-URI field of IMSRecord
	/*	if( poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList > 0 )
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,&(pTempIcscfRecord->list_Of_Associated_URI),
															  oUTF8StringList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInvolveParty() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}*/
        
		// fill event field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent;
		nSize = strlen(pucStringValue);
		if(  nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempIcscfRecord->event),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}
        
		// fill AccessNetworkInformation field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempIcscfRecord->accessNetworkInformation),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		// fill ServiceContextId field of IMSRecord
		pucStringValue = poCDRContents_i->ucServiceContextId;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempIcscfRecord->serviceContextID),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString()fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

       
        // fill NumberPortabilityRoutingInformation field of SCSCFRecord
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempIcscfRecord->numberPortabilityRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		// fill Carrier Select Routing Information field of SCSCFRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(pTempIcscfRecord->carrierSelectRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

     

        
		//pTempIcscfRecord->causeForRecordClosing = malloc( sizeof(long));

		//if(NULL == pTempIcscfRecord->causeForRecordClosing)
		//{
			//return;
		//}
		//memcpy(pTempIcscfRecord->causeForRecordClosing,&unTestValue,size );

				// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		/*if(1 != CGF_CopyListOfSDPMediaCompToASNListOfSDPMediaComp(&(pTempIcscfRecord->list_Of_SDP_Media_Components),
																  pTestString ) )
		{
			printf( " CGF_CreateASNListOfInterOpId() Fails \n");
			return;
		}*/




		// not found will have to add 
		//poCDRContents_i->oServiceInfo.oIMSInformation.
		/*if(1 != CGF_CreateASNString(&(pTempIcscfRecord->serviceReasonReturnCode),pucStringValue ) )
		{
			printf( " CGF_CreateASNString Fails \n");
			return;
		}*/

		// copy Expires information
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(pTempIcscfRecord->expiresInformation),sizeof(long)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		*(pTempIcscfRecord->expiresInformation) = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires;
			
         
        //ASN_STRUCT_FREE(asn_DEF_InvolvedParty, ptestPart);
		
		//asn_fprint(stdout,&asn_DEF_InvolvedParty,ptestPart);

        //char a[12];
		//gets(a);
        
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;

}
t_Offline_CS_ReturnVal inline CGF_CreateASN_MRFCReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
														t_CDR_Contents*		poCDRContents_i,
														MRFCRecord_t** 		ppoMRFCRecord_o )
{
		if(NULL == hCGFModuleContext_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL \n");
			return;
		}
	
		if(NULL == poCDRContents_i)
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL \n");
			return;
		}
	
		if(NULL == ppoMRFCRecord_o )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppoMRFCRecord_o is NULL \n");
			return;
		}
	
        long 				lValue           = 0 ;
		unsigned int 		nSize			 = 0;
		unsigned int 		unTimeStampValue = 0;
		unsigned char*      pucStringValue   = NULL;
        MRFCRecord_t*		poTempMrfcRecord = NULL;
		t_UTF8StringList	oUTF8StringList;
		t_InterOperatorIdentifierList oInterOptIdList;

		
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) ppoMRFCRecord_o,sizeof(MRFCRecord_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
		poTempMrfcRecord = *ppoMRFCRecord_o;

		int* ValueOfLongType = NULL;
		// fill the recordType field of sCSCFRecord
        poTempMrfcRecord->recordType = OFFLINE_CS_NODE_FUNCTIONALITY_S_CSCF;

		// fill the retransmission field of sCSCFRecord
		if(OFFLINE_CS_TRUE == poCDRContents_i->bRetransmission )
		{
            if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*) &(poTempMrfcRecord->retransmission),
														  sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempMrfcRecord->sIP_Method),pucStringValue ) )
			{
				//printf( " CGF_CreateASNString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}

		if( 0 <= poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode )
		{
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(poTempMrfcRecord->role_of_Node),
														 OFFLINE_CS_SIZE_OF_INTEGER_LONG_IN_BYTES ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            *(poTempMrfcRecord->role_of_Node) = poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode;
		}

        // copy Node address
		pucStringValue =poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(poTempMrfcRecord->nodeAddress),pucStringValue ) )
			{
				//printf( " CGF_CreateASNNodeAddress() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

	// copy session id field 
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId; 
		if( 0 < strlen(pucStringValue))
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(poTempMrfcRecord->session_Id),
																pucStringValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}
		
        // copy list of calling party address to ASN list of calling party address
		/*if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(poTempMrfcRecord->list_Of_Calling_Party_Address),
																			oUTF8StringList) )
			{
				printf( " CGF_CopyListOfStringToASNListOfString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		
		}*/
	

        // copy called-party address to ASN called-party address
		/*pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(poTempMrfcRecord->called_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}*/

        
        
       //copy service request time stamp to ASN service request time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempMrfcRecord->serviceRequestTimeStamp),unTimeStampValue ) )
		{
			//printf( " CGF_CreateASNString Fails \n");
			return Offline_CS_RetVal_Failure;
		}

		// copy service delivery start time stamp to ASN service delivery start time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempMrfcRecord->serviceDeliveryStartTimeStamp),unTimeStampValue ) )
		{
			//printf( " CGF_CreateASNString Fails \n");
			return Offline_CS_RetVal_Failure;
		}

 
		//copy service delivery end time stamp to ASN service delivery end time stamp
		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempMrfcRecord->serviceDeliveryEndTimeStamp),unTimeStampValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
		{
			//copy record opening time to ASN record opening time  
			// get current time and sned cpoy into that field
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempMrfcRecord->recordOpeningTime),unTimeStampValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			// get current time and sned cpoy into that field
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&poTempMrfcRecord->recordClosureTime,unTimeStampValue ))
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}


		// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		nSize = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.nNumberOfitemsInList;
        if( 0 < nSize  )
		{
			
			oInterOptIdList = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInterOpId(hCGFModuleContext_i,
																		 &(poTempMrfcRecord->interOperatorIdentifiers),
																		 oInterOptIdList ) )
			{
				//printf( " CGF_CreateASNListOfInterOpId() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}


		if( 0 < poCDRContents_i->nLocalRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(poTempMrfcRecord->localRecordSequenceNumber),
															poCDRContents_i->nLocalRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}


		if( 0 < poCDRContents_i->nRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(poTempMrfcRecord->recordSequenceNumber),
															poCDRContents_i->nRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

		

		//copy Incomplete cdr indication to ASN Incomplete cdr indication
       if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(poTempMrfcRecord->incomplete_CDR_Indication),
													 sizeof(Incomplete_CDR_Indication_t) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		poTempMrfcRecord->incomplete_CDR_Indication->aCRInterimLost = 1;

		//copy IMS-charging-Identifier to ASN  IMS-charging-Identifier
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier;
        nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																	  &(poTempMrfcRecord->iMS_Charging_Identifier),
																	  pucStringValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		
		// copy GGsnAddress
		pucStringValue = poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(poTempMrfcRecord->gGSNaddress),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNNodeAddress() ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
	
		
        
		// fill associated-URI field of IMSRecord
	/*	if( poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList.nNumberOfItemsInList > 0 )
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oAssociatedURIList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,&(poTempMrfcRecord->list_Of_Associated_URI),
															  oUTF8StringList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInvolveParty() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
      */  


		// fill event field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent;
		nSize = strlen(pucStringValue);
		if(  nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempMrfcRecord->event),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}
        
		// fill AccessNetworkInformation field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempMrfcRecord->accessNetworkInformation),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		// fill ServiceContextId field of IMSRecord
		pucStringValue = poCDRContents_i->ucServiceContextId;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempMrfcRecord->serviceContextID),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString()fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

        // fill List of Subscription-Id  field of SCSCFRecord
		t_SubscriptionId oSubscriptionId = poCDRContents_i->oServiceInfo.oSubscriptionId;
		pucStringValue = poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData;
		
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfSubscriptionId(hCGFModuleContext_i,
																			  (Subscription_Id_t*)&(poTempMrfcRecord->list_of_subscription_ID),
																			  (poCDRContents_i->oServiceInfo.oSubscriptionId) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfSubscriptionId() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

 
		// fill list of early-SDP-Media-Components here 


		
		// fill NumberPortabilityRoutingInformation field of SCSCFRecord
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempMrfcRecord->numberPortabilityRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		// fill Carrier Select Routing Information field of SCSCFRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempMrfcRecord->carrierSelectRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

       // fill List of Application-Server List  field of SCSCFRecord
		t_AppServerInfoList oAppServerInfoList = poCDRContents_i->oServiceInfo.oIMSInformation.oAppServerInfoList;
        if( oAppServerInfoList.nNumberOfitemsInList > 0 )
		{
			if( Offline_CS_RetVal_Success != CGF_CreateASNAppServerInfoList(hCGFModuleContext_i,
																			(Appserver_List_t*)&(poTempMrfcRecord->applicationServersInformation),
																			oAppServerInfoList ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNAppServerInfoList() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
 
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(poTempMrfcRecord->requested_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

		
        // fill Called-Asserted-Identities field of SCSCFRecord
		oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList;
		if(oUTF8StringList.nNumberOfItemsInList > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(poTempMrfcRecord->list_Of_Called_Asserted_Identity),
																			oUTF8StringList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInvolveParty() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
				
			}

		}
 
		// copy service id 
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucServiceId;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempMrfcRecord->service_Id),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

        // copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		/*if(1 != CGF_CopyListOfSDPMediaCompToASNListOfSDPMediaComp(&(poTempMrfcRecord->list_Of_SDP_Media_Components),
																  pTestString ) )
		{
			printf( " CGF_CreateASNListOfInterOpId() Fails \n");
			return;
		}*/




		// not found will have to add 
		//poCDRContents_i->oServiceInfo.oIMSInformation.
		/*if(1 != CGF_CreateASNString(&(poTempMrfcRecord->serviceReasonReturnCode),pucStringValue ) )
		{
			printf( " CGF_CreateASNString Fails \n");
			return;
		}*/


		// copy Expires information
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(poTempMrfcRecord->expiresInformation),sizeof(long)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		*(poTempMrfcRecord->expiresInformation) = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires;

		
        //ASN_STRUCT_FREE(asn_DEF_InvolvedParty, ptestPart);
		
		//asn_fprint(stdout,&asn_DEF_InvolvedParty,ptestPart);

        //char a[12];
		//gets(a);


		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;
 }
t_Offline_CS_ReturnVal inline CGF_CreateASN_ASReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
													  t_CDR_Contents*		poCDRContents_i,
													  ASRecord_t** 			ppoAsRecord_o )
{
		if( NULL == hCGFModuleContext_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCGFModuleContext_i is NULL \n");
			return;
		}
	
		if( NULL == poCDRContents_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poCDRContents_i is NULL \n");
			return;
		}
	
		if( NULL == ppoAsRecord_o )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ppoAsRecord_o is NULL \n");
			return;
		}
	
        
        
		long 				lValue           = 0 ;
		unsigned int 		nSize			 = 0;
		unsigned int 		unTimeStampValue = 0;
		unsigned char*      pucStringValue   = NULL;
        ASRecord_t*			poTempASRecord = NULL;
		t_UTF8StringList	oUTF8StringList;
		t_InterOperatorIdentifierList oInterOptIdList;

		
		if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*) ppoAsRecord_o,sizeof(ASRecord_t)) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		
		poTempASRecord = *ppoAsRecord_o;

		int* ValueOfLongType = NULL;
		// fill the recordType field of sCSCFRecord
        poTempASRecord->recordType = OFFLINE_CS_NODE_FUNCTIONALITY_AS;

		// fill the retransmission field of sCSCFRecord
		if(OFFLINE_CS_TRUE == poCDRContents_i->bRetransmission )
		{
            if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*) &(poTempASRecord->retransmission),
														  sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucSipMethod;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempASRecord->sIP_Method),pucStringValue ) )
			{
				//printf( " CGF_CreateASNString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}


		if( 0 <= poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode )
		{
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(poTempASRecord->role_of_Node),
														 OFFLINE_CS_SIZE_OF_INTEGER_LONG_IN_BYTES ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            *(poTempASRecord->role_of_Node) = poCDRContents_i->oServiceInfo.oIMSInformation.nRoleOfNode;
		}


        // copy Node address
		pucStringValue =poCDRContents_i->oServiceInfo.oIMSInformation.ucNodeAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(poTempASRecord->nodeAddress),pucStringValue ) )
			{
				//printf( " CGF_CreateASNNodeAddress() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

	// copy session id field 
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucUserSessionId; 
		if( 0 < strlen(pucStringValue))
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																&(poTempASRecord->session_Id),
																pucStringValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}
		
        // copy list of calling party address to ASN list of calling party address
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList.nNumberOfItemsInList > 0)
		{
			oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCallingPartyAddressList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(poTempASRecord->list_Of_Calling_Party_Address),
																			oUTF8StringList) )
			{
				//printf( " CGF_CopyListOfStringToASNListOfString() Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		
		}
	

        // copy called-party address to ASN called-party address
		/*pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCalledPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(poTempASRecord->called_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}*/

        
       
       //copy service request time stamp to ASN service request time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempASRecord->serviceRequestTimeStamp),unTimeStampValue ) )
		{
			//printf( " CGF_CreateASNString Fails \n");
			return Offline_CS_RetVal_Failure;
		}

		// copy service delivery start time stamp to ASN service delivery start time stamp
		unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipResponseTimeStamp;
		if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempASRecord->serviceDeliveryStartTimeStamp),unTimeStampValue ) )
		{
			//printf( " CGF_CreateASNString Fails \n");
			return Offline_CS_RetVal_Failure;
		}

 
		//copy service delivery end time stamp to ASN service delivery end time stamp
		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			unTimeStampValue = poCDRContents_i->oServiceInfo.oIMSInformation.oTimeStamps.unSipRequestTimeStamp;
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempASRecord->serviceDeliveryEndTimeStamp),unTimeStampValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		if( DMBASE_START_RECORD == poCDRContents_i->nAcctRecordType )
		{
			//copy record opening time to ASN record opening time  
			// get current time and sned cpoy into that field
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&(poTempASRecord->recordOpeningTime),unTimeStampValue ) )
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}


		if( DMBASE_STOP_RECORD == poCDRContents_i->nAcctRecordType )
		{
			// get current time and sned cpoy into that field
			if(Offline_CS_RetVal_Success != CGF_CreateASNTimeStamp(hCGFModuleContext_i,&poTempASRecord->recordClosureTime,unTimeStampValue ))
			{
				//printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}


		// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		nSize = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList.nNumberOfitemsInList;
        if( 0 < nSize  )
		{
			
			oInterOptIdList = poCDRContents_i->oServiceInfo.oIMSInformation.oInterOperatorIdentifierList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInterOpId(hCGFModuleContext_i,
																		 &(poTempASRecord->interOperatorIdentifiers),
																		 oInterOptIdList ) )
			{
				//printf( " CGF_CreateASNListOfInterOpId() Fails \n");
				return Offline_CS_RetVal_Failure;
			}

		}


		if( 0 < poCDRContents_i->nLocalRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(poTempASRecord->localRecordSequenceNumber),
															poCDRContents_i->nLocalRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}


		if( 0 < poCDRContents_i->nRecordSequenceNumber )
		{
			if( DMBase_Retval_Success !=  CGF_CreateASNInt( hCGFModuleContext_i,
															(void*)&(poTempASRecord->recordSequenceNumber),
															poCDRContents_i->nRecordSequenceNumber	 ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

		

		//copy Incomplete cdr indication to ASN Incomplete cdr indication
       if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
													 (void*)&(poTempASRecord->incomplete_CDR_Indication),
													 sizeof(Incomplete_CDR_Indication_t) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		poTempASRecord->incomplete_CDR_Indication->aCRInterimLost = 1;

		//copy IMS-charging-Identifier to ASN  IMS-charging-Identifier
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSChargingIdentifier;
        nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																	  &(poTempASRecord->iMS_Charging_Identifier),
																	  pucStringValue ) )
			{
				printf( " CGF_CreateASNString Fails \n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		
		// copy GGsnAddress
		pucStringValue = poCDRContents_i->oServiceInfo.oPSInformation.ucGGSNAddress;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNNodeAddress(hCGFModuleContext_i,&(poTempASRecord->gGSNaddress),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNNodeAddress() ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
	
		
		if( poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList.nNumberOfitemsInList > 0 )
		{
			t_MessageBodyList oMessageBodyList = poCDRContents_i->oServiceInfo.oIMSInformation.oMessageBodyList;
			if(Offline_CS_RetVal_Success != CGF_CreateASNMessageBodyList(hCGFModuleContext_i,
																		 (Message_Body_t*)&(poTempASRecord->list_Of_Message_Bodies),
																		 oMessageBodyList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNMessageBodyList() ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
	

        
		// fill event field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.ucEvent;
		nSize = strlen(pucStringValue);
		if(  nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempASRecord->event),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}
        
		// fill AccessNetworkInformation field of IMSRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucAccessNetworkInformation;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempASRecord->accessNetworkInformation),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		// fill ServiceContextId field of IMSRecord
		pucStringValue = poCDRContents_i->ucServiceContextId;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempASRecord->serviceContextID),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString()fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

        // fill List of Subscription-Id  field of SCSCFRecord
		t_SubscriptionId oSubscriptionId = poCDRContents_i->oServiceInfo.oSubscriptionId;
		pucStringValue = poCDRContents_i->oServiceInfo.oSubscriptionId.ucSubscriptionIdData;
		
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfSubscriptionId(hCGFModuleContext_i,
																			  (Subscription_Id_t*)&(poTempASRecord->list_of_subscription_ID),
																			  (poCDRContents_i->oServiceInfo.oSubscriptionId) ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfSubscriptionId() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}

 
		// fill list of early-SDP-Media-Components here 


		// fill IMS-Communication-service  identifier
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucIMSCommunicationServiceIdentifier;
		nSize = strlen(pucStringValue);
        if( nSize > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempASRecord->iMSCommunicationServiceIdentifier),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		
		// fill NumberPortabilityRoutingInformation field of SCSCFRecord
        pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucNumberPortabilityRoutingInfo;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempASRecord->numberPortabilityRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}
		

		// fill Carrier Select Routing Information field of SCSCFRecord
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucCarrierSelectRoutingInformation;
		nSize = strlen(pucStringValue);
		if( nSize > 0 )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,&(poTempASRecord->carrierSelectRouting),pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
		}


		
		// copy service-specific-info
		if(Offline_CS_RetVal_Success != CGF_CreateASNServiceSpecificInfoList(hCGFModuleContext_i,
																			 &(poTempASRecord->serviceSpecificInfo),
																			 poCDRContents_i->oServiceInfo.oIMSInformation.oServiceSpecificInfoList ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
        
        
		pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucRequestedPartyAddress;
		nSize = strlen(pucStringValue);
		if( 0 < nSize )
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNInvolvePartyAddress(hCGFModuleContext_i,
																			 &(poTempASRecord->requested_Party_Address),
																			 pucStringValue ) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNString() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}

		}

		
        // fill Called-Asserted-Identities field of SCSCFRecord
		oUTF8StringList = poCDRContents_i->oServiceInfo.oIMSInformation.oCalledAssertedIdentityList;
		if(oUTF8StringList.nNumberOfItemsInList > 0)
		{
			if(Offline_CS_RetVal_Success != CGF_CreateASNListOfInvolveParty(hCGFModuleContext_i,
																			&(poTempASRecord->list_Of_Called_Asserted_Identity),
																			oUTF8StringList) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"CGF_CreateASNListOfInvolveParty() fails ,Leaving\n");
				return Offline_CS_RetVal_Failure;
				
			}

		}
 
				// copy Inter-opertator identifier list to ASN Inter-opertator identifier list
		/*if(1 != CGF_CopyListOfSDPMediaCompToASNListOfSDPMediaComp(&(poTempASRecord->list_Of_SDP_Media_Components),
																  pTestString ) )
		{
			printf( " CGF_CreateASNListOfInterOpId() Fails \n");
			return;
		}*/




		// not found will have to add 
		//poCDRContents_i->oServiceInfo.oIMSInformation.
		/*if(1 != CGF_CreateASNString(&(poTempASRecord->serviceReasonReturnCode),pucStringValue ) )
		{
			printf( " CGF_CreateASNString Fails \n");
			return;
		}*/

		// copy Expires information
			if( DMBase_Retval_Success !=  DMBase_Malloc( hCGFModuleContext_i->hDiameterStackContext,
														 (void*)&(poTempASRecord->expiresInformation),sizeof(long)) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Memory Allocation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
			*(poTempASRecord->expiresInformation) = poCDRContents_i->oServiceInfo.oIMSInformation.oEventType.unExpires;


			// copy Alternate-Charged-Party-address
			pucStringValue = poCDRContents_i->oServiceInfo.oIMSInformation.ucAlternateChargedPartyAddress; 
			if( 0 < strlen(pucStringValue))
			{
				if(Offline_CS_RetVal_Success != CGF_CreateASNString(hCGFModuleContext_i,
																	&(poTempASRecord->alternateChargedPartyAddress),
																	pucStringValue ) )
				{
					//printf( " CGF_CreateASNString Fails \n");
					return Offline_CS_RetVal_Failure;
				}
	
			}


        //ASN_STRUCT_FREE(asn_DEF_InvolvedParty, ptestPart);
		
		//asn_fprint(stdout,&asn_DEF_InvolvedParty,ptestPart);

        //char a[12];
		//gets(a);
        
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success ;

}
/******************************************************************************
** Function Name: 
**			Db_GenerateResponseToRFmodule
**  
** Description:
**		This function is generating response to Rf Mudlue to inform 
**      about the status of opeation of cdr writing in file.
**
** Parameters:
**		  HCGFMODULECONTEXT    phCdrFWriterModuleContext_i
**			 type: input
**			 detail : This is a handle to DB Module Context.
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
t_Offline_CS_ReturnVal CGF_GenerateResponseToRFmodule( HCGFMODULECONTEXT        	phCGFModuleContext_i,
													   unsigned int					unAcctRecordNumber_i,
													   unsigned char*				pucSessionId_i,
													   unsigned int					unResultCode_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if( NULL == phCGFModuleContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"phCGFModuleContext_i is NULL \n");
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
		if( DMBase_Retval_Success != DMBASE_EvtSysSendExtEvt( phCGFModuleContext_i->hDiameterStackContext,
															  OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_RESPONSE_FROM_CGF,
															  (void* )&(oResponseDataToRFModule),
															  sizeof(t_ResponseEvtDataFromCdrFWModule) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in sending Operation Result Event to Rf Module\n");
			return Offline_CS_RetVal_Failure;
		}
        
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success; 
}

/******************************************************************************
** Function Name: 
**			CGF_ModuleCleanup
**  
** Description:
**		This function is a clean up function of CGFModule.
**		All the momory allocated to CGFModule is must to free here.
**
** Parameters:
**		  AMPS_HANDLE                 r_hAMPS_HANDLE
**			 type: input
**			 detail : this is handle to AMPS Frame work. 
**			
**		 AMPS_MODULE_HANDLE          r_hAMPS_MODULE_HANDLE
**			 type: input
**			 detail : this is handle to the CGFModule Context. 
** Return:
**		Its return value is void. 
*******************************************************************************
** Created By: 	Simab Shahid	
*******************************************************************************
*******************************************************************************
*/
void CGF_ModuleCleanup( AMPS_HANDLE 				r_hAMPS_HANDLE,
						AMPS_MODULE_HANDLE 		    r_hAMPS_MODULE_HANDLE)
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if( NULL == r_hAMPS_MODULE_HANDLE )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"r_hAMPS_MODULE_HANDLE is NULL \n");
            return ;
        }

		HCGFMODULECONTEXT      phCGFModuleContext = (HCGFMODULECONTEXT)r_hAMPS_MODULE_HANDLE;

		if( Offline_CS_RetVal_Success != CGF_DestroyModuleContext(&phCGFModuleContext))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in destroying CGF Module Context \n");
            return ;
		}
	
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return ;
}


