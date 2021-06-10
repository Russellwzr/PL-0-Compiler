//******************************************************************************************************************
//
// Author : Zhirui WANG
// Title  : SDU Compiler Lab
//                --- A PL/0 Language Compiler
//
//******************************************************************************************************************

#include <bits/stdc++.h>
#include "Lexical_Analyzer.h"
#include "Syntax_Analyzer.h"
#include "Code_Interpreter.h"
using namespace std;

// Reserved Words
string key_words[13] = {"CONST","VAR","PROCEDURE","BEGIN","END","ODD","IF","THEN","CALL","WHILE","DO","READ","WRITE"};
string key_operators[11] = {"=",":=","+","-","*","/","#","<","<=",">",">="};
string key_split[5] = {";",",",".","(",")"};

// SYM : word type
vector<string> SYM;
// ID : the value of the user-defined identifier
vector<string> ID;
// NUM : user-defined num
vector<ll> NUM;
// LABEL : words array (lexical analyzer's output)
vector<string> LABEL;
// CODE : target instructions
vector<Instruction> CODE;

int main() {

    // true  : compiler
    // false : interpreter
    bool flag = false;

    if(flag){
        // true  : output for debugging
        // false : output for oj
        bool debug = false;

        freopen("../test3/t6.in","r", stdin);
        freopen("../test3/t6.out","w",stdout);

        // Create Reserved Words Map
        map<string, int> mp;
        for(int i = 0; i < 13; i ++){
            mp[key_words[i]] = KEY_WORDS;  // 1
        }
        for(int i = 0; i < 11; i ++){
            mp[key_operators[i]] = KEY_OP; // 2
        }
        for(int i = 0; i < 5; i ++){
            mp[key_split[i]] = KEY_SP;     // 3
        }

        // Lexical Analyzer
        if(GETSYM(SYM, ID, NUM, LABEL, mp) < 0){
            cout<<"Lexical Error"<<endl;
            return -1;
        }

        // Syntax Analyzer
        Syntax_Analyzer Test1 = Syntax_Analyzer(SYM, LABEL);


        if(Test1.Program()){
            CODE = Test1.Target_Instructions();
            for(int i = 0; i < CODE.size(); i ++){
                cout << CODE[i].f << " " << CODE[i].l << " " << CODE[i].a << endl;
            }
            //Test1.TableOutput();
        }
        else{
            cout<<"Syntax Error"<<endl;
            return -1;
        }
    }
    else{
        ifstream in;
        in.open("../test3/t6.out");
        string f;
        int l,a;
        vector<Instruction> CODES;
        while(in >> f >> l >> a){
            CODES.push_back(Instruction(f, l, a));
        }
        Code_Interpreter test(CODES);
        test.Run();
        return 0;
    }
    return 0;
}