//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_UTILS_HPP
#define RCONSOLE_UTILS_HPP

#include <string>
#ifndef _WIN32
#define MB_ICONERROR 1
#else
#include <windows.h>
#endif

namespace RC::Client::Utils
{
	//! @brief Display a Windows dialog box.
	//! @details This functions opens a Windows dialog box and return the button clicked by the user.
	//! @param title The title of the window.
	//! @param content The content of the box.
	//! @param variate A bit combination of the window attributes (see Windows MessageBox function for a list of the enums).
	//! @return The button clicked by the user.
	//! @note On Non-Windows systems, it will simulate the Windows dialog box. Only MB_ICONERROR and MB_OK are simulated on those systems.
	int	dispMsg(const std::string &title, const std::string &content, int variate);

	std::string getLastExceptionName();
}

#endif //RCONSOLE_UTILS_HPP
