/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2024 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * Changes
 * ---------------------
 * 
 */

#pragma once

#include "MCNamespace.h"
#include "MultiApp_Def.h"

// Windows SDK Addition - This are only defined in NEWER Sdk Version
// so if they are not found. redefine them
#ifndef FILE_ATTRIBUTE_NO_SCRUB_DATA        
  #define FILE_ATTRIBUTE_NO_SCRUB_DATA       0x00020000  
// MSDN
// The user data stream not to be read by the background data integrity scanner(AKA scrubber).
// When set on a directory it only provides inheritance.This flag is only supported on Storage Spaces and ReFS volumes.
// It is not included in an ordinary directory listing.
// Windows Server 2008 R2, Windows 7, Windows Server 2008, Windows Vista, Windows Server 2003 and Windows XP : This flag is not supported until Windows 8 and Windows Server 2012.
#endif

#ifndef FILE_ATTRIBUTE_EA        
#define FILE_ATTRIBUTE_EA                    0x00040000  
#endif

#ifndef FILE_ATTRIBUTE_PINNED        
#define FILE_ATTRIBUTE_PINNED                0x00080000  
#endif

#ifndef FILE_ATTRIBUTE_UNPINNED        
#define FILE_ATTRIBUTE_UNPINNED              0x00100000  
#endif

#ifndef FILE_ATTRIBUTE_RECALL_ON_OPEN        
#define FILE_ATTRIBUTE_RECALL_ON_OPEN        0x00040000  // same as FILE_ATTRIBUTE_EA ??
 // MSDN
 // This attribute only appears in directory enumeration classes(FILE_DIRECTORY_INFORMATION, FILE_BOTH_DIR_INFORMATION, etc.).When this attribute is set, 
 // it means that the file or directory has no physical representation on the local system; the item is virtual.Opening the item will be more expensive than normal,
 // e.g.it will cause at least some of it to be fetched from a remote store.
#endif

#ifndef FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS        
#define FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS 0x00400000 
 // MSDN
 // When this attribute is set, it means that the file or directory is not fully present locally.
 // For a file that means that not all of its data is on local storage(e.g.it may be sparse with some data still in remote storage).
 // For a directory it means that some of the directory contents are being virtualized from another location.Reading the file / enumerating the directory will be more expensive than normal, 
 // e.g.it will cause at least some of the file / directory content to be fetched from a remote store.Only kernel - mode callers can set this bit.
#endif


MCNSBEGIN

// Attributes flags for FileSystem item ( Lower 32bit values is same as Windows FILE_ATTRIBUTE_xxx flags )
#define ZFF_READONLY          0x0000000000000001ui64 // Item has ReadOnly Attribute set
#define ZFF_HIDDEN            0x0000000000000002ui64 // Item has Hidden Attribute Set
#define ZFF_SYSTEM            0x0000000000000004ui64 // Item has System attribute Set
#define ZFF_NOTUSED           0x0000000000000008ui64 // Windows do not have any for this anymore
#define ZFF_FOLDER            0x0000000000000010ui64 // Item is a Folder
#define ZFF_ARCHIVE           0x0000000000000020ui64 // Item has Archive Attribute set
                              
#define ZFF_DEVICE            0x0000000000000040ui64 // FILE_ATTRIBUTE_DEVICE
#define ZFF_NORMAL            0x0000000000000080ui64 // FILE_ATTRIBUTE_NORMAL
#define ZFF_TEMPORARY         0x0000000000000100ui64 //
#define ZFF_SPARSE_FILE       0x0000000000000200ui64L //
#define ZFF_REPARSE_POINT     0x0000000000000400ui64 // Junction or Symlink or Mounted Path
#define ZFF_COMPRESSED        0x0000000000000800ui64 // File is compressed
#define ZFF_OFFLINE           0x0000000000001000ui64 // File is offline - Reading it will be very slow since filesystem will go out to backup system and read it
#define ZFF_NOTINDEXED        0x0000000000002000ui64 // FILE_ATTRIBUTE_NOT_CONTENT_INDEXED 
#define ZFF_ENCRYPTED         0x0000000000004000ui64 // File is encrypted
#define ZFF_INTSTREAM         0x0000000000008000ui64 // FILE_ATTRIBUTE_INTEGRITY_STREAM
#define ZFF_VIRTUAL           0x0000000000010000ui64 // File is virtual (Reserved by windows but not used?)

#define ZFF_NOSCRUBDATA       0x0000000000020000ui64 // 
#define ZFF_EA                0x0000000000040000ui64 // 
#define ZFF_PINNED            0x0000000000080000ui64 // OnDemand files (onedrive) - When pinned a file or folder is always available locally.
#define ZFF_UNPINNED          0x0000000000100000ui64 // OnDemand files (onedrive) - When unpinned a file or folder is available only online.
#define ZFF_RECALLONOPEN      0x0000000000040000ui64 // same as ZFF_EA ?   
#define ZFF_NOTUSED1          0x0000000000200000ui64
#define ZFF_RECALONDATAACCESS 0x0000000000400000ui64 // OnDemand Files - If exist only in the cloud. Windows need to get file if data is accessed
#define ZFF_NOTUESED2         0x0000000000800000ui64

#define ZFF_NOTUESED3         0x0000000001000000ui64
#define ZFF_NOTUESED4         0x0000000002000000ui64
#define ZFF_NOTUESED5         0x0000000004000000ui64
#define ZFF_NOTUESED6         0x0000000008000000ui64
#define ZFF_NOTUESED7         0x0000000010000000ui64
#define ZFF_STRICTLY_SEQUENTIAL 0x0000000020000000ui64
#define ZFF_NOTUESED9         0x0000000040000000ui64
#define ZFF_NOTUESED10        0x0000000080000000ui64

/*
https://searchenterprisedesktop.techtarget.com/blog/Windows-Enterprise-Desktop/OneDrive-File-Attributes-Uncovered
1. Online only equals -P +U +R.
2. Locally available equals -P -U -R.
3. Always available equals +P -U  -R.
*/
// MC Extended file attributes Flags ( NOT same as Windows internal - upper 32bit )
#define ZFF_RESERVED_START    0x0000000100000000ui64 // Start of ´MC Extended Attributes (Upper 32bit dword) 
#define ZFF_EXT_NOTUSED1      0x0000000200000000ui64
#define ZFF_EXT_NOTUSED2      0x0000000400000000ui64
#define ZFF_EXT_NOTUSED3      0x0000000800000000ui64
#define ZFF_EXT_NOTUSED4      0x0000001000000000ui64
#define ZFF_EXT_NOTUSED5      0x0000002000000000ui64
#define ZFF_EXT_NOTUSED6      0x0000004000000000ui64
#define ZFF_EXT_NOTUSED7      0x0000008000000000ui64
#define ZFF_EXT_PSW_PROTECTED 0x0000010000000000ui64 // Item is password protected, Need password to open (eg Item inside protected zip)
#define ZFF_EXT_NOTUSED9      0x0000020000000000ui64
#define ZFF_EXT_NOTUSED10     0x0000040000000000ui64 
#define ZFF_EXT_NOTUSED11     0x0000080000000000ui64
#define ZFF_EXT_NOTUSED12     0x0000100000000000ui64
#define ZFF_EXT_NOTUSED13     0x0000200000000000ui64
#define ZFF_CLOUD             0x0000400000000000ui64 // Cloud Drive (onedrive) Item is probably on a cloud synced folder (onedrive) (check ZFF_RECALONDATAACCESS if data is accessable)
#define ZFF_EXT_NOTUSED14     0x0000800000000000ui64
#define ZFF_EXT_NOTUSED15     0x0001000000000000ui64
#define ZFF_PLACEHOLDER       0x0002000000000000ui64 // Placeholder file are fake files with have a size but do not actually take up any room. (New from Win8.1, Used in Win8.1 for Skydrive/OneDirve folder, Removed in Windows 10)
#define ZFF_SHORTCUT          0x0004000000000000ui64 // Item is a .lnk shortcut file
#define ZFF_SHORTCUTFOLDER    0x0008000000000000ui64 // Shortcut file links to a folder.
#define ZFF_SERVER            0x0010000000000000ui64 // Item is a Network Server 
#define ZFF_SHARE_DRIVE       0x0020000000000000ui64 // Item is a Network Share (Drive)
#define ZFF_MCLINK            0x0040000000000000ui64 // Item is a Virtual Item. (Does not exist on disk) But the item contain a path in Extended PropData (0, EXTPROP_LINKTARGET )
#define ZFF_VOLCOMMAND        0x0080000000000000ui64 // Item is a special volume commands. (Special handling)
#define ZFF_HIGHLIGHT         0x0100000000000000ui64 // File Item is to be highlighted. Show as BOLD ? or Highlight color 
#define ZFF_SYMLINK           0x0200000000000000ui64 // File item is a SymLink ( check Reserv0 for IS_REPARSE_TAG_SYMLINK )
#define ZFF_JUNKTION          0x0400000000000000ui64 // File Item is a Junction
#define ZFF_MOUNTPOINT        0x0800000000000000ui64 // File Item is a Junction to a Device (Device Mounted in a folder )
#define ZFF_DISCONNECT        0x1000000000000000ui64 // Item can be Disconnected. Eg Items exists on FTP/CDROM/USB Device (require the volume also support IVF_CONNECT )
#define ZFF_NOFILE            0x2000000000000000ui64 // This file item is not a file. So do not split names with dot into a file extension part (Eg Registry Values)
#define ZFF_CANNOTDELETE      0x4000000000000000ui64 // Item can not be deleted. (Can be a fake item, like FSFTP "Quick connect" 'file')
#define ZFF_RESERVED          0x8000000000000000ui64 // Not used

#define ZFF_WIN_ATTRIBUTES    0x00000000FFFFFFFFui64 // 
#define ZFF_MC_EXTENDED       0xFFFFFFFF00000000ui64 // Multi Commander Attribute. Use "dwAttributes ^= dwAttributes & ZFF_MC_EXTENDED;" to remove all MC attributes.
#define ZFF_INVALID_FILE_ATTRIBUTES  0xFFFFFFFFFFFFFFFFui64

// ZFileitem Update Flags, returned by Set_Filedata
#define ZFU_NAME   0x00001 // Name is updated.. ( Not used , can't update name )
#define ZFU_SIZE   0x00002 // size has changed
#define ZFU_ATTRIB 0x00004 // Attrib has changed
#define ZFU_DATE   0x00008 // Date is changed

// ExtraProp DataType ID  // BYTE   TypeID;
#define ZFXP_STRING      0x01
#define ZFXP_BYTEDATA    0x02
#define ZFXP_VAL32       0x04  // value prop
#define ZFXP_VAL64       0x08  // value prop
#define ZFXP_VOIDPTR     0x10 
#define ZFXP_FILETIME    0x20  // value prop - You can only set 1 Of the value props in the same property
#define ZFXP_DOUBLE      0x40  // value prop is double
#define ZFXP_DISPLAYNAME 0x80  // DisplayName is set

// Extended property for fileitem with attribute ZFF_MCLINK
// GetExtraPropData(0, ZEXTPROP_LINKTARGET)
#define ZEXTPROP_LINKTARGET 42
#define ZEXTPROP_LINKTARGET_TYPE_PATH 1
#define ZEXTPROP_LINKTARGET_TYPE_URL  2

#define ZF_TAG1   0x01
#define ZF_TAG2   0x02
#define ZF_TAG3   0x04
#define ZF_TAG4   0x08
#define ZF_TAG5   0x10
#define ZF_TAG6   0x20
#define ZF_TAG7   0x40
#define ZF_TAG8   0x80

// Extended FileProperty Data 
struct ExtraProp
{
  BYTE   Flag;

  WCHAR  szDisplayName[512];

  /* change to
  union
  {
    void* pPtr;
    const wchar_t* pText;
  } pData;
  */
  const void* pPtr;
  DWORD  nDataLen;  // In GetProp. this is the LEN of pPtr

  union
  {
    INT64  value64;
    double valueD; // Flag must have ZFXP_DOUBLE
  };
};

class __declspec(novtable) IFileItem
{
public:
  virtual        ~IFileItem() = default;

  virtual WCHAR*   Get_Name(_Out_writes_z_(nMaxLen) WCHAR* szName, _In_ DWORD nMaxLen) const = 0;
  virtual WCHAR*   Get_NameOnly(_Out_writes_z_(nMaxLen) WCHAR* szName, _In_ DWORD nMaxLen) const = 0;
  virtual WCHAR*   Get_Path(_Out_writes_z_(nMaxLen) WCHAR* szName, _In_ DWORD nMaxLen) const = 0;
  virtual WCHAR*   Get_PathReal(_Out_writes_z_(nMaxLen) WCHAR* szName, _In_ DWORD nMaxLen) const = 0; // IF Item is virtual the real and not the virtual path is returned

  virtual WCHAR* Get_FullPath(_Out_writes_z_(nMaxLen) WCHAR* szName, _In_ DWORD nMaxLen) const = 0;

  // The the name of the parent item
  virtual WCHAR* Get_ParentName(_Out_writes_z_(nMaxLen) WCHAR* szName, _In_ DWORD nMaxLen) const = 0;

  virtual WCHAR* Get_Comment( WCHAR* strComment ) = 0;
//  virtual WCHAR* Get_Ext( WCHAR* strExt,BOOL bStripDot = FALSE  ) const = 0; // REMOVE Not Safe no length
  virtual WCHAR* Get_Ext(_Out_writes_z_(nMaxLen) WCHAR* strExt, _In_ DWORD nMaxLen, BOOL bStripDot) const = 0;

  [[nodiscard]] virtual const WCHAR* Get_Ext(BOOL bStripDot = FALSE) const = 0;


  [[nodiscard]] virtual UINT64 Get_Size() const = 0;
  [[nodiscard]] virtual UINT64 Get_Attributes() const = 0;
  [[nodiscard]] virtual DWORD  Get_AttributesWindows() const = 0;

  [[nodiscard]] virtual int Get_IconIdx(MCIconSize nIconSize) const = 0;
  [[nodiscard]] virtual int Get_IconNormalSmall() const = 0;
  [[nodiscard]] virtual int Get_IconNormalLarge() const = 0;
  [[nodiscard]] virtual int Get_IconNormalMedium() const = 0;
  [[nodiscard]] virtual int Get_IconNormalXLarge() const = 0;

  [[nodiscard]] virtual  UINT8  ColorID() const = 0;

  virtual void  Get_Date(FILETIME* ft) const = 0;

  virtual void  Get_DateAccess(FILETIME* ft) const = 0;
  virtual void  Get_DateWrite(FILETIME* ft) const = 0;
  virtual void  Get_DateCreate(FILETIME* ft) const = 0;

  [[nodiscard]] virtual bool isFolder() const = 0;
  [[nodiscard]] virtual bool isReadOnly() const = 0;
  [[nodiscard]] virtual bool isHidden() const = 0;
  [[nodiscard]] virtual bool isSystem() const = 0;
  [[nodiscard]] virtual bool isArchive() const = 0;

  [[nodiscard]] virtual bool isRoot() const = 0;
  [[nodiscard]] virtual bool isDotFolder() const = 0;

  [[nodiscard]] virtual bool  IsVirtualDeviceItem() const = 0;

  [[nodiscard]] virtual bool IsProtected() const = 0;

    // Experimental.  Onedrive Cloud sync
  // OnDemand  items 
  [[nodiscard]] virtual bool IsOnlineOnly() const = 0;
  [[nodiscard]] virtual bool IsLocalAvailable() const = 0;
  [[nodiscard]] virtual bool IsAlreadyAvailable() const = 0;
  [[nodiscard]] virtual bool IsOnDemandItem() const = 0;


  virtual DWORD Set_FileData( const WCHAR* strName , const WCHAR* strComment ,  INT64 i64Size , UINT64 dwAttribute , const FILETIME* pftWriteTime, const FILETIME* pftCreateTime, const FILETIME* pftAccessTime ) = 0;
  virtual BOOL  Set_FindData( WIN32_FIND_DATA* pFindData ) = 0;

  [[nodiscard]] virtual  short GetVolumeID() const = 0;

  // IF true then this item is about to be deleted. (The c++ object, not file)
  [[nodiscard]] virtual BOOL  HasFlagDelete() const = 0;

  // if true this item will be tagged as being on a remote volume (eg ftp ) 
  // This item is located on a volume that is SLOW with long latency. 
  virtual void  SetIsRemote(bool b) = 0;
  virtual BOOL  IsRemote() = 0;

  virtual UINT64 GetHashID() = 0; // Need to be changed. 64bit value not big enough for unique data, Can generate conflicting IDs.

  // set to true if browsing this path should be redirected to a NumDevice ( eg 6:\ )
  virtual void SetRedirectToNumDevice(bool b) = 0;

  // Set Extra property info that you want to store inside the FileItem object.
  // Can be used when getting property with a property info extensions. 
  virtual BOOL  SetExtraPropData(WORD nExtID, WORD nPropID, ExtraProp* pProp) = 0;
  virtual BOOL  GetExtraPropData(WORD nExtID, WORD nPropID, ExtraProp* pProp) const = 0;

  // if set to true then extra prop data will be fetched again. even if it is already cached.
  virtual void  SetExtraPropDataInvalid(bool b) = 0;

  virtual IFileItem* GetParentItem() = 0;

  //virtual bool GetLinkTarget(WCHAR* szTarget, DWORD nLen) = 0;

  // bool bRecursive parameter obsolete
  virtual void AddRef(bool bRecursive = false) = 0;
  virtual void DelRef(bool bRecursive = false) = 0;
  virtual DWORD GetRef() = 0;
};


struct FileItem
{
  UINT64 lAttribute;   // ZFF_ Flags

  ULARGE_INTEGER lSize; // unsigned long

  FILETIME  tCreate;  // Can be 0 - on FAT file system this is not known
  FILETIME  tAccess;  // Can be 0 - on FAT file system this is not known
  FILETIME  tWrite;   //

  WCHAR   wName[1024];  //We have a virtual filesystem here and we must allow bigger name parts then 256
  int     iIconSmall;
  int     iIconLarge;
  int     iIconMedium;
  int     iIconXLarge;
  DWORD_PTR dwData;   // Custom Data that the plugin can put there..  this data is returned if used select item
                      // ***** WARRNING!!! *********
                      // Since you can have multiple instances of a modules/plugin and you can not be sure that an item you get is from 
                      // the same instance that created the item. so be careful of what data you put here
};

MCNSEND
