# Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Custom/mod_ArenaWatcher.cpp
  Custom/mod_AuctionHouseBot.cpp
  Custom/mod_AutoLearn.cpp
  Custom/mod_BattlegroundHistory.cpp
  Custom/mod_BossHistory.cpp
  Custom/mod_CharacterHistory.cpp
  Custom/mod_CharacterModel.cpp
  Custom/mod_DynamicInsctane.cpp
  Custom/mod_DynamicRate.cpp
  Custom/mod_ExternalMail.cpp
  Custom/mod_Tournaments.cpp
  Custom/mod_GuildDefault.cpp
  Custom/mod_HonorRank.cpp
  Custom/mod_ItemUpgrade.cpp
  Custom/mod_MailHistory.cpp
  Custom/mod_MultiKill.cpp
  Custom/mod_SpecialCode.cpp
  Custom/mod_LexicsCutter.cpp
)

message("  -> Prepared: Custom")
