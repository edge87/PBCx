#include "PlayerMonitor.hpp"
#include "LogController.hpp"
#include "VideoController.hpp"
#include <sstream>

using namespace std;

PlayerMonitor* PlayerMonitor::pinstance = NULL;

PlayerMonitor* PlayerMonitor::instance()
{
	if(pinstance == NULL)
	{
		pinstance = new PlayerMonitor;
	}
	return pinstance;
}

PlayerMonitor::PlayerMonitor() : currentPlayer(NULL)
{
}

PlayerMonitor::~PlayerMonitor()
{
}

void PlayerMonitor::resetGame(int numberOfPlayers, int numberOfBalls)
{
	for(vector<Player*>::iterator iter = players.begin(); iter != players.end(); iter++)
	{
		delete (*iter);
	}
	players.clear();
	for(int i = 0; i < numberOfPlayers; i++)
	{
		players.push_back(new Player(numberOfBalls));
		VideoController::EnablePlayerScore(i + 1);
	}
	currentPlayer = players.begin();
	
}

bool PlayerMonitor::changePlayer()
{
	cout << "change player called" << endl;
	currentPlayer++;
	if(currentPlayer == players.end())
	{
		currentPlayer = players.begin();
	}
}

void PlayerMonitor::incrementScore(int score)
{
	(*currentPlayer)->incrementScore(score);
	stringstream ss;
	ss << (*currentPlayer)->getScore();
	cout << "Player " << (currentPlayer - players.begin()) + 1 << ": " << (*currentPlayer)->getScore() << endl;
	VideoController::UpdateScore((currentPlayer - players.begin())+1, ss.str());
}
