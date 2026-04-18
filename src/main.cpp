#include "main.hpp"
#include "MenuController.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"

MAKE_HOOK_MATCH(
    SceneManager_ActiveSceneChanged,
    &UnityEngine::SceneManagement::SceneManager::activeSceneChanged,
    void,
    UnityEngine::SceneManagement::Scene prev,
    UnityEngine::SceneManagement::Scene next)
{
    SceneManager_ActiveSceneChanged(prev, next);
    auto* host = UnityEngine::GameObject::New_ctor(StringW("SBModMenuHost"));
    UnityEngine::Object::DontDestroyOnLoad(host);
}

extern "C" __attribute__((visibility("default")))
void setup(CModInfo* info) {
    info->id = "scary-baboon-mod";
    info->version = "0.1.0";
    info->version_long = 0;
}

extern "C" __attribute__((visibility("default")))
void load() {
    INSTALL_HOOK(Logger, SceneManager_ActiveSceneChanged);
}
