#ifndef  ENVIRONMENTCONFIG_H_
#define ENVIRONMENTCONFIG_H_

class EnvironmenConfig
{
public:
	bool check();

#ifdef __linux__
private:

	bool modiffyProfile();
#endif
};

#endif // ! ENVIRONMENTCONFIG_H_
