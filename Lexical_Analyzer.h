// Lexical Analyzer
#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

// Define Word Type
#define KEY_WORDS 1
#define KEY_OP 2
#define KEY_SP 3
#define IDENT_TYPE 4
#define NUM_TYPE 5

// Reserved Words : Output the Word
// Number : NUMBER VAL
// Identifier : IDENTIFIER VAL
// ERROR : Lexical Error


// Output for Online Judge
string output_info = "";
void LEXICAL_OUTPUT(){
    cout<<output_info;
}

//***********************************************************************************************************
//
// Lexical Analyzer
//
// Parameters:
//
// SYM   : word type (KEY / IDENT / NUM)
// LABEL : words array (lexical analyzer's output)
// mp    : Reserved Words Map
//
// Return 1   ------> success
// Return -1  ------> fail
// NOTE : Regardless of grammatical errors in this part
//
//***********************************************************************************************************
int GETSYM(vector<string>& SYM, vector<string>& LABEL, map<string, int> mp){
    // Processing Loop......
    string str;
    while(getline(cin, str)){
        int len = str.length();
        for(int i = 0; i < len; i ++){
            // pass space, tab and CR\LF......
            if(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'){
                continue;
            }
            // Number
            // Such as:
            // 11111 : TRUE
            // 1111a : FALSE
            else if(IsNum(str[i])){
                string tmp = "";
                tmp += str[i ++];
                while((i < len) && IsNum(str[i])){
                    tmp += str[i ++];
                }
                // TRUE
                if((i == len) || !IsLetter(str[i])){
                    SYM.push_back("NUM");
                    ll tmp_num = atoll(tmp.c_str());
                    output_info += "NUMBER ";
                    LABEL.push_back(LLtoString(tmp_num));
                    output_info += LLtoString(tmp_num);
                    output_info += '\n';
                }
                // FALSE
                else{
                    return -1;
                }
                i --;
            }
            // KeyWord or Identifier
            // In Map ----> KeyWord
            // Not in map ----> Identifier
            // Len > 10 ----> FALSE
            else if(IsLetter(str[i])){
                string tmp = "";
                // Lower to Upper
                if(str[i] >= 'a' && str[i] <= 'z'){
                    str[i] -= 32;
                }
                tmp += str[i ++];
                while((i < len) && (IsLetter(str[i]) || IsNum(str[i]))){
                    // Lower to Upper
                    if(str[i] >= 'a' && str[i] <= 'z'){
                        str[i] -= 32;
                    }
                    tmp += str[i ++];
                }
                // Len > 10 ----> FALSE
                if(tmp.length() > 10){
                    return -1;
                }
                // In Map ----> KeyWord
                if(mp.find(tmp) != mp.end()){
                    if(mp[tmp] == KEY_WORDS){
                        SYM.push_back("KEY");
                        LABEL.push_back(tmp);
                        output_info += tmp;
                        output_info += '\n';
                    }
                    else{
                        return -1;
                    }
                }
                // Not in map ----> Identifier
                else{
                    if((i >= len) || !IsLetter(str[i])){
                        SYM.push_back("IDENT");
                        LABEL.push_back(tmp);
                        output_info += "IDENTIFIER ";
                        output_info += tmp;
                        output_info += '\n';
                    }
                    else{
                        return -1;
                    }
                }
                i--;
            }
            // Search Map ----> Judge Type
            // then judge match one or two chars
            else{

                string tmp = "";
                tmp += str[i ++];
                // i < len then,
                // continue read next char to judge if two chars can be matched......
                if( i < len ){
                    string tmp2 = tmp;
                    tmp2 += str[i];
                    // match two
                    if(mp.find(tmp2) != mp.end()){
                        SYM.push_back("KEY");
                        LABEL.push_back(tmp2);
                        output_info += tmp2;
                        output_info += '\n';
                    }
                    // if the advanced one not match
                    // , then judge the previous one
                    else{
                        // match the previous one
                        if(mp.find(tmp) != mp.end()){
                            SYM.push_back("KEY");
                            LABEL.push_back(tmp);
                            output_info += tmp;
                            output_info += '\n';
                            i --;
                        }
                        // not match
                        else{
                            return -1;
                        }
                    }
                }
                // i >= len then,
                // no char remains to be read, so only read this one
                else{
                    // match
                    if(mp.find(tmp) != mp.end()){
                        SYM.push_back("KEY");
                        LABEL.push_back(tmp);
                        output_info += tmp;
                        output_info += '\n';
                    }
                    // not match
                    else{
                        return -1;
                    }
                }
            }
        }
        str = "";
    }
    return 1;
}

#endif //COMPILER_LAB_LEXICAL_ANALYZER_H
