#include <kmboxNet.h>
#include <KmboxB.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include "Config.h"
#include <Memory.h>
#include <blops6/blackops6.h>

// can put these in a settings namespace in another file to make them look pretty
bool Meni = false;
bool Fov = false;
float ValidityDist = 1600.0f;
float LowerValidityDist = 0.1f;
float FovSize = 20;
int Smoothing = 2;
bool Names = false;
bool Health = false;
bool Box = false;
bool Skeleton = false;
int selectedBone = 1;
bool Box3d = false;
bool Snapline = false;
bool Aimbot = false;
bool Crosshair = false;
bool buttontest = false;
bool moonlight = false;
bool debug = true;
_com comPort;
struct ClientBits
{
    int Array[7];
};

template <typename... Args>
void DebugLog(const char* format, Args... args)
{
    if (debug) // bool debug = true;
    {
        printf(format, args...); // fucking args
        std::cout << std::endl;
    }
}

const char* BoneTypeNames[] = {
    "Helmet", "Head", "Neck", "Chest", "LowerChest", "Stomach", "Pelvis",
    "RightThigh", "RightKnee", "RightAnkle", "RightFoot",
    "LeftThigh", "LeftKnee", "LeftAnkle", "LeftFoot",
    "LeftShoulder", "LeftElbow", "LeftHand",
    "RightShoulder", "RightElbow", "RightHand"
};
// dropdown 1337 leet

// fix by reading/writing bitmask - otherwise crash
bool visible(int32_t playerIndex, uintptr_t clientInfo, Memory& mem)
{
    auto player = mem.Read<ClientBits>(clientInfo + steam::visible_client_bits); // fixed by reading bitmask!
    auto bitmask = 0x80000000 >> (playerIndex & 0x1F);

    return player.Array[playerIndex >> 5] & bitmask;
}
// retarded kmbox functions
void InitKMbox(const Config& config)
{
    if (config.kmboxType == "Net")
    {
        std::cout << "Initializing KMbox NET with IP: " << config.kmboxIP
            << ", Port: " << config.kmboxPort
            << ", UUID: " << config.kmboxUUID << std::endl;
        int initResult = kmNet_init(
            const_cast<char*>(config.kmboxIP.c_str()),
            const_cast<char*>(std::to_string(config.kmboxPort).c_str()),
            const_cast<char*>(config.kmboxUUID.c_str())
        );
        if (initResult != success)
        {
            std::cerr << "Failed to initialize KMbox NET, error code: " << initResult << std::endl;
            //exit(1);
        } else { std::cout << "KMbox NET initialized successfully\n"; }
    }
}
void InitArdiuno(const Config& config)
{
    if (config.kmboxType == "Arduino")
    {
        if (!comPort.open(config.KmboxComPort, config.kmboxBaud))
        {
            std::cerr << "Failed to initialize Arduino, Are you sure you got port/baud right?" << std::endl;
            //exit(1);
        }else { std::cout << "arduino initialized successfully\n"; }
    }
}
void MoveKMBOX(Config config, int x, int y, int smoothing)
{
    kmNet_mouse_move_auto(x, y, smoothing); // Adjust timing as necessary
}
void MoveArduino(Config config, int x, int y, int smoothing)
{
    char cmd[1024] = { 0 };
    sprintf_s(cmd, "km.move(%d, %d, %d)\r\n", x, y, smoothing);
    comPort.write(cmd);
}
bool CheckMR(Config config)
{
    if (config.kmboxType == "Net") {
        int moved = kmNet_monitor_mouse_right();
        if (moved == 1)
        {
            return true;
        }
        else { return false; }
    }
    else {
        char cmd[1024] = { 0 };
        sprintf_s(cmd, "km.right()\r\n"); //  " Right mouse right control function "
        comPort.write(cmd);
        cmd[1024] = {0}; // dump cmd
        std::vector<char> buffer(1024, 0);
        size_t BytesR = comPort.read(buffer.data(), buffer.size()); // Read response
        std::string op(buffer.begin(), buffer.begin() + BytesR); 
        /*

        mouse button:
0: pop-up
1: physical press down -- means mouse is clicked
2: Software press down -- means mouse is clicked from ardunio
3: Physical software all pressed
When giving parameters is to set the right mouse button state: (write paramter)
0: sets the right mouse button to click
1: Set the right mouse button to press down


        */
        if (op == "1")
        {
            return true;
        }
        else { return false;  }

    }
}

// ingame functions, or utilites
bool IsValidTarget(const Player& player, const Player& localPlayer) {
    return !(player.dead > 0 || player.teamId == localPlayer.teamId);
}
bool IsOnScreen(const Vector2& pos) {
    return pos.x > 0 && pos.y > 0 &&
        pos.x < ScreenX && pos.y < ScreenY;
}
float CalculateScreenDistance(const Vector2& pos) {
    float deltaX = pos.x - ScreenCenterX;
    float deltaY = pos.y - ScreenCenterY;
    return sqrt(deltaX * deltaX + deltaY * deltaY);
}
void ApplySmoothedMovement(const Vector2& targetPos, const Config& confg) {
    float deltaX = targetPos.x - ScreenCenterX;
    float deltaY = targetPos.y - ScreenCenterY;

    // Apply acceleration curve for more natural movement
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    float speedFactor = (((1.0f) < (distance / FovSize)) ? (1.0f) : (distance / FovSize));
    float adjustedSmoothing = Smoothing * (1.0f + (1.0f - speedFactor));

    deltaX /= adjustedSmoothing;
    deltaY /= adjustedSmoothing;

    MoveKMBOX(confg, deltaX, deltaY, Smoothing * 100);
}
bool is_bone_valid(Vector3& base, Vector3& bone) {
    float dist = base.Dist2D(bone);

    if (dist < 0.1f || dist > ValidityDist /*forgot, value is 1700-1800*/) {
        return false;
    }
    return true;
}
bool world_to_screen(Vector3& world_location, Vector2& out, Vector3& camera_pos,
    int screen_width, int screen_height, Vector2& fov, Vector3 matricies[3]) {
    constexpr float MIN_Z_DISTANCE = 0.01f;
    constexpr float SCREEN_BORDER_MARGIN = 1.0f;

    auto local = world_location - camera_pos;

    auto trans = Vector3{
        local.Dot(matricies[1]),
        local.Dot(matricies[2]),
        local.Dot(matricies[0])
    };

    if (trans.z < MIN_Z_DISTANCE) return false;


    out.x = (screen_width * 0.5f) * (1.0f - (trans.x / fov.x / trans.z));
    out.y = (screen_height * 0.5f) * (1.0f - (trans.y / fov.y / trans.z));

    return out.x >= SCREEN_BORDER_MARGIN &&
        out.y >= SCREEN_BORDER_MARGIN &&
        out.x <= (steam::decrypt_refdef->ref_def_nn.width - SCREEN_BORDER_MARGIN) &&
        out.y <= (steam::decrypt_refdef->ref_def_nn.height - SCREEN_BORDER_MARGIN);
}
auto W2S(Vector3 world_position, Vector2& screen_position, Vector3 cam_position) -> bool
{
    return world_to_screen(world_position, screen_position, cam_position, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), steam::decrypt_refdef->ref_def_nn.tan_half_fov, steam::decrypt_refdef->ref_def_nn.axis);
}

// theme colors
void SetHRGColors()
{
    ImGuiStyle& Style = ImGui::GetStyle();
    auto Color = Style.Colors;
    Style.WindowBorderSize = 1;
    Style.FrameBorderSize = 1;
    Style.ChildRounding = 0;
    Style.FrameRounding = 0;
    Style.ScrollbarRounding = 0;
    Style.GrabRounding = 0;
    Style.PopupRounding = 0;
    Style.WindowRounding = 0;
    Color[ImGuiCol_TitleBg] = ImColor(9, 9, 9, 255);
    Color[ImGuiCol_TitleBgActive] = ImColor(9, 9, 9, 255);
    Color[ImGuiCol_WindowBg] = ImColor(9, 9, 9, 255);
    Color[ImGuiCol_FrameBg] = ImColor(11, 11, 11, 255);
    Color[ImGuiCol_FrameBorder] = ImColor(56, 56, 56, 255);
    Color[ImGuiCol_FrameBgActive] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_FrameBgHovered] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_Button] = ImColor(29, 29, 29, 255);
    Color[ImGuiCol_ButtonActive] = ImColor(32, 32, 32, 255);
    Color[ImGuiCol_ButtonHovered] = ImColor(36, 36, 36, 255);
    Color[ImGuiCol_Border] = ImColor(255, 36, 36, 255);
    Color[ImGuiCol_Separator] = ImColor(36, 36, 36, 255);
    Color[ImGuiCol_ResizeGrip] = ImColor(30, 30, 30, 255);
    Color[ImGuiCol_ResizeGripActive] = ImColor(30, 30, 30, 255);
    Color[ImGuiCol_ResizeGripHovered] = ImColor(30, 30, 30, 255);
    Color[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 255);
    Color[ImGuiCol_SliderGrab] = ImColor(121, 52, 205, 255);
    Color[ImGuiCol_SliderGrabActive] = ImColor(150, 64, 255, 255);
    Color[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_Header] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_HeaderActive] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_HeaderHovered] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
    Color[ImGuiCol_CheckMarkBorder] = ImColor(56, 56, 56, 255);
}
void DebugColors()
{
    ImGuiStyle& Style = ImGui::GetStyle();
    auto Color = Style.Colors;
    Style.WindowBorderSize = 1;
    Style.FrameBorderSize = 1;
    Style.ChildRounding = 0;
    Style.FrameRounding = 0;
    Style.ScrollbarRounding = 0;
    Style.GrabRounding = 0;
    Style.PopupRounding = 0;
    Style.WindowRounding = 0;
    Color[ImGuiCol_TitleBg] = ImColor(9, 9, 9, 255);
    Color[ImGuiCol_TitleBgActive] = ImColor(9, 9, 9, 255);
    Color[ImGuiCol_WindowBg] = ImColor(9, 9, 9, 255);
    Color[ImGuiCol_FrameBg] = ImColor(11, 11, 11, 255);
    Color[ImGuiCol_FrameBorder] = ImColor(56, 56, 56, 255);
    Color[ImGuiCol_FrameBgActive] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_FrameBgHovered] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_Button] = ImColor(29, 29, 29, 255);
    Color[ImGuiCol_ButtonActive] = ImColor(32, 32, 32, 255);
    Color[ImGuiCol_ButtonHovered] = ImColor(36, 36, 36, 255);
    Color[ImGuiCol_Border] = ImColor(36, 36, 36, 255);
    Color[ImGuiCol_Separator] = ImColor(36, 36, 36, 255);
    Color[ImGuiCol_ResizeGrip] = ImColor(30, 30, 30, 255);
    Color[ImGuiCol_ResizeGripActive] = ImColor(30, 30, 30, 255);
    Color[ImGuiCol_ResizeGripHovered] = ImColor(30, 30, 30, 255);
    Color[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 255);
    Color[ImGuiCol_SliderGrab] = ImColor(121, 52, 205, 255);
    Color[ImGuiCol_SliderGrabActive] = ImColor(150, 64, 255, 255);
    Color[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
    Color[ImGuiCol_Header] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_HeaderActive] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_HeaderHovered] = ImColor(18, 17, 17, 255);
    Color[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
    Color[ImGuiCol_CheckMarkBorder] = ImColor(56, 56, 56, 255);
}

// obsolete
struct BoneInfo {
    Vector3 bonePos;
    Vector2 boneW2S;
    const char* boneName;
};

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImFont* regular_font;
ImFont* bold_font;
ImFont* italic_font;
ImFont* huge_font;

// can make debug check, for errors related to customer crashes
void Log(const std::string& text) {
    std::string filename = "Log.txt";
    if (!std::filesystem::exists(filename)) {
        std::ofstream outfile(filename);
        outfile.close(); // Close immediately after creation
    }

    // Open the file in append mode and write the text
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << text << std::endl;
        file.close();
    }
    else {
    }
}
// awesome player loop
void Players(Memory& mem, Game& game, Player& localPlayer, VMMDLL_SCATTER_HANDLE& handle, Config& confg)
{
    game.scatter_ingame(handle);
    mem.ExecuteReadScatter(handle);

    if (game.InGame > 1) {
        Log("In Game");
        DebugLog("in game!\n");
        DebugLog("igv: %d\n", game.InGame);
        game.clientInfo = steam::decrypt_client_info();
        DebugLog("clientInfo 0x%llx\n", game.clientInfo);

        if (game.clientInfo) {
            game.clientBase = steam::decrypt_client_base(game.clientInfo);
            game.refDefPtr = steam::decrypt_refdef->retrieve_ref_def();
            game.BoneBase = steam::decrypt_bone_base();
            DebugLog("BoneBase 0x%llx\n", game.BoneBase);
            DebugLog("ClientBase 0x%llx\n", game.clientBase);
            localPlayer.get_index(game.clientInfo);
            localPlayer.SPlayer = game.clientBase + (localPlayer.index * steam::player::size);
            DebugLog("LocalPlayer 0x%llx\n", localPlayer.SPlayer);
            localPlayer.info(handle);
            localPlayer.getCam(handle);
            mem.ExecuteReadScatter(handle);

            localPlayer.get_index(game.clientInfo);
            localPlayer.updatePlayer(handle);
            localPlayer.updateCam(handle);
            mem.ExecuteReadScatter(handle);
            
            mem.AddScatterReadRequest(handle, game.refDefPtr, &steam::decrypt_refdef->ref_def_nn, sizeof(ref_def_t));
            ref_def_t& ref_def = steam::decrypt_refdef->ref_def_nn;

            for (int i = 0; i < game.InGame; i++) {
                auto playerExists = [&game](int index) {
                    return std::any_of(game.Players.begin(), game.Players.end(), [index](const Player& player) {
                        return player.index == index;
                        });
                    };

                if (i == localPlayer.index || playerExists(i)) {
                    continue;
                }

                Player player;
                player.index = i;
                player.SPlayer = (game.clientBase + (i * steam::player::size));
                game.Players.push_back(player);

                
            }
            for (int i = 0; i < game.Players.size(); i++) {
                game.Players[i].info(handle);
            }
            mem.ExecuteReadScatter(handle);
            for (int i = 0; i < game.Players.size(); i++) {
                if (game.Players[i].valid) {
                    //std::cout << "Player " << i << " Is valid." << std::endl;
                    game.Players[i].updatePlayer(handle);
                } else {  }
            }
            game.scatter_ingame(handle);
            mem.ExecuteReadScatter(handle);
            if (game.InGame == 1) {
                game.Players.clear();
            }

            // player loop
            for (int i = 0; i < game.Players.size(); i++) {
                if (game.Players[i].valid) {
                    if (game.Players[i].dead > 0 || game.Players[i].teamId == localPlayer.teamId) {
                        game.Players[i].w2s = { 0,0 };
                        game.Players[i].headW2S = { 0,0 };
           
                        std::cout << game.Players[i].teamId << std::endl;
                        std::cout << game.Players[i].dead << std::endl;
                        continue;
                    }
                    Player& player = game.Players[i];
                    ImColor color = ImColor(255, 255, 255, 255);
                    //game.Players[i].updateBoneinfo(game.clientInfo, steam::bone::bone_base);
                    // scatters
                    auto boneptr = mem.Read<uint64_t>(game.BoneBase + (steam::get_bone_index(player.index) * steam::bone::size) + steam::bone::offset);
                    {
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::Helmet * 0x20) + 0x10, &player.helmet, sizeof(player.helmet));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::Head * 0x20) + 0x10, &player.head, sizeof(player.head));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::Neck * 0x20) + 0x10, &player.neck, sizeof(player.neck));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::Chest * 0x20) + 0x10, &player.chest, sizeof(player.chest));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LowerChest * 0x20) + 0x10, &player.lowerChest, sizeof(player.lowerChest));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::Stomach * 0x20) + 0x10, &player.stomach, sizeof(player.stomach));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::Pelvis * 0x20) + 0x10, &player.pelvis, sizeof(player.pelvis));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::RightThigh * 0x20) + 0x10, &player.rightThigh, sizeof(player.rightThigh));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::RightKnee * 0x20) + 0x10, &player.rightKnee, sizeof(player.rightKnee));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::RightAnkle * 0x20) + 0x10, &player.rightAnkle, sizeof(player.rightAnkle));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::RightFoot * 0x20) + 0x10, &player.rightFoot, sizeof(player.rightFoot));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LeftThigh * 0x20) + 0x10, &player.leftThigh, sizeof(player.leftThigh));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LeftKnee * 0x20) + 0x10, &player.leftKnee, sizeof(player.leftKnee));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LeftAnkle * 0x20) + 0x10, &player.leftAnkle, sizeof(player.leftAnkle));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LeftFoot * 0x20) + 0x10, &player.leftFoot, sizeof(player.leftFoot));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LeftShoulder * 0x20) + 0x10, &player.leftShoulder, sizeof(player.leftShoulder));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LeftElbow * 0x20) + 0x10, &player.leftElbow, sizeof(player.leftElbow));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::LeftHand * 0x20) + 0x10, &player.leftHand, sizeof(player.leftHand));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::RightShoulder * 0x20) + 0x10, &player.rightShoulder, sizeof(player.rightShoulder));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::RightElbow * 0x20) + 0x10, &player.rightElbow, sizeof(player.rightElbow));
                        mem.AddScatterReadRequest(handle, boneptr + (BoneIDs::RightHand * 0x20) + 0x10, &player.rightHand, sizeof(player.rightHand));
                    }
                    mem.ExecuteReadScatter(handle);
                    // fucking stupid bullshit - fake head size, will need to debug yourself
                    player.TempPos = player.pos;
                    player.TempPos.y = player.TempPos.y + 46;
                    // w2s
                    {
                        W2S(player.helmet + localPlayer.cam, player.helmetW2S, localPlayer.cam);
                        W2S(player.head + localPlayer.cam, player.headW2S, localPlayer.cam);
                        W2S(player.neck + localPlayer.cam, player.neckW2S, localPlayer.cam);
                        W2S(player.chest + localPlayer.cam, player.chestW2S, localPlayer.cam);
                        W2S(player.lowerChest + localPlayer.cam, player.lowerChestW2S, localPlayer.cam);
                        W2S(player.stomach + localPlayer.cam, player.stomachW2S, localPlayer.cam);
                        W2S(player.pelvis + localPlayer.cam, player.pelvisW2S, localPlayer.cam);
                        W2S(player.rightThigh + localPlayer.cam, player.rightThighW2S, localPlayer.cam);
                        W2S(player.rightKnee + localPlayer.cam, player.rightKneeW2S, localPlayer.cam);
                        W2S(player.rightAnkle + localPlayer.cam, player.rightAnkleW2S, localPlayer.cam);
                        W2S(player.rightFoot + localPlayer.cam, player.rightFootW2S, localPlayer.cam);
                        W2S(player.leftThigh + localPlayer.cam, player.leftThighW2S, localPlayer.cam);
                        W2S(player.leftKnee + localPlayer.cam, player.leftKneeW2S, localPlayer.cam);
                        W2S(player.leftAnkle + localPlayer.cam, player.leftAnkleW2S, localPlayer.cam);
                        W2S(player.leftFoot + localPlayer.cam, player.leftFootW2S, localPlayer.cam);
                        W2S(player.leftShoulder + localPlayer.cam, player.leftShoulderW2S, localPlayer.cam);
                        W2S(player.leftElbow + localPlayer.cam, player.leftElbowW2S, localPlayer.cam);
                        W2S(player.leftHand + localPlayer.cam, player.leftHandW2S, localPlayer.cam);
                        W2S(player.rightShoulder + localPlayer.cam, player.rightShoulderW2S, localPlayer.cam);
                        W2S(player.rightElbow + localPlayer.cam, player.rightElbowW2S, localPlayer.cam);
                        W2S(player.rightHand + localPlayer.cam, player.rightHandW2S, localPlayer.cam);
                        W2S(player.pos, player.w2s, localPlayer.cam);
                        W2S(player.TempPos, player.TempPostW2S, localPlayer.cam);
                    }
                    std::lock_guard<std::mutex> lock(mutexlock);
                    player.distance = units_to_m(localPlayer.pos.DistTo(player.pos));
                    
                    player.NameEnt = steam::get_name_entry(player.index);
                    player.visible = visible(player.index, game.clientInfo, mem);
                    if (Snapline)
                    {
                        ImGui::GetForegroundDrawList()->AddLine(ImVec2(ScreenCenterX, ScreenCenterY - ScreenCenterY), ImVec2(player.w2s.x, player.w2s.y), color);
                    }
                    if (Box) {
                        if (player.distance <= 0.1f || player.w2s.x < 0 || player.w2s.y < 0) {
                            player.w2s = { 0, 0 };
                            player.headW2S = { 0, 0 };
                            Log("Invalid Player Info on player " + std::to_string(i));
                            continue;
                        }
                        float height;
                        /*if (is_bone_valid(player.pos, player.head))
                        {
                            height = player.w2s.Dist2D(player.headW2S);  // Minimum height (stupid way)
                        }
                        else {
                             height = player.TempPostW2S.y;
                        }*/
                        height = player.headW2S.y;
                        float width = height * 0.5f;

                        // Prevent zero or invalid coordinates
                        float safe_x = max(player.w2s.x, 0.1f);
                        float safe_y = max(player.w2s.y, 0.1f);

                        ImVec2 top_left(safe_x - width / 2, safe_y - height);
                        ImVec2 bottom_right(safe_x + width / 2, safe_y);

                        // Health calculation and clamping to ensure it's between 0 and 1
                        int health = player.NameEnt.health;
                        float health_percentage = std::clamp(health / 100.0f, 0.0f, 1.0f);

                        if (Health)
                        {
                            float health_bar_height = height * health_percentage;

                            auto col = ImColor((int)(255 * (1 - health_percentage)), (int)(255 * health_percentage), 0, 255);

                            ImVec2 health_top_left(player.w2s.x - width / 2 - 5, player.w2s.y - height);
                            ImVec2 health_bottom_right(player.w2s.x - width / 2 - 1, player.w2s.y - height + health_bar_height);

                            // Health bar
                            ImGui::GetForegroundDrawList()->AddRectFilled(health_top_left, health_bottom_right, col);

                            // Health bar border
                            ImGui::GetForegroundDrawList()->AddRect(health_top_left, health_bottom_right, ImColor(255, 255, 255, 255));
                        }
                        if (Names)
                        {
                            std::string name_text = "[" + std::string(player.NameEnt.tag) + "] " + std::string(player.NameEnt.name);

                            auto name_size = ImGui::CalcTextSize(name_text.c_str());
                            ImVec2 name_position(player.headW2S.x - (name_size.x / 2), player.headW2S.y - height - 10);

                            // player name text
                            ImGui::GetForegroundDrawList()->AddText(name_position, ImColor(255, 255, 255, 255), name_text.c_str());

                        }
                        // BOX
                        ImGui::GetForegroundDrawList()->AddRect(top_left, bottom_right, color);  // White box
                        
                    }
                    if (Skeleton) {
                        // Positions for bones (using ImVec2 for 2D screen coordinates)
                        ImVec2 helmetPos = ImVec2(player.helmetW2S.x, player.helmetW2S.y);
                        ImVec2 headPos = ImVec2(player.headW2S.x, player.headW2S.y);
                        ImVec2 neckPos = ImVec2(player.neckW2S.x, player.neckW2S.y);
                        ImVec2 chestPos = ImVec2(player.chestW2S.x, player.chestW2S.y);
                        ImVec2 lowerChestPos = ImVec2(player.lowerChestW2S.x, player.lowerChestW2S.y);
                        ImVec2 stomachPos = ImVec2(player.stomachW2S.x, player.stomachW2S.y);
                        ImVec2 pelvisPos = ImVec2(player.pelvisW2S.x, player.pelvisW2S.y);
                        ImVec2 rightThighPos = ImVec2(player.rightThighW2S.x, player.rightThighW2S.y);
                        ImVec2 rightKneePos = ImVec2(player.rightKneeW2S.x, player.rightKneeW2S.y);
                        ImVec2 rightAnklePos = ImVec2(player.rightAnkleW2S.x, player.rightAnkleW2S.y);
                        ImVec2 rightFootPos = ImVec2(player.rightFootW2S.x, player.rightFootW2S.y);
                        ImVec2 leftThighPos = ImVec2(player.leftThighW2S.x, player.leftThighW2S.y);
                        ImVec2 leftKneePos = ImVec2(player.leftKneeW2S.x, player.leftKneeW2S.y);
                        ImVec2 leftAnklePos = ImVec2(player.leftAnkleW2S.x, player.leftAnkleW2S.y);
                        ImVec2 leftFootPos = ImVec2(player.leftFootW2S.x, player.leftFootW2S.y);
                        ImVec2 leftShoulderPos = ImVec2(player.leftShoulderW2S.x, player.leftShoulderW2S.y);
                        ImVec2 leftElbowPos = ImVec2(player.leftElbowW2S.x, player.leftElbowW2S.y);
                        ImVec2 leftHandPos = ImVec2(player.leftHandW2S.x, player.leftHandW2S.y);
                        ImVec2 rightShoulderPos = ImVec2(player.rightShoulderW2S.x, player.rightShoulderW2S.y);
                        ImVec2 rightElbowPos = ImVec2(player.rightElbowW2S.x, player.rightElbowW2S.y);
                        ImVec2 rightHandPos = ImVec2(player.rightHandW2S.x, player.rightHandW2S.y);
                        /*
                        && (player.distance <= 50)
                        && (player.w2s.x >= 0 && player.w2s.x <= ScreenX) &&
                        (player.w2s.y >= 0 && player.w2s.y <= ScreenY)
                        */
                        if ((is_bone_valid(player.pos, player.head /*1700-1800 is because of how big cod maps are, can debug by printing player pos and player head*/)
                            ))
                        {

                            // is_bone_valid POS TO BONE!
                            // DO NOT DO BONE TO BONE, WILL NOT WORK!
                            // Head to Neck
                            if (is_bone_valid(player.head, player.neck)) {
                                ImGui::GetForegroundDrawList()->AddLine(headPos, neckPos, color, 2);
                            }

                            // Neck to Chest
                            if (is_bone_valid(player.pos, player.chest)) {
                                ImGui::GetForegroundDrawList()->AddLine(neckPos, chestPos, color, 2);
                            }

                            // Chest to Shoulders
                            if (is_bone_valid(player.pos, player.leftShoulder)) {
                                ImGui::GetForegroundDrawList()->AddLine(chestPos, leftShoulderPos, color, 2);
                            }
                            if (is_bone_valid(player.pos, player.rightShoulder)) {
                                ImGui::GetForegroundDrawList()->AddLine(chestPos, rightShoulderPos, color, 2);
                            }

                            // Shoulders to Elbows
                            if (is_bone_valid(player.pos, player.leftElbow)) {
                                ImGui::GetForegroundDrawList()->AddLine(leftShoulderPos, leftElbowPos, color, 2);
                            }
                            if (is_bone_valid(player.pos, player.rightElbow)) {
                                ImGui::GetForegroundDrawList()->AddLine(rightShoulderPos, rightElbowPos, color, 2);
                            }

                            // Elbows to Hands
                            if (is_bone_valid(player.pos, player.leftHand)) {
                                ImGui::GetForegroundDrawList()->AddLine(leftElbowPos, leftHandPos, color, 2);
                            }
                            if (is_bone_valid(player.pos, player.rightHand)) {
                                ImGui::GetForegroundDrawList()->AddLine(rightElbowPos, rightHandPos, color, 2);
                            }

                            // Chest to Pelvis
                            if (is_bone_valid(player.pos, player.pelvis)) {
                                ImGui::GetForegroundDrawList()->AddLine(chestPos, pelvisPos, color, 2);
                            }

                            // Pelvis to Thighs
                            if (is_bone_valid(player.pos, player.leftThigh)) {
                                ImGui::GetForegroundDrawList()->AddLine(pelvisPos, leftThighPos, color, 2);
                            }
                            if (is_bone_valid(player.pos, player.rightThigh)) {
                                ImGui::GetForegroundDrawList()->AddLine(pelvisPos, rightThighPos, color, 2);
                            }

                            // Thighs to Knees
                            if (is_bone_valid(player.pos, player.leftKnee)) {
                                ImGui::GetForegroundDrawList()->AddLine(leftThighPos, leftKneePos, color, 2);
                            }
                            if (is_bone_valid(player.pos, player.rightKnee)) {
                                ImGui::GetForegroundDrawList()->AddLine(rightThighPos, rightKneePos, color, 2);
                            }

                            // Knees to Ankles
                            if (is_bone_valid(player.pos, player.leftAnkle)) {
                                ImGui::GetForegroundDrawList()->AddLine(leftKneePos, leftAnklePos, color, 2);
                            }
                            if (is_bone_valid(player.pos, player.rightAnkle)) {
                                ImGui::GetForegroundDrawList()->AddLine(rightKneePos, rightAnklePos, color, 2);
                            }

                            // Ankles to Feet
                            if (is_bone_valid(player.pos, player.leftFoot)) {
                                ImGui::GetForegroundDrawList()->AddLine(leftAnklePos, leftFootPos, color, 2);
                            }
                            if (is_bone_valid(player.pos, player.rightFoot)) {
                                ImGui::GetForegroundDrawList()->AddLine(rightAnklePos, rightFootPos, color, 2);
                            }
                        }
                    }
                }
 else {  }
            };
            // mem.GetKeyboard dosent work. (mem.GetKeyboard()->IsKeyDown(VK_RBUTTON))
            /*
// Monitor the status of the physical mouse right button
Return values
-1: Monitoring function not enabled yet. Call kmNet_monitor(1) first.
0 : Physical mouse right button released
1 : Physical mouse right button pressed
*/
            if (Fov && CheckMR(confg)/*write your own keyboard library from dma for support even without kmbox, i am not doing it as it takes weeks, so check through the kmbox yourself*/) {
                bool moved = false;
                float closestDistance = FovSize;
                Vector2 targetPos = { 0, 0 };

                for (int i = 0; i < game.Players.size(); i++) {
                    // Skip dead players and teammates
                    if (game.Players[i].dead > 0 || game.Players[i].teamId == localPlayer.teamId) {
                        continue;
                    }

                    Vector2 targetBonePos = { 0, 0 };

                    // Get the position of the selected bone
                    switch (selectedBone) {
                    case 1: targetBonePos = game.Players[i].helmetW2S; break;
                    case 2: targetBonePos = game.Players[i].headW2S; break;
                    case 3: targetBonePos = game.Players[i].neckW2S; break;
                    case 4: targetBonePos = game.Players[i].chestW2S; break;
                    case 5: targetBonePos = game.Players[i].lowerChestW2S; break;
                    case 6: targetBonePos = game.Players[i].stomachW2S; break;
                    case 7: targetBonePos = game.Players[i].pelvisW2S; break;
                    case 8: targetBonePos = game.Players[i].rightThighW2S; break;
                    case 9: targetBonePos = game.Players[i].rightKneeW2S; break;
                    case 10: targetBonePos = game.Players[i].rightAnkleW2S; break;
                    case 11: targetBonePos = game.Players[i].rightFootW2S; break;
                    case 12: targetBonePos = game.Players[i].leftThighW2S; break;
                    case 13: targetBonePos = game.Players[i].leftKneeW2S; break;
                    case 14: targetBonePos = game.Players[i].leftAnkleW2S; break;
                    case 15: targetBonePos = game.Players[i].leftFootW2S; break;
                    case 16: targetBonePos = game.Players[i].leftShoulderW2S; break;
                    case 17: targetBonePos = game.Players[i].leftElbowW2S; break;
                    case 18: targetBonePos = game.Players[i].leftHandW2S; break;
                    case 19: targetBonePos = game.Players[i].rightShoulderW2S; break;
                    case 20: targetBonePos = game.Players[i].rightElbowW2S; break;
                    case 21: targetBonePos = game.Players[i].rightHandW2S; break;
                    }

                    float headX = targetBonePos.x;
                    float headY = targetBonePos.y;
                    bool visible = game.Players[i].visible;

                    // Skip players whose target bone is off-screen
                    if (headX <= 0 || headY <= 0 || headX >= ScreenX || headY >= ScreenY) {
                        continue;
                    }

                    float deltaX = headX - ScreenCenterX;
                    float deltaY = headY - ScreenCenterY;
                    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

                    // Check if the player is within FOV
                    if (distance < FovSize && distance < closestDistance && visible) {
                        closestDistance = distance;
                        targetPos = { headX, headY };
                        moved = true;
                    }
                }

                // Move to the closest valid target
                if (moved) {
                    float deltaX = targetPos.x - ScreenCenterX;
                    float deltaY = targetPos.y - ScreenCenterY;

                    // Smoothing
                    deltaX /= Smoothing;
                    deltaY /= Smoothing;

                    if (confg.kmboxType == "Net") {
                        MoveKMBOX(confg, deltaX, deltaY, Smoothing * 100);
                    }
                    else {
                        MoveArduino(confg, deltaX, deltaY, Smoothing * 10);
                    }
                }
            }
        }
    }
    game.distance.clear();
    game.coords.clear();
    game.Players.clear();
   // DebugLog("not in game\n");
}
 // awesome render loop
void Render(Config& config, Memory& mem, Game& game)
{
    Log("Starting Render...");
    VMMDLL_SCATTER_HANDLE handle = mem.CreateScatterHandle();
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    Player localPlayer;//
    bool result;
    if (config.kmboxType == "Net")
    {
        InitKMbox(config);
        kmNet_monitor(config.kmboxPort);
    }
    else {
        InitArdiuno(config);
    }
    // hehe
   // kmNet_lcd_color(RGB(125, 0, 0));
    //kmNet_lcd_picture(Logo);
    
    // test move to 0 0
    //MoveKMBOX(config, 0, 0, 0);
    //kmNet_mouse_move_auto(600, 0, 2000);
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"adad", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName, L"adad", WS_POPUP,
        0, 0, width, height, nullptr, nullptr, wc.hInstance, nullptr);

    COLORREF transparencyKey = RGB(0, 0, 0);
    SetLayeredWindowAttributes(hwnd, 25, RGB(0, 0, 0), LWA_COLORKEY);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    regular_font = io.Fonts->AddFontFromFileTTF("regular.otf", 12.0f);
    bold_font = io.Fonts->AddFontFromFileTTF("bold.otf", 12.0f);
    italic_font = io.Fonts->AddFontFromFileTTF("italic.otf", 12.0f);
    huge_font = io.Fonts->AddFontFromFileTTF("bold.otf", 20.0f);
    // fdef = custom, can put any font, even downloaded ones
    ImFont* fdef = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);
    io.FontDefault = fdef;
    //ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    // Set a slightly visible background color - This is for fuser, if you wanted to use a moonlight, change the 25.0f to 0, and the SetLayerdWindowAttributes from 25 to 0
    ImVec4 clear_color = ImVec4(25.0f / 255.0f, 25.0f / 255.0f, 25.0f / 255.0f, 0.1f);
    Log("Entering Render Loop...");
    bool running = true;
    while (running)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                running = false;
        }

        if (!running)
        {
            Log("Breaking @ line 359");
            break;
        }

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(width, height));
        // overlay is probably needed, will create a debug menu if it dosent exist (blows)
        Log("Creating Overlay");
        ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoInputs);
        {

            ImDrawList* fg_draw = ImGui::GetForegroundDrawList();

            if (!moonlight)
            {
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(width + 20, height + 20), ImColor(0, 0, 0, 255));
            }
            std::string PerformanceString = "Render FPS: " + std::to_string(static_cast<int>(ImGui::GetIO().Framerate));
            fg_draw->AddText(ImVec2(0, 0), IM_COL32(255, 255, 255, 255), PerformanceString.c_str());
            if (Fov)
            {
                fg_draw->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), FovSize, ImColor(255, 255, 255, 255), 48, 2);
            }
            if (Crosshair)
            {
                fg_draw->AddLine(ImVec2(ScreenCenterX, ScreenCenterY + 10), ImVec2(ScreenCenterX, ScreenCenterY - 10), ImColor(255, 255, 255, 255), 2);
                fg_draw->AddLine(ImVec2(ScreenCenterX + 10, ScreenCenterY), ImVec2(ScreenCenterX - 10, ScreenCenterY), ImColor(255, 255, 255, 255), 2);
            }
            Log("Entering Player Loop");
            // can do a thread for 10-15 fps improvement, but good luck!
            Players(mem, game, localPlayer, handle, config);
            Log("Exiting Player Loop");

        }
        Log("Ending Overlay");
        ImGui::End();
        if (Meni)
        {
            SetHRGColors();
            Log("Beginning Menu");
            ImGui::SetNextWindowSize(ImVec2(500, 250));
            ImGui::Begin("Menu", &Meni, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
            {
                // dumb retarded fucking visual studio
                // imgui::Checkbox("checkbox", value);
                // imgui::SameLine();
                // imgui::ColorEdit4(some bullshit ask chatgpt);
                // ^^ for color pickers next to checkboxes
                ImGui::SetCursorPosX((500 - ImGui::CalcTextSize("HRG DMA").x) * 0.5f);
                ImGui::TextColored(ImColor(255, 0, 0, 255), "HRG DMA");
                ImGui::Checkbox("Box esp", &Box);
                ImGui::Checkbox("Crosshair", &Crosshair);
                ImGui::Checkbox("Aimbot",  &Aimbot);
                if (Aimbot)
                {
                    ImGui::Checkbox("Fov", &Fov);
                    if (Fov)
                    {
                        ImGui::SliderFloat("Fov Size", &FovSize, 0, 360);
                        ImGui::SliderInt("Smoothing", &Smoothing, 0, 60);
                    }
                }
                ImGui::Checkbox("Snaplines", &Snapline);
                ImGui::Checkbox("Names", &Names);
                ImGui::Checkbox("Health", &Health);
                ImGui::Checkbox("Skeletons", &Skeleton);
                ImGui::SliderFloat("Validity Dist", &ValidityDist,1600.0f, 1800.0f);
                if (ImGui::BeginCombo("Select Bone", BoneTypeNames[selectedBone])) {
                    for (int i = 0; i < IM_ARRAYSIZE(BoneTypeNames); i++) {
                        bool isSelected = (selectedBone == i);
                        if (ImGui::Selectable(BoneTypeNames[i], isSelected)) {
                            selectedBone = i;
                        }
                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                // if you want a toggle for moonlight support, use a different d3d version, or have different builds
                // clear_color = 25.0f / 255.0f
                // setwindowattributes(hwnd, 0 -> 25
                //ImGui::Checkbox("Moonlight", &moonlight);
            }
            Log("Ending Menu");
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


        DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        if (GetAsyncKeyState(VK_INSERT) & 1)  // toggle overlay interaction with insert key, better way to do this, blocks input when menu is opened
        {
            static bool isInteractive = false;
            isInteractive = !isInteractive;

            if (isInteractive)
            {
                Meni = true;
                SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle & ~WS_EX_TRANSPARENT);
            }
            else
            {
                Meni = false;
                SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle | WS_EX_TRANSPARENT);
            }
        }

        // keep window topmost without forcing focus
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        g_pSwapChain->Present(1, 0); // use vsync
    }
    Log("Exiting Render Loop...");
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}
 // everything else is imgui related, if you dont know what your doing, dont touch they will break!
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
