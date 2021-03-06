#ifndef COBALT_ZANDRONUM_SERVER_H
#define COBALT_ZANDRONUM_SERVER_H

#include "main.h"
#include "libcobaltcore/ip.h"
#include "libcobaltcore/udp.h"

#define DEFINE_GAME_ENUMS
#include "gameenums.h"
#undef DEFINE_GAME_ENUMS

extern const StringList gamestrings;

void tickServerRequests();
void addServerQuery (CoIPAddress addr, str target);

// =============================================================================
// -----------------------------------------------------------------------------
class LauncherRequest : public CoUDPSocket
{
	PROPERTY (CoTime, nextRequest, setNextRequest)
	PROPERTY (String, payloadTarget, setPayloadTarget)
	PROPERTY (CoIPAddress, address, setAddress)
	PROPERTY (CoTime, deadline, setDeadline)
	PROPERTY (bool, done, setDone)

	public:
		enum State
		{
			Unknown,
			Known,
			Throttled,
			Banned,
			TimedOut,
		};

		struct InfoStruct
		{
			str version, name, mapname;
			uint8 maxclients, players, gametype, instagib, buckshot;
			State state;
			str iwad;
			StringList pwads;
		};

		enum Flags
		{
			Name                 = (1 << 0x00),
			URL                  = (1 << 0x01),
			EMail                = (1 << 0x02),
			MapName              = (1 << 0x03),
			MaxClients           = (1 << 0x04),
			MaxPlayers           = (1 << 0x05),
			PWADs                = (1 << 0x06),
			GameType             = (1 << 0x07),
			GameName             = (1 << 0x08),
			IWAD                 = (1 << 0x09),
			ForcePassword        = (1 << 0x0A),
			ForceJoinPassword    = (1 << 0x0B),
			GameSkill            = (1 << 0x0C),
			BotSkill             = (1 << 0x0D),
			OldDMFlags           = (1 << 0x0E),
			//                   = (1 << 0x0F),
			Limits               = (1 << 0x10),
			TeamDamage           = (1 << 0x11),
			OldTeamScores        = (1 << 0x12),
			NumPlayers           = (1 << 0x13),
			PlayerData           = (1 << 0x14),
			TeamInfoNumber       = (1 << 0x15),
			TeamInfoName         = (1 << 0x16),
			TeamInfoColor        = (1 << 0x17),
			TeamInfoScore        = (1 << 0x18),
			TestingServer        = (1 << 0x19),
			DataMD5Sum           = (1 << 0x1A),
			AllDMFlags           = (1 << 0x1B),
			SecuritySettings     = (1 << 0x1C),
		};

		LauncherRequest();
		void update();
		void query();
		void incoming (const Bytestream& encoded, CoIPAddress addr);

	private:
		InfoStruct mInfo;
};

namespace ZandronumProtocol
{
	enum Code
	{
		VersionNumber = 2,
		BeginServerList = 0,
		Server,
		EndServerList,
		IPIsBanned,
		RequestIgnored,
		WrongVersion,
		BeginServerListPart,
		EndServerListPart,
		ServerBlock,

		Challenge            = 5660020,
		Statistics           = 5660022,
		ServerQueryData,
		ServerQueryIgnoring,
		ServerQueryBanned,
		LauncherChallenge    = 5660028,
		ServerVerify,
		BanlistReceipt,

		Banlist              = 205,
		MasterVerify,
		BanlistPart,

		LauncherServerChallenge = 199,
	};

	enum BanCode
	{
		Ban = 0,
		BanExempt,
		EndBanlistPart,
		EndBanlist,
	};
};

#endif // COBALT_ZANDRONUM_SERVER_H
