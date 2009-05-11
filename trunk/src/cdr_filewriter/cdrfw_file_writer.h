
#ifndef _DB_CDR_FILE_WRITER_H
#define	_DB_CDR_FILE_WRITER_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include"RF_defines.h"
	#include"RF_include.h"
	#include "CdrFileWriterModule.h"

	 typedef struct HighReleaseAndVersionId
   {
	   unsigned char HighVersionIdentifier:5;
	   unsigned char HighReleasIdentifier:3;
   }t_HighReleaseAndVersionId;

   typedef struct LowReleaseAndVersionId
   {
	   unsigned char LowVersionIdentifier:5;
	   unsigned char LowReleasIdentifier:3;
   }t_LowReleaseAndVersionId;

   typedef struct time_stamp
   {
	   unsigned int Month:4;
	   unsigned int Date:5;
	   unsigned int Hour:5;
	   unsigned int Minute:6;
	   unsigned int Sign:1;
	   unsigned int UTCHour:5;
	   unsigned int UTCMinute:5;
   }t_time_stamp;


   typedef struct LostCDRIndicator
   {
	   unsigned char MSB:1;
	   unsigned char AllOtherBits:7;
   }t_LostCDRIndicator;

   typedef struct FileClouserTriggerReason
   {
	   unsigned char ucFileClouserTriggerReason:8;
   }t_FileClouserTriggerReason;

   typedef struct CDRFileHeader
   {
	 unsigned int					unFileLength;
	 unsigned int  				unHeaderLength;
	 t_HighReleaseAndVersionId 	oHighReleaseAndVersionId;
	 t_LowReleaseAndVersionId		oLowReleaseAndVersionId;
	 t_time_stamp  				oFileOpeningTimeStamp;
	 t_time_stamp  				oLastAppendCDRTimeStamp;
	 unsigned int  				unNumberOfCDRsInFile;
	 unsigned int  				unFileSequenceNumber;
	 t_FileClouserTriggerReason 	oFileClosureTriggerReasone;
	 t_LostCDRIndicator 			oLostCDRIndicator;
  }t_CDRFileHeader;



  t_Offline_CS_ReturnVal CdrFW_WriteCdrToFile( HCDRFILEWRITERTHREADCONTEXT  hCdrFWriterThreadContext_i,
											 t_EncodedCDR	 					oEncodedCDR_i);

  t_Offline_CS_ReturnVal CdrFW_CreateNewCdrFile( HCDRFILEWRITERTHREADCONTEXT hCdrFWriterThreadContext_i );

  t_Offline_CS_ReturnVal CdrFW_CreateCdrFileName( HCDRFILEWRITERTHREADCONTEXT hCdrFWriterThreadContext_i );

  t_Offline_CS_ReturnVal CdrFW_RotateCdrFile( HCDRFILEWRITERTHREADCONTEXT        hCdrFWriterThreadContext_i );

  t_Offline_CS_ReturnVal CdrFW_CreateCdrFileHeader( HCDRFILEWRITERTHREADCONTEXT        hCdrFWriterThreadContext_i );

  int CdrFW_FileRotationNotifyHandler(void* pvHandle_i, void* pvData_i);



















#ifdef	__cplusplus
}
#endif

#endif	/* _DB_CDR_FILE_WRITER_H */
