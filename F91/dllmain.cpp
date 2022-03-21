// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#ifdef __cplusplus
extern "C"{
#endif
int (*f91memaccess)(int, int, int);
void (*f91spiaccess)(bool);
void (*cpu_int)(int);
char* fname4if;
UINT8 ramaubr = 0xFF;
UINT8 ramcr = 0xC0;
char intram4000h[0x4000];
char mbist[2];
UINT16 spispeed=0x0000;
UINT8 spicr = 0x04;
UINT8 buf4rttmpx2 = 0;
UINT8 buf4rttmpx = 0;
UINT8 buf4rttmp = 0;
UINT8 buf4rt[256];
UINT8 buf4acc = 0;

__declspec(dllexport) void f91spimisocall(bool prm_0) { 
    buf4rttmpx &= 0x7; if (buf4rttmpx == 0) { buf4rttmp = 0; } buf4rttmp |= prm_0 << (buf4rttmpx++); if (buf4rttmpx == 7) { buf4rt[buf4acc++] = buf4rttmp; buf4rttmpx = 0; }
}

#if 0
switch (prm_1) {
case 0:
    buf4rttmpx &= 0x7; if (buf4rttmpx == 0) { buf4rttmp = 0; } buf4rttmp |= prm_0 << (buf4rttmpx++); if (buf4rttmpx == 7) { buf4rt[buf4acc++] = buf4rttmp; buf4rttmpx = 0; }
    break;
case 1:
    buf4rttmpx2 &= 0x7;
    bool ret = (buf4rt[0] >> (7 ^ (buf4rttmpx2++))) ? true : false;
    if (buf4rttmpx2 == 7) {
        buf4rttmpx2 = 0;
        buf4rt[0] = 0;
        if ((buf4acc > 0)) {
            for (int i = 0; i < 255; i++) {
                buf4rt[i] = buf4rt[i + 1];
            }
            buf4rt[255] = 0;
            buf4acc--;
        }
    }
    return ret;
    break;
}
return false;
#endif

__declspec(dllexport) void f91macfuncset(int (*tmpfunc)(int, int, int), void (*tmpfunc2)(bool), void (*tmpfunc3)(int)) { f91memaccess = tmpfunc; f91spiaccess = tmpfunc2; cpu_int = tmpfunc3; }
__declspec(dllexport) void f91internalflashpathset(char *tmpfname) { fname4if = tmpfname; }

__declspec(dllexport) int mac4ez80dll(int prm_0, int prm_1, int prm_2) {
    switch (prm_2) {
    case 0:
    case 1:
        if ((ramaubr = ((prm_0 >> 16) & 0xFF)) && ((prm_0 & 0xFFFF) >= ((ramcr&0xC0)<<8))) {
            switch (prm_2) { 
            case 0:
                intram4000h[prm_0 & 0x3FFF] = (prm_1 & 0xFF);
                return 0;
            case 1:
                return intram4000h[prm_0 & 0x3FFF];
            }
        } else { 
            return f91memaccess(prm_0, prm_1, prm_2);
        }
        break;
    case 2:
        switch (prm_0) {
        case 0xb4:
            ramcr = prm_1&0xC0;
            break;
        case 0xb5:
            ramaubr = prm_1;
            break;
        case 0xb6:
            mbist[0] = (prm_1 & 0x80) | (mbist[0] & 0x7F);
            break;
        case 0xb7:
            mbist[1] = (prm_1 & 0x80) | (mbist[1] & 0x7F);
            break;
        case 0xb8:
            spispeed = (spispeed & 0xFF00) | ((prm_1 & 0xFF) << (8 * 0));
            break;
        case 0xb9:
            spispeed = (spispeed & 0x00FF) | ((prm_1 & 0xFF) << (8 * 1));
            break;
        case 0xba:
            spicr = prm_1 & 0xBC;
            break;

        case 0xbc:
            for (int i = 0; i < 7; i++) {
                f91spiaccess(((prm_1 >> i)&1) ? true : false);
            }
            break;
        }
        break;
    case 3:
        switch (prm_0) {
        case 0xb4:
            return ramcr;
            break;
        case 0xb5:
            return ramaubr;
            break;
        case 0xb6:
            return mbist[0];
            break;
        case 0xb7:
            return mbist[1];
            break;
        case 0xb8:
            return (spispeed >> (8 * 0)) & 0xFF;
            break;
        case 0xb9:
            return (spispeed >> (8 * 1)) & 0xFF;
            break;
        case 0xba:
            return spicr;
            break;

        case 0xbc:
            int ret = buf4rt[0];
            buf4rt[0] = 0;
            if ((buf4acc > 0)) {
                for (int i = 0; i < 255; i++) {
                    buf4rt[i] = buf4rt[i + 1];
                }
                buf4rt[255] = 0;
                buf4acc--;
            }
            return ret;
            break;
        }
        break;
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
