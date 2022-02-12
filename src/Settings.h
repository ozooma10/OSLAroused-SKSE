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

	void SetNudeArousalBaseline(float newVal) { 
		Locker locker(m_Lock);
		m_IsNudeBaseline = newVal; 
	}
	float GetNudeArousalBaseline() const { 
		Locker locker(m_Lock);
		return m_IsNudeBaseline;
	}

	void SetNudeViewingBaseline(float newVal)
	{
		Locker locker(m_Lock);
		m_ViewingNudeBaseline = newVal;
	}
	float GetHourlyNudityArousalModifier() const
	{
		Locker locker(m_Lock);
		return m_ViewingNudeBaseline;
	}

	void SetSceneParticipantBaseline(float newVal)
	{
		Locker locker(m_Lock);
		m_SceneParticipateBaseline = newVal;
	}
	float GetSceneParticipantBaseline() const
	{
		Locker locker(m_Lock);
		return m_SceneParticipateBaseline;
	}

	void SetSceneViewingBaseline(float newVal)
	{
		Locker locker(m_Lock);
		m_SceneViewingBaseline = newVal;
	}
	float GetSceneViewingBaseline() const
	{
		Locker locker(m_Lock);
		return m_SceneViewingBaseline;
	}

	void SetSceneVictimGainsArousal(bool newVal)
	{
		Locker locker(m_Lock);
		m_SceneVictimGainsArousal = newVal;
	}
	bool GetSceneVictimGainsArousal() const
	{
		Locker locker(m_Lock);
		return m_SceneVictimGainsArousal;
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
private:
	float m_IsNudeBaseline = 30.f;
	float m_ViewingNudeBaseline = 20.f;

	float m_SceneParticipateBaseline = 50.f;
	float m_SceneViewingBaseline = 30.f;
	bool m_SceneVictimGainsArousal = false;

	float m_ScanDistance = 5120.f;

	mutable Lock m_Lock;
};
