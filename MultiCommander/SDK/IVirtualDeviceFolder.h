#pragma once

#include "MCNamespace.h"
#include "IVolumeContextMenu.h"

MCNSBEGIN

class __declspec(novtable) ICommandHandler
{
public:
  virtual void  SetFirstCommandId(DWORD nID) = 0; // The system will call this with the First availvible commandId to be used
  virtual DWORD GetLastCommandId() = 0;

  virtual void AddCommands(MCNS::IFileItemCollection* pItems, MCNS::IVolumeContextMenu* pMenu, bool bContextMenu, DWORD nIDStart) = 0;
  virtual void OnCommand(DWORD nCommand, MCNS::IFileItemCollection* pItems) = 0;
  virtual void Release() = 0; // Delete your self
};

class __declspec(novtable) IVirtualDeviceFolder
{
public:
  virtual DWORD Count() = 0;
  virtual bool AddFileItem(MCNS::IFileItem* pFileItem) = 0;
  virtual bool RemoveFileItem(const MCNS::IFileItem* pFileItem) = 0;
  virtual bool IsChanged() = 0;
  virtual IFileItem* GetSubItem(int idx) = 0;

  virtual bool IsBusy() const = 0;

  virtual bool PrepareVirtualDevice() = 0;

  // some multi app operation is finished.
  virtual bool OnBegin() = 0;
  virtual bool OnFinished(bool bUserAbort) = 0;
  virtual bool IsAbortRequested() = 0;

  virtual ICommandHandler* GetCommandHandler() = 0;
  virtual void SetCommandHandler(ICommandHandler* pCH) = 0;

  // The content added was depended on this parameter (used for search content so viewer can find out what search content was used)
  virtual void SetContentParameter(const WCHAR* szContentParam) = 0;
  virtual const wchar_t* GetContentParameter() = 0;
};

MCNSEND