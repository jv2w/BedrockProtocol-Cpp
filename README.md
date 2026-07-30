# BedrockProtocol-Cpp

마인크래프트 베드락 서버가 게임과 주고받는 패킷을 플러그인에서 다루기 위한 라이브러리입니다.

서버는 패킷을 원시 데이터로만 넘겨주기 때문에 그대로는 내용을 알 수 없습니다. 이 라이브러리는 패킷
**229종 전부**의 형식을 알고 있어서, 오가는 패킷을 **읽고, 고치고, 취소하고, 새로 만들어 보낼 수**
있게 합니다.

새로 만들어 보낸 패킷은 받은 플레이어에게만 반영되며 서버 월드는 그대로입니다.

지원 구동기는 **[Endstone](https://github.com/EndstoneMC/endstone) v0.11.6** 입니다.

## 원본 프로젝트를 잇습니다

이 라이브러리는 [PocketMine-MP의 BedrockProtocol](https://github.com/pmmp/BedrockProtocol)을 C++로 옮긴
것입니다. 원본은 PHP로 만들어졌고, 현재 **보관 처리되어 더 이상 업데이트되지 않습니다.**

**이 저장소가 그 뒤를 잇습니다.** 마인크래프트가 새 버전을 내면 여기서 계속 따라갑니다.

이름과 구조를 원본과 똑같이 맞춰두었으므로 원본의 문서와 예제를 그대로 참고할 수 있습니다.

- 현재 대응: 마인크래프트 **1.26.30** (프로토콜 1001)
- 라이선스: LGPL-3.0

## 쓰는 법

플러그인의 `CMakeLists.txt`에 아래를 넣습니다. Endstone을 불러온 **뒤에** 놓아야 합니다.

```cmake
include(FetchContent)
FetchContent_Declare(bedrock_protocol
        GIT_REPOSITORY https://github.com/<owner>/BedrockProtocol-Cpp.git GIT_TAG main)
FetchContent_MakeAvailable(bedrock_protocol)

target_link_libraries(my_plugin PRIVATE bedrock_protocol_bridge)
```

그다음 관심 있는 패킷 종류를 등록해 두면, 그 패킷이 지나갈 때마다 코드가 불립니다. 오는 것은
`onReceive`, 가는 것은 `onSend`입니다.

```cpp
auto &interceptor = bedrock_protocol::bridge::PacketInterceptor::get();
interceptor.enable(*this);                          // onEnable 안에서 1회만

interceptor.onSend<bedrock_protocol::TextPacket>([](auto &event) {
    event.view();       // 읽기만 - 서버는 원래 바이트를 그대로 씁니다
    event.mutate();     // 고치기 - 여기서 바꾼 값으로 다시 만들어 보냅니다
    event.cancel();     // 없던 일로
});
```

`view()`와 `mutate()`가 나뉘어 있는 이유는, 읽기만 할 때는 패킷을 다시 만드는 비용을 치르지 않기
위해서입니다. 새 패킷을 직접 보낼 때는 `interceptor.sendPacket(player, packet)`을 씁니다.

더 자세한 사용법은 [PORTING.md](PORTING.md), 검증 범위와 현황은 [PORTING_STATUS.md](PORTING_STATUS.md)에 있습니다.
