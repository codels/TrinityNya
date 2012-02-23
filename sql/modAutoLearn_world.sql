SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `world_autolearn`
-- ----------------------------
DROP TABLE IF EXISTS `world_autolearn`;
CREATE TABLE `world_autolearn` (
  `SpellId` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `SpellMask` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredClassMask` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RequiredRaceMask` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RequiredLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredSpellId` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RequiredSkillId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredSkillValue` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Comment` varchar(255) NOT NULL,
  PRIMARY KEY (`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of world_autolearn
-- ----------------------------
INSERT INTO `world_autolearn` VALUES ('33388', '2', '0', '0', '20', '0', '0', '0', 'Верховая езда (ученик)');
INSERT INTO `world_autolearn` VALUES ('33391', '2', '0', '0', '40', '0', '0', '0', 'Верховая езда (подмастерье)');
INSERT INTO `world_autolearn` VALUES ('34090', '2', '0', '0', '60', '0', '0', '0', 'Верховая езда (умелец)');
INSERT INTO `world_autolearn` VALUES ('34091', '2', '0', '0', '70', '0', '0', '0', 'Верховая езда (искусник)');
INSERT INTO `world_autolearn` VALUES ('54197', '2', '0', '0', '77', '0', '0', '0', 'Полеты в непогоду');
INSERT INTO `world_autolearn` VALUES ('5011', '8', '13', '0', '0', '0', '0', '0', 'Арбалеты');
INSERT INTO `world_autolearn` VALUES ('264', '8', '13', '0', '0', '0', '0', '0', 'Луки');
INSERT INTO `world_autolearn` VALUES ('266', '8', '13', '0', '0', '0', '0', '0', 'Ружья');
INSERT INTO `world_autolearn` VALUES ('2567', '8', '9', '0', '0', '0', '0', '0', 'Метательное оружие');
INSERT INTO `world_autolearn` VALUES ('1180', '8', '1501', '0', '0', '0', '0', '0', 'Кинжалы');
INSERT INTO `world_autolearn` VALUES ('15590', '8', '1101', '0', '0', '0', '0', '0', 'Кулачное оружие');
INSERT INTO `world_autolearn` VALUES ('198', '8', '1147', '0', '0', '0', '0', '0', 'Одноручное дробящее оружие');
INSERT INTO `world_autolearn` VALUES ('201', '8', '431', '0', '0', '0', '0', '0', 'Одноручные мечи');
INSERT INTO `world_autolearn` VALUES ('196', '8', '111', '0', '0', '0', '0', '0', 'Одноручные топоры');
INSERT INTO `world_autolearn` VALUES ('202', '8', '39', '0', '0', '0', '0', '0', 'Двуручные мечи');
INSERT INTO `world_autolearn` VALUES ('197', '8', '103', '0', '0', '0', '0', '0', 'Двуручные топоры');
INSERT INTO `world_autolearn` VALUES ('200', '8', '1063', '0', '20', '0', '0', '0', 'Древковое оружие');
INSERT INTO `world_autolearn` VALUES ('227', '8', '1493', '0', '0', '0', '0', '0', 'Посохи');
INSERT INTO `world_autolearn` VALUES ('199', '8', '1123', '0', '0', '0', '0', '0', 'Двуручное дробящее оружие');
INSERT INTO `world_autolearn` VALUES ('458', '4', '0', '1', '20', '0', '0', '0', 'Гнедой конь');
INSERT INTO `world_autolearn` VALUES ('23228', '4', '0', '1', '40', '0', '0', '0', 'Стремительный белый рысак');
INSERT INTO `world_autolearn` VALUES ('64658', '4', '0', '2', '20', '0', '0', '0', 'Черный волк');
INSERT INTO `world_autolearn` VALUES ('23251', '4', '0', '2', '40', '0', '0', '0', 'Стремительный лесной волк');
INSERT INTO `world_autolearn` VALUES ('6898', '4', '0', '4', '20', '0', '0', '0', 'Белый баран');
INSERT INTO `world_autolearn` VALUES ('23240', '4', '0', '4', '40', '0', '0', '0', 'Стремительный белый баран');
INSERT INTO `world_autolearn` VALUES ('8394', '4', '0', '8', '20', '0', '0', '0', 'Полосатый ледопард');
INSERT INTO `world_autolearn` VALUES ('23221', '4', '0', '8', '40', '0', '0', '0', 'Стремительный ледопард');
INSERT INTO `world_autolearn` VALUES ('64977', '4', '0', '16', '20', '0', '0', '0', 'Черный конь-скелет');
INSERT INTO `world_autolearn` VALUES ('23246', '4', '0', '16', '40', '0', '0', '0', 'Лиловый боевой конь-скелет');
INSERT INTO `world_autolearn` VALUES ('64657', '4', '0', '32', '20', '0', '0', '0', 'Белый кодо');
INSERT INTO `world_autolearn` VALUES ('23248', '4', '0', '32', '40', '0', '0', '0', 'Огромный серый кодо');
INSERT INTO `world_autolearn` VALUES ('10873', '4', '0', '64', '20', '0', '0', '0', 'Красный механодолгоног');
INSERT INTO `world_autolearn` VALUES ('23225', '4', '0', '64', '40', '0', '0', '0', 'Стремительный зеленый механодолгоног');
INSERT INTO `world_autolearn` VALUES ('8395', '4', '0', '128', '20', '0', '0', '0', 'Изумрудный ящер');
INSERT INTO `world_autolearn` VALUES ('23241', '4', '0', '128', '40', '0', '0', '0', 'Стремительный синий ящер');
INSERT INTO `world_autolearn` VALUES ('35022', '4', '0', '512', '20', '0', '0', '0', 'Черный крылобег');
INSERT INTO `world_autolearn` VALUES ('33660', '4', '0', '512', '40', '0', '0', '0', 'Стремительный розовый крылобег');
INSERT INTO `world_autolearn` VALUES ('35710', '4', '0', '1024', '20', '0', '0', '0', 'Серый элекк');
INSERT INTO `world_autolearn` VALUES ('35713', '4', '0', '1024', '40', '0', '0', '0', 'Большой синий элекк');
INSERT INTO `world_autolearn` VALUES ('32240', '4', '0', '1101', '60', '0', '0', '0', 'Белоснежный грифон');
INSERT INTO `world_autolearn` VALUES ('32243', '4', '0', '690', '60', '0', '0', '0', 'Рыжий ветрокрыл');
INSERT INTO `world_autolearn` VALUES ('32289', '4', '0', '1101', '70', '0', '0', '0', 'Стремительный красный грифон');
INSERT INTO `world_autolearn` VALUES ('32246', '4', '0', '690', '70', '0', '0', '0', 'Стремительный красный ветрокрыл');
