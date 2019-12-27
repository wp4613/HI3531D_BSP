#ifndef FACTORY_H
#define FACTORY_H
#include<map>
#include<list>
#include<string>
#include "element.h"
#include "cxs_comm.h"
#include"cxstreamer.h"
namespace CXS{
    class Factory;

    /**************************************************************************
      * @brief  工厂描述结构
      * @note   提供工厂的名称、初始化方法、逆初始化方法和获得工厂实例方法
    **************************************************************************/
    struct FactoryDescriptor{
        const char* factoryName;
        /**************************************************************************
          * @brief  初始化方法
          * @return 0 成功;<0 失败代码
        **************************************************************************/
        int (*init)();
        void (*deInit)();
        Factory* (*getFactoryInstance)();
    };

    struct ElementDescriptor{
        const char* factoryName;
        const char* elementClassName;
        int (*init)();
        void (*deInit)();
        Element* (*createElementInstance)();
    };

    class Factory
    {

        typedef Factory*(*GetFactoryInstanceFuncPtr)();
        typedef Element*(*CreateElementFuncPtr)();

        static void registerFactory(FactoryDescriptor* descriptor);
        static void addElementDesc(ElementDescriptor* descriptor);
        void registerElement(ElementDescriptor* descriptor);
        friend class __ModuleWrap;

        std::map<std::string,ElementDescriptor*> elemDescGetFuncPtrMap;

    protected:

    public:
        Factory();
        int init();
        virtual ~Factory(){}

        static Factory* getFactoryInstanceByName(const char* factoryName);
        Element* createElementByName(const char* elementName)
        {
            if(elemDescGetFuncPtrMap[elementName])
                return elemDescGetFuncPtrMap[elementName]->createElementInstance();
            else
            {
                ERROR("Not found element:%s\n",elementName);

            }
            return  nullptr;
        }
    };


    class __ModuleWrap{
    public:
        __ModuleWrap(FactoryDescriptor* descriptor){
            Factory::registerFactory(descriptor);
        }
        __ModuleWrap(ElementDescriptor* descriptors,int count){
            for(int i = 0;i < count;i++)
                Factory::addElementDesc(&descriptors[i]);
                //Factory::getFactoryInstanceByName(descriptors[i].factoryName)->registerElement(descriptors);
        }
    };

    #define DECLARE_FACTORY_GET_INSTANCE(FACTORY_CLASS)  \
    static Factory* getInstance()   \
    {   \
        static Factory* _instance = nullptr;\
        if(_instance == nullptr)\
        {\
            _instance = new FACTORY_CLASS;\
            _instance->init();  \
        }\
        return _instance;\
    }

    #define REGISTER_FACTORY(factoryDescriptor) static __ModuleWrap _moduleWrap(factoryDescriptor)
    #define REGISTER_ELEMENT(elementDescriptors,count) static __ModuleWrap _moduleWrap(elementDescriptors,count)



}

#endif // FACTORY_H
