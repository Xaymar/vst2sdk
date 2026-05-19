#include <windows.h>

#include "..\include\vstbase.h"


extern vstbase* createPlugin(vst_host_callback_t callback);

extern "C"
{
    static HINSTANCE hInstance = nullptr;

    BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
    {
        if (reason == DLL_PROCESS_ATTACH)
            hInstance = hInst;

        return true;
    }

    __declspec(dllexport) VST_ENTRYPOINT
    {
        // old version
        if (callback(nullptr, VST_HOST_OPCODE_01 /* version */, 0, 0, nullptr, 0.0f) == 0)
            return nullptr;

        vstbase* vstplugin = createPlugin(callback);
        if (!vstplugin)
            return nullptr;

        return vstplugin->getEffect();
    }
}


