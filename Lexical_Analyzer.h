// Lexical Analyzer
#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#define KEY_WORDS 1
#define KEY_OP 2
#define KEY_SP 3
#define IDENT_TYPE 4
#define NUM_TYPE 5

// Reserved Words : Output the word
// Number : NUMBER VAL
// Identifier : IDENTIFIER VAL
// ERROR : Lexical Error
string output_info = "";
void LEXICAL_OUTPUT(){
    cout<<output_info;
}

// KEY：SYM = KEY
// IDENT：SYM = IDENT, ID = WORD_VAL
// NUM：SYM = NUM, NUM = WORD_VAL
// NOTE : Regardless of grammatical errors in this lab
int GETSYM(vector<string>& SYM, vector<string>& ID, vector<ll>& NUM, vector<string>& LABEL, map<string, int> mp){
    // Processing Loop......
    string str;
    while(getline(cin, str)){
        int len = str.length();
        for(int i = 0; i < len; i ++){
            if(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'){
                continue;
            }
            // Number
            // 11111 : TRUE
            // 1111a : FALSE
            else if(IsNum(str[i])){
                string tmp = "";
                tmp += str[i ++];
                while((i < len) && IsNum(str[i])){
                    tmp += str[i ++];
                }
                //TRUE
                if((i == len) || !IsLetter(str[i])){
                    SYM.push_back("NUM");
                    ll tmp_num = atoll(tmp.c_str());
                    NUM.push_back(tmp_num);
                    output_info += "NUMBER ";
                    LABEL.push_back(LLtoString(tmp_num));
                    output_info += LLtoString(tmp_num);
                    output_info += '\n';
                }
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
                if(str[i] >= 'a' && str[i] <= 'z'){
                    str[i] -= 32;
                }
                tmp += str[i ++];
                while((i < len) && (IsLetter(str[i]) || IsNum(str[i]))){
                    if(str[i] >= 'a' && str[i] <= 'z'){
                        str[i] -= 32;
                    }
                    tmp += str[i ++];
                }
                if(tmp.length() > 10){
                    return -1;
                }
                // KeyWord
                if(mp.find(tmp) != mp.end()){
                    if(mp[tmp] == KEY_WORDS){
                        //string tmp_sym  = tmp;
                        //tmp_sym += "SYM";
                        SYM.push_back(tmp);
                        LABEL.push_back(tmp);
                        output_info += tmp;
                        output_info += '\n';
                        //cout<< tmp << endl;
                    }
                    else{
                        return -1;
                    }
                }
                // Identifier
                else{
                    if((i >= len) || !IsLetter(str[i])){
                        SYM.push_back("IDENT");
                        LABEL.push_back(tmp);
                        ID.push_back(tmp);
                        output_info += "IDENTIFIER ";
                        output_info += tmp;
                        output_info += '\n';
                        //cout<<"IDENTIFIER"<<" "<<tmp<<endl;
                    }
                    else{
                        return -1;
                    }
                }
                i--;
            }
            // Search Map ----> Judge Type
            else{

                string tmp = "";
                tmp += str[i ++];
                // continue read next char
                if( i < len ){
                    string tmp2 = tmp;
                    tmp2 += str[i];
                    if(mp.find(tmp2) != mp.end()){
                        //string tmp_sym  = tmp2;
                        //tmp_sym += "SYM";
                        SYM.push_back(tmp2);
                        LABEL.push_back(tmp2);
                        output_info += tmp2;
                        output_info += '\n';
                    }
                    // the advanced one not match
                    // , then judge the previous one
                    else{
                        if(mp.find(tmp) != mp.end()){
                            //string tmp_sym  = tmp;
                            //tmp_sym += "SYM";
                            SYM.push_back(tmp);
                            LABEL.push_back(tmp);
                            output_info += tmp;
                            output_info += '\n';
                            i --;
                        }
                        else{
                            return -1;
                        }
                    }
                }
                // only read this one
                else{
                    if(mp.find(tmp) != mp.end()){
                        //string tmp_sym  = tmp;
                        //tmp_sym += "SYM";
                        SYM.push_back(tmp);
                        LABEL.push_back(tmp);
                        output_info += tmp;
                        output_info += '\n';
                    }
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
