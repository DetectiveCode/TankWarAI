#include "robotTank.h"

RobotTank::RobotTank() // Construtor
{
	bodyTex.loadFromFile("assets\\redTank.png");
	body.setTexture(bodyTex);
	body.setOrigin(100, 100);
	body.setScale(0.2f, 0.2f);

	turretTex.loadFromFile("assets\\redTankTurret.png");
	turret.setTexture(turretTex);
	turret.setOrigin(46, 44);
	turret.setScale(0.2f, 0.2f);

	numberOfShells = 15;

	clearMovement();
	stopTurret();

	//Create the nodes.
	nodeVector.push_back(Node(Position(400, 300))); //Middle
	nodeVector.push_back(Node(Position(100, 75))); //Top Left Corner
	nodeVector.push_back(Node(Position(100, 525))); //Bottom Left Corner
	nodeVector.push_back(Node(Position(700, 75))); //Top Right Corner
	nodeVector.push_back(Node(Position(700, 525))); //Bottom Right Corner
	nodeVector.push_back(Node(Position(400, 75))); //Top Middle
	nodeVector.push_back(Node(Position(400, 525))); //Bottom Middle
	nodeVector.push_back(Node(Position(700, 300))); //Right Middle
	nodeVector.push_back(Node(Position(100, 300))); //Left Middle
}

RobotTank::~RobotTank() {} // Destructor

void RobotTank::reset()
{
	//if tank dies, this is called
}

void RobotTank::move()
{
	if (stop) //if target or enemy has been spotted
	{
		shoot(); //shoot it
		return;
	}

	//Check all nodes
	for (size_t i = 0; i < nodeVector.size(); i++)
	{
		//Check to see if this node has been visited
		if (!nodeVector[i].visited)
		{
			//If the tank is there
			if (calculatePath(nodeVector[i].position))
			{
				//set visited to try
				nodeVector[i].visited = true;
			}
			std::cout << "Visiting node " << i << ".\n";
			break;
		}

		if (i == nodeVector.size() - 1)
		{
			std::cout << "Resetting all nodes " << i << ".\n";
			
			//Reset all nodes.
			for (size_t k = 0; k < nodeVector.size(); k++)
			{
				nodeVector[k].visited = false;
			}
		}
	}
}

bool RobotTank::calculatePath(Position &p)
{
	clearMovement(); //Stop moving turret
	stopTurret(); //Stop moving barrel

	float pathAngle = calculateAngle(p); //Get angle towards path and round
	float tankAngle = round(pos.getTh()); //Get current tank angle and round

	float roundX = round(pos.getX()); //Get rounded x coord
	float roundY = round(pos.getY()); //Get rounded y coord

	if (withinRange(p) || collide) //Check to see if the tank is within range of the position
	{
		clearMovement(); //Stop moving turret
		stopTurret(); //Stop moving barrel
		collide = false;
		return true;
	}
	else if (tankAngle == pathAngle || tankAngle == pathAngle + 1 || tankAngle == pathAngle - 1)
	{
		goForward();
		return false;
	}
	else if (tankAngle > pathAngle)
	{
		goLeft();
	}
	else
	{
		goRight();
	}
	return false;

}

void RobotTank::shoot()
{
	clearMovement(); //Stop moving turret
	stopTurret(); //Stop moving barrel

	turretTh = round(turretTh); //Round the barrel rotation.

	if (turretTh == barrelAngle)
	{
		fire = true;
	}
	else if (turretTh > barrelAngle)
	{
		turretGoLeft();
	}
	else
	{
		turretGoRight();
	}
}

void RobotTank::collided()
{
	clearMovement();
	stopTurret();
	collide = true;
}

float RobotTank::calculateAngle(Position &p)
{
	float dx = p.getX() - pos.getX(); //Calculate x difference
	float dy = p.getY() - pos.getY(); //Calculate y difference
	float angle = RAD2DEG(atan2(dy, dx)); //Calculate atan 2 and convert radian to degree

	if (angle < 0)
	{
		angle += 360;
	}

	return round(angle);
}

bool RobotTank::withinRange(Position &p)
{
	float tankX = round(pos.getX());
	float tankY = round(pos.getY());

	float otherX = round(p.getX());
	float otherY = round(p.getY());

	//Check if tank is within 1 range of the position
	if ((tankX == otherX && tankY == otherY)
		|| (tankX == otherX + 1 && tankY == otherY + 1)
		|| (tankX == otherX - 1 && tankY == otherY - 1)
		|| (tankX == otherX + 1 && tankY == otherY)
		|| (tankX == otherX - 1 && tankY == otherY)
		|| (tankX == otherX && tankY == otherY + 1)
		|| (tankX == otherX && tankY == otherY - 1))
	{
		return true;
	}

	return false;
}

void RobotTank::markTarget(Position &p) //Detected a blue building
{
	std::cout << "Blue base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
	calculatePath(p); //Calculate path towards blue building.
	barrelAngle = calculateAngle(p);
	stop = true;
}

void RobotTank::markEnemy(Position &p) //Detected player tank
{
	std::cout << "Enemy tank spotted at (" << p.getX() << ", " << p.getY() << ")\n";
	calculatePath(p);
	barrelAngle = calculateAngle(p);
	stop = true;
}

void RobotTank::markBase(Position &p) //Detected a red building (friendly building)
{
	std::cout << "Red base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void RobotTank::markShell(Position &p) //Detected enemy bullet (Doesn't detect own bullets)
{
	//std::cout << "Enemy shell spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

bool RobotTank::isFiring()
{
	if (fire)
	{
		stop = false;
		fire = false;
		return true;
	}

	return fire; // Fire when going backwards
}

void RobotTank::score(int thisScore, int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}