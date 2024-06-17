/*
 * Multi Commander - SDK
 * 
 * Copyright (C) 2000-2016 All Rights Reserved , http://multicommander.com
 * =======================================================================================
 * 
 * Error codes 
 * 
 * TODO
 * Error codes need to be redesigned
 * use HRESULT kind of codes instead. 
 * use DWORD as error code and use the HIWORD as mask for different categoryes and LOWORD for error code.
 * All WinError codes are 0 - 65000, always lower bits, So use Higher bits for control codes.
 * examples
 * 0x10000000  // Windows Error
 * 0x20000000  // MC Volume errors
 * 
 * 0x10000005 = Windows Error code ACCESS_DENIED (5)
 * 0x20000014 = MC Volume Error code for VERROR_ACCESS_DENIED (20, 0x14)
 * Or 
 * 0x10000005 = Windows Error code ACCESS_DENIED (5)
 * 0x20140000= MC Volume Error code for VERROR_ACCESS_DENIED (20, 0x14)
 * Then they can be combined to hold both WInError and MCerror.
 * 0x30140005
 * 
 * Have one bit for fileopeation type. Copy/Move/Delete/Rename because a error might need to be handled different
 * 
 * Changes
 *
 * ---------------------
 * 
 **/

#include "MCNamespace.h"
MCNSBEGIN

// Error Codes
//
// Error Codes needs to be remapped to match Windows Error codes better. and MC specific code should start with a higher number
//
// TODO - Redesign and move codes around - Use Windows Error Code up to X and Put MC error Code at xxxxx+
// ^^^^ 
// 
#define VERROR_NOERROR             0L // No error
#define VERROR_FILE_NOT_FOUND      2L // File can not be found.
#define VERROR_PATH_NOT_FOUND      3L // Path can not be found.
#define VERROR_ALREADY_EXISTS     10L // Cannot create a file, It already exists.
#define VERROR_ALREADY_EXISTS_AS_FILE     11L // Cannot create Folder, A FILE already exits with the same name
#define VERROR_ALREADY_EXISTS_AS_FOLDER   12L // Can not create file, a FOLDER already exists with the same name

#define VERROR_ACCESS_DENIED      20L // Access to file/folder is denied

#define VERROR_WACCESS_DENIED_PROTECTED 24L // Replaces Error 25,26, File has S/H/RO attribute can cannot be overwritten without removing attribute
#define VERROR_WACCESS_DENIED_RO  25L // Write Access Denied. File/folders is Readonly
#define VERROR_WACCESS_DENIED_VRO 26L // Write Access Denied. volumes is Readonly ( CD/DVD ROM or write protected disk)
#define VERROR_WACCESS_DENIED_SH  27L // Write Access Denied. file/folder is System or Hidden
#define VERROR_CANT_ACCESS_PLACEHOLDER 28L // Cant access file becuse it is only a placeholder

#define VERROR_SHARING_VIOLATION  32L // File is being used by another process

#define VERROR_VOLUME_NOT_ENOUGH_SPACE 39L // There is not enough space on volume
#define VERROR_VOLUME_FULL        40L // Volume is full (use the error code above in most cases instead.)
#define VERROR_VOLUME_BADFORMAT   41L // Volume file is bad. corrupt or invalid format.
#define VERROR_VOLUME_NOTFOUND    42L // Can open volume. Corrupt or bad volume file.

  //#define VERROR_FOLDER_NOT_EMPTY   50L // Can not Delete folder. folder not empty

#define VERROR_CONNECTION_LOST      55  // Connection to volume is lost (eg if connect to FTP or other remote conection is lost)
#define VERROR_OP_TIMEOUT_TRYAGAIN  60  // Operation took long time and timed out, but try again later 
  // Eg ReadFile/WriteFile operation on IRWFile can return this if a read/write takes too long. 
  // This so we can handle abort/skip request from the user. ( good for slow volumes likes FTP)

#define VERROR_SOURCE_TARGET_SAME 109 // Operation Failed, Source and Target is file/folder is the same. ( eg Renaming a file and new and old name are identical )
#define VERROR_OPEN_FAILED        110 // 
#define VERROR_OPEN_FAILED_UNSUPPORTED_FORMAT  111 // Unable to open file because it is located in a unsupported format. 
  // (eg opening a file inside zip archive that is compressed with an unsupported compression mode )
#define VERROR_OPEN_FAILED_BADPASSWORD  112

#define VERROR_PATH_TO_LONG       120 // Path is to Long
#define VERROR_PATH_INVALID_CHAR  121 // Path is Invalid. Invalid characters in Path
#define VERROR_INVALID_PATH_NAME  123 // The filename, directory name, or volume label syntax is incorrect.
#define VERROR_INVALID_PATH_OR_VOLUME 124

#define VERROR_CANT_OPEN_SOURCEVOLUME 150 // Can open volume.. pVolume->Open( path ) failed..
#define VERROR_CANT_OPEN_TARGETVOLUME 151 // Can open volume.. pVolume->Open( path ) failed..
#define VERROR_TARGETVOLUME_ALREADY_EXISTS 152 // Can not create TargetVolume. it already exists

#define VERROR_READERROR            158 // Error when trying to read data

#define VERROR_DELETE_READONLY      160 // Can not delete file. because it is read-only
#define VERROR_DELETE_HIDDEN        161 // Can not delete file. because it is hidden 
#define VERROR_DELETE_SYSTEM        162 // Can not delete file. because it is a system file
#define VERROR_DELETE_DIRNOTEMPTY   163 // Can not delete directory. because it is not empty
#define VERROR_DELETE_ACCESSDENIED  164 // Can not delete file/folder. Access is denied. no permission to delete folders
#define VERROR_FILE_INCOMPLETE      165 // A file write have been aborted.. file X is incomplete

#define VERROR_DELETE_PROTECTED     166 // Can't delete file, it is protected. has S/H or RO attributes

#define VERROR_FILE_CORRUPT         170 // file X is corrupt
#define VERROR_FILE_CORRUPT_CRC     171 // File %s is corrupt due to CRC error!

#define VERROR_MISSING_VOLFILE      172 // Volume file is missing.. like ( RAR . if a rar file is missing. or other sold archive stuff )
#define VERROR_BROWSING_VOLUME      173 // Error browsing volume
  //#define VERROR_FILE_CORRUPT_KEEP    171 // file X is corrupt, Keep it anyway ?

#define VERROR_WRITEERROR_SC1           175 // Write happened during write.. (Special case, no sys buffer, HW do not support that)
#define VERROR_WRITEERROR               176 // Unknown error happen while writing

#define VERROR_CUSTOM                   178 // Custom error.

#define VERROR_NOT_SAME_ROOTPATH        180
#define VERROR_EXISTS_NOT_SAME_ROOTPATH 181 // Exists Function - Not Same Rootpath.. can't check if path exists.. 
#define VERROR_NOT_SAME_ROOTVOLUME      182 // (ERROR_NOT_SAME_DEVICE 17 )

#define VERROR_AUTORENAME_FAILED        183
#define VERROR_COMMAND_FAILED            186
#define VERROR_FAILED_TO_OPEN_CONNECTION 187  // eg FTP error 425
#define VERROR_VOLUME_OPERATION_NOTSUPPORTED 196L  // If a operation on a volume is not supported.
#define VERROR_INVALID_PARAMETERS            197L
#define VERROR_NOTIMPLEMENTED                198L
#define VERROR_ABORT_FILEOPERATION           199L

#define VERROR_CANCELLED_BY_USER        ERROR_CANCELLED // 1223
#define VERROR_READERROR_AUTORETRY      3500L // Some read error happen. But Just autoretry
#define VERROR_ABORTED_PROCESS_IN_SHUTDOWN 3998L // Operation was aborted, Process is in Shutdown mode
#define VERROR_INVALID                  3999L
#define VERROR_UNKNOWN_ERROR            4000L // Error code with 4000 and over is Unknown. 

MCNSEND