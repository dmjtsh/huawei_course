# 💥Papich Language 💥
## Description
This language is the part of C language, Data Structures and Compilers course in **MIPT**. On Papich language you can write any program because it is Turing Complete.
This language is inspired by famous video game streamer *Arthas*.

![Papich Image](papich.jpg)

## How it works?
This program divides on four independent parts: **frontend**, **backend**, **assembler** and **CPU**. \
You can read about assembler and CPU parts on this link: ---.
### Frontend
**Recursive Decent grammar**
Entire frontend works on Recursive Decent algorithm. \
Firstly frontend divides program on tokens, this part of program is called **lexical analysis**:

![Lexical Analysis](lexical_analysis.png)

After dividing program on tokens, we will use Recursive Decent  algorithm that creates Scope Nametables and builds Code Tree from these tokens. \
**Example of Code Tree:**

![Code Tree Example](example1.png)

We need to save this tree and nametables for backend transformations. For this there is a special format of .txt file, in which we save our information about program. \
**Example of database file:** 

![Database Example](database_example.png)

### Backend
**Code Tree translating** \
The main idea of backend part is translating code tree into an assembler code. It recursively processes every node in exact way and in the end we will get our program fully translated into assembler code. \
**Example of transformation:**

![Backend Transformation Example](example2.png)

### Commands in Papich Language

**Here is the list of all commands in Papich Language:** 


| **Num** |     **Operator**     |                                                                        **Description**                                                                        |                                                    **Syntax**                                                    |
|:-------:|:--------------------:|:-------------------------------------------------------------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------:|
|  **0**  | *                    | Binary arithmetic MULTIPLICATION operator. Returns result of multiplication.                                                                                  | *Num\|Var* * *Num\|Var*                                                                                              |
|  **1**  | /                    | Binary arithmetic DIVISION operator. Returns result of division.                                                                                              | *Num\|Var* / *Num\|Var*                                                                                              |
|  **2**  | ЛежатьПлюсСосать     | Binary arithmetic ADDITION operator. Returns result of addition.                                                                                              | *Num\|Var* ЛежатьПлюсСосать *Num\|Var*                                                                               |
|  **3**  | -                    | Binary arithmetic SUBTRACTION operator. Returns result of subtraction.                                                                                        | *Num\|Var* - *Num\|Var*                                                                                              |
|  **4**  | ПаунсВОкно           | Binary arithmetic EXPONENTIATION operator. Returns result of exponentiation.                                                                                  | *Num\|Var* ПаунсВОкно *Num\|Var*                                                                                     |
|  **5**  | БОЛЬШЕ               | Binary logical operator. Returns 1 if first operand is GREATER then second, otherwise returns 0.                                                              | *Num\|Var* БОЛЬШЕ *Num\|Var*                                                                                         |
|  **6**  | МАЛА                 | Binary logical operator. Returns 1 if first operand is LESS then second, otherwise returns 0.                                                                 | *Num\|Var* МАЛА *Num\|Var*                                                                                           |
|  **7**  | РЕАЛИСТИЧНА          | Binary logical operator. Returns 1 if first operand is EQUAL with second, otherwise returns 0.                                                                | *Num\|Var* РЕАЛИСТИЧНА *Num\|Var*                                                                                    |
|  **8**  | НЕРЕАЛИСТИЧНА        | Binary logical operator. Returns 1 if first operand is NOT EQUAL with second, otherwise returns 0.                                                            | *Num\|Var* НЕРЕАЛИСТИЧНА *Num\|Var*                                                                                  |
|  **9**  | И                    | Binary logical operator. Returns 1 if first operand is not equal 0 AND second operand is not equal 0 too,  otherwise returns 0.                               | *Num\|Var* И *Num\|Var*                                                                                              |
|  **10** | ИЛИ                  | Binary logical operator. Returns 1 if first operand is not equal 0 OR second operand is not equal 0,  otherwise returns 0.                                    | *Num\|Var* ИЛИ *Num\|Var*                                                                                            |
|  **11** | ДаДаЯПроверяй        | Condition Operator. If Expression is not equal 0 program executes sequence of operators enclosed in figured brackets.                                         | ДаДаЯПроверяй Опа *Expression* ВСолянава РофланЗдарова   *Sequence of Operators*   СтрашнаВырубай НЫА                |
|  **12** | ДобрыйПочанток       | Input operator. Puts user input from command line into the Variable.                                                                                          | ДобрыйПочанток Опа *Var* ВСолянава НЫА                                                                             |
|  **13** | НАНАНА               | Output operator. Puts ASCII code that contains in Variable into the command line.                                                                             | НАНАНА Опа *Var* ВСолянава НЫА                                                                                     |
|  **14** | ОпаФКу               | Output operator. Puts Number that contains in Variable into the command line.                                                                                 | ОпаФКу Опа *Var* ВСолянава НЫА                                                                                     |
|  **15** | ЛучшийВМиреЗаРаботой | Cycle Operator. While Expression is not equal 0 program performs executes of operators enclosed in figured brackets.                                          | ЛучшийВМиреЗаРаботой Опа *Expression* ВСолянава РофланЗдарова     *Sequence of Operators* СтрашнаВырубай НЫА         |
|  **16** | ЭтоМНЕ               | Assignment Operator. Puts Expression into the Variable.                                                                                                       | *Var* ЭтоМНЕ *Expression* НЫА                                                                                        |
|  **17** | НЫА                  | Operators sepator.                                                                                                                                            | *Operator* НЫА                                                                                                     |
|  **18** | ПодрубалитиИзРеалити | Function Operator. Creates function, with some name and with any number of arguments. If called executes sequence of operators enclosed in figured brackets.  | ПодрубалитиИзРеалити *FuncName* Опа *FuncArgs* ВСолянава РофланЗдарова     *Sequence of Operators* СтрашнаВырубай НЫА  |
|  **19** | ХАХАХА               | Function arguments separator.                                                                                                                                 | ПодрубалитиИзРеалити *FuncName* Опа *Arg1* ХАХАХА *Arg2* ХАХАХА ... *ArgN* ВСолянава                                     |
|  **20** | Опа                  | Open Bracket.                                                                                                                                                 | Опа *Expression* ВСолянава                                                                                         |
|  **21** | ВСолянава            | Close Bracket.                                                                                                                                                | Опа *Expression* ВСолянава                                                                                         |
|  **22** | РофланЗдарова        | Open Figure Bracket.                                                                                                                                          | РофланЗдарова     *Sequence of Operators* СтрашнаВырубай НЫА                                                       |
|  **23** | СтрашнаВырубай       | Close Figure Bracket.                                                                                                                                         | РофланЗдарова     *Sequence of Operators* СтрашнаВырубай НЫА                                                       |
|  **24** | ОтлетайОчередянра    | Return Operator. Returns Num\|Var from function and exites it.                                                                                                | ОтлетайОчередняра *Num\|Var* НЫА                                                                                   |
### Examples of Programms in Papich Language
**#1 Quadratic equation solver:** \

**Assembler code:**
...
**Inputs & Outputs:**
**#2 Factorial Calculator:** \
...
**Assembler code:**
...
**Inputs & Outputs:**

**💘 Dmit DREC MIPT 2023**
