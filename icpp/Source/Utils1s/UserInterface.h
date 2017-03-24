#ifndef UTILS1S_USER_INTERFACE_H
#define UTILS1S_USER_INTERFACE_H

#pragma once

#include "../Utils/UserInterface.h"

namespace Utils1s {

class CUserInterface : public Utils::CUserInterface
{
public:
	virtual void AddMessage (std::string const & MessageText,
		Utils::MessageType::Enumeration Type = Utils::MessageType::Log) const;

	virtual void SetStatusLine (std::string const &MessageText) const;

	virtual void MessageBox (std::string const &Message) const;
}; // class CUserInterface

} // namespace Utils1s

#endif // UTILS1S_USER_INTERFACE_H
