#ifndef _RF_PS_INFO_AVP_PARSER_H
#define	_RF_PS_INFO_AVP_PARSER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include"RF_defines.h"
#include"RF_include.h"
#include "RFModule.h"



t_Offline_CS_ReturnVal Rf_AddPSInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
														  HAVP					  	hToAVP_i,
														  t_PSInformation*			poPSInfoCDRContent_o);

t_Offline_CS_ReturnVal RF_AddOfflineChargingAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 	hRFModuleContext_i,
																   HAVP					  	hToAVP_i,
																   t_OfflineCharging*		poOfflineChargingCDRContent_o);



t_Offline_CS_ReturnVal RF_AddMultipleServicesCreditControlAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 					hRFModuleContext_i,
																				 HAVP					  				hToAVP_i,
																				 t_MultipleServicesCreditControlList*	poMultiServCreditControlListCDRContent_o);


t_Offline_CS_ReturnVal RF_AddUsedServiceUnitAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 					hRFModuleContext_i,
																   HAVP					  					hToAVP_i,
																   t_UsedServiceUnitList*					poUsedServiceUnitListCDRContent_o);

t_Offline_CS_ReturnVal RF_AddGSUPoolReferenceAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 					hRFModuleContext_i,
																	HAVP					  				hToAVP_i,
																	t_GSUPoolReferenceList*					poGSUPoolReferenceListCDRContent_o);


t_Offline_CS_ReturnVal RF_AddUnitValueAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
															 HAVP					  	hToAVP_i,
															 t_UnitValue*				poUnitValueCDRContent_o);
  
t_Offline_CS_ReturnVal RF_AddFinalUnitIndicationAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																	   HAVP					  		hToAVP_i,
																	   t_FinalUnitIndication*		poFinalUnitIndicationCDRContent_o);
				
t_Offline_CS_ReturnVal RF_AddRedirectServerAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																  HAVP					  		hToAVP_i,
																  t_RedirectServer*				poRedirectServerCDRContent_o);

t_Offline_CS_ReturnVal RF_AddTriggerAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
														   HAVP					  		hToAVP_i,
														   t_Trigger*					poTriggerCDRContent_o);


t_Offline_CS_ReturnVal RF_AddPSFurnishChargingInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 			hRFModuleContext_i,
																		 HAVP					  		hToAVP_i,
																		 t_PSFurnishingChargingInfo*	poPSFurnishingChargingInfoCDRContent_o);


t_Offline_CS_ReturnVal RF_AddAFCorrelationInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 				hRFModuleContext_i,
																	 HAVP					  			hToAVP_i,
																	 t_AFCorrelationInformationList*	poAFCorrelationInfoListCDRContent_o);


t_Offline_CS_ReturnVal RF_AddEnvelopeAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
															HAVP					  	hToAVP_i,
															t_EnvelopeList*				poEnvelopeListCDRContent_o);


t_Offline_CS_ReturnVal RF_AddTimeQuotaMechnicmAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																	 HAVP					  	hToAVP_i,
																	 t_TimeQuotaMechanism*		poTimeQuotaMechanismCDRContent_o);

t_Offline_CS_ReturnVal RF_AddServiceSpecificInfoAVPToCdrContentsStruct( HRFAPPLICATIONCONTEXT 		hRFModuleContext_i,
																	   HAVP					  		hToAVP_i,
																	   t_ServiceSpecificInfoList*	poServiceSpecificInfoListCDRContent_o);

#ifdef	__cplusplus
}
#endif

#endif	/* _RF_PS_INFO_AVP_PARSER_H */

