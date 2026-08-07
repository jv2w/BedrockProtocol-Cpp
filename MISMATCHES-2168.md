# 1.26.30 (1001) → 1.26.40 (2168) 불일치 전수 목록

레퍼런스: gophertunnel `v1.58.0` (= `b36ddad`), `minecraft/protocol/info.go:5-7` → `CurrentProtocol = 2168`, `CurrentVersion = "1.26.40"`.
대상 baseline: gophertunnel `6353c49` (1.26.30 / 1001) 와 일치 확인됨.
델타 근거: `git diff 6353c49 b36ddad -- minecraft/` (70 파일, +1673/−1263; 그중 `minecraft/protocol/**` 62 파일).

심각도: `접속불가` / `크래시` / `desync` / `표시오류` / `무해`

---

## 0. 패킷 ID 테이블 — 이상 없음

`packet/id.go` 는 이 구간에서 **변경 없음**. `ProtocolInfo.h` 와 전수 대조 결과 222개 값·이름 일치.
차이 12건은 전부 밀림이 아님:

- 철자만 다름(무해): `0x41` `IDEvent`↔`LEGACY_TELEMETRY_EVENT`, `0x59` `Behaviour`↔`Behavior`, `0x71` `Initialised`↔`Initialized`, `0xaa` `EducationResourceURI`↔`EDU_URI_RESOURCE`, `0xb0` `ClientStartItemCooldown`↔`PLAYER_START_ITEM_COOLDOWN`, `0xb5` `AgentAction`↔`AGENT_ACTION_EVENT`
- C++ 미구현: `0x37` AdventureSettings, `0x75` ScriptCustomEvent, `0xa3` FilterText, `0xad` PhotoInfoRequest, `0xc5` ClientCheatAbility
- C++ 에만 존재: `0x10` `SERVER_PLAYER_POST_MOVE_POSITION_PACKET` (레퍼런스는 `_`)

---

## 1. 버전 상수 — 접속불가

| 항목 | 레퍼런스 | 대상 |
|---|---|---|
| 프로토콜 | `info.go:5` `2168` | `ProtocolInfo.h:33` `CURRENT_PROTOCOL = 1001` |
| 네트워크 버전 | `info.go:7` `"1.26.40"` | `ProtocolInfo.h:37` `"1.26.30"` |
| 표시 버전 | — | `ProtocolInfo.h:35` `"v26.30"` → `"v26.40"` (표시오류) |

네트워크 버전은 `LevelSettings::vanillaVersion`, `ResourcePackStackPacket::baseGameVersion`, `SkinData::geometryDataEngineVersion` 기본값으로도 흘러감.

---

## 2. 접속 경로

### 2.1 NetworkSettings — 압축 (기존 결함, 이번 버전 변경 아님)

`CompressionAlgorithm::NONE` 값 불일치 — **접속불가**(무압축 협상 시)
- 레퍼런스 `packet/network_settings.go:7-11`: `CompressionAlgorithmNone = 0xffff`
- 대상 `types/CompressionAlgorithm.h:31`: `NONE = 255`

`ZLIB=0` / `SNAPPY=1` 은 일치. NetworkSettings 필드 목록 자체는 일치(변경 없음).
`RequestNetworkSettings` 일치.

### 2.2 ResourcePacksInfo — 접속불가

`TexturePacks` 길이 접두사 `SliceUint16Length` → `Slice`(varuint32)
- 레퍼런스 `packet/resource_packs_info.go:49`: `protocol.Slice(io, &pk.TexturePacks)`
- 대상 `ResourcePacksInfoPacket.cpp:49` `LE::readUnsignedShort`, `:64` `LE::writeUnsignedShort`

선행 6필드 및 `TexturePackInfo` 엔트리(10필드)는 일치.

### 2.3 ResourcePackStack — 일치

변경 없음. `Experiments` 의 LE uint32 count + trailing bool 구조까지 일치.

### 2.4 ResourcePackClientResponse — 전면 재작성, 접속불가

1. enum 기저가 `iota+1` → `iota` (off-by-one)
   - 레퍼런스 `packet/resource_pack_client_response.go:7-12`: Refused=0, SendPacks=1, AllPacksDownloaded=2, Completed=3
   - 대상 `ResourcePackClientResponsePacket.h:34-37`: 1,2,3,4
2. 응답 폭 `Uint8` → `Varuint32`
   - 레퍼런스 `:31` `io.Varuint32(&pk.Response)` / 대상 `.cpp:37,48` `Byte::readUnsigned`/`writeUnsigned`
3. enum 뒤에 **문자열 variant 이름 신규** — 누락
   - 레퍼런스 `:32-45`, 리터럴: `"cancel"`, `"downloading"`, `"downloadingfinished"`, `"resourcepackstackfinished"`
4. `PacksToDownload` 가 조건부 + varuint32 접두사
   - 레퍼런스 `:39-41`: `if pk.Response == PackResponseSendPacks` 일 때만, `FuncSlice`
   - 대상 `.cpp:38`: 무조건 + `LE::readUnsignedShort`

### 2.5 StartGame / LevelSettings — 81필드 중 4건 불일치, 접속불가

전 필드 순서 대조 완료. 아래 외 전부 일치.

1. `EducationEditionOffer` `Varint32` → `Varuint32` — **접속불가**
   - 레퍼런스 `packet/start_game.go:302` / 대상 `LevelSettings.cpp:49,104` `VarInt::read/writeSignedInt`
2. `PlayerPermissions` `Varint32` → `Uint8` — **접속불가**
   - 레퍼런스 `start_game.go:319` `io.Uint8` / 대상 `LevelSettings.cpp:65,120` `VarInt::read/writeSignedInt`
3. `IsLoggingChat` bool **삭제됨** — 대상에 잉여 1바이트 — **접속불가**
   - 대상 `StartGamePacket.cpp:98,141`
4. GameRule 인코딩 — 아래 2.6

### 2.6 GameRule — 접속불가 / 크래시

`GameRuleLegacy` 가 삭제되어 StartGame 과 GameRulesChanged 가 **동일 인코딩**을 씀.

1. int 게임룰 값이 `Varuint32` → `Uint32`(고정 4바이트 LE)
   - 레퍼런스 `writer.go:215-218`, `reader.go:278-281`
   - 대상 `types/IntGameRule.cpp:22-30` 의 `isStartGame` 분기가 `LevelSettings.cpp:116` 에서 `true` 로 호출되어 varuint32 를 씀. `isStartGame` 분기 자체가 사장(死藏)됨.
2. **타입 0 = null 게임룰 신규** — 페이로드 없음 — **크래시**
   - 레퍼런스 `reader.go:272-273`, `writer.go:208-210`
   - 대상 `types/GameRuleType.h:22-24` 는 `BOOL=1, INT=2, FLOAT=3` 뿐, `CommonTypes.cpp:433` 에 case 0 없음 → 디코드 시 throw

### 2.7 GatheringJoinInfo — 접속불가

4개 필드가 Optional 로 변경(존재 bool 4개 누락)
- 레퍼런스 `server_join_information.go:26-35`: `ExperienceWorldID`, `ExperienceWorldName`, `TargetID`, `ScenarioID`, `ServerID` 중 `OptionalFunc` 적용분
- 대상 `types/GatheringJoinInfo.cpp:45-52` 전부 무조건 기록

`ServerJoinInformation`, `StoreEntryPointInfo`, `PresenceInfo` 자체는 일치.
StartGame 및 (신규) Transfer 경유로 도달.

---

## 3. PlayerAuthInput — 매 틱 수신, 접속불가

1. **입력 플래그가 비트셋 → 플래그 ID 리스트** (신규 파일 `input_flags.go`)
   - 레퍼런스 `packet/player_auth_input.go:178` `protocol.InputFlagList(io, &pk.InputData, InputFlagCount)`
   - `input_flags.go:76-104`: `bool 존재` + `varuint32 개수` + `개수 × varint32 플래그ID`(중복 불가, `[0,size)`), 존재=false 면 그 뒤 아무것도 안 씀
   - 대상 `PlayerAuthInputPacket.cpp:100,146` 은 65비트 패킹 비트셋(`serializer/BitSet.cpp:76,108`)
2. 플래그 총수 65 → 66, `InputFlagInternalUpdate`(65) 신규
   - 레퍼런스 `player_auth_input.go:74-77` / 대상 `types/PlayerAuthInputFlags.h:122-124` `NUMBER_OF_FLAGS = 65`
   - 0~64 는 1:1 대응(명칭만 상이: 44 `ACK_ACTOR_DATA`, 45 `IN_CLIENT_PREDICTED_VEHICLE`, 58 `IS_HOTBAR_ONLY_TOUCH`)
3. `InteractionModel` `Varuint32` → `Varint32`(zigzag) — desync
   - 레퍼런스 `:181` / 대상 `.cpp:103` `VarInt::readUnsignedInt`
4. 옵셔널 5종이 `DoubleOptionalFunc` 로 변경 — 프레이밍 10바이트 누락 — 접속불가
   - 레퍼런스 `:186-194`: `ItemInteractionData`, `ItemStackRequest`, `BlockActions`, `VehicleRotation`, `ClientPredictedVehicle`
   - 대상 `.cpp:107-130,153-168` 은 플래그로 존재 판정, bool 접두사 전무
5. 차량 정보가 두 독립 옵셔널로 분리 — 접속불가
   - 대상 `types/PlayerAuthInputVehicleInfo.h:71-94` 는 `float,float,ActorUniqueID` 결합 구조
6. `BlockActions` 개수 varint 부호 — desync
   - 레퍼런스 varuint32 / 대상 `.cpp:115,160` `readSignedInt`/`writeSignedInt`

필드 순서 자체는 일치.

---

## 4. 스킨 — desync 전반

`protocol/skin.go` 대 `serializer/CommonTypes.cpp`(`getSkin`/`putSkin`).

| # | 항목 | 레퍼런스 | 대상 |
|---|---|---|---|
| 4.1 | animations 개수 | `skin.go:92` `Slice`(varuint32) | `CommonTypes.cpp:179,256` 고정 4바이트 LE |
| 4.2 | `AnimationType` | `skin.go:178` `Varuint32` | `:183,259` LE uint32 |
| 4.3 | `ExpressionType` | `skin.go:180` `Varuint32` | `:185,261` LE uint32 |
| 4.4 | `ArmSize` | `skin.go:101` `Uint8` (`ArmSizeSlim=0/Wide=1`, `skin.go:11-14`) | `:194,269` 문자열 `"slim"/"wide"` (`SkinData.h:29-30`) |
| 4.5 | `SkinColour` | `skin.go:102` `BEARGB` (4바이트 B,G,R,A) | `:195,270` 16진 문자열 |
| 4.6 | personaPieces 개수 | `skin.go:103` `Slice` | `:196,271` LE uint32 |
| 4.7 | `PersonaPiece.PieceType` | `skin.go:234` `Uint32` enum | `:200,274` 문자열 |
| 4.8 | `PersonaPiece.PackID` | `skin.go:235` `UUID`(16바이트 raw) | `:201,275` 문자열 |
| 4.9 | pieceTintColours 개수 | `skin.go:104` `Slice` | `:207,279` LE uint32 |
| 4.10 | tint 색상 배열 | `skin.go:256-258` **접두사 없는 고정 4개** `BEARGB` | `:211-215,282-285` 개수 + 16진 문자열 |
| 4.11 | tint `PieceType` 값 매핑 | `skin.go:250-255,263-279`: wire 는 `persona_` 접두사 제거, `persona_hand`→`hands`, `unsupported` 는 그대로 | `:210,281` 원문 그대로 (`PersonaPieceTintColor.h:23-25` 가 `"persona_eyes"` 등) |
| 4.12 | `Trusted` | `skin.go:113-118` 스킨 본문 내부 **문자열** `"true"/"false"` | 패킷 꼬리 bool (§4.14) |
| 4.13 | `ProfileHash` | `skin.go:119` 문자열 신규, 본문 말미 | 없음 |

4.14 `PlayerSkinPacket` 꼬리 `Trusted` bool 제거 — **EXTRA**
- 레퍼런스 `packet/player_skin.go:29-34` (UUID, Skin, NewSkinName, OldSkinName)
- 대상 `PlayerSkinPacket.cpp:43,55`

4.15 `PersonaPiece.PieceType` 상수 테이블 신규 29값 (`skin.go:183-213`, `Unknown=0` 선두) — 대상 `PersonaSkinPiece.h:22-31` 은 구 문자열 10개뿐.
순서: Unknown, Skeleton, Body, Skin, Bottom, Feet, Dress, Top, HighPants, Hands, Outerwear, FacialHair, Mouth, Eyes, Hair, Hood, Back, FaceAccessory, Head, Legs, LeftLeg, RightLeg, Arms, LeftArm, RightArm, Capes, ClassicSkin, Emote, Unsupported

4.16 일치 확인: `SkinImage` width→height 순, 스킨 말미 bool 5종(Premium, Persona, PersonaCapeOnClassic, PrimaryUser, OverrideAppearance) 순서, `PersonaPiece` 필드 순서, `EXPRESSION_LINEAR=0/BLINKING=1`.

### 로그인 ClientData JSON 키

48개 키 문자 단위 일치 확인. 누락 3건:
- `ProfileHash` (`login/data.go:253-254`) — 신규, §4.13 의 입력원
- `PlayFabId` (`login/data.go:190`) — `ClientDataToSkinDataHelper.cpp:255` 가 `""` 하드코딩
- `ThirdPartyNameOnly` (`login/data.go:224`) — 무해

`LoginPacket` / `SubClientLoginPacket` 자체는 일치.
`ClientDataToSkinDataHelper.cpp:247,251,267` 은 login 측 문자열을 그대로 전달하므로 4.4/4.5/4.7/4.11 변환 계층이 필요.

---

## 5. 플레이어 / 엔티티

### 5.1 PlayerList — 접속불가
- 패킷 선두 action 바이트 삭제. 레퍼런스 `packet/player_list.go:23-25` 는 `Slice(&pk.Entries)` 뿐. 대상 `PlayerListPacket.cpp:47,81`
- 엔트리마다 `varuint32 variant`(Add=1/Remove=0) + `uint8 legacyAction` 신규, 그 뒤 UUID, Remove 면 조기 종료 (`player.go:96-97,115-130`). 대상 `.cpp:52-67,84-99` 는 UUID 부터 시작
- 꼬리 `Trusted` bool 루프 제거 (`.cpp:71-75,101-105`)
- 색상: 대상 `LE::writeUnsignedInt(toARGB())` 는 `BEARGB` 와 **바이트 동일**(직접 확인) → 수정 불필요
- Skin/XUID 필드 순서 일치

### 5.2 MovePlayer — 접속불가
teleport 필드가 `Mode` 조건부 → `OptionalMarshaler`(항상 존재 bool)
- 레퍼런스 `packet/move_player.go:68`, 본문 `player.go:187-190` (`Int32 TeleportCause`, `Int32 TeleportSourceEntityType`)
- 대상 `MovePlayerPacket.cpp:59-62,77-80`

### 5.3 MoveActorDelta — 접속불가
`uint16` 플래그 워드 삭제, 6성분 전부 Optional, 꼬리 bool 4개 신규
- 레퍼런스 `packet/move_actor_delta.go:40-52` (`OnGround`, `ForceMove`, `ForceMoveLocalEntity`, `ForceCompletion`)
- 대상 `MoveActorDeltaPacket.cpp:43-53,70-80`, `MoveActorDeltaPacket.h:34-42`

### 5.4 PlayerLocation — 접속불가
- 순서: `ActorUniqueID` 먼저, 그다음 type
- type `Varuint32` (대상은 고정 LE uint32)
- **예약 `Varint32` 신규**
- 레퍼런스 `packet/player_location.go:32-45` / 대상 `PlayerLocationPacket.cpp:48-53,59-66`

### 5.5 PlayerUpdateEntityOverrides — 접속불가
- 선두 `varuint32 variant` 신규(타입 중복) — 레퍼런스 `:40-42`
- 엔티티 ID 가 runtime → **unique** (`ActorUniqueID`, zigzag varint64) — 레퍼런스 `:38` / 대상 `.cpp:60,74` `getActorRuntimeId`

### 5.6 엔티티 메타데이터 키 — desync
139~141 신규 3건 누락으로 이후가 밀림
- 레퍼런스 `entity_metadata.go:143-145`: `ArrowShooterID`(139), `FireworkDirection`(140), `FireworkShooterID`(141); `Reserved139` = **142**, `NameplateRenderDistanceMax` = **143**
- 대상 `types/entity/EntityMetadataProperties.h:170-171`: `RESERVED_139 = 139`, `NAMEPLATE_RENDER_DISTANCE_MAX = 140`
- 0~138 은 전수 일치

키 16 별칭 신규(`DisplayFirework`, `WitherSkullDangerous`) 및 25 별칭 — 무해

### 5.7 엔티티 메타데이터 플래그 — desync
- 레퍼런스 `entity_metadata.go:288-291`: `UsesUniformAirDrag`(128), `NameplateDepthTested`(129), `NotPickableFromInside`(130, 신규), `Count`=131
- 대상 `types/entity/EntityMetadataFlags.h:149,151`: `USES_LEGACY_FRICTION = 127`, `NUMBER_OF_FLAGS = 128`
- 0~127 일치

### 5.8 ClientMovementPredictionSync 비트셋 크기 — 접속불가
128 → 131 (`packet/client_movement_prediction_sync.go:49` = `EntityDataFlagCount`)
- 대상 `ClientMovementPredictionSyncPacket.h:36`, `.cpp:61`
- 나머지 14필드 순서 일치

### 5.9 PlayerAction 상수 — 표시오류
36 `ReceivedServerData`, 38 `InternalUpdate`, 39 `Count` 누락 (`player.go:51-54`) / 대상 `types/PlayerAction.h:156-158`

### 5.10 일치 확인
`MobEffect`, `CorrectPlayerMovePrediction`, `MotionPredictionHints`, `SetActorData` 프레이밍 전부 일치.

---

## 6. 아이템 / 인벤토리 / 레시피

`ItemInstanceNew` 가 `ItemInstance` 이름을 승계(구 varint-ID 형식 소멸). `Item` 은 `ItemStack` 전용 별도 메서드. `StackRequestItem` 신설. `Recipe` IO 메서드·인터페이스·타입 상수 전부 삭제.

### 6.1 `Item` — air 단축 제거 — desync
- 레퍼런스 `writer.go:332-339`, `reader.go:412-420`: 모든 필드 무조건 기록. `NetworkID==0` 이어도 user-data 는 `varuint32 0` 을 냄(`writer.go:360-365`)
- 대상 `CommonTypes.cpp:69-80,82-95` 및 `:294-307` 는 `id==0` 이면 1바이트로 종료
- 레시피 결과·크리에이티브 그룹 아이콘·스미싱 결과에 매번 등장

### 6.2 `ItemInstance` — `stackIdVariant` varuint32 삭제 — desync
- 레퍼런스 `writer.go:312-329`: `hasNetID` bool + (있으면) `Varint32 StackNetworkID` 뿐
- 대상 `CommonTypes.cpp:339-378` 가 variant 를 추가로 기록. `ItemStackWrapper::stackIdVariant`(`ItemStackWrapper.h:104,117,124`) 는 제거 후 사장

### 6.3 구 `ItemInstance`(varint32-ID) 형식 완전 소멸 — desync
- 레퍼런스는 `inventory.go:49-50,250,263,272`, `inventory_content.go:32,34`, `inventory_slot.go:37-38`, `mob_armour_equipment.go:35-39`, `mob_equipment.go:35` 전부 새 `ItemInstance` 사용
- 대상 `CommonTypes.cpp:309-337` `get/putItemStackWrapper` 가 구 형식

### 6.4 `StackRequestItem` 신규 — desync
- 레퍼런스 `item.go:41-60`, `writer.go:342-358`, `reader.go:423-444`
- `Varuint32 variant`(있으면 `ItemDescriptorDefault`) + `Uint8 legacyVariant` + (있으면 `String Identifier`, `Varint32 Meta`) + `Int16 Count` + `Varuint32 BlockRuntimeID` + user-data
- **방패 판정이 숫자 ID 가 아니라 문자열 `"minecraft:shield"`**
- 대상 없음

### 6.5 아이템 디스크립터 타입 재번호 + 의미 재배치 — desync
- 레퍼런스 `item_descriptor.go:19-24`: `Invalid=0, Default=1, MoLang=2, ItemTag=3` (`Deferred`, `ComplexAlias` 삭제)
- 대상 `types/recipe/ItemDescriptorType.h:20-24`: `INT_ID_META=1, MOLANG=2, TAG=3, STRING_ID_META=4, COMPLEX_ALIAS=5`
- 슬롯별 의미가 다름: 레퍼런스 `Default`(1)=문자열 이름 기반, 대상 `INT_ID_META`(1)=숫자 ID 기반

### 6.6 `ItemDescriptorCount` 프레이밍 전면 변경 — desync
- 레퍼런스 `writer.go:286-309` / `reader.go:351-385`
- `Varuint32 variant`(0 또는 1로 클램프) + `String tag` + 본문 + (ItemTag/Invalid 면 `Varint32 32767` 필러) + `Varint32 Count`
- 태그 리터럴: `"name"`, `"molang"`, `"item_tag"` (`item_descriptor.go:88-106`)
- 대상 `CommonTypes.cpp:380-397` 은 uint8 판별자 + 본문 + count

### 6.7 `DefaultItemDescriptor` — desync
- 레퍼런스 `item_descriptor.go:44-47`: `String Name` + `Varint32 Meta`
- 대상 `IntIdMetaItemDescriptor.cpp` 는 int16 id + int16 meta; `StringIdMetaItemDescriptor` 는 meta 가 `uint16`

### 6.8 `MoLangItemDescriptor.Version` uint8 → int16 — desync
- 레퍼런스 `item_descriptor.go:58-61` / 대상 `MolangItemDescriptor.cpp`, `.h:478,492`

### 6.9 `Deferred` / `ComplexAlias` 디스크립터 삭제 — EXTRA
- 대상 `ComplexAliasItemDescriptor.{h,cpp}` 및 `ItemDescriptor.cpp` 의 `case COMPLEX_ALIAS` 잔존

### 6.10 `StackRequestItemDescriptorCount` 신규(별도 함수) — desync
- 레퍼런스 `item_descriptor.go:94-123`: `Varuint32 variant` + `Uint8 legacyID` + 본문 + **`Uint16 Count`**(고정 2바이트)
- 대상은 일반 `getRecipeIngredient` 사용 (`CraftRecipeAutoStackRequestAction.cpp:26-28`)

### 6.11 CraftingData — 타입 태그 소멸, 8개 개별 벡터 — 접속불가
- 레퍼런스 `packet/crafting_data.go:228-241` 순서:
  Shaped, Shapeless, Multi, ShulkerBox, ShapelessChemistry, ShapedChemistry, SmithingTransform, SmithingTrim, Potion, PotionContainerChange, MaterialReducers, `Bool ClearRecipes`
- 대상 `CraftingDataPacket.cpp:41-77,109-115` 은 단일 리스트 + 엔트리별 `Varint32` 타입
- `CraftingDataPacket.h:44-51` 의 `ENTRY_*` 상수는 전부 사장. 대상은 ShulkerBox 를 처리한 적 없고 `ENTRY_USER_DATA_SHAPELESS=5` 를 Shapeless 로 매핑 중

### 6.12 `RecipeUnlockRequirement` — desync
- 레퍼런스 `recipe.go:76-83`: `Varint32 Context` + `Bool present`(= Context==None) + (present 면 재료 슬라이스)
- 대상 `RecipeUnlockingRequirement.cpp:28-38,42-49` 는 bool 하나뿐, context 값 없음
- context 상수(`recipe.go:59-64`: `None, AlwaysUnlocked, PlayerInWater, PlayerHasManyItems`) 대상에 없음

### 6.13 unlock requirement 가 Optional 로 — desync
- 레퍼런스 `recipe.go:287,298`: `OptionalMarshaler` (선행 bool 1개 추가)
- 대상 `ShapedRecipe.cpp` / `ShapelessRecipe.cpp` 는 인라인

### 6.14 Shaped 레시피 입력에 개수 접두사 — desync
- 레퍼런스 `recipe.go:278-281`: `FuncSlice`(varuint32) + `len == width*height` 검증
- 대상 `ShapedRecipe.cpp` 는 이중 루프, 접두사 없음

### 6.15 StackRequestAction 태그에 variant 추가 — desync
- 레퍼런스 `writer.go:394-403`: `Varuint32 variant` + `Uint8 id`
- variant 규칙 `item_stack.go:47-52`: `id > TakeOutContainer` 면 `id - 2`; 리더는 `variant >= PlaceInContainer` 면 `+2`
- 대상 `stackrequest/ItemStackRequest.cpp:99,114` 은 uint8 만
- 타입 ID 자체(7,8 결번)는 이미 일치

### 6.16 `StackReqSlotInfo.StackNetworkID` varint32 → 고정 Int32 — desync
- 레퍼런스 `item_stack.go:622-626` / 대상 `ItemStackRequestSlotInfo.h:39,46` → `CommonTypes.h:297-310`

### 6.17 `MineBlockStackRequestAction.StackNetworkID` varint32 → 고정 Int32 — desync
- 레퍼런스 `item_stack.go:469-473` / 대상 `MineBlockStackRequestAction.h:44,51`

### 6.18 `CraftGrindstoneRecipeStackRequestAction` — desync
- 레퍼런스 `item_stack.go:566-570`: `Int32 RecipeNetworkID`(고정), `Uint8 NumberOfCrafts`, `Varint32 Cost`
- 대상 `GrindstoneStackRequestAction.h:46-49`: recipeId 가 varuint32, **cost 와 repetitions 순서가 뒤바뀜**

### 6.19 `AutoCraftRecipeStackRequestAction` — desync
- 레퍼런스 `item_stack.go:510-514`: `Varuint32 RecipeNetworkID`, `Uint8 NumberOfCrafts`, `FuncIOSlice`(varuint32) + `StackRequestItemDescriptorCount`
- 대상 `CraftRecipeAutoStackRequestAction.cpp:23-40`: `repetitions2` 잉여 바이트, 재료 개수가 uint8, 일반 디스크립터 사용

### 6.20 `ItemStackResponse.ContainerInfo` → DoubleOptional — desync
- 레퍼런스 `item_stack.go:231-244`: status(u8), requestID(varint32), bool(항상 true), bool(존재), [count+엔트리]. 존재 판정이 status 가 아니라 **비어있는지 여부**
- 대상 `stackresponse/ItemStackResponse.cpp:40,52` 는 `result == RESULT_OK` 로 게이트, bool 없음

### 6.21 `CraftResultsDeprecated.ResultItems` → `[]StackRequestItem` — desync
- 레퍼런스 `item_stack.go:598-607` / 대상 `DeprecatedCraftingResultsStackRequestAction.h:49-50` 은 `getItemStackWithoutStackId`

### 6.22 `StackResponseSlotInfo.StackNetworkID` → DoubleOptional — desync
- 레퍼런스 `item_stack.go:283-301`: bool 2개 추가
- 대상 `ItemStackResponseSlotInfo.cpp:29,42` 무조건 zigzag varint

### 6.23 `CreativeGroup.Category` Int32 → Uint8 — desync
- 레퍼런스 `creative.go:25-29`(필드 타입도 `byte`) / 대상 `CreativeGroupEntry.cpp:26,34` `LE::read/writeSignedInt` (`CreativeGroupEntry.h:42`)

### 6.24 일치 확인
`InventoryAction`(DoubleOptional 2종 포함 — 대상이 이미 선행 구현), `UseItem*`/`ReleaseItem` 트랜잭션 본문, `InventoryTransaction` 프레이밍, 트랜잭션·소스·윈도우 ID 상수, `InventoryContent`, `InventorySlot`, `MobArmourEquipment`, `MobEquipment`, `ItemRegistry`/`ItemEntry`, `CreativeItem`, `CreativeContent`, `FullContainerName`, `MultiRecipe`/`PotionRecipe`/`PotionContainerChange`/`MaterialReducer`, `SmithingTransform`/`SmithingTrim` 필드 순서, `ItemStackRequest` 외곽 프레이밍, `CraftRecipe`/`CraftCreative` 액션.

---

## 7. 월드 / 청크 / 사운드 / 맵

### 7.1 LevelChunk — desync
- `SubChunkLimit` Optional 신규가 fake-count 방식을 대체
  - 레퍼런스 `packet/level_chunk.go:46-50`: `Varuint32 SubChunkCount`(≤64) + `OptionalFunc(SubChunkLimit, Varint32)`
  - 대상 `LevelChunkPacket.cpp:79-91` 의 `CLIENT_REQUEST_*_FAKE_COUNT` + `uint16 HighestSubChunk`
- blob 해시 목록이 무조건 기록(비어도 varuint32 0)
  - 레퍼런스 `:51-52` / 대상 `.cpp:93-99` 는 캐시 플래그 시에만

### 7.2 SubChunk — desync
- 엔트리 개수 고정 LE uint32 → varuint32, 캐시/비캐시 분기 소멸
  - 레퍼런스 `packet/sub_chunk.go:28` / 대상 `SubChunkPacket.cpp:44,69` + `std::variant` 분기
- `SubChunkPacket.Position` 이 `SubChunkPos`(고정 LE int32 ×3) — 대상 `.cpp:42,65` 는 varint (기존 결함)
- `SubChunkEntry` 본문이 전부 Optional
  - 레퍼런스 `sub_chunk.go:43-56`: `RawPayload`, `HeightMapData`, `RenderHeightMapData`, `BlobHash` 4개 존재 바이트. `BlobHash` 는 캐시 여부와 무관하게 optional 로 항상 존재
  - 대상 `types/SubChunkPacketEntryCommon.cpp:31-70`, `SubChunkPacketEntryWithCache.cpp:23,31`
- `SubChunkRequest.Position` 은 이미 고정 int32 — 일치
- `SubChunkResult`(0 무명) / HeightMap 타입 상수 — 값 일치

### 7.3 PlaySound — desync
`Varint32 LoopCount` 신규, `Pitch` 와 optional `Handle` **사이**에 삽입
- 레퍼런스 `packet/play_sound.go:40` / 대상 `PlaySoundPacket.cpp:21-23`, `.h:41-42`

### 7.4 ClientboundUpdateSoundData — 전면 재구성 — desync
- 레퍼런스 `packet/clientbound_update_sound_data.go:25-34`: `Uint64 handle` + `OptionalMarshaler` 7종(Stop, SetVolume, SetPitch, Fade, SeekTo, Pause, Resume)
- 대상 `ClientboundUpdateSoundDataPacket.cpp:36-39` 는 handle + `String soundEvent`
- 신규 `protocol/sound.go` 전체가 대상에 없음:
  - 상수(uint32): `Stop=0, SetVolume=1, SetPitch=2, Fade=3, SeekTo=4, Pause=5, Resume=6` (`sound.go:3-11`)
  - `SoundDataUpdate.Marshal`(`sound.go:30-45`): `Varuint32 Type` + Stop/Pause/Resume 무본문, SetVolume=`Float32`, SetPitch=`Float32`, Fade=`Float32 Duration`+`Float32 TargetVolume`, SeekTo=`Float32 Seconds`

### 7.5 LevelSoundEvent — 무해
프레이밍 일치. 신규 문자열 3개 누락(테이블 **말미** 추가라 밀림 없음): `"mount"`, `"dismount"`, `"straw_bed.break_leave"` (`level_sound_event.go:583-585`), 그리고 `"undefined"`.

### 7.6 LevelEvent — 무해
프레이밍 일치(`Varint32 EventType`, `Vec3`, `Varint32 EventData`).
- 신규 `ParticleType*` 테이블 105값(`level_event.go:8-114`, `Undefined=0` … `YellowPoplarLeaves=104`) 대상에 전무. 별도 const 블록이라 기존 값 밀림 없음
- LevelEvent 상수 누락 13건: `SoundAmethystResonate=1067`, `QueueCustomMusic=1900`, `PlayCustomMusic=1901`, `StopCustomMusic=1902`, `SetMusicVolume=1903`, `SculkCharge=2037`, `SculkChargePop=2038`, `SonicExplosion=2039`, `ParticlesTrialSpawnerDetectionCharged=3615`, `ParticlesTrialSpawnerBecomeCharged=3616`, `AnimationSpawnCobweb=9814`, `ParticleSmashAttackGroundDust=9815`, `ParticleCreakingHeartTrail=9816`
- 대상에만 존재: `SOUND_CAMERA=1050`, `SET_DATA=4000`
- 전부 명시값이라 밀림 없음

### 7.7 ClientboundMapItemData — desync
- `UpdateFlags` 삭제, 전 필드 Optional 화
  - 레퍼런스 `packet/client_bound_map_item_data.go:61-83` 순서: `Varint64 MapID`, `Uint8 Dimension`, `Bool LockedMap`, `BlockPos Origin`, `Opt<FuncSlice<Varint64>> MapsIncludedIn`, `Opt<Uint8> Scale`, `Opt<Slice<TrackedObject>>`, `Opt<Slice<Decoration>>`, `Opt<Varint32> Width/Height/XOffset/YOffset`, `Opt<FuncSlice<BEARGB>> Pixels`
  - 대상 `ClientboundMapItemDataPacket.cpp:41-59,66-120` 은 `BITFLAG_*` 게이트 유지. `:54` 의 `count == width*height` 검증도 대응물 소멸
- `MapDecoration.Colour` `VarRGBA` → `BEARGB`
  - 레퍼런스 `map.go:87` / 대상 `.cpp:117,42` `VarInt(flipIntEndianness(toRGBA()))`
- `MapImage` 픽셀 `VarRGBA` → `BEARGB`
  - 레퍼런스 `client_bound_map_item_data.go:81-83` / 대상 `types/MapImage.cpp:64-67`
  - **수정 형태**: `LE::writeUnsignedInt(toARGB())` 가 `BEARGB` 와 바이트 동일(직접 검증)
- `MapTrackedObject` 두 하위 필드가 무조건 Optional
  - 레퍼런스 `map.go:51-58` / 대상 `.cpp:23-32`, `types/MapTrackedObject.h:26-31`
  - `EntityUniqueID` 가 `ActorUniqueID`(zigzag varint64) 로. `Type` 은 양쪽 다 고정 LE int32 — 일치
- decoration type 상수 24값(`map.go:7-32`) 대상에 없음 — 무해

### 7.8 StructureBlockUpdate — desync
`RedstoneSaveMode` `Varint32` → `Uint8`
- 레퍼런스 `packet/structure_block_update.go:75` / 대상 `CommonTypes.cpp:551,568`, `types/StructureEditorData.h:37`
- 공유 구조체이므로 `StructureTemplateDataRequestPacket` 도 영향 (미확인)
- 나머지 11필드 및 `StructureSettings` 14필드 순서 일치

### 7.9 DimensionDefinition — desync
`UUID PackID` 말미 신규
- 레퍼런스 `world.go:32-39` / 대상 `types/DimensionData.cpp:20-35`, `.h:42-45`

### 7.10 Shapes — 전부 일치
`PrimitiveShape` 11필드 순서, `ShapeData` 타입 ID 10종, shape 타입 enum 10종, 페이로드 본문 10종, `PrimitiveShapes`/`VoxelShapes` 패킷 모두 일치.
색상은 `LE(toARGB())` == `BEARGB` 로 **R/B 뒤바뀜 없음** 확인.

---

## 8. 기타

### 8.1 AnvilDamage — desync
`Uint8 Damage` 필드 삭제됨
- 레퍼런스 `packet/anvil_damage.go:20-21` 은 `BlockPos` 만 / 대상 `AnvilDamagePacket.cpp:37-38,44-45`

### 8.2 Transfer — desync
말미 `OptionalMarshaler(GatheringJoinInfo)` 신규
- 레퍼런스 `packet/transfer.go:26-30` / 대상 `TransferPacket.cpp:46-48`
- §2.7 을 그대로 상속

### 8.3 SetScore — desync
- 선두 `ActionType` 바이트 삭제 (`packet/set_score.go:20-22` 는 `Slice(&pk.Entries)` 뿐). 대상 `SetScorePacket.cpp:38,65`, `.h:35-38`
- 엔트리 본문 재구성 (`scoreboard.go:39-68`):
  `Varuint32 variant` → `String variantName` → `Varint64 EntryID` → variant 별 꼬리
  - Remove: `OptionalFunc(objective, String)`
  - Player/Entity: `String Objective`, `Int32 Score`, `ActorUniqueID`
  - FakePlayer: `String Objective`, `Int32 Score`, `String DisplayName`
  - 리터럴: `"remove"`, `"changeplayer"`, `"changeentity"`, `"changefakeplayer"`
  - 대상 `.cpp:41-57` 은 `varint64 id → objective → LE int32 score → uint8 type → 꼬리`
- identity enum 기저 `iota+1` → `iota`: `Remove=0` 신규, Player=1/Entity=2/FakePlayer=3 은 값 일치 (`scoreboard.go:3-8` / `types/ScorePacketEntry.h:22-24`)

### 8.4 SetScoreboardIdentity — desync
`EntryID` 뒤 `EntityUniqueID` 가 **모든 action** 에서 기록되며 `OptionalFunc` 존재 바이트를 가짐
- 레퍼런스 `packet/set_scoreboard_identity.go:31-37`, `scoreboard.go:82-85`
- 대상 `SetScoreboardIdentityPacket.cpp:40-43` 은 `type == TYPE_REGISTER_IDENTITY` 게이트
- `TYPE_REGISTER_IDENTITY=0` / `TYPE_CLEAR_IDENTITY=1` 값 일치

### 8.5 ServerBoundDiagnostics — desync
`SystemCategories` 슬라이스가 `SystemDiagnostics` 와 `WhiskerScopes` **사이**에 신규 삽입
- 레퍼런스 `packet/server_bound_diagnostics.go:62-66`, 타입 `memory_category.go:173-182` (`String CategoryName`, `Uint64 SystemIndex`)
- 대상 `ServerboundDiagnosticsPacket.cpp:68-76` 에 해당 타입 자체가 없음
- 선행 9개 float 및 나머지 타입은 일치

### 8.6 MemoryCategory 테이블 — 표시오류
0~7 일치, 8부터 전면 어긋남 (레퍼런스 112값 / 대상 81값, `memory_category.go:3-116` vs `types/MemoryCategory.h:25-105`)
- 중간 삽입 누락: `Blobs`(8), `OreUIClient`(55), `Rendering*` 10종(67~71, 73~77), `TestLoadTestTags`(89)
- `Persona` 가 6개로 분할(56~61: `PersonaPieces, PersonaAnimations, PersonaTextures, PersonaCharacters, PersonaSkinPacks, PersonaRepo`) — 대상은 `PERSONA=54` 단일
- `VR` 위치 이동: 대상 68 → 레퍼런스 **111(최후미)**
- 꼬리 누락: `Gameface*` 13종(98~110)
- `LightVolumeManager` 는 2168 에서 삭제 — 대상에 원래 없어 문제 없음

### 8.7 ServerBoundDataDrivenScreenClosed — 일치
대상이 이미 신형(`LE uint32 formId` + `String closeReason`).
close reason 상수 테이블만 없음(무해): `"programmaticclose"`, `"programmaticcloseall"`, `"clientcanceled"`, `"userbusy"`, `"invalidform"`

### 8.8 data_store — 일치
2168 이 추가한 `Double=3`, `List=5` 를 대상 `types/cereal/DynamicValueType.h:34-40` 이 이미 전부 보유.

---

## 9. reader.go / writer.go — wire 변경 vs Go 리팩터 분류

**wire 변경 4건뿐**:
1. GameRule int 값 `Varuint32` → `Uint32` (§2.6)
2. GameRule `case 0` / `case nil` 신규 (§2.6)
3. `SubChunkPos` `Varint32×3` → `Int32×3` (§7.2)
4. `StackRequestAction` 에 `Varuint32 variant` 선행 (§6.15)

**순수 Go 리팩터(바이트 영향 없음)**:
- `io.go` 에서 `ARGB`/`VarRGBA`/`Recipe`/`GameRuleLegacy` 제거, `ItemInstanceNew`→`StackRequestItem` 개명
- `SliceUint16Length`/`SliceVarint32Length`/`FuncSliceUint16Length`/`FuncIOSliceUint32Length` 삭제 (호출부 변경이 wire 변경이지 헬퍼 삭제 자체는 아님)
- `SliceLimit`→`SliceLength`, `checkRemaining` 추가 등 디코드 측 방어 강화
- `OptionalFuncIO`→`DoubleOptionalFunc` (프리미티브 신설; wire 영향은 호출부에서 계상)
- 주석 삭제

`BEARGB`(`writer.go:167-168`)는 **변경 없음**. 바이트 순서 `B,G,R,A` 이며 C++ `LE::writeUnsignedInt(Color::toARGB())` 와 동일함을 직접 검증.

---

## 10. 미확인 항목 (추측하지 않고 남김)

1. `AvailableCommands` 인자 타입 테이블 — `command.go` 의 const 블록 분리로 `CommandArgTypeInt` 가 실제로 1부터 시작하게 됐는지, 그리고 대상 테이블이 이에 맞는지. **off-by-one 위험 있음**
2. `StructureTemplateDataRequestPacket` — §7.8 의 공유 구조체 영향
3. `GameRulesChangedPacket` 이 `isStartGame=false` 로 호출하는지
4. `serializer::BitSet` 의 패킹이 Go `Bitset` 과 바이트 호환인지 (§5.8 에 영향)
5. `PlayerBlockAction` 요소 본문 (개수 varint 문제는 확정, 요소 레이아웃 미확인)
6. `MapInfoRequestPacketClientPixel` 의 `RGBA` vs `BEARGB`
7. `LevelSoundEvent` 571(C++) vs 557(ref) 이름 전수 대조 미실시 — 문자열 키라 wire 무해
8. 스택 리퀘스트 액션 19종 중 12종의 C++ 헤더 본문 미열람 (`Loom`, `LabTableCombine`, `BeaconPayment`, `DeprecatedCraftingNonImplemented`, `Swap`, `Drop`, `CraftingConsumeInput`, `CraftingCreateSpecificResult`, `Place`, `Destroy`, `CraftRecipeOptional` 등)
9. `ContainerType*` 상수 전수 대조 미실시 (`container.go` 는 이번 diff 에 없음)
10. `NetworkInventoryAction::readAuthInput`/`writeAuthInput` — 레퍼런스에 대응 경로가 없어 사장 코드인지 PMMP 고유 추상인지 판단 불가
11. C++ `EducationEditionOffer` / `PlayerPermissions` 상수 **값** 자체의 이동 여부 (폭 변경만 확정)
12. 로그인 JWT/OIDC(`login/request.go`) — C++ 에 대응 코드 없음

---

## 11. 검증 게이트에 대한 고지

이 저장소의 최상위 게이트는 PHP 원본(PMMP BedrockProtocol)과의 바이트 일치(`tools/check_php_parity.py`).
PHP 원본은 아카이브되어 1.26.40 을 따라오지 않으므로, 위에서 wire 가 바뀌는 패킷은 **PHP 대조가 구조적으로 불가능**해진다.
해당 패킷은 PHP 대신 gophertunnel 기준 기대 바이트열 단언으로 게이트를 대체하고, 제외 목록을 명시할 것.

베이스라인(수정 전): `run_deep_roundtrip.ps1` 종료 코드 0, 223 패킷 / 794 필드 / 실패 0.
