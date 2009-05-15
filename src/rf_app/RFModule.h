
/******************************************************************************
** Written by Simab Shahid
**
** Copyright (C) 2009 AdvancedIMS
**
** All rights reserved.
** 
*******************************************************************************
** FILE NAME      		: RF_Module.h
** DESCRIPTION    		: The file contains defines, data structures and function
						  prototypes for Rf Module of Offline Charging Server.
** PROJECT NAME   		: 
** TYPE           		: ANSI C
** CONTRIBUTOR(S) 		: Simab Shahid
*******************************************************************************
** $Id: AMPS_Defines.h,v 1.30 2008/04/01 12:40:39 zaka Exp $
** $Source: /work/cvsroot/amps/include/AMPS_Defines.h,v $
*******************************************************************************
*******************************************************************************
*/


// Changes to TODO: Must be updated the DMBASE_GetNodeData(); As Corrected Now

#ifndef _RFMODULE_H
#define	_RFMODULE_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include"RF_defines.h"
	#include"RF_include.h"
	#include "offcs_com_cdr_contents.h"



    

 typedef struct RFApplicationContext
 {
     _Bool					  bLoggingInDBEnabled;
	 _Bool					  bLoggingInCdrFileEnabled;
     AMPS_HANDLE              hAMPSHandle;
	 DMBase_SList*            pListOfSessionTableKey;
     HDIAMETERSTACKCONTEXT    hDiameterStackContext;  
     DMBASE_HASH_HANDLE       HandleToSessionTable;
	 FILE*					  fptr;
	 unsigned int			  uncounter;
	 unsigned int 			  unReceiveCallBackCounter; // Gebug
	 unsigned int 			  unDiscardedMessageCounter; // Gebug
	 unsigned int 			  unSuccesfulMessageCounter; // Gebug
	 unsigned int 			  unSendEventToCGFCounter; // Gebug	
	 unsigned int 			  unRecvdfrmCGFEventCounter; // Gebug
	 unsigned int 			  unStartMsgCounter; // Gebug
	 unsigned int 			  unStopMsgCounter; // Gebug
	 unsigned int 			  unInvalidMsgCounter; // Gebug
	 unsigned int 			  unSessionCreateCounter; // Gebug
	 unsigned int 			  unSessionDeleteCounter; // Gebug
	 unsigned int 			  unAddMsgToExistingSession; // Gebug
	 unsigned int 			  unStopDiscarded; // Gebug
	 unsigned int 			  unStartDiscarded; // Gebug

    
 }t_RFApplicationContext ;

 typedef     t_RFApplicationContext* HRFAPPLICATIONCONTEXT;

 	typedef struct TimerDataObject
 {
	 unsigned int 					 unAcctRecordNumber;
	 unsigned char					 pucSessionId[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 HRFAPPLICATIONCONTEXT			 hRfModuleContext;
     
 }t_TimerDataObject;


 typedef struct ACRInfo //ACRInfo
 {
     unsigned char		        ucFlags;
     unsigned int            	unHopByHopID;
     unsigned int            	unEndToEndID;
     int		             	nAcctRecordType;
     unsigned int		       	unAcctRecordNumber;
     unsigned int       		unAcctApplicationID;
  }t_ACRInfo;

  typedef struct SessionObject
 {
     AMPS_TIMER_ID          	TimerID;
     HPEERTABLEENTRY	    	hPeerTabEntry;
     unsigned char	     		ucSessionID[OFFLINE_CS_MEDIUM_STRING_SIZE];
	 t_ACRInfo* 				poLastReceivedACRInfo;
     DMBase_SList*          	pListOfUnprocessedACRInfo;     
	 t_TimerDataObject*			poTimerDataForCurrSess;
 }t_SessionObject;


 typedef struct ObjectOfAVPDetails
 {
	 unsigned int 			unAVPCount;
     HAVP 					hAVP_Validation;
 }t_ObjectOfAVPDetails;


 
 t_Offline_CS_ReturnVal Rf_CreateAndInitModuleContext( HRFAPPLICATIONCONTEXT*     phRFApplicationContext_o,
													   HDIAMETERSTACKCONTEXT      hDiameterContext_i);

 t_Offline_CS_ReturnVal Rf_RegisterForModuleEvents(HRFAPPLICATIONCONTEXT    hRFModuleContext_i);

 t_DiamAPIReturnVal  Rf_ProcessACRMsg( HPEERTABLEENTRY		hPeer_i, 
                                       HDIAMETERMESSAGE     ACRMsg_i,
                                       void*                hRFModuleContext_i);

 t_Offline_CS_ReturnVal  Rf_GenerateInterimACAMsg( HRFAPPLICATIONCONTEXT    hRFModuleContext_i,
												   HPEERTABLEENTRY			 hPeer_i,
												   t_MessageStructure		 oMessageStructure_i );


 t_Offline_CS_ReturnVal Rf_FillMessageStructure( HRFAPPLICATIONCONTEXT          hRFModuleContext_i,
												 HDIAMETERMESSAGE				hACRMsg_i,
												 t_MessageStructure* 			poMessageStructure_i);

 t_Offline_CS_ReturnVal Rf_AddServiceInfoAVPToCdrContents( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
														   HAVP					  	hToAVP_ACR_i,
														   t_MessageAVPsStructure*	poMessageAVPsStructure_o);

 t_Offline_CS_ReturnVal Rf_AddSubscriptionIdAVPToCdrContents( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
															  HAVP					  	hToAVP_ACR_i,
															  t_MessageAVPsStructure*	poMessageAVPsStructure_o);
 
 t_Offline_CS_ReturnVal inline Rf_CopyDiamStringToCharArray( unsigned char* 	pucSourceString_o,
															 unsigned int 		unDestinationStrSize_i,
															 unsigned char* 	pucDiamSourceString_i,
															 unsigned int 		unSourceStrSize_i );

 t_Offline_CS_ReturnVal inline Rf_CopyOctetStringToStringList( t_UTF8StringList* 	poOctetStringList_o,
															   unsigned int 		unDestinationStrSize_i,
															   unsigned char* 		pucDiamSourceString_i,
															   unsigned int 		unSourceStrSize_i );

 t_Offline_CS_ReturnVal Rf_ValidateACRMessage( HRFAPPLICATIONCONTEXT          hRFModuleContext_i,
                                               HDIAMETERMESSAGE       	      hACRMsg_i,
											   HPEERTABLEENTRY				  hPeer_i);

 t_Offline_CS_ReturnVal Rf_ValidateACRMsgAVPs(	HRFAPPLICATIONCONTEXT			  hAppContext,
												HDIAMETERMESSAGE                  hDiamMsg_i,
												HPEERTABLEENTRY				      hPeer_i);

 t_Offline_CS_ReturnVal  Rf_GetAVPDetailsforValidation(	HRFAPPLICATIONCONTEXT			  	hRFModuleContext_i,
														HDIAMETERMESSAGE                	hACRMsg_i,
														unsigned int 						unAVPCode_i,
														t_ObjectOfAVPDetails*				pOAVPsDetails_i);

 t_Offline_CS_ReturnVal Rf_AddExpectedAVPToList( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
											     unsigned int 				unAVPCode_i,
											     DMBase_SList**     		ppoFailedAVPsList_i);

 t_Offline_CS_ReturnVal Rf_AddAVPToFailedAVPList( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
												  DMBase_SList**     		ppoFailedAVPsList_i,
												  unsigned int 				unAVPCode_i,
												  HAVP						hFirstOccurrenceOfAVP_i);
 
 t_Offline_CS_ReturnVal Rf_GenerateACAMsg( HRFAPPLICATIONCONTEXT     hRFModuleContext_i,
										   HPEERTABLEENTRY			 hPeer_i,
                                           int                       unRecultCode_i,
										   unsigned char*	         pucSessionID_i,
										   t_ACRInfo*                poACRInfo_i);

 void  Rf_CGFResponseEvtHandler( AMPS_HANDLE 		hAMPS_HANDLE_i,
								 AMPS_APP_HANDLE 	hAMPS_APP_HANDLE_i, 
								 void* 				pvEventHandle_i,
								 void* 				pvEvtData_i);

 
int	  Rf_EvtValidationCallback( AMPS_APP_HANDLE         r_hAMPS_APP_HANDLE,
                                int                     r_nEventID,
                                void*                   r_pvEventPayload);

 int   Rf_InterimIntervalExpiryNotifyHandler( void*            pvHandle_i, 
											  void*            pvData_i);

t_Offline_CS_ReturnVal Rf_SetTimerIDInSessionObject(HRFAPPLICATIONCONTEXT            hRFModuleContext_i,
                                                    t_SessionObject*      			 poSessionObject_i,
                                                    AMPS_TIMER_ID                    TimerID_i,
													t_TimerDataObject* 				 poTimerData_i );

t_Offline_CS_ReturnVal  Rf_KillSessionTimer(HRFAPPLICATIONCONTEXT   hRFModuleContext_i,
                                            t_SessionObject*        poSessionObject);


t_Offline_CS_ReturnVal Rf_CreateACRInfo( HRFAPPLICATIONCONTEXT   	        hRFModuleContext_io ,
                                         t_MessageStructure	  	 			oMessageStructure_i,
                                         HPEERTABLEENTRY		            hPeerTabEntry_i,
                                         t_ACRInfo**       	                ppSessionRecord_o);

// Session Table Functions Prototype

t_Offline_CS_ReturnVal Rf_CreateSessionTable( HRFAPPLICATIONCONTEXT		  hRFModuleContext_io);
t_Offline_CS_ReturnVal Rf_DeleteSessionTable( HRFAPPLICATIONCONTEXT       hRFModuleContext_i);
t_Offline_CS_ReturnVal Rf_AddACRInfoToExistingSession( HRFAPPLICATIONCONTEXT		 hRFModuleContext_i,
													   t_SessionObject*	      	 	 poSessionObject_o,
													   t_ACRInfo*		             poACRInfo_i);
t_Offline_CS_ReturnVal Rf_AddACRInfoToNewSession( HRFAPPLICATIONCONTEXT		        	hRFModuleContext_i,
												  DMBASE_HASHTABLE_KEY_HANDLE       	hSessionTableKey_i,
												  unsigned char*	                  	pucSessionID_i,
												  HPEERTABLEENTRY	                    hPeerTableEntry_i,
												  t_SessionObject**						ppoSessionObject_o,
												  unsigned int                      	unSessionRecordNumber,
												  t_ACRInfo*		       	            pACRInfo_i);

t_Offline_CS_ReturnVal Rf_AddACRInfoToSessionObject( HRFAPPLICATIONCONTEXT		 hRFModuleContext_i,
                                                     t_SessionObject*	      	 poSessionObject_o,
                                                     t_ACRInfo*		             poACRInfo_i);


t_Offline_CS_ReturnVal Rf_SetSessionIDInSessionObject( HRFAPPLICATIONCONTEXT	 hRFModuleContext_io,
                                                       t_SessionObject*	         poSessionObject_o,
                                                       t_DiamOctetString*        pOSessionID_i);

t_Offline_CS_ReturnVal Rf_DestroySessionObject( HRFAPPLICATIONCONTEXT	   hRFModuleContext_i,
                                                t_SessionObject**		   poSessionObject_i);




t_Offline_CS_ReturnVal Rf_SearchACRInfoInSessionObject( HRFAPPLICATIONCONTEXT		hRFModuleContext_i,
                                                        t_SessionObject*	     	poSessionObject_i,
                                                        unsigned int 			    unAcctRecordNumber,
                                                        t_ACRInfo**		            ppoACRInfo_o,
                                                        _Bool*				        pbRecordFound_o);

t_Offline_CS_ReturnVal Rf_SearchACRInfoNodeInSessionObject( HRFAPPLICATIONCONTEXT    	hRFModuleContext_i,
                                                            t_SessionObject*	        poSessionTableEntry_i,
                                                            unsigned int 				unAcctRecordNumber_i,
                                                            _Bool*					    pbRecordFound_o,
                                                            DMBase_SList**              ppoListNodeFound_o);


t_Offline_CS_ReturnVal Rf_SearchObjectInSessionTable( HRFAPPLICATIONCONTEXT		        hRFModuleContext_io,
                                                      DMBASE_HASHTABLE_KEY_HANDLE		hSessionTableKey_i,
                                                      t_SessionObject**		            ppoSessionObject_o);



t_Offline_CS_ReturnVal Rf_InsertObjectToSessionTable( HRFAPPLICATIONCONTEXT		            hRFModuleContext_i,
                                                      DMBASE_HASHTABLE_KEY_HANDLE     	    hSessionTableKey_i,
                                                      t_SessionObject*            	        poSessionTableObject_i);


t_Offline_CS_ReturnVal Rf_DeleteACRInfoFromSessionTable( HRFAPPLICATIONCONTEXT                  g_hRFModuleContext_io,
														 t_SessionObject*						poSessionObject_i,
														 DMBASE_HASHTABLE_KEY_HANDLE            hSessionTableKey,
														 unsigned int                           unSessionRecodrNumber_i);

t_Offline_CS_ReturnVal Rf_DeleteSession( HRFAPPLICATIONCONTEXT		    hRFModuleContext_io,
                                         t_SessionObject*       	    poSessionObject_i,
										 t_DiamHashTableKey*			poDiamHashTableKey_i );




int Rf_LListFreeLinkDataCallback( void**    ppvDatToFree);

void  Rf_SessionTableProcessNodeDataCallback( AMPS_HANDLE 			r_hAMPS_HANDLE,
											  AMPS_APP_HANDLE 		r_hAMPS_APP_HANDLE,
											  AMPS_APP_DATA_HANDLE 	r_hAMPS_APP_DATA_HANDLE);



////// Should move to Base Protocol , as are AMPS wrappers will move to base protocol 



t_DiamAPIReturnVal DMBASE_EvtSysSendExtEvt(HDIAMETERSTACKCONTEXT		hDiameterContext_i, 
                                           int                          nEventType,
                                           void*                        poEventData, 
                                           unsigned int                 unSizeofEventData);


#ifdef	__cplusplus
}
#endif

#endif	/* _RFMODULE_H */

 
 

