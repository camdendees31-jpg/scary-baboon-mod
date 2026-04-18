#include "MenuController.hpp"
#include "main.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/CanvasScaler.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Text.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Slider.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

ModFeatures g_features;
ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::instance = nullptr;

ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::GetInstance() {
    return instance;
}

void ScaryBaboonMod::MenuController::Awake() {
    instance   = this;
    isVisible  = false;
    yHoldTimer = 0.0f;
    BuildMenu();
    HideMenu();
    Paper::Logger::info("MenuController awake");
}

void ScaryBaboonMod::MenuController::Update() {
    // Y-button hold
    bool yPressed = GlobalNamespace::OVRInput::Get(
        GlobalNamespace::OVRInput::Button::Two,
        GlobalNamespace::OVRInput::Controller::LTouch
    );
    if (yPressed) {
        yHoldTimer += UnityEngine::Time::get_deltaTime();
        if (yHoldTimer >= HOLD_THRESHOLD) {
            yHoldTimer = 0.0f;
            ToggleMenu();
        }
    } else {
        yHoldTimer = 0.0f;
    }

    if (g_features.freezeTimer)
        UnityEngine::Time::set_timeScale(0.0f);
    else
        UnityEngine::Time::set_timeScale(g_features.timeScale);
}

void ScaryBaboonMod::MenuController::ShowMenu() {
    if (menuRoot) menuRoot->SetActive(true);
    isVisible = true;
}

void ScaryBaboonMod::MenuController::HideMenu() {
    if (menuRoot) menuRoot->SetActive(false);
    isVisible = false;
}

void ScaryBaboonMod::MenuController::ToggleMenu() {
    isVisible ? HideMenu() : ShowMenu();
}

void ScaryBaboonMod::MenuController::BuildMenu() {
    menuRoot = UnityEngine::GameObject::New_ctor(StringW("SBModMenu"));
    UnityEngine::Object::DontDestroyOnLoad(menuRoot);

    auto* canvas = menuRoot->AddComponent<UnityEngine::Canvas*>();
    canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);
    canvas->set_sortingOrder(100);

    auto* rt = menuRoot->GetComponent<UnityEngine::RectTransform*>();
    rt->set_sizeDelta({800.0f, 1200.0f});
    rt->set_localScale({0.001f, 0.001f, 0.001f});
    rt->set_localPosition({0.0f, 1.4f, 1.5f});

    Paper::Logger::info("Menu built");
}
