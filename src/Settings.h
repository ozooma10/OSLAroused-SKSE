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

	void SetHourlyNudityArousalModifier(float newVal)
	{
		Locker locker(m_Lock);
		m_HourlyNudityArousalModifier = newVal;
	}
	float GetHourlyNudityArousalModifier() const
	{
		Locker locker(m_Lock);
		return m_HourlyNudityArousalModifier;
	}

	void SetHourlySceneParticipantArousalModifier(float newVal)
	{
		Locker locker(m_Lock);
		m_HourlySceneParticipantArousalModifier = newVal;
	}
	float GetHourlySceneParticipantArousalModifier() const
	{
		Locker locker(m_Lock);
		return m_HourlySceneParticipantArousalModifier;
	}

	void SetHourlySceneViewerArousalModifier(float newVal)
	{
		Locker locker(m_Lock);
		m_HourlySceneViewerArousalModifier = newVal;
	}
	float GetHourlySceneViewerArousalModifier() const
	{
		Locker locker(m_Lock);
		return m_HourlySceneViewerArousalModifier;
	}

	void SetScanDistance(float newVal)
	{
		Locker locker(m_Lock);
		m_ScanDistance = newVal;
	}
	float GetScanDistance() const
	{
		Locker locker(m_Lock);
		return m_ScanDistance;
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

private:
	bool m_EnablePlayerNudityCheck = true;
	
	float m_HourlyNudityArousalModifier = 20.f;

	float m_HourlySceneParticipantArousalModifier = 20.f;
	float m_HourlySceneViewerArousalModifier = 20.f;

	float m_ScanDistance = 5120.f;

	ArousalMode m_CurrentArousalMode = ArousalMode::kSexlabAroused;

	//In SLA mode, this is the number of days to remove 1/3rd of Exposure and time rate
	float m_DecayRate = 2.f;

	float m_DefaultArousalMultiplier = 2.f;

	mutable Lock m_Lock;
};
