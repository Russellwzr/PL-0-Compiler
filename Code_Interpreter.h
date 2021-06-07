#ifndef COMPILER_LAB_CODE_INTERPRETER_H
#define COMPILER_LAB_CODE_INTERPRETER_H

#include <bits/stdc++.h>
#include "Syntax_Analyzer.h"
using namespace std;

/*
struct Instruction{
    string f;
    int l;
    int a;
    Instruction(){
        f = "";
        l = a = 0;
    }
    Instruction(string ff,int ll, int aa){
        f = ff;
        l = ll;
        a = aa;
    }
};
*/

class Code_Interpreter{

    public:
        Code_Interpreter();
        Code_Interpreter(vector<Instruction>& C);
        ~Code_Interpreter();
        void Push(int );
        int Pop();
        void OneInstruction();      // process one instruction
        void AdvancePC();           // advance IP register
        void Run();                 // start running

    private:
        vector<int> stack;          // running stack
        vector<Instruction> CODE;   // instructions that need to be processed
        Instruction IR;             // current instruction
        int IP;                     // the address of next instruction
        int SP;                     // the top of stack
        int BP;                     // the base pointer
};

#endif //COMPILER_LAB_CODE_INTERPRETER_H
