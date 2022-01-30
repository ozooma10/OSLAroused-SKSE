#pragma once
//This Serialization is based off how powerof3's did it in Afterlife 

namespace Serialization
{
	typedef std::map<RE::FormID, float> FormFloatData;

	class BaseData
	{
	public:
		float GetData(RE::FormID formId, float missing = 0.f);
		void SetData(RE::FormID formId, float value);

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

	class TimeRateData final : public BaseData
	{
	public:
		static TimeRateData* GetSingleton()
		{
			static TimeRateData singleton;
			return &singleton;
		}

		const char* GetType() override
		{
			return "TimeRate";
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

	class LastOrgasmTimeData final : public BaseData
	{
	public:
		static LastOrgasmTimeData* GetSingleton()
		{
			static LastOrgasmTimeData singleton;
			return &singleton;
		}

		const char* GetType() override
		{
			return "LastOrgasmTime";
		}
	};

	constexpr std::uint32_t kSerializationVersion = 1;
	constexpr std::uint32_t kArousalDataKey = 'OSLA';
	constexpr std::uint32_t kMultiplerDataKey = 'OSLM';
	constexpr std::uint32_t kTimeRateDataKey = 'OSLT';
	constexpr std::uint32_t kLastCheckTimeDataKey = 'OSLC';
	constexpr std::uint32_t kLastOrgasmTimeDataKey = 'OSLO';

	std::string DecodeTypeCode(std::uint32_t typeCode);

	void SaveCallback(SKSE::SerializationInterface* serializationInterface);
	void LoadCallback(SKSE::SerializationInterface* serializationInterface);
	void RevertCallback(SKSE::SerializationInterface* serializationInterface);
};
