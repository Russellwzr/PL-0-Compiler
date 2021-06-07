//******************************************************************************************************************
//
// Author : Zhirui WANG
// Title  : SDU Compiler Lab
//                --- A PL/0 Language Compiler
//
//******************************************************************************************************************

#include <bits/stdc++.h>
#include "Code_Interpreter.h"
using namespace std;

// CODE : target instructions
vector<Instruction> CODE;

int main() {

    freopen("../test3/t1.out","r", stdin);
    string f;
    int l,a;
    while(cin >> f >> l >> a){
        CODE.push_back(Instruction(f, l, a));
    }
    Code_Interpreter Test2 = Code_Interpreter(CODE);
    Test2.Run();

    return 0;

}