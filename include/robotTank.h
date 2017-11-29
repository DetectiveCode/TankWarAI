#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>

#include "tank.h"
#include "position.h"
#include "playerTank.h"
#include "node.h"

class RobotTank : public Tank
{
private:
	bool visible = true;
	bool fire = false;
	bool stop = false;
	bool collide = false;

	float barrelAngle;

	std::vector<Node> nodeVector;

public:
	RobotTank();
	~RobotTank();

	void setVisible() { visible = true; } //!< Make the AI tank visible to the player
	void setInvisible() { visible = false; } //!< Make the AI tank invisible to the player
	bool isVisible()const { return visible; } //!< Is the tank visiable to the player?

	void move();
	bool calculatePath(Position &p);
	void shoot();
	void reset();
	void collided();
	float calculateAngle(Position &p);
	bool withinRange(Position &p);

	void markTarget(Position &p);
	void markEnemy(Position &p);
	void markBase(Position &p);
	void markShell(Position &p);
	bool isFiring();
	void score(int thisScore, int enemyScore);
};