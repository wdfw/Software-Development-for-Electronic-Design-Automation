## 檔案說明  
main.cpp: 讀檔，演算法與輸出的程式  
testcase.blif: 用於測試的BLIF檔  
## 使用方法(要使用 Gurobi中的guroi_cl 指令可以正常使用)
    1. 進入此專案
    2. 輸入 cmake .
    3. 輸入 make 
       編譯專案並產生執行檔 main   
    4. 輸入 ./main [-e/-h] [test_case.blif] [AND_Constrain] [OR_Constrain] [NOT_Constrain]
        -e: 使用 ILP-based Scheduling演算法
        -h: 使用Heuristic Scheduling演算法
        test_case.blif: 為要處理的.blif檔案
        AND_Constrain: 代表AND閘的限制
        OR_Constrain: 代表OR閘的限制
        NOT_Constrain: 代表NOT閘的限制
    5. 在符合邏輯閘資源限制的條件下，顯示每個時段的Boolen Opeartion

以下分別為Heuristic Scheduling與ILP-based Scheduling對test_case.blif的執行結果
分別輸入(./main -h testcase.blif 2 1 0)與(./main -e testcase.blif 2 1 0)
## 測試結果
以下是對testcase.blif資源分配後的排程結果  
Heuristic Scheduling Result  
   1 : {a0 a1}{a2}{}  
   2 : {b1 b0}{c0}{}  
   3 : {c1 e0}{d0}{}  
   4 : {d1 h0}{f0}{}  
   5 : {e1 j0}{m0}{}  
   6 : {f1}{g0}{}  
   7 : {n0 g1}{o0}{}  
   8 : {h1 p0}{k0}{}  
   9 : {i1 t0}{b2}{}  
  10 : {v0}{c2}{}  
  11 : {}{a3}{}  
  12 : {}{b3}{}  
  13 : {}{c3}{}  
  14 : {}{d3}{}  
  15 : {}{d2}{}  
  16 : {}{e2}{}  
  17 : {}{f2}{}  
  18 : {}{i0}{}  
  19 : {l0}{g2}{}  
  20 : {r0 z0}{e3}{}  
  21 : {zd0 zl0}{h2}{}  
  22 : {zp0 zo0}{u0}{}  
  23 : {zn0 zk0}{q0}{}  
  24 : {zm0 zj0}{f3}{}  
  25 : {zi0 zh0}{i2}{}  
  26 : {zg0 zf0}{y0}{}  
  27 : {ze0 zc0}{s0}{}  
  28 : {x0 za0}{w0}{}  
  29 : {zb0 f}{e}{}  
  30 : {g h}{}{}  
LATENCY : 30  
  
ILP-based Scheduling Result  
   1 : {a0 a1}{a2}{}  
   2 : {b0 b1}{c0}{}  
   3 : {c1 e0}{d0}{}  
   4 : {d1 h0}{g0}{}  
   5 : {e1 n0}{f0}{}  
   6 : {f1 j0}{i0}{}  
   7 : {g1}{o0}{}  
   8 : {h1 t0}{k0}{}  
   9 : {i1 l0}{m0}{}  
  10 : {zb0 zj0}{u0}{}  
  11 : {p0 zc0}{b2}{}  
  12 : {zd0 zn0}{s0}{}  
  13 : {ze0 zp0}{y0}{}  
  14 : {zl0 zo0}{a3}{}  
  15 : {zh0 zi0}{w0}{}  
  16 : {za0 zf0}{b3}{}  
  17 : {zg0 zm0}{c2}{}  
  18 : {x0 zk0}{d2}{}  
  19 : {h r0}{c3}{}  
  20 : {z0}{e2}{}  
  21 : {}{d3}{}  
  22 : {v0}{e3}{}  
  23 : {}{f2}{}  
  24 : {}{f3}{}  
  25 : {}{g2}{}  
  26 : {}{h2}{}  
  27 : {}{q0}{}  
  28 : {}{i2}{}  
  29 : {f g}{e}{}  
LATENCY : 29  
