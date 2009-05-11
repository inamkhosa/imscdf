extern int g_nDiamBaseTraceID;


#include "rf_msg_api.h"



/******************************************************************************
** Function Name: 
**			RFAPI_CreateAccountingRequest
**  
** Description:
**		This function returns a hnadle to newly created ACA message 
**      in response of Valid ACR Message Received by RF Application
**      
**
** Parameters:
**		  HDIAMETERSTACKCONTEXT hDiameterStackContext_io
**			 type: input/output
**			 detail : this is a RF Application context handle. 
**
**		 HDIAMETERMESSAGE*              phDiaMsgACA_o
**			 type: Output
**			 detail : this is a pointer to Handle to the ACA message
**                   to be returned.
**
**		 unsigned char                  ucDiaMsgVersion_i,
**			type: input
**			detail: this is a parameter contains the newly pointer which points to 
**          the the data of Result Code AVP
**          
**      unsigned int                   unDiaMsgCommandCode_i 
**          type: input
**			detail: this parameter contains  
**       
** Return:
**		An t_DiamAPIReturnVal value specifung wheather the function is 
**     is returnuing successfully or any problem occured.
**
** 
*******************************************************************************
** Created By: 		: 
*******************************************************************************
*******************************************************************************
*/
RfAPIReturnVal_t RFAPI_CreateAccountingRequest(HDIAMETERSTACKCONTEXT       hDiameterStackContext_io , 
                                               HDIAMETERMESSAGE*           phDiaMsgACR_o,
                                               unsigned char              ucDiaMsgVersion_i,
                                                unsigned int               unDiaMsgCommandCode_i,  
                                                unsigned int               unDiaMsgApplicationID_i, 
                                                t_ACR_Params                oACR_Param_i )
{

    _Bool           bFlagValue = TRUE;
    HAVPCOLLECTION	pACR_AVPCollection = NULL;    


   
    // AcctSessionID Mandatory AVP
    if( NULL == oACR_Param_i.hAVP_SessionID )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter hAVP_AcctSessionID  is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {        
       DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AcctSessionID );
        
    }

     // OriginHost Mandatory AVP
    if( NULL == oACR_Param_i.hAVP_OriginHost )
    {
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter hAVP_OriginHost  is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {        
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_OriginHost );
    }

     // OriginRealm Mandatory AVP
    if( NULL == oACR_Param_i.hAVP_OriginRealm )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter hAVP_OriginRealm is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {
       DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_OriginRealm  );
    }

    // Destination Realm Mandatory AVP
    if( NULL == oACR_Param_i.hAVP_DestinationRealm )
    {
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " oACR_Param_i.hAVP_DestinationRealm is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_DestinationRealm  );
    }

     // AccountingRecordType Mandatory AVP
    if( NULL == oACR_Param_i.hAVP_AccountingRecordType )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter hAVP_AccountingRecordTypeis NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AccountingRecordType );
    }

     //AccountingRecordNumber Mandatory AVP
    if( NULL == oACR_Param_i.hAVP_AccountingRecordNumber )
    {
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter hAVP_AccountingRecordNumber is NULL , Leaving\n");
         return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {       
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AccountingRecordNumber );
    }


    //  one of AcctApplicationID and  VendorSpecificApplicationID AVPs must be present    
    if( (NULL == ( oACR_Param_i.hAVP_AcctApplicationID ) )&& (NULL== ( oACR_Param_i.hAVP_VendorSpecificApplicationID ) ) )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter hAVP_AcctApplicationID or hAVP_VendorSpecificApplicationID must not be NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ; 
    }
    else 
    {
        // if AcctApplicationID AVP is present Add it to List
        if ( NULL != oACR_Param_i.hAVP_AcctApplicationID ) 
        {
            DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*)oACR_Param_i.hAVP_AcctApplicationID);
        }

        // if VendorSpecificApplicationID AVP is present Add it to AVPList
        if ( NULL != oACR_Param_i.hAVP_VendorSpecificApplicationID )
        {
            DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_VendorSpecificApplicationID );
        }

    }


    // if UserName optional AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_UserName )
    {
       DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_UserName );
    }


    // if AccountingSubSessionID optional AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_AccountingSubSessionID )
    {
         DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AccountingSubSessionID  );
    }


    // if AcctSessionID AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_AcctSessionID )
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AcctSessionID );
    }

    // if AcctMultiSessionID optional AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_AcctMultiSessionID )
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AcctMultiSessionID);
    }

    // if AcctInterimInterval optional AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_AcctInterimInterval )
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AcctInterimInterval );
    }

    // if AccountingRealtimeRequired optional AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_AccountingRealtimeRequired )
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_AccountingRealtimeRequired );
    }


    // if OriginStateID optional AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_OriginStateID )
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_OriginStateID );
    }

    // if EventTimestamp  optional AVP is present Add it to AVPList
    if(NULL != oACR_Param_i.hAVP_EventTimestamp )
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) oACR_Param_i.hAVP_EventTimestamp );
    }

    // Traverse the list of  ProxyInfoAVPs  
    if(NULL != oACR_Param_i.pListOfProxyInfoAVPs )
    {
        void*				pData = NULL;
        HAVPCOLLECTION		pCurrentNode = oACR_Param_i.pListOfProxyInfoAVPs;

        while (NULL != pCurrentNode) 
        {
            DMBase_SListGetNodeData(hDiameterStackContext_io, pCurrentNode, &pData);
            DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) &pData) ;            
        }
    }


    // Traverse the list of  RouteRecordAVPs 
    if(NULL != oACR_Param_i.pListOfRouteRecordAVPs )
    {
        void*				pData = NULL;
        HAVPCOLLECTION		pCurrentNode = oACR_Param_i.pListOfRouteRecordAVPs;

        while (NULL != pCurrentNode) 
        {
            DMBase_SListGetNodeData(hDiameterStackContext_io, pCurrentNode, &pData);
            DMBase_AppendSList( hDiameterStackContext_io, &pACR_AVPCollection, (void*) &pData);            
        }
    }

        
    /*if( RfAPI_Retval_Success != DMBase_CreateDiameterMessage( hDiameterStackContext_io , phDiaMsgACR_o ,
                                                              ucDiaMsgVersion_i ,        unDiaMsgCommandCode_i ,  
                                                              unDiaMsgApplicationID_i ) )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACR Message Creation fail , Leaving \n");
        return RfAPI_Retval_Failure ;
    }*/


   if( RfAPI_Retval_Success != DMBase_SetDiaMsgRFlag( hDiameterStackContext_io, *phDiaMsgACR_o, bFlagValue ) )
   {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Message Header P-flag setting fail , Leaving \n");
        return RfAPI_Retval_Failure ; 
   }


   if( RfAPI_Retval_Success != DMBase_AddAVPCollectionToDiameterMessage(hDiameterStackContext_io,*phDiaMsgACR_o, pACR_AVPCollection) )
   {
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding AVP collection to ACR Message, Leaving \n");
       return RfAPI_Retval_Failure ; 
   }

   return RfAPI_Retval_Success; 
}



RfAPIReturnVal_t RFAPI_CreateAccountingAnswer( HDIAMETERSTACKCONTEXT          hDiameterStackContext_io ,
                                               HDIAMETERMESSAGE*              phDiaMsgACA_o,
                                               t_ACA_Params                   oACA_Param_i,
											   unsigned char 				  ucFlags_i,
											   unsigned int  				  unHopByHopID_i,
										       unsigned int  				  unEndToEndID_i)
{

    _Bool           bFlagValue = TRUE;
    HAVPCOLLECTION	pACA_AVPList = NULL;
    unsigned int    unDiaMsgCommandCode     = RF_MSG_CMD_CODE_ACR ;
    unsigned int    unDiaMsgApplicationID   = RF_APPLICATION_ID;
    unsigned char   ucDiaMsgVersion         = RF_MSG_VERSION;

   
     // AcctSessionID Mandatory AVP
    if( NULL == oACA_Param_i.hAVP_SessionID )
    {
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter oACA_Param_i.hAVP_AcctSessionID   is NULL , Leaving\n");
         return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_SessionID);
        
    }
    
     // ResultCode Mandatory AVP
    if( NULL == oACA_Param_i.hAVP_ResultCode )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter oACA_Param_i.hAVP_ResultCode  is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {   
        DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_ResultCode);
        
    }

     // OriginHost Mandatory AVP
    if( NULL == oACA_Param_i.hAVP_OriginHost )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter oACA_Param_i.hAVP_OriginHost  is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {   
        DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_OriginHost );
    }

     // OriginRealm Mandatory AVP
    if( NULL == oACA_Param_i.hAVP_OriginRealm )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter oACA_Param_i.hAVP_OriginRealm is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {
        DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_OriginRealm  );
    }

     // AccountingRecordType Mandatory AVP
    if( NULL == oACA_Param_i.hAVP_AccountingRecordType )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter oACA_Param_i.hAVP_AccountingRecordType is NULL , Leaving\n");
        return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {   
        DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AccountingRecordType );
    }

     // AccountingRecordNumber Mandatory AVP
    if( NULL == oACA_Param_i.hAVP_AccountingRecordNumber )
    {
         DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter oACA_Param_i.hAVP_AccountingRecordNumber is NULL , Leaving\n");
         return RfAPI_Retval_Null_Pointer ;  
    }
    else
    {  
        DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AccountingRecordNumber );
    }


    //  one of AcctApplicationID and  VendorSpecificApplicationID AVPs must be present    
    if( (NULL == ( oACA_Param_i.hAVP_AcctApplicationID )))
         // && ( NULL==oACA_Param_i.hAVP_VendorSpecificApplicationID ) ) )
    {
        //DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, " parameter hAVP_AcctApplicationID or //hAVP_VendorSpecificApplicationID must not be NULL , Leaving\n");
         
    }
    else 
    {    
        // if AcctApplicationID AVP is present Add it to List
        /*if ( NULL != oACA_Param_i.hAVP_AcctApplicationID ) 
        {
            DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AcctApplicationID);
        }
        
        // if VendorSpecificApplicationID AVP is present Add it to AVPList
        if ( NULL != oACA_Param_i.hAVP_VendorSpecificApplicationID )
        {
            DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_VendorSpecificApplicationID );
        }*/
        

    }
     
    // if UserName optional AVP is present Add it to AVPList
   if(NULL != oACA_Param_i.hAVP_UserName )
   {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_UserName );
   }

  
   // if AccountingSubSessionID optional AVP is present Add it to AVPList
   if(NULL != oACA_Param_i.hAVP_AccountingSubSessionID )
   {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AccountingSubSessionID  );
   }
    
	
   // if AcctSessionID AVP is present Add it to AVPList
   if(NULL != oACA_Param_i.hAVP_AcctSessionID )
   {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AcctSessionID );
   }

   // if AcctMultiSessionID optional AVP is present Add it to AVPList
   if(NULL != oACA_Param_i.hAVP_AcctMultiSessionID )
   {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AcctMultiSessionID);
   }
  
   // if Error-Reporting-Host optional AVP is present Add it to AVPList
   if(NULL != oACA_Param_i.hAVP_ErrorReportingHost )
   {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_ErrorReportingHost);
   }

   // if AcctInterimInterval optional AVP is present Add it to AVPList
    if(NULL != oACA_Param_i.hAVP_AcctInterimInterval )
    {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AcctInterimInterval );
    }
     
    // if AccountingRealtimeRequired optional AVP is present Add it to AVPList
    if(NULL != oACA_Param_i.hAVP_AccountingRealtimeRequired )
    {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_AccountingRealtimeRequired );
    }


    // if OriginStateID optional AVP is present Add it to AVPList
    if(NULL != oACA_Param_i.hAVP_OriginStateID )
    {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_OriginStateID );
    }
   
    // if EventTimestamp  optional AVP is present Add it to AVPList
    if(NULL != oACA_Param_i.hAVP_EventTimestamp )
    {
       DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) oACA_Param_i.hAVP_EventTimestamp );
    }
     
	
    // Traverse the list of  ProxyInfoAVPs
    if(NULL != oACA_Param_i.pListOfProxyInfoAVPs )
    {
        
         void*				pData = NULL;
        HAVPCOLLECTION		pCurrentNode = oACA_Param_i.pListOfProxyInfoAVPs ;
        
       /* while (NULL != pCurrentNode) 
        {
            DMBase_SListGetNodeData(hDiameterStackContext_io, pCurrentNode, &pData);
            DMBase_AppendSList( hDiameterStackContext_io, &pACA_AVPList, (void*) &pData);            
        }*/
    }

    
    if( RfAPI_Retval_Success != DMBase_CreateDiameterMessage( hDiameterStackContext_io , phDiaMsgACA_o ,
                                                              ucDiaMsgVersion , unDiaMsgCommandCode ,  
                                                              unDiaMsgApplicationID,ucFlags_i,
															  unHopByHopID_i, unEndToEndID_i) )
    {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACA Message Creation fail , Leaving \n");
        return RfAPI_Retval_Failure ;
    }


   if( RfAPI_Retval_Success != DMBase_SetDiaMsgRFlag( hDiameterStackContext_io, *phDiaMsgACA_o, bFlagValue ) )
   {
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ACA Message Header P-flag clearing fail , Leaving \n");
        return RfAPI_Retval_Failure ; 
   }


   if( RfAPI_Retval_Success != DMBase_AddAVPCollectionToDiameterMessage(hDiameterStackContext_io,*phDiaMsgACA_o, pACA_AVPList) )
   {
       DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Problem in adding AVP collection to ACA Message, Leaving \n");
       return RfAPI_Retval_Failure ; 
   }


   return RfAPI_Retval_Success; 
}


