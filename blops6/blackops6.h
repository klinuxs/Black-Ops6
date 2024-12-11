#include "Helper.h"
#include "../Memory/Memory.h"
#include "Addresses.h"

struct NameEntry
{
    int idx = 0;
    char name[0x24] = "NONE";
    char tag[0x40] = "NONE";
    char unk1[0x1C];
    int health;
    char unk2[0x38];
};

// will need to convert decryption on xbox namespace to the same way as steam
// otherwise, you will get a generic c++ error and pull your hair out (my personal experience)
//https://github.com/c-metaphor/White-Ops-6/tree/main
//https://www.unknowncheats.me/forum/call-of-duty-black-ops-6-a/653959-call-duty-black-ops-6-reversal-structs-offsets-27.html
namespace xbox
{
    inline uint64_t ref_def_ptr = 0x1386DB70;           // 33 05 ?? ?? ?? ?? 89 44 24 20 8D 41 02 0F AF C1 33 05
    inline uint64_t name_array = 0x13A48598;
    inline uint64_t name_array_pos = 0x1838;
    inline uint64_t name_array_size = 0xC8;
    inline uint64_t camera_base = 0x139DA870;           // 48 8B 05 ? ? ? ? 48 8B 7C 24 ? 48 05 ? ? ? ? 48 69 CA ? ? ? ? 48 03 C1 C3 On bottom you an find camera_pos add rax, 1F4h
    inline uint64_t camera_pos = 0x1F4;
    inline uint64_t local_index = 0x4E020;
    inline uint64_t local_index_pos = 0x33C;
    inline uint64_t game_mode = 0xCA55428;
    inline uint64_t visible_client_bits = 0x11B2DC;     // 41 21 84 ?? ?? ?? ?? ?? 41 83 ??

    inline uint32_t o_entity_size = 0x418;
    inline uint32_t o_entity_type = 0x23C;
    inline uint32_t o_entity_index = 0x234;
    inline uint32_t o_entity_valid = 0x224;
    inline uint32_t o_entity_pos = o_entity_type + 0x18;
    inline uint32_t o_entity_flags = 0xF4;

    inline uint32_t o_no_recoil = 0x17DBA4;
    inline uint32_t o_weapon_definition = 0x13D6E8D0;

    inline uint32_t o_visible_size = 0x1D4C;
    inline uint32_t o_visible_pointer = 0x1F8;
    inline uint32_t o_visible_cached_base = 0x78;
    inline uint32_t o_visible_cmp_index = 0x1EE;
    inline uint32_t o_visible_cmp_function = 0xD532AE0;
    inline uint32_t o_visible_cmp_value = 0x29;

    inline uint32_t o_item_base = 0xE306430;

    inline uint32_t o_item_pos = 0x1D0;
    inline uint32_t o_item_size = 0x338;
    inline uint32_t o_item_valid_1 = 0xC8;
    inline uint32_t o_item_valid_2 = o_item_valid_1 + 0x2;


    inline uint32_t o_visible_bit = 0x11B2DC;
    inline uint32_t o_local_entity = 0x760;

    namespace bone {
        constexpr auto bone_base = 0x61168; // B8 ?? ?? ?? ?? C5 4A 58 04 10  mov eax, 76628h
        constexpr auto size = 0x1A8;
        constexpr auto offset = 0xD8;
    };


    // E8 ?? ?? ?? ?? 8B C7 48 8D 1D
    namespace player {
        constexpr auto size = 0x1DF8;   // 41 B8 ?? ?? ?? ?? 48 8B C8 48 8B F8 48 8B 98
        constexpr auto valid = 0x17D0;  // 41 C6 85 61 01 00 00 01
        constexpr auto pos = 0x8D0;     // 41 80 ?? ?? ?? ?? ?? ?? 74 ?? 48 85 ?? 74 ?? 49
        constexpr auto team = 0x8C2;
        constexpr auto stance = 0x1004;
        constexpr auto dead_1 = 0x918;
        constexpr auto dead_2 = 0x164;
        inline uint32_t o_player_stance = 0x753;
        inline  uint32_t o_player_weapon_index = 0x13892;
    };


    inline uintptr_t decrypt_client_info()
    {
        const uint64_t mb = Addresses::base;
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rbx = mem.Read<uintptr_t>(Addresses::base + 0x13795E58);
        if (!rbx)
            return rbx;
        rcx = mem.GetProcessPebAddress();          //mov rcx, gs:[rax]
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
        rax ^= mem.Read<uintptr_t>(Addresses::base + 0xC7F05FC);            //xor rax, [0x0000000009D6C458]
        rax = _byteswap_uint64(rax);            //bswap rax
        rbx *= mem.Read<uintptr_t>(rax + 0x15);              //imul rbx, [rax+0x15]
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
    inline uintptr_t decrypt_client_base(uint64_t client_info)
    {
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rax = mem.Read<uintptr_t>(client_info + 0x1dc688);
        if (!rax)
            return rax;
        r11 = ~mem.GetProcessPebAddress();          //mov r11, gs:[rcx]
        rcx = r11;              //mov rcx, r11
        //failed to translate: mov [rsp+0x3E0], r13
        rcx = _rotl64(rcx, 0x34);               //rol rcx, 0x34
        rcx &= 0xF;
        switch (rcx) {
        case 0:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F73A5]
            r13 = Addresses::base + 0x16CBED5E;            //lea r13, [0x0000000013CB0AFB]
            rdx = Addresses::base + 0x33D36DEA;            //lea rdx, [0x0000000030D28B29]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0x31EA6C7327F2F48F;               //mov rcx, 0x31EA6C7327F2F48F
            rax *= rcx;             //imul rax, rcx
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            uintptr_t RSP_0xFFFFFFFFFFFFFF90;
            RSP_0xFFFFFFFFFFFFFF90 = Addresses::base + 0xDD05;             //lea rcx, [0xFFFFFFFFFCFFFABE] : RBP+0xFFFFFFFFFFFFFF90
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
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF1B61]
            rax -= rcx;             //sub rax, rcx
            return rax;
        }
        case 1:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F6EC3]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF149B]
            rcx += 0x4C577047;              //add rcx, 0x4C577047
            rcx += r11;             //add rcx, r11
            rax ^= rcx;             //xor rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x16;           //shr rcx, 0x16
            rax ^= rcx;             //xor rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x2C;           //shr rcx, 0x2C
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF15A1]
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF166C]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            return rax;
        }
        case 2:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F6A4D]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = Addresses::base + 0x40BA9587;            //lea rcx, [0x000000003DB9A889]
            rax += r11;             //add rax, r11
            rax += rcx;             //add rax, rcx
            rcx = 0xB820512AEEAE20F1;               //mov rcx, 0xB820512AEEAE20F1
            rax *= rcx;             //imul rax, rcx
            rcx = 0x5B4702F448FEE148;               //mov rcx, 0x5B4702F448FEE148
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF12DA]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F6501]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF0C2E]
            rax -= rcx;             //sub rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF0AAF]
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
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            uintptr_t RSP_0xFFFFFFFFFFFFFF88;
            RSP_0xFFFFFFFFFFFFFF88 = Addresses::base + 0x639AA956;                 //lea rcx, [0x000000006099B826] : RBP+0xFFFFFFFFFFFFFF88
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
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F6085]
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
            rax -= Addresses::base;                //sub rax, [rbp-0x70] -- didn't find trace -> use Addresses::base
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r9;              //xor rcx, r9
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F597F]
            rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
            rcx = r11 * 0xFFFFFFFFFFFFFFFE;                 //imul rcx, r11, 0xFFFFFFFFFFFFFFFE
            rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
            rax += rcx;             //add rax, rcx
            rdx ^= r10;             //xor rdx, r10
            rcx = Addresses::base + 0x5F737FAA;            //lea rcx, [0x000000005C7280B8]
            rdx = ~rdx;             //not rdx
            rax += rcx;             //add rax, rcx
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x19;           //shr rcx, 0x19
            rax ^= rcx;             //xor rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x32;           //shr rcx, 0x32
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEFF90]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F5513]
            rdx = Addresses::base + 0x50AF1F4F;            //lea rdx, [0x000000004DAE1DF5]
            rax += r11;             //add rax, r11
            rcx = r11;              //mov rcx, r11
            rcx ^= rdx;             //xor rcx, rdx
            rax -= rcx;             //sub rax, rcx
            rcx = 0xFA37AFACEF63040B;               //mov rcx, 0xFA37AFACEF63040B
            rax ^= rcx;             //xor rax, rcx
            rcx = 0xE15A50F0F0B4D5D9;               //mov rcx, 0xE15A50F0F0B4D5D9
            rax *= rcx;             //imul rax, rcx
            rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
            rcx = Addresses::base + 0xEE8B;                //lea rcx, [0xFFFFFFFFFCFFE99B]
            rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
            rcx -= r11;             //sub rcx, r11
            rax ^= rcx;             //xor rax, rcx
            rdx ^= r10;             //xor rdx, r10
            rdx = ~rdx;             //not rdx
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F4FFD]
            r13 = Addresses::base + 0xBEE4;                //lea r13, [0xFFFFFFFFFCFFB8E4]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEF69A]
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
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rax *= rcx;             //imul rax, rcx
            rcx = r11;              //mov rcx, r11
            rcx -= r13;             //sub rcx, r13
            rax ^= rcx;             //xor rax, rcx
            rax -= r11;             //sub rax, r11
            return rax;
        }
        case 8:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F4BAE]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEF0E3]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F44EB]
            rdx = r11;              //mov rdx, r11
            rcx = Addresses::base + 0xFCE2;                //lea rcx, [0xFFFFFFFFFCFFEA2E]
            rdx *= rcx;             //imul rdx, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEED41]
            rdx -= rcx;             //sub rdx, rcx
            rcx = r11;              //mov rcx, r11
            rax += rdx;             //add rax, rdx
            rcx = ~rcx;             //not rcx
            rax ^= rcx;             //xor rax, rcx
            rdx = Addresses::base + 0x6EC75C18;            //lea rdx, [0x000000006BC64943]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0x272542A0099256AB;               //mov rcx, 0x272542A0099256AB
            rax *= rcx;             //imul rax, rcx
            return rax;
        }
        case 10:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3FB9]
            rcx = Addresses::base + 0x240D8AB0;            //lea rcx, [0x00000000210C731B]
            rcx += r11;             //add rcx, r11
            rax ^= rcx;             //xor rax, rcx
            rdx = Addresses::base + 0x3B21;                //lea rdx, [0xFFFFFFFFFCFF213E]
            rcx = r11;              //mov rcx, r11
            rdx *= r11;             //imul rdx, r11
            rcx = ~rcx;             //not rcx
            rdx += rax;             //add rdx, rax
            rax = Addresses::base + 0x24E689C1;            //lea rax, [0x0000000021E56FBA]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0xBDB64B81FECB6E7D;               //mov rcx, 0xBDB64B81FECB6E7D
            rax *= rcx;             //imul rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x28;           //shr rcx, 0x28
            rax ^= rcx;             //xor rax, rcx
            return rax;
        }
        case 11:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3AD2]
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
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEDFD6]
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
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEE374]
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base + 0x5B0222F4;            //lea rcx, [0x0000000058010275]
            rcx *= r11;             //imul rcx, r11
            rax += rcx;             //add rax, rcx
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            return rax;
        }
        case 12:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3553]
            r13 = Addresses::base + 0x852;                 //lea r13, [0xFFFFFFFFFCFEE7AD]
            rcx = 0xF7D4FABCE6FC022;                //mov rcx, 0xF7D4FABCE6FC022
            rax += rcx;             //add rax, rcx
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEDD06]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3183]
            r13 = Addresses::base + 0x12585A59;            //lea r13, [0x000000000F5735E4]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0xA4979265BBC7E3D5;               //mov rcx, 0xA4979265BBC7E3D5
            rax *= rcx;             //imul rax, rcx
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            rcx -= Addresses::base;                //sub rcx, [rbp-0x70] -- didn't find trace -> use Addresses::base
            rcx -= 0x756FAD6E;              //sub rcx, 0x756FAD6E
            rcx ^= rax;             //xor rcx, rax
            rax = Addresses::base + 0x78217335;            //lea rax, [0x0000000075204CE2]
            rcx += r11;             //add rcx, r11
            rax += rcx;             //add rax, rcx
            rcx = 0xC20F4E2AD24BC517;               //mov rcx, 0xC20F4E2AD24BC517
            rax ^= rcx;             //xor rax, rcx
            return rax;
        }
        case 14:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F2C05]
            r13 = Addresses::base + 0x3CF1;                //lea r13, [0xFFFFFFFFFCFF12FE]
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
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rax *= rcx;             //imul rax, rcx
            rcx = 0x56A40B352BF2FDB7;               //mov rcx, 0x56A40B352BF2FDB7
            rax -= rcx;             //sub rax, rcx
            rcx = Addresses::base + 0x32FFEB8B;            //lea rcx, [0x000000002FFEC0B1]
            rcx = ~rcx;             //not rcx
            rcx *= r11;             //imul rcx, r11
            rax += rcx;             //add rax, rcx
            return rax;
        }
        case 15:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F26AC]
            rdx = Addresses::base + 0x23AC2B6F;            //lea rdx, [0x0000000020AAFB7C]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFECE86]
            rax -= rcx;             //sub rax, rcx
            rax += r11;             //add rax, r11
            rdx = ~rdx;             //not rdx
            rdx ^= r11;             //xor rdx, r11
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
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
    inline uintptr_t decrypt_bone_base()
    {
        const uint64_t mb = Addresses::base;
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rdx = mem.Read<uintptr_t>(Addresses::base + 0x114527F8);
        if (!rdx)
            return rdx;
        r11 = mem.GetProcessPebAddress();          //mov r11, gs:[rax]
        rax = r11;              //mov rax, r11
        rax <<= 0x22;           //shl rax, 0x22
        rax = _byteswap_uint64(rax);            //bswap rax
        rax &= 0xF;
        switch (rax) {
        case 0:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEFD8E]
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5EA408]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEF8C8]
            rax = 0x311E3C7DD1297B69;               //mov rax, 0x311E3C7DD1297B69
            rdx *= rax;             //imul rdx, rax
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r10;             //xor rax, r10
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base + 0x437D3D7F;            //lea rax, [0x0000000040DBDDDE]
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
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5EA029]
            rax += 0x3B261317;              //add rax, 0x3B261317
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            rax = 0x85B82AEE944DCF96;               //mov rax, 0x85B82AEE944DCF96
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5EA12E]
            rdx += rax;             //add rdx, rax
            return rdx;
        }
        case 2:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEF41F]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
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
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEEFE3]
            r10 = Addresses::base + 0x1314A155;            //lea r10, [0x0000000010733A67]
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E9833]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEECEA]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E9296]
            rax += 0x8F29;          //add rax, 0x8F29
            rax += r11;             //add rax, r11
            rdx ^= rax;             //xor rdx, rax
            rax = 0xD43375ADC5407E51;               //mov rax, 0xD43375ADC5407E51
            rdx *= rax;             //imul rdx, rax
            return rdx;
        }
        case 5:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEE866]
            r13 = Addresses::base + 0x12426297;            //lea r13, [0x000000000FA0F425]
            rax = 0x7C5DF0A12057BE6;                //mov rax, 0x7C5DF0A12057BE6
            rdx -= rax;             //sub rdx, rax
            rax = 0x83F8FC0408B5D1AB;               //mov rax, 0x83F8FC0408B5D1AB
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base + 0x158DE932;            //lea rax, [0x0000000012EC7940]
            rax = ~rax;             //not rax
            rax *= r11;             //imul rax, r11
            rdx += rax;             //add rdx, rax
            rax = r11;              //mov rax, r11
            rax *= r13;             //imul rax, r13
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E8F4D]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = 0x3F31D045C89ED8C5;               //mov rax, 0x3F31D045C89ED8C5
            rdx *= rax;             //imul rdx, rax
            return rdx;
        }
        case 6:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEE2F9]
            r13 = Addresses::base + 0x6DA9DAD4;            //lea r13, [0x000000006B0866FC]
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
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E89C5]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rdx += r13;             //add rdx, r13
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            rdx += rcx;             //add rdx, rcx
            return rdx;
        }
        case 7:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEDF15]
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
            uintptr_t RSP_0x38;
            RSP_0x38 = Addresses::base + 0x4354;           //lea rax, [0xFFFFFFFFFD5ECBA9] : RSP+0x38
            rax *= RSP_0x38;                //imul rax, [rsp+0x38]
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rdx += rax;             //add rdx, rax
            rcx ^= r10;             //xor rcx, r10
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            return rdx;
        }
        case 8:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEDA88]
            r13 = Addresses::base + 0x7D814959;            //lea r13, [0x000000007ADFCD15]
            rdx -= r11;             //sub rdx, r11
            rax = 0xEDC13D6B57B6E285;               //mov rax, 0xEDC13D6B57B6E285
            rdx *= rax;             //imul rdx, rax
            rax = r11;              //mov rax, r11
            rax ^= r13;             //xor rax, r13
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = rdx;              //mov rax, rdx
            rax >>= 0x21;           //shr rax, 0x21
            rdx ^= rax;             //xor rdx, rax
            rdx += r11;             //add rdx, r11
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E8268]
            rdx ^= rax;             //xor rdx, rax
            return rdx;
        }
        case 9:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DED75A]
            rax = rdx;              //mov rax, rdx
            rax >>= 0xB;            //shr rax, 0x0B
            rdx ^= rax;             //xor rdx, rax
            rax = rdx;              //mov rax, rdx
            rax >>= 0x16;           //shr rax, 0x16
            rdx ^= rax;             //xor rdx, rax
            rax = rdx;              //mov rax, rdx
            rax >>= 0x2C;           //shr rax, 0x2C
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E7C74]
            rdx ^= rax;             //xor rdx, rax
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
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
            rax -= Addresses::base;                //sub rax, [rbp-0x59] -- didn't find trace -> use Addresses::base
            rdx += rax;             //add rdx, rax
            rax = 0x5E76F88978AFE528;               //mov rax, 0x5E76F88978AFE528
            rdx += rax;             //add rdx, rax
            rax = 0x33557C5CEFBE234B;               //mov rax, 0x33557C5CEFBE234B
            rdx *= rax;             //imul rdx, rax
            return rdx;
        }
        case 10:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DED25F]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E7A42]
            rdx -= rax;             //sub rdx, rax
            rcx = rdx;              //mov rcx, rdx
            rcx >>= 0x27;           //shr rcx, 0x27
            rcx ^= rdx;             //xor rcx, rdx
            rdx = Addresses::base + 0x9688;                //lea rdx, [0xFFFFFFFFFD5F0FC0]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = 0x3331EF2FFF7DD801;               //mov rax, 0x3331EF2FFF7DD801
            rdx *= rax;             //imul rdx, rax
            rax = 0x2130706D6228E017;               //mov rax, 0x2130706D6228E017
            rdx ^= rax;             //xor rdx, rax
            return rdx;
        }
        case 11:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DECDA3]
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rax = Addresses::base + 0x1E13DCA3;            //lea rax, [0x000000001B724FDA]
            rax = ~rax;             //not rax
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rax += r11;             //add rax, r11
            rcx ^= r10;             //xor rcx, r10
            rdx += rax;             //add rdx, rax
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E7309]
            rax += 0x9D9E;          //add rax, 0x9D9E
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            rax = r11;              //mov rax, r11
            rax -= Addresses::base;                //sub rax, [rbp-0x59] -- didn't find trace -> use Addresses::base
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
            rcx = Addresses::base + 0x7C2A0085;            //lea rcx, [0x000000007988744D]
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
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEC962]
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEC4E3]
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
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = 0xEA5C4AF83EEC98D;                //mov rax, 0xEA5C4AF83EEC98D
            rdx += rax;             //add rdx, rax
            rax = Addresses::base + 0x7FFA;                //lea rax, [0xFFFFFFFFFD5EEC64]
            rax = ~rax;             //not rax
            rax ^= r11;             //xor rax, r11
            rdx ^= rax;             //xor rdx, rax
            rax = 0x58CCD785809FBA65;               //mov rax, 0x58CCD785809FBA65
            rdx *= rax;             //imul rdx, rax
            rax = 0xCB1252FFDD097729;               //mov rax, 0xCB1252FFDD097729
            rdx *= rax;             //imul rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E6A0E]
            rax += 0x1CFB;          //add rax, 0x1CFB
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            return rdx;
        }
        case 14:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEBF63]
            rax = rdx;              //mov rax, rdx
            rax >>= 0x1C;           //shr rax, 0x1C
            rdx ^= rax;             //xor rdx, rax
            rax = rdx;              //mov rax, rdx
            rax >>= 0x38;           //shr rax, 0x38
            rax ^= rdx;             //xor rax, rdx
            rdx = Addresses::base;                 //lea rdx, [0xFFFFFFFFFD5E6554]
            rax -= rdx;             //sub rax, rdx
            rdx = r11 + 0xffffffffd3bc8867;                 //lea rdx, [r11-0x2C437799]
            rdx += rax;             //add rdx, rax
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r10;             //xor rax, r10
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E646F]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEBAF2]
            rax = rdx;              //mov rax, rdx
            rax >>= 0x25;           //shr rax, 0x25
            rdx ^= rax;             //xor rdx, rax
            rdx -= r11;             //sub rdx, r11
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E61D8]
            rax += 0x953;           //add rax, 0x953
            rax += r11;             //add rax, r11
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx ^= rax;             //xor rdx, rax
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = 0xEDC186E4F45D82CF;               //mov rax, 0xEDC186E4F45D82CF
            rdx *= rax;             //imul rdx, rax
            rdx -= r11;             //sub rdx, r11
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E60E4]
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
    inline uint16_t get_bone_index(uint32_t bone_index)
    {
        const uint64_t mb = Addresses::base;
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rsi = bone_index;
        rcx = rsi * 0x13C8;
        rax = 0xD8CE01BF28E39A45;               //mov rax, 0xD8CE01BF28E39A45
        rax = _umul128(rax, rcx, (uintptr_t*)&rdx);             //mul rcx
        rdi = Addresses::base;                 //lea rdi, [0xFFFFFFFFFD5F51DA]
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
        rax = mem.Read<uint16_t>(rcx + rdi * 1 + 0xB5D2010);                 //movzx eax, word ptr [rcx+rdi*1+0xB5E22A0]
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
        r12 = mem.Read<uint16_t>(rcx + rdi * 1 + 0xB5E0C40);                 //movsx r12d, word ptr [rcx+rdi*1+0xB5F0ED0]
        return r12;
    }

    class refdef_class {
    public:
        auto retrieve_ref_def() -> uintptr_t
        {
            key encrypted = mem.Read<key>(Addresses::base + xbox::ref_def_ptr);
            DWORD lowerref = encrypted.ref0 ^ (encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr)) * ((encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr)) + 2);
            DWORD upperref = encrypted.ref1 ^ (encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr + 0x4)) * ((encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr + 0x4)) + 2); \
                return (uint64_t)upperref << 32 | lowerref;
        }
        ref_def_t ref_def_nn;
    };
    static refdef_class* decrypt_refdef = new refdef_class();

}
namespace steam
{
    
    inline uint64_t ref_def_ptr = 0x136F5A38;
    inline uint64_t name_array = 0x138B4D48;
    inline uint64_t loot_ptr = 0xDFDB360;
    inline uint64_t camera_base = 0x13871160;
    inline uint64_t local_index = 0x1B5C0;
    inline uint64_t game_mode = 0xCC43C98;
    inline uint64_t weapon_definitions = 0x1374F330;
    inline uint64_t distribute = 0x100AF3E0;
    inline uint64_t visible_client_bits = 0x101E68;     // 41 21 84 ?? ?? ?? ?? ?? 41 83 ??
    // will very rarely change, maybe once every 20-40 updates
    inline uint64_t camera_pos = 0x1F4;
    inline uint64_t local_index_pos = 0x344;
    inline uint64_t name_array_pos = 0x1838;
    inline uint64_t name_array_size = 0xC8;
    // will need to get the rest of these from uc thread, they are not posted in white-ops.
    inline uint32_t o_entity_size = 0x418;
    inline uint32_t o_entity_type = 0x23C;
    inline uint32_t o_entity_index = 0x234;
    inline uint32_t o_entity_valid = 0x224;
    inline uint32_t o_entity_pos = o_entity_type + 0x18;
    inline uint32_t o_entity_flags = 0xF4;

    inline uint32_t o_no_recoil = 0x15080C;
    inline uint32_t o_weapon_definition = 0x138180A0;

    inline uint32_t o_visible_size = 0x1D4C;
    inline uint32_t o_visible_pointer = 0x1F8;
    inline uint32_t o_visible_cached_base = 0x78;
    inline uint32_t o_visible_cmp_index = 0x1EE;
    inline uint32_t o_visible_cmp_function = 0xD532AE0;
    inline uint32_t o_visible_cmp_value = 0x29;

    inline uint32_t o_item_base = 0xDE24A70;

    inline uint32_t o_item_pos = 0x1D0;
    inline uint32_t o_item_size = 0x338;
    inline uint32_t o_item_valid_1 = 0xC8;
    inline uint32_t o_item_valid_2 = o_item_valid_1 + 0x2;


    inline uint32_t o_visible_bit = 0x1507E0;
    inline uint32_t o_local_entity = 0x1507E0;

    namespace bone {
        inline uint32_t bone_base = 0x8B640; // B8 ?? ?? ?? ?? C5 4A 58 04 10  mov eax, 76628h
        inline uint32_t size = 0x1A8;
        inline uint32_t offset = 0xD8;
    };


    // E8 ?? ?? ?? ?? 8B C7 48 8D 1D
    namespace player {
        inline uint32_t size = 0x1E68;
        inline uint32_t valid = 0x1124;
        inline uint32_t pos = 0x12C0;
        inline uint32_t team = 0x1D80;
        inline uint32_t stance = 0x1409;
        inline uint32_t weapon_index = 0x5D0;
        inline uint32_t dead_1 = 0x12FC;
        inline uint32_t dead_2 = 0x5C0;
        inline uint32_t o_player_stance = 0x753;
        inline uint32_t o_player_weapon_index = 0x19B0;
    };

    // all these are defined in blackops6.cpp, steam::decryptbullshit
    // would need to repeat for xbox and bnet
    uintptr_t decrypt_client_info();
    uintptr_t decrypt_client_base(uintptr_t client_info);
    uintptr_t decrypt_bone_base();
    uint16_t get_bone_index(uint32_t bone_index);
    uint64_t retrieve_name_list();
    NameEntry get_name_entry(uint32_t p_index);
    class refdef_class {
    public:
        auto retrieve_ref_def() -> uintptr_t
        {
            key encrypted = mem.Read<key>(Addresses::base + steam::ref_def_ptr);
            DWORD lowerref = encrypted.ref0 ^ (encrypted.ref2 ^ (uint64_t)(Addresses::base + steam::ref_def_ptr)) * ((encrypted.ref2 ^ (uint64_t)(Addresses::base + steam::ref_def_ptr)) + 2);
            DWORD upperref = encrypted.ref1 ^ (encrypted.ref2 ^ (uint64_t)(Addresses::base + steam::ref_def_ptr + 0x4)) * ((encrypted.ref2 ^ (uint64_t)(Addresses::base + steam::ref_def_ptr + 0x4)) + 2); \
                return (uint64_t)upperref << 32 | lowerref;
        }
        ref_def_t ref_def_nn;
    };
    static refdef_class* decrypt_refdef = new refdef_class();

}
namespace bnet
{
    inline uint64_t ref_def_ptr = 0x13CECA78;           // 33 05 ?? ?? ?? ?? 89 44 24 20 8D 41 02 0F AF C1 33 05
    inline uint64_t name_array = 0x13EAD068;
    inline uint64_t name_array_pos = 0x1838;
    inline uint64_t name_array_size = 0xC8;
    inline uint64_t camera_base = 0x13E57BA0;           // 48 8B 05 ? ? ? ? 48 8B 7C 24 ? 48 05 ? ? ? ? 48 69 CA ? ? ? ? 48 03 C1 C3 On bottom you an find camera_pos add rax, 1F4h
    inline uint64_t camera_pos = 0x1F4;
    inline uint64_t local_index = 0x82A70;
    inline uint64_t local_index_pos = 0x33C;
    inline uint64_t game_mode = 0xCEAD048;
    inline uint64_t visible_client_bits = 0x133910;     // 41 21 84 ?? ?? ?? ?? ?? 41 83 ??

    inline uint32_t o_entity_size = 0x418;
    inline uint32_t o_entity_type = 0x23C;
    inline uint32_t o_entity_index = 0x234;
    inline uint32_t o_entity_valid = 0x224;
    inline uint32_t o_entity_pos = o_entity_type + 0x18;
    inline uint32_t o_entity_flags = 0xF4;

    inline uint32_t o_no_recoil = 0x17DBA4;
    inline uint32_t o_weapon_definition = 0x13D6E8D0;

    inline uint32_t o_visible_size = 0x1D4C;
    inline uint32_t o_visible_pointer = 0x1F8;
    inline uint32_t o_visible_cached_base = 0x78;
    inline uint32_t o_visible_cmp_index = 0x1EE;
    inline uint32_t o_visible_cmp_function = 0xD532AE0;
    inline uint32_t o_visible_cmp_value = 0x29;

    inline uint32_t o_item_base = 0xE306430;

    inline uint32_t o_item_pos = 0x1D0;
    inline uint32_t o_item_size = 0x338;
    inline uint32_t o_item_valid_1 = 0xC8;
    inline uint32_t o_item_valid_2 = o_item_valid_1 + 0x2;


    inline uint32_t o_visible_bit = 0x11B2DC;
    inline uint32_t o_local_entity = 0x760;

    namespace bone {
        constexpr auto bone_base = 0x76628; // B8 ?? ?? ?? ?? C5 4A 58 04 10  mov eax, 76628h
        constexpr auto size = 0x1A8;
        constexpr auto offset = 0xD8;
    };


    // E8 ?? ?? ?? ?? 8B C7 48 8D 1D
    namespace player {
        constexpr auto size = 0x1DF8;   // 41 B8 ?? ?? ?? ?? 48 8B C8 48 8B F8 48 8B 98
        constexpr auto valid = 0x770;  // 41 C6 85 61 01 00 00 01
        constexpr auto pos = 0x748;     // 41 80 ?? ?? ?? ?? ?? ?? 74 ?? 48 85 ?? 74 ?? 49
        constexpr auto team = 0xF68;
        constexpr auto stance = 0x1360;
        constexpr auto dead_1 = 0x1134;
        constexpr auto dead_2 = 0x1C52;
        inline uint32_t o_player_stance = 0x753;
        inline  uint32_t o_player_weapon_index = 0x13892;
    };


    inline uintptr_t decrypt_client_info()
    {
        const uint64_t mb = Addresses::base;
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rbx = mem.Read<uintptr_t>(Addresses::base + 0x13795E58);
        if (!rbx)
            return rbx;
        rcx = mem.GetProcessPebAddress();          //mov rcx, gs:[rax]
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
        rax ^= mem.Read<uintptr_t>(Addresses::base + 0xC7F05FC);            //xor rax, [0x0000000009D6C458]
        rax = _byteswap_uint64(rax);            //bswap rax
        rbx *= mem.Read<uintptr_t>(rax + 0x15);              //imul rbx, [rax+0x15]
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
    inline uintptr_t decrypt_client_base(uint64_t client_info)
    {
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rax = mem.Read<uintptr_t>(client_info + 0x1dc688);
        if (!rax)
            return rax;
        r11 = ~mem.GetProcessPebAddress();          //mov r11, gs:[rcx]
        rcx = r11;              //mov rcx, r11
        //failed to translate: mov [rsp+0x3E0], r13
        rcx = _rotl64(rcx, 0x34);               //rol rcx, 0x34
        rcx &= 0xF;
        switch (rcx) {
        case 0:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F73A5]
            r13 = Addresses::base + 0x16CBED5E;            //lea r13, [0x0000000013CB0AFB]
            rdx = Addresses::base + 0x33D36DEA;            //lea rdx, [0x0000000030D28B29]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0x31EA6C7327F2F48F;               //mov rcx, 0x31EA6C7327F2F48F
            rax *= rcx;             //imul rax, rcx
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            uintptr_t RSP_0xFFFFFFFFFFFFFF90;
            RSP_0xFFFFFFFFFFFFFF90 = Addresses::base + 0xDD05;             //lea rcx, [0xFFFFFFFFFCFFFABE] : RBP+0xFFFFFFFFFFFFFF90
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
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF1B61]
            rax -= rcx;             //sub rax, rcx
            return rax;
        }
        case 1:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F6EC3]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF149B]
            rcx += 0x4C577047;              //add rcx, 0x4C577047
            rcx += r11;             //add rcx, r11
            rax ^= rcx;             //xor rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x16;           //shr rcx, 0x16
            rax ^= rcx;             //xor rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x2C;           //shr rcx, 0x2C
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF15A1]
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF166C]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            return rax;
        }
        case 2:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F6A4D]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = Addresses::base + 0x40BA9587;            //lea rcx, [0x000000003DB9A889]
            rax += r11;             //add rax, r11
            rax += rcx;             //add rax, rcx
            rcx = 0xB820512AEEAE20F1;               //mov rcx, 0xB820512AEEAE20F1
            rax *= rcx;             //imul rax, rcx
            rcx = 0x5B4702F448FEE148;               //mov rcx, 0x5B4702F448FEE148
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF12DA]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F6501]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF0C2E]
            rax -= rcx;             //sub rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFF0AAF]
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
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            uintptr_t RSP_0xFFFFFFFFFFFFFF88;
            RSP_0xFFFFFFFFFFFFFF88 = Addresses::base + 0x639AA956;                 //lea rcx, [0x000000006099B826] : RBP+0xFFFFFFFFFFFFFF88
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
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F6085]
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
            rax -= Addresses::base;                //sub rax, [rbp-0x70] -- didn't find trace -> use Addresses::base
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r9;              //xor rcx, r9
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F597F]
            rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
            rcx = r11 * 0xFFFFFFFFFFFFFFFE;                 //imul rcx, r11, 0xFFFFFFFFFFFFFFFE
            rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
            rax += rcx;             //add rax, rcx
            rdx ^= r10;             //xor rdx, r10
            rcx = Addresses::base + 0x5F737FAA;            //lea rcx, [0x000000005C7280B8]
            rdx = ~rdx;             //not rdx
            rax += rcx;             //add rax, rcx
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x19;           //shr rcx, 0x19
            rax ^= rcx;             //xor rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x32;           //shr rcx, 0x32
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEFF90]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F5513]
            rdx = Addresses::base + 0x50AF1F4F;            //lea rdx, [0x000000004DAE1DF5]
            rax += r11;             //add rax, r11
            rcx = r11;              //mov rcx, r11
            rcx ^= rdx;             //xor rcx, rdx
            rax -= rcx;             //sub rax, rcx
            rcx = 0xFA37AFACEF63040B;               //mov rcx, 0xFA37AFACEF63040B
            rax ^= rcx;             //xor rax, rcx
            rcx = 0xE15A50F0F0B4D5D9;               //mov rcx, 0xE15A50F0F0B4D5D9
            rax *= rcx;             //imul rax, rcx
            rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
            rcx = Addresses::base + 0xEE8B;                //lea rcx, [0xFFFFFFFFFCFFE99B]
            rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
            rcx -= r11;             //sub rcx, r11
            rax ^= rcx;             //xor rax, rcx
            rdx ^= r10;             //xor rdx, r10
            rdx = ~rdx;             //not rdx
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F4FFD]
            r13 = Addresses::base + 0xBEE4;                //lea r13, [0xFFFFFFFFFCFFB8E4]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEF69A]
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
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rax *= rcx;             //imul rax, rcx
            rcx = r11;              //mov rcx, r11
            rcx -= r13;             //sub rcx, r13
            rax ^= rcx;             //xor rax, rcx
            rax -= r11;             //sub rax, r11
            return rax;
        }
        case 8:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);              //mov r9, [0x00000000097F4BAE]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEF0E3]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F44EB]
            rdx = r11;              //mov rdx, r11
            rcx = Addresses::base + 0xFCE2;                //lea rcx, [0xFFFFFFFFFCFFEA2E]
            rdx *= rcx;             //imul rdx, rcx
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEED41]
            rdx -= rcx;             //sub rdx, rcx
            rcx = r11;              //mov rcx, r11
            rax += rdx;             //add rax, rdx
            rcx = ~rcx;             //not rcx
            rax ^= rcx;             //xor rax, rcx
            rdx = Addresses::base + 0x6EC75C18;            //lea rdx, [0x000000006BC64943]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0x272542A0099256AB;               //mov rcx, 0x272542A0099256AB
            rax *= rcx;             //imul rax, rcx
            return rax;
        }
        case 10:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3FB9]
            rcx = Addresses::base + 0x240D8AB0;            //lea rcx, [0x00000000210C731B]
            rcx += r11;             //add rcx, r11
            rax ^= rcx;             //xor rax, rcx
            rdx = Addresses::base + 0x3B21;                //lea rdx, [0xFFFFFFFFFCFF213E]
            rcx = r11;              //mov rcx, r11
            rdx *= r11;             //imul rdx, r11
            rcx = ~rcx;             //not rcx
            rdx += rax;             //add rdx, rax
            rax = Addresses::base + 0x24E689C1;            //lea rax, [0x0000000021E56FBA]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0xBDB64B81FECB6E7D;               //mov rcx, 0xBDB64B81FECB6E7D
            rax *= rcx;             //imul rax, rcx
            rcx = rax;              //mov rcx, rax
            rcx >>= 0x28;           //shr rcx, 0x28
            rax ^= rcx;             //xor rax, rcx
            return rax;
        }
        case 11:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3AD2]
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
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEDFD6]
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
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEE374]
            rax ^= rcx;             //xor rax, rcx
            rcx = Addresses::base + 0x5B0222F4;            //lea rcx, [0x0000000058010275]
            rcx *= r11;             //imul rcx, r11
            rax += rcx;             //add rax, rcx
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            return rax;
        }
        case 12:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3553]
            r13 = Addresses::base + 0x852;                 //lea r13, [0xFFFFFFFFFCFEE7AD]
            rcx = 0xF7D4FABCE6FC022;                //mov rcx, 0xF7D4FABCE6FC022
            rax += rcx;             //add rax, rcx
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFEDD06]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F3183]
            r13 = Addresses::base + 0x12585A59;            //lea r13, [0x000000000F5735E4]
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
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
            rcx = 0xA4979265BBC7E3D5;               //mov rcx, 0xA4979265BBC7E3D5
            rax *= rcx;             //imul rax, rcx
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            rcx -= Addresses::base;                //sub rcx, [rbp-0x70] -- didn't find trace -> use Addresses::base
            rcx -= 0x756FAD6E;              //sub rcx, 0x756FAD6E
            rcx ^= rax;             //xor rcx, rax
            rax = Addresses::base + 0x78217335;            //lea rax, [0x0000000075204CE2]
            rcx += r11;             //add rcx, r11
            rax += rcx;             //add rax, rcx
            rcx = 0xC20F4E2AD24BC517;               //mov rcx, 0xC20F4E2AD24BC517
            rax ^= rcx;             //xor rax, rcx
            return rax;
        }
        case 14:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F2C05]
            r13 = Addresses::base + 0x3CF1;                //lea r13, [0xFFFFFFFFFCFF12FE]
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
            rax *= mem.Read<uintptr_t>(rdx + 0x7);               //imul rax, [rdx+0x07]
            rax *= rcx;             //imul rax, rcx
            rcx = 0x56A40B352BF2FDB7;               //mov rcx, 0x56A40B352BF2FDB7
            rax -= rcx;             //sub rax, rcx
            rcx = Addresses::base + 0x32FFEB8B;            //lea rcx, [0x000000002FFEC0B1]
            rcx = ~rcx;             //not rcx
            rcx *= r11;             //imul rcx, r11
            rax += rcx;             //add rax, rcx
            return rax;
        }
        case 15:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0665);             //mov r10, [0x00000000097F26AC]
            rdx = Addresses::base + 0x23AC2B6F;            //lea rdx, [0x0000000020AAFB7C]
            rcx = Addresses::base;                 //lea rcx, [0xFFFFFFFFFCFECE86]
            rax -= rcx;             //sub rax, rcx
            rax += r11;             //add rax, r11
            rdx = ~rdx;             //not rdx
            rdx ^= r11;             //xor rdx, r11
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = ~rcx;             //not rcx
            rax *= mem.Read<uintptr_t>(rcx + 0x7);               //imul rax, [rcx+0x07]
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
    inline uintptr_t decrypt_bone_base()
    {
        const uint64_t mb = Addresses::base;
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rdx = mem.Read<uintptr_t>(Addresses::base + 0x114527F8);
        if (!rdx)
            return rdx;
        r11 = mem.GetProcessPebAddress();          //mov r11, gs:[rax]
        rax = r11;              //mov rax, r11
        rax <<= 0x22;           //shl rax, 0x22
        rax = _byteswap_uint64(rax);            //bswap rax
        rax &= 0xF;
        switch (rax) {
        case 0:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEFD8E]
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5EA408]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEF8C8]
            rax = 0x311E3C7DD1297B69;               //mov rax, 0x311E3C7DD1297B69
            rdx *= rax;             //imul rdx, rax
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r10;             //xor rax, r10
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base + 0x437D3D7F;            //lea rax, [0x0000000040DBDDDE]
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
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5EA029]
            rax += 0x3B261317;              //add rax, 0x3B261317
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            rax = 0x85B82AEE944DCF96;               //mov rax, 0x85B82AEE944DCF96
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5EA12E]
            rdx += rax;             //add rdx, rax
            return rdx;
        }
        case 2:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEF41F]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
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
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEEFE3]
            r10 = Addresses::base + 0x1314A155;            //lea r10, [0x0000000010733A67]
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E9833]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEECEA]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E9296]
            rax += 0x8F29;          //add rax, 0x8F29
            rax += r11;             //add rax, r11
            rdx ^= rax;             //xor rdx, rax
            rax = 0xD43375ADC5407E51;               //mov rax, 0xD43375ADC5407E51
            rdx *= rax;             //imul rdx, rax
            return rdx;
        }
        case 5:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEE866]
            r13 = Addresses::base + 0x12426297;            //lea r13, [0x000000000FA0F425]
            rax = 0x7C5DF0A12057BE6;                //mov rax, 0x7C5DF0A12057BE6
            rdx -= rax;             //sub rdx, rax
            rax = 0x83F8FC0408B5D1AB;               //mov rax, 0x83F8FC0408B5D1AB
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base + 0x158DE932;            //lea rax, [0x0000000012EC7940]
            rax = ~rax;             //not rax
            rax *= r11;             //imul rax, r11
            rdx += rax;             //add rdx, rax
            rax = r11;              //mov rax, r11
            rax *= r13;             //imul rax, r13
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E8F4D]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = 0x3F31D045C89ED8C5;               //mov rax, 0x3F31D045C89ED8C5
            rdx *= rax;             //imul rdx, rax
            return rdx;
        }
        case 6:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEE2F9]
            r13 = Addresses::base + 0x6DA9DAD4;            //lea r13, [0x000000006B0866FC]
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
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E89C5]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rdx += r13;             //add rdx, r13
            rcx = r11;              //mov rcx, r11
            rcx = ~rcx;             //not rcx
            rdx += rcx;             //add rdx, rcx
            return rdx;
        }
        case 7:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEDF15]
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
            uintptr_t RSP_0x38;
            RSP_0x38 = Addresses::base + 0x4354;           //lea rax, [0xFFFFFFFFFD5ECBA9] : RSP+0x38
            rax *= RSP_0x38;                //imul rax, [rsp+0x38]
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rdx += rax;             //add rdx, rax
            rcx ^= r10;             //xor rcx, r10
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            return rdx;
        }
        case 8:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEDA88]
            r13 = Addresses::base + 0x7D814959;            //lea r13, [0x000000007ADFCD15]
            rdx -= r11;             //sub rdx, r11
            rax = 0xEDC13D6B57B6E285;               //mov rax, 0xEDC13D6B57B6E285
            rdx *= rax;             //imul rdx, rax
            rax = r11;              //mov rax, r11
            rax ^= r13;             //xor rax, r13
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = rdx;              //mov rax, rdx
            rax >>= 0x21;           //shr rax, 0x21
            rdx ^= rax;             //xor rdx, rax
            rdx += r11;             //add rdx, r11
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E8268]
            rdx ^= rax;             //xor rdx, rax
            return rdx;
        }
        case 9:
        {
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DED75A]
            rax = rdx;              //mov rax, rdx
            rax >>= 0xB;            //shr rax, 0x0B
            rdx ^= rax;             //xor rdx, rax
            rax = rdx;              //mov rax, rdx
            rax >>= 0x16;           //shr rax, 0x16
            rdx ^= rax;             //xor rdx, rax
            rax = rdx;              //mov rax, rdx
            rax >>= 0x2C;           //shr rax, 0x2C
            rdx ^= rax;             //xor rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E7C74]
            rdx ^= rax;             //xor rdx, rax
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
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
            rax -= Addresses::base;                //sub rax, [rbp-0x59] -- didn't find trace -> use Addresses::base
            rdx += rax;             //add rdx, rax
            rax = 0x5E76F88978AFE528;               //mov rax, 0x5E76F88978AFE528
            rdx += rax;             //add rdx, rax
            rax = 0x33557C5CEFBE234B;               //mov rax, 0x33557C5CEFBE234B
            rdx *= rax;             //imul rdx, rax
            return rdx;
        }
        case 10:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DED25F]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E7A42]
            rdx -= rax;             //sub rdx, rax
            rcx = rdx;              //mov rcx, rdx
            rcx >>= 0x27;           //shr rcx, 0x27
            rcx ^= rdx;             //xor rcx, rdx
            rdx = Addresses::base + 0x9688;                //lea rdx, [0xFFFFFFFFFD5F0FC0]
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
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = 0x3331EF2FFF7DD801;               //mov rax, 0x3331EF2FFF7DD801
            rdx *= rax;             //imul rdx, rax
            rax = 0x2130706D6228E017;               //mov rax, 0x2130706D6228E017
            rdx ^= rax;             //xor rdx, rax
            return rdx;
        }
        case 11:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DECDA3]
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rax = Addresses::base + 0x1E13DCA3;            //lea rax, [0x000000001B724FDA]
            rax = ~rax;             //not rax
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rax += r11;             //add rax, r11
            rcx ^= r10;             //xor rcx, r10
            rdx += rax;             //add rdx, rax
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E7309]
            rax += 0x9D9E;          //add rax, 0x9D9E
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            rax = r11;              //mov rax, r11
            rax -= Addresses::base;                //sub rax, [rbp-0x59] -- didn't find trace -> use Addresses::base
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
            rcx = Addresses::base + 0x7C2A0085;            //lea rcx, [0x000000007988744D]
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
            r9 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);              //mov r9, [0x0000000009DEC962]
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r9;              //xor rax, r9
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEC4E3]
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
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = 0xEA5C4AF83EEC98D;                //mov rax, 0xEA5C4AF83EEC98D
            rdx += rax;             //add rdx, rax
            rax = Addresses::base + 0x7FFA;                //lea rax, [0xFFFFFFFFFD5EEC64]
            rax = ~rax;             //not rax
            rax ^= r11;             //xor rax, r11
            rdx ^= rax;             //xor rdx, rax
            rax = 0x58CCD785809FBA65;               //mov rax, 0x58CCD785809FBA65
            rdx *= rax;             //imul rdx, rax
            rax = 0xCB1252FFDD097729;               //mov rax, 0xCB1252FFDD097729
            rdx *= rax;             //imul rdx, rax
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E6A0E]
            rax += 0x1CFB;          //add rax, 0x1CFB
            rax += r11;             //add rax, r11
            rdx += rax;             //add rdx, rax
            return rdx;
        }
        case 14:
        {
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEBF63]
            rax = rdx;              //mov rax, rdx
            rax >>= 0x1C;           //shr rax, 0x1C
            rdx ^= rax;             //xor rdx, rax
            rax = rdx;              //mov rax, rdx
            rax >>= 0x38;           //shr rax, 0x38
            rax ^= rdx;             //xor rax, rdx
            rdx = Addresses::base;                 //lea rdx, [0xFFFFFFFFFD5E6554]
            rax -= rdx;             //sub rax, rdx
            rdx = r11 + 0xffffffffd3bc8867;                 //lea rdx, [r11-0x2C437799]
            rdx += rax;             //add rdx, rax
            rax = 0;                //and rax, 0xFFFFFFFFC0000000
            rax = _rotl64(rax, 0x10);               //rol rax, 0x10
            rax ^= r10;             //xor rax, r10
            rax = _byteswap_uint64(rax);            //bswap rax
            rdx *= mem.Read<uintptr_t>(rax + 0x11);              //imul rdx, [rax+0x11]
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E646F]
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
            r10 = mem.Read<uintptr_t>(Addresses::base + 0xC7F0739);             //mov r10, [0x0000000009DEBAF2]
            rax = rdx;              //mov rax, rdx
            rax >>= 0x25;           //shr rax, 0x25
            rdx ^= rax;             //xor rdx, rax
            rdx -= r11;             //sub rdx, r11
            rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
            rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
            rcx ^= r10;             //xor rcx, r10
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E61D8]
            rax += 0x953;           //add rax, 0x953
            rax += r11;             //add rax, r11
            rcx = _byteswap_uint64(rcx);            //bswap rcx
            rdx ^= rax;             //xor rdx, rax
            rdx *= mem.Read<uintptr_t>(rcx + 0x11);              //imul rdx, [rcx+0x11]
            rax = 0xEDC186E4F45D82CF;               //mov rax, 0xEDC186E4F45D82CF
            rdx *= rax;             //imul rdx, rax
            rdx -= r11;             //sub rdx, r11
            rax = Addresses::base;                 //lea rax, [0xFFFFFFFFFD5E60E4]
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
    inline uint16_t get_bone_index(uint32_t bone_index)
    {
        const uint64_t mb = Addresses::base;
        uint64_t rax = Addresses::base, rbx = Addresses::base, rcx = Addresses::base, rdx = Addresses::base, rdi = Addresses::base, rsi = Addresses::base, r8 = Addresses::base, r9 = Addresses::base, r10 = Addresses::base, r11 = Addresses::base, r12 = Addresses::base, r13 = Addresses::base, r14 = Addresses::base, r15 = Addresses::base;
        rsi = bone_index;
        rcx = rsi * 0x13C8;
        rax = 0xD8CE01BF28E39A45;               //mov rax, 0xD8CE01BF28E39A45
        rax = _umul128(rax, rcx, (uintptr_t*)&rdx);             //mul rcx
        rdi = Addresses::base;                 //lea rdi, [0xFFFFFFFFFD5F51DA]
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
        rax = mem.Read<uint16_t>(rcx + rdi * 1 + 0xB5D2010);                 //movzx eax, word ptr [rcx+rdi*1+0xB5E22A0]
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
        r12 = mem.Read<uint16_t>(rcx + rdi * 1 + 0xB5E0C40);                 //movsx r12d, word ptr [rcx+rdi*1+0xB5F0ED0]
        return r12;
    }

    class refdef_class {
    public:
        auto retrieve_ref_def() -> uintptr_t
        {
            key encrypted = mem.Read<key>(Addresses::base + xbox::ref_def_ptr);
            DWORD lowerref = encrypted.ref0 ^ (encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr)) * ((encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr)) + 2);
            DWORD upperref = encrypted.ref1 ^ (encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr + 0x4)) * ((encrypted.ref2 ^ (uint64_t)(Addresses::base + xbox::ref_def_ptr + 0x4)) + 2); \
                return (uint64_t)upperref << 32 | lowerref;
        }
        ref_def_t ref_def_nn;
    };
    static refdef_class* decrypt_refdef = new refdef_class();

}

inline int units_to_m(float units) {
    return static_cast<int>(round(units * 0.0254));
}

// all of player information stored here
class Player {
private:



public:
    uint64_t SPlayer;
    uint64_t refDefPtr;
    uint64_t posPtr;
    uint64_t camPtr;
    Vector3 temp2;
    Vector3 RBPV;
    uint16_t BPI;
    uintptr_t PBP;
    Vector3 RBP;

    Vector3 pos;
    Vector3 TempPos;
    Vector3 helmet, head, neck, chest, lowerChest, stomach, pelvis;
    Vector3 rightThigh, rightKnee, rightAnkle, rightFoot;
    Vector3 leftThigh, leftKnee, leftAnkle, leftFoot;
    Vector3 leftShoulder, leftElbow, leftHand;
    Vector3 rightShoulder, rightElbow, rightHand;
    Vector2 helmetW2S, headW2S, neckW2S, chestW2S, lowerChestW2S, stomachW2S, pelvisW2S;
    Vector2 rightThighW2S, rightKneeW2S, rightAnkleW2S, rightFootW2S;
    Vector2 leftThighW2S, leftKneeW2S, leftAnkleW2S, leftFootW2S;
    Vector2 leftShoulderW2S, leftElbowW2S, leftHandW2S;
    Vector2 rightShoulderW2S, rightElbowW2S, rightHandW2S;
    Vector3 cam;
    Vector2 w2s;
    Vector2 TempPostW2S;
    uint32_t stance;
    byte IsDead;

    int index;
    uint8_t teamId;
    bool valid;
    NameEntry NameEnt;
    bool visible;
    int distance;
    int dead;

    void get_index(uint64_t client_info);
    void info(VMMDLL_SCATTER_HANDLE handle);
    void updatePlayer(VMMDLL_SCATTER_HANDLE handle);
    void getCam(VMMDLL_SCATTER_HANDLE handle);
    void updateCam(VMMDLL_SCATTER_HANDLE handle);
    void updateBoneinfo(VMMDLL_SCATTER_HANDLE handle, uint64_t ClientInfo, uintptr_t BoneBase, Player& localplayer);
    void updateBoneW2Sinfo(Player& localplayer);
};
 // game cache stuff, can do better but :shrug:
class Game {
private:
public:
    std::vector<Player> Players;
    std::vector<Vector2> coords;
    std::vector<float> distance;

    int InGame;
    int PlayerCount;
    uint64_t clientInfo;
    uint64_t clientBase;
    uint64_t refDefPtr;
    uint64_t BoneBase;
    void scatter_ingame(VMMDLL_SCATTER_HANDLE handle);
};
// doubt will change
enum BoneIDs : int
{
    Helmet = 8,
    Head = 7,
    Neck = 6,
    Chest = 5,
    LowerChest = 4,
    Stomach = 3,
    Pelvis = 2,

    RightThigh = 21,
    RightKnee = 22,
    RightAnkle = 23,
    RightFoot = 24,

    LeftThigh = 17,
    LeftKnee = 18,
    LeftAnkle = 19,
    LeftFoot = 20,

    LeftShoulder = 14,
    LeftElbow = 15,
    LeftHand = 16,

    RightShoulder = 10,
    RightElbow = 11,
    RightHand = 12
};