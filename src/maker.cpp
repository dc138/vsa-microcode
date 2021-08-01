/**
 *  List of all control signals:
 *
 *    HLT Halt System Clock
 *
 *    ACI Accumulator In
 *    ACO Accumulator Out
 *
 *    ALS ALU Subtract
 *    FRI Flags Register In
 *
 *    OTI Output Register In
 *
 *    PCO Program Counter Out
 *    PCI Program Counter Increment
 *    PCJ Program Counter Branch
 *    PCZ Program Counter Branch (if zero)
 *    PCF Program Counter Branch (if overflow)
 *
 *    MRI Memory Address Register In
 *    RMI Random Acess Memory In
 *    RMO Random Acess Memory Out
 *
 *    IRI Instruction Register In
 *    CLR Control Logic Microstep reset
 *
 *
 *  List of all instructions:
 *
 *    0 0000 HLT          Halt System Clock
 *    1 0001 LDA  <Addr>  Load from Adress into Accumulator
 *    2 0010 STA  <Addr>  Store from Accumulator into Address
 *    3 0011 ADA  <Addr>  Add to Accumulator from Address
 *    4 0100 SBA  <Addr>  Subtract to Accumulator from Address
 *    5 0101 JPO  <Addr>  Jump to Address if Overflow
 *    6 0110 JPZ  <Addr>  Jump to Address if Zero
 *    7 0111 OTA  <Addr>  Output Memory at Address
 *
 *    8 1000 JMP  <Addr>  Jump to Address
 *    9 1001 LDI  <Data>  Load from Imidiate into Accumulator
 *    A 1010 NOP          No Operation
 *    B 1011 ADI  <Data>  Add to Accumulator from Imidiate
 *    C 1100 SBI  <Data>  Subtract to Accumulator from Imidiate
 *    D 1101 JNO  <Addr>  Jump to Address if not Overflow
 *    E 1110 JNZ  <Addr>  Jump to Address if not Zero
 *    F 1111 OTC          Output Accumulator
 *
 *
 *  Instruction Explanation:
 *
 *    MicroCode  Instruction   Control
 *    Step       Register      Word
 *
 *    0000       XXXX          PCO MRI
 *    0001       XXXX          RMO IRI
 *
 *    HLT Halt System Clock:
 *
 *    0010       0000          HLT
 *
 *    LDA Load from Adress into Accumulator
 *
 *    0010       0001          PCI
 *    0011       0001          PCO MRI
 *    0100       0001          RMO MRI
 *    0101       0001          ACO ACI ALS
 *    0110       0001          RMO ACI PCI
 *    0111       0001          CLR
 *
 *    STA Store from Accumulator into Address
 *
 *    0010       0010          PCI
 *    0011       0010          PCO MRI
 *    0100       0010          RMO MRI
 *    0101       0010          ACO RMI PCI
 *    0110       0010          CLR
 *
 *    ADA Add to Accumulator from Address
 *
 *    0010       0011          PCI
 *    0011       0011          PCO MRI
 *    0100       0011          RMO MRI
 *    0101       0011          RMO ACI FRI PCI
 *    0110       0011          CLR
 *
 *    SBA Subtract to Accumulator from Address
 *
 *    0010       0100          PCI
 *    0011       0100          PCO MRI
 *    0100       0100          RMO MRI
 *    0101       0100          RMO ACI ALS FRI PCI
 *    0110       0100          CLR
 *
 *    JPO Jump to Address if Overflow
 *
 *    0010       0101          PCI
 *    0011       0101          PCO MRI
 *    0100       0101          PCI
 *    0101       0101          RMO PCO
 *    0110       0101          CLR
 *
 *    JPZ Jump to Address if Zero
 *
 *    0010       0110          PCI
 *    0011       0110          PCO MRI
 *    0100       0110          PCI
 *    0101       0110          RMO PCZ
 *    0110       0110          CLR
 *
 *    OTA Output Memory at Address
 *
 *    0010       0111          PCI
 *    0011       0111          PCO MRI
 *    0100       0111          RMO MRI
 *    0101       0111          RMO OTI PCI
 *    0110       0111          CLR
 *
 *    JMP Jump to Address
 *
 *    0010       1000          PCI
 *    0011       1000          PCO MRI
 *    0100       1000          RMO PCJ
 *    0101       1000          CLR
 *
 *    LDI Load from Imidiate into Accumulator
 *
 *    0010       1001          PCI
 *    0011       1001          PCO MRI
 *    0100       1001          ACO ACI ALS
 *    0101       1001          RMO ACI PCI
 *    0110       1001          CLR
 *
 *    NOP No Operation
 *
 *    0010       1010          PCI
 *    0011       1010          CLR
 *
 *    ADI Add to Accumulator from Imidiate
 *
 *    0010       1011          PCI
 *    0010       1011          PCO MRI
 *    0010       1011          RMO ACI FRI PCI
 *    0010       1011          CLR
 *
 *    SBI Subtract to Accumulator from Imidiate
 *
 *    0010       1100          PCI
 *    0010       1100          PCO MRI
 *    0010       1100          RMO ACI ALS FRI PCI
 *    0010       1100          CLR
 *
 *    JNO Jump to Address if not Overflow
 *
 *    0010       1101          PCI
 *    0011       1101          PCO MRI
 *    0100       1101          PCI
 *    0101       1101          RMO PCO PCJ
 *    0110       1101          CLR
 *
 *    JNZ Jump to Address if not Zero
 *
 *    0010       1110          PCI
 *    0011       1110          PCO MRI
 *    0100       1110          PCI
 *    0101       1110          RMO PCZ PCJ
 *    0110       1110          CLR
 *
 *    OTC Output Accumulator
 *
 *    0010       1111          ACO OTI PCI
 *    0011       1111          CLR
 *
 */

#include <cstring>
#include <fstream>
#include <iostream>

#define FRI 0b0000000000000001
#define ACI 0b0000000000000010
#define HLT 0b0000000000000100
#define PCO 0b0000000000001000
#define OTI 0b0000000000010000
#define ALS 0b0000000000100000
#define PCJ 0b0000000001000000
#define PCI 0b0000000010000000
#define ACO 0b0000000100000000
#define PCF 0b0000001000000000
#define PCZ 0b0000010000000000
#define RMI 0b0000100000000000
#define RMO 0b0001000000000000
#define MRI 0b0010000000000000
#define IRI 0b0100000000000000
#define CLR 0b1000000000000000
#define NON 0b0000000000000000

// Some bits might be active low, this represents the inactive position for all
// bits To make the appropiate control word we just flip the bits that we want
// to be active

#define CWI 0b0111000100011111
#define CWH(bits) ((CWI ^ (bits)) >> 8)
#define CWL(bits) ((CWI ^ (bits)) & 0x00FF)

uint8_t microcode[512];

int main() {
  std::memset((void *)microcode, 0, sizeof(uint8_t) * 512);
  uint8_t i = 0;

  // clang-format off
  // Ensure invalid locations redirect back to a valid instruction code

  do {
    microcode[i*2  ] = CWH(CLR);
    microcode[i*2+1] = CWL(CLR);
  } while (i++ < 0xFF);

  // Common instruction fetch cycle

  do {
    microcode[(0b00000000+i)*2  ] = CWH(PCO|MRI);
    microcode[(0b00000000+i)*2+1] = CWL(PCO|MRI);

    microcode[(0b00010000+i)*2  ] = CWH(RMO|IRI);
    microcode[(0b00010000+i)*2+1] = CWL(RMO|IRI);
  } while (i++ < 0xF);

  // HLT Halt System Clock

  microcode[(0b00100000*2)  ] = CWH(HLT);
  microcode[(0b00100000*2)+1] = CWL(HLT);

  // LDA Load from Adress into Accumulator

  microcode[(0b00100001*2)  ] = CWH(PCI);
  microcode[(0b00100001*2)+1] = CWL(PCI);

  microcode[(0b00110001*2)  ] = CWH(PCO|MRI);
  microcode[(0b00110001*2)+1] = CWL(PCO|MRI);

  microcode[(0b01000001*2)  ] = CWH(RMO|MRI);
  microcode[(0b01000001*2)+1] = CWL(RMO|MRI);

  microcode[(0b01010001*2)  ] = CWH(ACO|ACI|ALS);
  microcode[(0b01010001*2)+1] = CWL(ACO|ACI|ALS);

  microcode[(0b01100001*2)  ] = CWH(RMO|ACI|PCI);
  microcode[(0b01100001*2)+1] = CWL(RMO|ACI|PCI);

  // STA Store from Accumulator into Address

  microcode[(0b00100010*2)  ] = CWH(PCI);
  microcode[(0b00100010*2)+1] = CWL(PCI);

  microcode[(0b00110010*2)  ] = CWH(PCO|MRI);
  microcode[(0b00110010*2)+1] = CWL(PCO|MRI);

  microcode[(0b01000010*2)  ] = CWH(RMO|MRI);
  microcode[(0b01000010*2)+1] = CWL(RMO|MRI);

  microcode[(0b01010010*2)  ] = CWH(ACO|RMI|PCI);
  microcode[(0b01010010*2)+1] = CWL(ACO|RMI|PCI);

  // ADA Add to Accumulator from Address

  microcode[(0b00100011*2)  ] = CWH(PCI);
  microcode[(0b00100011*2)+1] = CWL(PCI);

  microcode[(0b00110011*2)  ] = CWH(PCO|MRI);
  microcode[(0b00110011*2)+1] = CWL(PCO|MRI);

  microcode[(0b01000011*2)  ] = CWH(RMO|MRI);
  microcode[(0b01000011*2)+1] = CWL(RMO|MRI);

  microcode[(0b01010011*2)  ] = CWH(RMO|ACI|FRI|PCI);
  microcode[(0b01010011*2)+1] = CWL(RMO|ACI|FRI|PCI);

  // SBA Subtract to Accumulator from Address

  microcode[(0b00100100*2)  ] = CWH(PCI);
  microcode[(0b00100100*2)+1] = CWL(PCI);

  microcode[(0b00110100*2)  ] = CWH(PCO|MRI);
  microcode[(0b00110100*2)+1] = CWL(PCO|MRI);

  microcode[(0b01000100*2)  ] = CWH(RMO|MRI);
  microcode[(0b01000100*2)+1] = CWL(RMO|MRI);

  microcode[(0b01010100*2)  ] = CWH(RMO|ACI|ALS|FRI|PCI);
  microcode[(0b01010100*2)+1] = CWL(RMO|ACI|ALS|FRI|PCI);

  // JPO Jump to Address if Overflow

  microcode[(0b00100101*2)  ] = CWH(PCI);
  microcode[(0b00100101*2)+1] = CWL(PCI);

  microcode[(0b00110101*2)  ] = CWH(PCO|MRI);
  microcode[(0b00110101*2)+1] = CWL(PCO|MRI);

  microcode[(0b01000101*2)  ] = CWH(PCI);
  microcode[(0b01000101*2)+1] = CWL(PCI);

  microcode[(0b01010101*2)  ] = CWH(RMO|PCF);
  microcode[(0b01010101*2)+1] = CWL(RMO|PCF);

  // JPZ Jump to Address if Zero

  microcode[(0b00100110*2)  ] = CWH(PCI);
  microcode[(0b00100110*2)+1] = CWL(PCI);

  microcode[(0b00110110*2)  ] = CWH(PCO|MRI);
  microcode[(0b00110110*2)+1] = CWL(PCO|MRI);

  microcode[(0b01000110*2)  ] = CWH(PCI);
  microcode[(0b01000110*2)+1] = CWL(PCI);

  microcode[(0b01010110*2)  ] = CWH(RMO|PCZ);
  microcode[(0b01010110*2)+1] = CWL(RMO|PCZ);

  // OTA Output Memory at Address

  microcode[(0b00100111*2)  ] = CWH(PCI);
  microcode[(0b00100111*2)+1] = CWL(PCI);

  microcode[(0b00110111*2)  ] = CWH(PCO|MRI);
  microcode[(0b00110111*2)+1] = CWL(PCO|MRI);

  microcode[(0b01000111*2)  ] = CWH(RMO|MRI);
  microcode[(0b01000111*2)+1] = CWL(RMO|MRI);

  microcode[(0b01010111*2)  ] = CWH(RMO|OTI|PCI);
  microcode[(0b01010111*2)+1] = CWL(RMO|OTI|PCI);

  // JMP Jump to Address

  microcode[(0b00101000*2)  ] = CWH(PCI);
  microcode[(0b00101000*2)+1] = CWL(PCI);

  microcode[(0b00111000*2)  ] = CWH(PCO|MRI);
  microcode[(0b00111000*2)+1] = CWL(PCO|MRI);

  microcode[(0b01001000*2)  ] = CWH(RMO|PCJ);
  microcode[(0b01001000*2)+1] = CWL(RMO|PCJ);

  // LDI Load from Imidiate into Accumulator

  microcode[(0b00101001*2)  ] = CWH(PCI);
  microcode[(0b00101001*2)+1] = CWL(PCI);

  microcode[(0b00111001*2)  ] = CWH(PCO|MRI);
  microcode[(0b00111001*2)+1] = CWL(PCO|MRI);

  microcode[(0b01001001*2)  ] = CWH(ACO|ACI|ALS);
  microcode[(0b01001001*2)+1] = CWL(ACO|ACI|ALS);

  microcode[(0b01011001*2)  ] = CWH(RMO|ACI|PCI);
  microcode[(0b01011001*2)+1] = CWL(RMO|ACI|PCI);

  // NOP No Operation

  microcode[(0b00101010*2)  ] = CWH(PCI);
  microcode[(0b00101010*2)+1] = CWL(PCI);

  // ADI Add to Accumulator from Imidiate

  microcode[(0b00101011*2)  ] = CWH(PCI);
  microcode[(0b00101011*2)+1] = CWL(PCI);

  microcode[(0b00111011*2)  ] = CWH(PCO|MRI);
  microcode[(0b00111011*2)+1] = CWL(PCO|MRI);

  microcode[(0b01001011*2)  ] = CWH(RMO|ACI|FRI|PCI);
  microcode[(0b01001011*2)+1] = CWL(RMO|ACI|FRI|PCI);

  // SBI Subtract to Accumulator from Imidiate

  microcode[(0b00101100*2)  ] = CWH(PCI);
  microcode[(0b00101100*2)+1] = CWL(PCI);

  microcode[(0b00111100*2)  ] = CWH(PCO|MRI);
  microcode[(0b00111100*2)+1] = CWL(PCO|MRI);

  microcode[(0b01001100*2)  ] = CWH(RMO|ACI|ALS|FRI|PCI);
  microcode[(0b01001100*2)+1] = CWL(RMO|ACI|ALS|FRI|PCI);

  // JNO Jump to Address if not Overflow

  microcode[(0b00101101*2)  ] = CWH(PCI);
  microcode[(0b00101101*2)+1] = CWL(PCI);

  microcode[(0b00111101*2)  ] = CWH(PCO|MRI);
  microcode[(0b00111101*2)+1] = CWL(PCO|MRI);

  microcode[(0b01001101*2)  ] = CWH(PCI);
  microcode[(0b01001101*2)+1] = CWL(PCI);

  microcode[(0b01011101*2)  ] = CWH(RMO|PCF|PCJ);
  microcode[(0b01011101*2)+1] = CWL(RMO|PCF|PCJ);

  // JNZ Jump to Address if not Zero

  microcode[(0b00101110*2)  ] = CWH(PCI);
  microcode[(0b00101110*2)+1] = CWL(PCI);

  microcode[(0b00111110*2)  ] = CWH(PCO|MRI);
  microcode[(0b00111110*2)+1] = CWL(PCO|MRI);

  microcode[(0b01001110*2)  ] = CWH(PCI);
  microcode[(0b01001110*2)+1] = CWL(PCI);

  microcode[(0b01011110*2)  ] = CWH(RMO|PCZ|PCJ);
  microcode[(0b01011110*2)+1] = CWL(RMO|PCZ|PCJ);

  // OTC Output Accumulator

  microcode[(0b00101111*2)  ] = CWH(ACO|OTI|PCI);
  microcode[(0b00101111*2)+1] = CWL(ACO|OTI|PCI);

  // clang-format on

  std::ofstream out;
  out.open("microcode.rom", std::ofstream::binary);
  out.write((char *)microcode, sizeof(uint8_t) * 512);
  out.close();

  return 0;
}
