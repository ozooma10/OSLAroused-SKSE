#pragma once

#include "SavedDataManager.h"

namespace Debug
{
	void DumpAllArousalData()
	{
		SavedDataManager::ArousalData::GetSingleton()->DumpToLog();
		SavedDataManager::BaseLibidoData::GetSingleton()->DumpToLog();
		SavedDataManager::LastCheckTimeData::GetSingleton()->DumpToLog();
	}

	void ClearAllArousalData()
	{
		SavedDataManager::ArousalData::GetSingleton()->Clear();
		SavedDataManager::BaseLibidoData::GetSingleton()->Clear();
		SavedDataManager::LastCheckTimeData::GetSingleton()->Clear();
	}
}
