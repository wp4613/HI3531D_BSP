#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdint.h>

int main(int argc,char** argv)
{
    uint8_t arr[256];
    uint8_t ver[4];
    FILE* f= NULL;
    if(argc < 2){
        printf("参数错误\n");
        exit(0);
    }
    char cmd[128];
    sprintf(cmd,"echo '%s'|cut -d ' ' -f3",argv[1]);
    f= popen(cmd,"r");
    fread(arr,sizeof(arr),1,f);
    pclose(f);

    sscanf((char*)arr,"V%hhu.%hhu.%hhu.%hhu",&ver[0],&ver[1],&ver[2],&ver[3]);

    memset(arr,0,sizeof(arr));
    memcpy(arr+4,ver,4);
    sprintf((char*)arr+8,"%s",argv[1]);
    f = fopen("./mcu_version.bin","wb");
    fwrite(arr,sizeof(arr),1,f);
    fclose(f);
    return 0;
}
