#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include<utility>
using namespace std ;
string MARK = "_" ;
int Number_OR_OPERATION = 3 ; 
typedef enum _Operation{
    AND,
    OR,
    NOT, 

    NOP,
    BEG,
    END,
    N_A 
}Operation;
typedef enum _Algorithm{
    Heuristic,
    FORMULATED_ILP,
    BRUTE_ILP,
    Unspecified 
}Algorithm;

string OP2STR(Operation op){
    switch (op){
        case AND:
            return "AND" ;
        case OR:
            return "OR" ;
        case NOT:
            return "NOT" ;
        case NOP:
            return "NOP" ;
        case END:
            return "END" ;
        case BEG:
            return "BEG" ;
        case N_A:
            return "N_A" ;
        default:
            throw invalid_argument("字串轉換 輸入錯誤Opertaion") ;
    }
}
int OP2INT(Operation op){
    switch (op){
    case AND :
        return 0 ;
    case OR :
        return 1 ;
    case NOT :
        return 2 ;
    default:    
        throw invalid_argument("數字轉換 輸入錯誤Opertaion") ;
    }
}
string ALG2STR(Algorithm op){
    switch (op){
        case Heuristic :
            return "Heuristic" ;
        case FORMULATED_ILP :
            return "ILP Formulation" ;
        case BRUTE_ILP :
            return "ILP Directilly" ;
        case Unspecified :
            return "Unspecified" ;
        default:
            throw invalid_argument("字串轉換 輸入錯誤Opertaion") ;
    }
}
string BOOL2STR(bool b){
    if(b) return "True" ;
    return "False" ;
}
string VEC2STR(vector<int> vec, string sp=" "){
    string str ;
    for (auto it = vec.begin(); it != vec.end()-1; it++) {
        str.append(to_string(*it) + sp) ;
    }
    str.append(to_string(vec.back())) ;
    return str ;
}

typedef class Node{
    protected :
        string name = "N_A";
    public :
        Node() {} 
        Node(string name){
            this->name = name ;
        }
        string getName(){
            return  this->name ;
        }
        
} Node;
typedef class BoolenNode : public Node{
        
        
    public :
        int minLatency = 0 ;
        int maxLatency = 0 ;
        vector<int> label = {} ;
        int passed = 0 ;
        Operation type = N_A;

        BoolenNode() {}
        BoolenNode(string name,Operation type):Node(name){
            this->type = type ;
        }
        void setLatency(int minimum, int maxnimum){
            if(minimum<0) throw invalid_argument("Node " + this->name + " Input Nagative Minimum Latency\n") ;
            else this->minLatency = minimum ;

            if(maxnimum<0) throw invalid_argument("Node " + this->name + " Input Nagative Maxnimum Latency\n") ;
            else this->maxLatency = maxLatency ;
        }
        void Show(){
            cout << this->name << " " << OP2STR(this->type);
        }
} BoolenNode;
typedef class BoolenVertex : public BoolenNode{
    public :
        vector<BoolenVertex*> successor ;
        vector<BoolenVertex*> predecessor ;

        BoolenVertex() {}
        BoolenVertex(string name,Operation type):BoolenNode(name, type){} 
            
        void ShowSuccessor(){
            cout << "Successor of Node "<< getName() << " : " ; 
            for(auto &it : successor)
                cout << it->getName() << " " ;
            cout << "\n" ;
        }

        void ShowPredecessor(){
            cout << "Predecessor of Node "<< getName() << " : " ; 
            for(auto &it : predecessor)
                cout << it->getName() << " " ;
            cout << "\n" ;
        }
        void AddSuccessor(BoolenVertex *other){
            this->successor.push_back(other) ;
            other->predecessor.push_back(this) ;
        } 
        void AddPredecessor(BoolenVertex *other){
            this->predecessor.push_back(other) ;
            other->successor.push_back(this) ;
        }
        void ShowRelation(){
            cout << "Successor of " + name + " : " ;
            for(auto &it : successor){
                cout << " " ;
                it->Show() ;
                cout << " " ;
            }
            cout << "\nPredecessor of " + name + " : " ;
            for(auto &it : predecessor){
                cout << " " ;
                it->Show() ;
                cout << " " ;
            }
            cout << "\n" ;
        }

} BoolenVertex;

namespace TextProcessing{
    vector<string> Split(string& str, string pattern) {
        vector<string> result;
        size_t begin, end;

        end = str.find(pattern);
        begin = 0;

        while (end != std::string::npos) {
            if (end - begin != 0) {
                result.push_back(str.substr(begin, end-begin)); 
            }    
            begin = end + pattern.size();
            end = str.find(pattern, begin);
        }

        if (begin != str.length()) {
            result.push_back(str.substr(begin));
        }
        return result;        
    }
    
    bool Strip(string s, string pattern){
        
        if(s.empty()) return false ;
        s.erase(0,s.find_first_not_of(" ")) ;

        if(s.empty()) return false ;
        s.erase(s.find_last_not_of(" ")+1) ;

        return true ;
    }

}
class BoolenGraph{
    private :
        string graphName = "N_A" ;
        int readLineCounter = 0; 
        bool modelModule = false ;
        bool inputsModule = false ;
        bool outputsModule = false ;
        bool namesModule = false ;
        bool endModule = false ;
        void parse_init(){
            graphName = "N_A" ;
            readLineCounter = 0; 
            modelModule = false ;
            inputsModule = false ;
            outputsModule = false ;
            namesModule = false ;
            endModule = false ;
            graphBody.clear() ;
            graphEnd = { {"EOM_PI", new BoolenVertex("EOM_PI",END)} } ; 
        }
        
        bool NotParse(string &str, int size){
            if((int)str.size()!=size) invalid_argument( "第 " + to_string(readLineCounter) + " 行 .names 邏輯錯誤\n") ;
            for(int i=0; i<str.size();i++) 
                if(str[i]!='0')
                    return false ;
            return true ;
        }
        bool AndParse(string &str, int size){
            if((int)str.size()!=size) invalid_argument( "第 " + to_string(readLineCounter) + " 行 .names 邏輯錯誤\n") ;
            for(int i=0; i<str.size();i++)
                if(str[i]!='1')
                    return false ;

            return true;
        }
        bool OrParse(string &str, int size){
            if((int)str.size()!=size) invalid_argument( "第 " + to_string(readLineCounter) + " 行 .names 邏輯錯誤\n") ;
            bool firstOne = false ;
            for(int i=0; i<str.size();i++){
                if(str[i]=='1' && !firstOne){
                    firstOne = true ;
                }else if(str[i]=='-'){
                    continue ; 
                }else{
                    return false ;
                }
            }
            return true ;
        }
        int ReadLine(ifstream &BlifFile, string &BlifText, int Code){
            if(!getline(BlifFile, BlifText)) return 0 ;
            readLineCounter ++ ;
            if(BlifText.empty()) return 1 ; 
            return 2 ;
        }
    public :
        map<string, BoolenVertex*> graphBody ; //Don't include Input Node 
        map<string, BoolenVertex*> graphEnd = { {"EOM_PI", new BoolenVertex("EOM_PI",END)} } ; //Input & END
        vector<int> demand = {0,0,0} ;
        BoolenGraph() {}

        void AddVertex(string key, BoolenVertex *newVertex, bool isBody=true){
            if(isBody){
                if(graphBody.find(key)==graphBody.end()) this->graphBody[key] = newVertex ;
                else throw invalid_argument( "第 " + to_string(readLineCounter) + " 行 節點" + key + " 重複定義\n") ;
            }else{
                if(graphEnd.find(key)==graphEnd.end()) this->graphEnd[key] = newVertex ;
                else throw invalid_argument( "第 " + to_string(readLineCounter) + " 行 節點" + key + " 重複定義\n") ;
            }
        }
        void ShowDemand(){
            for(auto &it : vector<Operation>{AND,OR,NOT} ) 
                cout << OP2STR(it) << " : " << demand[OP2INT(it)] << " " ;
            cout << "\n" ;
        }
        void CreateGraph(string FileName, vector<int>Constrain){
            ifstream BlifFile(FileName) ;
            string BlifText ;
            vector<string> SplitedText ;

            vector<string> TemSplitedText ; 
            int InputLength ;
            Operation temType ; 

            bool ContinuedFlag = false ;
            parse_init() ;
            BoolenVertex* k ;
            while (ReadLine(BlifFile, BlifText,0)) {
                
                SplitedText = TextProcessing::Split(BlifText," ") ;
                if(SplitedText.empty()) continue ; 

                if(SplitedText[0]==".model"){
                    if(graphName!="N_A") throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .model重複定義\n") ;
                    else if(SplitedText.size()!=2) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .model輸入非法輸入\n") ;
                    else graphName = SplitedText[1] ;
                    modelModule = true ;
                }else if(SplitedText[0]==".inputs"){
                    if(SplitedText.size()==1) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .inputs無輸入節點名稱\n") ;
                    else{
                        for(int i = 1 ; i<SplitedText.size(); i++){
                            if(SplitedText[i]=="\\") {
                                if(ReadLine(BlifFile, BlifText,0)!=2) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .inputs無輸入節點名稱\n") ;
                                SplitedText = TextProcessing::Split(BlifText," ") ;
                                i = 0 ; 
                            }
                            AddVertex(SplitedText[i],new BoolenVertex(SplitedText[i],BEG),false) ;
                        }
                        inputsModule = true ; 
                    }
                }else if(SplitedText[0]==".outputs"){
                    if(SplitedText.size()==1) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .outputs無輸入節點名稱\n") ;
                    else{
                        for(int i = 1 ; i<SplitedText.size(); i++){
                            if(SplitedText[i]=="\\") {
                                if(ReadLine(BlifFile, BlifText,0)!=2) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .outputs無輸入節點名稱\n") ;
                                SplitedText = TextProcessing::Split(BlifText," ") ;
                                i = 0 ; 
                            }
                            AddVertex(SplitedText[i],new BoolenVertex(SplitedText[i],N_A),true) ;
                            graphEnd["EOM_PI"]->AddPredecessor(graphBody[SplitedText[i]]) ;
                        }
                        outputsModule = true ; 
                    }
                }else if(SplitedText[0]==".names"){
                    if(SplitedText.size()<=2) throw invalid_argument( "第 " + to_string(readLineCounter) + " 行 .names無輸入節點名稱\n") ;
                    else{
                        InputLength = (int)SplitedText.size()-2 ;
                        TemSplitedText = SplitedText ;
                        if(ReadLine(BlifFile, BlifText,0)!=2) throw invalid_argument( "第 " + to_string(readLineCounter) + " 行 .names 輸入邏輯錯誤\n") ;
                        SplitedText = TextProcessing::Split(BlifText," ") ;
                        temType = N_A ;

                        if(NotParse(SplitedText[0],InputLength)){
                            temType = NOT ; 
                        }else if(AndParse(SplitedText[0],InputLength)){
                            temType = AND ; 
                        }else{
                            for(int i=1; i<InputLength;i++){
                                if(!OrParse(SplitedText[0],InputLength)) throw invalid_argument( "第 " + to_string(readLineCounter) + " 行 .names輸入邏輯錯誤\n") ;
                                if(ReadLine(BlifFile, BlifText,0)!=2) throw invalid_argument( "第 " + to_string(readLineCounter) + " 行 .names輸入邏輯錯誤\n") ;
                                SplitedText = TextProcessing::Split(BlifText," ") ;
                                if(SplitedText.size()!=2) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .names輸入邏輯錯誤\n") ;
                            }
                            temType = OR ;
                        }

                        if(graphBody.find(TemSplitedText.back())!=graphBody.end()){
                            if(graphBody[TemSplitedText.back()]->type != N_A) 
                                throw invalid_argument( "第 " + to_string(readLineCounter) + " 行" + TemSplitedText.back() + " 重複定義\n") ;
                            else{
                                graphBody[TemSplitedText.back()]->type = temType ;
                                for(int j=1; j<TemSplitedText.size()-1;j++)
                                    if(graphEnd.find(TemSplitedText[j]) != graphEnd.end())
                                        graphEnd[TemSplitedText[j]]->AddSuccessor(graphBody[TemSplitedText.back()]) ;
                                    else if(graphBody.find(TemSplitedText[j]) != graphBody.end())
                                        graphBody[TemSplitedText[j]]->AddSuccessor(graphBody[TemSplitedText.back()]) ;
                                    else 
                                        throw invalid_argument( "第 " + to_string(readLineCounter) + " 行" + TemSplitedText[j] + " 未被定義\n") ;

                            }
                        }else{
                            AddVertex(TemSplitedText.back(),new BoolenVertex(TemSplitedText.back(),temType),true) ;
                            
                            for(int j=1; j<TemSplitedText.size()-1;j++)
                                if(graphEnd.find(TemSplitedText[j]) != graphEnd.end())
                                    graphEnd[TemSplitedText[j]]->AddSuccessor(graphBody[TemSplitedText.back()]) ;
                                else if(graphBody.find(TemSplitedText[j]) != graphBody.end())
                                    graphBody[TemSplitedText[j]]->AddSuccessor(graphBody[TemSplitedText.back()]) ;
                                else 
                                    throw invalid_argument( "第 " + to_string(readLineCounter) + " 行" + TemSplitedText[j] + " 未被定義\n") ;

                        }
                    }
                    
                }else if(SplitedText[0]==".end"){
                    endModule = true ;
                    break ;
                }else{
                    //throw invalid_argument( "第 " + to_string(readLineCounter) + "行 輸入無效參數\n") ;
                }
            }
            for(auto &it : graphBody){
                demand[OP2INT(it.second->type)] ++ ;
            }
            //for(auto &it : graphBody) it.second->ShowRelation() ;
            //for(auto &it : graphEnd) it.second->ShowRelation() ;

        }
        void GraphInfo(bool type=false, bool label=false, bool minLatency=false, bool maxLatency=false){
            for(auto &it : graphBody){
                cout << "{" ;
                cout << it.second->getName() << " ";
                if(type) cout << OP2STR(it.second->type) << " ";
                if(label) for(auto &it2 : it.second->label) cout << it2 << " ";
                if(minLatency) cout << it.second->minLatency << " ";
                if(maxLatency) cout << it.second->maxLatency << " ";
                cout << "}" ;
            }
            cout << "\n" ;
        }
        
        void GraphPassedInit(int code=0){
            switch (code){
                case 0:
                    for(auto &it : graphEnd) it.second->passed = it.second->predecessor.size() ;
                    for(auto &it : graphBody) it.second->passed = it.second->predecessor.size() ;
                    break;
                case 1:
                    for(auto &it : graphEnd) it.second->passed = it.second->successor.size() ;
                    for(auto &it : graphBody) it.second->passed = it.second->successor.size() ;
                    break;
            }
        }
        void GraphLabelInit(int code=0){
            switch (code){
                case 0://廣度
                    for(auto &it : graphEnd) it.second->label.push_back(0) ;
                    for(auto &it : graphBody) it.second->label.push_back(0) ;
                    for(auto &it : graphEnd) 
                        for(auto &it2 : it.second->predecessor)
                            it2->label.back()++ ;
                    for(auto &it : graphBody) 
                        for(auto &it2 : it.second->predecessor)
                            it2->label.back() ++ ;
                    break;
                case 1://深度
                    for(auto &it : graphEnd) it.second->label.push_back(-it.second->maxLatency) ;
                    for(auto &it : graphBody) it.second->label.push_back(-it.second->maxLatency) ;
                    break;
                case 2 :
                    for(auto &it : graphEnd) it.second->label.push_back(0) ;
                    for(auto &it : graphBody) it.second->label.push_back(0) ;
                    break;
                case 3 :
                    for(auto &it : graphEnd) it.second->label.clear() ;
                    for(auto &it : graphBody)it.second->label.clear() ;
                    break;
            }
        }
} ;
class CMDParser{
    public :
        bool isDebug = false ;
        bool isSilent = false ; 
        bool removeMetaFile = false ;
        Algorithm  useAlgorithm = Unspecified ;
        string blifFile ;
        vector<int> constrain ;
        CMDParser() {} ;
        void Parse(int argc, char* argv[]){
            string arg ;
            for(int i=1; i<argc; i++){
                arg = string(argv[i]) ;

                if(arg=="-h"){
                    useAlgorithm = Heuristic ;
                }else if(arg=="-e"){
                    useAlgorithm = FORMULATED_ILP ;
                }else if(arg=="-b"){
                    useAlgorithm = BRUTE_ILP ;
                }else if(arg=="-s"){
                    isSilent = true ;
                }else if(arg=="-c"){
                    removeMetaFile = true ;
                }else if(arg=="-debug"){
                    isDebug = true ;
                }else{
                    if(arg.substr(max((int)arg.size()-5,0),arg.size())==".blif"){
                        blifFile = arg ;
                    }else{
                        if(!blifFile.empty()){
                            if(constrain.size()!=Number_OR_OPERATION){
                                for(auto &j : vector<Operation>{AND, OR, NOT}){
                                    try{
                                        constrain.push_back(stoi(string(argv[i]))) ;
                                        i ++ ;
                                    }catch (invalid_argument){
                                        switch (j){
                                            case AND:
                                                throw invalid_argument("錯誤參數 " + string(argv[i]) + " 請輸入AND資源的限制\n") ;
                                            case OR:
                                                throw invalid_argument("錯誤參數 " + string(argv[i]) + " 請輸入OR資源的限制\n") ;
                                            case NOT:
                                                throw invalid_argument("錯誤參數 " + string(argv[i]) + " 請輸入NOT資源的限制\n") ;
                                        }
                                    } catch (logic_error) {
                                        throw invalid_argument("未輸入" + to_string(Number_OR_OPERATION) + "種資源限制\n") ;
                                    }
                                }
                                i--;
                            }else{
                                throw invalid_argument( "輸入超過" + to_string(Number_OR_OPERATION) +"種資源限制\n");
                            }
                        }else{
                            throw invalid_argument( "參數 " + arg + " 錯誤 請輸入正確參數\n");
                        }
                    }
                }
            }
            if(constrain.size()!=Number_OR_OPERATION) throw invalid_argument( "資源限制輸入不全\n") ;
            if(blifFile.empty()) throw invalid_argument( "未指定BLIF檔案\n") ;

        }
        void ShowState(void){
            cout << "Algorithm : " << ALG2STR(useAlgorithm) << '\n';
            cout << "Read File : " << blifFile << '\n';
            cout << "Resource Constrain : " << VEC2STR(constrain) << '\n';
            cout << "Silent Mode : " << BOOL2STR(isSilent) << '\n';
            cout << "Remove Meta File : " << BOOL2STR(removeMetaFile) << '\n';
        }
} ;

inline bool LabelCompare(BoolenVertex* a, BoolenVertex* b){ //大->小
    return (a->label[0] < b->label[0]) | ((a->label[0] == b->label[0]) && (a->label[1] < b->label[1])); 
}
inline bool StartTimeCompare(BoolenVertex* a, BoolenVertex* b) {
    return a->maxLatency < b->maxLatency; 
}
class SchedulingAlgorithm{
    private :
        ofstream fp ;
        vector<string> ParmeterName ;
        vector<int> ParmeterOrder ;
        void UniqueStartTimeWritter(string name, int startTime, int finishTime){
            for(int i=startTime; i<=finishTime; i++){
                fp << name << MARK + to_string(i) ;
                if(i<finishTime) fp << " + ";
            }
            fp << " = 1 \n";
        }
        void ResourceConstraintWritter(vector<string> name,int time,int constrain){
            for(int i=0; i<name.size(); i++){
                fp << name[i] << MARK + to_string(time) ;
                if(i<name.size()-1) fp << " + ";
            }
            fp << " <= "<<constrain << '\n';
        }
        void DependencyWritter(string name1, string name2, int startTime1, int finishTime1, int startTime2, int finishTime2){
            for(int i=startTime1; i<=finishTime1; i++){
                fp << to_string(i) << " " << name1 << MARK + to_string(i) ;
                if(i<finishTime1) fp << " + ";
            }
            for(int i=startTime2; i<=finishTime2; i++){
                if(i<=finishTime2) fp << " - " ;
                fp << to_string(i) << " " << name2 << MARK + to_string(i)  ;
               
            }
            fp << " >= 1 \n";
        }
        void MinimizeFunctionWritter(string name, int minTime, int maxTime){
            for(int i=minTime; i<=maxTime; i++){
                fp << to_string(i) << " " << name << MARK + to_string(i) ;
                if(i<maxTime) fp << " + " ;
            }
            fp << "\n" ;
        }
        void ParameterSaver(string name, int startTime, int endTime){
            for(int i=startTime; i<=endTime;i++){
                ParmeterName.push_back(name) ;
                ParmeterOrder.push_back(i) ;
            }
        }
        int SOLReader(map<string, BoolenVertex*>&graphBody,map<string, BoolenVertex*>&graphEnd,bool ShowResult=false){
                            
            string FileName = ".ILPsol.sol" ;
            ifstream SolFile(FileName) ;
            string SolText ;
            vector<string> SplitedText ;
            vector<vector<vector<string>>> TimeFrame ;
            Operation type ;
            int idx = -1;
            int minLatency = 999999 ;
            
            while (getline(SolFile, SolText)) {
                switch (SolText[0]){
                    case '#':
                        SplitedText = TextProcessing::Split(SolText, " ") ;
                        minLatency = stoi(SplitedText.back())-1 ;
                        TimeFrame = vector<vector<vector<string>>>(3,vector<vector<string>>(minLatency));
                        break;
                    default :
                        SplitedText = TextProcessing::Split(SolText, " ") ;
                        //cout <<SolText << " " << ParmeterName[idx] << ' ' <<  ParmeterOrder[idx] << " " << SplitedText[1]<<"\n";
                        idx ++ ;
                        if(SplitedText[1]!="0"){
                            if(ParmeterName[idx]=="EOM_PI") continue; 
                            //cout << ParmeterName[idx];
                            type = graphBody[ParmeterName[idx]]->type ; 
                            //TimeFrame[OP2INT(type)][ParmeterOrder[idx]-1].push_back("wdfw");
                            TimeFrame[OP2INT(type)][ParmeterOrder[idx]-1].push_back(ParmeterName[idx]);
                        }
                        break;
                }
            }
            if(ShowResult){
                for(int i=0;i<TimeFrame[0].size();i++){
                    
                    printf("%4d : ",i+1) ;
                    for(int j=0; j<TimeFrame.size();j++){
                        cout << "{" ;
                        for(int k=0; k<TimeFrame[j][i].size();k++){
                            cout << TimeFrame[j][i][k] ;
                            if(k<TimeFrame[j][i].size()-1) cout << " " ;
                        }
                        cout << "}" ;
                    }
                    cout << "\n" ;
                }
            }

            return minLatency ;
        }
    public :
        SchedulingAlgorithm() {}
        
        void EcecuteCheck(BoolenGraph& graph ,CMDParser &config){
            for(auto& it: graph.graphEnd){
                if(it.second->type==BEG){
                    if(it.second->predecessor.size()!=0)
                        throw invalid_argument( "無效輸入 " + it.first + " \n") ;
                }else if(it.second->type==END){
                    if(it.second->predecessor.size()==0)
                        throw invalid_argument( "缺少輸出\n") ;
                    if(it.second->successor.size()!=0)
                        throw invalid_argument( "無效輸出\n") ;
                }else{
                    throw invalid_argument( "輸出入定義錯誤 " + it.first + " \n") ;
                }
            }
            for(auto& it:graph.graphBody){
                if(it.second->type!=AND && it.second->type!=OR && it.second->type!=NOT) 
                    throw invalid_argument( "過程定義錯誤 " + it.first + " \n") ;
            }
            for(Operation &i : vector<Operation>{AND, OR, NOT}){
                if(graph.demand[OP2INT(i)]!=0 && config.constrain[OP2INT(i)]==0)
                    throw invalid_argument( "邏輯 " + OP2STR(i) + " 限制為0 但此任務須要" +  OP2STR(i) + " 無法完成任務\n") ;
            }
            if(config.useAlgorithm==Unspecified){
                    throw invalid_argument("請指定演算法\n-h Heuristic Algorithm\n-e ILP formulation\n") ;
            }
        }

        int ASAP(map<string, BoolenVertex*>&graphBody,map<string, BoolenVertex*>&graphEnd,bool ShowResult=false){
            
            int latency = 1 ;
            bool completed = false ;
            vector<vector<BoolenVertex*>> AvailableQueue(3,vector<BoolenVertex*>()) ;
            vector<vector<BoolenVertex*>> ReadyQueue(3,vector<BoolenVertex*>()) ; 
            
            for(auto &it : graphEnd){
                if(it.second->type ==END) continue;
                it.second->minLatency = 0 ;
                for(auto &it2 : it.second->successor){
                    if(--it2->passed <= 0){
                        it2->minLatency = latency ;
                        if(it2->type==NOP) completed=true ; 
                        else AvailableQueue[OP2INT(it2->type)].push_back(it2) ;
                    }
                }
            }
            while(!completed){
                if(ShowResult) printf("%4d : ",latency) ;
                latency++;

                for(int i=0; i<AvailableQueue.size();i++){
                    ReadyQueue[i] = AvailableQueue[i] ;
                    AvailableQueue[i].clear() ;
                    if(ShowResult){
                        cout << "{" ;
                        for(int j=0; j<(int)ReadyQueue[i].size();j++){
                            cout << ReadyQueue[i][j]->getName() ;
                            if(j<(int)ReadyQueue[i].size()-1) cout << " " ;
                        }
                        cout << "}" ;
                    } 
                }

                for(int i=0; i<ReadyQueue.size();i++){
                    for(int j=0; j<ReadyQueue[i].size(); j++){
                        for(int k=0; k<ReadyQueue[i][j]->successor.size(); k++){
                            if(--ReadyQueue[i][j]->successor[k]->passed<=0){
                                ReadyQueue[i][j]->successor[k]->minLatency = latency ;
                                if(ReadyQueue[i][j]->successor[k]->type==END) completed=true ;
                                else AvailableQueue[OP2INT(ReadyQueue[i][j]->successor[k]->type)].push_back(ReadyQueue[i][j]->successor[k]) ;
                            }
                        }
                    }
                }
                if(ShowResult) cout << '\n';
            }
            latency -- ;

            return latency ;
        }
        int ALAP(map<string, BoolenVertex*>&graphBody,map<string, BoolenVertex*>&graphEnd,int timeConstrain,bool ShowResult=false){
                int latency = timeConstrain ;
                int Completed = 0 ;

                vector<vector<BoolenVertex*>> AvailableQueue(3,vector<BoolenVertex*>());
                vector<vector<BoolenVertex*>> ReadyQueue(3,vector<BoolenVertex*>()) ; 

                graphEnd["EOM_PI"]->maxLatency = latency+1 ;
                
                for(auto &it : graphEnd) 
                    if(it.second->type==BEG && !it.second->successor.empty()) Completed ++ ;
                for(auto &it2 : graphEnd["EOM_PI"]->predecessor){
                    if(--it2->passed <= 0){
                        it2->maxLatency = latency ;
                        if(it2->type==BEG) Completed-- ;
                        else AvailableQueue[OP2INT(it2->type)].push_back(it2) ;
                    }
                }
                
                while(Completed>0){
                    if(ShowResult) printf("%4d : ",latency) ;
                    latency -- ;

                    for(int i=0; i<AvailableQueue.size();i++){
                        ReadyQueue[i] = AvailableQueue[i] ;
                        AvailableQueue[i].clear() ;
                        if(ShowResult){
                            cout << "{" ;
                            for(int j=0; j<(int)ReadyQueue[i].size();j++){
                                cout << ReadyQueue[i][j]->getName() ;
                                if(j<(int)ReadyQueue[i].size()-1) cout << " " ;
                            }
                            cout << "}" ;
                        }
                    }
                    for(int i=0; i<ReadyQueue.size();i++){  
                        for(int j=0; j<ReadyQueue[i].size(); j++){
                            for(int k=0; k<ReadyQueue[i][j]->predecessor.size(); k++){
                                if(--ReadyQueue[i][j]->predecessor[k]->passed<=0){
                                    ReadyQueue[i][j]->predecessor[k]->maxLatency = latency ;
                                    if(ReadyQueue[i][j]->predecessor[k]->type==BEG) Completed-- ;
                                    else AvailableQueue[OP2INT(ReadyQueue[i][j]->predecessor[k]->type)].push_back(ReadyQueue[i][j]->predecessor[k]) ;
                                }
                            }
                        }
                    }
                    if(ShowResult) cout << '\n';
                }
                latency++ ;

                return latency ;
        }
        int HeuristicAlgorithm(map<string, BoolenVertex*>&graphBody,map<string, BoolenVertex*>&graphEnd,vector<int>ResourceConstrain, bool ShowResult=false){
            int latency = 1 ;
            bool Completed = false ;
            vector<vector<BoolenVertex*>> AvailableQueue(3,vector<BoolenVertex*>()) ;
            vector<vector<BoolenVertex*>> ReadyQueue(3,vector<BoolenVertex*>()) ;
            
            for(auto &it : graphEnd){
                for(auto &it2 : it.second->successor){
                    if(--it2->passed <= 0){
                        if(it2->type==END){
                            Completed = true ; 
                        }else{
                            AvailableQueue[OP2INT(it2->type)].push_back(it2) ;
                            push_heap(AvailableQueue[OP2INT(it2->type)].begin(),AvailableQueue[OP2INT(it2->type)].end(),LabelCompare) ;
                        }
                    }
                }
            }

            //設定排序法
            while(!Completed){
                if(ShowResult) printf("%4d : ",latency) ;
                latency ++ ;
                for(int i=0; i<AvailableQueue.size();i++){
                    ReadyQueue[i].clear() ;
                    if(ShowResult) cout << "{" ;
                    for(int j=min(ResourceConstrain[i],(int)AvailableQueue[i].size()); j>0; j--){
                        ReadyQueue[i].push_back(AvailableQueue[i][0]) ;
                        
                        pop_heap(AvailableQueue[i].begin(), AvailableQueue[i].end(),LabelCompare) ;
                        AvailableQueue[i].pop_back() ;
                        
                        if(ShowResult)  cout << ReadyQueue[i].back()->getName() ;
                        if(ShowResult && j>1) cout << " " ;
                    }
                    if(ShowResult) cout << "}" ;
                }

                for(int i=0; i<ReadyQueue.size();i++){
                    for(int j=0; j<ReadyQueue[i].size(); j++){
                        for(int k=0; k<ReadyQueue[i][j]->successor.size(); k++){
                            if(--ReadyQueue[i][j]->successor[k]->passed<=0){
                                if(ReadyQueue[i][j]->successor[k]->type==END){
                                    Completed = true ;
                                }else{
                                    AvailableQueue[ReadyQueue[i][j]->successor[k]->type].push_back(ReadyQueue[i][j]->successor[k]) ;
                                    push_heap(AvailableQueue[ReadyQueue[i][j]->successor[k]->type].begin(), AvailableQueue[ReadyQueue[i][j]->successor[k]->type].end(),LabelCompare) ;
                                }
                            }
                        }
                    }
                }
                if(ShowResult) cout << '\n';
            }
            latency -- ;
            return latency ;
        }
        
        int ILPFormulation(map<string, BoolenVertex*>&graphBody,map<string, BoolenVertex*>&graphEnd,vector<int>ResourceConstrain, 
                            vector<int>TimeConstrain, bool ShowResult=false){
            ParmeterName.clear() ;
            ParmeterOrder.clear() ;
            string SaveName = ".formulation.lp" ;
            fp.open(SaveName) ;
            
            vector<vector<BoolenVertex*>>SortedNode(3,vector<BoolenVertex*>());
            vector<int> Base = {0,0,0} ;

            //0. min function 
            fp<<"Min\n" ;
            MinimizeFunctionWritter("EOM_PI",TimeConstrain[0]+1,TimeConstrain[1]+1) ;
            ParameterSaver("EOM_PI",TimeConstrain[0]+1,TimeConstrain[1]+1) ;

            //1. Unique start times
            fp<<"Subject To\n" ;
            UniqueStartTimeWritter("EOM_PI",TimeConstrain[0]+1,TimeConstrain[1]+1) ;
            for(auto &it : graphBody){
                UniqueStartTimeWritter(it.second->getName(),it.second->minLatency,it.second->maxLatency) ;
                ParameterSaver(it.second->getName(),it.second->minLatency,it.second->maxLatency) ;
                SortedNode[OP2INT(it.second->type)].push_back(it.second) ;
            }
            //2. Dependency Constrain
            for(auto &it : graphBody){
                for(auto &dep : it.second->predecessor)
                    if(dep->type!=BEG)
                        DependencyWritter(it.second->getName(), dep->getName(), \
                                            it.second->minLatency, it.second->maxLatency, \
                                            dep->minLatency, dep->maxLatency) ;
            }
            for(auto &dep : graphEnd["EOM_PI"]->predecessor)
                if(dep->type!=BEG)
                    DependencyWritter(graphEnd["EOM_PI"]->getName(), dep->getName(), \
                                        TimeConstrain[0]+1, TimeConstrain[1]+1, \
                                        dep->minLatency, dep->maxLatency) ;

            //3. Resource constraints in every time slice
            vector<string> ResourceName ; 
            for(int i=0; i<3; i++)  sort(SortedNode[i].begin(), SortedNode[i].end(), StartTimeCompare);

            for(int time=1;time<=TimeConstrain[1]+1;time++){
                for(int i=0; i<3; i++){
                    ResourceName.clear() ;
                    for(int j=Base[i]; j<SortedNode[i].size();j++) {
                        if(SortedNode[i][j]->maxLatency < time) Base[i] = j+1 ;
                        else if(SortedNode[i][j]->minLatency <= time) ResourceName.push_back(SortedNode[i][j]->getName()) ;
                    }
                    if(!ResourceName.empty()) ResourceConstraintWritter(ResourceName, time, ResourceConstrain[i]) ;
                }
            }
            
            //4. Paremeter 
            fp << "Binary\n" ;
            for(int i=0; i<ParmeterName.size(); i++) fp << ParmeterName[i] << MARK << ParmeterOrder[i] << "\n" ;
            fp << "END" ;
            fp.close() ;
            system(string("gurobi_cl ResultFile=.ILPsol.sol " + SaveName + " >> /dev/null").c_str()) ;
            
            return SOLReader(graphBody,graphEnd,ShowResult) ;
        }
        int BruteILPFormulation(map<string, BoolenVertex*>&graphBody,map<string, BoolenVertex*>&graphEnd,vector<int>ResourceConstrain, 
                                vector<int>TimeConstrain, bool ShowResult=false){
            ParmeterName.clear() ;
            ParmeterOrder.clear() ;
            string SaveName = ".formulation.lp" ;
            fp.open(SaveName) ;
            
            vector<vector<BoolenVertex*>>SortedNode(3,vector<BoolenVertex*>());
            vector<int> Base = {0,0,0} ;

            //0. min function 
            fp<<"Min\n" ;
            MinimizeFunctionWritter("EOM_PI",1,TimeConstrain[1]+1) ;
            ParameterSaver("EOM_PI",1,TimeConstrain[1]+1) ;

            //1. Unique start times
            fp<<"Subject To\n" ;
            UniqueStartTimeWritter("EOM_PI",1,TimeConstrain[1]+1) ;
            for(auto &it : graphBody){
                UniqueStartTimeWritter(it.second->getName(),1,TimeConstrain[1]) ;
                ParameterSaver(it.second->getName(),1,TimeConstrain[1]) ;
                SortedNode[OP2INT(it.second->type)].push_back(it.second) ;
            }

            //2. Dependency Constrain
            for(auto &it : graphBody){
                for(auto &dep : it.second->predecessor)
                    if(dep->type!=BEG)
                        DependencyWritter(it.second->getName(), dep->getName(), 
                                            1, TimeConstrain[1], 
                                            1, TimeConstrain[1]) ;
            }
            for(auto &dep : graphEnd["EOM_PI"]->predecessor)
                if(dep->type!=BEG)
                    DependencyWritter(graphEnd["EOM_PI"]->getName(), dep->getName(), \
                                        1, TimeConstrain[1]+1, \
                                        1, TimeConstrain[1]) ;
            
            //3. Resource constraints in every time slice
            vector<string> ResourceName ; 
            for(int time=1;time<=TimeConstrain[1];time++){
                for(int i=0; i<3; i++){
                    ResourceName.clear() ;
                    for(int j=0; j<SortedNode[i].size();j++) 
                        ResourceName.push_back(SortedNode[i][j]->getName()) ;
                    if(!ResourceName.empty()) ResourceConstraintWritter(ResourceName, time, ResourceConstrain[i]) ;
                }
            }
            //4. Paremeter 
            fp << "Binary\n" ;
            for(int i=0; i<ParmeterName.size(); i++) fp << ParmeterName[i] << MARK << ParmeterOrder[i] << "\n" ;
            fp << "END" ;
            fp.close() ;
            system(string("gurobi_cl ResultFile=.ILPsol.sol " + SaveName + " >> /dev/null").c_str()) ;
            
            return SOLReader(graphBody,graphEnd,ShowResult) ;
        }

};

int main(int argc, char* argv[]){
    
    CMDParser ExecuteConfig ;
    BoolenGraph GraphCreator ;
    SchedulingAlgorithm Process ;
    ExecuteConfig.Parse(argc, argv);

    bool SilentMode = ExecuteConfig.isSilent ;
    int minLatency = 0 ;
    int maxLatency = 0 ;
    int result = 0 ;
    GraphCreator.CreateGraph(ExecuteConfig.blifFile, ExecuteConfig.constrain) ;
    Process.EcecuteCheck(GraphCreator, ExecuteConfig) ;


    GraphCreator.GraphPassedInit(0) ;
    minLatency = Process.ASAP(GraphCreator.graphBody,GraphCreator.graphEnd,false) ;

    GraphCreator.GraphPassedInit(1) ;
    Process.ALAP(GraphCreator.graphBody,GraphCreator.graphEnd,maxLatency,false) ;

    GraphCreator.GraphPassedInit(0) ;
    GraphCreator.GraphLabelInit(0) ;
    GraphCreator.GraphLabelInit(1) ;

if(ExecuteConfig.isDebug) ExecuteConfig.ShowState() ;
if(ExecuteConfig.isDebug) GraphCreator.ShowDemand() ;
if(ExecuteConfig.isDebug) GraphCreator.GraphInfo(true,true) ;

    if(ExecuteConfig.useAlgorithm==Heuristic){
        cout << "Heuristic Scheduling Result\n" ;
        result = Process.HeuristicAlgorithm(GraphCreator.graphBody,GraphCreator.graphEnd,ExecuteConfig.constrain , !SilentMode) ;
    }else if(ExecuteConfig.useAlgorithm==FORMULATED_ILP || ExecuteConfig.useAlgorithm==BRUTE_ILP){
        
        if(ExecuteConfig.useAlgorithm==FORMULATED_ILP) cout << "ILP-based Scheduling Result\n" ;
        else  cout << "ILP-brute Scheduling Result\n" ;

        maxLatency = Process.HeuristicAlgorithm(GraphCreator.graphBody,GraphCreator.graphEnd,ExecuteConfig.constrain ,false) ;
        if(maxLatency==minLatency){
            if(ExecuteConfig.isDebug) cout << "Best Solution by Heuristic Scheduling\n" ;
            GraphCreator.GraphPassedInit(0) ;
            result = Process.HeuristicAlgorithm(GraphCreator.graphBody,GraphCreator.graphEnd,ExecuteConfig.constrain ,!SilentMode) ;
        }else{
            GraphCreator.GraphPassedInit(1) ;
            Process.ALAP(GraphCreator.graphBody,GraphCreator.graphEnd,maxLatency,false) ;
            
            if(ExecuteConfig.useAlgorithm==FORMULATED_ILP)
                result = Process.ILPFormulation(GraphCreator.graphBody,GraphCreator.graphEnd,ExecuteConfig.constrain, \
                            vector<int>{minLatency,maxLatency},  !SilentMode) ;
            else
                result = Process.BruteILPFormulation(GraphCreator.graphBody,GraphCreator.graphEnd,ExecuteConfig.constrain, \
                            vector<int>{minLatency,maxLatency},  !SilentMode) ;
        }
    }

    if(ExecuteConfig.removeMetaFile && ExecuteConfig.useAlgorithm!=Heuristic)
        system("rm .ILPsol.sol .formulation.lp gurobi.log >> /dev/null") ;
    cout << "LATENCY : "<< result << "\n" ;
    cout << "END\n" ;

    return 0 ;
}
