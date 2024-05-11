# REG约定
```c
/* register */
#define R_UNDEF -1
#define R_FLAG 0 // FLAG bit
#define R_IP 1	 // reg PC
#define R_BP 2	 // Base Address，通常存STACK
#define R_JP 3	 // Jump Address
#define R_TP 4	 // Temp
#define R_GEN 5	 // General starting from 5, ending at 15
#define R_NUM 16
```

# 汇编场景
## 1. `asm_bin` 二元操作(binary)

格式： a = b + c

流程：先申请两个能用的寄存器，给b和c，生成汇编：`ADD reg1 reg2`，临时结果会存入`reg1`，然后再把`reg1`的值用load绑定到`a`中，即`insert_desc(reg1, a, MODIFIED)`。

# OBJ.C SYMBOL 功能记录
## 1. `clear_desc`

清空`reg_describtor`, 即`rdesc[r].var=NULL;`, `rdesc[].var`是一个`SYM`类型，与tac中的结构一致。

## 2. `insert_desc(int r, SYM *n, int mod)`

插入一条关于symbol`n`reg的使用记录到`rdesc[r]`中，并记录为不必回写。如果有某个别的寄存器保存了`n`，将其清空。


## 3. `spill_one(int r)`
回写`reg[r]`，刷新原`reg[r]`中的symbol为UNMODIFIED

```c
if (rdesc[r].var->store == 1) /* local var */
{
  printf("	STO (R%u+%u),R%u\n", R_BP, rdesc[r].var->offset, r);
}
else /* global var */
{
  printf("	LOD R%u,STATIC\n", R_TP);
  printf("	STO (R%u+%u),R%u\n", R_TP, rdesc[r].var->offset, r);
}
rdesc[r].modified = UNMODIFIED;
```

local的写进栈，global的要写进static，因此需要用到static开头地址（暂存到`R_TP`，再写）

## 4. spill_all()

同上，通用寄存器全写回，全刷新为UNMODIFIED。

没有涉及保留寄存器，包括BP、TP之类的。

## 5. flush_all()

同上，通用寄存器全写回，但对应的rdesc全清空，不再绑定symbol。

TP也会被清空，不回写，因为是temp。

## 6. load_reg(int r, SYM *n)

把`n` load到`reg[r]`，如果`n`已经在另一个寄存器被使用了，将另一个寄存器的n清空，插入到`reg[r]`即可。这项工作在`insert_desc`函数中有定义。

如果`n`是首次插入，设置为UNMODIFIED，否则copy之前占用的寄存器`reg[s]`的modified状态。

具体打印哪种LOD指令，得根据`n`的类型来确定
- 常数？-> 直接写立即数
- 局部变量？-> 要加上栈offset
- 全局变量？-> 要从STATIC区域LOD过来
- Label？-> 用读Label的方式读（arm64可以简略，当变量处理即可）

## 7. get_first_reg(SYM *n)

让一个reg与`n`绑定。返回一个绑定好了`n`的reg编号。

实现细节：

如果`n`之前已经绑定过`reg[r]`，只需要回写原值，刷新成UNMODIFIED，就能使用了（`spill_one`函数可以搞定这个）

如果有空寄存器，或者有不需要回写的寄存器，直接鸠占鹊巢即可。

否则需要回写一个倒霉蛋（这里回写`R_GEN`，第一个通用寄存器），然后鸠占鹊巢。

- 这里可以尝试引入LRU算法，找个不常用的倒霉蛋替换。否则很多寄存器会没用上。

## 8. get_second_reg(SYM *n, int first_reg)

基本上跟上一个函数一样，建议优化一下，复用代码。甚至可以两个一起考虑，避免第一个reg鸠占鹊巢的时候把第二个symbol本来可以复用的reg给占了。

## 9. asm_bin()

最基础的tac指令，拿两个symbol做二元操作，然后存到第一个symbol中，只要用上面的get_reg函数规范地拿到两个能做运算的reg，然后把reg1强制绑定为存结果的那个symbol并标记MODIFIED使其之后能被回写即可。

## 10. 

