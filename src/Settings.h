#pragma once
using Lock = std::recursive_mutex;
using Locker = std::lock_guard<Lock>;

class Settings
{
public:
	static Settings* GetSingleton()
	{
		static Settings singleton;
		return &singleton;
	}

	void SetPlayerNudityCheckEnabled(bool newVal) { 
		Locker locker(m_Lock);
		m_EnablePlayerNudityCheck = newVal; 
	}
	bool GetPlayerNudityCheckEnabled() const { 
		Locker locker(m_Lock);
		return m_EnablePlayerNudityCheck;
	}

private:
	bool m_EnablePlayerNudityCheck = true;

	mutable Lock m_Lock;
};
