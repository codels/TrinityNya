ALTER TABLE `world_autolearn`
MODIFY COLUMN `SpellMask`  tinyint(2) UNSIGNED NOT NULL DEFAULT 0 AFTER `SpellId`;
