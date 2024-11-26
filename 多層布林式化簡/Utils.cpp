#include "./Utils.hpp"
string EXPAND_BASE_NAME = "E" ;
string VIRTUAL_BASE_NAME = "$" ;
void operator&=(BooleanPOS& A, set<DBset>& B){

    for(int i=A.size()-1; i>=0; i--) 
        if(B.find(A[i]) == B.end()){
            A[i] = A.back() ;
            A.pop_back() ;
        }
            
}


int maxIndex(const vector<int>& v){
    int val=-999999, idx ;
    for(int i=0; i<v.size(); i++) 
        if(val<v[i]){
            val = v[i] ;
            idx = i ;
        }
    return idx ;
}
//-------------------------Product-Of-Sum Begin-------------------------
BooleanPOS::BooleanPOS(const vector<DBset>& productTerms){
    resize(productTerms.size()) ;
    for(int i=0; i<productTerms.size(); i++) at(i) = productTerms[i] ;
}
BooleanPOS::BooleanPOS(const set<DBset>& productTerms){
    resize(productTerms.size()) ;
    int i=0 ;
    for(auto& it : productTerms) at(i++) = it ;
}
int BooleanPOS::encodeSize(void){
    if(empty()) return 0 ;
    return at(0).size() ;
}
DBset BooleanPOS::intersect(void){
    DBset Q = at(0) ;
    for(int i=1; i<size(); i++) Q &= at(i);
    return Q ;
}
DBset BooleanPOS::selfUnion(void){
    if(empty()) return DBset(0) ;
    DBset Q = at(0) ;
    for(int i=1; i<size(); i++) Q |= at(i);
    return Q ;
}

void BooleanPOS::kernel(Cubes& K, Cubes& Cok, vector<int>& saveLiteral, int begin, int functionalCodeLength){
    K.clear() ;
    Cok.clear() ;
    saveLiteral.clear();
    DBset ConsiderVariable = selfUnion() ; 
    ConsiderVariable.set(begin+1,functionalCodeLength,0) ;
    DBset foo(encodeSize(),0); 
    foo.set() ;
    Cok.push_back({foo}) ;
    kernel(*this, K, Cok, ConsiderVariable, saveLiteral, DBset(encodeSize(),0), begin, begin , 0) ;
}
void BooleanPOS::kernel(BooleanPOS& A, Cubes& K, Cubes& Cok, DBset& CV, vector<int>& saveLiteral, DBset lastCok, int begin, int encodeTail, int level){
    int quotient = 0 ;
    K.push_back(A) ;
        if(level==0) saveLiteral.push_back(0);
        else saveLiteral.push_back((A.size())*(lastCok.count()-1)) ;
    
    if(begin<0) return ;
    BooleanPOS T ;
    DBset dividen(encodeSize(), 0) ; 
    DBset maxDividen(encodeSize(), 0) ; 
    DBset mask(encodeSize(),0) ;
    mask.set(begin, encodeTail-begin, 1) ;
    
    for(int i=begin; i>=0; i--){
        if(!CV[i]){
            mask.set(i) ;
            continue ; 
        }
        dividen.set(i) ;
        T = A % dividen ;
        quotient = T.size() ;
        if(quotient>=2){
            maxDividen = T.intersect() ;
            if(!(maxDividen & mask).any() ){
                T = A / maxDividen ;
                //T = A.divideKeepZero(maxDividen) ;
                //cout << T <<"\n";
                if(T.size()==quotient){
                    //if(level==0) Cok.push_back({maxDividen}) ;
                    //else Cok.push_back({maxDividen | lastCok}) ;
                    Cok.push_back({maxDividen | lastCok}) ;
                    kernel(T, K, Cok, CV,saveLiteral, (maxDividen|lastCok),  i-1, encodeTail, level+1) ;
                }
            }
        }
        dividen.reset(i) ;
        mask.set(i) ;
    }
}

BooleanPOS BooleanPOS::operator%(const BooleanPOS& B){
    BooleanPOS Q, Res ;
    set<DBset> C ;
    for(int i=0; i<B.size(); i++){
        C.clear() ;
        for(int j=0; j<size(); j++){
            if((at(j) | B[i]) == at(j))
                C.insert(at(j)-B[i]) ;
        }
        if(i==0) Q = BooleanPOS(C) ;
        else Q &= C ;
    }

    for(auto& it : Q) 
        for(auto& it2 : B) 
            Res.push_back(it|it2) ;
    return Res ;
}
BooleanPOS BooleanPOS::operator%(const DBset& B){
    BooleanPOS Q ;
    for(int i=0; i<size(); i++)
        if((at(i) | B) == at(i))
            Q.push_back(at(i)) ;
    return Q ;
}
void BooleanPOS::divideAndPadding(const BooleanPOS& B, int paddingVariable){
    
    
    BooleanPOS Res ;
    BooleanPOS Q, R;
    set<DBset> C ;
    int k = 0;
    Q = (*this)%B ;
    for(int i=0; i<Q.size(); i++) C.insert(Q.at(i)) ;    
    for(int i=0; i<size(); i++)
        if(C.find(at(i)) == C.end()) R.push_back(at(i)) ;
    
    Q = Q.divideKeepZero(B) ;

    if(!Q.empty()) 
        for(int i=0; i<Q.size(); i++) Q[i][paddingVariable] = 1 ;

    resize(Q.size()+R.size()) ;
    for(auto& it : Q) at(k++) = it ;
    for(auto& it : R) at(k++) = it ;

}

BooleanPOS BooleanPOS::operator/(const BooleanPOS& B){
    BooleanPOS Q ;
    set<DBset> C ;
    for(int i=0; i<B.size(); i++){
        C.clear() ;
        for(int j=0; j<size(); j++){
            if((at(j) | B[i]) == at(j) && (at(j)!=B[i]))
                C.insert(at(j)-B[i]) ;
        }
        if(i==0) Q = BooleanPOS(C) ;
        else Q &= C ;
        for(auto&it : C) cout << it << " " ;
        
    }

    return Q ;
}
BooleanPOS BooleanPOS::operator/(const DBset& B){
    BooleanPOS Q ;
    for(int i=0; i<size(); i++)
        if((at(i) | B) == at(i) && (at(i)!=B))
            Q.push_back(at(i) - B) ;
    return Q ;
}
BooleanPOS BooleanPOS::divideKeepZero(const BooleanPOS& B){
    BooleanPOS Q ;
    set<DBset> C ;
    for(int i=0; i<B.size(); i++){
        C.clear() ;
        for(int j=0; j<size(); j++){
            if((at(j) | B[i]) == at(j))
                C.insert(at(j)-B[i]) ;
        }
        if(i==0) Q = BooleanPOS(C) ;
        else Q &= C ;
    }

    return Q ;
}
BooleanPOS BooleanPOS::divideKeepZero(const DBset& B){
    BooleanPOS Q ;
    for(int i=0; i<size(); i++)
        if((at(i) | B) == at(i))
            Q.push_back(at(i) - B) ;
    return Q ;
}

Cubes BooleanPOS::toCubes(void){
    Cubes T(1,*this) ;
    return T ;
}

void BooleanPOS::operator<<(const int& B){
    for(int i=0; i<size(); i++) 
        at(i).resize(at(i).size()+B) ;
}
void BooleanPOS::operator>>(const int& B){
    for(int i=0; i<size(); i++) 
        at(i).resize(at(i).size()-B) ;
}
ostream& operator<<(ostream& os,const BooleanPOS& p){
    string str(""),tem ;

    for(auto& it : p) {
        to_string(it, tem) ;
        str = str + tem  + " ";
    }
    os << str ;
    return os ;
}
//-------------------------Product-Of-Sum Begin-------------------------

//-------------------------Cubes Begin-------------------------
//a+b+c = f // (a) (b c) (f)
//d+v+as = K // (d) (v) (a s) (K)
//vector<BooleanPOS>
Cubes::Cubes(const vector<vector<vector<string>>>& booleanVariables, 
             const map<int, string>& encodeMap, const map<string, int>& decodeMap, bool endWithOutput, int encodeOffset, int encodeSize){
    encodeTable = encodeMap ;
    decodeTable = decodeMap ;
    if(encodeOffset==0) validSize = encodeMap.size() ;
    else validSize = encodeOffset ;
    if(encodeSize==0) originalEncodeSize = encodeMap.size() ;
    else originalEncodeSize = encodeSize ;
    //for(auto &it : decodeMap) cout << it.first <<" " << it.second << " " ;
    resize(booleanVariables.size()) ;
    for(int i=0; i<booleanVariables.size(); i++){
        at(i) = BooleanPOS(booleanVariables.at(i).size()) ;
        for(int j=0; j<booleanVariables[i].size(); j++){
            at(i).at(j) = DBset( encodeMap.size(),0) ;
            if(endWithOutput && j==booleanVariables.at(i).size()-1){
                positionalTable[positionalTable.size()] = booleanVariables.at(i).at(j).back();
                outputTable.push_back(booleanVariables.at(i).at(j).back()) ;
                at(i).pop_back() ;
            }else{
                for(int k=0; k<booleanVariables[i][j].size(); k++){
                     at(i).at(j)[decodeMap.at(booleanVariables[i][j][k])] = 1 ;
                }
            }
        }
    }
    for(int i=0; i<encodeMap.size(); i++){
        if(decodeMap.find(encodeMap.at(i)+"'") != decodeMap.end())
            inputTable.push_back(encodeMap.at(i)) ;
    }
     
}
Cubes::Cubes(const map<int, string>& encodeMap,const map<string, int>& decodeMap, int encodeOffset, int encodeSize){
    encodeTable = encodeMap ;
    decodeTable = decodeMap ;
    if(encodeOffset==0) validSize = encodeMap.size() ;
    else validSize = encodeOffset ;
    if(encodeSize==0) originalEncodeSize = encodeMap.size() ;
    else originalEncodeSize = encodeSize ;
}
int Cubes::encodeSize(void){
    if(!empty()) return at(0).at(0).size() ;
    else return 0 ;
}
void Cubes::expandVariable(vector<string>variableName){
    int encodeLength = encodeSize() ;
    for(int i=0; i<variableName.size(); i++){
        if(decodeTable.find(variableName[i]) != decodeTable.end())  throw invalid_argument( "變數 " + variableName[i] + " 已存在\n") ; 
        //positionalTable[size()] = variableName[i] ;
        encodeTable[encodeLength] = variableName[i] ;
        decodeTable[variableName[i]] = encodeLength ;
        encodeLength ++ ;
    }
    for(int i=0; i<size(); i++) at(i) << variableName.size() ;
}

void Cubes::substitution(const BooleanPOS& substitutionPOS, const string& substitutionVariable, int maxIdx){
    if(maxIdx==-1) maxIdx = size() ; 
    if(decodeTable.find(substitutionVariable)==decodeTable.end()) throw invalid_argument( "變數 " + substitutionVariable + " 不存在\n") ; 
    int paddingPosition = decodeTable[substitutionVariable] ;
    for(int i=0; i<maxIdx; i++)
        at(i).divideAndPadding(substitutionPOS,paddingPosition) ;   
}
BooleanPOS Cubes::createCode(const vector<vector<string>>& code){
    BooleanPOS R ;
    R.resize(code.size()) ;
    for(int i=0; i<code.size(); i++){
        R[i] = DBset(encodeSize(),0) ;
        for(int j=0; j<code[i].size(); j++){
            R[i][decodeTable[code[i][j]]] = true ;
        }
    }
    return R ;
}
ostream& operator<<(ostream& os,const Cubes& p){
    string str(""),tem ;

    for(int i=0; i<p.size(); i++) {
        tem.clear() ;

        for(int j=0; j<p.at(i).size(); j++){
            if(p.at(i).at(j).all()){
                tem = tem + "1" ;
            }else if(p.at(i).at(j).none()){
                tem = tem + "0" ;
            }else{
                for(int k=0; k<p.at(i).at(j).size(); k++)
                    if(p.at(i).at(j)[k])
                        tem = tem + p.encodeTable.at(k) + " ";
            }
            if(j<p.at(i).size()-1)tem = tem + " + " ;    
        }

        if(p.positionalTable.find(i)!=p.positionalTable.end()) str = str + tem + " = " + p.positionalTable.at(i) +"\n" ;
        else str = str + tem + "\n" ;
    }
    os << str ;
    return os ;
}

vector<int> Set2SortedVector(set<int>& v){
    vector<int> R ;
    for(auto& it : v) R.push_back(it) ;
    sort(R.begin(), R.end()) ;
    return R ;
}
void Cubes::writeVariables(string modelName, string fileName){
    ofstream BlifFile(fileName) ;

    vector<string> outputNames ;
    vector<string> inputNames ;
    map<string,int> mapTable = decodeTable ; 
    map<string,bool> isInputs ;
    for(auto& it : decodeTable){
        if(decodeTable.find(it.first + "'")!=decodeTable.end()){
            mapTable[it.first+ "'"] = mapTable[it.first] ;
            inputNames.push_back(it.first) ;
            isInputs[it.first] = true ;
        }else{
            if(it.first.back()!='\'') isInputs[it.first] = false ;
        }
    } ;
    for(auto &it : outputTable) outputNames.push_back(it) ;
    sort(inputNames.begin(), inputNames.end()) ;
    sort(outputNames.begin(), outputNames.end()) ;


    BlifFile << ".model " << modelName << "\n" ;

    BlifFile << ".inputs " ;
    for(auto& it : inputTable) if(it.back()!='\'') BlifFile << it << " ";
    BlifFile << "\n" ;

    BlifFile << ".outputs " ;
    for(auto& it : outputTable) if(it.back()!='\'') BlifFile << it << " ";
    BlifFile << "\n" ;
    string tem ;
    DBset usedVariables ; 
    vector<string> orderedVarables ;
    for(int i=0; i<size(); i++){
        
        if(positionalTable.find(i)==positionalTable.end()) continue ; 
        
        
        usedVariables = at(i).selfUnion() ;

        orderedVarables.clear() ;
        
        BlifFile << ".names " ;
        
        for(int j=0; j<usedVariables.size(); j++){
            if(usedVariables[j]){
                orderedVarables.push_back(encodeTable[mapTable[encodeTable[j]]]) ;
                if(orderedVarables.size()>=2 && orderedVarables[orderedVarables.size()-1]==orderedVarables[orderedVarables.size()-2]) orderedVarables.pop_back() ;
            }
        }
        sort(orderedVarables.begin(), orderedVarables.end()) ;

        for(auto& it : orderedVarables) BlifFile << it << " " ;
        BlifFile << positionalTable[i] << "\n" ;
        
        for(int j=0; j<at(i).size(); j++){
            tem = string(orderedVarables.size(), '-') ;
            
            for(int k=0; k<orderedVarables.size(); k++){
                if(isInputs[orderedVarables[k]]){
                    if(at(i).at(j)[decodeTable[orderedVarables[k]]]){
                        tem[k] = '1' ;   
                    }else if(at(i).at(j)[decodeTable[orderedVarables[k]+"'"]]){
                        tem[k] = '0' ;   
                    }
                }else{
                    if(at(i).at(j)[decodeTable[orderedVarables[k]]]){
                        tem[k] = '1' ;
                    }
                }
            }
            
            BlifFile << tem << " 1\n" ;
        }
    }
    

    BlifFile << ".end\n" ;
    BlifFile.close() ;
    
}
int Cubes::literalCount(){
    int literalCount = 0;
    for(int i=0; i<size(); i++){
        for(int j=0; j<at(i).size(); j++){
            literalCount += at(i).at(j).count() ;
        }
    }
    return literalCount ;
}
string Cubes::generateName(int begin, string& base){
    while(1){
        if(decodeTable.find(base+to_string(begin))==decodeTable.end()) 
            return base+to_string(begin) ;
        begin ++ ;
    }
}
void Cubes::addPos(const BooleanPOS& p, string& varName){
    positionalTable[size()] = varName ;
    push_back(p) ;
}
void Cubes::singleExtraction(void){
    Cubes K(encodeTable,decodeTable),Cok(encodeTable,decodeTable) ;
    vector<int> saveLiteral ;
    int maxIteration = originalEncodeSize ;
    int maxIdx ;
    int currendIdx = 0 ;
    int beginOrder = 0 ; 
    string newName ;
    bool expand = true ;

    while(currendIdx<size()){
        if(at(currendIdx).empty()){
            currendIdx ++ ;
            continue ; 
        }
        if(expand){
            newName = generateName(beginOrder, EXPAND_BASE_NAME) ;
            expandVariable({newName});
            expand = false ;
            //K.encodeTable = encodeTable ;
            //K.decodeTable = decodeTable; cout << K ;
        }
        at(currendIdx).kernel(K,Cok,saveLiteral,validSize-1,encodeSize()-validSize) ;
        maxIdx = maxIndex(saveLiteral) ;
        if(saveLiteral[maxIdx]>0){
            substitution(K[maxIdx], newName) ;
            addPos(K[maxIdx],newName) ;
            expand = true ;
        }else{
            currendIdx ++ ;
        }
    }
}
bool compare(const pair<int,int> &cmp1, const pair<int,int> &cmp2){
    return cmp1.first < cmp2.first ;
}
void Cubes::sorted(){
    vector<pair<int,int>> cmpIdx ;
    BooleanPOS tem ;
    map<int, string> newPositionalTable ;
    vector<BooleanPOS> newCube ;

    for(int i=0; i<size(); i++) 
        cmpIdx.push_back(pair<int,int>(at(i).size(), i)) ;
    sort(cmpIdx.begin(),cmpIdx.end(),compare) ;    
    //for(int i=0; i<size(); i++) cout << cmpIdx[i].first <<" " ;
    
    for(int i=0; i<size(); i++){
        if(positionalTable.find(cmpIdx[i].second) != positionalTable.end())
           newPositionalTable[i] = positionalTable[cmpIdx[i].second] ;
        newCube.push_back(at(cmpIdx[i].second)) ;
    }
    for(int i=0; i<size(); i++) at(i) = newCube[i] ;
    positionalTable = newPositionalTable ;
}
int maxIndex(vector<int>& v, Cubes& c){
    int idx=0, val=-999999; 
    for(int i=0; i<v.size(); i++){
        if(val<v[i] && c[i][0].count()>1){
            idx = i ;
            val = v[i] ;
        }
    }
    return idx ;
}
void Cubes::multipleExtraction(void){
    int beginIdx = encodeSize(), endIdx = encodeSize()+outputTable.size() ;
    int position ;
    int beginOrder = 0;
    int currIdx, maxIdx = 0;
    vector<int> saveLiteral ;
    string newName ;
    Cubes K(encodeTable,decodeTable),Cok(encodeTable,decodeTable) ;
    BooleanPOS combination ;
    //cout << *this <<"\n" ;
    int dd = 0;
    while (true){
        combination.clear() ;
        K.clear() ;
        currIdx = 0 ;
        maxIdx = 0;
        while(currIdx<outputTable.size() && maxIdx==0){
            for(int i=0; i<at(currIdx).size(); i++) combination.push_back(at(currIdx).at(i)) ;
            combination.kernel(K,Cok,saveLiteral,validSize-1,encodeSize()-validSize) ;
            currIdx ++ ;
            K.encodeTable = encodeTable ;
            K.decodeTable = decodeTable; 
            Cok.encodeTable = encodeTable ;
            Cok.decodeTable = decodeTable; 
            maxIdx = maxIndex(saveLiteral,Cok) ;
            //for(int i=0; i<Cok.size(); i++) 
            //    cout << Cok[i][0].count() << " " ;
            //cout <<"\n";
        }
        //cout <<maxIdx<<"\n";
        // cout << K<<"\n----------------\n";
        // cout << Cok << "\n" ;
        // cout << *this <<"\n\n" ;
        
        if(dd++>100) break ;
        
        if(currIdx>=outputTable.size()) break;
        newName = generateName(beginOrder++, EXPAND_BASE_NAME) ;
        
        
        
        expandVariable({newName});
        for(int i=0; i<Cok[maxIdx].size(); i++) Cok[maxIdx][i].resize(Cok[maxIdx][i].size()+1) ;
        substitution(Cok[maxIdx], newName) ;
        addPos(Cok[maxIdx],newName) ;
    }
    
    
    
 

    
}

