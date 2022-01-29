#pragma once
//This Serialization is based off how powerof3's did it in Afterlife 

namespace Serialization
{
	typedef std::map<uint64_t, float> FormFloatData;

	class BaseData
	{
	public:
		float GetData(uint64_t formId);
		void SetData(uint64_t formId, float value);

		virtual const char* GetType() = 0;

		bool Save(SKSE::SerializationInterface* serializationInterface, std::uint32_t type, std::uint32_t version);
		bool Save(SKSE::SerializationInterface* serializationInterface);
		bool Load(SKSE::SerializationInterface* serializationInterface);

		void Clear();

	protected:
		FormFloatData m_Data;

		using Lock = std::recursive_mutex;
		using Locker = std::lock_guard<Lock>;
		mutable Lock m_Lock;
	};

	class ArousalData final : public BaseData
	{
	public:
		static ArousalData* GetSingleton()
		{
			static ArousalData singleton;
			return &singleton;
		}

		const char* GetType() override
		{
			return "Arousal";
		}
	};

	class MultiplierData final : public BaseData
	{
	public:
		static MultiplierData* GetSingleton()
		{
			static MultiplierData singleton;
			return &singleton;
		}

		const char* GetType() override
		{
			return "Multiplier";
		}
	};

	class LastCheckTimeData final : public BaseData
	{
	public:
		static ArousalData* GetSingleton()
		{
			static ArousalData singleton;
			return &singleton;
		}

		const char* GetType() override
		{
			return "LastCheckTime";
		}
	};


	constexpr std::uint32_t kSerializationVersion = 1;
	constexpr std::uint32_t kArousalDataKey = 'OSLA';
	constexpr std::uint32_t kMultiplerDataKey = 'OSLM';
	constexpr std::uint32_t kLastCheckTimeDataKey = 'OSLC';

	std::string DecodeTypeCode(std::uint32_t typeCode);

	void SaveCallback(SKSE::SerializationInterface* serializationInterface);
	void LoadCallback(SKSE::SerializationInterface* serializationInterface);
	void RevertCallback(SKSE::SerializationInterface* serializationInterface);
};
