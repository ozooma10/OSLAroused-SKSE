#pragma once

#include "PersistedData.h"

namespace Debug
{
	void DumpAllArousalData()
	{
		PersistedData::ArousalData::GetSingleton()->DumpToLog();
		PersistedData::BaseLibidoData::GetSingleton()->DumpToLog();
		PersistedData::LastCheckTimeData::GetSingleton()->DumpToLog();
	}

	void ClearAllArousalData()
	{
		PersistedData::ArousalData::GetSingleton()->Clear();
		PersistedData::BaseLibidoData::GetSingleton()->Clear();
		PersistedData::LastCheckTimeData::GetSingleton()->Clear();
	}
}
