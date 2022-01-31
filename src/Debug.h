#pragma once

#include "Serialization.h"

namespace Debug
{
	void DumpAllArousalData()
	{
		Serialization::LastCheckTimeData::GetSingleton()->DumpToLog();
		Serialization::ArousalData::GetSingleton()->DumpToLog();
		Serialization::MultiplierData::GetSingleton()->DumpToLog();
		Serialization::LastOrgasmTimeData::GetSingleton()->DumpToLog();
		Serialization::TimeRateData::GetSingleton()->DumpToLog();
	}

	void ClearSecondaryArousalData()
	{
		Serialization::LastCheckTimeData::GetSingleton()->Clear();
		Serialization::ArousalData::GetSingleton()->Clear();
		Serialization::MultiplierData::GetSingleton()->Clear();
		Serialization::LastOrgasmTimeData::GetSingleton()->Clear();
		Serialization::TimeRateData::GetSingleton()->Clear();
	}

	void ClearAllArousalData()
	{
		Serialization::LastCheckTimeData::GetSingleton()->Clear();
		Serialization::ArousalData::GetSingleton()->Clear();
		Serialization::MultiplierData::GetSingleton()->Clear();
		Serialization::LastOrgasmTimeData::GetSingleton()->Clear();
		Serialization::TimeRateData::GetSingleton()->Clear();
	}
}
