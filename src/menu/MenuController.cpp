#include "MenuController.hpp"
#include "main.hpp"

ModFeatures g_features;
ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::instance = nullptr;

ScaryBaboonMod::MenuController* ScaryBaboonMod::MenuController::GetInstance() { return instance; }

void ScaryBaboonMod::MenuController::Awake() {
    instance = this;
    isVisible = false;
    yHoldTimer = 0.0f;
}

void ScaryBaboonMod::MenuController::Update() {
    // Y-hold to toggle menu - OVR hook goes here once codegen is available
    yHoldTimer = 0.0f;
}

void ScaryBaboonMod::MenuController::ShowMenu()  { isVisible = true; }
void ScaryBaboonMod::MenuController::HideMenu()  { isVisible = false; }
void ScaryBaboonMod::MenuController::ToggleMenu() { isVisible ? HideMenu() : ShowMenu(); }
void ScaryBaboonMod::MenuController::BuildMenu()  { }
