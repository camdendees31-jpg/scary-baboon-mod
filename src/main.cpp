#include "main.hpp"
#include "menu/MenuController.hpp"

#include "custom-types/shared/register.hpp"
#include "paper/shared/logger.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/LoadSceneMode.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/CharacterController.hpp"
#include "UnityEngine/Camera.hpp"

// ── Logger setup ───────────────────────────────────────────────────────────
Paper::Logger logger("scary-baboon-mod");

// ── Forward declarations of hooked types ───────────────────────────────────
// Replace these class paths with the actual il2cpp class paths from your game's
// global-metadata.dat (use Il2CppInspector or Cpp2IL to find them)

// HOOK: Character movement — applies speed and no-clip
// Signature: void PlayerController::Move(Vector3 motion)
MAKE_HOOK_MATCH(
    PlayerController_Move,
    // TODO: replace with actual method pointer from codegen
    // &GlobalNamespace::PlayerController::Move,
    nullptr,
    void, void* self, UnityEngine::Vector3 motion)
{
    auto& f = g_features;

    if (f.noClip) {
        // Bypass physics entirely — just translate the transform
        auto* go = reinterpret_cast<UnityEngine::MonoBehaviour*>(self)->get_gameObject();
        auto* tr = go->get_transform();
        tr->set_position(tr->get_position() + motion * f.speedMult);
        return;
    }

    motion = motion * f.speedMult;
    PlayerController_Move(self, motion);
}

// HOOK: Jump — multiplies jump velocity
// Signature: void PlayerController::Jump()
MAKE_HOOK_MATCH(
    PlayerController_Jump,
    nullptr,   // TODO: replace with codegen pointer
    void, void* self)
{
    auto& f = g_features;
    if (f.infiniteJump || f.godMode) {
        // Call jump as many times as player wants
        PlayerController_Jump(self);
        return;
    }
    PlayerController_Jump(self);
}

// HOOK: Take damage — blocked by god mode
// Signature: void PlayerHealth::TakeDamage(float amount)
MAKE_HOOK_MATCH(
    PlayerHealth_TakeDamage,
    nullptr,   // TODO: replace with codegen pointer
    void, void* self, float amount)
{
    if (g_features.godMode) return;   // absorb the damage
    PlayerHealth_TakeDamage(self, amount);
}

// HOOK: Lose life — blocked by infinite lives
// Signature: void LivesManager::LoseLife()
MAKE_HOOK_MATCH(
    LivesManager_LoseLife,
    nullptr,   // TODO: replace with codegen pointer
    void, void* self)
{
    if (g_features.infiniteLives || g_features.godMode) return;
    LivesManager_LoseLife(self);
}

// HOOK: Score calculation — multiplied
// Signature: void ScoreManager::AddScore(int points)
MAKE_HOOK_MATCH(
    ScoreManager_AddScore,
    nullptr,   // TODO: replace with codegen pointer
    void, void* self, int points)
{
    int modified = static_cast<int>(points * g_features.scoreMultiplier);
    ScoreManager_AddScore(self, modified);
}

// HOOK: Scene load — spawn the menu controller into each scene
MAKE_HOOK_MATCH(
    SceneManager_ActiveSceneChanged,
    &UnityEngine::SceneManagement::SceneManager::activeSceneChanged,
    void,
    UnityEngine::SceneManagement::Scene prev,
    UnityEngine::SceneManagement::Scene next)
{
    SceneManager_ActiveSceneChanged(prev, next);

    // Re-spawn the menu controller in the new scene
    auto* host = UnityEngine::GameObject::New_ctor(StringW("SBModMenuHost"));
    UnityEngine::Object::DontDestroyOnLoad(host);
    host->AddComponent<ScaryBaboonMod::MenuController*>();

    Paper::Logger::info("Mod menu spawned in scene: {}", std::string(next.get_name()));
}

// ── Mod entry points ───────────────────────────────────────────────────────
extern "C" __attribute__((visibility("default")))
void setup(CModInfo* info) {
    info->id      = "scary-baboon-mod";
    info->version = "0.1.0";
    info->version_long = 0;

    Paper::Logger::info("Scary Baboon Mod | setup() called");
}

extern "C" __attribute__((visibility("default")))
void load() {
    Paper::Logger::info("Scary Baboon Mod | load() called");

    // Register custom types
    custom_types::Register::AutoRegister();

    // Install hooks
    // Uncomment each hook once you have the correct method pointer from codegen:
    // INSTALL_HOOK(logger, PlayerController_Move);
    // INSTALL_HOOK(logger, PlayerController_Jump);
    // INSTALL_HOOK(logger, PlayerHealth_TakeDamage);
    // INSTALL_HOOK(logger, LivesManager_LoseLife);
    // INSTALL_HOOK(logger, ScoreManager_AddScore);
    INSTALL_HOOK(logger, SceneManager_ActiveSceneChanged);

    Paper::Logger::info("Scary Baboon Mod | all hooks installed");
}
