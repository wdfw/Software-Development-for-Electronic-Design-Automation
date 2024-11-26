#include "./Utils.hpp"

typedef boost::dynamic_bitset<> DBset ;
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
//AB = 10_10 // S[3]S[2]_S[1]S[0] 
map<string, int> VARIABLE = {{"a",8},{"b",6},{"c",4},{"d",2},{"e",0},{"f",10},{"g",12},{"x",14},{"y",16}} ;
map<string, int> VARIABLE11 = {{"a",8},{"b",6},{"c",4},{"d",2},{"e",0},{"f",10},{"g",12},{"x",14},{"y",16}} ;
map< int,string> SHIFT = {{8,"a"},{6,"b"},{4,"c"},{2,"d"},{0,"e"},{10,"f"},{12,"g"},{14,"x"},{16,"y"}} ;


//vector<DBset> = {"ab", "cd", "ef"} = ab + cd + ef
template <typename T>
void ShowVector(vector<T> v){
    for(auto &it : v) cout << it << " " ;
    cout << "\n" ;
}
template <typename T>
void ShowSet(set<T> v){
    for(auto &it : v) cout << it << " " ;
    cout << "\n" ;
}
template <typename T,typename T2>
void ShowMap(map<T,T2> v){
    for(auto &it : v) cout <<"{"<< it.first << " " << it.second <<"} " ;
    cout <<"\n" ;
}
namespace CubeOperation{
    DBset Boolean2Cube(string boolean, map<string,int>& variable){ //AA'BB' ...
        DBset cube(variable.size()*2, 0) ;
        for(int i=0; i<boolean.size(); i++){
            if(boolean.substr(i,1) == "'"){
                cube.set(variable[boolean.substr(i-1,1)]) ;
                cube.reset(variable[boolean.substr(i-1,1)]+1) ;
            }else{
                cube.set(variable[boolean.substr(i,1)]+1) ;
                cube.reset(variable[boolean.substr(i,1)]) ;
            }
        }
        return cube ;
    }

    DBset Boolean2Cube(string boolean, vector<int>& variable, int size){ //AA'BB' ...
        DBset cube(size, 0) ;

        for(int i=0; i<boolean.size(); i++){
            if(boolean.substr(i,1) == "0"){
                cube.set(variable[i]) ;
                cube.reset(variable[i]+1) ;
            }else if(boolean.substr(i,1) == "1"){
                cube.set(variable[i]+1) ;
                cube.reset(variable[i]) ;
            }else if(boolean.substr(i,1) == "-"){
                cube.reset(variable[i]+1) ;
                cube.reset(variable[i]) ;
            }else{
                throw invalid_argument( "Boolean錯誤符號 : " + boolean + "\n") ;
            }
        }
        return cube ;
    }
    string Cube2Boolean(DBset cube, map<int,string>& shift){
        
        if(cube.all()) return "1" ;
        else if(!cube.any()) return "0" ;
        string boolean = "";
        for(int i=0; i<cube.size(); i+=2){
            if(~cube[i+1] & cube[i]){
                boolean =  shift[i] + "'" + boolean;
            }else if(cube[i+1] & ~cube[i]){
                boolean = shift[i] + boolean;
            }
        }
        return boolean ;
    }
    vector<DBset> Booleans2Cubes(vector<string>boolean, map<string, int>& variable){
        vector<DBset> dbResult ;
        for(int i=0; i<boolean.size(); i++)
            dbResult.push_back(Boolean2Cube(boolean[i], variable)) ;
        return dbResult ;
    }
    vector<string> Cubes2Booleans(vector<DBset>cube, map<int,string>& shift){
        vector<string> booleanResult ;
        for(int i=0; i<cube.size(); i++)
            booleanResult.push_back(Cube2Boolean(cube[i],shift)) ;
        return booleanResult ;
    }
    

    set<DBset> Intersect(set<DBset>& A, set<DBset>& B){
        set<DBset> I ;
        for(auto &it : A) if(B.find(it) != B.end()) I.insert(it) ;
        return I ;
    }
    set<DBset> Subtract(set<DBset>& A, set<DBset>& B){
        set<DBset> S ;
        for(auto &it : A) if(B.find(it) == B.end()) S.insert(it) ;
        return S ;
    }
    set<DBset> Union(set<DBset>& A, set<DBset>& B){
        set<DBset> U ;
        for(auto &it : A) U.insert(it) ;
        for(auto &it : B) U.insert(it) ;
        return U ;
    }
    
    DBset Divide(DBset& A, DBset& B){ // return A/B
        if( (A | B) != A) return DBset(A.size(),0) ;
        else return A-B ;
    }
    bool FilterTesting(vector<DBset> A, vector<DBset> B){
        if(A.size() < B.size()) return false ;
        return true ;
    }
    
    vector<vector<DBset>> IntersectCubes(vector<DBset>& A, vector<DBset>& B){ //BBBBBBBBBBUUG
        DBset tem ;
        vector<DBset> R, Q;
        set<DBset> D, T ;
        
        if(!FilterTesting(A,B)) return {R, A} ;

        for(int j=0; j<B.size(); j++){
            T.clear() ;
            for(int i=0; i<A.size(); i++){
                //tem = Divide(A[i],B[j], true) ;
                if(tem.any()) T.insert(tem) ;
            }
            if(j==0) D = T ;
            else D = Intersect(D,T);

            if(D.empty()) return {R, A} ;
        }
        Q = vector<DBset>(D.begin(), D.end()) ;
        
        //T = Subtract(set<DBset>(A.begin(), A.end()), D) ;
        R = vector<DBset>(T.begin(), T.end()) ;
        
        return {Q,R} ;
    }
    vector<DBset> IntersectCubes(vector<DBset>& A, DBset& B){
        DBset tem ;
        vector<DBset> Q;
        for(int i=0; i<A.size(); i++){
            if((A[i]&B)==B) Q.push_back(A[i]) ;
        }
        return Q ;
    }

    vector<vector<DBset>> AlgebricDivision(vector<DBset>& A, vector<DBset>& B){
        DBset tem ;
        vector<DBset> R, Q;
        set<DBset> D, T ;
        
        if(!FilterTesting(A,B)) return {R, A} ;

        for(int j=0; j<B.size(); j++){
            T.clear() ;
            for(int i=0; i<A.size(); i++){
                tem = Divide(A[i],B[j]) ;
                if(tem.any()) T.insert(tem) ;
            }
            if(j==0) D = T ;
            else D = Intersect(D,T);
            
            if(D.empty()) return {R, A} ;
        }
        Q = vector<DBset>(D.begin(), D.end()) ;

        T.clear() ;
        for(auto &it : Q) 
            for(auto &it2 : B)
                T.insert(it | it2) ;
        //T = Subtract(set<DBset>(A.begin(), A.end()), T) ;
        
        R = vector<DBset>(T.begin(), T.end()) ;
        
        return {Q,R} ;
    }
    vector<DBset> AlgebricDivision(vector<DBset>& A, DBset& B){
        DBset tem ;
        vector<DBset> Q;
        for(int i=0; i<A.size(); i++){
            tem = Divide(A[i],B) ;
            if(tem.any()) Q.push_back(tem) ;
        }
        return Q ;
    }
    DBset MaxDividen(vector<DBset>& A){
        
        DBset Q(A[0].size(),0) ;
        Q.set() ;
        for(int i=0; i<A.size(); i++) {
            Q &= A[i] ;
        }
        return Q ;
    }

    void KERNELS(vector<DBset>& A, DBset& considerVariable,int begin ,int level, vector<vector<DBset>>& K, vector<DBset>& Cok){
            DBset dividen(A[0].size(), 0) ; 
            DBset maxDividen(A[0].size(), 0) ; 
            DBset mask(A[0].size(),0) ;
            for(int i=0; i<begin; i++) mask[i] = 1 ;
            
            vector<DBset> tem ;
            
            K.push_back(A) ;
            for(int i=begin; i>=0; i--){
                if(!considerVariable[i]) {
                    mask.reset(i) ;
                    continue ; 
                }
                dividen.set(i) ;
                tem = IntersectCubes(A,dividen) ;
                if(tem.size()>=2){
                    maxDividen = (MaxDividen(tem) & mask) ;
                    tem = AlgebricDivision(A,maxDividen) ;
                    
                    if(tem.size()>=2){
                        Cok.push_back(maxDividen) ;
                        KERNELS(tem, considerVariable, i-1, level+1, K, Cok) ;
                    }
                }
                mask.reset(i) ;
                dividen.reset(i) ;
            }
    }
    void KERNELS(vector<DBset>& A, vector<vector<DBset>>& K, vector<DBset>& Cok, int begin=-1){
        DBset CV(A[0].size(),0); //ConsiderVariable
        for(int i=0; i<A.size(); i++) CV |=  A[i] ;
        if(begin==-1) begin = CV.size()-1 ;

        Cok.push_back(DBset(A[0].size(),0)) ;
        
        Cok[0].set() ;
        KERNELS(A, CV, begin, 0, K, Cok) ;
    }
    void CubePadding(DBset &A, DBset &B){
        for(int i=0; i<B.size(); i++) A.push_back(B[i]) ;
    }
        
} ;


vector<string> operator+(const vector<string>& v1, const vector<string>& v2){
    vector<string> v3(v1.size()+v2.size()) ;
    for(int i=0; i<v1.size(); i++) v3[i] = v1.at(i) ;
    for(int i=0; i<v2.size(); i++) v3[i+v1.size()] = v2.at(i) ;
    return v3 ;
}

class CMDParser{
    public :
        bool isDebug = false ;
        bool extractSingle = false ;
        bool extractMultiple = false ;
        bool countOnly = false ;
        bool showBoolean = false ;
        string blifFile ;

        CMDParser() {} ;
        void Parse(int argc, char* argv[]){
            string arg ;
            for(int i=1; i<argc; i++){
                arg = string(argv[i]) ;
                
                if(arg=="-debug") isDebug = true ;
                else if(arg=="-s") extractSingle=true ;
                else if(arg=="-m") extractMultiple=true ;
                else if(arg=="-c") countOnly=true ;
                else if(arg=="-t") showBoolean = true ;
                else if(arg.substr(max((int)arg.size()-5,0),arg.size())==".blif") blifFile = arg ;
            }
            if(blifFile.empty()) throw invalid_argument( "未指定BLIF檔案\n") ;
        }

} ;

class Variable{
    int readLineCounter = 0 ;
    void parseInitial(void){
            modelName = "N_A" ;
            readLineCounter = 0 ; 
            encodeTable.clear() ;
            decodeTable.clear() ;
            booleanString.clear() ;
            inputVariables.clear() ;
            outputVariables.clear() ;
            encodeSize = 0 ;
            encodeOffset = 0 ;
            literalCount = 0 ;
        }
    int readLine(ifstream &BlifFile, string &BlifText){
            readLineCounter ++ ;
            if(!getline(BlifFile, BlifText)) return 0 ;
            else if(BlifText.empty()) return 1 ; 
            else if(BlifText.back()=='\\') return 2 ;
            else return 4 ;
        }
    
    public :
        int encodeSize = 0 ;
        int encodeOffset = 0 ;
        int literalCount = 0 ;
        string modelName = "N_A" ;
        set<string> inputVariables ;
        set<string> outputVariables ;
        map<int,string> encodeTable ;
        map<string,int> decodeTable ;
        vector<vector<vector<string>>> booleanString ;
        
    Variable() {} ;
    void CreateVariables(string fileName, bool debugMode=false){
        parseInitial() ;
        ifstream BlifFile(fileName) ;
        string BlifText ;
        vector<string> splitedText, concatedText ;
        vector<string> partialVariables ;
        string nextVariable = "" ;
        
        int rState ;
        while((rState=readLine(BlifFile, BlifText))!=0){
            if(rState==1) continue ;
            splitedText = TextProcessing::Split(BlifText," ") ;
            
            if(splitedText[0]==".model"){
                if(modelName!="N_A") throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .model重複定義\n") ;
                if(splitedText.size()!=2) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .model輸入非法輸入\n") ;
                modelName = splitedText[1] ;
            }else if(splitedText[0]==".inputs"){
                if(splitedText.size()==1) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .inputs無輸入節點名稱\n") ;
                if(!decodeTable.empty()) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .inputs重複定義\n") ;
                if(splitedText.back()=="\\") splitedText.pop_back() ;

                concatedText = splitedText ;
                while(rState==2){
                    rState=readLine(BlifFile, BlifText) ;
                    splitedText = TextProcessing::Split(BlifText," ") ;
                    if(splitedText.back()=="\\") splitedText.pop_back() ;
                    concatedText =  concatedText + splitedText ;
                }

                for(int i=1; i<concatedText.size(); i++){
                    decodeTable[concatedText[i]+"'"] = decodeTable.size() ;
                    decodeTable[concatedText[i]] = decodeTable.size() ;  
                    inputVariables.insert(concatedText[i]) ;
                }
            }else if(splitedText[0]==".outputs"){
                
                if(splitedText.size()==1) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .outputs無輸入節點名稱\n") ;
                if(decodeTable.empty()) throw invalid_argument( "在定義.outputs前尚未定義.inputs\n") ;
                if(splitedText.back()=="\\") splitedText.pop_back() ;

                concatedText = splitedText ;
                while(rState==2){
                    rState=readLine(BlifFile, BlifText) ;
                    splitedText = TextProcessing::Split(BlifText," ") ;
                    if(splitedText.back()=="\\") splitedText.pop_back() ;
                    concatedText =  concatedText + splitedText ;
                }
               
                for(int i=1; i<concatedText.size(); i++){
                    decodeTable[concatedText[i]] = decodeTable.size() ;  
                    outputVariables.insert(concatedText[i]) ;
                }
            }else if(splitedText[0]==".names"){
                if(!nextVariable.empty()){
                    booleanString.back().push_back({nextVariable}) ;
                    if(decodeTable.find(nextVariable)==decodeTable.end()){
                        decodeTable[nextVariable] = decodeTable.size() ;  
                    }
                }
                if(splitedText.size()==1) throw invalid_argument( "第 " + to_string(readLineCounter) + "行 .names無輸入節點名稱\n") ;
                if(splitedText.back()=="\\") splitedText.pop_back() ;

                concatedText = splitedText ;
                while(rState==2){
                    rState=readLine(BlifFile, BlifText) ;
                    splitedText = TextProcessing::Split(BlifText," ") ;
                    if(splitedText.back()=="\\") splitedText.pop_back() ;
                    concatedText =  concatedText + splitedText ;
                }
        
                partialVariables.resize(concatedText.size()-2) ;
                for(int i=1; i<concatedText.size()-1; i++){
                    partialVariables[i-1] = concatedText[i] ;
                }

                nextVariable = splitedText.back() ;
                booleanString.push_back(vector<vector<string>>()) ;
            }else if(splitedText[0]==".end"){
                if(!nextVariable.empty()){
                    booleanString.back().push_back({nextVariable}) ;
                    if(decodeTable.find(nextVariable)==decodeTable.end()){
                        decodeTable[nextVariable] = decodeTable.size() ;  
                    }
                }
                
                break ;
            }else if(!nextVariable.empty() && splitedText.back()=="1"){
                booleanString.back().push_back(vector<string>()) ;
                for(int i=0; i<splitedText[0].size(); i++){
                    if(splitedText[0][i]=='0'){
                        booleanString.back().back().push_back(partialVariables[i]+"'") ;
                        literalCount ++ ;
                    }else if(splitedText[0][i]=='1'){
                        booleanString.back().back().push_back(partialVariables[i]) ;
                        literalCount ++ ;
                    }
                }
            }else if(splitedText[0][0]=='#'){
                continue ; 
            }
        }
        if(splitedText[0]!=".end") throw invalid_argument( "錯誤BLIF檔案\n") ;
        for(auto& it : decodeTable) encodeTable[it.second] = it.first ;
        encodeSize = encodeTable.size() ;
        encodeOffset = inputVariables.size()*2 ;
        if(debugMode) _DEBUG() ;
     }
     void _DEBUG(void){
        //map<string, int> encodeTable ;
        //map< int,string> decodeTable ;
        cout << "Model Name : " << modelName << "\n" ;
        cout << "Literal Count : " << literalCount << "\n" ;
        cout << "Encode Size : " << encodeSize << "\n" ;
        cout << "Input Variables : " ;
        for(auto& it : inputVariables) cout << it << " " ;
        cout << "\n" ;
        cout << "Output Variables : " ;
        for(auto& it : outputVariables) cout << it << " " ;
        cout << "\n" ; 

        cout << "Boolean Function : \n" ;

        for(int i=0; i<booleanString.size(); i++){
            for(int j=0; j<booleanString[i].size()-1; j++){
                for(int k=0; k<booleanString[i][j].size(); k++){
                    cout << booleanString[i][j][k] ;
                }
                if(j!=booleanString[i].size()-2)cout << " + " ; 
            }
            cout << " = " << booleanString[i].back()[0] << "\n" ;
        }

        cout << "Encode Table :\n" ;
        for(auto& it : encodeTable) cout << it.first << " " << it.second << " , " ; 
        cout << "\n" ;  
     }
} ;
template <typename T,typename T2>
void ShowMaps(map<T,T2> v){
    for(auto &it : v) cout <<"{"<< it.first << " " << it.second <<"} " ;
    cout <<"\n" ;
}
int main(int argc, char* argv[]){
    clock_t clockVariable = clock();
    
    CMDParser ExecuteConfig ;
    Variable CubeCreator ;
    vector<vector<DBset>> Kernel ;
    vector<DBset> CoKeneral ;
    vector<int> EliminateCounter ;
    ExecuteConfig.Parse(argc, argv);
    CubeCreator.CreateVariables(ExecuteConfig.blifFile, ExecuteConfig.isDebug) ;

    cout << "Original literal count : " << CubeCreator.literalCount << "\n";
    if(ExecuteConfig.countOnly) return 0 ;
    
    Cubes optCubes(CubeCreator.booleanString,CubeCreator.encodeTable,CubeCreator.decodeTable,true,CubeCreator.encodeOffset, CubeCreator.encodeSize) ;
    if(ExecuteConfig.showBoolean){
        cout << optCubes <<"\n" ;
        return 0 ;
    }
    if(ExecuteConfig.extractSingle && !ExecuteConfig.extractMultiple){
        optCubes.singleExtraction() ;
    }else if(!ExecuteConfig.extractSingle && ExecuteConfig.extractMultiple){
        optCubes.multipleExtraction() ;
    }else{
        optCubes.singleExtraction() ;
        optCubes.multipleExtraction() ;
    }
    
    optCubes.writeVariables(CubeCreator.modelName) ;

    cout << "Optimized literal count : " << optCubes.literalCount() << "\n";
    cout << "Execute Time : " << float(clock() - clockVariable)/1000 << "\n";
    cout << "File Name : " << ExecuteConfig.blifFile <<"\n" ;
    cout << "----------------------------------------------\n" ;
    

    return 0 ;
}

