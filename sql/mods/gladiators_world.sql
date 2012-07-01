DROP TABLE IF EXISTS `_gladiator_creature`;
CREATE TABLE `_gladiator_creature` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '0',
  `from_point_id` int(10) unsigned NOT NULL DEFAULT '0',
  `to_point_id` int(10) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

INSERT INTO `_gladiator_creature` VALUES ('1', '1', '15931', '1', '1', '2', '0');
INSERT INTO `_gladiator_creature` VALUES ('2', '2', '6', '1', '1', '2', '0');


DROP TABLE IF EXISTS `_gladiator_points`;
CREATE TABLE `_gladiator_points` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `map_id` int(10) unsigned NOT NULL DEFAULT '0',
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `o` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

INSERT INTO `_gladiator_points` VALUES ('1', '0', '-13218', '249', '22', '1.34');
INSERT INTO `_gladiator_points` VALUES ('2', '0', '-13205', '275', '22', '4.37');


DROP TABLE IF EXISTS `_gladiator_template`;
CREATE TABLE `_gladiator_template` (
  `level` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `time_limit` int(11) NOT NULL DEFAULT '0',
  `say_start` int(11) unsigned NOT NULL DEFAULT '0',
  `say_win` int(11) unsigned NOT NULL DEFAULT '0',
  `say_lose` int(10) unsigned NOT NULL DEFAULT '0',
  `chest_id` int(10) unsigned NOT NULL DEFAULT '0',
  `point_id` int(10) unsigned NOT NULL DEFAULT '0',
  `req_quest_id` int(10) unsigned NOT NULL DEFAULT '0',
  `kill_credit` int(10) unsigned NOT NULL DEFAULT '0',
  `menu_string` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`level`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

INSERT INTO `_gladiator_template` VALUES ('1', '30000', '0', '0', '0', '2850', '1', '0', '0', '0');
INSERT INTO `_gladiator_template` VALUES ('2', '20000', '0', '0', '0', '3715', '1', '0', '0', '0');
