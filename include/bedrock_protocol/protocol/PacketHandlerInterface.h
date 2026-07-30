/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PacketHandlerInterface.php + src/PacketHandlerDefaultImplTrait.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

namespace bedrock_protocol {

class ActorEventPacket;
class ActorPickRequestPacket;
class AddActorPacket;
class AddBehaviorTreePacket;
class AddItemActorPacket;
class AddPaintingPacket;
class AddPlayerPacket;
class AddVolumeEntityPacket;
class AgentActionEventPacket;
class AgentAnimationPacket;
class AnimateEntityPacket;
class AnimatePacket;
class AnvilDamagePacket;
class AutomationClientConnectPacket;
class AvailableActorIdentifiersPacket;
class AvailableCommandsPacket;
class AwardAchievementPacket;
class BiomeDefinitionListPacket;
class BlockActorDataPacket;
class BlockEventPacket;
class BlockPickRequestPacket;
class BookEditPacket;
class BossEventPacket;
class CameraAimAssistActorPriorityPacket;
class CameraAimAssistPacket;
class CameraAimAssistPresetsPacket;
class CameraInstructionPacket;
class CameraPacket;
class CameraPresetsPacket;
class CameraShakePacket;
class CameraSplinePacket;
class ChangeDimensionPacket;
class ChangeMobPropertyPacket;
class ChunkRadiusUpdatedPacket;
class ClientCacheBlobStatusPacket;
class ClientCacheMissResponsePacket;
class ClientCacheStatusPacket;
class ClientCameraAimAssistPacket;
class ClientMovementPredictionSyncPacket;
class ClientToServerHandshakePacket;
class ClientboundAttributeLayerSyncPacket;
class ClientboundCloseFormPacket;
class ClientboundControlSchemeSetPacket;
class ClientboundDataDrivenUICloseScreenPacket;
class ClientboundDataDrivenUIReloadPacket;
class ClientboundDataDrivenUIShowScreenPacket;
class ClientboundDataStorePacket;
class ClientboundDebugRendererPacket;
class ClientboundMapItemDataPacket;
class ClientboundTextureShiftPacket;
class ClientboundUpdateSoundDataPacket;
class CodeBuilderPacket;
class CodeBuilderSourcePacket;
class CommandBlockUpdatePacket;
class CommandOutputPacket;
class CommandRequestPacket;
class CompletedUsingItemPacket;
class ContainerClosePacket;
class ContainerOpenPacket;
class ContainerRegistryCleanupPacket;
class ContainerSetDataPacket;
class CorrectPlayerMovePredictionPacket;
class CraftingDataPacket;
class CreatePhotoPacket;
class CreativeContentPacket;
class CurrentStructureFeaturePacket;
class DeathInfoPacket;
class DebugInfoPacket;
class DimensionDataPacket;
class DisconnectPacket;
class EditorNetworkPacket;
class EduUriResourcePacket;
class EducationSettingsPacket;
class EmoteListPacket;
class EmotePacket;
class FeatureRegistryPacket;
class GameRulesChangedPacket;
class GameTestRequestPacket;
class GameTestResultsPacket;
class GraphicsOverrideParameterPacket;
class GuiDataPickItemPacket;
class HurtArmorPacket;
class InteractPacket;
class InventoryContentPacket;
class InventorySlotPacket;
class InventoryTransactionPacket;
class ItemRegistryPacket;
class ItemStackRequestPacket;
class ItemStackResponsePacket;
class JigsawStructureDataPacket;
class LabTablePacket;
class LecternUpdatePacket;
class LegacyTelemetryEventPacket;
class LessonProgressPacket;
class LevelChunkPacket;
class LevelEventGenericPacket;
class LevelEventPacket;
class LevelSoundEventPacket;
class LocatorBarPacket;
class LoginPacket;
class MapCreateLockedCopyPacket;
class MapInfoRequestPacket;
class MobArmorEquipmentPacket;
class MobEffectPacket;
class MobEquipmentPacket;
class ModalFormRequestPacket;
class ModalFormResponsePacket;
class MotionPredictionHintsPacket;
class MoveActorAbsolutePacket;
class MoveActorDeltaPacket;
class MovePlayerPacket;
class MovementEffectPacket;
class MultiplayerSettingsPacket;
class NetworkChunkPublisherUpdatePacket;
class NetworkSettingsPacket;
class NetworkStackLatencyPacket;
class NpcDialoguePacket;
class NpcRequestPacket;
class OnScreenTextureAnimationPacket;
class OpenSignPacket;
class PacketViolationWarningPacket;
class PartyChangedPacket;
class PartyDestinationCookieResponsePacket;
class PhotoTransferPacket;
class PlaySoundPacket;
class PlayStatusPacket;
class PlayerActionPacket;
class PlayerArmorDamagePacket;
class PlayerAuthInputPacket;
class PlayerEnchantOptionsPacket;
class PlayerFogPacket;
class PlayerHotbarPacket;
class PlayerListPacket;
class PlayerLocationPacket;
class PlayerSkinPacket;
class PlayerStartItemCooldownPacket;
class PlayerToggleCrafterSlotRequestPacket;
class PlayerUpdateEntityOverridesPacket;
class PlayerVideoCapturePacket;
class PositionTrackingDBClientRequestPacket;
class PositionTrackingDBServerBroadcastPacket;
class PrimitiveShapesPacket;
class PurchaseReceiptPacket;
class RefreshEntitlementsPacket;
class RemoveActorPacket;
class RemoveObjectivePacket;
class RemoveVolumeEntityPacket;
class RequestAbilityPacket;
class RequestChunkRadiusPacket;
class RequestNetworkSettingsPacket;
class RequestPermissionsPacket;
class ResourcePackChunkDataPacket;
class ResourcePackChunkRequestPacket;
class ResourcePackClientResponsePacket;
class ResourcePackDataInfoPacket;
class ResourcePackStackPacket;
class ResourcePacksInfoPacket;
class ResourcePacksReadyForValidationPacket;
class RespawnPacket;
class ScriptMessagePacket;
class SendPartyDestinationCookiePacket;
class ServerPlayerPostMovePositionPacket;
class ServerPresenceInfoPacket;
class ServerSettingsRequestPacket;
class ServerSettingsResponsePacket;
class ServerStatsPacket;
class ServerStoreInfoPacket;
class ServerToClientHandshakePacket;
class ServerboundDataDrivenScreenClosedPacket;
class ServerboundDataStorePacket;
class ServerboundDiagnosticsPacket;
class ServerboundLoadingScreenPacket;
class ServerboundPackSettingChangePacket;
class SetActorDataPacket;
class SetActorLinkPacket;
class SetActorMotionPacket;
class SetCommandsEnabledPacket;
class SetDefaultGameTypePacket;
class SetDifficultyPacket;
class SetDisplayObjectivePacket;
class SetHealthPacket;
class SetHudPacket;
class SetLastHurtByPacket;
class SetLocalPlayerAsInitializedPacket;
class SetPlayerGameTypePacket;
class SetPlayerInventoryOptionsPacket;
class SetScorePacket;
class SetScoreboardIdentityPacket;
class SetSpawnPositionPacket;
class SetTimePacket;
class SetTitlePacket;
class SettingsCommandPacket;
class ShowCreditsPacket;
class ShowProfilePacket;
class ShowStoreOfferPacket;
class SimpleEventPacket;
class SimulationTypePacket;
class SpawnExperienceOrbPacket;
class SpawnParticleEffectPacket;
class StartGamePacket;
class StopSoundPacket;
class StructureBlockUpdatePacket;
class StructureTemplateDataRequestPacket;
class StructureTemplateDataResponsePacket;
class SubChunkPacket;
class SubChunkRequestPacket;
class SubClientLoginPacket;
class SyncActorPropertyPacket;
class SyncWorldClocksPacket;
class TakeItemActorPacket;
class TextPacket;
class TickingAreasLoadStatusPacket;
class ToastRequestPacket;
class TransferPacket;
class TrimDataPacket;
class UnlockedRecipesPacket;
class UpdateAbilitiesPacket;
class UpdateAdventureSettingsPacket;
class UpdateAttributesPacket;
class UpdateBlockPacket;
class UpdateBlockSyncedPacket;
class UpdateClientInputLocksPacket;
class UpdateClientOptionsPacket;
class UpdateEquipPacket;
class UpdatePlayerGameTypePacket;
class UpdateSoftEnumPacket;
class UpdateSubChunkBlocksPacket;
class UpdateTradePacket;
class VoxelShapesPacket;

/**
 * Receives decoded packets dispatched by Packet::handle().
 *
 * Every method defaults to returning false ("not handled"), mirroring PacketHandlerDefaultImplTrait
 * in the PHP original, so implementations only override the packets they care about.
 *
 * This file is generated automatically by tools/gen_from_php.php, do not edit it manually.
 */
class PacketHandlerInterface {
public:
    virtual ~PacketHandlerInterface() = default;

    virtual bool handleActorEvent(ActorEventPacket &packet) { return false; }
    virtual bool handleActorPickRequest(ActorPickRequestPacket &packet) { return false; }
    virtual bool handleAddActor(AddActorPacket &packet) { return false; }
    virtual bool handleAddBehaviorTree(AddBehaviorTreePacket &packet) { return false; }
    virtual bool handleAddItemActor(AddItemActorPacket &packet) { return false; }
    virtual bool handleAddPainting(AddPaintingPacket &packet) { return false; }
    virtual bool handleAddPlayer(AddPlayerPacket &packet) { return false; }
    virtual bool handleAddVolumeEntity(AddVolumeEntityPacket &packet) { return false; }
    virtual bool handleAgentActionEvent(AgentActionEventPacket &packet) { return false; }
    virtual bool handleAgentAnimation(AgentAnimationPacket &packet) { return false; }
    virtual bool handleAnimateEntity(AnimateEntityPacket &packet) { return false; }
    virtual bool handleAnimate(AnimatePacket &packet) { return false; }
    virtual bool handleAnvilDamage(AnvilDamagePacket &packet) { return false; }
    virtual bool handleAutomationClientConnect(AutomationClientConnectPacket &packet) { return false; }
    virtual bool handleAvailableActorIdentifiers(AvailableActorIdentifiersPacket &packet) { return false; }
    virtual bool handleAvailableCommands(AvailableCommandsPacket &packet) { return false; }
    virtual bool handleAwardAchievement(AwardAchievementPacket &packet) { return false; }
    virtual bool handleBiomeDefinitionList(BiomeDefinitionListPacket &packet) { return false; }
    virtual bool handleBlockActorData(BlockActorDataPacket &packet) { return false; }
    virtual bool handleBlockEvent(BlockEventPacket &packet) { return false; }
    virtual bool handleBlockPickRequest(BlockPickRequestPacket &packet) { return false; }
    virtual bool handleBookEdit(BookEditPacket &packet) { return false; }
    virtual bool handleBossEvent(BossEventPacket &packet) { return false; }
    virtual bool handleCameraAimAssistActorPriority(CameraAimAssistActorPriorityPacket &packet) { return false; }
    virtual bool handleCameraAimAssist(CameraAimAssistPacket &packet) { return false; }
    virtual bool handleCameraAimAssistPresets(CameraAimAssistPresetsPacket &packet) { return false; }
    virtual bool handleCameraInstruction(CameraInstructionPacket &packet) { return false; }
    virtual bool handleCamera(CameraPacket &packet) { return false; }
    virtual bool handleCameraPresets(CameraPresetsPacket &packet) { return false; }
    virtual bool handleCameraShake(CameraShakePacket &packet) { return false; }
    virtual bool handleCameraSpline(CameraSplinePacket &packet) { return false; }
    virtual bool handleChangeDimension(ChangeDimensionPacket &packet) { return false; }
    virtual bool handleChangeMobProperty(ChangeMobPropertyPacket &packet) { return false; }
    virtual bool handleChunkRadiusUpdated(ChunkRadiusUpdatedPacket &packet) { return false; }
    virtual bool handleClientCacheBlobStatus(ClientCacheBlobStatusPacket &packet) { return false; }
    virtual bool handleClientCacheMissResponse(ClientCacheMissResponsePacket &packet) { return false; }
    virtual bool handleClientCacheStatus(ClientCacheStatusPacket &packet) { return false; }
    virtual bool handleClientCameraAimAssist(ClientCameraAimAssistPacket &packet) { return false; }
    virtual bool handleClientMovementPredictionSync(ClientMovementPredictionSyncPacket &packet) { return false; }
    virtual bool handleClientToServerHandshake(ClientToServerHandshakePacket &packet) { return false; }
    virtual bool handleClientboundAttributeLayerSync(ClientboundAttributeLayerSyncPacket &packet) { return false; }
    virtual bool handleClientboundCloseForm(ClientboundCloseFormPacket &packet) { return false; }
    virtual bool handleClientboundControlSchemeSet(ClientboundControlSchemeSetPacket &packet) { return false; }
    virtual bool handleClientboundDataDrivenUICloseScreen(ClientboundDataDrivenUICloseScreenPacket &packet) { return false; }
    virtual bool handleClientboundDataDrivenUIReload(ClientboundDataDrivenUIReloadPacket &packet) { return false; }
    virtual bool handleClientboundDataDrivenUIShowScreen(ClientboundDataDrivenUIShowScreenPacket &packet) { return false; }
    virtual bool handleClientboundDataStore(ClientboundDataStorePacket &packet) { return false; }
    virtual bool handleClientboundDebugRenderer(ClientboundDebugRendererPacket &packet) { return false; }
    virtual bool handleClientboundMapItemData(ClientboundMapItemDataPacket &packet) { return false; }
    virtual bool handleClientboundTextureShift(ClientboundTextureShiftPacket &packet) { return false; }
    virtual bool handleClientboundUpdateSoundData(ClientboundUpdateSoundDataPacket &packet) { return false; }
    virtual bool handleCodeBuilder(CodeBuilderPacket &packet) { return false; }
    virtual bool handleCodeBuilderSource(CodeBuilderSourcePacket &packet) { return false; }
    virtual bool handleCommandBlockUpdate(CommandBlockUpdatePacket &packet) { return false; }
    virtual bool handleCommandOutput(CommandOutputPacket &packet) { return false; }
    virtual bool handleCommandRequest(CommandRequestPacket &packet) { return false; }
    virtual bool handleCompletedUsingItem(CompletedUsingItemPacket &packet) { return false; }
    virtual bool handleContainerClose(ContainerClosePacket &packet) { return false; }
    virtual bool handleContainerOpen(ContainerOpenPacket &packet) { return false; }
    virtual bool handleContainerRegistryCleanup(ContainerRegistryCleanupPacket &packet) { return false; }
    virtual bool handleContainerSetData(ContainerSetDataPacket &packet) { return false; }
    virtual bool handleCorrectPlayerMovePrediction(CorrectPlayerMovePredictionPacket &packet) { return false; }
    virtual bool handleCraftingData(CraftingDataPacket &packet) { return false; }
    virtual bool handleCreatePhoto(CreatePhotoPacket &packet) { return false; }
    virtual bool handleCreativeContent(CreativeContentPacket &packet) { return false; }
    virtual bool handleCurrentStructureFeature(CurrentStructureFeaturePacket &packet) { return false; }
    virtual bool handleDeathInfo(DeathInfoPacket &packet) { return false; }
    virtual bool handleDebugInfo(DebugInfoPacket &packet) { return false; }
    virtual bool handleDimensionData(DimensionDataPacket &packet) { return false; }
    virtual bool handleDisconnect(DisconnectPacket &packet) { return false; }
    virtual bool handleEditorNetwork(EditorNetworkPacket &packet) { return false; }
    virtual bool handleEduUriResource(EduUriResourcePacket &packet) { return false; }
    virtual bool handleEducationSettings(EducationSettingsPacket &packet) { return false; }
    virtual bool handleEmoteList(EmoteListPacket &packet) { return false; }
    virtual bool handleEmote(EmotePacket &packet) { return false; }
    virtual bool handleFeatureRegistry(FeatureRegistryPacket &packet) { return false; }
    virtual bool handleGameRulesChanged(GameRulesChangedPacket &packet) { return false; }
    virtual bool handleGameTestRequest(GameTestRequestPacket &packet) { return false; }
    virtual bool handleGameTestResults(GameTestResultsPacket &packet) { return false; }
    virtual bool handleGraphicsOverrideParameter(GraphicsOverrideParameterPacket &packet) { return false; }
    virtual bool handleGuiDataPickItem(GuiDataPickItemPacket &packet) { return false; }
    virtual bool handleHurtArmor(HurtArmorPacket &packet) { return false; }
    virtual bool handleInteract(InteractPacket &packet) { return false; }
    virtual bool handleInventoryContent(InventoryContentPacket &packet) { return false; }
    virtual bool handleInventorySlot(InventorySlotPacket &packet) { return false; }
    virtual bool handleInventoryTransaction(InventoryTransactionPacket &packet) { return false; }
    virtual bool handleItemRegistry(ItemRegistryPacket &packet) { return false; }
    virtual bool handleItemStackRequest(ItemStackRequestPacket &packet) { return false; }
    virtual bool handleItemStackResponse(ItemStackResponsePacket &packet) { return false; }
    virtual bool handleJigsawStructureData(JigsawStructureDataPacket &packet) { return false; }
    virtual bool handleLabTable(LabTablePacket &packet) { return false; }
    virtual bool handleLecternUpdate(LecternUpdatePacket &packet) { return false; }
    virtual bool handleLegacyTelemetryEvent(LegacyTelemetryEventPacket &packet) { return false; }
    virtual bool handleLessonProgress(LessonProgressPacket &packet) { return false; }
    virtual bool handleLevelChunk(LevelChunkPacket &packet) { return false; }
    virtual bool handleLevelEventGeneric(LevelEventGenericPacket &packet) { return false; }
    virtual bool handleLevelEvent(LevelEventPacket &packet) { return false; }
    virtual bool handleLevelSoundEvent(LevelSoundEventPacket &packet) { return false; }
    virtual bool handleLocatorBar(LocatorBarPacket &packet) { return false; }
    virtual bool handleLogin(LoginPacket &packet) { return false; }
    virtual bool handleMapCreateLockedCopy(MapCreateLockedCopyPacket &packet) { return false; }
    virtual bool handleMapInfoRequest(MapInfoRequestPacket &packet) { return false; }
    virtual bool handleMobArmorEquipment(MobArmorEquipmentPacket &packet) { return false; }
    virtual bool handleMobEffect(MobEffectPacket &packet) { return false; }
    virtual bool handleMobEquipment(MobEquipmentPacket &packet) { return false; }
    virtual bool handleModalFormRequest(ModalFormRequestPacket &packet) { return false; }
    virtual bool handleModalFormResponse(ModalFormResponsePacket &packet) { return false; }
    virtual bool handleMotionPredictionHints(MotionPredictionHintsPacket &packet) { return false; }
    virtual bool handleMoveActorAbsolute(MoveActorAbsolutePacket &packet) { return false; }
    virtual bool handleMoveActorDelta(MoveActorDeltaPacket &packet) { return false; }
    virtual bool handleMovePlayer(MovePlayerPacket &packet) { return false; }
    virtual bool handleMovementEffect(MovementEffectPacket &packet) { return false; }
    virtual bool handleMultiplayerSettings(MultiplayerSettingsPacket &packet) { return false; }
    virtual bool handleNetworkChunkPublisherUpdate(NetworkChunkPublisherUpdatePacket &packet) { return false; }
    virtual bool handleNetworkSettings(NetworkSettingsPacket &packet) { return false; }
    virtual bool handleNetworkStackLatency(NetworkStackLatencyPacket &packet) { return false; }
    virtual bool handleNpcDialogue(NpcDialoguePacket &packet) { return false; }
    virtual bool handleNpcRequest(NpcRequestPacket &packet) { return false; }
    virtual bool handleOnScreenTextureAnimation(OnScreenTextureAnimationPacket &packet) { return false; }
    virtual bool handleOpenSign(OpenSignPacket &packet) { return false; }
    virtual bool handlePacketViolationWarning(PacketViolationWarningPacket &packet) { return false; }
    virtual bool handlePartyChanged(PartyChangedPacket &packet) { return false; }
    virtual bool handlePartyDestinationCookieResponse(PartyDestinationCookieResponsePacket &packet) { return false; }
    virtual bool handlePhotoTransfer(PhotoTransferPacket &packet) { return false; }
    virtual bool handlePlaySound(PlaySoundPacket &packet) { return false; }
    virtual bool handlePlayStatus(PlayStatusPacket &packet) { return false; }
    virtual bool handlePlayerAction(PlayerActionPacket &packet) { return false; }
    virtual bool handlePlayerArmorDamage(PlayerArmorDamagePacket &packet) { return false; }
    virtual bool handlePlayerAuthInput(PlayerAuthInputPacket &packet) { return false; }
    virtual bool handlePlayerEnchantOptions(PlayerEnchantOptionsPacket &packet) { return false; }
    virtual bool handlePlayerFog(PlayerFogPacket &packet) { return false; }
    virtual bool handlePlayerHotbar(PlayerHotbarPacket &packet) { return false; }
    virtual bool handlePlayerList(PlayerListPacket &packet) { return false; }
    virtual bool handlePlayerLocation(PlayerLocationPacket &packet) { return false; }
    virtual bool handlePlayerSkin(PlayerSkinPacket &packet) { return false; }
    virtual bool handlePlayerStartItemCooldown(PlayerStartItemCooldownPacket &packet) { return false; }
    virtual bool handlePlayerToggleCrafterSlotRequest(PlayerToggleCrafterSlotRequestPacket &packet) { return false; }
    virtual bool handlePlayerUpdateEntityOverrides(PlayerUpdateEntityOverridesPacket &packet) { return false; }
    virtual bool handlePlayerVideoCapture(PlayerVideoCapturePacket &packet) { return false; }
    virtual bool handlePositionTrackingDBClientRequest(PositionTrackingDBClientRequestPacket &packet) { return false; }
    virtual bool handlePositionTrackingDBServerBroadcast(PositionTrackingDBServerBroadcastPacket &packet) { return false; }
    virtual bool handlePrimitiveShapes(PrimitiveShapesPacket &packet) { return false; }
    virtual bool handlePurchaseReceipt(PurchaseReceiptPacket &packet) { return false; }
    virtual bool handleRefreshEntitlements(RefreshEntitlementsPacket &packet) { return false; }
    virtual bool handleRemoveActor(RemoveActorPacket &packet) { return false; }
    virtual bool handleRemoveObjective(RemoveObjectivePacket &packet) { return false; }
    virtual bool handleRemoveVolumeEntity(RemoveVolumeEntityPacket &packet) { return false; }
    virtual bool handleRequestAbility(RequestAbilityPacket &packet) { return false; }
    virtual bool handleRequestChunkRadius(RequestChunkRadiusPacket &packet) { return false; }
    virtual bool handleRequestNetworkSettings(RequestNetworkSettingsPacket &packet) { return false; }
    virtual bool handleRequestPermissions(RequestPermissionsPacket &packet) { return false; }
    virtual bool handleResourcePackChunkData(ResourcePackChunkDataPacket &packet) { return false; }
    virtual bool handleResourcePackChunkRequest(ResourcePackChunkRequestPacket &packet) { return false; }
    virtual bool handleResourcePackClientResponse(ResourcePackClientResponsePacket &packet) { return false; }
    virtual bool handleResourcePackDataInfo(ResourcePackDataInfoPacket &packet) { return false; }
    virtual bool handleResourcePackStack(ResourcePackStackPacket &packet) { return false; }
    virtual bool handleResourcePacksInfo(ResourcePacksInfoPacket &packet) { return false; }
    virtual bool handleResourcePacksReadyForValidation(ResourcePacksReadyForValidationPacket &packet) { return false; }
    virtual bool handleRespawn(RespawnPacket &packet) { return false; }
    virtual bool handleScriptMessage(ScriptMessagePacket &packet) { return false; }
    virtual bool handleSendPartyDestinationCookie(SendPartyDestinationCookiePacket &packet) { return false; }
    virtual bool handleServerPlayerPostMovePosition(ServerPlayerPostMovePositionPacket &packet) { return false; }
    virtual bool handleServerPresenceInfo(ServerPresenceInfoPacket &packet) { return false; }
    virtual bool handleServerSettingsRequest(ServerSettingsRequestPacket &packet) { return false; }
    virtual bool handleServerSettingsResponse(ServerSettingsResponsePacket &packet) { return false; }
    virtual bool handleServerStats(ServerStatsPacket &packet) { return false; }
    virtual bool handleServerStoreInfo(ServerStoreInfoPacket &packet) { return false; }
    virtual bool handleServerToClientHandshake(ServerToClientHandshakePacket &packet) { return false; }
    virtual bool handleServerboundDataDrivenScreenClosed(ServerboundDataDrivenScreenClosedPacket &packet) { return false; }
    virtual bool handleServerboundDataStore(ServerboundDataStorePacket &packet) { return false; }
    virtual bool handleServerboundDiagnostics(ServerboundDiagnosticsPacket &packet) { return false; }
    virtual bool handleServerboundLoadingScreen(ServerboundLoadingScreenPacket &packet) { return false; }
    virtual bool handleServerboundPackSettingChange(ServerboundPackSettingChangePacket &packet) { return false; }
    virtual bool handleSetActorData(SetActorDataPacket &packet) { return false; }
    virtual bool handleSetActorLink(SetActorLinkPacket &packet) { return false; }
    virtual bool handleSetActorMotion(SetActorMotionPacket &packet) { return false; }
    virtual bool handleSetCommandsEnabled(SetCommandsEnabledPacket &packet) { return false; }
    virtual bool handleSetDefaultGameType(SetDefaultGameTypePacket &packet) { return false; }
    virtual bool handleSetDifficulty(SetDifficultyPacket &packet) { return false; }
    virtual bool handleSetDisplayObjective(SetDisplayObjectivePacket &packet) { return false; }
    virtual bool handleSetHealth(SetHealthPacket &packet) { return false; }
    virtual bool handleSetHud(SetHudPacket &packet) { return false; }
    virtual bool handleSetLastHurtBy(SetLastHurtByPacket &packet) { return false; }
    virtual bool handleSetLocalPlayerAsInitialized(SetLocalPlayerAsInitializedPacket &packet) { return false; }
    virtual bool handleSetPlayerGameType(SetPlayerGameTypePacket &packet) { return false; }
    virtual bool handleSetPlayerInventoryOptions(SetPlayerInventoryOptionsPacket &packet) { return false; }
    virtual bool handleSetScore(SetScorePacket &packet) { return false; }
    virtual bool handleSetScoreboardIdentity(SetScoreboardIdentityPacket &packet) { return false; }
    virtual bool handleSetSpawnPosition(SetSpawnPositionPacket &packet) { return false; }
    virtual bool handleSetTime(SetTimePacket &packet) { return false; }
    virtual bool handleSetTitle(SetTitlePacket &packet) { return false; }
    virtual bool handleSettingsCommand(SettingsCommandPacket &packet) { return false; }
    virtual bool handleShowCredits(ShowCreditsPacket &packet) { return false; }
    virtual bool handleShowProfile(ShowProfilePacket &packet) { return false; }
    virtual bool handleShowStoreOffer(ShowStoreOfferPacket &packet) { return false; }
    virtual bool handleSimpleEvent(SimpleEventPacket &packet) { return false; }
    virtual bool handleSimulationType(SimulationTypePacket &packet) { return false; }
    virtual bool handleSpawnExperienceOrb(SpawnExperienceOrbPacket &packet) { return false; }
    virtual bool handleSpawnParticleEffect(SpawnParticleEffectPacket &packet) { return false; }
    virtual bool handleStartGame(StartGamePacket &packet) { return false; }
    virtual bool handleStopSound(StopSoundPacket &packet) { return false; }
    virtual bool handleStructureBlockUpdate(StructureBlockUpdatePacket &packet) { return false; }
    virtual bool handleStructureTemplateDataRequest(StructureTemplateDataRequestPacket &packet) { return false; }
    virtual bool handleStructureTemplateDataResponse(StructureTemplateDataResponsePacket &packet) { return false; }
    virtual bool handleSubChunk(SubChunkPacket &packet) { return false; }
    virtual bool handleSubChunkRequest(SubChunkRequestPacket &packet) { return false; }
    virtual bool handleSubClientLogin(SubClientLoginPacket &packet) { return false; }
    virtual bool handleSyncActorProperty(SyncActorPropertyPacket &packet) { return false; }
    virtual bool handleSyncWorldClocks(SyncWorldClocksPacket &packet) { return false; }
    virtual bool handleTakeItemActor(TakeItemActorPacket &packet) { return false; }
    virtual bool handleText(TextPacket &packet) { return false; }
    virtual bool handleTickingAreasLoadStatus(TickingAreasLoadStatusPacket &packet) { return false; }
    virtual bool handleToastRequest(ToastRequestPacket &packet) { return false; }
    virtual bool handleTransfer(TransferPacket &packet) { return false; }
    virtual bool handleTrimData(TrimDataPacket &packet) { return false; }
    virtual bool handleUnlockedRecipes(UnlockedRecipesPacket &packet) { return false; }
    virtual bool handleUpdateAbilities(UpdateAbilitiesPacket &packet) { return false; }
    virtual bool handleUpdateAdventureSettings(UpdateAdventureSettingsPacket &packet) { return false; }
    virtual bool handleUpdateAttributes(UpdateAttributesPacket &packet) { return false; }
    virtual bool handleUpdateBlock(UpdateBlockPacket &packet) { return false; }
    virtual bool handleUpdateBlockSynced(UpdateBlockSyncedPacket &packet) { return false; }
    virtual bool handleUpdateClientInputLocks(UpdateClientInputLocksPacket &packet) { return false; }
    virtual bool handleUpdateClientOptions(UpdateClientOptionsPacket &packet) { return false; }
    virtual bool handleUpdateEquip(UpdateEquipPacket &packet) { return false; }
    virtual bool handleUpdatePlayerGameType(UpdatePlayerGameTypePacket &packet) { return false; }
    virtual bool handleUpdateSoftEnum(UpdateSoftEnumPacket &packet) { return false; }
    virtual bool handleUpdateSubChunkBlocks(UpdateSubChunkBlocksPacket &packet) { return false; }
    virtual bool handleUpdateTrade(UpdateTradePacket &packet) { return false; }
    virtual bool handleVoxelShapes(VoxelShapesPacket &packet) { return false; }
};

}  // namespace bedrock_protocol
