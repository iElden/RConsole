//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_UTILS_HPP
#define RCONSOLE_UTILS_HPP

#include <string>
#define MB_ICONERROR 1

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
	void	dispMsg(tgui::Gui &gui, const std::string &title, const std::string &content, int variate, const std::function<void()> &onClose);
	tgui::ChildWindow::Ptr	msgWin(const std::string &title, const std::string &content, int variate);
	void	msgWin(tgui::ChildWindow::Ptr gui, const std::string &title, const std::string &content, int variate, const std::function<void()> &onClose);

	//! @brief Get the last Exception Name
	//! @details Return the last type of Exception name
	//! @return std::string The last Exception name
	std::string getLastExceptionName();

	//! @brief Display a focused window.
	//! @param gui The gui handling the window.
	//! @param width The width of the window.
	//! @param height The height of the window.
	//! @return A pointer to the window
	tgui::ChildWindow::Ptr openWindowWithFocus(tgui::Gui &gui, tgui::Layout width, tgui::Layout height);
}

#endif //RCONSOLE_UTILS_HPP
