# BedrockProtocol-Cpp

[PocketMine-MP의 BedrockProtocol](https://github.com/pmmp/BedrockProtocol)을 C++20으로 1:1 이식한
라이브러리입니다. **모든 Minecraft Bedrock 패킷을 타입 객체로 디코드하고, 수정하고, 새로 인코드**할 수
있습니다.

원본 PHP와 클래스명·필드명·상수명·직렬화 순서·주석이 모두 일치하므로, PMMP 문서와 코드를 그대로
참조할 수 있습니다.

- 원본: PocketMine-MP `pocketmine/bedrock-protocol`
- 프로토콜: `ProtocolInfo::CURRENT_PROTOCOL` = **1001** (Minecraft `1.26.30`)
- 라이선스: LGPL-3.0 (원본의 파생 저작물)

## 지원 구동기: Endstone

**공식적으로 지원하는 서버 구동기는 [Endstone](https://github.com/EndstoneMC/endstone) (v0.11.6) 하나입니다.**
`bedrock_protocol_bridge` 타깃이 Endstone의 `PacketReceiveEvent`/`PacketSendEvent`에 직접 연결되어, 플러그인이
패킷을 가로채고 수정하고 취소하고 주입할 수 있게 합니다.

다만 **코어 라이브러리는 어떤 구동기에도 의존하지 않습니다.** Endstone을 아는 파일은 `bridge/`의 3개뿐이고,
229개 패킷·NBT 직렬화·인코딩 계층·검증 스위트는 전부 표준 C++20만 씁니다. CMake가 `endstone::endstone`
타깃이 없으면 브릿지를 건너뛰고 코어만 빌드하므로, 프록시·패킷 분석기·자체 서버 구현에도 그대로 쓸 수
있습니다. 다른 구동기 지원은 `bridge/`에 대응물을 하나 더 쓰는 일이지만, 이 저장소가 검증하고 책임지는
범위는 Endstone입니다.

---

## 왜 이 라이브러리가 필요한가

Endstone은 `PacketReceiveEvent`와 `PacketSendEvent` 두 이벤트만 제공하며, 이들이 노출하는 것은
**패킷 ID와 원시 바이트열(`std::string_view`)** 뿐입니다. 공개된 패킷 클래스가 전혀 없어
`getPayload()`를 직접 파싱하지 않으면 패킷 내용을 알 수도, 바꿀 수도 없습니다.

이 라이브러리는 그 사이를 메웁니다.

```
클라이언트 <-> BDS <-> Endstone 이벤트(원시 바이트) <-> [이 라이브러리] <-> 플러그인(타입 객체)
```

---

## 빠른 시작

`CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(bedrock_protocol
        GIT_REPOSITORY https://github.com/<owner>/BedrockProtocol-Cpp.git
        GIT_TAG main)

# endstone 이후에 선언할 것. 브릿지 타깃이 endstone::endstone 을 필요로 하며,
# 그 타깃이 없으면 브릿지 없이 코어 라이브러리만 빌드됩니다.
FetchContent_MakeAvailable(bedrock_protocol)

target_link_libraries(my_plugin PRIVATE bedrock_protocol_bridge)
```

체크아웃을 직접 관리한다면 `add_subdirectory`도 됩니다.

```cmake
add_subdirectory(../BedrockProtocol-Cpp ${CMAKE_BINARY_DIR}/bedrock_protocol)
```

### 툴체인

이 라이브러리 자체는 MSVC와 clang-cl 양쪽에서 빌드되지만, **Endstone v0.11.4부터 Windows에서
clang-cl과 Ninja 생성기를 강제**합니다. 현재 이 프로젝트는 Endstone **v0.11.6** 기준입니다.

필요한 것:

- Visual Studio 컴포넌트 `C++ Clang Compiler for Windows`
  (`Microsoft.VisualStudio.Component.VC.Llvm.Clang`) — clang-cl, lld-link 제공
- Ninja — VS의 CMake 도구 컴포넌트에 포함
- CMake 3.29 이상

플러그인 쪽 `build.bat`이 위 경로를 자동으로 찾아 환경을 구성하는 것이 보통입니다. 이 저장소만
단독으로 빌드할 경우 (브릿지 없이 코어 라이브러리만 나옵니다):

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

`tools/`의 검증 스크립트는 코어 라이브러리를 알아서 찾고, 없으면 위 명령으로 직접 빌드합니다.

Visual Studio 생성기(`-G "Visual Studio ..."`)는 Endstone이 거부하므로 쓸 수 없습니다.

플러그인:

```cpp
#include <bedrock_protocol/bridge/PacketInterceptor.h>
#include <bedrock_protocol/protocol/TextPacket.h>

using bedrock_protocol::bridge::PacketInterceptor;

void MyPlugin::onEnable() {
    auto &interceptor = PacketInterceptor::get();
    interceptor.enable(*this);              // onEnable 안에서 1회만 호출

    interceptor.onSend<bedrock_protocol::TextPacket>([](auto &event) {
        if (event.view().type != bedrock_protocol::TextPacket::TYPE_CHAT) return;
        event.mutate().message = "[filtered] " + event.view().message;
    });
}
```

---

## 핵심 개념

### `view()` 와 `mutate()`

```cpp
const TextPacket &p = event.view();   // 읽기만 — 패킷을 다시 인코딩하지 않음
TextPacket &p = event.mutate();       // 수정 의사 표시 — 콜백 종료 후 재인코딩됨
```

이 구분은 성능상 중요합니다. Endstone은 `setPayload()` 호출 여부를 **포인터 비교**로 감지하며,
호출되는 순간 BDS의 빠른 경로를 벗어나 패킷을 다시 디코드(수신)하거나 다시 직렬화(송신)합니다.
따라서 이 라이브러리는 `mutate()`가 한 번이라도 호출된 경우에만 `setPayload()`를 부릅니다.
읽기만 할 때는 반드시 `view()`를 쓰십시오.

### 취소

```cpp
event.cancel();
```

수신 패킷은 서버가 처리하지 않고, 송신 패킷은 전송되지 않습니다. 취소된 패킷은 수정 여부와
관계없이 재인코딩하지 않습니다.

### 패킷 주입 / 교체

`setPayload()`는 본문만 바꿀 수 있고 **패킷 ID는 바꿀 수 없습니다**(헤더는 Endstone이 보존).
다른 패킷으로 바꾸려면 취소 후 새로 보냅니다.

```cpp
event.cancel();
auto replacement = bedrock_protocol::ToastRequestPacket::create("제목", "내용");
interceptor.sendPacket(*event.getPlayer(), replacement);
```

`sendPacket()`은 자기 자신의 `PacketSendEvent`를 재귀적으로 다시 타지 않도록 내부에서
스레드 로컬 가드를 겁니다.

### PMMP 스타일 핸들러

타입별 구독 대신 PMMP처럼 하나의 핸들러로 받을 수도 있습니다.

```cpp
class MyHandler : public bedrock_protocol::PacketHandlerInterface {
public:
    bool handleText(bedrock_protocol::TextPacket &packet) override {
        packet.message = "...";
        return true;    // true = 수정했으므로 재인코딩하라
    }
};
interceptor.setReceiveHandler(std::make_shared<MyHandler>());
```

핸들러를 설치하면 해당 방향의 **모든** 패킷을 디코드하므로, 관심 있는 타입만 구독하는 쪽이
훨씬 저렴합니다.

---

## 디코드 실패 처리

프로토콜 버전이 어긋나면 패킷 본문 해석이 실패할 수 있습니다. 기본 동작은
`DecodeFailurePolicy::PassThrough` — 경고를 패킷 ID당 한 번만 로그에 남기고, 패킷은
**손대지 않은 채** 그대로 통과시킵니다.

다음 두 경우를 모두 실패로 간주합니다.

1. 디코드 중 예외 발생
2. 디코드 후 **남은 바이트가 있는 경우** — 이는 서버의 패킷 레이아웃이 이 라이브러리가
   생성된 버전과 다르다는 뜻이며, 이대로 재인코딩하면 데이터가 잘려나갑니다.

`setDecodeFailurePolicy(DecodeFailurePolicy::Cancel)` 로 바꾸면 실패한 패킷을 폐기합니다.

---

## 구조

```
include/bedrock_protocol/
  encoding/    ByteBufferReader/Writer, Byte, LE, BE, VarInt   (pmmp/encoding 이식)
  math/        Vector3, Vector2                                (pocketmine/math 이식)
  color/       Color                                           (pocketmine/color 이식)
  uuid/        Uuid                                            (ramsey/uuid 부분 이식)
  nbt/         Tag 계층, TreeRoot, 직렬화기                     (pocketmine/nbt 이식)
  protocol/    229개 패킷, ProtocolInfo, PacketPool,
               PacketHandlerInterface, DataPacket
    serializer/  CommonTypes, NetworkNbtSerializer, BitSet, ...
    types/       ~380개 프로토콜 타입 (PHP 디렉토리 구조 그대로)
  bridge/      PacketInterceptor, PacketEvent                  (신규 - PHP에 대응물 없음)
```

빌드 타깃은 두 개입니다.

- `bedrock_protocol` — 코어. **Endstone 의존성이 전혀 없어** 단독으로 테스트할 수 있습니다.
- `bedrock_protocol_bridge` — Endstone 글루. 코어 + `endstone::endstone`.

### 이름 규칙

파일명과 식별자는 PHP 원본을 그대로 씁니다(`TextPacket.h`, `senderSubId`, `MODE_TELEPORT`).
Endstone의 snake_case 파일명 관례와 다르지만, 업스트림과 1:1로 대조할 수 있다는 점을
우선했습니다. 자세한 규칙은 [PORTING.md](PORTING.md)에 있습니다.

예외는 C 매크로와 이름이 겹치는 **두 개뿐**입니다. 전처리기가 식별자를 치환해버려 컴파일이
불가능하기 때문입니다.

| PHP | 이 라이브러리 | 충돌 상대 |
|---|---|---|
| `DeviceOS::WIN32` | `DeviceOS::OsWIN32` | `<windows.h>` |
| `ProtocolInfo::MINECRAFT_VERSION` | `ProtocolInfo::MINECRAFT_VERSION_DISPLAY` | Endstone `detail.h` |

### 정수 폭

PHP의 `int`는 폭 정보가 없으므로, **그 필드를 읽는 와이어 함수가 C++ 타입을 결정**합니다.
`VarInt::readUnsignedLong` 으로 읽는 필드는 `std::uint64_t`, `LE::readSignedInt` 는
`std::int32_t` 입니다. 직접 시프트로 varint를 구현하지 말고 항상 `encoding::` 함수를 쓰십시오.

---

## 여러 플러그인이 함께 쓸 때

이 라이브러리는 플러그인마다 **정적 링크**되므로 플러그인별로 독립된 인터셉터를 갖습니다.
플러그인 간 조합은 Endstone의 이벤트 우선순위를 통해 자연스럽게 이루어집니다. 우선순위가
앞선 플러그인이 페이로드를 수정하면, 뒤이은 플러그인은 수정된 바이트를 디코드하게 됩니다.

`enable()`의 두 번째 인자로 우선순위를 지정할 수 있습니다.

```cpp
interceptor.enable(*this, endstone::EventPriority::High);
```

---

## 스레드

Endstone은 두 패킷 이벤트를 모두 서버 스레드에서 동기적으로 디스패치합니다. 이 라이브러리는
락을 걸지 않으며 필요하지도 않습니다. 다른 스레드에서 구독하는 것은 지원하지 않습니다.

또한 `getPayload()`가 주는 `string_view`와 `ByteBufferReader`가 반환하는 뷰는 **디스패치 동안만
유효**합니다. 콜백 밖으로 내보내야 할 값은 반드시 `std::string`으로 복사하십시오.

---

## 검증

```
php  tools/gen_fixtures.php        # PHP 참조 구현으로 인코딩 테스트 벡터 생성
tools\run_encoding_test.bat        # 인코딩 계층 검증
python tools/check_symmetry.py     # 전 패킷의 decode/encode 연산 대칭성 검사
python tools/check_eval_order.py   # 시퀀스 포인트 없는 다중 읽기 검출
tools\compile_check.bat            # 링크 없이 전체 컴파일 검사
```

### 정밀 검증 스위트

```
powershell -ExecutionPolicy Bypass -File tools\run_deep_roundtrip.ps1
```

131개 패킷의 **모든 필드**를 서로 다른 값으로 채워 encode → decode → re-encode 바이트 일치를
확인하고, 헤더 경로까지 왕복시킨 뒤, 절단·변조 퍼징으로 디코더가 크래시하지 않음을 검사합니다.

기본 생성 패킷만 검사하는 `run_roundtrip_test.ps1`과는 답하는 질문이 다릅니다.

| | 질문 |
|---|---|
| `run_roundtrip_test.ps1` | 229개 패킷을 만들고 인코딩·디코딩할 수 **있는가** |
| `run_deep_roundtrip.ps1` | 모든 **필드**가 올바르게 기록·복원되는가, 디코더를 깨뜨릴 수 있는가 |

값은 일부러 진단력을 갖도록 만들어집니다 — 0을 절대 쓰지 않고(필드 뒤바뀜이 숨는 자리),
정수마다 폭 하한을 두며(varint32/64 혼동이 바이트 수를 바꾸도록), 부호를 번갈아 주고(zigzag),
문자열에 비ASCII를 섞습니다(길이 접두 오류 노출). 자세한 이유는 `verify/include/.../ValueWell.h`에
규칙별로 적혀 있습니다.

인게임에서는 `/proto` 한 명령으로 같은 스위트와 실시간 트래픽 감사, 쇼케이스를 실행합니다.
자세한 내용은 [PORTING_STATUS.md](PORTING_STATUS.md)의 "검증 스위트" 절을 보십시오.

패킷 정의를 다시 생성하려면:

```
php tools/gen_from_php.php         # ProtocolInfo, 핸들러 인터페이스, 229개 패킷 골격
```

현재 남은 작업은 [PORTING_STATUS.md](PORTING_STATUS.md)에 정리되어 있습니다.

---

## 라이선스

원본 BedrockProtocol이 LGPL-3.0이므로 이 이식본도 LGPL-3.0입니다. 정적 링크로 배포하는
플러그인에는 LGPL 4조의 재링크 의무가 따릅니다.
