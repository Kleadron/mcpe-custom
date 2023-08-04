#include "Inventory.hpp"
#include "Item.hpp"

#ifdef DEMO

static void MoveItemToSlot(int* pItems, int item, int index)
{
	if (index < 0 || index >= C_MAX_INVENTORY_ITEMS)
		return;

	if (pItems[index] == item)
		return;

	// search for the item, if it doesn't exist, return

	int i = 0;
	for (; i < C_MAX_INVENTORY_ITEMS; i++)
	{
		if (pItems[i] == item)
			break;
	}

	if (i == C_MAX_INVENTORY_ITEMS)
		return;

	// swap the slot where our `item` was, and the slot at the index
	int oldItem = pItems[index];
	pItems[index] = pItems[i];
	pItems[i] = oldItem;

#ifndef ORIGINAL_CODE
	if (item > 0)
#endif
	printf("adding item: %s to %d\n", Tile::tiles[item]->getDescriptionId().c_str(), index);
}

static void ShuffleInventoryForDemo(int* pHotbar, int* pItems)
{
	pHotbar[0] = Tile::wood->m_ID;
	pHotbar[1] = Tile::stoneBrick->m_ID;
	pHotbar[2] = Tile::sandStone->m_ID;
	pHotbar[3] = Tile::dirt->m_ID;
	pHotbar[4] = Tile::redBrick->m_ID;
	pHotbar[5] = Tile::rock->m_ID;
	pHotbar[6] = Tile::torch->m_ID;
	pHotbar[7] = Tile::ladder->m_ID;
#ifdef ENH_ENABLE_9TH_SLOT
	pHotbar[8] = Tile::rose->m_ID;
#endif

	MoveItemToSlot(pItems, pHotbar[0], 27);
	MoveItemToSlot(pItems, pHotbar[1], 28);
	MoveItemToSlot(pItems, pHotbar[2], 29);
	MoveItemToSlot(pItems, pHotbar[3], 30);
	MoveItemToSlot(pItems, pHotbar[4], 31);
	MoveItemToSlot(pItems, pHotbar[5], 32);
	MoveItemToSlot(pItems, pHotbar[6], 33);
	MoveItemToSlot(pItems, pHotbar[7], 34);
	MoveItemToSlot(pItems, Tile::flower->m_ID, 35);
	MoveItemToSlot(pItems, Tile::cloth_10->m_ID, 18);
	MoveItemToSlot(pItems, Tile::cloth_20->m_ID, 19);
	MoveItemToSlot(pItems, Tile::cloth_30->m_ID, 20);
	MoveItemToSlot(pItems, Tile::cloth_40->m_ID, 21);
	MoveItemToSlot(pItems, Tile::cloth_50->m_ID, 22);
	MoveItemToSlot(pItems, Tile::cloth_60->m_ID, 23);
	MoveItemToSlot(pItems, Tile::cloth_70->m_ID, 24);
	MoveItemToSlot(pItems, Tile::sand->m_ID, 25);
	MoveItemToSlot(pItems, Tile::glass->m_ID, 26);
	MoveItemToSlot(pItems, Tile::mushroom1->m_ID, 1);
	MoveItemToSlot(pItems, Tile::obsidian->m_ID, 8);

#ifndef ORIGINAL_CODE
	// @NOTE: For Testing
	//pHotbar[1] = Item::camera->m_itemID;
	//pHotbar[2] = Tile::tnt->m_ID;
	//pHotbar[3] = Tile::water->m_ID;
	//pHotbar[4] = Tile::lava->m_ID;
#endif
}

#endif

Inventory::Inventory(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_SelectedHotbarSlot = 0;

	for (int i = 0; i < C_MAX_HOTBAR_ITEMS; i++)
		m_hotbar[i] = -1;
	for (int i = 0; i < C_MAX_INVENTORY_ITEMS; i++)
		m_items[i] = -1;

	// @NOTE: This layout of the hotbar and inventory can be seen in the following video,
	// titled "Minecraft - Pocket Edition on Xperia Play".
	// https://www.youtube.com/watch?v=jO-y5wzmK4E

#ifndef MOD_JAVA_STARTING_BLOCKS
	m_hotbar[0]  = Tile::wood->m_ID;
	m_hotbar[1]  = Tile::cloth_10->m_ID;
	m_hotbar[2]  = Tile::cloth_20->m_ID;
	m_hotbar[3]  = Tile::cloth_30->m_ID;
	m_hotbar[4]  = Tile::cloth_40->m_ID;
	m_hotbar[5]  = Tile::cloth_50->m_ID;
	m_hotbar[6]  = Tile::cloth_60->m_ID;
	m_hotbar[7]  = Tile::ladder->m_ID;
#else
	m_hotbar[0] = Tile::rock->m_ID;
	m_hotbar[1] = Tile::stoneBrick->m_ID;
	m_hotbar[2] = Tile::redBrick->m_ID;
	m_hotbar[3] = Tile::dirt->m_ID;
	m_hotbar[4] = Tile::wood->m_ID;
	m_hotbar[5] = Tile::treeTrunk->m_ID;
	m_hotbar[6] = Tile::leaves->m_ID;
	m_hotbar[7] = Tile::torch->m_ID;
#ifdef ENH_ENABLE_9TH_SLOT
	m_hotbar[8] = Tile::stoneSlabHalf->m_ID;
#endif
#endif
	// slot 8 missing. I assume that's the "..." button

#ifndef MOD_EXTRA_BLOCKS
	m_items[0] = Tile::rock->m_ID;
	m_items[1] = Tile::stoneBrick->m_ID;
	m_items[2] = Tile::sandStone->m_ID;
	m_items[3] = Tile::wood->m_ID;
	m_items[4] = Tile::treeTrunk->m_ID;
	m_items[5] = Tile::goldBlock->m_ID;
	m_items[6] = Tile::ironBlock->m_ID;
	m_items[7] = Tile::emeraldBlock->m_ID;
	m_items[8] = Tile::redBrick->m_ID;
	m_items[9] = Tile::leaves->m_ID;
	m_items[10] = Tile::cloth_10->m_ID;
	m_items[11] = Tile::cloth_20->m_ID;
	m_items[12] = Tile::cloth_30->m_ID;
	m_items[13] = Tile::cloth_40->m_ID;
	m_items[14] = Tile::cloth_50->m_ID;
	m_items[15] = Tile::cloth_60->m_ID;
	m_items[16] = Tile::cloth_70->m_ID;
	m_items[17] = Tile::glass->m_ID;
	m_items[18] = Tile::cloth_01->m_ID;
	m_items[19] = Tile::cloth_11->m_ID;
	m_items[20] = Tile::cloth_21->m_ID;
	m_items[21] = Tile::cloth_31->m_ID;
	m_items[22] = Tile::cloth_41->m_ID;
	m_items[23] = Tile::stairs_wood->m_ID;
	m_items[24] = Tile::stairs_stone->m_ID;
	m_items[25] = Tile::stoneSlabHalf->m_ID;
	m_items[26] = Tile::sand->m_ID;
	m_items[27] = Tile::ladder->m_ID;
	m_items[28] = Tile::torch->m_ID;
	m_items[29] = Tile::flower->m_ID;
	m_items[30] = Tile::rose->m_ID;
	m_items[31] = Tile::mushroom1->m_ID;
	m_items[32] = Tile::mushroom2->m_ID;
	m_items[33] = Tile::reeds->m_ID;
	m_items[34] = Tile::obsidian->m_ID;
	m_items[35] = Tile::dirt->m_ID;

#ifdef DEMO
	ShuffleInventoryForDemo(m_hotbar, m_items);
#endif

#ifdef ENH_EXTRA_ITEMS_IN_INV
	// populate the 5th row now with items that might be of interest
	m_items[36] = Tile::tnt->m_ID;
	m_items[37] = Item::camera->m_itemID;
	m_items[38] = Item::door_wood->m_itemID;
	m_items[39] = Tile::gravel->m_ID;
	m_items[40] = Tile::cloth->m_ID;
	m_items[41] = Tile::mossStone->m_ID;
	m_items[42] = Tile::bookshelf->m_ID;
	m_items[43] = Tile::sponge->m_ID;
	m_items[44] = Tile::sapling->m_ID;
#endif
#else
	m_items[0] = Tile::stoneBrick->m_ID;
	m_items[1] = Tile::rock->m_ID;
	m_items[2] = Tile::emeraldOre->m_ID;
	m_items[3] = Tile::goldOre->m_ID;
	m_items[4] = Tile::ironOre->m_ID;
	m_items[5] = Tile::coalOre->m_ID;
	m_items[6] = Tile::lapisOre->m_ID;
	m_items[7] = Tile::redStoneOre->m_ID;
	m_items[8] = Tile::clay->m_ID;
	m_items[9] = Tile::emeraldBlock->m_ID;
	m_items[10] = Tile::goldBlock->m_ID;
	m_items[11] = Tile::ironBlock->m_ID;
	m_items[12] = Tile::unbreakable->m_ID;
	m_items[13] = Tile::lapisBlock->m_ID;
	m_items[14] = Tile::redBrick->m_ID;
	m_items[15] = Tile::mossStone->m_ID;
	m_items[16] = Tile::stoneSlabHalf->m_ID;
	m_items[17] = Tile::obsidian->m_ID;
	m_items[18] = Tile::netherrack->m_ID;
	m_items[19] = Tile::glowStone->m_ID;
	m_items[20] = Tile::soulSand->m_ID;
	m_items[21] = Tile::treeTrunk->m_ID;

	m_items[22] = Tile::treeTrunk->m_ID; //spruce
	m_items[23] = Tile::treeTrunk->m_ID; //birch

	m_items[24] = Tile::leaves->m_ID;
	m_items[25] = Tile::dirt->m_ID;
	m_items[26] = Tile::grass->m_ID;
	m_items[27] = Tile::sand->m_ID;
	m_items[28] = Tile::sandStone->m_ID;
	m_items[29] = Tile::gravel->m_ID;
	m_items[30] = Tile::cobweb->m_ID;
	m_items[31] = Tile::wood->m_ID;
	m_items[32] = Tile::sapling->m_ID;
	m_items[33] = Tile::sponge->m_ID;
	m_items[34] = Tile::ice->m_ID;
	m_items[35] = Tile::snow->m_ID;
	m_items[36] = Tile::flower->m_ID;
	m_items[37] = Tile::rose->m_ID;
	m_items[38] = Tile::mushroom1->m_ID;
	m_items[39] = Tile::mushroom2->m_ID;
	m_items[40] = Tile::reeds->m_ID;
	m_items[41] = Tile::cactus->m_ID;
	m_items[42] = Tile::rock->m_ID; //pumpkin
	m_items[43] = Tile::rock->m_ID; //jack o lanturn
	m_items[44] = Tile::rock->m_ID; //chest
	m_items[45] = Tile::rock->m_ID; //crafting table
	m_items[46] = Tile::glass->m_ID;
	m_items[47] = Tile::tnt->m_ID;
	m_items[48] = Tile::rock->m_ID; //bookshelf
	m_items[49] = Tile::rock->m_ID; //white wool
	m_items[50] = Tile::rock->m_ID; //orange wool
	m_items[51] = Tile::rock->m_ID; //magenta wool

	m_items[52] = Tile::cloth_41->m_ID; // Light Blue Wool
	m_items[53] = Tile::cloth_31->m_ID; // Yellow Wool
	m_items[54] = Tile::cloth_21->m_ID; // Lime Wool
	m_items[55] = Tile::cloth_11->m_ID; // Pink Wool
	m_items[56] = Tile::cloth_01->m_ID; // Grey Wool
	m_items[57] = Tile::cloth_70->m_ID; // Light Grey Wool
	m_items[58] = Tile::cloth_60->m_ID; // Cyan Wool
	m_items[59] = Tile::cloth_50->m_ID; // Purple Wool
	m_items[60] = Tile::cloth_40->m_ID; // Blue Wool
	m_items[61] = Tile::cloth_30->m_ID; // Brown Wool
	m_items[62] = Tile::cloth_20->m_ID; // Green Wool
	m_items[63] = Tile::cloth_10->m_ID; // Red Wool
	m_items[64] = Tile::rock->m_ID; //black wool

	m_items[65] = Tile::rock->m_ID; //dispenser
	m_items[66] = Tile::rock->m_ID; //furnace
	m_items[67] = Tile::rock->m_ID; //note block
	m_items[68] = Tile::rock->m_ID; //jukebox
	m_items[69] = Tile::ladder->m_ID;
	m_items[70] = Tile::rock->m_ID; //rail
	m_items[71] = Tile::torch->m_ID; 
	m_items[72] = Tile::stairs_wood->m_ID;
	m_items[73] = Tile::stairs_stone->m_ID;
	m_items[74] = Tile::rock->m_ID; //lever
	m_items[75] = Tile::rock->m_ID; //redstone torch
	m_items[76] = Tile::rock->m_ID; //cake
	m_items[77] = Item::door_wood->m_itemID;
	m_items[78] = Tile::water->m_ID;
	m_items[79] = Tile::lava->m_ID;
	m_items[80] = Item::door_iron->m_itemID;
	//m_items[81] = Item::redStone->m_itemID; //redstone
	m_items[82] = Item::reeds->m_itemID; //reeds
	//m_items[83] = Item::cake->m_itemID; //cake
    m_items[83] = Item::sword_iron->m_itemID;
	//m_items[84] = Item::bed->m_itemID; //bed
	m_items[84] = Tile::fire->m_ID; //cake
	m_items[85] = Item::door_iron->m_itemID; //redstone repeater
	m_items[86] = Item::camera->m_itemID;
	m_items[87] = Tile::spawner->m_ID;
	m_items[88] = Tile::rock->m_ID; //lit furnace
	m_items[89] = Tile::rock->m_ID; //double stone slab
#endif
}

#ifdef ENH_ENABLE_9TH_SLOT
#define HOTBAR_DIFF 0
#else
#define HOTBAR_DIFF 1
#endif

int Inventory::getSelectionSize()
{
	return C_MAX_HOTBAR_ITEMS;
}

int Inventory::getSelectionSlotItemId(int slot)
{
	if (slot >= 0 && slot < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		return m_hotbar[slot];

	if (slot > C_MAX_HOTBAR_ITEMS + C_MAX_INVENTORY_ITEMS - 1 || slot < 0)
		return -1;

	return m_items[slot - C_MAX_HOTBAR_ITEMS];
}

void Inventory::setSelectionSlotItemId(int slotNo, int item)
{
	if (slotNo >= 0 && slotNo < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		m_hotbar[slotNo] = item;
}

int Inventory::getSelectedItemId()
{
	return getSelectionSlotItemId(m_SelectedHotbarSlot);
}

void Inventory::selectSlot(int slotNo)
{
	if (slotNo < 0 || slotNo >= C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		return;

	m_SelectedHotbarSlot = slotNo;
}
