
/******************************************************************************
** Written by Simab Shahid
**
** Copyright (C) 2007 AdvancedIMS
**
** All rights reserved.
** 
*******************************************************************************
** FILE NAME      		: AMPS_Defines.h
** DESCRIPTION    		: The file contains defines for AMPS framework.
** PROJECT NAME   		: AMPS
** TYPE           		: ANSI C
** CONTRIBUTOR(S) 		: 
*******************************************************************************
** $Id: AMPS_Defines.h,v 1.30 2008/04/01 12:40:39 zaka Exp $
** $Source: /work/cvsroot/amps/include/AMPS_Defines.h,v $
*******************************************************************************
*******************************************************************************
*/



#ifndef _RF_DEFINES_H
#define	_RF_DEFINES_H

#ifdef	__cplusplus
extern "C" {
#endif




    #define	OFFLINE_CS_TRUE										1
	#define	OFFLINE_CS_FALSE									0

	#define OFFLINE_CS_SQL_QUERY_STRING_SIZE_IN_BYTES				1524
	#define OFFLINE_CS_DATABASE_TABLE_NAME_SIZE_IN_BYTES			50
	#define OFFLINE_CS_SQL_QUERY_START_PART_STRING_SIZE_IN_BYTES	70	
	#define OFFLINE_CS_SQL_COL_NAME_STRING_SIZE_IN_BYTES			500
	#define OFFLINE_CS_SQL_COL_VLAUES_STRING_SIZE_IN_BYTES			1024
			
    #define OFFLINE_CS_NODE_FUNCTIONALITY_S_CSCF	0
	#define OFFLINE_CS_NODE_FUNCTIONALITY_P_CSCF	1
	#define OFFLINE_CS_NODE_FUNCTIONALITY_I_CSCF	2
	#define OFFLINE_CS_NODE_FUNCTIONALITY_MRFC		3
	#define OFFLINE_CS_NODE_FUNCTIONALITY_MGCF		67
	#define OFFLINE_CS_NODE_FUNCTIONALITY_BGCF		68
	#define OFFLINE_CS_NODE_FUNCTIONALITY_AS		6
	#define OFFLINE_CS_NODE_FUNCTIONALITY_IBCF		82


	#define OFFLINE_CS_SubscriptionIDType_endUserE164		 0
	#define OFFLINE_CS_SubscriptionIDType_endUserIMSI		 1
	#define OFFLINE_CS_SubscriptionIDType_endUserSIPURI		 2
	#define OFFLINE_CS_SubscriptionIDType_endUserNAI		 3
	#define OFFLINE_CS_SubscriptionIDType_endUserPRIVATE	 4

   

	#define	OFFLINE_CS_SIZE_OF_INTEGER_32_IN_BYTES        	(sizeof(int))
	#define	OFFLINE_CS_SIZE_OF_UNSIGNED_INT_IN_BYTES   		(sizeof(unsigned int))
	#define	OFFLINE_CS_SIZE_OF_INTEGER_LONG_IN_BYTES 	   	sizeof(long)
	#define	OFFLINE_CS_SIZE_OF_INTEGER_64_IN_BYTES        	(sizeof(long long))

	#define	OFFLINE_CS_CDR_FILE_HEADER_LENGTH_IN_BYTES                 48
	#define OFFLINE_CS_MINUS_SIGN								   	   0
	#define OFFLINE_CS_PLUS_SIGN								   	    1
	#define	OFFLINE_CS_ENCODED_CDR_BUF_SIZE_IN_BYTS                		1000//65534
	#define OFFLINE_CS_RELEASE_IDETIFIER						   		4
	#define OFFLINE_CS_VERSION_IDETIFIER						   		9
	#define OFFLINE_CS_DATA_RECORD_FORMAT								1// BER	
	#define OFFLINE_CS_DATA_TS_NUMBER    			                    9
	#define OFFLINE_CS_CDR_HEADER_LENGTH_IN_BYTES						4//fixed length
	#define OFFLINE_CS_LOW_RELEASE_AND_VERSION_IDENTIFIER               118
	#define OFFLINE_CS_MAX_CDR_FILE_SEQUENCE_NUMBER						4294967295//32-bit value
    #define OFFLINE_CS_MAX_CDR_FILE_RUNNING_COUNT						4294967295//32-bit value																				  
	#define OFFLINE_CS_MAX_CDR_FILE_LENGTH								4294967295//32-bit value
	#define OFFLINE_CS_MAX_NUMBER_OF_CDRS_IN_FILE						4294967295//32-bit value
	#define OFFLINE_CS_FILE_CLOUSER_TRIGGER_REASON						0					



	#define OFFLINE_CS_EVENT_TYPE_IS_ENCODED_CDR_CONTENTS_RESPONSE      3
	#define OFFLINE_CS_EVENT_TYPE_IS_ENCODED_CDR_CONTENTS				4
	#define OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_REQUEST_TO_CGF    			5
	#define OFFLINE_CS_EVENT_TYPE_IS_LOG_CDR_RESPONSE_FROM_CGF    		6

	#define OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_REQUEST      			7
	#define OFFLINE_CS_EVENT_TYPE_IS_ENCODED_CDR_FILE_WRITE_REQUEST   	8
	#define OFFLINE_CS_EVENT_TYPE_IS_CDR_DB_WRITE_RESPONSE      		9
	#define OFFLINE_CS_EVENT_TYPE_IS_ERROR_LOG_REQUEST					10
	#define OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITE_REQUEST_FROM_DB   	11
	#define OFFLINE_CS_EVENT_TYPE_IS_CDR_FILE_WRITER_RESPONSE   		12

	
	
	#define OFFLINE_CS_EXPIRY_TIME_IN_MILLI_SECONDS  					60000

	#define	OFFLINE_CS_NUM_OF_CDRS_BASED_ROTATION       1
	#define	OFFLINE_CS_TIME_BASED_ROTATION				2

	// self defined
	#define DMBASE_INTERIM_INTERVAL_EXPIRE      		1000
	/*#define DIAMETER_INVALID_HDR_BITS                  3008
	#define DIAMETER_MISSING_AVP                       5005
	#define DIAMETER_CONDRATING_AVPS                   5007
	#define DIAMETER_AVPS_OCCUR_TOO_MANY_TIMES         5009*/


	// should move to base protocol header file 
	#define DMBASE_EVENT_RECORD                        1
	#define DMBASE_START_RECORD                        2
	#define DMBASE_INTERIM_RECORD                      3
	#define DMBASE_STOP_RECORD                         4
	#define DIAMETER_SUCCESS                           2001
	#define DIAMETER_UNABLE_TO_COMPLY                  5012
	#define DIAMETER_INVALID_MESSAGE_LENGTH            5015
	#define DIAMETER_INVALID_BIT_IN_HEADER             5013
	#define DMBASE_BITMASK_RESERVED_BITS_SET           0X0F
	#define DMBASE_BITMASK_RESERVED_BITS_CLEAR         0X00


	#define OFFLINE_CS_ACCT_INTERIM_INTERVAL_VALUE_IN_SECONDS   60000 

	//enable the CDF_DUMP_ACA_MSG_TO_CONSOLE_AND_FILE macro to dump the ACA message to be sent to console and file ( for debugging) 
	//#define	CDF_DUMP_ACA_MSG_TO_CONSOLE_AND_FILE
	
	#define OFFLINE_CS_HASHTABLE_MAX_SIZE               500003//2003
	#define	OFFLINE_CS_MAX_QUERY_SIZE					2000

	#define	OFFLINE_CS_DB_ERROR							1

	

    

	typedef enum Rf_ReturnVal
	{
		Offline_CS_RetVal_Success,
		Offline_CS_RetVal_Failure,
		Offline_CS_RetVal_Null_Pointer,
		Offline_CS_RetVal_Invalid_Pointer,
		Offline_CS_RetVal_Invalid_Parameter,
		Offline_CS_RetVal_Invalid_Bit_In_Hdr,
		Offline_CS_RetVal_Message_Length,
		Offline_CS_RetVal_No_Data_In_Session_Table,
		Offline_CS_RetVal_Data_Not_Found_In_SessionTable,
		Offline_CS_RetVal_Data_Not_Found_In_SessionObject,
		Offline_CS_RetVal_Data_Found_In_SessionTable,
		Offline_CS_RetVal_Data_Base_Request_Fail,
		Offline_CS_RetVal_Command_Not_found,
		Offline_CS_RetVal_AVP_Missing,
		Offline_CS_RetVal_AVP_Occurs_Too_Many_Times,
		Offline_CS_RetVal_Invalid_AVP_Value,
		Offline_CS_RetVal_Event_Acct_Record_Type

	}t_Offline_CS_ReturnVal;





	




    #ifdef	__cplusplus
}
#endif

#endif	/* _RF_DEFINES_H */



