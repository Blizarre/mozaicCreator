#pragma once

#include <Winbase.h>
#include <string>

class Benchmarking
{
public:
	Benchmarking() { start(); }

	DWORD start() { 
		m_previousTime = GetTickCount(); 
		m_totalTime = 0;
		return m_previousTime; 
	}

	DWORD stop()
	{
		if (m_totalTime == 0)
			m_totalTime = GetTickCount() - m_previousTime;

		return m_totalTime;
	}

	std::string stopString()
	{
		char tmp[100];
		stop();
		float timeInSecond = ((float)m_totalTime) / 1000.0f;
		sprintf_s(tmp, 100, "%.2fs.", timeInSecond);
		return std::string(tmp);
	}

protected:
	DWORD m_previousTime;
	DWORD m_totalTime;
};

