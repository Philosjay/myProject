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


	data[Aircraft::RaptorA1].hitpoints = 20;
	data[Aircraft::RaptorA1].speed = 70.f;
	data[Aircraft::RaptorA1].texture = Textures::Raptor;
	data[Aircraft::RaptorA1].directions.push_back(Direction(+45.f, 200.f));
	data[Aircraft::RaptorA1].directions.push_back(Direction(+90.f, 500.f));
	data[Aircraft::RaptorA1].fireInterval = sf::Time::Zero;

	data[Aircraft::RaptorA2].hitpoints = 20;
	data[Aircraft::RaptorA2].speed = 70.f;
	data[Aircraft::RaptorA2].texture = Textures::Raptor;
	data[Aircraft::RaptorA2].directions.push_back(Direction(-45.f, 200.f));
	data[Aircraft::RaptorA2].directions.push_back(Direction(-90.f, 500.f));
	data[Aircraft::RaptorA2].fireInterval = sf::Time::Zero;

	data[Aircraft::AvengerA1].hitpoints = 40;
	data[Aircraft::AvengerA1].speed = 70.f;
	data[Aircraft::AvengerA1].texture = Textures::Avenger;
	data[Aircraft::AvengerA1].directions.push_back(Direction(+45.f, 500.f));
	data[Aircraft::AvengerA1].fireInterval = sf::seconds(2);
	
	data[Aircraft::AvengerA2].hitpoints = 40;
	data[Aircraft::AvengerA2].speed = 70.f;
	data[Aircraft::AvengerA2].texture = Textures::Avenger;
	data[Aircraft::AvengerA2].directions.push_back(Direction(-45.f, 500.f));
	data[Aircraft::AvengerA2].fireInterval = sf::seconds(2);
	
	data[Aircraft::RaptorB1].hitpoints = 20;
	data[Aircraft::RaptorB1].speed = 70.f;
	data[Aircraft::RaptorB1].texture = Textures::Raptor;
	data[Aircraft::RaptorB1].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::RaptorB1].directions.push_back(Direction(-25.f, 160.f));
	data[Aircraft::RaptorB1].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::RaptorB1].fireInterval = sf::Time::Zero;

	data[Aircraft::RaptorC1].hitpoints = 20;
	data[Aircraft::RaptorC1].speed = 70.f;
	data[Aircraft::RaptorC1].texture = Textures::Raptor;
	data[Aircraft::RaptorC1].directions.push_back(Direction(0.f, 500.f));
	data[Aircraft::RaptorC1].fireInterval = sf::Time::Zero;

	data[Aircraft::RaptorC2].hitpoints = 20;
	data[Aircraft::RaptorC2].speed = 70.f;
	data[Aircraft::RaptorC2].texture = Textures::Raptor;
	data[Aircraft::RaptorC2].directions.push_back(Direction(0.f, 500.f));
	data[Aircraft::RaptorC2].fireInterval = sf::Time::Zero;

	data[Aircraft::AvengerB1].hitpoints = 40;
	data[Aircraft::AvengerB1].speed = 70.f;
	data[Aircraft::AvengerB1].texture = Textures::Avenger;
	data[Aircraft::AvengerB1].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::AvengerB1].directions.push_back(Direction(-25.f, 160.f));
	data[Aircraft::AvengerB1].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::AvengerB1].fireInterval = sf::seconds(1.5);

	data[Aircraft::RaptorB2].hitpoints = 20;
	data[Aircraft::RaptorB2].speed = 70.f;
	data[Aircraft::RaptorB2].texture = Textures::Raptor;
	data[Aircraft::RaptorB2].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::RaptorB2].directions.push_back(Direction(-25.f, 160.f));
	data[Aircraft::RaptorB2].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::RaptorB2].fireInterval = sf::Time::Zero;



	data[Aircraft::AvengerB2].hitpoints = 40;
	data[Aircraft::AvengerB2].speed = 70.f;
	data[Aircraft::AvengerB2].texture = Textures::Avenger;
	data[Aircraft::AvengerB2].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::AvengerB2].directions.push_back(Direction(-25.f, 160.f));
	data[Aircraft::AvengerB2].directions.push_back(Direction(+25.f, 80.f));
	data[Aircraft::AvengerB2].fireInterval = sf::seconds(1.5);

	data[Aircraft::AvengerC1].hitpoints = 40;
	data[Aircraft::AvengerC1].speed = 70.f;
	data[Aircraft::AvengerC1].texture = Textures::Avenger;
	data[Aircraft::AvengerC1].directions.push_back(Direction(0.f, 80.f));
	data[Aircraft::AvengerC1].directions.push_back(Direction(-25.f, 100.f));
	data[Aircraft::AvengerC1].directions.push_back(Direction(-50.f, 100.f));
	data[Aircraft::AvengerC1].directions.push_back(Direction(-75.f, 100.f));
	data[Aircraft::AvengerC1].directions.push_back(Direction(-90.f, 200.f));
	data[Aircraft::AvengerC1].fireInterval = sf::seconds(1.5);

	data[Aircraft::AvengerC2].hitpoints = 40;
	data[Aircraft::AvengerC2].speed = 70.f;
	data[Aircraft::AvengerC2].texture = Textures::Avenger;
	data[Aircraft::AvengerC2].directions.push_back(Direction(0.f, 80.f));
	data[Aircraft::AvengerC2].directions.push_back(Direction(+25.f, 100.f));
	data[Aircraft::AvengerC2].directions.push_back(Direction(+50.f, 100.f));
	data[Aircraft::AvengerC2].directions.push_back(Direction(+75.f, 100.f));
	data[Aircraft::AvengerC2].directions.push_back(Direction(+90.f, 200.f));
	data[Aircraft::AvengerC2].fireInterval = sf::seconds(1.5);

	data[Aircraft::EagleA1].hitpoints = 100;
	data[Aircraft::EagleA1].speed = 130.f;
	data[Aircraft::EagleA1].texture = Textures::Eagle;
	data[Aircraft::EagleA1].directions.push_back(Direction(0.f, 100.f));
	data[Aircraft::EagleA1].directions.push_back(Direction(+110.f, 100.f));
	data[Aircraft::EagleA1].directions.push_back(Direction(+140.f, 200.f));
	data[Aircraft::EagleA1].directions.push_back(Direction(0.f, 30.f));
	data[Aircraft::EagleA1].directions.push_back(Direction(-110.f, 100.f));
	data[Aircraft::EagleA1].directions.push_back(Direction(-140.f, 200.f));
	data[Aircraft::EagleA1].directions.push_back(Direction(0.f, 40.f));
	data[Aircraft::EagleA1].directions.push_back(Direction(-90.f, 200.f));
	data[Aircraft::EagleA1].fireInterval = sf::seconds(2);

	data[Aircraft::EagleA2].hitpoints = 100;
	data[Aircraft::EagleA2].speed = 130.f;
	data[Aircraft::EagleA2].texture = Textures::Eagle;
	data[Aircraft::EagleA2].directions.push_back(Direction(0.f, 100.f));
	data[Aircraft::EagleA2].directions.push_back(Direction(-110.f, 100.f));
	data[Aircraft::EagleA2].directions.push_back(Direction(-140.f, 200.f));
	data[Aircraft::EagleA2].directions.push_back(Direction(0.f, 30.f));
	data[Aircraft::EagleA2].directions.push_back(Direction(+110.f, 100.f));
	data[Aircraft::EagleA2].directions.push_back(Direction(+140.f, 200.f));
	data[Aircraft::EagleA2].directions.push_back(Direction(0.f, 40.f));
	data[Aircraft::EagleA2].directions.push_back(Direction(+90.f, 200.f));
	data[Aircraft::EagleA2].fireInterval = sf::seconds(2);

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
	data[Projectile::Missile].speed = 300.f;
	data[Projectile::Missile].texture = Textures::Missile;

	return data;
}

