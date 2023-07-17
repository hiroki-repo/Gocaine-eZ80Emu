// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"
#include <stdio.h>
#pragma warning(disable : 4996)

extern "C" FILE * flashfdcrpt;

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

typedef BOOL(*typeof_f92sendsignal)(BOOL);

UINT64 emacaddr = 0;
UINT16 EMAC_MAXF = 0x600;
int (*f92memaccess)(int, int, int);
void (*f92spiaccess)(bool);
void (*f92gpioack)(UINT32);
interrupt_state_t* (*cpu_get_interruptstat)();
void (*cpu_int)(int);
void (*cpu_reset)(void);
int (*cpu_execute)(void);
int externaltime = 0;
char* fname4if;
UINT8 ramaubr = 0xFF;
UINT8 ramcr = 0x80;
char intram2000h[0x2000];
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
UINT8 TMRx_CTR[6];
UINT8 TMRx_IER[6];
UINT8 TMRx_IIR[6];
UINT16 TMRx_DR[6];
UINT16 TMRx_RR[6];
UINT8 TMRx_CAP_CTL[6];
UINT16 TMRx_CAPA[6];
UINT8 TMR3_OC_CTLx[2];
UINT16 TMR3_OCx[6];
UINT16 TMR1_CAPB;

UINT16 UARTx_BRG[2] = { 2,2 };
UINT8 UARTx_IIR[2] = { 1,1 };
UINT8 UARTx_IER[2];
UINT8 UARTx_FCTL[2];
UINT8 UARTx_LCTL[2];
char UARTx_FIFOBuffer[2][2][16];
UINT8 UARTx_FIFOPos[2][2];
UINT8 UARTx_1ByteBuffer[2][2];
BOOL UARTx_valueof1ByteBuffer[2][2];
BOOL UARTx_DR[2];

UINT8 UARTx_MCTL[2];

UINT8 UARTx_SPR[2];

UINT8 FLASH_KEY_step = 0;

UINT8 EMAC_TEST = 0;
UINT32 EMAC_CFG = 0x000f3700;
UINT16 EMAC_TPTV = 0;
UINT8 EMAC_IPGT = 0x15;
UINT8 EMAC_IPGR[2] = { 0x0c,0x12 };
UINT8 EMAC_AFR = 0;
UINT64 EMAC_HTBL = 0;
UINT8 EMAC_MIIMGT = 0;
UINT16 EMAC_CTLD = 0;
UINT8 EMAC_RGAD = 0;
UINT8 EMAC_FIAD = 0;
UINT8 EMAC_PTMR = 0;
UINT8 EMAC_RST = 0x20;
UINT16 EMAC_TLBP = 0;
UINT32 EMAC_BP = 0xffc000;
UINT16 EMAC_RHBP = 0;
UINT16 EMAC_RRP = 0;
UINT8 EMAC_BUFSZ = 0;
UINT8 EMAC_IEN = 0;
UINT8 EMAC_ISTAT = 0;
UINT16 EMAC_PRSD = 0;
UINT8 EMAC_MIISTAT = 0;
UINT16 EMAC_RWP = 0;
UINT16 EMAC_TRP = 0;
UINT16 EMAC_BLKSLFT = 0x0020;
UINT16 EMAC_FDATA = 0x0000;
UINT8 EMAC_FFLAGS = 0x33;

UINT16 PLL_DIV = 2;
UINT8 PLL_CTL0 = 0;
UINT8 PLL_CTL1 = 0;

UINT8 PWM_CTL1 = 0;
UINT8 PWM_CTL2 = 0;
UINT8 PWM_CTL3 = 0;

UINT16 PWMxx[8] = { 0,0,0,0,0,0,0,0 };

const UINT8 UARTx_tlpos[4] = { 1,4,8,14 };
const UINT8 UARTx_prior1[8] = { 5,4,1,0,7,3,2,6 };
const UINT8 UARTx_prior2[8] = { 3,2,6,5,1,0,7,4 };

typeof_f92sendsignal* f92UARTRTS;
typeof_f92sendsignal* f92UARTDTR;
typeof_f92sendsignal* f92UARTCTS;
typeof_f92sendsignal* f92UARTDCD;
typeof_f92sendsignal* f92UARTDSR;
typeof_f92sendsignal* f92UARTRI;

void f92cpu_int(int prm_0) { if (intrpt != nullptr) { if (intrpt->status == 0) { cpuinterruptbak = 0; } } if (cpuinterruptbak >= prm_0) { cpu_int(prm_0); cpuinterruptbak = prm_0; }/*if (intrpt != nullptr) { if (intrpt->status == 0) { cpuinterruptbak = 0; } } if (cpuinterruptbak <= ((prm_0) | ((prm_0 >= 0x40) && (prm_0 <= 0xFF) ? (((intpr[(prm_0 - 0x40) / 8] >> (prm_0 % 8)) & 1) ? 0x100 : 0) : 0))) { cpu_int(prm_0); cpuinterruptbak = prm_0; }*/ }

__declspec(dllexport) void f92spimisocall(bool prm_0) { 
	buf4rttmpx &= 0x7; if (buf4rttmpx == 0) { buf4rttmp = 0; spisr &= 0x7F; } buf4rttmp |= prm_0 << (buf4rttmpx++); if (buf4rttmpx == 8) { spisr |= 0x80; buf4rt[buf4acc++] = buf4rttmp; buf4rttmpx = 0; if (spicr & 0x80) { f92cpu_int(0x1e); } }
}

__declspec(dllexport) UINT8 f92gpiocall(UINT32 prm_0,UINT8 prm_1,bool prm_2) {
	UINT16 f92gpiointvective = 0;
	UINT8 f92gpioretvalue = 0;
	for (int cnt = 0; cnt < 8; cnt++) {
		UINT8 f92gpioinmode = (((GPIO[prm_0][1] >> cnt) & 1) << 0) | (((GPIO[prm_0][2] >> cnt) & 1) << 1) | (((GPIO[prm_0][3] >> cnt) & 1) << 2);
		switch (f92gpioinmode) {
		case 0:
			f92gpioretvalue &= (~(1 << cnt));
			f92gpioretvalue |= ((((GPIO[prm_0][0] & (~GPIO[prm_0][1])) >> cnt) & 1) << cnt);
			break;
		case 1:
			if (prm_2 == false) {
				GPIO[prm_0][0] &= (~(1 << cnt));
				GPIO[prm_0][0] |= ((((prm_1 & GPIO[prm_0][1]) >> cnt) & 1) << cnt);
			}
			break;
		case 2:
			if ((GPIO[prm_0][0] >> cnt) & 1) { if (prm_2 == false){GPIO[prm_0][0] &= (~(1 << cnt)); GPIO[prm_0][0] |= ((((prm_1) >> cnt) & 1) << cnt);} }else{ f92gpioretvalue &= (~(1 << cnt)); f92gpioretvalue &= (~(1 << cnt)); }
			break;
		case 3:
			if ((GPIO[prm_0][0] >> cnt) & 1) { f92gpioretvalue &= (~(1 << cnt)); f92gpioretvalue |= ((1 << cnt)); }else{ if (prm_2 == false){GPIO[prm_0][0] &= (~(1 << cnt)); GPIO[prm_0][0] |= ((((prm_1) >> cnt) & 1) << cnt);} }
			break;
		case 4:
			if ((GPIO[prm_0][0] >> cnt) & 1) {
				if (((((prm_1 & (~val4GPIOMode6)) & GPIO[prm_0][0]) & (~GPIO[prm_0][4])) >> cnt) & 1) { if (((f92gpiointvective & 0xFF) >= (0x20 + (2 * cnt) + (32 * prm_0))) || ((((f92gpiointvective >> 8) & 1) != ((intpr[2 + prm_0] >> cnt) & 1)) && ((intpr[2 + prm_0] >> cnt) & 1) && (f92gpiointvective & 0x8000))) { f92gpiointvective = 0x8000 | (0x20 + (2 * cnt) + (32 * prm_0)); } f92gpiointvective |= (((intpr[2 + prm_0] >> cnt) & 1) ? 0x100 : 0); }
				val4GPIOMode6 &= (~(1 << cnt)); val4GPIOMode6 |= (((prm_1 >> cnt) & 1) << cnt);
			}
			break;
		case 5:
			break;
		case 6:
			if ((((~prm_1) ^ GPIO[prm_0][0]) >> cnt) & 1) { if (((f92gpiointvective & 0xFF) >= (0x20 + (2 * cnt) + (32 * prm_0))) || ((((f92gpiointvective >> 8) & 1) != ((intpr[2 + prm_0] >> cnt) & 1)) && ((intpr[2 + prm_0] >> cnt) & 1) && (f92gpiointvective & 0x8000))) { f92gpiointvective = 0x8000 | (0x20 + (2 * cnt) + (32 * prm_0)); } f92gpiointvective |= (((intpr[2 + prm_0] >> cnt) & 1) ? 0x100 : 0); }
			break;
		case 7:
			if (((prm_1 ^ GPIO[prm_0][0]) >> cnt) & 1) { if (((f92gpiointvective & 0xFF) >= (0x20 + (2 * cnt) + (32 * prm_0))) || ((((f92gpiointvective >> 8) & 1) != ((intpr[2 + prm_0] >> cnt) & 1)) && ((intpr[2 + prm_0] >> cnt) & 1) && (f92gpiointvective & 0x8000))) { f92gpiointvective = 0x8000 | (0x20 + (2 * cnt) + (32 * prm_0)); } f92gpiointvective |= (((intpr[2 + prm_0] >> cnt) & 1) ? 0x100 : 0); }
			break;
		}
	}
	if ((f92gpiointvective & 0x8000) && (prm_0 != 0)) { f92cpu_int(f92gpiointvective&0xFF); }
	return f92gpioretvalue;
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

__declspec(dllexport) void f92macfuncset(int (*tmpfunc)(int, int, int), void (*tmpfunc2)(bool), void (*tmpfunc3)(int), void (*tmpfunc4)(void), int (*tmpfunc5)(void), void (*tmpfunc6)(UINT32), interrupt_state_t* (*tmpfunc7)()) { f92memaccess = tmpfunc; f92spiaccess = tmpfunc2; cpu_int = tmpfunc3; cpu_reset = tmpfunc4; cpu_execute = tmpfunc5; f92gpioack = tmpfunc6; if (tmpfunc7 != nullptr) { intrpt = tmpfunc7(); } }
__declspec(dllexport) void f92internalflashpathset(char *tmpfname) { fname4if = tmpfname;
flashfdcrpt = fopen(fname4if, "rb+");
if (flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
}

void f92_pit(void) {
	bool isclockstockold = false;
	for (int cnt = 0; cnt < 6; cnt++) {
		if (TMRx_CTR[cnt] & 1) {
			if ((clockstock - clockstockold) >= (1 << ((((TMRx_CTR[cnt] >> 3) & 3) + 1) * 2))) {
				isclockstockold = true;
				if ((TMRx_DR[cnt] != 0) || (TMRx_CTR[cnt] & 4))
					TMRx_DR[cnt]--;
				if (TMRx_DR[cnt] == 0) { if (!(TMRx_CTR[cnt] & 16)) { TMRx_CTR[cnt] &= 0xFE; } TMRx_IIR[cnt] |= 1; if (TMRx_CTR[cnt] & 64) { f92cpu_int(0x0A + (cnt * 2)); } }
				if (((TMRx_CTR[cnt] & 0x12) && (TMRx_DR[cnt] == 0))) {
					TMRx_DR[cnt] = TMRx_RR[cnt];
				}
			}
		}
	}
	if (isclockstockold == true) { clockstockold = clockstock; }
}

__declspec(dllexport) int f92_execute(void) { UINT32 clockstocktmp = 0; cpuinterruptbak = 0; externaltime = 0; clockstocktmp = cpu_execute(); clockstocktmp += externaltime; clockstock += clockstocktmp; f92_pit(); return clockstocktmp; }

__declspec(dllexport) int f92_execute_cc(UINT32 prm_0) { UINT32 clockstocktmp2 = 0; while(clockstocktmp2 < prm_0){ UINT32 clockstocktmp = 0; cpuinterruptbak = 0; externaltime = 0; clockstocktmp = cpu_execute(); clockstocktmp += externaltime; clockstock += clockstocktmp; f92_pit(); clockstocktmp2 += clockstocktmp; } return clockstocktmp2; }

__declspec(dllexport) void f92_reset(void) { 
	
	emacaddr = 0;
	EMAC_MAXF = 0x600;
	ramaubr = 0xFF;
	ramcr = 0x80;
	for(int i=0;i<0x2000;i++){ intram2000h[i] = 0; }
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

	UARTx_BRG[0] = 2;
	UARTx_BRG[1] = 2;

	UARTx_IIR[0] = 1;
	UARTx_IIR[1] = 1;

	UARTx_IER[0] = 0;
	UARTx_IER[1] = 0;

	UARTx_LCTL[0] = 0;
	UARTx_LCTL[1] = 0;

	UARTx_FCTL[0] = 0;
	UARTx_FCTL[1] = 0;

	UARTx_FIFOPos[0][0] = 0;
	UARTx_FIFOPos[1][0] = 0;
	UARTx_FIFOPos[0][1] = 0;
	UARTx_FIFOPos[1][1] = 0;

	UARTx_MCTL[0] = 0;
	UARTx_MCTL[1] = 0;

	UARTx_valueof1ByteBuffer[0][0] = false;
	UARTx_valueof1ByteBuffer[1][0] = false;
	UARTx_valueof1ByteBuffer[0][1] = false;
	UARTx_valueof1ByteBuffer[1][1] = false;

	UARTx_DR[0] = false;
	UARTx_DR[1] = false;

	UARTx_SPR[0] = 0;
	UARTx_SPR[1] = 0;

	FLASH_KEY_step = 0;

	EMAC_TEST = 0;
	EMAC_CFG = 0x000f3700;
	EMAC_TPTV = 0;
	EMAC_IPGT = 0x15;
	EMAC_IPGR[0] = 0x0c;
	EMAC_IPGR[0] = 0x12;
	EMAC_AFR = 0;
	EMAC_HTBL = 0;
	EMAC_MIIMGT = 0;
	EMAC_CTLD = 0;
	EMAC_RGAD = 0;
	EMAC_FIAD = 0;
	EMAC_PTMR = 0;
	EMAC_RST = 0x20;
	EMAC_TLBP = 0;
	EMAC_BP = 0xffc000;
	EMAC_RHBP = 0;
	EMAC_RRP = 0;
	EMAC_BUFSZ = 0;
	EMAC_IEN = 0;
	EMAC_ISTAT = 0;
	EMAC_PRSD = 0;
	EMAC_MIISTAT = 0;
	EMAC_RWP = 0;
	EMAC_TRP = 0;
	EMAC_BLKSLFT = 0x0020;
	EMAC_FDATA = 0x0000;
	EMAC_FFLAGS = 0x33;

	PLL_DIV = 2;
	PLL_CTL0 = 0;
	PLL_CTL1 = 0;

	PWM_CTL1 = 0;
	PWM_CTL2 = 0;
	PWM_CTL3 = 0;

	for (int i = 0; i < 8; i++) { PWMxx[i] = 0; }
	for (int i = 0; i < 6; i++) { TMRx_CTR[i] = 0; TMRx_IER[i] = 0; TMRx_IIR[i] = 0; TMRx_DR[i] = 0; TMRx_RR[i] = 0; TMRx_CAP_CTL[i] = 0; TMRx_CAPA[i] = 0; TMR3_OCx[i] = 0; }
	for (int i = 0; i < 16; i++) { UARTx_FIFOBuffer[0][0][i] = 0; UARTx_FIFOBuffer[1][0][i] = 0; UARTx_FIFOBuffer[0][1][i] = 0; UARTx_FIFOBuffer[1][1][i] = 0; }
	cpu_reset();
}

__declspec(dllexport) UINT8 f92_receiveUART(int prm_0) { if (UARTx_FCTL[prm_0] & 1) { if ((UARTx_FIFOPos[prm_0][1] == 0) && (UARTx_prior1[((UARTx_IIR[prm_0] >> 1) & 7)] >= 3) && (UARTx_IER[prm_0] & 16)) { UARTx_IIR[prm_0] &= 0xF1; UARTx_IIR[prm_0] |= 0x0A; f92cpu_int(0x18 + ((prm_0 & 1) * 2)); } if (UARTx_FIFOPos[prm_0][1] == 0) { return 0xff; } UINT8 UARTretBUFF1b = UARTx_FIFOBuffer[prm_0][1][0]; memmove(&UARTx_FIFOBuffer[prm_0][1][0], &UARTx_FIFOBuffer[prm_0][1][1], (UARTx_FIFOPos[prm_0][1]--)); if ((UARTx_valueof1ByteBuffer[prm_0][1] == false) && (UARTx_prior1[((UARTx_IIR[prm_0] >> 1) & 7)] >= 4) && (UARTx_IER[prm_0] & 2)) { UARTx_IIR[prm_0] &= 0xF1; UARTx_IIR[prm_0] |= 0x02; f92cpu_int(0x18 + ((prm_0 & 1) * 2)); } return UARTretBUFF1b; } else { if ((UARTx_FIFOPos[prm_0][1] == 0) && (UARTx_prior1[((UARTx_IIR[prm_0] >> 1) & 7)] >= 4) && (UARTx_IER[prm_0] & 2)) { UARTx_IIR[prm_0] &= 0xF1; UARTx_IIR[prm_0] |= 0x02; f92cpu_int(0x18 + ((prm_0 & 1) * 2)); return 0xff; } UARTx_valueof1ByteBuffer[prm_0][1] = false; if ((UARTx_prior1[((UARTx_IIR[prm_0] >> 1) & 7)] >= 3) && (UARTx_IER[prm_0] & 16)) { UARTx_IIR[prm_0] &= 0xF1; UARTx_IIR[prm_0] |= 0x0A; f92cpu_int(0x18 + ((prm_0 & 1) * 2)); } return UARTx_1ByteBuffer[prm_0][1]; } };
__declspec(dllexport) void f92_transmitUART(int prm_0, UINT8 prm_1) { UARTx_DR[prm_0] = true; if (UARTx_FCTL[prm_0] & 1) { UARTx_FIFOBuffer[prm_0][0][UARTx_FIFOPos[prm_0][0]] = (prm_1 & ((1 << ((UARTx_LCTL[prm_0] & 3) + 5)) - 1)); UARTx_FIFOPos[prm_0][0]++; if ((UARTx_tlpos[(UARTx_FCTL[prm_0] >> 6) & 3] == UARTx_FIFOPos[prm_0][0]) && (UARTx_prior1[((UARTx_IIR[prm_0] >> 1) & 7)] >= 1) && (UARTx_IER[prm_0] & 1)) { UARTx_IIR[prm_0] &= 0xF1; UARTx_IIR[prm_0] |= 0x04; f92cpu_int(0x18+ ((prm_0 & 1) * 2)); } } else { UARTx_valueof1ByteBuffer[prm_0][0] = true; UARTx_1ByteBuffer[prm_0][0] = (prm_1 & ((1 << ((UARTx_LCTL[prm_0] & 3) + 5)) - 1)); if ((UARTx_prior1[((UARTx_IIR[prm_0] >> 1) & 7)] >= 1) && (UARTx_IER[prm_0] & 1)) { UARTx_IIR[prm_0] &= 0xF1; UARTx_IIR[prm_0] |= 0x04; f92cpu_int(0x18+ ((prm_0 & 1) * 2)); } } if ((UARTx_prior1[((UARTx_IIR[prm_0] >> 1) & 7)] >= 1) && (UARTx_IER[prm_0] & 1)) { UARTx_IIR[prm_0] &= 0xF1; UARTx_IIR[prm_0] |= 0x04; f92cpu_int(0x18+ ((prm_0 & 1) * 2)); } return; }
__declspec(dllexport) void f92_setmsgcbUART(typeof_f92sendsignal* prm_0, typeof_f92sendsignal* prm_1, typeof_f92sendsignal* prm_2, typeof_f92sendsignal* prm_3, typeof_f92sendsignal* prm_4, typeof_f92sendsignal* prm_5) { f92UARTRTS = prm_0; f92UARTDTR = prm_1; f92UARTCTS = prm_2; f92UARTDCD = prm_3; f92UARTDSR = prm_4; f92UARTRI = prm_5; }

UINT8 chipselectinfo = 0;

__declspec(dllexport) int mac4ez80dll(int prm_0, int prm_1, int prm_2) {
	chipselectinfo = 0;
    switch (prm_2) {
    case 0:
    case 1:
        if ((ramaubr == ((prm_0 >> 16) & 0xFF)) && (((prm_0 & 0xFFFF) >= (((ramcr & 0x80)?0xE0:0x100)<<8)) && ((prm_0 & 0xFFFF) < (((ramcr & 0x80)?0x100:0xE0)<<8)))) {
            switch (prm_2) { 
            case 0:
                intram2000h[prm_0 & 0x1FFF] = (prm_1 & 0xFF);
                return 0;
            case 1:
                return intram2000h[prm_0 & 0x1FFF];
            }
        } else if ((flashaubr <= ((prm_0 >> 16) & 0xFF)) && (((flashaubr+1)&0xFF) >= ((prm_0 >> 16) & 0xFF)) && ((flashcr & 0x08) != 0)) {
			externaltime += (flashcr >> 5);
			if (fname4if == nullptr) { return 0; }
			if ((&fname4if) == NULL) { return 0; }
			fseek(flashfdcrpt,(prm_0 - (flashaubr << 16)),SEEK_SET);
			ret = 0;
			switch (prm_2) {
			case 0:
				if (((flashwepr >> ((prm_0 - (flashaubr << 16)) >> 14))&1) == 0) { fputc((prm_1 & 0xFF), flashfdcrpt); }
			case 1:
				return fgetc(flashfdcrpt);//ret = fgetc(flashfdcrpt);
			}
			return ret;
		}
		else {
			for (int cnt = 0; cnt <= 3; cnt++) { if (((chipselect[cnt][2] & 16) == 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 16) & 0xFF) >= chipselect[cnt][0]) && (((prm_0 >> 16) & 0xFF) <= chipselect[cnt][1])) { chipselectinfo = (cnt | 0x80); break; } }
			return f92memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24));
        }
        break;
    case 2:
		if (prm_0 >= 0x100 || prm_0<=0x7f) { for (int cnt = 0; cnt <= 3; cnt++) { if (((chipselect[cnt][2] & 16) != 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 8) & 0xFF) == chipselect[cnt][0])) { chipselectinfo = (cnt | 0x80); break; } } return f92memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24)); }
		switch (prm_0) {
		case 0x80:
			TMRx_CTR[0] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[0] = TMRx_RR[0];
			}
			break;
		case 0x81:
			TMRx_RR[0] = (TMRx_RR[0] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x82:
			TMRx_RR[0] = (TMRx_RR[0] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x83:
			TMRx_CTR[1] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[1] = TMRx_RR[1];
			}
			break;
		case 0x84:
			TMRx_RR[1] = (TMRx_RR[1] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x85:
			TMRx_RR[1] = (TMRx_RR[1] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x86:
			TMRx_CTR[2] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[2] = TMRx_RR[2];
			}
			break;
		case 0x87:
			TMRx_RR[2] = (TMRx_RR[2] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x88:
			TMRx_RR[2] = (TMRx_RR[2] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x89:
			TMRx_CTR[3] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[3] = TMRx_RR[3];
			}
			break;
		case 0x8a:
			TMRx_RR[3] = (TMRx_RR[3] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x8b:
			TMRx_RR[3] = (TMRx_RR[3] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x8c:
			TMRx_CTR[4] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[4] = TMRx_RR[4];
			}
			break;
		case 0x8d:
			TMRx_RR[4] = (TMRx_RR[4] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x8e:
			TMRx_RR[4] = (TMRx_RR[4] & 0x00FF) | (prm_1 << (8 * 1));
			break;
		case 0x8f:
			TMRx_CTR[5] = prm_1;
			if (prm_1 & 2) {
				TMRx_DR[5] = TMRx_RR[5];
			}
			break;
		case 0x90:
			TMRx_RR[5] = (TMRx_RR[5] & 0xFF00) | (prm_1 << (8 * 0));
			break;
		case 0x91:
			TMRx_RR[5] = (TMRx_RR[5] & 0x00FF) | (prm_1 << (8 * 1));
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
			if ((((prm_0 - 0x96) % 4) == 0) && (f92gpioack != nullptr)) { for (int cnt = 0; cnt < 8; cnt++) { UINT8 f92gpioinmode = (((GPIO[(prm_0 - 0x96) / 4][1] >> cnt) & 1) << 0) | (((GPIO[(prm_0 - 0x96) / 4][2] >> cnt) & 1) << 1) | (((GPIO[(prm_0 - 0x96) / 4][3] >> cnt) & 1) << 2); if ((f92gpioinmode == 0) || (f92gpioinmode == 2) || (f92gpioinmode == 3)) { f92gpioack((prm_0 - 0x96) / 4); break; } } }
			break;
		/*case 0xa6:
		case 0xa7:
			GPIO[prm_0 - 0xa6][4] = prm_1;
			break;*/
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
            ramcr = prm_1&0x80;
            break;
        case 0xb5:
            ramaubr = prm_1;
            break;
        /*case 0xb6:
            mbist[0] = (prm_1 & 0x80) | (mbist[0] & 0x7F);
            break;
        case 0xb7:
            mbist[1] = (prm_1 & 0x80) | (mbist[1] & 0x7F);
            break;*/
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
			if (f92spiaccess == nullptr) { return 0; }
			if (&f92spiaccess == NULL) { return 0; }
			for (int i = 0; i < 8; i++) {
                f92spiaccess(((prm_1 >> i)&1) ? true : false);
            }
            break;

		case 0xc0:
			if (UARTx_LCTL[0] & 128) { UARTx_BRG[0] = (UARTx_BRG[0] & 0xFF00) | ((prm_1 & 0xFF) << (8 * 0)); }
			else { if (UARTx_FCTL[0] & 1) { UARTx_FIFOBuffer[0][1][UARTx_FIFOPos[0][1]] = (prm_1 & ((1 << ((UARTx_LCTL[0] & 3) + 5)) - 1)); UARTx_FIFOPos[0][1]++; } else { UARTx_valueof1ByteBuffer[0][1] = true; UARTx_1ByteBuffer[0][1] = (prm_1 & ((1 << ((UARTx_LCTL[0] & 3) + 5)) - 1)); } if ((f92UARTDTR[0] != nullptr) && (!(UARTx_MCTL[0] & 16))) { f92UARTDTR[0]((UARTx_MCTL[0] & 1) ? false : true); } }
			break;
		case 0xc1:
			if (UARTx_LCTL[0] & 128) { UARTx_BRG[0] = (UARTx_BRG[0] & 0x00FF) | ((prm_1 & 0xFF) << (8 * 1)); }
			else { UARTx_IER[0] = prm_1; }
			break;
		case 0xc2:
			UARTx_FCTL[0] = prm_1;
			break;
		case 0xc3:
			UARTx_LCTL[0] = prm_1;
			break;
		case 0xc4:
			UARTx_MCTL[0] = prm_1 & 0x7F;
			if ((f92UARTRTS[0] != nullptr)) { f92UARTRTS[0]((prm_1 & 2) ? false : true); }
			if ((f92UARTDTR[0] != nullptr)) { f92UARTDTR[0]((prm_1 & 1) ? false : true); }
			break;

		case 0xc7:
			UARTx_SPR[0] = prm_1;
			break;

		/*case 0xce:
		case 0xcf:
			GPIO[prm_0 - 0xcc][4] = prm_1;
			break;*/

		case 0xd0:
			if (UARTx_LCTL[1] & 128) { UARTx_BRG[1] = (UARTx_BRG[1] & 0xFF00) | ((prm_1 & 0xFF) << (8 * 0)); }
			else { if (UARTx_FCTL[1] & 1) { UARTx_FIFOBuffer[1][1][UARTx_FIFOPos[1][1]] = (prm_1 & ((1 << ((UARTx_LCTL[1] & 3) + 5)) - 1)); UARTx_FIFOPos[1][1]++; } else { UARTx_valueof1ByteBuffer[1][1] = true; UARTx_1ByteBuffer[1][1] = (prm_1 & ((1 << ((UARTx_LCTL[1] & 3) + 5)) - 1)); } if ((f92UARTDTR[1] != nullptr) && (!(UARTx_MCTL[1] & 16))) { f92UARTDTR[1]((UARTx_MCTL[1] & 1) ? false : true); } }
			break;
		case 0xd1:
			if (UARTx_LCTL[0] & 128) { UARTx_BRG[1] = (UARTx_BRG[1] & 0x00FF) | ((prm_1 & 0xFF) << (8 * 1)); }
			else { UARTx_IER[1] = prm_1; }
			break;
		case 0xd2:
			UARTx_FCTL[1] = prm_1;
			break;
		case 0xd3:
			UARTx_LCTL[1] = prm_1;
			break;
		case 0xd4:
			UARTx_MCTL[1] = prm_1 & 0x7F;
			if ((f92UARTRTS[1] != nullptr)) { f92UARTRTS[1]((prm_1 & 2) ? false : true); }
			if ((f92UARTDTR[1] != nullptr)) { f92UARTDTR[1]((prm_1 & 1) ? false : true); }
			break;

		case 0xd7:
			UARTx_SPR[1] = prm_1;
			break;

		case 0xf5:
			FLASH_KEY_step = (FLASH_KEY_step == 0) ? ((prm_1 == 0xb6) ? 1 : 0) : (FLASH_KEY_step == 1) ? ((prm_1 == 0x49) ? 2 : 0) : 0;
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
			if (((flashwepr >> ((pointer4accflash) >> 14)) & 1) == 0) { fputc((prm_1 & 0xFF), flashfdcrpt); flashicr |= 0x20; if (flashicr & 0x80) { f92cpu_int(0x08); } } else { flashicr |= 0x8; if (flashicr & 0x40) { f92cpu_int(0x08); } }
			//fclose(flashfdcrpt);
			break;
		case 0xf7:
			flashaubr = prm_1 & 0xFE;
			break;
		case 0xf8:
			flashcr = prm_1 & 0xE8;
			break;
		case 0xf9:
			if (FLASH_KEY_step >= 2) {
				flashfdr = prm_1;
			}
			break;
		case 0xfa:
			if (FLASH_KEY_step >= 2) {
				flashwepr = prm_1;
			}
			else {
				flashwepr = 0xff;
			}
			break;
		case 0xfb:
			flashicr = prm_1 & 0xC0;
			break;
		case 0xfc:
		case 0xfd:
		case 0xfe:
			flashptr[prm_0 - 0xfc] = prm_1 & ((prm_0 == 0xfd) ? 0x7 : ((prm_0 == 0xfe) ? 0x7F : 0xFF));
			break;
		case 0xff:
			flashpcr = prm_1 & 0x07;
			if (prm_1 & 2) {
				pointer4accflash = (((flashptr[0] & 0x7F) << 10) | ((flashptr[1] & ((flashptr[0] & 0x80) ? 0x01 : 0x07)) << 7) | ((flashptr[2] & 0x7F) << 0));
				for(int cnt = 0; cnt < 2048; cnt++) {
					fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
					if ((((flashwepr >> ((pointer4accflash) >> 14)) & 1) == 0) && ((flashptr[0] & 0x80) ? ((pointer4accflash % 2048) < 512) : true)) { fputc((0xff), flashfdcrpt); flashicr |= 0x20; if (flashicr & 0x80) { f92cpu_int(0x08); } }
					else { flashicr |= 0x2; if (flashicr & 0x40) { f92cpu_int(0x08); } }
				}
				if (!(flashicr & 2)) { flashpcr &= 0x5; }
			}
			if (prm_1 & 1) {
				pointer4accflash = 0;
				for (int cnt = 0; cnt < (256 * 1024); cnt++) {
					fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
					if ((((flashwepr >> ((pointer4accflash) >> 14)) & 1) == 0) && ((flashptr[0] & 0x80) ? true : ((pointer4accflash % 2048) >= 512))) { fputc((0xff), flashfdcrpt); flashicr |= 0x20; if (flashicr & 0x80) { f92cpu_int(0x08); } }
					else { flashicr |= 0x1; if (flashicr & 0x40) { f92cpu_int(0x08); } }
				}
				if (!(flashicr & 1)) { flashpcr &= 0x6; }
			}
			break;
        }
        break;
    case 3:
		if (prm_0 >= 0x100 || prm_0<=0x7f){ for (int cnt = 0; cnt <= 3; cnt++) { if (((chipselect[cnt][2] & 16) != 0) && (chipselect[cnt][2] & 8) && (((prm_0 >> 8) & 0xFF) == chipselect[cnt][0])) { chipselectinfo = (cnt | 0x80); break; } } return f92memaccess(prm_0, prm_1, prm_2 | (chipselectinfo << 24)); }
        switch (prm_0) {
		case 0x80:
			return TMRx_CTR[0];
			break;
		case 0x81:
			return (TMRx_DR[0] >> (8 * 0)) & 0xFF;
			break;
		case 0x82:
			return (TMRx_DR[0] >> (8 * 1)) & 0xFF;
			break;
		case 0x83:
			return TMRx_CTR[1];
			break;
		case 0x84:
			return (TMRx_DR[1] >> (8 * 0)) & 0xFF;
			break;
		case 0x85:
			return (TMRx_DR[1] >> (8 * 1)) & 0xFF;
			break;
		case 0x86:
			return TMRx_CTR[2];
			break;
		case 0x87:
			return (TMRx_DR[2] >> (8 * 0)) & 0xFF;
			break;
		case 0x88:
			return (TMRx_DR[2] >> (8 * 1)) & 0xFF;
			break;
		case 0x89:
			return TMRx_CTR[3];
			break;
		case 0x8a:
			return (TMRx_DR[3] >> (8 * 0)) & 0xFF;
			break;
		case 0x8b:
			return (TMRx_DR[3] >> (8 * 1)) & 0xFF;
			break;
		case 0x8c:
			return TMRx_CTR[4];
			break;
		case 0x8d:
			return (TMRx_DR[4] >> (8 * 0)) & 0xFF;
			break;
		case 0x8e:
			return (TMRx_DR[4] >> (8 * 1)) & 0xFF;
			break;
		case 0x8f:
			return TMRx_CTR[5];
			break;
		case 0x90:
			return (TMRx_DR[5] >> (8 * 0)) & 0xFF;
			break;
		case 0x91:
			return (TMRx_DR[5] >> (8 * 1)) & 0xFF;
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
			if ((((prm_0 - 0x96) % 4) == 0) && (f92gpioack != nullptr)) { f92gpioack((prm_0 - 0x96) / 4); }
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
        /*case 0xb6:
            return mbist[0];
            break;
        case 0xb7:
            return mbist[1];
            break;*/
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
			ret = spisr; spisr = 0; return ret;
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

		case 0xc0:
			if (UARTx_LCTL[0] & 128) { return (UARTx_BRG[0] >> (8 * 0)) & 0xFF; }
			else { if ((f92UARTRTS[0] != nullptr) && (!(UARTx_MCTL[0] & 16))) { f92UARTRTS[0]((UARTx_MCTL[0] & 2) ? false : true); } if (UARTx_FCTL[0] & 1) { UINT8 UARTretBUFF1b = UARTx_FIFOBuffer[0][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)][0]; memmove(&UARTx_FIFOBuffer[0][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)][0], &UARTx_FIFOBuffer[0][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)][0], (UARTx_FIFOPos[0][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)]--)); if (UARTx_FIFOPos[0][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)] == 0) { UARTx_DR[0] = false; } return UARTretBUFF1b; } else { UARTx_valueof1ByteBuffer[0][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)] = false; UARTx_DR[0] = false; return UARTx_1ByteBuffer[0][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)]; } }
			break;
		case 0xc1:
			if (UARTx_LCTL[0] & 128) { return (UARTx_BRG[0] >> (8 * 1)) & 0xFF; }
			else { return UARTx_IER[0]; }
			break;
		case 0xc2:
			ret = UARTx_IIR[0] | ((UARTx_FCTL[0] & 1) ? 128 : 0);
			UARTx_IIR[0] = 1;
			return ret;
			break;
		case 0xc3:
			return UARTx_LCTL[0];
			break;
		case 0xc4:
			return UARTx_MCTL[0];
			break;
		case 0xc5:
			return ((UARTx_FCTL[0] & 1) ? ((UARTx_FIFOPos[0][1 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)] == 0) ? 64 : 0) : ((UARTx_valueof1ByteBuffer[0][1 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)]) ? 0 : 64)) | ((UARTx_FCTL[0] & 1) ? ((UARTx_FIFOPos[0][1 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)] == 0) ? 32 : 0) : ((UARTx_valueof1ByteBuffer[0][1 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)]) ? 0 : 32)) | (UARTx_DR[0] ? 1 : 0 );
			break;
		case 0xc6:
			return (((!(UARTx_MCTL[0] & 16)) ? ((f92UARTCTS[0] != nullptr) ? (f92UARTCTS[0]((UARTx_MCTL[0] & 2) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[0] & 2) ? 1 : 0)) << 4) | (((!(UARTx_MCTL[0] & 16)) ? ((f92UARTDSR[0] != nullptr) ? (f92UARTDSR[0]((UARTx_MCTL[0] & 1) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[0] & 1) ? 1 : 0)) << 5) | (((!(UARTx_MCTL[0] & 16)) ? ((f92UARTRI[0] != nullptr) ? (f92UARTRI[0]((UARTx_MCTL[0] & 4) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[0] & 4) ? 1 : 0)) << 6) | (((!(UARTx_MCTL[0] & 16)) ? ((f92UARTDCD[0] != nullptr) ? (f92UARTDCD[0]((UARTx_MCTL[0] & 8) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[0] & 8) ? 1 : 0)) << 7);
			break;
		case 0xc7:
			return UARTx_SPR[0];
			break;

		case 0xd0:
			if (UARTx_LCTL[1] & 128) { return (UARTx_BRG[1] >> (8 * 0)) & 0xFF; }
			else { if ((f92UARTRTS[1] != nullptr) && (!(UARTx_MCTL[1] & 16))) { f92UARTRTS[1]((UARTx_MCTL[1] & 2) ? false : true); } if (UARTx_FCTL[1] & 1) { UINT8 UARTretBUFF1b = UARTx_FIFOBuffer[1][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)][0]; memmove(&UARTx_FIFOBuffer[1][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)][0], &UARTx_FIFOBuffer[1][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)][0], (UARTx_FIFOPos[1][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)]--)); if (UARTx_FIFOPos[1][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)] == 0) { UARTx_DR[1] = false; } return UARTretBUFF1b; } else { UARTx_valueof1ByteBuffer[1][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)] = false; UARTx_DR[1] = false; return UARTx_1ByteBuffer[1][0 ^ ((UARTx_MCTL[0] & 16) ? 1 : 0)]; } }
			break;
		case 0xd1:
			if (UARTx_LCTL[1] & 128) { return (UARTx_BRG[1] >> (8 * 1)) & 0xFF; }
			else { return UARTx_IER[1]; }
			break;
		case 0xd2:
			ret = UARTx_IIR[1] | ((UARTx_FCTL[1] & 1) ? 128 : 0);
			UARTx_IIR[1] = 1;
			return ret;
			break;
		case 0xd3:
			return UARTx_LCTL[1];
			break;
		case 0xd4:
			return UARTx_MCTL[1];
			break;
		case 0xd5:
			return ((UARTx_FCTL[1] & 1) ? ((UARTx_FIFOPos[1][1 ^ ((UARTx_MCTL[1] & 16) ? 1 : 0)] == 0) ? 64 : 0) : ((UARTx_valueof1ByteBuffer[1][1 ^ ((UARTx_MCTL[1] & 16) ? 1 : 0)]) ? 0 : 64)) | ((UARTx_FCTL[1] & 1) ? ((UARTx_FIFOPos[1][1 ^ ((UARTx_MCTL[1] & 16) ? 1 : 0)] == 0) ? 32 : 0) : ((UARTx_valueof1ByteBuffer[1][1 ^ ((UARTx_MCTL[1] & 16) ? 1 : 0)]) ? 0 : 32)) | (UARTx_DR[1] ? 1 : 0);
			break;
		case 0xd6:
			return (((!(UARTx_MCTL[1] & 16)) ? ((f92UARTCTS[1] != nullptr) ? (f92UARTCTS[1]((UARTx_MCTL[1] & 2) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[1] & 2) ? 1 : 0)) << 4) | (((!(UARTx_MCTL[1] & 16)) ? ((f92UARTDSR[1] != nullptr) ? (f92UARTDSR[1]((UARTx_MCTL[1] & 1) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[1] & 1) ? 1 : 0)) << 5) | (((!(UARTx_MCTL[1] & 16)) ? ((f92UARTRI[1] != nullptr) ? (f92UARTRI[1]((UARTx_MCTL[1] & 4) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[1] & 4) ? 1 : 0)) << 6) | (((!(UARTx_MCTL[1] & 16)) ? ((f92UARTDCD[1] != nullptr) ? (f92UARTDCD[1]((UARTx_MCTL[1] & 8) ? false : true) ? 1 : 0) : 0) : ((UARTx_MCTL[1] & 8) ? 1 : 0)) << 7);
			break;
		case 0xd7:
			return UARTx_SPR[1];
			break;

		case 0xf6:
			externaltime += flashfdr;
			if (fname4if == nullptr) { return 0; }
			if (&fname4if == NULL) { return 0; }
			//flashfdcrpt = fopen(fname4if, "rb+");
			//if (&flashfdcrpt == 0) { flashfdcrpt = fopen(fname4if, "wb"); }
			//if (&flashfdcrpt == 0) { return 0; }
			pointer4accflash = (((flashptr[0] & 0x7F) << 10) | ((flashptr[1] & ((flashptr[0] & 0x80) ? 0x01 : 0x07)) << 7) | ((flashptr[2] & 0x7F) << 0));
			fseek(flashfdcrpt, pointer4accflash++, SEEK_SET);
			flashptr[0] = (flashptr[0] & 0x80) | ((pointer4accflash >> 10) & 0x7F); flashptr[1] = (flashptr[1] & ((flashptr[0] & 0x80) ? 0xFE : 0xF8)) | ((pointer4accflash >> 7) & ((flashptr[0] & 0x80) ? 0x01 : 0x07)); flashptr[2] = ((pointer4accflash >> 0) & 0x7F);
			ret = fgetc(flashfdcrpt);
			//fclose(flashfdcrpt);
			//f92cpu_int(0x08);
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
