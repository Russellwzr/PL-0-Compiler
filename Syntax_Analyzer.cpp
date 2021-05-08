#include "Syntax_Analyzer.h"
#include "utils.h"

Syntax_Analyzer::Syntax_Analyzer(){
    ip = 0;
    SYM.clear();
    curIndex = 0;
}

Syntax_Analyzer::Syntax_Analyzer(vector<string>& SS, vector<string>& LL){
    ip = 0;
    SYM = SS;
    LABEL = LL;
    curIndex = 0;
}

Syntax_Analyzer::~Syntax_Analyzer(){
    ip = 0;
    SYM.clear();
    curIndex = 0;
}

void Syntax_Analyzer::PrintErrorPos(string msg){
    cerr<< "Error: Syntax Error at position "<< ip <<" in "<< msg << "!"<<endl;
}

void Syntax_Analyzer::InsertNode(int fatherID, string label){
    vector<int> tmp;
    tree.push_back(tmp);
    nodeInfo.push_back(label);
    tree[fatherID].push_back(curIndex);
    curIndex ++;
}

bool Syntax_Analyzer::Program(){

    string msg = "PROGRAM";

    int fatherID = curIndex;
    vector<int> tmp;
    tree.push_back(tmp);
    nodeInfo.push_back("PROGRAM");
    curIndex ++;

    if(!SubProgram(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(SYM[ip] != "." || ip != SYM.size() - 1){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

bool Syntax_Analyzer::SubProgram(int f){
    string msg = "SUBPROG";
    int fatherID = curIndex;
    InsertNode(f, msg);
    if(SYM[ip] == "CONST"){
        if(!ConstantDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(SYM[ip] == "VAR"){
        if(!VariableDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(SYM[ip] == "PROCEDURE"){
        if(!ProcedureDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

bool Syntax_Analyzer::ConstantDeclare(int f){
    int fatherID = curIndex;
    string msg = "CONSTANTDECLARE";
    InsertNode(f, msg);
    if(SYM[ip] != "CONST"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    if(!ConstantDefine(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(SYM[ip] == ","){
        ip ++;
        if(!ConstantDefine(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(SYM[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::ConstantDefine(int f){
    int fatherID = curIndex;
    string msg = "CONSTANTDEFINE";
    InsertNode(f, msg);
    if(!Identifier(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(SYM[ip] != "="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    if(!UnsignedNum(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

bool Syntax_Analyzer::UnsignedNum(int f){
    string msg = LABEL[ip];
    InsertNode(f, msg);
    for(int i = 0; i < msg.size(); i ++){
        if(!IsNum(msg[i])){
            PrintErrorPos(msg);
            return false;
        }
    }
    ip ++;
    return true;
}

bool Syntax_Analyzer::VariableDeclare(int f){
    int fatherID = curIndex;
    string msg = "VARIABLEDECLARE";
    InsertNode(f, msg);
    if(SYM[ip] != "VAR"){
        PrintErrorPos(msg);
        return false;
    }
    if(!Identifier(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(SYM[ip] == ","){
        ip ++;
        if(!Identifier(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(SYM[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::Identifier(int f){
    string msg = LABEL[ip];
    InsertNode(f, msg);
    if(!IsLetter(msg[0])){
        PrintErrorPos(msg);
        return false;
    }
    else{
        for(int i = 1; i < msg.size(); i ++){
            if(!IsLetter(msg[i]) && !IsNum(msg[i])){
                PrintErrorPos(msg);
                return false;
            }
        }
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::ProcedureDeclare(int f){
    int fatherID = curIndex;
    string msg = "PROCEDUREDECLARE";
    InsertNode(f, msg);
    if(!ProcedureHead(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(!SubProgram(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(SYM[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    while(ProcedureDeclare(fatherID)){

    }
    return true;
}

bool Syntax_Analyzer::ProcedureHead(int f){
    int fatherID = curIndex;
    string msg = "PROCEDUREHEAD";
    InsertNode(f, msg);
    if(SYM[ip] != "PROCEDURE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    if(!Identifier(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(SYM[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    return true;
}

bool Syntax_Analyzer::Sentence(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);
    if(Assignment(fatherID) || IfSentence(fatherID) || WhileSentence(fatherID) || CallSentence(fatherID)
        || ReadSentence(fatherID) || WriteSentence(fatherID) || Combined(fatherID) || Empty(fatherID)){
        return true;
    }
    return false;
}

bool Syntax_Analyzer::Assignment(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);
    if(!Identifier(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(SYM[ip] != ":="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    if(!Expression(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

bool Syntax_Analyzer::Combined(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);
    if(SYM[ip] != "BEGIN"){
        PrintErrorPos(msg);
        return false;
    }
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(SYM[ip] == ";"){
        ip ++;
        if(!Sentence(fatherID)){
            return false;
        }
    }
    if(SYM[ip] != "END"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::Condition(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);
    if(SYM[ip] == "ODD"){
        ip ++;
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else{
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(!RelationOp(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    return true;
}

bool Syntax_Analyzer::Expression(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);
    if(SYM[ip] == "+" || SYM[ip] == "-"){
        ip ++;
    }
    if(!Item(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(SYM[ip] == "+" || SYM[ip] == "-"){
        if(!AddorSubOp(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(!Item(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    return true;
}

bool Syntax_Analyzer::Item(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);
    if(!Factor(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(SYM[ip] == "*" || SYM[ip] == "/"){
        if(!MulorDivOp(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(!Factor(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    return true;
}

bool Syntax_Analyzer::Factor(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);
    bool flag = false;
    if(SYM[ip] == "("){
        ip ++;
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(SYM[ip] != ")"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            ip ++;
            return true;
        }
    }
    if(Identifier(fatherID)){
        return true;
    }
    if(UnsignedNum(fatherID)){
        return true;
    }
    return false;
}

bool Syntax_Analyzer::AddorSubOp(int f){
    int fatherID = curIndex;
    string msg = LABEL[ip];
    InsertNode(f, msg);
    if(SYM[ip] != "+" && SYM[ip] != "-"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::MulorDivOp(int f){
    int fatherID = curIndex;
    string msg = LABEL[ip];
    InsertNode(f, msg);
    if(SYM[ip] != "*" && SYM[ip] != "/"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::RelationOp(int f){
    int fatherID = curIndex;
    string msg = LABEL[ip];
    InsertNode(f, msg);
    if(SYM[ip] == "=" || SYM[ip] == "#" || SYM[ip] == "<"
    || SYM[ip] == "<=" || SYM[ip] == ">" || SYM[ip] == ">="){
        ip ++;
        return true;
    }
    else{
        PrintErrorPos(msg);
        return false;
    }
}

bool Syntax_Analyzer::IfSentence(int f){
    int fatherID = curIndex;
    string msg = "IFSENTENCE";
    InsertNode(f, msg);
    if(SYM[ip] == "IF"){
        ip ++;
        if(!Condition(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(SYM[ip] != "THEN"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            ip ++;
        }
        if(!Sentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        return true;
    }
    else{
        PrintErrorPos(msg);
        return false;
    }
}

bool Syntax_Analyzer::CallSentence(int f){
    int fatherID = curIndex;
    string msg = "CALLSENTENCE";
    InsertNode(f, msg);
    if(SYM[ip] != "CALL"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    if(!Identifier(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

bool Syntax_Analyzer::WhileSentence(int f){
    int fatherID = curIndex;
    string msg = "WHILESENTENCE";
    InsertNode(f, msg);
    if(SYM[ip] != "WHILE"){
        PrintErrorPos(msg);
        return false;
    }
    if(!Condition(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(SYM[ip] != "DO"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

bool Syntax_Analyzer::ReadSentence(int f){
    int fatherID = curIndex;
    string msg = "READSENTENCE";
    InsertNode(f, msg);

    if(SYM[ip] != "READ"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }

    if(SYM[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }

    if(!Identifier(fatherID)){
        PrintErrorPos(msg);
        return false;
    }

    while(SYM[ip] == ","){
        ip ++;
        if(!Identifier(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }

    if(SYM[ip] != ")"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }

    return true;
}

bool Syntax_Analyzer::WriteSentence(int f){
    int fatherID = curIndex;
    string msg = "WRITESENTENCE";
    InsertNode(f, msg);

    if(SYM[ip] != "WRITE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }

    if(SYM[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }

    if(!Identifier(fatherID)){
        PrintErrorPos(msg);
        return false;
    }

    while(SYM[ip] == ","){
        ip ++;
        if(!Identifier(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }

    if(SYM[ip] != ")"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        ip ++;
    }

    return true;
}

bool Syntax_Analyzer::Letter(int f){
    return IsLetter(SYM[ip][0]);
}

bool Syntax_Analyzer::Num(int f){
    return IsNum(SYM[ip][0]);
}

bool Syntax_Analyzer::Empty(int f){
    int fatherID = curIndex;
    string msg = "EMPTY";
    InsertNode(f, msg);
}