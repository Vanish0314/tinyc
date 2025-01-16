#include <cstddef>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

#include "cgltf.h"
#include "tiny_gltf.h"


class Profile
{
public:
    Profile(const char* name = "default") : m_name(name), m_startTime(0), m_endTime(0)
    {
        m_startTime = GetCurrentTime();
        // m_startStackMemory = GetCurrentStackMemoryUsage();
        m_startHeapMemory = Profile::m_totalHeapMemoryUsage;
        m_startDynamicMemory = Profile::m_dynamicMemoryUsage;
    }
    ~Profile()
    {
        m_endTime = GetCurrentTime();
        // m_endStackMemory = GetCurrentStackMemoryUsage();
        m_endHeapMemory = Profile::m_totalHeapMemoryUsage;
        m_endDynamicMemory = Profile::m_dynamicMemoryUsage;
        printStats();
    }
    double GetDuration() const
    {
        return (m_endTime - m_startTime) * 1000.0;
    }
    size_t GetMemoryDelta() const
    {
        return (m_endStackMemory - m_startStackMemory) + (m_endHeapMemory - m_startHeapMemory) + (m_dynamicMemoryUsage - m_startDynamicMemory);
    }
public:
    static size_t GetTotalMemoryUsage()
    {
        return m_totalHeapMemoryUsage;
    }

private:
    double GetCurrentTime()
    {
        using namespace std::chrono;
        auto now = high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        return duration_cast<std::chrono::duration<double>>(duration).count();
    }

    size_t GetCurrentStackMemoryUsage() // return bytes
    {
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0)
            return usage.ru_maxrss;
        return 0;
    }

    void printStats() const
    {
        std::cout << "Profile [" << m_name << "]:" << std::endl
                  << "  Time: " << GetDuration() << " ms" << std::endl
                  << "  Memory delta: " << GetMemoryDelta() / 1024.0 / 1024.0 << " MB" << std::endl;
    }

private:
    std::string m_name;        
    double m_startTime;        
    double m_endTime;
    size_t m_startStackMemory;      
    size_t m_endStackMemory;   
    size_t m_startHeapMemory;
    size_t m_endHeapMemory;
    size_t m_startDynamicMemory;
    size_t m_endDynamicMemory;
public:
    static size_t m_dynamicMemoryUsage;
    static size_t m_totalHeapMemoryUsage;
};
size_t Profile::m_dynamicMemoryUsage = 0;
size_t Profile::m_totalHeapMemoryUsage = 0;

void* my_alloc_func(void* user, cgltf_size size)
{
    void* ptr = malloc(size);
    Profile::m_totalHeapMemoryUsage += size;
    // std::cout << "Allocating " << size << " bytes at " << ptr << std::endl;
    return ptr;
}
void* operator new(std::size_t size) {
    Profile::m_dynamicMemoryUsage += size;
    return std::malloc(size);
}
void* operator new[](std::size_t size) {
    Profile::m_dynamicMemoryUsage += size;
    return std::malloc(size);
}
void operator delete(void* ptr) noexcept {
    std::free(ptr);
}
void operator delete[](void* ptr) noexcept {
    std::free(ptr);
}



// 移除了node的材质，tiny成功加载，cgltf无法加载
// static const std::string abPath = "/Users/vanish/Desktop/WorkPlace/tinyc/Assets/glTF-Sample-Assets/Models/DragonDispersion/glTF/DragonDispersion.gltf";

// //均正常加载
// static const std::string relativePath = "./Assets/glTF-Sample-Assets/Models/DamagedHelmet/glTF/DamagedHelmet.gltf";
// static const std::string absolutePath = "/Users/vanish/Desktop/WorkPlace/tinyc/Assets/glTF-Sample-Assets/Models/DamagedHelmet/glTF/DamagedHelmet.gltf";

// blender导出无材质多条龙，cgltf无法加载
// static const std::string abPath = "/Users/vanish/Desktop/WorkPlace/tinyc/Assets/dragon.glb"; // cgltf无法加载

// static const std::string abPath = "/Users/vanish/Desktop/WorkPlace/tinyc/Assets/OneDragon.glb";

// static const std::string abPath = "/Users/vanish/Desktop/WorkPlace/tinyc/Assets/dragon.gltf";

//Sponza
// static const std::string abPath = "/Users/vanish/Desktop/WorkPlace/tinyc/Assets/Sponza.glb";
static const std::string abPath = "/Users/vanish/Desktop/WorkPlace/tinyc/Assets/Sponza.gltf";



int main ()
{
    Profile profile("main");

    // 文件大小
    {
        std::filesystem::path filePath(abPath);
        std::cout << "File size:"
                  << std::filesystem::file_size(filePath) / 1024.0 / 1024.0 << "MB" <<std::endl;
    }

    // cgltf
    {
        Profile profile("cgltf");

        cgltf_options options = {};
        options.memory.alloc_func = my_alloc_func;
        cgltf_data *data = NULL;
        cgltf_result result = cgltf_parse_file(&options, abPath.c_str(), &data);
        if (result == cgltf_result_success)
        	cgltf_free(data);
    }

    // tinygltf
    {
        Profile profile("tinygltf");

        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;
        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, abPath.c_str());
        if (!warn.empty()) {
            std::cout << "Warn: " << warn << std::endl;
        }
        if (!err.empty()) {
            std::cout << "Err: " << err << std::endl;
        }
        if (!ret) {
            std::cout << "Failed to parse glTF\n";
        }
    }
}