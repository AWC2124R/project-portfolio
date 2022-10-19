#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

char map[4][3] = {
	{'j', 'k', 's'},
	{' ', 'p', ' '},
	{' ', 'P', ' '},
	{'S', 'K', 'J'}
};

std::vector<char> myPieces = {'P', 'S', 'K', 'J'};
std::vector<char> oppPieces = {'p', 's', 'k', 'j'};

std::vector<char> myPrisonerPieces;
std::vector<char> oppPrisonerPieces;

void print_map() {
	std::cout << "  ";
	for (int x = 0; x < 3; x++) {
		std::cout << x <<" ";
	}
	std::cout << std::endl;

	for (int y = 0; y < 4; y++) {
		std::cout << y << " ";
		for (int x = 0; x < 3; x++) {
			std::cout << map[y][x] << " ";
		}
		std::cout << std::endl;
	}

    std::cout << std::endl;

    std::cout << "My Pieces: ";
	for (int i = 0; i < myPieces.size(); i++) {
		std::cout << myPieces[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "Opponent Pieces: ";
	for (int i = 0; i < oppPieces.size(); i++) {
		std::cout << oppPieces[i] << " ";
	}
	std::cout << std::endl;

    std::cout << std::endl;

	std::cout << "My Prisoner Pieces: ";
	for (int i = 0; i < myPrisonerPieces.size(); i++) {
		std::cout << myPrisonerPieces[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "Opponent Prisoner Pieces: ";
	for (int i = 0; i < oppPrisonerPieces.size(); i++) {
		std::cout << oppPrisonerPieces[i] << " ";
	}
	std::cout << std::endl;

	std::cout << std::endl;
}

bool check_king_me() {
	bool my_has_king = false;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 3; x++) {
			if (map[y][x] == 'K') {
				my_has_king = true;
			}
		}
	}
	return my_has_king;
}

bool check_king_opp() {
	bool opp_has_king = false;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 3; x++) {
			if (map[y][x] == 'k') {
				opp_has_king = true;
			}
		}
	}
	return opp_has_king;
}

int main() {
	int turn_count = 0;
	bool player_turn = true;
	print_map();
	while (true) {
		if (player_turn == true) { // Player's Turn
			std::string input;

			std::cout << "Input your move. Ex) !P14, J23" << std::endl;
			std::cin >> input;

			if(input[0] != '!'){ // Move Piece
                char moveChar = input[0];
                int targetX = input[1] - '0';
                int targetY = input[2] - '0';

                bool isMyPiece = false;
                for(int i = 0; i < myPieces.size(); i++){
                    if(myPieces[i] == moveChar){
                        isMyPiece = true;
                        break;
                    }
                }

                if(isMyPiece == true){ // If Target Piece Exists
                    if(0 > targetX || 2 < targetX || 0 > targetY || 3 < targetY){
                        std::cout << "Target Position Out of Range." << std::endl;
                        continue;
                    }

                    int currentX = 0;
                    int currentY = 0;

                    for(int i = 0; i < 4; i++){
                        for(int j = 0; j < 3; j++){
                            if(map[i][j] == moveChar){
                                currentX = j;
                                currentY = i;
                                break;
                            }
                        }
                    }

                        bool canMove = false;
                        if(moveChar == 'P' || moveChar == 'p'){
                            std::vector<std::vector<int>> possibleMoves = {{currentY - 1, currentX}};
                            for(int i = 0; i < possibleMoves.size(); i++){
                                if(possibleMoves[i][0] == targetY && possibleMoves[i][1] == targetX){
                                    canMove = true;
                                }
                            }
                        }
                        else if(moveChar == 'J' || moveChar == 'j'){
                            std::vector<std::vector<int>> possibleMoves = {{currentY - 1, currentX},
                                                                           {currentY + 1, currentX},
                                                                           {currentY, currentX + 1},
                                                                           {currentY, currentX - 1}};

                            for(int i = 0; i < possibleMoves.size(); i++){
                                if(possibleMoves[i][0] == targetY && possibleMoves[i][1] == targetX){
                                    canMove = true;
                                }
                            }
                        }
                        else if(moveChar == 'S' || moveChar == 's'){
                            std::vector<std::vector<int>> possibleMoves = {{currentY - 1, currentX - 1},
                                                                           {currentY + 1, currentX + 1},
                                                                           {currentY - 1, currentX + 1},
                                                                           {currentY + 1, currentX - 1}};
                            for(int i = 0; i < possibleMoves.size(); i++){
                                if(possibleMoves[i][0] == targetY && possibleMoves[i][1] == targetX){
                                    canMove = true;
                                }
                            }
                        }
                        else if(moveChar == 'H'){
                            std::vector<std::vector<int>> possibleMoves = {{currentY - 1, currentX},
                                                                           {currentY - 1, currentX + 1},
                                                                           {currentY - 1, currentX - 1},
                                                                           {currentY, currentX - 1},
                                                                           {currentY, currentX + 1},
                                                                           {currentY + 1, currentX}};
                            for(int i = 0; i < possibleMoves.size(); i++){
                                if(possibleMoves[i][0] == targetY && possibleMoves[i][1] == targetX){
                                    canMove = true;
                                }
                            }
                        }
                        else if(moveChar == 'K'){
                            std::vector<std::vector<int>> possibleMoves = {{currentY - 1, currentX - 1},
                                                                           {currentY - 1, currentX},
                                                                           {currentY - 1, currentX + 1},
                                                                           {currentY, currentX - 1},
                                                                           {currentY, currentX + 1},
                                                                           {currentY + 1, currentX - 1},
                                                                           {currentY + 1, currentX},
                                                                           {currentY + 1, currentX + 1}}; // {X, Y}
                            for(int i = 0; i < possibleMoves.size(); i++){
                                if(possibleMoves[i][0] == targetY && possibleMoves[i][1] == targetX){
                                    canMove = true;
                                }
                            }
                    }

                    if(canMove == true){
                        if(map[targetY][targetX] == ' '){
                            map[currentY][currentX] = ' ';
                            if(moveChar == 'P'){
                                if(targetY < 1){
                                    map[targetY][targetX] = 'H';

                                    int myPieceIndex = 0;
                                    for(int i = 0; i < oppPieces.size(); i++){
                                        if(map[targetY][targetX] == oppPieces[i]){
                                            myPieceIndex = i;
                                            break;
                                        }
                                    }

                                    myPieces.erase(myPieces.begin() + myPieceIndex);
                                    myPieces.push_back('H');
                                }
                            }
                            else if(moveChar == 'p'){
                                if(targetY < 1){
                                    map[targetY][targetX] = 'h';

                                    int myPieceIndex = 0;
                                    for(int i = 0; i < oppPieces.size(); i++){
                                        if(map[targetY][targetX] == oppPieces[i]){
                                            myPieceIndex = i;
                                            break;
                                        }
                                    }

                                    myPieces.erase(myPieces.begin() + myPieceIndex);
                                    myPieces.push_back('h');
                                }
                            }
                            else{
                                map[targetY][targetX] = moveChar;
                            }
                        }

                        else if(map[targetY][targetX] != ' '){
                            bool isOppPiece = false;
                            int oppPieceIndex = 0;
                            for(int i = 0; i < oppPieces.size(); i++){
                                if(map[targetY][targetX] == oppPieces[i]){
                                    isOppPiece = true;
                                    oppPieceIndex = i;
                                }
                            }
                            if(isOppPiece == true){
                                oppPieces.erase(oppPieces.begin() + oppPieceIndex);
                                map[currentY][currentX] = ' ';

                                if(map[targetY][targetX] == 'h'){
                                    myPrisonerPieces.push_back('p');
                                }
                                else if(map[targetY][targetX] != 'h'){
                                    myPrisonerPieces.push_back(map[targetY][targetX]);
                                }

                                if(moveChar == 'P' && targetY < 1){
                                    map[targetY][targetX] = 'H';

                                    int myPieceIndex = 0;
                                    for(int i = 0; i < oppPieces.size(); i++){
                                        if(map[targetY][targetX] == oppPieces[i]){
                                            myPieceIndex = i;
                                            break;
                                        }
                                    }

                                    myPieces.erase(myPieces.begin() + myPieceIndex);
                                    myPieces.push_back('H');
                                }
                                else if(moveChar == 'p' && targetY < 1){
                                    map[targetY][targetX] = 'h';

                                    int myPieceIndex = 0;
                                    for(int i = 0; i < oppPieces.size(); i++){
                                        if(map[targetY][targetX] == oppPieces[i]){
                                            myPieceIndex = i;
                                            break;
                                        }
                                    }

                                    myPieces.erase(myPieces.begin() + myPieceIndex);
                                    myPieces.push_back('h');
                                }
                                else{
                                    map[targetY][targetX] = moveChar;
                                }

                            }
                            else if(isOppPiece == false){
                                std::cout << "That piece cannot be taken." << std::endl;
                                continue;
                            }
                        }
                    }
                    else if(canMove == false){
                        std::cout << "That isn't a legal move." << std::endl;
                        continue;
                    }
                }
                else if(isMyPiece == false){
                    std::cout << "You don't have that piece." << std::endl;
                    continue;
                }

			}
			else if(input[0] == '!'){ // Place Piece
                char placeChar = input[1];
                int targetX = input[2] - '0';
                int targetY = input[3] - '0';

                if(0 > targetX || 2 < targetX || 0 > targetY || 3 < targetY){
                    std::cout << "Target Position Out of Range." << std::endl;
                    continue;
                }

                bool isMyPrisoner = false;
                int prisonerIndex = 0;
                for(int i = 0; i < myPrisonerPieces.size(); i++){
                    if(myPrisonerPieces[i] == placeChar){
                        isMyPrisoner = true;
                        prisonerIndex = i;
                        break;
                    }
                }

                if(isMyPrisoner == true){
                    if(map[targetY][targetX] == ' '){
                        if(targetY > 0){
                            map[targetY][targetX] = placeChar;
                            myPrisonerPieces.erase(myPrisonerPieces.begin() + prisonerIndex);
                            myPieces.push_back(placeChar);
                        }
                        else if(targetY == 0){
                            std::cout << "You can't place pieces on opponent land." << std::endl;
                            continue;
                        }
                    }
                    else if(map[targetY][targetX] != ' '){
                        std::cout << "That position is occupied." << std::endl;
                        continue;
                    }
                }
                else if(isMyPrisoner == false){
                    std::cout << "You don't have that prisoner." << std::endl;
                    continue;
                }
			}
		}
		//=============================================AI=============================================
		else if(player_turn == false){
            //AI Move
		}

		if (!check_king_me()) {
			std::cout << "You have lost." << std::endl;
			break;
		}
		if (!check_king_opp()) {
			std::cout << "You have won!" << std::endl;
			break;
		}
		player_turn = !player_turn;
		turn_count++;
		system("cls");
		print_map();
	}
	system("pause");
	return 0;
}
