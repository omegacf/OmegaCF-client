#ifndef __REVERSI_PLAYER_BOT_SIMULATE_H__
#define __REVERSI_PLAYER_BOT_SIMULATE_H__

#include <memory> 
#include <vector>
#include <climits>
#include <sstream>

struct MapOption; 
struct MapResult; 
class MapGame;

#include "../game/MapOption.hpp"
#include "../game/MapResult.hpp"
#include "../game/MapGame.hpp"
#include "../util/Settings.hpp"
#include "PlayerInterface.h"
#include "../util/Debug.hpp"

class PlayerSimulateBot : public PlayerInterface {
private: 
	static int workcounter; 
	int tiefe; 
	int erreichteTiefe; 
	intSpielerId * nextSpielerQueue; 
	std::shared_ptr<MapGame> game0;
	bool hasZeitLimit; 
	intZeitPunkt zeitLimit; 

	intPts bewerteOption(std::shared_ptr<MapOption> option); 
	intPts bewerteBombenOption(std::shared_ptr<MapOption> option); 

	void buildAst(std::vector<std::shared_ptr<MapOption> > & astVector, std::vector<std::shared_ptr<MapResult> > & astMaxResult,  std::vector<intPts> & punkteVector, std::vector<intPts> & pruningVector, int & tiefe);
	inline void processBlatt(std::vector<std::shared_ptr<MapOption> > & astVector, std::vector<std::shared_ptr<MapResult> > & astMaxResult, int & tiefe);
	inline void initialiseCalculation(std::shared_ptr<MapGame> mapGame, std::shared_ptr<MapOption> & optionen, uint32_t & zeitMillisekunden, uint8_t & tiefe); 
	// abnahme-only 
	void executeIterateDeepening(intCellUId & feldUIndex, char & specialParam, std::shared_ptr<MapGame> mapGame, uint32_t zeitMillisekunden, uint8_t tiefe); 

public:
	PlayerSimulateBot(intSpielerId spielerAscII);
	void setTiefe(int tiefe); 

	virtual void execute(intCellUId & feldUIndex, char & specialParam, std::shared_ptr<MapGame> mapGame, std::shared_ptr<MapOption> optionen, uint32_t zeitMillisekunden, uint8_t tiefe);
	virtual void executeBomb(intCellUId & feldUIndex, std::shared_ptr<MapGame> mapGame, uint32_t zeitMillisekunden);
};

#endif // !__REVERSI_PLAYER_BOT_SIMULATE_H__
