#include <stdlib.h>
#include <stdio.h>
#define M 6 //rows
#define N 7 //columns
#define K 4 

struct Game {
	int *board;			//2d array that contains data describing board cells
   	int current_move;	//column number of the current player's move
	int valid_move;		//(0 or 1) for whether current move is valid
	int current_p;		//(1 or 2) for which player's turn it is
	int winning_p;		//(1 or 2) for winning player,if there is a winner
	int winner;			//(0 or 1) 1 if there is a winning move on board
	int computer;		//(0 or 1) 1 if a computer is playing
	int difficulty;		//(4 or 6) recursion depth for computer moves
};

struct Game *makeGame();
int play(struct Game *game);
void getMove(struct Game * game);
void enterMove(struct Game * game,int player);
void checkMove(struct Game * game);
void checkWin(struct Game * game);
void printBoard(struct Game * game);
int minMax(int depth, int max_depth,int player,struct Game * game, int alpha, int beta);
struct Game *cloneGame(struct Game * old_game);
int scoreBoard(struct Game * game);
int evaluateLine(int *board, int row, int col, int deltaRow, int deltaCol);
int fullBoard(struct Game * game);
void freeGame(struct Game * game);

int main(void) {
	struct Game *current_game = NULL;
	current_game = makeGame();	//create game
	printf("Choose difficulty : \n1 -> easy\n2 -> hard\n\n");
	int difficulty;
	scanf("%d",&difficulty); //player picks difficulty
	while (difficulty != 1 && difficulty != 2) {
		printf("Enter a valid option: ");
		scanf("%d",&difficulty);
	}
	current_game -> computer = 1;	//activate computer player
	//set difficulty to 4 for easy or 6 for hard
	if (difficulty == 1) {current_game -> difficulty = 4;}
	if (difficulty == 2) {current_game -> difficulty = 6;}
	play(current_game);
	freeGame(current_game);
	return 0;
} 

int play(struct Game *current_game) {
	while (1) {
		printBoard(current_game);			
		getMove(current_game);				//get move input from player
		checkMove(current_game);		
		while(current_game->valid_move == 0) {	//if not valid...
			printBoard(current_game);
			printf("Invalid move,try again\n");
			getMove(current_game);				//get and check new move
			checkMove(current_game);
		}
		enterMove(current_game,current_game->current_p);
		checkWin(current_game);				//check if winner found
		if (current_game->winner == 1) {	//end game and declare winner
			printBoard(current_game);
			if (current_game->winning_p == 1) {
				printf("\nComputer wins!\n\n");
			} else {
				printf("\nPlayer wins!\n\n");
			}
			return 1;
		}
		if (fullBoard(current_game)) {
			printBoard(current_game);
			printf("\nGame ended in a draw!\n\n");
			return 0;
		}
		if (current_game->current_p == 1) {	//switch players
			current_game->current_p = 2;
		}
		else {
			current_game->current_p = 1;
		}
    }
}

void printBoard(struct Game * game) {
	int i;
	int j;
	printf("| 0   1   2   3   4   5   6 |\n");
	printf("|___|___|___|___|___|___|___|\n");
	for(i=0;i<5;i++) {							
		printf("|");
		for(j=0;j<N;j++) {
			if (game->board[i*N+j]==1) {
				printf(" x |");
			} else if (game->board[i*N+j]==2) {
				printf(" o |");
			} else {
				printf("   |");
			}									
		}
		printf("\n");
	}
	printf("|");
	for(j=0;j<N;j++) {
		if (game->board[5*N+j]==1) {
  			printf("_x_|");
		} else if (game->board[5*N+j]==2) {
			printf("_o_|");
		} else {
			printf("___|");
		}
	}
	printf("\n");
}

void checkMove(struct Game * game){
	int *board = game->board;
	int move = game->current_move;
	if (move>=0 && move <=6) {
		if (board[move] == 0) {
			game->valid_move = 1;	
		}
		else {
			game->valid_move = 0;
		}
	}
	else {
		game->valid_move = 0;
	}
}

void getMove(struct Game * game) {
	int move,diff;
	if (game -> computer == 1 && game -> current_p == 1) {
		printf("\nGetting computer move...\n");
		diff = game -> difficulty;
		
		// First check for immediate winning move
		int win_move = -1;
		for (int col = 0; col < N; col++) {
			struct Game *test = cloneGame(game);
			test->current_move = col;
			checkMove(test);
			if (test->valid_move) {
				enterMove(test, 1); // computer is player 1
				checkWin(test);
				if (test->winner == 1 && test->winning_p == 1) {
					win_move = col;
					freeGame(test);
					break;
				}
			}
			freeGame(test);
		}
		
		if (win_move != -1) {
			game->current_move = win_move;
			printf("Computer chooses winning move: %d\n", win_move);
			return;
		}
		
		// Then check for blocking opponent's winning move
		int block_move = -1;
		for (int col = 0; col < N; col++) {
			struct Game *test = cloneGame(game);
			test->current_move = col;
			checkMove(test);
			if (test->valid_move) {
				enterMove(test, 2); // human is player 2
				checkWin(test);
				if (test->winner == 1 && test->winning_p == 2) {
					block_move = col;
					freeGame(test);
					break;
				}
			}
			freeGame(test);
		}
		
		if (block_move != -1) {
			game->current_move = block_move;
			printf("Computer chooses blocking move: %d\n", block_move);
			return;
		}
		
		// Otherwise use minimax
		game->current_move = minMax(diff,diff,1,game, -1000000, 1000000);
		printf("Computer chooses column: %d\n", game->current_move);
	} else {
		printf("\nChoose the next move for Player: ");
		scanf("%d",&move);
		game->current_move=move; 
	}
}

void enterMove(struct Game * game,int player) {
	int move = game->current_move;		
	int *board = game->board;
	int i;
	
	for(i=0; i<M; i++){
        if (board[i*N+move] != 0) {
			board[(i-1)*N+move] = player;
			return;
		}
		else if (i==5) {
			board[35+move] = player;
			return;
		}
    }
}

void checkWin(struct Game * game) {
	int *board = game->board;
	int i,j;
	
	// Reset winner status
	game->winner = 0;
	game->winning_p = 0;
	
	//check horizontal wins
	for(i=0; i<M; i++){ 
		for(j=0; j<=N-4; j++){
			if(board[i*N+j] != 0 && 
			   board[i*N+j] == board[i*N+j+1] &&
			   board[i*N+j+1] == board[i*N+j+2] &&
			   board[i*N+j+2] == board[i*N+j+3]) {
				game->winner = 1;
				game->winning_p = board[i*N+j];
				return;
			}
		}
	}
	
	//check vertical wins
	for(j=0; j<N; j++){ 
		for(i=0; i<=M-4; i++){
			if(board[i*N+j] != 0 &&
			   board[i*N+j] == board[(i+1)*N+j] &&
			   board[(i+1)*N+j] == board[(i+2)*N+j] &&
			   board[(i+2)*N+j] == board[(i+3)*N+j]) {
				game->winner = 1;
				game->winning_p = board[i*N+j];
				return;
			}
		}
	}
	
	//check diagonal (down-right) wins
	for(i=0; i<=M-4; i++){ 
		for(j=0; j<=N-4; j++){
			if(board[i*N+j] != 0 &&
			   board[i*N+j] == board[(i+1)*N+j+1] &&
			   board[(i+1)*N+j+1] == board[(i+2)*N+j+2] &&
			   board[(i+2)*N+j+2] == board[(i+3)*N+j+3]) {
				game->winner = 1;
				game->winning_p = board[i*N+j];
				return;
			}
		}
	}
	
	//check diagonal (up-right) wins
	for(i=3; i<M; i++){ 
		for(j=0; j<=N-4; j++){
			if(board[i*N+j] != 0 &&
			   board[i*N+j] == board[(i-1)*N+j+1] &&
			   board[(i-1)*N+j+1] == board[(i-2)*N+j+2] &&
			   board[(i-2)*N+j+2] == board[(i-3)*N+j+3]) {
				game->winner = 1;
				game->winning_p = board[i*N+j];
				return;
			}
		}
	}
}

// Improved minimax that properly handles win detection
int minMax(int depth, int max_depth, int player, struct Game * game, int alpha, int beta) {
    // Check for terminal states first
    checkWin(game);
    if (game->winner == 1) {
        if (game->winning_p == player) {
            return 100000 + depth; // Prefer faster wins
        } else {
            return -100000 - depth; // Prefer slower losses
        }
    }

    if (fullBoard(game)) {
        return 0;
    }
    
    if (depth == 0) {
        return scoreBoard(game);
    }
    
    int best_score = (player == 1) ? -1000000 : 1000000;
    int best_column = 3; // Default to center
    
    for (int col = 0; col < N; col++) {
        struct Game *game_clone = cloneGame(game);
        game_clone->current_move = col;
        checkMove(game_clone);
        
        if (!game_clone->valid_move) {
            freeGame(game_clone);
            continue;
        }
        
        enterMove(game_clone, player);
        
        // Check if this move results in immediate win
        checkWin(game_clone);
        if (game_clone->winner == 1) {
            if (game_clone->winning_p == player) {
                freeGame(game_clone);
                if (depth == max_depth) return col;
                return 100000 + depth;
            } else {
                freeGame(game_clone);
                if (depth == max_depth) continue; // Don't choose losing move
                return -100000 - depth;
            }
        }
        
        // Switch players for next move
        int next_player = (player == 1) ? 2 : 1;
        int score = minMax(depth-1, max_depth, next_player, game_clone, alpha, beta);
        
        freeGame(game_clone);
        
        if (player == 1) { // Maximizing player (computer)
            if (score > best_score) {
                best_score = score;
                best_column = col;
            }
            if (score > alpha) alpha = score;
            if (beta <= alpha) break; // Beta cutoff
        } else { // Minimizing player (human)
            if (score < best_score) {
                best_score = score;
                best_column = col;
            }
            if (score < beta) beta = score;
            if (beta <= alpha) break; // Alpha cutoff
        }
    }
    
    if (depth == max_depth) {
        return best_column;
    }
    return best_score;
}

struct Game *cloneGame(struct Game * old_game){
	struct Game *new_game = makeGame();
   	new_game->current_move = old_game->current_move;
   	new_game->valid_move = old_game->valid_move;
   	new_game->current_p = old_game->current_p;
   	new_game->winning_p = old_game->winning_p;
   	new_game->winner = old_game->winner;
	new_game->computer = old_game->computer;
	new_game->difficulty = old_game->difficulty;
	
    for(int i = 0; i < M*N; i++){
        new_game->board[i] = old_game->board[i];
    }
	return new_game;
}

// Helper function to evaluate a line of 4 positions
int evaluateLine(int *board, int row, int col, int deltaRow, int deltaCol) {
    int computer_count = 0;
    int human_count = 0;
    
    for (int i = 0; i < 4; i++) {
        int r = row + i * deltaRow;
        int c = col + i * deltaCol;
        if (board[r*N+c] == 1) computer_count++;
        else if (board[r*N+c] == 2) human_count++;
    }
    
    // Score based on the line composition - much higher weights for threats
    if (computer_count == 4) return 100000;
    if (human_count == 4) return -100000;
    if (computer_count == 3 && human_count == 0) return 5000;  // Very high for 3 in a row
    if (computer_count == 2 && human_count == 0) return 100;   // Good for 2 in a row
    if (computer_count == 1 && human_count == 0) return 10;    // Decent for 1
    if (human_count == 3 && computer_count == 0) return -5000; // Must block
    if (human_count == 2 && computer_count == 0) return -100;  // Should block
    if (human_count == 1 && computer_count == 0) return -10;   // Slight negative
    
    return 0;
}

// Improved scoring function with better threat detection
int scoreBoard(struct Game * game) {
    int score = 0;
    int *board = game->board;
    
    // Center column preference (stronger)
    for (int i = 0; i < M; i++) {
        if (board[i*N+3] == 1) score += 10;
        else if (board[i*N+3] == 2) score -= 10;
        if (board[i*N+2] == 1 || board[i*N+4] == 1) score += 5;
        if (board[i*N+2] == 2 || board[i*N+4] == 2) score -= 5;
    }
    
    // Evaluate all possible lines
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            // Horizontal
            if (j <= N-4) {
                score += evaluateLine(board, i, j, 0, 1);
            }
            // Vertical
            if (i <= M-4) {
                score += evaluateLine(board, i, j, 1, 0);
            }
            // Diagonal down-right
            if (i <= M-4 && j <= N-4) {
                score += evaluateLine(board, i, j, 1, 1);
            }
            // Diagonal up-right
            if (i >= 3 && j <= N-4) {
                score += evaluateLine(board, i, j, -1, 1);
            }
        }
    }
    
    return score;
}

int fullBoard(struct Game * game) {
	int *board = game->board;
	for (int j = 0; j < N; j++) {
		if (board[j] == 0) {
			return 0;
		}
	}
	return 1;
}

struct Game *makeGame(){
	struct Game *current_game = malloc(sizeof(struct Game));
	if(!current_game) {
		printf("Failed to allocate game\n");
		exit(1);
	}
	
	current_game->board = malloc(sizeof(int)*M*N);
	if(!current_game->board) {
		printf("Failed to allocate board\n");
		free(current_game);
		exit(1);
	}

	current_game->current_move = 0;
	current_game->current_p = 1;
	current_game->winner = 0;
	current_game->winning_p = 0;
	current_game->valid_move = 1;
	current_game->computer = 0;
	current_game->difficulty = 0;
	
	for(int i = 0; i < M*N; i++){
		current_game->board[i] = 0;
	}
	
	return current_game;	
}

void freeGame(struct Game * game) {
    if (game) {
        if (game->board) {
            free(game->board);
        }
        free(game);
    }
}