# mini语言ARM64汇编

## Requirements:

- gcc == 10.2 arrch64
- GNU make == 4.3
- flex == 2.6.4
- bison == 3.7.5

## Get Started

1. 修改makefile中的前缀地址
2. `make remake` 构建可执行文件mini
3. `mini ./sample/*.m > output.s` 生成ARM64汇编文件
4. `gcc output.s -o obj` 生成可执行文件obj

## Change Log

- 5/16: 完成了if.m和func.m的测试
  - //todo： 大于8个参数的传入测试

