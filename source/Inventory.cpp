#include "Inventory.hpp"
#include "Item.hpp"

// lower 16 bits are used for the ID and higher 8 are used for the Aux
#define MAKE_AUX(id) ((id << 16) & 0x000F0000)
#define GET_AUX(id) ((id & 0x000F0000) >> 16)
#define GET_ID(id) (id & 0xFFF0FFFF)

//int makeAux(int aux)
//{
//	return (aux & 0xF) << 16;
//}
//
//int getAux(int combo)
//{
//	int id = combo & 0xFF00FFFF;
//	int aux = combo & 0xF0000;
//
//	if (id <= -1)
//		return 0;
//
//	if (combo & 0xF0000 == 0)
//		return 0;
//	
//	combo = combo & 0xF0000;
//	combo = combo >> 16;
//
//	return combo;
//}
//
//int getID(int combo)
//{
//	if (combo <= -1)
//		return 0;
//
//	combo = combo & 0x0F00;
//	combo = combo >> 16;
//
//	return combo;
//}

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
	int s = 0; // slot number

	m_items[s++] = Tile::stoneBrick->m_ID;
	m_items[s++] = Tile::rock->m_ID;
	m_items[s++] = Tile::emeraldOre->m_ID;
	m_items[s++] = Tile::goldOre->m_ID;
	m_items[s++] = Tile::ironOre->m_ID;
	m_items[s++] = Tile::coalOre->m_ID;
	m_items[s++] = Tile::lapisOre->m_ID;
	m_items[s++] = Tile::redStoneOre->m_ID;
	m_items[s++] = Tile::clay->m_ID;
	m_items[s++] = Tile::emeraldBlock->m_ID;
	m_items[s++] = Tile::goldBlock->m_ID;
	m_items[s++] = Tile::ironBlock->m_ID;
	m_items[s++] = Tile::unbreakable->m_ID;
	m_items[s++] = Tile::lapisBlock->m_ID;
	m_items[s++] = Tile::redBrick->m_ID;
	m_items[s++] = Tile::mossStone->m_ID;
	m_items[s++] = Tile::stoneSlabHalf->m_ID;
	m_items[s++] = Tile::obsidian->m_ID;
	m_items[s++] = Tile::netherrack->m_ID;
	m_items[s++] = Tile::glowStone->m_ID;
	m_items[s++] = Tile::soulSand->m_ID;
	m_items[s++] = Tile::treeTrunk->m_ID;

	m_items[s++] = Tile::treeTrunk->m_ID + MAKE_AUX(1); //spruce
	m_items[s++] = Tile::treeTrunk->m_ID + MAKE_AUX(2); //birch

	m_items[s++] = Tile::leaves->m_ID;
	m_items[s++] = Tile::dirt->m_ID;
	m_items[s++] = Tile::grass->m_ID;
	m_items[s++] = Tile::sand->m_ID;
	m_items[s++] = Tile::sandStone->m_ID;
	m_items[s++] = Tile::gravel->m_ID;
	m_items[s++] = Tile::cobweb->m_ID;
	m_items[s++] = Tile::wood->m_ID;
	m_items[s++] = Tile::sapling->m_ID;
	m_items[s++] = Tile::sponge->m_ID;
	m_items[s++] = Tile::ice->m_ID;
	m_items[s++] = Tile::snow->m_ID;
	m_items[s++] = Tile::flower->m_ID;
	m_items[s++] = Tile::rose->m_ID;
	m_items[s++] = Tile::mushroom1->m_ID;
	m_items[s++] = Tile::mushroom2->m_ID;
	m_items[s++] = Tile::reeds->m_ID;
	m_items[s++] = Tile::cactus->m_ID;
	//m_items[s++] = Tile::rock->m_ID; //pumpkin
	//m_items[s++] = Tile::rock->m_ID; //jack o lanturn
	//m_items[s++] = Tile::rock->m_ID; //chest
	//m_items[s++] = Tile::rock->m_ID; //crafting table
	m_items[s++] = Tile::glass->m_ID;
	m_items[s++] = Tile::tnt->m_ID;
	m_items[s++] = Tile::bookshelf->m_ID; //bookshelf

	m_items[s++] = Tile::cloth->m_ID; //white wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(1); //orange wool
    m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(2); //magenta wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(3); // Light Blue Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(4); // Yellow Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(5); // Lime Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(6); // Pink Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(7); // Grey Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(8); // Light Grey Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(9); // Cyan Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(10); // Purple Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(11); // Blue Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(12); // Brown Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(13); // Green Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(14); // Red Wool
	m_items[s++] = Tile::cloth->m_ID + MAKE_AUX(15); //black wool

	//m_items[s++] = Tile::rock->m_ID; //dispenser
	//m_items[s++] = Tile::rock->m_ID; //furnace
	//m_items[s++] = Tile::rock->m_ID; //note block
	//m_items[s++] = Tile::rock->m_ID; //jukebox
	m_items[s++] = Tile::ladder->m_ID;
	//m_items[s++] = Tile::rock->m_ID; //rail
	m_items[s++] = Tile::torch->m_ID;
	m_items[s++] = Tile::stairs_wood->m_ID;
	m_items[s++] = Tile::stairs_stone->m_ID;
	//m_items[s++] = Tile::rock->m_ID; //lever
	//m_items[s++] = Tile::rock->m_ID; //redstone torch
	//m_items[s++] = Tile::rock->m_ID; //cake
	m_items[s++] = Item::door_wood->m_itemID;
	m_items[s++] = Tile::water->m_ID;
	m_items[s++] = Tile::lava->m_ID;
	m_items[s++] = Item::door_iron->m_itemID;
	//m_items[s++] = Item::redStone->m_itemID; //redstone
	m_items[s++] = Item::reeds->m_itemID; //reeds
	//m_items[s++] = Item::cake->m_itemID; //cake
    m_items[s++] = Item::sword_iron->m_itemID;
	//m_items[s++] = Item::bed->m_itemID; //bed
	m_items[s++] = Tile::fire->m_ID; //cake
	m_items[s++] = Item::door_iron->m_itemID; //redstone repeater
	m_items[s++] = Item::camera->m_itemID;
	m_items[s++] = Tile::spawner->m_ID;
	//m_items[s++] = Tile::rock->m_ID; //lit furnace
	m_items[s++] = Tile::stoneSlab->m_ID; //double stone slab

	if (s >= sizeof(m_items) / sizeof(int))
	{
		throw std::exception("More items were added to the m_items array than should have been. You probably broke something.");
	}
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
		return GET_ID(m_hotbar[slot]);

	if (slot > C_MAX_HOTBAR_ITEMS + C_MAX_INVENTORY_ITEMS - 1 || slot < 0)
		return -1;

	return GET_ID(m_items[slot - C_MAX_HOTBAR_ITEMS]);
}

int Inventory::getSelectionSlotItemAux(int slot)
{
	if (slot >= 0 && slot < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		return GET_AUX(m_hotbar[slot]);

	if (slot > C_MAX_HOTBAR_ITEMS + C_MAX_INVENTORY_ITEMS - 1 || slot < 0)
		return -1;

	return GET_AUX(m_items[slot - C_MAX_HOTBAR_ITEMS]);
}

int Inventory::getSelectionSlotCombo(int slot)
{
	if (slot >= 0 && slot < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		return m_hotbar[slot];

	if (slot > C_MAX_HOTBAR_ITEMS + C_MAX_INVENTORY_ITEMS - 1 || slot < 0)
		return -1;

	return m_items[slot - C_MAX_HOTBAR_ITEMS];
}

void Inventory::setSelectionSlotItemId(int slotNo, int item, int aux)
{
	if (slotNo >= 0 && slotNo < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		m_hotbar[slotNo] = item + MAKE_AUX(aux);
}

void Inventory::setSelectionSlotCombo(int slotNo, int combo)
{
	if (slotNo >= 0 && slotNo < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		m_hotbar[slotNo] = combo;
}

int Inventory::getSelectedItemId()
{
	return getSelectionSlotItemId(m_SelectedHotbarSlot);
}

int Inventory::getSelectedItemAux()
{
	return getSelectionSlotItemAux(m_SelectedHotbarSlot);
}

void Inventory::selectSlot(int slotNo)
{
	if (slotNo < 0 || slotNo >= C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		return;

	m_SelectedHotbarSlot = slotNo;
}
