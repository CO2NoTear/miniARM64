label main
begin
var i
var j
i = 1
var t0
t0 = (i != 2)
ifz t0 goto L2
j = 1
goto L3
label L2
j = 2
var t1
t1 = (i != 1)
ifz t1 goto L1
j = 3
label L1
j = 4
label L3
i = 5
j = 5
end
RD rresult for BB : 0
IN_vector: 
0 0 0 0 0 0 0 0 0 
OUT_vector: 
1 1 0 0 0 0 0 0 0 
RD rresult for BB : 1
IN_vector: 
1 1 0 0 0 0 0 0 0 
OUT_vector: 
1 1 1 0 0 0 0 0 0 
RD rresult for BB : 2
IN_vector: 
1 1 0 0 0 0 0 0 0 
OUT_vector: 
1 1 0 1 1 0 0 0 0 
RD rresult for BB : 3
IN_vector: 
1 1 0 1 1 0 0 0 0 
OUT_vector: 
1 1 0 0 1 1 0 0 0 
RD rresult for BB : 4
IN_vector: 
1 1 0 1 1 1 0 0 0 
OUT_vector: 
1 1 0 0 1 0 1 0 0 
RD rresult for BB : 5
IN_vector: 
1 1 1 0 1 0 1 0 0 
OUT_vector: 
0 1 0 0 1 0 0 1 1 
