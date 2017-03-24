#include "stdafx.h"
#include "UserInterface.h"

namespace Utils1s {

void CUserInterface::AddMessage (std::string const & MessageText,
	Utils::MessageType::Enumeration Type /*= Utils::MessageType::Log*/) const
{
	CBkEndUI *pUI = GetBkEndUI ();
	ASSERT (pUI);
	enum MessageMarker Marker;

	switch (Type) {
	case Utils::MessageType::Info: Marker = mmInformation; break;
	case Utils::MessageType::Log: Marker = mmBlueTriangle; break;
	case Utils::MessageType::Error: Marker = mmRedErr; break;
	default:
		Marker = mmBlueTriangle; break;
	}

	pUI->DoMessageLine (MessageText.c_str (), Marker);
} // CUserInterface::AddMessage

void CUserInterface::SetStatusLine (std::string const &MessageText) const
{
	CBkEndUI *pUI = GetBkEndUI ();
	ASSERT (pUI);
	pUI->DoStatusLine (MessageText.c_str ());
} // CUserInterface::SetStatusLine

void CUserInterface::MessageBox (std::string const &Message) const
{
	::AfxMessageBox (Message.c_str (), MB_ICONINFORMATION, 0);
} // CUserInterface::MessageBox

} // namespace Utils1s
