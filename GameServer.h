#ifndef GAME_SERVER_H
#define GAME_SERVER_H
#include <netinet/in.h>
#include <unistd.h>


#include "Player.h"

class GameServer {
private:
    static const int PORT = 8080;
    int server_fd;
    sockaddr_in address{};

    // 플레이어 데이터 저장 메서드
    bool savePlayerData(const Player& player);

    // 소켓 닫기 함수 객체
    struct SocketCloser {
        void operator()(int* socket) {
            if (socket) {
                close(*socket);
            }
        }
    };

public:
    // 생성자
    GameServer();

    // 소멸자
    ~GameServer();

    // 서버 시작
    void start();

    // 클라이언트 연결 처리
    void handleConnection();

    // 복사 생성자와 대입 연산자 삭제
    GameServer(const GameServer&) = delete;
    GameServer& operator=(const GameServer&) = delete;
};

#endif // GAME_SERVER_H