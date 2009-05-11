#ifndef _RF_MMS_INFO_AVP_PARSER_H
#define	_RF_MMS_INFO_AVP_PARSER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include"RF_defines.h"
#include"RF_include.h"


t_Offline_CS_ReturnVal RF_AddMMSInfoAVPDataToEventData( unsigned char**     ppncDBEvtBuff_o ,
														unsigned int        unTotalEvtDataLengthInBytes_i,
														unsigned int*       punNewEvtDataLengthInBytes_o,
														HAVP                hToAVP_i);


t_Offline_CS_ReturnVal RF_AddOrignatorAddressAVPDatToEventData( unsigned char**     ppuncDBEvtBuff_o,
																 unsigned int        unTotalEvtDataLengthInBytes_i,
																 unsigned int*       punNewEvtDataLengthInBytes_o,
																 HAVP                hToAVP_i);


t_Offline_CS_ReturnVal RF_AddAddressDomainAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
															  unsigned int        unTotalEvtDataLengthInBytes_i,
															  unsigned int*       punNewEvtDataLengthInBytes_o,
															  HAVP                hToAVP_i);


t_Offline_CS_ReturnVal RF_AddRecepientAddressAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
																 unsigned int        unTotalEvtDataLengthInBytes_i,
																 unsigned int*       punNewEvtDataLengthInBytes_o,
																 HAVP                hToAVP_i);


t_Offline_CS_ReturnVal RF_AddMMContentTypeAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
															  unsigned int        unTotalEvtDataLengthInBytes_i,
															  unsigned int*       punNewEvtDataLengthInBytes_o,
															  HAVP                hToAVP_i);
t_Offline_CS_ReturnVal RF_AddAdditionalContentInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o,
																	  unsigned int        unTotalEvtDataLengthInBytes_i,
																	  unsigned int*       punNewEvtDataLengthInBytes_o,
																	  HAVP                hToAVP_i);


t_Offline_CS_ReturnVal RF_AddMessageClassAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
															 unsigned int        unTotalEvtDataLengthInBytes_i,
															 unsigned int*       punNewEvtDataLengthInBytes_o,
															 HAVP                hToAVP_i);
#ifdef	__cplusplus
}
#endif

#endif	/* _RF_MMS_INFO_AVP_PARSER_H */
