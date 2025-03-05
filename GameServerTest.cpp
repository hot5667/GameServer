#include <fstream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include "GameServer.h"
#include "Player.h"

using namespace std;

class GameServerTest : public ::testing::Test {
protected:
    GameServer server;

    void SetUp() override {
        // 테스트 환경 준비 (필요 시 파일 삭제)
        remove("players.txt");
    }

    void TearDown() override {
        // 테스트 후 정리 작업
        remove("players.txt");
    }
};

// ✅ 1️⃣ 플레이어 데이터 저장 테스트
TEST_F(GameServerTest, SavePlayerDataTest) {
    Player testPlayer("TestUser", "Warrior");

    // 저장 시도
    // EXPECT_TRUE(server.savePlayerData(testPlayer));

    // 파일에 저장되었는지 확인
    ifstream file("players.txt");
    ASSERT_TRUE(file.is_open());

    string line;
    getline(file, line);
    file.close();

    // 예상된 데이터가 저장되었는지 확인
    EXPECT_FALSE(line.empty());
    EXPECT_NE(line.find("TestUser"), string::npos);
    EXPECT_NE(line.find("Warrior"), string::npos);
}

// ✅ 2️⃣ 소켓 생성 여부 테스트
TEST_F(GameServerTest, ServerSocketCreationTest) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_NE(sock, -1) << "소켓 생성 실패";

    if (sock != -1) {
        close(sock);
    }
}

// ✅ 3️⃣ 클라이언트 요청 검증 테스트 (입력 검증)
TEST_F(GameServerTest, HandleConnectionInputValidationTest) {
    string invalidRequest = "InvalidFormat";  // 공백 없이 단일 문자열

    EXPECT_THROW({
        size_t space = invalidRequest.find(' ');
        if (space == string::npos) {
            throw invalid_argument("잘못된 요청 형식");
        }
    }, invalid_argument);
}

// ✅ 4️⃣ 플레이어 생성 및 응답 전송 테스트
TEST_F(GameServerTest, HandleConnectionTest) {
    // 서버 소켓 생성
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(client_sock, -1) << "클라이언트 소켓 생성 실패";

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    in_port_t PORT = 69600;
    server_addr.sin_port = htons(PORT);

    // 서버 실행 (별도 스레드에서 실행해야 실제 수락 가능)
    thread serverThread([&]() {
        EXPECT_NO_THROW(server.start());
    });

    sleep(1); // 서버 실행 대기

    // 서버 연결 시도
    int result = connect(client_sock, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));
    EXPECT_EQ(result, 0) << "서버 연결 실패";

    // 요청 전송
    string request = "TestUser Warrior";
    send(client_sock, request.c_str(), request.size(), 0);

    // 응답 받기
    char buffer[1024] = {0};
    ssize_t bytes_received = read(client_sock, buffer, sizeof(buffer) - 1);
    EXPECT_GT(bytes_received, 0) << "서버 응답 없음";
    EXPECT_NE(string(buffer).find("플레이어 생성 완료"), string::npos);

    close(client_sock);
    serverThread.detach();
}
