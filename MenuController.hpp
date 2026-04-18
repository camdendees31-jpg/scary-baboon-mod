#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/Text.hpp"
#include "UnityEngine/UI/Slider.hpp"
#include "UnityEngine/UI/Toggle.hpp"

// ── Feature state ──────────────────────────────────────────────────────────
struct ModFeatures {
    // Movement
    bool  godMode        = false;
    bool  noClip         = false;
    bool  infiniteJump   = false;
    float speedMult      = 1.0f;
    float jumpHeightMult = 1.0f;

    // Game
    bool  infiniteLives  = false;
    bool  freezeTimer    = false;
    float timeScale      = 1.0f;
    bool  autoWin        = false;

    // Visuals
    bool  espEnabled     = false;
    bool  wireframe      = false;
    bool  thirdPerson    = false;
    float fovOverride    = 90.0f;

    // Misc
    bool  unlockAll      = false;
    bool  silentAim      = false;
    float scoreMultiplier = 1.0f;
};

extern ModFeatures g_features;

// ── MenuController ─────────────────────────────────────────────────────────
DECLARE_CLASS_CODEGEN(ScaryBaboonMod, MenuController, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, menuRoot);
    DECLARE_INSTANCE_FIELD(bool, isVisible);

    // Y-button hold detection
    DECLARE_INSTANCE_FIELD(float, yHoldTimer);
    static constexpr float HOLD_THRESHOLD = 0.6f;   // seconds to hold Y

    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, BuildMenu);
    DECLARE_INSTANCE_METHOD(void, ShowMenu);
    DECLARE_INSTANCE_METHOD(void, HideMenu);
    DECLARE_INSTANCE_METHOD(void, ToggleMenu);

    // Feature toggle callbacks
    DECLARE_INSTANCE_METHOD(void, OnGodModeToggled,        bool value);
    DECLARE_INSTANCE_METHOD(void, OnNoClipToggled,         bool value);
    DECLARE_INSTANCE_METHOD(void, OnInfiniteJumpToggled,   bool value);
    DECLARE_INSTANCE_METHOD(void, OnInfiniteLivesToggled,  bool value);
    DECLARE_INSTANCE_METHOD(void, OnFreezeTimerToggled,    bool value);
    DECLARE_INSTANCE_METHOD(void, OnAutoWinToggled,        bool value);
    DECLARE_INSTANCE_METHOD(void, OnESPToggled,            bool value);
    DECLARE_INSTANCE_METHOD(void, OnWireframeToggled,      bool value);
    DECLARE_INSTANCE_METHOD(void, OnThirdPersonToggled,    bool value);
    DECLARE_INSTANCE_METHOD(void, OnUnlockAllToggled,      bool value);
    DECLARE_INSTANCE_METHOD(void, OnSilentAimToggled,      bool value);

    // Slider callbacks
    DECLARE_INSTANCE_METHOD(void, OnSpeedChanged,          float value);
    DECLARE_INSTANCE_METHOD(void, OnJumpHeightChanged,     float value);
    DECLARE_INSTANCE_METHOD(void, OnTimeScaleChanged,      float value);
    DECLARE_INSTANCE_METHOD(void, OnFOVChanged,            float value);
    DECLARE_INSTANCE_METHOD(void, OnScoreMultChanged,      float value);

    // Helpers
    DECLARE_INSTANCE_METHOD(UnityEngine::UI::Toggle*, AddToggle,
        UnityEngine::Transform* parent, StringW label, bool defaultVal,
        std::function<void(bool)> callback);
    DECLARE_INSTANCE_METHOD(UnityEngine::UI::Slider*, AddSlider,
        UnityEngine::Transform* parent, StringW label,
        float minVal, float maxVal, float defaultVal,
        std::function<void(float)> callback);
    DECLARE_INSTANCE_METHOD(UnityEngine::GameObject*, AddCategory,
        UnityEngine::Transform* parent, StringW title);

    static ScaryBaboonMod::MenuController* instance;
    static ScaryBaboonMod::MenuController* GetInstance();
)
