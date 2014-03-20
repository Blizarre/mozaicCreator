#pragma once

#include <Winbase.h>
#include <string>

class Benchmarking
{
public:
	Benchmarking() { }

	DWORD start() {
		return start("");
	}

	DWORD start(std::string message) { 
		int padding = message.length() - 30;
		for (int i = 0; i < padding; i++) message += " ";
		std::cout << message << " : "; std::cout.flush();
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

	float stopString(int number)
	{
		stop();
		float timeInSecond = ((float)m_totalTime) / 1000.0f;
		char tmp[100];
		std::cout << timeInSecond << "s. - " << number << " items" << std::endl;;
		return m_totalTime;
	}

	float stopString()
	{
		stop();
		float timeInSecond = ((float)m_totalTime) / 1000.0f;
		std::cout << timeInSecond << "s." << std::endl;
		return m_totalTime;
	}

protected:
	DWORD m_previousTime;
	DWORD m_totalTime;
};

