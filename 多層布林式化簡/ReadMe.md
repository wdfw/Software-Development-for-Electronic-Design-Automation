## 專案說明
在本次專案中，我們要以啟發式的方式實現多層布林代數的化簡演算法，我們要透過algebraic division、decomposition、  
kernel extraction、substitution等四個代數操作來最小化布林代數的數量
1. 讀取.blif檔來設定電路的邏輯關係
2. 對這些多層級的布林代數進行化簡
3. 輸出簡化後的.blif檔
## 檔案說明  
main.cpp:　輸入與輸出的主檔案  
Utils.hpp:　定義邏輯間的表示方式與代數化簡方法  
Utils.cpp:　實現邏輯資料的操作與代數化簡的演算法  
sample.blif: 測試用的.blif檔
out.blif: sample.blif化簡後的輸出
## 使用方法(要安裝Boost Libraries才可以正常使用)
    1. 進入此專案
    2. 輸入 cmake .
    3. 輸入 make
       編譯專案並產生執行檔 main
    4. 輸入 ./main [sample.blif]
        sample.blif: 要簡化的.blif檔
    5. 產生out.blif檔，並顯示化簡後的邏輯數量


## 測試結果
以下為執行./main sample.blif後的結果，簡化後的邏輯會透過Ex(x為任意數字)變數進行擴充，以組成等效的布林代數
###sample.blif
![image](https://github.com/user-attachments/assets/e832aaf8-96a2-4597-be8c-5776352e38ae)
###out.blif
![image](https://github.com/user-attachments/assets/0811ba5e-6905-4062-95a7-a3a42c8ffa61)
###執行狀況
將原本52個布林代數減少至36個
![Uploading image.png…]()




