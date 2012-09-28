ALTER TABLE `characters` 
ADD `currentPetSlot` tinyint(3) NULL AFTER `actionBars`,
ADD `PetSlotUsed` bigint(10) NULL AFTER `currentPetSlot`;