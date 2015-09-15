#include "Screens/GameScreen.h"

using namespace std;

GameScreen::GameScreen(CharacterCore* core) : Screen(core)
{
}

GameScreen::~GameScreen()
{
	delete m_interface;
}

void GameScreen::notifyItemChange(const std::string& itemID, int amount)
{
	// add to core
	m_characterCore->notifyItemChange(itemID, amount);
	m_progressLog.addItemProgress(itemID, amount);
	m_interface->reloadInventory(itemID);
}

void GameScreen::notifyQuestConditionFulfilled(const std::string& questID, const std::string& condition)
{
	// TODO
	m_characterCore->setQuestConditionFulfilled(questID, condition);
	m_progressLog.addQuestProgress(questID);
	m_interface->reloadQuestLog(questID);
}

void GameScreen::notifyQuestTargetKilled(const std::string& questID, const std::string& name)
{
	// TODO
	m_characterCore->setQuestTargetKilled(std::make_pair(questID, name));
	m_progressLog.addQuestProgress(questID);
	m_interface->reloadQuestLog(questID);
}

Screen* GameScreen::update(const sf::Time& frameTime)
{
	m_interface->update(frameTime);
	m_progressLog.update(frameTime);
	return this;
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	m_interface->render(renderTarget);
	m_progressLog.render(renderTarget);
}
