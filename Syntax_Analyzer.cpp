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

// Output the error information
void Syntax_Analyzer::PrintErrorPos(string msg){
    if(ip >= LABEL.size())
        cerr<< "Error: Out Of Range in " << msg << "!" <<endl;
    else
        cerr<< "Error: Syntax Error at position "<< ip <<" "<<LABEL[ip]<<" in "<< msg << "!"<<endl;
}

// Insert a node to the syntax tree
// int fatherID : the father's node id
// string label : the node name
void Syntax_Analyzer::InsertNode(int fatherID, string label){
    // add new node
    vector<int> tmp;
    tree.push_back(tmp);
    // add node name
    nodeInfo.push_back(label);
    // add link : father ----> this node
    tree[fatherID].push_back(curIndex);
    // node number ++
    curIndex ++;
}

// <程序>→<分程序>.
bool Syntax_Analyzer::Program(){

    string msg = "PROGRAM";

    // Insert root node to the syntax tree
    int fatherID = curIndex;
    vector<int> tmp;
    tree.push_back(tmp);
    nodeInfo.push_back("PROGRAM");
    curIndex ++;

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge <分程序>
    if(!SubProgram(fatherID)){
        PrintErrorPos(msg);
        return false;
    }

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge .
    if(LABEL[ip] == "." && ip == LABEL.size() - 1){
        InsertNode(fatherID, LABEL[ip]);
    }
    else{
        PrintErrorPos(msg);
        return false;
    }

    return true;

}

// <分程序>→ [<常量说明部分>][<变量说明部分>][<过程说明部分>]<语句>
bool Syntax_Analyzer::SubProgram(int f){
    string msg = "SUBPROG";

    // Insert current node to the syntax tree
    int fatherID = curIndex;
    InsertNode(f, msg);

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge [<常量说明部分>]
    if(LABEL[ip] == "CONST"){
        if(!ConstantDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // Judge [<变量说明部分>]
    if(LABEL[ip] == "VAR"){
        if(!VariableDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // Judge [<过程说明部分>]
    if(LABEL[ip] == "PROCEDURE"){
        procnt ++;
        if(!ProcedureDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // Judge <语句>
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

// <常量说明部分> → CONST<常量定义>{ ,<常量定义>};
bool Syntax_Analyzer::ConstantDeclare(int f){

    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CONSTANTDECLARE";
    InsertNode(f, msg);

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge CONST
    if(LABEL[ip] != "CONST"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // Judge <常量定义>
    if(!ConstantDefine(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // Judge { ,<常量定义>}
    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!ConstantDefine(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // Judge ;
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

// <常量定义> → <标识符>=<无符号整数>
bool Syntax_Analyzer::ConstantDefine(int f){

    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CONSTANTDEFINE";
    InsertNode(f, msg);

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // Judge =
    if(LABEL[ip] != "="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // Judge <无符号整数>
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

// <变量说明部分> → VAR<标识符>{ ,<标识符>};
bool Syntax_Analyzer::VariableDeclare(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "VARIABLEDECLARE";
    InsertNode(f, msg);

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // VAR
    if(LABEL[ip] != "VAR"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // { ,<标识符>}
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

    // ;
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

// <过程说明部分> → <过程首部><分程序>;{<过程说明部分>}
bool Syntax_Analyzer::ProcedureDeclare(int f){
    // Judge procedure declare level
    if(procnt >= 4){
        cerr<<"Error: Too Many Procedure Nesting Levels!"<<endl;
        return false;
    }
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "PROCEDUREDECLARE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // <过程首部>
    if(!ProcedureHead(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // <分程序>
    if(!SubProgram(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // ;
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // {<过程说明部分>}
    while(LABEL[ip] == "PROCEDURE"){
        if(!ProcedureDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // reset procnt
    procnt = 0;
    return true;
}

// <过程首部> → PROCEDURE <标识符>;
bool Syntax_Analyzer::ProcedureHead(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "PROCEDUREHEAD";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // PROCEDURE
    if(LABEL[ip] != "PROCEDURE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // ;
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

// <语句> → <赋值语句>|<条件语句>|<当型循环语句>|<过程调用语句>|<读语句>|<写语句>|<复合语句>|<空语句>

bool Syntax_Analyzer::Sentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // <赋值语句>
    if(SYM[ip] == "IDENT"){
        if(!Assignment(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <条件语句>
    else if(LABEL[ip] == "IF"){
        if(!IfSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <当型循环语句>
    else if(LABEL[ip] == "WHILE"){
        if(!WhileSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <过程调用语句>
    else if(LABEL[ip] == "CALL"){
        if(!CallSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <读语句>
    else if(LABEL[ip] == "READ"){
        if(!ReadSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <写语句>
    else if(LABEL[ip] == "WRITE"){
        if(!WriteSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <复合语句>
    else if(LABEL[ip] == "BEGIN"){
        if(!Combined(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <空语句>
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

// <赋值语句> → <标识符>:=<表达式>
bool Syntax_Analyzer::Assignment(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "ASSIGNMENT";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // :=
    if(LABEL[ip] != ":="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // <表达式>
    if(!Expression(fatherID)){
        PrintErrorPos(msg);
        return false;
    }

    return true;
}

// <复合语句> → BEGIN<语句>{ ;<语句>} END
bool Syntax_Analyzer::Combined(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "COMBINED";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // BEGIN
    if(LABEL[ip] != "BEGIN"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // <语句>
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }

    // { ;<语句>}
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

    // END
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

// <条件> → <表达式><关系运算符><表达式>|ODD<表达式>
bool Syntax_Analyzer::Condition(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CONDITION";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // ODD<表达式>
    if(LABEL[ip] == "ODD"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <表达式><关系运算符><表达式>
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

// <表达式> → [+|-]<项>{<加减运算符><项>}
bool Syntax_Analyzer::Expression(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "EXPRESSION";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // [+|-]
    if(LABEL[ip] == "+" || LABEL[ip] == "-"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <项>
    if(!Item(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // {<加减运算符><项>}
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

// <项> → <因子>{<乘除运算符><因子>}
bool Syntax_Analyzer::Item(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "ITEM";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // <因子>
    if(!Factor(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // {<乘除运算符><因子>}
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

// <因子> → <标识符>|<无符号整数>|(<表达式>)
bool Syntax_Analyzer::Factor(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "FACTOR";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // (<表达式>)
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

    // <标识符>
    if(SYM[ip] == "IDENT"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }

    // <无符号整数>
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

// <关系运算符> → =|#|<|<=|>|>=
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

// <条件语句> → IF<条件>THEN<语句>
bool Syntax_Analyzer::IfSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "IFSENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // IF
    if(LABEL[ip] == "IF"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        // <条件>
        if(!Condition(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        // THEN
        if(LABEL[ip] != "THEN"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
        // <语句>
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

// <过程调用语句> → CALL<标识符>
bool Syntax_Analyzer::CallSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CALLSENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // CALL
    if(LABEL[ip] != "CALL"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <标识符>
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

// <当型循环语句> → WHILE<条件>DO<语句>
bool Syntax_Analyzer::WhileSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "WHILESENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // WHILE
    if(LABEL[ip] != "WHILE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <条件>
    if(!Condition(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // DO
    if(LABEL[ip] != "DO"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <语句>
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    return true;
}

// <读语句> → READ(<标识符>{ ,<标识符>})
bool Syntax_Analyzer::ReadSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "READSENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // READ
    if(LABEL[ip] != "READ"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // (
    if(LABEL[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // { ,<标识符>}
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

    // )
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

// <写语句> → WRITE(<标识符>{,<标识符>})
bool Syntax_Analyzer::WriteSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "WRITESENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // WRITE
    if(LABEL[ip] != "WRITE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // (
    if(LABEL[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // {,<标识符>}
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
    // )
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

// <空语句> → epsilon
bool Syntax_Analyzer::EmptySentence(int f){
    //int fatherID = curIndex;
    //string msg1 = "SENTENCE";
    //InsertNode(f, msg1);
    string msg2 = "EMPTY";
    InsertNode(f, msg2);
    return true;
}

// Output the syntax tree using dfs
// int s : the source node's id
void Syntax_Analyzer::TreeOutput(int s){
    // transfer the node name for output
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
    // no child node, then return
    if(tree[s].size() == 0){
        return;
    }
    // continue dfs......
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

// Depth first search
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

// Transfer the output for debugging
// bool debug
//  = true  : output for debugging
//  = false : output for oj
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
