#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "human.h"
#include "computer.h"
#include "grid.h"

#include "window-v2.h"

//test
int main(int argc, char* argv[])
{
	Xwindow* w = NULL;
	if (argc >= 2) {
		w = new Xwindow(400,400);
	}
	//Score
	double whiteScore = 0;
	double blackScore = 0;

	//Game status (If game is currently been initiated or not)
	bool game = false;
	bool gameOnce = false; //If game has run once

	//If white player or black player is human or not
	bool whiteHuman = false;
	bool blackHuman = false;

	//Set up initial board
	Grid *gameBoard = new Grid();
	Player* p1w = NULL;
	Player* p2b = NULL;

	while(!std::cin.eof())
	{
		std::string line;
		while(std::getline(std::cin, line))
		{
			std::stringstream piece(line);
			std::string token;
			while(piece >> token)
			{
				//For testing
				if(token == "p") {
					if (argc >= 2)
						gameBoard->printChess(*w);
					else
						std::cout << *(gameBoard) << std::endl;
				}
				//Start new game
				if(token == "game")
				{
					//Initialize board state with default or pre-setup
					gameBoard->startGame();

					//Make white-player human or computer[1234]
					piece >> token;
					if(token == "human")
					{
					    p1w = new Human('w');
						whiteHuman = true;
					}
					else
					{
						int level = token[8] - '0';
						p1w = new Computer(level, 'w');
						whiteHuman = false;
					}

					//Make black-player human or computer[1234]
					piece >> token;
					if(token == "human")
					{
						p2b = new Human('b');
						blackHuman = true;
					}
					else
					{
						int level = token[8] - '0';
						p2b = new Computer(level, 'b');
						blackHuman = false;
					}
					game = true;
					gameOnce = true;
				    if (argc >= 2)
                        gameBoard->printChess(*w);
                    else
                        std::cout << *(gameBoard) << std::endl;
                    std::string gameAction; //Loop over game commands
					while(std::getline(std::cin, gameAction))
					{
						std::stringstream comm(gameAction);
						std::string gameComm;
						while(comm >> gameComm)
						{
							if(gameComm == "p") std::cout << *(gameBoard) << std::endl;
							if(gameComm == "resign") 	//Resign
							{
								//Add 1 to the score for the non-resigning player and end the game
								if(gameBoard->getTurn() == 'w')
								{
									//p2b->addScore(1);
									blackScore++;
									std::cout << "Black wins!" << std::endl;
									//blackScore = p2b->getScore();
								}
								else //gameBoard->isTurn() == 'b'
								{
									//p1w->addScore(1);
									std::cout << "White wins!" << std::endl;
									whiteScore++;
									//whiteScore = p1w->getScore();
								}
								if (p1w != NULL) {
                                    delete p1w;
                                    p1w = NULL;
								}
								if (p2b != NULL) {
                                    delete p2b;
                                    p2b = NULL;
								}
								game = false;
								gameBoard->resetBoard();
								break;
							}
							else if(gameComm == "move") //Move
							{
								//Current player == Human
								//std::cerr << "White Human = " <<whiteHuman << "The turn is " << gameBoard->getTurn() << std::endl;
								//std::cerr << "Black Human = " <<blackHuman << std::endl;
								if((gameBoard->getTurn() == 'w' && whiteHuman) || (gameBoard->getTurn() == 'b' && blackHuman))
								{
									//std::cerr << "Human move attempt" << std::endl;
									//Set to and from
									comm >> gameComm;
									std::string from = gameComm;
									comm >> gameComm;
									std::string to = gameComm;

									//Execute move for the current turn's player
									if(gameBoard->movePiece(from, to))
									{
										//Check for pawn promotion
										if(gameBoard->pawnCheck())
										{
											char promotionPiece = 0;
											comm >> promotionPiece;
											gameBoard->promotePawn(promotionPiece);
										}

										//swap turn
										if(gameBoard->getTurn() == 'w') gameBoard->setTurn('b');
										else gameBoard->setTurn('w');

										if (argc >= 2) {
                                            gameBoard->updateBoard(*w, from, to);
                                            if (gameBoard->getJustCastled()) {
                                                gameBoard->updateBoard(*w, 1);
                                                gameBoard->setJustCastled(false);
                                            } else if (gameBoard->getJustEnPassant()) {
                                                gameBoard->updateBoard(*w, 2);
                                                gameBoard->setJustEnPassant(false);
                                            }
										}
                                        else
                        					std::cout << *(gameBoard) << std::endl;

										std::cout << "Successful move" <<std::endl;
									}
									else std::cout << "Invalid move." << std::endl;
								}
								else // Current player == Computer
								{
									//std::cerr << "CPU move initiated" << std::endl;
									int whoseTurn;
									//Execute computer generated move
									if(gameBoard->getTurn() == 'w') {
                                        static_cast<Computer*>(p1w)->generateMove(gameBoard);
                                        whoseTurn = 1;
									}
									else if(gameBoard->getTurn() == 'b') {
                                        static_cast<Computer*>(p2b)->generateMove(gameBoard);
                                        whoseTurn = 2;
									}
									//std::cerr << "CPU move ended" << std::endl;

									//swap turn
									if(gameBoard->getTurn() == 'w') gameBoard->setTurn('b');
									else gameBoard->setTurn('w');

									 if (argc >= 2) {
                                        Computer* newComp;
                                        if (whoseTurn == 1) {
                                            newComp = static_cast<Computer*>(p1w);
                                        } else if (whoseTurn == 2) {
                                            newComp = static_cast<Computer*>(p2b);
                                        }
                                        gameBoard->updateBoard(*w, newComp->getLastFrom(), newComp->getLastTo());
                                    }
                                    else
                                        std::cout << *(gameBoard) << std::endl;

									//std::cerr << "Successful move" <<std::endl;
								}

								//Check gameStatus
								if(gameBoard->isInCheck(gameBoard->getTurn()))
								{
									//std::cerr << "Check" << std::endl;
									if(gameBoard->isInCheckmate(gameBoard->getTurn()))
									{
										if(gameBoard->getTurn() == 'b') { std::cout << "Checkmate! White wins!" << std::endl; whiteScore++; }
										else { std::cout << "Checkmate! Black wins!" << std::endl; blackScore++; }
										if (p1w != NULL) {
                                            delete p1w;
                                            p1w = NULL;
                                        }
                                        if (p2b != NULL) {
                                            delete p2b;
                                            p2b = NULL;
                                        }
										game = false;
									}
									else
									{
										if(gameBoard->getTurn() == 'b') std::cout << "Black is in check" << std::endl;
										else std::cout << "White is in check" << std::endl;
									}
								}
								else if(gameBoard->isStalemate(gameBoard->getTurn()))
								{
									std::cout << "Stalemate!" << std::endl;
									if (p1w != NULL) {
                                        delete p1w;
                                        p1w = NULL;
                                    }
                                    if (p2b != NULL) {
                                        delete p2b;
                                        p2b = NULL;
                                    }
									game = false;
									whiteScore += 0.5;
									blackScore += 0.5;
								}
							}
						}
						if(!game) break; //Exit loop if game ended
					}
				}
				else if(token == "setup")
				{
				    gameBoard->setupBoard(true);
				    std::cout << "Setup Mode initiated" << std::endl;
				    if (argc >= 2)
                        gameBoard->printChess(*w);
				    bool done = false;
				    bool doneTry = false;
				    std::string setupLine;
					while(std::getline(std::cin, setupLine))
					{
						std::stringstream setup(setupLine);
						std::string setupAction;
						while(setup >> setupAction)
						{
							std::string command = setupAction;
							if(command == "done")
							{
								doneTry = true;
								switch(gameBoard->validSetup())
								{
									case '1':
										done = true;
										break;
									case 'c':
										std::cout << "Please fix invalid setup. King cannot be in check." << std::endl;
										break;
									case 'k':
										std::cout << "Please fix invalid setup. There must be exactly one King for each player." << std::endl;
										break;
									case 'p':
										std::cout << "Please fix invalid setup. There cannot be any pawns on the ends of the board." << std::endl;
										break;
								}
								break;
							}
							else //Setup commands
							{
								char action = command[0];
								char gamePiece;
								std::string location;
								switch(action)
								{
									case '+':
										setup >> gamePiece;
										setup >> location;

										//Add piece to board. Note: this replaces any piece that may be in this position
										gameBoard->addPiece(gamePiece, location);
										if (argc >= 2)
                                            gameBoard->updateBoard(*w, gamePiece, location);
										break;
									case '-':
										setup >> setupAction;
										location = setupAction;

										//Remove piece
										gameBoard->removePiece(location);
										if (argc >= 2)
                                            gameBoard->updateBoard(*w, location);
										break;
									case '=':
										//Make it colour's turn to go next, ie. = colour
										setup >> setupAction;
										char newColour;
										if(setupAction == "white")
										{
											newColour = 'w';
										}
										else // if(setupAction == "black")
										{
											newColour = 'b';
										}

										if (argc >= 2)
                                            gameBoard->updateBoard(*w, newColour);

										//Set turn
										gameBoard->setTurn(newColour);

										break;
									default:
										std::cout << "Invalid command" << std::endl;
								}

							}
						}
						if(done) break; //Break the loop if done was invoked
						if(!doneTry) //Only display if you didnt attempt to exit/done
						{
							 if (argc < 2)
	        	                std::cout << *(gameBoard) << std::endl;
						}
						doneTry = false;
					}
					std::cout << "Setup mode exited" << std::endl;
				}
			}
		}
	}
	//End of game, [ctrl-D] --> Display Final score of Black and White
	std::cout << "Final Score: " << std::endl;
	std::cout << "White: " << whiteScore << std::endl;
	std::cout << "Black: " << blackScore << std::endl;
	if (w != NULL) delete w;
	delete gameBoard;
	if (p1w != NULL) delete p1w;
	if (p2b != NULL) delete p2b;
}
