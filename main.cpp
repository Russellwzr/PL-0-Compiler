// Zhirui WANG
// Compiler Lab

#include <bits/stdc++.h>
#include "Lexical_Analyzer.h"
#include "Syntax_Analyzer.h"
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
// LABEL : the label
vector<string> LABEL;

int main() {

    freopen("../test/test1.txt","r", stdin);
    //freopen("../test/more_1.out","w",stdout);

    // Create Reserved Words Map
    map<string, int> mp;
    for(int i = 0; i < 13; i ++){
        mp[key_words[i]] = KEY_WORDS;
    }
    for(int i = 0; i < 11; i ++){
        mp[key_operators[i]] = KEY_OP;
    }
    for(int i = 0; i < 5; i ++){
        mp[key_split[i]] = KEY_SP;
    }

    if(GETSYM(SYM, ID, NUM, LABEL, mp) < 0){
        cout<<"Lexical Error"<<endl;
    }
    else{
        LEXICAL_OUTPUT();
        //cout << "LABEL OUTPUT"<< endl;
        //for(int i = 0; i < LABEL.size(); i ++){
        //    cout << LABEL[i] << endl;
        //}
    }

    //Syntax_Analyzer Test = Syntax_Analyzer(SYM);
    //string tmp = "Precedure";
    //Test.PrintErrorPos(tmp);

    return 0;
}