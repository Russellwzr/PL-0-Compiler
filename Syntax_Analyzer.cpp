#include "Syntax_Analyzer.h"
#include "utils.h"

Syntax_Analyzer::Syntax_Analyzer(){
    ip = 0;
    curIndex = 0;
    SYM.clear();
    LABEL.clear();
    tree.clear();
    nodeInfo.clear();
    tokens.clear();
    exp = "";
    ans = "";
    procnt = 0;
}

Syntax_Analyzer::Syntax_Analyzer(vector<string>& SS, vector<string>& LL){
    ip = 0;
    curIndex = 0;
    SYM = SS;
    LABEL = LL;
    tree.clear();
    nodeInfo.clear();
    tokens.clear();
    exp = "";
    ans = "";
    procnt = 0;
}

Syntax_Analyzer::~Syntax_Analyzer(){
    ip = 0;
    curIndex = 0;
    SYM.clear();
    LABEL.clear();
    tree.clear();
    nodeInfo.clear();
    tokens.clear();
    exp = "";
    ans = "";
    procnt = 0;
}

void Syntax_Analyzer::PrintErrorPos(string msg){
    if(ip >= LABEL.size())
        cerr<< "Error: Out Of Range in " << msg << "!" <<endl;
    else
        cerr<< "Error: Syntax Error at position "<< ip <<" "<<LABEL[ip]<<" in "<< msg << "!"<<endl;
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

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(!SubProgram(fatherID)){
        PrintErrorPos(msg);
        return false;
    }

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] == "." && ip == LABEL.size() - 1){
        InsertNode(fatherID, LABEL[ip]);
    }
    else{
        PrintErrorPos(msg);
        return false;
    }

    return true;

}

bool Syntax_Analyzer::SubProgram(int f){
    string msg = "SUBPROG";
    int fatherID = curIndex;
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] == "CONST"){
        if(!ConstantDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(LABEL[ip] == "VAR"){
        if(!VariableDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(LABEL[ip] == "PROCEDURE"){
        procnt ++;
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

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "CONST"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(!ConstantDefine(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!ConstantDefine(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::ConstantDefine(int f){
    int fatherID = curIndex;
    string msg = "CONSTANTDEFINE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    if(LABEL[ip] != "="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    if(SYM[ip] != "NUM"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    return true;

}

/*
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
*/

bool Syntax_Analyzer::VariableDeclare(int f){
    int fatherID = curIndex;
    string msg = "VARIABLEDECLARE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "VAR"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(SYM[ip] != "IDENT"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
    }

    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }

}

/*
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
*/

bool Syntax_Analyzer::ProcedureDeclare(int f){
    if(procnt >= 4){
        cerr<<"Error: Too Many Procedure Nesting Levels!"<<endl;
        return false;
    }

    int fatherID = curIndex;
    string msg = "PROCEDUREDECLARE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(!ProcedureHead(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(!SubProgram(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    while(LABEL[ip] == "PROCEDURE"){
        if(!ProcedureDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    procnt = 0;
    return true;
}

bool Syntax_Analyzer::ProcedureHead(int f){
    int fatherID = curIndex;
    string msg = "PROCEDUREHEAD";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "PROCEDURE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    return true;
}

bool Syntax_Analyzer::Sentence(int f){
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(SYM[ip] == "IDENT"){
        if(!Assignment(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else if(LABEL[ip] == "IF"){
        if(!IfSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else if(LABEL[ip] == "WHILE"){
        if(!WhileSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else if(LABEL[ip] == "CALL"){
        if(!CallSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else if(LABEL[ip] == "READ"){
        if(!ReadSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else if(LABEL[ip] == "WRITE"){
        if(!WriteSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else if(LABEL[ip] == "BEGIN"){
        if(!Combined(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    else{
        EmptySentence(fatherID);
        return true;
        /*
        if((LABEL[ip] == "END") || ip == LABEL.size() - 1){
            EmptySentence(fatherID);
        }
        else{
            PrintErrorPos(msg);
            return false;
        }
         */
    }

    /*
    if(Assignment(fatherID) || IfSentence(fatherID) || WhileSentence(fatherID) || CallSentence(fatherID)
        || ReadSentence(fatherID) || WriteSentence(fatherID) || Combined(fatherID) || Empty(fatherID)){
        return true;
    }
     */

    return true;
}

bool Syntax_Analyzer::Assignment(int f){
    int fatherID = curIndex;
    string msg = "ASSIGNMENT";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(LABEL[ip] != ":="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
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
    string msg = "COMBINED";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "BEGIN"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(LABEL[ip] == ";"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        //if(LABEL[ip] == "END"){
        //    EmptySentence(fatherID);
        //   break;
        //}
        if(!Sentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    if(LABEL[ip] != "END"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }
}

bool Syntax_Analyzer::Condition(int f){
    int fatherID = curIndex;
    string msg = "CONDITION";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] == "ODD"){
        InsertNode(fatherID, LABEL[ip]);
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
    string msg = "EXPRESSION";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] == "+" || LABEL[ip] == "-"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(!Item(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(LABEL[ip] == "+" || LABEL[ip] == "-"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Item(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    return true;
}

bool Syntax_Analyzer::Item(int f){
    int fatherID = curIndex;
    string msg = "ITEM";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(!Factor(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    while(LABEL[ip] == "*" || LABEL[ip] == "/"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Factor(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    return true;
}

bool Syntax_Analyzer::Factor(int f){
    int fatherID = curIndex;
    string msg = "FACTOR";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] == "("){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(LABEL[ip] != ")"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
            return true;
        }
    }

    if(SYM[ip] == "IDENT"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }

    if(SYM[ip] == "NUM"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }

    return false;
}

/*
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

 */

bool Syntax_Analyzer::RelationOp(int f){
    /*
    int fatherID = curIndex;

    InsertNode(f, msg);
     */
    string msg = LABEL[ip];
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] == "=" || LABEL[ip] == "#" || LABEL[ip] == "<"
    || LABEL[ip] == "<=" || LABEL[ip] == ">" || LABEL[ip] == ">="){
        InsertNode(f, LABEL[ip]);
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

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] == "IF"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Condition(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(LABEL[ip] != "THEN"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
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

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "CALL"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    return true;
}

bool Syntax_Analyzer::WhileSentence(int f){
    int fatherID = curIndex;
    string msg = "WHILESENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "WHILE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    if(!Condition(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(LABEL[ip] != "DO"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
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

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "READ"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    if(LABEL[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(SYM[ip] != "IDENT"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
    }

    if(LABEL[ip] != ")"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    return true;
}

bool Syntax_Analyzer::WriteSentence(int f){
    int fatherID = curIndex;
    string msg = "WRITESENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    if(LABEL[ip] != "WRITE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    if(LABEL[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(SYM[ip] != "IDENT"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
    }

    if(LABEL[ip] != ")"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    return true;
}

bool Syntax_Analyzer::EmptySentence(int f){
    //int fatherID = curIndex;
    //string msg1 = "SENTENCE";
    //InsertNode(f, msg1);
    string msg2 = "EMPTY";
    InsertNode(f, msg2);
    return true;
}

void Syntax_Analyzer::TreeOutput(int s){
    if(nodeInfo[s] == "("){
        exp += "LP";
    }
    else if(nodeInfo[s] == ")"){
        exp += "RP";
    }
    else if(nodeInfo[s] == ","){
        exp += "COMMA";
    }
    else{
        exp += nodeInfo[s];
    }
    if(tree[s].size() == 0){
        return;
    }
    else{
        exp += "(";
        for(int i = 0; i < tree[s].size(); i ++){
            TreeOutput(tree[s][i]);
            if(i != tree[s].size() -1)
                exp += ",";
        }
        exp += ")";
        return;
    }
}

void Syntax_Analyzer::dfs(int &index, int depth){
    again:
    ans += string(depth, '\t');
    ans += tokens[index];
    index++;
    if (index < tokens.size() && tokens[index] == "(")
    {
        ans += "(\n";
        index++;
        dfs(index, depth + 1);
        if (tokens[index] != ")")throw 233;
        ans += string(depth,'\t') +")";
        index++;
    }
    if (index < tokens.size() && tokens[index] == ",")
    {
        ans += ",\n";
        index++;
        goto again;
    }
    ans += "\n";
}

void Syntax_Analyzer::TransferOutput(bool debug){
    TreeOutput(0);
    if(!debug){
        cout << exp << endl;
    }
    else{
        string tmp;
        for (int i = 0; i < exp.length(); i++)
        {
            if (exp[i] == '(' || exp[i] == ')' || exp[i] == ',')
            {
                if(!tmp.empty())tokens.push_back(tmp);
                tmp = "";
                tokens.push_back(string(1, exp[i]));
            }
            else tmp += exp[i];
        }
        if (!tmp.empty())tokens.push_back(tmp);
        try
        {
            int id = 0;
            dfs(id, 0);
        }
        catch (...)
        {
            cerr << "Error Parsing Expression" << endl;
            return;
        }
        cout << ans << endl;
    }
}
