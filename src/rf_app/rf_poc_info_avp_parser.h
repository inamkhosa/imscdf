
#ifndef _RF_POC_INFO_AVP_PARSER_H
#define	_RF_POC_INFO_AVP_PARSER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include"RF_defines.h"
#include"RF_include.h"


t_Offline_CS_ReturnVal RF_AddPoCInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
														unsigned int        unTotalEvtDataLengthInBytes_i,
														unsigned int*       punNewEvtDataLengthInBytes_o,
														HAVP                hToAVP_i);


t_Offline_CS_ReturnVal RF_AddParticipantGroupAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
																 unsigned int        unTotalEvtDataLengthInBytes_i,
																 unsigned int*       punNewEvtDataLengthInBytes_o,
																 HAVP                hToAVP_i);

t_Offline_CS_ReturnVal RF_AddPoCUserRoleAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
															unsigned int        unTotalEvtDataLengthInBytes_i,
															unsigned int*       punNewEvtDataLengthInBytes_o,
															HAVP                hToAVP_i);


t_Offline_CS_ReturnVal RF_AddTalkBurstExchangeAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
																  unsigned int        unTotalEvtDataLengthInBytes_i,
																  unsigned int*       punNewEvtDataLengthInBytes_o,
																  HAVP                hToAVP_i);


#ifdef	__cplusplus
}
#endif

#endif	/* _RF_POC_INFO_AVP_PARSER_H */
