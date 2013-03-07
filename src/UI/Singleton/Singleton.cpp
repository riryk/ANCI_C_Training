

#pragma once  

#include <Windowsx.h>
#include <tchar.h>
#include "Resource.h"

#pragma warning(push, 3)
#include <Windows.h>
#pragma warning(pop) 
#pragma warning(push, 4)
#include <CommCtrl.h>
/* For _beginthreadex */
#include <process.h>  


/* Main dialog */
HWND     g_hDlg;
HANDLE   g_hBoundary = NULL;

/* Names of boundary and private namespace */
PCTSTR   g_szBoundary = TEXT("3-Boundary");
PCTSTR   g_szNamespace = TEXT("3-Namespace");

#define chHANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg,     \
      HANDLE_##message((hWnd), (wParam), (lParam), (fn))))

inline void chSETDLGICONS(HWND hWnd, int idi) 
{
   SendMessage(hWnd, WM_SETICON, ICON_BIG,  (LPARAM) 
      LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
         MAKEINTRESOURCE(idi)));
   SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) 
      LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
      MAKEINTRESOURCE(idi)));
}

/* Adds a string to the "Details" edit control */
void AddText(PCTSTR pszFormat, ...) 
{
   va_list argList;
   va_start(argList, pszFormat);

   TCHAR sz[20 * 1024];

   Edit_GetText(DETAILS_CTRL, sz, _countof(sz));
   _vstprintf_s
   (
      _tcschr(sz, TEXT('\0')), 
	  _countof(sz) - _tcslen(sz), 
      pszFormat, 
	  argList
   );
   Edit_SetText(DETAILS_CTRL, sz);

   va_end(argList);
}

void CheckInstances() 
{
   /* Create a SID corresponding to the Local Administrator group */
   BYTE localAdminSID[SECURITY_MAX_SID_SIZE];
   PSID pLocalAdminSID = &localAdminSID;
   DWORD cbSID = sizeof(localAdminSID); 
   BOOL CreateWellKnownSidResult;
   BOOL AddSIDToBoundaryResult;
   /* Create the boundary descriptor 
    * A new boundary descriptor must have at least one security identifier (SID). 
	* To add a SID to a boundary descriptor, use the AddSIDToBoundaryDescriptor function.
    */
   g_hBoundary = CreateBoundaryDescriptor(g_szBoundary, 0);
   /* The CreateWellKnownSid function creates a SID for predefined aliases. 
    * The first parameter WellKnownSidType is WELL_KNOWN_SID_TYPE enumeration.
	* The WELL_KNOWN_SID_TYPE enumeration is a list of commonly used security identifiers (SIDs).
	* Programs can pass these values to the CreateWellKnownSid function 
	* to create a SID from this list.
	* Examples of possible values:
	* WinBuiltinDomainSid                          = 25,
    * WinBuiltinAdministratorsSid                  = 26,
    * WinBuiltinUsersSid                           = 27,
    * WinBuiltinGuestsSid                          = 28,
    * WinBuiltinPowerUsersSid                      = 29
	* We choose only administrators SID
	* DomainSid [in, optional] - A pointer to a SID 
	* that identifies the domain to use when creating the SID. 
	* Pass NULL to use the local computer.
	* cbSid [in, out]
    * A pointer to a DWORD that contains the number of bytes available at pSid. 
	* The CreateWellKnownSid function stores the number of bytes actually used at this location.
    */
   CreateWellKnownSidResult = 
	   CreateWellKnownSid(
	     WinBuiltinAdministratorsSid,
         NULL,
         &cbSID);
   /* If we cannot create SID, we show a message on the interface. */ 
   if (CreateWellKnownSidResult == FALSE) 
   {
	    AddText(
			TEXT("AddSIDToBoundaryDescriptor failed: %u\r\n"), 
            GetLastError());
        return;
   }
   /* Associate the Local Admin SID to the boundary descriptor
    * --> only applications running under an administrator user
    *     will be able to access the kernel objects in the same namespace
	* The following function 
	* adds a security identifier (SID) to the specified boundary descriptor.
	* The first parameter HANDLE *BoundaryDescriptor is a handle to the boundary descriptor. 
	* RequiredSid [in] a pointer to a SID structure.
	*/
   AddSIDToBoundaryResult = 
	   AddSIDToBoundaryDescriptor(
	     &g_hBoundary, 
		 pLocalAdminSID);
    /* If we cannot add the SID to the boundary descriptor,
	 * we show a message on the interface. */ 
   if (CreateWellKnownSidResult == FALSE) 
   {
        AddText(
			TEXT("AddSIDToBoundaryDescriptor failed: %u\r\n"), 
            GetLastError());
        return;
   }
   /* Create the namespace for Local Administrators only */
   SECURITY_ATTRIBUTES sa;
   sa.nLength = sizeof(sa);
   sa.bInheritHandle = FALSE;
   /* The ConvertStringSecurityDescriptorToSecurityDescriptor function converts 
    * a string-format security descriptor into a valid, functional security descriptor.
	* This function retrieves a security descriptor that the 
	* ConvertSecurityDescriptorToStringSecurityDescriptor function converted to string format. 
	* Parameters:
	* LPCTSTR StringSecurityDescriptor - A pointer to a null-terminated string containing 
	*                                    the string-format security descriptor to convert.
	*
	* The Security Descriptor String Format is a text format for storing 
	* or transporting information in a security descriptor. The format is a null-terminated string 
	* with tokens to indicate each of the four main components of a security descriptor: 
	* owner (O:), primary group (G:), DACL (D:), and SACL (S:).
	*
	* We use this pattern string
	* D:dacl_flags(string_ace1)(string_ace2)... (string_acen)
	* dacl_flags - Security descriptor control flags that apply to the DACL. 
	* The dacl_flags string can be a concatenation of zero or more of the following strings.
	*
	* DWORD StringSDRevision - Specifies the revision level of the StringSecurityDescriptor string.
	*                          Currently this value must be SDDL_REVISION_1.
    * SecurityDescriptor[out] - A pointer to a variable that receives a pointer to the converted security descriptor. 
	* 
	*/
   if (!ConvertStringSecurityDescriptorToSecurityDescriptor(
      TEXT("D:(A;;GA;;;BA)"), 
      SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL)) {
      AddText(TEXT("Security Descriptor creation failed: %u\r\n"), GetLastError());
      return;
   }
}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
   chSETDLGICONS(hwnd, IDI_SINGLETON);
   /* Keep track of the main dialog window handle */
   g_hDlg = hwnd;
   /* Check whether another instance is already running */
   CheckInstances();
   return(TRUE);
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) 
{
   switch (id) 
   {
      case IDOK:
      case IDCANCEL:
         /* User has clicked on the Exit button
          * or dismissed the dialog with ESCAPE
		  */
         EndDialog(hwnd, id);
         break;
   }
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) 
   {
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
   }

   return(FALSE);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(lpCmdLine);

   /* Show main window */
   DialogBox(hInstance, MAKEINTRESOURCE(IDD_SINGLETON), NULL, Dlg_Proc);

   ///* Don't forget to clean up and release kernel resources */
   //if (g_hSingleton != NULL) 
   //{
   //    CloseHandle(g_hSingleton);
   //}

   //if (g_hNamespace != NULL) 
   //{
	  // /* Open namespace */
   //    if (g_bNamespaceOpened) 
	  // {  
		 // /* Open namespace */
   //       ClosePrivateNamespace(g_hNamespace, 0);
   //    } 
	  // else 
	  // {  
		 // /* Created namespace */
   //       ClosePrivateNamespace(g_hNamespace, PRIVATE_NAMESPACE_FLAG_DESTROY);
   //    }
   //}

   //if (g_hBoundary != NULL) 
   //{
   //   DeleteBoundaryDescriptor(g_hBoundary);
   //}

   return(0);
}