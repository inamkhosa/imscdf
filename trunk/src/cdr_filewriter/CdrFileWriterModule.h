#ifndef _CDRFILEWRITERMODULE_H
#define	_CDRFILEWRITERMODULE_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include "dappcommon_srvrconfload.h"

	#define OFFLINE_CS_NO_OF_CDR_FILE_WRITER_THREAD		1
	
    
	typedef struct CdrFileObject
	{
		FILE*			pFileHandle;// --
		unsigned char*  pucFileName;// --
		unsigned int 	unFileNameCounter;//TODO: remove 
		unsigned int 	unFileRunningCount;
		unsigned int 	unFileSequenceNumber;
        unsigned int 	unCdrCounter;// --
       
	}t_CdrFileObject;

    
	typedef struct CdrFileWriterModuleContext
	{
	   DMBASE_IOAGENTS_HANDLE		hIOAgentsHandle;
	   t_cdf_config_params			ocdf_config_params;
       HDIAMETERSTACKCONTEXT        hDiameterStackContext;
	}t_CdrFileWriterModuleContext ;
	

	typedef    t_CdrFileWriterModuleContext*      HCDRFILEWRITERMODULECONTEXT;


	typedef struct CdrFileWriterThreadContext
	{
       HCDRFILEWRITERMODULECONTEXT	hMainCDRModContext;
       AMPS_UIOAGENT_HANDLE			hUnitIOAgent;
	   t_CdrFileObject				oCdrFileObject;
	   unsigned int 				unEventsRecvdFromCGFCounter; // DEBUG
	   unsigned int 				unEventsSendToCGFCounter; // DEBUG
	}t_CdrFileWriterThreadContext ;

	typedef    t_CdrFileWriterThreadContext*      HCDRFILEWRITERTHREADCONTEXT;


	

   
   t_Offline_CS_ReturnVal CdrFW_CreateAndInitModuleContext(HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o,
														   HDIAMETERSTACKCONTEXT    		   hDiameterStackContext_i);

   t_Offline_CS_ReturnVal CdrFW_RegisterForModuleEvents( HCDRFILEWRITERMODULECONTEXT       hCdrFWriterModuleContext_o);

   t_Offline_CS_ReturnVal CdrFW_DestroyModuleContext(HCDRFILEWRITERMODULECONTEXT        phCdrFWriterModuleContext_o);
   
   
   void  				  CdrFW_EventHandlerForLogCdrFromCGF(AMPS_HANDLE r_hAMPSHandle, void* r_pvEventData,
															 void* r_pvEventHandle, void* r_pvEventPayload);


   t_Offline_CS_ReturnVal CdrFW_GenerateResponseToCGF( HCDRFILEWRITERMODULECONTEXT    phCdrFWriterModuleContext_i,
													   unsigned int					  unAcctRecordNumber_i,
													   unsigned char*				  pucSessionId_i,
													   unsigned int					  unResultCode_i);


  
   // will remove after initialization as IOAgent 
   int	CdrFW_EventValidationCallback(   AMPS_APP_HANDLE 	hAMPS_APP_HANDLE_i,
										 int 				nEventID_i, 
										 void* 				pvEventPayload_i);

   void* CdrFW_ThreadInit(AMPS_HANDLE hAMPSHandle_i, void* hUIOAgentsHandle_i, void* pvUserData_i);
   void CdrFW_ThreadCleanup(void* pvCdrFWriterThreadCtxt_i);



  t_DiamAPIReturnVal  DMBase_FRead(HDIAMETERSTACKCONTEXT    hDiameterContext_i,
								   void* 					pvDestination_o,
								   unsigned int 			unSizeOfBlocks_i, 
								   unsigned int 			unNumberOfBlocks_i,
								   FILE* 					pfile_i,
								   unsigned int*			punNumberOfBlocksRead_o);

  t_DiamAPIReturnVal  DMBase_FWrite(HDIAMETERSTACKCONTEXT    	hDiameterContext_i,
									void* 						pvSource_o,
									unsigned int 				unSizeOfBlocks_i, 
									unsigned int 				unNumberOfBlocks_i,
									FILE* 						pfile_i,
									unsigned int*				punNumberOfBlocksWritten_o);
  
   t_DiamAPIReturnVal DMBase_IOAgentsInit(HDIAMETERSTACKCONTEXT    			hDiameterContext_i,
									   int 									nNoOfThreads_i, 
									   DMBASE_IOAgentsDispatcherCallback 	pf_IOAgentsDispatcherCallback_i,
									   DMBASE_IOAgentsInitCallback 			pf_IOAgentsInitCallback_i,
									   DMABSE_IOAgentsCleanupCallback 		pf_IOAgentsCleanupCallback_i, 
									   void* 								pvUserData_i,
									   DMBASE_IOAGENTS_HANDLE*		 		phIOAgentsHandle_o);

   
   t_DiamAPIReturnVal DMBASE_IOAgentsSendEvt( DMBASE_IOAGENTS_HANDLE	  hIOAgentsHandle_i, 
											  int                          nEventType,
											  void*                        pvEventData, 
											  unsigned int                 unSizeofEventData);


   // base Wrapper
   t_DiamAPIReturnVal DMBase_GetCurrentTime(HDIAMETERSTACKCONTEXT    	hDiameterContext_i,
										 t_AMPSTimerValue*			poTimerValue_i);

   
   /******************************************************************************
** Function Name: 
**			DMBASE_EvtSysSendIntEvt
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
/*t_DiamAPIReturnVal DMBASE_IOAgentsSendEvt( HDIAMETERSTACKCONTEXT    hDiameterContext_i, 
                                           int             			nEventType_i,
                                           void*           			pvEventData_i, 
                                           unsigned int    			unSizeofEventData_i)
{
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

        
        if( (NULL == pvEventData_i)&&(unSizeofEventData_i >0) )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ERROR!Either unSizeofEventData_i or pvEventData_i is invalid parameter, Leaving \n");
            return DMBase_Retval_IncorrectParameters ;
        }

        if(AMPS_SUCCESS != AMPS_EvtSysSendIntEvt(hDiameterContext_i->hFrameworkHandle, 
                                                 nEventType_i, 
                                                 pvEventData_i, 
                                                 unSizeofEventData_i))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"ERROR! AMPS_EvtSysSendIntEvt Fails, Leaving \n");
            return DMBase_Retval_Failure ;
        }

		AMPS_API int AMPS_IOAgentsSendEvt(AMPS_UIOAGENT_HANDLE r_hAMPS_UIOAGENT_HANDLE, int r_nEventType, void* r_pvEventData, int r_nSizeOfEventData);

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Leaving \n");
        return DMBase_Retval_Success ;
}*/
  


   
   
#ifdef	__cplusplus
}
#endif

#endif	/* _CDRFILEWRITERMODULE_H */

