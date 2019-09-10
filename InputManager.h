#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <string>
#include "SingleClass.hpp"

class InputManager : public SingleClass<InputManager>
{
public:

	std::string get_name() const;

	std::string get_password() const;

	double get_double(const std::string& msg) const;

	int get_int(const std::string& msg) const;

	std::string get_dir() const;

	std::string get_command() const;

	void waitKey() const noexcept;

private:

	static const size_t _lengthLimit = 20;
};

#endif
