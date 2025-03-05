#include "GameServer.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

GameServer::GameServer() : server_fd(0) {
    // 서버 소켓 초기화
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        throw runtime_error("소켓 생성 실패: " + string(strerror(errno)));
    }

    // 소켓 옵션 설정 (재사용 가능하도록)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(server_fd);
        throw runtime_error("소켓 옵션 설정 실패: " + string(strerror(errno)));
    }

    // 서버 주소 설정
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
}

GameServer::~GameServer() {
    if (server_fd > 0) {
        close(server_fd);
    }
}

bool GameServer::savePlayerData(const Player& player) {
    try {
        ofstream file("players.txt", ios::app);
        if (!file) {
            throw runtime_error("파일 열기 실패");
        }

        file << player << endl;
        file.close();

        cout << "플레이어 저장 완료: " << player.getName() << " (" << player.getJob() << ")" << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "파일 저장 실패: " << e.what() << endl;
        return false;
    }
}

void GameServer::start() {
    // 소켓 바인딩
    if (::bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) == -1) {
        throw runtime_error("바인딩 실패: " + string(strerror(errno)));
    }

    // 리스닝 시작
    if (listen(server_fd, SOMAXCONN) == -1) {
        throw runtime_error("리스닝 실패: " + string(strerror(errno)));
    }

    cout << "서버가 " << PORT << " 포트에서 실행 중..." << endl;

    while (true) {
        try {
            handleConnection();
        }
        catch (const exception& e) {
            cerr << "연결 처리 중 오류: " << e.what() << endl;
        }
    }
}

void GameServer::handleConnection() {
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // 클라이언트 연결 수락
    int new_socket = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), &addrlen);
    if (new_socket == -1) {
        throw runtime_error("연결 수락 실패: " + string(strerror(errno)));
    }

    // RAII 소켓 래퍼
    unique_ptr<int, function<void(int*)>> socket_guard(&new_socket, [](const int* sock) {
        if (sock) {
            close(*sock);
        }
    });

    // 클라이언트 요청 읽기
    ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        throw runtime_error("데이터 읽기 실패: " + string(strerror(errno)));
    }
    buffer[bytes_read] = '\0';

    string request(buffer);
    cout << "클라이언트 요청: " << request << endl;

    // 요청 데이터 검증 및 정제
    request.erase(remove_if(request.begin(), request.end(), ::isspace), request.end());

    // 플레이어 정보 파싱
    size_t space = request.find(' ');
    if (space == string::npos || space == 0 || space == request.length() - 1) {
        throw invalid_argument("잘못된 요청 형식");
    }

    string name = request.substr(0, space);
    string job = request.substr(space + 1);

    // 플레이어 생성 및 저장
    Player newPlayer(name, job);
    if (!savePlayerData(newPlayer)) {
        throw runtime_error("플레이어 데이터 저장 실패");
    }

    // 응답 전송
    string response = "플레이어 생성 완료\n";
    if (send(new_socket, response.c_str(), response.size(), 0) == -1) {
        throw runtime_error("응답 전송 실패: " + string(strerror(errno)));
    }
}
