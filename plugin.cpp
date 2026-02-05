#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace HealFollowersSKSE {
    static RE::AlchemyItem* FindBestHealPotionImpl(RE::Actor* a_actor, RE::BGSKeyword* a_restoreKW) {
        if (!a_actor || !a_restoreKW) {
            return nullptr;
        }

        const auto invCounts = a_actor->GetInventoryCounts(
            [](RE::TESBoundObject& a_obj) { return a_obj.GetFormType() == RE::FormType::AlchemyItem; });

        RE::AlchemyItem* best = nullptr;
        std::uint32_t bestValue = 0;

        for (const auto& [obj, count] : invCounts) {
            if (!obj || count <= 0) {
                continue;
            }

            auto* alch = obj->As<RE::AlchemyItem>();
            if (!alch) {
                continue;
            }

            if (alch->IsFood()) {
                continue;
            }

            if (alch->effects.size() != 1) {
                continue;
            }

            if (!alch->HasKeyword(a_restoreKW)) {
                continue;
            }

            const auto value = alch->GetGoldValue();
            if (!best || value < bestValue) {
                best = alch;
                bestValue = value;
            }
        }

        return best;
    }

    static RE::AlchemyItem* FindBestHealPotion(RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSKeyword* a_restoreKW) {
        return FindBestHealPotionImpl(a_actor, a_restoreKW);
    }

    static bool BindPapyrusFunctions(RE::BSScript::IVirtualMachine* a_vm) { a_vm->RegisterFunction("FindBestHealPotion", "HealFollowers_SKSE", FindBestHealPotion);
        return true;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
    SKSE::Init(a_skse);
    SKSE::GetPapyrusInterface()->Register(HealFollowersSKSE::BindPapyrusFunctions);
    return true;
}