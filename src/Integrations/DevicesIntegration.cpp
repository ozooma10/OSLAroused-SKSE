#include "DevicesIntegration.h"
#include "Utilities/Utils.h"
#include "Settings.h"
#include <Managers/LibidoManager.h>

WornDeviceField GetWornDevices(RE::Actor* actorRef, RE::TESForm* equipmentToIgnore)
{
	WornDeviceField wornDevices;

	//Get Equipped items
	const auto actorEquipment = actorRef->GetInventory([=](RE::TESBoundObject& a_object) {
		return a_object.IsArmor();
	});

	//Get set of  keywords for worn items
	std::set<RE::FormID> wornArmorKeywordIds;
	for (const auto& [item, invData] : actorEquipment) {
		const auto& [count, entry] = invData;
		if (count > 0 && entry->IsWorn() && (!equipmentToIgnore || item->formID != equipmentToIgnore->formID)) {
			if (const auto keywordForm = item->As<RE::BGSKeywordForm>()) {
				for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
					wornArmorKeywordIds.insert(keywordForm->keywords[i]->formID);
				}
			}
		}
	}
	const auto DI = DevicesIntegration::GetSingleton();

	for (const auto keywordFormId : wornArmorKeywordIds) {
		//Soup World USA
		if (keywordFormId == DI->ToysPelvis || keywordFormId == DI->DDBelt) {
			wornDevices.Belt = true;
		} else if (keywordFormId == DI->ToysArms || keywordFormId == DI->DDArmCuffs) {
			wornDevices.ArmCuffs = true;
		} else if (keywordFormId == DI->ToysEyes || keywordFormId == DI->DDBlindfold) {
			wornDevices.Blindfold = true;
		} else if (keywordFormId == DI->DDBondageMittens) {
			wornDevices.BondageMittens = true;
		} else if (keywordFormId == DI->ToysFeet || keywordFormId == DI->DDBoots) {
			wornDevices.Boots = true;
		} else if (keywordFormId == DI->ToysBreasts || keywordFormId == DI->DDBra) {
			wornDevices.Bra = true;
		} else if (keywordFormId == DI->ToysTorso || keywordFormId == DI->DDCorset) {
			wornDevices.Corset = true;
		} else if (keywordFormId == DI->ToysMouth || keywordFormId == DI->DDGag) {
			wornDevices.Gag = true;
		} else if (keywordFormId == DI->ToysHands || keywordFormId == DI->DDGloves) {
			wornDevices.Gloves = true;
		} else if (keywordFormId == DI->DDHarness) {
			wornDevices.Harness = true;
		} else if (keywordFormId == DI->ToysWrists || keywordFormId == DI->DDHeavyBondage) {
			wornDevices.HeavyBondage = true;
		} else if (keywordFormId == DI->DDHood) {
			wornDevices.Hood = true;
		} else if (keywordFormId == DI->ToysLegs || keywordFormId == DI->DDLegCuffs) {
			wornDevices.LegCuffs = true;
		} else if (keywordFormId == DI->ToysNipples || keywordFormId == DI->DDPiercingsNipple) {
			wornDevices.PiercingsNipple = true;
		} else if (keywordFormId == DI->ToysGenital || keywordFormId == DI->DDPiercingsVaginal) {
			wornDevices.PiercingsVaginal = true;
		} else if (keywordFormId == DI->ToysAnal || keywordFormId == DI->DDPlugAnal) {
			wornDevices.PlugAnal = true;
			logger::info("AN found on device its true");
		} else if (keywordFormId == DI->ToysVaginal || keywordFormId == DI->DDPlugVaginal) {
			wornDevices.PlugVaginal = true;
		} else if (keywordFormId == DI->DDSuit) {
			wornDevices.Suit = true;
		}
	}

	return wornDevices;
}

WornDeviceField GetWornDevicesCache(RE::Actor* actorRef)
{
	return GetWornDevices(actorRef, nullptr);
}

float DevicesIntegration::GetArousalBaselineFromDevices(RE::Actor* actorRef)
{
	const auto wornDevices = GetActorWornDevices(actorRef);

	const auto deviceConfig = Settings::GetSingleton()->GetDeviceBaseline();

	float arousalBaselineGain = 0.0f;

	if (wornDevices.ArmCuffs) {
		arousalBaselineGain += deviceConfig.ArmCuffs;
	}
	if (wornDevices.Belt) {
		arousalBaselineGain += deviceConfig.Belt;
	}
	if (wornDevices.Blindfold) {
		arousalBaselineGain += deviceConfig.Blindfold;
	}
	if (wornDevices.BondageMittens) {
		arousalBaselineGain += deviceConfig.BondageMittens;
	}
	if (wornDevices.Boots) {
		arousalBaselineGain += deviceConfig.Boots;
	}
	if (wornDevices.Bra) {
		arousalBaselineGain += deviceConfig.Bra;
	}
	if (wornDevices.Collar) {
		arousalBaselineGain += deviceConfig.Collar;
	}
	if (wornDevices.Corset) {
		arousalBaselineGain += deviceConfig.Corset;
	}
	if (wornDevices.Gag) {
		arousalBaselineGain += deviceConfig.Gag;
	}
	if (wornDevices.Gloves) {
		arousalBaselineGain += deviceConfig.Gloves;
	}
	if (wornDevices.Harness) {
		arousalBaselineGain += deviceConfig.Harness;
	}
	if (wornDevices.HeavyBondage) {
		arousalBaselineGain += deviceConfig.HeavyBondage;
	}
	if (wornDevices.Hood) {
		arousalBaselineGain += deviceConfig.Hood;
	}
	if (wornDevices.LegCuffs) {
		arousalBaselineGain += deviceConfig.LegCuffs;
	}
	if (wornDevices.PiercingsNipple) {
		arousalBaselineGain += deviceConfig.PiercingsNipple;
	}
	if (wornDevices.PiercingsVaginal) {
		arousalBaselineGain += deviceConfig.PiercingsVaginal;
	}
	if (wornDevices.PlugAnal) {
		logger::info("An Worn");
		arousalBaselineGain += deviceConfig.PlugAnal;
	}
	if (wornDevices.PlugVaginal) {
		logger::info("Vag Worn");
		arousalBaselineGain += deviceConfig.PlugVaginal;
	}
	if (wornDevices.Suit) {
		arousalBaselineGain += deviceConfig.Suit;
	}
	return arousalBaselineGain;
}

WornDeviceField DevicesIntegration::GetActorWornDevices(RE::Actor* actorRef)
{
	return m_ActorWornDeviceCache(actorRef);
}

void DevicesIntegration::ActiveEquipmentChanged(RE::Actor* actorRef, RE::TESForm* changedEquipment, bool bEquipped)
{
	float existingValue = GetArousalBaselineFromDevices(actorRef);
	const auto actorWornItems = GetWornDevices(actorRef, bEquipped ? nullptr : changedEquipment);
	m_ActorWornDeviceCache.UpdateItem(actorRef, actorWornItems);
	float updatedVal = GetArousalBaselineFromDevices(actorRef);

	if (existingValue != updatedVal) {
		LibidoManager::GetSingleton()->ActorLibidoModifiersUpdated(actorRef);
	}
}

void DevicesIntegration::Initialize()
{
	//We need to get the mod index of dd/toys, for checking keywords
	const auto dataHandler = RE::TESDataHandler::GetSingleton();

	if (const auto ddAssetsInfo = dataHandler->LookupModByName("Devious Devices - Assets.esm")) {
		m_DDAssetsModIndex = ddAssetsInfo->GetPartialIndex();
	}
	if (const auto ddIntegrationInfo = dataHandler->LookupModByName("Devious Devices - Integration.esm")) {
		m_DDIntegrationModIndex = ddIntegrationInfo->GetPartialIndex();
	}
	if (const auto toysInfo = dataHandler->LookupModByName("Toys.esm")) {
		m_ToysModIndex = toysInfo->GetPartialIndex();
	}

	using namespace Utilities::Forms;
	using namespace DeviousDeviceKeywordFormIds;

	DDBelt = ResolveFormId(m_DDAssetsModIndex, DeviousBelt);
	DDCollar = ResolveFormId(m_DDAssetsModIndex, DeviousCollar);
	DDLegCuffs = ResolveFormId(m_DDAssetsModIndex, DeviousLegCuffs);
	DDArmCuffs = ResolveFormId(m_DDAssetsModIndex, DeviousArmCuffs);
	DDBra = ResolveFormId(m_DDAssetsModIndex, DeviousBra);
	DDGag = ResolveFormId(m_DDAssetsModIndex, DeviousGag);
	DDPiercingsNipple = ResolveFormId(m_DDAssetsModIndex, DeviousPiercingsNipple);
	DDPiercingsVaginal = ResolveFormId(m_DDAssetsModIndex, DeviousPiercingsVaginal);
	DDBlindfold = ResolveFormId(m_DDAssetsModIndex, DeviousBlindfold);
	DDHarness = ResolveFormId(m_DDAssetsModIndex, DeviousHarness);
	DDPlugVaginal = ResolveFormId(m_DDAssetsModIndex, DeviousPlugVaginal);
	DDPlugAnal = ResolveFormId(m_DDAssetsModIndex, DeviousPlugAnal);
	DDCorset = ResolveFormId(m_DDAssetsModIndex, DeviousCorset);
	DDBoots = ResolveFormId(m_DDAssetsModIndex, DeviousBoots);
	DDGloves = ResolveFormId(m_DDAssetsModIndex, DeviousGloves);
	DDHood = ResolveFormId(m_DDAssetsModIndex, DeviousHood);
	DDSuit = ResolveFormId(m_DDAssetsModIndex, DeviousSuit);
	DDHeavyBondage = ResolveFormId(m_DDIntegrationModIndex, DeviousHeavyBondage);
	DDBondageMittens = ResolveFormId(m_DDIntegrationModIndex, DeviousBondageMittens);

	using namespace ToysKeywordFormIds;
	ToysVaginal = ResolveFormId(m_ToysModIndex, Vaginal);
	ToysHands = ResolveFormId(m_ToysModIndex, Hands);
	ToysFeet = ResolveFormId(m_ToysModIndex, Feet);
	ToysMouth = ResolveFormId(m_ToysModIndex, Mouth);
	ToysNeck = ResolveFormId(m_ToysModIndex, Neck);
	ToysWrists = ResolveFormId(m_ToysModIndex, Wrists);
	ToysAnal = ResolveFormId(m_ToysModIndex, Anal);
	ToysPelvis = ResolveFormId(m_ToysModIndex, Pelvis);
	ToysGenital = ResolveFormId(m_ToysModIndex, Genital);
	ToysNipples = ResolveFormId(m_ToysModIndex, Nipples);
	ToysLegs = ResolveFormId(m_ToysModIndex, Legs);
	ToysEyes = ResolveFormId(m_ToysModIndex, Eyes);
	ToysBreasts = ResolveFormId(m_ToysModIndex, Breasts);
	ToysTorso = ResolveFormId(m_ToysModIndex, Torso);
	ToysArms = ResolveFormId(m_ToysModIndex, Arms);
}
