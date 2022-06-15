#uselib "eZ80.dll"
#func cpu_crash "cpu_crash"
#cfunc cpu_execute "cpu_execute"
#cfunc cpu_get_regptr "cpu_get_regptr"
#func cpu_halt "cpu_halts"
#func cpu_init "cpu_init"
#func cpu_int "cpu_int" int
#func cpu_nmi "cpu_nmi"
#func cpu_reset "cpu_reset"
#func ez80macfuncset "ez80macfuncset" int
#func control_reset "control_reset"
#func control_restore "control_restore"
#func init_control "init_control"
#cfunc cpu_get_ctlptr "cpu_get_ctlptr"
#func cpu_set_speed "cpu_set_speed" int
#cfunc cpu_get_speed "cpu_get_speed"
cpu_init
init_control
