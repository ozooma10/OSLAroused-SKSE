#pragma once
using Lock = std::recursive_mutex;
using Locker = std::lock_guard<Lock>;

class Settings
{
public:
	enum class ArousalMode
	{
		kSexlabAroused,
		kOAroused
	};

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

	void SetArousalMode(ArousalMode newVal)
	{
		Locker locker(m_Lock);
		m_CurrentArousalMode = newVal;
	}
	ArousalMode GetArousalMode() const
	{
		Locker locker(m_Lock);
		return m_CurrentArousalMode;
	}

	void SetDecayRate(float newRate) {
		Locker locker(m_Lock);
		m_DecayRate = newRate;
	}
	float GetDecayRate() const {
		Locker locker(m_Lock);
		return m_DecayRate;
	}

	void SetDefaultArousalMultiplier(float newRate)
	{
		Locker locker(m_Lock);
		m_DefaultArousalMultiplier = newRate;
	}
	float GetDefaultArousalMultiplier() const
	{
		Locker locker(m_Lock);
		return m_DefaultArousalMultiplier;
	}

	void SetPlayerInSexScene(bool newVal)
	{
		Locker locker(m_Lock);
		m_PlayerInSexScene = newVal;
	}
	float GetPlayerInSexScene() const
	{
		Locker locker(m_Lock);
		return m_PlayerInSexScene;
	}

private:
	bool m_EnablePlayerNudityCheck = true;

	ArousalMode m_CurrentArousalMode = ArousalMode::kSexlabAroused;

	//In SLA mode, this is the number of days to remove 1/3rd of Exposure and time rate
	float m_DecayRate = 2.f;

	float m_DefaultArousalMultiplier = 2.f;

	bool m_PlayerInSexScene = false;

	mutable Lock m_Lock;
};
