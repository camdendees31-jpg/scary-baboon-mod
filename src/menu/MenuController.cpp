#include "MenuController.hpp"
#include "logger.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"

ModFeatures g_features;
ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::instance = nullptr;

ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::GetInstance() { return instance; }

void ScaryBaboonMod::MenuController::Awake() {
    instance = this;
    isVisible = false;
    yHoldTimer = 0.0f;
    BuildMenu();
    HideMenu();
}

void ScaryBaboonMod::MenuController::Update() {
    bool yPressed = GlobalNamespace::OVRInput::Get(
        GlobalNamespace::OVRInput::Button::Two,
        GlobalNamespace::OVRInput::Controller::LTouch
    );
    if (yPressed) {
        yHoldTimer += UnityEngine::Time::get_deltaTime();
        if (yHoldTimer >= HOLD_THRESHOLD) { yHoldTimer = 0.0f; ToggleMenu(); }
    } else { yHoldTimer = 0.0f; }
    UnityEngine::Time::set_timeScale(g_features.freezeTimer ? 0.0f : g_features.timeScale);
}

void ScaryBaboonMod::MenuController::ShowMenu() { if (menuRoot) menuRoot->SetActive(true); isVisible = true; }
void ScaryBaboonMod::MenuController::HideMenu() { if (menuRoot) menuRoot->SetActive(false); isVisible = false; }
void ScaryBaboonMod::MenuController::ToggleMenu() { isVisible ? HideMenu() : ShowMenu(); }

void ScaryBaboonMod::MenuController::BuildMenu() {
    menuRoot = UnityEngine::GameObject::New_ctor(StringW("SBModMenu"));
    UnityEngine::Object::DontDestroyOnLoad(menuRoot);
}
