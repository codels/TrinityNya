DROP TABLE IF EXISTS `world_item_upgrade`;
CREATE TABLE `world_item_upgrade` (
  `enchant_id` int(10) NOT NULL,
  `prev_enchant_id` int(10) NOT NULL DEFAULT '0',
  `description` varchar(255) NOT NULL,
  `golds` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`enchant_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
