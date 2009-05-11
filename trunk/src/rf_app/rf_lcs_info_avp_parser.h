
#ifndef _RF_LCS_INFO_AVP_PARSER_H
#define	_RF_LCS_INFO_AVP_PARSER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include"RF_defines.h"
#include"RF_include.h"



t_Offline_CS_ReturnVal RF_AddLCSInfoAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
														unsigned int        unTotalEvtDataLengthInBytes_i,
														unsigned int*       punNewEvtDataLengthInBytes_o,
														HAVP                hToAVP_i);


/*t_Offline_CS_ReturnVal RF_AddLocationTypeAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
															 unsigned int        unTotalEvtDataLengthInBytes_i,
															 unsigned int*       punNewEvtDataLengthInBytes_o,
															 HAVP                hToAVP_i);*/

t_Offline_CS_ReturnVal RF_AddLCSClientIdAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
															unsigned int        unTotalEvtDataLengthInBytes_i,
															unsigned int*       punNewEvtDataLengthInBytes_o,
															HAVP                hToAVP_i);

t_Offline_CS_ReturnVal RF_AddLCSClientNameAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
															  unsigned int        unTotalEvtDataLengthInBytes_i,
															  unsigned int*       punNewEvtDataLengthInBytes_o,
															  HAVP                hToAVP_i);

t_Offline_CS_ReturnVal RF_AddLCSRequestorIdAVPDataToEventData( unsigned char**     ppuncDBEvtBuff_o ,
															   unsigned int        unTotalEvtDataLengthInBytes_i,
															   unsigned int*       punNewEvtDataLengthInBytes_o,
															   HAVP                hToAVP_i);

#ifdef	__cplusplus
}
#endif

#endif	/* _RF_LCS_INFO_AVP_PARSER_H */
