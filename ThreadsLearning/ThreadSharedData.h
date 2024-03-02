#pragma once

#include <string>

namespace threads_learning {

class ThreadSharedData {
public:
	ThreadSharedData(std::string value) :
		m_value{ value } {}

	std::string getValue() {
		return m_value;
	}

	void setString(std::string value) {
		m_value = value;
	}

private:
	std::string m_value;
};

}
