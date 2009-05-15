#ifndef _CGF_H
#define	_CGF_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include "IMSRecord.h"

    
	typedef struct CGFModuleContext
	{
       HDIAMETERSTACKCONTEXT        hDiameterStackContext;
	   unsigned int 			    unSendEventToRFCounter; // Gebug	
	   unsigned int 			    unRecvdfrmRFEventCounter; // Gebug
	   unsigned int 			    unSendEventToFileWriterModuleCounter; // Gebug	
	   unsigned int 			    unRecvdfrmFileWriterModuleEventCounter; // Gebug
	}t_CGFModuleContext ;

	typedef    t_CGFModuleContext*      HCGFMODULECONTEXT;

	typedef struct SCSCFRecord__list_of_subscription_ID*				Subscription_Id_t;
	typedef struct SCSCFRecord__applicationServersInformation*			Appserver_List_t;
	typedef struct SCSCFRecord__list_Of_Message_Bodies*					Message_Body_t;

   
   t_Offline_CS_ReturnVal CGF_CreateAndInitModuleContext(HCGFMODULECONTEXT*        phCdrFWriterModuleContext_o,
														 HDIAMETERSTACKCONTEXT     hDiameterStackContext_i);

   t_Offline_CS_ReturnVal CGF_RegisterForModuleEvents( HCGFMODULECONTEXT       hCdrFWriterModuleContext_o);

   t_Offline_CS_ReturnVal CGF_DestroyModuleContext(HCGFMODULECONTEXT*        phCdrFWriterModuleContext_o);

   
   void  CGF_EvtHandlerForRFModule( AMPS_HANDLE                           hAMPS_HANDLE,
									DB_MODULE_LOCAL_CONTEXT_HANDLE        hModule_Context_Handle_i, 
									void*                                 r_pvEventHandle_i, 
									void*                                 pvDataForCGFModule_i);

   void  CGF_EvtHandlerForCdrFWriter( AMPS_HANDLE 		    	hAMPS_HANDLE_i,
									  AMPS_APP_HANDLE 	    	hAMPS_APP_HANDLE_i, 
									  void* 			    	pvEventHandle_i,
									  void* 			    	pvEvtData_i);

   void  CGF_EvtHandlerForDBModule( AMPS_HANDLE 		    	hAMPS_HANDLE_i,
								   AMPS_APP_HANDLE 	    		hAMPS_APP_HANDLE_i, 
								   void* 			    		pvEventHandle_i,
								   void* 			    		pvEvtData_i);

   static int CGF_Write_EncodedContents(const void *buffer, size_t size, void *app_key);

   t_Offline_CS_ReturnVal inline CGF_CreateEncodedCDR( HCGFMODULECONTEXT 	hCGFModuleContext_i,
													   t_CDR_Contents*		poCDRContents_i,
													   t_EncodedCDR* 		poEncodedCDR);

  t_Offline_CS_ReturnVal inline CGF_CreateASN_SCSCFReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
														   t_CDR_Contents*		poCDRContents_i,
														   SCSCFRecord_t** 		ppoScscfRecord_o );

   t_Offline_CS_ReturnVal inline CGF_CreateASN_PCSCFReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
															t_CDR_Contents*		poCDRContents_i,
															PCSCFRecord_t** 		ppoPcscfRecord_o);

   t_Offline_CS_ReturnVal inline CGF_CreateASN_ICSCFReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
															t_CDR_Contents*		poCDRContents_i,
															ICSCFRecord_t** 		ppoIcscfRecord_o );

   t_Offline_CS_ReturnVal inline CGF_CreateASN_MRFCReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
														   t_CDR_Contents*		poCDRContents_i,
														   MRFCRecord_t** 		ppoMrfcRecord_o );

   t_Offline_CS_ReturnVal inline CGF_CreateASN_ASReocrd( HCGFMODULECONTEXT 	hCGFModuleContext_i,
														 t_CDR_Contents*	poCDRContents_i,
														 ASRecord_t** 		ppoAsRecord_o );

   t_Offline_CS_ReturnVal inline CGF_CreateASNInvolvePartyAddress(HCGFMODULECONTEXT		hCGFModuleContext_i,
																  InvolvedParty_t** 	ppInvolvedParty_o,
																  unsigned char*		pucStringValue_i );
   
   t_Offline_CS_ReturnVal CGF_GenerateResponseToRFmodule( HCGFMODULECONTEXT         phCdrFWriterModuleContext_i,
															unsigned int			unAcctRecordNumber_i,
															unsigned char*			pucSessionId_i,
															unsigned int			unResultCode_i);

   // will remove after initialization as IOAgent 
   int	CGF_EventValidationCallback(   AMPS_APP_HANDLE 	hAMPS_APP_HANDLE_i,
									   int 				nEventID_i, 
									   void* 			pvEventPayload_i);

   t_Offline_CS_ReturnVal inline CGF_CreateASNString( HCGFMODULECONTEXT 	hCGFModuleContext_i,
													  OCTET_STRING_t** 		ppASNOctetString_o,
													  unsigned char* 		pucStringValue_i);


   t_Offline_CS_ReturnVal inline CGF_CreateASNServiceSpecificInfoList(HCGFMODULECONTEXT							hCGFModuleContext_i,
																	  struct ASRecord__serviceSpecificInfo**	ppoServiceSpecificInfo_o,
																	  t_ServiceSpecificInfoList					oServiceSpecificInfoList_i);

   t_Offline_CS_ReturnVal inline CGF_CreateASNInt( HCGFMODULECONTEXT 			hCGFModuleContext_i,
												 LocalSequenceNumber_t**	ppoLocalSequenceNumber_o,
												 int		 				nLocalSequenceNumber_i);
  
   t_Offline_CS_ReturnVal inline CGF_CreateASNListOfInterOpId(HCGFMODULECONTEXT 				hCGFModuleContext_i,
															  InterOperatorIdentifierlist_t**  	ppInterOperatorIdentifierlist_o,
															  t_InterOperatorIdentifierList		InterOptIdList_i);

   t_Offline_CS_ReturnVal inline CGF_CreateASNMessageBodyList(HCGFMODULECONTEXT 							hCGFModuleContext_i,
															  struct SCSCFRecord__list_Of_Message_Bodies 	**ppMessageList_o,
															  t_MessageBodyList 							oMessageBodyList_i);

   t_Offline_CS_ReturnVal inline CGF_CreateASNListOfSubscriptionId(HCGFMODULECONTEXT 								hCGFModuleContext_i,
																   struct SCSCFRecord__list_of_subscription_ID** 	pplistofsubscriptionID_o,
																   t_SubscriptionId 								oSubscriptionId_i );

   t_Offline_CS_ReturnVal inline CGF_CreateASNAppServerInfoList(HCGFMODULECONTEXT 										hCGFModuleContext_i,
																struct SCSCFRecord__applicationServersInformation** 	ppAppServersInfo_o,
																t_AppServerInfoList								 		oAppServerInfoList);
   
   t_Offline_CS_ReturnVal inline CGF_CreateASNTimeStamp(HCGFMODULECONTEXT 	hCGFModuleContext_i,
														TimeStamp_t** 		pTimeStamp_o,
														unsigned int 		unTimeStamp_i);

   t_Offline_CS_ReturnVal inline CGF_CreateASNListOfSDPMediaComp(HCGFMODULECONTEXT											hCGFModuleContext_i,
																 struct SCSCFRecord__list_Of_SDP_Media_Components** 	pplistOfSDPMediaComponents_o,
																 t_SDPMediaComponentList								oSDPMediaComponentList_i);

   t_Offline_CS_ReturnVal inline CGF_FreeASNListOfMessageBody(HCGFMODULECONTEXT								hCGFModuleContext_i,
															  struct SCSCFRecord__list_Of_Message_Bodies 	**ppMessageList_o );

   t_Offline_CS_ReturnVal inline CGF_FreeASNListOfSubscriptionId(HCGFMODULECONTEXT								hCGFModuleContext_i,
																 struct SCSCFRecord__list_of_subscription_ID** 	pplistofsubscriptionID_o );

   
   
#ifdef	__cplusplus
}
#endif

#endif	/* _CGF_H */


