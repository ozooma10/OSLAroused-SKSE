#include "Hooks.h"

#include "RuntimeEvents.h"
#include "Papyrus.h"
#include "PapyrusInterface.h"
#include "Utilities/Utils.h"
#include "PersistedData.h"
#include "Papyrus/PapyrusConfig.h"

namespace
{
	void InitializeLog()
	{
		auto path = logger::log_directory();
		if (!path) {
			util::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format("{}.log"sv, Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

		const auto level = spdlog::level::trace;

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v"s);
	}

	void MessageHandler(SKSE::MessagingInterface::Message* message)
	{
		switch (message->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			RuntimeEvents::OnEquipEvent::RegisterEvent();
			logger::info("OnEquipEvent Registered...");

			WorldChecks::AurousalUpdateTicker::GetSingleton()->Start();

			break;
		case SKSE::MessagingInterface::kPostLoadGame:
			//Distribute Keywords
			Utilities::Keywords::DistributeKeywords();

			break;
		}
	}
}

#if defined(SKYRIMSSE)
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver <
		SKSE::RUNTIME_1_5_39
	) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}
#else
extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;

	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);

	v.UsesAddressLibrary(true);
	v.CompatibleVersions({ SKSE::RUNTIME_LATEST });

	return v;
}();
#endif

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION.string());

	SKSE::Init(a_skse);

	const auto papyrus = SKSE::GetPapyrusInterface();
	papyrus->Register(Papyrus::RegisterFunctions);
	papyrus->Register(PapyrusInterface::RegisterFunctions);
	papyrus->Register(PapyrusConfig::RegisterFunctions);

	const auto serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(PersistedData::kArousalDataKey);
	serialization->SetSaveCallback(PersistedData::SaveCallback);
	serialization->SetLoadCallback(PersistedData::LoadCallback);
	serialization->SetRevertCallback(PersistedData::RevertCallback);

	auto messagingInterface = SKSE::GetMessagingInterface();
	if (!messagingInterface->RegisterListener(MessageHandler)) {
		return false;
	}

	return true;
}
