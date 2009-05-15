

	#include"RF_defines.h"
	#include"RF_include.h"
	#include"DBModule.h"
	#include"cdrfw_file_writer.h"
	#include "IMSRecord.h"

	extern	 int g_nDiamBaseTraceID ;


	/******************************************************************************
** Function Name: 
**			CdrFW_WriteCdrToFile
**  
** Description:
**		This function is writing cdr contents to file  .
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o
**			 type: output
**			 detail : This is a pointer to CdrFWModuleContext Handle . 
**
**       t_CDR_Contents 				   oCDRContents_i
**			 type: input
**			 detail : This parameter contains the Cdr Contents that needs to be stored 
**					   in file.       
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function. 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal CdrFW_WriteCdrToFile( HCDRFILEWRITERTHREADCONTEXT  hCdrFWriterThreadContext_i,
											 t_EncodedCDR	 					oEncodedCDR_i)
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if(NULL == hCdrFWriterThreadContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCdrFWriterThreadContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		HCDRFILEWRITERMODULECONTEXT	hCdrFWriterModuleContext = hCdrFWriterThreadContext_i->hMainCDRModContext;

        
		unsigned int    	unFileRotationType      		= hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.unFileRotationType;
		unsigned int    	unMaxNumCdrsForFileRotation     = hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.unMaxNumCdrsInOneFile;
		unsigned int    	unNumCdrsWrittenInCurrentFile   = hCdrFWriterThreadContext_i->oCdrFileObject.unCdrCounter;
		unsigned int		unFileSequenceNumber		    = hCdrFWriterThreadContext_i->oCdrFileObject.unFileSequenceNumber;
		unsigned int        unRecvdCdrLength				= 0,
							unCurrentCdrFileLength 			= 0,
							unTotalBytesRead 				= 0,
                            unTotalBytesWritten 			= 0,
							unReadBytes 					= 0,
							unNumbytesWritten 				= 0;
		t_CDRFileHeader		oCDRFileHeader;

		
       
       
        if(DMBase_Retval_Success !=  DMBase_MemsetZero((void*)&oCDRFileHeader,sizeof(t_CDRFileHeader) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader with zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		//printf("\n inside write cdr to File \n"); 

		// set the file position at start
		fseek(hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,0,SEEK_SET);
		if(DMBase_Retval_Success !=  DMBase_FRead( hCdrFWriterModuleContext->hDiameterStackContext,
												   (void*)&oCDRFileHeader,sizeof(t_CDRFileHeader), 
												   1,hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,
												   &unReadBytes))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

       
        //printf(" File Length before adding new CDR   =  %d \n", oCDRFileHeader.unFileLength);
		//printf(" Number of CDRs in File  before adding new CDR  =  %d \n", oCDRFileHeader.unNumberOfCDRsInFile);
		//printf(" unMaxNumCdrsForFileRotation   =  %d \n", unMaxNumCdrsForFileRotation);
		//printf(" hCdrFWriterThreadContext_i->oCdrFileObject.unFileRunningCount   =  %d \n", hCdrFWriterThreadContext_i->oCdrFileObject.unFileRunningCount);
		

		unRecvdCdrLength	 		= oEncodedCDR_i.oCdrData.unsize;
		unCurrentCdrFileLength 		= oCDRFileHeader.unFileLength;
		unNumCdrsWrittenInCurrentFile = oCDRFileHeader.unNumberOfCDRsInFile;

        
        if( ( ( (  OFFLINE_CS_NUM_OF_CDRS_BASED_ROTATION  == unFileRotationType ) && 
			  (  unNumCdrsWrittenInCurrentFile == unMaxNumCdrsForFileRotation ) )||
              ( OFFLINE_CS_MAX_CDR_FILE_LENGTH <= (unCurrentCdrFileLength + unRecvdCdrLength) ) ||
			  ( OFFLINE_CS_MAX_NUMBER_OF_CDRS_IN_FILE <= (unNumCdrsWrittenInCurrentFile+1) )
			)
		  )
		{ 

			//printf(" \n\n About to call Rotate File \n\n");
			if( Offline_CS_RetVal_Success != CdrFW_RotateCdrFile(hCdrFWriterThreadContext_i))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Cdr File Rotation,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            fseek(hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,0,SEEK_SET);
			if(DMBase_Retval_Success !=  DMBase_FRead( hCdrFWriterModuleContext->hDiameterStackContext,
													   (void*)&oCDRFileHeader,sizeof(t_CDRFileHeader), 
													   1,hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,
													   &unReadBytes))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
       }

		//printf("\n Reached here \n");
		// modify the length field by adding cdr header and cdr data size to the existing file length
		oCDRFileHeader.unFileLength = ( OFFLINE_CS_CDR_HEADER_LENGTH_IN_BYTES +
									    oEncodedCDR_i.oCdrData.unsize + oCDRFileHeader.unFileLength);


		// time stamp when last cdr was appended to the file
		t_AMPSTimerValue oTimerValue;

		if( DMBase_Retval_Success !=  DMBase_GetCurrentTime(hCdrFWriterModuleContext->hDiameterStackContext,&oTimerValue) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_GetCurrentTime() Fails,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		oCDRFileHeader.oLastAppendCDRTimeStamp.Month = oTimerValue.nMonth;
		oCDRFileHeader.oLastAppendCDRTimeStamp.Date = oTimerValue.nDay;
		oCDRFileHeader.oLastAppendCDRTimeStamp.Hour = oTimerValue.nHour;
		oCDRFileHeader.oLastAppendCDRTimeStamp.Minute = oTimerValue.nMinute;
		/*oCDRFileHeader.oLastAppendCDRTimeStamp.Sign = OFFLINE_CS_SIGN; // will ask about it
		oCDRFileHeader.oLastAppendCDRTimeStamp.UTCHour = (oTimerValue.nHour + );
		oCDRFileHeader.oLastAppendCDRTimeStamp.UTCMinute = (oTimerValue.nMinute + );*/


		// modify the number of cdrs in file
		oCDRFileHeader.unNumberOfCDRsInFile++;

        // TODO:lost cdr indicator

		
		// set the file position at start
        fseek(hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,0,SEEK_SET);
		// firts write the cdr file header to file
		if(DMBase_Retval_Success !=  DMBase_FWrite( hCdrFWriterModuleContext->hDiameterStackContext,
												    (void*)&oCDRFileHeader,sizeof(t_CDRFileHeader), 
												     1,hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,
												     &unNumbytesWritten))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
        
        //set the file position at end
		fseek( hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,0,SEEK_END );
		// now write the cdr to file 
		if(DMBase_Retval_Success !=  DMBase_FWrite(hCdrFWriterModuleContext->hDiameterStackContext,
												   (void*)&oCDRFileHeader,
												   (OFFLINE_CS_CDR_HEADER_LENGTH_IN_BYTES+oEncodedCDR_i.oCdrData.unsize), 
												   1,hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,
												   &unNumbytesWritten))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		 //printf("OFFLINE_CS_CDR_HEADER_LENGTH_IN_BYTES 			=  %d \n", OFFLINE_CS_CDR_HEADER_LENGTH_IN_BYTES);
		 //printf("Length of New CDR 								=  %d \n", oCDRFileHeader.unFileLength);
		 //printf("File Length After Adding New CDR       		=  %d \n", oCDRFileHeader.unFileLength);
		 //printf("Number of CDRs in File  before adding new CDR  =  %d \n", oCDRFileHeader.unNumberOfCDRsInFile);

        
 /*		if( DMBase_Retval_Success !=  DMBase_MemsetZero((void*)&oCDRFileHeader,sizeof(t_CDRFileHeader) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}
		fseek(hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,0,SEEK_SET);

		if(DMBase_Retval_Success !=  DMBase_FRead(hCdrFWriterModuleContext->hDiameterStackContext,
												  (void*)&oCDRFileHeader,sizeof(t_CDRFileHeader), 
												  1,hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,
												  &unReadBytes))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		printf(" New Cdr Length	  =  %d \n", oEncodedCDR_i.oCdrData.unsize );
		printf(" File Length after adding new CDR =  %d \n", oCDRFileHeader.unFileLength);
		printf(" Number of CDRs in File after adding new CDR  =  %d \n", oCDRFileHeader.unNumberOfCDRsInFile);

		// set the file position at end
		fseek(hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,0,SEEK_END);
		//char a[1];
		//gets(a);

	*/
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
 }

int CdrFW_FileRotationNotifyHandler(void* pvHandle_i, void* pvData_i)
{


        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO," Entering \n");

        //printf("\n\n\n\n Entering the File Rotation timer function \n\n\n\n ");

        if( NULL == pvData_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "Null Parameter pvData_i , Leaving \n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		HCDRFILEWRITERTHREADCONTEXT	hCdrFWriterThreadContext = ( HCDRFILEWRITERTHREADCONTEXT )pvData_i;
		HCDRFILEWRITERMODULECONTEXT hCdrFWriterModuleContext = hCdrFWriterThreadContext->hMainCDRModContext;

        if( Offline_CS_RetVal_Success != CdrFW_RotateCdrFile( hCdrFWriterThreadContext ) )
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Cdr File Rotation,Leaving\n");
				return Offline_CS_RetVal_Failure;
		}

		// start the File Rotation Notify Timer again 
		DMBASE_TIMERID 	TimerID = NULL;
		unsigned int 	unExpiryTime = ( hCdrFWriterThreadContext->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.unRotationTimerValueInMinutes * 60000);
		if( DMBase_Retval_Success!= DMBase_StartTimer( hCdrFWriterModuleContext->hDiameterStackContext,
													   unExpiryTime,
													   CdrFW_FileRotationNotifyHandler, 
													   (void*)hCdrFWriterModuleContext,
													   &TimerID))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Setting timer for Cdr File Rotation, Leaving\n");
			return DMBase_Retval_Failure;
		}

        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
        return Offline_CS_RetVal_Success;
  }

/******************************************************************************
** Function Name: 
**			CdrFW_WriteCdrToFile
**  
** Description:
**		This function is writing cdr contents to file  .
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o
**			 type: output
**			 detail : This is a pointer to CdrFWModuleContext Handle . 
**
**       t_CDR_Contents 				   oCDRContents_i
**			 type: input
**			 detail : This parameter contains the Cdr Contents that needs to be stored 
**					   in file.       
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function. 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/
 t_Offline_CS_ReturnVal CdrFW_CreateCdrFileName( HCDRFILEWRITERTHREADCONTEXT hCdrFWriterThreadContext_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if(NULL == hCdrFWriterThreadContext_i)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCdrFWriterThreadContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		char            cFileName[400];
		char 			TimeStamp[100];
        time_t 			time_value 		  = 0;
        struct tm 		*now 			  = NULL;
		unsigned int	unNextIndex		  = 0;
		unsigned int   	unSize 			  = 0;
		unsigned char* 	pucCdrLogFilepath = NULL;

		HCDRFILEWRITERMODULECONTEXT hCdrFWriterModuleContext = hCdrFWriterThreadContext_i->hMainCDRModContext;
		

		time_value = time(NULL);          // Get time value
        now = localtime(&time_value);     // Get time and date structure 

		t_AMPSTimerValue SSTimerValue;

       
		gethostname(cFileName, sizeof(cFileName));
		unNextIndex = strlen(cFileName);
		sprintf( &cFileName[unNextIndex],"_-_%d.",hCdrFWriterThreadContext_i->oCdrFileObject.unFileRunningCount);

        
		strftime(TimeStamp, sizeof(TimeStamp), "%Y%m%d", now);
		unNextIndex = strlen(cFileName);
		sprintf( &cFileName[unNextIndex],"%s_-_",TimeStamp);
        //strftime(TimeStamp, sizeof(TimeStamp), "CdrFile%d%m%Y%H:%M:%S", now);

		 
        //SAPI_GetCurrentTime(hCdrFWriterModuleContext->hDiameterStackContext->hFrameworkHandle, &SSTimerValue);
		strftime(TimeStamp, sizeof(TimeStamp), "%H%M", now);
		unNextIndex = strlen(cFileName);
        sprintf( &cFileName[unNextIndex],"%s",TimeStamp);

		if( hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.bUTCDifferentialSign )
		{
			unNextIndex = strlen(cFileName);
			sprintf( &cFileName[unNextIndex],"+%d%d.dat", hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialHour,
														  hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialMinute);
		}
		else
		{
			unNextIndex = strlen(cFileName);
			sprintf( &cFileName[unNextIndex],"-%d%d.dat",hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialHour,
														  hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialMinute);
		}

        
		unSize = (strlen(cFileName)+ 20);
		if( Offline_CS_RetVal_Success !=  DMBase_Malloc( hCdrFWriterModuleContext->hDiameterStackContext,
														 (void*)&(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName),unSize  ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in memory allocation to Filename String Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

        strcpy(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName,hCdrFWriterThreadContext_i->hMainCDRModContext->ocdf_config_params.ocdf_CdrFile_config_params.pucCdrFileDirectoryName );
		strcat(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName,"/");
		strcat(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName,cFileName);
		

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
}

t_Offline_CS_ReturnVal CdrFW_CreateCdrFileHeader( HCDRFILEWRITERTHREADCONTEXT        hCdrFWriterThreadContext_i )
{

		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
	
		if( NULL == hCdrFWriterThreadContext_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCdrFWriterThreadContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}

		HCDRFILEWRITERMODULECONTEXT	hCdrFWriterModuleContext = hCdrFWriterThreadContext_i->hMainCDRModContext;

        t_CDRFileHeader		oCDRFileHeader;
		if(DMBase_Retval_Success !=  DMBase_MemsetZero((void*)&oCDRFileHeader,sizeof(t_CDRFileHeader) ) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

        // First Field Of CDR File Header, File Length at file creation is equal to length of file header 
		 oCDRFileHeader.unFileLength = OFFLINE_CS_CDR_FILE_HEADER_LENGTH_IN_BYTES;

		// Second Field Of CDR File Header, Header Length at file creation time it is setting
		oCDRFileHeader.unHeaderLength = OFFLINE_CS_CDR_FILE_HEADER_LENGTH_IN_BYTES;

		// Third Field Of CDR File Header, High Release identifier and High Version Identifier,
		// Low Release identifier and Low Version Identifier at file creation time it is setting
		oCDRFileHeader.oHighReleaseAndVersionId.HighVersionIdentifier = OFFLINE_CS_VERSION_IDETIFIER;
		oCDRFileHeader.oHighReleaseAndVersionId.HighReleasIdentifier  = OFFLINE_CS_RELEASE_IDETIFIER;
		oCDRFileHeader.oLowReleaseAndVersionId.LowVersionIdentifier   = OFFLINE_CS_VERSION_IDETIFIER;
		oCDRFileHeader.oLowReleaseAndVersionId.LowReleasIdentifier    = OFFLINE_CS_RELEASE_IDETIFIER;


		// Fourth Field Of CDR File Header, File Openting Time Stamp
		//  at file creation time it is setting
		t_AMPSTimerValue oTimerValue;

		if( DMBase_Retval_Success !=  DMBase_GetCurrentTime(hCdrFWriterModuleContext->hDiameterStackContext,&oTimerValue) )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"DMBase_GetCurrentTime() Fails,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		oCDRFileHeader.oFileOpeningTimeStamp.Month = oTimerValue.nMonth;
		oCDRFileHeader.oFileOpeningTimeStamp.Date = oTimerValue.nDay;
		oCDRFileHeader.oFileOpeningTimeStamp.Hour = oTimerValue.nHour;
		oCDRFileHeader.oFileOpeningTimeStamp.Minute = oTimerValue.nMinute;

		if( OFFLINE_CS_PLUS_SIGN  == hCdrFWriterModuleContext->ocdf_config_params.ocdf_CdrFile_config_params.bUTCDifferentialSign )
		{
			oCDRFileHeader.oFileOpeningTimeStamp.Sign = OFFLINE_CS_PLUS_SIGN ;
			oCDRFileHeader.oFileOpeningTimeStamp.UTCHour = (oTimerValue.nHour +
															hCdrFWriterModuleContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialHour );
			oCDRFileHeader.oFileOpeningTimeStamp.UTCMinute = (oTimerValue.nMinute +
															  hCdrFWriterModuleContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialMinute );
		}
		else if (OFFLINE_CS_MINUS_SIGN == hCdrFWriterModuleContext->ocdf_config_params.ocdf_CdrFile_config_params.bUTCDifferentialSign )
		{
			oCDRFileHeader.oFileOpeningTimeStamp.Sign 		= OFFLINE_CS_MINUS_SIGN; 
			oCDRFileHeader.oFileOpeningTimeStamp.UTCHour 	= (oTimerValue.nHour + 
															   hCdrFWriterModuleContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialHour);
			oCDRFileHeader.oFileOpeningTimeStamp.UTCMinute 	= (oTimerValue.nMinute +
															   hCdrFWriterModuleContext->ocdf_config_params.ocdf_CdrFile_config_params.nUTCDifferentialMinute );
		}
		else
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Invalid sign set in UTCDifferentialSign,Leaving\n");
			return Offline_CS_RetVal_Failure;

		}

		
        

		// Fifth  Field Of CDR File Header, Last Cdr Append Time Stamp 
		// it is modifying at thearrival of each new cdr , currently it is set to zero

		// Sixth  Field Of CDR File Header, Number of CDRS is 
		// incremented at at the arrival of each new cdr , currently it is set to zero


		// Seventh  Field Of CDR File Header, File Sequence number
		if( OFFLINE_CS_MAX_CDR_FILE_SEQUENCE_NUMBER > hCdrFWriterThreadContext_i->oCdrFileObject.unFileSequenceNumber )
		{
			oCDRFileHeader.unFileSequenceNumber = hCdrFWriterThreadContext_i->oCdrFileObject.unFileSequenceNumber++;
		}
		else
		{
			oCDRFileHeader.unFileSequenceNumber = hCdrFWriterThreadContext_i->oCdrFileObject.unFileSequenceNumber=1;
		}
		

		// Eight  Field Of CDR File Header, File closure Trigger Reason will be set at file closing time

		// ninth  Field Of CDR File Header, Node IP Address

		// Tenth  Field Of CDR File Header, Lost CDR Indication will be set at each cdr writing 

		unsigned int unNumbytesWritten = 0;
        if( DMBase_Retval_Success !=  DMBase_FWrite(hCdrFWriterModuleContext->hDiameterStackContext,
												  (void*)&oCDRFileHeader,sizeof(t_CDRFileHeader), 
												  1,hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,
												  &unNumbytesWritten))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in filling oCDRFileHeader vwith zeros ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		
        DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
 }




t_DiamAPIReturnVal DMBase_GetCurrentTime(HDIAMETERSTACKCONTEXT    	hDiameterContext_i,
										 t_AMPSTimerValue*			poTimerValue_i)
{
	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

	if(NULL == hDiameterContext_i)
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hDiameterContext_i is NULL , leaving\n");
		return DMBase_Retval_Null_Pointer;
	}

	if( NULL == poTimerValue_i )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"poTimerValue_i is NULL , leaving\n");
		return DMBase_Retval_Null_Pointer;
	}

	if( AMPS_SUCCESS !=  AMPS_GetCurrentTime( hDiameterContext_i->hFrameworkHandle,poTimerValue_i ) )
	{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"AMPS_GetCurrentTime() Fails,leaving\n");
		return DMBase_Retval_Failure;
	}

	DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
	return Offline_CS_RetVal_Success;
}




/******************************************************************************
** Function Name: 
**			CdrFW_RotateCdrFile
**  
** Description:
**		This function is rotating the file means creating a new file to cdrs
**		and setting it hand module context  .
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o
**			 type: output
**			 detail : This is a pointer to CdrFWModuleContext Handle . 
**
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function. 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal CdrFW_RotateCdrFile( HCDRFILEWRITERTHREADCONTEXT        hCdrFWriterThreadContext_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");
		
		
		if( NULL == hCdrFWriterThreadContext_i )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCdrFWriterThreadContext_i is NULL , leaving\n");
			return Offline_CS_RetVal_Null_Pointer;
		}
		long lOffSet = 27;
		unsigned int unTotalBytesWritten; 
		unsigned char ucDataToWrite;
		unsigned char* pucOldNameOfFile = NULL;
		t_FileClouserTriggerReason	oFileClouserTriggerReason;	

		HCDRFILEWRITERMODULECONTEXT	hCdrFWriterModuleContext = hCdrFWriterThreadContext_i->hMainCDRModContext;


		// before opening new cdrs file first colse the previously opened cdrs file 
        if( NULL !=  hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle )
		{
			// modify the file clouser trigger reason field of cdr File Header first
			// set the file position at start
			fseek(hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,lOffSet,SEEK_SET);
			oFileClouserTriggerReason.ucFileClouserTriggerReason = OFFLINE_CS_FILE_CLOUSER_TRIGGER_REASON;
			ucDataToWrite = oFileClouserTriggerReason.ucFileClouserTriggerReason;
			
			if(DMBase_Retval_Success !=  DMBase_FWrite( hCdrFWriterModuleContext->hDiameterStackContext,
													   (void*)&ucDataToWrite,1, 1,
													    hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle,
													    &unTotalBytesWritten))
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in writing File clouser trigger reasone in file ,Leaving\n");
				return Offline_CS_RetVal_Failure;// TODO: what should have to do here with memory allocated to FileName
			}
			
            fclose( hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle );
			hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle = NULL;
			pucOldNameOfFile = hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName;


            // after closing the file rename it with current timestamp
			if( Offline_CS_RetVal_Success != CdrFW_CreateCdrFileName( hCdrFWriterThreadContext_i) )
			{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in creation CdrFile name for cdr file renaming ,Leaving\n");
				return Offline_CS_RetVal_Failure;
			}
            rename( pucOldNameOfFile , hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName );
			
			DMBase_Free(NULL,(void**)&(pucOldNameOfFile) );
			DMBase_Free(NULL,(void**)&(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName) );
		}

		if( OFFLINE_CS_MAX_CDR_FILE_RUNNING_COUNT > hCdrFWriterThreadContext_i->oCdrFileObject.unFileRunningCount )
		{
			hCdrFWriterThreadContext_i->oCdrFileObject.unFileRunningCount++;
		}
		else
		{
			hCdrFWriterThreadContext_i->oCdrFileObject.unFileRunningCount=1;
		}
		
		
		if( Offline_CS_RetVal_Success != CdrFW_CreateNewCdrFile( hCdrFWriterThreadContext_i ) )
		{
			
		  DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Creating New CdrFile ,Leaving\n");
         return Offline_CS_RetVal_Failure;
		}
		
		
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
}
/******************************************************************************
** Function Name: 
**			CdrFW_WriteCdrToFile
**  
** Description:
**		This function is writing cdr contents to file  .
**
** Parameters:
**		  HCDRFILEWRITERMODULECONTEXT*        phCdrFWriterModuleContext_o
**			 type: output
**			 detail : This is a pointer to CdrFWModuleContext Handle . 
**
**       t_CDR_Contents 				   oCDRContents_i
**			 type: input
**			 detail : This parameter contains the Cdr Contents that needs to be stored 
**					   in file.       
** Return:
**		An t_Offline_CS_ReturnVal value specifing the status of the function. 
*******************************************************************************
** Created By: Simab Shahid
*******************************************************************************
*******************************************************************************
*/

t_Offline_CS_ReturnVal CdrFW_CreateNewCdrFile( HCDRFILEWRITERTHREADCONTEXT hCdrFWriterThreadContext_i )
{
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Entering\n");

		if( NULL == hCdrFWriterThreadContext_i )
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"hCdrFWriterThreadContext_i is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		

        if( Offline_CS_RetVal_Success != CdrFW_CreateCdrFileName( hCdrFWriterThreadContext_i ))
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in CdrFile name Creation ,Leaving\n");
			return Offline_CS_RetVal_Failure;
		}

		unsigned char* pucFileName = hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName;

		if( NULL == pucFileName)
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"pucFileName in Context is NULL , leaving\n");
            return Offline_CS_RetVal_Null_Pointer;
        }

		HCDRFILEWRITERMODULECONTEXT	hCdrFWriterModuleContext = hCdrFWriterThreadContext_i->hMainCDRModContext;

		
		FILE *fileptr = NULL;// TODO: File pointer according to name convention,
		
		fileptr = fopen( pucFileName,"r" );
		
		
        if ( NULL != fileptr )//TODO: NULL on right side
		{
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Cdr File Could not be Created, file of same name allready exists\n");
			fclose(fileptr);
            fileptr = hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle = NULL;
            DMBase_Free(hCdrFWriterModuleContext->hDiameterStackContext,(void**)&(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName));
            return Offline_CS_RetVal_Failure;
		}

		
		fileptr = fopen(pucFileName,"a+");
		//printf(" \n %s \n",hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName);

		if( NULL == fileptr )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Cdr File Could not be Created \n");
			hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle = NULL;
			DMBase_Free(hCdrFWriterModuleContext->hDiameterStackContext,(void**)&(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName));
            return Offline_CS_RetVal_Failure;
		}

		fclose(fileptr);

		fileptr = fopen(pucFileName,"r+");

		if( NULL == fileptr )
		{
			DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Cdr File Could not be opened \n");
			hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle = NULL;
			DMBase_Free(hCdrFWriterModuleContext->hDiameterStackContext,(void**)&(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName));
            return Offline_CS_RetVal_Failure;
		}

	

        hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle = fileptr ;

		if( Offline_CS_RetVal_Success != CdrFW_CreateCdrFileHeader( hCdrFWriterThreadContext_i ))
		{
				DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR,"Problem in Cdr File Rotation,Leaving\n");
				if( NULL != fileptr )
				{	fclose(fileptr);
                    hCdrFWriterThreadContext_i->oCdrFileObject.pFileHandle = NULL;
					DMBase_Free(hCdrFWriterModuleContext->hDiameterStackContext,(void**)&(hCdrFWriterThreadContext_i->oCdrFileObject.pucFileName));
                }
				return Offline_CS_RetVal_Failure;
		}
        
		DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_INFO,"Leaving\n");
		return Offline_CS_RetVal_Success;
 }
