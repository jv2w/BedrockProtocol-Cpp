# PORTING.md — BedrockProtocol PHP → C++ 이식 규칙서

이 문서는 **규범(normative)** 이다. 이식 작업(사람이든 에이전트든)은 여기 적힌 규칙을 그대로 따른다.
목표는 "동작이 같은 C++"이 아니라 **"PHP 원본과 1:1로 대조 가능한 C++"** 이다. 이름·필드 순서·주석이
원본과 일치해야 LLM과 사람이 업스트림 diff를 그대로 신뢰할 수 있다.

원본 루트(이하 `$PHP`): `c:/Users/User/Desktop/BetterPMMP/source/vendor/pocketmine/bedrock-protocol/src`

---

## 1. 파일·이름 규칙

| 대상 | 규칙 |
|---|---|
| 파일명 | PHP 파일명 그대로 (`TextPacket.php` → `TextPacket.h` / `TextPacket.cpp`). **snake_case로 바꾸지 않는다** — 업스트림 추적성이 우선이며, Endstone 관례와 다른 점은 의도적이다. |
| 디렉토리 | `$PHP` 하위 구조를 정확히 미러. `src/types/entity/` → `include/bedrock_protocol/protocol/types/entity/` |
| 클래스명 | PHP와 동일 |
| 필드·메서드명 | PHP와 동일 (PHP camelCase = C++ camelBack이므로 그대로) |
| 상수명 | PHP와 동일 (`MODE_TELEPORT` 등) |
| private 멤버 접미사 | **붙이지 않는다.** PHP에 `$skinId`면 C++도 `skinId`. Endstone의 `member_` 관례를 따르지 않는다 (1:1 대조 우선). 단 코어 인프라(encoding/, bridge/)는 예외로 `member_` 사용. |

### 1.1 매크로와 충돌하는 이름 (유일한 개명 예외)

C 매크로와 이름이 겹치면 전처리기가 식별자를 치환해버려 컴파일이 불가능하다.
`#undef` 는 **쓰지 않는다** — 그 헤더 이후 include되는 모든 코드에 조용히 영향을 주기 때문이다.
대신 **개명**하고, 클래스 docblock에 `@note` 로 PHP 원래 이름을 남긴다.

| PHP 이름 | C++ 이름 | 충돌 상대 |
|---|---|---|
| `DeviceOS::WIN32` | `DeviceOS::OsWIN32` | `<windows.h>` 의 `WIN32` |
| `ProtocolInfo::MINECRAFT_VERSION` | `ProtocolInfo::MINECRAFT_VERSION_DISPLAY` | Endstone `detail.h` 의 `MINECRAFT_VERSION` |

개명하지 않고 해결할 구조적 방법이 있으면 그쪽을 우선한다. 예: `Attribute` 의 `min`/`max` 필드는
`<windows.h>` 의 **함수형** 매크로와 겹치지만, 함수형 매크로는 뒤에 `(` 가 와야 확장되므로
멤버 초기화 리스트(`min(min)`) 대신 생성자 본문에서 대입하면 이름을 그대로 지킬 수 있다.

새로운 충돌을 발견하면 이 표에 추가하고, 생성 파일이면 `tools/gen_from_php.php` 의
`PROTOCOL_INFO_RENAMES` 에도 등록한다.

네임스페이스 매핑:

| PHP | C++ |
|---|---|
| `pmmp\encoding` | `bedrock_protocol::encoding` |
| `pocketmine\nbt`, `pocketmine\nbt\tag` | `bedrock_protocol::nbt`, `bedrock_protocol::nbt::tag` |
| `pocketmine\math` / `pocketmine\color` / Ramsey UUID | `bedrock_protocol::math` / `::color` / `::uuid` |
| `pocketmine\network\mcpe\protocol` | `bedrock_protocol` |
| `...\protocol\serializer` | `bedrock_protocol::serializer` |
| `...\protocol\types\<sub>` | `bedrock_protocol::types::<sub>` |

include 경로는 항상 절대형: `#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"`

---

## 2. 파일 골격

모든 파일은 이 헤더로 시작한다 (원본 LGPL 블록 + 출처 1줄):

```cpp
/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/TextPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once
```

헤더는 선언 위주, 본문은 `.cpp`. 예외: 한 줄짜리 접근자, `constexpr` 상수, 템플릿은 헤더에 인라인.

---

## 3. 타입 매핑

### 3.1 정수 — **선언 타입이 아니라 와이어 함수가 타입을 결정한다**

PHP의 `int`는 전부 부호 있는 64비트라 정보가 없다. `decodePayload`에서 그 필드를 읽는 함수를 보고 정한다.

| 읽기 함수 | C++ 타입 |
|---|---|
| `Byte::readUnsigned` / `readSigned` | `std::uint8_t` / `std::int8_t` |
| `LE/BE::readUnsignedShort` / `readSignedShort` | `std::uint16_t` / `std::int16_t` |
| `LE/BE::readUnsignedInt` / `readSignedInt` | `std::uint32_t` / `std::int32_t` |
| `LE/BE::readUnsignedLong` / `readSignedLong` | `std::uint64_t` / `std::int64_t` |
| `VarInt::readUnsignedInt` / `readSignedInt` | `std::uint32_t` / `std::int32_t` |
| `VarInt::readUnsignedLong` / `readSignedLong` | `std::uint64_t` / `std::int64_t` |
| `CommonTypes::getActorUniqueId` | `std::int64_t` |
| `CommonTypes::getActorRuntimeId` | `std::uint64_t` |
| `LE/BE::readFloat` / `readDouble` | `float` / `double` |

직접 시프트/마스크로 varint를 손으로 구현하지 말 것. 반드시 `VarInt`/`LE`/`BE`/`Byte`를 쓴다.

### 3.2 나머지

| PHP | C++ |
|---|---|
| `string` | `std::string` (필드) — 필드에 `std::string_view`를 **절대 저장하지 않는다** |
| `bool` | `bool` |
| `?T` (값 타입) | `std::optional<T>` |
| `?T` (다형 타입) | `std::unique_ptr<T>` (null 허용) |
| `T[]` / `list<T>` | `std::vector<T>` |
| `array<K,V>` (인코드가 순회 순서에 의존) | `std::vector<std::pair<K,V>>` — **`unordered_map` 금지** |
| `array<K,V>` (순서 무관, 조회용) | `std::unordered_map<K,V>` (인코드가 정렬하는 경우만) |
| `CompoundTag` | `nbt::tag::CompoundTag` |
| `CacheableNbt<T>` | `types::CacheableNbt<T>` (인코드 바이트 memoize) |
| `UuidInterface` | `uuid::Uuid` |
| `Vector3` / `Vector2` | `math::Vector3` / `math::Vector2` |
| `Color` | `color::Color` |
| PHP `enum: int` + `PacketIntEnumTrait` | `enum class X : std::int32_t {...};` + 같은 헤더에 `X XFromPacket(std::int32_t)` (미지 값이면 `PacketDecodeException` throw) |
| `final class` + 생성자 프로퍼티 승격 + getter | `final` 클래스, private 멤버, 명시적 생성자, `[[nodiscard]]` getter — 이름 전부 동일 |
| `static function read(...) : self` | `static T read(encoding::ByteBufferReader &in);` |
| `function write(ByteBufferWriter $out) : void` | `void write(encoding::ByteBufferWriter &out) const;` |
| `equals()` | `[[nodiscard]] bool equals(const T &other) const;` + `operator==`/`!=` |
| `match(x){...}` | `switch` + `default: throw` |
| `\LogicException` / `\InvalidArgumentException` | `std::logic_error` / `std::invalid_argument` |

### 3.3 다형 계층 (`ItemStackRequestAction`, `ItemDescriptor`, `RecipeWithTypeId`, `MetadataProperty`, `GameRule`)

```cpp
class Base {
public:
    virtual ~Base() = default;
    [[nodiscard]] virtual std::int32_t getTypeId() const = 0;
    virtual void write(encoding::ByteBufferWriter &out) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Base> clone() const = 0;   // 값 의미론 보존용
};
// 별도 free 함수 또는 static: std::unique_ptr<Base> readBase(ByteBufferReader&) — discriminator switch, 미지 값이면 PacketDecodeException
```

`unique_ptr` 멤버를 가진 타입은 move-only가 되므로, 해당 타입을 담는 컨테이너는 `std::vector<std::unique_ptr<Base>>`를 쓰고 복사가 필요하면 `clone()`을 명시 호출한다.

---

## 4. 주석 이식 규칙 (PHPDoc → Doxygen)

| PHP | C++ | 규칙 |
|---|---|---|
| `/** 산문 */` | `/** 산문 */` | **한 글자도 바꾸지 않는다** |
| `@param Type $name 설명` | `@param name 설명` | `$`와 타입만 제거, 설명은 원문 유지 |
| `@return Type 설명` | `@return 설명` | 설명이 없고 타입만 있으면 통째로 제거 |
| `@throws X` | `@throws X` | **유지** — decode 계약 문서라 load-bearing |
| `@see`, `@link`, `@deprecated`, `@internal` | 동일 유지 | `{@link X}` → `@see X` |
| `@var`, `@phpstan-*` | 제거 | 단 원소 의미 등 정보가 있으면 `@note`로 변환 |
| `/** @generate-create-func */` | 동일 유지 | 생성기가 재생성 시 인식하는 마커 |
| 트레일링 `// 설명` (필드 뒤) | 같은 위치에 그대로 | **LLM 가치의 핵심 — 반드시 보존** |
| `//TODO`, `//facepalm Mojang`, `//NOOP` | 그대로 유지 | |
| `//This is two little-endian longs...` 같은 본문 주석 | 그대로 유지 | |

한국어로 번역하지 않는다. 원문 영어 유지.

---

## 5. 패킷 클래스 형태

```cpp
class TextPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::TEXT_PACKET;

    static constexpr std::uint8_t TYPE_RAW = 0;
    // ... PHP의 public const 전부, 값·순서·주석 그대로

    std::uint8_t type;
    bool needsTranslation = false;
    std::string sourceName;
    // ... PHP 필드 순서 그대로, 기본값 그대로

    /**
     * @generate-create-func
     */
    static TextPacket create(/* PHP create()의 파라미터 순서·이름 그대로 */);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "TextPacket"; }
    bool handle(PacketHandlerInterface &handler) override;   // .cpp: return handler.handleText(*this);

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};
```

- `canBeSentBeforeLogin()`은 PHP가 오버라이드한 패킷만 오버라이드.
- private const는 `private:` 섹션에 그대로.
- `handle()`은 `.cpp`에 (헤더에서 `PacketHandlerInterface` 완전 정의가 불필요해짐).
- 필드에 기본값이 없는 PHP 타입 프로퍼티(`public int $actorRuntimeId;`)는 C++에서 값 초기화(`= 0`)를 준다. PHP는 미초기화 접근이 에러지만 C++은 UB이므로 이는 안전 방향의 의도적 차이.

---

## 6. decodePayload / encodePayload 번역 규칙

**절대 원칙: 읽기 순서와 쓰기 순서는 PHP와 완전히 동일해야 한다.** 최적화·재배열·"정리" 금지.

| PHP | C++ |
|---|---|
| `$this->x = CommonTypes::getString($in);` | `x = serializer::CommonTypes::getString(in);` |
| `CommonTypes::putString($out, $this->x);` | `serializer::CommonTypes::putString(out, x);` |
| `$v = LE::readSignedInt($in);` | `const auto v = encoding::LE::readSignedInt(in);` |
| `$this->list = [];` + `$this->list[] = X::read($in);` | `list.clear();` + `list.push_back(X::read(in));` |
| `for($i = 0, $count = VarInt::readUnsignedInt($in); $i < $count; ++$i){` | `for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {` |
| `foreach($this->list as $v){` | `for (const auto &v : list) {` |
| `VarInt::writeUnsignedInt($out, count($this->list));` | `encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(list.size()));` |
| `$v->write($out);` | `v.write(out);` (값) / `v->write(out);` (`unique_ptr`) |
| `if($this->x !== null){` | `if (x.has_value()) {` (optional) / `if (x != nullptr) {` (포인터) |
| `$this->x === self::MODE_X` | `x == MODE_X` |
| `CommonTypes::readOptional($in, fn(...) => ...)` | `serializer::CommonTypes::readOptional(in, [](auto &in) { return ...; })` |
| `throw new PacketDecodeException("msg")` | `throw PacketDecodeException("msg")` |

파일 상단에 `using` 별칭을 두어 잡음을 줄여도 된다 (`.cpp` 안에서만):
```cpp
using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;
```

번역이 불확실한 줄은 **추측하지 말고** 원본 PHP를 주석으로 남기고 `#error` 대신 다음 마커를 단다:
```cpp
// PORT-TODO: <원본 PHP 한 줄>
```
그리고 작업 보고에 해당 파일·줄을 반드시 보고한다.

---

## 7. 안전 규칙 (Endstone 런타임 특성에서 유래)

1. **`std::string_view`를 필드에 저장 금지.** `readByteArray()`는 원본 버퍼를 가리키는 뷰를 반환하며, 이벤트 페이로드 뷰는 디스패치 동안만 유효하다. 저장은 항상 `std::string`.
2. **디코드 경로는 전부 예외 안전.** 부분 디코드된 객체가 남아도 되지만, 메모리 누수·이중 해제는 없어야 한다 (`unique_ptr`/컨테이너만 사용, raw `new` 금지).
3. **길이 검증은 PHP와 동일하게.** PHP가 `Limits::INT32_MAX` 등으로 검사하면 C++도 같은 지점에서 같은 예외를 던진다. PHP에 없는 검사를 임의로 추가하지 않는다.
4. `count()` → `size()` 변환 시 `static_cast<std::uint32_t>` 명시.

---

## 8. 작업 완료 기준 (배치 단위)

1. 컴파일 통과 (경고 포함 확인).
2. 파일 안에 `PORT-TODO` 마커가 남았으면 전부 보고.
3. 생성기가 넣어둔 원본 PHP 주석 블록(`/* PHP: ... */`)을 번역 완료 후 제거.
4. `tools/check_symmetry.py` 통과 — `decodePayload`의 `read*`/`get*` 호출 순서와 `encodePayload`의 `write*`/`put*` 호출 순서가 대칭인지 검사.
5. 자가 직렬화 타입은 라운드트립 테스트 추가 (`tests/`).
