/*                                                                                                                     
********************************************************************************************************************** 
*                                                                                                                      
* COPYRIGHT RESERVED, 2010 Robert Bosch GmbH. All rights reserved.                                                     
* The reproduction, distribution and utilization of this document as well as the communication of its contents to      
* others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.       
* All rights reserved in the event of the grant of a patent, utility model or design.                                  
*                                                                                                                      
********************************************************************************************************************** 
*/                                                                                                                     


#include "Dcm.h"
#include "DcmCore_DslDsd_Inf.h"




/* Variables to store the Array of signals for different Data Types */
VAR (uint8,   DCM_VAR) Dcm_DspArraySignal_au8 [1];
VAR (uint16,  DCM_VAR) Dcm_DspArraySignal_au16[1];
VAR (uint32,  DCM_VAR) Dcm_DspArraySignal_au32[1];
VAR (sint8,   DCM_VAR) Dcm_DspArraySignal_as8 [1];
VAR (sint16,  DCM_VAR) Dcm_DspArraySignal_as16[1];
VAR (sint32,  DCM_VAR) Dcm_DspArraySignal_as32[1];



/* This array of structure gives info about received rxpduid belongs to which CONNECTION                        */
/* In Dcm_providRxbuffer() function DCM will get the Rx pduid.Now this rxpduid act as index,                    */
/* go to this array get the info of this rxpduid belongs to which connection                                    */
/* First physical rx pduids CONNECTION info is configured and then functional ids CONNECTION info is configured.*/
/* One index is generated in dcm_Lcfg.h file (DCM_INDEX_FUNC_RX_PDUID). If rxpduid is less than this index      */
/* means this request is physical request.*/
static CONST(uint8, DCM_CONST) Dcm_Dsld_rx_table_acu8[DCM_CFG_NUM_RX_PDUID]={
0x0, /*Rxpduid 0 belonging to the connection DcmDslConnection_0*/
};

static CONST(PduIdType, DCM_CONST) Dcm_DsldTxTable_cast[DCM_NUM_TX_PDUID] =
{

	PduRConf_PduRSrcPdu_

};

/* Generation of Diagnosis Buffers for Rx/Tx, one single buffer */
static VAR(uint8, DCM_VAR_NOINIT) Dcm_DsldDiagBuffer_au8[50+2];



/* Connection number acts as the index to get the channel and TxPdu information for a particular connetion */
/* Each connection will be associated with single TxPduId                                           */
static CONST(Dcm_Dsld_connType, DCM_CONST) Dcm_DsldConnTable_cast[DCM_CFG_NUM_CONN]=
{
};

/* Array for storing ComM channels configured in DCM  and their states*/
VAR(Dcm_Dsld_ComMChannel, DCM_VAR_POWER_ON_INIT) Dcm_active_commode_e[DCM_NUM_COMM_CHANNEL]=
{
{ComMConf_ComMChannel_ComMChannel_CAN,DCM_DSLD_NO_COM_MODE }
};



/* configuration of protocol table */
static CONST(Dcm_Dsld_protocol_tableType, DCM_CONST) Dcm_DsldProtocol_cast[]=
{
    { 
	  &Dcm_DsldDiagBuffer_au8[0],     /* Tx buffer address */
	  &Dcm_DsldDiagBuffer_au8[0],     /* Rx buffer address */

	  50,                           /* Tx buffer size          */
	  50,                           /* Rx buffer size          */
	  DCM_UDS_ON_CAN,               /* Protocol id             */
	  0,                            /* Id of Sid table         */
	  0,                    /* Preemption Level      */

  	  0,  	       /* Index to the Temporary Pdu Info Structure */
	  FALSE,							/* Is Endianness conversion enabled for a Protocol */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
	  ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
	   (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)), /* Configuration mask which indicates the availability of protocol in different configsets*/
#endif
	  FALSE,			/* Silently ignore new request on this protocol if received and rejected during pre-emption assertion */
	  TRUE              /* WaitPend sent on transit to Boot */
   }

};





/* Subservice of SID 0x28 of Service table DcmDsdServiceTable_0*/
static CONST(Dcm_Dsld_SubServiceType, DCM_CONST) Dcm_SrvTab0_SubSrv4_acs[]=
{

	{
	  0xfffffffful,                 			/* Allowed in all sessions */
	  
      0xfffffffful,           		    						/* Allowed in all security levels */
	  NULL_PTR,									/* No Mode Rule Reference has been configured for this Sub-Service, thus no function is referred here */
      0x0                						/* Subservice Identifier */
    }
,

	{
	  0xfffffffful,                 			/* Allowed in all sessions */
	  
      0xfffffffful,           		    						/* Allowed in all security levels */
	  NULL_PTR,									/* No Mode Rule Reference has been configured for this Sub-Service, thus no function is referred here */
      0x1                						/* Subservice Identifier */
    }
};



/* Service table for DCM_UDS_ON_CAN  protocol  */
static CONST(Dcm_Dsld_ServiceType, DCM_CONST) Dcm_Dsld_sid_table0_acs[]=
{
    {

	  0xfffffffful,                 			/* Allowed in all sessions                */

      0xfffffffful,           		/* Allowed in all security levels                */
	  NULL_PTR,						/* No Mode Rule Reference has been configured for this Service, thus no function is referred here */
	  &Dcm_DcmReadDataByIdentifier,  /* Service handler                        */
	  &Dcm_Dsp_RdbiIni,                 /* Init function of service               */
	  0x22,                         /* Service id              */
      FALSE,                  /* Is sub function exist for this service?*/
	  TRUE,                         /* Info about location of service */
	  0x00,                         /* Dummy (unused)                         */

	  NULL_PTR,        				/* Pointer to subservice table            */
       0                                                       /* Number of subservices */
    },
    {

	  0xfffffffful,                 			/* Allowed in all sessions                */

      0xfffffffful,           		/* Allowed in all security levels                */
	  NULL_PTR,						/* No Mode Rule Reference has been configured for this Service, thus no function is referred here */
	  &Dcm_DcmWriteDataByIdentifier, /* Service handler                        */
	  &Dcm_Dcm_WDBIInit,                /* Init function of service               */
	  0x2E,                         /* Service id              */
      FALSE,                  /* Is sub function exist for this service?*/
	  TRUE,                         /* Info about location of service */
	  0x00,                         /* Dummy (unused)                         */

	  NULL_PTR,        				/* Pointer to subservice table            */
       0                                                       /* Number of subservices */
    },
    {

	  0xfffffffful,                 			/* Allowed in all sessions                */

      0xfffffffful,           		/* Allowed in all security levels                */
	  NULL_PTR,						/* No Mode Rule Reference has been configured for this Service, thus no function is referred here */
	  &Dcm_DcmInputOutputControlByIdentifier, /* Service handler                        */
	  &Dcm_Dsp_IOCBI_Ini,               /* Init function of service               */
	  0x2F,                         /* Service id              */
      FALSE,                  /* Is sub function exist for this service?*/
	  TRUE,                         /* Info about location of service */
	  0x00,                         /* Dummy (unused)                         */

	  NULL_PTR,        				/* Pointer to subservice table            */
       0                                                       /* Number of subservices */
    },
    {

	  0xfffffffful,                 			/* Allowed in all sessions                */

      0xfffffffful,           		/* Allowed in all security levels                */
	  NULL_PTR,						/* No Mode Rule Reference has been configured for this Service, thus no function is referred here */
	  &Dcm_DcmTesterPresent,         /* Service handler                        */
	  NULL_PTR,                        /* Init function of service               */
	  0x3E,                         /* Service id              */
      TRUE,                         /* Is sub function exist for this service?*/
	  TRUE,                         /* Info about location of service */
	  0x00,                         /* Dummy (unused)                         */

	  NULL_PTR,        				/* Pointer to subservice table            */
       0                                                       /* Number of subservices */
    },
    {

	  0xfffffffful,                 			/* Allowed in all sessions                */

      0xfffffffful,           		/* Allowed in all security levels                */
	  NULL_PTR,						/* No Mode Rule Reference has been configured for this Service, thus no function is referred here */
	  &Dcm_DcmCommunicationControl,  /* Service handler                        */
	  NULL_PTR,                        /* Init function of service               */
	  0x28,                         /* Service id              */
      TRUE,                         /* Is sub function exist for this service?*/
	  TRUE,                         /* Info about location of service */
	  0x00,                         /* Dummy (unused)                         */

 	  Dcm_SrvTab0_SubSrv4_acs,      /* Pointer to subservice table            */
      2                             /* Number of subservices */
    }
};


/* Sid table configuration */
static CONST(Dcm_Dsld_sid_tableType, DCM_CONST) Dcm_DsldServiceTables_cast[]=
{
    {
	  Dcm_Dsld_sid_table0_acs,      /* Pointer to service table               */
	  5,                            /* No of services in the Service table    */
	  0x7F,                         /* NRC for service not supported for active session                        */
	  0xFF                           /* Dcm_DcmControlDTCSetting is not present in the service list, and invalid index is printed */
    }
};

/* This is the array contains the supported sessions in this ECU                             */
/* sessions id are configured here (eg: default session=0x01, programming session=0x02)      */
/* number of sessions in DSC service is same as here                                         */
/* this lookup table is used to calculate the bit mask from session ID                       */
/* these ids are always in ascending order                                                    */

/* The DscmDsp session and security configuration is used for the UDS protocol */
static CONST(uint8, DCM_CONST) Dcm_DsldSupportedSessions_cau8[]= {0x1,0x2,0x3};

#if (DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
CONST(uint8, DCM_CONST) Dcm_Dsld_KWPsupported_sessions_acu8[]= {0x86};
#endif











/* this is the array contains the supported security level in this ECU                       */
/* security levels are configured here (eg: DCM_SEC_LEV_LOCKED =0x00)                        */
/* number of security levels here is one more than in SECA service                           */
/* this lookup table is used to calculate the bit mask from security level                   */
/* this ids are always in ascending order                                                    */
/* Allowed range of security levels are 0x00,0x01,0x02,.....0x3F                             */

static CONST(uint8, DCM_CONST) Dcm_Dsld_supported_security_acu8[]= {0x0};



/* final structre of DSL-DSD */
CONST(Dcm_Dsld_confType, DCM_CONST) Dcm_Dsld_Conf_cs =
{
   Dcm_Dsld_rx_table_acu8,                 /* Reference to Rx table array */
   Dcm_DsldTxTable_cast,                      /* Reference to Tx table array */
   Dcm_DsldConnTable_cast,                /* Reference to Connection table array */
   Dcm_DsldProtocol_cast,                  /* Reference to protocol table */
   Dcm_DsldServiceTables_cast,            /* Reference to sid table      */
   Dcm_DsldSupportedSessions_cau8,       /* Session look up table       */
   Dcm_Dsld_supported_security_acu8        /* Security look up table      */
};



