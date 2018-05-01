#include <arpa/inet.h>   
#include <netinet/in.h>  // for sockaddr_in  
#include <sys/types.h>   // for socket  
#include <sys/socket.h>  // for socket  
#include <stdio.h>       // for printf  
#include <stdlib.h>      // for exit  
#include <string.h>      // for bzero  
#include <unistd.h>      // for close
#include <dlfcn.h>

#define LIB_SEGMENT_SO "libsegment.so"

#define HELLO_WORLD_SERVER_PORT    6666   
#define LENGTH_OF_LISTEN_QUEUE 5  
#define BUFFER_SIZE 4096  

typedef char* (*SEGMENT_FUNC)(char*);
   
int main(int argc, char *argv[])  
{  
    //设置一个socket地址结构server_addr,代表服务器internet地址, 端口  
    struct sockaddr_in server_addr;  
    bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0  
    server_addr.sin_family = AF_INET;  
    //server_addr.sin_addr.s_addr = htons(INADDR_ANY); 
    server_addr.sin_addr.s_addr = inet_addr("xx.xx.xx.xx");//设置服务器IP
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);  
   
    //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket  
    int server_socket = socket(PF_INET,SOCK_STREAM,0);  
    if( server_socket < 0)  
    {  
        printf("Create Socket Failed!\n");  
        exit(1);  
    }  
    else
    {
        printf("Create Socket Success!\n");
    }
       
    int opt =1;  
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  
  
    //把socket和socket地址结构联系起来  
    int ret = bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(ret < 0)  
    {  
        printf("Server Bind Port : %d Failed!\n", HELLO_WORLD_SERVER_PORT);   
        exit(2);  
    }  
    else
    {
        printf("Server Bind Port : %d Success!\n", HELLO_WORLD_SERVER_PORT); 
    }
   
    //server_socket用于监听  
    ret = listen(server_socket, LENGTH_OF_LISTEN_QUEUE);
    if (ret < 0)  
    {  
        printf("Server Listen Failed!\n");   
        exit(3);  
    }  
    else
    {
        printf("Server Listen Success!\n");
    }

    //服务器端要一直运行
    while (1)   
    {  
        //定义客户端的socket地址结构client_addr  
        struct sockaddr_in client_addr;  
        socklen_t length = sizeof(client_addr);  

        //接受一个到server_socket代表的socket的一个连接  
        //如果没有连接请求,就等待到有连接请求--这是accept函数的特性  
        //accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信  
        //new_server_socket代表了服务器和客户端之间的一个通信通道  
        //accept函数把连接到的客户端信息填写到客户端的socket地址结构client_addr中  
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);  
        if ( new_server_socket < 0)  
        {  
            printf("Server Accept Failed!\n");  
            break;  
        }  
        else
        {
            printf("Server Accept Success!\n");
        }
        
        void* handle = dlopen(LIB_SEGMENT_SO, RTLD_LAZY);
        if (!handle)
        {
            printf("dlopen error\n");
        }

        dlerror();
        SEGMENT_FUNC pf = dlsym(handle, "segment");
        
        char buffer[BUFFER_SIZE];  
        char segment[BUFFER_SIZE];
        while (1)
        {
            bzero(buffer, BUFFER_SIZE); 
            bzero(segment, BUFFER_SIZE); 
            length = recv(new_server_socket, buffer, BUFFER_SIZE, 0);  
            if (length <= 0)  
            {  
                printf("Server Recieve Data Failed!\n");  
                break;  
            }  
            else
            {
                printf("Recieved Data is :%s\n",buffer);
            }
            sprintf(segment, "%s", pf(buffer));

            printf("Segmented Data is:%s\n\n", segment);

            //发送buffer中的字符串到new_server_socket,实际是给客户端  
            ret = send(new_server_socket, segment, BUFFER_SIZE,0);
            if(ret < 0)  
            {  
                printf("Server Send Data Failed\n");  
                break;  
            }  
        }
        //关闭动态链接库
        dlclose(handle);
        //关闭与客户端的连接  
        close(new_server_socket);  
    }  
    //关闭监听用的socket  
    close(server_socket);  
    return 0;  
}  