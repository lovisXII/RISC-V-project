1. 
```c
sc_out <sc_uint
//devient :
sc_out<
``` 
```
(sc_[\w]+)(|[\s]+)<
$1<
```
2. 
```c
sc_out< sc_uint
//devient :
sc_out<sc_uint
``` 
```c
(sc_[\w]+<)[\s]+
$1
```
3. 
```c
sc_out<sc_uint<6> > 
//devient :
sc_out<sc_uint<32>>
```     
```
(sc_[\w]+<sc_[\w]+<[\w]+>)[\s]+>
$1>
```
4. 
```c
sc_out<bool >
//devient :
sc_out<bool>
```          
```
(sc_[\w]+<bool)[\s]+>
$1>
```
5. 
```c
sc_in<bool>               BP_EXE2MEM_EMPTY_SE ;
sc_in<sc_uint<6>>                    BP_DEST_RM ;
sc_in<sc_uint<32>>      BP_MEM_RES_RM 
//devient :
sc_in<bool>             BP_EXE2MEM_EMPTY_SE ;
sc_in<sc_uint<6>>       BP_DEST_RM ;
sc_in<sc_uint<32>>      BP_MEM_RES_RM 
```   
```
(sc_[\w]+<[\w]+(|<[\w]+>)>)[\s]+([\w]+)[\s]+;
$1\t\t\t$3;
```

6. 
To align sensitivity list :
```
(<< [\w]+)

\n
\t\t\t$1
```
7. 
```
sc_[\w]+<([\w]+|sc_[\w]+<[\w]+>)>[\s]+[\w]+([\s]+|);
```
```
sc_trace(tf,$2,GET_NAME($2));
```