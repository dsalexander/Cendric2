#include "Level/MOBBehavior/MovingBehaviors/AggressiveCreepingBehavior.h"

AggressiveCreepingBehavior::AggressiveCreepingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	CreepingBehavior(enemy) {
}

void AggressiveCreepingBehavior::execHandleMovementInput() {
	// movement AI
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;
	if (m_isGrounded && hasTarget && m_enemy->getEnemyState() == EnemyState::Chasing) {
		// the enemy tries to get near its target
		if (isCreepingOnX()) {
			if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
				m_movingDirectionX = -1;
			}
			else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
				m_movingDirectionX = 1;
			}
			else {
				m_movingDirectionX = 0;
			}
		}
		else {
			if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
				m_movingDirectionY = -1;
			}
			else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
				m_movingDirectionY = 1;
			}
			else {
				m_movingDirectionY = 0;
			}
		}
	}
	else if (m_isGrounded && hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		// the enemy tries to get away from its target
		if (isCreepingOnX()) {
			m_movingDirectionX = targetCenter.x < center.x ? 1 : -1;
		}
		else {
			m_movingDirectionY = targetCenter.y < center.y ? 1 : -1;
		}
	}
}