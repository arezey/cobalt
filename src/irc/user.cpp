#include "irc.h"
#include "user.h"
#include "channel.h"
#include "commands.h"
#include "connection.h"

CONFIG (StringList, irc_adminmasks, CoStringList())

// =============================================================================
// Determine status level of this user.
// -----------------------------------------------------------------------------
IRCChannel::Status IRCUser::chanStatus (IRCChannel* chan) {
	return chan->effectiveStatus (this);
}

// =============================================================================
// Check if this user is our administrator
// -----------------------------------------------------------------------------
void IRCUser::checkAdmin() {
	if (user().length() == 0 || host().length() == 0)
		return;
	
	str uhost = userhost();
	bool admin = false;
	
	for (int i = 0; i < irc_adminmasks.size() && !admin; i++)
		if (mask (uhost, irc_adminmasks[i]))
			admin = true;
	
	if (admin)
		*this |= Admin;
	else
		*this &= ~Admin;
}

// =============================================================================
// -----------------------------------------------------------------------------
str IRCUser::userhost() const {
	return Format ("%1!%2@%3", nick(), user(), host());
}

// =============================================================================
// -----------------------------------------------------------------------------
IRCUser::Flags IRCUser::operator| (Flags f) const {
	return flags() | f;
}

// =============================================================================
// -----------------------------------------------------------------------------
IRCUser IRCUser::operator|= (Flags f) {
	setFlags (flags() | f);
	return *this;
}

// =============================================================================
// -----------------------------------------------------------------------------
IRCUser::Flags IRCUser::operator& (Flags f) const {
	return flags() & f;
}

// =============================================================================
// -----------------------------------------------------------------------------
IRCUser IRCUser::operator&= (Flags f) {
	setFlags (flags() & f);
	return *this;
}

// =============================================================================
// -----------------------------------------------------------------------------
IRCUser::Flags IRCUser::operator^ (Flags f) const {
	return flags() ^ f;
}

// =============================================================================
// -----------------------------------------------------------------------------
IRCUser IRCUser::operator^= (Flags f) {
	setFlags (flags() ^ f);
	return *this;
}

// =============================================================================
// -----------------------------------------------------------------------------
str IRCUser::asString() const {
	return Format ("%1 (%2@%3)", nick(), user(), host());
}

// =============================================================================
// -----------------------------------------------------------------------------
void IRCUser::addKnownChannel (IRCChannel* chan) {
	m_channels << chan;
}

// =============================================================================
// -----------------------------------------------------------------------------
void IRCUser::delKnownChannel (IRCChannel* chan) {
	m_channels.remove (chan);
}

// =============================================================================
// -----------------------------------------------------------------------------
bool IRCUser::isAdmin() const {
	return flags() & Flags (Admin);
}

// =============================================================================
// -----------------------------------------------------------------------------
bool IRCUser::canCallCommand (const IRCCommandInfo& cmd, IRCChannel* where) {
	if (cmd.adminOnly && !isAdmin())
		return false;
	
	if (where) {
		Print ("status: %1 <-> %2\n", chanStatus (where), cmd.reqstatus);
	}
	
	if (cmd.reqstatus > IRCChannel::Normal &&
		(!where || chanStatus (where) < cmd.reqstatus)
	) {
		return false;
	}
}