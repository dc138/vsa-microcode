/*
 *
 * MicroCode generator for the Very Simple Architecture
 * Copyright (C) 2021 Antonio de Haro
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * */

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
