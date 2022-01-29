#include "C:/Users/jel-m/Documents/Projects/OSLAroused-SKSE/build/src/CMakeFiles/OSLAroused_SKSE.dir/Debug/cmake_pch.hxx"
#include "Serialization.h"
//This Serialization is based off how powerof3's did it in Afterlife

namespace Serialization
{
	float BaseData::GetData(uint64_t formId)
	{
		Locker locker(m_Lock);

		if (auto idx = m_Data.find(formId) != m_Data.end()) {
			return m_Data[formId];
		}

		return 0.f;
	}

	void BaseData::SetData(uint64_t formId, float value)
	{
		Locker locker(m_Lock);
		m_Data[formId] = value;
	}

	bool BaseData::Save(SKSE::SerializationInterface* serializationInterface, std::uint32_t type, std::uint32_t version)
	{
		if (!serializationInterface->OpenRecord(type, version)) {
			logger::error("Failed to open record for Data Serialization!");
			return false;
		}
		
		return Save(serializationInterface);
	}

	bool BaseData::Save(SKSE::SerializationInterface* serializationInterface)
	{
		assert(serializationInterface);
		Locker locker(m_Lock);

		const auto numRecords = m_Data.size();
		if (!serializationInterface->WriteRecordData(numRecords)) {
			logger::error("Failed to save {} data records", numRecords);
			return false;
		}

		for (const auto& [formId, value] : m_Data) {
			if (!serializationInterface->WriteRecordData(formId)) {
				logger::error("Failed to save data for FormID: ({:X})", formId);
				return false;
			}

			if (!serializationInterface->WriteRecordData(value)) {
				logger::error("Failed to save data for value: ({})", value);
				return false;
			}
		}

		logger::info("{} - {} actor form data saved", GetType(), numRecords);
		return true;
	}

	bool BaseData::Load(SKSE::SerializationInterface* serializationInterface)
	{
		assert(serializationInterface);
		
		std::size_t recordDataSize;
		serializationInterface->ReadRecordData(recordDataSize);

		Locker locker(m_Lock);
		m_Data.clear();

		RE::FormID formID;
		float value;

		for (auto i = 0; i < recordDataSize; i++) {
			serializationInterface->ReadRecordData(formID);
			if (!serializationInterface->ResolveFormID(formID, formID)) {
				logger::error("Failed to resolve formID {}"sv, formID);
				continue;
			}

			serializationInterface->ReadRecordData(value);
			m_Data[formID] = value;
		}

		logger::info("{} - {} actor form data loaded", GetType(), m_Data.size());
		return true;
	}

	void BaseData::Clear()
	{
		Locker locker(m_Lock);
		m_Data.clear();
	}

	std::string DecodeTypeCode(std::uint32_t typeCode)
	{
		constexpr std::size_t SIZE = sizeof(std::uint32_t);

		std::string sig;
		sig.resize(SIZE);
		char* iter = reinterpret_cast<char*>(&typeCode);
		for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
			sig[j] = iter[i];
		}
		return sig;
	}

	void SaveCallback(SKSE::SerializationInterface* serializationInterface)
	{
		const auto arousalData = ArousalData::GetSingleton();
		if (!arousalData->Save(serializationInterface, kArousalDataKey, kSerializationVersion)) {
			logger::critical("Failed to save Arousal Data");
		}

		const auto multiplierData = MultiplierData::GetSingleton();
		if (!multiplierData->Save(serializationInterface, kMultiplerDataKey, kSerializationVersion)) {
			logger::critical("Failed to save Arousal Multipler Data");
		}

		const auto lastCheckData = LastCheckTimeData::GetSingleton();
		if (!lastCheckData->Save(serializationInterface, kLastCheckTimeDataKey, kSerializationVersion)) {
			logger::critical("Failed to save Arousal Last Check Time Data Data");
		}

		logger::info("OSLArousal Saved");
	}

	void LoadCallback(SKSE::SerializationInterface* serializationInterface)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (serializationInterface->GetNextRecordInfo(type, version, length)) {
			if (version != kSerializationVersion) {
				logger::critical("Loaded data has incorrect version. Recieved ({}) - Expected ({}) for Data Key ({})", version, kSerializationVersion, DecodeTypeCode(type));
				continue;
			}

			switch (type) {
			case kArousalDataKey:
				{
					auto arousalData = ArousalData::GetSingleton();
					if (!arousalData->Load(serializationInterface)) {
						logger::critical("Failed to Load Arousal Data");
					}
				}
				break;
			case kMultiplerDataKey:
				{
					auto multiplierData = MultiplierData::GetSingleton();
					if (!multiplierData->Load(serializationInterface)) {
						logger::critical("Failed to Load Multipler Data");
					}
				}
				break;
			case kLastCheckTimeDataKey:
				{
					auto lastCheckData = LastCheckTimeData::GetSingleton();
					if (!lastCheckData->Load(serializationInterface)) {
						logger::critical("Failed to Load LastCheckTime Data");
					}
				}
				break;
			default:
				logger::critical("Unrecognized Record Type: {}", DecodeTypeCode(type));
				break;
			}
		}
	}

	void RevertCallback(SKSE::SerializationInterface*)
	{
		auto arousalData = ArousalData::GetSingleton();
		arousalData->Clear();
		auto multiplierData = MultiplierData::GetSingleton();
		multiplierData->Clear();
		auto lastCheckData = LastCheckTimeData::GetSingleton();
		lastCheckData->Clear();

		logger::info("Reverting Data State...");
	}
}
