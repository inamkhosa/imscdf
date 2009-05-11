#ifndef _OFFCS_COM_DEFINES_CONTENTS_H
#define	_OFFCS_COM_DEFINES_CONTENTS_H

#ifdef	__cplusplus
extern "C" {
#endif


	#define OFFLINE_CS_LIST_SIZE              5

	typedef struct CorrIdObject
	{
		unsigned int 			unAcctRecordNumber;
		int					nResultCode;
		t_DiamOctetString		oSessionId;

    }t_CorrIdObject;


   #ifdef	__cplusplus
}
#endif

#endif	/* _OFFCS_COMM_CDR_CONTENTS_H */	
