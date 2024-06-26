{ "!<", kwNOT_LSS },
{ "!=", kwNEQ },
{ "!>", kwNOT_GTR },
//{ "(", '(' },
//{ ")", ')' },
//{ ",", ',' },
{ ":=", kwBIND_PARAM },
{ "^<", kwNOT_LSS },   // Alias of !<
{ "^=", kwNEQ }, // Alias of !=
{ "^>", kwNOT_GTR }, // Alias of !>
{ "||", kwCONCATENATE },
{ "~<", kwNOT_LSS }, // Alias of !<
{ "~=", kwNEQ }, // Alias of !=
{ "~>", kwNOT_GTR }, // Alias of !>
{ "<",  kwLESS },
{ "<=", kwLEQ },
{ "<>", kwNEQ },  // Alias of !=
{ ">=", kwGEQ },
{ "=",  kwEQU },
{ ">",  kwGRA },
{ "ABS", kwABS },
{ "ABSOLUTE", kwABSOLUTE },
{ "ACCENT", kwACCENT },
{ "ACOS", kwACOS },
{ "ACOSH", kwACOSH },
{ "ACTION", kwACTION },
{ "ACTIVE", kwACTIVE },
{ "ADD", kwADD },
{ "ADMIN", kwADMIN },
{ "AFTER", kwAFTER },
{ "ALL", kwALL },
{ "ALTER", kwALTER },
{ "ALWAYS", kwALWAYS },
{ "AND", kwAND },
{ "ANY", kwANY },
{ "AS", kwAS },
{ "ASC", kwASCENDING },  // Alias of ASCENDING
{ "ASCENDING", kwASCENDING },
{ "ASCII_CHAR", kwASCII_CHAR },
{ "ASCII_VAL", kwASCII_VAL },
{ "ASIN", kwASIN },
{ "ASINH", kwASINH },
{ "AT", kwAT },
{ "ATAN", kwATAN },
{ "ATAN2", kwATAN2 },
{ "ATANH", kwATANH },
{ "AUTO", kwAUTO },
{ "AUTODDL", kwAUTODDL },
{ "AVG", kwAVG },
{ "BACKUP", kwBACKUP }, //fb2.0
{ "BASE64_DECODE", kwBASE64_DECODE },
{ "BASE64_ENCODE", kwBASE64_ENCODE },
{ "BASED", kwBASED },
{ "BASENAME", kwBASENAME },
{ "BEFORE", kwBEFORE },
{ "BEGIN", kwBEGIN },
{ "BETWEEN", kwBETWEEN },
{ "BIGINT", kwBIGINT },
{ "BIN_AND", kwBIN_AND },
{ "BIN_NOT", kwBIN_NOT },
{ "BIN_OR", kwBIN_OR },
{ "BIN_SHL", kwBIN_SHL },
{ "BIN_SHR", kwBIN_SHR },
{ "BIN_XOR", kwBIN_XOR },
{ "BINARY", kwBINARY },
{ "BIND", kwBIND },
{ "BIT_LENGTH", kwBIT_LENGTH }, //fb2.0
{ "BLOB", kwBLOB },
{ "BLOBEDIT", kwBLOBEDIT },
{ "BLOCK", kwBLOCK }, //fb2.0
{ "BODY", kwBODY }, //FB3.0
{ "BOOLEAN", kwBOOLEAN },
{ "BOTH", kwBOTH }, //fb2.0
{ "BREAK", kwBREAK },
{ "BUFFER", kwBUFFER },
{ "BY", kwBY },
{ "CACHE", kwCACHE },
{ "CALLER", kwCALLER },
{ "CASCADE", kwCASCADE },
{ "CASE", kwCASE },
{ "CAST", kwCAST },
{ "CEIL", kwCEIL },  // Alias of CEILING
{ "CEILING", kwCEIL },
{ "CHAR", kwCHAR }, //fb2.0
{ "CHAR_LENGTH", kwCHAR_LENGTH },
{ "CHAR_TO_UUID", kwCHAR_TO_UUID },
{ "CHARACTER", kwCHARACTER },
{ "CHARACTER_LENGTH", kwCHARACTER_LENGTH }, //fb2.0
{ "CHECK", kwCHECK },
{ "CHECK_POINT_LEN", kwCHECK_POINT_LEN },
{ "CLEAR", kwCLEAR },
{ "CLOSE", kwCLOSE }, //fb2.0
{ "COALESCE", kwCOALESCE },
{ "COLLATE", kwCOLLATE },
{ "COLLATION", kwCOLLATION },
{ "COLUMN", kwCOLUMN },
{ "COMMENT", kwCOMMENT },
{ "COMMIT", kwCOMMIT },
{ "COMMITTED", kwCOMMITTED },
{ "COMMON", kwCOMMON },
{ "COMPARE_DECFLOAT", kwCOMPARE_DECFLOAT },
{ "COMPILETIME", kwCOMPILETIME },
{ "COMPUTED", kwCOMPUTED },
{ "CONDITIONAL", kwCONDITIONAL },
{ "CONNECT", kwCONNECT },
{ "CONNECTIONS", kwCONNECTIONS },
{ "CONSISTENCY", kwCONSISTENCY },
{ "CONSTRAINT", kwCONSTRAINT },
{ "CONTAINING", kwCONTAINING },
{ "CONTINUE", kwCONTINUE },
{ "CORR", kwCORR },
{ "COS", kwCOS },
{ "COSH", kwCOSH },
{ "COT", kwCOT },
{ "COUNT", kwCOUNT },
{ "COUNTER", kwCOUNTER },
{ "COVAR_POP", kwCOVAR_POP },
{ "COVAR_SAMP", kwCOVAR_SAMP },
{ "CREATE", kwCREATE },
{ "CROSS", kwCROSS }, //fb2.0
{ "CRYPT_HASH", kwCRYPT_HASH },
{ "CSTRING", kwCSTRING },
{ "CTR_BIG_ENDIAN", kwCTR_BIG_ENDIAN },
{ "CTR_LENGTH", kwCTR_LENGTH },
{ "CTR_LITTLE_ENDIAN", kwCTR_LITTLE_ENDIAN },
{ "CUME_DIST", kwCUME_DIST },
{ "CURRENT", kwCURRENT },
{ "CURRENT_CONNECTION", kwCURRENT_CONNECTION },
{ "CURRENT_DATE", kwCURRENT_DATE },
{ "CURRENT_ROLE", kwCURRENT_ROLE },
{ "CURRENT_TIME", kwCURRENT_TIME },
{ "CURRENT_TIMESTAMP", kwCURRENT_TIMESTAMP },
{ "CURRENT_TRANSACTION", kwCURRENT_TRANSACTION },
{ "CURRENT_USER", kwCURRENT_USER },
{ "CURSOR", kwCURSOR },
{ "DATA", kwDATA },
{ "DATABASE", kwDATABASE },
{ "DATE", kwDATE },
{ "DATEADD", kwDATEADD },
{ "DATEDIFF", kwDATEDIFF },
{ "DAY", kwDAY },
{ "DB_KEY", kwDB_KEY },
{ "DDL", kwDDL },
{ "DEC", kwDEC },
{ "DECFLOAT", kwDECFLOAT },
{ "DECIMAL", kwDECIMAL },
{ "DECLARE", kwDECLARE },
{ "DECODE", kwDECODE },
{ "DECRYPT", kwDECRYPT },
{ "DEFAULT", kwDEFAULT },
{ "DEFINER", kwDEFINER },
{ "DELETE", kwDELETE },
{ "DELETING", kwDELETING },
{ "DENSE_RANK", kwDENSE_RANK },
{ "DESC", kwDESCENDING },
{ "DESCENDING", kwDESCENDING },
{ "DESCRIBE", kwDESCRIBE },
{ "DESCRIPTOR", kwDESCRIPTOR },
{ "DETERMINISTIC", kwDETERMINISTIC },
{ "DIFFERENCE", kwDIFFERENCE }, //fb2.0
{ "DISABLE", kwDISABLE },
{ "DISCONNECT", kwDISCONNECT },
{ "DISPLAY", kwDISPLAY },
{ "DISTINCT", kwDISTINCT },
{ "DO", kwDO },
{ "DOMAIN", kwDOMAIN },
{ "DOUBLE", kwDOUBLE },
{ "DROP", kwDROP },
{ "ECHO", kwECHO },
{ "EDIT", kwEDIT },
{ "ELSE", kwELSE },
{ "ENABLE", kwENABLE },
{ "ENCRYPT", kwENCRYPT },
{ "END", kwEND },
{ "ENGINE", kwENGINE },
{ "ENTRY_POINT", kwENTRY_POINT },
{ "ESCAPE", kwESCAPE },
{ "EVENT", kwEVENT },
{ "EXCEPTION", kwEXCEPTION },
{ "EXCESS", kwEXCESS },
{ "EXCLUDE", kwEXCLUDE },
{ "EXECUTE", kwEXECUTE },
{ "EXISTS", kwEXISTS },
{ "EXIT", kwEXIT },
{ "EXP", kwEXP },
{ "EXTENDED", kwEXTENDED },
{ "EXTERN", kwEXTERN },
{ "EXTERNAL", kwEXTERNAL },
{ "EXTRACT", kwEXTRACT },
{ "FALSE", kwFALSE },
{ "FETCH", kwFETCH }, //fb2.0
{ "FILE", kwFILE },
{ "FILTER", kwFILTER },
{ "FIRST", kwFIRST },
{ "FIRST_DAY", kwFIRST_DAY },
{ "FIRST_VALUE", kwFIRST_VALUE },
{ "FIRSTNAME", kwFIRSTNAME },
{ "FLOAT", kwFLOAT },
{ "FLOOR", kwFLOOR },
{ "FOLLOWING", kwFOLLOWING },
{ "FOR", kwFOR },
{ "FOREIGN", kwFOREIGN },
{ "FOUND", kwFOUND },
{ "FREE_IT", kwFREE_IT },
{ "FROM", kwFROM },
{ "FULL", kwFULL },
{ "FUNCTION", kwFUNCTION },
{ "GDSCODE", kwGDSCODE },
{ "GEN_ID", kwGEN_ID },
{ "GEN_UUID", kwGEN_UUID },
{ "GENERATED", kwGENERATED },
{ "GENERATOR", kwGENERATOR },
{ "GLOBAL", kwGLOBAL },
{ "GOTO", kwGOTO },
{ "GRANT", kwGRANT },
{ "GRANTED", kwGRANTED },
{ "GROUP", kwGROUP },
{ "GROUP_COMMIT_", kwGROUP_COMMIT_ },
{ "GROUP_COMMIT_WAIT", kwGROUP_COMMIT_WAIT },
{ "HASH", kwHASH },
{ "HAVING", kwHAVING },
{ "HELP", kwHELP },
{ "HEX_DECODE", kwHEX_DECODE },
{ "HEX_ENCODE", kwHEX_ENCODE },
{ "HOUR", kwHOUR },
{ "IDENTITY", kwIDENTITY },
{ "IDLE", kwIDLE },
{ "IF", kwIF },
{ "IGNORE", kwIGNORE },
{ "IIF", kwIIF },
{ "IMMEDIATE", kwIMMEDIATE },
{ "IN", kwIN },
{ "INACTIVE", kwINACTIVE },
{ "INCLUDE", kwINCLUDE },
{ "INCREMENT", kwINCREMENT },
{ "INDENTITY", kwINDENTITY },
{ "INDEX", kwINDEX },
{ "INDICATOR", kwINDICATOR },
{ "INIT", kwINIT },
{ "INNER", kwINNER },
{ "INPUT", kwINPUT },
{ "INPUT_TYPE", kwINPUT_TYPE },
{ "INSENSITIVE", kwINSENSITIVE },
{ "INSERT", kwINSERT },
{ "INSERTING", kwINSERTING },
{ "INT", kwINT },
{ "INT128", kwINT128 },
{ "INTEGER", kwINTEGER },
{ "INTO", kwINTO },
{ "INVOKER", kwINVOKER },
{ "IS", kwIS },
{ "ISOLATION", kwISOLATION },
{ "ISQL", kwISQL },
{ "IV", kwIV },
{ "JOIN", kwJOIN },
{ "KEY", kwKEY },
{ "LAG", kwLAG },
{ "LAST", kwLAST },
{ "LAST_DAY", kwLAST_DAY },
{ "LAST_VALUE", kwLAST_VALUE },
{ "LASTNAME", kwLASTNAME },
{ "LATERAL", kwLATERAL },
{ "LC_MESSAGES", kwLC_MESSAGES },
{ "LC_TYPE", kwLC_TYPE },
{ "LEAD", kwLEAD },
{ "LEADING", kwLEADING }, //fb2.0
{ "LEAVE", kwLEAVE },
{ "LEFT", kwLEFT },
{ "LEGACY", kwLEGACY },
{ "LENGTH", kwLENGTH },
{ "LEV", kwLEV },
{ "LEVEL", kwLEVEL },
{ "LIFETIME", kwLIFETIME },
{ "LIKE", kwLIKE },
{ "LIMBO", kwLIMBO },
{ "LINGER", kwLINGER },
{ "LIST", kwLIST },
{ "LN", kwLN },
{ "LOCAL", kwLOCAL },
{ "LOCALTIME", kwLOCALTIME },
{ "LOCALTIMESTAMP", kwLOCALTIMESTAMP },
{ "LOCK", kwLOCK },
{ "LOG", kwLOG },
{ "LOG_BUF_SIZE", kwLOG_BUF_SIZE },
{ "LOG10", kwLOG10 },
{ "LOGFILE", kwLOGFILE },
{ "LONG", kwLONG },
{ "LOWER", kwLOWER }, //fb2.0
{ "LPAD", kwLPAD },
{ "LPARAM", kwLPARAM },
{ "MAKE_DBKEY", kwMAKE_DBKEY },
{ "MANUAL", kwMANUAL },
{ "MAPPING", kwMAPPING },
{ "MATCHED", kwMATCHED },
{ "MATCHING", kwMATCHING },
{ "MAX", kwMAX },
{ "MAX_SEGMENT", kwMAX_SEGMENT },
{ "MAXIMUM", kwMAXIMUM },
{ "MAXIMUM_SEGMENT", kwMAXIMUM_SEGMENT },
{ "MAXVALUE", kwMAXVALUE },
{ "MERGE", kwMERGE },
{ "MESSAGE", kwMESSAGE },
{ "MIDDLENAME", kwMIDDLENAME },
{ "MILLISECOND", kwMILLISECOND },
{ "MIN", kwMIN },
{ "MINIMUM", kwMINIMUM },
{ "MINUTE", kwMINUTE },
{ "MINVALUE", kwMINVALUE },
{ "MOD", kwMOD },
{ "MODE", kwMODE },
{ "MODULE_NAME", kwMODULE_NAME },
{ "MONTH", kwMONTH },
{ "NAME", kwNAME },
{ "NAMES", kwNAMES },
{ "NATIONAL", kwNATIONAL },
{ "NATIVE", kwNATIVE },
{ "NATURAL", kwNATURAL },
{ "NCHAR", kwNCHAR },
{ "NEXT", kwNEXT },
{ "NO", kwNO },
{ "NOAUTO", kwNOAUTO },
{ "NORMALIZE_DECFLOAT", kwNORMALIZE_DECFLOAT },
{ "NOT", kwNOT },
{ "NTH_VALUE", kwNTH_VALUE },
{ "NTILE", kwNTILE },
{ "NULL", kwNULL },
{ "NULLIF", kwNULLIF },
{ "NULLS", kwNULLS },
{ "NUM_LOG_BUFS", kwNUM_LOG_BUFS },
{ "NUMBER", kwNUMBER },
{ "NUMERIC", kwNUMERIC },
{ "OCTET_LENGTH", kwOCTET_LENGTH }, //fb2.0
{ "OF", kwOF },
{ "OFFSET", kwOFFSET },
{ "OLDEST", kwOLDEST },
{ "ON", kwON },
{ "ONLY", kwONLY },
{ "OPEN", kwOPEN }, //fb2.0
{ "OPTION", kwOPTION },
{ "OR", kwOR },
{ "ORDER", kwORDER },
{ "OS_NAME", kwOS_NAME },
{ "OTHERS", kwOTHERS },
{ "OUTER", kwOUTER },
{ "OUTPUT", kwOUTPUT },
{ "OUTPUT_TYPE", kwOUTPUT_TYPE },
{ "OVER", kwOVER },
{ "OVERFLOW", kwOVERFLOW },
{ "OVERLAY", kwOVERLAY },
{ "OVERRIDING", kwOVERRIDING },
{ "PACKAGE", kwPACKAGE },
{ "PAD", kwPAD },
{ "PAGE", kwPAGE },
{ "PAGE_SIZE", kwPAGE_SIZE },
{ "PAGELENGTH", kwPAGELENGTH },
{ "PAGES", kwPAGES },
{ "PARAMETER", kwPARAMETER },
{ "PARTITION", kwPARTITION },
{ "PASSWORD", kwPASSWORD },
{ "PERCENT", kwPERCENT },
{ "PERCENT_RANK", kwPERCENT_RANK },
{ "PI", kwPI },
{ "PLACING", kwPLACING },
{ "PLAN", kwPLAN },
{ "PLUGIN", kwPLUGIN },
{ "POOL", kwPOOL },
{ "POSITION", kwPOSITION },
{ "POST_EVENT", kwPOST_EVENT },
{ "POWER", kwPOWER },
{ "PRECEDING", kwPRECEDING },
{ "PRECISION", kwPRECISION },
{ "PREPARE", kwPREPARE },
{ "PRESERVE", kwPRESERVE },
{ "PRIMARY", kwPRIMARY },
{ "PRIOR", kwPRIOR },
{ "PRIVILEGE", kwPRIVILEGE },
{ "PRIVILEGES", kwPRIVILEGES },
{ "PROCEDURE", kwPROCEDURE },
{ "PROTECTED", kwPROTECTED },
{ "PUBLIC", kwPUBLIC },
{ "PUBLICATION", kwPUBLICATION },
{ "QUANTIZE", kwQUANTIZE },
{ "QUIT", kwQUIT },
{ "RAND", kwRAND },
{ "RANGE", kwRANGE },
{ "RANK", kwRANK },
{ "RAW_PARTITIONS", kwRAW_PARTITIONS },
{ "RDB$DB_KEY", kwDB_KEY },
{ "RDB$ERROR", kwRDB_ERROR },
{ "RDB$GET_CONTEXT", kwRDB_GET_CONTEXT },
{ "RDB$GET_TRANSACTION_CN", kwRDB_GET_TRANSACTION_CN },
{ "RDB$RECORD_VERSION", kwRDB_RECORD_VERSION },
{ "RDB$ROLE_IN_USE", kwRDB_ROLE_IN_USE },
{ "RDB$SET_CONTEXT", kwRDB_SET_CONTEXT },
{ "RDB$SYSTEM_PRIVILEGE", kwRDB_SYSTEM_PRIVILEGE },
{ "READ", kwREAD },
{ "REAL", kwREAL },
{ "RECORD_VERSION", kwRECORD_VERSION },
{ "RECREATE", kwRECREATE },
{ "RECURSIVE", kwRECURSIVE },
{ "REFERENCES", kwREFERENCES },
{ "REGR_AVGX", kwREGR_AVGX },
{ "REGR_AVGY", kwREGR_AVGY },
{ "REGR_COUNT", kwREGR_COUNT },
{ "REGR_INTERCEPT", kwREGR_INTERCEPT },
{ "REGR_R2", kwREGR_R2 },
{ "REGR_SLOPE", kwREGR_SLOPE },
{ "REGR_SXX", kwREGR_SXX },
{ "REGR_SXY", kwREGR_SXY },
{ "REGR_SYY", kwREGR_SYY },
{ "RELATIVE", kwRELATIVE },
{ "RELEASE", kwRELEASE },
{ "REPLACE", kwREPLACE },
{ "REQUESTS", kwREQUESTS },
{ "RESERV", kwRESERV },
{ "RESERVING", kwRESERVING },
{ "RESET", kwRESET },
{ "RESTART", kwRESTART },
{ "RESTRICT", kwRESTRICT },
{ "RETAIN", kwRETAIN },
{ "RETURN", kwRETURN },
{ "RETURNING", kwRETURNING },
{ "RETURNING_VALUES", kwRETURNING_VALUES },
{ "RETURNS", kwRETURNS },
{ "REVERSE", kwREVERSE },
{ "REVOKE", kwREVOKE },
{ "RIGHT", kwRIGHT },
{ "ROLE", kwROLE },
{ "ROLLBACK", kwROLLBACK },
{ "ROUND", kwROUND },
{ "ROW", kwROW },
{ "ROW_COUNT", kwROW_COUNT },
{ "ROW_NUMBER", kwROW_NUMBER },
{ "ROWS", kwROWS }, //fb2.0
{ "RPAD", kwRPAD },
{ "RSA_DECRYPT", kwRSA_DECRYPT },
{ "RSA_ENCRYPT", kwRSA_ENCRYPT },
{ "RSA_PRIVATE", kwRSA_PRIVATE },
{ "RSA_PUBLIC", kwRSA_PUBLIC },
{ "RSA_SIGN", kwRSA_SIGN },
{ "RSA_VERIFY", kwRSA_VERIFY },
{ "RUNTIME", kwRUNTIME },
{ "SALT_LENGTH", kwSALT_LENGTH },
{ "SAVEPOINT", kwSAVEPOINT },
{ "SCALAR_ARRAY", kwSCALAR_ARRAY },
{ "SCHEMA", kwSCHEMA },
{ "SCROLL", kwSCROLL },
{ "SECOND", kwSECOND },
{ "SEQUENCE", kwSEQUENCE },
{ "SECURITY", kwSECURITY },
{ "SEGMENT", kwSEGMENT },
{ "SELECT", kwSELECT },
{ "SENSITIVE", kwSENSITIVE },
{ "SEQUENCE", kwGENERATOR },
{ "SERVERWIDE", kwSERVERWIDE },
{ "SESSION", kwSESSION },
{ "SET", kwSET },
{ "SHADOW", kwSHADOW },
{ "SHARED", kwSHARED },
{ "SHELL", kwSHELL },
{ "SHOW", kwSHOW },
{ "SIGN", kwSIGN },
{ "SIGNATURE", kwSIGNATURE },
{ "SIMILAR", kwSIMILAR },
{ "SIN", kwSIN },
{ "SINGULAR", kwSINGULAR },
{ "SINH", kwSINH },
{ "SIZE", kwSIZE },
{ "SKIP", kwSKIP },
{ "SMALLINT", kwSMALLINT },
{ "SNAPSHOT", kwSNAPSHOT },
{ "SOME", kwSOME },
{ "SORT", kwSORT },
{ "SOURCE", kwSOURCE },
{ "SPACE", kwSPACE },
{ "SQL", kwSQL },
{ "SQLCODE", kwSQLCODE },
{ "SQLERROR", kwSQLERROR },
{ "SQLSTATE", kwSQLSTATE },
{ "SQLWARNING", kwSQLWARNING },
{ "SQRT", kwSQRT },
{ "STABILITY", kwSTABILITY },
{ "START", kwSTART },
{ "STARTING", kwSTARTING },
{ "STARTS", kwSTARTS },
{ "STATEMENT", kwSTATEMENT },
{ "STATIC", kwSTATIC },
{ "STATISTICS", kwSTATISTICS },
{ "STDDEV_POP", kwSTDDEV_POP },
{ "STDDEV_SAMP", kwSTDDEV_SAMP },
{ "SUB_TYPE", kwSUB_TYPE },
{ "SUBSTRING", kwSUBSTRING },
{ "SUM", kwSUM },
{ "SUSPEND", kwSUSPEND },
{ "SYSTEM", kwSYSTEM },
{ "TABLE", kwTABLE },
{ "TAGS", kwTAGS },
{ "TAN", kwTAN },
{ "TANH", kwTANH },
{ "TEMPORARY", kwTEMPORARY },
{ "TERM", kwTERMINATOR },
{ "TERMINATOR", kwTERMINATOR },
{ "THEN", kwTHEN },
{ "TIES", kwTIES },
{ "TIME", kwTIME },
{ "TIMEOUT", kwTIMEOUT },
{ "TIMESTAMP", kwTIMESTAMP },
{ "TIMEZONE_HOUR", kwTIMEZONE_HOUR },
{ "TIMEZONE_MINUTE", kwTIMEZONE_MINUTE },
{ "TO", kwTO },
{ "TOTALORDER", kwTOTALORDER },
{ "TRAILING", kwTRAILING }, //fb2.0
{ "TRANSACTION", kwTRANSACTION },
{ "TRANSLATE", kwTRANSLATE },
{ "TRANSLATION", kwTRANSLATION },
{ "TRAPS", kwTRAPS },
{ "TRIGGER", kwTRIGGER },
{ "TRIM", kwTRIM }, //fb2.0
{ "TRUE", kwTRUE },
{ "TRUNC", kwTRUNC },
{ "TRUSTED", kwTRUSTED },
{ "TWO_PHASE", kwTWO_PHASE },
{ "TYPE", kwTYPE },
{ "UNBOUNDED", kwUNBOUNDED },
{ "UNCOMMITTED", kwUNCOMMITTED },
{ "UNDO", kwUNDO },
{ "UNION", kwUNION },
{ "UNIQUE", kwUNIQUE },
{ "UNKNOWN", kwUNKNOWN },
{ "UPDATE", kwUPDATE },
{ "UPDATING", kwUPDATING },
{ "UPPER", kwUPPER },
{ "USAGE", kwUSAGE },
{ "USER", kwUSER },
{ "USING", kwUSING },
{ "UUID_TO_CHAR", kwUUID_TO_CHAR },
{ "VALUE", kwVALUE },
{ "VALUES", kwVALUES },
{ "VAR_POP", kwVAR_POP },
{ "VAR_SAMP", kwVAR_SAMP },
{ "VARBINARY", kwVARBINARY },
{ "VARCHAR", kwVARCHAR },
{ "VARIABLE", kwVARIABLE },
{ "VARYING", kwVARYING },
{ "VERSION", kwVERSION },
{ "VIEW", kwVIEW },
{ "WAIT", kwWAIT },
{ "WAIT_TIME", kwWAIT_TIME },
{ "WEEK", kwWEEK },
{ "WEEKDAY", kwWEEKDAY },
{ "WHEN", kwWHEN },
{ "WHENEVER", kwWHENEVER },
{ "WHERE", kwWHERE },
{ "WHILE", kwWHILE },
{ "WINDOW", kwWINDOW },
{ "WITH", kwWITH },
{ "WITHOUT", kwWITHOUT },
{ "WORK", kwWORK },
{ "WRITE", kwWRITE },
{ "YEAR", kwYEAR },
{ "YEARDAY", kwYEARDAY },
{ "ZONE", kwZONE },
