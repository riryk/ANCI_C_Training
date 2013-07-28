#include <stdio.h>
#include <Windows.h>

int main(int argc, char* argv[]) 
{
    /*
     * Load library in which we'll be hooking our functions.
     */
    HMODULE dll = LoadLibrary(L"C:\\drivers\\dllinject.dll");
    if(dll == NULL) {
        printf("The DLL could not be found.\n");
        getchar();
        return -1;
    }
 
    /*
     * Get the address of the function inside the DLL.
     */
    HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "meconnect");
    if(addr == NULL) {
        printf("The function was not found.\n");
        getchar();
        return -1;
    }
 
    /*
     * Hook the function.
     */
    HHOOK handle = SetWindowsHookEx(WH_KEYBOARD, addr, dll, 0);
    if(handle == NULL) {
        printf("The KEYBOARD could not be hooked.\n");
    }
 
    /*
     * Unhook the function.
     */
    printf("Program successfully hooked.\nPress enter to unhook the function and stop the program.\n");
    getchar();
    UnhookWindowsHookEx(handle);
 
    return 0;
}