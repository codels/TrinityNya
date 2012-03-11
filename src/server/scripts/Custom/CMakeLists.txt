# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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
  Custom/mod_MultiKill.cpp
  Custom/mod_HonorRank.cpp
  Custom/mod_AutoLearn.cpp
  Custom/mod_FriendChat.cpp
  Custom/mod_DynamicRate.cpp
  Custom/mod_KillBoss.cpp
  Custom/mod_ExternalMail.cpp
  Custom/mod_DynamicInsctane.cpp
  Custom/mod_AccountHistory.cpp
  Custom/mod_BattlegroundHistory.cpp
)

message("  -> Prepared: Custom")
