

Dmit's Virtual Machine (DVM)
=========================
Description
--------------

This virtual machine is the part of C language, Data Structures and Compilers course in MIPT. 
DVM executes any programs that are represented on its bytecode.
 For programming on DVM I created Dmit's ASM that translates ASM commands into their bytecodes.

 How It Works?
--------------
DVM can execute different commands with their unique bytecode.
Each command has its number. All commands has one byte size. The bytecode also depends on the arguments type.
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-0pky{border-color:inherit;text-align:left;vertical-align:top}
.tg .tg-dvpl{border-color:inherit;text-align:right;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-0pky">Bit Number</th>
    <th class="tg-c3ow">7</th>
    <th class="tg-c3ow">6</th>
    <th class="tg-c3ow">5</th>
    <th class="tg-c3ow">4</th>
    <th class="tg-c3ow">3</th>
    <th class="tg-c3ow">2</th>
    <th class="tg-c3ow">1</th>
    <th class="tg-c3ow">0</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0pky">Meaning</td>
    <td class="tg-0pky">Padding to 0 bit</td>
    <td class="tg-0pky">Arg is Memory Ref</td>
    <td class="tg-0pky">Arg is Register</td>
    <td class="tg-0pky">Arg is Imm Number</td>
    <td class="tg-dvpl">Command Number</td>
  </tr>
  <tr>
    <td class="tg-0pky">Byte Code</td>
    <td class="tg-c3ow"></td>
    <td class="tg-c3ow">1 bit</td>
    <td class="tg-c3ow">1 bit</td>
    <td class="tg-c3ow">1 bit</td>
    <td class="tg-c3ow">1 bit</td>
    <td class="tg-c3ow">1 bit</td>
    <td class="tg-c3ow">1 bit</td>
    <td class="tg-c3ow">1 bit</td>
  </tr>
</tbody>
</table>
### Examples
**#1: 0110 0011** \
Right four bytes of this command are 0011 => PUSH command has 3 number. \
Left four bytes of this command are 0110 => It has memory and register arg. \
This command is **PUSH [RAX]**. \
**#2: 0000 0101** \
 Right four bytes of this command are 0101 => ADD command has 5 num. \
Left four bytes of this command are 0000 => It has no args. \
This command is **ADD**. 
### Execution
After defining exact command by reading its byte code, DVM executes it.
## Dmit's Assembler
If you want to write programs, there is Dmit's Assembler for it.  Assembler translates commands into their bytecodes. There is a table that represent each command purpose and number:
| Command Num | Command Name |                                                           Purpose                                                          | Args Count |
|-------------|:------------:|:--------------------------------------------------------------------------------------------------------------------------:|:----------:|
|      0      |      IN      | Gets Num input from Command Line and then Pushes it on Stack                                                                 |      0     |
|      1      |      OUT     | Pop's Num from Stack and then displays it on screen                                                                        |      0     |
|      2      |   PUSH Imm   | Pushes Num on Stack                                                                                                        |      1     |
|      3      |   PUSH Reg   | Gets Num from Reg and then pushes it on Stack                                                                              |      1     |
|      3      |  PUSH [Imm]  | Pushes Num which is located in Memory on Imm adress                                                                        |      1     |
|      3      |  PUSH [Reg]  | Pushes Num which is located at Memory address stored in Reg                                                         |      1     |
|      4      |      DIV     | Gets two Nums from Stack and DIVIDES them, pushing result on Stack                                                     |      0     |
|      5      |      ADD     | Gets two Nums from Stack and SUMS them, pushing result on Stack                                                         |      0     |
|      6      |      SUB     | Gets two Nums from Stack and SUBTRACTS them, pushing result on Stack                                                  |      0     |
|      7      |      MUL     | Gets two Nums from Stack and MULTIPLIES  them, pushing result on Stack                                                  |      0     |
|      8      |      POW     | Gets two Nums from Stack and then RAISES one number to the POWER of a second, pushing result on Stack 
|      9      |      SIN     | Gets Num from Stack and then take it as an argument of SINE FUNCTION, pushing result on Stack                              |      0     |
|      10     |      COS     | Gets Num from Stack and then take it as an argument of COSINE FUNCTION, pushing result on Stack                            |      0     |
|      11     |     SQRT     | Gets Num from Stack and then take SQUARE ROOT of it, pushing result on Stack                                                |      0     |
|      12     |    POP Reg   | Gets Num from Stack and then puts it in Reg                                                                                |      1     |
|      12     |   POP [Imm]  | Gets Num from Stack and then puts it in  Memory, in imm address                                                            |      1     |
|      12     |   POP [Reg]  | Gets num from Stack and then puts it in Memory, at address stored in Reg                                                |      1     |
|      13     |   JMP label  | Moves IP from current position to label                                                                                    |      1     |
|      14     |   JA label   | Gets two Nums from Stack and then moves IP from current position to label, if first Num is GREATER then other Num          |      1     |
|      15     |   JB label   | Gets two Nums from Stack and then moves IP from current position to label, if first Num is LESS then other Num             |      1     |
|      16     |   JAE label  | Gets two Nums from Stack and then moves IP from current position to label, if first Num is GREATER OR EQUAL then other Num |      1     |
|      17     |   JBE label  | Gets two Nums from Stack and then moves IP from current position to label, if first Num is LESS OR EQUAL then other Num    |      1     |
|      18     |   JE label   | Gets two Nums from Stack and then moves IP from current position to label, if these two nums are EQUAl                     |      1     |
|      19     |   JNE label  | Gets two Nums from Stack and then moves IP from current position to label, if these two nums are NOT EQUAL                 |      1     |
|      20     |  CALL label  | Moves IP from current position to label offset and pushes OLD IP to Stack                                                         |      1     |
|      21    |      RET     | Gets Num from Stack and then moves IP from current position to that Num                                                    |      0     |
|      22     |     OUTC     | Gets Num from Stack and then displays symbol that matches this Num in ASCII table                                          |      0     |
|      23     |     DRAW     | Displays Memory on screen. If a non-zero value contains in this Memory Cell it displays '*', otherwise '.'            |      0     |
|      -1     |      HLT     | Exits Program                                                                                                              |      0     |

## Program Examples
### #1 Quadratic Equation Solver:
### #2 Factorial Calculator:
### #3 Circle Drawer:
 **MIPT 2023**
