#include <bits/stdc++.h>

#include "../src/function_space.h"

using namespace std;

int main(){
    cout << "Sudoku" << endl;

    FunctionSpace s;

    int mx = 2;
    int sz = mx*mx;
    vector<vector<NodeVector>> board(sz, vector<NodeVector>(sz));
    for(int i = 0 ; i < sz; i++)
        for(int j = 0 ; j < sz; j++)
            board[i][j] = s.createVars(sz, "s" + to_string(i) + to_string(j));

    #define get_row(i,j) i
    #define get_column(i,j) j
    #define get_square(i,j) (i/mx)*mx+j/mx

    NodeVector constraints[3][sz][sz];

    for(int i = 0; i < sz; i++)
        for(int j = 0; j < sz; j++){
            for(int k = 0; k < sz; k++){
                constraints[0][i                ][j].push_back(board[i][j][k]);
                constraints[1][get_row(i,j)     ][k].push_back(board[i][j][k]);
                constraints[2][get_column(i,j)  ][k].push_back(board[i][j][k]);
                //constraints[3][get_square(i,j)  ][k].push_back(board[i][j][k]);
            }
        }

    Node eq = s.True();
    NodeVector constraintsCollapsed(4);

    for(int constraint_type = 0; constraint_type < 3; constraint_type++){
        constraintsCollapsed[constraint_type] = s.True();
        for(int i = 0; i < sz; i++){
            for(int j = 0; j < sz; j++){
                constraintsCollapsed[constraint_type] = s.and2(
                    constraintsCollapsed[constraint_type], s.isOneHot(constraints[constraint_type][i][j]));
            }
            cout << ">";
        }
    }
    cout << endl;

    /*eq = s.and2(
        s.and2(constraintsCollapsed[0],constraintsCollapsed[1]),
        s.and2(constraintsCollapsed[2],constraintsCollapsed[3])
    );*/
    eq = s.and2(constraintsCollapsed[0],s.and2(constraintsCollapsed[2], constraintsCollapsed[1]));


    //std::cout << s << std::endl;


    auto v = s.dfs(eq);

    int prev = 1;
    while(v[prev].first != -1){
        auto [next, val] = v[prev];
        std::cout << s.getLabel(s.top(next)) << " " << val << std::endl;
        prev = next;
    }


    return 0;
}