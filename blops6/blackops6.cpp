#include "blackops6.h"
#define peb mem.GetProcessPebAddress()
#define IO_READ_MEM mem.Read
#define base Addresses::base

void Game::scatter_ingame(VMMDLL_SCATTER_HANDLE handle) {
   mem.AddScatterReadRequest(handle, base + steam::game_mode, &InGame, sizeof(Game::InGame));
}

void Player::get_index(uint64_t client_info) {
    uintptr_t index_ptr = mem.Read<uintptr_t>(client_info + steam::local_index);
    Player::index = mem.Read<int>(index_ptr + steam::local_index_pos);
}

void Player::info(VMMDLL_SCATTER_HANDLE handle) {
    mem.AddScatterReadRequest(handle, Player::SPlayer + steam::player::valid, &valid, sizeof(Player::valid));
    mem.AddScatterReadRequest(handle, Player::SPlayer + steam::player::team, &teamId, sizeof(Player::teamId));
    mem.AddScatterReadRequest(handle, Player::SPlayer + steam::player::pos, &posPtr, sizeof(Player::posPtr));
}

void Player::updatePlayer(VMMDLL_SCATTER_HANDLE handle) {
    mem.AddScatterReadRequest(handle, Player::posPtr + 0x50, &pos, sizeof(Player::pos));
    mem.AddScatterReadRequest(handle, Player::SPlayer + steam::player::dead_1, &dead, sizeof(Player::dead));
    mem.AddScatterReadRequest(handle, Player::SPlayer + steam::player::stance, &stance, sizeof(Player::stance));
}

void Player::getCam(VMMDLL_SCATTER_HANDLE handle) {
    mem.AddScatterReadRequest(handle, Addresses::base + steam::camera_base, &camPtr, sizeof(Player::camPtr));
}

void Player::updateCam(VMMDLL_SCATTER_HANDLE handle) {
    mem.AddScatterReadRequest(handle, Player::camPtr + steam::camera_pos, &cam, sizeof(Player::cam));
}

void Player::updateBoneinfo(VMMDLL_SCATTER_HANDLE handle, uint64_t ClientInfo, uintptr_t BoneBase, Player& localPlayer) {
}
void Player::updateBoneW2Sinfo(Player& localplayer) {
}


uintptr_t steam::decrypt_client_info()
{
    const uint64_t mb = base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rbx = IO_READ_MEM<uintptr_t>(base + 0x13621F18);
    if (!rbx)
        return rbx;
    rcx = peb;          //mov rcx, gs:[rax]
    //rdx = 000001B4DEB500E8//failed to trace. base: 00007FF6147D0000 It's possibly wrong
    rbx += 0xFFFFFFFFFFFF9357;              //add rbx, 0xFFFFFFFFFFFF9357
    rcx -= rdx;             //sub rcx, rdx
    rcx += rbx;             //add rcx, rbx
    rax = rcx;              //mov rax, rcx
    rax >>= 0x1D;           //shr rax, 0x1D
    rcx ^= rax;             //xor rcx, rax
    rbx = rcx;              //mov rbx, rcx
    rax = 0;                //and rax, 0xFFFFFFFFC0000000
    rbx >>= 0x3A;           //shr rbx, 0x3A
    rax = _rotl64(rax, 0x10);               //rol rax, 0x10
    rbx ^= rcx;             //xor rbx, rcx
    rax ^= IO_READ_MEM<uintptr_t>(base + 0xC9D737C);            //xor rax, [0x000000000A1C5338]
    rax = _byteswap_uint64(rax);            //bswap rax
    rbx *= IO_READ_MEM<uintptr_t>(rax + 0x15);              //imul rbx, [rax+0x15]
    rax = 0xD67810518E7F25DD;               //mov rax, 0xD67810518E7F25DD
    rbx *= rax;             //imul rbx, rax
    rax = rbx;              //mov rax, rbx
    rax >>= 0x5;            //shr rax, 0x05
    rbx ^= rax;             //xor rbx, rax
    rax = rbx;              //mov rax, rbx
    rax >>= 0xA;            //shr rax, 0x0A
    rbx ^= rax;             //xor rbx, rax
    rax = rbx;              //mov rax, rbx
    rax >>= 0x14;           //shr rax, 0x14
    rbx ^= rax;             //xor rbx, rax
    rax = rbx;              //mov rax, rbx
    rax >>= 0x28;           //shr rax, 0x28
    rbx ^= rax;             //xor rbx, rax
    return rbx;
}
uintptr_t steam::decrypt_client_base(uintptr_t client_info)
{
    const uint64_t mb = base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rax = IO_READ_MEM<uintptr_t>(client_info + 0x1d3630);
    if (!rax)
        return rax;
    r11 = ~peb;          //mov r11, gs:[rcx]
    rcx = r11;              //mov rcx, r11
    //failed to translate: mov [rsp+0x3E0], r13
    rcx = _rotl64(rcx, 0x34);               //rol rcx, 0x34
    rcx &= 0xF;
    switch (rcx) {
    case 0:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C512F5]
        r13 = base + 0x16CBED5E;            //lea r13, [0x0000000013B76FFB]
        rdx = base + 0x33D36DEA;            //lea rdx, [0x0000000030BEF029]
        rax ^= r11;             //xor rax, r11
        rax ^= rdx;             //xor rax, rdx
        rcx = r11;              //mov rcx, r11
        rcx *= r13;             //imul rcx, r13
        rax -= rcx;             //sub rax, rcx
        rcx = 0x2F44AFC8DA410289;               //mov rcx, 0x2F44AFC8DA410289
        rax *= rcx;             //imul rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = 0x31EA6C7327F2F48F;               //mov rcx, 0x31EA6C7327F2F48F
        rax *= rcx;             //imul rax, rcx
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        uintptr_t RSP_0xFFFFFFFFFFFFFF90;
        RSP_0xFFFFFFFFFFFFFF90 = base + 0xDD05;             //lea rcx, [0xFFFFFFFFFCEC5FBE] : RBP+0xFFFFFFFFFFFFFF90
        rcx ^= RSP_0xFFFFFFFFFFFFFF90;          //xor rcx, [rbp-0x70]
        rax -= rcx;             //sub rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xF;            //shr rcx, 0x0F
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1E;           //shr rcx, 0x1E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x3C;           //shr rcx, 0x3C
        rax ^= rcx;             //xor rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB8061]
        rax -= rcx;             //sub rax, rcx
        return rax;
    }
    case 1:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);              //mov r9, [0x0000000009C50E13]
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB799B]
        rcx += 0x4C577047;              //add rcx, 0x4C577047
        rcx += r11;             //add rcx, r11
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x16;           //shr rcx, 0x16
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2C;           //shr rcx, 0x2C
        rax ^= rcx;             //xor rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB7AA1]
        rax ^= rcx;             //xor rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB7B6C]
        rcx += 0x673BCC74;              //add rcx, 0x673BCC74
        rcx += r11;             //add rcx, r11
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x14CEC15D15237845;               //mov rcx, 0x14CEC15D15237845
        rax *= rcx;             //imul rax, rcx
        rcx = 0xC6C8F26557A42C57;               //mov rcx, 0xC6C8F26557A42C57
        rax *= rcx;             //imul rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        return rax;
    }
    case 2:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);              //mov r9, [0x0000000009C5099D]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xC;            //shr rcx, 0x0C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x18;           //shr rcx, 0x18
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x30;           //shr rcx, 0x30
        rax ^= rcx;             //xor rax, rcx
        rax -= r11;             //sub rax, r11
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = base + 0x40BA9587;            //lea rcx, [0x000000003DA60D89]
        rax += r11;             //add rax, r11
        rax += rcx;             //add rax, rcx
        rcx = 0xB820512AEEAE20F1;               //mov rcx, 0xB820512AEEAE20F1
        rax *= rcx;             //imul rax, rcx
        rcx = 0x5B4702F448FEE148;               //mov rcx, 0x5B4702F448FEE148
        rax ^= rcx;             //xor rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB77DA]
        rax += rcx;             //add rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x12;           //shr rcx, 0x12
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x24;           //shr rcx, 0x24
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 3:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C50451]
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB712E]
        rax -= rcx;             //sub rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB6FAF]
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x13;           //shr rcx, 0x13
        rax ^= rcx;             //xor rax, rcx
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rcx = rax;              //mov rcx, rax
        rdx ^= r10;             //xor rdx, r10
        rcx >>= 0x26;           //shr rcx, 0x26
        rdx = ~rdx;             //not rdx
        rax ^= rcx;             //xor rax, rcx
        rax *= IO_READ_MEM<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        uintptr_t RSP_0xFFFFFFFFFFFFFF88;
        RSP_0xFFFFFFFFFFFFFF88 = base + 0x639AA956;                 //lea rcx, [0x0000000060861D26] : RBP+0xFFFFFFFFFFFFFF88
        rcx += RSP_0xFFFFFFFFFFFFFF88;          //add rcx, [rbp-0x78]
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xEF7AA6541B0960DD;               //mov rcx, 0xEF7AA6541B0960DD
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xAC44478E4E7E319F;               //mov rcx, 0xAC44478E4E7E319F
        rax *= rcx;             //imul rax, rcx
        rcx = 0x69DF0E377EDBC9BB;               //mov rcx, 0x69DF0E377EDBC9BB
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 4:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);              //mov r9, [0x0000000009C4FFD5]
        rcx = 0x6B6B6FEB24A18CBC;               //mov rcx, 0x6B6B6FEB24A18CBC
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xE26425F12DE4CEB;                //mov rcx, 0xE26425F12DE4CEB
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x15;           //shr rcx, 0x15
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2A;           //shr rcx, 0x2A
        rax ^= rcx;             //xor rax, rcx
        rax += r11;             //add rax, r11
        rcx = 0xC6BD746DB1DF1B31;               //mov rcx, 0xC6BD746DB1DF1B31
        rax *= rcx;             //imul rax, rcx
        rax -= base;                //sub rax, [rbp-0x70] -- didn't find trace -> use base
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1;            //shr rcx, 0x01
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2;            //shr rcx, 0x02
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x4;            //shr rcx, 0x04
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x8;            //shr rcx, 0x08
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x10;           //shr rcx, 0x10
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 5:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4F8CF]
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rcx = r11 * 0xFFFFFFFFFFFFFFFE;                 //imul rcx, r11, 0xFFFFFFFFFFFFFFFE
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rax += rcx;             //add rax, rcx
        rdx ^= r10;             //xor rdx, r10
        rcx = base + 0x5F737FAA;            //lea rcx, [0x000000005C5EE5B8]
        rdx = ~rdx;             //not rdx
        rax += rcx;             //add rax, rcx
        rax *= IO_READ_MEM<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x19;           //shr rcx, 0x19
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x32;           //shr rcx, 0x32
        rax ^= rcx;             //xor rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB6490]
        rcx += 0x6987CC3C;              //add rcx, 0x6987CC3C
        rcx += r11;             //add rcx, r11
        rax += rcx;             //add rax, rcx
        rcx = 0xBEC2B746A9461603;               //mov rcx, 0xBEC2B746A9461603
        rax *= rcx;             //imul rax, rcx
        rcx = 0x7D25D991052F24B5;               //mov rcx, 0x7D25D991052F24B5
        rax += rcx;             //add rax, rcx
        rcx = 0x6B9AD86F193C7172;               //mov rcx, 0x6B9AD86F193C7172
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 6:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4F463]
        rdx = base + 0x50AF1F4F;            //lea rdx, [0x000000004D9A82F5]
        rax += r11;             //add rax, r11
        rcx = r11;              //mov rcx, r11
        rcx ^= rdx;             //xor rcx, rdx
        rax -= rcx;             //sub rax, rcx
        rcx = 0xFA37AFACEF63040B;               //mov rcx, 0xFA37AFACEF63040B
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xE15A50F0F0B4D5D9;               //mov rcx, 0xE15A50F0F0B4D5D9
        rax *= rcx;             //imul rax, rcx
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rcx = base + 0xEE8B;                //lea rcx, [0xFFFFFFFFFCEC4E9B]
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rcx -= r11;             //sub rcx, r11
        rax ^= rcx;             //xor rax, rcx
        rdx ^= r10;             //xor rdx, r10
        rdx = ~rdx;             //not rdx
        rax *= IO_READ_MEM<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
        rcx = 0x61E687AD8B6807D4;               //mov rcx, 0x61E687AD8B6807D4
        rax += rcx;             //add rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x15;           //shr rcx, 0x15
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2A;           //shr rcx, 0x2A
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 7:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4EF4D]
        r13 = base + 0xBEE4;                //lea r13, [0xFFFFFFFFFCEC1DE4]
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB5B9A]
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x8DB3E6B3BD449D8;                //mov rcx, 0x8DB3E6B3BD449D8
        rax += rcx;             //add rax, rcx
        rcx = 0x7CFD8CC5318E532F;               //mov rcx, 0x7CFD8CC5318E532F
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x7;            //shr rcx, 0x07
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xE;            //shr rcx, 0x0E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1C;           //shr rcx, 0x1C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x38;           //shr rcx, 0x38
        rax ^= rcx;             //xor rax, rcx
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rcx = 0x6DE6E637B4DB68F1;               //mov rcx, 0x6DE6E637B4DB68F1
        rdx ^= r10;             //xor rdx, r10
        rdx = ~rdx;             //not rdx
        rax *= IO_READ_MEM<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
        rax *= rcx;             //imul rax, rcx
        rcx = r11;              //mov rcx, r11
        rcx -= r13;             //sub rcx, r13
        rax ^= rcx;             //xor rax, rcx
        rax -= r11;             //sub rax, r11
        return rax;
    }
    case 8:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);              //mov r9, [0x0000000009C4EAFE]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1E;           //shr rcx, 0x1E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x3C;           //shr rcx, 0x3C
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xD16E8EE163C9A6B;                //mov rcx, 0xD16E8EE163C9A6B
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xE;            //shr rcx, 0x0E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1C;           //shr rcx, 0x1C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x38;           //shr rcx, 0x38
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xA9C58221B90E7C46;               //mov rcx, 0xA9C58221B90E7C46
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x55F6ED3AB42B87A0;               //mov rcx, 0x55F6ED3AB42B87A0
        rax -= rcx;             //sub rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB55E3]
        rax += rcx;             //add rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x14;           //shr rcx, 0x14
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x28;           //shr rcx, 0x28
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 9:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4E43B]
        rdx = r11;              //mov rdx, r11
        rcx = base + 0xFCE2;                //lea rcx, [0xFFFFFFFFFCEC4F2E]
        rdx *= rcx;             //imul rdx, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB5241]
        rdx -= rcx;             //sub rdx, rcx
        rcx = r11;              //mov rcx, r11
        rax += rdx;             //add rax, rdx
        rcx = ~rcx;             //not rcx
        rax ^= rcx;             //xor rax, rcx
        rdx = base + 0x6EC75C18;            //lea rdx, [0x000000006BB2AE43]
        rax ^= rdx;             //xor rax, rdx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x7;            //shr rcx, 0x07
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xE;            //shr rcx, 0x0E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1C;           //shr rcx, 0x1C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x38;           //shr rcx, 0x38
        rax ^= rcx;             //xor rax, rcx
        r13 = 0xB691FFD9C9D61B2E;               //mov r13, 0xB691FFD9C9D61B2E
        rax += r13;             //add rax, r13
        r13 = 0x89C1B16D1EAC4B85;               //mov r13, 0x89C1B16D1EAC4B85
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rax ^= r13;             //xor rax, r13
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = 0x272542A0099256AB;               //mov rcx, 0x272542A0099256AB
        rax *= rcx;             //imul rax, rcx
        return rax;
    }
    case 10:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4DF09]
        rcx = base + 0x240D8AB0;            //lea rcx, [0x0000000020F8D81B]
        rcx += r11;             //add rcx, r11
        rax ^= rcx;             //xor rax, rcx
        rdx = base + 0x3B21;                //lea rdx, [0xFFFFFFFFFCEB863E]
        rcx = r11;              //mov rcx, r11
        rdx *= r11;             //imul rdx, r11
        rcx = ~rcx;             //not rcx
        rdx += rax;             //add rdx, rax
        rax = base + 0x24E689C1;            //lea rax, [0x0000000021D1D4BA]
        rcx += rax;             //add rcx, rax
        rax = rdx;              //mov rax, rdx
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x489A1BC87CDCD670;               //mov rcx, 0x489A1BC87CDCD670
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xA00242052F60AE53;               //mov rcx, 0xA00242052F60AE53
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = 0xBDB64B81FECB6E7D;               //mov rcx, 0xBDB64B81FECB6E7D
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x28;           //shr rcx, 0x28
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 11:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4DA22]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x15;           //shr rcx, 0x15
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2A;           //shr rcx, 0x2A
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xD3A53D9499733245;               //mov rcx, 0xD3A53D9499733245
        rax *= rcx;             //imul rax, rcx
        rcx = 0x72C2AC821062ABD1;               //mov rcx, 0x72C2AC821062ABD1
        rax ^= rcx;             //xor rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB44D6]
        rax -= rcx;             //sub rax, rcx
        rax += 0xFFFFFFFFFFFF4954;              //add rax, 0xFFFFFFFFFFFF4954
        rax += r11;             //add rax, r11
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1;            //shr rcx, 0x01
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2;            //shr rcx, 0x02
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x4;            //shr rcx, 0x04
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x8;            //shr rcx, 0x08
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x10;           //shr rcx, 0x10
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB4874]
        rax ^= rcx;             //xor rax, rcx
        rcx = base + 0x5B0222F4;            //lea rcx, [0x0000000057ED6775]
        rcx *= r11;             //imul rcx, r11
        rax += rcx;             //add rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        return rax;
    }
    case 12:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4D4A3]
        r13 = base + 0x852;                 //lea r13, [0xFFFFFFFFFCEB4CAD]
        rcx = 0xF7D4FABCE6FC022;                //mov rcx, 0xF7D4FABCE6FC022
        rax += rcx;             //add rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB4206]
        rax ^= rcx;             //xor rax, rcx
        rax -= rcx;             //sub rax, rcx
        rcx = 0x5C9D9DBA026E85B7;               //mov rcx, 0x5C9D9DBA026E85B7
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rdx = r11;              //mov rdx, r11
        rcx >>= 0x21;           //shr rcx, 0x21
        rdx = ~rdx;             //not rdx
        rdx *= r13;             //imul rdx, r13
        rdx ^= rcx;             //xor rdx, rcx
        rax ^= rdx;             //xor rax, rdx
        rax += r11;             //add rax, r11
        return rax;
    }
    case 13:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4D0D3]
        r13 = base + 0x12585A59;            //lea r13, [0x000000000F439AE4]
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rcx ^= r13;             //xor rcx, r13
        rax += rcx;             //add rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x8;            //shr rcx, 0x08
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x10;           //shr rcx, 0x10
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x11;           //shr rcx, 0x11
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x22;           //shr rcx, 0x22
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = 0xA4979265BBC7E3D5;               //mov rcx, 0xA4979265BBC7E3D5
        rax *= rcx;             //imul rax, rcx
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rcx -= base;                //sub rcx, [rbp-0x70] -- didn't find trace -> use base
        rcx -= 0x756FAD6E;              //sub rcx, 0x756FAD6E
        rcx ^= rax;             //xor rcx, rax
        rax = base + 0x78217335;            //lea rax, [0x00000000750CB1E2]
        rcx += r11;             //add rcx, r11
        rax += rcx;             //add rax, rcx
        rcx = 0xC20F4E2AD24BC517;               //mov rcx, 0xC20F4E2AD24BC517
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 14:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4CB55]
        r13 = base + 0x3CF1;                //lea r13, [0xFFFFFFFFFCEB77FE]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x16;           //shr rcx, 0x16
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2C;           //shr rcx, 0x2C
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xAF96B7C88EDF2B75;               //mov rcx, 0xAF96B7C88EDF2B75
        rax *= rcx;             //imul rax, rcx
        rax ^= r11;             //xor rax, r11
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rdx ^= r10;             //xor rdx, r10
        rcx = r11 + r13 * 1;            //lea rcx, [r11+r13*1]
        rax += rcx;             //add rax, rcx
        rdx = ~rdx;             //not rdx
        rcx = 0x7B695E53D3CD7B7F;               //mov rcx, 0x7B695E53D3CD7B7F
        rax *= IO_READ_MEM<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
        rax *= rcx;             //imul rax, rcx
        rcx = 0x56A40B352BF2FDB7;               //mov rcx, 0x56A40B352BF2FDB7
        rax -= rcx;             //sub rax, rcx
        rcx = base + 0x32FFEB8B;            //lea rcx, [0x000000002FEB25B1]
        rcx = ~rcx;             //not rcx
        rcx *= r11;             //imul rcx, r11
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 15:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D73E5);             //mov r10, [0x0000000009C4C5FC]
        rdx = base + 0x23AC2B6F;            //lea rdx, [0x000000002097607C]
        rcx = base;                 //lea rcx, [0xFFFFFFFFFCEB3386]
        rax -= rcx;             //sub rax, rcx
        rax += r11;             //add rax, r11
        rdx = ~rdx;             //not rdx
        rdx ^= r11;             //xor rdx, r11
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= IO_READ_MEM<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x17;           //shr rcx, 0x17
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2E;           //shr rcx, 0x2E
        rax ^= rcx;             //xor rax, rcx
        rax -= rdx;             //sub rax, rdx
        rcx = 0xAE9091426078C4DF;               //mov rcx, 0xAE9091426078C4DF
        rax *= rcx;             //imul rax, rcx
        rcx = 0x2E839B5F3DB76D2B;               //mov rcx, 0x2E839B5F3DB76D2B
        rax += rcx;             //add rax, rcx
        rcx = 0x632E9341FBDD9A7C;               //mov rcx, 0x632E9341FBDD9A7C
        rax -= rcx;             //sub rax, rcx
        return rax;
    }
    }
}
uintptr_t steam::decrypt_bone_base()
{
    const uint64_t mb = base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rdx = IO_READ_MEM<uintptr_t>(base + 0x11324D68);
    if (!rdx)
        return rdx;
    r11 = peb;          //mov r11, gs:[rax]
    rax = r11;              //mov rax, r11
    rax <<= 0x22;           //shl rax, 0x22
    rax = _byteswap_uint64(rax);            //bswap rax
    rax &= 0xF;
    switch (rax) {
    case 0:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);              //mov r9, [0x000000000A2D34A4]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = base;                 //lea rax, [0xFFFFFFFFFD53A0A6]
        rdx -= rax;             //sub rdx, rax
        rax = 0xC98EF2FFB1E013D2;               //mov rax, 0xC98EF2FFB1E013D2
        rdx += 0xFFFFFFFFFFFFC795;              //add rdx, 0xFFFFFFFFFFFFC795
        rdx += r11;             //add rdx, r11
        rdx ^= rax;             //xor rdx, rax
        rdx ^= r11;             //xor rdx, r11
        rax = 0x1AE0F1058D3590F1;               //mov rax, 0x1AE0F1058D3590F1
        rdx *= rax;             //imul rdx, rax
        rax = 0x1EAC0325CBA779BC;               //mov rax, 0x1EAC0325CBA779BC
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1F;           //shr rax, 0x1F
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x3E;           //shr rax, 0x3E
        rdx ^= rax;             //xor rdx, rax
        rdx ^= r11;             //xor rdx, r11
        return rdx;
    }
    case 1:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2D2FA8]
        rax = 0x311E3C7DD1297B69;               //mov rax, 0x311E3C7DD1297B69
        rdx *= rax;             //imul rdx, rax
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = base + 0x437D3D7F;            //lea rax, [0x0000000040D0DA51]
        rax = ~rax;             //not rax
        rcx += rax;             //add rcx, rax
        rdx ^= rcx;             //xor rdx, rcx
        rax = rdx;              //mov rax, rdx
        rax >>= 0x28;           //shr rax, 0x28
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x13;           //shr rax, 0x13
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = base;                 //lea rax, [0xFFFFFFFFFD539C9C]
        rax += 0x3B261317;              //add rax, 0x3B261317
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rax = 0x85B82AEE944DCF96;               //mov rax, 0x85B82AEE944DCF96
        rdx ^= rax;             //xor rdx, rax
        rax = base;                 //lea rax, [0xFFFFFFFFFD539DA9]
        rdx += rax;             //add rdx, rax
        return rdx;
    }
    case 2:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);              //mov r9, [0x000000000A2D2ACF]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x25;           //shr rax, 0x25
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xC;            //shr rax, 0x0C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        rdx += r11;             //add rdx, r11
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rdx ^= r11;             //xor rdx, r11
        rax = 0x6B65DF2C3A88AE69;               //mov rax, 0x6B65DF2C3A88AE69
        rdx -= rax;             //sub rdx, rax
        rax = 0xA4B331303E4E7A67;               //mov rax, 0xA4B331303E4E7A67
        rdx *= rax;             //imul rdx, rax
        rax = 0x6A137DDDFCE4C0D7;               //mov rax, 0x6A137DDDFCE4C0D7
        rdx -= rax;             //sub rdx, rax
        return rdx;
    }
    case 3:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);              //mov r9, [0x000000000A2D2679]
        r10 = base + 0x1314A155;            //lea r10, [0x00000000106836AD]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = base;                 //lea rax, [0xFFFFFFFFFD53946F]
        rdx -= rax;             //sub rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1F;           //shr rax, 0x1F
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x3E;           //shr rax, 0x3E
        rdx ^= rax;             //xor rdx, rax
        rdx ^= r11;             //xor rdx, r11
        rax = r10;              //mov rax, r10
        rax -= r11;             //sub rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = 0xD5A4D08183955257;               //mov rax, 0xD5A4D08183955257
        rdx *= rax;             //imul rdx, rax
        rax = 0x8DC8AE43913090FA;               //mov rax, 0x8DC8AE43913090FA
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 4:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2D2370]
        rax = 0xF7A45523CB2EF07F;               //mov rax, 0xF7A45523CB2EF07F
        rdx ^= rax;             //xor rdx, rax
        rdx -= r11;             //sub rdx, r11
        rdx -= r11;             //sub rdx, r11
        rdx -= r11;             //sub rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x14;           //shr rax, 0x14
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x28;           //shr rax, 0x28
        rdx ^= rax;             //xor rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = base;                 //lea rax, [0xFFFFFFFFFD538EA4]
        rax += 0x8F29;          //add rax, 0x8F29
        rax += r11;             //add rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = 0xD43375ADC5407E51;               //mov rax, 0xD43375ADC5407E51
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 5:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);              //mov r9, [0x000000000A2D1EB7]
        rax = 0x7C5DF0A12057BE6;                //mov rax, 0x7C5DF0A12057BE6
        rdx -= rax;             //sub rdx, rax
        rax = 0x83F8FC0408B5D1AB;               //mov rax, 0x83F8FC0408B5D1AB
        rdx ^= rax;             //xor rdx, rax
        rax = base + 0x158DE932;            //lea rax, [0x0000000012E1752A]
        rax = ~rax;             //not rax
        rax *= r11;             //imul rax, r11
        rdx += rax;             //add rdx, rax
        rax = r11;              //mov rax, r11
        uintptr_t RSP_0x98;
        RSP_0x98 = base + 0x12426297;               //lea rax, [0x000000000F95F02B] : RSP+0x98
        rax *= RSP_0x98;                //imul rax, [rsp+0x98]
        rdx ^= rax;             //xor rdx, rax
        rax = base;                 //lea rax, [0xFFFFFFFFFD538B37]
        rax += 0x5541;          //add rax, 0x5541
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x5;            //shr rax, 0x05
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xA;            //shr rax, 0x0A
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x14;           //shr rax, 0x14
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x28;           //shr rax, 0x28
        rdx ^= rax;             //xor rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = 0x3F31D045C89ED8C5;               //mov rax, 0x3F31D045C89ED8C5
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 6:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2D1914]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x6;            //shr rax, 0x06
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xC;            //shr rax, 0x0C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        rax = 0x7564570D161CA18D;               //mov rax, 0x7564570D161CA18D
        rdx *= rax;             //imul rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = base;                 //lea rax, [0xFFFFFFFFFD53857B]
        rax += 0x144205B7;              //add rax, 0x144205B7
        rax += r11;             //add rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = 0x80C6B6FC948F6729;               //mov rax, 0x80C6B6FC948F6729
        rdx *= rax;             //imul rdx, rax
        rax = 0x31DF20582505A415;               //mov rax, 0x31DF20582505A415
        rdx += rax;             //add rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = base + 0x6DA9DAD4;            //lea rax, [0x000000006AFD6203]
        rdx += rax;             //add rdx, rax
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rdx += rcx;             //add rdx, rcx
        return rdx;
    }
    case 7:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2D150F]
        rdx ^= r11;             //xor rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0xD;            //shr rax, 0x0D
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1A;           //shr rax, 0x1A
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x34;           //shr rax, 0x34
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x7;            //shr rax, 0x07
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xE;            //shr rax, 0x0E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1C;           //shr rax, 0x1C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x38;           //shr rax, 0x38
        rdx ^= rax;             //xor rdx, rax
        rax = 0xCCB3012D7BB7524F;               //mov rax, 0xCCB3012D7BB7524F
        rdx *= rax;             //imul rdx, rax
        rax = 0x11516F5E5F563F90;               //mov rax, 0x11516F5E5F563F90
        rdx -= rax;             //sub rdx, rax
        rax = r11;              //mov rax, r11
        rax = ~rax;             //not rax
        uintptr_t RSP_0x68;
        RSP_0x68 = base + 0x4354;           //lea rax, [0xFFFFFFFFFD53C753] : RSP+0x68
        rax *= RSP_0x68;                //imul rax, [rsp+0x68]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rdx += rax;             //add rdx, rax
        rcx ^= r10;             //xor rcx, r10
        rcx = _byteswap_uint64(rcx);            //bswap rcx
        rdx *= IO_READ_MEM<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
        return rdx;
    }
    case 8:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2D1068]
        rdx -= r11;             //sub rdx, r11
        rax = 0xEDC13D6B57B6E285;               //mov rax, 0xEDC13D6B57B6E285
        rdx *= rax;             //imul rdx, rax
        rax = r11;              //mov rax, r11
        uintptr_t RSP_0x98;
        RSP_0x98 = base + 0x7D814959;               //lea rax, [0x000000007AD4C88C] : RSP+0x98
        rax ^= RSP_0x98;                //xor rax, [rsp+0x98]
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = _byteswap_uint64(rcx);            //bswap rcx
        rdx *= IO_READ_MEM<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x21;           //shr rax, 0x21
        rdx ^= rax;             //xor rdx, rax
        rdx += r11;             //add rdx, r11
        rax = base;                 //lea rax, [0xFFFFFFFFFD537DE8]
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 9:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);              //mov r9, [0x000000000A2D0D18]
        rax = rdx;              //mov rax, rdx
        rax >>= 0xB;            //shr rax, 0x0B
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x16;           //shr rax, 0x16
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x2C;           //shr rax, 0x2C
        rdx ^= rax;             //xor rdx, rax
        rax = base;                 //lea rax, [0xFFFFFFFFFD5377CA]
        rdx ^= rax;             //xor rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = rdx;              //mov rax, rdx
        rax >>= 0xB;            //shr rax, 0x0B
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x16;           //shr rax, 0x16
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x2C;           //shr rax, 0x2C
        rdx ^= rax;             //xor rdx, rax
        rax = 0x53198E81F809E193;               //mov rax, 0x53198E81F809E193
        rax -= r11;             //sub rax, r11
        rax -= base;                //sub rax, [rsp+0xD0] -- didn't find trace -> use base
        rdx += rax;             //add rdx, rax
        rax = 0x5E76F88978AFE528;               //mov rax, 0x5E76F88978AFE528
        rdx += rax;             //add rdx, rax
        rax = 0x33557C5CEFBE234B;               //mov rax, 0x33557C5CEFBE234B
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 10:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2D07FD]
        rax = base;                 //lea rax, [0xFFFFFFFFFD53756B]
        rdx -= rax;             //sub rdx, rax
        rcx = rdx;              //mov rcx, rdx
        rcx >>= 0x27;           //shr rcx, 0x27
        rcx ^= rdx;             //xor rcx, rdx
        rdx = base + 0x9688;                //lea rdx, [0xFFFFFFFFFD540AE5]
        rdx *= r11;             //imul rdx, r11
        rdx += rcx;             //add rdx, rcx
        rax = rdx;              //mov rax, rdx
        rax >>= 0x13;           //shr rax, 0x13
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = 0x3331EF2FFF7DD801;               //mov rax, 0x3331EF2FFF7DD801
        rdx *= rax;             //imul rdx, rax
        rax = 0x2130706D6228E017;               //mov rax, 0x2130706D6228E017
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 11:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2D030A]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rax = base + 0x1E13DCA3;            //lea rax, [0x000000001B674AD7]
        rax = ~rax;             //not rax
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rax += r11;             //add rax, r11
        rcx ^= r10;             //xor rcx, r10
        rdx += rax;             //add rdx, rax
        rcx = _byteswap_uint64(rcx);            //bswap rcx
        rdx *= IO_READ_MEM<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
        rax = base;                 //lea rax, [0xFFFFFFFFFD536E06]
        rax += 0x9D9E;          //add rax, 0x9D9E
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rax = r11;              //mov rax, r11
        rax -= base;                //sub rax, [rsp+0xD0] -- didn't find trace -> use base
        rdx += rax;             //add rdx, rax
        rax = 0x10CE41F37EB30D3D;               //mov rax, 0x10CE41F37EB30D3D
        rdx *= rax;             //imul rdx, rax
        rax = 0x3078E9571E8D51B0;               //mov rax, 0x3078E9571E8D51B0
        rdx -= rax;             //sub rdx, rax
        rax = 0x13796DAAB7614CCB;               //mov rax, 0x13796DAAB7614CCB
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x11;           //shr rax, 0x11
        rdx ^= rax;             //xor rdx, rax
        rcx = base + 0x7C2A0085;            //lea rcx, [0x00000000797D6F4F]
        rcx = ~rcx;             //not rcx
        rcx *= r11;             //imul rcx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x22;           //shr rax, 0x22
        rcx ^= rax;             //xor rcx, rax
        rdx ^= rcx;             //xor rdx, rcx
        return rdx;
    }
    case 12:
    {
        r9 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);              //mov r9, [0x000000000A2CFEAF]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rdx -= r11;             //sub rdx, r11
        rax = 0x509EB372CDB3AEF3;               //mov rax, 0x509EB372CDB3AEF3
        rdx *= rax;             //imul rdx, rax
        rax = 0x3978478545AD0B0E;               //mov rax, 0x3978478545AD0B0E
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x11;           //shr rax, 0x11
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x22;           //shr rax, 0x22
        rdx ^= rax;             //xor rdx, rax
        rax = 0xF1F3C3AA4D6D1089;               //mov rax, 0xF1F3C3AA4D6D1089
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 13:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2CFA18]
        rax = rdx;              //mov rax, rdx
        rax >>= 0xE;            //shr rax, 0x0E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1C;           //shr rax, 0x1C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rax >>= 0x38;           //shr rax, 0x38
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rdx ^= rax;             //xor rdx, rax
        rcx ^= r10;             //xor rcx, r10
        rdx += r11;             //add rdx, r11
        rcx = _byteswap_uint64(rcx);            //bswap rcx
        rdx *= IO_READ_MEM<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
        rax = 0xEA5C4AF83EEC98D;                //mov rax, 0xEA5C4AF83EEC98D
        rdx += rax;             //add rdx, rax
        rax = base + 0x7FFA;                //lea rax, [0xFFFFFFFFFD53E72B]
        rax = ~rax;             //not rax
        rax ^= r11;             //xor rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = 0x58CCD785809FBA65;               //mov rax, 0x58CCD785809FBA65
        rdx *= rax;             //imul rdx, rax
        rax = 0xCB1252FFDD097729;               //mov rax, 0xCB1252FFDD097729
        rdx *= rax;             //imul rdx, rax
        rax = base;                 //lea rax, [0xFFFFFFFFFD5364C7]
        rax += 0x1CFB;          //add rax, 0x1CFB
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        return rdx;
    }
    case 14:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2CF45F]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1C;           //shr rax, 0x1C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x38;           //shr rax, 0x38
        rax ^= rdx;             //xor rax, rdx
        rdx = base;                 //lea rdx, [0xFFFFFFFFFD535FE9]
        rax -= rdx;             //sub rax, rdx
        rdx = r11 + 0xffffffffd3bc8867;                 //lea rdx, [r11-0x2C437799]
        rdx += rax;             //add rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = _byteswap_uint64(rax);            //bswap rax
        rdx *= IO_READ_MEM<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
        rax = base;                 //lea rax, [0xFFFFFFFFFD535EFA]
        rdx -= rax;             //sub rdx, rax
        rax = 0x311461FA31B150C8;               //mov rax, 0x311461FA31B150C8
        rdx ^= rax;             //xor rdx, rax
        rax = 0x3D057F65AC2E944D;               //mov rax, 0x3D057F65AC2E944D
        rdx += rax;             //add rdx, rax
        rax = 0x3EA61F392134306F;               //mov rax, 0x3EA61F392134306F
        rdx *= rax;             //imul rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x16;           //shr rax, 0x16
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x2C;           //shr rax, 0x2C
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 15:
    {
        r10 = IO_READ_MEM<uintptr_t>(base + 0xC9D74B9);             //mov r10, [0x000000000A2CEFCD]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x25;           //shr rax, 0x25
        rdx ^= rax;             //xor rdx, rax
        rdx -= r11;             //sub rdx, r11
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rax = base;                 //lea rax, [0xFFFFFFFFFD535C52]
        rax += 0x953;           //add rax, 0x953
        rax += r11;             //add rax, r11
        rcx = _byteswap_uint64(rcx);            //bswap rcx
        rdx ^= rax;             //xor rdx, rax
        rdx *= IO_READ_MEM<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
        rax = 0xEDC186E4F45D82CF;               //mov rax, 0xEDC186E4F45D82CF
        rdx *= rax;             //imul rdx, rax
        rdx -= r11;             //sub rdx, r11
        rax = base;                 //lea rax, [0xFFFFFFFFFD535B5A]
        rdx -= rax;             //sub rdx, rax
        rax = 0x579691DADE4159FD;               //mov rax, 0x579691DADE4159FD
        rdx *= rax;             //imul rdx, rax
        rax = 0x20B206512FA8AEE;                //mov rax, 0x20B206512FA8AEE
        rdx -= rax;             //sub rdx, rax
        rax = 0x804CFF40F9D9BEBF;               //mov rax, 0x804CFF40F9D9BEBF
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    }
}
uint16_t steam::get_bone_index(uint32_t bone_index)
{
    const uint64_t mb = base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rsi = bone_index;
    rcx = rsi * 0x13C8;
    rax = 0xD8CE01BF28E39A45;               //mov rax, 0xD8CE01BF28E39A45
    rax = _umul128(rax, rcx, (uintptr_t*)&rdx);             //mul rcx
    rdi = base;                 //lea rdi, [0xFFFFFFFFFD4C3F5A]
    r10 = 0xB225E47EA96E19B5;               //mov r10, 0xB225E47EA96E19B5
    rdx >>= 0xD;            //shr rdx, 0x0D
    rax = rdx * 0x25C9;             //imul rax, rdx, 0x25C9
    rcx -= rax;             //sub rcx, rax
    rax = 0xCEA41D97BF6494DF;               //mov rax, 0xCEA41D97BF6494DF
    r8 = rcx * 0x25C9;              //imul r8, rcx, 0x25C9
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rax = r8;               //mov rax, r8
    rax -= rdx;             //sub rax, rdx
    rax >>= 0x1;            //shr rax, 0x01
    rax += rdx;             //add rax, rdx
    rax >>= 0xE;            //shr rax, 0x0E
    rax = rax * 0x46D4;             //imul rax, rax, 0x46D4
    r8 -= rax;              //sub r8, rax
    rax = 0x526226F064679F75;               //mov rax, 0x526226F064679F75
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rax = 0xD79435E50D79435F;               //mov rax, 0xD79435E50D79435F
    rdx >>= 0x9;            //shr rdx, 0x09
    rcx = rdx * 0x637;              //imul rcx, rdx, 0x637
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rdx >>= 0x4;            //shr rdx, 0x04
    rcx += rdx;             //add rcx, rdx
    rax = rcx * 0x26;               //imul rax, rcx, 0x26
    rcx = r8 + r8 * 4;              //lea rcx, [r8+r8*4]
    rcx <<= 0x3;            //shl rcx, 0x03
    rcx -= rax;             //sub rcx, rax
    rax = IO_READ_MEM<uint16_t>(rcx + rdi * 1 + 0xB9152D0);                 //movzx eax, word ptr [rcx+rdi*1+0xBD077E0]
    r8 = rax * 0x13C8;              //imul r8, rax, 0x13C8
    rax = r10;              //mov rax, r10
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rcx = r8;               //mov rcx, r8
    rax = r10;              //mov rax, r10
    rcx -= rdx;             //sub rcx, rdx
    rcx >>= 0x1;            //shr rcx, 0x01
    rcx += rdx;             //add rcx, rdx
    rcx >>= 0xD;            //shr rcx, 0x0D
    rcx = rcx * 0x25BD;             //imul rcx, rcx, 0x25BD
    r8 -= rcx;              //sub r8, rcx
    r9 = r8 * 0x319C;               //imul r9, r8, 0x319C
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rax = r9;               //mov rax, r9
    rax -= rdx;             //sub rax, rdx
    rax >>= 0x1;            //shr rax, 0x01
    rax += rdx;             //add rax, rdx
    rax >>= 0xD;            //shr rax, 0x0D
    rax = rax * 0x25BD;             //imul rax, rax, 0x25BD
    r9 -= rax;              //sub r9, rax
    rax = 0x842108421084211;                //mov rax, 0x842108421084211
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rax = r9;               //mov rax, r9
    rax -= rdx;             //sub rax, rdx
    rax >>= 0x1;            //shr rax, 0x01
    rax += rdx;             //add rax, rdx
    rax >>= 0xA;            //shr rax, 0x0A
    rcx = rax * 0x7C0;              //imul rcx, rax, 0x7C0
    rax = 0xE38E38E38E38E38F;               //mov rax, 0xE38E38E38E38E38F
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rdx >>= 0x3;            //shr rdx, 0x03
    rcx += rdx;             //add rcx, rdx
    rax = rcx + rcx * 8;            //lea rax, [rcx+rcx*8]
    rax += rax;             //add rax, rax
    rcx = r9 + r9 * 4;              //lea rcx, [r9+r9*4]
    rcx <<= 0x2;            //shl rcx, 0x02
    rcx -= rax;             //sub rcx, rax
    r12 = IO_READ_MEM<uint16_t>(rcx + rdi * 1 + 0xB923F00);                 //movsx r12d, word ptr [rcx+rdi*1+0xBD16410]
    return r12;
}


auto steam::retrieve_name_list() -> uint64_t {
    auto ptr = mem.Read<uint64_t>(Addresses::base + steam::name_array);
    if (!ptr)
        return 0;
    return ptr + 0x1838;
}

auto steam::get_name_entry(uint32_t p_index) -> NameEntry {
    auto name = mem.Read<NameEntry>(steam::retrieve_name_list() + (p_index * steam::name_array_size));
    return name;
}
