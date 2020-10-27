#pragma once
/*
Example Syncrone call.

  IHTTPRequest* pRequest = m_pInterfce->CreateHTTPRequester();
  
  pRequest->SetHeader("Cookie", "3435465654");
  IHTTPResponse* pResponse = pRequest->SendRequestGET("http://example.com/jason/query?windows");
  if(pResonse)
  {
    if(pResonse->DataLength() > 0)
    {
      MyJSONParser parser(pResonse->Data(), pResonse->DataLength());
      ...
      ...
    }

    //pResonse->Release();
    pRequest->Release(); // Will also delete the pResonse that SendRequstGET returned, if not already relased.
  }

*/

#include "MCNamespace.h"

MCNSBEGIN
class IHTTPResponse;

class __declspec(novtable) IHTTPAsyncCallback
{
public:
  virtual void OnTransferFailed() = 0;
  virtual void OnTransferCompleted(IHTTPResponse* pResponse) = 0;
};
// Receive
class __declspec(novtable) IHTTPReceiveCallback
{
public:
  // return false to Abort data transfer
  virtual bool OnDataReceive(const BYTE* pData, DWORD nDataLen) = 0;
};


class __declspec(novtable) IHTTPSendCallback : public IHTTPReceiveCallback
{
public:
  // When sending data to the server the interface is called to get the data
  // that should be sent to the server. it will continue to ask for data until *pDataWritten is 0.
  // if false is return the data transfer is aborted.
  //
  // pData = pointer to where to copy data
  // nDataLen = Max size of pData
  // *pDataWritten = How many byte actual written to pData
  virtual bool OnGetDataSend(BYTE* pData, DWORD nDataLen, DWORD* pDataWritten) = 0;
};

class __declspec(novtable) IHTTPResponse
{
public:
  virtual DWORD ResponseCode(char* szResponseCodeText, int len) = 0;
  virtual DWORD DataLen() = 0;
  virtual const BYTE* Data() = 0;

  virtual bool CopyData(BYTE* dst, DWORD nDataLen) = 0;

  virtual DWORD ErrorCode() = 0;
  
  virtual void Release() = 0; // will release and delete it
};

/*
 *  Internal receive/send buffer is 1 MB.
 *  If you want to receive or send larger the use the callback.
 **/
class __declspec(novtable) IHTTPRequest
{
public:
  virtual bool SetHeader(const char* szKey, const char* szValue) = 0;
    
  virtual IHTTPResponse* SendRequestGET(const char* szURL) = 0;
  virtual IHTTPResponse* SendRequestGET(const char* szURL, IHTTPReceiveCallback* pCallback) = 0;

  // Not Implemented yet
  virtual IHTTPResponse* SendRequestPOST(const char* szURL, const char* szBody, DWORD bodyLen) = 0;
  virtual IHTTPResponse* SendRequestPOST(const char* szURL, IHTTPSendCallback* pCallback) = 0;
  
  // if result is NULL for SendRequestGET/SendRequestPOST
  virtual DWORD GetResponseCode() = 0;
  virtual DWORD LastError(WCHAR* szErrorMessage = NULL, DWORD nLen = 0) = 0;

  virtual void Release() = 0; // will release and delete it, Will also release IHTTPResponse returned belonging to this interface

  // Not implemented Yet
  virtual bool SendAsyncRequestGET(const char* szURL, IHTTPAsyncCallback* pCallback) = 0;
};

MCNSEND
