int main(int argc, char *argv[]);
__size32 fib(int param2, __size32 param3, __size32 param4, __size32 param5, __size32 param6, __size32 param7, __size32 param9, __size32 param8, __size32 param9);

/** address: 0x00001cf0 */
int main(int argc, char *argv[])
{
    int g10; 		// r10
    int g11; 		// r11
    int g12; 		// r12
    int g3; 		// r3
    __size32 g31; 		// r31
    int g4; 		// r4
    int g5; 		// r5
    int g6; 		// r6
    int g7; 		// r7
    int g8; 		// r8
    int local0; 		// m[g1 - 32]

    printf(/* machine specific */ (int) LR + 724);
    g4 = scanf(/* machine specific */ (int) LR + 720); /* Warning: also results in g5, g6, g7, g8, g10, g11, g12 */
    g3 = fib(local0, g4, g5, g6, g7, g8, g10, g11, g12); /* Warning: also results in g31 */
    *(__size32*)(g30 + 68) = g3;
    printf(g31 + 740);
    return 0;
}

/** address: 0x00001c34 */
__size32 fib(int param2, __size32 param3, __size32 param4, __size32 param5, __size32 param6, __size32 param7, __size32 param9, __size32 param8, __size32 param9)
{
    int g0; 		// r0
    __size32 g1; 		// r1
    __size32 g10; 		// r10
    __size32 g11; 		// r11
    __size32 g12; 		// r12
    int g3; 		// r3
    __size32 g30_1; 		// r30{0}
    __size32 g30_5; 		// r30{0}
    __size32 g30_8; 		// r30{0}
    __size32 g30_9; 		// r30{0}
    __size32 g31_1; 		// r31{0}
    __size32 g31_4; 		// r31{0}
    __size32 g31_7; 		// r31{0}
    __size32 g31_8; 		// r31{0}
    __size32 g4; 		// r4
    __size32 g5; 		// r5
    __size32 g6; 		// r6
    __size32 g7; 		// r7
    __size32 g8; 		// r8
    int g9; 		// r9
    __size32 local10; 		// param7{0}
    __size32 local11; 		// param9{0}
    __size32 local12; 		// param8{0}
    __size32 local13; 		// param9{0}
    __size32 local14; 		// g30_8{0}
    __size32 local15; 		// g31_7{0}
    __size32 local6; 		// param3{0}
    __size32 local7; 		// param4{0}
    __size32 local8; 		// param5{0}
    __size32 local9; 		// param6{0}

    g30_1 = g1 - 96;
    g31_1 = /* machine specific */ (int) LR;
    local6 = param3;
    local6 = param3;
    local7 = param4;
    local7 = param4;
    local8 = param5;
    local8 = param5;
    local9 = param6;
    local9 = param6;
    local10 = param7;
    local10 = param7;
    local11 = param9;
    local11 = param9;
    local12 = param8;
    local12 = param8;
    local13 = param9;
    local13 = param9;
    local14 = g30_1;
    local14 = g30_1;
    local15 = g31_1;
    local15 = g31_1;
    if (param2 <= 1) {
        if (param2 != 1) {
        }
        else {
        }
    }
    else {
        g3 = fib(param2 - 1, param3, param4, param5, param6, param7, param9, param8, param9); /* Warning: also results in g4, g5, g6, g7, g8, g10, g11, g12 */
        *(__size32*)(g30_4 + 120) = g3;
        g3 = fib(g9 - 1, g4, g5, g6, g7, g8, g10, g11, g12); /* Warning: also results in g30_5, g31_4 */
        local14 = g30_5;
        local15 = g31_4;
        *(__size32*)(g30_5 + 64) = g3;
        g4 = printf(g31_4 + 908); /* Warning: also results in g5, g6, g7, g8, g10, g11, g12 */
        local6 = g4;
        local7 = g5;
        local8 = g6;
        local9 = g7;
        local10 = g8;
        local11 = g10;
        local12 = g11;
        local13 = g12;
        g0 = *(g30_5 + 120);
        *(int*)(g30_5 + 68) = g0;
    }
    param3 = local6;
    param4 = local7;
    param5 = local8;
    param6 = local9;
    param7 = local10;
    param9 = local11;
    param8 = local12;
    param9 = local13;
    g30_8 = local14;
    g31_7 = local15;
    g3 = *(g30_8 + 68);
    return g30_9; /* WARNING: Also returning: g31 := g31_8, g3 := g3, g4 := param3, g5 := param4, g6 := param5, g7 := param6, g8 := param7, g10 := param9, g11 := param8, g12 := param9, g30 := g30_9, g30 := g30_9, g30 := g30_8, g31 := g31_8, g31 := g31_8, g31 := g31_7 */
}
