#pragma once

namespace Yugo
{

	class Time
	{
	private:
		static float m_CurrentTime;
		static float m_PreviosTime;
		static float m_DeltaTime;

	public:
		static void OnStart();
		static void CalculateDeltaTime();
		static float GetDeltaTime();
		static float CurrentRealTime();
	};

}