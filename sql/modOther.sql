SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `_battlegroundLog`
-- ----------------------------
DROP TABLE IF EXISTS `_battlegroundLog`;
CREATE TABLE `_battlegroundLog` (
  `guid` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `mapId` int(11) NOT NULL DEFAULT '-1',
  `battleType` int(10) unsigned NOT NULL DEFAULT '0',
  `bracket` int(11) NOT NULL DEFAULT '-1',
  `levelMin` tinyint(3) NOT NULL DEFAULT '0',
  `levelMax` tinyint(3) NOT NULL DEFAULT '0',
  `isArena` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `isRate` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `battleTime` bigint(20) unsigned NOT NULL DEFAULT '0',
  `factionWin` tinyint(4) NOT NULL DEFAULT '-1',
  `arenaTeamIdWin` int(10) unsigned NOT NULL DEFAULT '0',
  `arenaTeamIdLose` int(10) unsigned NOT NULL DEFAULT '0',
  `guildIdWin` bigint(20) unsigned NOT NULL DEFAULT '0',
  `guildIdLose` bigint(20) unsigned NOT NULL DEFAULT '0',
  `peopleCountWin` int(10) unsigned NOT NULL DEFAULT '0',
  `peopleDataWin` longtext,
  `peopleCountLose` int(10) unsigned NOT NULL DEFAULT '0',
  `peopleDataLose` longtext,
  `battleDate` datetime NOT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB AUTO_INCREMENT=5326 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `_killLog`
-- ----------------------------
DROP TABLE IF EXISTS `_killLog`;
CREATE TABLE `_killLog` (
  `guid` bigint(20) NOT NULL AUTO_INCREMENT,
  `guildId` bigint(20) NOT NULL,
  `creatureEntry` bigint(20) NOT NULL,
  `creatureName` varchar(255) DEFAULT NULL,
  `creatureParent` bigint(20) DEFAULT NULL,
  `killerCount` bigint(20) DEFAULT NULL,
  `killDate` datetime DEFAULT NULL,
  `killData` longtext,
  `isRaid` tinyint(1) DEFAULT NULL,
  `mapId` int(4) DEFAULT NULL,
  `mapName` varchar(255) DEFAULT NULL,
  `instanceMode` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM AUTO_INCREMENT=1233 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Table structure for `accountHistory`
-- ----------------------------
DROP TABLE IF EXISTS `accountHistory`;
CREATE TABLE `accountHistory` (
  `guid` bigint(20) NOT NULL AUTO_INCREMENT,
  `accountId` int(20) NOT NULL,
  `dateInsert` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `historyText` varchar(255) NOT NULL,
  `characterGuid` varchar(255) DEFAULT NULL,
  `dataOld` varchar(255) DEFAULT NULL,
  `dataNew` varchar(255) DEFAULT NULL,
  `connectIp` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM AUTO_INCREMENT=122624 DEFAULT CHARSET=cp1251;