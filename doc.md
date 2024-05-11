# 控制流绘制
# 数据流分析
## 1.
## 2.live var analysis
    活变量分析是一种倒序分析方式，若该变量将会在以后用到则其状态为1，若未被使用则状态为0。live var analysis将会在deadassign中用到，为了防止定义变量的var_type的tac被删除，额外加入了2号状态。
### 状态说明:
    0: 此变量以后不会出现
    1: 此变量的值以后会被使用
    2: 此变量的值以后不会使用，但会被重新赋值
### 参数输入:
    BB **BB_array: 存放BB的数组
    int BB_num: BB_block的数量
    TAC *tac_first: 第一个三地址码地址
    R_node **res: 存放basic block的状态向量结果
    SYM **def_vars: 存放变量符号向量
    int *res_def_index: 存放变量符号向量长度


## 3.
## 4.
# 优化器设计
## 1.
## 2.
## 3.
## 4.deadcode_opt
## 5.deadvar_opt
## 6.