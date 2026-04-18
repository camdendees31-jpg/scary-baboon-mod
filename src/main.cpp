#include "main.hpp"
#include "MenuController.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

extern "C" __attribute__((visibility("default")))
void setup(CModInfo* info) {
    info->id = "scary-baboon-mod";
    info->version = "0.1.0";
    info->version_long = 0;
}

extern "C" __attribute__((visibility("default")))
void load() {
    Paper::Logger::info("Scary Baboon Mod loaded!");
}
