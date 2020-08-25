#include "pch.h"
#include "Time.h"
#include "Common.h"


namespace Yugo
{

	float Time::m_CurrentTime = 0.0f;
	float Time::m_PreviosTime = 0.0f;
	float Time::m_DeltaTime = 0.0f;

	void Time::OnStart()
	{
		m_CurrentTime = glfwGetTime();
	}

	void Time::CalculateDeltaTime()
	{
		m_PreviosTime = m_CurrentTime;
		m_CurrentTime = CurrentRealTime();
		m_DeltaTime = m_CurrentTime - m_PreviosTime;
	}

	float Time::GetDeltaTime()
	{
		return m_DeltaTime;
	}

	float Time::CurrentRealTime()
	{
		return glfwGetTime();
	}

}
