
#include <stdio.h>
#include "rf_msg_api.h"
#include "AMPS_API.h"
#include "dmbase_utility.h"
#include "dmbase_ampswrappers.h"
#include "dmbase_parser.h"
#include "dmbase_initconfig.h"
#include "dmbase_transport_mgmt.h"
#include "dmbase_peer_mgmt.h"
#include "dmbase_realmtablemgmt.h"
#include "dmbase_appcallbackreg.h"
#include "offline_charging_server.h"
#include "dmbase_avp.h"
#include "dmbase_msgvalidate.h"
#include "rf_msg_api.h"
#include "dappcommon_srvrconfload.h"


int                         g_nDiamBaseTraceID = 0;
HDIAMETERSTACKCONTEXT       g_hDiameterStackContext = NULL;


int  main (int argc, char *argv[])
{

        printf("\n\t\t_ _ _*** Offline Charging Server Started ***_ _ _\t\t\n");


        AMPS_HANDLE                      hAMPSHandle= NULL;
        HDIAMETERSTACKCONTEXT            hDiameterStackContext= NULL;
        AMPS_BOOL                        btrace = 1;
        char                             pFilePath[] = "/root/Temp/tryAmps"	;
        DMBase_SList*	                 pListOfModuleInfo = NULL;


    	/*if ( 2 !=argc) 
    	{
    		printf("\n Incorrect parameters \n");
    		printf("\n Correct Usage:\n\t offline_charging_server PathToOffCSConfigFile.xml\n\n");
    		return 0;
    	}*/
    
	unsigned char ucConfigFile[1024] = {0};
	strcpy(ucConfigFile, DATADIR);
	strcat(ucConfigFile,"offcs_config.xml");

    	FILE*	fpTemp = NULL;
    	_Bool	bIsPathRelative = 0;
		_Bool 	bExitOnModFail  = 0;
    	//try opening as relative path
    	fpTemp = fopen(ucConfigFile,"r");
    	if (NULL == fpTemp ) 
    	{
    		bIsPathRelative = 1;
    	}
    	else
    	{
    		bIsPathRelative = 0;
    	}


        hAMPSHandle = AMPS_Init(AMPS_TIMER_RESOLUTION_20MS,btrace, pFilePath ,AMPS_PORT_POOL_SIZE_10, 20000);

        if (NULL == hAMPSHandle)
        {
            printf("AMPS_Init: AMPS_Init is failed.\n");
            return DMBase_Retval_Failure;
        }
 
       AMPS_SetTraceMode(hAMPSHandle, AMPS_TRACE_MODE_DISPLAY );
       AMPS_SetTraceLevel(hAMPSHandle, AMPS_TRACE_LEVEL_ERROR);//31 is the max
       //AMPS_SetTraceForDBEngine(hAMPSHandle); //do not use it for general case
       AMPS_SetTraceForSAPI(hAMPSHandle); //do not use it for general case

        AMPS_EnableLogging(hAMPSHandle, "testapp_log.txt");
        g_nDiamBaseTraceID = AMPS_GetTraceID(hAMPSHandle);
        AMPS_SetTraceForTraceID(hAMPSHandle, g_nDiamBaseTraceID);
        
  
        if( NULL == AMPS_SchedInit(hAMPSHandle,20,20,NULL,NULL,NULL, NULL))
        {
            printf("AMPS_Init: AMPS_SchedInit is failed ");
            return DMBase_Retval_Failure;
         }

      
        if(DMBase_Retval_Failure == DMBase_CreateDiameterContext(&hDiameterStackContext))
        {
            DMBASE_TRACE(g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR|DMBASE_TRACE_LEVEL_INFO|DMBASE_TRACE_LEVEL_DEBUG_2, "CreateDiameterContext failed.\n");
            return DMBase_Retval_Failure;
        }

        g_hDiameterStackContext = hDiameterStackContext;

        DMBase_SetFrameworkHandleInDiameterContext(hDiameterStackContext , hAMPSHandle);

        
        t_DiaStackInitData DiaStackInitDataObj;

          
        DMBase_MemsetZero(&DiaStackInitDataObj, sizeof(t_DiaStackInitData));
        t_DiaStackInitData*  pDiaStackInitData = &DiaStackInitDataObj;  


    	t_DiamOctetString	oAppConfigFilePathName;
    	if (0==bIsPathRelative) 
    	{
    			DMBase_CopyCharArrayToDiamString(&oAppConfigFilePathName,ucConfigFile);
    	}
    	else
    	{
    		//path is relative, make it absolute
    		char	pchCWD[1024];
    		getcwd(pchCWD,1024);
    		DMBase_CopyCharArrayToDiamString(&oAppConfigFilePathName,
                                             pchCWD);
    		DMBase_AppendCharStringToDiamString(&oAppConfigFilePathName,
    											"/");
    		DMBase_AppendCharStringToDiamString(&oAppConfigFilePathName,
    											argv[1]);
    
    	}

	t_Dappcommon_AppPath oAppPath = {0};
	strcpy(oAppPath.ucDatadir, DATADIR);
	strcpy(oAppPath.ucLibdir, LIBDIR);
    
		if ( DappCommon_Retval_Success != DappCommon_LoadServerConfigParamsFromFile(hDiameterStackContext ,
																					oAppConfigFilePathName, &oAppPath,
																					pDiaStackInitData ,
																					&pListOfModuleInfo)
	   )
		{
			DMBASE_TRACE( g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "ERROR: DappCommon_LoadServerConfigParamsFromFile Failed.\n");
			return DMBase_Retval_Failure;
		}


        
        if ( DMBase_Retval_Success != DMBase_InitDiameterStack ( hDiameterStackContext ,
                                                                 pDiaStackInitData ))
        {
            DMBASE_TRACE( g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "InitDiameterStack.\n");
            return DMBase_Retval_Failure;
        }

	bExitOnModFail = 1;
    	if ( DappCommon_Retval_Success != DappCommon_LoadDiamAppModules(hDiameterStackContext , pListOfModuleInfo, bExitOnModFail) )
	{
            DMBASE_TRACE( g_nDiamBaseTraceID, DMBASE_TRACE_LEVEL_ERROR, "DappCommon_LoadDiamAppModules Failed , Leaving.\n");
            return DMBase_Retval_Failure;
        }

        AMPS_Main(hAMPSHandle);
        
        return 0;

    }
