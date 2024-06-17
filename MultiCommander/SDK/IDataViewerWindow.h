/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2024 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * the IDataViewerWindow interface is used both by consuming and creating a viewer.
 *
 * Extensions that act as a Viewer App can create a IDataViewerWindow interface and send it to the IDataViewerWindowConnector interface that is sent
 * if the extension gets a AM_VIEWEX command. (WPARAM is a IDataViewerWindowsConnector*)
 * 
 * Any extension can also request to show a viewer window by using CreateDataViewerWindow() and will then recieve a IDataViewerWindow* 
 * This interface is really a proxy to a real instance of a viewer
 * 
 */

#pragma once
#include "MCNamespace.h"

MCNSBEGIN

#define DVW_FEAT_VIEWMEMORY       0x00000010L  // Support for viewing data located in a dynamic memory buffer.  ViewMemory( nMaxMemorySize, viewMode ), then use AddData to add data 
#define DVW_FEAT_VIEWFILE         0x00000020L  // Support for viewing data located in a file. Use ViewFile( <filename> , viewMode );
#define DVW_FEAT_VIEWMODE_ASCII   0x00000100L  // View data as Ascii
#define DVW_FEAT_VIEWMODE_UNICODE 0x00000200L  // View data as Unicode (UTF16)
#define DVW_FEAT_VIEWMODE_UTF8    0x00000400L  // View data as UTF8
#define DVW_FEAT_VIEWMODE_HEX     0x00000800L  // View data as Hex
#define DVW_FEAT_VIEWMODE_BINARY  0x00001000L  // View data as Binary

class IRWFile;
class IFileItem;

class IDataViewerWindow
{
public:

  // Since viewer might be created in another thread. this can be called before everything is setup.
  // Return true when everything is setup and the interface is ready to be used.
  virtual bool  IsReady() = 0;

  // Return what features the current viewer supports.
  virtual DWORD GetFeatureSupport() = 0;

  // get/set options
  virtual DWORD SetOptions(DWORD dwOptions) = 0; // Not implemented yet
  virtual DWORD GetOptions() = 0;                // Not implemented yet

  virtual DWORD Created() = 0;

  // View data from memory. Use AddData to add data to memory buffer that will be shown.
  virtual DWORD ViewMemory(size_t size) = 0;
  // Add data to the view. (Only allowed for the view is opened using ViewMemory)
  virtual DWORD AddData(const char* szText, size_t len) = 0;
  virtual DWORD AddData(const wchar_t* szText, size_t len) = 0;
  virtual DWORD AddData(const BYTE* szData, size_t len) = 0;

  // View a file. (Really? if you want to do this use ViewFile(..) in IFileSystemManager instead )
  virtual DWORD ViewFile(const WCHAR* szFile) = 0;             // Not implemented yet
  virtual DWORD ViewFile(const IFileItem* pFileItem) = 0;      // Not implemented yet
  virtual DWORD ViewFile(const IRWFile* pVolumeFileItem) = 0;  // Not implemented yet
  
  // Set how the data should be viewed. any of the DVW_FEAT_VIEWMODE_ flags can be set as long as it is support in GetFeatureSupport();
  virtual DWORD SetViewMode(DWORD dwViewMode) = 0;

  // Show/Hide window
  virtual DWORD ShowWindow() = 0;
  virtual DWORD HideWindow() = 0;

  // Release the interface. But if the interface was created from IAppInterface->CreateDataViewerWindow(..) with true to AutoDelete. then do NOT call this.
  // Unless something goes wrong during initialization and you need to cleanup.
  virtual void Release() = 0;
};

// Inherit from this if you create a viewer that should be connected to the public viewer interface
class IDataViewerWindowConnector
{
public:
  virtual bool OnViewerConnected(MCNS::IDataViewerWindow* pViewer) = 0;
  virtual bool OnViewerDisconnected(MCNS::IDataViewerWindow* pViewer) = 0;
};

MCNSEND
