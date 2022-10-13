#include "include/memflow.hpp"
#include "include/search.hpp"

#include <iostream>
#include <stdio.h>
#include <vector>

void fmt_arch(char *arch, int n, ArchitectureIdent ident);

int main(int argc, char *argv[]) {
	log_init(LevelFilter::LevelFilter_Info);

	Inventory *inventory = inventory_scan();

	if (!inventory) {
		log_error("unable to create inventory");
		return 1;
	}

	printf("inventory initialized: %p\n", inventory);

	const char *conn_name = argc > 1? argv[1]: "qemu";
	const char *conn_arg = argc > 2? argv[2]: "";
	const char *os_name = argc > 3? argv[3]: "win32";
	const char *os_arg = argc > 4? argv[4]: "";

	ConnectorInstance<> connector, *conn = conn_name[0] ? &connector : nullptr;

	if (conn) {
		if (inventory_create_connector(inventory, conn_name, conn_arg, &connector)) {
			printf("unable to initialize connector\n");
			inventory_free(inventory);
			return 1;
		}

		printf("connector initialized: %p\n", connector.container.instance.instance);
	}

	OsInstance<> os;

	if (inventory_create_os(inventory, os_name, os_arg, conn, &os)) {
		printf("unable to initialize OS\n");
		inventory_free(inventory);
		return 1;
	}

	inventory_free(inventory);

	printf("os initialized: %p\n", os.container.instance.instance);

	auto info = os.info();
	char arch[11];
	fmt_arch(arch, sizeof(arch), info->arch);

	printf("Kernel base: %llx\nKernel size: %llx\nArchitecture: %s\n", info->base, info->size, arch);

	printf("Process List:\n");

	printf("%-4s | %-8s | %-10s | %-10s | %s\n", "Seq", "Pid", "Sys Arch", "Proc Arch", "Name");

	int i = 0;

	os.process_info_list_callback([i](ProcessInfo info) mutable {
		char sys_arch[11];
		char proc_arch[11];

		fmt_arch(sys_arch, sizeof(sys_arch), info.sys_arch);
		fmt_arch(proc_arch, sizeof(proc_arch), info.proc_arch);

		printf("%-4d | %-8d | %-10s | %-10s | %s\n", i++, info.pid, sys_arch, proc_arch, info.name);

		return true;
	});

    ProcessInstance<CBox<void>, CArc<void>> proc;
	std::string targetprocess = "Notepad.exe";
	//
    os.process_by_name(targetprocess, &proc);
	if (!proc.vtbl_process)
	{
		log_error(("unable to "+ targetprocess).c_str());
		return 0;
	}
    Search search;
	search.setprocess(&proc);
	search.getpagemap();
	std::string message = "search word";
	search.searchmemory(message);
	//search.searchmemory((char)12);
	//search.searchmemory((short)12);
	//search.searchmemory((int)36);
	//search.searchrepetition(message);
	//search.searchrepetition((int)12);


	while (true)
	{
		int c = 0;
		int u = 0;
		u_int8_t u8; 
		u_int16_t u16; 
		u_int32_t u32; 
		u_int64_t u64; 
		u_long addr = 0;
		std::cout << "0:search.searchmemory" << std::endl;
		std::cout << "1:search.searchrepetition" << std::endl;
		std::cout << "2:search.writememory" << std::endl;
		std::cout << "3:search.dumpmemory" << std::endl;
		std::cout << "4:search.searchmemorystring" << std::endl;
		std::cout << "5:search.writememory all hits" << std::endl;
		std::cin >> c ;
		switch (c)
		{
		case 0:
			std::cin >> u ;
			switch (u)
			{
			case 1:
				std::cin >> u8 ;
				search.searchmemory(u8);
				break;
			case 2:
				std::cin >> u16 ;
				search.searchmemory(u16);
				break;
			case 4:
				std::cin >> u32 ;
				search.searchmemory(u32);
				break;
			case 8:
				std::cin >> u64 ;
				search.searchmemory(u64);
				break;
			default:
				break;
			}			
			break;
		case 1:
			switch (u)
			{
			case 1:
				std::cin >> u8 ;
				search.searchrepetition(u8);
				break;
			case 2:
				std::cin >> u16 ;
				search.searchrepetition(u16);
				break;
			case 4:
				std::cin >> u32 ;
				search.searchrepetition(u32);
				break;
			case 8:
				std::cin >> u64 ;
				search.searchrepetition(u64);
				break;
			default:
				break;
			}
			break;
		case 2:
			std::cin >> std::hex >> addr;
			std::cin >> std::dec >> u32 ;
			search.writememory(addr,u32);
			break;
		case 3:
			std::cin >> std::hex >> addr;
			std::cin >> std::dec >> u32 ;
			search.dumpmemory(addr,u32);
			break;			
		case 4:
			std::cin >> message ;
			search.searchmemory(message);
			break;	
		case 5:
			std::cin >> std::dec >> u32 ;
			for (auto &&i : search.memory_hit_vec)
			{				
				search.writememory(i,u32);
			}			
			break;		
		default:
			break;
		}
		search.printhits();
	}
	






	return 1;
}

void fmt_arch(char *arch, int n, ArchitectureIdent ident) {
	switch (ident.tag) {
		case ArchitectureIdent::Tag::ArchitectureIdent_X86:
			snprintf(arch, n, "X86_%d", ident.x86._0);
			break;
		case ArchitectureIdent::Tag::ArchitectureIdent_AArch64:
			snprintf(arch, n, "AArch64");
			break;
		default:
			snprintf(arch, n, "Unknown");
	}
}
