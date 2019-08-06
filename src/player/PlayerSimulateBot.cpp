#include "../../include/player/PlayerSimulateBot.h"
#include "../../include/game/MapOptionManager.hpp"

#include <cstdlib>
#include "../../include/util/Settings.hpp"
#include "../../include/game/ExceptionOutOfTime.hpp"

int PlayerSimulateBot::workcounter = 0; 

#define DEBUG_BEWERTUNGEN_TOP 1 

PlayerSimulateBot::PlayerSimulateBot(intSpielerId spielerId) : PlayerInterface(spielerId), tiefe(0), nextSpielerQueue(nullptr), game0(nullptr) {
	std::srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	if (Debug::getFlag()) {
		if (spielerId < 1 || spielerId > 8) {
			std::ostringstream fehlertext; fehlertext << "Fehler beim Erstellen des Spielers: spielerId = " << spielerId;
			Debug::printErrorLine(fehlertext.str()); 
		}
	}
}

void PlayerSimulateBot::setTiefe(int tiefe) {
	if (this->tiefe < tiefe) {
		delete[] this->nextSpielerQueue;
		this->nextSpielerQueue = nullptr;
	}
	this->tiefe = tiefe;
}


void PlayerSimulateBot::buildAst(std::vector<std::shared_ptr<MapOption> > & astVector, std::vector<std::shared_ptr<MapResult> > & astMaxResult, std::vector<intPts> & punkteVector, std::vector<intPts> & pruningVector, int & tiefe) {
	intZeitPunkt jetzt = std::chrono::steady_clock::now(), bis;
	intZeitNanoSek dauer = intZeitNanoSek(987654); 
	std::shared_ptr<MapOption> knoten = astVector[tiefe]; 
	std::shared_ptr<MapOption> knotenNeu = nullptr; 
	const int anzahlSpieler = this->game0->getGrid()->COUNT_PLAYERS;

	++workcounter;
	if (knoten == nullptr) {
		return; 
	}
	if (jetzt >= this->zeitLimit) {
		throw ExceptionOutOfTime();
	}
	for (++tiefe;  tiefe <= this->tiefe && (dauer > intZeitNanoSek(300000) || !this->hasZeitLimit); ++tiefe) {
		intSpielerId playerId = this->nextSpielerQueue[tiefe]; 
		bis = knoten->zeitLimit; 
		dauer = bis - jetzt; 
		/*if (tiefe > 3 && tiefe < 7) {
			std::cout << tiefe;
			std::cout << "Dauer = " << dauer.count() << " in Tiefe " << tiefe << std::endl;
		}*/
		knotenNeu = knoten->map->doEveryThing(playerId);
		if (knotenNeu != nullptr) {
			MapOptionManager * manager = MapOptionManager::getInstance(); 
			manager->sortOptionen(knotenNeu, dauer, jetzt);
			knoten = knotenNeu; 
			astVector[tiefe] = knoten; 
			//TODO tote Aeste??? astVector[tief] = astVector[tief - 1];
		} else {
			astVector[tiefe] = nullptr; 
		}
		punkteVector[tiefe] = INT_MIN;
		for (intSpielerId spielerId = anzahlSpieler - 1, pruningIndexOben = tiefe * anzahlSpieler, pruningIndexUnten = (tiefe - 1) * anzahlSpieler; spielerId >= 0; --spielerId) {
			pruningVector[spielerId + pruningIndexUnten] = pruningVector[spielerId + pruningIndexOben]; 
		}
		astMaxResult[tiefe] = nullptr; 
	}
	this->processBlatt(astVector, astMaxResult, tiefe); 
}

/** reisst dem Baum die Blaetter aus. 
 * arbeitet die tiefste Schicht des Baumes ab (also die Blaetter) */
inline void PlayerSimulateBot::processBlatt(std::vector<std::shared_ptr<MapOption> > & astVector, std::vector<std::shared_ptr<MapResult> > & astMaxResult, int & tiefe) {
	if (this->erreichteTiefe < tiefe) {
		this->erreichteTiefe = tiefe; 
	}
	std::shared_ptr<MapOption> ast = astVector[tiefe]; 
	if (ast == nullptr) {
		//toter Ast
		for (--tiefe; tiefe >= 0 && ast == nullptr; --tiefe) {
			ast = astVector[tiefe]; 
		}
		//error: Baum ist ab. 
		if (tiefe <= 0) {
			tiefe = -1; 

			if (astMaxResult[0] == nullptr && astVector[0] != nullptr) {
				astMaxResult[0] = astMaxResult[1] = astVector[0]->map->getMapResult(); 
			}

			return; 
		}
	}

	std::shared_ptr<MapResult> currentResult = nullptr; 
	std::shared_ptr<MapResult> bestResult = nullptr; 
	intPts currentPunkte, bestPunkte = INT_MIN; 

		
	for (; ast != nullptr; ast = ast->next) {
		currentResult = ast->map->getMapResult(); 
		currentPunkte = currentResult->getScoreForPlayer(this->nextSpielerQueue[tiefe]); 
		if (currentPunkte > bestPunkte) {
			bestPunkte = currentPunkte; 
			bestResult = currentResult; 
		}
	} //next ast
	astMaxResult[tiefe] = bestResult; 
	astVector[tiefe] = nullptr; 
}

intPts PlayerSimulateBot::bewerteOption(std::shared_ptr<MapOption> option) {
	int aktTiefe = 0; 
	intPts score; 
	int sizeLimit = this->tiefe + option->map->getGrid()->COUNT_PLAYERS; 
	intSpielerId spielerId;
	MapGrid * grid = option->map->getGrid(); 

	/** astVector ist der Ast im Baum, den wir gerade auswerten */
	std::vector<std::shared_ptr<MapOption> > astVector(sizeLimit, nullptr); 
	
	/** astMaxPunkte ist das MapResult */
	std::vector<std::shared_ptr<MapResult> > astMaxResult(sizeLimit, nullptr); 
	std::vector<intPts> punkteVector(sizeLimit, 0); 
	
	/** pruningVector - index: tiefe x #spielerAscII + spielerid */
	std::vector<intPts> pruningVector(sizeLimit * grid->COUNT_PLAYERS, INT_MIN); 
	char pruningVectorIndex;

	astVector[0] = option; 
	punkteVector[0] = INT_MIN; 

	this->buildAst(astVector, astMaxResult, punkteVector, pruningVector, aktTiefe);
	while (aktTiefe > 0) {
		if (astVector[aktTiefe] == nullptr) {
			//von aktTiefe auf aktTiefe -1 hochschicken
			spielerId = this->nextSpielerQueue[aktTiefe - 1];
			score = astMaxResult[aktTiefe]->getScoreForPlayer(spielerId); 
			if (score > punkteVector[aktTiefe - 1]) {
				punkteVector[aktTiefe - 1] = score; 
				astMaxResult[aktTiefe - 1] = astMaxResult[aktTiefe];

				//pruning
				pruningVectorIndex = (aktTiefe - 1) * grid->COUNT_PLAYERS + (spielerId - 1); 
				if (pruningVector[pruningVectorIndex] < score) {
					pruningVector[pruningVectorIndex] = score; 
				}
				pruningVectorIndex = (aktTiefe - 1) * grid->COUNT_PLAYERS; 
				for (spielerId = grid->COUNT_PLAYERS; spielerId > 0; --spielerId) {
					if (astMaxResult[aktTiefe]->getScoreForPlayer(spielerId) < pruningVector[pruningVectorIndex + spielerId - 1]) {
						//cut off
						//astVector[aktTiefe - 1] = nullptr;
					}
				}


			}
			aktTiefe--; 
		} else {
			astVector[aktTiefe] = astVector[aktTiefe]->next; 
			this->buildAst(astVector, astMaxResult, punkteVector, pruningVector, aktTiefe); 
		}
	}


	return punkteVector[0];
}

inline void PlayerSimulateBot::initialiseCalculation(std::shared_ptr<MapGame> mapGame, std::shared_ptr<MapOption> & optionen, uint32_t & zeitMillisekunden, uint8_t & tiefe) {
	if(tiefe < 2){
		tiefe = 30; 
	}
	this->setTiefe(30);
	this->erreichteTiefe = 0; 
	if (optionen == nullptr) {
		optionen = mapGame->doEveryThing(this->spielerId); 
		//TODO 
	}
	this->game0 = mapGame; 
	this->hasZeitLimit = zeitMillisekunden > 0; 
	//Reservezeit
	if (zeitMillisekunden > 2000) {
		zeitMillisekunden -= 1000; 
	} else {
		zeitMillisekunden /= 2; 
	}

	uint64_t zeitNanosekunden = static_cast<uint64_t>(zeitMillisekunden) * 1000000; 
	std::stringstream ss;
	ss << "Workcounter steht bei " << workcounter; 
	Debug::printLine(ss.str());
	MapOptionManager * managerOption = MapOptionManager::getInstance();
	managerOption->sortOptionen(optionen, intZeitNanoSek(zeitNanosekunden), std::chrono::steady_clock::now()); 
	intZeitPunkt jetzt = std::chrono::steady_clock::now(); 
	jetzt += intZeitNanoSek(zeitNanosekunden);
	this->zeitLimit = jetzt;
	if (this->nextSpielerQueue == nullptr) {
		intSpielerId spielerId = this->spielerId; ;
		MapGrid * grid = optionen->map->getGrid(); 
		this->nextSpielerQueue = new char[this->tiefe + grid->COUNT_PLAYERS + 1]; 
		for (int i = 0; i < this->tiefe + grid->COUNT_PLAYERS; i++) {
			this->nextSpielerQueue[i] = spielerId; 
			spielerId = grid->getNextPlayer(spielerId); 
		}
		this->nextSpielerQueue[this->tiefe + grid->COUNT_PLAYERS] = '\0'; 
	}
}

void PlayerSimulateBot::executeIterateDeepening(intCellUId & feldUIndex, char & specialParam, std::shared_ptr<MapGame> mapGame, uint32_t zeitMillisekunden, uint8_t tiefe) {
	intZeitPunkt start = std::chrono::steady_clock::now(), estimatedEndZeitpunkt;
	intZeitNanoSek zeitVorvorvorher(1), zeitVorvorher(1), zeitVorher(1); 
	intZeitFaktor zeitFaktor1, zeitFaktor2, zeitFaktorMax, zeitFaktorMin; 
	std::shared_ptr<MapOption> optionen = nullptr; 
	std::shared_ptr<MapOption> result = nullptr;
	std::shared_ptr<MapOption> resultIteration = nullptr;
	intPts maxScore = INT_MIN, score;
	this->initialiseCalculation(mapGame, optionen, zeitMillisekunden, tiefe);
	if (optionen != nullptr) {
		feldUIndex = optionen->feldUIndex;
	}
	this->hasZeitLimit = false;
	for (int iterate_tiefe = 2; iterate_tiefe < tiefe; ++iterate_tiefe) {
		//lohnt es sich noch?
		zeitVorvorvorher = zeitVorvorher; 
		zeitVorvorher = zeitVorher;
		zeitVorher = std::chrono::steady_clock::now() - start;
		if (iterate_tiefe >= 4) {
			zeitFaktor1 = zeitVorher / zeitVorvorher; 
			zeitFaktor2 = zeitVorvorher / zeitVorvorvorher;
			if (zeitFaktor1 < zeitFaktor2) {
				zeitFaktorMin = zeitFaktor1; 
				zeitFaktorMax = zeitFaktor2; 
			} else {
				zeitFaktorMin = zeitFaktor2;
				zeitFaktorMax = zeitFaktor1; 
			}
			zeitFaktor1 = (zeitFaktorMax * 11 - zeitFaktorMin * 2) / 8; 
			estimatedEndZeitpunkt = start + (zeitFaktor1 * zeitVorher);
			if (estimatedEndZeitpunkt >= this->zeitLimit) {
				if (DEBUG_BEWERTUNGEN_TOP) {
					std::ostringstream text;
					text << "Tiefe " << iterate_tiefe << " wird nicht mehr versucht"; 
					Debug::printLine(text.str());
				}
				break;
			}
		}



		try {
			this->tiefe = iterate_tiefe;
			maxScore = INT_MIN; 
			resultIteration = nullptr;
			optionen = mapGame->doEveryThing(this->spielerId);
			while (optionen != nullptr) {
				score = this->bewerteOption(optionen);
				if (DEBUG_BEWERTUNGEN_TOP) {
					short x, y;
					mapGame->getGrid()->getXY(optionen->feldUIndex, x, y);
					std::cout << "score of u" << optionen->feldUIndex << " (" << x << ", " << y << ") [T: " << this->erreichteTiefe << "] = " << score << std::endl;
					this->erreichteTiefe = 0;
				}

				if (maxScore < score || resultIteration == nullptr) {
					maxScore = score;
					resultIteration = optionen;
				}
				optionen = optionen->next;
			}
			if (resultIteration != nullptr) {
				result = resultIteration;
				if (DEBUG_BEWERTUNGEN_TOP) {
					short x, y;
					std::ostringstream text;
					mapGame->getGrid()->getXY(result->feldUIndex, x, y);
					text << "Tiefe " << iterate_tiefe << " Feldvorschlag u" << result->feldUIndex << " (" << x << ", " << y << ") ";
					Debug::printLine(text.str());
				}
			}
			else if (DEBUG_BEWERTUNGEN_TOP) {
				std::ostringstream text;
				text << "Tiefe " << iterate_tiefe << " kein Feldvorschlag";
				Debug::printLine(text.str());
			}
		} catch (ExceptionOutOfTime err) {
			if (DEBUG_BEWERTUNGEN_TOP) {
				std::ostringstream text;
				text << "Tiefe " << iterate_tiefe << " TIME-OUT-EXCEPTION";
				Debug::printLine(text.str());
			}
			break; 
		}
	}
	
	if (result == nullptr) {
		result = mapGame->doEveryThing(this->spielerId); 
		if (result == nullptr) {
			Debug::printErrorLine("kein Feld gefunden! Waehle stattdessen Ersatzfeld. Grund: Keine Optionen vorhanden. Eigentlich haetten wir keine Zugaufforderung bekommen duerfen. ");
		} else {
			Debug::printErrorLine("kein Feld gefunden! Waehle stattdessen Ersatzfeld erste Option. "); 
		}
	}

	if (result != nullptr) {
		feldUIndex = result->feldUIndex;
		specialParam = result->special;
	}

	if (this->nextSpielerQueue != nullptr) {
		delete[] this->nextSpielerQueue;
		this->nextSpielerQueue = nullptr;
	}

}

void PlayerSimulateBot::execute(intCellUId & feldUIndex, char & specialParam, std::shared_ptr<MapGame> mapGame, std::shared_ptr<MapOption> optionen, uint32_t zeitMillisekunden, uint8_t tiefe) {
	this->executeIterateDeepening(feldUIndex, specialParam, mapGame, zeitMillisekunden, tiefe); return; 

	this->initialiseCalculation(mapGame, optionen, zeitMillisekunden, tiefe);
	//TODO out of time
	std::shared_ptr<MapOption> result = nullptr; 
	intPts maxScore = INT_MIN, score; 

	while (optionen != nullptr) {
		score = this->bewerteOption(optionen); 

		if (DEBUG_BEWERTUNGEN_TOP) {
			short x, y; 
			mapGame->getGrid()->getXY(optionen->feldUIndex, x, y);
			std::stringstream ss;
			ss << "score of u" << optionen->feldUIndex << " (" << x << ", " << y << ") [T: " << this->erreichteTiefe << "] = " << score << std::endl;
			this->erreichteTiefe = 0; 
			Debug::print(ss.str());
		}

		if (maxScore < score || result == nullptr) {
			maxScore = score; 
			result = optionen; 
		}
		optionen = optionen->next; 
	}

	if (result != nullptr) {
		feldUIndex = result->feldUIndex; 
		specialParam = result->special;
	}

	if (this->nextSpielerQueue != nullptr) {
		delete[] this->nextSpielerQueue; 
		this->nextSpielerQueue = nullptr; 
	}
}

intPts PlayerSimulateBot::bewerteBombenOption(std::shared_ptr<MapOption> option) {
	//zunaechst billig: eigene-feindliche
	intPts result = 0;
	charCell feld; 

	for (intCellUId feldUIndex = option->map->getGrid()->COUNT_U_FELDER - 1; feldUIndex >= 0; --feldUIndex) {
		feld = option->map->getUFeldValue(feldUIndex); 
		if (feld == this->spielerId) {
			result++; 
		} else if (feld >= 1 && feld <= 9) {
			result--;
		}
	}
	return result; 
}

void PlayerSimulateBot::executeBomb(intCellUId & feldUIndex, std::shared_ptr<MapGame> mapGame, uint32_t zeitMillisekunden) {
	std::shared_ptr<MapOption> optionen = mapGame->doEveryBomb(this->spielerId); 
	std::shared_ptr<MapOption> result = nullptr; 
	intPts maxScore = INT_MIN, score; 

	while (optionen != nullptr) {
		score = this->bewerteBombenOption(optionen); 
		if (maxScore < score || result == nullptr) {
			maxScore = score; 
			result = optionen; 
		}
		optionen = optionen->next; 
	}

	if (result != nullptr) {
		feldUIndex = result->feldUIndex; 
	}

	this->game0 = mapGame;
	while (1) {
		short result = rand() % mapGame->getGrid()->COUNT_U_FELDER;
		if (mapGame->getUFeldValue(result) != '-') {
			feldUIndex = result;
			return;
		}
	}
}