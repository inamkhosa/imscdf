

#ifndef _RF_IMS_INFO_AVP_PARSER_H
#define	_RF_IMS_INFO_AVP_PARSER_H

#ifdef	__cplusplus
extern "C" {
#endif

	#include "RF_defines.h"
	#include "RF_include.h"
	#include "RFModule.h"



t_Offline_CS_ReturnVal inline Rf_AddIMSInfoAVPToCdrContntsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																  HAVP					  	hToAVP_i,
																  t_IMSInformation*		poIMSInfoCDRContent_o);


t_Offline_CS_ReturnVal inline Rf_AddEventTypeAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																	 HAVP					  	hToAVP_i,
																	 t_Event_Type*				poEventTypeCDRContent_o);

t_Offline_CS_ReturnVal inline Rf_AddTimeStampsAVPToContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																   HAVP					hToAVP_i,
																   t_TimeStamps*			poTimeStampCDRContent_o);

t_Offline_CS_ReturnVal inline Rf_AddAppServersInfoAVPToContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																	   HAVP					hToAVP_i,
																	   t_AppServerInfoList*	poAppServerListCDRContent_o);

t_Offline_CS_ReturnVal inline Rf_AddInterOperatorIdentifierAVPToContentsStruct( HRFAPPLICATIONCONTEXT 				hRFModuleContext_i,
																				HAVP								hToAVP_i,
																				t_InterOperatorIdentifierList*		poInterOpratorIdentifierCDRContent_o);

t_Offline_CS_ReturnVal inline Rf_AddMessageBodyAVPToContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																	HAVP					hToAVP_i,
																	t_MessageBodyList*		poMessageBodyCDRContent_o);

t_Offline_CS_ReturnVal inline Rf_AddSDPMediaComponentsAVPToContentsStruct( HRFAPPLICATIONCONTEXT 			hRFModuleContext_i,
																		   HAVP							hToAVP_i,
																		   t_SDPMediaComponentList*		poSDPMediaComponentsCDRContent_o);

#ifdef	__cplusplus
}
#endif

#endif	/* _RF_IMS_INFO_AVP_PARSER_H */

