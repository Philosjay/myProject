#include "Include\datatables.hpp"
#include "Include\Aircraft.hpp"
#include "Include\Projectile.hpp"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<AircraftData> initializeAircraftData()
{
	std::vector<AircraftData> data(Aircraft::TypeCount);

	data[Aircraft::Eagle].hitpoints = 100;
	data[Aircraft::Eagle].speed = 200.f;
	data[Aircraft::Eagle].fireInterval = sf::seconds(1);
	data[Aircraft::Eagle].texture = Textures::Eagle;

	data[Aircraft::Raptor].hitpoints = 20;
	data[Aircraft::Raptor].speed = 80.f;
	data[Aircraft::Raptor].texture = Textures::Raptor;
	data[Aircraft::Raptor].directions.push_back(Direction(+45.f, 80.f));
	data[Aircraft::Raptor].directions.push_back(Direction(-45.f, 160.f));
	data[Aircraft::Raptor].directions.push_back(Direction(+45.f, 80.f));
	data[Aircraft::Raptor].fireInterval = sf::Time::Zero;

	data[Aircraft::Avenger].hitpoints = 40;
	data[Aircraft::Avenger].speed = 50.f;
	data[Aircraft::Avenger].texture = Textures::Avenger;
	data[Aircraft::Avenger].directions.push_back(Direction(+45.f,  50.f));
	data[Aircraft::Avenger].directions.push_back(Direction(  0.f,  50.f));
	data[Aircraft::Avenger].directions.push_back(Direction(-45.f, 100.f));
	data[Aircraft::Avenger].directions.push_back(Direction(  0.f,  50.f));
	data[Aircraft::Avenger].directions.push_back(Direction(+45.f,  50.f));
	data[Aircraft::Avenger].fireInterval = sf::seconds(2);

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Bullet;

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::Bullet;

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Textures::Missile;

	return data;
}

