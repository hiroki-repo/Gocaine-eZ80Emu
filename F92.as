#uselib "F92.dll"
#cfunc f92_execute "f92_execute"
#cfunc f92_execute_cc "f92_execute_cc" int
#cfunc f92_receiveUART "f92_receiveUART" int
#func f92_reset "f92_reset"
#func f92_setmsgcbUART "f92_setmsgcbUART" int,int,int,int,int,int
#func f92_transmitUART "f92_transmitUART" int,int
#func f92internalflashpathset "f92internalflashpathset" sptr
#func f92macfuncset "f92macfuncset" int,int,int,int,int,int,int
#func f92spimisocall "f92spimisocall" int
#cfunc mac4ez80dll_f92 "mac4ez80dll" int,int,int
#cfunc f92gpiocall "f92gpiocall" int,int,int
