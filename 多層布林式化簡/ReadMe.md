## 檔案說明  
main.cpp: 輸入，輸出的主檔案  
Utils.cpp:　包含表示邏輯的資料結構與代數化簡的演算法
sample.blif: 測試用的.blif檔
## 使用方法(要安裝Boost Libraries才可以正常使用)
    1. 進入此專案
    2. 輸入 cmake .
    3. 輸入 make
       編譯專案並產生執行檔 main
    4. 輸入 ./main [sample.blif]
        sample.blif: 要簡化的.blif檔
    5. 產生out.blif檔，並顯示化簡後的邏輯數量
以下為執行./main sample.blif後的結果
簡化後的邏輯會透過Ex(x為任意數字)變數進行擴充，以組成Multi-level Logic.

顯示:
Original literal count : 52
Optimized literal count : 36

原始檔                                  簡化後
sample.blif  		                out.blif

.model sample                           .model sample
.inputs a b c d e f g h i j k l m n     .inputs a b c d e f g h i j k l m n
.outputs o p                            .outputs o p
.names a b c d e g h i j k o            .names E1 b d e o
01111---1- 1                            1111 1
01011--11- 1                            .names E2 c d e f m n p
010111--0- 1                            1-0-1-0 1
11111---11 1                            -11111- 1
11011-111- 1                            .names E3 a c i k E0
.names c d e f g h l m n p              1-01- 1
1111---1- 1                             -01-- 1
-001--0-0 1                             -11-1 1
101111--0 1                             .names E0 a c g j E1
.end                                    1---1 1
                                        -0010 1
                                        .names c e g h l E2
                                        -0--0 1
                                        1111- 1
                                        .names a h E3
                                        0- 1
                                        11 1
                                        .end


## 專案說明
在本次專案中，我們要以啟發式與線性規劃的方式實現邏輯閘資源的分配演算法，  
分配演算法的目標是在限定資源並給定邏輯閘關係的情況下，最小化電路的最大延遲。  
在本專案中需要滿足下列的要求:  
1. 為了簡化，我們的可用資源只有AND、OR、NOT三種邏輯閘
2. 每個邏輯運算的時間都為1單位週期
3. 讀取.blif檔來設定電路的邏輯關係
5. -h表示採用啟發式方法實現的演算法
6. -e表示採用線性規劃實現的演算法
7. 在執行完後輸出每個階段的執行結果與電路的延遲
## 檔案說明  
main.cpp: 讀檔，演算法與輸出的程式  
testcase.blif: 用於測試的BLIF檔  
## 使用方法(要安裝 Gurobi才可以正常使用)
    1. 進入此專案
    2. 輸入 cmake .
    3. 輸入 make 
       編譯專案並產生執行檔 main   
    4. 輸入 ./main [-e/-h] [test_case.blif] [AND_Constrain] [OR_Constrain] [NOT_Constrain]
        -e: 使用 ILP-based Scheduling演算法
        -h: 使用Heuristic Scheduling演算法
        test_case.blif: 為要處理的.blif檔案
        AND_Constrain: 代表AND閘的限制數量
        OR_Constrain: 代表OR閘的限制數量
        NOT_Constrain: 代表NOT閘的限制數量
    5. 在符合邏輯閘資源限制的條件下，顯示每個時段的Boolen Opeartion

