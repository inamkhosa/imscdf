
#ifndef _RF_MSG_API_H
#define	_RF_MSG_API_H

#ifdef	__cplusplus
extern "C" {
#endif



#include "dmbase_message.h"
#include "dmbase_data_types.h"
#include "dmbase_dictionary.h"
#include "dmbase_ampswrappers.h"
#include "dmbase_avp.h"



   #include "dmbase_utility.h"
#include "dmbase_initconfig.h"
#include "dmbase_transport_mgmt.h"
#include "dmbase_peer_mgmt.h"
#include "AMPS_API.h"
#include "dmbase_realmtablemgmt.h"
#include "dmbase_ampswrappers.h"


/**************************************************************************************/





typedef enum RfAPIReturnVal
{
    RfAPI_Retval_Success,
    RfAPI_Retval_Failure,
    RfAPI_Retval_Null_Pointer

} RfAPIReturnVal_t;

#define RF_MSG_CMD_CODE_ACR      271
#define RF_MSG_CMD_CODE_ACA      271
#define RF_APPLICATION_ID        3 
#define RF_MSG_VERSION               1




typedef struct ACR_Params
{
    HAVP            hAVP_SessionID;
    HAVP            hAVP_OriginHost;
    HAVP            hAVP_OriginRealm;
    HAVP            hAVP_DestinationRealm;
    HAVP            hAVP_AccountingRecordType;
    HAVP            hAVP_AccountingRecordNumber;
    HAVP            hAVP_AcctApplicationID;
    HAVP            hAVP_VendorSpecificApplicationID;
    HAVP            hAVP_UserName;
    HAVP            hAVP_AccountingSubSessionID;
    HAVP            hAVP_AcctSessionID;
    HAVP            hAVP_AcctMultiSessionID;
    HAVP            hAVP_AcctInterimInterval;
    HAVP            hAVP_AccountingRealtimeRequired;
    HAVP            hAVP_OriginStateID;
    HAVP            hAVP_EventTimestamp;
    DMBase_SList*   pListOfProxyInfoAVPs;
    DMBase_SList*   pListOfRouteRecordAVPs;
     
    
}t_ACR_Params;

typedef struct ACA_Params
{
    HAVP            hAVP_SessionID;
    HAVP            hAVP_ResultCode;
    HAVP            hAVP_OriginHost;
    HAVP            hAVP_OriginRealm;
    HAVP            hAVP_AccountingRecordType;
    HAVP            hAVP_AccountingRecordNumber;
    HAVP            hAVP_AcctApplicationID;
    HAVP            hAVP_VendorSpecificApplicationID;
    HAVP            hAVP_UserName;
    HAVP            hAVP_Failed_AVP;
    HAVP            hAVP_AccountingSubSessionID;
    HAVP            hAVP_AcctSessionID;
    HAVP            hAVP_AcctMultiSessionID;
    HAVP            hAVP_ErrorReportingHost;
    HAVP            hAVP_AcctInterimInterval;
    HAVP            hAVP_AccountingRealtimeRequired;
    HAVP            hAVP_OriginStateID;
    HAVP            hAVP_EventTimestamp;
    DMBase_SList*   pListOfProxyInfoAVPs;
    
}t_ACA_Params;


RfAPIReturnVal_t RFAPI_CreateAccountingRequest( HDIAMETERSTACKCONTEXT       g_hDiameterStackContext,
                                                HDIAMETERMESSAGE*           phDiaMsgACR,
												unsigned char              ucDiaMsgVersion_i,
                                                unsigned int               unDiaMsgCommandCode_i,  
                                                unsigned int               unDiaMsgApplicationID_i, 
                                                t_ACR_Params                oACR_Param);

RfAPIReturnVal_t RFAPI_CreateAccountingAnswer(  HDIAMETERSTACKCONTEXT       g_hDiameterStackContext,
                                                HDIAMETERMESSAGE*           phDiaMsgACA,
												t_ACA_Params                oACA_Param,
												unsigned char 				ucFlags_i,
												unsigned int  				unHopByHopID_i,
												unsigned int  				unEndToEndID_i);





/**************************************/

/*int CreateTransportObj( HDIAMETERSTACKCONTEXT*,
                        t_DiaStackInitData*, 
                        const char*,
                        const char* pcStrFQDN);
int CreatePeerTabEntry(HDIAMETERSTACKCONTEXT*,t_DiaStackInitData*, 
                    const char* ,  
                    const char* ipaddress,
                    unsigned int           unPort,
                    t_PeerRole      oPeerRole,
                    _Bool                   IsStatic,
                    _Bool                   IsTLSEnable);

int CreateRealmTabEntry(HDIAMETERSTACKCONTEXT*,
                    t_DiaStackInitData*, 
                    const char*,
                    unsigned int            unAppID,
                    _Bool                   IsStaticEntry,
                    t_LocalAction           oLocalAction);
int LoadApplicationInfo(HDIAMETERSTACKCONTEXT*);

int DMBase_TimeOutNotifyTestFunction(void* r_hAMPS_HANDLE, void* r_pvData);*/









#ifdef	__cplusplus
}
#endif

#endif	/* _RF_MSG_API_H */

