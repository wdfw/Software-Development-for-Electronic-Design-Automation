*使用g++版本 : g++ 11.4
*B10902227.cpp主要執行檔與文字讀取
*Utils.cpp包含邏輯簡化的演算法
*請確認可以使用C++ Boost Libraries(會使用"boost/dynamic_bitset.hpp"與"boost/utility/binary.hpp") (WSL安裝的C++應該有內建)

使用方法 :
    1. 進入此專案
    2. 輸入 cmake .
    3. 輸入 make
       編譯專案並產生執行檔 main
    4. 輸入 ./main "your.blif" 
       會產生out.blif檔，並顯示化簡後的邏輯數量
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




