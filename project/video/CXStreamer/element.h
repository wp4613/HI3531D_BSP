#ifndef ELEMENT_H
#define ELEMENT_H
#include <vector>
#include <stdint.h>
#include "cxstreamer.h"
namespace CXS{
    class Element;
    struct Pad{
        Element* parent;
        Pad* linkedElemPad;
        Pad(Element* parent)
        {
            OnAppend = nullptr;
            linkedElemPad = nullptr;
            this->parent = parent;
        }
        void(*OnAppend)(uint8_t* data,int32_t len,Element* elem);
        void append(uint8_t* data,int32_t len)
        {
            if(OnAppend){
                OnAppend(data,len,parent);
            }
        }
    };
    class Element
    {
        int matchElemPad(Element* elem,Pad** pad1,Pad** pad2)
        {
            *pad1 = allocFreeOutPad();
            *pad2 = elem->allocFreeInPad();
            DEBUG("elem1  %p\n",this);
            DEBUG("elem2  %p\n",elem);
            DEBUG("pad1->parent  %p\n",(*pad1)->parent);
            DEBUG("pad2->parent  %p\n",(*pad2)->parent);
            return 0;
        }
    protected:
        std::vector<Pad> mInPads;
        std::vector<Pad> mOutPads;
        Element();
        virtual int linkTo(Element* ){return 0;}
        virtual int startSelf(){return true;}

    public:
        virtual ~Element();
        Pad* allocFreeInPad()
        {
            for(size_t i = 0; i < mInPads.size();i++)
                if(mInPads[i].linkedElemPad == nullptr)
                    return &mInPads[i];
            mInPads.push_back(Pad(this));
            return &mInPads[mInPads.size()-1];
        }
        Pad* allocFreeOutPad()
        {
            for(size_t i = 0; i < mOutPads.size();i++)
                if(mOutPads[i].linkedElemPad == nullptr)
                    return &mOutPads[i];
            mOutPads.push_back(Pad(this));
            return &mOutPads[mOutPads.size()-1];
        }

        virtual int setAttr(const char* attrName,const char* attrVal);
        virtual int setAttr(const char* attrName,int attrVal);
        virtual int getAttr(const char* attrName,int* attrVal);
        virtual int setAttr(const char* attrName,uint8_t* buff,int len)
        {
            attrName = attrName;
            buff =buff;
            len = len;
            return 0;
        }
        virtual int listenEvent(const char* eventName,void (*eventHandler)(void* user_data,void* event_para),void* user_data){
            eventName = eventName;
            eventHandler = eventHandler;
            user_data = user_data;
            return 0;
        }
        int stop(){return 0;}
        int start(){
            if(startSelf() == 0 )
            {
                for(size_t i = 0;i < mOutPads.size();i++)
                {
                    DEBUG("mOutPads[i].linkedElemPad->parent %u %p\n",(uint32_t)i,mOutPads[i].linkedElemPad);
                    DEBUG("mInPads[0].parent %p\n",mOutPads[i].linkedElemPad);

                    if(mOutPads[i].linkedElemPad && mOutPads[i].linkedElemPad->parent->start() != 0)
                    {
                        stop();
                        return -1;
                    }
                }
            }
            return 0;
        }
        int link(Element* elem){
            Pad* pad1;
            Pad* pad2;
            if(matchElemPad(elem,&pad1,&pad2) == 0)
            {

                if(linkTo(elem) == 0)
                {
                    pad1->linkedElemPad = pad2;
                    pad2->linkedElemPad = pad1;
                    DEBUG("pad1.linkedElemPad->parent  %p\n",pad1->linkedElemPad->parent);
                    DEBUG("pad2.linkedElemPad->parent  %p\n",pad2->linkedElemPad->parent);
                    return 0;
                }

            }
            return -1;
        }
        virtual Pad* requestPad( ){
            return nullptr;
        }
    };
}
#endif // ELEMENT_H
