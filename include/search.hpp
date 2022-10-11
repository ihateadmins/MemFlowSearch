#include <iostream>
#include <vector>


class Search
{
private:
    /* data */
public:
    ProcessInstance<CBox<void>, CArc<void>>* process;
    std::vector<MemoryRange> memory_range_vec;
    std::vector<uint64_t> memory_hit_vec;

    void setprocess(ProcessInstance<CBox<void>, CArc<void>>* process);
    bool getpagemap(ProcessInstance<CBox<void>, CArc<void>>* proc);
    bool getpagemap();
    bool searchmemory(std::string message);
    bool searchrepetition(std::string message);

    int checkarray(uint8_t* buf, int size,std::string bytes,uint64_t baseaddr);
    
    bool clear();

    Search();
    ~Search();
};

int Search::checkarray(uint8_t* buf, int size,std::string bytes,uint64_t baseaddr)
{
	for (uint64_t i = 0; i < size; i++)
	{
		if (buf[i] == bytes[0])
		{
			for (int j = 0; j < bytes.length(); j++)
			{
				if (buf[i + j] != bytes[j])
				{
					break;
				}
				if (j == bytes.length() - 1)
				{
                    this->memory_hit_vec.push_back(baseaddr+i);
                    std::cout << "hits: " << "0x" << std::uppercase << std::hex << baseaddr+i << std::endl;
                    std::cout << &buf[i] << std::endl;
				}
			}
		}
	}


	return 0;
}

bool Search::searchmemory(std::string message)
{
    for (auto &&info : this->memory_range_vec)
    {
        CSliceMut<uint8_t> buf;
        buf.data = new u_int8_t[info._1];
        buf.len = info._1;
        this->process->read_raw_into(info._0,buf);

        this->checkarray(buf.data, info._1,message, info._0);

        delete buf.data;        
    }
    




    /*




        CSliceMut<uint8_t> buf;
        buf.data = new u_int8_t[info._1];
        buf.len = info._1;
        proc->read_raw_into(info._0,buf);
        delete buf.data;
        */
    return true;
}

bool Search::searchrepetition(std::string message){
    for (auto &&i : this->memory_hit_vec)
    {
        
    }
    
}

bool Search::getpagemap(ProcessInstance<CBox<void>, CArc<void>>* proc)
{
    this->memory_range_vec.clear();
    imem gap = 0x1000000;
    proc->virt_page_map_range(gap,0x0,0x800000000000,[proc,this](MemoryRange info) mutable {
        this->memory_range_vec.push_back(info);
        return true;
	});
    std::cout << this->memory_range_vec.size() << std::endl;
    return true;
}

bool Search::getpagemap()
{
    this->memory_range_vec.clear();
    imem gap = 0x1000000;
    this->process->virt_page_map_range(gap,0x0,0x800000000000,[this](MemoryRange info) mutable {
        this->memory_range_vec.push_back(info);
        return true;
	});
    std::cout << this->memory_range_vec.size() << std::endl;
    return true;
    
}

void Search::setprocess(ProcessInstance<CBox<void>, CArc<void>>* process)
{
    this->process=process;
    return;
}



bool Search::clear()
{
    this->process=nullptr;
    this->memory_range_vec.clear();
    this->memory_hit_vec.clear();
}

Search::Search()
{

}

Search::~Search()
{
}