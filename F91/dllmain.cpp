// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"
#include <stdio.h>
#pragma warning(disable : 4996)

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
void (*cpu_reset)(void);
int (*cpu_execute)(void);
int externaltime = 0;
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
UINT8 spisr = 0;
UINT8 flashaubr = 0;
UINT8 flashcr = 0x88;
UINT8 flashfdr = 1;
UINT8 flashptr[3];
UINT8 flashwepr = 0xFF;
UINT8 flashicr = 0;
UINT8 flashpcr = 0;
UINT8 intpr[6];
UINT8 chipselect[4][3] = { {0x00,0xff,0xe8},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00} };
int pointer4accflash = 0;
FILE* flashfdcrpt = 0;
int ret = 0;

__declspec(dllexport) void f91spimisocall(bool prm_0) { 
	buf4rttmpx &= 0x7; if (buf4rttmpx == 0) { buf4rttmp = 0; spisr &= 0x7F; } buf4rttmp |= prm_0 << (buf4rttmpx++); if (buf4rttmpx == 7) { spisr |= 0x80; buf4rt[buf4acc++] = buf4rttmp; buf4rttmpx = 0; if (spicr & 0x80) { cpu_int(0x7c); } }
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

__declspec(dllexport) void f91macfuncset(int (*tmpfunc)(int, int, int), void (*tmpfunc2)(bool), void (*tmpfunc3)(int), void (*tmpfunc4)(void), int (*tmpfunc5)(void)) { f91memaccess = tmpfunc; f91spiaccess = tmpfunc2; cpu_int = tmpfunc3; cpu_reset = tmpfunc4; cpu_execute = tmpfunc5; }
__declspec(dllexport) void f91internalflashpathset(char *tmpfname) { fname4if = tmpfname; }

__declspec(dllexport) int f91_execute(void) { externaltime = 0; return cpu_execute() + externaltime; }

__declspec(dllexport) void f91_reset(void) { 
	
	ramaubr = 0xFF;
	ramcr = 0xC0;
	for(int i=0;i<0x4000;i++){ intram4000h[i] = 0; }
	mbist[0] = 0;
	mbist[1] = 0;
	spispeed = 0x0000;
	spicr = 0x04;
	buf4rttmpx2 = 0;
	buf4rttmpx = 0;
	buf4rttmp = 0;
	for (int i = 0; i < 256; i++) { buf4rt[i] = 0; }
	buf4acc = 0;
	spisr = 0;
	flashaubr = 0;
	flashcr = 0x88;
	flashfdr = 1;
	flashptr[0] = 0;
	flashptr[1] = 0;
	flashptr[2] = 0;
	flashwepr = 0xFF;
	flashicr = 0;
	flashpcr = 0;
	intpr[0] = 0;
	intpr[1] = 0;
	intpr[2] = 0;
	intpr[3] = 0;
	intpr[4] = 0;
	intpr[5] = 0;
	chipselect[0][0] = 0x00,0xff,0xe8;
	chipselect[1][0] = 0x00,0x00,0x00;
	chipselect[2][0] = 0x00,0x00,0x00;
	chipselect[3][0] = 0x00,0x00,0x00;
	cpu_reset();
}

UINT8 chipselectinfo = 0;

__declspec(dllexport) int mac4ez80dll(int prm_0, int prm_1, int prm_2) {
	chipselectinfo = 0;
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
        } else if ((flashaubr <= ((prm_0 >> 16) & 0xFF)) && (((flashaubr+3)&0xFF) >= ((prm_0 >> 16) & 0xFF)) && ((flashcr & 0x08) != 0)) {
			externaltime += (flashcr >> 5);
			if ((&fname4if) == NULL) { return 0; }
			flashfdcrpt = fopen(fname4if,"rb+");
			if (&flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
			fseek(flashfdcrpt,(prm_0 - (flashaubr << 16)),SEEK_SET);
			ret = 0;
			switch (prm_2) {
			case 0:
				if (((flashwepr >> ((prm_0 - (flashaubr << 16)) >> 15))&1) == 0) { fputc((prm_1 & 0xFF), flashfdcrpt); }
			case 1:
				ret = fgetc(flashfdcrpt);
			}
			fclose(flashfdcrpt);
			return ret;
		}
		else {
			for (int cnt = 0; cnt < 4; cnt++) { if (((chipselect[cnt][2] & 16) == 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 16) & 0xFF) >= chipselect[cnt][0]) && (((prm_0 >> 16) & 0xFF) <= chipselect[cnt][1])) { chipselectinfo = (cnt | 0x80); break; } }
			return f91memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24));
        }
        break;
    case 2:
		if (prm_0 >= 0x100) { for (int cnt = 0; cnt < 4; cnt++) { if (((chipselect[cnt][2] & 16) != 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 8) & 0xFF) == chipselect[cnt][0])) { chipselectinfo = (cnt | 0x80); break; } } return f91memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24)); }
		switch (prm_0) {
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
			intpr[prm_0 - 0x10] = prm_1;
			break;

		case 0xa8:
		case 0xa9:
		case 0xaa:
		case 0xab:
		case 0xac:
		case 0xad:
		case 0xae:
		case 0xaf:
		case 0xb0:
		case 0xb1:
		case 0xb2:
		case 0xb3:
			chipselect[(prm_0 - 0xa8) / 3][(prm_0 - 0xa8) % 3] = (UINT8(prm_1&0xFF));
			break;

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
			if (&f91spiaccess == NULL) { return 0; }
            for (int i = 0; i < 8; i++) {
                f91spiaccess(((prm_1 >> i)&1) ? true : false);
            }
            break;

		case 0xf5:
			break;
		case 0xf6:
			externaltime += flashfdr;
			if (&fname4if == NULL) { return 0; }
			flashfdcrpt = fopen(fname4if, "rb+");
			if (&flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
			if (&flashfdcrpt == 0) { return 0; }
			pointer4accflash = (((flashptr[0] & 0x7F) << 11) | ((flashptr[1] & ((flashptr[0] & 0x80) ? 0x01 : 0x07)) << 8) | ((flashptr[2] & 0xFF) << 0));
			fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
			flashptr[0] = (flashptr[0] & 0x80) | ((pointer4accflash >> 11) & 0x7F); flashptr[1] = (flashptr[1] & ((flashptr[0] & 0x80) ? 0xFE : 0xF8)) | ((pointer4accflash >> 8) & ((flashptr[0] & 0x80) ? 0x01 : 0x07)); flashptr[2] = ((pointer4accflash >> 0) & 0xFF);
			if (((flashwepr >> ((pointer4accflash) >> 15)) & 1) == 0) { fputc((prm_1 & 0xFF), flashfdcrpt); if (flashicr & 0x80) { cpu_int(0x50); } } else { if (flashicr & 0x40) { cpu_int(0x50); } }
			fclose(flashfdcrpt);
			break;
		case 0xf7:
			flashaubr = prm_1 & 0xFC;
			break;
		case 0xf8:
			flashcr = prm_1 & 0xE8;
			break;
		case 0xf9:
			flashfdr = prm_1;
			break;
		case 0xfa:
			flashwepr = prm_1;
			break;
		case 0xfb:
			flashicr = prm_1 & 0xC0;
			break;
		case 0xfc:
		case 0xfd:
		case 0xfe:
			flashptr[prm_0 - 0xfc] = prm_1 & ((prm_0 == 0xfd) ? 0x7 : 0xFF);
			break;
		case 0xff:
			flashpcr = prm_1 & 0x07;
			break;
        }
        break;
    case 3:
		if (prm_0 >= 0x100){ for (int cnt = 0; cnt < 4; cnt++) { if (((chipselect[cnt][2] & 16) != 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 8) & 0xFF) == chipselect[cnt][0])) { chipselectinfo = (cnt | 0x80); break; } } return f91memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24)); }
        switch (prm_0) {
		case 0x00:
			return 0x08;
			break;
		case 0x01:
			return 0x00;
			break;
		case 0x02:
			return 0xFF;
			break;

		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
			return intpr[prm_0-0x10];
			break;

		case 0xa8:
		case 0xa9:
		case 0xaa:
		case 0xab:
		case 0xac:
		case 0xad:
		case 0xae:
		case 0xaf:
		case 0xb0:
		case 0xb1:
		case 0xb2:
		case 0xb3:
			return chipselect[(prm_0 - 0xa8) / 3][(prm_0 - 0xa8) % 3];
			break;

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
		case 0xbb:
			return spisr = 0;
			break;
        case 0xbc:
            ret = buf4rt[0];
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

		case 0xf6:
			externaltime += flashfdr;
			if (&fname4if == NULL) { return 0; }
			flashfdcrpt = fopen(fname4if, "rb+");
			if (&flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
			if (&flashfdcrpt == 0) { return 0; }
			pointer4accflash = (((flashptr[0] & 0x7F) << 11) | ((flashptr[1] & ((flashptr[0] & 0x80) ? 0x01 : 0x07)) << 8) | ((flashptr[2] & 0xFF) << 0));
			fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
			flashptr[0] = (flashptr[0] & 0x80) | ((pointer4accflash >> 11) & 0x7F); flashptr[1] = (flashptr[1] & ((flashptr[0] & 0x80) ? 0xFE : 0xF8)) | ((pointer4accflash >> 8) & ((flashptr[0] & 0x80) ? 0x01 : 0x07)); flashptr[2] = ((pointer4accflash >> 0) & 0xFF);
			ret = fgetc(flashfdcrpt);
			fclose(flashfdcrpt);
			//cpu_int(0x50);
			return ret;
			break;
		case 0xf7:
			return flashaubr;
			break;
		case 0xf8:
			return flashcr;
			break;
		case 0xf9:
			return flashfdr;
			break;
		case 0xfa:
			return flashwepr;
			break;
		case 0xfb:
			return flashicr &= 0xD0;
			break;
		case 0xfc:
		case 0xfd:
		case 0xfe:
			return flashptr[prm_0 - 0xfc];
			break;
		case 0xff:
			return flashpcr;
			break;
		}
        break;
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
