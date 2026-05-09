#include<windows.h>
#include<cstdint>
#include<cstring>

#define CODE_OFFSET 0x1F636     
#define LENGQUE_OFFSET 0x958C2 
#define MONEY_OFFSET 0x34786 
#define FANGJU_OFFSET 0x145B0E 
#define MIAOSHA_OFFSET 0x145DF5 
#define TIEMEM_OFFSET 0x145771 

#define SIZE_PATH 6


uintptr_t hook_End_SUN;
uintptr_t hook_End_MONEY;
uintptr_t hook_End_MIAOSHA;
uintptr_t hook_End_TIEMEM;
uintptr_t base = (uintptr_t)GetModuleHandleA("popcapgame1.exe");

void __declspec(naked) hook_sub() {
    __asm {
        pushad
        mov dword ptr [edi + 0x5578],9999
        popad
        jmp hook_End_SUN
    }
}

void __declspec(naked) hook_money() {
    _asm {
        mov eax,[eax+0x94C]
        mov edx,99999
        jmp hook_End_MONEY
    }
}

void __declspec(naked) hook_miaosha() {
    _asm {
        push eax
        mov eax,dword ptr[base]
        add eax,0x142170
        call eax
        mov ebp,0
        mov [esp+0x18],0   
        pop eax
        jmp hook_End_MIAOSHA
    }
}

void __declspec(naked)hook_tiemem() {
    __asm {
        mov [esi+0xDC],0
        jmp hook_End_TIEMEM
    }
}

void newsub_1() {
    DWORD old_mem_save;
    DWORD temp;
    uintptr_t work_addr = base + CODE_OFFSET;
    uintptr_t lengque_addr = base + LENGQUE_OFFSET;
    uintptr_t money_addr = base + MONEY_OFFSET;
    uintptr_t fangju_addr = base + FANGJU_OFFSET;
    uintptr_t miaosha_addr = base + MIAOSHA_OFFSET;
    uintptr_t tiemem_addr = base + TIEMEM_OFFSET;

    hook_End_SUN = work_addr + 6;
    hook_End_MIAOSHA = miaosha_addr + 9;
    hook_End_MONEY = money_addr + 6;
    hook_End_TIEMEM = tiemem_addr + 6;

    BYTE new_mem[SIZE_PATH] = {
        0xE9,0x00,0x00,0x00,0x00,0x90
    };

    BYTE new_mem_money[SIZE_PATH] = {
        0xE9,0x00,0x00,0x00,0x00,0x90
    };

    BYTE new_mem_lengque[3] = {
        0x83,0xF8,0xFF
    };
    int size_lengque = sizeof(new_mem_lengque);

    BYTE new_mem_fangju[2] = {
        0xEB,0x02
    };
    int size_fangju = sizeof(new_mem_fangju);

    BYTE new_mem_miaosha[9]{
        0xE9,0x00,0x00,0x00,0x00,0x90,0x90,0x90,0x90
    };
    int size_miaosha = sizeof(new_mem_miaosha);

    BYTE new_mem_tiemem[6]{
        0xE9,0x00,0x00,0x00,0x00,0x90
    };
    int size_tiemem = sizeof(new_mem_tiemem);

    //种植植物启动无限阳光
    uintptr_t jmp_offset = (uintptr_t)&hook_sub - (work_addr + 5);
    memcpy(&new_mem[1], &jmp_offset, 4);
    {
        VirtualProtect((LPVOID)work_addr, SIZE_PATH, PAGE_EXECUTE_READWRITE, &old_mem_save);
        memcpy((void*)work_addr, new_mem, SIZE_PATH);
        VirtualProtect((LPVOID)work_addr, SIZE_PATH, old_mem_save, &temp);
    }

    //无限冷却
    {
        VirtualProtect((LPVOID)lengque_addr, size_lengque, PAGE_EXECUTE_READWRITE, &old_mem_save);
        memcpy((void*)lengque_addr, new_mem_lengque, size_lengque);
        VirtualProtect((LPVOID)lengque_addr, size_lengque, old_mem_save, &temp);
    }

    //拾取金币启动无限金币
    uintptr_t jmp_offset_money = (uintptr_t)&hook_money - (money_addr + 5);
    memcpy(&new_mem_money[1],&jmp_offset_money, 4);
    {
        VirtualProtect((LPVOID)money_addr, SIZE_PATH, PAGE_EXECUTE_READWRITE, &old_mem_save);
        memcpy((void*)money_addr, new_mem_money, SIZE_PATH);
        VirtualProtect((LPVOID)money_addr, SIZE_PATH, old_mem_save, &temp);
    }

    //一击碎甲
    {
        VirtualProtect((LPVOID)fangju_addr, size_fangju, PAGE_EXECUTE_READWRITE, &old_mem_save);
        memcpy((void*)fangju_addr, new_mem_fangju, size_fangju);
        VirtualProtect((LPVOID)fangju_addr, size_fangju, old_mem_save, &temp);
    }

    //一击秒杀
    uintptr_t jmp_offset_miaosha = (uintptr_t)&hook_miaosha - (miaosha_addr + 5);
    memcpy(&new_mem_miaosha[1], &jmp_offset_miaosha, 4);
    VirtualProtect((LPVOID)miaosha_addr, size_miaosha, PAGE_EXECUTE_READWRITE, &old_mem_save);
    memcpy((void*)miaosha_addr, new_mem_miaosha, size_miaosha);
    VirtualProtect((LPVOID)miaosha_addr, size_miaosha, old_mem_save, &temp);

    //针对特殊类防具的秒碎
    uintptr_t jmp_offset_tiemem = (uintptr_t)&hook_tiemem - (tiemem_addr + 5);
    memcpy(&new_mem_tiemem[1], &jmp_offset_tiemem, 4);
    VirtualProtect((LPVOID)tiemem_addr, size_tiemem, PAGE_EXECUTE_READWRITE, &old_mem_save);
    memcpy((void*)tiemem_addr, new_mem_tiemem, size_tiemem);
    VirtualProtect((LPVOID)tiemem_addr, size_tiemem, old_mem_save, &temp);

    //是的，见到这个弹窗你就无敌了
    MessageBoxA(NULL, "dll is runing!", "DLL_WORKPATH", MB_OK);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:newsub_1();break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

