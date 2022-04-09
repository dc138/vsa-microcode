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
uint8_t microcode_low[256];
uint8_t microcode_high[256];

void set_addr(uint16_t addr, uint16_t bits) {
  microcode[(addr*2)  ] = CWH(bits);
  microcode[(addr*2)+1] = CWL(bits);

  microcode_low[addr] = CWL(bits);
  microcode_high[addr] = CWH(bits);
}

int main() {
  std::memset((void *)microcode, 0, sizeof(uint8_t) * 512);
  std::memset((void *)microcode_low, 0, sizeof(uint8_t) * 256);
  std::memset((void *)microcode_high, 0, sizeof(uint8_t) * 256);
  uint8_t i = 0;

  // clang-format off
  // Ensure invalid locations redirect back to a valid instruction code

  do {
    set_addr(i, CLR);
  } while (i++ < 0xFF);

  // Common instruction fetch cycle

  do {
    set_addr(0b00000000+i, PCO|MRI);
    set_addr(0b00010000+i, RMO|IRI);
  } while (i++ < 0xF);

  // HLT Halt System Clock

  set_addr(0b00100000, HLT);

  // LDA Load from Adress into Accumulator

  set_addr(0b00100001, PCI);
  set_addr(0b00110001, PCO|MRI);
  set_addr(0b01000001, RMO|MRI);
  set_addr(0b01010001, ACO|ACI|ALS);
  set_addr(0b01100001, RMO|ACI|PCI);

  // STA Store from Accumulator into Address

  set_addr(0b00100010, PCI);
  set_addr(0b00110010, PCO|MRI);
  set_addr(0b01000010, RMO|MRI);
  set_addr(0b01010010, ACO|RMI|PCI);
  
  // ADA Add to Accumulator from Address

  set_addr(0b00100011, PCI);
  set_addr(0b00110011, PCO|MRI);
  set_addr(0b01000011, RMO|MRI);
  set_addr(0b01010011, RMO|ACI|FRI|PCI);

  // SBA Subtract to Accumulator from Address

  set_addr(0b00100100, PCI);
  set_addr(0b00110100, PCO|MRI);
  set_addr(0b01000100, RMO|MRI);
  set_addr(0b01010100, RMO|ACI|ALS|FRI|PCI);

  // JPO Jump to Address if Overflow

  set_addr(0b00100101, PCI);
  set_addr(0b00110101, PCO|MRI);
  set_addr(0b01000101, PCI);
  set_addr(0b01010101, RMO|PCF);

  // JPZ Jump to Address if Zero

  set_addr(0b00100110, PCI);
  set_addr(0b00110110, PCO|MRI);
  set_addr(0b01000110, PCI);
  set_addr(0b01010110, RMO|PCZ);

  // OTA Output Memory at Address

  set_addr(0b00100111, PCI);
  set_addr(0b00110111, PCO|MRI);
  set_addr(0b01000111, RMO|MRI);
  set_addr(0b01010111, RMO|OTI|PCI);

  // JMP Jump to Address

  set_addr(0b00101000, PCI);
  set_addr(0b00111000, PCO|MRI);
  set_addr(0b01001000, RMO|PCJ);

  // LDI Load from Imidiate into Accumulator

  set_addr(0b00101001, PCI);
  set_addr(0b00111001, PCO|MRI);
  set_addr(0b01001001, ACO|ACI|ALS);
  set_addr(0b01011001, RMO|ACI|PCI);

  // NOP No Operation

  set_addr(0b00101010, PCI);

  // ADI Add to Accumulator from Imidiate

  set_addr(0b00101011, PCI);
  set_addr(0b00111011, PCO|MRI);
  set_addr(0b01001011, RMO|ACI|FRI|PCI);

  // SBI Subtract to Accumulator from Imidiate

  set_addr(0b00101100, PCI);
  set_addr(0b00111100, PCO|MRI);
  set_addr(0b01001100, RMO|ACI|ALS|FRI|PCI);

  // JNO Jump to Address if not Overflow

  set_addr(0b00101101, PCI);
  set_addr(0b00111101, PCO|MRI);
  set_addr(0b01001101, PCI);
  set_addr(0b01011101, RMO|PCF|PCJ);

  // JNZ Jump to Address if not Zero

  set_addr(0b00101110, PCI);
  set_addr(0b00111110, PCO|MRI);
  set_addr(0b01001110, PCI);
  set_addr(0b01011110, RMO|PCZ|PCJ);

  // OTC Output Accumulator

  set_addr(0b00101111, ACO|OTI|PCI);

  // clang-format on

  std::ofstream out;
  out.open("microcode.rom", std::ofstream::binary);
  out.write((char *)microcode, sizeof(uint8_t) * 512);
  out.close();

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
