#include "ez80memaccess.h"
#ifdef VC_DLL_EXPORTS
#undef VC_DLL_EXPORTS
#define VC_DLL_EXPORTS extern "C" __declspec(dllexport)
#else
#define VC_DLL_EXPORTS extern "C" __declspec(dllimport)
#endif
VC_DLL_EXPORTS void ez80macfuncset(int (*tmp) (int, int, int));
int (*ez80memaccessx)(int, int, int);

void ez80macfuncset(int (*tmp) (int,int,int)) {
	ez80memaccessx = tmp;
}

int ez80memaccess(int prm_0, int prm_1, int prm_2) { return ez80memaccessx(prm_0, prm_1, prm_2); }
