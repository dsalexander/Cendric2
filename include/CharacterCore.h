#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "FileIO/QuestLoader.h"
#include "World/Weapon.h"

#include "Structs/AttributeData.h"
#include "Structs/CharacterCoreData.h"
#include "Structs/Condition.h"
#include "Structs/MerchantData.h"
#include "ScreenOverlays/ScreenOverlay.h"

class ScreenManager;
class AchievementManager;

class CharacterCore final {
public:
	CharacterCore();
	~CharacterCore();

	// use this instead of a constructor to copy a character core
	CharacterCore* createFromThis();

	// returns the currently equipped item of type 'type'
	const std::string& getEquippedItem(ItemType type) const;
	// getter for currently equipped weapon. 
	const Weapon* getWeapon() const;

	QuestState getQuestState(const std::string& id) const;
	// sets the quest state of a certain quest. Returns whether the operation was successful.
	bool setQuestState(const std::string& id, QuestState state);
	void setQuickslot(const std::string& item, int nr);
	// loads a new game with a new core and default attributes
	void loadNew();
	// tries to create a new file with this name. if successful, returns true. if it already exists, returns false
	bool createFile(const std::string& fileName) const;
	// uses the character core reader to load a .sav file
	bool load(const std::string& fileName);
	bool quickload();
	// uses the character core writer to save a .sav file. The name is the name chosen by the user.
	bool save(const std::string& fileName, const std::string& name);
	bool quicksave();
	bool autosave();
	// loads the weapon with its slots (reload if already loaded)
	void loadWeapon();
	// loads all quests that are started, completed or failed.
	void loadQuests();
	// reloads only the weapon slots.
	void reloadWeaponSlots();
	// base attributes plus the attributes gotten by equipment
	AttributeData getTotalAttributes() const;
	// getter for items
	std::map<std::string, int>* getItems();
	// an item was added or removed. if itemID equals "gold", gold is added or removed
	// returns true iff an equipment item was removed
	bool notifyItemChange(const std::string& itemID, int amount);
	void setOverworld(const std::string& map);
	// save the position and map id. also sets "isInLevel" = false
	void setMap(const sf::Vector2f& position, const std::string& map);
	// save the position and level id  also sets "isInLevel" = true
	void setLevel(const sf::Vector2f& position, const std::string& level);
	// sets the position and map id of the forced map.
	void setForcedMap(const sf::Vector2f& position, const std::string& map);
	// replaces the map with the currently forced map and cleares the forced one.
	void replaceForcedMap();
	// sets the "in level" property of the character core data
	void setInLevel(bool inLevel);
	// equips an item at the given position and adds the former equipment part into the inventory. To deequip an item, use this method with an empty string as item id.
	// it returns the item that was previously equipped, empty string if none
	std::string equipItem(const std::string& item, ItemType type, bool keepOldItem = true);
	// tries to remove the spell from slot nr "slotNr"
	void removeSpell(int slotNr);
	// tries to add the spell with id "id" to slot nr "slotNr"
	void addSpell(SpellID id, int slotNr);
	// tries to remove a modifier with modifierNr "modifierNr" from weapon slot nr "slotNr"
	void removeModifier(int slotNr, int modifierNr);
	// tries to add a modifier of type "type" to weapon slot nr "slotNr" into the modifier slot with nr "modifierNr"
	// it uses "force" for that 
	void addModifier(const SpellModifier& modifier, int slotNr, int modifierNr);
	// adds permanent (!) attributes to the core.
	void addPermanentAttributes(const AttributeData& attributes);
	// getter for core part
	const CharacterCoreData& getData() const;
	// get explored tiles information
	ExploredTiles& getExploredTiles();
	// returns whether a map is already (partially) explored
	bool isMapExplored(const std::string& mapId) const;
	// signal enemy kill
	void setEnemyKilled(const std::string& level, int pos);
	// signal enemy looted
	void setEnemyLooted(const std::string& level, int pos);
	// signal item looted
	void setItemLooted(const std::string& level, int pos);
	// signal chest looted
	void setChestLooted(const std::string& level, int pos);
	// signal waypoint unlocked
	void setWaypointUnlocked(const std::string& map, int objectID, const sf::Vector2f& pos);
	// signal trigger triggered
	void setTriggerTriggered(const std::string& world, int objectID);
	// signal door opened
	void setDoorOpen(const std::string& world, int objectID);
	// tracks or untracks a quest
	void setQuestTracked(const std::string& questID, bool isTracked);
	// getter for quest data. If there is none for that id, returns nullptr
	const QuestData* getQuestData(const std::string& questID) const;
	// getter for number of targets with name "name" killed for quest with id "questID"
	int getNumberOfTargetsKilled(const std::string& questID, const std::string& name) const;
	// getter for number of targets in total with name "name" killed for quest with id "questID"
	int getNumberOfTotalTargets(const std::string& questID, const std::string& name) const;
	// is the quest with id "id" complete?
	bool isQuestComplete(const std::string& questID) const;
	// a target for a quest has been killed
	void setQuestTargetKilled(const std::string& questID, const std::string& name);
	// a condition for a quest has been fulfilled
	void setQuestConditionFulfilled(const std::string& questID, const std::string& condition);
	// is a quest condition for a certain quest fulfilled?
	bool isQuestConditionFulfilled(const std::string& questID, const std::string& condition) const;
	// is a quest description for a certain quest unlocked?
	bool isQuestDescriptionUnlocked(const std::string& questID, int description) const;
	// is a certain quest set as tracked?
	bool isQuestTracked(const std::string& questID) const;
	// is a spell learned?
	bool isSpellLearned(SpellID id) const;
	// is a spell equipped?
	bool isSpellEquipped(SpellID id) const;
	// is a waypoint unlocked
	bool isWaypointUnlocked(const std::string& mapId, int objectId) const;
	// is a item looted
	bool isItemLooted(const std::string& levelId, int position) const;
	// set a condition fulfilled. Returns whether the operation was successful.
	bool setConditionFulfilled(const std::string& conditionType, const std::string& condition);
	// unlock a certain quest description. Returns whether the operation was successful.
	bool unlockQuestDescription(const std::string& questID, int descriptionID);
	// is a condition fulfilled?
	bool isConditionFulfilled(const std::string& conditionType, const std::string& condition) const;
	// are all those conditions fulfilled?
	bool isConditionsFulfilled(const std::vector<Condition>& conditions) const;
	// is a enemy in a certain level dead?
	bool isEnemyKilled(const std::string& levelID, int objectID) const;
	// is a enemy in a certain level looted?
	bool isEnemyLooted(const std::string& levelID, int objectID) const;
	// was a oneway door already opened?
	bool isDoorOpen(const std::string& world, int objectID);
	// was a trigger already triggered?
	bool isTriggerTriggered(const std::string& world, int triggerID);
	// returns the amount of items of this key in his inventory and/or equipped. Also works for gold with itemID "gold".
	int getItemAmount(const std::string& itemID) const;
	// returns the amount of items of this key in the stored items. Also works for gold with itemID "gold".
	int getStoredItemAmount(const std::string& itemID) const;
	// returns whether cendric has at least the given amount of items of this key in his inventory and/or equipped. Also works for gold with itemID "gold".
	bool hasItem(const std::string& itemID, int amount) const;
	// returns whether cendric has at least the given amount of items of this key in the stored items. Also works for gold with itemID "gold".
	bool hasStoredItem(const std::string& itemID, int amount) const;
	// returns whether cendric has this item currently equipped
	bool isItemEquipped(const std::string& itemID) const;
	// returns a copy of the merchant data for that id. If it is not already present, tries to load it.
	MerchantData getMerchantData(const std::string& merchantID);
	// sets the merchant wares of this merchant id
	void setMerchantData(const std::string& merchantID, const std::map<std::string, int>& wares);
	// learns the spell with id "spellID"
	void learnSpell(SpellID spellID);
	// learns this modifier
	void learnModifier(SpellModifierType modifier, const std::string& levelID, int objectID);
	// learns this hint
	void learnHint(const std::string& hintKey);
	// sets the weather for this particular world 
	void setWeather(const std::string& worldID, const WeatherData& data);
	// gets the weather for this particular world. if its not set, returns nullptr.
	const WeatherData* getWeather(const std::string& worldID) const;
	// adds reputation to the corresponding fraction id
	void addReputation(FractionID fraction, int amount);
	// sets a guild
	void setGuild(FractionID fraction);
	// sets the key for the currently selected spell.
	void setWeaponSpell(Key key);
	// queries the reputation for the corresponding fraction
	int getReputation(FractionID fraction) const;
	// is this an autosave?
	bool isAutosave();
	// mark as autosave
	void setAutosave(bool value);
	// stores all items in the inventory temporarily and leaves the player only with a pickaxe and prisoners clothes
	void setCharacterJailed();
	// returns a map with all stored items
	const std::map<std::string, int>& getStoredItems() const;
	// returns a the stored gold
	int getStoredGold() const;
	// resets the stored items and gold
	void resetStoredItems();
	// sets an achievement unlocked in the data.
	void setAchievementUnlocked(const std::string& achievement);
	// sets a book read in the data.
	void setBookRead(const std::string& itemId);
	// increase death count by one
	void increaseDeathCount(const std::string& level);
	// marks the player as a cheater
	void setHashInvalid();
	// returns the current time played
	sf::Time getTimePlayed() const;

	// this is a very specific function, for the timewarp
	void timewarpToThrone();

protected:
	// protected constructor for copying
	CharacterCore(const CharacterCoreData& data);

private:
	// reload attributes based on equipped items (called by load). all attributes coming from food are removed.
	void reloadAttributes();
	// add gold to the data
	void addGold(int gold);
	// remove gold from the data
	void removeGold(int gold);
	// adds item(s) to the data
	void addItem(const std::string& item, int quantity);
	// removes item(s) to the data
	// returns true if an equipment item was removed
	bool removeItem(const std::string& item, int quantity);
	// adds item(s) to the stored data
	void addStoredItem(const std::string& item, int quantity);
	// removes item(s) to the stored data
	void removeStoredItem(const std::string& item, int quantity);
	// removes an already fulfilled condition
	void removeConditionFulfilled(const std::string& conditionType, const std::string& condition);
	// removes all conditions of type conditionType
	void removeConditionsFulfilled(const std::string& conditionType);

private:
	// base attributes plus the attributes of all currently equipped items
	AttributeData m_totalAttributes;

	std::map<std::string, QuestData> m_quests;
	Weapon* m_weapon = nullptr;

	CharacterCoreData m_data;

	sf::Clock m_stopwatch;

	bool m_isNew = false;
	bool m_isAutosave = false;
};