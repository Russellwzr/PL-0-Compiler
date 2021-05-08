#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

static bool IsNum(char a){
    return ( a >= '0' && a <= '9' );
}

static bool IsLetter(char a){
    return ( a >= 'A' && a <= 'Z' ) || ( a >= 'a' && a <= 'z');
}

static string LLtoString(ll num){
    string str;
    ostringstream ss;
    ss<<num;
    istringstream is(ss.str());
    is>>str;
    return str;
}

#endif //COMPILER_LAB_UTILS_H
