#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class LevelMainCharacter;
class InteractComponent;

namespace particles {
	class LineSpawner;
	class TextureParticleSystem;
	class AngledVelocityGenerator;
}

class MirrorTile final : public LevelDynamicTile {
public:
	explicit MirrorTile(LevelScreen* levelScreen);

	void update(const sf::Time& frameTime) override;

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;
	void setRotation(float rotation);
	void setActive(bool active);
	void setColor(const sf::Color& color);
	
	float getRotation() const;

	void switchLever();
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Mirror; }

private:
	std::string getSpritePath() const override;

private:
	// the mirror lever can only be used by hand if the main char is in this range.
	// it is however always possible for a telekinesis spell to activate the lever if its projectile reaches the lever.
	static const float ACTIVATE_RANGE;
	static const float TICK_ANGLE;

	InteractComponent* m_interactComponent;
	float m_currentRotation;
	bool  m_isLocked;
};

class Ray final {
public:
	Ray(const Level* level, const sf::Color& color);
	~Ray();

	// casts the ray using origin and direction and returns the end position
	const sf::Vector2f& cast(const sf::Vector2f& origin, const sf::Vector2f& direction);
	const sf::Vector2f& cast(const sf::Vector2f& origin, float angle);
	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& target);

	const sf::Color& getColor() const;

	MirrorTile* getMirrorTile() const;

private:
	void loadParticleSystem();

	particles::TextureParticleSystem* m_particleSystem = nullptr;
	particles::LineSpawner* m_lineSpawner = nullptr;
	particles::AngledVelocityGenerator* m_lineVelGen = nullptr;

	sf::Vector2f m_startPos;
	sf::Vector2f m_endPos;
	sf::Vector2f m_direction;
	sf::Color m_color;

	MirrorTile* m_mirrorTile = nullptr;
	const Level* m_level;
	float m_currentAngle = 0.f;
};

class MirrorRay final : public GameObject {
public:
	explicit MirrorRay(LevelScreen* levelScreen);
	~MirrorRay();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void initRay(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Color& color);

	GameObjectType getConfiguredType() const override { return GameObjectType::_Undefined; }

private:
	LevelScreen* m_screen;
	sf::Vector2f m_origin;
	sf::Vector2f m_direction;

	std::vector<Ray*> m_rays;
};