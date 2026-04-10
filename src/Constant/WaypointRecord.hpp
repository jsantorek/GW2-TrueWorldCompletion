#pragma once
#include <Content/Record.hpp>

namespace TWC::ConstantWaypointRecord
{
// https://wiki.guildwars2.com/wiki/Kasha%27s%20Tomb%20Waypoint
constexpr auto KashasTombWaypoint = Record<ContentType::Waypoint>(942);

// https://wiki.guildwars2.com/wiki/The_Lovers%27_Crypt
constexpr auto CryptWaypoint = Record<ContentType::Waypoint>(951);

// https://wiki.guildwars2.com/wiki/Vault_Waypoint
constexpr auto VaultWaypoint = Record<ContentType::Waypoint>(1138);

// https://wiki.guildwars2.com/wiki/Mists_Arena_Spectator_Rally_Point
constexpr auto MistsArenaSpectatorRallyPoint = Record<ContentType::Waypoint>(3204);

// https://wiki.guildwars2.com/wiki/Toypocalypse#Player_Respawn
constexpr auto PlayerRespawn = Record<ContentType::Waypoint>(2849);

// https://wiki.guildwars2.com/wiki/First_Level#Tower_of_Nightmares_Waypoint
constexpr auto TowerOfNightmaresWaypointPublic = Record<ContentType::Waypoint>(1839);
constexpr auto TowerOfNightmaresWaypointPrivate = Record<ContentType::Waypoint>(3457);

// https://wiki.guildwars2.com/wiki/Moogooloo
constexpr auto MoogoolooWaypoint = Record<ContentType::Waypoint>(14);

// https://wiki.guildwars2.com/index.php?title=Greyhoof_Meadows&oldid=589817
constexpr auto MeadowsWaypoint = Record<ContentType::Waypoint>(5);

// https://wiki.guildwars2.com/wiki/Victory_or_Death
constexpr auto PactCampWaypoint = Record<ContentType::Waypoint>(859);
constexpr auto TheHumbleWaypoint = Record<ContentType::Waypoint>(1631);
constexpr auto TheGloryOfTyriasWaypoint = Record<ContentType::Waypoint>(1632);

// https://wiki.guildwars2.com/wiki/Molten_Furnace
constexpr auto CavernEntranceWaypoint = Record<ContentType::Waypoint>(1799);
constexpr auto OverlookWaypoint = Record<ContentType::Waypoint>(1800);
constexpr auto OreProcessingWaypoint = Record<ContentType::Waypoint>(1801);

// https://wiki.guildwars2.com/wiki/Molten_Furnace
constexpr auto RoughneckCrookWaypoint = Record<ContentType::Waypoint>(1806);
constexpr auto KleptotronicAdvancedDesignsWaypoint = Record<ContentType::Waypoint>(1807);
constexpr auto ScoundrelsHovelWaypoint = Record<ContentType::Waypoint>(1808);
} // namespace TWC::ConstantWaypointRecord
