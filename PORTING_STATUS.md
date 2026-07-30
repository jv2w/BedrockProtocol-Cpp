# 이식 현황

## 진행률

| 계층 | 상태 |
|---|---|
| `encoding/` | 완료 (테스트 184개 통과) |
| `math/` `color/` `uuid/` `nbt/` | 완료 |
| `protocol/serializer/` | 완료 |
| `protocol/types/**` (~370) | 완료 |
| 패킷 229개 | **전부 컴파일·링크 통과** |
| `verify/` | **131개 패킷 전 필드 정밀 검증 통과** (아래 참고) |
| `bridge/` | 빌드됨, 서버 기동 확인 · 실트래픽 검증 도구 제공 |

- 코어 467개 번역 단위 컴파일 오류 0개
- `bedrock_protocol.lib`, `bedrock_protocol_bridge.lib`, `packetTest.dll` 빌드 성공
- 서버(Endstone 0.11.6)가 플러그인을 로드·활성화하고 정상 기동함 — 헤더·런타임 버전 일치
- **PORT-TODO 마커 0개**
- PHP 정적 팩토리 **39개 전부 이식** (`SetTitlePacket::title()`, `BossEventPacket::show()` 등)
- 인코딩 테스트 184개 통과 (PHP 참조 구현 대조)
- 왕복 테스트: **229개 등록 / 218개 바이트 단위 일치** (나머지 11개는 아래 5번 참고)
- `check_symmetry.py` 지적 32건 중 25건은 왕복 테스트가 바이트 대칭을 증명 → 위양성.
  나머지 7건은 PHP 원본과 수동 대조 완료 — **전부 PocketMine에 동일하게 존재하는 의도된 비대칭**

---

## 주요 문제

### 1. 프로토콜 버전 (사용자 확인으로 위험 완화됨)

| 대상 | 버전 |
|---|---|
| PMMP 원본 소스 | 프로토콜 **1001** / v1.26.30 |
| 설치된 BDS (실측) | **1.26.33.1** |
| 실행 중인 Endstone (실측) | **0.11.6** (Minecraft 26.33) |
| 빌드 대상 Endstone 태그 | **v0.11.6** (런타임과 일치) |

26.3X 대역은 패치 번호가 달라도 클라이언트가 상호 접속 가능 = **프로토콜이 동일**하므로
PMMP의 1001과 BDS 26.33은 같은 프로토콜로 판단됨.
브리지에 "디코드 후 잔여 바이트 발생 시 원본 그대로 통과" 안전장치가 있어, 만에 하나 어긋나도
손상 대신 무동작으로 떨어짐.

### 2. 검증 스위트가 찾아낸 실제 결함 3건 (전부 수정됨)

기존 왕복 테스트는 **기본 생성 패킷**만 검사했다 — 모든 필드가 0이라 필드 뒤바뀜·정수 폭 오류·
미독출 필드가 전부 통과했다. 전 필드를 서로 다른 값으로 채우자 즉시 세 건이 드러났다.

| 패킷 | 결함 | 원인 |
|---|---|---|
| `PlaySoundPacket` | 좌표가 블록 단위로 잘림 (`x = pos.getX() / 8`) | PHP의 `/`는 실수를 내지만 C++는 정수 나눗셈 |
| `SetTimePacket` | `create(time)`이 인자를 버려 항상 0 반환 | 팩토리 본문 대입 누락 |
| `CorrectPlayerMovePredictionPacket` | `vehicleRotation`의 X/Y가 뒤바뀜 | `Vector2(read(in), read(in))` — C++ 인자 평가 순서 미지정, MSVC는 역순 |

세 번째가 가장 위험한 부류라 전 코드베이스를 훑어 동일 패턴이 없음을 확인했고,
재발 방지용으로 `tools/check_eval_order.py`를 추가했다. 첫 번째는 트랜스파일러에서
근본 수정했으므로 재생성해도 되살아나지 않는다.

### 3. 74개 패킷 수동 완성 = 업스트림 갱신 시 재작업

`HAND_FINISHED_PACKETS` 보호 목록에 등록되어 재생성에서 제외됨.
PMMP가 프로토콜을 올리면 이 74개는 손으로 다시 맞춰야 함.

### 4. 툴체인이 clang-cl + Ninja 로 고정됨 (해결됨)

Endstone v0.11.4부터 Windows에서 clang-cl과 Ninja 생성기를 강제함. clang-cl 20.1.8 설치 완료,
`build.bat` 이 경로를 자동 탐지하도록 갱신됨. Visual Studio 생성기는 더 이상 사용 불가.

### 5. 기본 생성 패킷 11개는 인코딩 불가 (PHP와 동일)

필수 멤버가 PHP에서 미초기화 타입 프로퍼티인 패킷들. PHP도 접근 시 오류가 나므로 동작은 일치하지만,
**C++에서는 예외가 아니라 접근 위반으로 프로세스가 죽음.** 플러그인이 패킷을 손으로 만들다 필드를
빠뜨리면 서버가 크래시함. 목록과 사유는 `tests/PacketRoundTripTest.cpp` 의 `EXPECTED_INCOMPLETE` 참고.

---

### 6. C++ 문법 제약으로 개명한 식별자

PHP 이름을 1:1로 보존하는 것이 원칙이나, 아래는 언어 제약상 불가능해 개명했다.
각 선언 위치에 `@note` 로 PHP 원래 이름을 남겨 두었다.

| PHP | 이 포트 | 이유 |
|---|---|---|
| `DeviceOS::WIN32` | `DeviceOS::OsWIN32` | `<windows.h>` 의 `WIN32` 매크로 |
| `ProtocolInfo::MINECRAFT_VERSION` | `ProtocolInfo::MINECRAFT_VERSION_DISPLAY` | Endstone `detail.h` 의 매크로 |
| `SetTitlePacket::type()` / `text()` | `makeType()` / `makeText()` | C++은 같은 이름의 필드와 메서드 공존 불가 (PHP는 허용) |

---

## 결정 기록

| 항목 | 결정 |
|---|---|
| 프로토콜 정합 | 26.3X 대역은 패치가 달라도 프로토콜 동일 → PMMP 1001 그대로 사용 |
| Endstone 버전 | clang-cl 설치 후 **v0.11.6** 으로 상향 (런타임과 일치) |
| 매크로 충돌 | `#undef` 대신 **개명** (`OsWIN32`, `MINECRAFT_VERSION_DISPLAY`) |
| 검증 범위 | 클라이언트 접속 검증은 생략, 오프라인 전수 검증으로 대체 |
| 기본 생성 패킷 크래시 | PHP와 동작 일치를 우선해 **그대로 유지** |

---

## 검증 스위트

세 시스템이 각각 다른 종류의 증거를 만든다. 사각지대가 겹치지 않도록 설계했다.

| | 무엇을 입증하나 | 사각지대 |
|---|---|---|
| **A** 오프라인 정밀 왕복 | 131개 패킷 **576개 필드**가 고유값으로 기록·복원되고, 디코더를 크래시시킬 수 없음 | **대칭 오류** (encode/decode가 똑같이 틀림) 통과 |
| **B** 실시간 트래픽 감사 | 우리 레이아웃이 실제 BDS 26.33 / 실제 클라이언트의 와이어와 일치 | 흐르는 패킷만 커버 |
| **C** 인게임 쇼케이스 | 우리가 만든 패킷을 모장 클라이언트가 올바르게 렌더 | 자동화 아님 (판정자가 사람 눈) |

A·B는 둘 다 **우리 인코더가 심판**이라 대칭 오류를 못 잡는다. C만이 독립 디코더를 심판으로
세우므로 그 사각지대를 닫는다 — 이것이 C가 존재하는 이유다.

```
# A (권위 게이트, MSVC 필요)
powershell -ExecutionPolicy Bypass -File bedrock_protocol\tools\run_deep_roundtrip.ps1

# A를 인게임과 동일한 clang-cl 경로로 (엔진을 수정했다면 반드시)
powershell -ExecutionPolicy Bypass -File bedrock_protocol\tools\run_deep_roundtrip.ps1 -Clang -OutDir build\test-clang

python bedrock_protocol\tools\check_eval_order.py

# 인게임 - 이것만 쓰면 된다
/proto verify       셋 다 돌리고 결과를 한 줄로. 약 70초.

# 인게임 - 개별 실행
/proto              폼 메뉴
/proto selftest     A를 인게임에서 (반동어반복·퍼징은 생략됨, 아래 참고)
/proto audit on 300 실트래픽 감사, 평소처럼 플레이 후 /proto audit report
/proto show         약 60초 쇼케이스, 전부 자동 복구
```

`/proto verify`가 존재하는 이유: 세 보고서는 각각 해석이 필요하고, 무엇이 진짜 실패인지가 서로
다르다. 특히 비정규 재인코딩은 알기 전까지는 심각해 보인다. 하나의 판정으로 접는 것이
"쓸 수 있는 도구"와 "건너뛰는 도구"의 차이다.

### 출시 전 전수 검수 (2026-07-30)

공개 배포를 앞두고 6개 축으로 적대적 감사를 수행했다. **원격에서 서버를 죽일 수 있는 취약점 2건을
포함해 13건의 결함**이 나왔고 전부 수정했다.

| 심각도 | 결함 | 영향 |
|---|---|---|
| **Critical** | NBT 깊이 제한 해제 시 스택 고갈 | `catch(...){--d; throw;}`가 레벨마다 중첩돼, 제한이 걸리는 순간 512개 프레임에서 재전파. 레벨당 8KB 이상 소비. **약 1.5KB 패킷으로 원격 서버 종료.** 스택이 없어 `_set_se_translator`로도 못 잡는다 |
| **Critical** | `LevelEventGenericPacket`이 깊이 제한 없이 NBT 파싱 | `maxDepth = 0`이 "무제한"이었다. 다른 호출부는 전부 512를 넘기는데 여기만 누락 |
| **Critical** | 쇼케이스가 정상 종료 시 복구 안 함 | `finish()`가 중단·비활성화 경로에서만 호출. 유령 블록·엔티티·밤이 영구 잔존 |
| **Critical** | 브리지 주입 가드 예외 누수 | 카운터가 영구히 >0이 되어 **패킷 가로채기 전체가 조용히 정지**. 가상 창고도 함께 사망 |
| **Critical** | 인터셉터 싱글턴 미정리 | 리로드 후 해제된 메모리 호출 |
| High | `putRotationByte` 음수 회전 UB | 음수 float→`uint8_t` 변환은 UB. 엔티티 피치는 상시 음수 |
| Medium | `LittleEndian`/`BigEndian`/`Network` NBT 정수 배열 선점 할당 | 11바이트 입력으로 **8GiB 할당** 요구 |
| Medium | `CraftingDataPacket` 부호 있는 32비트 시프트 UB | PHP는 64비트 연산 |
| Medium | `GraphicsOverrideParameterPacket` 디코드 시 항목 누적 | |
| Major | 감사 통계를 자기 트래픽이 오염 | 가상 창고 패킷이 실트래픽으로 집계돼 일치율 부풀림 |
| Major | 복구 시 런타임 ID 0 덮어쓰기 | |
| Major | `/proto audit on <초>` 정수 오버플로 | |
| Major | 인게임 퍼징이 툴체인 우연으로만 안전 | MSVC 구성 시 서버 수 분 정지 |
| Low | 초기화 안 된 `optional` 역참조 UB 3건 | `.value()`로 교체 |

감사 범위와 결과:

| 축 | 범위 | 결과 |
|---|---|---|
| 커버리지 | 딥 스위트 미포함 98종 | **223/229** 로 확대 (794필드). 나머지 6종은 필드 0개 |
| 악의적 입력 | 229개 × 106종 버퍼 = **24,274회 디코드** | Critical 3건 발견·수정, 최종 0건 |
| `types/`·`serializer/` | **365개 타입군 / 570파일 전수** 기계 대조 | 필드 순서·정수 폭·부호 100% 검증. 결함 2건 |
| 수작업 패킷 | 73종 중 82건 감사 | 와이어 결함 2건 |
| 플러그인·브리지 | 적대적 코드 리뷰 | Critical 3 / Major 4 |
| 대칭성 경고 | 29건 | **전부 오탐** 판정 (PHP 자체가 구조적 비대칭) |

#### 1:1 원칙을 의도적으로 어긴 곳 (2건)

이 이식본은 PocketMine과 1:1 대응을 원칙으로 하지만, 두 곳에서 **업스트림이 틀렸다고 판단해
정확성을 택했다.** 둘 다 코드에 사유를 명시했다.

- `EducationSettingsExternalLinkSettings::read` — PHP가 `url`→`displayName` 순으로 읽고 생성자에
  `(displayName, url)`로 넘겨 **두 문자열을 뒤바꾼다.** 그대로 두면 플러그인이 이 패킷을 만질 때마다
  조용히 값이 뒤바뀐다. 와이어 포맷은 모장이 정의하므로 업스트림이 단순히 틀린 것이다.
- `BitSet::write` — PHP는 부호 있는 64비트에 산술 시프트를 써서, 비트 63이 서면 부호 확장된 1들이
  다음 파트로 새어 들어간다. 비트 63은 `PlayerAuthInputFlags::SNEAK_PRESSED_RAW`라 웅크릴 때마다
  재현된다. C++는 `uint64_t`라 논리 시프트이고, 이쪽이 바닐라 클라이언트와 맞다.

### 실측 결과 (2026-07-30, BDS 1.26.33.1 / Endstone 0.11.6, 실제 클라이언트 접속)

| | 결과 |
|---|---|
| **A** 오프라인 | **223/223 통과**, 794필드, 중복값 0, 절단 26,904 · 변조 14,272회 퍼징에 폴트 0 |
| **B** 실트래픽 | 23종 관측, 실제 결함 **0건**. 형식차 2종(`PlayerAuthInputPacket` BitSet, `SubChunkPacket` renderHeightMap) — 둘 다 PHP와 동일한 비정규 재인코딩 |
| **C** 인게임 | **전 단계 육안 확인 완료.** 클라이언트 전용 발광석 발판이 통과 가능한 상태로 렌더됨 = 서버에 없는 블록을 클라이언트가 우리 바이트만으로 그렸다는 증거 |

### 출시 전 반드시 통과해야 하는 게이트

```
build.bat                                                            # 무경고
tools\run_deep_roundtrip.ps1                                         # 223/223 (권위 게이트, MSVC)
tools\run_deep_roundtrip.ps1 -Clang -OutDir build\test-clang         # 인게임 경로 - 아래 참고
tools\run_malformed_test.ps1                                         # 229개 × 106 버퍼, RESULT: PASS
tools\run_roundtrip_test.ps1                                         # 229개 얕은 게이트
tools\run_encoding_test.bat                                          # PHP 참조 대조 184건
python tools\check_eval_order.py                                     # 미시퀀스 다중 읽기 0
```

`-Clang`은 형식이 아니라 필수다. 인게임 빌드는 clang-cl이고 하드웨어 폴트를 잡지 못하므로,
**오프라인은 초록인데 인게임에서 치명적인 부류**가 존재한다. 실제로 반동어반복 단계가 그렇게
서버를 죽인 적이 있다.

C의 확인이 결정적이다. A와 B는 둘 다 우리 인코더가 심판이라 대칭 오류를 통과시키지만, C의
심판은 우리가 작성하지 않은 모장 클라이언트다. 세 층이 모두 통과했으므로 이식의 정합성은
서로 독립적인 세 종류의 증거로 뒷받침된다.

### 실트래픽 감사에서 나온 결과 (실측)

| 패킷 | 결과 | 해석 |
|---|---|---|
| `PlayerAuthInputPacket` | 203건 관측, 전건 **형식차** | 21개 필드가 실제 클라이언트 바이트와 **전부 일치**. 차이는 `inputFlags` BitSet 하나뿐 — 서버는 연속 비트가 끝나면 조기 종료(`02`)하지만 우리 `write`는 선언 길이만큼 전부 기록(`82 80 00`)한다. **값은 동일하고 PHP도 같으므로 결함이 아니다.** |
| `InventoryTransactionPacket` 외 20여 종 | 100% 일치 | 인벤토리·상호작용·청크 요청 등 실제 게임플레이 경로 |
| `SubChunkPacket` | 5건 중 1건 **형식차** | BitSet과 같은 현상. 서버는 `renderHeightMap`을 `ALL_COPIED`(1바이트)로 보내는데, 디코드가 이를 heightMap 복사본으로 채우고 인코드는 `DATA`+256바이트로 쓴다. 5엔트리 × 256 = 1,280바이트. PHP와 한 줄씩 동일. |

`write`가 항상 전체 길이를 쓰는 것은 브리지에 실질적 영향이 있다 — 플러그인이
`PlayerAuthInputPacket`을 수정하면 `inputFlags`가 더 긴 형식으로 재전송된다. 값이 같고 리더가
연속 비트를 따라가므로 동작은 정상이나, 바이트 단위 비교로는 달라 보인다.

감사는 이제 이 경우를 **형식차(reform)** 로 따로 집계한다. 판정 방법: 재인코딩을 한 번 더 디코딩·
인코딩해서 2세대와 3세대가 같으면 정보 손실 없이 표현만 다른 것이다.

### 알려진 커버리지 공백

- `TextPacket::sourceName` — 필러가 `TYPE_TRANSLATION` 분기(파라미터 컬렉션 검증)를 택해서,
  `sourceName`을 쓰는 CHAT/WHISPER/ANNOUNCEMENT 분기는 미검증. 레지스트리가 패킷 ID당 한 항목만
  받으므로 두 분기를 동시에 넣으려면 변형(variant) 개념이 필요하다.
- `BiomeDefinitionData::chunkGenData` — 하위 22개 타입 트리라 별도 작업으로 미룸. `writeOptional`의
  부재 분기는 검증됨.
- **`bool` 필드끼리의 뒤바뀜은 이 스위트가 볼 수 없다.** 값이 둘뿐이라 고유값을 줄 수 없기 때문이며,
  이것이 A의 유일한 구조적 한계다.
- **인게임 실행은 반동어반복(S2)과 퍼징(S9·S10)을 건너뛴다.** 두 단계 모두 하드웨어 폴트를 유발할 수
  있어 폴트를 잡을 수 있는 곳에서만 돌릴 수 있는데, 플러그인은 clang-cl 빌드라 잡지 못한다.
  이걸 몰랐을 때 `/proto selftest`가 실제로 서버를 죽였다 — 그래서 `-Clang` 스위치가 있다.

## 다음 단계

1. ~~74개 패킷 컴파일 통과~~ 완료
2. ~~전체 링크 · 플러그인 빌드 · 서버 기동~~ 완료
3. ~~오프라인 왕복 검증~~ 완료 (131개 전 필드 + 퍼징)
4. ~~클라이언트 접속 후 실트래픽 감사 · 쇼케이스 육안 확인~~ **완료**

남은 것은 선택 사항이며 어느 것도 정합성을 막지 않는다.

- 딥 스위트 미포함 98종(229 − 131)에 필러 추가. 대부분 필드가 0~3개이거나 실사용 빈도가 낮다.
- `TextPacket::sourceName` 분기, `BiomeDefinitionData::chunkGenData` 하위 트리
- PMMP가 프로토콜을 올릴 때 `HAND_FINISHED_PACKETS` 74종 재작업
