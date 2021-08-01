# VSA MicroCode

This repo contains a c++ program to generate the microcode for the very simple architecture

## List of all control signals:

  * HLT Halt System Clock

  * ACI Accumulator In
  * ACO Accumulator Out

  * ALS ALU Subtract
  * FRI Flags Register In

  * OTI Output Register In

  * PCO Program Counter Out
  * PCI Program Counter Increment
  * PCJ Program Counter Branch
  * PCZ Program Counter Branch (if zero)
  * PCF Program Counter Branch (if overflow)

  * MRI Memory Address Register In
  * RMI Random Acess Memory In
  * RMO Random Acess Memory Out

  * IRI Instruction Register In
  * CLR Control Logic Microstep reset


## List of all instructions:

  * 0 0000 HLT          Halt System Clock
  * 1 0001 LDA  <Addr>  Load from Adress into Accumulator
  * 2 0010 STA  <Addr>  Store from Accumulator into Address
  * 3 0011 ADA  <Addr>  Add to Accumulator from Address
  * 4 0100 SBA  <Addr>  Subtract to Accumulator from Address
  * 5 0101 JPO  <Addr>  Jump to Address if Overflow
  * 6 0110 JPZ  <Addr>  Jump to Address if Zero
  * 7 0111 OTA  <Addr>  Output Memory at Address

  * 8 1000 JMP  <Addr>  Jump to Address
  * 9 1001 LDI  <Data>  Load from Imidiate into Accumulator
  * A 1010 NOP          No Operation
  * B 1011 ADI  <Data>  Add to Accumulator from Imidiate
  * C 1100 SBI  <Data>  Subtract to Accumulator from Imidiate
  * D 1101 JNO  <Addr>  Jump to Address if not Overflow
  * E 1110 JNZ  <Addr>  Jump to Address if not Zero
  * F 1111 OTC          Output Accumulator


## Instruction Explanation:

  * MicroCode  Instruction   Control
  * Step       Register      Word

    0000       XXXX          PCO MRI<br/>
    0001       XXXX          RMO IRI<br/>

  * HLT Halt System Clock:

    0010       0000          HLT <br/>

  * LDA Load from Adress into Accumulator

    0010       0001          PCI<br/>
    0011       0001          PCO MRI<br/>
    0100       0001          RMO MRI<br/>
    0101       0001          ACO ACI ALS<br/>
    0110       0001          RMO ACI PCI<br/>
    0111       0001          CLR<br/>

  * STA Store from Accumulator into Address

    0010       0010          PCI<br/>
    0011       0010          PCO MRI<br/>
    0100       0010          RMO MRI<br/>
    0101       0010          ACO RMI PCI<br/>
    0110       0010          CLR<br/>

  * ADA Add to Accumulator from Address

    0010       0011          PCI<br/>
    0011       0011          PCO MRI<br/>
    0100       0011          RMO MRI<br/>
    0101       0011          RMO ACI FRI PCI<br/>
    0110       0011          CLR<br/>

  * SBA Subtract to Accumulator from Address

    0010       0100          PCI<br/>
    0011       0100          PCO MRI<br/>
    0100       0100          RMO MRI<br/>
    0101       0100          RMO ACI ALS FRI PCI<br/>
    0110       0100          CLR<br/>

  * JPO Jump to Address if Overflow

    0010       0101          PCI<br/>
    0011       0101          PCO MRI<br/>
    0100       0101          PCI<br/>
    0101       0101          RMO PCO<br/>
    0110       0101          CLR<br/>

  * JPZ Jump to Address if Zero

    0010       0110          PCI<br/>
    0011       0110          PCO MRI<br/>
    0100       0110          PCI<br/>
    0101       0110          RMO PCZ<br/>
    0110       0110          CLR<br/>

  * OTA Output Memory at Address

    0010       0111          PCI<br/>
    0011       0111          PCO MRI<br/>
    0100       0111          RMO MRI<br/>
    0101       0111          RMO OTI PCI<br/>
    0110       0111          CLR<br/>

  * JMP Jump to Address

    0010       1000          PCI<br/>
    0011       1000          PCO MRI<br/>
    0100       1000          RMO PCJ<br/>
    0101       1000          CLR<br/>

  * LDI Load from Imidiate into Accumulator

    0010       1001          PCI<br/>
    0011       1001          PCO MRI<br/>
    0100       1001          ACO ACI ALS<br/>
    0101       1001          RMO ACI PCI<br/>
    0110       1001          CLR<br/>

  * NOP No Operation

    0010       1010          PCI<br/>
    0011       1010          CLR<br/>

  * ADI Add to Accumulator from Imidiate

    0010       1011          PCI<br/>
    0010       1011          PCO MRI<br/>
    0010       1011          RMO ACI FRI PCI<br/>
    0010       1011          CLR<br/>

  * SBI Subtract to Accumulator from Imidiate

    0010       1100          PCI<br/>
    0010       1100          PCO MRI<br/>
    0010       1100          RMO ACI ALS FRI PCI<br/>
    0010       1100          CLR<br/>

  * JNO Jump to Address if not Overflow

    0010       1101          PCI<br/>
    0011       1101          PCO MRI<br/>
    0100       1101          PCI<br/>
    0101       1101          RMO PCO PCJ<br/>
    0110       1101          CLR<br/>

  * JNZ Jump to Address if not Zero

    0010       1110          PCI<br/>
    0011       1110          PCO MRI<br/>
    0100       1110          PCI<br/>
    0101       1110          RMO PCZ PCJ<br/>
    0110       1110          CLR<br/>

  * OTC Output Accumulator

    0010       1111          ACO OTI PCI<br/>
    0011       1111          CLR<br/>

