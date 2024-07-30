#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>
#include <map>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include "definitions.hpp"
#include <chrono>
#include <string>
#include <memory>

using namespace std;
using namespace chess;
using namespace chrono;

const float piece_values[6] = {1.0, 3.0, 3.25, 5.0, 9.0, 2.0}; // Values for pawn, knight, bishop, rook, queen, king

#define MAXTIME 10000
#define MAXVAL 10000000

const array<int, 64> pawns = {
    0, 0, 0, 0, 0, 0, 0, 0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5, 5, 10, 25, 25, 10, 5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, -5, -10, 0, 0, -10, -5, 5,
    5, 10, 10, -20, -20, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0
};  
const array<int, 64> knights_util = {
    -50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50
} ;       
const array<int, 64> bishops_util = {
    -20,-10,-10,-10,-10,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};      
const array<int, 64> rooks_util = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 5, 5, 0, 0, 0
};    
const array<int, 64> queens_util = {
    -20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5,  5,  5,  5,  0,-10,
	-5,   0,  5,  5,  5,  5,  0, -5,
	0,    0,  5,  5,  5,  5,  0, -5,
	-10,  5,  5,  5,  5,  5,  0,-10,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};
const array<int, 64> kings_start_util = {
    -80, -70, -70, -70, -70, -70, -70, -80,
	-60, -60, -60, -60, -60, -60, -60, -60,
	-40, -50, -50, -60, -60, -50, -50, -40,
	-30, -40, -40, -50, -50, -40, -40, -30,
	-20, -30, -30, -40, -40, -30, -30, -20,
	-10, -20, -20, -20, -20, -20, -20, -10,
	20,  20,  -5,  -5,  -5,  -5,  20,  20, 
	20,  30,  10,   0,   0,  10,  30,  20
};
const array<int, 64> kings_end_util = {
    -20, -10, -10, -10, -10, -10, -10, -20,
	-5,   0,   5,   5,   5,   5,   0,  -5,
	-10, -5,   20,  30,  30,  20,  -5, -10,
	-15, -10,  35,  45,  45,  35, -10, -15,
	-20, -15,  30,  40,  40,  30, -15, -20,
	-25, -20,  20,  25,  25,  20, -20, -25,
	-30, -25,   0,   0,   0,   0, -25, -30,
	-50, -30, -30, -30, -30, -30, -30, -50
};

class pleaseWork
{
    public:
        Board board;
        unordered_map<uint64_t, pair<float,int>> transpositionMatrix;
    //finding out in what state did the game end

    pleaseWork(const string& fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):board(fen)
    {
        getTranspositionMatrix();
    }

    void updateTranspositionMatrix()
    {
        ofstream file("transposition.txt");
        if (file.is_open())
        {
            for (auto& element: transpositionMatrix)
            {
                string id=to_string(element.first);
                string a=to_string(element.second.first);
                string b=to_string(element.second.second);
                file << id << " " << a << " " << b << "\n";
            }
            file.close();
        }
    }

    int endStatus()
    {
        pair<GameResultReason,GameResult> result=board.isGameOver();
        int player=whoseTurn();
        if ((result.second==GameResult::WIN) || (result.second==GameResult::LOSE))
        {
            if (player==1)
            {
                return +1;
            }
            else
            {
                return -1;
            }
        }
        if (result.second==GameResult::DRAW)
        {
            return 0;
        }
    }

    float evaluate() 
    {
            float diff = 0;
            float wEval = 0, bEval = 0;
            int whiteNumber = 0, blackNumber = 0; //this is not meant as a racist comment, its chess
            int P= 0, p = 0;
            int R=0,r=0;
            int N=0,n=0;
            int B = 0, b = 0;
            int Q=0,q=0;
            int K = 0, k = 0;

        const array<const array<int, 64>, 6> piece_util = {pawns, knights_util, bishops_util, rooks_util, queens_util, {}};

        for (int i = 0; i < 64; ++i) {
            int piece = board.at(i);
            if (piece != 12) {
                if (piece < 6) 
                { 
                    whiteNumber++;
                    if (piece!=5)
                    {
                        wEval += piece_values[piece] + piece_util[piece][i] / 100.0;
                    }
                    if (piece == 0) P++;
                    else if (piece==1) N++;
                    else if (piece == 2) B++;
                    else if (piece==3) R++;
                    else if (piece==4) Q++;
                    else if (piece == 5) K = i;
                } 
                else 
                { 
                    blackNumber++;
                    if (piece!=11)
                    {
                        bEval += piece_values[piece] + piece_util[piece][i] / 100.0;
                    }
                    if (piece == 6) p++;
                    else if (piece==7) n++;
                    else if (piece == 8) b++;
                    else if (piece==9) r++;
                    else if (piece==10) q++;
                    else if (piece == 11) k = 63-i;
                }
            }
        }

        const int* kingUtil = (whiteNumber + blackNumber > 8) ? kings_start_util.data() : kings_end_util.data();

        wEval += kingUtil[K] / 100.0;
        bEval += kingUtil[k] / 100.0;

        if (B >= 2 || N>=2 || R>=2) wEval += 1;
        if (b >= 2 || n>=2 || r>=2) bEval += 1;

        if (Q >=1) wEval+=5;
        if (q>=1) bEval+=5;

        diff = wEval - bEval;
        return diff;
    }

    bool isThisTheEnd() //ode to Skyfall
    {
        return (board.isGameOver().second != GameResult::NONE);
    }

    double utilityTerminalState()
    {
        double endState=endStatus();
        if (endState)
        {
            return 1000000*endState;
        }
    }

    array<int,12> pieceArray()
    {
        array<int,12> pieces;
        for (int i=0;i<64;i++)
        {
            int piece=board.at(i);
            if (piece!=12)
            {
                pieces[piece]++;
            }
        }
        return pieces;
    }

    //Black=2 white=1
    int whoseTurn()
    {
        return board.sideToMove()==Color::WHITE?1:2;
    }

    Board updateBoard(const Move& cMove)
    {
        Board updatedBoard=board;
        updatedBoard.makeMove(cMove);
        return updatedBoard;
    }

    vector<Move> fetchLegalMoves()
    {
        Movelist possMoves;
        vector<Move> legalMoves;
        movegen::legalmoves(possMoves,board);
        for (auto& move:possMoves)
        {
            legalMoves.push_back(move);
        }
        return legalMoves;
    }
    
    vector<Move> getOrderedMoves()
    {
        auto moves=fetchLegalMoves();
        vector<pair<Move,float>> order;
        for (auto& move: moves)
        {
            board.makeMove(move);
            int x= endStatus();
            if (x==1 || x==-1)
            {
                board.unmakeMove(move);
                return {move};
            }

            else if (board.inCheck())
            {
                board.unmakeMove(move);
                array<int,12> pieces=pieceArray();
                board.makeMove(move);
                array<int,12> updatedPieces=pieceArray();
                // capture factor
                int player=whoseTurn();
                float score=0;
                if (player==1)
                {
                    if (updatedPieces[6]+updatedPieces[7]+updatedPieces[8]+updatedPieces[9]+updatedPieces[10]<pieces[6]+pieces[7]+pieces[8]+pieces[9]+pieces[10])
                    {
                        if (updatedPieces[10]<pieces[10])
                        {
                            score+=10;
                        }
                        else if (updatedPieces[7]<pieces[7] || updatedPieces[8]<pieces[8] || updatedPieces[9]<pieces[9])
                        {
                            score+=5;
                        }
                        else
                        {
                            score++;
                        }
                    }
                }
                else
                {
                    if (updatedPieces[1]+updatedPieces[0]+updatedPieces[2]+updatedPieces[3]+updatedPieces[4]<pieces[0]+pieces[1]+pieces[2]+pieces[3]+pieces[4])
                    {
                        if (updatedPieces[4]<pieces[4])
                        {
                            score+=10;
                        }
                        else if (updatedPieces[3]<pieces[3] || updatedPieces[2]<pieces[2] || updatedPieces[1]<pieces[1])
                        {
                            score+=5;
                        }
                        else
                        {
                            score++;
                        }
                    }
                }
                order.push_back({move,score+50});
                board.unmakeMove(move);
            }

            else
            {
                board.unmakeMove(move);
                array<int,12> pieces=pieceArray();
                board.makeMove(move);
                array<int,12> updatedPieces=pieceArray();
                
                // capture factor
                int player=whoseTurn();
                float score=0;
                if (player==1)
                {
                    if (updatedPieces[6]+updatedPieces[7]+updatedPieces[8]+updatedPieces[9]+updatedPieces[10]<pieces[6]+pieces[7]+pieces[8]+pieces[9]+pieces[10])
                    {
                        if (updatedPieces[10]<pieces[10])
                        {
                            score+=10;
                        }
                        else if (updatedPieces[7]<pieces[7] || updatedPieces[8]<pieces[8] || updatedPieces[9]<pieces[9])
                        {
                            score+=5;
                        }
                        else
                        {
                            score++;
                        }
                    }
                }
                else
                {
                    if (updatedPieces[1]+updatedPieces[0]+updatedPieces[2]+updatedPieces[3]+updatedPieces[4]<pieces[0]+pieces[1]+pieces[2]+pieces[3]+pieces[4])
                    {
                        if (updatedPieces[4]<pieces[4])
                        {
                            score+=10;
                        }
                        else if (updatedPieces[3]<pieces[3] || updatedPieces[2]<pieces[2] || updatedPieces[1]<pieces[1])
                        {
                            score+=5;
                        }
                        else
                        {
                            score++;
                        }
                    }
                }
                order.push_back({move,evaluate()+score});
                board.unmakeMove(move);
            }
        }

        sort(order.begin(),order.end(),[](auto& x, auto& y)
        {
            return x.second > y.second;
        }
        );

        vector<Move> orderedMoves;

        for(auto& move: order)
        {
            orderedMoves.push_back(move.first);
        }
        return orderedMoves;
    }

    string getTheGoddamnMove(int timeAllocated=10000)
    {
        auto start=high_resolution_clock::now();
        int depth=1;
        if (whoseTurn()==1)
        {
            float bestscore=-MAXVAL;
            string bestmove="";
            double duration=0;
            while (duration<=timeAllocated)
            {
                auto begin=high_resolution_clock::now();
                bool flag=false;
                pair<float,string> result=alphaBetaPrune(depth,-10000000,10000000,start,flag);
                if (flag)
                {
                    break;
                }
                auto duration=duration_cast<milliseconds>(high_resolution_clock::now()-start).count();
                bestscore=result.first;
                bestmove=result.second;
                if (bestscore>=MAXVAL-1000)
                {
                    break;
                }
                if (duration>timeAllocated)
                {
                    break;
                }
                cout<<"depth"<<depth<<"score"<<static_cast<int>(bestscore)<<"time"<<duration<<endl;
                vector<Move> moves(depth);
                for (int x=0;x<depth;x++)
                {
                    if (x==0)
                    {
                        moves[0]=uci::uciToMove(board,bestmove);
                        board.makeMove(moves[0]);
                        cout<<bestmove<<" ";
                    }
                    else
                    {
                        auto newRes=alphaBetaPrune(1,-MAXVAL,MAXVAL,start,flag,0,true);
                        moves[x]=uci::uciToMove(board,newRes.second);
                        board.makeMove(moves[x]);
                        cout<<newRes.second<<" ";
                    }
                }
                cout<<endl;
                for (int i=depth-1;i>=0;i--)
                {
                    board.unmakeMove(moves[i]);
                }
                depth++;
            }
            return bestmove;
        }

        else
        {
            float bestscore=MAXVAL;
            string bestmove="";
            double duration=0;
            while (duration<=timeAllocated)
            {
                //auto begin=high_resolution_clock::now();
                bool flag=false;
                auto result=alphaBetaPrune(depth,-10000000,10000000,start,flag);
                if (flag)
                {
                    break;
                }
                auto duration=duration_cast<milliseconds>(high_resolution_clock::now()-start).count();
                bestscore=result.first;
                bestmove=result.second;
                if (duration>timeAllocated)
                {
                    break;
                }
                if (bestscore==-MAXVAL+1000)
                {
                    break;
                }
                cout<<"depth"<<depth<<"score"<<static_cast<int>(bestscore)<<"time"<<duration<<endl;
                vector<Move> moves(depth);
                for (int x=0;x<depth;x++)
                {
                    if (x==0)
                    {
                        moves[0]=uci::uciToMove(board,bestmove);
                        board.makeMove(moves[0]);
                        cout<<bestmove<<" ";
                    }
                    else
                    {
                        auto newRes=alphaBetaPrune(1,-MAXVAL,MAXVAL,start,flag,0,true);
                        moves[x]=uci::uciToMove(board,newRes.second);
                        board.makeMove(moves[x]);
                        cout<<newRes.second<<" ";
                    }
                }
                cout<<endl;
                for (int i=depth-1;i>=0;i--)
                {
                    board.unmakeMove(moves[i]);
                }
                depth++;
            }
            return bestmove;
        }
    }
    
    private:
    pair<float, string> alphaBetaPrune(int depth, float alpha, float beta, time_point<high_resolution_clock> start, bool& flag, int depthAchieved = 0, bool seed = false)
    {
        if (!seed)
        {
            //auto it = transpositionMatrix.find(board.hash());
            if (transpositionMatrix.find(board.hash()) != transpositionMatrix.end())
            {
                auto tHash = transpositionMatrix[board.hash()];
                if (tHash.second >= depth)
                {
                    return {tHash.first, ""}; 
                }
            }
        }

        if (flag)
        {
            if (whoseTurn()==1)
            {
                return {10000001, ""};
            }
            else
            {
                return {10000001, ""};
            }
        }

        double runningTime = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        if (runningTime >= MAXTIME)
        {
            flag = true;
            if (whoseTurn()==1)
            {
                return {-10000001, ""};
            }
            else
            {
                return {10000001, ""};
            }
        }
        
        if (isThisTheEnd())
        {
            int x=endStatus();
            if (x==1)
            {
                transpositionMatrix[board.hash()] = {-MAXVAL+(100-depth), depth};
            }
            else if (x==-1)
            {
                transpositionMatrix[board.hash()] = {MAXVAL-(100-depth), depth};
            }
            else
            {
                if (whoseTurn()==1) transpositionMatrix[board.hash()] = {10000-MAXVAL, depth};
                else transpositionMatrix[board.hash()] = {-10000+MAXVAL, depth};
            }
            return {utilityTerminalState(), ""}; // Game over
        }

        if (depth == 0)
        {
            return {evaluate(), ""}; 
        }

        vector<Move> orderedMoves = getOrderedMoves();
        pair<float, string> bestMove = whoseTurn()==1?make_pair(-10000000, ""):make_pair(10000000, "");
        bool firstMove = true;
        int moveCount = 0;

        for (const auto& move : orderedMoves)
        {
            board.makeMove(move);
            float score;
            //LMR reqs
            bool tacticalMove = board.inCheck() || board.isCapture(move) || evaluate()>45;
            board.unmakeMove(move);
            if (firstMove || tacticalMove)
            {
                score = -alphaBetaPrune(depth - 1, -beta, -alpha, start, flag, depthAchieved + 1, false).first;
                firstMove = false;
            }
            else
            {
                int reduction = 1; // Simple reduction because not tactical
                if (depth > 3 && moveCount > 2)
                {
                    reduction = 2; // More aggressive reduction
                }

                score = -alphaBetaPrune(depth - 1 - reduction, -beta, -alpha, start, flag, depthAchieved + 1, false).first;
                if (score > alpha)
                {
                    score = -alphaBetaPrune(depth - 1, -beta, -alpha, start, flag, depthAchieved + 1, false).first;
                }
            }

            board.unmakeMove(move);

            if (whoseTurn()==1)
            {
                if (score>bestMove.first)
                {
                    bestMove={score,uci::moveToUci(move)};
                    alpha=max(alpha,score);
                }
            }
            else
            {
                if (score<bestMove.first)
                {
                    bestMove={score,uci::moveToUci(move)};
                    beta=min(beta,score);
                }
            }

            if (score > bestMove.first)
            {
                bestMove = {score, uci::moveToUci(move)};
            }

            if (alpha >= beta)
            {
                break;
            }

            moveCount++;
        }

        if (flag)
        {
            return {whoseTurn()==1?alpha:beta, bestMove.second};
        }

        if (depthAchieved == 0)
        {
            transpositionMatrix[board.hash()] = {bestMove.first, depth};
        }

        return bestMove;
    }

    void getTranspositionMatrix()
    {
        ifstream file("transposition.txt");
        if (file.is_open())
        {
            string line;
            while(getline(file,line))
            {
                istringstream thisString(line);
                string id;
                float a, b;
                if (thisString>>id>>a>>b)
                {
                    uint64_t hash=stoull(id);
                    transpositionMatrix[hash]={a,b};
                }
            }
            file.close();
        }
    }
};

void sendResponse(const std::string& response) 
{
    cout << response << std::endl;
}

void handleUci() 
{
    sendResponse("id name AbbyV3");
    sendResponse("id author Ganymede");
    sendResponse("uciok");
}

void handleQuit() 
{
    exit(0);
}

void handleIsSet() 
{
    sendResponse("letsGo!");
}



int main()
{
    pleaseWork chessEngine;
    string input;
    cin>>input;
        while(getline(cin, input)) 
        {
            if (input == "uci") 
            {
                handleUci();
            } 
            else if (input == "isready") 
            {
                handleIsSet();
            } 
            else if (input == "quit") 
            {
                handleQuit();
            }
            else if (input.substr(0,8)=="position")
            {
                istringstream iss(input);
                string word;
                iss>>word;
                string type;
                iss>>type;
                if (type=="startpos")
                {
                    chessEngine.board=Board();
                    iss>>word;
                }
                else if (type=="fen")
                {
                    string fen;
                    while(iss>>word && word!="moves")
                    {
                        fen+=word+" ";
                    }
                    chessEngine=pleaseWork(fen);
                }
                if (word=="moves")
                {
                    string move;
                    while(iss>>move)
                    {
                        chessEngine.board.makeMove(uci::uciToMove(chessEngine.board,move));
                    }
                }
            }
            else if (input.substr(0,2)=="go")
            {
                string move=chessEngine.getTheGoddamnMove();
                sendResponse("bestmove "+move);
            }
            else if (input.substr(0,9)=="lets play")
            {
                istringstream iss(input);
                string playComm, time;
                int maxTime=10000;
                iss>>playComm>>time>>maxTime;
                cout<<maxTime<<"->maxTime in milliseconds"<<endl;
                string move=chessEngine.getTheGoddamnMove(maxTime);
                sendResponse("bestmove "+move);
            }
        }
}