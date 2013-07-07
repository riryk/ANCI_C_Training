/******************************************************************************
Module:  SparseStream.h
Notices: Copyright (c) 2007 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "CmnHdr.h"     /* See Appendix A. */
#include <WinIoCtl.h>


///////////////////////////////////////////////////////////////////////////////


#pragma once


///////////////////////////////////////////////////////////////////////////////


class CSparseStream {
public:
   static BOOL DoesFileSystemSupportSparseStreams(PCTSTR pszVolume);
   static BOOL DoesFileContainAnySparseStreams(PCTSTR pszPathname);

public:
   CSparseStream(HANDLE hStream = INVALID_HANDLE_VALUE) { 
      Initialize(hStream); 
   }

   virtual ~CSparseStream() { }

   void Initialize(HANDLE hStream = INVALID_HANDLE_VALUE) { 
      m_hStream = hStream; 
   }

public:
   operator HANDLE() const { return(m_hStream); }

public:
   BOOL IsStreamSparse() const;
   BOOL MakeSparse();
   BOOL DecommitPortionOfStream(
      __int64 qwFileOffsetStart, __int64 qwFileOffsetEnd);

   FILE_ALLOCATED_RANGE_BUFFER* QueryAllocatedRanges(PDWORD pdwNumEntries);
   BOOL FreeAllocatedRanges(FILE_ALLOCATED_RANGE_BUFFER* pfarb);

private:
   HANDLE m_hStream;

private:
   static BOOL AreFlagsSet(DWORD fdwFlagBits, DWORD fFlagsToCheck) {
      return((fdwFlagBits & fFlagsToCheck) == fFlagsToCheck);
   }
};


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::DoesFileSystemSupportSparseStreams(
   PCTSTR pszVolume) {

   DWORD dwFileSystemFlags = 0;
   /* Retrieves information about the file system and volume associated with the specified root directory.
      To specify a handle when retrieving this information, use the GetVolumeInformationByHandleW function.
      To retrieve the current compression state of a file or directory, use FSCTL_GET_COMPRESSION.
    */
   BOOL bOk = GetVolumeInformation(pszVolume, NULL, 0, NULL, NULL, 
      &dwFileSystemFlags, NULL, 0);
   bOk = bOk && AreFlagsSet(dwFileSystemFlags, FILE_SUPPORTS_SPARSE_FILES);
   return(bOk);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::IsStreamSparse() const {

   BY_HANDLE_FILE_INFORMATION bhfi;
   GetFileInformationByHandle(m_hStream, &bhfi);
   return(AreFlagsSet(bhfi.dwFileAttributes, FILE_ATTRIBUTE_SPARSE_FILE));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::MakeSparse() {

   DWORD dw;
   /* Sends a control code directly to a specified device driver,
    * causing the corresponding device to perform the corresponding operation. 
	*
	* dwIoControlCode [in]
    *   The control code for the operation. 
	*   This value identifies the specific operation to be performed 
	*   and the type of device on which to perform it.
    *   For a list of the control codes, see Remarks. 
	*   The documentation for each control code provides 
	*   usage details for the lpInBuffer, nInBufferSize, lpOutBuffer, and nOutBufferSize parameters.
	* 
	* FSCTL_SET_SPARSE
	*   Marks the indicated file as sparse or not sparse. 
	*   In a sparse file, large ranges of zeros may not require disk allocation. 
	*   Space for nonzero data will be allocated as needed as the file is written.
    *   To perform this operation, call the DeviceIoControl function with the following parameters.
	*
	*/
   return(DeviceIoControl(m_hStream, FSCTL_SET_SPARSE, 
      NULL, 0, NULL, 0, &dw, NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::DecommitPortionOfStream(
   __int64 qwOffsetStart, __int64 qwOffsetEnd) {

   // NOTE: This function does not work if this file is memory-mapped.
   DWORD dw;
   FILE_ZERO_DATA_INFORMATION fzdi;
   fzdi.FileOffset.QuadPart = qwOffsetStart;
   fzdi.BeyondFinalZero.QuadPart = qwOffsetEnd + 1;
   /* Fills a specified range of a file with zeros (0). 
    * If the file is sparse or compressed, 
	* the NTFS file system may deallocate disk space in the file. 
	* This sets the range of bytes to zeros (0) without extending the file size. 
    */
   return(DeviceIoControl(m_hStream, FSCTL_SET_ZERO_DATA, (PVOID) &fzdi, 
      sizeof(fzdi), NULL, 0, &dw, NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::DoesFileContainAnySparseStreams(
   PCTSTR pszPathname) {

   DWORD dw = GetFileAttributes(pszPathname);
   return((dw == 0xfffffff) 
      ? FALSE : AreFlagsSet(dw, FILE_ATTRIBUTE_SPARSE_FILE));
}


///////////////////////////////////////////////////////////////////////////////


inline FILE_ALLOCATED_RANGE_BUFFER* CSparseStream::QueryAllocatedRanges(
   PDWORD pdwNumEntries) {

   FILE_ALLOCATED_RANGE_BUFFER farb;
   farb.FileOffset.QuadPart = 0;
   farb.Length.LowPart = 
      GetFileSize(m_hStream, (PDWORD) &farb.Length.HighPart);

   // There is no way to determine the correct memory block size prior to 
   // attempting to collect this data, so I just picked 100 * sizeof(*pfarb)
   DWORD cb = 100 * sizeof(farb);
   FILE_ALLOCATED_RANGE_BUFFER* pfarb = (FILE_ALLOCATED_RANGE_BUFFER*) 
      HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cb);

   /* Scans a file or alternate stream looking for 
    * ranges that may contain nonzero data. 
	* Only compressed or sparse files 
	* can have zeroed ranges known to the operating system. 
	* For other files, the output buffer will contain 
	* only a single entry that contains 
	* the starting point and the length requested. */
   DeviceIoControl(m_hStream, FSCTL_QUERY_ALLOCATED_RANGES,
      &farb, sizeof(farb), pfarb, cb, &cb, NULL);
   *pdwNumEntries = cb / sizeof(*pfarb);
   return(pfarb);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CSparseStream::FreeAllocatedRanges(
   FILE_ALLOCATED_RANGE_BUFFER* pfarb) {

   // Free the queue entry's allocated memory
   return(HeapFree(GetProcessHeap(), 0, pfarb));
}


///////////////////////////////// End Of File /////////////////////////////////
