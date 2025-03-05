#include "GameServer.h"

int main() {
    try {
        GameServer server;
        server.start();
    }
    catch (const std::exception& e) {
        std::cerr << "서버 시작 중 오류: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}