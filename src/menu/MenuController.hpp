#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include <functional>

struct ModFeatures {
    bool godMode = false;
    bool noClip = false;
    bool infiniteJump = false;
    float speedMult = 1.0f;
    bool infiniteLives = false;
    bool freezeTimer = false;
    float timeScale = 1.0f;
    bool autoWin = false;
    bool espEnabled = false;
    bool thirdPerson = false;
    float fovOverride = 90.0f;
    bool unlockAll = false;
    float scoreMultiplier = 1.0f;
};

extern ModFeatures g_features;

namespace ScaryBaboonMod {
    class MenuController {
    public:
        static MenuController* instance;
        static MenuController* GetInstance();
        UnityEngine::GameObject* menuRoot = nullptr;
        bool isVisible = false;
        float yHoldTimer = 0.0f;
        static constexpr float HOLD_THRESHOLD = 0.6f;
        void Awake();
        void Update();
        void BuildMenu();
        void ShowMenu();
        void HideMenu();
        void ToggleMenu();
    };
}
