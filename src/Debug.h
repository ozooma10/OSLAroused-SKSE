#pragma once

#include "SavedDataManager.h"

namespace Debug
{
	void DumpAllArousalData()
	{
		SavedDataManager::ArousalData::GetSingleton()->DumpToLog();
		SavedDataManager::BaseArousalData::GetSingleton()->DumpToLog();
		SavedDataManager::LastCheckTimeData::GetSingleton()->DumpToLog();
	}

	void ClearAllArousalData()
	{
		SavedDataManager::ArousalData::GetSingleton()->Clear();
		SavedDataManager::BaseArousalData::GetSingleton()->Clear();
		SavedDataManager::LastCheckTimeData::GetSingleton()->Clear();
	}
}
