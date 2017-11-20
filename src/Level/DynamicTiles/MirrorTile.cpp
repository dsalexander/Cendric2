#include "Level/DynamicTiles/MirrorTile.h"
#include "Spells/Spell.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Particles/ParticleSystem.h"
#include "World/CustomParticleUpdaters.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Mirror, MirrorTile)

const float MirrorTile::ACTIVATE_RANGE = 80.f;
const float MirrorTile::TICK_ANGLE = 15.f;

MirrorTile::MirrorTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_interactComponent = new InteractComponent(g_textProvider->getText("FocusCrystal"), this, m_mainChar);
	m_interactComponent->setInteractRange(ACTIVATE_RANGE);
	m_interactComponent->setInteractText("ToRotate");
	m_interactComponent->setOnInteract(std::bind(&MirrorTile::switchLever, this));
	addComponent(m_interactComponent);
}

void MirrorTile::update(const sf::Time& frameTime) {
	setState(GameObjectState::Inactive);
	m_animatedSprite.setColor(COLOR_WHITE);
	LevelDynamicTile::update(frameTime);
}

bool MirrorTile::init(const LevelTileProperties& properties) {
	setPositionOffset(sf::Vector2f(15.f, 15.f));
	setSpriteOffset(sf::Vector2f(-15.f, -15.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 20.f, 20.f));

	m_currentRotation = 0.f;
	if (contains(properties, std::string("angle"))) {
		setRotation(static_cast<float>(std::stoi(properties.at(std::string("angle")))));
	}

	m_isLocked = contains(properties, std::string("locked"));
	if (m_isLocked) {
		m_interactComponent->setActive(false);
	}

	return true;
}

void MirrorTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* defaultAnimation = new Animation(sf::seconds(10.0f));
	defaultAnimation->setSpriteSheet(tex);
	defaultAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Inactive, defaultAnimation);

	Animation* activeAnimation = new Animation(sf::seconds(10.0f));
	activeAnimation->setSpriteSheet(tex);
	activeAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Active, activeAnimation);

	// initial values
	setState(GameObjectState::Inactive);
	playCurrentAnimation(false);

	// set origin of animation to center
	m_animatedSprite.setOrigin(sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f));
}

void MirrorTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Telekinesis:
		switchLever();
		spell->setDisposed();
		break;
	default:
		break;
	}
}

void MirrorTile::onRightClick() {
	if (m_mainChar->isDead()) return;
	if (m_isLocked) return;
	// check if lever is in range
	if (dist(m_mainChar->getCenter(), getCenter()) <= ACTIVATE_RANGE) {
		switchLever();
	}
	else {
		m_screen->setNegativeTooltip("OutOfRange");
	}
	g_inputController->lockAction();
}

void MirrorTile::switchLever() {
	if (m_isLocked) return;
	setRotation(m_currentRotation + TICK_ANGLE);
}

void MirrorTile::setRotation(float rotation) {
	m_currentRotation = std::fmod(rotation, 360.f);
	m_animatedSprite.setRotation(m_currentRotation);
}

void MirrorTile::setActive(bool active) {
	setState(active ? GameObjectState::Active : GameObjectState::Inactive);
}

void MirrorTile::setColor(const sf::Color& color) {
	m_animatedSprite.setColor(color);
}

float MirrorTile::getRotation() const {
	return degToRad(m_currentRotation - 90.f);
}

std::string MirrorTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_mirror.png";
}

///////////// Ray /////////////////////

Ray::Ray(const Level* level, const sf::Color& color) {
	m_level = level;
	m_color = color;
	loadParticleSystem();
}

Ray::~Ray() {
	delete m_particleSystem;
}

const sf::Vector2f& Ray::cast(const sf::Vector2f& origin, float angle) {
	sf::Vector2f dir;
	dir.x = std::cos(angle);
	dir.y = std::sin(angle);
	return cast(origin, dir);
}

const sf::Vector2f& Ray::cast(const sf::Vector2f& origin, const sf::Vector2f& direction) {
	m_startPos = origin;
	m_direction = direction;

	RaycastQueryRecord rec;
	rec.rayOrigin = origin;
	rec.rayDirection = direction;

	m_level->raycast(rec);
	m_endPos = rec.rayHit;
	m_mirrorTile = dynamic_cast<MirrorTile*>(rec.mirrorTile);

	if (m_mirrorTile) {
		m_mirrorTile->setActive(true);
		m_mirrorTile->setColor(m_color);
	}

	return m_endPos;
}

void Ray::update(const sf::Time& frameTime) {
	if (!m_particleSystem) return;

	m_lineSpawner->point1 = m_startPos;
	m_lineSpawner->point2 = m_endPos;
	
	auto dir = m_lineSpawner->point2 - m_lineSpawner->point1;
	dir = dir / norm(dir);

	m_lineVelGen->minAngle = radToDeg(std::atan2(dir.y, dir.x)) + 90.f;
	m_lineVelGen->maxAngle = m_lineVelGen->minAngle;
	m_particleSystem->update(frameTime);
}

void Ray::render(sf::RenderTarget& target) {
	if (!m_particleSystem) return;
	m_particleSystem->render(target);
}

MirrorTile* Ray::getMirrorTile() const {
	return m_mirrorTile;
}

const sf::Color& Ray::getColor() const {
	return m_color;
}

void Ray::loadParticleSystem() {
	delete m_particleSystem;

	auto tex = g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_STAR);
	tex->setSmooth(true);
	m_particleSystem = new particles::TextureParticleSystem(200, tex);
	m_particleSystem->additiveBlendMode = true;
	m_particleSystem->emitRate = 1000.f;

	m_lineSpawner = new particles::LineSpawner();
	m_particleSystem->addSpawner(m_lineSpawner);

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 10.f;
	m_particleSystem->addGenerator(sizeGen);

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = m_color;
	colGen->maxStartCol = m_color;
	colGen->minEndCol = m_color;
	colGen->maxEndCol = m_color;
	m_particleSystem->addGenerator(colGen);
	
	m_lineVelGen = new particles::AngledVelocityGenerator();
	m_lineVelGen->minStartSpeed = 20.f;
	m_lineVelGen->maxStartSpeed = 50.f;
	m_lineVelGen->minAngle = -90.f;
	m_lineVelGen->maxAngle = 90.f;
	m_particleSystem->addGenerator(m_lineVelGen);
	
	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.1f;
	timeGen->maxTime = 0.2f;
	m_particleSystem->addGenerator(timeGen);
	
	m_particleSystem->addUpdater(new particles::EulerUpdater());
	m_particleSystem->addUpdater<particles::TimeUpdater>();
	m_particleSystem->addUpdater<particles::ColorUpdater>();
}

//////////// Mirror Ray ///////////////

MirrorRay::MirrorRay(LevelScreen* levelScreen) {
	m_screen = levelScreen;
}

MirrorRay::~MirrorRay() {
	CLEAR_VECTOR(m_rays);
}

void MirrorRay::update(const sf::Time& frameTime) {
	if (m_rays.size() == 0) return;
	m_rays[0]->cast(m_origin, m_direction);

	for (size_t i = 0; i < m_rays.size();) {
		auto ray = m_rays[i];
		ray->update(frameTime);
		if (!ray->getMirrorTile()) {
			for (size_t j = i + 1; j < m_rays.size(); j++) {
				delete m_rays[j];
				m_rays.erase(m_rays.begin() + j);
			}
			break;
		}

		i++;
		if (i >= m_rays.size()) {
			// add ray
			Ray* newRay = new Ray(m_screen->getWorld(), ray->getColor());
			m_rays.push_back(newRay);
		}
		m_rays[i]->cast(ray->getMirrorTile()->getCenter(), ray->getMirrorTile()->getRotation());
	}
}

void MirrorRay::render(sf::RenderTarget& target) {
	for (auto ray : m_rays) {
		ray->render(target);
	}
}

void MirrorRay::initRay(const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Color& color) {
	CLEAR_VECTOR(m_rays);

	Ray* ray = new Ray(m_screen->getWorld(), color);
	m_rays.push_back(ray);

	m_origin = origin;
	m_direction = direction;
}
