#uselib "F91.dll"
#cfunc f91_execute "f91_execute"
#cfunc f91_execute_cc "f91_execute_cc" int
#cfunc f91_receiveUART "f91_receiveUART" int
#func f91_reset "f91_reset"
#func f91_setmsgcbUART "f91_setmsgcbUART" int,int,int,int,int,int
#func f91_transmitUART "f91_transmitUART" int,int
#func f91internalflashpathset "f91internalflashpathset" sptr
#func f91macfuncset "f91macfuncset" int,int,int,int,int,int,int
#func f91spimisocall "f91spimisocall" int
#cfunc mac4ez80dll "mac4ez80dll" int,int,int
#cfunc f91gpiocall "f91gpiocall" int,int,int
