// FJCA_FUN_GT_DLL.h : FJCA_FUN_GT_DLL DLL ����ͷ�ļ�
//

#pragma once



bool __stdcall FJCA_XVerifySign(char * signdata, char * signcert);

bool __stdcall FJCA_VerifySign(const char *orgdata,const char *signdata,const char *signcert);

bool __stdcall FJCA_VerifyFileSign(const char *filename,const char *signdata,const char *signcert);

bool __stdcall FJCA_IsUsbKeyConnected();

bool __stdcall FJCA_OpenKey();

bool __stdcall FJCA_OpenKey2();

bool __stdcall FJCA_OpenKeyWithPin(const char *pPin);

void __stdcall FJCA_ClearPin();

void __stdcall FJCA_CloseKey();

bool __stdcall FJCA_ExportUserCert(int certid,char *certbuff,int certbufflen);

bool __stdcall FJCA_GetKeySerial(char *certdata,char *keyserialbuff,int keyserialbufflen);

bool __stdcall FJCA_GetKeyPubKey(char *certdata,char *keypubkeybuff,int keypubkeybufflen);

bool __stdcall FJCA_GetKeyCN(char *certdata,char *buff,int bufflen);

bool __stdcall FJCA_GetCertOID(char *certdata,char *buff,int bufflen);

bool __stdcall FJCA_GetCertUserID(char *certdata,char *buff,int bufflen);

bool __stdcall FJCA_GetCertUserOrgCode(char *certdata,char *buff,int bufflen);

bool __stdcall FJCA_GetCertNotAfter(char *certdata,char *buff,int bufflen);

bool __stdcall FJCA_GetIssuerDN( char *certdata,char *buff,int bufflen );

bool __stdcall FJCA_GetKeyDevID(char *keyidbuff,int keyidbufflen);

bool __stdcall  FJCA_SignData(char* orgtxt,char *signedBuff,int signedbufflen);

bool __stdcall FJCA_SignFile(char *filename,char *signedBuff,int signedbufflen);

bool __stdcall FJCA_EncryptByPubkey(char *cert,char * orgData,char *outencryptdata,int outencryptdatalen);

bool __stdcall FJCA_DecryptDataByPrivateKey(char * encdata,char *outorgdata,int outorgdatalen);

bool __stdcall FJCA_WriteStampImgToKey(char *imgdata,int ingdatalen);

bool __stdcall FJCA_WriteStampFileToKey(char *filePath);

int __stdcall FJCA_GetCurStampImgLenFromKey();

bool __stdcall FJCA_ReadStampImgFromKey(char *imgdata,int ingdatalen);

bool __stdcall FJCA_ReadStampFileFromKey(char *filePath);

void __stdcall FJCA_SetServerIPPort(char *ip,int port);

void __stdcall FJCA_SetServerIPPortBak( char *ip,int port );

//���������
void __stdcall FJCA_GenerateKey(char *key,int *keyLen);

//�ԳƼ���
bool __stdcall FJCA_EncryptFileWithKey(char *key,char *inFile,char *outFile);

//�Գƽ���
bool __stdcall FJCA_DecryptFileWithKey(char *key,char *inFile,char *outFile);

//��ѭ���Ĺ�Կ�������
bool __stdcall FJCA_EncryptDCKeyWithCert(char *certs,char *inData,int inLen,char *outData,int *outLen);

//��Ӧ��Կ������ܵĽ���
bool __stdcall FJCA_DecryptDCKeyWithUSBKEY(char *inData,int inLen,char *outData,int *outLen);
