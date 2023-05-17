﻿// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"
#include <stdio.h>
#pragma warning(disable : 4996)

extern "C" FILE* flashfdcrpt;

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
		break;
	case DLL_PROCESS_DETACH:
		if (flashfdcrpt != nullptr) { fclose(flashfdcrpt); }
		break;
    }
    return TRUE;
}

#ifdef __cplusplus
extern "C"{
#endif
#define uint32_t UINT32

typedef struct interrupt_state {
	uint32_t status : 22;
	uint32_t : 2;
	uint32_t enabled : 22;
	uint32_t : 2;
	uint32_t latched : 22;
	uint32_t : 2;
	uint32_t inverted : 22;
	uint32_t : 2;
} interrupt_state_t;

interrupt_state_t* intrpt;

UINT64 emacaddr = 0;
UINT16 EMAC_MAXF = 0x600;
int (*f91memaccess)(int, int, int);
void (*f91spiaccess)(bool);
void (*f91gpioack)(UINT32);
interrupt_state_t* (*cpu_get_interruptstat)();
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
UINT8 GPIO[4][5] = { {0x00,0xff,0x00,0x00,0x00},{0x00,0xff,0x00,0x00,0x00},{0x00,0xff,0x00,0x00,0x00},{0x00,0xff,0x00,0x00,0x00} };
UINT8 val4GPIOMode6 = 0;
int pointer4accflash = 0;
FILE* flashfdcrpt = 0;
int ret = 0;
UINT8 cpuinterruptbak = 0;
UINT32 clockstock = 0;
UINT32 clockstockold = 0;
UINT8 TMRx_CTR[4];
UINT8 TMRx_IER[4];
UINT8 TMRx_IIR[4];
UINT16 TMRx_DR[4];
UINT16 TMRx_RR[4];
UINT8 TMRx_CAP_CTL[4];
UINT16 TMRx_CAPA[4];
UINT8 TMR3_OC_CTLx[2];
UINT16 TMR3_OCx[4];
UINT16 TMR1_CAPB;

void f91cpu_int(int prm_0) { if (intrpt != nullptr) { if (intrpt->status == 0) { cpuinterruptbak = 0; } } if (cpuinterruptbak <= ((prm_0) | ((prm_0 >= 0x40) && (prm_0 <= 0xFF) ? (((intpr[(prm_0 - 0x40) / 8] >> (prm_0 % 8)) & 1) ? 0x100 : 0) : 0))) { cpu_int(prm_0); cpuinterruptbak = prm_0; } }

__declspec(dllexport) void f91spimisocall(bool prm_0) { 
	buf4rttmpx &= 0x7; if (buf4rttmpx == 0) { buf4rttmp = 0; spisr &= 0x7F; } buf4rttmp |= prm_0 << (buf4rttmpx++); if (buf4rttmpx == 7) { spisr |= 0x80; buf4rt[buf4acc++] = buf4rttmp; buf4rttmpx = 0; if (spicr & 0x80) { f91cpu_int(0x7c); } }
}

__declspec(dllexport) UINT8 f91gpiocall(UINT32 prm_0,UINT8 prm_1,bool prm_2) {
	UINT16 f91gpiointvective = 0;
	UINT8 f91gpioretvalue = 0;
	for (int cnt = 0; cnt < 8; cnt++) {
		UINT8 f91gpioinmode = (((GPIO[prm_0][1] >> cnt) & 1) << 0) | (((GPIO[prm_0][2] >> cnt) & 1) << 1) | (((GPIO[prm_0][3] >> cnt) & 1) << 2);
		switch (f91gpioinmode) {
		case 0:
			f91gpioretvalue &= (~(1 << cnt));
			f91gpioretvalue |= ((((GPIO[prm_0][0] & (~GPIO[prm_0][1])) >> cnt) & 1) << cnt);
			break;
		case 1:
			if (prm_2 == false) {
				GPIO[prm_0][0] &= (~(1 << cnt));
				GPIO[prm_0][0] |= ((((prm_1 & GPIO[prm_0][1]) >> cnt) & 1) << cnt);
			}
			break;
		case 2:
			if ((GPIO[prm_0][0] >> cnt) & 1) { if (prm_2 == false){GPIO[prm_0][0] &= (~(1 << cnt)); GPIO[prm_0][0] |= ((((prm_1) >> cnt) & 1) << cnt);} }else{ f91gpioretvalue &= (~(1 << cnt)); f91gpioretvalue &= (~(1 << cnt)); }
			break;
		case 3:
			if ((GPIO[prm_0][0] >> cnt) & 1) { f91gpioretvalue &= (~(1 << cnt)); f91gpioretvalue |= ((1 << cnt)); }else{ if (prm_2 == false){GPIO[prm_0][0] &= (~(1 << cnt)); GPIO[prm_0][0] |= ((((prm_1) >> cnt) & 1) << cnt);} }
			break;
		case 4:
			if ((GPIO[prm_0][0] >> cnt) & 1) {
				if (((((prm_1 & (~val4GPIOMode6)) & GPIO[prm_0][0]) & (~GPIO[prm_0][4])) >> cnt) & 1) { if (((f91gpiointvective & 0xFF) >= (0x80 + (4 * cnt) + (32 * prm_0))) || ((((f91gpiointvective >> 8) & 1) != ((intpr[2 + prm_0] >> cnt) & 1)) && ((intpr[2 + prm_0] >> cnt) & 1) && (f91gpiointvective & 0x8000))) { f91gpiointvective = 0x8000 | (0x80 + (4 * cnt) + (32 * prm_0)); } f91gpiointvective |= (((intpr[2 + prm_0] >> cnt) & 1) ? 0x100 : 0); }
				val4GPIOMode6 &= (~(1 << cnt)); val4GPIOMode6 |= (((prm_1 >> cnt) & 1) << cnt);
			}
			break;
		case 5:
			break;
		case 6:
			if ((((~prm_1) ^ GPIO[prm_0][0]) >> cnt) & 1) { if (((f91gpiointvective & 0xFF) >= (0x80 + (4 * cnt) + (32 * prm_0))) || ((((f91gpiointvective >> 8) & 1) != ((intpr[2 + prm_0] >> cnt) & 1)) && ((intpr[2 + prm_0] >> cnt) & 1) && (f91gpiointvective & 0x8000))) { f91gpiointvective = 0x8000 | (0x80 + (4 * cnt) + (32 * prm_0)); } f91gpiointvective |= (((intpr[2 + prm_0] >> cnt) & 1) ? 0x100 : 0); }
			break;
		case 7:
			if (((prm_1 ^ GPIO[prm_0][0]) >> cnt) & 1) { if (((f91gpiointvective & 0xFF) >= (0x80 + (4 * cnt) + (32 * prm_0))) || ((((f91gpiointvective >> 8) & 1) != ((intpr[2 + prm_0] >> cnt) & 1)) && ((intpr[2 + prm_0] >> cnt) & 1) && (f91gpiointvective & 0x8000))) { f91gpiointvective = 0x8000 | (0x80 + (4 * cnt) + (32 * prm_0)); } f91gpiointvective |= (((intpr[2 + prm_0] >> cnt) & 1) ? 0x100 : 0); }
			break;
		}
	}
	if (f91gpiointvective & 0x8000) { f91cpu_int(f91gpiointvective&0xFF); }
	return f91gpioretvalue;
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

__declspec(dllexport) void f91macfuncset(int (*tmpfunc)(int, int, int), void (*tmpfunc2)(bool), void (*tmpfunc3)(int), void (*tmpfunc4)(void), int (*tmpfunc5)(void), void (*tmpfunc6)(UINT32), interrupt_state_t* (*tmpfunc7)()) { f91memaccess = tmpfunc; f91spiaccess = tmpfunc2; cpu_int = tmpfunc3; cpu_reset = tmpfunc4; cpu_execute = tmpfunc5; f91gpioack = tmpfunc6; if (tmpfunc7 != nullptr) { intrpt = tmpfunc7(); } }
__declspec(dllexport) void f91internalflashpathset(char *tmpfname) { fname4if = tmpfname;
flashfdcrpt = fopen(fname4if, "rb+");
if (flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
}

void f91_pit(void) {
	bool isclockstockold = false;
	for (int cnt = 0; cnt < 4; cnt++) {
		if (TMRx_CTR[cnt] & 1) {
			if ((clockstock - clockstockold) >= (1 << ((((TMRx_CTR[cnt] >> 3) & 3) + 1) * 2))) {
				isclockstockold = true;
				if ((TMRx_DR[cnt] != 0) || (TMRx_CTR[cnt] & 4))
					TMRx_DR[cnt]--;
				if (TMRx_DR[cnt] == 0) { if (!(TMRx_CTR[cnt] & 4)) { TMRx_CTR[cnt] &= 0xFE; } TMRx_IIR[cnt] |= 1; if (TMRx_IER[cnt] & 1) { f91cpu_int(0x54 + (cnt * 4)); } }
				for (int cnt2 = 0; cnt2 < 4; cnt2++) {
					if (((TMRx_IER[cnt] >> (3 + cnt2)) & 1) && (TMRx_DR[cnt] == TMR3_OCx[cnt2]) && (TMR3_OC_CTLx[0] & 1)) { TMRx_IIR[cnt] |= (1 << (3 + cnt2)); if ((TMRx_IER[cnt] >> (3 + cnt2)) & 1) { f91cpu_int(0x54 + (cnt * 4)); } }
				}
				if (((TMRx_CTR[cnt] & 4) && (TMRx_DR[cnt] == 0))) {
					TMRx_DR[cnt] = TMRx_RR[cnt];
				}
			}
		}
	}
	if (isclockstockold == true) { clockstockold = clockstock; }
}

__declspec(dllexport) int f91_execute(void) { UINT32 clockstocktmp = 0; cpuinterruptbak = 0; externaltime = 0; clockstocktmp = cpu_execute(); clockstocktmp += externaltime; clockstock += clockstocktmp; f91_pit(); return clockstocktmp; }

__declspec(dllexport) int f91_execute_cc(UINT32 prm_0) { UINT32 clockstocktmp2 = 0; while(clockstocktmp2 < prm_0){ UINT32 clockstocktmp = 0; cpuinterruptbak = 0; externaltime = 0; clockstocktmp = cpu_execute(); clockstocktmp += externaltime; clockstock += clockstocktmp; f91_pit(); clockstocktmp2 += clockstocktmp; } return clockstocktmp2; }

__declspec(dllexport) void f91_reset(void) { 
	
	emacaddr = 0;
	EMAC_MAXF = 0x600;
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
	chipselect[0][0] = 0x00;
	chipselect[0][1] = 0xff;
	chipselect[0][2] = 0xe8;
	chipselect[1][0] = 0x00;
	chipselect[1][1] = 0x00;
	chipselect[1][2] = 0x00;
	chipselect[2][0] = 0x00;
	chipselect[2][1] = 0x00;
	chipselect[2][2] = 0x00;
	chipselect[3][0] = 0x00;
	chipselect[3][1] = 0x00;
	chipselect[3][2] = 0x00;
	GPIO[0][0] = 0x00;
	GPIO[0][1] = 0xff;
	GPIO[0][2] = 0x00;
	GPIO[0][3] = 0x00;
	GPIO[0][4] = 0x00;

	GPIO[1][0] = 0x00;
	GPIO[1][1] = 0xff;
	GPIO[1][2] = 0x00;
	GPIO[1][3] = 0x00;
	GPIO[1][4] = 0x00;

	GPIO[2][0] = 0x00;
	GPIO[2][1] = 0xff;
	GPIO[2][2] = 0x00;
	GPIO[2][3] = 0x00;
	GPIO[2][4] = 0x00;

	GPIO[3][0] = 0x00;
	GPIO[3][1] = 0xff;
	GPIO[3][2] = 0x00;
	GPIO[3][3] = 0x00;
	GPIO[3][4] = 0x00;

	clockstock=0;
	clockstockold = 0;
	TMR3_OC_CTLx[0] = 0;
	TMR3_OC_CTLx[1] = 0;

	TMR1_CAPB = 0;

	for (int i = 0; i < 4; i++) { TMRx_CTR[i] = 0; TMRx_IER[i] = 0; TMRx_IIR[i] = 0; TMRx_DR[i] = 0; TMRx_RR[i] = 0; TMRx_CAP_CTL[i] = 0; TMRx_CAPA[i] = 0; TMR3_OCx[i] = 0; }
	cpu_reset();
}

UINT8 chipselectinfo = 0;

__declspec(dllexport) int mac4ez80dll(int prm_0, int prm_1, int prm_2) {
	chipselectinfo = 0;
    switch (prm_2) {
    case 0:
    case 1:
        if ((ramaubr == ((prm_0 >> 16) & 0xFF)) && (((prm_0 & 0xFFFF) >= (((ramcr&0x40)?0xC0:((ramcr & 0x80)?0xE0:0x100))<<8)) && ((prm_0 & 0xFFFF) < (((ramcr & 0x80)?0x100:0xE0)<<8)))) {
            switch (prm_2) { 
            case 0:
                intram4000h[prm_0 & 0x3FFF] = (prm_1 & 0xFF);
                return 0;
            case 1:
                return intram4000h[prm_0 & 0x3FFF];
            }
        } else if ((flashaubr <= ((prm_0 >> 16) & 0xFF)) && (((flashaubr+3)&0xFF) >= ((prm_0 >> 16) & 0xFF)) && ((flashcr & 0x08) != 0)) {
			externaltime += (flashcr >> 5);
			if (fname4if == nullptr) { return 0; }
			if ((&fname4if) == NULL) { return 0; }
			fseek(flashfdcrpt,(prm_0 - (flashaubr << 16)),SEEK_SET);
			ret = 0;
			switch (prm_2) {
			case 0:
				if (((flashwepr >> ((prm_0 - (flashaubr << 16)) >> 15))&1) == 0) { fputc((prm_1 & 0xFF), flashfdcrpt); }
			case 1:
				return fgetc(flashfdcrpt);//ret = fgetc(flashfdcrpt);
			}
			return ret;
		}
		else {
			for (int cnt = 0; cnt <= 3; cnt++) { if (((chipselect[cnt][2] & 16) == 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 16) & 0xFF) >= chipselect[cnt][0]) && (((prm_0 >> 16) & 0xFF) <= chipselect[cnt][1])) { chipselectinfo = (cnt | 0x80); break; } }
			return f91memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24));
        }
        break;
    case 2:
		if (prm_0 >= 0x100) { for (int cnt = 0; cnt <= 3; cnt++) { if (((chipselect[cnt][2] & 16) != 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 8) & 0xFF) == chipselect[cnt][0])) { chipselectinfo = (cnt | 0x80); break; } } return f91memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24)); }
		switch (prm_0) {
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
			intpr[prm_0 - 0x10] = prm_1;
			break;

		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:
		case 0x2a:
			emacaddr = (emacaddr & (~(255 << (8 * (prm_0 - 0x25))))) | (((UINT64)prm_1 & 0xFF) << (8 * (prm_0 - 0x25)));
			break;

		case 0x30:
		case 0x31:
			EMAC_MAXF = (EMAC_MAXF & ((prm_0 & 1) ? 0x00ff : 0xff00)) | ((prm_1 & 0xFF) << (8 * (prm_0 & 1)));
			break;

		case 0x60:
			TMRx_CTR[0] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[0] = TMRx_RR[0];
			}
			break;
		case 0x61:
			TMRx_IER[0] = (prm_1 & 0x7f);
			break;

		case 0x63:
			TMRx_RR[0] = (TMRx_RR[0] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x64:
			TMRx_RR[0] = (TMRx_RR[0] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x65:
			TMRx_CTR[1] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[1] = TMRx_RR[1];
			}
			break;
		case 0x66:
			TMRx_IER[1] = (prm_1 & 0x7f);
			break;

		case 0x68:
			TMRx_RR[1] = (TMRx_RR[1] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x69:
			TMRx_RR[1] = (TMRx_RR[1] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x6a:
			TMRx_CAP_CTL[1] = prm_1;
			break;
		case 0x6b:
			TMRx_CAPA[1] = (TMRx_CAPA[1] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x6c:
			TMRx_CAPA[1] = (TMRx_CAPA[1] & 0x00FF) | (prm_1 << (8 * 1));
			break;

		case 0x6f:
			TMRx_CTR[2] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[2] = TMRx_RR[2];
			}
			break;
		case 0x70:
			TMRx_IER[2] = (prm_1 & 0x7f);
			break;

		case 0x72:
			TMRx_RR[2] = (TMRx_RR[2] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x73:
			TMRx_RR[2] = (TMRx_RR[2] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x74:
			TMRx_CTR[3] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[3] = TMRx_RR[3];
			}
			break;
		case 0x75:
			TMRx_IER[3] = (prm_1 & 0x7f);
			break;

		case 0x77:
			TMRx_RR[3] = (TMRx_RR[3] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x78:
			TMRx_RR[3] = (TMRx_RR[3] & 0x00FF) | (prm_1 << (8 * 1));
			break;

		case 0x7b:
			TMRx_CAP_CTL[1] = prm_1;
			break;
		case 0x7c:
			TMRx_CAPA[3] = (TMRx_CAPA[3] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x7d:
			TMRx_CAPA[3] = (TMRx_CAPA[3] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x7e:
			TMR1_CAPB = (TMR1_CAPB & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x7f:
			TMR1_CAPB = (TMR1_CAPB & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x80:
		case 0x81:
			TMR3_OC_CTLx[prm_0 & 1] = prm_1;
			break;
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
			TMR3_OCx[(prm_0 - 0x82) / 2] = (TMR3_OCx[(prm_0 - 0x82) / 2] & ((prm_0 & 1) ? 0x00FF : 0xFF00)) | ((prm_1 & 0xFF) << ((prm_0 & 1) * 8));
			break;

		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9a:
		case 0x9b:
		case 0x9c:
		case 0x9d:
		case 0x9e:
		case 0x9f:
		case 0xa0:
		case 0xa1:
		case 0xa2:
		case 0xa3:
		case 0xa4:
		case 0xa5:
			GPIO[(prm_0 - 0x96) / 4][(prm_0 - 0x96) % 4] = prm_1;
			if ((((prm_0 - 0x96) % 4) == 0) && (f91gpioack != nullptr)) { for (int cnt = 0; cnt < 8; cnt++) { UINT8 f91gpioinmode = (((GPIO[(prm_0 - 0x96) / 4][1] >> cnt) & 1) << 0) | (((GPIO[(prm_0 - 0x96) / 4][2] >> cnt) & 1) << 1) | (((GPIO[(prm_0 - 0x96) / 4][3] >> cnt) & 1) << 2); if ((f91gpioinmode == 0) || (f91gpioinmode == 2) || (f91gpioinmode == 3)) { f91gpioack((prm_0 - 0x96) / 4); break; } } }
			break;
		case 0xa6:
		case 0xa7:
			GPIO[prm_0 - 0xa6][4] = prm_1;
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
			if (f91spiaccess == nullptr) { return 0; }
			if (&f91spiaccess == NULL) { return 0; }
			for (int i = 0; i < 8; i++) {
                f91spiaccess(((prm_1 >> i)&1) ? true : false);
            }
            break;

		case 0xce:
		case 0xcf:
			GPIO[prm_0 - 0xcc][4] = prm_1;
			break;

		case 0xf5:
			break;
		case 0xf6:
			externaltime += flashfdr;
			if (fname4if == nullptr) { return 0; }
			if (&fname4if == NULL) { return 0; }
			//flashfdcrpt = fopen(fname4if, "rb+");
			//if (&flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
			//if (&flashfdcrpt == 0) { return 0; }
			pointer4accflash = (((flashptr[0] & 0x7F) << 11) | ((flashptr[1] & ((flashptr[0] & 0x80) ? 0x01 : 0x07)) << 8) | ((flashptr[2] & 0xFF) << 0));
			fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
			flashptr[0] = (flashptr[0] & 0x80) | ((pointer4accflash >> 11) & 0x7F); flashptr[1] = (flashptr[1] & ((flashptr[0] & 0x80) ? 0xFE : 0xF8)) | ((pointer4accflash >> 8) & ((flashptr[0] & 0x80) ? 0x01 : 0x07)); flashptr[2] = ((pointer4accflash >> 0) & 0xFF);
			if (((flashwepr >> ((pointer4accflash) >> 15)) & 1) == 0) { fputc((prm_1 & 0xFF), flashfdcrpt); flashicr |= 0x20; if (flashicr & 0x80) { f91cpu_int(0x50); } } else { flashicr |= 0x8; if (flashicr & 0x40) { f91cpu_int(0x50); } }
			//fclose(flashfdcrpt);
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
			if (prm_1 & 2) {
				pointer4accflash = (((flashptr[0] & 0x7F) << 11) | ((flashptr[1] & ((flashptr[0] & 0x80) ? 0x01 : 0x07)) << 8) | ((flashptr[2] & 0xFF) << 0));
				for(int cnt = 0; cnt < 2048; cnt++) {
					fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
					if ((((flashwepr >> ((pointer4accflash) >> 15)) & 1) == 0) && ((flashptr[0] & 0x80) ? ((pointer4accflash % 2048) < 512) : true)) { fputc((0), flashfdcrpt); flashicr |= 0x20; if (flashicr & 0x80) { f91cpu_int(0x50); } }
					else { flashicr |= 0x2; if (flashicr & 0x40) { f91cpu_int(0x50); } }
				}
				if (!(flashicr & 2)) { flashpcr &= 0x5; }
			}
			if (prm_1 & 1) {
				pointer4accflash = 0;
				for (int cnt = 0; cnt < 2048; cnt++) {
					fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
					if ((((flashwepr >> ((pointer4accflash) >> 15)) & 1) == 0) && ((flashptr[0] & 0x80) ? true : ((pointer4accflash % 2048) >= 512))) { fputc((0), flashfdcrpt); flashicr |= 0x20; if (flashicr & 0x80) { f91cpu_int(0x50); } }
					else { flashicr |= 0x1; if (flashicr & 0x40) { f91cpu_int(0x50); } }
				}
				if (!(flashicr & 1)) { flashpcr &= 0x6; }
			}
			break;
        }
        break;
    case 3:
		if (prm_0 >= 0x100){ for (int cnt = 0; cnt <= 3; cnt++) { if (((chipselect[cnt][2] & 16) != 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 8) & 0xFF) == chipselect[cnt][0])) { chipselectinfo = (cnt | 0x80); break; } } return f91memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24)); }
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

		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:
		case 0x2a:
			return (emacaddr >> (8 * (prm_0 - 0x25))) & 0xFF;
			break;

		case 0x30:
		case 0x31:
			return (EMAC_MAXF >> (8 * (prm_0 & 1))) & 0xFF;
			break;

		case 0x60:
			return TMRx_CTR[0];
			break;
		case 0x61:
			return TMRx_IER[0];
			break;
		case 0x62:
		{UINT8 iirtmp = TMRx_IIR[0]; TMRx_IIR[0] = 0; return iirtmp; }
			break;
		case 0x63:
			return (TMRx_DR[0] >> (8 * 0)) & 0xFF;
			break;
		case 0x64:
			return (TMRx_DR[0] >> (8 * 1)) & 0xFF;
			break;
		case 0x65:
			return TMRx_CTR[1];
			break;
		case 0x66:
			return TMRx_IER[1];
			break;
		case 0x67:
		{UINT8 iirtmp = TMRx_IIR[1]; TMRx_IIR[1] = 0; return iirtmp; }
			break;
		case 0x68:
			return (TMRx_DR[1] >> (8 * 0)) & 0xFF;
			break;
		case 0x69:
			return (TMRx_DR[1] >> (8 * 1)) & 0xFF;
			break;
		case 0x6a:
			return TMRx_CAP_CTL[1];
			break;
		case 0x6b:
			return (TMRx_CAPA[1] >> (8 * 0)) & 0xFF;
			break;
		case 0x6c:
			return (TMRx_CAPA[1] >> (8 * 1)) & 0xFF;
			break;

		case 0x6f:
			return TMRx_CTR[2];
			break;
		case 0x70:
			return TMRx_IER[2];
			break;
		case 0x71:
		{UINT8 iirtmp = TMRx_IIR[2]; TMRx_IIR[2] = 0; return iirtmp; }
			break;
		case 0x72:
			return (TMRx_DR[2] >> (8 * 0)) & 0xFF;
			break;
		case 0x73:
			return (TMRx_DR[2] >> (8 * 1)) & 0xFF;
			break;
		case 0x74:
			return TMRx_CTR[3];
			break;
		case 0x75:
			return TMRx_IER[3];
			break;
		case 0x76:
		{UINT8 iirtmp = TMRx_IIR[3]; TMRx_IIR[3] = 0; return iirtmp; }
			break;
		case 0x77:
			return (TMRx_DR[3] >> (8 * 0)) & 0xFF;
			break;
		case 0x78:
			return (TMRx_DR[3] >> (8 * 1)) & 0xFF;
			break;

		case 0x7b:
			return TMRx_CAP_CTL[1];
			break;
		case 0x7c:
			return (TMRx_CAPA[3] >> (8 * 0)) & 0xFF;
			break;
		case 0x7d:
			return (TMRx_CAPA[3] >> (8 * 1)) & 0xFF;
			break;
		case 0x7e:
			return (TMR1_CAPB >> (8 * 0)) & 0xFF;
			break;
		case 0x7f:
			return (TMR1_CAPB >> (8 * 1)) & 0xFF;
			break;
		case 0x80:
		case 0x81:
			return TMR3_OC_CTLx[prm_0 & 1];
			break;
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
			return (TMR3_OCx[(prm_0 - 0x82) / 2] >> ((prm_0 & 1) * 8)) & 0xFF;
			break;

		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9a:
		case 0x9b:
		case 0x9c:
		case 0x9d:
		case 0x9e:
		case 0x9f:
		case 0xa0:
		case 0xa1:
		case 0xa2:
		case 0xa3:
		case 0xa4:
		case 0xa5:
			return GPIO[(prm_0 - 0x96) / 4][(prm_0 - 0x96) % 4];
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
			if (fname4if == nullptr) { return 0; }
			if (&fname4if == NULL) { return 0; }
			//flashfdcrpt = fopen(fname4if, "rb+");
			//if (&flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
			//if (&flashfdcrpt == 0) { return 0; }
			pointer4accflash = (((flashptr[0] & 0x7F) << 11) | ((flashptr[1] & ((flashptr[0] & 0x80) ? 0x01 : 0x07)) << 8) | ((flashptr[2] & 0xFF) << 0));
			fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
			flashptr[0] = (flashptr[0] & 0x80) | ((pointer4accflash >> 11) & 0x7F); flashptr[1] = (flashptr[1] & ((flashptr[0] & 0x80) ? 0xFE : 0xF8)) | ((pointer4accflash >> 8) & ((flashptr[0] & 0x80) ? 0x01 : 0x07)); flashptr[2] = ((pointer4accflash >> 0) & 0xFF);
			ret = fgetc(flashfdcrpt);
			//fclose(flashfdcrpt);
			//f91cpu_int(0x50);
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
			ret = flashicr;
			flashicr &= 0xD0;
			return ret;
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
