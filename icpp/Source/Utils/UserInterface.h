#ifndef UTILS_USER_INTERFACE_H
#define UTILS_USER_INTERFACE_H

#pragma once

namespace Utils {

namespace MessageType {

enum Enumeration {
	Log,
	Info,
	Error
};

} // namespace MessageType

class CUserInterface
{
public:
	static CUserInterface &Instance ();
	virtual void AddMessage (std::string const &,
		MessageType::Enumeration = MessageType::Log) const = 0;
	virtual void SetStatusLine (std::string const &) const = 0;
	virtual void MessageBox (std::string const &) const = 0;
};

} // namespace Utils

#endif // UTILS_USER_INTERFACE_H
