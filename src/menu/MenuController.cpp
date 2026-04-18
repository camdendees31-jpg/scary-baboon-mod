#include "menu/MenuController.hpp"
#include "main.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/CanvasScaler.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Slider.hpp"
#include "UnityEngine/UI/Text.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/XR/InputDevice.hpp"
#include "UnityEngine/XR/CommonUsages.hpp"
#include "UnityEngine/XR/InputDeviceRole.hpp"
#include "UnityEngine/XR/XRNode.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Screen.hpp"
#include "UnityEngine/Font.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/OVRInput.hpp"

DEFINE_TYPE(ScaryBaboonMod, MenuController);

ModFeatures g_features;
ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::instance = nullptr;

// ── Static accessor ────────────────────────────────────────────────────────
ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::GetInstance() {
    return instance;
}

// ── Lifecycle ──────────────────────────────────────────────────────────────
void ScaryBaboonMod::MenuController::Awake() {
    instance    = this;
    isVisible   = false;
    yHoldTimer  = 0.0f;
    BuildMenu();
    HideMenu();
    Paper::Logger::info("MenuController awake");
}

void ScaryBaboonMod::MenuController::Update() {
    // ── Y-button hold detection ──────────────────────────────────────────
    bool yPressed = GlobalNamespace::OVRInput::Get(
        GlobalNamespace::OVRInput::Button::Two,          // Y button
        GlobalNamespace::OVRInput::Controller::LTouch    // left Touch controller
    );

    if (yPressed) {
        yHoldTimer += UnityEngine::Time::get_deltaTime();
        if (yHoldTimer >= HOLD_THRESHOLD) {
            yHoldTimer = 0.0f;   // reset so it doesn't re-fire every frame
            ToggleMenu();
        }
    } else {
        yHoldTimer = 0.0f;
    }

    // ── Feature tick logic ───────────────────────────────────────────────
    if (g_features.freezeTimer)
        UnityEngine::Time::set_timeScale(0.0f);
    else
        UnityEngine::Time::set_timeScale(g_features.timeScale);
}

// ── Menu visibility ────────────────────────────────────────────────────────
void ScaryBaboonMod::MenuController::ShowMenu() {
    if (menuRoot)
        menuRoot->SetActive(true);
    isVisible = true;
}

void ScaryBaboonMod::MenuController::HideMenu() {
    if (menuRoot)
        menuRoot->SetActive(false);
    isVisible = false;
}

void ScaryBaboonMod::MenuController::ToggleMenu() {
    isVisible ? HideMenu() : ShowMenu();
}

// ── Build the UI ───────────────────────────────────────────────────────────
void ScaryBaboonMod::MenuController::BuildMenu() {
    // World-space canvas so it floats in front of the player
    menuRoot = UnityEngine::GameObject::New_ctor(StringW("SBModMenu"));
    UnityEngine::Object::DontDestroyOnLoad(menuRoot);

    auto* canvas = menuRoot->AddComponent<UnityEngine::Canvas*>();
    canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);
    canvas->set_sortingOrder(100);

    auto* scaler = menuRoot->AddComponent<UnityEngine::CanvasScaler*>();
    scaler->set_dynamicPixelsPerUnit(100.0f);

    // Root rect — 0.8 × 1.2 m panel sitting at arm's length
    auto* rt = menuRoot->GetComponent<UnityEngine::RectTransform*>();
    rt->set_sizeDelta({800.0f, 1200.0f});
    rt->set_localScale({0.001f, 0.001f, 0.001f});
    rt->set_localPosition({0.0f, 1.4f, 1.5f});  // in front of player

    // Background panel
    auto* bgGO  = UnityEngine::GameObject::New_ctor(StringW("Background"));
    bgGO->get_transform()->SetParent(menuRoot->get_transform(), false);
    auto* bgImg = bgGO->AddComponent<UnityEngine::UI::Image*>();
    bgImg->set_color({0.05f, 0.05f, 0.08f, 0.92f});
    auto* bgRT  = bgGO->GetComponent<UnityEngine::RectTransform*>();
    bgRT->set_anchorMin({0, 0});
    bgRT->set_anchorMax({1, 1});
    bgRT->set_offsetMin({0, 0});
    bgRT->set_offsetMax({0, 0});

    // Title
    auto* titleGO = UnityEngine::GameObject::New_ctor(StringW("Title"));
    titleGO->get_transform()->SetParent(bgGO->get_transform(), false);
    auto* titleTxt = titleGO->AddComponent<UnityEngine::UI::Text*>();
    titleTxt->set_text(StringW("Scary Baboon Mod Menu"));
    titleTxt->set_fontSize(40);
    titleTxt->set_alignment(UnityEngine::TextAnchor::MiddleCenter);
    titleTxt->set_color({0.9f, 0.3f, 0.3f, 1.0f});   // red title
    auto* titleRT = titleGO->GetComponent<UnityEngine::RectTransform*>();
    titleRT->set_anchorMin({0, 0.92f});
    titleRT->set_anchorMax({1, 1.0f});
    titleRT->set_offsetMin({10, 0});
    titleRT->set_offsetMax({-10, 0});

    // Scrollable content area
    auto* contentGO = UnityEngine::GameObject::New_ctor(StringW("Content"));
    contentGO->get_transform()->SetParent(bgGO->get_transform(), false);
    auto* layout = contentGO->AddComponent<UnityEngine::UI::VerticalLayoutGroup*>();
    layout->set_spacing(8.0f);
    layout->set_padding({20, 20, 10, 10});
    layout->set_childControlWidth(true);
    layout->set_childForceExpandWidth(true);
    auto* cRT = contentGO->GetComponent<UnityEngine::RectTransform*>();
    cRT->set_anchorMin({0, 0});
    cRT->set_anchorMax({1, 0.91f});
    cRT->set_offsetMin({0, 0});
    cRT->set_offsetMax({0, 0});

    auto* t = contentGO->get_transform();

    // ── Category: Movement ───────────────────────────────────────────────
    AddCategory(t, "Movement");
    AddToggle(t, "God Mode",        false, [this](bool v){ OnGodModeToggled(v); });
    AddToggle(t, "No Clip",         false, [this](bool v){ OnNoClipToggled(v); });
    AddToggle(t, "Infinite Jump",   false, [this](bool v){ OnInfiniteJumpToggled(v); });
    AddSlider(t, "Speed",           0.5f, 5.0f,  1.0f, [this](float v){ OnSpeedChanged(v); });
    AddSlider(t, "Jump Height",     0.5f, 5.0f,  1.0f, [this](float v){ OnJumpHeightChanged(v); });

    // ── Category: Game ───────────────────────────────────────────────────
    AddCategory(t, "Game");
    AddToggle(t, "Infinite Lives",  false, [this](bool v){ OnInfiniteLivesToggled(v); });
    AddToggle(t, "Freeze Timer",    false, [this](bool v){ OnFreezeTimerToggled(v); });
    AddToggle(t, "Auto Win",        false, [this](bool v){ OnAutoWinToggled(v); });
    AddSlider(t, "Time Scale",      0.1f, 3.0f,  1.0f, [this](float v){ OnTimeScaleChanged(v); });
    AddSlider(t, "Score x",         1.0f, 10.0f, 1.0f, [this](float v){ OnScoreMultChanged(v); });

    // ── Category: Visuals ────────────────────────────────────────────────
    AddCategory(t, "Visuals");
    AddToggle(t, "ESP / Outlines",  false, [this](bool v){ OnESPToggled(v); });
    AddToggle(t, "Wireframe",       false, [this](bool v){ OnWireframeToggled(v); });
    AddToggle(t, "Third Person",    false, [this](bool v){ OnThirdPersonToggled(v); });
    AddSlider(t, "FOV",             60.0f, 150.0f, 90.0f, [this](float v){ OnFOVChanged(v); });

    // ── Category: Misc ───────────────────────────────────────────────────
    AddCategory(t, "Misc");
    AddToggle(t, "Unlock All",      false, [this](bool v){ OnUnlockAllToggled(v); });
    AddToggle(t, "Silent Aim",      false, [this](bool v){ OnSilentAimToggled(v); });
}

// ── Feature callbacks ──────────────────────────────────────────────────────
void ScaryBaboonMod::MenuController::OnGodModeToggled(bool v)       { g_features.godMode = v;          Paper::Logger::info("God mode: {}", v); }
void ScaryBaboonMod::MenuController::OnNoClipToggled(bool v)        { g_features.noClip = v;           Paper::Logger::info("No clip: {}", v); }
void ScaryBaboonMod::MenuController::OnInfiniteJumpToggled(bool v)  { g_features.infiniteJump = v;     Paper::Logger::info("Inf jump: {}", v); }
void ScaryBaboonMod::MenuController::OnInfiniteLivesToggled(bool v) { g_features.infiniteLives = v;    Paper::Logger::info("Inf lives: {}", v); }
void ScaryBaboonMod::MenuController::OnFreezeTimerToggled(bool v)   { g_features.freezeTimer = v;      Paper::Logger::info("Freeze timer: {}", v); }
void ScaryBaboonMod::MenuController::OnAutoWinToggled(bool v)       { g_features.autoWin = v;          Paper::Logger::info("Auto win: {}", v); }
void ScaryBaboonMod::MenuController::OnESPToggled(bool v)           { g_features.espEnabled = v;       Paper::Logger::info("ESP: {}", v); }
void ScaryBaboonMod::MenuController::OnWireframeToggled(bool v)     { g_features.wireframe = v;        Paper::Logger::info("Wireframe: {}", v); }
void ScaryBaboonMod::MenuController::OnThirdPersonToggled(bool v)   { g_features.thirdPerson = v;      Paper::Logger::info("Third person: {}", v); }
void ScaryBaboonMod::MenuController::OnUnlockAllToggled(bool v)     { g_features.unlockAll = v;        Paper::Logger::info("Unlock all: {}", v); }
void ScaryBaboonMod::MenuController::OnSilentAimToggled(bool v)     { g_features.silentAim = v;        Paper::Logger::info("Silent aim: {}", v); }

void ScaryBaboonMod::MenuController::OnSpeedChanged(float v)        { g_features.speedMult = v;        Paper::Logger::info("Speed: {}x", v); }
void ScaryBaboonMod::MenuController::OnJumpHeightChanged(float v)   { g_features.jumpHeightMult = v;   Paper::Logger::info("Jump height: {}x", v); }
void ScaryBaboonMod::MenuController::OnTimeScaleChanged(float v)    { g_features.timeScale = v;        Paper::Logger::info("Time scale: {}", v); }
void ScaryBaboonMod::MenuController::OnFOVChanged(float v)          { g_features.fovOverride = v;
    auto* cam = UnityEngine::Camera::get_main();
    if (cam) cam->set_fieldOfView(v);
    Paper::Logger::info("FOV: {}", v);
}
void ScaryBaboonMod::MenuController::OnScoreMultChanged(float v)    { g_features.scoreMultiplier = v;  Paper::Logger::info("Score mult: {}x", v); }

// ── UI helper builders ─────────────────────────────────────────────────────
UnityEngine::GameObject* ScaryBaboonMod::MenuController::AddCategory(
    UnityEngine::Transform* parent, StringW title)
{
    auto* go  = UnityEngine::GameObject::New_ctor(title);
    go->get_transform()->SetParent(parent, false);

    auto* le  = go->AddComponent<UnityEngine::UI::LayoutElement*>();
    le->set_preferredHeight(36.0f);

    auto* txt = go->AddComponent<UnityEngine::UI::Text*>();
    txt->set_text(StringW("── " + std::string(title) + " ──"));
    txt->set_fontSize(28);
    txt->set_color({0.8f, 0.5f, 0.1f, 1.0f});   // amber section header
    txt->set_alignment(UnityEngine::TextAnchor::MiddleLeft);
    return go;
}

UnityEngine::UI::Toggle* ScaryBaboonMod::MenuController::AddToggle(
    UnityEngine::Transform* parent, StringW label, bool defaultVal,
    std::function<void(bool)> callback)
{
    auto* rowGO  = UnityEngine::GameObject::New_ctor(StringW("Toggle_" + std::string(label)));
    rowGO->get_transform()->SetParent(parent, false);

    auto* hLayout = rowGO->AddComponent<UnityEngine::UI::HorizontalLayoutGroup*>();
    hLayout->set_spacing(12.0f);
    hLayout->set_childControlHeight(true);
    hLayout->set_childForceExpandHeight(false);

    auto* le = rowGO->AddComponent<UnityEngine::UI::LayoutElement*>();
    le->set_preferredHeight(44.0f);

    // Label
    auto* lblGO = UnityEngine::GameObject::New_ctor(StringW("Label"));
    lblGO->get_transform()->SetParent(rowGO->get_transform(), false);
    auto* lbl   = lblGO->AddComponent<UnityEngine::UI::Text*>();
    lbl->set_text(label);
    lbl->set_fontSize(24);
    lbl->set_color({0.9f, 0.9f, 0.9f, 1.0f});
    lbl->set_alignment(UnityEngine::TextAnchor::MiddleLeft);
    auto* lblLE = lblGO->AddComponent<UnityEngine::UI::LayoutElement*>();
    lblLE->set_flexibleWidth(1.0f);

    // Toggle
    auto* tglGO  = UnityEngine::GameObject::New_ctor(StringW("Toggle"));
    tglGO->get_transform()->SetParent(rowGO->get_transform(), false);
    auto* tgl    = tglGO->AddComponent<UnityEngine::UI::Toggle*>();
    tgl->set_isOn(defaultVal);
    tgl->onValueChanged->AddListener(
        UnityEngine::Events::UnityAction_1<bool>::New_ctor(
            reinterpret_cast<void*>(&callback),
            [](void* data, bool v){ (*reinterpret_cast<std::function<void(bool)>*>(data))(v); }
        )
    );

    return tgl;
}

UnityEngine::UI::Slider* ScaryBaboonMod::MenuController::AddSlider(
    UnityEngine::Transform* parent, StringW label,
    float minVal, float maxVal, float defaultVal,
    std::function<void(float)> callback)
{
    auto* rowGO = UnityEngine::GameObject::New_ctor(StringW("Slider_" + std::string(label)));
    rowGO->get_transform()->SetParent(parent, false);

    auto* vLayout = rowGO->AddComponent<UnityEngine::UI::VerticalLayoutGroup*>();
    vLayout->set_spacing(4.0f);
    vLayout->set_childControlWidth(true);
    vLayout->set_childForceExpandWidth(true);

    auto* le = rowGO->AddComponent<UnityEngine::UI::LayoutElement*>();
    le->set_preferredHeight(60.0f);

    // Label + value readout
    auto* lblGO = UnityEngine::GameObject::New_ctor(StringW("Label"));
    lblGO->get_transform()->SetParent(rowGO->get_transform(), false);
    auto* lbl   = lblGO->AddComponent<UnityEngine::UI::Text*>();
    lbl->set_text(StringW(std::string(label) + ": " + std::to_string(defaultVal)));
    lbl->set_fontSize(22);
    lbl->set_color({0.85f, 0.85f, 0.85f, 1.0f});

    // Slider
    auto* sldGO = UnityEngine::GameObject::New_ctor(StringW("Slider"));
    sldGO->get_transform()->SetParent(rowGO->get_transform(), false);
    auto* sld   = sldGO->AddComponent<UnityEngine::UI::Slider*>();
    sld->set_minValue(minVal);
    sld->set_maxValue(maxVal);
    sld->set_value(defaultVal);

    // Capture lbl and label string for live readout
    auto labelStr = std::string(label);
    auto wrappedCb = [lbl, labelStr, callback](float v) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f", v);
        lbl->set_text(StringW(labelStr + ": " + buf));
        callback(v);
    };

    sld->onValueChanged->AddListener(
        UnityEngine::Events::UnityAction_1<float>::New_ctor(
            new std::function<void(float)>(wrappedCb),
            [](void* data, float v){ (*reinterpret_cast<std::function<void(float)>*>(data))(v); }
        )
    );

    return sld;
}
