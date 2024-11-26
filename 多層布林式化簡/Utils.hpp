#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <bitset>
#include <set>
#include <utility>
#include <ctime>
#include <cmath>
#include "boost/dynamic_bitset.hpp"
#include "boost/utility/binary.hpp"

using namespace std ;
typedef boost::dynamic_bitset<> DBset ;

namespace Utils{
    vector<string> Split(string& str, string pattern) ;
    template <typename T> void ShowVector(vector<T> v) ;
    template <typename T> void ShowSet(set<T> v) ;
    template <typename T,typename T2> void ShowMap(map<T,T2> v) ;

    template <typename T> set<T> Intersect(set<T>& A, set<T>& B);
    template <typename T> set<T> Subtract(set<T>& A, set<T>& B);
    template <typename T> set<T> Union(set<T>& A, set<T>& B);
} 

class BooleanPOS ;
class Cubes;

template <typename T> set<T> operator&(set<T>& A, set<T>& B);

class BooleanPOS : public vector<DBset> {
    public :
        using vector<DBset>::vector ;
        BooleanPOS(const vector<DBset>& productTerms) ;
        BooleanPOS(const set<DBset>& productTerms) ;
        DBset intersect(void) ;
        DBset selfUnion(void) ;
        int encodeSize(void) ;
        void kernel(Cubes& K, Cubes& Cok, vector<int>& saveLiteral, int begin, int functionalCodeLength) ;
        void kernel(BooleanPOS& A, Cubes& K, Cubes& Cok, DBset& CV,vector<int>& saveLiteral, DBset lastCok,int begin, int encodeTail, int level) ;
        
        friend ostream& operator<<(ostream& os,const BooleanPOS& p) ;
        void operator<<(const int& B) ;
        void operator>>(const int& B) ;

        BooleanPOS operator%(const BooleanPOS& B) ; 
        BooleanPOS operator%(const DBset& B) ; 
        void operator%=(const BooleanPOS& B) ;

        BooleanPOS operator/(const BooleanPOS& B) ; 
        BooleanPOS operator/(const DBset& B) ; 
        BooleanPOS divideKeepZero(const BooleanPOS& B) ;
        BooleanPOS divideKeepZero(const DBset& B) ;

        void divideAndPadding(const BooleanPOS& B, int paddingVariable) ;
        Cubes toCubes(void) ;
} ;

class Cubes : public vector<BooleanPOS>{
    int validSize = 0;
    int originalEncodeSize = 0;
    vector<string> outputTable ;
    vector<string> inputTable ;
    string generateName(int begin, string& base) ;
    public :
        map<int, string> encodeTable ; 
        map<string, int> decodeTable ;
        map<int, string> positionalTable ;
        
        using vector<BooleanPOS>::vector ;
        Cubes(const map<int, string>& encodeMap, const map<string, int>& decodeMap,  int encodeOffset=0, int encodeSize=0) ;
        Cubes(const vector<vector<vector<string>>>& booleanVariables, 
              const map<int, string>& encodeMap, const map<string, int>& decodeMap, bool endWithOutput=false, int encodeOffset=0, int encodeSize=0) ;
       
        void expandVariable(vector<string>variableName) ;
        void virtualExpand(vector<string>variableName) ;

        void substitution(const BooleanPOS& substitutionPOS, const string& substitutionVariable, int maxIdx=-1) ;
        int encodeSize(void) ;

        BooleanPOS createCode(const vector<vector<string>>& code) ;
        friend ostream& operator<<(ostream& os,const Cubes& p) ;
        void writeVariables(string modelName, string fileName="out.blif");
        void addPos(const BooleanPOS& p, string& varName) ;
        void singleCubes() ;
        void singleExtraction() ;
        void multipleExtraction() ;
        int literalCount() ;
        void sorted() ;
        
} ;
// {A+B+CD=E , DE+OPS=Q}