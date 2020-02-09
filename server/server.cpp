#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

int main(int argc, char ** argv) {
    if(argc!=2){
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    char * endp;
    long port = strtol(argv[1], &endp, 10);
    if(*endp || port > 65535 || port < 1){
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    
    // Przygotowanie adresu przekazywanego do funkcji bind.
    // Połączenia przychodzące pod podany w funkcji bind adres będą trafiać do nasłuchującego gniazda.
    // Użycie INADDR_ANY określa, że połączenia przychodzące pod dowolny adres (i wskazany port) będą odbierane.
    sockaddr_in myAddr {};
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons((uint16_t)port);
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket failed");
        return 1;
    }
 
    int fail = bind(fd, (sockaddr*) &myAddr, sizeof(myAddr));
    if(fail){
        perror("bind failed");
        return 1;
    }
    
    // listen żąda od systemu operacyjnego by podane gniazdo rozpoczęło czekanie na przychodzące połączenia
    // drugi argument określa ilość połączeń które czekają na odebranie ich funkcja accept
    fail = listen(fd, 1);
    if(fail){
        perror("listen failed");
        return 1;
    }
    
    // accept zwraca deskryptor pliku nawiązanego połączenia (czekając na to połączenie, jeśli żadnego nie ma w kolejce)
    int clientFd = accept(fd, nullptr, nullptr);
    if(clientFd == -1){
        perror("accept failed");
        return 1;
    }

    close(fd);
    
    //auto currTime = std::time(nullptr);
	//char * text = std::ctime(&currTime);
    
    int count = write(clientFd, "&&0&&A to jest randomowy tekst\n", strlen("&&0&&A to jest randomowy tekst\n"));
    printf("polaczono z klientem\n");
    if(count != (int) strlen("&&0&&A to jest randomowy tekst\n"))
        perror("write failed");

    count = write(clientFd, "&&1&&0&&A to jest kolejny randomowy tekst\n", strlen("&&1&&2&&A to jest kolejny randomowy tekst\n"));
    printf("polaczono z klientem\n");
    if(count != (int) strlen("&&1&&2&&A to jest kolejny randomowy tekst\n"))
        perror("write failed");

    count = write(clientFd, "&&1&&1&&A to jest 1 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
    printf("polaczono z klientem\n");
    if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
        perror("write failed");

    count = write(clientFd, "&&1&&2&&A to jest 2 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
    printf("polaczono z klientem\n");
    if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
        perror("write failed");
    
    count = write(clientFd, "&&1&&3&&A to jest 3 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
    printf("polaczono z klientem\n");
    if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
        perror("write failed");
    
    count = write(clientFd, "&&1&&4&&A to jest 4 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
    printf("polaczono z klientem\n");
    if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
        perror("write failed");

    count = write(clientFd, "&&1&&5&&A to jest 5 randomowy tekst\n", strlen("&&1&&2&&A to jest 5 randomowy tekst\n"));
    printf("polaczono z klientem\n");
    if(count != (int) strlen("&&1&&2&&A to jest 5 randomowy tekst\n"))
        perror("write failed");



    ifstream ifs;
    ifs.open("script.txt", ios::in);
    string line;
    getline(ifs, line);

    while (!ifs.eof()) {
	 /*count = write(clientFd, &line, 200);
    	 printf("polaczono z klientem\n");
    	 if(count != 200)
        	perror("write failed");*/
	 cout << line << endl;

	 getline(ifs, line);
     }

    ifs.close();
 
    shutdown(clientFd, SHUT_RDWR);
    close(clientFd);
    
    return 0;
}

