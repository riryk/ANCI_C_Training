

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
#include <strsafe.h>
/* for SID management */
#include <Sddl.h>    

/* Main dialog */
HWND     g_hDlg;

/* Mutex, boundary and namespace used to detect previous running instance */
HANDLE   g_hSingleton = NULL;
HANDLE   g_hBoundary = NULL;
HANDLE   g_hNamespace = NULL;

/* Keep track whether or not the namespace was created or open for clean-up */
BOOL     g_bNamespaceOpened = FALSE;

/* Names of boundary and private namespace */
PCTSTR   g_szBoundary = TEXT("3-Boundary");
PCTSTR   g_szNamespace = TEXT("3-Namespace");

#define DETAILS_CTRL GetDlgItem(g_hDlg, IDC_EDIT_DETAILS)

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
   /* Save char print */
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
   BOOL ConvertStringResult;
   DWORD dwLastError;
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
         &cbSID,
		 NULL);
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
   ConvertStringResult = 
	  ConvertStringSecurityDescriptorToSecurityDescriptor(
         TEXT("D:(A;;GA;;;BA)"),
         SDDL_REVISION_1,
         &sa.lpSecurityDescriptor,
	     NULL);
    /* If we cannot convert string sequrity descriptor,
	 * we show a message on the interface. */ 
   if (ConvertStringResult == FALSE) 
   {
      AddText(
		  TEXT("Security Descriptor creation failed: %u\r\n"), 
		  GetLastError());
      return;
   }
   /* Creates a private namespace. 
    *
    * lpPrivateNamespaceAttributes [in, optional] -
	*   A pointer to a SECURITY_ATTRIBUTES structure 
    *   that specifies the security attributes of the namespace object.
	* 
 	* lpBoundaryDescriptor [in] -
    *   A descriptor that defines how the namespace is to be isolated. 
	*   The caller must be within this boundary. 
	*   The CreateBoundaryDescriptor function creates a boundary descriptor.
	* 
	* lpAliasPrefix [in] -
    *   The prefix for the namespace. To create an object in this namespace,
	*   specify the object name as prefix\objectname.
    *   The system supports multiple private namespaces with the same name, 
	*   as long as they define different boundaries.
	* 
	* Return value
    *   If the function succeeds, it returns a handle to the new namespace.
    *   If the function fails, the return value is NULL. 
	*   To get extended error information, call GetLastError. 
    */
   g_hNamespace = 
	  CreatePrivateNamespace(
	    &sa, 
	    g_hBoundary, 
	    g_szNamespace);
   /* The memory for sa.lpSecurityDescriptor has been allocated by Convert...
    * windows function. It up to us to free the memory.
    * Don't forget to release memory for the security descriptor.
    */
   LocalFree(sa.lpSecurityDescriptor);
   /* Check the private namespace creation result */
   dwLastError = GetLastError();
   /* If the function has returned null. That means that some error
    * has happened. In this case we need to investigate a GetLastError()
    */
   if (g_hNamespace == NULL) 
   {
      /* Nothing to do if access is denied
       * --> this code must run under a Local Administrator account
	   */
      if (dwLastError == ERROR_ACCESS_DENIED) 
	  {
         AddText(TEXT("Access denied when creating the namespace.\r\n"));
         AddText(TEXT("   You must be running as Administrator.\r\n\r\n"));
         return;
	  }
	  else 
	  { 
         /* If another instance has already created the namespace, 
            we need to open it instead.  */
         if (dwLastError == ERROR_ALREADY_EXISTS) 
		 {
            AddText(TEXT("CreatePrivateNamespace failed: %u\r\n"), dwLastError);
			/* Try to open existed private namespace 
			 * pBoundaryDescriptor [in] - A descriptor that defines how 
			 * the namespace is to be isolated. The CreateBoundaryDescriptor function
			 * creates a boundary descriptor.
			 */
            g_hNamespace = OpenPrivateNamespace(g_hBoundary, g_szNamespace);
			/* If the function returns null, that means that some error has happened */
			if (g_hNamespace == NULL) 
			{
               AddText(
				   TEXT("   and OpenPrivateNamespace failed: %u\r\n"), 
                   dwLastError);
               return;
            } 
			else 
			{
               g_bNamespaceOpened = TRUE;
               AddText(TEXT("   but OpenPrivateNamespace succeeded\r\n\r\n"));
            }
		 }
	  }
   }

   /* Try to create the mutex object with a name 
    * based on the private namespace 
	*/
   TCHAR szMutexName[64];
   StringCchPrintf(
	   szMutexName, 
	   _countof(szMutexName), 
	   TEXT("%s\\%s"), 
       g_szNamespace, 
	   TEXT("Singleton"));
   /* Try to create mutex */
   g_hSingleton = CreateMutex(NULL, FALSE, szMutexName);
   if (GetLastError() == ERROR_ALREADY_EXISTS) 
   {
      /* There is already an instance of this Singleton object */
      AddText(TEXT("Another instance of Singleton is running:\r\n"));
      AddText(TEXT("--> Impossible to access application features.\r\n"));
   } 
   else  
   {
      /* First time the Singleton object is created */
      AddText(TEXT("First instance of Singleton:\r\n"));
      AddText(TEXT("--> Access application features now.\r\n"));
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

   /* Don't forget to clean up and release kernel resources */
   if (g_hSingleton != NULL) 
   {
       CloseHandle(g_hSingleton);
   }

   if (g_hNamespace != NULL) 
   {
	   /* Open namespace */
       if (g_bNamespaceOpened) 
	   {  
		   /* Open namespace */
           ClosePrivateNamespace(g_hNamespace, 0);
       } 
	   else 
	   {  
		   /* Created namespace */
           ClosePrivateNamespace(g_hNamespace, PRIVATE_NAMESPACE_FLAG_DESTROY);
       }
   }

   if (g_hBoundary != NULL) 
   {
       DeleteBoundaryDescriptor(g_hBoundary);
   }

   return(0);
}