
#ifndef _DB_CMD_ID_H_
#define _DB_CMD_ID_H_

#include "dmbase_data_types.h"
                                            
#define DB_CX_CMD_AUTHORIZATION                 8001
#define DB_CX_CMD_SERVER_ASSIGNMENT             8002
#define DB_CX_CMD_LOCATION_INFO                 8003
#define DB_CX_CMD_MULTIMEDIA_AUTH               8004
#define DB_CX_CMD_REGISTERATION_TERMINATION     8005
#define DB_CX_CMD_PUSH_PROFILE                  8006









#define MAX_SIZE_IN_BYTES                   512
#define DB_OPERATION_FAIL                   0
#define DB_OPERATION_SUCCESS                1
#define OFFLINE_CS_CMND_SENDING_CDR_WRITE_REQUEST     5
#define OFFLINE_CS_CMND_SENDING_CDR_WRITE_RESPONSE 	  6


// here all the defines for event data will be made 
#define OFFLINE_CS_EVT_DATA_OPERATION_RESULT 	1 
#define OFFLINE_CS_EVT_DATA_SESSION_ID          2
#define OFFLINE_CS_EVT_DATA_ACCT_RECORD_NUMBER  3


#define	OFFLINE_CS_EVT_DATA_HEADER_SIZE		 (3 *(sizeof(unsigned int))) 


typedef struct DBDataToStore
 {  
    unsigned char           ucSessionID[MAX_SIZE_IN_BYTES];
    int                     nAcctRecordType;
    unsigned int            unAcctRecordNumber;
    unsigned int            unAcctApplicationID;
    unsigned char           ucUserName[MAX_SIZE_IN_BYTES];
    unsigned char           ucEventTimeStamp[MAX_SIZE_IN_BYTES]; 

 }t_DBDataToStore;


typedef struct CorrID
{
    unsigned int                    unAcctRecordNumber;
    unsigned char                   ucSessionID[MAX_SIZE_IN_BYTES];
	unsigned int 					unSessionIDSizeInBytes;
    DMBASE_HASHTABLE_KEY_HANDLE     hSessionTableKey;
    
 }t_CorrID;





typedef struct DBRequestEvtData
{
    int	      	         nDBCmdID;
    t_CorrID             oDBCorrID;        
    t_DBDataToStore      oDBDataToStore;
    
}t_DBRequestEvtData;


/*typedef struct ResponseEvtData
{
    t_CorrID        oDBCorrID;        
    int             nResultCode;
}t_ResponseEvtData;*/






#endif
