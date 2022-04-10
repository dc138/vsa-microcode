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

uint8_t microcode_low[256];
uint8_t microcode_high[256];

uint8_t invert_nibble(uint8_t in) {
  uint8_t b0 = (in & 0b0001) >> 0;
  uint8_t b1 = (in & 0b0010) >> 1;
  uint8_t b2 = (in & 0b0100) >> 2;
  uint8_t b3 = (in & 0b1000) >> 3;

  return (b0 << 3) | (b1 << 2) | (b2 << 1) | (b3 << 0);
}

void set_addr(uint8_t ins, uint8_t step, uint16_t bits) {
  microcode_low[(invert_nibble(step) << 4) | (invert_nibble(ins) << 0)] = CWL(bits);
  microcode_high[(ins << 4) | (step << 0)] = CWH(bits);
}

int main() {
  std::memset((void *)microcode_low, 0, sizeof(uint8_t) * 256);
  std::memset((void *)microcode_high, 0, sizeof(uint8_t) * 256);

  uint8_t i = 0;
  uint8_t j = 0;

  // clang-format off
  // Ensure invalid locations redirect back to a valid instruction code

  do {
    j = 0;

    do {
      set_addr(i, j, CLR);
    } while (j++ < 0xF);
  } while (i++ < 0xF);

  i = 0;

  // Common instruction fetch cycle

  do {
    set_addr(i, 0b0000, PCO|MRI);
    set_addr(i, 0b0001, RMO|IRI);
  } while (i++ < 0xF);

  // HLT Halt System Clock

  set_addr(0b0000, 0b0010, HLT);

  // LDA Load from Adress into Accumulator

  set_addr(0b0001, 0b0010, PCI);
  set_addr(0b0001, 0b0011, PCO|MRI);
  set_addr(0b0001, 0b0100, RMO|MRI);
  set_addr(0b0001, 0b0101, ACO|ACI|ALS);
  set_addr(0b0001, 0b0110, RMO|ACI|PCI);

  // STA Store from Accumulator into Address

  set_addr(0b0010, 0b0010, PCI);
  set_addr(0b0010, 0b0011, PCO|MRI);
  set_addr(0b0010, 0b0100, RMO|MRI);
  set_addr(0b0010, 0b0101, ACO|RMI|PCI);
  
  // ADA Add to Accumulator from Address

  set_addr(0b0011, 0b0010, PCI);
  set_addr(0b0011, 0b0011, PCO|MRI);
  set_addr(0b0011, 0b0100, RMO|MRI);
  set_addr(0b0011, 0b0101, RMO|ACI|FRI|PCI);

  // SBA Subtract to Accumulator from Address

  set_addr(0b0100, 0b0010, PCI);
  set_addr(0b0100, 0b0011, PCO|MRI);
  set_addr(0b0100, 0b0100, RMO|MRI);
  set_addr(0b0100, 0b0101, RMO|ACI|ALS|FRI|PCI);

  // JPO Jump to Address if Overflow

  set_addr(0b0101, 0b0010, PCI);
  set_addr(0b0101, 0b0011, PCO|MRI);
  set_addr(0b0101, 0b0100, PCI);
  set_addr(0b0101, 0b0101, RMO|PCF);

  // JPZ Jump to Address if Zero

  set_addr(0b0110, 0b0010, PCI);
  set_addr(0b0110, 0b0011, PCO|MRI);
  set_addr(0b0110, 0b0100, PCI);
  set_addr(0b0110, 0b0101, RMO|PCZ);

  // OTA Output Memory at Address

  set_addr(0b0111, 0b0010, PCI);
  set_addr(0b0111, 0b0011, PCO|MRI);
  set_addr(0b0111, 0b0100, RMO|MRI);
  set_addr(0b0111, 0b0101, RMO|OTI|PCI);

  // JMP Jump to Address

  set_addr(0b1000, 0b0010, PCI);
  set_addr(0b1000, 0b0011, PCO|MRI);
  set_addr(0b1000, 0b0100, RMO|PCJ);

  // LDI Load from Imidiate into Accumulator

  set_addr(0b1001, 0b0010, PCI);
  set_addr(0b1001, 0b0011, PCO|MRI);
  set_addr(0b1001, 0b0100, ACO|ACI|ALS);
  set_addr(0b1001, 0b0101, RMO|ACI|PCI);

  // NOP No Operation

  set_addr(0b1010, 0b0010, PCI);

  // ADI Add to Accumulator from Imidiate

  set_addr(0b1011, 0b0010, PCI);
  set_addr(0b1011, 0b0011, PCO|MRI);
  set_addr(0b1011, 0b0100, RMO|ACI|FRI|PCI);

  // SBI Subtract to Accumulator from Imidiate

  set_addr(0b1100, 0b0010, PCI);
  set_addr(0b1100, 0b0011, PCO|MRI);
  set_addr(0b1100, 0b0100, RMO|ACI|ALS|FRI|PCI);

  // JNO Jump to Address if not Overflow

  set_addr(0b1101, 0b0010, PCI);
  set_addr(0b1101, 0b0011, PCO|MRI);
  set_addr(0b1101, 0b0100, PCI);
  set_addr(0b1101, 0b0101, RMO|PCF|PCJ);

  // JNZ Jump to Address if not Zero

  set_addr(0b1110, 0b0010, PCI);
  set_addr(0b1110, 0b0011, PCO|MRI);
  set_addr(0b1110, 0b0100, PCI);
  set_addr(0b1110, 0b0101, RMO|PCZ|PCJ);

  // OTC Output Accumulator

  set_addr(0b1111, 0b0010, ACO|OTI|PCI);

  // clang-format on

  std::ofstream out_low;
  out_low.open("microcode_low.rom", std::ofstream::binary);
  out_low.write((char *)microcode_low, sizeof(uint8_t) * 256);
  out_low.close();

  std::ofstream out_high;
  out_high.open("microcode_high.rom", std::ofstream::binary);
  out_high.write((char *)microcode_high, sizeof(uint8_t) * 256);
  out_high.close();

  return 0;
}
