#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <sys/uio.h>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

#ifdef __AVX2__
#include <immintrin.h>
#endif

#define ELF_PROGRAM_HEADER_OFFSET 0x20
#define ELF_PROGRAM_HEADER_ENTRY_SIZE 0x36
#define ELF_PROGRAM_HEADER_NUM_ENTRIES 0x38

#define ELF_SECTION_HEADER_OFFSET 0x28
#define ELF_SECTION_HEADER_ENTRY_SIZE 0x3A
#define ELF_SECTION_HEADER_NUM_ENTRIES 0x3C

#define ELF_DYNAMIC_SECTION_PHT_TYPE 0x02

namespace Memory {

  static bool write(pid_t pid, uintptr_t address, void* buf, size_t size) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buf;
    local[0].iov_len = size;
    remote[0].iov_base = (void*)address;
    remote[0].iov_len = size;

    // if (process_vm_writev(pid, local, 1, remote, 1, 0) == -1) {
    //   std::cout << "Error write: " << errno << '\n';
    //   std::cout << "Error write address: " << std::hex << address << '\n';
    // }

    return (process_vm_writev(pid, local, 1, remote, 1, 0) == size);
  }

  static bool read(pid_t pid, uintptr_t address, void* buf, size_t size) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buf;
    local[0].iov_len = size;
    remote[0].iov_base = (void*)address;
    remote[0].iov_len = size;

    if (process_vm_readv(pid, local, 1, remote, 1, 0) == -1) {
      std::cout << "Error read: " << errno << '\n';
      std::cout << "Error read address: " << std::hex << address << '\n';
    }

    return (process_vm_readv(pid, local, 1, remote, 1, 0) == size);
  }

  static uintptr_t module_base_address(pid_t proc_pid, std::string module_name) {
    std::ifstream infile(("/proc/" + std::to_string(proc_pid) + "/maps").c_str());
    std::string line;

    bool found = false;

    std::string concatTemp;
    while (std::getline(infile, line)) {
      for (int i = 0; i <= line.length(); i++) {
        concatTemp = "";
        for (int h = 0; h < module_name.length(); h++) {
          concatTemp += line[i+h];
        }
        if (concatTemp == module_name) { found = true; break; }
      }

      if (found == true) {break;}
    }

    concatTemp = "";
    for (int i = 0; i <= line.length(); i++) {
      if (line[i] == '-') { break; }
      concatTemp += line[i];
    }

    // Use stringstream for conversion
    std::stringstream ss;
    ss << std::hex << "0x"+concatTemp;

    // Store the result
    uintptr_t result;
    ss >> result;

    return result;
    
  }

  static uintptr_t module_size(pid_t proc_pid, uintptr_t module_base_address) {
    uintptr_t section_header_offset = (uintptr_t)NULL;
    Memory::read(proc_pid, module_base_address + ELF_SECTION_HEADER_OFFSET, &section_header_offset, sizeof(unsigned long));
    uintptr_t section_header_entry_size = (uintptr_t)NULL;
    Memory::read(proc_pid, module_base_address + ELF_SECTION_HEADER_ENTRY_SIZE, &section_header_entry_size, sizeof(unsigned short));
    uintptr_t section_header_num_entries = (uintptr_t)NULL;
    Memory::read(proc_pid, module_base_address + ELF_SECTION_HEADER_NUM_ENTRIES, &section_header_num_entries, sizeof(unsigned short));

    return section_header_offset + section_header_entry_size * section_header_num_entries;
  }
  
  static std::vector<unsigned char> read_bytes(pid_t proc_pid, uintptr_t address, unsigned long count) {
    const auto path = "/proc/" + std::to_string(proc_pid) + "/mem";
    const int file = open(path.c_str(), O_RDONLY);
    std::vector<unsigned char> buffer(count);
    pread(file, buffer.data(), count, static_cast<long>(address));
    close(file);
    return buffer;
  }

  static std::vector<unsigned char> dump_module(pid_t proc_pid, uintptr_t module_address) {
    const uintptr_t module_size = Memory::module_size(proc_pid, module_address);
    
    // should be 1 gb
    if (module_size == 0 || module_size > 1000000000) {
      printf("could not dump module at %lu\n", module_address);
      return std::vector<unsigned char>();
    }
    
    return read_bytes(proc_pid, module_address, module_size);
  }

  static uintptr_t scan_pattern(pid_t proc_pid, std::vector<unsigned char> pattern, std::vector<bool> mask, uintptr_t length, uintptr_t module_address) {
    const std::vector<unsigned char> module = Memory::dump_module(proc_pid, module_address);
    
    if (module.size() < 500) {
      return (uintptr_t)NULL;
    }

    const unsigned long module_end = module.size();
    for (unsigned long i = 0; i < module_end - length; i++) {
      bool found = true;
      for (unsigned long j = 0; j < length; j++) {
	if (mask[j] && module[i + j] != pattern[j]) {
	  found = false;
	  break;
	}
      }
      if (found) {
	return module_address + i;
      }
    }

    printf("Bad pattern\n");
    return (uintptr_t)NULL;
  }

  static uintptr_t segment_from_pht(pid_t proc_pid, uintptr_t module_address, uintptr_t tag) {
    uintptr_t first_entry = (uintptr_t)NULL;
    Memory::read(proc_pid, module_address + ELF_PROGRAM_HEADER_OFFSET, &first_entry, sizeof(uintptr_t));
    first_entry += module_address;
    
    uintptr_t entry_size = (uintptr_t)NULL;
    Memory::read(proc_pid, module_address + ELF_PROGRAM_HEADER_ENTRY_SIZE, &entry_size, sizeof(unsigned short));

    unsigned short num_entries = (uintptr_t)NULL;
    Memory::read(proc_pid, module_address + ELF_PROGRAM_HEADER_NUM_ENTRIES, &num_entries, sizeof(unsigned short));
    
    for (uintptr_t i = 0; i < num_entries; i++) {
      const uintptr_t entry_address = first_entry + i * entry_size;
      unsigned int entry = (unsigned)NULL;
      Memory::read(proc_pid, entry_address, &entry, sizeof(unsigned int));
      if (entry == tag) {
        return entry_address;
      }
    }
    printf("could not find tag in header table\n");
    return (uintptr_t)NULL;
  }

  static uintptr_t address_from_dynamic_section(pid_t proc_pid, uintptr_t module_address, uintptr_t tag) {
    const uintptr_t dynamic_section_offset_opt = Memory::segment_from_pht(proc_pid, module_address, ELF_DYNAMIC_SECTION_PHT_TYPE);
    if (!dynamic_section_offset_opt) {
      return (uintptr_t)NULL;
    }
    const uintptr_t dynamic_section_offset = dynamic_section_offset_opt;

    const uintptr_t register_size = 8;
    uintptr_t address = (uintptr_t)NULL;
    Memory::read(proc_pid, dynamic_section_offset + 2 * register_size, &address, sizeof(uintptr_t));
    address += module_address;

    while (true) {
      const uintptr_t tag_address = address;
      uintptr_t tag_value = (uintptr_t)NULL;
      Memory::read(proc_pid, tag_address, &tag_value, sizeof(uintptr_t));
      
      if (tag_value == 0) {
	printf("breaking out of the loop oh no!\n");
	break;
      }
      if (tag_value == tag) {
	uintptr_t value = (uintptr_t)NULL;
	Memory::read(proc_pid, tag_address + register_size, &value, sizeof(uintptr_t));
	return value;
      }

      address += register_size * 2;
    }

    printf("could not find tag\n");
    return (uintptr_t)NULL;
  }

  static std::string read_string(pid_t proc_pid, uintptr_t address) {
    std::string value;
    value.reserve(64);
    for (unsigned long i = address; i < address + 512; i += sizeof(uintptr_t)) {
      uintptr_t chunk = (uintptr_t)NULL;
      Memory::read(proc_pid, i, &chunk, sizeof(uintptr_t));

      // https://graphics.stanford.edu/~seander/bithacks.html
      if (((chunk - 0x0101010101010101ULL) & ~chunk & 0x8080808080808080ULL) != 0) {
	// at least one byte is null, process each individually
	// Process each byte individually.
	for (int offset = 0; offset < 8; ++offset) {
	  unsigned char byte = (chunk >> (offset * 8)) & 0xFF;
	  if (byte == 0) return value;
	  value.push_back(byte);
	}
      } else {
	// no null, just append the chunk
	for (int offset = 0; offset < 8; ++offset) {
	  unsigned char byte = (chunk >> (offset * 8)) & 0xFF;
	  value.push_back(byte);
	}
      }
    }
    return value;
  }

  static uintptr_t module_export(pid_t proc_pid, uintptr_t module_address, std::string export_name) {
    const uintptr_t add = 0x18;

    const uintptr_t string_table_opt = address_from_dynamic_section(proc_pid, module_address, 0x05);
    const uintptr_t symbol_table_opt = address_from_dynamic_section(proc_pid, module_address, 0x06);
    if (!string_table_opt || !symbol_table_opt) {
      return (uintptr_t)NULL;
    }
    const uintptr_t string_table = string_table_opt;
    uintptr_t symbol_table = symbol_table_opt;

    symbol_table += add;

    unsigned int symbol = (unsigned)NULL;
    Memory::read(proc_pid, symbol_table, &symbol, sizeof(unsigned int));
    
    while (symbol != 0) {
      uintptr_t st_name = (uintptr_t)NULL;
      Memory::read(proc_pid, symbol_table, &st_name, sizeof(unsigned int));
      const std::string name = Memory::read_string(proc_pid, string_table + st_name);
      if (name == export_name) {
	uintptr_t value = (uintptr_t)NULL;
	Memory::read(proc_pid, symbol_table + 0x08, &value, sizeof(uintptr_t));
	return value + module_address;
      }
      symbol_table += add;
      Memory::read(proc_pid, symbol_table, &symbol, sizeof(unsigned int));
    }

    printf("could not find export\n");
    return (uintptr_t)NULL;
  }

  
  static uintptr_t relative_address(pid_t proc_pid, uintptr_t instruction, uintptr_t offset, uintptr_t instruction_size) {
    int rip_address = (uintptr_t)NULL;
    Memory::read(proc_pid, instruction + offset, &rip_address, sizeof(int));
    return instruction + instruction_size + rip_address;
  }

  static uintptr_t interface_offset(pid_t proc_pid, uintptr_t module_address, std::string interface_name) {
    const uintptr_t create_interface_opt = module_export(proc_pid, module_address, "CreateInterface");
    if (!create_interface_opt) {
      return (uintptr_t)NULL;
    }
    const uintptr_t create_interface = create_interface_opt;
    const uintptr_t export_address_opt = Memory::relative_address(proc_pid, create_interface, 0x01, 0x05) + 0x10;
    if (!export_address_opt) {
      return (uintptr_t)NULL;
    }
    const uintptr_t export_address = export_address_opt;

    uintptr_t interface_entry = (uintptr_t)NULL;
    unsigned int exp = (unsigned)NULL;
    Memory::read(proc_pid, export_address + 0x03, &exp, sizeof(unsigned int));
    Memory::read(proc_pid, export_address + 0x07 + exp, &interface_entry, sizeof(uintptr_t));
    
    while (true) {
      uintptr_t entry_name_address = (uintptr_t)NULL;
      Memory::read(proc_pid, interface_entry + 8, &entry_name_address, sizeof(uintptr_t));
      const std::string entry_name = read_string(proc_pid, entry_name_address);
      if (entry_name.rfind(interface_name) != std::string::npos) {
        uintptr_t vfunc_address = (uintptr_t)NULL;
	Memory::read(proc_pid, interface_entry, &vfunc_address, sizeof(uintptr_t));
	unsigned int instruction = (unsigned)NULL;
	Memory::read(proc_pid, vfunc_address + 0x03, &instruction, sizeof(unsigned int));
	return instruction + vfunc_address + 0x07;
      }
      Memory::read(proc_pid, interface_entry + 0x10, &interface_entry, sizeof(uintptr_t));
      if (interface_entry == 0) {
	printf("oh nos we broke out of the interface func :(\n");
	break;
      }
    }

    printf("could not find interface offset\n");
    return (uintptr_t)NULL;
  }
}

#endif
